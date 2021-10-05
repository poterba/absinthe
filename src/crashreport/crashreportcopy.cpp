/**
 * GreenPois0n Absinthe - crashreportcopy.c
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
#include "crashreportcopy.hpp"

#include "debug.hpp"
#include "debug.hpp"
#include "lockdown.hpp"

#include <libimobiledevice/libimobiledevice.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <memory>

namespace absinthe {
namespace crash_report {

Copy::Copy( device_t* device )
: _device(device)
{
	const auto& lockdown = std::make_shared< util::Lockdown >(device);

	int err = lockdown->start_service("com.apple.crashreportcopymobile", &_port);
	if(err < 0)
	{
		error("Unable to start AFC service\n");
	}
	lockdown->close();
	lockdown->free();

	open(device, _port);
}

void Copy::open(device_t* device, uint16_t port)
{
	int err = afc_client_new(device->client, port, &_client);
	if(err < 0)
	{
		error("Unable to open connection to CrashReporter copy service\n");
	}
}

int Copy::close()
{
	afc_client_free(_client);
	_client = NULL;
	return 0;
}

void Copy::free()
{
	if(_client)
	{
		close();
	}
}

} // namespace crash_report
} // namespace absinthe
