#include "game.h"

#ifdef GAME_STARS

#define STAR_WIDTH 16
#define STAR_MAX_STARS 6
#define SPRITE_SPRITESHEET_Y 547
#define SPRITE_SPRITESHEET_X 112

#define star_screenx(ptr) (0xf + ptr->x-game_cameraX-game_screenScrollX)
#define star_screeny(ptr) (ptr->y)

typedef struct {
  uint8_t vStartLo;
  uint8_t hStartHi;
  uint8_t vStopLo;
  uint8_t attach:1;
  uint8_t unused:4;
  uint8_t vStartHi:1;
  uint8_t vStopHi:1;
  uint8_t hStartLow:1;
} star_sprite_control_t;


typedef struct star {
  struct star* next;
  struct star* prev;
  int16_t x;
  int16_t y;
  object_velocity_t velocity;
  uint16_t _state;
  int16_t startY;
  star_sprite_control_t* sprite;
  uint16_t spriteHi;
  uint16_t spriteLo;
  uint16_t index;
} star_t;


int16_t star_count;
star_t* star_activeList;
static star_t* star_freeList;
static __SECTION_RANDOM star_t star_buffer[STAR_MAX_STARS];
static uint16_t star_yOffset;

__SECTION_DATA_C uint16_t sprite_nullhsprite[] = {
  0x0000, 0x0000,
  0x0000, 0x0000
};
uint16_t sprite_nullhspriteHi;
uint16_t sprite_nullhspriteLo;

__SECTION_DATA_C uint16_t star_sprite0[] = {
  #include "out/star-sprite.h"
};
__SECTION_DATA_C uint16_t star_sprite1[] = {
  #include "out/star-sprite.h"
};
__SECTION_DATA_C uint16_t star_sprite2[] = {
  #include "out/star-sprite.h"
};
__SECTION_DATA_C uint16_t star_sprite3[] = {
  #include "out/star-sprite.h"
};
__SECTION_DATA_C uint16_t star_sprite4[] = {
  #include "out/star-sprite.h"
};
__SECTION_DATA_C uint16_t star_sprite5[] = {
  #include "out/star-sprite.h"
};

uint16_t* star_sprites[STAR_MAX_STARS] = {
  star_sprite0,
  star_sprite1,
  star_sprite2,
  star_sprite3,
  star_sprite4,
  star_sprite5,
};

static star_t*
star_getFree(void)
{
#ifdef DEBUG
  if (star_count > STAR_MAX_STARS) {
    PANIC("star_getFree: empty list");
  }
#endif


  star_t* entry = star_freeList;
  star_freeList = star_freeList->next;
  if (star_freeList) {
    star_freeList->prev = 0;
  }

  return entry;
}


static void
star_addFree(void* data)
{
  star_t* ptr = data;
  star_count--;

  if (star_freeList == 0) {
    star_freeList = ptr;
    ptr->next = 0;
    ptr->prev = 0;
  } else {
    ptr->next = star_freeList;
    ptr->next->prev = ptr;
    ptr->prev = 0;
    star_freeList = ptr;
  }
}

__EXTERNAL void
star_resetSprpt(void)
{
  int16_t index = 1, i = 0;

  for (; i < STAR_MAX_STARS; i++) {
    star_t* ptr = &star_buffer[i];
    if (ptr->_state == OBJECT_STATE_REMOVED) {
      copper.sprpt[index] = sprite_nullhspriteLo;
      index += 2;
      copper.sprpt[index] = sprite_nullhspriteHi;
      index += 2;
    } else {
      copper.sprpt[index] = ptr->spriteLo;
      index += 2;
      copper.sprpt[index] = ptr->spriteHi;
      index += 2;
    }
  }

  for (; i < 8; i++) {
    copper.sprpt[index] = sprite_nullhspriteLo;
    index += 2;
    copper.sprpt[index] = sprite_nullhspriteHi;
    index += 2;
  }
}

static void
star_renderSprite(star_t* ptr)
{
  int16_t x = star_screenx(ptr);
  int16_t y = star_screeny(ptr);
  uint16_t vStartLo = y + RASTER_Y_START;
  uint16_t vStopLo = vStartLo + 10;
  uint16_t vStopHi = ((vStopLo) & 0x100) >> 8;
  uint16_t hStartHi = (x + RASTER_X_START) >> 1;
  uint8_t vStartHi = (y + RASTER_Y_START) > 255;

  /*  if (vStopLo >= RASTER_Y_START + SCREEN_HEIGHT) {
    vStopLo =  RASTER_Y_START + SCREEN_HEIGHT;
    }*/

  ptr->sprite->vStartLo = vStartLo;
  ptr->sprite->vStartHi = vStartHi;
  ptr->sprite->hStartHi = hStartHi;
  ptr->sprite->vStopLo =  vStopLo;
  ptr->sprite->vStopHi =  vStopHi;
}


