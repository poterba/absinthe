/**
 * GreenPois0n Absinthe - file.h
 * Copyright (C) 2010 Chronic-Dev Team
 * Copyright (C) 2010 Joshua Hill
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

#include <stdint.h>
#include <stdio.h>
#include <string>

namespace absinthe {
namespace util {

typedef struct file_t
{
    FILE* desc;
    char* path;
    uint64_t size;
    uint64_t offset;
    unsigned char* data;
} file_t;

file_t* file_create();
void file_close(file_t* file);
void file_free(file_t* file);
file_t* file_open(const std::string& path);

int file_read(const std::string& file, std::string& buf, unsigned int* length);
int file_write(const std::string& file, unsigned char* buf, unsigned int length);
int file_copy(const std::string& from, const std::string& to);

} // namespace util
} // namespace absinthe
