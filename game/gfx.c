#include "game.h"

__EXTERNAL uint16_t gfx_dyOffsetsLUT[FRAME_BUFFER_NUM_LINES];
uint16_t gfx_heightLUT[129];
uint16_t gfx_renderSprite16NoShiftSetup;
uint32_t gfx_dySpriteOffsetsLUT[SPRITE_SHEET_HEIGHT];

#ifdef GAME_ONE_BITPLANE_SPRITE_MASK
uint32_t gfx_dySpriteMaskOffsetsLUT[SPRITE_SHEET_HEIGHT];
#endif

void 
gfx_ctor()
{
  for (uint16_t y = 0; y < FRAME_BUFFER_NUM_LINES; y++) {
    gfx_dyOffsetsLUT[y] = (y * (FRAME_BUFFER_WIDTH_BYTES*SCREEN_BIT_DEPTH));
  }

  for (uint16_t y = 0; y < SPRITE_SHEET_HEIGHT; y++) {
    gfx_dySpriteOffsetsLUT[y] = (y * (SPRITE_SHEET_WIDTH_BYTES*SCREEN_BIT_DEPTH));
  }

#ifdef GAME_ONE_BITPLANE_SPRITE_MASK
  for (uint16_t y = 0; y < SPRITE_SHEET_HEIGHT; y++) {
    gfx_dySpriteMaskOffsetsLUT[y] = (y * (SPRITE_SHEET_WIDTH_BYTES));
  }
#endif

  for (uint16_t h = 0; h <= 128; h++) {
    gfx_heightLUT[h] = (h*SCREEN_BIT_DEPTH)<<6;
  }
}


#ifndef GAME_ONE_BITPLANE_SPRITE_MASK
INLINE void
gfx_screenWidthBitBlit(frame_buffer_t dest, int16_t sx, int16_t sy, int16_t dx, int16_t dy, int16_t w, int16_t h)
{
  static volatile struct Custom* _custom = CUSTOM;
  frame_buffer_t source = level.spriteBitplanes;
  frame_buffer_t mask = level.spriteMask;
  uint32_t widthWords =  ((w+15)>>4)+1;
  int32_t shift = (dx&0xf);
  
  dest += (dy * (SCREEN_WIDTH_BYTES*SCREEN_BIT_DEPTH)) + (dx>>3);
  source += gfx_dySpriteOffsetsLUT[sy] + (sx>>3);
  mask += gfx_dySpriteOffsetsLUT[sy] + (sx>>3);

  hw_waitBlitter();   
  //  _custom->bltafwm = 0xffff;
  _custom->bltalwm = 0x0000;
  _custom->bltcon0 = (SRCA|SRCB|SRCC|DEST|0xca|shift<<ASHIFTSHIFT);
  _custom->bltcon1 = shift<<BSHIFTSHIFT;
  _custom->bltamod = (SPRITE_SHEET_WIDTH_BYTES-(widthWords<<1));
  _custom->bltbmod = (SPRITE_SHEET_WIDTH_BYTES-(widthWords<<1));
  _custom->bltcmod = (SCREEN_WIDTH_BYTES-(widthWords<<1));
  _custom->bltdmod = (SCREEN_WIDTH_BYTES-(widthWords<<1));
  
  _custom->bltapt = (uint8_t*)mask;
  _custom->bltbpt = (uint8_t*)source;
  _custom->bltcpt = (uint8_t*)dest;
  _custom->bltdpt = (uint8_t*)dest;
  //  _custom->bltsize = (h*SCREEN_BIT_DEPTH)<<6 | widthWords;
  _custom->bltsize = gfx_heightLUT[h] | widthWords;
}
#else
INLINE void
gfx_screenWidthBitBlit(frame_buffer_t dest, int16_t sx, int16_t sy, int16_t dx, int16_t dy, int16_t w, int16_t h)
{
  static volatile struct Custom* _custom = CUSTOM;
  frame_buffer_t source = level.spriteBitplanes;
  frame_buffer_t mask = level.spriteMask;
  uint32_t widthWords =  ((w+15)>>4)+1;
  int32_t shift = (dx&0xf);
  
  dest += (dy * (SCREEN_WIDTH_BYTES*SCREEN_BIT_DEPTH)) + (dx>>3);
  source += gfx_dySpriteOffsetsLUT[sy] + (sx>>3);
  mask += gfx_dySpriteMaskOffsetsLUT[sy] + (sx>>3);

  uint16_t bltsize = (h)<<6 | widthWords;

  uint16_t bltamod = (SPRITE_SHEET_WIDTH_BYTES-(widthWords<<1));
  uint16_t bltbmod = (SPRITE_SHEET_WIDTH_BYTES*(SCREEN_BIT_DEPTH-1))+(SPRITE_SHEET_WIDTH_BYTES-(widthWords<<1));
  uint16_t bltdmod = (SCREEN_WIDTH_BYTES*(SCREEN_BIT_DEPTH-1))+(SCREEN_WIDTH_BYTES-(widthWords<<1));
  
  hw_waitBlitter();   
  //  _custom->bltafwm = 0xffff;
  _custom->bltalwm = 0x0000;
  _custom->bltcon0 = (SRCA|SRCB|SRCC|DEST|0xca|shift<<ASHIFTSHIFT);
  _custom->bltcon1 = shift<<BSHIFTSHIFT;

  _custom->bltamod = bltamod;
  _custom->bltbmod = bltbmod;
  _custom->bltcmod = bltdmod;
  _custom->bltdmod = bltdmod;
  
  _custom->bltapt = (uint8_t*)mask;
  _custom->bltbpt = (uint8_t*)source;
  _custom->bltcpt = (uint8_t*)dest;
  _custom->bltdpt = (uint8_t*)dest;
  _custom->bltsize = bltsize;

  frame_buffer_t s = source;
  frame_buffer_t d = dest;
  
  for (uint16_t i = 0; i < SCREEN_BIT_DEPTH-1; i++) {
    s += SPRITE_SHEET_WIDTH_BYTES;
    d += SCREEN_WIDTH_BYTES;
    hw_waitBlitter();    
    _custom->bltapt = (uint8_t*)mask;
    _custom->bltbpt = (uint8_t*)s;
    _custom->bltcpt = (uint8_t*)d;
    _custom->bltdpt = (uint8_t*)d;
    _custom->bltsize = bltsize;    
  }


}
#endif

