#include "game.h"

#if TRACKLOADER==0
__EXTERNAL struct ExecBase *SysBase = 0;
__EXTERNAL struct Library *DOSBase = 0;

void
dos_init(void)
{
  if (!SysBase) {
    SysBase = *((struct ExecBase **) 4); 
  }

  if (!DOSBase) {
    DOSBase = OpenLibrary("dos.library", 0);
  }
}
#else
void dos_init(void)
{

}
#endif
