#pragma once

#if 0

typedef unsigned int __32bit_ptr;

struct vn_ioctl {
    __32bit_ptr vn_file;        /* pathname of file to mount */
    int         vn_size;        /* (returned) size of disk */
    int         vn_control;     /* this is vncontrol_t, but we need to ensure 32 bit size */
};

#define VNIOCATTACH _IOWR('F', 0, struct vn_ioctl) /* attach file */
#define VNIOCDETACH _IOWR('F', 1, struct vn_ioctl)

#endif
