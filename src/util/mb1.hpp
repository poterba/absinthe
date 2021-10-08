/**
 * GreenPois0n Absinthe - mb1.h
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

#pragma once

#include "device.hpp"

#include <libimobiledevice/mobilebackup.h>

#include <memory>

namespace absinthe
{
namespace util
{

class MB1 final
{
public:
    MB1() = default;
    ~MB1();

    void connect(std::shared_ptr<Device> device);
    void open(uint16_t port);
    int crash();

private:
    uint16_t _port{0};
    std::shared_ptr<Device> _device;
    mobilebackup_client_t _client;
};

} // namespace util
} // namespace absinthe
