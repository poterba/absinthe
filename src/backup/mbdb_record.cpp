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
        domain = (char*) malloc(strsize + 1);
        if (domain == NULL) {
            throw std::runtime_error("Allocation Error!");
        }
        offset += 2;
        memcpy(domain, &data[offset], strsize);
        domain[strsize] = 0;
        offset += strsize;
    } else {
        domain = NULL;
        offset += 2;
    }
    domain_size = strsize;

    // Parse Path
    strsize = be16toh(*((unsigned short*) &data[offset]));
    if (strsize > 0 && strsize < 0xFFFF) {
        path = (char*) malloc(strsize + 1);
        if (path == NULL) {
            throw std::runtime_error("Allocation Error!");
        }
        offset += 2;
        memcpy(path, &data[offset], strsize);
        path[strsize] = 0;
        offset += strsize;
    } else {
        path = NULL;
        offset += 2;
    }
    path_size = strsize;

    // Parse Target
    strsize = be16toh(*((unsigned short*) &data[offset]));
    if (strsize > 0 && strsize < 0xFFFF) {
        target = (char*) malloc(strsize + 1);
        if (target == NULL) {
            throw std::runtime_error("Allocation Error!");
        }
        offset += 2;
        memcpy(target, &data[offset], strsize);
        target[strsize] = 0;
        offset += strsize;
    } else {
        target = NULL;
        offset += 2;
    }
    target_size = strsize;

    // parse DataHash
    strsize = be16toh(*((unsigned short*) &data[offset]));
    if (strsize > 0 && strsize < 0xFFFF) {
        datahash = (char*) malloc(strsize);
        if (datahash == NULL) {
            throw std::runtime_error("Allocation Error!");
        }
        offset += 2;
        memcpy(datahash, &data[offset], strsize);
        offset += strsize;
    } else {
        datahash = NULL;
        offset += 2;
    }
    datahash_size = strsize;

    // parse unknown1
    strsize = be16toh(*((unsigned short*) &data[offset]));
    if (strsize > 0 && strsize < 0xFFFF) {
        unknown1 = (char*) malloc(strsize + 1);
        if (unknown1 == NULL) {
            throw std::runtime_error("Allocation Error!");
        }
        offset += 2;
        memcpy(unknown1, &data[offset], strsize);
        unknown1[strsize] = 0;
        offset += strsize;
    } else {
        unknown1 = NULL;
        offset += 2;
    }
    unknown1_size = strsize;

    mode = be16toh(*((unsigned short*) &data[offset]));
    offset += 2;

    unknown2 = be32toh(*((unsigned int*) &data[offset]));
    offset += 4;

    inode = be32toh(*((unsigned int*) &data[offset]));
    offset += 4;

    uid = be32toh(*((unsigned int*) &data[offset]));
    offset += 4;

    gid = be32toh(*((unsigned int*) &data[offset]));
    offset += 4;

    time1 = be32toh(*((unsigned int*) &data[offset]));
    offset += 4;

    time2 = be32toh(*((unsigned int*) &data[offset]));
    offset += 4;

    time3 = be32toh(*((unsigned int*) &data[offset]));
    offset += 4;

    length = be64toh(*((unsigned long long*) &data[offset]));
    offset += 8;

    flag = *((unsigned char*) &data[offset]);
    offset += 1;

    property_count = *((unsigned char*) &data[offset]);
    offset += 1;

    for (int i = 0; i < property_count; i++) {
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

        properties.push_back(prop);
    }
    this_size = offset;

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
    std::cout << "\tdomain = " << domain << std::endl;
    std::cout << "\tpath = " << path << std::endl;
    std::cout << "\ttarget = " << target << std::endl;
    std::cout << "\tdatahash = " << datahash << std::endl;
    std::cout << "\tunknown1 = " << unknown1 << std::endl;
    std::cout << "\tmode = " << std::hex << mode << std::dec << std::endl;
    std::cout << "\tunknown2 = 0x" << unknown2 << std::endl;
    std::cout << "\tinode = 0x" << inode << std::endl;
    std::cout << "\tuid = " << uid << std::endl;
    std::cout << "\tgid = " << gid << std::endl;
    std::cout << "\ttime1 = 0x" << time1 << std::endl;
    std::cout << "\ttime2 = 0x" << time2 << std::endl;
    std::cout << "\ttime3 = 0x" << time3 << std::endl;
    std::cout << "\tlength = " << length << std::endl;
    std::cout << "\tflag = 0x" << flag << std::endl;
    std::cout << "\tproperty_count = " << property_count << std::endl;
}

