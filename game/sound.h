#ifndef __SOUND_H
#define __SOUND_H

typedef enum {
  SOUND_HEADSMASH = 0,
  SOUND_LAND = 1,
  SOUND_PICKUP,
  SOUND_KILL,
  SOUND_FALLING,
#ifdef GAME_JETPACK
  SOUND_JETPACK,
#endif
  SOUND_MENU
} sound_t;

#if SFX==1
void
sound_queueSound(sound_t sound);
void
sound_schedule(void);
void
sound_endLoop(void);
void
sound_init(void);
void
sound_vbl(void);
void
sound_playSound(sound_t sound);
#else
#define sound_init()
#define sound_vbl()
#define sound_queueSound(x)
#define sound_schedule()
#define sound_playSound(x)
#define sound_endLoop()
#endif

#endif
