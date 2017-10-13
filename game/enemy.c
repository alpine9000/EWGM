#include "game.h"

uint16_t enemy_count;
#ifdef DEBUG
uint16_t enemy_pause;
#endif

object_t*
enemy_closestPlayer(object_t* ptr)
{
  if (game_player2 == 0) {
    if (game_player1) {
      return game_player1;
    } else {
      return 0;
    }
  } else if (game_player1 == 0) {
    return game_player2;
  }

  if (object_get_state(game_player1) == OBJECT_STATE_REMOVED &&
      object_get_state(game_player2) != OBJECT_STATE_REMOVED) {
    return game_player2;
  } else  if (object_get_state(game_player2) == OBJECT_STATE_REMOVED &&
	      object_get_state(game_player1) != OBJECT_STATE_REMOVED) {
    return game_player1;
  } else if (object_get_state(game_player2) == OBJECT_STATE_REMOVED &&
	     object_get_state(game_player1) == OBJECT_STATE_REMOVED) {
    return 0;
  }

  int32_t player1Diff = abs(object_x(ptr)-object_x(game_player1)) + abs(object_y(ptr)-object_y(game_player1));
  int32_t player2Diff = abs(object_x(ptr)-object_x(game_player2)) + abs(object_y(ptr)-object_y(game_player2));

  if (player1Diff < player2Diff) {
    return game_player1;
  } else {
    return game_player2;
  }
}


static uint16_t
enemy_attackSolution(object_t* player, object_t* enemy, __UNUSED uint16_t deltaT)
{
  fighter_data_t* enemyData = fighter_data(enemy);
  uint16_t thresholdx;

#if 1
  if (enemyData->numAttacks == 1) {
    thresholdx = enemyData->attackConfig[OBJECT_PUNCH_LEFT1].rangeX;
  } else {
    thresholdx = max(enemyData->attackConfig[OBJECT_PUNCH_LEFT1].rangeX, enemyData->attackConfig[OBJECT_PUNCH_LEFT2].rangeX);
  }
#else
  thresholdx = enemyData->attackConfig[enemy->actionId].rangeX;
#endif


  int16_t a_x1 = (((object_x(player))) + player->widthOffset);
  int16_t a_x2 = (((object_x(player))) + (player->width - player->widthOffset));

  int16_t b_x1 = (((object_x(enemy))) + enemy->widthOffset);//-thresholdx;
  int16_t b_x2 = ((object_x(enemy))) + (enemy->width - enemy->widthOffset);// + thresholdx;

  if (enemy->anim->facing == FACING_RIGHT) {
    if (!(enemy->attributes & OBJECT_ATTRIBUTE_PROJECTILE_LAUNCHING_ENEMY)) {
      b_x1 += thresholdx;
    }
    b_x2 += thresholdx;
  } else {
    b_x1 -= thresholdx;
    if (!(enemy->attributes & OBJECT_ATTRIBUTE_PROJECTILE_LAUNCHING_ENEMY)) {
      b_x2 -= thresholdx;
    }
  }

  int16_t deltaY = object_z(player)-object_z(enemy);

  int16_t deltaX = 0;

  //  if (a_x1 < b_x2 && a_x2 > b_x1) {

  if (a_x1 >= b_x2) {   /*  beeeeeeb  appppppa */
    deltaX = (a_x1-b_x2)+1;
  } else if (a_x2 <= b_x1) {   /* appppppa beeeeeeb */
    deltaX = -(b_x1-a_x2)-1;
  }

  int16_t solution = 1;

  if (abs(deltaY) <= enemyData->attackRangeY) {
    enemy->velocity.y = 0;
  } else {
    if (abs(deltaY) > enemyData->speedY) {
      enemy->velocity.y = deltaY > 0 ? enemyData->speedY : -enemyData->speedY;
    } else {
      enemy->velocity.y = deltaY;
    }
    solution = 0;
  }

  if (a_x1 < b_x2 && a_x2 > b_x1) {
    enemy->velocity.x = 0;
  } else {
    if (abs(deltaX) > enemyData->speedX) {
      enemy->velocity.x = deltaX > 0 ? enemyData->speedX : -enemyData->speedX;
    } else {
      enemy->velocity.x = deltaX;
    }
    solution = 0;
  }

  if (solution) {
    if ((deltaX > 0 && (enemy->anim->facing == FACING_LEFT)) ||
	(deltaX < 0 && (enemy->anim->facing == FACING_RIGHT))) {
      enemy->velocity.x = enemy->anim->facing == FACING_LEFT ? 1 : -1;
      solution = 0;
    }
  }

  return solution;
}


