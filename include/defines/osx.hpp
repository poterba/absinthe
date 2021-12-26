
#pragma once

#include <sys/ioctl.h>
#include <sys/ptrace.h>

#define O_RDONLY 0x0000 /* open for reading only */
#define O_RDWR 0x0002   /* open for reading and writing */
// #define O_CREAT 0x0200  /* create if nonexistant */

#define S_IRWXU 0000700 /* [XSI] RWX mask for owner */
#define S_IROTH 0000004 /* [XSI] R for other */
#define S_IXOTH 0000001 /* [XSI] X for other */

#define PROT_READ 0x01  /* [MC2] pages can be read */
#define PROT_WRITE 0x02 /* [MC2] pages can be written */

#define MAP_SHARED 0x0001 /* [MF|SHM] share changes */

#define _IOC(inout, group, num, len) (inout | ((len & IOCPARM_MASK) << 16) | ((group) << 8) | (num))

// #define IOC_OUT (__uint32_t)0x40000000
// #define IOC_IN (__uint32_t)0x80000000
// #define IOC_INOUT (IOC_IN | IOC_OUT)

#define _IOWR(g, n, t) _IOC(IOC_INOUT, (g), (n), sizeof(t))

#define PT_CONTINUE 7 /* continue the child */
// #define PT_ATTACH 10  /* trace some running process */
#define PT_DETACH 11 /* stop tracing a process */