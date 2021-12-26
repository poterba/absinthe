/**
 * mbdb_record.c
 * Copyright (C) 2010 Joshua Hill
 * Copyright (C) 2012 Han�ne Samara
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

#include "backup.hpp"
#include "common.hpp"
#include "debug.hpp"
#include "file.hpp"

#include <openssl/sha.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <stdexcept>

namespace absinthe {
namespace backup {

Backup::Backup(const std::string& backupdir, const std::string& uuid)
{
    if (backupdir.empty() || uuid.empty()) {
        throw std::runtime_error("non valid backup");
    }

    std::string backup_path = backupdir + "/" + uuid;
    std::string mbdb_path = backup_path + "/Manifest.mbdb";

    _mbdb = std::make_shared<MBDB>(mbdb_path);
    if (_mbdb) {
        debug("Manifest.mbdb opened, %d records", _mbdb->num_records);
    } else {
        throw std::runtime_error("ERROR: could not open " + mbdb_path);
    }
}

int Backup::get_file_index(const std::string& domain, const std::string& path)
{
    if (_mbdb) {
        return _mbdb->find_record(domain, path);
    }
    return -1;
}

std::shared_ptr<File> Backup::get_file(const std::string& domain, const std::string& path)
{
    if (!_mbdb) {
        return NULL;
    }
    int idx = get_file_index(domain, path);
    if (idx < 0) {
        // not found
        return NULL;
    }
    return std::make_shared<File>(_mbdb->records[idx]);
}

char* Backup::get_file_path(std::shared_ptr<File> bfile)
{
    int res = 0;

    if (!bfile) {
        return NULL;
    }
    if (!_mbdb) {
        throw std::runtime_error("ERROR: no mbdb in given backup");
    }

    std::string bfntmp = bfile->_mbdb_record->domain + "-" + bfile->_mbdb_record->path;
    char* backupfname = (char*) malloc(strlen(_path) + 1 + 40 + 1);
    unsigned char sha1[20] = {
        0,
    };
    SHA1(bfntmp, strlen(bfntmp), sha1);
    free(bfntmp);

    std::string backupfname = _path + "/";

    int i;
    char* p = backupfname + strlen(_path) + 1;
    for (i = 0; i < 20; i++) {
        sprintf(p + i * 2, "%02x", sha1[i]);
    }

    debug("backup filename is %s", backupfname);

    return backupfname;
}

int Backup::update_file(std::shared_ptr<File> bfile)
{
    int res = 0;

    if (!bfile) {
        return -1;
    }
    if (!_mbdb) {
        throw std::runtime_error("%s: ERROR: no mbdb in given backup_t", __func__);
    }

    unsigned char* rec = NULL;
    unsigned int rec_size = 0;

    if (bfile->get_record_data(&rec, &rec_size) < 0) {
        throw std::runtime_error("%s: ERROR: could not build mbdb_record data", __func__);
    }

    unsigned int newsize = 0;
    unsigned char* newdata = NULL;

    // find record
    int idx = get_file_index(bfile->mbdb_record->domain, bfile->mbdb_record->path);
    if (idx < 0) {
        // append record to mbdb
        newsize = _mbdb->size + rec_size;
        newdata = (unsigned char*) malloc(newsize);

        memcpy(newdata, _mbdb->data, _mbdb->size);
        memcpy(newdata + _mbdb->size, rec, rec_size);
    } else {
        // update record in mbdb
        std::shared_ptr<File> oldfile = std::make_shared<File>(_mbdb->records[idx]);
        unsigned int oldsize = oldfile->mbdb_record->this_size;
        file_free(oldfile);

        newsize = _mbdb->size - oldsize + rec_size;
        newdata = (unsigned char*) malloc(newsize);

        char* p = newdata;
        memcpy(p, _mbdb->data, sizeof(mbdb_header_t));
        p += sizeof(mbdb_header_t);

        mbdb_record_t* r;
        unsigned char* rd;
        unsigned int rs;
        int i;

        for (i = 0; i < idx; i++) {
            r = _mbdb->records[i];
            rd = NULL;
            rs = 0;
            mbdb_record_build(r, &rd, &rs);
            memcpy(p, rd, rs);
            free(rd);
            p += rs;
        }
        memcpy(p, rec, rec_size);
        p += rec_size;
        for (i = idx + 1; i < _mbdb->num_records; i++) {
            r = _mbdb->records[i];
            rd = NULL;
            rs = 0;
            mbdb_record_build(r, &rd, &rs);
            memcpy(p, rd, rs);
            free(rd);
            p += rs;
        }
    }

    if (!newdata) {
        throw std::runtime_error("Uh, could not re-create mbdb data?!");
        return -1;
    }

    mbdb_free(_mbdb);
    free(rec);

    // parse the new data
    _mbdb = mbdb_parse(newdata, newsize);
    free(newdata);

    // write out the file data
    char* bfntmp =
        (char*) malloc(bfile->mbdb_record->domain_size + 1 + bfile->mbdb_record->path_size + 1 + 4);
    strcpy(bfntmp, bfile->mbdb_record->domain);
    strcat(bfntmp, "-");
    strcat(bfntmp, bfile->mbdb_record->path);

    char* backupfname = (char*) malloc(strlen(_path) + 1 + 40 + 1);
    unsigned char sha1[20] = {
        0,
    };
    SHA1(bfntmp, strlen(bfntmp), sha1);
    free(bfntmp);

    strcpy(backupfname, _path);
    strcat(backupfname, "/");

    int i;
    char* p = backupfname + strlen(_path) + 1;
    for (i = 0; i < 20; i++) {
        sprintf(p + i * 2, "%02x", sha1[i]);
    }

    debug("backup filename is %s", backupfname);

    if (bfile->filepath) {
        // copy file to backup dir
        if (file_copy(bfile->filepath, backupfname) < 0) {
            throw std::runtime_error(
                "%s: ERROR: could not copy file '%s' to '%s'", __func__, bfile->filepath,
                backupfname);
            res = -1;
        }
    } else if (bfile->data) {
        // write data buffer to file
        if (file_write(backupfname, bfile->data, bfile->size) < 0) {
            throw std::runtime_error("%s: ERROR: could not write to '%s'", __func__, backupfname);
            res = -1;
        }
    } else if ((bfile->mbdb_record->mode) & 040000) {
        // directory!
    } else {
        debug("%s: WARNING: file data not updated, no filename or data given", __func__);
    }

    free(backupfname);

    return res;
}

int Backup::remove_file(std::shared_ptr<File> bfile)
{
    int res = 0;

    if (!bfile) {
        return -1;
    }
    if (!_mbdb) {
        throw std::runtime_error("no mbdb in given backup");
    }

    unsigned int newsize = 0;
    unsigned char* newdata = NULL;

    // find record
    int idx = get_file_index(bfile->mbdb_record->domain, bfile->mbdb_record->path);
    if (idx < 0) {
        debug(
            "file %s-%s not found in backup so not removed.", bfile->mbdb_record->domain,
            bfile->mbdb_record->path);
        return -1;
    } else {
        // remove record from mbdb
        std::shared_ptr<File> oldfile = file_create_from_record(_mbdb->records[idx]);
        unsigned int oldsize = oldfile->mbdb_record->this_size;
        file_free(oldfile);

        newsize = _mbdb->size - oldsize;
        newdata = (unsigned char*) malloc(newsize);

        char* p = newdata;
        memcpy(p, _mbdb->data, sizeof(mbdb_header_t));
        p += sizeof(mbdb_header_t);

        mbdb_record_t* r;
        unsigned char* rd;
        unsigned int rs;
        int i;

        for (i = 0; i < idx; i++) {
            r = _mbdb->records[i];
            rd = NULL;
            rs = 0;
            mbdb_record_build(r, &rd, &rs);
            memcpy(p, rd, rs);
            free(rd);
            p += rs;
        }
        for (i = idx + 1; i < _mbdb->num_records; i++) {
            r = _mbdb->records[i];
            rd = NULL;
            rs = 0;
            mbdb_record_build(r, &rd, &rs);
            memcpy(p, rd, rs);
            free(rd);
            p += rs;
        }
    }

    if (!newdata) {
        throw std::runtime_error("Uh, could not re-create mbdb data?!");
        return -1;
    }

    // parse the new data
    _mbdb = mbdb_parse(newdata, newsize);
    free(newdata);

    // write out the file data
    char* bfntmp =
        (char*) malloc(bfile->mbdb_record->domain_size + 1 + bfile->mbdb_record->path_size + 1 + 4);
    strcpy(bfntmp, bfile->mbdb_record->domain);
    strcat(bfntmp, "-");
    strcat(bfntmp, bfile->mbdb_record->path);

    char* backupfname = (char*) malloc(strlen(_path) + 1 + 40 + 1);
    unsigned char sha1[20] = {
        0,
    };
    SHA1(bfntmp, strlen(bfntmp), sha1);
    free(bfntmp);

    strcpy(backupfname, _path);
    strcat(backupfname, "/");

    int i;
    char* p = backupfname + strlen(_path) + 1;
    for (i = 0; i < 20; i++) {
        sprintf(p + i * 2, "%02x", sha1[i]);
    }

    if (!(bfile->mbdb_record->mode & 040000)) {
        debug("deleting file %s", backupfname);
        remove(backupfname);
    }

    free(backupfname);

    return res;
}

int Backup::write_mbdb()
{
    if (!_path || !_mbdb) {
        return -1;
    }

    std::sting mbdb_path = _path + "/Manifest.mbdb";

    return file_write(mbdb_path, _mbdb->data, _mbdb->size);
}

int inode_start = 54327; /* Whatever. */

