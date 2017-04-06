#ifndef __PLAYER_H
#define __PLAYER_H

typedef struct {
  uint16_t punchCount;
  uint16_t punchType;
} player_data_t;

object_t*
player_init(uint32_t id, uint16_t animId, int16_t x);
#endif
