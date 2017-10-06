#include "game.h"

__SECTION_RANDOM_C level_chip_t levelChip;
__SECTION_RANDOM level_fast_t levelFast;

__SECTION_DISK uint8_t level_level1_c_data[] DISK_SECTOR_ALIGN = {
  #include "leveldata_c_1.c"
};
__SECTION_DISK uint8_t level_level1_f_data[] DISK_SECTOR_ALIGN = {
  #include "leveldata_f_1.c"
};
__SECTION_DISK uint8_t level_level2_c_data[] DISK_SECTOR_ALIGN = {
  #include "leveldata_c_2.c"
};
__SECTION_DISK uint8_t level_level2_f_data[] DISK_SECTOR_ALIGN = {
  #include "leveldata_f_2.c"
};
__SECTION_DISK uint8_t level_level3_c_data[] DISK_SECTOR_ALIGN = {
  #include "leveldata_c_3.c"
};
__SECTION_DISK uint8_t level_level3_f_data[] DISK_SECTOR_ALIGN = {
  #include "leveldata_f_3.c"
};
__SECTION_DISK uint8_t level_level4_c_data[] DISK_SECTOR_ALIGN = {
  #include "leveldata_c_4.c"
};
__SECTION_DISK uint8_t level_level4_f_data[] DISK_SECTOR_ALIGN = {
  #include "leveldata_f_4.c"
};

typedef struct {
  void* levelChipData;
  void* levelFastData;
  uint32_t chipDataSize;
  uint32_t fastDataSize;
  uint16_t moduleIndex;
  conductor_instruction_t* instructions;
  char* readyMessage;
  fighter_intelligence_functor playerIntelligence;
  int16_t mapTileWidth;
} level_config_t;

level_config_t level_levels[LEVEL_NUM_LEVELS] = {
  {
    .levelChipData = level_level1_c_data,
    .levelFastData = level_level1_f_data,
    .chipDataSize = sizeof(level_level1_c_data),
    .fastDataSize = sizeof(level_level1_f_data),
    .instructions = level1_instructions,
    .moduleIndex = MUSIC_LEVEL_1,
    .readyMessage = I18N_LEVEL1_READY,
    .playerIntelligence = player_intelligence,
    .mapTileWidth = MAP_LEVEL1_TILE_WIDTH
  },
  {
    .levelChipData = level_level2_c_data,
    .levelFastData = level_level2_f_data,
    .chipDataSize = sizeof(level_level2_c_data),
    .fastDataSize = sizeof(level_level2_f_data),
    .instructions = level2_instructions,
    .moduleIndex = MUSIC_LEVEL_2,
    .readyMessage = I18N_LEVEL2_READY,
    .playerIntelligence = level2_playerIntelligence,
    .mapTileWidth = MAP_LEVEL2_TILE_WIDTH
  },
  {
    .levelChipData = level_level3_c_data,
    .levelFastData = level_level3_f_data,
    .chipDataSize = sizeof(level_level3_c_data),
    .fastDataSize = sizeof(level_level3_f_data),
    .instructions = level3_instructions,
    .moduleIndex = 0,
    .readyMessage = I18N_LEVEL3_READY,
    .playerIntelligence = player_intelligence,
    .mapTileWidth = MAP_LEVEL3_TILE_WIDTH
  },
  {
    .levelChipData = level_level4_c_data,
    .levelFastData = level_level4_f_data,
    .chipDataSize = sizeof(level_level4_c_data),
    .fastDataSize = sizeof(level_level4_f_data),
    .instructions = level4_instructions,
    .moduleIndex = 0,
    .readyMessage = I18N_LEVEL4_READY,
    .playerIntelligence = player_intelligence,
    .mapTileWidth = MAP_LEVEL4_TILE_WIDTH
  }
};

static uint16_t level_current = 0xFFFF;

fighter_intelligence_functor
level_playerIntelligence(void)
{
  return level_levels[level_current].playerIntelligence;
}

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
  //  index++;
  if (!game_scoreboardLoaded) {
    message_loading(I18N_LOADING);
    extern uint8_t scoreBoardBitplanes;
    disk_loadData((void*)game_scoreBoardFrameBuffer, &scoreBoardBitplanes, SCOREBOARD_BUFFER_SIZE_BYTES);
    game_scoreboardLoaded = 1;
  }

  if (index == level_current) {
    if (!game_demo) {
      music_play(level_levels[index].moduleIndex);
    }
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


#ifdef GAME_COMPRESS_DATA
  disk_loadCompressedData(&levelChip, level_levels[index].levelChipData, level_levels[index].chipDataSize, 0);
  disk_loadCompressedData(&levelFast, level_levels[index].levelFastData, level_levels[index].fastDataSize, 0);
#else
  disk_loadData(&level, level_levels[index].levelData, sizeof(level_t));
#endif

  levelFast.instructions = level_levels[index].instructions;
  levelFast.moduleIndex = level_levels[index].moduleIndex;
#ifdef GAME_RECORDING
  levelFast.record = (record_t*)&levelFast.recordData;
#endif

  game_maxScrollX = (level_levels[index].mapTileWidth*TILE_WIDTH)-SCREEN_WIDTH;
  if (!game_demo) {
    music_play(level_levels[index].moduleIndex);
  }

  level_current = index;

  if (!game_demo) {
    level_readyMessage();
  }

  message_screenOff();
}


int16_t
level_scroll(uint16_t argument)
{
  if (enemy_count == 0) {
    conductor_setScrollTarget(argument);
    return 1;
  }

  return 0;
}
