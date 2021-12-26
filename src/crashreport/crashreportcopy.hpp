/**
 * GreenPois0n Absinthe - crashreportcopy.h
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

#include <libimobiledevice/afc.h>

#include "lockdown.hpp"

namespace absinthe {
namespace util {
class Device;
} // namespace util
namespace crashreport {

class Copy final
{
  public:
    Copy(std::shared_ptr<util::Device> device);
    Copy(std::shared_ptr<util::Device> device, uint16_t port);
    ~Copy();

    const afc_client_t& client() const;

  private:
    uint16_t _port;
    std::shared_ptr<util::Device> _device;
    afc_client_t _client;
    idevice_connection_t connection;
};

} // namespace crashreport
} // namespace absinthe