#include "game.h"

int16_t object_count;
object_t* object_activeList;
static object_t* object_freeList;
static __section(random_c) object_t object_buffer[OBJECT_MAX_OBJECTS];
object_t* object_zBuffer[OBJECT_MAX_OBJECTS];
#ifdef OBEJCT_Z_BUFFER_COLLISION
uint16_t object_zBufferValid;
#endif

static object_t*
object_getFree(void)
{
  object_t* entry = object_freeList;
  object_freeList = object_freeList->next;
  if (object_freeList) {
    object_freeList->prev = 0;
  }

#ifdef DEBUG
  if (!entry) {
    PANIC("object_getFree: empty list");
  }
#endif
  
  return entry;
}


static void
object_addFree(object_t* ptr)
{
  object_count--;

  if (object_freeList == 0) {
    object_freeList = ptr;
    ptr->next = 0;
    ptr->prev = 0;
  } else {
    ptr->next = object_freeList;
    ptr->next->prev = ptr;
    ptr->prev = 0;
    object_freeList = ptr;
  }
}


static void
object_addToActive(object_t* ptr)
{
  object_count++;

  if (object_activeList == 0) {
    object_activeList = ptr;
    ptr->next = 0;
    ptr->prev = 0;
  } else {
    ptr->next = object_activeList;    
    ptr->next->prev = ptr;
    ptr->prev = 0;
    object_activeList = ptr;
  }
}


static void
object_removeFromActive(object_t* ptr)
{
  if (ptr->prev == 0) {
    object_activeList = ptr->next;
    if (object_activeList) {
      object_activeList->prev = 0;
    }
  } else {
    ptr->prev->next = ptr->next;
    if (ptr->next != 0) {
      ptr->next->prev = ptr->prev;
    }
  }
}


void
object_setAnim(object_t* ptr, int16_t anim)
{
  if (ptr->animId != anim) {
    ptr->animId = anim;
    ptr->anim = &object_animations[ptr->animId];
    ptr->imageIndex = ptr->anim->start;
    ptr->image = &object_imageAtlas[ptr->imageIndex];
    ptr->frameCounter = 0;   
  }
}


void
object_free(object_t* ptr)
{
  if (ptr->freeData) {
    ptr->freeData(ptr->data);
  }
  object_removeFromActive(ptr);
  object_addFree(ptr);
}


void
object_setAction(object_t* ptr, object_action_t action)
{
  ptr->actionId = action;
  object_setAnim(ptr, ptr->baseId + action);
}


void
object_updatePosition(uint16_t deltaT, object_t* ptr)
{
  int16_t vx = ptr->velocity.x;
  int16_t vy = ptr->velocity.y;

  if (deltaT == 2) {
    vx *= 2;
    vy *= 2;
  }  
  
  int16_t lastX = object_px(ptr);
  int16_t lastY = object_py(ptr);

  object_set_px(ptr, lastX + vx);
  object_set_py(ptr, lastY + vy);
    
  ptr->velocity.dx = object_px(ptr) - lastX;
  ptr->velocity.dy = object_py(ptr) - lastY;

  #if 0
  if (ptr->state == OBJECT_STATE_ALIVE) {
    object_set_z(ptr, object_y(ptr));
    if (vx || vy) {
      if (vx > 0) {
	object_setAction(ptr, OBJECT_RUN_RIGHT);
      } else if (vx < 0) {
	object_setAction(ptr, OBJECT_RUN_LEFT);
      } else {
	if (ptr->anim->facing == FACING_RIGHT) {
	  object_setAction(ptr, OBJECT_RUN_RIGHT);
	} else {
	  object_setAction(ptr, OBJECT_RUN_LEFT);
	}
      }
    } else {
      if (ptr->anim->facing == FACING_RIGHT) {
	object_setAction(ptr, OBJECT_STAND_RIGHT);
      } else {
	object_setAction(ptr, OBJECT_STAND_LEFT);
      }
    }
  }
  #endif
}


void
object_init(void)
{
  USE(object_animations[0].start);
#ifdef OBEJCT_Z_BUFFER_COLLISION
  object_zBufferValid = 0;
#endif
  object_count = 0;
  object_activeList = 0;
  object_freeList = &object_buffer[0];
  object_freeList->prev = 0;
  object_t* ptr = object_freeList;
  for (int16_t i = 1; i < OBJECT_MAX_OBJECTS; i++) {
      ptr->next = &object_buffer[i];
      ptr->next->prev = ptr;
      ptr = ptr->next;
  }
  ptr->next = 0;
  
#if 0

#endif
}

