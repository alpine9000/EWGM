#ifndef __DOS_H
#define __DOS_H

#define MODE_NEWFILE     1006
#define MODE_OLDFILE     1005 

extern struct ExecBase *SysBase;
extern struct Library *DOSBase;

typedef uint32_t BPTR;

__stdargs struct Library *
OpenLibrary(const char* library, uint32_t version);                          

__stdargs void 
CloseLibrary(struct Library* library);

__stdargs BPTR 
Open(const char* file, int32_t mode);

__stdargs void 
Close(BPTR);

__stdargs int32_t 
Write(BPTR file, void* buffer, int32_t length);

__stdargs int32_t
Read(BPTR file, void* buffer, int32_t length);

BPTR CurrentDir(BPTR);

void
dos_init(void);

#endif
