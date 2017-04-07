#include "game.h"

// http://pages.ripco.net/~jgamble/nw.html

#define _min(a, b) (d[a]->_z<d[b]->_z?d[a]:d[b])
#define _max(a, b) (d[a]->_z<d[b]->_z?d[b]:d[a]) 
#define SWAP(x,y) { object_t* a = _min(x,y);object_t* b = _max(x,y); d[x] = a; d[y] = b; }


static void sort0(object_t **d){
  USE(d);
}

static void sort2(object_t **d){
    SWAP(0, 1);
}

static void sort3(object_t **d){
    SWAP(1, 2);
    SWAP(0, 2);
    SWAP(0, 1);
}

static void sort4(object_t **d){
    SWAP(0, 1);
    SWAP(2, 3);
    SWAP(0, 2);
    SWAP(1, 3);
    SWAP(1, 2);
}

static void sort5(object_t **d){
    SWAP(0, 1);
    SWAP(3, 3);
    SWAP(2, 4);
    SWAP(2, 3);
    SWAP(1, 4);
    SWAP(0, 3);
    SWAP(0, 2);
    SWAP(1, 3);
    SWAP(1, 2);
}

static void sort6(object_t **d){
    SWAP(1, 2);
    SWAP(4, 5);
    SWAP(0, 2);
    SWAP(3, 5);
    SWAP(0, 1);
    SWAP(3, 4);
    SWAP(1, 4);
    SWAP(0, 3);
    SWAP(2, 5);
    SWAP(1, 3);
    SWAP(2, 4);
    SWAP(2, 3);
}

#if OBJECT_MAX_OBJECTS > 6
static void sort7(object_t **d){
  SWAP(1,2);
  SWAP(3,4);
  SWAP(5,6);
  SWAP(0,2);
  SWAP(3,5);
  SWAP(4,6);
  SWAP(0,1);
  SWAP(4,5);
  SWAP(2,6);
  SWAP(0,4);
  SWAP(1,5);
  SWAP(0,3);
  SWAP(2,5);
  SWAP(1,3);
  SWAP(2,4);
  SWAP(2,3);
}
#endif

#if OBJECT_MAX_OBJECTS > 7
static void sort8(object_t **d){
  SWAP(0,1);
  SWAP(2,3);
  SWAP(4,5);
  SWAP(6,7);
  SWAP(0,2);
  SWAP(1,3);
  SWAP(4,6);
  SWAP(5,7);
  SWAP(1,2);
  SWAP(5,6);
  SWAP(0,4);
  SWAP(3,7);
  SWAP(1,5);
  SWAP(2,6);
  SWAP(1,4);
  SWAP(3,6);
  SWAP(2,4);
  SWAP(3,5);
  SWAP(3,4);
}
#endif

static void(*sort_zs[OBJECT_MAX_OBJECTS+1])(object_t**) = {
  sort0,
  sort0,
  sort2,
  sort3,
  sort4,
  sort5,
  sort6,
#if OBJECT_MAX_OBJECTS > 6
  sort7,
#endif
#if OBJECT_MAX_OBJECTS > 7  
  sort8
#endif
};
  

void
sort_z(int16_t count, object_t **d)
{
  sort_zs[count](d);
}
