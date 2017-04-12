#include "game.h"

wave_instruction_t wave_instructions[] = {
  {0, enemy_wave1},
  //  {SCREEN_WIDTH, enemy_wave2},
  {(SCREEN_WIDTH*2)-1, enemy_wave3}  
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
    if (wave_instructions[wave_instructionIndex].cameraX <= game_cameraX) {
      wave_instructions[wave_instructionIndex].callback();
    } else {
      break;
    }
  }
}
