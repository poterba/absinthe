/**
 * GreenPois0n Apparition - bpatch.c
 * Copyright (C) 2010 Chronic-Dev Team
 * Copyright (C) 2010 Joshua Hill
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 **/

#include "bpatch.hpp"

#include "common.hpp"
#include "debug.hpp"
#include "file.hpp"

#include <bzlib.h>

#include <cstring>

#define BUFSIZE 512000

namespace absinthe {
namespace util {

static off_t offtin(uint8_t* buf)
{
    off_t y;

    y = buf[7] & 0x7F;
    y = y * 256;
    y += buf[6];
    y = y * 256;
    y += buf[5];
    y = y * 256;
    y += buf[4];
    y = y * 256;
    y += buf[3];
    y = y * 256;
    y += buf[2];
    y = y * 256;
    y += buf[1];
    y = y * 256;
    y += buf[0];

    if (buf[7] & 0x80)
        y = -y;

    return y;
}

BinaryPatch::BinaryPatch(const std::string& path) : _path(path)
{
    auto file = std::make_shared<File>(path);
    unsigned int size = 0;
    uint8_t* data = NULL;
    file->read(&data, &size);
    if (data == NULL || size == 0) {
        throw std::runtime_error("Unable to open binary patch file");
    }

    load(data, size);
}

BinaryPatch::BinaryPatch(uint8_t* data, int64_t size)
{
    int err = 0;
    uint8_t* buf = NULL;
    int64_t offset = 0;
    int64_t buf_size = 0;
    int64_t data_size = 0;
    int64_t extra_size = 0;
    int64_t control_size = 0;

    bpatch_t* bpatch = bpatch_create();
    if (bpatch != NULL) {
        _header = bpatch_header_load(&data[offset], sizeof(bpatch_header_t));
        if (_header == NULL) {
            throw std::runtime_error("Unable to load binary patch header");
        }
        offset += sizeof(bpatch_header_t);

        buf = malloc(BUFSIZE + 1);
        if (buf == NULL) {
            throw std::runtime_error("Unable to allocate buffer");
        }

        //////////////////////////
        // Load in control block
        control_size = BUFSIZE;
        memset(buf, '\0', BUFSIZE);
        err = decompress(&data[offset], _header->ctrllen, buf, &control_size);
        if (err < 0 || control_size <= 0) {
            throw std::runtime_error("Unable to decompress control block");
        }

        // Make sure there's enough data left
        if (_header->ctrllen + offset > size) {
            throw std::runtime_error("Sanity check failed");
        }

        // Allocate memory for our decompressed control block
        _control_size = control_size;
        _control = (uint8_t*) malloc(control_size + 1);
        if (_control == NULL) {
            throw std::runtime_error("Unable to allocate control block");
        }
        memset(_control, '\0', control_size);
        memcpy(_control, buf, control_size);
        offset += _header->ctrllen;

        //////////////////////////
        // Load in diff block
        data_size = BUFSIZE;
        memset(buf, '\0', BUFSIZE);
        err = bpatch_decompress(&data[offset], _header->datalen, buf, &data_size);
        if (err < 0 || data_size <= 0) {
            throw std::runtime_error("Unable to decompress diff block");
        }

        // Allocate memory for our diff block
        _data_size = data_size;
        _data = malloc(data_size + 1);
        if (_data == NULL) {
            throw std::runtime_error("Unable to allocate memory for diff block");
        }
        memset(_data, '\0', data_size);
        memcpy(_data, buf, data_size);
        offset += _header->datalen;

        ////////////////////////
        // Load in extra block
        extra_size = BUFSIZE;
        memset(buf, '\0', BUFSIZE);
        err = bpatch_decompress(&data[offset], (size - offset), buf, &extra_size);
        if (err < 0 || extra_size <= 0) {
            throw std::runtime_error("Unable to decompress extra block");
        }

        _extra_size = extra_size;
        _extra = malloc(extra_size + 1);
        if (_extra == NULL) {
            throw std::runtime_error("Unable to allocate memory for extra block");
        }
        memset(_extra, '\0', extra_size + 1);
        memcpy(_extra, buf, extra_size);
        offset += extra_size;
    }

    return bpatch;
}

void BinaryPatch::debug()
{
    if (bpatch) {
        debug("Binary Patch");
        if (_header) {
            _header->debug();
        }
        if (_path != NULL) {
            debug("path = %s", _path);
        }
        if (_control_size > 0 && _control_size < BUFSIZE) {
            debug("Control:");
            debug("\tsize = %llu", _control_size);
            if (_control != NULL) {
                // debug("compressed: ");
                // hexdump(&_data[sizeof(bpatch_header_t)],
                // _header->ctrllen); debug("decompressed: ");
                // hexdump(_control, _control_size);
            }
        }
        if (_data_size > 0 && _data_size < BUFSIZE) {
            debug("Data:");
            debug("\tsize = %llu", _data_size);
            if (_data != NULL && _data_size <= 512) {
                // debug("compressed: ");
                // hexdump(&_data[sizeof(bpatch_header_t)],
                // _header->ctrllen); debug("decompressed: ");
                // hexdump(_data, _data_size);
            }
        }
        // hexdump(_data, _data_size);
    }
}

int BinaryPatch::apply(const char* path)
{
    int i = 0;
    int64_t x = 0;
    int64_t y = 0;
    int64_t z = 0;
    int64_t ctrl[3];

    int64_t ctrl_size = 0;
    int64_t source_size = 0;
    int64_t target_size = 0;

    int64_t ctrl_offset = 0;
    int64_t data_offset = 0;
    int64_t extra_offset = 0;
    int64_t source_offset = 0;
    int64_t target_offset = 0;

    int8_t* source_data = NULL;
    int8_t* target_data = NULL;
    int8_t* ctrl_data = NULL;
    int8_t* cur_data = NULL;

    ctrl_data = _control;
    ctrl_size = _control_size;
    target_size = _header->filelen;

    if (path != NULL) {
        file_read(path, &source_data, &source_size);
        if (source_data == NULL || source_size == 0) {
            throw std::runtime_error("Unable to read target file %s", path);
            return -1;
        }

        target_data = (uint8_t*) malloc(target_size + 1);
        if (target_data == NULL) {
            throw std::runtime_error("Unable to allocate data for new file");
            return -1;
        }
        memset(target_data, '\0', target_size + 1);

        while (ctrl_data + ctrl_offset < ctrl_data + ctrl_size) {
            // Loop 3 times to read in X, Y, and Z values from the control vector
            for (i = 0; i <= 2; i++) {
                ctrl[i] = offtin(&_control[ctrl_offset]);
                ctrl_offset += 8;
            }
            x = ctrl[0];
            y = ctrl[1];
            z = ctrl[2];
            debug("x = %qd, y = %qd, z = %qd", x, y, z);

            // if(source_offset + x > source_size) {
            //	throw std::runtime_error("Fail sanitary check");
            //	return -1;
            //}

            for (i = 0; i < x; i++) {
                uint8_t value1 = source_data[source_offset + i];
                uint8_t value2 = _data[data_offset + i];
                target_data[target_offset + i] = (value1 + value2) & 0xFF;
            }

            data_offset += x;
            target_offset += x;
            source_offset += x;

            // hexdump(&target_data[target_offset], 0x200);
            for (i = 0; i < y; i++) {
                uint8_t value1 = _extra[extra_offset + i];
                uint8_t value2 = _data[data_offset + i];
                target_data[target_offset + i] = value1;
            }
            // hexdump(&target_data[target_offset], 0x200);

            extra_offset += y;
            target_offset += y;
            source_offset += z;
        }

        // CleanUp
        file_write(path, target_data, target_size);
    }
    // if (target_data)
    //	free(target_data);
    // if (source_data)
    //	free(source_data);
    return 0;
}

/*
 * Binary Patch Header
 */
bpatch_header_t* bpatch_header_create()
{
    bpatch_header_t* header = (bpatch_header_t*) malloc(sizeof(bpatch_header_t));
    if (header) {
        memset(header, '\0', sizeof(bpatch_header_t));
    }
    return header;
}

bpatch_header_t* bpatch_header_load(uint8_t* data, int64_t size)
{
    bpatch_header_t* header = bpatch_header_create();
    if (header != NULL) {
        if (memcmp(data, "BSDIFF40", 8) != 0) {
            throw std::runtime_error("Unable to find magic string in binary patch header");
            bpatch_header_free(header);
            return NULL;
        }
        memcpy(header->magic, data, 8);

        header->ctrllen = offtin(data + 8);
        header->datalen = offtin(data + 16);
        header->filelen = offtin(data + 24);

        if ((header->ctrllen < 0) || (header->datalen < 0) || (header->filelen < 0)) {
            throw std::runtime_error("Unable to read lengths in from binary patch header");
            bpatch_header_free(header);
            return NULL;
        }
    }
    return header;
}

void bpatch_header_free(bpatch_header_t* header)
{
    if (header) {
        free(header);
    }
}

void bpatch_header_debug(bpatch_header_t* header)
{
    debug("Header:");
    debug(
        "\tmagic = %c%c%c%c%c%c%c%c", header->magic[0], header->magic[1], header->magic[2],
        header->magic[3], header->magic[4], header->magic[5], header->magic[6], header->magic[7]);
    debug("\tctrllen = %llu (compressed)", header->ctrllen);
    debug("\tdatalen = %llu (compressed)", header->datalen);
    debug("\tfilelen = %llu (uncompressed)", header->filelen);
    debug("");
}

unsigned int
bpatch_decompress(char* input, unsigned int in_size, char* output, unsigned int* out_size)
{
    int err = 0;
    char* dest = input;
    char* source = output;
    unsigned int was = *out_size;
    unsigned int size = in_size;
    unsigned int got = BUFSIZE;

    err = BZ2_bzBuffToBuffDecompress(output, out_size, input, in_size, 0, 0);
    got = *out_size;
    if (err != BZ_OK) {
        debug("Unable to decompress buffer %d", err);
        got = 0;
    }

    if (got != 0) {
        if (got < was) {
            // debug("bingo!!");
        } else {
            throw std::runtime_error("Uh Oh, I think we need a bigger buffer");
            got = 0;
        }
    }
    *out_size = got;
    return got;
}

} // namespace util
} // namespace absinthe
