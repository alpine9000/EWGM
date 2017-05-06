#include "game.h"

__section(random_c) level_t level;

#define __LEVEL_NUMBER       1
#include "define_level.h"


typedef struct {
  level_t* levelData;
  uint16_t clouds;
  uint16_t moduleIndex;
  conductor_instruction_t* instructions;
  void (*initFunctor)(void);
  void (*effectFunctor)(frame_buffer_t fb);
} level_config_t;

level_config_t level_levels[LEVEL_NUM_LEVELS] = {
  { 
    .levelData = &level_level1,
    .instructions = level1_instructions,
    .clouds = 0,
    .moduleIndex = 0,
    .initFunctor = 0,
    .effectFunctor = 0,
  }
};

static uint16_t level_current = 0xFFFF;

void
level_load(uint16_t index)
{
  if (index == level_current) {
    message_loading(I18N_LOADING);  
    music_play(level_levels[index].moduleIndex);        
    disk_loadData(&level, level_levels[index].levelData, sizeof(level.tileAddresses) /*+ sizeof(level.item_spriteIds)*/);
    message_screenOff();    
    return;
  }

#ifdef DEBUG
  if (index >= LEVEL_NUM_LEVELS) {
    message_screenOn("Invalid level number");
    for(;;);
  }
#endif

  message_loading(I18N_LOADING);

  music_play(level_levels[index].moduleIndex);

  disk_loadData(&level, level_levels[index].levelData, sizeof(level_t));
  level.clouds = level_levels[index].clouds;
  level.instructions = level_levels[index].instructions;
  level.moduleIndex = level_levels[index].moduleIndex;
#ifdef GAME_RECORDING
  level.record = (record_t*)&level.recordData;
#endif
  level.initFunctor = level_levels[index].initFunctor;
#if 0
  level.effectFunctor = level_levels[index].effectFunctor;
#endif

  message_screenOff();

  level_current = index;
}
