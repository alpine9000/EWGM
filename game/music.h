#ifndef __MUSIC_H
#define __MUSIC_H

#define MUSIC_MAX_MUSIC_VOLUME 32

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
