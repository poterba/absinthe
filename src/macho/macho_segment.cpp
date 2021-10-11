/**
 * GreenPois0n Absinthe - macho_segment.c
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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "common.hpp"
#include "debug.hpp"
#include "macho_segment.hpp"

/*
 * Mach-O Segment Functions
 */
macho_segment_t* macho_segment_create()
{
    macho_segment_t* segment = (macho_segment_t*)malloc(sizeof(macho_segment_t));
    if (segment)
    {
        memset(segment, '\0', sizeof(macho_segment_t));
    }
    return segment;
}

macho_segment_t* macho_segment_load(unsigned char* data, unsigned int offset)
{
    unsigned char* address = NULL;
    macho_segment_t* segment = macho_segment_create();
    if (segment)
    {
        segment->command = macho_segment_cmd_load(data, offset);
        if (!segment->command)
        {
            macho_segment_free(segment);
            return NULL;
        }
        segment->name = strdup(segment->command->segname);
        segment->size = segment->command->filesize;
        segment->offset = segment->command->fileoff;
        segment->address = segment->command->vmaddr;
        segment->data = NULL; // ????
        // segment->sections = malloc(segment->cmd->nsects * sizeof(macho_section_t*));
    }
    return segment;
}

void macho_segment_debug(macho_segment_t* segment)
{
    debug("\tSegment:");
    debug("\t\tname: %s", segment->name);
    debug("\t\tsize: 0x%x", segment->size);
    debug("\t\toffset: 0x%x", segment->offset);
    debug("\t\taddress: 0x%08x", segment->address);
}

void macho_segment_free(macho_segment_t* segment)
{
    if (segment)
    {
        if (segment->command)
        {
            macho_segment_cmd_free(segment->command);
        }
        if (segment->name)
        {
            free(segment->name);
        }
        free(segment);
    }
}

/*
 * Mach-O Segment Info Functions
 */
macho_segment_cmd_t* macho_segment_cmd_create()
{
    macho_segment_cmd_t* info = malloc(sizeof(macho_segment_cmd_t));
    if (info)
    {
        memset(info, '\0', sizeof(macho_segment_cmd_t));
    }
    return info;
}

macho_segment_cmd_t* macho_segment_cmd_load(unsigned char* data, unsigned int offset)
{
    macho_segment_cmd_t* cmd = macho_segment_cmd_create();
    if (cmd)
    {
        memcpy(cmd, data + offset, sizeof(macho_segment_cmd_t));
        // macho_segment_cmd_debug(cmd);
    }
    return cmd;
}

void macho_segment_cmd_debug(macho_segment_cmd_t* cmd)
{
    debug("\tSegment Command:");
    debug("\t\t     cmd = 0x%x", cmd->cmd);
    debug("\t\t cmdsize = 0x%x", cmd->cmdsize);
    debug("\t\t segname = %s", cmd->segname);
    debug("\t\t  vmaddr = 0x%08x", cmd->vmaddr);
    debug("\t\t  vmsize = 0x%x", cmd->vmsize);
    debug("\t\t fileoff = 0x%x", cmd->fileoff);
    debug("\t\tfilesize = 0x%x", cmd->filesize);
    debug("\t\t maxprot = 0x%08x", cmd->maxprot);
    debug("\t\tinitprot = 0x%08x", cmd->initprot);
    debug("\t\t  nsects = 0x%x", cmd->nsects);
    debug("\t\t   flags = 0x%08x", cmd->flags);
}

void macho_segment_cmd_free(macho_segment_cmd_t* cmd)
{
    if (cmd)
    {
        free(cmd);
    }
}
