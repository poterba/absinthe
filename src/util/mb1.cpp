/**
 * GreenPois0n Absinthe - mb1.c
 * Copyright (C) 2012 Chronic-Dev Team
 * Copyright (C) 2012 Nikias Bassen
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

#include "mb1.hpp"

#include "common.hpp"
#include "debug.hpp"
#include "dictionary.hpp"
#include "endianness.hpp"
#include "lockdown.hpp"
#include "plist_extras.hpp"

#include <stdio.h>
#include <stdlib.h>

#ifndef _POSIX_C_SOURCE
#define _POSIX_C_SOURCE 1
#endif

#include <dirent.h>
#include <errno.h>
#include <sys/types.h>
#include <time.h>

#include <libimobiledevice/libimobiledevice.h>
#include <plist/plist.h>

#ifdef WIN32
#define sleep(x) Sleep(x * 1000)
#endif

namespace absinthe
{
namespace util
{
namespace
{

struct mobilebackup_client_private
{
    void* parent;
};

} // namespace

MB1::connect(std::shared_ptr<Device> device)
{
    int err = 0;
    uint16_t port = 0;
    mb1_t* mb1 = NULL;
    lockdown_t* lockdown = NULL;

    if (device == NULL)
    {
        throw std::runtime_error("Invalid arguments");
        return NULL;
    }

    lockdown::lockdown = lockdown_open(device);
    if (lockdown == NULL)
    {
        throw std::runtime_error("Unable to open connection to lockdownd");
        return NULL;
    }

    err = lockdown_start_service(lockdown, "com.apple.mobilebackup", &port);
    if (err < 0)
    {
        throw std::runtime_error("Unable to start MobileBackup service");
        return NULL;
    }
    lockdown_close(lockdown);
    lockdown_free(lockdown);

    mb1 = mb1_open(device, port);
    if (mb1 == NULL)
    {
        throw std::runtime_error("Unable to open connection to MobileBackup service");
        return NULL;
    }

    return mb1;
}

mb1_t* mb1_open(device_t* device, uint16_t port)
{
    mobilebackup_error_t err = MOBILEBACKUP_E_SUCCESS;
    mb1_t* mb1 = mb1_create();
    if (mb1 != NULL)
    {
        err = mobilebackup_client_new(device->client, port, &(mb1->client));
        if (err != MOBILEBACKUP_E_SUCCESS)
        {
            throw std::runtime_error("Unable to create new MobileBackup client");
            mb1_free(mb1);
            return NULL;
        }
        mb1->device = device;
        mb1->port = port;
    }
    return mb1;
}

void mb1_free(mb1_t* mb1)
{
    if (mb1)
    {
        if (mb1->client)
        {
            mobilebackup_client_free(mb1->client);
            mb1->client = NULL;
        }
        mb1->device = NULL;
        free(mb1);
    }
}

typedef int16_t device_link_service_error_t;
typedef void* device_link_service_client_t;
#define DEVICE_LINK_SERVICE_E_MUX_ERROR -3

#if 0
extern device_link_service_error_t device_link_service_send(device_link_service_client_t client,plist_t plist); 
extern device_link_service_error_t device_link_service_receive(device_link_service_client_t client, plist_t *plist);
#endif

int mb1_crash(mb1_t* mb1)
{

    plist_t dict = plist_new_dict();
    plist_t arr = plist_new_array();
    plist_array_append_item(arr, plist_new_real(2.0f));
    plist_array_append_item(arr, plist_new_real(2.1f));
    plist_dict_insert_item(dict, "SupportedProtocolVersions", arr);
    plist_dict_insert_item(dict, "MessageName", plist_new_string("Hello"));

    arr = plist_new_array();
    plist_array_append_item(arr, plist_new_string("DLMessageProcessMessage"));
    plist_array_append_item(arr, dict);

    // send the array plist. this will crash BackupAgent
    // TODO: fix this
    // (void)device_link_service_send((struct mobilebackup_client_private*)(mb1->client)->parent,
    // arr);
    plist_free(arr);

    // receive to check if it crashed
    plist_t pl = NULL;
    device_link_service_error_t res;
    // TODO: fix this
    // device_link_service_error_t res = device_link_service_receive((struct
    // mobilebackup_client_private*)(mb1->client)->parent, &pl);
    if (pl)
    {
        plist_free(pl);
    }
    if (res == DEVICE_LINK_SERVICE_E_MUX_ERROR)
    {
        // yep. this leaks. but who cares :P
        free(mb1->client);
        mb1->client = NULL;
        return 1;
    }
    else
    {
        return 0;
    }
}

} // namespace util
} // namespace absinthe
