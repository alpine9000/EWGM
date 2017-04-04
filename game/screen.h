#ifndef __SCREEN_H
#define __SCREEN_H

void 
screen_pokeCopperList(frame_buffer_t frameBuffer, uint16_t volatile* copperPtr, uint16_t screenWidthBytes);
void 
screen_setup(uint16_t volatile* copperPtr);

#endif

