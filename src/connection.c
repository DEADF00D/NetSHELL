#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "connection.h"
#include "utils.h"
#include "terminal.h"

bool Connection_isValidIpv4(char *ipAddress){
    struct sockaddr_in sa;
    int result = inet_pton(AF_INET, ipAddress, &(sa.sin_addr));
    return result != 0;
}

bool Connection_isValidIpv6(char *ipAddress){
    struct sockaddr_in6 sa;
    int result = inet_pton(AF_INET6, ipAddress, &(sa.sin6_addr));
    return result != 0;
}

void Connection_Init(Connection **connection){
    *connection = (Connection*)calloc(1,sizeof(Connection));
    if(!(*connection)){
        error("calloc() failed on allocation connection structure.\n");
        exit_(1);
    }
}

int Connection_setIpPortAddress(Connection *c, char *ip, int port){
    if(Connection_isValidIpv4(ip)){
        c->addr.sin_family = AF_INET;
        c->addr.sin_port = htons(port);
        c->addr.sin_addr.s_addr = inet_addr(ip);

        c->type = IPV4;
        return c->type;
    }
    else if(Connection_isValidIpv6(ip)){
        c->addr6.sin6_family = AF_INET6;
        c->addr6.sin6_port = htons(port);
        inet_pton(AF_INET6, ip, &(c->addr6.sin6_addr));

        c->type = IPV6;
        return c->type;
    }
    return -1;
}

int Connection_Connect(Connection *c){
    if(c->type == IPV4){
        if(connect(c->socket, (struct sockaddr*)&(c->addr), sizeof(c->addr)) < 0){
            return 0;
        }
        return 1;
    }
    else if(c->type == IPV6){
        if(connect(c->socket, (struct sockaddr*)&(c->addr6), sizeof(c->addr6)) < 0){
            return 0;
        }
        return 1;
    }
    return -1;
}
