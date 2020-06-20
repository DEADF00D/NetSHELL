#ifndef _HANDLERS_H_
#define _HANDLERS_H_

#include "arguments.h"

void *connectionHandlerReadingThreadHandler(void *arg);
int connectionHandler(Connection *c, Arguments *args);

int client(Connection *c, Arguments *args);
int server(Connection *c, Arguments *args);

#endif
