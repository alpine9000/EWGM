#include "game.h"

#define BALOON_MAX_BALOONS 8

static int16_t baloon_count;
static baloon_data_t* baloon_freeList;
static __SECTION_RANDOM baloon_data_t baloon_buffer[BALOON_MAX_BALOONS];


static baloon_data_t*
baloon_getFree(void)
{
  baloon_count++;

#ifdef DEBUG
  if (baloon_count > BALOON_MAX_BALOONS) {
    PANIC("baloon_getFree: empty list");
  }
#endif


  baloon_data_t* entry = baloon_freeList;
  baloon_freeList = baloon_freeList->next;
  if (baloon_freeList) {
    baloon_freeList->prev = 0;
  }

#ifdef DEBUG
  entry->magicNumber = BALOON_DATA_MAGIC_NUMBER;
#endif

  return entry;
}


static void
baloon_addFree(void* data)
{
  baloon_data_t* ptr = data;
  baloon_count--;

  if (baloon_freeList == 0) {
    baloon_freeList = ptr;
    ptr->next = 0;
    ptr->prev = 0;
  } else {
    ptr->next = baloon_freeList;
    ptr->next->prev = ptr;
    ptr->prev = 0;
    baloon_freeList = ptr;
  }
}


void
baloon_init(void)
{
  baloon_count = 0;
  baloon_freeList = &baloon_buffer[0];
  baloon_freeList->prev = 0;
  baloon_data_t* ptr = baloon_freeList;
  for (int16_t i = 1; i < BALOON_MAX_BALOONS; i++) {
      ptr->next = &baloon_buffer[i];
      ptr->next->prev = ptr;
      ptr = ptr->next;
  }
  ptr->next = 0;
}


void
baloon_updatePosition(uint16_t deltaT, object_t* ptr)
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

  //  if (object_y(ptr) >= object_z(ptr)) {
  //    object_set_py_no_checks(ptr, object_z(ptr)*OBJECT_PHYSICS_FACTOR);
  //  }

  ptr->velocity.dx = object_px(ptr) - lastX;
  ptr->velocity.dy = object_py(ptr) - lastY;


}


void
baloon_update(uint16_t deltaT, object_t* ptr)
{
  baloon_data_t* data = baloon_data(ptr);
  object_collision_t collision;

  data->frameCount += deltaT;

  if (data->frameCount >= 10 && object_collision(deltaT, ptr, &collision, ENEMY_INTERCEPT_X_RANGE+1, ENEMY_INTERCEPT_Y_THRESHOLD*2)) {
    if (collision.left) {
      baloon_attack(collision.left, ptr, 1);
    } else if (collision.right) {
      baloon_attack(collision.right, ptr, -1);
    }
  }
  if (data->underAttack) {
    if (object_y(ptr) >= object_z(ptr) && ptr->velocity.y > 0) {
      object_set_py(ptr, object_z(ptr)*OBJECT_PHYSICS_FACTOR);
      ptr->velocity.y = 0;
      ptr->velocity.x = 0;
      data->underAttack = 0;
      object_set_z(ptr, object_y(ptr));
      object_setAnim(ptr, OBJECT_ANIM_BALOON_WAIT);
    } else {
      if (data->frameCount >= 10) {
	//	ptr->velocity.x = ptr->velocity.x / 2;
	if (ptr->velocity.x > 0) {
	  ptr->velocity.x--;
	} else if (ptr->velocity.x < 0){
	  ptr->velocity.x++;
	}
	data->frameCount = 0;
      }

      ptr->velocity.y += deltaT;
      if (ptr->velocity.y > 1) {
	ptr->velocity.y = 1;
      }
    }

    baloon_updatePosition(deltaT, ptr);

  } else if (ptr->velocity.y || ptr->velocity.x){
    baloon_updatePosition(deltaT, ptr);
  }

  if (object_screenx(ptr) < -ptr->image->w) {
    object_set_state(ptr, OBJECT_STATE_REMOVED);
  }
}


object_t*
baloon_add(int16_t x, int16_t y)
{
  baloon_data_t* data = baloon_getFree();
  data->underAttack = 0;
  data->frameCount = 0;
  object_t* ptr = object_add(OBJECT_ID_BALOON, 0, x, y, 0, OBJECT_ANIM_BALOON_WAIT, baloon_update, OBJECT_DATA_TYPE_BALOON, data, baloon_addFree);
  object_set_z(ptr, object_y(ptr));
  ptr->width = ptr->image->w;
  ptr->widthOffset = 0;
  return ptr;
}


void
baloon_attack(object_t* attacker, object_t* ptr, int16_t dx)
{
  if (attacker->id == OBJECT_ID_BALOON) {
    return;
  }
  baloon_data_t* data = baloon_data(ptr);
  if (!data->underAttack) {
    //    sound_queueSound(SOUND_BUD_PUNCH01);

    object_setAnim(ptr, OBJECT_ANIM_BALOON);
    data->underAttack = 1;
    int16_t r = (random()%6)-3;
    int16_t proposedY = (object_z(ptr) + r);

    if (proposedY >= PLAYAREA_HEIGHT) {
      proposedY = PLAYAREA_HEIGHT-2;
    } else if (proposedY <= GAME_PAVEMENT_START) {
      proposedY = GAME_PAVEMENT_START+2;
    }

    object_set_z(ptr, proposedY);

    ptr->velocity.y = -8*OBJECT_PHYSICS_FACTOR;
    ptr->velocity.x = 4*dx;
  }

  object_set_state(ptr, OBJECT_STATE_ALIVE);
}
