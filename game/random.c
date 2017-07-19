#include "game.h"

static uint32_t random_v = 0;

void
random_seed(uint32_t seed)
{
  random_v = seed;
}

uint32_t random(void)
{
  random_v = (random_v * (uint32_t)2654435761)>>16;
  return random_v;
}
