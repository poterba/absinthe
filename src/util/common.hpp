/**
 * GreenPois0n Absinthe - common.h
 * Copyright (C) 2011 Chronic-Dev Team
 * Copyright (C) 2011 Nikias Bassen
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

// #include <errno.h>
// #include <libgen.h>
// #include <stdarg.h>
// #include <stdbool.h>
// #include <stdint.h>
// #include <stdio.h>
// #include <stdlib.h>
#include <string>
// #include <sys/stat.h>

#define BUFSMALL 0x40
#define BUFMEDIUM 0x400
#define BUFLARGE 0x4000

namespace absinthe {
namespace util {

int mkdir_with_parents(const std::string& dir, int mode);
std::string build_path(const std::string& elem, ...);
std::string prot2str(uint32_t prot);
void hexdump(unsigned char* buf, unsigned int len);
void rmdir_recursive(const std::string& path);
int __mkdir(const std::string& path, int mode);

} // namespace util
} // namespace absinthe