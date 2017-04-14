#ifndef __SCRIPT_H
#define __SCRIPT_H

#ifdef DEBUG
#ifdef SCRIPTING
void
script_process(void);

extern uint32_t script_breakpoint;
#endif
#endif
#endif
