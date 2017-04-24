#ifndef __COPPER_H
#define __COPPER_H

#define AMIGA_SPRITES 8
#define NUM_SPRITE_COLUMNS 3
#define NUM_SPRITES 4

typedef struct {
  uint16_t bpl[SCREEN_BIT_DEPTH*2*2];
  uint16_t bpl1mod[2];
  uint16_t bpl2mod[2];  
  uint16_t bplcon1[2];
  uint16_t ddfstrt[2];
#ifdef GAME_STARS
  uint16_t sprpt[8*4];
#endif
  uint16_t wait[4];
  uint16_t sb_ddfstrt[2];
  uint16_t sb_bplcon1[2];  
  uint16_t sb_bpl[SCREEN_BIT_DEPTH*2*2];
  uint16_t sb_bpl1mod[2];
  uint16_t sb_bpl2mod[2];

  uint16_t fontColorWait[4];
  uint16_t fontColor[2];
  uint16_t fontColorWaitEnd[2];  
  uint16_t fontResetColor[2];    
  uint16_t end[2];
} copper_t;

extern copper_t copper;

#endif
