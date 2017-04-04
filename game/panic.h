#ifndef __PANIC_H
#define __PANIC_H

#ifdef DEBUG
#define PANIC(x) _panic(x)
void
_panic(char* message);
#else
#define PANIC(x)
#endif

#endif
