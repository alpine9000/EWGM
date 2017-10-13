#ifndef __BALOON_H
#define __BALOON_H

#define BALOON_DATA_MAGIC_NUMBER 0xFAADB55B

#define baloon_data(x) ((baloon_data_t*)(_object_data(x, OBJECT_DATA_TYPE_BALOON)))

typedef struct _baloon{
#ifdef DEBUG
  uint32_t magicNumber;
#endif
  struct _baloon* prev;
  struct _baloon* next;
  int16_t underAttack;
  uint16_t frameCount;
} baloon_data_t;


void
baloon_init(void);

object_t*
baloon_add(int16_t x, int16_t y);

void
baloon_attack(object_t* attacker, object_t* baloon, int16_t dx);

void
baloon_updatePosition(uint16_t deltaT, object_t* ptr);

object_t*
baloon_collision(object_t* a);

#endif
