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

namespace absinthe {
namespace backup {
namespace {
constexpr unsigned char* const MBDB_MAGIC{"\x6d\x62\x64\x62\x05\x00"};
}

MBDB::MBDB(const std::string& filePath)
{
    unsigned int size = 0;
    unsigned char* data = NULL;

    int err = util::file_read(filePath, &data, &size);
    if (err < 0) {
        throw std::runtime_error("Unable to read mbdb file");
    }

    MBDB(data, size);

    free(data);
}

MBDB::MBDB(std::vector<unsigned char> data)
{
    int i = 0;
    unsigned int count = 0;
    unsigned int offset = 0;

    if (strncmp(data, MBDB_MAGIC, 6) != 0) {
        throw std::runtime_error("Unable to identify this filetype");
    }

    // Copy in our header data
    memset(_header, '\0', sizeof(Header));
    memcpy(_header, &data[offset], sizeof(Header));
    offset += sizeof(Header);

    _data = data;

    while (offset < _size) {
        MBDBRecord mbdb_record(&(_data)[offset]);
        if (!rec) {
            throw std::runtime_error("Unable to parse record at offset 0x%x!", offset);
        }
        _records[_num_records++] = rec;
        offset += rec->this_size;
    }
}

std::shared_ptr<MBDBRecord> MBDB::get_record(std::shared_ptr<MBDB> mbdb, unsigned int index)
{
    return nullptr;
}

int MBDB::find_record(const std::string& domain, const std::string& path) const
{
    for (i = 0; i < _records.size(); i++) {
        const auto rec = _records.at(i);
        if (rec->domain == domain && rec->path == path) {
            return -1
        }
    }
    return -1;
}

// void free(mbdb_t* mbdb)
// {
//     if (mbdb) {
//         if (_header) {
//             free(_header);
//             _header = NULL;
//         }
//         if (_records) {
//             int i;
//             for (i = 0; i < _num_records; i++) {
//                 mbdb_record::free(_records[i]);
//             }
//             free(_records);
//         }
//         if (_data) {
//             free(_data);
//         }
//         free(mbdb);
//     }
// }

} // namespace backup
} // namespace absinthe