void 
gfx_screenWidthBitBlitNoMask(frame_buffer_t dest, frame_buffer_t src, int16_t sx, int16_t sy, int16_t dx, int16_t dy, int16_t w, int16_t h)
{
  static volatile struct Custom* _custom = CUSTOM;
  uint32_t widthWords = ((w+15)>>4)+1;
  int32_t shift = (dx&0xf);
  
  dest +=  (dy * (SCREEN_WIDTH_BYTES*SCREEN_BIT_DEPTH)) + (dx>>3);
  src += (sy * (SCREEN_WIDTH_BYTES*SCREEN_BIT_DEPTH)) + (sx>>3);

  hw_waitBlitter();

  _custom->bltcon0 = (SRCA|DEST|0xf0|shift<<ASHIFTSHIFT);
  _custom->bltcon1 = shift<<BSHIFTSHIFT;
  _custom->bltalwm = 0x0;
  _custom->bltamod = (SCREEN_WIDTH_BYTES-(widthWords<<1));
  _custom->bltdmod = (SCREEN_WIDTH_BYTES-(widthWords<<1));
  _custom->bltapt = (uint8_t*)src;
  _custom->bltdpt = (uint8_t*)dest;
  _custom->bltsize = ((h*SCREEN_BIT_DEPTH)<<6) | widthWords;
}



