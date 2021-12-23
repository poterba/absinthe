/**
 * GreenPois0n Apparition - bpatch.h
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

#pragma once

#include <bzlib.h>

#include <string>
#include <vector>

/*
    File format:
            0	8	"BSDIFF40" (bzip2) or "BSDIFN40" (raw)
            8	8	X
            16	8	Y
            24	8	sizeof(newfile)
            32	X	bzip2(control block)
            32+X	Y	bzip2(diff block)
            32+X+Y	???	bzip2(extra block)
    with control block a set of triples (x,y,z) meaning "add x bytes
    from oldfile to x bytes from the diff block; copy y bytes from the
    extra block; seek forwards in oldfile by z bytes".
*/

namespace absinthe {
namespace util {

class BinaryPatchHeader
{
  public:
    BinaryPatchHeader();
    BinaryPatchHeader(uint8_t* data, int64_t size);
    ~BinaryPatchHeader();
    void debug();

  private:
    uint8_t _magic[8];
    int64_t _ctrllen;
    int64_t _datalen;
    int64_t _filelen;
};

class BinaryPatch
{
  public:
    BinaryPatch() = default;
    BinaryPatch(const std::string& path);
    BinaryPatch(uint8_t* data, int64_t size);
    ~BinaryPatch() = default;

    int apply(const char* path);
    void debug();

  private:
    std::string _path;
    std::vector<uint8_t> _data;
    std::vector<uint8_t> _extra;
    std::vector<uint8_t> _control;
    int64_t _data_size;
    int64_t _extra_size;
    int64_t _control_size;
    int64_t _data_offset;
    int64_t _extra_offset;
    int64_t _control_offset;
    std::shared_ptr<BinaryPatchHeader> _header;
};

int bpatch(const char* in, const char* out, const char* patch);
unsigned int
bpatch_decompress(char* input, unsigned int in_size, char* output, unsigned int* out_size);

} // namespace util
} // namespace absinthe
