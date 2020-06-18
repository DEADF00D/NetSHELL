#include <stdio.h>
#include <stdlib.h>

#include <termios.h>
#include <sys/ioctl.h>

#include <unistd.h>

#include "terminal.h"

struct termios OriginalTermios;

void TerminalStop(void){
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &OriginalTermios);
}

void TerminalStart(){
    /* Save current terminal attributes */
    tcgetattr(STDIN_FILENO, &OriginalTermios);

    /* Restore them at exit */
    atexit(TerminalStop);

    struct termios new_opts;
    cfmakeraw(&new_opts);
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &new_opts);
}

void exit_(int ex){
    exit(ex);
}