void
gfx_fillRect(frame_buffer_t fb, uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color)
{
  static custom_t _custom = CUSTOM;
  static uint16_t startBitPatterns[] = { 0xffff,
			       0x7fff, 0x3fff, 0x1fff, 0x0fff, 
			       0x07ff, 0x03ff, 0x01ff, 0x00ff,
			       0x007f, 0x003f, 0x001f, 0x000f,
			       0x0007, 0x0003, 0x0001, 0x0000 };

  static uint16_t endBitPatterns[] = { 0xffff, 
				    0x8000, 0xc000, 0xe000, 0xf000,
				    0xf800, 0xfc00, 0xfe00, 0xff00,
				    0xff80, 0xffc0, 0xffe0, 0xfff0,
				    0xfff8, 0xfffc, 0xfffe, 0xffff};

  uint16_t startMask = startBitPatterns[x & 0xf]; 
  uint16_t endMask = endBitPatterns[(x+w) & 0xf]; 
  uint32_t widthWords = (((x&0x0f)+w)+15)>>4;
  
  if (widthWords == 1) {
    startMask &= endMask;
  }
  
  fb += gfx_dyOffsetsLUT[y] + (x>>3);

  int32_t colorInPlane;
  for (int32_t plane = 0; plane < SCREEN_BIT_DEPTH; plane++) {
    colorInPlane = (1<<plane) & color;
    hw_waitBlitter();
    
    _custom->bltcon0 = (SRCC|DEST|0xca);
    _custom->bltcon1 = 0;
    _custom->bltafwm = 0xffff;
    _custom->bltalwm = 0xffff;
    _custom->bltdmod = (FRAME_BUFFER_WIDTH_BYTES*(SCREEN_BIT_DEPTH-1))+(FRAME_BUFFER_WIDTH_BYTES-2);
    _custom->bltcmod = (FRAME_BUFFER_WIDTH_BYTES*(SCREEN_BIT_DEPTH-1))+(FRAME_BUFFER_WIDTH_BYTES-2);
    _custom->bltbmod = 0;
    _custom->bltamod = 0;
    _custom->bltadat = startMask;
    _custom->bltbdat = colorInPlane ? 0xffff : 0x0;
    _custom->bltcpt = (uint8_t*)fb;
    _custom->bltdpt = (uint8_t*)fb;
    _custom->bltsize = h<<6 | 1;
    
    if (widthWords > 1) {
      hw_waitBlitter();    
      _custom->bltcon0 = (SRCC|DEST|0xca);
      _custom->bltadat = endMask;
      _custom->bltcpt = (uint8_t*)fb+((widthWords-1)<<1);
      _custom->bltdpt = (uint8_t*)fb+((widthWords-1)<<1);
      _custom->bltsize = h<<6 | 1;
    }
    
    if (widthWords > 2) {
      hw_waitBlitter();    
      _custom->bltcon0 = (DEST|(colorInPlane ? 0xff : 0x00));
      _custom->bltdmod = (FRAME_BUFFER_WIDTH_BYTES*(SCREEN_BIT_DEPTH-1))+(FRAME_BUFFER_WIDTH_BYTES-((widthWords-2)<<1));
      _custom->bltdpt = (uint8_t*)fb+2;
      _custom->bltsize = h<<6 | (widthWords-2);
    }    

    fb += FRAME_BUFFER_WIDTH_BYTES;
  }
}

