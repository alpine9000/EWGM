#include "game.h"

#define ALARM_MAX_ALARMS 5

alarm_t* alarm_activeList;
static alarm_t* alarm_freeList;
static __section(random) alarm_t alarm_buffer[ALARM_MAX_ALARMS];
uint16_t alarm_count;
uint32_t alarm_tic;

static alarm_t*
alarm_getFree(void)
{
  alarm_t* entry = alarm_freeList;
  alarm_freeList = alarm_freeList->next;
  if (alarm_freeList) {
    alarm_freeList->prev = 0;
  }

#ifdef DEBUG
  if (!entry) {
    PANIC("alarm_getFree: empty list");
  }
#endif
  
  return entry;
}


static void
alarm_addFree(alarm_t* ptr)
{
  alarm_count--;

  if (alarm_freeList == 0) {
    alarm_freeList = ptr;
    ptr->next = 0;
    ptr->prev = 0;
  } else {
    ptr->next = alarm_freeList;
    ptr->next->prev = ptr;
    ptr->prev = 0;
    alarm_freeList = ptr;
  }
}


static void
alarm_addToActive(alarm_t* ptr)
{
  alarm_count++;

  if (alarm_activeList == 0) {
    alarm_activeList = ptr;
    ptr->next = 0;
    ptr->prev = 0;
  } else {
    ptr->next = alarm_activeList;    
    ptr->next->prev = ptr;
    ptr->prev = 0;
    alarm_activeList = ptr;
  }
}


static void
alarm_removeFromActive(alarm_t* ptr)
{
  if (ptr->prev == 0) {
    alarm_activeList = ptr->next;
    if (alarm_activeList) {
      alarm_activeList->prev = 0;
    }
  } else {
    ptr->prev->next = ptr->next;
    if (ptr->next != 0) {
      ptr->next->prev = ptr->prev;
    }
  }
}


void
alarm_add(uint32_t tic, void (*handler)(void))
{
  alarm_t* ptr = alarm_getFree();
  ptr->tic = alarm_tic + tic;
  ptr->handler = handler;
  alarm_addToActive(ptr);
}

void
alarm_init(void)
{
  alarm_tic = 0;
  alarm_count = 0;
  alarm_activeList = 0;
  alarm_freeList = &alarm_buffer[0];
  alarm_freeList->prev = 0;
  alarm_t* ptr = alarm_freeList;
  for (int16_t i = 1; i < ALARM_MAX_ALARMS; i++) {
      ptr->next = &alarm_buffer[i];
      ptr->next->prev = ptr;
      ptr = ptr->next;
  }
  ptr->next = 0;
}


void alarm_process(uint16_t deltaT)
{
  alarm_tic += deltaT;

  alarm_t* ptr = alarm_activeList;

  while (ptr) {
    alarm_t* next = ptr->next;
    if (ptr->tic <= alarm_tic) {
      ptr->handler();
      alarm_removeFromActive(ptr);
      alarm_addFree(ptr);
    }
    ptr = next;
  }
}
