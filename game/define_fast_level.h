/* Welcome to the jungle */

#define __LEVEL_STRINGIFY(x) #x
#define __LEVEL_BACKGROUND_MAP2(x)   __LEVEL_STRINGIFY(out/background_ ##x-map.c)
#define __LEVEL_BACKGROUND_MAP(x)    __LEVEL_BACKGROUND_MAP2(x)
#define __LEVEL_FADE_IN2(x)          __LEVEL_STRINGIFY(out/palette_sprite_ ##x.h)
#define __LEVEL_FADE_IN(x)           __LEVEL_FADE_IN2(x)
#define __LEVEL_PALETTE_GREY2(x)     __LEVEL_STRINGIFY(out/palette_grey_sprite_ ##x.h)
#define __LEVEL_PALETTE_GREY(x)      __LEVEL_PALETTE_GREY2(x)
#ifndef RELEASE
#define __LEVEL_RECORDING2(x)        __LEVEL_STRINGIFY(out/level_recording_ ##x.h)
#else
#define __LEVEL_RECORDING2(x)        __LEVEL_STRINGIFY(out/demo.h)
#endif
#define __LEVEL_RECORDING(x)         __LEVEL_RECORDING2(x)
#define __LEVEL_F_DEFINITION2(x)       level_f_level##x
#define __LEVEL_F_DEFINITION(x)        __LEVEL_F_DEFINITION2(x)

level_fast_t
__LEVEL_F_DEFINITION(__LEVEL_NUMBER) =
  {
#include __LEVEL_BACKGROUND_MAP(__LEVEL_NUMBER)
    .palette = {
#include __LEVEL_FADE_IN(__LEVEL_NUMBER)
    },
    .greyPalette = {
#include __LEVEL_PALETTE_GREY(__LEVEL_NUMBER)
    },
#ifdef GAME_RECORDING
    .recordData = {
#include __LEVEL_RECORDING(__LEVEL_NUMBER)
    },
#endif
    .end = 0xB00BF00D
  };

#undef __LEVEL_NUMBER
#undef __LEVEL_HAS_CLOUDS
