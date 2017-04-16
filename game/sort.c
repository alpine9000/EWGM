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
    SWAP(3, 4);
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

#if OBJECT_MAX_OBJECTS > 8
static void sort9(object_t **d){
  SWAP(0,1);
  SWAP(3,4);
  SWAP(6,7);
  SWAP(1,2);
  SWAP(4,5);
  SWAP(7,8);
  SWAP(0,1);
  SWAP(3,4);
  SWAP(6,7);
  SWAP(2,5);
  SWAP(0,3);
  SWAP(1,4);
  SWAP(5,8);
  SWAP(3,6);
  SWAP(4,7);
  SWAP(2,5);
  SWAP(0,3);
  SWAP(1,4);
  SWAP(5,7);
  SWAP(2,6);
  SWAP(1,3);
  SWAP(4,6);
  SWAP(2,4);
  SWAP(5,6);
  SWAP(2,3);
}
#endif


#if OBJECT_MAX_OBJECTS > 9
static void sort10(object_t **d){
  SWAP(4,9);
  SWAP(3,8);
  SWAP(2,7);
  SWAP(1,6);
  SWAP(0,5);  
  SWAP(1,4);
  SWAP(6,9);
  SWAP(0,3);
  SWAP(5,8);  
  SWAP(0,2);
  SWAP(3,6);
  SWAP(7,9);  
  SWAP(0,1);
  SWAP(2,4);
  SWAP(5,7);
  SWAP(8,9);  
  SWAP(1,2);
  SWAP(4,6);
  SWAP(7,8);
  SWAP(3,5);  
  SWAP(2,5);
  SWAP(6,8);
  SWAP(1,3);
  SWAP(4,7);  
  SWAP(2,3);
  SWAP(6,7);  
  SWAP(3,4);
  SWAP(5,6);  
  SWAP(4,5);
}
#endif


#if OBJECT_MAX_OBJECTS > 10
static void sort11(object_t **d){
  SWAP(0,1);
  SWAP(2,3);
  SWAP(4,5);
  SWAP(6,7);
  SWAP(8,9);  
  SWAP(1,3);
  SWAP(5,7);
  SWAP(0,2);
  SWAP(4,6);
  SWAP(8,10);  
  SWAP(1,2);
  SWAP(5,6);
  SWAP(9,10);
  SWAP(0,4);
  SWAP(3,7);  
  SWAP(1,5);
  SWAP(6,10);
  SWAP(4,8);  
  SWAP(5,9);
  SWAP(2,6);
  SWAP(0,4);
  SWAP(3,8);  
  SWAP(1,5);
  SWAP(6,10);
  SWAP(2,3);
  SWAP(8,9);  
  SWAP(1,4);
  SWAP(7,10);
  SWAP(3,5);
  SWAP(6,8);  
  SWAP(2,4);
  SWAP(7,9);
  SWAP(5,6);  
  SWAP(3,4);
  SWAP(7,8);  
}
#endif


#if OBJECT_MAX_OBJECTS > 11
static void sort12(object_t **d){
  SWAP(0,1);
  SWAP(2,3);
  SWAP(4,5);
  SWAP(6,7);
  SWAP(8,9);
  SWAP(10,11);  
  SWAP(1,3);
  SWAP(5,7);
  SWAP(9,11);
  SWAP(0,2);
  SWAP(4,6);
  SWAP(8,10);  
  SWAP(1,2);
  SWAP(5,6);
  SWAP(9,10);
  SWAP(0,4);
  SWAP(7,11);  
  SWAP(1,5);
  SWAP(6,10);
  SWAP(3,7);
  SWAP(4,8);  
  SWAP(5,9);
  SWAP(2,6);
  SWAP(0,4);
  SWAP(7,11);
  SWAP(3,8);  
  SWAP(1,5);
  SWAP(6,10);
  SWAP(2,3);
  SWAP(8,9);  
  SWAP(1,4);
  SWAP(7,10);
  SWAP(3,5);
  SWAP(6,8);  
  SWAP(2,4);
  SWAP(7,9);
  SWAP(5,6);  
  SWAP(3,4);
  SWAP(7,8); 
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
  sort8,
#endif
#if OBJECT_MAX_OBJECTS > 8
  sort9,
#endif
#if OBJECT_MAX_OBJECTS > 9
  sort10,
#endif
#if OBJECT_MAX_OBJECTS > 10
  sort11,
#endif 
#if OBJECT_MAX_OBJECTS > 11
  sort12,
#endif       
};
  

void
sort_z(int16_t count, object_t **d)
{
#ifdef DEBUG
  if (count > OBJECT_MAX_OBJECTS) {
    PANIC("sort_z: out of bounds");
  }
#endif
  sort_zs[count](d);
}
