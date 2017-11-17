#ifndef __MUSIC_H
#define __MUSIC_H

#define MUSIC_MAX_MUSIC_VOLUME 32

enum {
  MUSIC_LEVEL_1 = 0,
  MUSIC_UNKNOWN_1,
  MUSIC_GAME_OVER,
  MUSIC_BOSS_1,
  MUSIC_BOSS_COMPLETE,
  MUSIC_INTRO,
  MUSIC_LEVEL_2,
  MUSIC_LEVEL_3,
  MUSIC_LEVEL_3_BOSS,
  MUSIC_LEVEL_3_BOSS_COMPLETE,
  MUSIC_LEVEL_3_GAME_OVER,
};

extern uint16_t P61_Target;
extern uint16_t P61_Master;

void
music_setVolume(__REG("d0", int32_t volume));
void
music_play(uint16_t moduleIndex);
void
music_restart(void);
void
music_next(void);
uint16_t
music_toggle(void);
uint16_t
music_enabled(void);
#endif
