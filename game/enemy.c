#include "game.h"

static object_t* enemy_player1;
static object_t* enemy_player2;


object_t*
enemy_closestPlayer(object_t* ptr)
{
  if (enemy_player2 == 0) {
    return enemy_player1;
  }

  int player1Diff = abs(ptr->x-enemy_player1->x) + abs(ptr->y-enemy_player1->y);
  int player2Diff = abs(ptr->x-enemy_player2->x) + abs(ptr->y-enemy_player2->y);

  if (player1Diff < player2Diff) {
    return enemy_player1;
  } else {
    return enemy_player2;
  }
}

void
object_updateEnemy(object_t* ptr)
{  
  if (ptr->walkAbout) {
    ptr->walkAbout--;
  } else {
    object_t* player = enemy_closestPlayer(ptr);
    object_t* collision;
    
    if ((collision = object_collision(ptr))) {
      if (ptr->x & 0x1 && ptr->y & 0x1) {
	ptr->velocity.y = ptr->y < collision->y ? -1 : 1;
	ptr->velocity.x = 0;
      } else {
	ptr->velocity.x = ptr->x < collision->x ? -1 : 1;
	ptr->velocity.y = 0;
      }
      ptr->walkAbout = 50;
    } else {
      ptr->velocity.x = object_strikingDistanceX(player, ptr);
      
      if (ptr->y < player->y) {
	ptr->velocity.y = 1;
      } else if (ptr->y > player->y) {
	ptr->velocity.y = -1;
      } else {
	ptr->velocity.y = 0;
      }
    }
  }
  object_updateObject(ptr);
}


void
enemy_init(object_t* player1, object_t * player2)
{
  enemy_player1 = player1;
  enemy_player2 = player2;
  if (1) {
    object_add(10, 85, 0, OBJECT_ANIM_PLAYER1_STAND_RIGHT, object_updateEnemy, 0); 
    object_add(SCREEN_WIDTH-64, 85, 0, OBJECT_ANIM_PLAYER1_STAND_RIGHT, object_updateEnemy, 0);
    object_add(SCREEN_WIDTH/2, 168, 0, OBJECT_ANIM_PLAYER1_STAND_RIGHT, object_updateEnemy, 0);
    object_add(64, 188, 0, OBJECT_ANIM_PLAYER1_STAND_RIGHT, object_updateEnemy, 0);
    object_add(SCREEN_WIDTH-64, 85, 0, OBJECT_ANIM_PLAYER1_STAND_RIGHT, object_updateEnemy, 0);
    object_add(64, 188, 0, OBJECT_ANIM_PLAYER1_STAND_RIGHT, object_updateEnemy, 0);
  }
}
