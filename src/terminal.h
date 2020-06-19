#ifndef _TERMINAL_H_
#define _TERMINAL_H_

#include <termios.h>
#include <sys/ioctl.h>

extern struct termios OriginalTermios;

void TerminalStart();
void TerminalStop(void);
struct winsize TerminalGetSize();
void exit_(int ex);

#endif