uint16_t
enemy_intelligence(uint16_t deltaT, object_t* ptr, fighter_data_t* data)
{
  uint16_t attack = 0;

  if (data->attackCount != 0) {
    return 0;
  }

  if (object_get_state(ptr) != OBJECT_STATE_ALIVE) {
    return 0;
  }

#ifdef DEBUG
  if (enemy_pause) {
    ptr->velocity.x = 0;
    ptr->velocity.y = 0;
    return 0;
  }
#endif

  uint32_t rand = random();
  object_t* player = enemy_closestPlayer(ptr);
  if (!player) {
    ptr->velocity.x = 0;
    ptr->velocity.y = 0;
    return 0;
  }
  object_collision_t collision;

  if (object_x(ptr)-game_cameraX <= 0) {
    data->walkAbout = -(object_x(ptr)-game_cameraX)+1;//rand & data->walkAboutMask;//0x7f; //ENEMY_WALKABOUT_TICS;
    ptr->velocity.x = 1;
  } else if (object_x(ptr)-game_cameraX >= SCREEN_WIDTH) {
    data->walkAbout = (object_x(ptr)-game_cameraX )+1;//rand & data->walkAboutMask;//0x7f; //ENEMY_WALKABOUT_TICS;
    ptr->velocity.x = -1;
  } else {
    int16_t oldX = ptr->velocity.x;
    int16_t oldY = ptr->velocity.y;
    attack = enemy_attackSolution(player, ptr, deltaT);

    if (attack) {
      if (data->attackWait > 0) {
	data->attackWait-=deltaT;
	attack = 0;
      }
    } else {
      if (data->walkAbout > 0) {
	ptr->velocity.x = oldX;
	ptr->velocity.y = oldY;
	data->walkAbout-=deltaT;
	if (object_y(ptr) >= PLAYAREA_HEIGHT-1) {
	  ptr->velocity.y = 0;
	}
      } else if ((object_collision(deltaT, ptr, &collision, ENEMY_INTERCEPT_X_RANGE, ENEMY_INTERCEPT_Y_THRESHOLD*2))) {
	switch (rand & 0x7) {
	case 0:
	  attack = 1;
	  break;
	case 1:
	  ptr->velocity.x = 0;
	  ptr->velocity.y = 0;
	  break;
	case 2:
	  ptr->velocity.x = -data->speedX;
	  ptr->velocity.y = 0;
	  break;
	case 3:
	  ptr->velocity.x = data->speedX;
	  ptr->velocity.y = 0;
	  break;
	case 4:
	  ptr->velocity.x = -data->speedX;
	  ptr->velocity.y = -data->speedY;
	  break;
	case 5:
	  ptr->velocity.x = -data->speedX;
	  ptr->velocity.y = data->speedY;
	  break;
	case 6:
	  ptr->velocity.x = data->speedX;
	  ptr->velocity.y = data->speedY;
	  break;
	case 7:
	  ptr->velocity.x = data->speedX;
	  ptr->velocity.y = -data->speedY;
	  break;
	}
	data->walkAbout = rand & 0x7f;//ENEMY_WALKABOUT_TICS;
      } else {
	if ((rand & data->randomFrequencyMask) == 0) {
	  data->walkAbout = rand & data->randomDistanceMask;

	  switch (rand & 0x3) {
	  case 0:
	    if (ptr->velocity.y != 0) {
	      ptr->velocity.x = 0;
	    }
	    break;
	  case 1:
	    if (ptr->velocity.x != 0) {
	      ptr->velocity.y = 0;
	    }
	    break;
	  default:
	    break;
	  }
	}
      }
    }
  }

  return attack;
}


object_t* __NOINLINE
enemy_add(uint16_t animId, uint16_t attributes, uint16_t x, uint16_t y, enemy_config_t* config)
{
  uint16_t (*intelligence)(uint16_t deltaT, object_t* ptr, fighter_data_t* data)  = config->intelligence;
  if (intelligence == 0) {
    intelligence = enemy_intelligence;
  }
  if (config->initialHealth == 0) {
    config->initialHealth = ENEMY_INITIAL_HEALTH;
  }
  object_t* ptr =  fighter_add(OBJECT_ID_ENEMY, attributes, animId, x, y, config->initialHealth, config->attackConfig, intelligence);
  fighter_data_t* data = fighter_data(ptr);
  ptr->widthOffset = (OBJECT_WIDTH-ENEMY_WIDTH)/2;
  ptr->width = OBJECT_WIDTH;

  data->minAttackWaitTics = config->minAttackWait;
  data->maxAttackWaitTics = config->maxAttackWait;
  uint16_t rn = random();
  if (data->maxAttackWaitTics) {
    data->attackWait = rn % data->maxAttackWaitTics;// + data->attackCount;
    if (data->attackWait < data->minAttackWaitTics) {
      data->attackWait = data->minAttackWaitTics;
    }
  } else {
    data->attackWait = 0;
  }
  data->randomDistanceMask = config->randomDistanceMask;
  data->randomFrequencyMask = config->randomFrequencyMask;
  data->speedX = config->speedX;
  data->speedY = config->speedY;
  data->numAttacks = config->numAttacks;
  data->attackRangeY = config->attackRangeY;
  data->postAttackInvincibleTics = config->postAttackInvincibleTics;

  if (!(ptr->attributes & OBJECT_ATTRIBUTE_IMMOVABLE)) {
    enemy_count++;
  }
  return ptr;
}


void
enemy_init(void)
{
  enemy_count = 0;
#ifdef DEBUG
  enemy_pause = 0;
#endif
}
