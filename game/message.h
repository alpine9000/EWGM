#ifndef __MESSAGE_H
#define __MESSAGE_H

void
message_screenOn(char* message);
void
message_loading(char* message);
void
message_screenOff(void);
uint16_t
message_ask(char* message);
#if TRACKLOADER==1
void
message_loadingAnimate(void);
extern uint16_t message_loadingAnimation;
#endif
#endif

extern uint16_t message_on;
