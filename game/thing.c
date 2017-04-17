#include "game.h"

#define THING_MAX_THINGS 6

typedef struct _thing{
  struct _thing* prev;
  struct _thing* next;  
  int16_t underAttack;
  int16_t attack_py;
  int16_t attackable;
  int16_t bonus;
  int16_t hasBonus;
  int16_t brokenId;
  int16_t junkStartId;
} thing_data_t;

static int16_t thing_count;
static thing_data_t* thing_freeList;
static __section(random_c) thing_data_t thing_buffer[THING_MAX_THINGS];


static thing_data_t*
thing_getFree(void)
{
  thing_count++;

#ifdef DEBUG
  if (thing_count > THING_MAX_THINGS) {
    PANIC("thing_getFree: empty list");
  }
#endif
  
  
  thing_data_t* entry = thing_freeList;
  thing_freeList = thing_freeList->next;
  if (thing_freeList) {
    thing_freeList->prev = 0;
  }

  return entry;
}


static void
thing_addFree(void* data)
{
  thing_data_t* ptr = data;
  thing_count--;

  if (thing_freeList == 0) {
    thing_freeList = ptr;
    ptr->next = 0;
    ptr->prev = 0;
  } else {
    ptr->next = thing_freeList;
    ptr->next->prev = ptr;
    ptr->prev = 0;
    thing_freeList = ptr;
  }
}


void
thing_init(void)
{
  thing_count = 0;
  thing_freeList = &thing_buffer[0];
  thing_freeList->prev = 0;
  thing_data_t* ptr = thing_freeList;
  for (int16_t i = 1; i < THING_MAX_THINGS; i++) {
      ptr->next = &thing_buffer[i];
      ptr->next->prev = ptr;
      ptr = ptr->next;
  }
  ptr->next = 0;
}


void
thing_awardBonus(object_t* ptr, object_t* collision)
{
  if (collision->id == OBJECT_ID_PLAYER1) {
    ptr->state = OBJECT_STATE_REMOVED;    
    sound_queueSound(SOUND_PICKUP);
  } else if (collision->id == OBJECT_ID_PLAYER2) {
    ptr->state = OBJECT_STATE_REMOVED;
    sound_queueSound(SOUND_PICKUP);    
  }

  fighter_data_t* data = collision->data;
  data->health += 40;
  if (data->health > PLAYER_INITIAL_HEALTH) {
    data->health = PLAYER_INITIAL_HEALTH;
  }
}

void
thing_updatePosition(uint16_t deltaT, object_t* ptr)
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
  object_set_py_no_checks(ptr, lastY + vy);
    
  ptr->velocity.dx = object_px(ptr) - lastX;
  ptr->velocity.dy = object_py(ptr) - lastY;
}


object_t*
thing_collision(object_t* a)
{
  object_t* b = object_activeList; 

#ifdef DEBUG
  if (!game_collisions) {
    return 0;
  }
#endif

  int16_t a_y = object_y(a);  
  int16_t a_x1 = object_x(a) + a->widthOffset;
  int16_t a_x2 = object_x(a) + (a->width - a->widthOffset);
  
  while (b) {  
    if ((b->class == OBJECT_CLASS_FIGHTER) && b->state == OBJECT_STATE_ALIVE) {
      
      int16_t b_y = object_y(b);

      if (abs(a_y - b_y) <= 1) {
	int16_t b_x1 = object_x(b) + b->widthOffset;
	int16_t b_x2 = object_x(b) + (b->width - b->widthOffset);
	
	if (a_x1 < b_x2 && a_x2 > b_x1) {		  
	  return b;
	}
      }
    }
    b = b->next;
  }
  
  return 0;
}

void
thing_update(uint16_t deltaT, object_t* ptr)
{
  thing_data_t* data = ptr->data;

  if (data->underAttack) {
    if (object_py(ptr) >= data->attack_py && ptr->velocity.y > 0) {
      object_set_py(ptr, data->attack_py);
      ptr->velocity.y = 0;
      ptr->velocity.x = 0;    
      data->underAttack = 0;
      if (!data->bonus && !data->attackable) {
	ptr->state = OBJECT_STATE_REMOVED;
      } else {
	object_set_z(ptr, object_y(ptr));
      }
    } else {
      ptr->velocity.y += deltaT;
    }

    thing_updatePosition(deltaT, ptr);
  } else if (data->bonus) {
    object_t* collision = thing_collision(ptr);
    if (collision) {
      thing_awardBonus(ptr, collision);
    }
  }

  if (object_screenx(ptr) < -ptr->image->w) {
    ptr->state = OBJECT_STATE_REMOVED;
  }
}


object_t*
thing_add(uint16_t id, uint16_t animId, uint16_t brokenId, uint16_t junkStartId, int16_t x, int16_t y)
{
  thing_data_t* data = thing_getFree();
  data->underAttack = 0;
  data->attackable = 1;
  data->hasBonus = 2;
  data->bonus = 0;
  data->brokenId = brokenId;
  data->junkStartId = junkStartId;
  object_t* ptr = object_add(id, OBJECT_CLASS_THING, x, y, 0, animId, thing_update, data, thing_addFree);
  ptr->width = ptr->image->w;
  ptr->widthOffset = 0;
  return ptr;
}


static void
thing_addJunk(object_t* ptr, uint16_t animId, int16_t dx, int16_t yOffset, uint16_t bonus)
{
  thing_data_t* junk = thing_getFree();
  junk->underAttack = 1;
  junk->attackable = 0;
  junk->attack_py = object_py(ptr);
  junk->hasBonus = 0;
  junk->bonus = bonus;
  int16_t x = object_x(ptr) + (dx > 0 ? ptr->image->w : 0);
 
  object_t* jptr = object_add(OBJECT_ID_JUNK, OBJECT_CLASS_JUNK, x, yOffset+object_y(ptr)-40, 0, animId, thing_update, junk, thing_addFree);
  USE(jptr);
  jptr->widthOffset = 0;
  jptr->width = jptr->image->w;
  jptr->velocity.y = -4*OBJECT_PHYSICS_FACTOR;
  jptr->velocity.x = dx*4;
}


void
thing_attack(object_t* ptr, int16_t dx)
{
  thing_data_t* data = ptr->data;
  if (data->attackable) {
    sound_queueSound(SOUND_BUD_PUNCH01);
    if (ptr->animId != data->brokenId) {
      object_setAnim(ptr, data->brokenId);
      thing_addJunk(ptr, data->junkStartId, dx, 0, 0);
      thing_addJunk(ptr, data->junkStartId+1, dx*2, 20, 0);        
      thing_addJunk(ptr, data->junkStartId+2, -dx, 0, 0);          
    } else if (data->hasBonus) {
      if (data->hasBonus == 1) {
	thing_addJunk(ptr, OBJECT_ANIM_BONUS_BURGER, -dx, 0, 1);
      } else if (data->hasBonus == 2) {
	thing_addJunk(ptr, OBJECT_ANIM_BONUS_COLA, dx, 0, 1);
      }
      data->hasBonus--;
    }

    data->underAttack = 1; 
    data->attack_py = object_py(ptr); 
    ptr->velocity.y = -4*OBJECT_PHYSICS_FACTOR;
    ptr->velocity.x = dx;    
  }
}
