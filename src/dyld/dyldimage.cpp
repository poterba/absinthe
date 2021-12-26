/**
 * GreenPois0n Absinthe - dyldimage.c
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

#include "common.hpp"
#include "debug.hpp"
#include "dyldimage.hpp"
#include "dyldmap.hpp"
#include "file.hpp"

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

namespace absinthe {
namespace dyld {

Image::Info::Info(unsigned char* data, uint32_t offset)
{
    memcpy(this, &data[offset], sizeof(Info));
}

void Image::Info::_debug()
{
    debug("\t\t\taddress = 0x%qx", address);
    debug("\t\t\t  inode = 0x%qx", inode);
    debug("\t\t\tmodtime = 0x%qx", modtime);
    debug("\t\t\t offset = 0x%08x", offset);
    debug("\t\t\t    pad = 0x%08x", pad);
}

Image::Image(unsigned char* data, uint32_t offset)
{
    unsigned char* buffer = &data[offset];

    _info = std::make_unique<Info>(data, offset);
    _path = data[_offset];
    _name = _path.substr(_path.rfind('/'));
    // _address = _address;
    _size = 0;
}

Image::~Image()
{
    if (_info) {
        _info = NULL;
    }
}

void Image::_debug()
{
    debug("\t\tImage {");
    if (_info) {
        _debug();
    }
    debug("\t\t}");
}

void Image::save(const std::string& path)
{
    if (_data != NULL && _size > 0) {
        std::cout << "Writing dylib to " << path << std::endl;
        auto file = std::make_shared<util::File>(path);
        file->write(_data, _size);
    }
}

const std::string& Image::get_name() { return _name; }

} // namespace dyld
} // namespace absinthe
