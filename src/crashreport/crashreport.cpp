/**
 * GreenPois0n Absinthe - crashreport.c
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

#include "crashreport.hpp"
#include "debug.hpp"
#include <libimobiledevice/libimobiledevice.h>
#include <plist/plist.h>
#include <stdexcept>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

namespace absinthe
{
namespace crashreport
{

Report::Report(plist_t plist)
{
    char* description = NULL;
    plist_t description_node = NULL;

    // The description element is the one with all the good stuff
    description_node = plist_dict_get_item(plist, "description");
    if (description_node && plist_get_node_type(description_node) == PLIST_STRING)
    {
        plist_get_string_val(description_node, &description);
        if (!description)
        {
            throw std::runtime_error("Unable to get description node");
        }

        _pid = parse_pid(description);

        _name = parse_name(description);
        if (_name.empty())
        {
            free(description);
            throw std::runtime_error("Unable to parse process name from crashreport");
        }
        debug("Crashed process: %s", _name);

        _state = parse_state(description);
        if (_state == NULL)
        {
            free(description);
            throw std::runtime_error("Unable to parse ARM state from crashreport");
        }

        _dylibs = parse_dylibs(description);
        if (_dylibs.empty())
        {
            free(description);
            throw std::runtime_error("Unable to parse dylib base addresses from crashreport");
        }
        free(description);
    }
}

void Report::_debug()
{
    // TODO: Implement _debug function
}

std::string Report::parse_name(const std::string& description)
{
    char name[256];
    const char* start = strstr(description.data(), "Process:");
    if (start)
    {
        start += 8;
        if (sscanf(start, "%*[ ]%s ", name) == 1)
        {
            return name;
        }
    }
    return {};
}

unsigned int Report::parse_pid(const std::string& description)
{
    unsigned int pid;
    const char* start = strstr(description.data(), "Process:");
    if (!start)
    {
        return 0;
    }
    start += 8;
    if (sscanf(start, "%*[ ]%*s [%d]", &pid) == 1)
    {
        return pid;
    }
    else
    {
        return 0;
    }
}

std::unique_ptr<Report::arm_state> Report::parse_state(const std::string& description)
{
    int num = 0;
    char line[256];
    const char* lf{nullptr};
    const char* start{nullptr};

    // Find beginning of thread state section
    start = strstr(description.data(), "ARM Thread State");
    if (!start)
    {
        throw std::runtime_error("Couldn't find ARM state beginning");
    }

    // Find beginning of first line
    start = strchr(start, '\n');
    if (!start)
    {
        throw std::runtime_error("Couldn't get linebreak after beginning line");
    }
    start++;

    // Find end of first line
    lf = strchr(start, '\n');
    if (!lf)
    {
        throw std::runtime_error("Couldn't get linebreak at end of line");
    }

    // Sanity check and copy into temp buffer
    if (lf - start >= 256 || lf - start <= 0)
    {
        throw std::runtime_error("Unable to copy data into stack buffer");
    }
    memcpy(line, start, lf - start);
    line[lf - start] = 0;

    // Allocate memory for our state structure
    auto state = std::make_unique<arm_state>();
    // Read in formatted data for first line
    num = sscanf(line, "%*[ ]r0: 0x%08x%*[ ]r1: 0x%08x%*[ ]r2: 0x%08x%*[ ]r3: 0x%08x", &state->r0,
                 &state->r1, &state->r2, &state->r3);
    if (num != 4)
    {
        return NULL;
    }

    // Find beginning of second line
    start = lf + 1;
    lf = strchr(start, '\n');
    if (!lf)
    {
        return NULL;
    }

    // Sanity check and copy second line into temp buffer
    if (lf - start >= 256 || lf - start <= 0)
    {
        throw std::runtime_error("Unable to copy data into stack buffer");
    }
    memcpy(line, start, lf - start);
    line[lf - start] = 0;

    // Read in formatted data for second line
    num = sscanf(line, "%*[ ]r4: 0x%08x%*[ ]r5: 0x%08x%*[ ]r6: 0x%08x%*[ ]r7: 0x%08x", &state->r4,
                 &state->r5, &state->r6, &state->r7);
    if (num != 4)
    {
        return {};
    }

    // Find beginning of third line
    start = lf + 1;
    lf = strchr(start, '\n');
    if (!lf)
    {
        return {};
    }

    // Sanity check and copy third line into temp buffer
    if (lf - start >= 256 || lf - start <= 0)
    {
        throw std::runtime_error("Unable to copy data into stack buffer");
    }
    memcpy(line, start, lf - start);
    line[lf - start] = 0;

    // Read in formatted data for third line
    num = sscanf(line, "%*[ ]r8: 0x%08x%*[ ]r9: 0x%08x%*[ ]r10: 0x%08x%*[ ]r11: 0x%08x", &state->r8,
                 &state->r9, &state->r10, &state->r11);
    if (num != 4)
    {
        return {};
    }

    // Find beginning of forth line
    start = lf + 1;
    lf = strchr(start, '\n');
    if (!lf)
    {
        return {};
    }

    // Sanity check and copy forth line into temp buffer
    if (lf - start >= 256 || lf - start <= 0)
    {
        throw std::runtime_error("Unable to copy data into stack buffer");
    }
    memcpy(line, start, lf - start);
    line[lf - start] = 0;

    // Read in formatted data for forth line
    num = sscanf(line, "%*[ ]ip: 0x%08x%*[ ]sp: 0x%08x%*[ ]lr: 0x%08x%*[ ]pc: 0x%08x", &state->ip,
                 &state->sp, &state->lr, &state->pc);
    if (num != 4)
    {
        return {};
    }

    // Find beginning of fifth line
    start = lf + 1;
    lf = strchr(start, '\n');
    if (!lf)
    {
        return {};
    }

    // Sanity check and copy fifth line into temp buffer
    if (lf - start >= 256 || lf - start <= 0)
    {
        throw std::runtime_error("Unable to copy data into stack buffer");
    }
    memcpy(line, start, lf - start);
    line[lf - start] = 0;

    // Read in formatted data for fifth line
    num = sscanf(line, "%*[ ]cpsr: 0x%08x", &state->cpsr);
    if (num != 1)
    {
        return NULL;
    }

    debug("ARM State = {\n\tr0:%08x r1:%08x  r2:%08x  r3:%08x"
          "\tr4:%08x r5:%08x  r6:%08x  r7:%08x"
          "\tr8:%08x r9:%08x r10:%08x r11:%08x"
          "\tip:%08x sp:%08x  lr:%08x  pc:%08x"
          "\tcpsr:%08x\n}",
          state->r0, state->r1, state->r2, state->r3, state->r4, state->r5, state->r6, state->r7,
          state->r8, state->r9, state->r10, state->r11, state->ip, state->sp, state->lr, state->pc,
          state->cpsr);

    return state;
}

std::vector<Report::dylib_info> Report::parse_dylibs(const std::string& description)
{
    std::vector<dylib_info> dylibs;
    int dylibs_cnt = 256;
    char line[256];
    int num;
    const char* lf;

    const char* start = strstr(description.data(), "Binary Images:");
    if (!start)
    {
        throw std::runtime_error("Couldn't find Binary Images beginning");
    }

    start = strchr(start, '\n');
    if (!start)
    {
        throw std::runtime_error("Couldn't get linebreak after beginning line");
    }

    start++;
    lf = strchr(start, '\n');
    while (lf)
    {
        memcpy(line, start, lf - start);
        line[lf - start] = 0;
        const char* lineptr = line;
        uint32_t offset = 0;
        char imagename[256];
        imagename[0] = 0;
        while ((lineptr[0] != 0) && (lineptr[0] == ' '))
        {
            lineptr++;
        }
        num = sscanf(lineptr, "0x%x -%*[ ]0x%*[0-9a-fA-F]%*[ ]%s arm", &offset, imagename);
        if (num == 2)
        {
            dylib_info info;
            info.offset = offset;
            info.name = imagename;
            dylibs.push_back(info);
        }

        start = lf + 1;
        lf = strchr(start, '\n');
    }

#ifdef _DEBUG
    {
        debug("Number of binary images: %d {", dylibs.size());
        int i = 0;
        while (dylibs[i])
        {
            debug("\t%d:\t0x%08x: %s", i, dylibs[i]->offset, dylibs[i]->name);
            i++;
        }
        debug("}");
    }
#endif

    return dylibs;
}

} // namespace crashreport
} // namespace absinthe
