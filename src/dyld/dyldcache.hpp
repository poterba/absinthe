/**
 * GreenPois0n Absinthe - dyldcache.h
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
#include "dyldimage.hpp"
#include "dyldmap.hpp"
#include "endianness.hpp"
#include "file.hpp"
#include <vector>

namespace absinthe {
namespace dyld {

class Cache final
{
  public:
    enum CPUType
    {
        kArmType,
        kIntelType
    };

    enum CPUSubType
    {
        kArmv6,
        kArmv7,
        kIntelx86,
        kIntelx86_64
    };

    struct Architecture
    {
        Architecture(const std::string& data);

        std::string name;
        endian_t cpu_endian;
        CPUType cpu_type;
        CPUSubType cpu_subtype;
    };

    struct Header
    {
        Header(unsigned char* data);

        char magic[16];
        uint32_t mapping_offset;
        uint32_t mapping_count;
        uint32_t images_offset;
        uint32_t images_count;
        uint64_t base_address;
        uint64_t codesign_offset;
        uint64_t codesign_size;
    };

    Cache(const char* path);
    ~Cache() = default;

    std::shared_ptr<Map> map_image(const std::shared_ptr<Image>& image);
    std::shared_ptr<Map> map_address(uint64_t address);
    std::shared_ptr<Image> get_image(const char* dylib);
    std::shared_ptr<Image> first_image();
    std::shared_ptr<Image> next_image(const std::shared_ptr<Image>& image);

    std::vector<std::shared_ptr<Image>> images_create(uint32_t count);
    void images_debug(std::vector<std::shared_ptr<Image>> images);
    void images_free(std::vector<std::shared_ptr<Image>> images);

    std::vector<std::shared_ptr<Map>> maps_create(uint32_t count);
    std::vector<std::shared_ptr<Map>> maps_load();

  private:
    std::shared_ptr<Header> _header;
    std::shared_ptr<Architecture> _arch;
    std::vector<std::shared_ptr<Image>> _images;
    std::vector<std::shared_ptr<Map>> _maps;
    util::file_t* _file;
    uint32_t _offset;
    uint32_t _count;
    unsigned int _size;
    unsigned char* _data;
};

} // namespace dyld
} // namespace absinthe
