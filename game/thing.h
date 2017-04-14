#ifndef __THING_H
#define __THING_H


void
thing_init(void);

object_t*
thing_add(uint16_t id, uint16_t animId, int16_t x, int16_t y);

void
thing_attack(object_t* thing, int16_t dx);
#endif 
