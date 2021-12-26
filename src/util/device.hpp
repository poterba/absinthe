/**
 * GreenPois0n Absinthe - device.h
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

#include <libimobiledevice/libimobiledevice.h>

#include <string>

namespace absinthe {
namespace util {

class Device final
{
  public:
    explicit Device(const std::string& udid);
    ~Device();

    void enable_debug();
    const idevice_t& client() const;

    const std::string& udid() const;

  private:
    idevice_t _client;
    std::string _udid;
    std::string _ecid;
};

} // namespace util
} // namespace absinthe
