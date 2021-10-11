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

#include <string>
#include <vector>

namespace absinthe {
namespace backup {

class MBDB;

class MBDBRecord final
{
  public:
    struct Property final
    {
        unsigned short name_size;
        std::string name;
        unsigned short value_size;
        std::string value;
    } __attribute__((__packed__));

    struct Record final
    {
    } __attribute__((__packed__));

  public:
    MBDBRecord(unsigned char* data);

    void _debug();
    void free();

    void init();
    void set_domain(const char* domain);
    void set_path(const char* path);
    void set_target(const char* target);
    void set_datahash(const char* hash, unsigned short hash_size);
    void set_unknown1(const char* data, unsigned short size);
    void set_mode(unsigned short mode);
    void set_unknown2(unsigned int unknown2);
    void set_inode(unsigned int inode);
    void set_uid(unsigned int uid);
    void set_gid(unsigned int gid);
    void set_time1(unsigned int time1);
    void set_time2(unsigned int time2);
    void set_time3(unsigned int time3);
    void set_length(unsigned long long length);
    void set_flag(unsigned char flag);
    // TODO sth like add_property()

    int build(unsigned char** data, unsigned int* size);

  private:
    unsigned short domain_size;
    char* domain;
    unsigned short path_size;
    char* path;
    unsigned short target_size;
    char* target; // absolute path
    unsigned short datahash_size;
    char* datahash; // SHA1 hash
    unsigned short unknown1_size;
    char* unknown1;
    unsigned short mode; // Axxx = symlink, 4xxx = dir, 8xxx = file
    unsigned int unknown2;
    unsigned int inode;
    unsigned int uid;
    unsigned int gid;
    unsigned int time1;
    unsigned int time2;
    unsigned int time3;
    unsigned long long length;        // 0 if link or dir
    unsigned char flag;               // 0 if link or dir
    unsigned char property_count;     // number of properties
    std::vector<Property> properties; // properties
    unsigned int this_size;           // size of this record in bytes

} __attribute__((__packed__));

} // namespace backup
} // namespace absinthe
