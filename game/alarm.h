#ifndef __ALARM_H
#define __ALARM_H

typedef struct _alarm {
  struct _alarm* prev;
  struct _alarm* next;
  uint32_t tic;
  void (*handler)(void*);
  void* data;
} alarm_t;

void
alarm_process(uint16_t deltaT);

void
alarm_init(void);

void
alarm_add(uint32_t tic, void (*handler)(void* data), void*data);


#endif
