#include "game.h"

#define FONTMAP_WIDTH_BYTES 32
#define FONTMAP_BIT_DEPTH   1
#define SCOREBOARD_TEXT_Y   4

extern char* fontAtlas[128];
extern frame_buffer_t fontPtr;

INLINE void
text_drawCharScoreBoard(char c, int16_t x, int16_t y)
{
  char* src = fontAtlas[(int)c];
  char* dest = (char*)game_scoreBoardFrameBuffer+(x>>3)+(SCREEN_WIDTH_BYTES*(SCREEN_BIT_DEPTH*SCOREBOARD_TEXT_Y))+(SCREEN_WIDTH_BYTES*3);
  dest += (y*SCREEN_WIDTH_BYTES*SCREEN_BIT_DEPTH);
  *dest = *src;
  *(dest+(1*SCREEN_WIDTH_BYTES*SCREEN_BIT_DEPTH)) = *(src+(1*FONTMAP_WIDTH_BYTES*FONTMAP_BIT_DEPTH));
  *(dest+(2*SCREEN_WIDTH_BYTES*SCREEN_BIT_DEPTH)) = *(src+(2*FONTMAP_WIDTH_BYTES*FONTMAP_BIT_DEPTH));
  *(dest+(3*SCREEN_WIDTH_BYTES*SCREEN_BIT_DEPTH)) = *(src+(3*FONTMAP_WIDTH_BYTES*FONTMAP_BIT_DEPTH));
  *(dest+(4*SCREEN_WIDTH_BYTES*SCREEN_BIT_DEPTH)) = *(src+(4*FONTMAP_WIDTH_BYTES*FONTMAP_BIT_DEPTH));
  *(dest+(5*SCREEN_WIDTH_BYTES*SCREEN_BIT_DEPTH)) = *(src+(5*FONTMAP_WIDTH_BYTES*FONTMAP_BIT_DEPTH));
  *(dest+(6*SCREEN_WIDTH_BYTES*SCREEN_BIT_DEPTH)) = *(src+(6*FONTMAP_WIDTH_BYTES*FONTMAP_BIT_DEPTH));
  *(dest+(7*SCREEN_WIDTH_BYTES*SCREEN_BIT_DEPTH)) = *(src+(7*FONTMAP_WIDTH_BYTES*FONTMAP_BIT_DEPTH));
}


INLINE void
_text_drawChar8(frame_buffer_t frameBuffer, char c, int16_t x, int16_t y)
{
  USE(y);
  char* src = fontAtlas[(int)c];
  char* dest = (char*)frameBuffer+(x>>3);
  dest += (SCREEN_WIDTH_BYTES*SCREEN_BIT_DEPTH)*y;

  for (y = 0; y < 8; y++) {
    *dest = *src;
    dest += (SCREEN_WIDTH_BYTES*SCREEN_BIT_DEPTH);
    src += FONTMAP_WIDTH_BYTES*FONTMAP_BIT_DEPTH;
  }  
}


INLINE void
__text_drawChar8(frame_buffer_t frameBuffer, char c, int16_t x, int16_t y, int16_t sy, int16_t ny)
{
  USE(y);
  char* src = fontAtlas[(int)c];
  char* dest = (char*)frameBuffer+(x>>3);
  dest += (SCREEN_WIDTH_BYTES*SCREEN_BIT_DEPTH)*y;

  src += (FONTMAP_WIDTH_BYTES*FONTMAP_BIT_DEPTH)*sy;

  for (y = sy; y < ny; y++) {
    *dest = *src;
    dest += (SCREEN_WIDTH_BYTES*SCREEN_BIT_DEPTH);
    src += FONTMAP_WIDTH_BYTES*FONTMAP_BIT_DEPTH;
  }  
}

