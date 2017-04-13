#include "game.h"

wave_instruction_t wave_instructions[] = {
  {0, enemy_wave1},
  //  {SCREEN_WIDTH, enemy_wave2, 0, 0},
  {(SCREEN_WIDTH*2)-1, enemy_wave3},
};

static uint16_t wave_instructionIndex;

void
wave_init(void)
{
  wave_instructionIndex = 0;
  wave_process();
}

void
wave_process(void)
{
  for (; wave_instructionIndex < sizeof(wave_instructions)/sizeof(wave_instruction_t); wave_instructionIndex++) {
    wave_instruction_t *ptr = &wave_instructions[wave_instructionIndex];
    if (ptr->cameraX <= game_cameraX) {
      ptr->callback();
    } else {
      break;
    }
  }
}

uint16_t
wave_complete(void)
{
  return wave_instructionIndex >= sizeof(wave_instructions)/sizeof(wave_instruction_t);
}
