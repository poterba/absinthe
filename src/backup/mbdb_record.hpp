/**
  * GreenPois0n Apparition - mbdb_record.h
  * Copyright (C) 2010 Chronic-Dev Team
  * Copyright (C) 2010 Joshua Hill
  * Copyright (C) 2012 Hanéne Samara
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

namespace absinthe {
namespace backup {
namespace mbdb {
struct mbdb_t;
} // namespace mbdb
namespace mbdb_record {

struct property_t {
	unsigned short name_size;
	char* name;
	unsigned short value_size;
	char* value;
} __attribute__((__packed__));

typedef struct property_t property_t;

struct mbdb_record_t {
    unsigned short domain_size;
    char* domain;
    unsigned short path_size;
    char* path;
    unsigned short target_size;
    char* target;	                  // absolute path
    unsigned short datahash_size;
    char* datahash;	                  // SHA1 hash
    unsigned short unknown1_size;
    char* unknown1;
    unsigned short mode;	          // Axxx = symlink, 4xxx = dir, 8xxx = file
    unsigned int unknown2;
    unsigned int inode;
    unsigned int uid;
    unsigned int gid;
    unsigned int time1;
    unsigned int time2;
    unsigned int time3;
    unsigned long long length;	      // 0 if link or dir
    unsigned char flag;	              // 0 if link or dir
    unsigned char property_count;     // number of properties
    property_t** properties; // properties
    unsigned int this_size; // size of this record in bytes
} __attribute__((__packed__));

typedef struct mbdb_record_t mbdb_record_t;

mbdb_record_t* create();
mbdb_record_t* parse(unsigned char* data);
void debug(mbdb_record_t* record);
void free(mbdb_record_t* record);

void init(mbdb_record_t* record);
void set_domain(mbdb_record_t* record, const char* domain);
void set_path(mbdb_record_t* record, const char* path);
void set_target(mbdb_record_t* record, const char* target);
void set_datahash(mbdb_record_t* record, const char* hash, unsigned short hash_size);
void set_unknown1(mbdb_record_t* record, const char* data, unsigned short size);
void set_mode(mbdb_record_t* record, unsigned short mode);
void set_unknown2(mbdb_record_t* record, unsigned int unknown2);
void set_inode(mbdb_record_t* record, unsigned int inode);
void set_uid(mbdb_record_t* record, unsigned int uid);
void set_gid(mbdb_record_t* record, unsigned int gid);
void set_time1(mbdb_record_t* record, unsigned int time1);
void set_time2(mbdb_record_t* record, unsigned int time2);
void set_time3(mbdb_record_t* record, unsigned int time3);
void set_length(mbdb_record_t* record, unsigned long long length);
void set_flag(mbdb_record_t* record, unsigned char flag);
// TODO sth like add_property()

int build(mbdb_record_t* record, unsigned char** data, unsigned int* size);

} // namespace mbdb_record
} // namespace backup
} // namespace absinthe
