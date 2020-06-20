#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include <netdb.h>

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

int Connection_Free(Connection *connection){
    if(connection!=NULL){ return 0; }
    free(connection);
    return 1;
}

int Connection_setIpPortAddress(Connection *c, char *ip, int port){
    struct in_addr *hostname_addr;

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
    else if((hostname_addr = Connection_ResolveHostname(ip)) != NULL){
        memcpy((void*)(&(c->addr.sin_addr)), hostname_addr, sizeof(struct in_addr));

        c->addr.sin_family = AF_INET;
        c->addr.sin_port = htons(port);

        c->type = IPV4;
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

struct in_addr *Connection_ResolveHostname(char *hostname){
    struct hostent *he;
    struct in_addr **addr_list;
    if(( he = gethostbyname(hostname) ) == NULL){
        return 0;
    }

    addr_list = (struct in_addr**) he->h_addr_list;
    return addr_list[0];
}

int Connection_RecvLine(Connection *c, char **line){
    *line = (char*)calloc(1, sizeof(char));

    int len, y=0;
    char s[2];
    while((len = recv(c->socket, s, 1, 0)) > 0){
        if(s[0] == '\n'){ break; }

        *line = (char*)realloc(*line, sizeof(char)*(y+3));
        (*line)[y] = s[0];
        (*line)[y+1] = 0;
        y++;
    }

    if(len == -1 || len == 0){
        return 0;
    }
    return y;
}
