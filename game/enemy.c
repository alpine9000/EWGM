#include "game.h"


uint16_t enemy_count;

#define ENEMY_BOSS_START_Y (7+56)
#define ENEMY_BOSS_START_X (WORLD_WIDTH-96)

object_t*
enemy_closestPlayer(object_t* ptr)
{
  if (game_player2 == 0) {
    if (game_player1->state != OBJECT_STATE_REMOVED) {
      return game_player1;
    } else {
      return 0;
    }
  }

  if (game_player1->state == OBJECT_STATE_REMOVED &&
      game_player2->state != OBJECT_STATE_REMOVED) {
    return game_player2;
  } else  if (game_player2->state == OBJECT_STATE_REMOVED &&
	      game_player1->state != OBJECT_STATE_REMOVED) {
    return game_player1;
  } else if (game_player2->state == OBJECT_STATE_REMOVED &&
	      game_player1->state == OBJECT_STATE_REMOVED) {
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

      if (abs(object_y(ptr)-object_y(player)) <= FIGHTER_ENEMY_Y_ATTACK_RANGE) {
	if (ptr->velocity.x == 0) {
	  if (data->enemyAttackWait <= 0) {
	    data->enemyAttackWait = ENEMY_ATTACK_WAIT_TICS;
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
enemy_add(uint16_t x, uint16_t y, uint16_t (*intelligence)(uint16_t deltaT, object_t* ptr, fighter_data_t* data))
{
  if (intelligence == 0) {
    intelligence = enemy_intelligence;
  }
  object_t* ptr =  fighter_add(OBJECT_ID_ENEMY, OBJECT_ANIM_PLAYER1_STAND_RIGHT, x, y, ENEMY_INITIAL_HEALTH, ENEMY_ATTACK_DAMMAGE, intelligence);
  fighter_data_t* data = (fighter_data_t*)ptr->data;
  data->attackDurationFrames = ENEMY_ATTACK_DURATION_TICS;
  data->widthOffset = (OBJECT_WIDTH-ENEMY_WIDTH)/2;
  data->enemyAttackWait = ENEMY_ATTACK_WAIT_TICS;
  data->attackHitAnimTic = 0;
  data->numAttacks = 2;
  enemy_count++;
}


uint16_t
enemy_doorIntelligence(uint16_t deltaT, object_t* ptr, fighter_data_t* data)
{
  if (object_y(ptr) == ENEMY_BOSS_START_Y && object_x(ptr) > WORLD_WIDTH-144) {
    ptr->velocity.x = -1;
  } else if (object_y(ptr) < GAME_PAVEMENT_START) {
    ptr->velocity.x = 0;
    ptr->velocity.y = 1;
  } else {
    return enemy_intelligence(deltaT, ptr, data);
  }
  
  return 0;
}


void
enemy_addBoss(uint16_t x, uint16_t y)
{
  object_t* ptr =  fighter_add(OBJECT_ID_ENEMY, OBJECT_ANIM_BOSS_STAND_RIGHT, x, y, ENEMY_INITIAL_HEALTH, ENEMY_ATTACK_DAMMAGE, enemy_doorIntelligence);
  fighter_data_t* data = (fighter_data_t*)ptr->data;
  data->attackDurationFrames = ENEMY_ATTACK_DURATION_TICS;
  data->widthOffset = (OBJECT_WIDTH-ENEMY_WIDTH)/2;
  data->enemyAttackWait = ENEMY_BOSS_ATTACK_TICS_PER_FRAME*ENEMY_BOSS_NUM_ATTACK_FRAMES;
  data->attackHitAnimTic = ENEMY_BOSS_ATTACK_TICS_PER_FRAME;
  data->numAttacks = 1;
  enemy_count++;
}


void
enemy_addDoorEnemy(void)
{
  enemy_add(ENEMY_BOSS_START_X, ENEMY_BOSS_START_Y, enemy_doorIntelligence);    
}


void
enemy_init(void)
{
  enemy_count = 0;
}


void
enemy_wave1(void)
{
  enemy_add(game_cameraX-64, 85, 0);
  enemy_add(game_cameraX-64, 185, 0);
  enemy_add(game_cameraX+SCREEN_WIDTH+64, 85, 0);
  enemy_add(game_cameraX+SCREEN_WIDTH+64, 185, 0);
  
  enemy_add(game_cameraX+64, 85, 0);
  enemy_add(game_cameraX+64, 185, 0);  
}


void
enemy_wave2(void)
{  
  enemy_add(game_cameraX-64, 85, 0);  
}


void
enemy_wave3(void)
{
  uint16_t x = ENEMY_BOSS_START_X;

  object_t* door =  object_add(/*id*/OBJECT_ID_DOOR,
			       /*x*/x,
			       /*y*/64,
			       /*dx*/0,
			       /*anim id*/OBJECT_ANIM_DOOR,
			       /*update*/0,
			       /*data*/0,
			       /*freeData*/0);
  door->tileRender = 1;
  
  enemy_addBoss(x, ENEMY_BOSS_START_Y);

  alarm_add(200, enemy_addDoorEnemy);
  alarm_add(300, enemy_addDoorEnemy);  
}
