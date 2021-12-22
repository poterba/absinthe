/**
 * GreenPois0n Absinthe - crashreportmover.c
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

#include "crashreportmover.hpp"
#include "debug.hpp"
#include "device.hpp"
#include "lockdown.hpp"
#include <libimobiledevice/libimobiledevice.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

namespace absinthe {
namespace crashreport {

Mover::Mover(std::shared_ptr<util::Device> device, std::optional<uint16_t> port)
{
    if (port) {
        _port = *port;
    } else {
        auto lockdown = std::make_unique<util::Lockdown>(device);

        if (lockdown->start_service("com.apple.crashreportmover", &_port) < 0) {
            throw std::runtime_error("Unable to start crash report mover service");
        }
    }

    int err = idevice_connect(device->client(), _port, &_connection);
    if (err < 0) {
        throw std::exception();
    }
}

Mover::~Mover() { close(); }

int Mover::close()
{
    if (_connection) {
        idevice_disconnect(_connection);
        _connection = NULL;
    }
    return 0;
}

} // namespace crashreport
} // namespace absinthe
