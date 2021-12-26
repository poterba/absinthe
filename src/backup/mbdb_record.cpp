/**
 * GreenPois0n Apparition - mbdb_record.c
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
#include "mbdb_record.hpp"

#include "debug.hpp"
#include "endianness.hpp"
#include "mbdb.hpp"

#include <cstring>
#include <iostream>
#include <stdexcept>
// #include <stdio.h>
// #include <stdlib.h>
// #include <string.h>

namespace absinthe {
namespace backup {

MBDBRecord::MBDBRecord(unsigned char* data)
{
    unsigned int offset = 0;

    // Parse Domain
    unsigned short strsize = be16toh(*((unsigned short*) &data[offset]));
    if (strsize > 0 && strsize < 0xFFFF) {
        offset += 2;
        _domain = {&data[offset], &data[offset] + strsize};
        _domain[strsize] = 0;
        offset += strsize;
    } else {
        _domain = {};
        offset += 2;
    }
    _domain_size = strsize;

    // Parse Path
    strsize = be16toh(*((unsigned short*) &data[offset]));
    if (strsize > 0 && strsize < 0xFFFF) {
        if (_path.empty()) {
            throw std::runtime_error("Allocation Error!");
        }
        offset += 2;
        _path = std::string(&data[offset], strsize);
        offset += strsize;
    } else {
        _path = {};
        offset += 2;
    }
    _path_size = strsize;

    // Parse Target
    strsize = be16toh(*((unsigned short*) &data[offset]));
    if (strsize > 0 && strsize < 0xFFFF) {
        offset += 2;
        _target = std::string(&data[offset], strsize);
        _target[strsize] = 0;
        offset += strsize;
    } else {
        _target = {};
        offset += 2;
    }
    _target_size = strsize;

    // parse DataHash
    strsize = be16toh(*((unsigned short*) &data[offset]));
    if (strsize > 0 && strsize < 0xFFFF) {
        offset += 2;
        _datahash = std::string(&data[offset], strsize);
        offset += strsize;
    } else {
        _datahash = {};
        offset += 2;
    }
    _datahash_size = strsize;

    // parse unknown1
    strsize = be16toh(*((unsigned short*) &data[offset]));
    if (strsize > 0 && strsize < 0xFFFF) {
        offset += 2;
        _unknown1 = std::string(&data[offset], strsize);
        _unknown1[strsize] = 0;
        offset += strsize;
    } else {
        _unknown1 = {};
        offset += 2;
    }
    _unknown1_size = strsize;

    _mode = be16toh(*((unsigned short*) &data[offset]));
    offset += 2;

    _unknown2 = be32toh(*((unsigned int*) &data[offset]));
    offset += 4;

    _inode = be32toh(*((unsigned int*) &data[offset]));
    offset += 4;

    _uid = be32toh(*((unsigned int*) &data[offset]));
    offset += 4;

    _gid = be32toh(*((unsigned int*) &data[offset]));
    offset += 4;

    _time1 = be32toh(*((unsigned int*) &data[offset]));
    offset += 4;

    _time2 = be32toh(*((unsigned int*) &data[offset]));
    offset += 4;

    _time3 = be32toh(*((unsigned int*) &data[offset]));
    offset += 4;

    _length = be64toh(*((unsigned long long*) &data[offset]));
    offset += 8;

    _flag = *((unsigned char*) &data[offset]);
    offset += 1;

    _property_count = *((unsigned char*) &data[offset]);
    offset += 1;

    for (int i = 0; i < _property_count; i++) {
        Property prop; // = malloc(sizeof(property_t));
        prop.name_size = be16toh(*((unsigned short*) &data[offset]));
        prop.name = (char*) malloc(prop.name_size + 1);
        offset += 2;
        memcpy(prop.name.data(), &data[offset], prop.name_size);
        prop.name[prop.name_size] = 0;
        offset += prop.name_size;

        prop.value_size = be16toh(*((unsigned short*) &data[offset]));
        prop.value = (char*) malloc(prop.value_size + 1);
        offset += 2;
        memcpy(prop.value.data(), &data[offset], prop.value_size);
        prop.value[prop.value_size] = 0;
        offset += prop.value_size;

        _properties.push_back(prop);
    }
    _this_size = offset;

    // _debug();
}

/*
 struct t {
 char* domain;
 char* path;
 char* target;	                  // absolute path
 char* datahash;	                  // SHA1 hash
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
 unsigned char properties;	      // number of properties
 } __attribute__((__packed__));
 */

