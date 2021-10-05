
#pragma once

// #include <pthread.h>

typedef int uid_t;
typedef int gid_t;
typedef uint32_t __uint32_t;

struct stat {
	__darwin_dev_t		st_dev;			/* [XSI] ID of device containing file */ \
	__darwin_mode_t		st_mode;		/* [XSI] Mode of file (see below) */ \
	__darwin_nlink_t	st_nlink;	/* [XSI] Number of hard links */ \
	uint64_t        st_ino;		/* [XSI] File serial number */ \
	uid_t		st_uid;			/* [XSI] User ID of the file */ \
	gid_t		st_gid;			/* [XSI] Group ID of the file */ \
	__darwin_dev_t		st_rdev;		/* [XSI] Device ID */ \
	struct __darwin_timespec st_atimespec;		/* time of last access */ \
	struct __darwin_timespec st_mtimespec;		/* time of last data modification */ \
	struct __darwin_timespec st_ctimespec;		/* time of last status change */ \
	struct __darwin_timespec st_birthtimespec;	/* time of file creation(birth) */
	__darwin_off_t		st_size;		/* [XSI] file size, in bytes */ \
	__darwin_blkcnt_t	st_blocks;	/* [XSI] blocks allocated for file */ \
	__darwin_blksize_t	st_blksize;	/* [XSI] optimal blocksize for I/O */ \
	uint32_t	st_flags;		/* user defined flags for file */ \
	uint32_t	st_gen;			/* file generation number */ \
	int32_t	        st_lspare;		/* RESERVED: DO NOT USE! */ \
	int64_t	        st_qspare[2];		/* RESERVED: DO NOT USE! */ \
} __attribute__((packed));
