#ifndef __POPUP_H
#define __POPUP_H

void
popup_ctor(void);
void
popup(char* popup, void (*callback)(void));
void
popup_dismiss(void);
void
popup_alert(char* message);
void
popup_saveBackground(frame_buffer_t fb);
void
popup_restoreBackground(void);
void
popup_render(frame_buffer_t fb);
void
popup_off(void);
#endif
