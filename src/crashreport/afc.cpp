/**
 * GreenPois0n Absinthe - afc.c
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

#include "afc.hpp"
#include "debug.hpp"
#include "lockdown.hpp"

#include <libimobiledevice/afc.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

namespace absinthe
{
namespace crashreport
{
void AFC::connect(std::shared_ptr<util::Device> device)
{
    int err = 0;
    uint16_t port = 0;
    afc_t* afc = NULL;
    lockdown_t* lockdown = NULL;

    lockdown = lockdown_open(device);
    if (lockdown == NULL)
    {
        throw std::runtime_error("Unable to open connection to lockdownd");
    }

    err = lockdown_start_service(lockdown, "com.apple.afc", &port);
    if (err < 0)
    {
        throw std::runtime_error("Unable to start AFC service");
    }
    lockdown_close(lockdown);

    afc = afc_open(device, port);
    if (afc == NULL)
    {
        throw std::runtime_error("Unable to open connection to AFC service");
    }
}

void AFC::open(std::shared_ptr<util::Device> device, uint16_t port)
{
    afc_error_t err = AFC_E_SUCCESS;
    afc_t* afc = afc_create();
    if (afc != NULL)
    {
        err = afc_client_new(device->client, port, &_client);
        if (err != AFC_E_SUCCESS)
        {
            afc_free(afc);
            throw std::runtime_error("Unable to create new MobileBackup2 client");
        }
        _device = device;
        _port = port;
    }
    return afc;
}

int AFC::close()
{
    // TODO: Implement Me
    return -1;
}

// ghetto i know, not sure where/how to put a global function for this
static void afc_free_dictionary(char** dictionary)
{
    int i = 0;

    if (!dictionary)
        return;

    for (i = 0; dictionary[i]; i++)
    {
        free(dictionary[i]);
    }
    free(dictionary);
}

void AFC::apparition_afc_get_file_contents(const char* filename, char** data, uint64_t* size)
{
    if (!data || !size)
    {
        return;
    }

    char** fileinfo = NULL;
    uint32_t fsize = 0;

    afc_get_file_info(_client, filename, &fileinfo);
    if (!fileinfo)
    {
        return;
    }
    int i;
    for (i = 0; fileinfo[i]; i += 2)
    {
        if (!strcmp(fileinfo[i], "st_size"))
        {
            fsize = atol(fileinfo[i + 1]);
            break;
        }
    }
    afc_free_dictionary(fileinfo);

    if (fsize == 0)
    {
        return;
    }

    uint64_t f = 0;
    afc_file_open(_client, filename, AFC_FOPEN_RDONLY, &f);
    if (!f)
    {
        return;
    }
    char* buf = (char*)malloc((uint32_t)fsize);
    uint32_t done = 0;
    while (done < fsize)
    {
        uint32_t bread = 0;
        afc_file_read(_client, f, buf + done, 65536, &bread);
        if (bread > 0)
        {
        }
        else
        {
            break;
        }
        done += bread;
    }
    if (done == fsize)
    {
        *size = fsize;
        *data = buf;
    }
    else
    {
        free(buf);
    }
    afc_file_close(_client, f);
}

int afc_send_file(afc_t* afc, const char* local, const char* remote)
{

    uint64_t lockfile = 0;
    uint64_t my_file = 0;
    unsigned int bytes = 0;

    afc_file_open(_client, remote, AFC_FOPEN_WR, &my_file);
    if (my_file)
    {
        // char *outdatafile = strdup("THIS IS HOW WE DO IT, WHORE");

        // FIXME: right here its just sending "local/file.txt, rather than the contents of the file.

        // afc_file_write(afc->client, my_file, outdatafile, strlen(outdatafile), &bytes);// <-- old
        // code
        afc_file_write(_client, my_file, local, strlen(local), &bytes);
        // free(outdatafile);
        if (bytes > 0)
            printf("File transferred successfully");
        else
            printf("File write failed!!! :(");
        afc_file_close(_client, my_file);
    }

    printf("afc all done.");
}
