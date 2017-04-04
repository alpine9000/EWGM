#include "game.h"

#define POPUP_BOX_WIDTH 160
#define POPUP_BOX_HEIGHT 16
#define POPUP_BOX_SAVE_WIDTH (POPUP_BOX_WIDTH+16)

extern frame_buffer_t popup_frameBuffer;

typedef struct {
  uint8_t fb[((POPUP_BOX_SAVE_WIDTH)/8)*SCREEN_BIT_DEPTH*POPUP_BOX_HEIGHT];
} popup_box_sprite_save_t;


typedef struct {
  sprite_t sprite;
  sprite_save_t saves[2];
  popup_box_sprite_save_t saveBuffers[2];
  uint16_t visible;
  image_t image;
  int16_t screenY;
  int16_t targetY;
  void (*callback)(void);
  uint32_t frameCounter;
} popup_box_sprite_t;

static __section(random_c) popup_box_sprite_t popup_boxSprite;

void 
popup_ctor(void)
{
  popup_boxSprite.visible = 0;
}

void
popup_render(frame_buffer_t fb)
{
  if (popup_boxSprite.visible) {
    if (popup_boxSprite.screenY == -POPUP_BOX_HEIGHT && popup_boxSprite.targetY == -POPUP_BOX_HEIGHT) {
      if (popup_boxSprite.saves[0].blit[0].size == 0 &&
	  popup_boxSprite.saves[1].blit[0].size == 0) {
	popup_boxSprite.visible = 0;
	if (popup_boxSprite.callback != 0) {
	  (*popup_boxSprite.callback)();
	}
      }
    } else {
      gfx_splitBlitNoMask(fb, popup_frameBuffer, popup_boxSprite.sprite.x, popup_boxSprite.sprite.y-game_cameraY, 0, 0, POPUP_BOX_WIDTH, POPUP_BOX_HEIGHT);
    }
  }
}


void
popup_saveBackground(frame_buffer_t fb)
{
  if (popup_boxSprite.visible) {
    popup_boxSprite.sprite.y = game_cameraY+popup_boxSprite.screenY;

    if (popup_boxSprite.targetY > 0) {
      if (popup_boxSprite.screenY < popup_boxSprite.targetY) {
	popup_boxSprite.screenY+=8;
      }
    } else if (popup_boxSprite.targetY < 0) {
      if (popup_boxSprite.screenY > popup_boxSprite.targetY) {
	popup_boxSprite.screenY-=8;
      }
    } 

    
    if (popup_boxSprite.targetY != -POPUP_BOX_HEIGHT && popup_boxSprite.screenY >= popup_boxSprite.targetY) {
      if (popup_boxSprite.frameCounter == 0) {
	popup_boxSprite.frameCounter = hw_verticalBlankCount;
      } else if ((hw_verticalBlankCount - popup_boxSprite.frameCounter ) > 100) {
	if (!enemy_find(ENEMY_ANIM_JOYSTICK)) {
	  enemy_addMapObject(ENEMY_ANIM_JOYSTICK, (SCREEN_WIDTH/2)-16, game_cameraY + (SCREEN_HEIGHT/2)-16, 0, 0);
	}
      }
    }
    

    if (popup_boxSprite.callback && game_fire()) {
      popup_boxSprite.targetY = -POPUP_BOX_HEIGHT;
    }

    sprite_save(fb, &popup_boxSprite.sprite);
    popup_boxSprite.sprite.save = popup_boxSprite.sprite.save == &popup_boxSprite.saves[0] ? &popup_boxSprite.saves[1] : &popup_boxSprite.saves[0];
    popup_boxSprite.sprite.saveBuffer = popup_boxSprite.sprite.saveBuffer == popup_boxSprite.saveBuffers[0].fb ? popup_boxSprite.saveBuffers[1].fb : popup_boxSprite.saveBuffers[0].fb;
  }
}


void
popup_restoreBackground(void)
{
  if (popup_boxSprite.visible) {
    sprite_restore(popup_boxSprite.sprite.save);
  }
}


void
popup(char* message, void(*callback)(void))
{
  if (popup_boxSprite.visible) {
    return;
  }
  popup_boxSprite.frameCounter = 0;
  popup_boxSprite.callback = callback;
  popup_boxSprite.screenY = -POPUP_BOX_HEIGHT;
  popup_boxSprite.targetY = 48;
  popup_boxSprite.sprite.x = (SCREEN_WIDTH/2)-(POPUP_BOX_WIDTH/2);
  popup_boxSprite.sprite.save = &popup_boxSprite.saves[0];
  popup_boxSprite.sprite.saveBuffer = popup_boxSprite.saveBuffers[0].fb;
  popup_boxSprite.sprite.saveBufferHeightOffset = ((POPUP_BOX_SAVE_WIDTH/8)*SCREEN_BIT_DEPTH);
  popup_boxSprite.saves[0].blit[0].size = 0;
  popup_boxSprite.saves[0].blit[1].size = 0;
  popup_boxSprite.saves[1].blit[0].size = 0;
  popup_boxSprite.saves[1].blit[1].size = 0;
  popup_boxSprite.sprite.image = &popup_boxSprite.image;
  popup_boxSprite.image.h = POPUP_BOX_HEIGHT;
  popup_boxSprite.image.w = POPUP_BOX_WIDTH;

  popup_boxSprite.visible = 1;

  gfx_fillRect(popup_frameBuffer, 4, 4, POPUP_BOX_WIDTH-8, 8, 0);
  hw_waitBlitter();

  frame_buffer_t ptr = popup_frameBuffer;
  ptr += FRAME_BUFFER_WIDTH_BYTES;
  text_drawText8(ptr, message, (POPUP_BOX_WIDTH/2)-((strlen(message)/2)*8), 4);
  ptr += 3*FRAME_BUFFER_WIDTH_BYTES;
  text_drawText8(ptr, message, (POPUP_BOX_WIDTH/2)-((strlen(message)/2)*8), 4);
}


void
popup_off(void)
{
  popup_boxSprite.visible = 0;
}


void
popup_dismiss(void)
{
  popup_boxSprite.targetY = -POPUP_BOX_HEIGHT;
}
