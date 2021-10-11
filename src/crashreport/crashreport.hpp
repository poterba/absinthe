/**
 * GreenPois0n Absinthe - crashreport.h
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

#include <plist/plist.h>
#include <string>
#include <vector>

namespace absinthe {
namespace crashreport {

/* The actual crashreport object containing it's data */
class Report final
{
  public:
    struct dylib_info final
    {
        char* name;
        uint32_t offset;
    };

    struct arm_state final
    {
        uint32_t r0;
        uint32_t r1;
        uint32_t r2;
        uint32_t r3;
        uint32_t r4;
        uint32_t r5;
        uint32_t r6;
        uint32_t r7;
        uint32_t r8;
        uint32_t r9;
        uint32_t r10;
        uint32_t r11;
        uint32_t ip;
        uint32_t sp;
        uint32_t lr;
        uint32_t pc;
        uint32_t cpsr;
    };

  public:
    Report() = default;
    Report(plist_t crash);
    void _debug();

    std::string parse_name(const std::string& description);
    unsigned int parse_pid(const std::string& description);
    std::unique_ptr<arm_state> parse_state(const std::string& description);
    std::vector<dylib_info> parse_dylibs(const std::string& description);

  private:
    std::string _name;
    unsigned int _pid;
    std::shared_ptr<arm_state> _state;
    std::vector<dylib_info> _dylibs;
};

} // namespace crashreport
} // namespace absinthe
