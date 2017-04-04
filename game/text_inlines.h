#include "game.h"

#define FONTMAP_WIDTH_BYTES 32
#define FONTMAP_BIT_DEPTH   1
#define SCOREBOARD_TEXT_Y   4

extern char* fontAtlas[128];

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
  dest += (FRAME_BUFFER_WIDTH_BYTES*SCREEN_BIT_DEPTH)*y;

  for (y = 0; y < 8; y++) {
    *dest = *src;
    dest += (FRAME_BUFFER_WIDTH_BYTES*SCREEN_BIT_DEPTH);
    src += FONTMAP_WIDTH_BYTES*FONTMAP_BIT_DEPTH;
  }  
}


INLINE void
__text_drawChar8(frame_buffer_t frameBuffer, char c, int16_t x, int16_t y, int16_t sy, int16_t ny)
{
  USE(y);
  char* src = fontAtlas[(int)c];
  char* dest = (char*)frameBuffer+(x>>3);
  dest += (FRAME_BUFFER_WIDTH_BYTES*SCREEN_BIT_DEPTH)*y;

  src += (FONTMAP_WIDTH_BYTES*FONTMAP_BIT_DEPTH)*sy;

  for (y = sy; y < ny; y++) {
    *dest = *src;
    dest += (FRAME_BUFFER_WIDTH_BYTES*SCREEN_BIT_DEPTH);
    src += FONTMAP_WIDTH_BYTES*FONTMAP_BIT_DEPTH;
  }  
}

#if 0
INLINE void
text_drawChar8(frame_buffer_t fb, char c, int16_t x, int16_t y)
{
  int16_t h = 8;
  y = y-game_screenScrollY;
  if (y >= 0) {
    __text_drawChar8(fb, c, x, y, 0, 8);
  } else {
    if (y > -h) {
      __text_drawChar8(fb, c, x, 0, -y, 8+y);
      __text_drawChar8(fb, c, x, FRAME_BUFFER_HEIGHT+y, 0, -y);
    } else {
      __text_drawChar8(fb, c, x, FRAME_BUFFER_HEIGHT+y, 0, 8);
    }
  }
}
#endif


INLINE void
text_drawText8(frame_buffer_t frameBuffer, char* string, int32_t x, int32_t y)
{
  char* ptr = &string[0]; 
  do {
    //    text_drawChar8(frameBuffer, *ptr, x, y);
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
