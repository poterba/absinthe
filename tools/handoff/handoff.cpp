#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "debug.hpp"
#include "device.hpp"
#include "libimobiledevice/house_arrest.hpp"

int main(int argc, char* argv[]) {
	debug("Opening device\n");
	device_t* device = device_create(NULL);
	if(device != NULL) {


		debug("Closing device\n");
		device_free(device);

	} else {
		error("Unable to open device\n");
	}

	return 0;
}
