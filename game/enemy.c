#include "game.h"

#define ENEMY_ATTACK_DURATION_FRAMES 20

static object_t* enemy_player1;
static object_t* enemy_player2;

uint16_t enemy_count;

object_t*
enemy_closestPlayer(object_t* ptr)
{
  if (enemy_player2 == 0) {
    if (enemy_player1->state != OBJECT_STATE_REMOVED) {
      return enemy_player1;
    } else {
      return 0;
    }
  }

  if (enemy_player1->state == OBJECT_STATE_REMOVED &&
      enemy_player2->state != OBJECT_STATE_REMOVED) {
    return enemy_player2;
  } else  if (enemy_player2->state == OBJECT_STATE_REMOVED &&
	      enemy_player1->state != OBJECT_STATE_REMOVED) {
    return enemy_player1;
  } else if (enemy_player2->state == OBJECT_STATE_REMOVED &&
	      enemy_player1->state == OBJECT_STATE_REMOVED) {
    return 0;
  }

  int player1Diff = abs(object_x(ptr)-object_x(enemy_player1)) + abs(object_y(ptr)-object_y(enemy_player1));
  int player2Diff = abs(object_x(ptr)-object_x(enemy_player2)) + abs(object_y(ptr)-object_y(enemy_player2));

  if (player1Diff < player2Diff) {
    return enemy_player1;
  } else {
    return enemy_player2;
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
  fighter_data_t* a_data = a->data;
  fighter_data_t* b_data = b->data;    
  uint16_t thresholdx = FIGHTER_SHORT_PUNCH_RANGE;
  int16_t a_widthOffset = a_data->widthOffset;
  int16_t b_widthOffset = b_data->widthOffset;
  int16_t a_x1 = (((object_px(a)) / OBJECT_PHYSICS_FACTOR) + a_widthOffset)-thresholdx;
  int16_t a_x2 = (((object_px(a)) / OBJECT_PHYSICS_FACTOR) + (OBJECT_WIDTH - a_widthOffset)) + thresholdx;
  int16_t b_x1 = ((object_px(b)) / OBJECT_PHYSICS_FACTOR) + b_widthOffset;
  int16_t b_x2 = ((object_px(b)) / OBJECT_PHYSICS_FACTOR) + (OBJECT_WIDTH - b_widthOffset);
  
  if (a_x1 < b_x2 && a_x2 > b_x1) {
    return 0;
  } else {
    return (object_x(a) > object_x(b)) ? 1 : -1;
  }
        
  return 0;
}

#endif


uint16_t
enemy_intelligence(uint16_t deltaT, object_t* ptr, fighter_data_t* data)
{
 uint16_t attack = 0;
  
  if (data->walkAbout) {
    data->walkAbout--;
  } else {
    object_t* player = enemy_closestPlayer(ptr);
    if (!player) {
      ptr->velocity.x = 0;
      ptr->velocity.y = 0;
      return 0;
    }
    object_collision_t collision;

    if ((fighter_collision(deltaT, ptr, &collision, 20, 1))) {
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
      data->walkAbout = 50;
    } else {
      ptr->velocity.x = enemy_strikingDistanceX(player, ptr);

      if (abs(object_y(ptr)-object_y(player)) <= FIGHTER_ENEMY_Y_ATTACK_RANGE) {
	if (ptr->velocity.x == 0) {
	  if (data->enemyAttackWait <= 0) {
	    data->enemyAttackWait = ENEMY_ATTACK_WAIT_FRAMES;
	    attack = 1;
	  } else {
	    data->enemyAttackWait-=deltaT;
	  }
	}
	ptr->velocity.y = 0;
      }
      else if (object_y(ptr) < object_y(player)) {
	ptr->velocity.y = 1;
      } else if (object_y(ptr) > object_y(player)) {
	ptr->velocity.y = -1;
      } 
    }
  }

  return attack;
}


void
enemy_add(uint16_t x, uint16_t y)
{
  object_t* ptr =  fighter_add(0, OBJECT_ANIM_PLAYER1_STAND_RIGHT, x, y, 100, 0, enemy_intelligence);
  fighter_data_t* data = (fighter_data_t*)ptr->data;
  data->attackDurationFrames = ENEMY_ATTACK_DURATION_FRAMES;
  data->widthOffset = (OBJECT_WIDTH-22)/2;
  data->enemyAttackWait = ENEMY_ATTACK_WAIT_FRAMES;
  enemy_count++;
}

void
enemy_init(object_t* player1, object_t * player2)
{
  enemy_count = 0;
  enemy_player1 = player1;
  enemy_player2 = player2;

  if (1) {
    if (player1) {
      enemy_add(game_cameraX-64, 85);
      //enemy_add(game_cameraX-132, 120);    
    }
    
    if (player2) {
      enemy_add(game_cameraX+SCREEN_WIDTH+64, 85);
      //      enemy_add(game_cameraX+SCREEN_WIDTH+132, 200);
      //      enemy_add(game_cameraX+SCREEN_WIDTH+160, 128);
    }
  }
}
