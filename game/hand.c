#include "game.h"

#define HAND_X SCREEN_WIDTH-54

static uint32_t hand_frame;
static uint16_t hand_hideNextAnimation;
object_t* hand;

static void
hand_update(__UNUSED uint16_t deltaT, __UNUSED object_t* ptr)
{
  if (hand_frame && (hw_verticalBlankCount - hand_frame > 25)) {
    if (hand_hideNextAnimation) {
      hand_frame = 0;
      object_set_state(ptr, OBJECT_STATE_REMOVED);
    } else {
      hand_frame = hw_verticalBlankCount;
      ptr->visible = !ptr->visible;
      if (ptr->visible) {
	sound_queueSound(SOUND_PICKUP);
      }
    }
  }
  object_set_px(hand, (game_cameraX+HAND_X)*OBJECT_PHYSICS_FACTOR);
}


object_t*
hand_init(void)
{
  return 0;
}


void
hand_show(void)
{
  hand = object_add(OBJECT_ID_HAND, 0, HAND_X, 65, 0, OBJECT_ANIM_HAND, hand_update, 0, 0, 0);
  object_set_z(hand, 4096);
  hand->visible = 1;
  sound_queueSound(SOUND_PICKUP);
  hand_frame = hw_verticalBlankCount;
  hand_hideNextAnimation = 0;
}


void
hand_hide(void)
{
  hand_hideNextAnimation = 1;
}
