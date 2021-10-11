/**
 * GreenPois0n Absinthe - crashreportmover.h
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

namespace absinthe
{
namespace util
{
class Device;
} // namespace util
namespace crashreport
{

class Mover final
{
public:
    Mover(std::shared_ptr<util::Device> device);
    Mover(std::shared_ptr<util::Device> device, uint16_t port);
    int close();

private:
    uint16_t port;
    std::shared_ptr<util::Device> device;
    idevice_connection_t connection;
};

} // namespace crashreport
} // namespace absinthe
