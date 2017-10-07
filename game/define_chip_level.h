/* Welcome to the jungle */

#define __LEVEL_STRINGIFY(x) #x
#define __LEVEL_SPRITE_IDS2(x)       __LEVEL_STRINGIFY(out/item_ ##x-sprites.c)
#define __LEVEL_SPRITE_IDS(x)        __LEVEL_SPRITE_IDS2(x)
#define __LEVEL_SPRITE_BITPLANES2(x) __LEVEL_STRINGIFY(out/sprite_ ##x.h)
#define __LEVEL_SPRITE_BITPLANES(x)  __LEVEL_SPRITE_BITPLANES2(x)
#ifdef GAME_ONE_BITPLANE_SPRITE_MASK
#define __LEVEL_SPRITE_MASK2(x)      __LEVEL_STRINGIFY(out/sprite_1bp_ ##x-mask.h)
#else
#define __LEVEL_SPRITE_MASK2(x)      __LEVEL_STRINGIFY(out/sprite_ ##x-mask.h)
#endif
#define __LEVEL_SPRITE_MASK(x)       __LEVEL_SPRITE_MASK2(x)
#define __LEVEL_TILE2(x)             __LEVEL_STRINGIFY(out/tile_ ##x.h)
#define __LEVEL_TILE(x)              __LEVEL_TILE2(x)
#define __LEVEL_C_DEFINITION2(x)       level_c_level##x
#define __LEVEL_C_DEFINITION(x)        __LEVEL_C_DEFINITION2(x)

//__SECTION_DISK
level_chip_t
__LEVEL_C_DEFINITION(__LEVEL_NUMBER) =
  {
    .spriteBitplanes = {
#include __LEVEL_SPRITE_BITPLANES(__LEVEL_NUMBER)
    },
    .spriteMask = {
#include __LEVEL_SPRITE_MASK(__LEVEL_NUMBER)
    },
    .tileBitplanes = {
#include __LEVEL_TILE(__LEVEL_NUMBER)
    },
    .end = 0xB00BF00D
  };

#undef __LEVEL_NUMBER
#undef __LEVEL_HAS_CLOUDS
