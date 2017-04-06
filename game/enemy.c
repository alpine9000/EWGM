#include "game.h"

static object_t* enemy_player1;
static object_t* enemy_player2;

#define ENEMY_MAX 6

player_data_t enemy_data[ENEMY_MAX];

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

  uint16_t punch = 0;
  
  if (ptr->walkAbout) {
    ptr->walkAbout--;
  } else {
    object_t* player = enemy_closestPlayer(ptr);
    object_collision_t collision;

    if ((object_collision2(ptr, &collision, 20, 1))) {
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
      ptr->walkAbout = 50;
    } else {
      ptr->velocity.x = object_strikingDistanceX(player, ptr);
      
      if (ptr->y < player->y) {
	ptr->velocity.y = 1;
      } else if (ptr->y > player->y) {
	ptr->velocity.y = -1;
      } else {
	if (ptr->velocity.x == 0) {
	  punch = 1;
	}
	ptr->velocity.y = 0;
      }
    }
  }

  object_updatePlayer(ptr, punch, ptr->data);
}


void
enemy_init(object_t* player1, object_t * player2)
{
  enemy_player1 = player1;
  enemy_player2 = player2;
  uint16_t i = 0;
    object_add(10, 85, 0, OBJECT_ANIM_PLAYER1_STAND_RIGHT, object_updateEnemy, &enemy_data[i++]);   
  if (1) {
    object_add(SCREEN_WIDTH-64, 85, 0, OBJECT_ANIM_PLAYER1_STAND_RIGHT, object_updateEnemy, &enemy_data[i++]);
    object_add(SCREEN_WIDTH/2, 168, 0, OBJECT_ANIM_PLAYER1_STAND_RIGHT, object_updateEnemy, &enemy_data[i++]);
    object_add(64, 188, 0, OBJECT_ANIM_PLAYER1_STAND_RIGHT, object_updateEnemy, &enemy_data[i++]);
    //    object_add(SCREEN_WIDTH-64, 85, 0, OBJECT_ANIM_PLAYER1_STAND_RIGHT, object_updateEnemy, &enemy_data[i++]);
    //    object_add(64, 188, 0, OBJECT_ANIM_PLAYER1_STAND_RIGHT, object_updateEnemy, &enemy_data[i++]);
  }
}
