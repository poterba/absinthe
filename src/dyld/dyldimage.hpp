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

namespace absinthe {
namespace dyld {

class Image final
{

    struct Info
    {
        /*
         * Dyld Image Info Functions
         */
        Info();
        Info(unsigned char* data, uint32_t offset);
        ~Info();
        void debug(info_t* info);

        uint64_t address;
        uint64_t modtime;
        uint64_t inode;
        uint32_t offset;
        uint32_t pad;
    };

    /*
     * Dyld Image Functions
     */
    dyldimage_t* parse(unsigned char* data, uint32_t offset);
    char* get_name(dyldimage_t* image);
    void save(dyldimage_t* image, const char* path);
    void free(dyldimage_t* image);
    void _debug(dyldimage_t* image);

  private:
    char* name;
    char* path;
    uint8_t* data;
    uint32_t size;
    uint32_t index;
    uint32_t offset;
    uint64_t address;
    map::dyldmap_t* map;
    image::info_t* info;
};

} // namespace dyld
} // namespace absinthe
