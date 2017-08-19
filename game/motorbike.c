#include "game.h"

#define MOTORBIKE_SPEED 4
enum {
  MOTORBIKE_ENTERING_SCREEN,
  MOTORBIKE_WAIT,
  MOTORBIKE_GO,
  MOTORBIKE_POST_ATTACK,
};

static uint16_t motorbike_state = MOTORBIKE_ENTERING_SCREEN;
static uint16_t motorbike_waitCount;
static fighter_data_t motorbike_data;

static uint16_t
level1_motorbikeIntelligence(uint16_t deltaT, object_t* ptr, __UNUSED fighter_data_t* data)
{
  switch (object_get_state(ptr)) {
  case OBJECT_STATE_ALIVE:
  case OBJECT_STATE_HIT:
    if ((object_screenx(ptr) >= SCREEN_WIDTH && ptr->velocity.x > 0) || (object_screenx(ptr) < -40 && ptr->velocity.x < 0)) {
      sound_endLoop();
    }

    if (object_screenx(ptr) >= SCREEN_WIDTH+160) {
      object_t* player = enemy_closestPlayer(ptr);
      object_set_state(ptr, OBJECT_STATE_ALIVE);
      ptr->velocity.x = -MOTORBIKE_SPEED;
      motorbike_state = MOTORBIKE_ENTERING_SCREEN;
      if (player) {
	if (abs(object_x(player)-object_x(ptr)) < 200) {
	  if (object_y(player) > SCREEN_HEIGHT/2) {
	    object_set_py(ptr, (GAME_PAVEMENT_START+2)*OBJECT_PHYSICS_FACTOR);
	  } else {
	    object_set_py(ptr, (SCREEN_HEIGHT-10)*OBJECT_PHYSICS_FACTOR);
	  }
	  ptr->velocity.x = -MOTORBIKE_SPEED*2;
	  motorbike_state = MOTORBIKE_GO;
	} else {
	  object_set_py_no_checks(ptr, object_py(player));
	}
      }
      sound_queueSound(SOUND_MOTORBIKE);
    } else if (object_screenx(ptr) <= -160) {
      object_t* player = enemy_closestPlayer(ptr);
      object_set_state(ptr, OBJECT_STATE_ALIVE);
      ptr->velocity.x = MOTORBIKE_SPEED;
      motorbike_state = MOTORBIKE_ENTERING_SCREEN;
      if (player) {
	if (abs(object_x(player)-object_x(ptr)) < 200) {
	  if (object_y(player) > SCREEN_HEIGHT/2) {
	    object_set_py(ptr, (GAME_PAVEMENT_START+2)*OBJECT_PHYSICS_FACTOR);
	  } else {
	    object_set_py(ptr, (SCREEN_HEIGHT-10)*OBJECT_PHYSICS_FACTOR);
	  }
	  ptr->velocity.x = MOTORBIKE_SPEED*2;
	  motorbike_state = MOTORBIKE_GO;
	} else {
	  object_set_py_no_checks(ptr, object_py(player));
	}
      }
      sound_queueSound(SOUND_MOTORBIKE);
    } else if (motorbike_state == MOTORBIKE_ENTERING_SCREEN) {
      if (object_screenx(ptr) <= SCREEN_WIDTH-40 && ptr->velocity.x < 0) {
	ptr->velocity.x = 0;
	motorbike_state = MOTORBIKE_WAIT;
	ptr->imageIndex = ptr->anim->start;
	motorbike_waitCount = 100;
	sound_endLoop();
      } else if (object_screenx(ptr) >= -40 && ptr->velocity.x > 0) {
	ptr->velocity.x	= 0;
	motorbike_state = MOTORBIKE_WAIT;
	motorbike_waitCount = 100;
	sound_endLoop();
      }
    } else if (motorbike_state == MOTORBIKE_WAIT) {
      motorbike_waitCount = motorbike_waitCount - deltaT;
      if (motorbike_waitCount <= 0) {
	motorbike_state = MOTORBIKE_GO;
	sound_queueSound(SOUND_MOTORBIKE);
	ptr->velocity.x = object_screenx(ptr) < 0 ? MOTORBIKE_SPEED : -MOTORBIKE_SPEED;
      } else {
	int16_t velx = object_screenx(ptr) < 0 ? 2 : -2;
	if (motorbike_waitCount == 30*2) {
	  object_set_py(ptr, object_py(ptr)-2*OBJECT_PHYSICS_FACTOR);
	  object_set_px(ptr, object_px(ptr)+velx*OBJECT_PHYSICS_FACTOR);
	  sound_queueSound(SOUND_REV);
	} else if (motorbike_waitCount == 28*2) {
	  object_set_py(ptr, object_py(ptr)+2*OBJECT_PHYSICS_FACTOR);
	  object_set_px(ptr, object_px(ptr)+velx*OBJECT_PHYSICS_FACTOR);
	  object_updateAnimation(100, ptr);
	} else if (motorbike_waitCount == 15*2) {
	  object_set_py(ptr, object_py(ptr)-2*OBJECT_PHYSICS_FACTOR);
	  object_set_px(ptr, object_px(ptr)+velx*OBJECT_PHYSICS_FACTOR);
	} else if (motorbike_waitCount == 13*2) {
	  object_set_py(ptr, object_py(ptr)+2*OBJECT_PHYSICS_FACTOR);
	  object_set_px(ptr, object_px(ptr)+velx*OBJECT_PHYSICS_FACTOR);
	  object_updateAnimation(100, ptr);
	  sound_queueSound(SOUND_REV);
	} else {
	  ptr->imageIndex = ptr->anim->start;
	}
      }
    }
    break;
  }

  return 0;
}

