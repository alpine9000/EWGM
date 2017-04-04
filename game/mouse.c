#include "game.h"

__attribute__ ((noinline)) uint32_t
mouse_leftButtonPressed(void)
{
  uint8_t volatile * ciaa = (uint8_t*)0xbfe001;
  return !(*ciaa & 0x40);
}

__attribute__ ((noinline)) uint32_t
mouse_rightButtonPressed(void)
{
  uint16_t volatile * potinp = (uint16_t*)0xdff016;
  return !(*potinp & 0x400);
}
