//#pragma once

typedef struct {
  uint16_t animId;
  uint16_t bulletAnimId;
  uint16_t bulletSpeed;
  uint16_t bulletHeight;
  uint16_t bulletDammage;
  int16_t bulletXOffsetLeft;
  int16_t bulletXOffsetRight;  
} gunfighter_config_t;


void
gunfighter_add(gunfighter_config_t* gunfighterConfig, enemy_config_t* config, int16_t x, int16_t y);

uint16_t
gunfighter_intelligence(uint16_t deltaT, object_t* ptr, fighter_data_t* data);