void
star_init(void)
{
  star_yOffset = 0;
  star_count = 0;
  star_activeList = 0;
  star_freeList = &star_buffer[0];
  star_freeList->prev = 0;
  star_t* ptr = star_freeList;
  for (int16_t i = 1; i < STAR_MAX_STARS; i++) {
    ptr->next = &star_buffer[i];
    ptr->next->prev = ptr;
    ptr = ptr->next;
  }
  ptr->next = 0;

  for (int16_t i = 0; i < STAR_MAX_STARS; i++) {
    star_buffer[i].sprite = (star_sprite_control_t*)star_sprites[i];
    star_buffer[i].spriteHi = (uint32_t)star_buffer[i].sprite >> 16;
    star_buffer[i].spriteLo = ((uint32_t)star_buffer[i].sprite & 0xffff);
    star_buffer[i]._state = OBJECT_STATE_REMOVED;
    star_buffer[i].index = i;
  }

  sprite_nullhspriteHi = (uint32_t) sprite_nullhsprite >> 16;
  sprite_nullhspriteLo = ((uint32_t)sprite_nullhsprite & 0xffff);

  star_resetSprpt();

  custom->dmacon = (DMAF_SPRITE|DMAF_SETCLR);
}

static void
star_addToActive(star_t* ptr)
{
  star_count++;

  if (star_activeList == 0) {
    star_activeList = ptr;
    ptr->next = 0;
    ptr->prev = 0;
  } else {
    ptr->next = star_activeList;
    ptr->next->prev = ptr;
    ptr->prev = 0;
    star_activeList = ptr;
  }
}


static void
star_removeFromActive(star_t* ptr)
{
  if (ptr->prev == 0) {
    star_activeList = ptr->next;
    if (star_activeList) {
      star_activeList->prev = 0;
    }
  } else {
    ptr->prev->next = ptr->next;
    if (ptr->next != 0) {
      ptr->next->prev = ptr->prev;
    }
  }
}


static void
star_free(star_t* ptr)
{
  star_removeFromActive(ptr);
  star_addFree(ptr);
}

static void
star_doUpdate(uint16_t deltaT, star_t* ptr)
{
  ptr->velocity.y += deltaT;

  int16_t vx = ptr->velocity.x;
  int16_t vy = ptr->velocity.y;

  /*  if (deltaT == 2) {
      vx *= 2;
      vy *= 2;
      } */

  int16_t lastX = ptr->x;
  int16_t lastY = ptr->y;

  ptr->x = lastX + vx;
  ptr->y = lastY + vy;

  if (star_screenx(ptr) < -STAR_WIDTH || (ptr->y >= ptr->startY && ptr->velocity.y > 0)) {
    ptr->_state = OBJECT_STATE_REMOVED;
  } else {
    star_renderSprite(ptr);
  }

}

void
star_update(uint16_t deltaT)
{
  star_t* ptr = star_activeList;

  while (ptr) {
    star_t* next = ptr->next;
    star_doUpdate(deltaT, ptr);
    if (ptr->_state == OBJECT_STATE_REMOVED) {
      star_free(ptr);
    }
    ptr = next;
  }
}


static void
star_addStar(object_t* actor, int16_t dx, int16_t dy, int16_t yOffset)
{
  if (star_count >= STAR_MAX_STARS) {
    return;
  }

  int16_t x = object_x(actor) + OBJECT_WIDTH/2;
  int16_t y = object_y(actor)-40-yOffset;

  star_t* ptr = star_getFree();
  ptr->_state = OBJECT_STATE_ALIVE;
  ptr->velocity.x = dx;
  ptr->velocity.y = dy;
  ptr->x = x;
  ptr->y = y;
  ptr->startY = object_y(actor)-10;
  star_addToActive(ptr);
}

void
star_add(object_t* ptr, int16_t dx)
{
  static int32_t star_cycle = 0;
  switch (star_cycle) {
  case 0:
    star_addStar(ptr, dx*4, -10, 0);
    star_addStar(ptr, -dx*4, -8, 2);
    star_addStar(ptr, dx*2, -10, 10);
    break;
  case 1:
    star_addStar(ptr, dx*4, -10, 0);
    break;
  case 2:
    star_addStar(ptr, -dx*4, -8, 2);
    star_addStar(ptr, dx*2, -10, 10);
    break;
  }

  star_cycle++;
  if (star_cycle > 2) {
    star_cycle = 0;
  }
}

#endif
