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

#include <memory>

namespace absinthe {
namespace dyld {

/*
 * Dyldcache Map
 */
class Map final
{
  public:
    struct Info
    {
        Info(unsigned char* data, uint32_t offset);
        void _debug();

        uint64_t address;
        uint64_t size;
        uint64_t offset;
        uint32_t maxProt;
        uint32_t initProt;
    };

    Map(unsigned char* data, uint32_t offset);
    ~Map();

    bool contains(uint64_t address);

  private:
    uint64_t _address;
    uint64_t _size;
    uint64_t _offset;
    std::unique_ptr<Info> _info;
};

} // namespace dyld
} // namespace absinthe
