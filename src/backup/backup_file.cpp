/**
 * GreenPois0n Apparition - backup_file.c
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

#include "backup_file.hpp"

#include "debug.hpp"
#include "mbdb_record.hpp"

#include <openssl/sha.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

namespace absinthe {
namespace backup {

File::File(const std::string& filepath)
{
    if (!filepath.empty()) {
        _filepath = filepath;
    }
    _mbdb_record = std::make_shared<MBDBRecord>();
    _mbdb_record->init();
}

File::File(const std::vector<unsigned char>& data, int copy)
{
    _mbdb_record = std::make_shared<MBDBRecord>();
    _mbdb_record->init();
    assign_file_data(data, copy);
}

File::File(std::shared_ptr<MBDBRecord> record) : _mbdb_record(record) {}

void File::assign_file_data(const std::vector<unsigned char>& data, int copy)
{
    _data = data;
    _filepath = {};
}

void File::assign_file_path(const std::string& path)
{
    if (!_data.empty() && _free_data) {
        _data = {};
        _free_data = 0;
    }
    _filepath = path;
}

void File::set_domain(const std::string& domain) { _mbdb_record->set_domain(domain); }

void File::set_path(const std::string& path) { _mbdb_record->set_path(path); }

void File::set_target(const std::string& target) { _mbdb_record->set_target(target); }

static void debug_hash(const unsigned char* hash, int len)
{
    int i;
    for (i = 0; i < len; i++) {
        debug("%02x", hash[i]);
    }
    debug("");
}

void File::update_hash()
{
    if (_filepath) {
        FILE* f = fopen(_filepath, "rb");
        if (!f) {
            throw std::runtime_error("%s: ERROR: Could not open file '%s'", __func__, _filepath);
        }
        unsigned char buf[8192];
        size_t bytes;
        unsigned char sha1[20] = {
            0,
        };
        SHA_CTX shactx;
        SHA1_Init(&shactx);
        while (!feof(f)) {
            bytes = fread(buf, 1, sizeof(buf), f);
            if (bytes > 0) {
                SHA1_Update(&shactx, buf, bytes);
            }
        }
        SHA1_Final(sha1, &shactx);
        fclose(f);
        debug("setting datahash to ");
        debug_hash(sha1, 20);
        _mbdb_record->set_datahash(sha1, 20);
    } else if (_data) {
        unsigned char sha1[20] = {
            0,
        };
        SHA1(_data, _size, sha1);
        debug("setting datahash to ");
        debug_hash(sha1, 20);
        _mbdb_record->set_datahash(sha1, 20);
    } else {
        throw std::runtime_error(
            "%s: ERROR: neither filename nor data given, setting hash to N/A", __func__);
        _mbdb_record->set_datahash(NULL, 0);
    }
}

void File::disable_hash() { mbdb_record->set_datahash(NULL, 0); }

void File::set_mode(unsigned short mode) { _mbdb_record->set_mode(mode); }

void File::set_inode(unsigned int inode) { _mbdb_record->set_inode(inode); }

void File::set_uid(unsigned int uid) { _mbdb_record->set_uid(uid); }

void File::set_gid(unsigned int gid) { _mbdb_record->set_gid(gid); }

void File::set_time1(unsigned int time1) { mbdb_record::set_time1(_mbdb_record, time1); }

void File::set_time2(unsigned int time2) { _mbdb_record->set_time2(time2); }

void File::set_time3(unsigned int time3) { _mbdb_record->set_time3(time3); }

void File::set_length(unsigned long long length) { _mbdb_record->set_length(length); }

void File::set_flag(unsigned char flag) { _mbdb_record->set_flag(flag); }

int File::get_record_data(unsigned char** data, unsigned int* size)
{
    if (!_mbdb_record) {
        throw std::runtime_error("%s: ERROR: no mbdb_record present", __func__);
    }

    if (_mbdb_record->build(data, size) < 0) {
        throw std::runtime_error("%s: ERROR: could not build mbdb_record data", __func__);
    }

    return 0;
}