void MBDBRecord::_debug()
{
    std::cout << "mbdb record" << std::endl;
    std::cout << "\tdomain = " << _domain << std::endl;
    std::cout << "\tpath = " << _path << std::endl;
    std::cout << "\ttarget = " << _target << std::endl;
    std::cout << "\tdatahash = " << _datahash << std::endl;
    std::cout << "\tunknown1 = " << _unknown1 << std::endl;
    std::cout << "\tmode = " << std::hex << _mode << std::dec << std::endl;
    std::cout << "\tunknown2 = 0x" << _unknown2 << std::endl;
    std::cout << "\tinode = 0x" << _inode << std::endl;
    std::cout << "\tuid = " << _uid << std::endl;
    std::cout << "\tgid = " << _gid << std::endl;
    std::cout << "\ttime1 = 0x" << _time1 << std::endl;
    std::cout << "\ttime2 = 0x" << _time2 << std::endl;
    std::cout << "\ttime3 = 0x" << _time3 << std::endl;
    std::cout << "\tlength = " << _length << std::endl;
    std::cout << "\tflag = 0x" << _flag << std::endl;
    std::cout << "\tproperty_count = " << _property_count << std::endl;
}
void MBDBRecord::init()
{
    _target_size = 0xFFFF;
    _datahash_size = 0xFFFF;
    _unknown1_size = 0xFFFF;
    _this_size = 2 + 2 + 2 + 2 + 2 + 2 + 4 + 4 + 4 + 4 + 4 + 4 + 4 + 8 + 1 + 1;
}

void MBDBRecord::set_domain(const std::string& domain)
{
    unsigned short old_size = _domain_size;
    if (_domain_size > 0 && _domain_size < 0xFFFF) {
        _this_size -= _domain_size;
    }
    if (!domain.empty()) {
        _domain_size = domain.size();
        _domain = domain;
        _this_size += _domain_size;
    } else {
        _domain_size = 0;
    }
}

void MBDBRecord::set_path(const std::string& path)
{
    unsigned short old_size = _path_size;
    _path = {};
    if (_path_size > 0 && _path_size < 0xFFFF) {
        _this_size -= _path_size;
    }
    if (!_path.empty()) {
        _path_size = path.size();
        _path = path;
        _this_size += _path_size;
    } else {
        _path_size = 0;
    }
}

void MBDBRecord::set_target(const std::string& target)
{
    unsigned short old_size = _target_size;
    _target = {};
    if (_target_size > 0 && _target_size < 0xFFFF) {
        _this_size -= _target_size;
    }
    if (!_target.empty()) {
        _target_size = target.size();
        _target = target;
        _this_size += _target_size;
    } else {
        _target_size = 0xFFFF;
    }
}

void MBDBRecord::set_datahash(const std::string& hash)
{
    unsigned short old_size = _datahash_size;
    _datahash = {};
    if (_datahash_size > 0 && _datahash_size < 0xFFFF) {
        _this_size -= _datahash_size;
    }
    if (!hash.empty()) {
        _datahash_size = hash.size();
        _datahash = hash;
        _this_size += _datahash_size;
    } else {
        _datahash_size = 0xFFFF;
    }
}

void MBDBRecord::set_unknown1(const std::string& data)
{
    unsigned short old_size = _unknown1_size;
    _unknown1 = {};
    if (_unknown1_size > 0 && _unknown1_size < 0xFFFF) {
        _this_size -= _unknown1_size;
    }
    if (!data.empty()) {
        _unknown1_size = data.size();
        _unknown1 = data;
        _this_size += _unknown1_size;
    } else {
        _unknown1_size = 0xFFFF;
    }
}

void MBDBRecord::set_mode(unsigned short mode) { mode = mode; }

void MBDBRecord::set_unknown2(unsigned int unknown2) { unknown2 = unknown2; }

void MBDBRecord::set_inode(unsigned int inode) { inode = inode; }

void MBDBRecord::set_uid(unsigned int uid) { uid = uid; }

void MBDBRecord::set_gid(unsigned int gid) { gid = gid; }

void MBDBRecord::set_time1(unsigned int time1) { time1 = time1; }

