#include "game.h"

object_animation_t object_animations[] = {
  [OBJECT_ANIM_PLAYER1_RUN_RIGHT] = {
    .start =SPRITE_PLAYER1_RUN_RIGHT_1,
    .stop =SPRITE_PLAYER1_RUN_RIGHT_4,
    .speed = 2,
    .facing = FACING_RIGHT
  },
  [OBJECT_ANIM_PLAYER1_STAND_RIGHT] = {
    .start =SPRITE_PLAYER1_STAND_RIGHT,
    .stop =SPRITE_PLAYER1_STAND_RIGHT,
    .speed = 0,
    .facing = FACING_RIGHT
  },
  [OBJECT_ANIM_PLAYER1_RUN_LEFT] = {
    .start =SPRITE_PLAYER1_RUN_LEFT_1,
    .stop =SPRITE_PLAYER1_RUN_LEFT_4,
    .speed = 2,
    .facing = FACING_LEFT
  },
  [OBJECT_ANIM_PLAYER1_STAND_LEFT] = {
    .start =SPRITE_PLAYER1_STAND_LEFT,
    .stop =SPRITE_PLAYER1_STAND_LEFT,
    .speed = 0,
    .facing = FACING_LEFT
  },
  [OBJECT_ANIM_PLAYER1_PUNCH_LEFT1] = {
    .start =SPRITE_PLAYER1_PUNCH_LEFT_1,
    .stop = SPRITE_PLAYER1_PUNCH_LEFT_1,
    .speed = 0,
    .facing = FACING_LEFT
  },
  [OBJECT_ANIM_PLAYER1_PUNCH_LEFT2] = {
    .start = SPRITE_PLAYER1_PUNCH_LEFT_2,
    .stop = SPRITE_PLAYER1_PUNCH_LEFT_2,
    .speed = 0,
    .facing = FACING_LEFT
  },
  [OBJECT_ANIM_PLAYER1_PUNCH_RIGHT1] = {
    .start =SPRITE_PLAYER1_PUNCH_RIGHT_1,
    .stop = SPRITE_PLAYER1_PUNCH_RIGHT_1,
    .speed = 0,
    .facing = FACING_RIGHT
  },
  [OBJECT_ANIM_PLAYER1_PUNCH_RIGHT2] = {
    .start = SPRITE_PLAYER1_PUNCH_RIGHT_2,
    .stop = SPRITE_PLAYER1_PUNCH_RIGHT_2,
    .speed = 0,
    .facing = FACING_RIGHT
  },
  [OBJECT_ANIM_PLAYER1_HIT_LEFT] = {
    .start = SPRITE_PLAYER1_HIT_LEFT,
    .stop = SPRITE_PLAYER1_HIT_LEFT,
    .speed = 0,
    .facing = FACING_LEFT
  },
  [OBJECT_ANIM_PLAYER1_HIT_RIGHT] = {
    .start = SPRITE_PLAYER1_HIT_RIGHT,
    .stop = SPRITE_PLAYER1_HIT_RIGHT,
    .speed = 0,
    .facing = FACING_RIGHT
  },          
  // =================================
  [OBJECT_ANIM_PLAYER2_RUN_RIGHT] = {
    .start =SPRITE_PLAYER2_RUN_RIGHT_1,
    .stop =SPRITE_PLAYER2_RUN_RIGHT_4,
    .speed = 2,
    .facing = FACING_RIGHT
  },
  [OBJECT_ANIM_PLAYER2_STAND_RIGHT] = {
    .start =SPRITE_PLAYER2_STAND_RIGHT,
    .stop =SPRITE_PLAYER2_STAND_RIGHT,
    .speed = 0,
    .facing = FACING_RIGHT
  },
  [OBJECT_ANIM_PLAYER2_RUN_LEFT] = {
    .start =SPRITE_PLAYER2_RUN_LEFT_1,
    .stop =SPRITE_PLAYER2_RUN_LEFT_4,
    .speed = 2,
    .facing = FACING_LEFT
  },
  [OBJECT_ANIM_PLAYER2_STAND_LEFT] = {
    .start =SPRITE_PLAYER2_STAND_LEFT,
    .stop =SPRITE_PLAYER2_STAND_LEFT,
    .speed = 0,
    .facing = FACING_LEFT
  },
  [OBJECT_ANIM_PLAYER2_PUNCH_LEFT1] = {
    .start =SPRITE_PLAYER2_PUNCH_LEFT_1,
    .stop = SPRITE_PLAYER2_PUNCH_LEFT_1,
    .speed = 0,
    .facing = FACING_LEFT
  },
  [OBJECT_ANIM_PLAYER2_PUNCH_LEFT2] = {
    .start = SPRITE_PLAYER2_PUNCH_LEFT_2,
    .stop = SPRITE_PLAYER2_PUNCH_LEFT_2,
    .speed = 0,
    .facing = FACING_LEFT
  },      
  [OBJECT_ANIM_PLAYER2_PUNCH_RIGHT1] = {
    .start =SPRITE_PLAYER2_PUNCH_RIGHT_1,
    .stop = SPRITE_PLAYER2_PUNCH_RIGHT_1,
    .speed = 0,
    .facing = FACING_RIGHT
  },
  [OBJECT_ANIM_PLAYER2_PUNCH_RIGHT2] = {
    .start = SPRITE_PLAYER2_PUNCH_RIGHT_2,
    .stop = SPRITE_PLAYER2_PUNCH_RIGHT_2,
    .speed = 0,
    .facing = FACING_RIGHT
  },
  [OBJECT_ANIM_PLAYER2_HIT_LEFT] = {
    .start = SPRITE_PLAYER2_HIT_LEFT,
    .stop = SPRITE_PLAYER2_HIT_LEFT,
    .speed = 0,
    .facing = FACING_LEFT
  },
  [OBJECT_ANIM_PLAYER2_HIT_RIGHT] = {
    .start = SPRITE_PLAYER2_HIT_RIGHT,
    .stop = SPRITE_PLAYER2_HIT_RIGHT,
    .speed = 0,
    .facing = FACING_RIGHT
  },            
  // =================================
  [OBJECT_ANIM_PLAYER3_RUN_RIGHT] = {
    .start =SPRITE_PLAYER3_RUN_RIGHT_1,
    .stop =SPRITE_PLAYER3_RUN_RIGHT_4,
    .speed = 2,
    .facing = FACING_RIGHT
  },
  [OBJECT_ANIM_PLAYER3_STAND_RIGHT] = {
    .start =SPRITE_PLAYER3_STAND_RIGHT,
    .stop =SPRITE_PLAYER3_STAND_RIGHT,
    .speed = 0,
    .facing = FACING_RIGHT
  },
  [OBJECT_ANIM_PLAYER3_RUN_LEFT] = {
    .start =SPRITE_PLAYER3_RUN_LEFT_1,
    .stop =SPRITE_PLAYER3_RUN_LEFT_4,
    .speed = 2,
    .facing = FACING_LEFT
  },
  [OBJECT_ANIM_PLAYER3_STAND_LEFT] = {
    .start =SPRITE_PLAYER3_STAND_LEFT,
    .stop =SPRITE_PLAYER3_STAND_LEFT,
    .speed = 0,
    .facing = FACING_LEFT
  },
  [OBJECT_ANIM_PLAYER3_PUNCH_LEFT1] = {
    .start =SPRITE_PLAYER3_PUNCH_LEFT_1,
    .stop = SPRITE_PLAYER3_PUNCH_LEFT_1,
    .speed = 0,
    .facing = FACING_LEFT
  },
  [OBJECT_ANIM_PLAYER3_PUNCH_LEFT2] = {
    .start = SPRITE_PLAYER3_PUNCH_LEFT_2,
    .stop = SPRITE_PLAYER3_PUNCH_LEFT_2,
    .speed = 0,
    .facing = FACING_LEFT
  },      
  [OBJECT_ANIM_PLAYER3_PUNCH_RIGHT1] = {
    .start =SPRITE_PLAYER3_PUNCH_RIGHT_1,
    .stop = SPRITE_PLAYER3_PUNCH_RIGHT_1,
    .speed = 0,
    .facing = FACING_RIGHT
  },
  [OBJECT_ANIM_PLAYER3_PUNCH_RIGHT2] = {
    .start = SPRITE_PLAYER3_PUNCH_RIGHT_2,
    .stop = SPRITE_PLAYER3_PUNCH_RIGHT_2,
    .speed = 0,
    .facing = FACING_RIGHT
  },
  [OBJECT_ANIM_PLAYER3_HIT_LEFT] = {
    .start = SPRITE_PLAYER3_HIT_LEFT,
    .stop = SPRITE_PLAYER3_HIT_LEFT,
    .speed = 0,
    .facing = FACING_LEFT
  },
  [OBJECT_ANIM_PLAYER3_HIT_RIGHT] = {
    .start = SPRITE_PLAYER3_HIT_RIGHT,
    .stop = SPRITE_PLAYER3_HIT_RIGHT,
    .speed = 0,
    .facing = FACING_RIGHT
  },            
};


