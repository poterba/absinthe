#pragma once

#if __APPLE__
#include "defines/osx.h"

#elif __linux__
#include "defines/linux.h"

#elif _WIN32
#include <pthread.h>
typedef int uid_t;
typedef int gid_t;
typedef uint32_t __uint32_t;
#endif

typedef unsigned int __32bit_ptr;

#define LOG_WARNING 4

typedef enum
{
	vncontrol_readwrite_io_e = 0
} vncontrol_t;

struct vn_ioctl {
    __32bit_ptr vn_file;        /* pathname of file to mount */
    int         vn_size;        /* (returned) size of disk */
    int         vn_control;     /* this is vncontrol_t, but we need to ensure 32 bit size */
};

#define VNIOCATTACH	_IOWR('F', 0, struct vn_ioctl)	/* attach file */
#define VNIOCDETACH _IOWR('F', 1, struct vn_ioctl)

#define CTL_KERN 1		/* "high kernel": proc, limits */
#define KERN_PROC 14	/* struct: process entries */
#define KERN_PROC_ALL 0 /* everything */

#define PROC_PIDREGIONPATHINFO 8
#define SM_SHARED 4

#define MNT_RDONLY 0x00000001 /* read only filesystem */
#define MNT_UPDATE 0x00010000 /* not a real mount, just an update */

#define RTLD_DEFAULT ((__32bit_ptr)-2) /* Use default search algorithm. */

struct __darwin_timezone
{
	int tz_minuteswest; /* minutes west of Greenwich */
	int tz_dsttime;		/* type of dst correction */
};

struct hfs_mount_args
{
	__32bit_ptr fspec;
	uid_t hfs_uid;
	gid_t hfs_gid;
	__darwin_mode_t hfs_mask;
	uint32_t hfs_encoding;
	struct __darwin_timezone hfs_timezone;
	int flags;
	int journal_tbuffer_size;
	int journal_flags;
	int journal_disable;
};
