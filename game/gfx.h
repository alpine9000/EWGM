#ifndef __GFX_H
#define __GFX_H

#include <hardware/blit.h>

#define gfx_retroFontWidth 5  
#define gfx_retroFontHeight 8

extern const unsigned char font[];

extern uint16_t gfx_dyOffsetsLUT[FRAME_BUFFER_NUM_LINES];
extern uint16_t gfx_heightLUT[65];
extern uint16_t gfx_renderSprite16NoShiftSetup;
typedef struct {
  frame_buffer_t dest;
  frame_buffer_t source;
  uint16_t mod;
  uint16_t size;  
} gfx_blit_t;


void 
gfx_ctor(void);
void
gfx_screenWidthBitBlit(frame_buffer_t dest, int16_t sx, int16_t sy, int16_t dx, int16_t dy, int16_t w, int16_t h);
void
gfx_screenWidthBitBlitNoMask(frame_buffer_t dest, frame_buffer_t src, int16_t sx, int16_t sy, int16_t dx, int16_t dy, int16_t w, int16_t h);
void
gfx_fillRect(frame_buffer_t fb, uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color);
void
gfx_fillRectSmallScreen(frame_buffer_t fb, uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color);
void
gfx_renderSprite(frame_buffer_t dest, int16_t sx, int16_t sy, int16_t dx, int16_t dy, int16_t w, int16_t h);
void
gfx_renderSpriteNoMask(frame_buffer_t dest, int16_t sx, int16_t sy, int16_t dx, int16_t dy, int16_t w, int16_t h);
void
gfx_renderSprite16NoShift(frame_buffer_t dest, int16_t sx, int16_t sy, int16_t dx, int16_t dy, int16_t w, int16_t h);
void
gfx_saveSprite(frame_buffer_t source, frame_buffer_t dest, gfx_blit_t* blit, int16_t dx, int16_t dy, int16_t w, int16_t h);
void
gfx_saveSprite16(frame_buffer_t source, frame_buffer_t dest, gfx_blit_t* blit, int16_t dx, int16_t dy, int16_t h);
void
gfx_restoreSprite(gfx_blit_t* blit);
void
gfx_setupQuickRenderSprite(void);
void
gfx_quickRenderSprite(frame_buffer_t dest, int16_t sx, int16_t sy, int16_t dx, int16_t dy);
void
gfx_renderTile(frame_buffer_t dest, int16_t sx, int16_t sy, frame_buffer_t tile);
void
gfx_renderTile(frame_buffer_t fb, int16_t x, int16_t y, frame_buffer_t tile);
void
gfx_setupRenderTile(void);
void
gfx_quickRenderTile(frame_buffer_t dest, int16_t x, int16_t y, frame_buffer_t tile);
void
gfx_renderPartialTile(frame_buffer_t dest, int16_t x, int16_t y, uint16_t h, frame_buffer_t tile);
void
gfx_splitBlitNoMask(frame_buffer_t dest, frame_buffer_t src, int16_t dx, int16_t dy, int16_t sx, int16_t sy, int16_t w, int16_t _h);

#endif
