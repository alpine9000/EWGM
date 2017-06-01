#include "game.h"

__SECTION_RANDOM_C level_t level;

#if 0
#define __LEVEL_NUMBER       1
#include "define_level.h"

#define __LEVEL_NUMBER       2
#include "define_level.h"
extern level_t level_level1;
extern level_t level_level2;

#endif


__SECTION_DISK uint8_t level_level1_data[] DISK_SECTOR_ALIGN = {
  #include "leveldata_1.c"
};
__SECTION_DISK uint8_t level_level2_data[] DISK_SECTOR_ALIGN = {
  #include "leveldata_2.c"
};
__SECTION_DISK uint8_t level_level3_data[] DISK_SECTOR_ALIGN = {
  #include "leveldata_3.c"
};

typedef struct {
  level_t* levelData;
  uint32_t dataSize;
  uint16_t moduleIndex;
  conductor_instruction_t* instructions;
  char* readyMessage;
} level_config_t;

level_config_t level_levels[LEVEL_NUM_LEVELS] = {
  { 
    .levelData = (level_t*)level_level1_data,
    .dataSize = sizeof(level_level1_data),
    .instructions = level1_instructions,
    .moduleIndex = 0,
    .readyMessage = I18N_LEVEL1_READY
  },
  { 
    .levelData = (level_t*)level_level2_data,
    .dataSize = sizeof(level_level2_data),    
    .instructions = level1_instructions,
    .moduleIndex = 0,
    .readyMessage = I18N_LEVEL2_READY    
  },
  { 
    .levelData = (level_t*)level_level3_data,
    .dataSize = sizeof(level_level3_data),    
    .instructions = level1_instructions,
    .moduleIndex = 0,
    .readyMessage = I18N_LEVEL3_READY    
  }      
};

static uint16_t level_current = 0xFFFF;

void
level_readyMessage(void)
{
#if 0
  if (level_current) {
    volatile int x = 1;
    __USE(x);
  }
#endif
  
  message_screenOn(level_levels[level_current].readyMessage);

  for (uint32_t i = 0; i < 100; i++) {
    hw_readJoystick();
    keyboard_read();
    if (game_fire()) {
      break;
    }
    hw_waitVerticalBlank();
  }
}


void
level_load(uint16_t index)
{
  if (!game_scoreboardLoaded) {
    message_loading(I18N_LOADING);    
    extern uint8_t scoreBoardBitplanes;
    disk_loadData((void*)game_scoreBoardFrameBuffer, &scoreBoardBitplanes, SCOREBOARD_BUFFER_SIZE_BYTES);
    game_scoreboardLoaded = 1;
  }
  
  if (index == level_current) {
    music_play(level_levels[index].moduleIndex);
    level_readyMessage();
    message_screenOff();    
    return;
  }

#ifdef DEBUG
  if (index >= LEVEL_NUM_LEVELS) {
    message_screenOn("Invalid level number");
    for(;;);
  }
#endif

  if (!message_on) {
    message_loading(I18N_LOADING);
  }
  
  music_play(level_levels[index].moduleIndex);

#ifdef GAME_COMPRESS_DATA 
  disk_loadCompressedData(&level, level_levels[index].levelData, level_levels[index].dataSize);
#else
  disk_loadData(&level, level_levels[index].levelData, sizeof(level_t));
#endif
  
  level.instructions = level_levels[index].instructions;
  level.moduleIndex = level_levels[index].moduleIndex;
#ifdef GAME_RECORDING
  level.record = (record_t*)&level.recordData;
#endif

  level_current = index;  
  
  level_readyMessage();
  
  message_screenOff();
}
