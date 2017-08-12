#include "game.h"

#define THING_MAX_THINGS 32

static int16_t thing_count;
static thing_data_t* thing_freeList;
static __SECTION_RANDOM thing_data_t thing_buffer[THING_MAX_THINGS];


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

#ifdef DEBUG
  entry->magicNumber = THING_DATA_MAGIC_NUMBER;
#endif

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
thing_removeBonus(void* _ptr)
{
  object_t* ptr = _ptr;
  object_set_state(ptr,  OBJECT_STATE_REMOVED);
}

void
thing_awardBonus(object_t* ptr, object_t* collision)
{
  if (!thing_data(ptr)->bonus) {
    return;
  }
  if (collision->id == OBJECT_ID_PLAYER1) {
    ptr->velocity.y = -2;
    alarm_add(25, thing_removeBonus, ptr);
    object_set_py_no_checks(ptr, object_py(ptr)-ptr->image->h*OBJECT_PHYSICS_FACTOR);
    object_set_z(ptr, SCREEN_HEIGHT);
    sound_queueSound(SOUND_PICKUP);
    ptr->imageIndex = 0;
    switch (thing_data(ptr)->bonusType) {
    case THING_BONUS_TYPE_HEALTH:
      ptr->image = &object_imageAtlas[SPRITE_HEALTH_COLA];
      break;
    case THING_BONUS_TYPE_POINTS:
      ptr->image = &object_imageAtlas[SPRITE_POINTS_WALLET];
      break;
    }
  } else if (collision->id == OBJECT_ID_PLAYER2) {
    ptr->velocity.y = -2;
    alarm_add(25, thing_removeBonus, ptr);
    object_set_py_no_checks(ptr, object_py(ptr)-ptr->image->h*OBJECT_PHYSICS_FACTOR);
    object_set_z(ptr, SCREEN_HEIGHT);
    sound_queueSound(SOUND_PICKUP);
    ptr->imageIndex = 0;
    switch (thing_data(ptr)->bonusType) {
    case THING_BONUS_TYPE_HEALTH:
      ptr->image = &object_imageAtlas[SPRITE_HEALTH_COLA];
      break;
    case THING_BONUS_TYPE_POINTS:
      ptr->image = &object_imageAtlas[SPRITE_POINTS_WALLET];
      break;
    }
  }

  fighter_data_t* data = fighter_data(collision);
  switch (thing_data(ptr)->bonusType) {
  case THING_BONUS_TYPE_HEALTH:
    data->health += 40;
    break;
  case THING_BONUS_TYPE_POINTS:
    switch (collision->id) {
    case OBJECT_ID_PLAYER1:
      game_player1Score += 10000;
      break;
    case OBJECT_ID_PLAYER2:
      game_player2Score += 10000;
    }
    break;
  }

  thing_data(ptr)->bonus = 0;

  if (data->health > PLAYER_INITIAL_HEALTH) {
    data->health = PLAYER_INITIAL_HEALTH;
  }

  if (collision->id == OBJECT_ID_PLAYER1) {
    game_updatePlayer1Health(GAME_PLAYER1_HEALTH_SCOREBOARD_X, fighter_data(game_player1)->health);
  } else {
    game_updatePlayer2Health(GAME_PLAYER2_HEALTH_SCOREBOARD_X, fighter_data(game_player2)->health);
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

  if (object_y(ptr) > object_z(ptr)+2) { // allow a tiny bounce
    object_set_py_no_checks(ptr, object_z(ptr)*OBJECT_PHYSICS_FACTOR);
  }

  ptr->velocity.dx = object_px(ptr) - lastX;
  ptr->velocity.dy = object_py(ptr) - lastY;


}


object_t*
thing_collision(object_t* a)
{
  object_t* b;

#ifdef DEBUG
  if (!game_collisions) {
    return 0;
  }
#endif

  int16_t a_z = object_z(a);
  int16_t a_x1 = object_x(a) + a->widthOffset;
  int16_t a_x2 = object_x(a) + (a->width - a->widthOffset);

  if (game_player1) {
    b = game_player1;
    //if ((b->class == OBJECT_CLASS_FIGHTER) && object_get_state(b) == OBJECT_STATE_ALIVE) {
      int16_t b_z = object_z(b);
      if (abs(a_z - b_z) <= 2) {
	a_x1 = object_x(a) + a->widthOffset;
	a_x2 = object_x(a) + (a->width - a->widthOffset);
	int16_t b_x1 = object_x(b) + b->widthOffset;
	int16_t b_x2 = object_x(b) + (b->width - b->widthOffset);

	if (a_x1 < b_x2 && a_x2 > b_x1) {
	  return b;
	}
      }
    //}
  }

  if (game_player2) {
    b = game_player2;
    //if ((b->class == OBJECT_CLASS_FIGHTER) && object_get_state(b) == OBJECT_STATE_ALIVE) {
      int16_t b_z = object_z(b);
      if (abs(a_z - b_z) <= 2) {
	int16_t b_x1 = object_x(b) + b->widthOffset;
	int16_t b_x2 = object_x(b) + (b->width - b->widthOffset);

	if (a_x1 < b_x2 && a_x2 > b_x1) {
	  return b;
	}
      }
    //}
  }

  return 0;
}

void
thing_update(uint16_t deltaT, object_t* ptr)
{
  thing_data_t* data = thing_data(ptr);

  if (data->platform && object_get_state(data->platform) == OBJECT_STATE_ABOUT_TO_BE_HIT) {
    object_set_z(ptr, object_y(data->platform));
    ptr->velocity.y = -4*OBJECT_PHYSICS_FACTOR;
    data->platform = 0;
    data->underAttack = 1;
  }

  if (object_get_state(ptr) == OBJECT_STATE_ABOUT_TO_BE_HIT) {
    thing_attack(ptr, ptr->hit.dx);
  }

  if (data->underAttack) {
    if (object_y(ptr) >= object_z(ptr) && ptr->velocity.y > 0) {
      object_set_py(ptr, object_z(ptr)*OBJECT_PHYSICS_FACTOR);
      ptr->velocity.y = 0;
      ptr->velocity.x = 0;
      data->underAttack = 0;
      if (!data->bonus && !data->attackable) {
	object_set_state(ptr, OBJECT_STATE_REMOVED);
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
  } else if (ptr->velocity.y || ptr->velocity.x){
    thing_updatePosition(deltaT, ptr);
  }

  if (object_screenx(ptr) < -ptr->image->w) {
    object_set_state(ptr, OBJECT_STATE_REMOVED);
  }
}


object_t*
thing_add(uint16_t id, uint16_t animId, uint16_t brokenId, uint16_t junkStartId, int16_t x, int16_t y, int16_t numBonus, uint16_t bonusType)
{
  thing_data_t* data = thing_getFree();
  data->platform = 0;
  data->underAttack = 0;
  data->attackable = 1;
  data->hasBonus = numBonus;
  data->bonusType = bonusType;
  data->bonus = 0;
  data->brokenId = brokenId;
  data->junkStartId = junkStartId;
  object_t* ptr = object_add(id, OBJECT_ATTRIBUTE_COLLIDABLE, x, y, 0, animId, thing_update, OBJECT_DATA_TYPE_THING, data, thing_addFree);
  object_set_z(ptr, object_y(ptr));
  ptr->width = ptr->image->w;
  ptr->widthOffset = 0;
  return ptr;
}


//static
object_t*
thing_addJunk(object_t* ptr, uint16_t animId, int16_t dx, int16_t xOffset, int16_t yOffset, uint16_t bonus, uint16_t bonusType)
{
  thing_data_t* junk = thing_getFree();
  junk->underAttack = dx != 0;
  junk->attackable = 0;

  int16_t proposedY = object_y(ptr);

  if (!dx) {
    proposedY = yOffset+object_y(ptr);
  }

  if (proposedY >= PLAYAREA_HEIGHT) {
    proposedY = PLAYAREA_HEIGHT-2;
  } else if (proposedY <= GAME_PAVEMENT_START) {
    proposedY = GAME_PAVEMENT_START+2;
  }

  junk->hasBonus = 0;
  junk->bonus = bonus;
  junk->bonusType = bonusType;
  int16_t x = object_x(ptr) + xOffset + (dx > 0 ? ptr->image->w : 0);

  object_t* jptr = object_add(OBJECT_ID_JUNK, 0, x, yOffset+object_y(ptr), 0, animId, thing_update, OBJECT_DATA_TYPE_THING, junk, thing_addFree);

    object_set_z(jptr, proposedY);
  jptr->widthOffset = 0;
  jptr->width = jptr->image->w;
  if (dx) {
    jptr->velocity.y = -4*OBJECT_PHYSICS_FACTOR;
    jptr->velocity.x = dx*4;
  } else {
    jptr->velocity.y = 0;
    jptr->velocity.x = 0;
  }

  return jptr;
}

static void
thing_addJunks(void* _ptr)
{
  object_t* ptr = _ptr;
  thing_data_t* data = thing_data(ptr);
  thing_addJunk(ptr, data->junkStartId, data->addJunkDx, 0, -40, 0, 0);
  thing_addJunk(ptr, data->junkStartId+1, data->addJunkDx*2, 0, 20-40, 0, 0);
  thing_addJunk(ptr, data->junkStartId+2, -data->addJunkDx, 0, -40, 0, 0);
}


static void
thing_addBonusJunk(void* _ptr)
{
  object_t* ptr = _ptr;
  thing_data_t* data = thing_data(ptr);
  if (data->hasBonus == 1) {
    thing_addJunk(ptr, OBJECT_ANIM_BONUS_BURGER, -data->addJunkDx, 0, -40, 1, THING_BONUS_TYPE_HEALTH);
  } else if (data->hasBonus == 2) {
    thing_addJunk(ptr, OBJECT_ANIM_BONUS_COLA, data->addJunkDx, 0, -40, 1, THING_BONUS_TYPE_HEALTH);
  }
  data->hasBonus--;
}


static void
thing_addBonusPointsJunk(void* _ptr)
{
  object_t* ptr = _ptr;
  thing_data_t* data = thing_data(ptr);
  thing_addJunk(ptr, OBJECT_ANIM_BONUS_WALLET, -data->addJunkDx, 0, -40, 1, THING_BONUS_TYPE_POINTS);
  data->hasBonus--;
}

void
thing_attack(object_t* ptr, int16_t dx)
{
  thing_data_t* data = thing_data(ptr);
  if (!data->underAttack && data->attackable) {
    sound_queueSound(SOUND_BUD_PUNCH01);
    if (ptr->animId != data->brokenId) {
      object_setAnim(ptr, data->brokenId);
      data->addJunkDx  = dx;
      alarm_add(0, thing_addJunks, ptr);
      /* thing_addJunk(ptr, data->junkStartId, dx, 0, 0);
      thing_addJunk(ptr, data->junkStartId+1, dx*2, 20, 0);
      thing_addJunk(ptr, data->junkStartId+2, -dx, 0, 0);          */
    } else if (data->hasBonus) {
      object_setAnim(ptr, data->brokenId);
      data->addJunkDx  = dx;
      if (data->bonusType == THING_BONUS_TYPE_HEALTH) {
	alarm_add(0, thing_addBonusJunk, ptr);
      } else {
	alarm_add(0, thing_addBonusPointsJunk, ptr);
      }
      /* if (data->hasBonus == 1) {
	thing_addJunk(ptr, OBJECT_ANIM_BONUS_BURGER, -dx, 0, 1);
      } else if (data->hasBonus == 2) {
	thing_addJunk(ptr, OBJECT_ANIM_BONUS_COLA, dx, 0, 1);
	}*/
    }

    data->underAttack = 1;
    int16_t r = (random()%6)-3;
    int16_t proposedY = (object_z(ptr) + r);

    if (proposedY >= PLAYAREA_HEIGHT) {
      proposedY = PLAYAREA_HEIGHT-2;
    } else if (proposedY <= GAME_PAVEMENT_START) {
      proposedY = GAME_PAVEMENT_START+2;
    }

    object_set_z(ptr, proposedY);

    ptr->velocity.y = -4*OBJECT_PHYSICS_FACTOR;
    ptr->velocity.x = dx;
  }

  object_set_state(ptr, OBJECT_STATE_ALIVE);
}
