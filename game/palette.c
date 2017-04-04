#include "game.h"

uint16_t palette_background = 0;

void
palette_black()
{
  for (int16_t i = 0; i < 32; i++) {
    custom->color[i] = 0x000;
  }

  palette_background = 0;
}

#define _PALETTE_RED(x) ((x >> 8) & 0xf)
#define _PALETTE_GREEN(x) ((x >> 4) & 0xf)
#define _PALETTE_BLUE(x) ((x) & 0xf)
#define _PALETTE_FIXED 8

void
palette_fadeTo(uint16_t* palette, uint16_t colors, uint16_t color)
{
  int fr = _PALETTE_RED(color) << 4;
  int fg = _PALETTE_GREEN(color) << 4;
  int fb = _PALETTE_BLUE(color) << 4;
  
  for (int i = 0; i < 64+1; i++) {
    for (int c = 0; c < colors; c++) {

      int tr = _PALETTE_RED(palette[c]);
      int tg = _PALETTE_GREEN(palette[c]);
      int tb = _PALETTE_BLUE(palette[c]);            
      
      int nr = ((fr<<_PALETTE_FIXED)+((((tr<<_PALETTE_FIXED)-(fr<<_PALETTE_FIXED))/64)*i))>>_PALETTE_FIXED;
      int ng = ((fg<<_PALETTE_FIXED)+((((tg<<_PALETTE_FIXED)-(fg<<_PALETTE_FIXED))/64)*i))>>_PALETTE_FIXED;
      int nb = ((fb<<_PALETTE_FIXED)+((((tb<<_PALETTE_FIXED)-(fb<<_PALETTE_FIXED))/64)*i))>>_PALETTE_FIXED;      

      custom->color[c] = (nr<<8)|(ng<<4)|nb;
    }

    hw_waitScanLines(100);
  }

  palette_background = palette[0];
}

void
palette_fadeFrom(uint16_t* palette, uint16_t colors, uint16_t color)
{
  int tr = _PALETTE_RED(color) << 4;
  int tg = _PALETTE_GREEN(color) << 4;
  int tb = _PALETTE_BLUE(color) << 4;
  
  for (int i = 0; i < 64+1; i++) {
    for (int c = 0; c < colors; c++) {

      int fr = _PALETTE_RED(palette[c]);
      int fg = _PALETTE_GREEN(palette[c]);
      int fb = _PALETTE_BLUE(palette[c]);            
      
      int nr = ((fr<<_PALETTE_FIXED)+((((tr<<_PALETTE_FIXED)-(fr<<_PALETTE_FIXED))/64)*i))>>_PALETTE_FIXED;
      int ng = ((fg<<_PALETTE_FIXED)+((((tg<<_PALETTE_FIXED)-(fg<<_PALETTE_FIXED))/64)*i))>>_PALETTE_FIXED;
      int nb = ((fb<<_PALETTE_FIXED)+((((tb<<_PALETTE_FIXED)-(fb<<_PALETTE_FIXED))/64)*i))>>_PALETTE_FIXED;      

      custom->color[c] = (nr<<8)|(ng<<4)|nb;
    }

    hw_waitScanLines(100);
  }

  palette_background = palette[0];
}


