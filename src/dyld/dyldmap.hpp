/**
 * GreenPois0n Absinthe - dyldmap.h
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

#include "common.hpp"

#define DYLDMAP_EXEC 1
#define DYLDMAP_WRITE 2
#define DYLDMAP_READ 4

namespace absinthe {
namespace dyld {
namespace map {

typedef struct info_t
{
    uint64_t address;
    uint64_t size;
    uint64_t offset;
    uint32_t maxProt;
    uint32_t initProt;
} info_t;

typedef struct dyldmap_t
{
    uint64_t address;
    uint64_t size;
    uint64_t offset;
    info_t* info;
} dyldmap_t;

/*
 * Dyldcache Map Functions
 */
dyldmap_t* parse(unsigned char* data, uint32_t offset);
bool contains(dyldmap_t* map, uint64_t address);
void _debug(dyldmap_t* image);
void free(dyldmap_t* map);

/*
 * Dyldcache Map Info Functions
 */
info_t* info_create();
info_t* info_parse(unsigned char* data, uint32_t offset);
void info_debug(info_t* map);
void info_free(info_t* map);

} // namespace map
} // namespace dyld
} // namespace absinthe
