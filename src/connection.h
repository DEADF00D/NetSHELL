#ifndef _CONNECTION_H_
#define _CONNECTION_H_

#include <stdbool.h>
#include <arpa/inet.h>


typedef enum { IPV4 = 1, IPV6 } ConnectionType;

typedef struct{
    int socket;
    struct sockaddr_in addr;
    struct sockaddr_in6 addr6;

    ConnectionType type;
} Connection;

bool Connection_isValidIpv4(char *ipAddress);
bool Connection_isValidIpv6(char *ipAddress);

void Connection_Init(Connection **connection);

int Connection_setIpPortAddress(Connection *c, char *ip, int port);
int Connection_Connect(Connection *c);
struct in_addr *Connection_ResolveHostname(char *hostname);

#endif