INLINE void
gfx_fillRectSmallScreen(frame_buffer_t fb, uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color)
{
  static custom_t _custom = CUSTOM;
  static uint16_t startBitPatterns[] = { 0xffff,
			       0x7fff, 0x3fff, 0x1fff, 0x0fff, 
			       0x07ff, 0x03ff, 0x01ff, 0x00ff,
			       0x007f, 0x003f, 0x001f, 0x000f,
			       0x0007, 0x0003, 0x0001, 0x0000 };

  static uint16_t endBitPatterns[] = { 0xffff, 
				    0x8000, 0xc000, 0xe000, 0xf000,
				    0xf800, 0xfc00, 0xfe00, 0xff00,
				    0xff80, 0xffc0, 0xffe0, 0xfff0,
				    0xfff8, 0xfffc, 0xfffe, 0xffff};

  uint16_t startMask = startBitPatterns[x & 0xf]; 
  uint16_t endMask = endBitPatterns[(x+w) & 0xf]; 
  uint32_t widthWords = (((x&0x0f)+w)+15)>>4;
  
  if (widthWords == 1) {
    startMask &= endMask;
  }
  
  fb += y*(SCREEN_WIDTH_BYTES*SCREEN_BIT_DEPTH)/*gfx_dyOffsetsLUT[y]*/ + (x>>3);

  int32_t colorInPlane;
  for (int32_t plane = 0; plane < SCREEN_BIT_DEPTH; plane++) {
    colorInPlane = (1<<plane) & color;
    hw_waitBlitter();
    
    _custom->bltcon0 = (SRCC|DEST|0xca);
    _custom->bltcon1 = 0;
    _custom->bltafwm = 0xffff;
    _custom->bltalwm = 0xffff;
    _custom->bltdmod = (SCREEN_WIDTH_BYTES*(SCREEN_BIT_DEPTH-1))+(SCREEN_WIDTH_BYTES-2);
    _custom->bltcmod = (SCREEN_WIDTH_BYTES*(SCREEN_BIT_DEPTH-1))+(SCREEN_WIDTH_BYTES-2);
    _custom->bltbmod = 0;
    _custom->bltamod = 0;
    _custom->bltadat = startMask;
    _custom->bltbdat = colorInPlane ? 0xffff : 0x0;
    _custom->bltcpt = (uint8_t*)fb;
    _custom->bltdpt = (uint8_t*)fb;
    _custom->bltsize = h<<6 | 1;
    
    if (widthWords > 1) {
      hw_waitBlitter();    
      _custom->bltcon0 = (SRCC|DEST|0xca);
      _custom->bltadat = endMask;
      _custom->bltcpt = (uint8_t*)fb+((widthWords-1)<<1);
      _custom->bltdpt = (uint8_t*)fb+((widthWords-1)<<1);
      _custom->bltsize = h<<6 | 1;
    }
    
    if (widthWords > 2) {
      hw_waitBlitter();    
      _custom->bltcon0 = (DEST|(colorInPlane ? 0xff : 0x00));
      _custom->bltdmod = (SCREEN_WIDTH_BYTES*(SCREEN_BIT_DEPTH-1))+(SCREEN_WIDTH_BYTES-((widthWords-2)<<1));
      _custom->bltdpt = (uint8_t*)fb+2;
      _custom->bltsize = h<<6 | (widthWords-2);
    }    

    fb += SCREEN_WIDTH_BYTES;
  }
}

#ifndef GAME_ONE_BITPLANE_SPRITE_MASK
INLINE void
gfx_renderSprite(frame_buffer_t dest, int16_t sx, int16_t sy, int16_t dx, int16_t dy, int16_t w, int16_t h)
{
  static volatile struct Custom* _custom = CUSTOM;
  frame_buffer_t source = level.spriteBitplanes;
  frame_buffer_t mask = level.spriteMask;
  uint32_t widthWords =  ((w+15)>>4)+1;
  int32_t shift = (dx&0xf);
  
  dest += gfx_dyOffsetsLUT[dy] + (dx>>3);
  source += gfx_dySpriteOffsetsLUT[sy] + (sx>>3);
  mask += gfx_dySpriteOffsetsLUT[sy] + (sx>>3);

  hw_waitBlitter();   
  //  _custom->bltafwm = 0xffff;
  _custom->bltalwm = 0x0000;
  _custom->bltcon0 = (SRCA|SRCB|SRCC|DEST|0xca|shift<<ASHIFTSHIFT);
  _custom->bltcon1 = shift<<BSHIFTSHIFT;
  _custom->bltamod = (SPRITE_SHEET_WIDTH_BYTES-(widthWords<<1));
  _custom->bltbmod = (SPRITE_SHEET_WIDTH_BYTES-(widthWords<<1));
  _custom->bltcmod = (FRAME_BUFFER_WIDTH_BYTES-(widthWords<<1));
  _custom->bltdmod = (FRAME_BUFFER_WIDTH_BYTES-(widthWords<<1));
  
  _custom->bltapt = (uint8_t*)mask;
  _custom->bltbpt = (uint8_t*)source;
  _custom->bltcpt = (uint8_t*)dest;
  _custom->bltdpt = (uint8_t*)dest;
  //  _custom->bltsize = (h*SCREEN_BIT_DEPTH)<<6 | widthWords;
  _custom->bltsize = gfx_heightLUT[h] | widthWords;

}

