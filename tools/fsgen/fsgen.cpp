/*
 * fsgen.c
 *
 *  Created on: Jan 12, 2012
 *      Author: posixninja
 */

#include "rop.hpp"
#include <stdio.h>

int main(int argc, char* argv[])
{
    int dscs = 0;
    if (argc < 2) {
        fprintf(stderr, "syntax: %s <dyld shared cache slide, ex: 0x40000>", argv[0]);
        exit(1);
    }

    sscanf(argv[1], "0x%x", &dscs);
    fprintf(stderr, "dyld shared cache slide: 0x%x", dscs);

    // This is fucking crazy, we really need to split this up into function
    //  sized blocks, (ropPtrace, ropSyslog, ropEtc..),  and we need some
    //  for doing loops!
    // ~posixninja
    return ropMain(dscs);
}