uint32_t bigNumAtlas[11] = {
  (33*FONTMAP_WIDTH_BYTES*FONTMAP_BIT_DEPTH)+(0*2),
  (33*FONTMAP_WIDTH_BYTES*FONTMAP_BIT_DEPTH)+(1*2),
  (33*FONTMAP_WIDTH_BYTES*FONTMAP_BIT_DEPTH)+(2*2),
  (33*FONTMAP_WIDTH_BYTES*FONTMAP_BIT_DEPTH)+(3*2),
  (33*FONTMAP_WIDTH_BYTES*FONTMAP_BIT_DEPTH)+(4*2),
  (33*FONTMAP_WIDTH_BYTES*FONTMAP_BIT_DEPTH)+(5*2),
  (33*FONTMAP_WIDTH_BYTES*FONTMAP_BIT_DEPTH)+(6*2),
  (33*FONTMAP_WIDTH_BYTES*FONTMAP_BIT_DEPTH)+(7*2),
  (33*FONTMAP_WIDTH_BYTES*FONTMAP_BIT_DEPTH)+(8*2),
  (33*FONTMAP_WIDTH_BYTES*FONTMAP_BIT_DEPTH)+(9*2),
  (33*FONTMAP_WIDTH_BYTES*FONTMAP_BIT_DEPTH)+(10*2),
};


INLINE void
text_clrBlit(frame_buffer_t dest, int16_t dx, int16_t dy, int16_t w, int16_t h)
{
  volatile struct Custom* _custom = CUSTOM;
  uint32_t widthWords =  ((w+15)>>4)+1;
  int32_t shift = (dx&0xf);
  
  dest += (dy * (SCREEN_WIDTH_BYTES*SCREEN_BIT_DEPTH)) + (dx>>3);

#define FONT_BIT_DEPTH 1
  
  hw_waitBlitter();   
  //  _custom->bltafwm = 0xffff;
  _custom->bltalwm = 0x0000;
  _custom->bltcon0 = (DEST|0xf0|shift<<ASHIFTSHIFT);
  _custom->bltcon1 = shift<<BSHIFTSHIFT;
  _custom->bltadat = 0;
  _custom->bltdmod = SCREEN_WIDTH_BYTES-(widthWords<<1)+(SCREEN_WIDTH_BYTES*(SCREEN_BIT_DEPTH-1));
  
  _custom->bltdpt = (uint8_t*)dest;
  //  _custom->bltsize = (h*SCREEN_BIT_DEPTH)<<6 | widthWords;
  _custom->bltsize = (h*FONT_BIT_DEPTH)<<6 | widthWords;

}

void
text_bitBlit(frame_buffer_t source, frame_buffer_t dest, int16_t dx, int16_t dy, int16_t w, int16_t h)
{
  volatile struct Custom* _custom = CUSTOM;
  uint32_t widthWords =  ((w+15)>>4)+1;
  int32_t shift = (dx&0xf);
  
  dest += (dy * (SCREEN_WIDTH_BYTES*SCREEN_BIT_DEPTH)) + (dx>>3);

#define FONT_BIT_DEPTH 1
  
  hw_waitBlitter();   
  //  _custom->bltafwm = 0xffff;
  _custom->bltalwm = 0x0000;
  _custom->bltcon0 = (SRCA|SRCB|SRCC|DEST|0xca|shift<<ASHIFTSHIFT);
  _custom->bltcon1 = shift<<BSHIFTSHIFT;
  _custom->bltamod = FONTMAP_WIDTH_BYTES-(widthWords<<1)+(FONTMAP_WIDTH_BYTES*(FONT_BIT_DEPTH-1));
  _custom->bltbmod = FONTMAP_WIDTH_BYTES-(widthWords<<1)+(FONTMAP_WIDTH_BYTES*(FONT_BIT_DEPTH-1));
  _custom->bltcmod = SCREEN_WIDTH_BYTES-(widthWords<<1)+(SCREEN_WIDTH_BYTES*(SCREEN_BIT_DEPTH-1));
  _custom->bltdmod = SCREEN_WIDTH_BYTES-(widthWords<<1)+(SCREEN_WIDTH_BYTES*(SCREEN_BIT_DEPTH-1));
  
  _custom->bltapt = (uint8_t*)source;
  _custom->bltbpt = (uint8_t*)source;
  _custom->bltcpt = (uint8_t*)dest;
  _custom->bltdpt = (uint8_t*)dest;
  //  _custom->bltsize = (h*SCREEN_BIT_DEPTH)<<6 | widthWords;
  _custom->bltsize = (h*FONT_BIT_DEPTH)<<6 | widthWords;

}


