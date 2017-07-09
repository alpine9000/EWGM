#ifndef __THING_H
#define __THING_H

#define THING_DATA_MAGIC_NUMBER 0xF00DB00B

#define thing_data(x) ((thing_data_t*)(_object_data(x, OBJECT_DATA_TYPE_THING)))

enum {
  THING_BONUS_TYPE_HEALTH,
  THING_BONUS_TYPE_POINTS
};

typedef struct _thing{
#ifdef DEBUG
  uint32_t magicNumber;
#endif
  struct _thing* prev;
  struct _thing* next;  
  int16_t underAttack;
  int16_t attack_py;
  int16_t attackable;
  int16_t bonus;
  int16_t hasBonus;
  uint16_t bonusType;
  int16_t brokenId;
  int16_t junkStartId;
  
  uint16_t junkType;
  int16_t addJunkDx;
} thing_data_t;


void
thing_init(void);

object_t*
thing_add(uint16_t id, uint16_t animId, uint16_t brokenId, uint16_t junkStartId, int16_t x, int16_t y, int16_t numBonus, uint16_t bonusType);

void
thing_attack(object_t* thing, int16_t dx);

void
thing_addStar(object_t* ptr, int16_t dx, int16_t yOffset);

void
thing_updatePosition(uint16_t deltaT, object_t* ptr);

object_t*
thing_collision(object_t* a);
#endif 
