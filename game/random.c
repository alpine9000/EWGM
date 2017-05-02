#include "game.h"

static uint32_t random_v = 0;
__EXTERNAL uint32_t _random_seed = 0xdeadbeef;
void
random_seed(uint32_t seed)
{
  random_v = seed;
  _random_seed = seed;
}

uint32_t random(void)
{
  random_v = random_v * (uint32_t)2654435761;
  return random_v;
}
