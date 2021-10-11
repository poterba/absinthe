/**
 * GreenPois0n Absinthe - lockdown.c
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
#include "lockdown.hpp"

#include "common.hpp"
#include "debug.hpp"
#include "device.hpp"

#include <libimobiledevice/libimobiledevice.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

namespace absinthe {
namespace util {

Lockdown::Lockdown(device_t* device) : _device(device)
{
    if (lockdownd_client_new_with_handshake(device->client, &_client, "absinthe") !=
        LOCKDOWN_E_SUCCESS) {
        throw std::runtime_error("Unable to pair with lockdownd");
        throw;
    }
}

int Lockdown::get_value(const char* domain, const char* key, plist_t* value)
{
    if (!lockdown || !lockdown->client) {
        return -1;
    }

    lockdownd_error_t err = lockdownd_get_value(lockdown->client, domain, key, value);
    if (err == LOCKDOWN_E_SUCCESS) {
        return 0;
    } else {
        return -1;
    }
}

int Lockdown::get_string(const char* key, char** value)
{
    if (!lockdown || !lockdown->client) {
        return -1;
    }

    char* str = NULL;
    plist_t pl = NULL;
    lockdownd_error_t err = lockdownd_get_value(lockdown->client, NULL, key, &pl);
    if (err == LOCKDOWN_E_SUCCESS) {
        if (pl != NULL && plist_get_node_type(pl) == PLIST_STRING) {
            plist_get_string_val(pl, &str);
            plist_free(pl);
            if (str != NULL) {
                *value = str;
                return 0;
            }
        }
    }
    if (pl) {
        plist_free(pl);
    }
    return -1;
}

int Lockdown::start_service(const char* service, uint16_t* port)
{
    return lockdown_start_service2(lockdown, service, port, 1);
}

int Lockdown::start_service2(const char* service, uint16_t* port, int warn_on_fail)
{
    uint16_t p = 0;
    lockdownd_start_service(lockdown->client, service, &p);

    if (p == 0) {
        if (warn_on_fail)
            throw std::runtime_error("%s failed to start!", service);
        return -1;
    }

    debug("Started %s successfully on port %d!", service, p);
    *port = p;
    return 0;
}

int Lockdown::stop_service(const char* service)
{
    // TODO: Implement Me
    return -1;
}

int Lockdown::close(lockdown_t* lockdown)
{
    lockdownd_client_free(lockdown->client);
    lockdown->client = NULL;
    return 0;
}

void Lockdown::free(lockdown_t* lockdown)
{
    if (lockdown) {
        if (lockdown->client) {
            lockdown_close(lockdown);
        }
        free(lockdown);
    }
}

} // namespace util
} // namespace absinthe
