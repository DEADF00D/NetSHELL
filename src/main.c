#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include <ctype.h>

#include <sys/socket.h>
#include <arpa/inet.h>

#include <unistd.h>
#include <pthread.h>

#include "connection.h"
#include "utils.h"
#include "terminal.h"
#include "handlers.h"

#define VERSION "1.2"

void help(char *argvo){
    printf("NetSHELL v" VERSION " by Deadf00d\n");
    printf("Usage: %s [-l <port>] [<ip> <port>]\n\n", argvo);
    printf("-l <port> : Server mode\n");
}

void version(){
    printf(
        "\n"
        "       /\\\n"
        "      {.-}          NetSHELL v" VERSION " by Deadf00d\n"
        "     ;_.-'\\\n"
        "    {    _.}_\n"
        "     \\.-' /  `,\n"
        "      \\  |    /\n"
        "       \\ |  ,/\n"
        "    jgs \\|_/\n"
        "\n"
    );
}

int main(int argc, char *argv[]) {
    bool isServer = false;
    bool isClient = false;

    char *cip;
    int cport = 0;

    int lport;


    if(argc == 1){
        help(argv[0]);
        exit_(1);
    }


    int c;
    while((c = getopt(argc, argv, ":l:vh")) != -1){
        switch (c) {
            case 'h':
                help(argv[0]);
                exit_(1);
                break;
            case 'l':
                if(!stringIsDigit(optarg)){
                    error("Invalid bind port.");
                    exit_(1);
                }
                lport = atoi(optarg);
                isServer = true;
                break;
            case 'v':
                version();
                exit_(1);
                break;
            case '?':
                error("Unknown option: %c, please check help.", optopt);
                exit_(1);
                break;
        }
    }

    if(optind + 2 <= argc){
        cip = argv[optind];
        cport = atoi(argv[optind+1]);
        isClient = true;
    }

    if((!isServer && !isClient) || (isServer && isClient)){
        error("Server or client ?");
        exit_(1);
    }

    Connection *connection;
    Connection_Init(&connection);

    if(isServer){
        server(connection, lport);
    }
    else if(isClient){
        client(connection, cip, cport);
    }

    return 0;
}
