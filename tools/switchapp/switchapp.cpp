/*
 * appswitch.c
 *
 *  Created on: Jan 20, 2013
 *      Author: posixninja
 */

#include "backup.hpp"
#include "backup_file.hpp"
#include "common.hpp"
#include "debug.hpp"
#include "device.hpp"
#include <stdio.h>

#define AFCTMP "HackStore"

int main(int argc, char* argv[])
{
    debug("Preparing to open device");
    device_t* device = device_create(NULL);
    if (device == NULL)
    {
        throw std::runtime_error("Unable to open device");
        return -1;
    }
    debug("Opened device %s", device->udid);
    // device_enable_debug();

    debug("Preparing to create device backup");
    char* bargv[] = {"idevicebackup2", "backup", ".", NULL};
    idevicebackup2(3, bargv);
    debug("Create device backup");

    backup_t* backup = backup_open(".", device->udid);
    if (!backup)
    {
        throw std::runtime_error("Unable to open backup");
        device_free(device);
        return -1;
    }

    /*
    unsigned int tm = (unsigned int) (time(NULL ));
    debug("Adding new file to device");
    backup_file_t* bf = backup_file_create(NULL);
    if (bf) {
            backup_file_set_domain(bf, "BooksDomain");
            backup_file_set_path(bf, "blah");
            backup_file_set_target(bf, "blah");
            backup_file_set_mode(bf, 0120644);
            backup_file_set_inode(bf, 54327);
            backup_file_set_uid(bf, 0);
            backup_file_set_gid(bf, 0);
            backup_file_set_time1(bf, tm);
            backup_file_set_time2(bf, tm);
            backup_file_set_time3(bf, tm);
            backup_file_set_flag(bf, 0);

            if (backup_update_file(backup, bf) < 0) {
                    fprintf(stderr, "ERROR: could not add file to backup");
            }
            backup_file_free(bf);
    }

    backup_write_mbdb(backup);
    backup_free(backup);
    */

    debug("Preparing to restore device backup");
    char* rargv[] = {"idevicebackup2", "restore", "--system", "--settings", ".", NULL};
    idevicebackup2(6, rargv);
    debug("Restored device backup");

    debug("Closing device");
    device_free(device);
    return 0;
}
