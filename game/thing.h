#ifndef __THING_H
#define __THING_H


void
thing_init(void);

object_t*
thing_add(uint16_t id, uint16_t animId, uint16_t brokenId, uint16_t junkStartId, int16_t x, int16_t y, int16_t numBonus);

void
thing_attack(object_t* thing, int16_t dx);

void
thing_addStar(object_t* ptr, int16_t dx, int16_t yOffset);

void
thing_updatePosition(uint16_t deltaT, object_t* ptr);

object_t*
thing_collision(object_t* a);
#endif 
