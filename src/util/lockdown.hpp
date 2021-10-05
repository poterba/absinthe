/**
 * GreenPois0n Absinthe - lockdown.h
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

#include "device.hpp"

#include <libimobiledevice/lockdown.h>

namespace absinthe {
namespace util {
	
class Lockdown
{
public:
	Lockdown( device_t* device );

	int get_value(const char *domain, const char *key, plist_t *value);
	int get_string(const char *key, char** value);
	int start_service(const char* service, uint16_t* port);
	int start_service2(const char* service, uint16_t* port, int warn_on_fail);
	int stop_service(const char* service);

	int close();
	void free();

private:
	device_t *device;
	lockdownd_client_t client;
};

} // namespace util
} // namespace absinthe
