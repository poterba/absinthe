/**
 * GreenPois0n Absinthe - file.c
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

#include "file.hpp"

#define BUFSIZE 4096

namespace absinthe {
namespace util {

File::File(const std::string& path)
{
    uint64_t got = 0;
    uint8_t buffer[4096];

    _desc = fopen(path.c_str(), "rb");
    if (_desc == NULL) {
        throw std::runtime_error("Unable to open file");
    }

    _path = path;
    if (_path.empty()) {
        throw std::runtime_error("Unable to allocate memory for file path");
    }

    fseek(_desc, 0, SEEK_END);
    _size = ftell(_desc);
    fseek(_desc, 0, SEEK_SET);

    _offset = 0;
    // if (!_data.empty()) {
    //     throw std::runtime_error("Unable to allocate memory for file");
    // }

    uint64_t offset = 0;
    while (offset < _size) {
        memset(buffer, '\0', BUFSIZE);
        got = fread(buffer, 1, BUFSIZE, _desc);
        if (got > 0) {
            offset += got;
            memcpy(&(_data[offset]), buffer, got);
        } else {
            break;
        }
    }
    // fprintf(stderr, "Read in %llu of %llu bytes from %s", _offset, _size, _path);
    // We have the data stored in memory now, so we don't need to keep this open anymore
    // fseek(_desc, 0, SEEK_SET);
    close();
    _offset = 0;
}

File::~File() { close(); }

void File::close()
{
    if (_desc) {
        fclose(_desc);
        _desc = NULL;
    }
}

int File::read(std::string& buf, unsigned int* length)
{
    FILE* fd = NULL;
    fd = fopen(_path.c_str(), "rb");
    if (fd == NULL) {
        return -1;
    }

    fseek(fd, 0, SEEK_END);
    long size = ftell(fd);
    fseek(fd, 0, SEEK_SET);

    unsigned char* data = (unsigned char*) malloc(size);

    int bytes = fread(data, 1, size, fd);
    if (bytes != size) {
        fclose(fd);
        return -1;
    }
    fclose(fd);

    buf = {data, data + sizeof data / sizeof data[0]};
    *length = bytes;
    return bytes;
}

int File::write(unsigned char* buf, unsigned int length)
{
    FILE* fd = NULL;
    fd = fopen(_path.c_str(), "wb");
    if (fd == NULL) {
        return -1;
    }

    int bytes = fwrite(buf, 1, length, fd);
    if (bytes != length) {
        fclose(fd);
        return -1;
    }
    fclose(fd);
    return bytes;
}

int File::copy(const std::string& to)
{
    FILE* ffr = NULL;
    FILE* fto = NULL;
    char buf[8192];
    size_t size;

    ffr = fopen(_path.c_str(), "rb");
    if (ffr == NULL) {
        throw std::runtime_error("could not open source file for reading");
    }
    fto = fopen(to.c_str(), "wb");
    if (fto == NULL) {
        fclose(ffr);
        throw std::runtime_error("could not open target file for writing");
    }

    while (!feof(ffr)) {
        size = fread(buf, 1, sizeof(buf), ffr);
        if (size > 0) {
            size_t bytes = fwrite(buf, 1, size, fto);
            if (bytes != size) {
                fclose(fto);
                fclose(ffr);
                return -1;
            }
        }
    }
    fclose(fto);
    fclose(ffr);

    return 0;
}

} // namespace util
} // namespace absinthe