static object_t*
motorbike_collision(object_t* a)
{
  object_t* b;

#ifdef DEBUG
  if (!game_collisions) {
    return 0;
  }
#endif

  if (a->velocity.x == 0) {
    return 0;
  }

  int16_t a_y = object_y(a);
  int16_t a_x1 = object_x(a) + a->widthOffset;
  int16_t a_x2 = object_x(a) + (a->width - a->widthOffset);

  if (game_player1) {
    b = game_player1;
    int16_t b_y = object_y(b);
    if (abs(a_y - b_y) <= 5) {
      a_x1 = object_x(a) + a->widthOffset;
      a_x2 = object_x(a) + (a->width - a->widthOffset);
      int16_t b_x1 = object_x(b) + b->widthOffset;
      int16_t b_x2 = object_x(b) + (b->width - b->widthOffset);

      if (a_x1 < b_x2 && a_x2 > b_x1) {
	return b;
      }
    }
  }

  if (game_player2) {
    b = game_player2;
    int16_t b_y = object_y(b);
    if (abs(a_y - b_y) <= 5) {
      int16_t b_x1 = object_x(b) + b->widthOffset;
      int16_t b_x2 = object_x(b) + (b->width - b->widthOffset);

      if (a_x1 < b_x2 && a_x2 > b_x1) {
	return b;
      }
    }
  }

  return 0;
}


