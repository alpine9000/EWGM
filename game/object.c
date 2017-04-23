#include "game.h"

int16_t object_count;
object_t* object_activeList;
static object_t* object_freeList;
static __section(random_c) object_t object_buffer[OBJECT_MAX_OBJECTS];
object_t* object_zBuffer[OBJECT_MAX_OBJECTS];

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
}


void
object_init(void)
{
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
    if (object_get_state(ptr) == OBJECT_STATE_REMOVED) {
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


static void
object_saveBackground(void)
{
  object_t* ptr = object_activeList;

  int16_t i = 0;
  while (ptr != 0) {
    object_zBuffer[i] = ptr;
    i++;
    ptr->save.position->x = object_x(ptr)+ptr->image->dx;
    ptr->save.position->y = object_y(ptr)-ptr->image->h;
    ptr->save.position->w = ptr->image->w;
    ptr->save.position->h = ptr->image->h;    
    ptr->save.position = ptr->save.position == &ptr->save.positions[0] ? &ptr->save.positions[1] : &ptr->save.positions[0];    
    ptr = ptr->next;
  }
}


void
object_render(frame_buffer_t fb, uint16_t deltaT)
{
  object_update(deltaT);
  object_restoreBackground(fb);
  object_saveBackground();  

  sort_z(object_count, object_zBuffer);

  for (int16_t i = 0; i < object_count; i++) {
    object_t* ptr = object_zBuffer[i];
    if (object_get_state(ptr) != OBJECT_STATE_REMOVED) {
      object_renderObject(fb, ptr);
      object_updateAnimation(deltaT, ptr);      
    }
  }
}


void
object_restoreBackground(frame_buffer_t fb)
{
  object_t* ptr = object_activeList;

  gfx_setupRenderTile();
  
  while (ptr != 0) {
    if (!ptr->tileRender) {
      object_clear(fb, ptr->save.position->x, ptr->save.position->y, ptr->save.position->w, ptr->save.position->h);
    }

    ptr = ptr->next;
  }
}


int16_t
object_collision(int16_t deltaT, object_t* a, object_collision_t* collision, uint16_t thresholdx, uint16_t thresholdy)
{
  int16_t vy = a->velocity.y;
  int16_t vx = a->velocity.x;
  
  if (deltaT == 2) {
    vx *= 2;
    vy *= 2;
  }
  
  int16_t _collision = 0;
  object_t* b = object_activeList;
  
  collision->up = collision->down = collision->left = collision->right = 0;

#ifdef DEBUG
  if (!game_collisions) {
    return 0;
  }
#endif

  int16_t a_y = ((object_py(a) + vy) / OBJECT_PHYSICS_FACTOR);
  int16_t a_x1 = (((object_px(a) + vx) / OBJECT_PHYSICS_FACTOR) + a->widthOffset)-thresholdx;
  int16_t a_x2 = (((object_px(a) + vx) / OBJECT_PHYSICS_FACTOR) + (a->width - a->widthOffset)) + thresholdx;
  
  while (b) {
    if (b->collidable && b != a) {
      int16_t b_y = ((object_y(b)));

      if (abs(a_y - b_y) <= thresholdy) {
	int16_t b_x1 = ((object_x(b))) + b->widthOffset;
	int16_t b_x2 = ((object_x(b))) + (b->width - b->widthOffset);
	
	if (a_x1 < b_x2 && a_x2 > b_x1) {		  
	  if (b_y >= a_y) {
	    collision->up = b;
	  } else if (b_y < a_y) {
	    collision->down = b;
	  }
	  if (b_x1 >= a_x1) {
	    collision->right = b;
	  } else if (b_x1 < a_x1) {
	    collision->left = b;
	  }
	  _collision = 1;
	}
      }
    }
    b = b->next;
  }
  
  return _collision;
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
  object_set_state(ptr, OBJECT_STATE_ALIVE);
  ptr->class = class;
  ptr->visible = 1;
  ptr->id = id;
  ptr->velocity.x = dx;
  ptr->velocity.y = 0;
  ptr->save.position = &ptr->save.positions[0];  
  ptr->save.positions[0].w = 0;
  ptr->save.positions[1].w = 0;  
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

  ptr->collidable = (ptr->class == OBJECT_CLASS_FIGHTER || ptr->class == OBJECT_CLASS_THING);
  object_addToActive(ptr);
  return ptr;
}
