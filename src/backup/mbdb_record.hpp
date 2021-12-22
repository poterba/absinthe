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
    MBDBRecord(unsigned char* data = nullptr);

    void _debug();
    void free();

    void init();
    void set_domain(const std::string& domain);
    void set_path(const std::string& path);
    void set_target(const std::string& target);
    void set_datahash(const std::string& hash, unsigned short hash_size);
    void set_unknown1(const std::string& data, unsigned short size);
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
    unsigned short _domain_size;
    std::string _domain;
    unsigned short _path_size;
    std::string _path;
    unsigned short _target_size;
    std::string _target; // absolute path
    unsigned short _datahash_size;
    std::string _datahash; // SHA1 hash
    unsigned short _unknown1_size;
    std::string _unknown1;
    unsigned short _mode; // Axxx = symlink, 4xxx = dir, 8xxx = file
    unsigned int _unknown2;
    unsigned int _inode;
    unsigned int _uid;
    unsigned int _gid;
    unsigned int _time1;
    unsigned int _time2;
    unsigned int _time3;
    unsigned long long _length;        // 0 if link or dir
    unsigned char _flag;               // 0 if link or dir
    unsigned char _property_count;     // number of properties
    std::vector<Property> _properties; // properties
    unsigned int _this_size;           // size of this record in bytes

} __attribute__((__packed__));

} // namespace backup
} // namespace absinthe
