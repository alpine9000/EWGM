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

typedef struct {
  level_t* levelData;
  uint32_t dataSize;
  uint16_t moduleIndex;
  conductor_instruction_t* instructions;
} level_config_t;

level_config_t level_levels[LEVEL_NUM_LEVELS] = {
  { 
    .levelData = (level_t*)level_level1_data,//&level_level1,
    .dataSize = sizeof(level_level1_data),
    .instructions = level1_instructions,
    .moduleIndex = 0,
  },
  { 
    .levelData = (level_t*)level_level2_data,//&level_level2,
    .instructions = level1_instructions,
    .moduleIndex = 0,
  }    
};

static uint16_t level_current = 0xFFFF;

#ifdef GAME_COMPRESS_LEVEL_DATA 
__SECTION_RANDOM static uint16_t level_buffer[100000/2];
#endif

  extern void delz(uint16_t* src, void* dest);
void
level_load(uint16_t index)
{
  if (index == level_current) {
    message_loading(I18N_LOADING);  
    music_play(level_levels[index].moduleIndex);        
    //    disk_loadData(&level, level_levels[index].levelData, sizeof(level.tileAddresses) /*+ sizeof(level.item_spriteIds)*/);
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

#ifdef GAME_COMPRESS_LEVEL_DATA 
  disk_loadData(&level_buffer, level_levels[index].levelData, level_levels[index].dataSize);
  extern void  depack(__REG("d0", void* src), __REG("d1", void* dest));
  depack(level_buffer, &level);
#else
  disk_loadData(&level, level_levels[index].levelData, sizeof(level_t));
#endif
  
  level.instructions = level_levels[index].instructions;
  level.moduleIndex = level_levels[index].moduleIndex;
#ifdef GAME_RECORDING
  level.record = (record_t*)&level.recordData;
#endif

  message_screenOff();

  level_current = index;
}
