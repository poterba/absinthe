/**
 * GreenPois0n Absinthe - dyldmap.c
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
#include "dyldmap.hpp"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

namespace absinthe {
namespace dyld {

Map::Info::Info(unsigned char* data, uint32_t offset) { memcpy(this, &data[offset], sizeof(Info)); }

void Map::Info::_debug()
{
    debug("\t\tInfo {");
    debug("\t\t\t address = 0x%08x", (uint32_t) _address);
    debug("\t\t\t    size = 0x%08x", (uint32_t) _size);
    debug("\t\t\t  offset = 0x%08x", (uint32_t) _offset);
    debug("\t\t\t maxProt = %s", prot2str(_maxProt));
    debug("\t\t\tinitProt = %s", prot2str(_initProt));
    debug("\t\t}");
}

Map::Map(unsigned char* data, uint32_t offset)
{
    _info = std::make_unique<Info>(data, offset);
    _address = _info->address;
    _size = _info->size;
    _offset = _info->offset;
}

bool Map::contains(uint64_t address)
{
    return (address >= _address && address < (_address + _size));
}

} // namespace dyld
} // namespace absinthe
