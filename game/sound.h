#ifndef __SOUND_H
#define __SOUND_H

typedef enum {
  SOUND_BUD_PUNCH01 = 0,
  SOUND_TERENCE_PUNCH01,
  SOUND_ENEMY_PUNCH01,
  SOUND_DIE01,
  SOUND_DIE02,
  SOUND_DIE03,
  SOUND_PICKUP,
  SOUND_MENU,
  SOUND_REV,
  SOUND_MOTORBIKE,
  SOUND_SHOOT,
  SOUND_BALOON,
  SOUND_LAST
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
