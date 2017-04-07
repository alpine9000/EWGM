#ifndef __FIGHTER_H
#define __FIGHTER_H


typedef struct fighter_data {
  struct fighter_data* prev;
  struct fighter_data* next;  
  uint16_t id;
  uint16_t punchCount;
  uint16_t punchType;
  int16_t attack_py;
  uint16_t (*intelligence)(object_t* ptr, struct fighter_data* data);
  int16_t walkAbout; // todo: move to enemy data  
  int16_t health;
  uint16_t attackDurationFrames;
  uint16_t attackDammage;
  int16_t flashCount;
  int16_t flashFrames;
} fighter_data_t;


void
fighter_init(void);

object_t*
fighter_add(uint16_t id, uint16_t animId, int16_t x, int16_t y, uint16_t initialHealth, uint16_t attackDammage, uint16_t (*intelligence)(object_t* ptr, struct fighter_data* data));

void
fighter_update(uint16_t deltaT, object_t* ptr);

#endif
