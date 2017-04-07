#ifndef __FIGHTER_H
#define __FIGHTER_H


typedef struct fighter_data{
  uint16_t punchCount;
  uint16_t punchType;
  int16_t attack_py;
  uint16_t (*intelligence)(object_t* ptr, struct fighter_data* data);
  int16_t walkAbout; // todo: move to enemy data  
} fighter_data_t;


object_t*
fighter_add(uint16_t animId, int16_t x, int16_t y, fighter_data_t* data, uint16_t (*intelligence)(object_t* ptr, struct fighter_data* data));

void
fighter_update(uint16_t deltaT, object_t* ptr);

#endif
