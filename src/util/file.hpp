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

// #include <stdint.h>
// #include <stdio.h>
#include <string>
#include <vector>

namespace absinthe {
namespace util {

class File
{
  public:
    File(const std::string& path);
    ~File();

    void close();
    int read(std::string& buf, unsigned int* length);
    int write(unsigned char* buf, unsigned int length);
    int copy(const std::string& to);

  private:
    std::string _path;
    FILE* _desc;
    uint64_t _size;
    uint64_t _offset;
    std::vector<unsigned char> _data;
};

} // namespace util
} // namespace absinthe