static void
motorbike_update(uint16_t deltaT, object_t* ptr)
{
  fighter_data_t* data = fighter_data(ptr);
  level1_motorbikeIntelligence(deltaT, ptr, 0);

  if ((object_get_state(ptr) == OBJECT_STATE_HIT ||
      object_get_state(ptr) == OBJECT_STATE_FLASHING) && motorbike_state == MOTORBIKE_POST_ATTACK) {
    if (object_y(ptr) >= object_z(ptr) && ptr->velocity.y > 0) {
      object_set_py_no_checks(ptr, object_z(ptr)*OBJECT_PHYSICS_FACTOR);
      ptr->velocity.y = 0;
      if (object_get_state(ptr) != OBJECT_STATE_FLASHING) {
	motorbike_state = MOTORBIKE_GO;
      }
    } else {
      ptr->velocity.y += deltaT;
    }
  }

  object_updatePositionNoChecks(deltaT, ptr);

  switch (object_get_state(ptr)) {
  case OBJECT_STATE_ALIVE:
    if (motorbike_state != MOTORBIKE_WAIT) {
      fighter_updateSprite(ptr);
    }
    break;
  case OBJECT_STATE_FLASHING:
    if (data->flashCount <= 0) {
      ptr->visible = !ptr->visible;
      data->flashCount = FIGHTER_HIT_FLASH_COUNT_TICS;
    }
    data->flashCount -= deltaT;
    data->flashDurationTics -= deltaT;
    if (data->flashDurationTics <= 0) {
      fighter_die(ptr);
    }
    break;
  default:
    return;
  }


  object_t* player = motorbike_collision(ptr);

  if (player) {
    if (motorbike_state != MOTORBIKE_POST_ATTACK && (player->actionId == OBJECT_KICK_LEFT || player->actionId == OBJECT_KICK_RIGHT)) {
      data->health -= 50;
      star_add(ptr, ptr->velocity.x);
      object_set_z(ptr, object_y(ptr));
      motorbike_state = MOTORBIKE_POST_ATTACK;
      if (data->health <= 0) {
	ptr->velocity.y = -16;
	sound_endLoop();
	object_set_state(ptr, OBJECT_STATE_FLASHING);
	if (ptr->velocity.x > 0) {
	  object_setAnim(ptr, OBJECT_ANIM_MOTORBIKE_BROKEN_RIGHT);
	} else {
	  object_setAnim(ptr, OBJECT_ANIM_MOTORBIKE_BROKEN_LEFT);
	}
	data->flashCount = FIGHTER_HIT_FLASH_COUNT_TICS;
	data->flashDurationTics = FIGHTER_HIT_FLASH_DURATION_TICS;
	sound_queueSound(SOUND_DIE01);
      } else {
	ptr->velocity.y = -8;
	object_set_state(ptr, OBJECT_STATE_HIT);
	if (ptr->velocity.x < 0) {
	  object_setAction(ptr, OBJECT_HIT_LEFT);
	} else {
	  object_setAction(ptr, OBJECT_HIT_RIGHT);
	}

	fighter_data_t* playerData = fighter_data(player);
	playerData->hitEnemyCallback(player, ptr);
      }
    } else if (motorbike_state == MOTORBIKE_GO && player) {
      motorbike_state = MOTORBIKE_POST_ATTACK;
      player->hit.attacker = ptr;
      player->hit.dammage = 25;
      player->hit.dx = ptr->velocity.x;
      object_set_state(player, OBJECT_STATE_ABOUT_TO_BE_HIT);
    }
  }
}


static void
motorbike_hitEnemyCallback(__UNUSED object_t* me, __UNUSED object_t* victim)
{
  sound_queueSound(SOUND_ENEMY_PUNCH01);
}

static void
motorbike_killEnemyCallback(__UNUSED object_t* me, __UNUSED object_t* victim)
{
  sound_queueSound(SOUND_DIE03);
}


void
motorbike_add(int16_t x, int16_t y)
{
  object_t* ptr = object_add(OBJECT_ID_ENEMY, /*OBJECT_ATTRIBUTE_COLLIDABLE*/0, x, y, 0, OBJECT_ANIM_MOTORBIKE_STAND_RIGHT, motorbike_update, OBJECT_DATA_TYPE_FIGHTER, &motorbike_data, 0);
#ifdef DEBUG
  motorbike_data.magicNumber = FIGHTER_DATA_MAGIC_NUMBER;
#endif
  motorbike_data.postAttackCount = 0;
  motorbike_data.health = 100;
  motorbike_data.hitEnemyCallback = motorbike_hitEnemyCallback;
  motorbike_data.killEnemyCallback = motorbike_killEnemyCallback;
  motorbike_data.dieCallback = fighter_dieCallback;
  ptr->width = ptr->image->w;
  ptr->widthOffset = 0;
  enemy_count++;
}
