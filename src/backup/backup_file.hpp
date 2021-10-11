/**
 * GreenPois0n Apparition - backup_file.h
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

#include "mbdb_record.hpp"

namespace absinthe {
namespace backup {
namespace backup_file {

typedef struct backup_file_t
{
    // mbdx_record_t* mbdx_record;
    mbdb_record::mbdb_record_t* mbdb_record;
    char* filepath;
    unsigned char* data;
    unsigned int size;
    int free_data;
} backup_file_t;

backup_file_t* create(const char* filepath);
backup_file_t* create_with_data(unsigned char* data, unsigned int size, int copy);
backup_file_t* create_from_record(mbdb_record::mbdb_record_t* record);

void assign_file_data(backup_file_t* bfile, unsigned char* data, unsigned int size, int copy);
void assign_file_path(backup_file_t* bfile, unsigned char* path);

void set_domain(backup_file_t* bfile, const char* domain);
void set_path(backup_file_t* bfile, const char* path);
void set_target(backup_file_t* bfile, const char* target);
void update_hash(backup_file_t* bfile);
void disable_hash(backup_file_t* bfile);
// void set_unknown1(backup_file_t* bfile, const char* data, unsigned short size);
void set_mode(backup_file_t* bfile, unsigned short mode);
// void set_unknown2(backup_file_t* bfile, unsigned int unknown2);
void set_inode(backup_file_t* bfile, unsigned int inode);
void set_uid(backup_file_t* bfile, unsigned int uid);
void set_gid(backup_file_t* bfile, unsigned int gid);
void set_time1(backup_file_t* bfile, unsigned int time1);
void set_time2(backup_file_t* bfile, unsigned int time2);
void set_time3(backup_file_t* bfile, unsigned int time3);
void set_length(backup_file_t* bfile, unsigned long long length);
void set_flag(backup_file_t* bfile, unsigned char flag);

void free(backup_file_t* file);

int get_record_data(backup_file_t* bfile, unsigned char** data, unsigned int* size);

} // namespace backup_file
} // namespace backup
} // namespace absinthe