#else
INLINE void
gfx_renderSprite(frame_buffer_t dest, int16_t sx, int16_t sy, int16_t dx, int16_t dy, int16_t w, int16_t h)
{
  static volatile struct Custom* _custom = CUSTOM;
  frame_buffer_t source = level.spriteBitplanes;
  frame_buffer_t mask = level.spriteMask;
  uint32_t widthWords =  ((w+15)>>4)+1;
  int32_t shift = (dx&0xf);
  
  dest += gfx_dyOffsetsLUT[dy] + (dx>>3);
  source += gfx_dySpriteOffsetsLUT[sy] + (sx>>3);
  mask += gfx_dySpriteMaskOffsetsLUT[sy] + (sx>>3);

  uint16_t bltsize = (h)<<6 | widthWords;

  uint16_t bltamod = (SPRITE_SHEET_WIDTH_BYTES-(widthWords<<1));
  uint16_t bltbmod = (SPRITE_SHEET_WIDTH_BYTES*(SCREEN_BIT_DEPTH-1))+(SPRITE_SHEET_WIDTH_BYTES-(widthWords<<1));
  uint16_t bltdmod = (FRAME_BUFFER_WIDTH_BYTES*(SCREEN_BIT_DEPTH-1))+(FRAME_BUFFER_WIDTH_BYTES-(widthWords<<1));
  hw_waitBlitter();   
  //  _custom->bltafwm = 0xffff;
  _custom->bltalwm = 0x0000;
  _custom->bltcon0 = (SRCA|SRCB|SRCC|DEST|0xca|shift<<ASHIFTSHIFT);
  _custom->bltcon1 = shift<<BSHIFTSHIFT;
  _custom->bltamod = bltamod;
  _custom->bltbmod = bltbmod;
  _custom->bltcmod = bltdmod;
  _custom->bltdmod = bltdmod;
  _custom->bltapt = (uint8_t*)mask;
  _custom->bltbpt = (uint8_t*)source;
  _custom->bltcpt = (uint8_t*)dest;
  _custom->bltdpt = (uint8_t*)dest;
  _custom->bltsize = bltsize;
  
  frame_buffer_t s = source;
  frame_buffer_t d = dest;
  
  for (uint16_t i = 0; i < SCREEN_BIT_DEPTH-1; i++) {
    s += SPRITE_SHEET_WIDTH_BYTES;
    d += FRAME_BUFFER_WIDTH_BYTES;
    hw_waitBlitter();    
    _custom->bltapt = (uint8_t*)mask;
    _custom->bltbpt = (uint8_t*)s;
    _custom->bltcpt = (uint8_t*)d;
    _custom->bltdpt = (uint8_t*)d;
    _custom->bltsize = bltsize;    
  }
}
#endif


INLINE void
gfx_setupRenderTile(void)
{
  volatile struct Custom* _custom = CUSTOM;

  hw_waitBlitter();

  _custom->bltcon0 = (SRCA|DEST|0xf0);
  _custom->bltcon1 = 0;
  //  _custom->bltafwm = 0xffff;
  _custom->bltalwm = 0xffff;
  //_custom->bltamod = FRAME_BUFFER_WIDTH_BYTES-2;
  _custom->bltamod = SPRITE_SHEET_WIDTH_BYTES-2;  
  _custom->bltdmod = FRAME_BUFFER_WIDTH_BYTES-2;
}

INLINE void
gfx_renderTile(frame_buffer_t dest, int16_t x, int16_t y, frame_buffer_t tile)
{
  volatile struct Custom* _custom = CUSTOM;
  
  dest += gfx_dyOffsetsLUT[y] + (x>>3);

  gfx_setupRenderTile();

  _custom->bltapt = (uint8_t*)tile;
  _custom->bltdpt = (uint8_t*)dest;
  _custom->bltsize = (16*SCREEN_BIT_DEPTH)<<6 | 1;
}

INLINE void
gfx_quickRenderTile(frame_buffer_t dest, int16_t x, int16_t y, frame_buffer_t tile)
{
  volatile struct Custom* _custom = CUSTOM;
  
  dest += gfx_dyOffsetsLUT[y] + (x>>3);

  hw_waitBlitter();

  _custom->bltapt = (uint8_t*)tile;
  _custom->bltdpt = (uint8_t*)dest;
  _custom->bltsize = (16*SCREEN_BIT_DEPTH)<<6 | 1;
}
