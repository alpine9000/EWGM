/* Welcome to the jungle */

#define __LEVEL_STRINGIFY(x) #x
#define __LEVEL_BACKGROUND_MAP2(x)   __LEVEL_STRINGIFY(out/background_ ##x-map.c)
#define __LEVEL_BACKGROUND_MAP(x)    __LEVEL_BACKGROUND_MAP2(x)
#define __LEVEL_SPRITE_IDS2(x)       __LEVEL_STRINGIFY(out/item_ ##x-sprites.c)
#define __LEVEL_SPRITE_IDS(x)        __LEVEL_SPRITE_IDS2(x)
#define __LEVEL_SPRITE_BITPLANES2(x) __LEVEL_STRINGIFY(out/sprite_ ##x.h)
#define __LEVEL_SPRITE_BITPLANES(x)  __LEVEL_SPRITE_BITPLANES2(x)
#define __LEVEL_SPRITE_MASK2(x)      __LEVEL_STRINGIFY(out/sprite_ ##x-mask.h)
#define __LEVEL_SPRITE_MASK(x)       __LEVEL_SPRITE_MASK2(x)
#define __LEVEL_TILE2(x)             __LEVEL_STRINGIFY(out/tile_ ##x.h)
#define __LEVEL_TILE(x)              __LEVEL_TILE2(x)
#define __LEVEL_FADE_IN2(x)          __LEVEL_STRINGIFY(out/palette_sprite_ ##x.h)
#define __LEVEL_FADE_IN(x)           __LEVEL_FADE_IN2(x)
#define __LEVEL_RECORDING2(x)        __LEVEL_STRINGIFY(out/level_recording_ ##x.h)
#define __LEVEL_RECORDING(x)         __LEVEL_RECORDING2(x)
#define __LEVEL_DEFINITION2(x)       level_level##x
#define __LEVEL_DEFINITION(x)        __LEVEL_DEFINITION2(x)

__NOLOAD level_t 
__LEVEL_DEFINITION(__LEVEL_NUMBER) =
  {
#include __LEVEL_BACKGROUND_MAP(__LEVEL_NUMBER)
    .item_spriteIds = {
#include __LEVEL_SPRITE_IDS(__LEVEL_NUMBER)
    },
    .spriteBitplanes = {
#include __LEVEL_SPRITE_BITPLANES(__LEVEL_NUMBER)
    },
    .spriteMask = {
#include __LEVEL_SPRITE_MASK(__LEVEL_NUMBER)
    },
    .tileBitplanes = {
#include __LEVEL_TILE(__LEVEL_NUMBER)
    },
    .palette = {
#include __LEVEL_FADE_IN(__LEVEL_NUMBER)
    },
#ifdef GAME_RECORDING
    .recordData = {
#include __LEVEL_RECORDING(__LEVEL_NUMBER)
    }
#endif
  };

#undef __LEVEL_NUMBER
#undef __LEVEL_HAS_CLOUDS
