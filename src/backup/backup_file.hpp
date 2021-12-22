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

class MBDBRecord;

struct File
{
  public:
    File(const std::string& filepath);
    File(const std::vector<unsigned char>& data, int copy);
    File(std::shared_ptr<MBDBRecord> mbdb_record);

    void assign_file_data(const std::vector<unsigned char>& data, int copy);
    void assign_file_path(const std::string& path);

    void set_domain(const std::string& domain);
    void set_path(const std::string& path);
    void set_target(const std::string& target);
    void update_hash();
    void disable_hash();
    // void set_unknown1(const char* data, unsigned short size);
    void set_mode(unsigned short mode);
    // void set_unknown2(unsigned int unknown2);
    void set_inode(unsigned int inode);
    void set_uid(unsigned int uid);
    void set_gid(unsigned int gid);
    void set_time1(unsigned int time1);
    void set_time2(unsigned int time2);
    void set_time3(unsigned int time3);
    void set_length(unsigned long long length);
    void set_flag(unsigned char flag);

    int get_record_data(unsigned char** data, unsigned int* size);

  private:
    // mbdx_record_t* mbdx_record;
    std::shared_ptr<MBDBRecord> _mbdb_record;
    std::string _filepath;

    std::vector<unsigned char> _data;
    unsigned int _size;
    int _free_data;
};

} // namespace backup
} // namespace absinthe
