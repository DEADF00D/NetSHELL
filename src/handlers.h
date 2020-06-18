#ifndef _HANDLERS_H_
#define _HANDLERS_H_

void *connectionHandlerReadingThreadHandler(void *arg);
int connectionHandler(Connection *c);

int client(Connection *c, char *ip, int port);
int server(Connection *c, int port);

#endif
