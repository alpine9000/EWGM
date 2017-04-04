#include "game.h"

#ifdef DEBUG
void
_panic(char* message)
{
  message_screenOn(message);
  for(;;);
}
#endif
