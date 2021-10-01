/*
 * rop.h
 *
 *  Created on: Jan 12, 2012
 *      Author: posixninja
 */

#ifndef ROP_H_
#define ROP_H_

#if __APPLE__
#include <sys/sysctl.h>
#include <servers/bootstrap.h>
#include <mach/mach.h>
#include <libproc.h>
#include "mach/message.h"
#else
#include "fsgen-global-constants.h"
#include "fsgen-mac-defines.h"
#include "defines/linux.h"
#endif

#define ROP_LOAD_REG4_CONST_LEN 0xc
#define ROP_SAVE_REG0_LEN (ROP_LOAD_REG4_CONST_LEN + 0xc)

#define ROP_LOAD_REG0_CONST_LEN 0x8
#define ROP_DEREF_REG0_LEN 0x8
#define ROP_LOAD_REG0_LEN (ROP_LOAD_REG0_CONST_LEN + ROP_DEREF_REG0_LEN)

typedef unsigned int Addr;
extern unsigned int dscs;
extern FILE* ropFile;
extern FILE* ropVarsFile;
extern unsigned int ropWriteMode;
extern unsigned int ropWriteVarsMode;
extern unsigned int ropWriteAddr;
extern unsigned int ropFileAddr;
extern unsigned char vars[VARS_MAX_SIZE];
extern Addr varsBaseAddr;
extern unsigned int varsWritten;

struct trojan_msg {
    mach_msg_header_t          header;
    uint32_t                   r8;
    uint32_t                   r10;
    uint32_t                   r4;
    uint32_t                   r5;
    uint32_t                   r6;
    uint32_t                   r7;
    uint32_t                   pc;
    uint32_t                   crap[7];
};

int ropMain(int dscs);

void ropOpen();
void ropClose();
// unsigned int dscs;
void ropStackPivotLo(Addr toAddr);
void ropLog(char* msg);
void ropCall4Reg(Addr addr);

typedef unsigned int RopArg;

void ropCall7(Addr, RopArg, RopArg, RopArg, RopArg, RopArg, RopArg, RopArg );
void ropCall6(Addr, RopArg, RopArg, RopArg, RopArg, RopArg, RopArg );
void ropCall5(Addr, RopArg, RopArg, RopArg, RopArg, RopArg );
void ropCall4(Addr, RopArg, RopArg, RopArg, RopArg);
void ropCall3(Addr, RopArg, RopArg, RopArg);
void ropCall2(Addr, RopArg, RopArg);
void ropCall1(Addr, RopArg);
void ropCall0(Addr);

void ropMovReg1Reg0();
void ropAddReg0Reg1();

void ropLoadReg0(Addr ptrValue);
void ropSaveReg0(Addr toAddr);
void ropLoadReg0Const(unsigned int value);
void ropAddReg0Const(unsigned int value);
void ropSubReg0Const(unsigned int value);
void ropDerefReg0();

void ropStoreValueAtOffsetFromVariableAddress(Addr address, unsigned int offset, unsigned int value);
void ropStoreVariableValueAtOffsetFromVariableAddress(Addr address, unsigned int offset, Addr value);

Addr newBinary(void* data, size_t size);
Addr newBuffer(unsigned int size);
Addr newString(char* s);
Addr newInteger(unsigned int value);
Addr newArray(unsigned int values[], unsigned int count);

#endif /* ROP_H_ */