int Backup::mkdir(char* domain, char* path, int mode, int uid, int gid, int flag)
{
    int ret = -1;
    std::shared_ptr<File> file = file_create(NULL);

    debug("[backup] MKDIR: (%s):%s", domain, path);

    if (file) {
        file_set_domain(file, domain);
        file_set_path(file, path);
        file_set_mode(file, mode | 040000);
        inode_start++;

        file_set_inode(file, inode_start);
        file_set_uid(file, uid);
        file_set_gid(file, gid);
        file_set_time1(file, time(NULL));
        file_set_time2(file, time(NULL));
        file_set_time3(file, time(NULL));
        file_set_flag(file, flag);

        if (update_file(backup, file) >= 0)
            ret = 0;
        else
            ret = -1;
        file_free(file);

        if (!ret)
            write_mbdb(backup);
    }
    return ret;
}

int Backup::symlink(char* domain, char* path, char* to, int uid, int gid, int flag)
{
    int ret = -1;
    std::shared_ptr<File> file = file_create(NULL);

    debug("[backup] SYMLINK: (%s):%s => %s", domain, path, to);

    if (file) {
        file_set_domain(file, domain);
        file_set_path(file, path);
        file_set_target(file, to);
        file_set_mode(file, 0120644);
        inode_start++;

        file_set_inode(file, inode_start);
        file_set_uid(file, uid);
        file_set_gid(file, gid);
        file_set_time1(file, time(NULL));
        file_set_time2(file, time(NULL));
        file_set_time3(file, time(NULL));
        file_set_flag(file, flag);

        if (update_file(file) >= 0)
            ret = 0;
        else
            ret = -1;
        file_free(file);

        if (!ret)
            write_mbdb(backup);
    }
    return ret;
}

