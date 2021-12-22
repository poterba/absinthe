/*
 * bpatch.c
 *
 *  Created on: May 4, 2012
 *      Author: posixninja
 */

#include "bpatch.hpp"
#include <stdio.h>

int main(int argc, char* argv[])
{
    int err = 0;             // Error to return
    bpatch_t* patch = NULL;  // Handle to our patch file
    std::string patch_path;  // Path to the patch file
    std::string target_path; // Path to the target file

    // Check for required arguments
    if (argc == 3) {
        target_path = argv[1];
        patch_path = argv[2];
    } else {
        printf("usage: ./binpatch <target> <patch>");
        return -1;
    }

    // Make file our path strings were cloned correctly
    if (target_path && patch_path) {

        // Open up handle to the patch
        patch = bpatch_open(patch_path);
        if (patch != NULL) {
            // Debugger
            bpatch_debug(patch);

            // Successfully opened path
            //  apply it to our target file
            if (bpatch_apply(patch, target_path) != 0) {
                printf("Failed to patch target");
                err = -1;
            }

            // We don't need this any longer
            bpatch_free(patch);
        } else {
            // Unable to open patch file
            //  is the path correct? is the format correct?
            printf("Unable to open patch file %s", patch_path);
            err = -1;
        }

        // We have no need for these any longer
        free(patch_path);
        free(target_path);
    } else {
        // WTF, we should never be here...
        err = -1;
    }

    return err;
}
