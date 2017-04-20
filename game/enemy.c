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

  int player1Diff = abs(object_x(ptr)-object_x(game_player1)) + abs(object_y(ptr)-object_y(game_player1));
  int player2Diff = abs(object_x(ptr)-object_x(game_player2)) + abs(object_y(ptr)-object_y(game_player2));

  if (player1Diff < player2Diff) {
    return game_player1;
  } else {
    return game_player2;
  }
}

#if 0

static int16_t
enemy_strikingDistanceX(object_t* a, object_t* b)
{
  int16_t a_x = object_x(a) + (a->image->w>>2);
  int16_t b_x = object_x(b) + (b->image->w>>2);

  if (abs(a_x - b_x) > OBJECT_HIT_DISTANCE) {
    return (object_x(a) > object_x(b)) ? 1 : -1;
  }
  
  return 0;
}

#else

static int16_t
enemy_strikingDistanceX(object_t* a, object_t* b)
{
  //  fighter_data_t* a_data = a->data;
  fighter_data_t* b_data = b->data;    
  //  uint16_t thresholdx = b_data->attackRange[OBJECT_PUNCH_LEFT2];
  uint16_t thresholdx = b_data->attackRange[OBJECT_PUNCH_LEFT2];  
    //  int16_t a_widthOffset = a_data->widthOffset;
  //int16_t b_widthOffset = b_data->widthOffset;
  int16_t a_widthOffset = a->widthOffset;
  int16_t b_widthOffset = b->widthOffset;  
  int16_t a_x1 = (((object_px(a)) / OBJECT_PHYSICS_FACTOR) + a_widthOffset);
  int16_t a_x2 = (((object_px(a)) / OBJECT_PHYSICS_FACTOR) + (OBJECT_WIDTH - a_widthOffset));
  int16_t b_x1 = (((object_px(b)) / OBJECT_PHYSICS_FACTOR) + b_widthOffset) - thresholdx;
  int16_t b_x2 = (((object_px(b)) / OBJECT_PHYSICS_FACTOR) + (OBJECT_WIDTH - b_widthOffset)) + thresholdx;
  
  if (a_x1 < b_x2 && a_x2 > b_x1) {
    return 0;
  } else {
    return (object_x(a) > object_x(b)) ? b_data->speed : -b_data->speed;
  }
        
  return 0;
}

#endif


uint16_t
enemy_intelligence(uint16_t deltaT, object_t* ptr, fighter_data_t* data)
{
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
  
  uint16_t attack = 0;
  
  if (data->walkAbout > 0) {
    data->walkAbout-=deltaT;
  } else {
    object_t* player = enemy_closestPlayer(ptr);
    if (!player) {
      ptr->velocity.x = 0;
      ptr->velocity.y = 0;
      return 0;
    }
    object_collision_t collision;

    if (object_x(ptr)-game_cameraX <= 0) {
      data->walkAbout = ENEMY_WALKABOUT_TICS;
      ptr->velocity.x = 1;
    } else if (object_x(ptr)-game_cameraX >= SCREEN_WIDTH) {
      data->walkAbout = ENEMY_WALKABOUT_TICS;
      ptr->velocity.x = -1;      
    } else if ((fighter_collision(deltaT, ptr, &collision, ENEMY_INTERCEPT_X_RANGE, ENEMY_INTERCEPT_Y_THRESHOLD))) {
      if (collision.left) {
	ptr->velocity.x = 1;
	ptr->velocity.y = 0;
      } else if (collision.right) {
	ptr->velocity.x = -1;
	ptr->velocity.y = 0;
      } else if (collision.up) {
	ptr->velocity.x = 0;
	ptr->velocity.y = 1;
      } else {
	ptr->velocity.x = 0;
	ptr->velocity.y = -1;
      }
      data->walkAbout = ENEMY_WALKABOUT_TICS;
    } else {
      ptr->velocity.x = enemy_strikingDistanceX(player, ptr);

      if (abs(object_y(ptr)-object_y(player)) <= data->attackRangeY) {
	if (ptr->velocity.x == 0) {
	  if (data->enemyAttackWait <= 0) {
	    data->enemyAttackWait = data->enemyAttackWaitTics;
	    attack = 1;
	  } else {
	    data->enemyAttackWait-=deltaT;
	  }
	}
	ptr->velocity.y = 0;
      }
      else if (object_y(ptr) < object_y(player)) {
	ptr->velocity.y = data->speed;
      } else if (object_y(ptr) > object_y(player)) {
	ptr->velocity.y = -data->speed;
      } 
    }
  }

  return attack;
}


void NOINLINE
enemy_add(uint16_t x, uint16_t y, uint16_t attackWait, uint16_t attackDuration, uint16_t (*intelligence)(uint16_t deltaT, object_t* ptr, fighter_data_t* data))
{
  if (intelligence == 0) {
    intelligence = enemy_intelligence;
  }
  object_t* ptr =  fighter_add(OBJECT_ID_ENEMY, OBJECT_ANIM_PLAYER1_STAND_RIGHT, x, y, ENEMY_INITIAL_HEALTH, ENEMY_ATTACK_DAMMAGE, intelligence);
  fighter_data_t* data = (fighter_data_t*)ptr->data;
  data->attackDurationTics = attackDuration;
  //  data->widthOffset = (OBJECT_WIDTH-ENEMY_WIDTH)/2;
  ptr->widthOffset = (OBJECT_WIDTH-ENEMY_WIDTH)/2;
  ptr->width = OBJECT_WIDTH;
  data->enemyAttackWaitTics = attackWait;
  data->enemyAttackWait = attackWait;
  data->attackHitAnimTic = 0;
  data->numAttacks = 2;
  enemy_count++;
}

void
enemy_init(void)
{
  enemy_count = 0;
#ifdef DEBUG
  enemy_pause = 0;
#endif
}
