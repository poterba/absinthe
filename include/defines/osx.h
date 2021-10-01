
#pragma once

#define	O_RDONLY	0x0000		/* open for reading only */
#define	O_RDWR		0x0002		/* open for reading and writing */
#define	O_CREAT		0x0200		/* create if nonexistant */

#define	S_IRWXU		0000700		/* [XSI] RWX mask for owner */
#define	S_IROTH		0000004		/* [XSI] R for other */
#define	S_IXOTH		0000001		/* [XSI] X for other */

#define	PROT_READ	0x01	/* [MC2] pages can be read */
#define	PROT_WRITE	0x02	/* [MC2] pages can be written */

#define	MAP_SHARED	0x0001		/* [MF|SHM] share changes */

#define _IOC(inout,group,num,len) \
        (inout | ((len & IOCPARM_MASK) << 16) | ((group) << 8) | (num))

#define IOC_OUT         (__uint32_t)0x40000000
#define IOC_IN          (__uint32_t)0x80000000
#define IOC_INOUT       (IOC_IN|IOC_OUT)

#define _IOWR(g,n,t)    _IOC(IOC_INOUT, (g), (n), sizeof(t))

#define PT_CONTINUE 7 /* continue the child */
#define PT_ATTACH 10  /* trace some running process */
#define PT_DETACH 11  /* stop tracing a process */

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