void
text_drawBigNumeral(frame_buffer_t frameBuffer, uint16_t n, int16_t x, int16_t y, int16_t ny)
{
  USE(y);
  frame_buffer_t src = fontPtr + bigNumAtlas[(int)n];
  text_bitBlit(src, frameBuffer, x, y, 8, ny);
}


INLINE void
text_drawText8(frame_buffer_t frameBuffer, char* string, int32_t x, int32_t y)
{
  char* ptr = &string[0]; 
  do {
    __text_drawChar8(frameBuffer, *ptr, x, y, 0, 8);
    ptr++;
    x += 8;
  } while (*ptr != 0);
}


INLINE void
text_drawScoreBoard(char* string, int32_t x, int32_t y)
{
  char* ptr = &string[0]; 
  do {
    text_drawCharScoreBoard(*ptr, x, y);
    ptr++;
    x += 8;
  } while (*ptr != 0);
}

INLINE void
__text_drawMaskedChar8(frame_buffer_t frameBuffer, char c, int16_t x, int16_t y, int16_t sx, int16_t ny, int16_t clear)
{
  USE(sx);
  char* src = fontAtlas[(int)c];
  char* dest = (char*)frameBuffer+(x>>3);
  dest += (SCREEN_WIDTH_BYTES*SCREEN_BIT_DEPTH)*y;

  
  volatile struct Custom* _custom = CUSTOM;
  uint32_t widthWords = 2;
  int32_t shift = (x&0xf);
  
#define FONT_BIT_DEPTH 1
  
  hw_waitBlitter();

  int16_t index = c - '0';
  if (index & 0x1) {
    _custom->bltafwm = 0x00ff;
    dest--;
    shift += 8;
  } else {
    _custom->bltafwm = 0xff00;    
  }
  _custom->bltalwm = 0x0000;
  _custom->bltadat = 0x0000;
  _custom->bltbdat = 0x0;
  _custom->bltcdat = 0x0;
  
  if (clear) {
    _custom->bltcon0 = (SRCA|SRCB|SRCC|DEST|0x2a|shift<<ASHIFTSHIFT);
  } else {
    _custom->bltcon0 = (SRCA|SRCB|SRCC|DEST|0xca|shift<<ASHIFTSHIFT);

  }

  _custom->bltamod = FONTMAP_WIDTH_BYTES-(widthWords<<1)+(FONTMAP_WIDTH_BYTES*(FONT_BIT_DEPTH-1));
  _custom->bltbmod = FONTMAP_WIDTH_BYTES-(widthWords<<1)+(FONTMAP_WIDTH_BYTES*(FONT_BIT_DEPTH-1));  
  _custom->bltcmod = SCREEN_WIDTH_BYTES-(widthWords<<1)+(SCREEN_WIDTH_BYTES*(SCREEN_BIT_DEPTH-1));
  _custom->bltdmod = SCREEN_WIDTH_BYTES-(widthWords<<1)+(SCREEN_WIDTH_BYTES*(SCREEN_BIT_DEPTH-1));
  _custom->bltcon1 = shift<<BSHIFTSHIFT;  
  _custom->bltapt = (uint8_t*)src;
    _custom->bltbpt = (uint8_t*)src;      
  _custom->bltcpt = (uint8_t*)dest;
  _custom->bltdpt = (uint8_t*)dest;
  _custom->bltsize = (ny*FONT_BIT_DEPTH)<<6 | widthWords;
  
}


void
text_drawMaskedText8Blitter(frame_buffer_t frameBuffer, char* string, int32_t x, int32_t y)
{
  char* ptr = &string[0]; 
  do {
    __text_drawMaskedChar8(frameBuffer, *ptr, x, y, 0, 8, 0);
    ptr++;
    x += 8;
  } while (*ptr != 0); 
}

void
text_clearMaskedText8Blitter(frame_buffer_t frameBuffer, char* string, int32_t x, int32_t y)
{
  char* ptr = &string[0]; 
  do {
    __text_drawMaskedChar8(frameBuffer, *ptr, x, y, 0, 8, 1);
    ptr++;
    x += 8;
  } while (*ptr != 0); 
}
