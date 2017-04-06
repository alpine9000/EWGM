#include "game.h"

extern uint16_t P61_Master;
extern uint16_t music_song1;

extern void
P61_Init(__REG("a0", void* module));

__EXTERNAL uint16_t P61_Target = 0;

__attribute__((aligned (4))) __section(random_c) uint32_t music_module1[(MAX_P61_SIZE+512)/4];
__attribute__((aligned (4))) __section(random_c) uint32_t music_module2[(MAX_P61_SIZE+512)/4];

static __section(section music) uint8_t music_level_a[] DISK_SECTOR_ALIGN = {
#include "out/P61.jojo_ingame.h"
 } ;

static __section(section music) uint8_t music_level_b[] DISK_SECTOR_ALIGN = {
#include "out/P61.jojo_boss.h"
 } ;

static __section(section music) uint8_t music_level_c[] DISK_SECTOR_ALIGN = {
#include "out/P61.switpiano.h"
 } ;

typedef struct {
  uint8_t* data;
  uint32_t length;
} music_song_t;

static music_song_t music_songs[] = {
  { music_level_a, sizeof(music_level_a)},
  { music_level_b, sizeof(music_level_b)},
  { music_level_c, sizeof(music_level_c)}
};

static void* music_current_ptr = music_module1;
__EXTERNAL void* music_spare_ptr = music_module2;
static uint16_t music_currentModule = 0xFFFF;

void 
music_play(uint16_t moduleIndex)
{
  if (moduleIndex == music_currentModule) {
    return;
  }
  music_currentModule = moduleIndex;
  uint16_t p61_Target = P61_Target;
  disk_loadData(music_spare_ptr, music_songs[moduleIndex].data, music_songs[moduleIndex].length);
  music_current_ptr = music_current_ptr == music_module1 ? music_module2 : music_module1;
  music_spare_ptr = music_spare_ptr == music_module1 ? music_module2 : music_module1;
  P61_Master = 0;
  P61_Target = 0;
  hw_waitVerticalBlank();
  hw_waitVerticalBlank();
  P61_Init(music_current_ptr);
  P61_Target = p61_Target;
}


void
music_restart(void)
{
  uint16_t p61_Target = P61_Target;
  P61_Master = 0;
  P61_Target = 0;
  hw_waitVerticalBlank();
  hw_waitVerticalBlank();
  P61_Init(music_current_ptr);
  P61_Target = p61_Target;  
}


void
music_next(void)
{
  uint16_t moduleIndex = music_currentModule+1;
  if (moduleIndex >= (sizeof(music_songs)/sizeof(music_song_t))) {
    moduleIndex = 0;
  }
  music_play(moduleIndex);
}


uint16_t
music_toggle(void)
{
  if (P61_Target == MUSIC_MAX_MUSIC_VOLUME) {
    P61_Target = 0;
  } else {
    P61_Target = MUSIC_MAX_MUSIC_VOLUME;
  }

  return P61_Target;
}


uint16_t
music_enabled(void)
{
  return P61_Target;
}
