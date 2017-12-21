#pragma once
#include "game.h" // for irony

#if TRACKLOADER==0

#include <dos/dos.h>
#include <exec/io.h>
#include <exec/memory.h>

extern struct ExecBase *SysBase;
extern struct Library *DOSBase;

void
amigaos_ctor(void);

void
amigaos_dtor(void);

struct Library *
amigaos_OpenLibrary(__REG("a6", void *), __REG("a1", const char* libName), __REG("d0", ULONG version));

VOID
amigaos_CloseLibrary(__REG("a6", void *), __REG("a1", struct Library * library));

BPTR
amigaos_Open(__REG("a6",  void *), __REG("d1", const char* name), __REG("d2", int32_t accessMode));

int32_t
amigaos_Close(__REG("a6", void *), __REG("d1", BPTR file));

LONG
amigaos_Write(__REG("a6", void *), __REG("d1", BPTR file), __REG("d2", CONST APTR buffer), __REG("d3", LONG length));

LONG
amigaos_Read(__REG("a6", void *), __REG("d1", BPTR file), __REG("d2", APTR buffer), __REG("d3", LONG length));

VOID
amigaos_Delay(__REG("a6", void *), __REG("d1", LONG timeout));

BPTR
amigaos_CurrentDir(__REG("a6", void *), __REG("d1", BPTR lock));

BYTE
amigaos_OpenDevice(__REG("a6", void *), __REG("a0", const char* devName), __REG("d0", ULONG unit), __REG("a1", struct IORequest * ioRequest), __REG("d1", ULONG flags));

BYTE
amigaos_AllocSignal(__REG("a6", void *), __REG("d0", LONG signalNum));

APTR
amigaos_AllocMem(__REG("a6", void *), __REG("d0", ULONG byteSize), __REG("d1", ULONG requirements));

VOID
amigaos_FreeSignal(__REG("a6", void *), __REG("d0", LONG signalNum));

VOID
amigaos_AddPort(__REG("a6", void *), __REG("a1", struct MsgPort * port));

struct Task *
amigaos_FindTask(__REG("a6", void *), __REG("a1", CONST_STRPTR name));

VOID
amigaos_RemPort(__REG("a6", void *), __REG("a1", struct MsgPort * port));

VOID
amigaos_FreeMem(__REG("a6", void *), __REG("a1", APTR memoryBlock), __REG("d0", ULONG byteSize));

VOID
amigaos_AbortIO(__REG("a6", void *), __REG("a1", struct IORequest * ioRequest));

BYTE
amigaos_WaitIO(__REG("a6", void *), __REG("a1", struct IORequest * ioRequest));

BYTE
amigaos_DoIO(__REG("a6", void *), __REG("a1", struct IORequest * ioRequest));

VOID
amigaos_SendIO(__REG("a6", void *), __REG("a1", struct IORequest * ioRequest));

#define SendIO(ioRequest) amigaos_SendIO(SysBase, (ioRequest))
#define DoIO(ioRequest) amigaos_DoIO(SysBase, (ioRequest))
#define WaitIO(ioRequest) amigaos_WaitIO(SysBase, (ioRequest))
#define AbortIO(ioRequest) amigaos_AbortIO(SysBase, (ioRequest))
#define FreeMem(memoryBlock, byteSize) amigaos_FreeMem(SysBase, (memoryBlock), (byteSize))
#define RemPort(port) amigaos_RemPort(SysBase, (port))
#define FindTask(name) amigaos_FindTask(SysBase, (name))
#define AddPort(port) amigaos_AddPort(SysBase, (port))
#define FreeSignal(signalNum) amigaos_FreeSignal(SysBase, (signalNum))
#define AllocMem(byteSize, requirements) amigaos_AllocMem(SysBase, (byteSize), (requirements))
#define AllocSignal(signalNum) amigaos_AllocSignal(SysBase, (signalNum))
#define CurrentDir(lock) amigaos_CurrentDir(DOSBase, (lock))
#define Delay(timeout) amigaos_Delay(DOSBase, (timeout))
#define Read(file, buffer, length) amigaos_Read(DOSBase, (file), (buffer), (length))
#define Write(file, buffer, length) amigaos_Write(DOSBase, (file), (buffer), (length))
#define Open(name, accessMode) amigaos_Open(DOSBase, (name), (accessMode))
#define Close(file) amigaos_Close(DOSBase, (file))
#define OpenLibrary(libName, version) amigaos_OpenLibrary(SysBase, (libName), (version))
#define CloseLibrary(library) amigaos_CloseLibrary(SysBase, (library))
#define OpenDevice(devName, unit, ioRequest, flags) amigaos_OpenDevice(SysBase, (devName), (unit), (ioRequest), (flags))

struct MsgPort *
CreatePort(CONST_STRPTR name,LONG pri);

struct IORequest *
CreateExtIO(CONST struct MsgPort *port,LONG iosize);

struct IOStdReq *
CreateStdIO(CONST struct MsgPort *port);

VOID
DeleteExtIO(struct IORequest *ioreq);

VOID
DeletePort(struct MsgPort *port);

#endif
