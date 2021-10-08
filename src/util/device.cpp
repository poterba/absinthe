/**
 * GreenPois0n Absinthe - device.c
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

#include "device.hpp"

#include <exception>
#include <stdio.h>
#include <stdlib.h>

#include <libimobiledevice/libimobiledevice.h>

#ifdef WIN32
#include <windows.h>
#define sleep(x) Sleep(x * 1000)
#else
#include <unistd.h>
#define sleep(x) usleep(x * 1000 * 1000);
#endif

namespace absinthe
{
namespace util
{

Device::Device(const std::string& udid) : _udid(udid)
{
    idevice_error_t err = 0;

    if (_udid == NULL)
    {
        err = idevice_new(&_client, NULL);
        if (err != IDEVICE_E_SUCCESS)
        {
            // TODO: add udid to log
            throw std::runtime_error("No device found with udid {}, is it plugged in?");
        }
        idevice_get_udid(_client, (char**)&_udid);
    }
    else
    {
        const auto retries = 5;
        for (auto i = 0; i < retries; ++i)
        {
            err = idevice_new(&_client, _udid.data());
            if (_client)
            {
                break;
            }
            sleep(1);
        }

        if (err != IDEVICE_E_SUCCESS)
        {
            // TODO: add udid to log
            throw std::runtime_error("No device found with udid {}, is it plugged in?", );
        }
    }
}

Device::~Device()
{
    if (_client)
    {
        idevice_free(_client);
    }
}

void Device::enable_debug() { idevice_set_debug_level(3); }

const idevice_t& Device::client() const { return _client; }

const std::string& udid() const { return _udid; }

} // namespace util
} // namespace absinthe
