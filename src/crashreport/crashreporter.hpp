/**
 * GreenPois0n Absinthe - crashreporter.h
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

// #include "afc.hpp"
#include "crashreport.hpp"
#include "crashreportcopy.hpp"
#include "crashreportmover.hpp"
#include <libimobiledevice/afc.h>
#include <libimobiledevice/libimobiledevice.h>
#include <plist/plist.h>

namespace absinthe
{
namespace crashreport
{

class Reporter final
{
public:
    Reporter(std::shared_ptr<util::Device> device);
    Reporter(std::shared_ptr<util::Device> device, uint16_t port);
    std::unique_ptr<Report> last_crash();

private:
    // std::shared_ptr< AFC > _afc;
    uint16_t _port;
    std::shared_ptr<util::Device> _device;
    std::shared_ptr<Copy> _copier;
    std::shared_ptr<Mover> _mover;
};

} // namespace crashreport
} // namespace absinthe