void MBDBRecord::init()
{
    target_size = 0xFFFF;
    datahash_size = 0xFFFF;
    unknown1_size = 0xFFFF;
    this_size = 2 + 2 + 2 + 2 + 2 + 2 + 4 + 4 + 4 + 4 + 4 + 4 + 4 + 8 + 1 + 1;
}

void MBDBRecord::set_domain(const char* domain)
{
    unsigned short old_size = domain_size;
    if (domain_size > 0 && domain_size < 0xFFFF) {
        this_size -= domain_size;
    }
    if (domain && (strlen(domain) > 0)) {
        domain_size = strlen(domain);
        domain = strdup(domain);
        this_size += domain_size;
    } else {
        domain_size = 0;
    }
}

void MBDBRecord::set_path(const char* path)
{
    unsigned short old_size = path_size;
    path = {};
    if (path_size > 0 && path_size < 0xFFFF) {
        this_size -= path_size;
    }
    if (path && (strlen(path) > 0)) {
        path_size = strlen(path);
        path = strdup(path);
        this_size += path_size;
    } else {
        path_size = 0;
    }
}

void MBDBRecord::set_target(const char* target)
{
    unsigned short old_size = target_size;
    target = {};
    if (target_size > 0 && target_size < 0xFFFF) {
        this_size -= target_size;
    }
    if (target && (strlen(target) > 0)) {
        target_size = strlen(target);
        target = strdup(target);
        this_size += target_size;
    } else {
        target_size = 0xFFFF;
    }
}

void MBDBRecord::set_datahash(const char* hash, unsigned short hash_size)
{
    unsigned short old_size = datahash_size;
    datahash = {};
    if (datahash_size > 0 && datahash_size < 0xFFFF) {
        this_size -= datahash_size;
    }
    if (hash && (hash_size > 0)) {
        datahash_size = hash_size;
        datahash = (char*) malloc(hash_size);
        memcpy(datahash, hash, hash_size);
        this_size += datahash_size;
    } else {
        datahash_size = 0xFFFF;
    }
}

void MBDBRecord::set_unknown1(const char* data, unsigned short size)
{
    unsigned short old_size = unknown1_size;
    unknown1 = {};
    if (unknown1_size > 0 && unknown1_size < 0xFFFF) {
        this_size -= unknown1_size;
    }
    if (data && (size > 0)) {
        unknown1_size = size;
        unknown1 = (char*) malloc(size);
        memcpy(unknown1, data, size);
        this_size += unknown1_size;
    } else {
        unknown1_size = 0xFFFF;
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

    data_buf = (unsigned char*) malloc(this_size);
    if (!data_buf) {
        throw std::runtime_error("Allocation Error!");
        return -1;
    }

    unsigned short strsize;

    // append Domain
    strsize = htobe16(domain_size);
    memcpy(&data_buf[offset], &strsize, 2);
    offset += 2;
    if (domain != NULL) {
        memcpy(&data_buf[offset], domain, domain_size);
        offset += domain_size;
    }

    // append Path
    strsize = htobe16(path_size);
    memcpy(&data_buf[offset], &strsize, 2);
    offset += 2;
    if (path != NULL) {
        memcpy(&data_buf[offset], path, path_size);
        offset += path_size;
    }

    // append Target
    strsize = htobe16(target_size);
    memcpy(&data_buf[offset], &strsize, 2);
    offset += 2;
    if (target != NULL) {
        memcpy(&data_buf[offset], target, target_size);
        offset += target_size;
    }

    // append DataHash
    strsize = htobe16(datahash_size);
    memcpy(&data_buf[offset], &strsize, 2);
    offset += 2;
    if (datahash != NULL) {
        memcpy(&data_buf[offset], datahash, datahash_size);
        offset += datahash_size;
    }

    // append unknown1
    strsize = htobe16(unknown1_size);
    memcpy(&data_buf[offset], &strsize, 2);
    offset += 2;
    if (unknown1 != NULL) {
        memcpy(&data_buf[offset], unknown1, unknown1_size);
        offset += unknown1_size;
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

    unsigned char prop = property_count;
    memcpy(&data_buf[offset], &prop, 1);
    offset++;

    // add properties
    int i;
    for (const auto& property : properties) {
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

    if (this_size != offset) {
        *data = NULL;
        *size = 0;
        throw std::runtime_error("ERROR: inconsistent record size");
        // throw std::runtime_error("%s: ERROR: inconsistent record size (present %d != created
        // %d)",  __func__, this_size, offset);
        return -1;
    }

    *data = data_buf;
    *size = offset;

    return 0;
}

} // namespace backup
} // namespace absinthe
