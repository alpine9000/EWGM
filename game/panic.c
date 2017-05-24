#include "game.h"

#ifdef DEBUG
void
_panic(char* message)
{
  custom->color[0] = 0xf00;
  message_screenOn(message);
  for(;;);
}
#endif
