/**
 * GreenPois0n Absinthe - dyldimage.h
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

#include "dyldmap.hpp"

#include <stdint.h>
#include <string>

namespace absinthe {
namespace dyld {

class Image final
{

    struct Info
    {
        Info(unsigned char* data, uint32_t offset);
        void _debug();

        uint64_t address;
        uint64_t modtime;
        uint64_t inode;
        uint32_t offset;
        uint32_t pad;
    };

    Image(unsigned char* data, uint32_t offset);
    ~Image();

    const std::string& get_name();
    void save(const std::string& path);
    void _debug();

  public:
    std::string _name;
    std::string _path;
    uint8_t* _data;
    uint32_t _size;
    uint32_t _index;
    uint32_t _offset;
    uint64_t _address;
    std::unique_ptr<Info> _info;
    std::unique_ptr<Map> map;
};

} // namespace dyld
} // namespace absinthe
