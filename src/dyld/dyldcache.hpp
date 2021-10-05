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

#include "dyldmap.hpp"
#include "dyldimage.hpp"

#include "file.hpp"
#include "common.hpp"
#include "endianness.hpp"

#define DYLDCACHE_DIR "/var/db/dyld"
#define DYLDCACHE_NAME "dyld_shared_cache"

#define DYLDARCH_PPC     "ppc"
#define DYLDARCH_I386    "i386"
#define DYLDARCH_X86_64  "x86_64"
#define DYLDARCH_ARMV6   "armv6"
#define DYLDARCH_ARMV7   "armv7"

namespace dyldcache {

typedef enum {
	kArmType,
	kIntelType
} cpu_type_t;

typedef enum {
	kArmv6,
	kArmv7,
	kIntelx86,
	kIntelx86_64
} cpu_subtype_t;

typedef struct architecture_t {
	char* name;
	endian_t cpu_endian;
	cpu_type_t cpu_type;
	cpu_subtype_t cpu_subtype;
} architecture_t;

typedef struct header_t {
	char magic[16];
	uint32_t mapping_offset;
	uint32_t mapping_count;
	uint32_t images_offset;
	uint32_t images_count;
	uint64_t base_address;
	uint64_t codesign_offset;
	uint64_t codesign_size;
} header_t;

typedef struct dyldcache_t {
	header_t* header;
	architecture_t* arch;
	dyldimage_t** images;
	dyldmap_t** maps;
	file_t* file;
	uint32_t offset;
	uint32_t count;
	unsigned int size;
	unsigned char* data;
} dyldcache_t;

/*
 * Dyldcache Functions
 */
dyldcache_t* create();
dyldcache_t* open(const char* path);
dyldmap_t* map_image(dyldcache_t* cache, dyldimage_t* image);
dyldmap_t* map_address(dyldcache_t* cache, uint64_t address);
dyldimage_t* get_image(dyldcache_t* cache, const char* dylib);
dyldimage_t* first_image(dyldcache_t* cache);
dyldimage_t* next_image(dyldcache_t* cache, dyldimage_t* image);
void debug(dyldcache_t* cache);
void free(dyldcache_t* cache);

/*
 * Dyldcache Architecture Functions
 */
architecture_t* architecture_create();
architecture_t* architecture_load(dyldcache_t* cache);
void architecture_debug(architecture_t* arch);
void architecture_free(architecture_t* arch);

/*
 * Dyldcache Header Functions
 */
header_t* header_create();
header_t* header_load(dyldcache_t* cache);
void header_debug(header_t* header);
void header_free(header_t* header);

/*
 * Dyldcache Images Functions
 */
dyldimage_t** images_create(uint32_t count);
dyldimage_t** images_load(dyldcache_t* cache);
void images_debug(dyldimage_t** images);
void images_free(dyldimage_t** images);

/*
 * Dyldcache Maps Functions
 */
dyldmap_t** maps_create(uint32_t count);
dyldmap_t** maps_load(dyldcache_t* cache);
void maps_debug(dyldmap_t** maps);
void maps_free(dyldmap_t** maps);

} // namespace dyldcache