static void
object_updateAnimation(uint16_t deltaT, object_t *ptr)
{
  if (ptr->frameCounter >= ptr->anim->speed) {
    ptr->imageIndex++;
    ptr->frameCounter = 0;
    if (ptr->imageIndex > ptr->anim->stop) {
      ptr->imageIndex = ptr->anim->start;
    }
    ptr->image = &object_imageAtlas[ptr->imageIndex];
  } else {
    ptr->frameCounter+=deltaT;
  }
}


#ifndef OBJECT_BACKING_STORE
void
object_clear(frame_buffer_t fb, int16_t ox, int16_t oy, int16_t ow, int16_t oh)
{
  if (ow) {
    int16_t sx = ox>>4;
    int16_t sy = (oy)>>4;
    int16_t ex = (ox+ow)>>4;
    int16_t ey = (oy+oh)>>4;  
    for (int32_t x = sx; x <= ex; x++) {
      for (int32_t y = sy; y <= ey; y++) {
	uint16_t tile = level.tileAddresses[x][y];
	int16_t screenX = 0xf+(x<<4)-game_cameraX-game_screenScrollX;
	int16_t screenY = y << 4;
	if (screenY >= 0 && screenX >= 0 && screenX <= SCREEN_WIDTH+TILE_WIDTH) {
	  gfx_quickRenderTile(fb, screenX, screenY, level.tileBitplanes+tile);
	}
      }
    }
  }
}
#endif

         
void
object_renderObject(frame_buffer_t fb, object_t* ptr)
{
  if (!ptr->visible) {
    return;
  }
  
  int16_t w = ptr->image->w;
  int16_t h = ptr->image->h;

  int16_t screenx = object_screenx(ptr);
  int16_t screeny = object_screeny(ptr);
  int16_t sx = ptr->image->x;
  int16_t sy = ptr->image->y;
    
    
  if (screenx < -TILE_WIDTH) {
    int16_t tiles = (-screenx>>4)<<4;
    sx += tiles;
    w -= tiles ;
    screenx += tiles;
  }

  if ((screenx+w) > SCREEN_WIDTH+TILE_WIDTH) {
    int16_t tiles = (((screenx+w) -(SCREEN_WIDTH+TILE_WIDTH))>>4)<<4;
    w -= tiles;
  }

  if (screeny < 0) {
    sy -= screeny;
    h += screeny;
    screeny = 0;
  }

  if (w > 0 && h > 0) {
    if (ptr->tileRender) {
      gfx_setupRenderTile();
      object_clear(fb, object_x(ptr)+ptr->image->dx, object_y(ptr)-h, w, h);
      return;
    }
    gfx_renderSprite(fb, sx, sy, screenx, screeny, w, h);
  }
}


static void
object_update(uint16_t deltaT)
{
  object_t* ptr = object_activeList;

  while (ptr) {
    object_t* next = ptr->next;
    if (ptr->update) {
      ptr->update(deltaT, ptr);
    }
    if (ptr->state == OBJECT_STATE_REMOVED) {
      if (ptr->deadRenderCount == 2) {
	if (ptr == game_player1) {
	  game_player1 = 0;
	} else if (ptr == game_player2) {
	  game_player2 = 0;
	}
	object_free(ptr);

      } else {
	ptr->deadRenderCount++;
      }
    }
    ptr = next;
  }
}


void
object_render(frame_buffer_t fb, uint16_t deltaT)
{
  object_update(deltaT);
  object_restoreBackground(fb);
  object_saveBackground(fb);  

  sort_z(object_count, object_zBuffer);
#ifdef OBEJCT_Z_BUFFER_COLLISION
  object_zBufferValid = 1;
#endif

  for (int16_t i = 0; i < object_count; i++) {
    object_t* ptr = object_zBuffer[i];
    if (ptr->state != OBJECT_STATE_REMOVED) {
      object_renderObject(fb, ptr);
      object_updateAnimation(deltaT, ptr);      
    }
  }
}