object_image_t object_imageAtlas[] = {
  [SPRITE_PLAYER1_STAND_RIGHT] = {
    .x = 0,
    .y = 0,
    .w = 32,
    .h = 53,
    .dx = 0,
    .dy = 0,    
  },
  [SPRITE_PLAYER1_RUN_RIGHT_1] = {
    .x = 32,
    .y = 0,
    .w = 32,
    .h = 53,
    .dx = 1,
    .dy = 0,    
  },
  [SPRITE_PLAYER1_RUN_RIGHT_2] = {
    .x = 64,
    .y = 0,
    .w = 32,
    .h = 53,
    .dx = 1,
    .dy = 0,    
  },
  [SPRITE_PLAYER1_RUN_RIGHT_3] = {
    .x = 96,
    .y = 0,
    .w = 32,
    .h = 53,
    .dx = -2,
    .dy = 0,    
  },
  [SPRITE_PLAYER1_RUN_RIGHT_4] = {
    .x = 32,
    .y = 0,
    .w = 32,
    .h = 53,
    .dx = 1,
    .dy = 0,    
    },
  [SPRITE_PLAYER1_STAND_LEFT] = {
    .x = 192,
    .y = 54,
    .w = 32,
    .h = 53,
    .dx = 0,
    .dy = 0,    
  },
  [SPRITE_PLAYER1_RUN_LEFT_1] = {
    .x = 160,
    .y = 54,
    .w = 32,
    .h = 53,
    .dx = -1,
    .dy = 0,    
  },
  [SPRITE_PLAYER1_RUN_LEFT_2] = {
    .x = 128,
    .y = 54,
    .w = 32,
    .h = 53,
    .dx = -1,
    .dy = 0,   
  },
  [SPRITE_PLAYER1_RUN_LEFT_3] = {
    .x = 96,
    .y = 54,
    .w = 32,
    .h = 53,
    .dx = 1,
    .dy = 0,    
  },
  [SPRITE_PLAYER1_RUN_LEFT_4] = {
    .x = 128,
    .y = 54,
    .w = 32,
    .h = 53,
    .dx = -1,
    .dy = 0,    
  },
  [SPRITE_PLAYER1_PUNCH_LEFT_1] = {
    .x = 32,
    .y = 54,
    .w = 32,
    .h = 53,
    .dx = -3,
    .dy = 0,    
    },
  [SPRITE_PLAYER1_PUNCH_LEFT_2] = {
    .x = 64,
    .y = 54,
    .w = 32,
    .h = 53,
    .dx = -2,
    .dy = 0,    
    },
  [SPRITE_PLAYER1_PUNCH_RIGHT_1] = {
    .x = 160,
    .y = 0,
    .w = 32,
    .h = 53,
    .dx = 3,
    .dy = 0,    
    },
  [SPRITE_PLAYER1_PUNCH_RIGHT_2] = {
    .x = 128,
    .y = 0,
    .w = 32,
    .h = 53,
    .dx = 4,
    .dy = 0,    
    },
  
  [SPRITE_PLAYER1_HIT_RIGHT] = {
    .x = 0,
    .y = 54,
    .w = 32,
    .h = 53,
    .dx = -3,
    .dy = 0,    
    },
  [SPRITE_PLAYER1_HIT_LEFT] = {
    .x = 192,
    .y = 0,
    .w = 32,
    .h = 53,
    .dx = 3,
    .dy = 0,    
    },
  //==============================
  [SPRITE_PLAYER2_STAND_RIGHT] = {
    .x = 0,
    .y = 108,
    .w = 32,
    .h = 53,
    .dx = 0,
    .dy = 0,    
  },
  [SPRITE_PLAYER2_RUN_RIGHT_1] = {
    .x = 32,
    .y = 108,
    .w = 32,
    .h = 53,
    .dx = 1,
    .dy = 0,    
  },
  [SPRITE_PLAYER2_RUN_RIGHT_2] = {
    .x = 64,
    .y = 108,
    .w = 32,
    .h = 53,
    .dx = 1,
    .dy = 0,    
  },
  [SPRITE_PLAYER2_RUN_RIGHT_3] = {
    .x = 96,
    .y = 108,
    .w = 32,
    .h = 53,
    .dx = -2,
    .dy = 0,    
  },
    [SPRITE_PLAYER2_RUN_RIGHT_4] = {
    .x = 32,
    .y = 108,
    .w = 32,
    .h = 53,
    .dx = 1,
    .dy = 0,    
    },
  [SPRITE_PLAYER2_STAND_LEFT] = {
    .x = 192,
    .y = 161,
    .w = 32,
    .h = 53,
    .dx = 0,
    .dy = 0,    
  },
  [SPRITE_PLAYER2_RUN_LEFT_1] = {
    .x = 160,
    .y = 161,
    .w = 32,
    .h = 53,
    .dx = -1,
    .dy = 0,    
  },
  [SPRITE_PLAYER2_RUN_LEFT_2] = {
    .x = 128,
    .y = 161,
    .w = 32,
    .h = 53,
    .dx = -1,
    .dy = 0,   
  },
  [SPRITE_PLAYER2_RUN_LEFT_3] = {
    .x = 96,
    .y = 161,
    .w = 32,
    .h = 53,
    .dx = 2,
    .dy = 0,    
  },

  [SPRITE_PLAYER2_RUN_LEFT_4] = {
    .x = 128,
    .y = 161,
    .w = 32,
    .h = 53,
    .dx = -1,
    .dy = 0,    
    },

  [SPRITE_PLAYER2_PUNCH_LEFT_1] = {
    .x = 224,
    .y = 161,
    .w = 48,
    .h = 53,
    .dx = -4,
    .dy = 0,    
    },
  [SPRITE_PLAYER2_PUNCH_LEFT_2] = {
    .x = 64,
    .y = 161,
    .w = 32,
    .h = 53,
    .dx = -3,
    .dy = 0,    
    },
  [SPRITE_PLAYER2_PUNCH_RIGHT_1] = {
    .x = 160,
    .y = 108,
    .w = 48,
    .h = 53,
    .dx = 3,
    .dy = 0,    
    },
  [SPRITE_PLAYER2_PUNCH_RIGHT_2] = {
    .x = 128,
    .y = 108,
    .w = 32,
    .h = 53,
    .dx = 3,
    .dy = 0,    
    },
  [SPRITE_PLAYER2_HIT_LEFT] = {
    .x = 0,
    .y = 161,
    .w = 32,
    .h = 53,
    .dx = 0,
    .dy = 0,    
    },
  [SPRITE_PLAYER2_HIT_RIGHT] = {
    .x = 208,
    .y = 108,
    .w = 32,
    .h = 53,
    .dx = 2,
    .dy = 0,    
    },      

  //==============================
  [SPRITE_PLAYER3_STAND_RIGHT] = {
    .x = 0,
    .y = 214,
    .w = 32,
    .h = 60,
    .dx = 0,
    .dy = 0,    
  },
  [SPRITE_PLAYER3_RUN_RIGHT_1] = {
    .x = 32,
    .y = 214,
    .w = 48,
    .h = 60,
    .dx = 0,
    .dy = 0,    
  },
  [SPRITE_PLAYER3_RUN_RIGHT_2] = {
    .x = 80,
    .y = 214,
    .w = 32,
    .h = 60,
    .dx = 0,
    .dy = 0,    
  },
  [SPRITE_PLAYER3_RUN_RIGHT_3] = {
    .x = 112,
    .y = 214,
    .w = 48,
    .h = 60,
    .dx = -5,
    .dy = 0,    
  },
    [SPRITE_PLAYER3_RUN_RIGHT_4] = {
    .x = 80,
    .y = 214,
    .w = 32,
    .h = 60,
    .dx = 0,
    .dy = 0,    
    },
  [SPRITE_PLAYER3_STAND_LEFT] = {
    .x = 240,
    .y = 274,
    .w = 32,
    .h = 60,
    .dx = 0,
    .dy = 0,    
  },
  [SPRITE_PLAYER3_RUN_LEFT_1] = {
    .x = 192,
    .y = 274,
    .w = 48,
    .h = 60,
    .dx = -6,
    .dy = 0,    
  },
  [SPRITE_PLAYER3_RUN_LEFT_2] = {
    .x = 160,
    .y = 274,
    .w = 32,
    .h = 60,
    .dx = 0,
    .dy = 0,   
  },
  [SPRITE_PLAYER3_RUN_LEFT_3] = {
    .x = 112,
    .y = 274,
    .w = 48,
    .h = 60,
    .dx = -3,
    .dy = 0,    
  },
    [SPRITE_PLAYER3_RUN_LEFT_4] = {
    .x = 160,
    .y = 274,
    .w = 32,
    .h = 60,
    .dx = 0,
    .dy = 0,    
    },

  [SPRITE_PLAYER3_PUNCH_LEFT_1] = {
    .x = 16,
    .y = 274,
    .w = 48,
    .h = 60,
    .dx = -19,
    .dy = 0,    
    },
  [SPRITE_PLAYER3_PUNCH_LEFT_2] = {
    .x = 64,
    .y = 274,
    .w = 48,
    .h = 60,
    .dx = -19,
    .dy = 0,    
    },
  [SPRITE_PLAYER3_PUNCH_RIGHT_1] = {
    .x = 208,
    .y = 214,
    .w = 48,
    .h = 60,
    .dx = 1,
    .dy = 0,    
    },
  [SPRITE_PLAYER3_PUNCH_RIGHT_2] = {
    .x = 160,
    .y = 214,
    .w = 48,
    .h = 60,
    .dx = 1,
    .dy = 0,    
    },  
  [SPRITE_PLAYER3_HIT_LEFT] = {
    .x = 32,
    .y = 334,
    .w = 32,
    .h = 60,
    .dx = 0,
    .dy = 0,    
    },
  [SPRITE_PLAYER3_HIT_RIGHT] = {
    .x = 0,
    .y = 334,
    .w = 32,
    .h = 60,
    .dx = 0,
    .dy = 0,    
    },      
};
