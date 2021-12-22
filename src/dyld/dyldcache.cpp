/**
 * GreenPois0n Absinthe - dyldcache.c
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

#include "dyldcache.hpp"

#include "common.hpp"
#include "debug.hpp"
#include "dyldimage.hpp"
#include "dyldmap.hpp"
#include "endianness.hpp"
#include "file.hpp"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

namespace absinthe {
namespace dyld {
namespace {

constexpr const char* DYLDCACHE_DIR{"/var/db/dyld"};
constexpr const char* DYLDCACHE_NAME{"dyld_shared_cache"};

constexpr const char* DYLDARCH_PPC{"ppc"};
constexpr const char* DYLDARCH_I386{"i386"};
constexpr const char* DYLDARCH_X86_64{"x86_64"};
constexpr const char* DYLDARCH_ARMV6{"armv6"};
constexpr const char* DYLDARCH_ARMV7{"armv7"};

} // namespace

Cache::Architecture::Architecture(unsigned char* data)
{
    unsigned char* found = NULL;
    found = strstr(data, DYLDARCH_ARMV6);
    if (found) {
        name = DYLDARCH_ARMV6;
        cpu_type = kArmType;
        cpu_subtype = kArmv6;
        cpu_endian = kLittleEndian;
        return;
    }

    found = strstr(data, DYLDARCH_ARMV7);
    if (found) {
        name = DYLDARCH_ARMV7;
        cpu_type = kArmType;
        cpu_subtype = kArmv7;
        cpu_endian = kLittleEndian;
        return;
    }

    // TODO: Add other architectures in here. We only need iPhone for now.

    if (!found) {
        throw std::runtime_error("Unknown architechure encountered!");
    }
}

/*
 * Dyldcache Header Functions
 */
Cache::Header::Header(unsigned char* data) { memcpy(this, data, sizeof(Header)); }

Cache::Cache(const char* path)
{
    int err = 0;
    uint32_t count = 0;
    uint32_t offset = 0;
    uint32_t length = 0;
    std::shared_ptr<Image> image = NULL;
    unsigned char* data = NULL;
    unsigned char* buffer = NULL;

    if (util::file_read(path, &buffer, &length) < 0) {
        throw std::runtime_error("Unable to open file at path");
    }

    _data = buffer;
    _size = length;

    _header = std::make_shared<Header>();
    _count = _header->images_count;
    _offset = _header->images_offset;

    _arch = std::make_shared<Architecture>(_data);
    _maps = maps_load();
    _images = images_load();

    // dyldcache_debug(cache);
}

std::vector<std::shared_ptr<Image>> Cache::images_load()
{
    uint32_t i = 0;
    uint32_t count = 0;
    uint32_t offset = 0;
    uint8_t* buffer = NULL;

    count = _header->images_count;

    for (i = 0; i < count; i++) {
        auto image = std::make_shared<Image>(_data, _header->images_offset + i * sizeof(Image));
        image->_map = map_address(_address);
        image->_offset = _address - _map->address;
        image->_data = &_data[_offset];
        image->_size = *(uint32_t*) (_data + 0x38);
        _images.push_back(image);
    }
}

void Cache::images_debug(std::shared_ptr<Image>* images)
{
    if (images) {
        debug("\tImages:");
        int i = 0;
        while (images[i]) {
            dyldimage_debug(images[i]);
            i++;
        }
        debug("");
    }
}

void Cache::images_free(std::shared_ptr<Image>* images)
{
    if (images) {
        // Loop through each image and free it
        int i = 0;
        while (images[i]) {
            dyldimage_free(images[i]);
            i++;
        }
        free(images);
        images = NULL;
    }
}

map::dyldmap_t** Cache::maps_create(uint32_t count)
{
    uint32_t size = (count + 1) * sizeof(map::dyldmap_t*);
    map::dyldmap_t** maps = (map::dyldmap_t**) malloc(size);
    if (maps) {
        memset(maps, '\0', size);
    }
    return maps;
}

map::dyldmap_t** Cache::maps_load()
{
    int i = 0;
    uint32_t count = 0;
    map::dyldmap_t** maps = NULL;
    if (cache) {
        count = _header->mapping_count;
        maps = maps_create(count);
        if (maps == NULL) {
            throw std::runtime_error("Unable to allocate memory for dyld maps");
            return NULL;
        }

        for (i = 0; i < count; i++) {
            maps[i] = dyldmap_parse(_data, _header->mapping_offset + i * sizeof(dyldmap_info_t));
            if (maps[i] == NULL) {
                throw std::runtime_error("Unable to parse dyld map from cache");
                return NULL;
            }
            _count++;
        }
        // dyldcache_maps_debug(maps);
    }
    return maps;
}

void Cache::maps_debug(map::dyldmap_t** maps)
{
    if (maps) {
        debug("\tMaps:");
        int i = 0;
        while (maps[i]) {
            dyldmap_info_debug(maps[i]->info);
            i++;
        }
        debug("");
    }
}

void Cache::maps_free(map::dyldmap_t** maps)
{
    if (maps) {
        int i = 0;
        // Loop through each map and free it
        while (maps[i]) {
            dyldmap_free(maps[i]);
            i++;
        }
        free(maps);
    }
}

map::dyldmap_t* Cache::map_image(std::shared_ptr<Image> image)
{
    return map_address(cache, image->address);
}

map::dyldmap_t* Cache::map_address(uint64_t address)
{
    int i = 0;
    map::dyldmap_t* map = NULL;
    for (i = 0; i < _header->mapping_count; i++) {
        map = _maps[i];
        if (dyldmap_contains(map, address)) {
            return map;
        }
    }
    return NULL;
}

std::shared_ptr<Image> Cache::get_image(const char* dylib)
{
    int i = 0;
    std::shared_ptr<Image> image = NULL;
    for (i = 0; i < _count; i++) {
        image = _images[i];
        if (image != NULL) {
            printf("Found %s", image->name);
            if (!strcmp(image->name, dylib)) {
                return image;
            }
        }
    }
    return NULL;
}

std::shared_ptr<Image> Cache::first_image() { return _images[0]; }

std::shared_ptr<Image> Cache::next_image(std::shared_ptr<Image> image)
{
    int i = 0;
    std::shared_ptr<Image> next = NULL;
    for (i = 0; i < _count; i++) {
        if (_images[i] == image) {
            next = _images[i + 1];
            break;
        }
    }
    return next;
}

} // namespace dyld
} // namespace absinthe
