/**
 * GreenPois0n Absinthe - absinthe.c
 * Copyright (C) 2010-2012 Chronic-Dev Team
 * Copyright (C) 2010-2012 Joshua Hill
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

#include "backup.hpp"
#include "crashreporter.hpp"
#include "device.hpp"
#include "dictionary.hpp"
#include "dyldcache.hpp"
#include "idevicebackup2.hpp"
#include "jailbreak.hpp"
#include "mb1.hpp"
#include "rop.hpp"
#include "version.h"

#include <cxxopts.hpp>
#include <libimobiledevice/sbservices.h>
#include <plist/plist.h>

#include <cxxopts.hpp>
#include <dirent.h>
#include <getopt.h>
#include <iostream>
#include <memory>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <unistd.h>
/////////////////////////////////////////////////////////////////////////////////////////
/// TODO: We need to add an event handler for when devices are connected. This handler needs to wait
/// for iTunes to autostart and kill it before it can start the syncing process and mess up our
/// connection.
/////////////////////////////////////////////////////////////////////////////////////////

namespace absinthe {
#if 0
unsigned long find_aslr_slide(crashreport_t* crash, char* cache)
{
    unsigned long slide = 0;
    if (crash == NULL || cache == NULL)
    {
        throw std::runtime_error("Invalid arguments");
        return 0;
    }

    dyld::cache::dyldcache_t* dyldCache = dyld::cache::open(cache);
    if (dyldCache != NULL)
    {
        dyld::cache::free(dyldCache);
    }
    return slide;
}
#endif // 0

void idevice_event_cb(const idevice_event_t* event, void* user_data)
{
    // char* udid = (char*)user_data;
    // printf("device event %d: %s", event->event, event->udid);
    // if (udid && strcmp(udid, event->udid)) return;
    // if (event->event == IDEVICE_DEVICE_ADD) {
    //     connected = 1;
    // } else if (event->event == IDEVICE_DEVICE_REMOVE) {
    //     connected = 0;
    // }
    absinthe::core::jb_device_event_cb(event, user_data);
}

void signal_handler(int sig) { absinthe::core::jb_signal_handler(sig); }

void status_cb(const char* msg, int progress)
{
    static std::string lastmsg{};
    if (!msg) {
        msg = lastmsg.data();
    } else {
        lastmsg = msg;
    }
    printf("[%d%%] %s", progress, msg);
}

} // namespace absinthe

int main(int argc, char* argv[])
{
    std::shared_ptr<absinthe::util::Device> device;

    char* name = strrchr(argv[0], '/');
    if (name) {
        int nlen = strlen(argv[0]) - strlen(name);
        char path[512];
        memcpy(path, argv[0], nlen);
        path[nlen] = 0;
        std::cout << "setting working directory to " << path << std::endl;
        if (chdir(path) != 0) {
            std::cerr << "unable to set working directory" << std::endl;
        }
    }

    cxxopts::Options options("absinthe", "Jailbreak iOS5.0 using ub3rl33t MobileBackup2 exploit.");
    // clang-format off
    options.add_options()
        ("u,udid", "target specific device by its 40-digit device UDID", cxxopts::value<std::string>()->default_value("UDID"))
        ("t,target", "offset to ROP gadget we want to execute", cxxopts::value<unsigned long>()->default_value(0))
        ("p,pointer", "heap address we're hoping contains our target", cxxopts::value<unsigned long>()->default_value(0))
        ("a,aslr-slide", "value of randomized dyldcache slide", cxxopts::value<unsigned long>()->default_value(0))
        ("v,verbose", "prints debuging info while running", cxxopts::value<bool>()->default_value("false"))
    ;
    // clang-format on

    auto result = options.parse(argc, argv);

    // we need to exit cleanly on running backups and restores or we cause havok
    signal(SIGINT, absinthe::signal_handler);
    signal(SIGTERM, absinthe::signal_handler);
#ifndef WIN32
    signal(SIGQUIT, absinthe::signal_handler);
    signal(SIGPIPE, SIG_IGN);
#endif

    std::string udid = result["udid"].as<std::string>();
    // device detection
    if (udid.empty()) {
        device = std::make_shared<absinthe::util::Device>(std::string{});
        udid = device->udid();
    } else {
        // Open a connection to our device
        std::cout << "Detecting device..." << std::endl;
        device = std::make_shared<absinthe::util::Device>(udid);
    }

    auto lockdown = std::make_unique<absinthe::util::Lockdown>(device);
    char *product, *build;
    if ((lockdown->get_string("ProductType", &product) != LOCKDOWN_E_SUCCESS) ||
        (lockdown->get_string("BuildVersion", &build) != LOCKDOWN_E_SUCCESS)) {
        std::cerr << "Could not get device information" << std::endl;
        return -1;
    }

    if (!absinthe::core::jb_device_is_supported(product, build)) {
        std::cerr << "Error: device " << product << " build " << build << " is not supported."
                  << std::endl;
        return -1;
    }

    int cc = absinthe::core::jb_check_consistency(product, build);
    if (cc == 0) {
        std::cout << "Consistency check passed" << std::endl;
    } else if (cc == -1) {
        std::cerr << "ERROR: Consistency check failed: device " << product << " build " << build
                  << " not supported" << std::endl;
        return -1;
    } else if (cc == -2) {
        std::cerr << "ERROR: Consistency check failed: could not find required files for device "
                  << product << " build " << build << std::endl;
        return -1;
    } else {
        std::cerr << "ERROR: Consistency check failed: unknown error" << std::endl;
        return -1;
    }

    plist_t pl = NULL;
    lockdown->get_value(NULL, "ActivationState", &pl);
    if (pl && plist_get_node_type(pl) == PLIST_STRING) {
        char* as = NULL;
        plist_get_string_val(pl, &as);
        plist_free(pl);
        if (as) {
            if (strcmp(as, "Unactivated") == 0) {
                free(as);
                std::cerr << "Error: The attached device is not activated. You need to "
                             "activate it "
                             "before "
                             "it can be used with Absinthe."
                          << std::endl;
                return -1;
            }
            free(as);
        }
    }

    pl = NULL;
    lockdown->get_value("com.apple.mobile.backup", "WillEncrypt", &pl);
    if (pl && plist_get_node_type(pl) == PLIST_BOOLEAN) {
        unsigned char c = 0;
        plist_get_bool_val(pl, &c);
        plist_free(pl);
        if (c) {
            std::cerr << "Error: You have a device backup password set. You need to "
                         "disable the backup password in iTunes."
                      << std::endl;
            return -1;
        }
    }

    // begin the process
    idevice_event_subscribe(absinthe::idevice_event_cb, udid.data());
    absinthe::core::jailbreak(udid.data(), absinthe::status_cb);
    idevice_event_unsubscribe();

    return 0;
}
