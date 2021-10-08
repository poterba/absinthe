/**
 * GreenPois0n Apparition - mbdb.c
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

#include "mbdb.hpp"

#include "debug.hpp"
#include "file.hpp"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

namespace absinthe
{
namespace backup
{

void MBDB::parse(unsigned char* data, unsigned int size)
{
    int i = 0;
    unsigned int count = 0;
    unsigned int offset = 0;

    mbdb_t* mbdb = NULL;
    header_t* header = NULL;
    MBDBRecord_t* record = NULL;

    header = (header_t*)data;
    if (strncmp(header->magic, MBDB_MAGIC, 6) != 0)
    {
        error("Unable to identify this filetype\n");
        return NULL;
    }

    // Copy in our header data
    _header = (header_t*)malloc(sizeof(header_t));
    if (_header == NULL)
    {
        error("Allocation error\n");
        return NULL;
    }
    memset(_header, '\0', sizeof(header_t));
    memcpy(_header, &data[offset], sizeof(header_t));
    offset += sizeof(header_t);

    _data = (unsigned char*)malloc(size);
    if (_data == NULL)
    {
        error("Allocation Error!!\n");
        return NULL;
    }
    memcpy(_data, data, size);
    _size = size;

    _records = (mbdb_record::mbdb_record_t**)malloc(
        (_size / 64) * sizeof(mbdb_record::mbdb_record_t)); // should be enough
    _num_records = 0;

    while (offset < _size)
    {
        mbdb_record::mbdb_record_t* rec = mbdb_record::parse(&(_data)[offset]);
        if (!rec)
        {
            error("Unable to parse record at offset 0x%x!\n", offset);
            break;
        }
        _records[_num_records++] = rec;
        offset += rec->this_size;
    }

    return mbdb;
}

void MBDB::open(unsigned char* file)
{
    int err = 0;
    unsigned int size = 0;
    unsigned char* data = NULL;

    err = util::file_read(file, &data, &size);
    if (err < 0)
    {
        throw std::runtime_error("Unable to read mbdb file");
    }

    mbdb = parse(data, size);
    if (mbdb == NULL)
    {
        throw std::runtime_error("Unable to parse mbdb file\n");
    }

    free(data);
}

mbdb_record::mbdb_record_t* get_record(mbdb_t* mbdb, unsigned int index) { return NULL; }

void free(mbdb_t* mbdb)
{
    if (mbdb)
    {
        if (_header)
        {
            free(_header);
            _header = NULL;
        }
        if (_records)
        {
            int i;
            for (i = 0; i < _num_records; i++)
            {
                mbdb_record::free(_records[i]);
            }
            free(_records);
        }
        if (_data)
        {
            free(_data);
        }
        free(mbdb);
    }
}

} // namespace mbdb
} // namespace backup
} // namespace absinthe