#ifdef OBEJCT_Z_BUFFER_COLLISION
void
object_initZbuffer(void)
{
  object_t* ptr = object_activeList;
  int16_t i = 0;
  while (ptr != 0) {
    object_zBuffer[i] = ptr;
    i++;
    ptr = ptr->next;
  }
  sort_z(object_count, object_zBuffer);
  object_zBufferValid = 1;
}
#endif

void
object_saveBackground(frame_buffer_t fb)
{
  object_t* ptr = object_activeList;

  int16_t i = 0;
  while (ptr != 0) {
    object_zBuffer[i] = ptr;
    i++;
    USE(fb);
#ifdef OBJECT_BACKING_STORE
      gfx_saveSprite(fb, ptr->save.buffer, ptr->save.blit, object_screenx(ptr), object_screeny(ptr), ptr->image->w, ptr->image->h);
      ptr->save.blit = ptr->save.blit == &ptr->save.blits[0] ? &ptr->save.blits[1] : &ptr->save.blits[0];
      ptr->save.buffer = ptr->save.buffer == ptr->save.buffers[0].fb ? ptr->save.buffers[1].fb : ptr->save.buffers[0].fb;
#else
      //      if (ptr->x >= 0) {
	ptr->save.position->x = object_x(ptr)+ptr->image->dx;
	ptr->save.position->y = object_y(ptr)-ptr->image->h;
	ptr->save.position->w = ptr->image->w;
	ptr->save.position->h = ptr->image->h;    
	ptr->save.position = ptr->save.position == &ptr->save.positions[0] ? &ptr->save.positions[1] : &ptr->save.positions[0];
	//      } else {
	//	ptr->save.position->w = 0;
	//      }
#endif
    
    ptr = ptr->next;
  }
}


void
object_restoreBackground(frame_buffer_t fb)
{
  object_t* ptr = object_activeList;

#ifndef OBJECT_BACKING_STORE  
    gfx_setupRenderTile();
#endif
  
  while (ptr != 0) {
#ifdef OBJECT_BACKING_STORE
    USE(fb);
    if (ptr->save.blit->size > 0) {
      gfx_restoreSprite(ptr->save.blit);
    } else {
      return;
    }
#else
    if (!ptr->tileRender) {
      object_clear(fb, ptr->save.position->x, ptr->save.position->y, ptr->save.position->w, ptr->save.position->h);
    }
#endif

    ptr = ptr->next;
  }
}

NOINLINE object_t*
object_add(uint16_t id, uint16_t class, int16_t x, int16_t y, int16_t dx, int16_t anim, void (*update)(uint16_t deltaT, object_t* ptr), void* data, void (*freeData)(void*))
{
#ifdef DEBUG
  if (object_count >= OBJECT_MAX_OBJECTS) {
    PANIC("object_add: no free objects");
    return 0;
  }
#endif

  object_t* ptr = object_getFree();
  ptr->state = OBJECT_STATE_ALIVE;
  ptr->class = class;
  ptr->visible = 1;
  ptr->id = id;
  ptr->velocity.x = dx;
  ptr->velocity.y = 0;
#ifdef OBJECT_BACKING_STORE
  ptr->save.blit = &ptr->save.blits[0];
  ptr->save.buffer = ptr->save.buffers[0].fb;
  //ptr->sprite.saveBufferHeightOffset = ((48/8)*SCREEN_BIT_DEPTH);
  ptr->save.blits[0].size = 0;
  ptr->save.blits[1].size = 0;
#else
  ptr->save.position = &ptr->save.positions[0];  
  ptr->save.positions[0].w = 0;
  ptr->save.positions[1].w = 0;  
#endif

  ptr->anim = &object_animations[anim];
  ptr->animId = anim;
  ptr->baseId = anim;
  ptr->imageIndex = ptr->anim->start;
  ptr->image = &object_imageAtlas[ptr->imageIndex];
  ptr->_px = x*OBJECT_PHYSICS_FACTOR;
  ptr->_x = x;
  ptr->_py = y*OBJECT_PHYSICS_FACTOR;
  ptr->_y = y;
  object_set_z(ptr, y);  
  ptr->frameCounter = 0;
  ptr->deadRenderCount = 0;
  ptr->update = update;
  ptr->data = data;
  ptr->freeData = freeData;
  ptr->tileRender = 0;
  object_addToActive(ptr);
  return ptr;
}
