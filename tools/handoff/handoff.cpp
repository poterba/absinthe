#include "debug.hpp"
#include "device.hpp"
#include "libimobiledevice/house_arrest.hpp"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char* argv[])
{
    debug("Opening device");
    device_t* device = device_create(NULL);
    if (device != NULL) {
        debug("Closing device");
        device_free(device);
    } else {
        throw std::runtime_error("Unable to open device");
    }

    return 0;
}