void MBDBRecord::set_time2(unsigned int time2) { time2 = time2; }

void MBDBRecord::set_time3(unsigned int time3) { time3 = time3; }

void MBDBRecord::set_length(unsigned long long length) { length = length; }

void MBDBRecord::set_flag(unsigned char flag) { flag = flag; }

int MBDBRecord::build(unsigned char** data, unsigned int* size)
{
    unsigned int offset = 0;
    unsigned char* data_buf = NULL;

    data_buf = (unsigned char*) malloc(_this_size);
    if (!data_buf) {
        throw std::runtime_error("Allocation Error!");
        return -1;
    }

    unsigned short strsize;

    // append Domain
    strsize = htobe16(_domain_size);
    memcpy(&data_buf[offset], &strsize, 2);
    offset += 2;
    if (!_domain.empty()) {
        memcpy(&data_buf[offset], _domain, _domain.size());
        offset += _domain_size;
    }

    // append Path
    strsize = htobe16(_path_size);
    memcpy(&data_buf[offset], &strsize, 2);
    offset += 2;
    if (!_path.empty()) {
        memcpy(&data_buf[offset], _path, _path.size());
        offset += _path_size;
    }

    // append Target
    strsize = htobe16(_target_size);
    memcpy(&data_buf[offset], &strsize, 2);
    offset += 2;
    if (!_target.empty()) {
        memcpy(&data_buf[offset], _target, _target.size());
        offset += _target_size;
    }

    // append DataHash
    strsize = htobe16(_datahash_size);
    memcpy(&data_buf[offset], &strsize, 2);
    offset += 2;
    if (!_datahash.empty()) {
        memcpy(&data_buf[offset], _datahash, _datahash.size());
        offset += _datahash_size;
    }

    // append unknown1
    strsize = htobe16(_unknown1_size);
    memcpy(&data_buf[offset], &strsize, 2);
    offset += 2;
    if (!_unknown1.empty()) {
        memcpy(&data_buf[offset], _unknown1, _unknown1.size());
        offset += _unknown1_size;
    }

    unsigned short mode = htobe16(mode);
    memcpy(&data_buf[offset], &mode, 2);
    offset += 2;

    int unknown2 = htobe32(unknown2);
    memcpy(&data_buf[offset], &unknown2, 4);
    offset += 4;

    int inode = htobe32(inode);
    memcpy(&data_buf[offset], &inode, 4);
    offset += 4;

    int uid = htobe32(uid);
    memcpy(&data_buf[offset], &uid, 4);
    offset += 4;

    int gid = htobe32(gid);
    memcpy(&data_buf[offset], &gid, 4);
    offset += 4;

    int time1 = htobe32(time1);
    memcpy(&data_buf[offset], &time1, 4);
    offset += 4;

    int time2 = htobe32(time2);
    memcpy(&data_buf[offset], &time2, 4);
    offset += 4;

    int time3 = htobe32(time3);
    memcpy(&data_buf[offset], &time3, 4);
    offset += 4;

    unsigned long long length = htobe64(length);
    memcpy(&data_buf[offset], &length, 8);
    offset += 8;

    unsigned char flag = flag;
    memcpy(&data_buf[offset], &flag, 1);
    offset++;

    unsigned char prop = _property_count;
    memcpy(&data_buf[offset], &prop, 1);
    offset++;

    // add properties
    int i;
    for (const auto& property : _properties) {
        unsigned short pnsize = htobe16(property.name_size);
        memcpy(&data_buf[offset], &pnsize, 2);
        offset += 2;
        memcpy(&data_buf[offset], &property.name, property.name_size);
        offset += property.name_size;

        unsigned short pvsize = htobe16(property.value_size);
        memcpy(&data_buf[offset], &pvsize, 2);
        offset += 2;
        memcpy(&data_buf[offset], &property.value, property.value_size);
        offset += property.value_size;
    }

    if (_this_size != offset) {
        *data = NULL;
        *size = 0;
        throw std::runtime_error("ERROR: inconsistent record size");
        // throw std::runtime_error("%s: ERROR: inconsistent record size (present %d != created
        // %d)",  __func__, this_size, offset);
    }

    *data = data_buf;
    *size = offset;

    return 0;
}

} // namespace backup
} // namespace absinthe