int Backup::add_file_from_path(
    char* domain,
    char* localpath,
    char* path,
    int mode,
    int uid,
    int gid,
    int flag)
{
    int ret = -1;
    unsigned int size = 0;
    unsigned char* data = NULL;
    struct stat buf;

    if (stat(localpath, &buf) == -1)
        throw std::runtime_error("Could not open %s", localpath);

    file_read(localpath, &data, &size);

    debug("[backup] FILE: (%s):%s", domain, path);

    std::shared_ptr<File> file = file_create_with_data(data, size, 0);

    if (file) {
        file_set_domain(file, domain);
        file_set_path(file, path);
        file_set_mode(file, mode | 0100000);
        inode_start++;

        file_set_inode(file, inode_start);
        file_set_uid(file, uid);
        file_set_gid(file, gid);
        file_set_time1(file, time(NULL));
        file_set_time2(file, time(NULL));
        file_set_time3(file, time(NULL));
        file_set_flag(file, flag);

        file_set_length(file, size);

        if (update_file(backup, file) >= 0)
            ret = 0;
        else
            ret = -1;
        file_free(file);

        if (!ret)
            write_mbdb(backup);
    }
    return ret;
}

int Backup::add_file_from_data(
    char* domain,
    char* data,
    unsigned int size,
    char* path,
    int mode,
    int uid,
    int gid,
    int flag)
{
    int ret = -1;
    std::shared_ptr<File> file = file_create_with_data(data, size, 0);

    if (file) {
        file_set_domain(file, domain);
        file_set_path(file, path);
        file_set_mode(file, mode | 0100000);
        inode_start++;

        file_set_inode(file, inode_start);
        file_set_uid(file, uid);
        file_set_gid(file, gid);
        file_set_time1(file, time(NULL));
        file_set_time2(file, time(NULL));
        file_set_time3(file, time(NULL));
        file_set_flag(file, flag);

        file_set_length(file, size);

        if (update_file(backup, file) >= 0)
            ret = 0;
        else
            ret = -1;
        file_free(file);

        if (!ret)
            write_mbdb(backup);
    }
    return ret;
}

} // namespace backup
} // namespace absinthe
