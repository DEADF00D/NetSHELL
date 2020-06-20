#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "connection.h"
#include "utils.h"
#include "terminal.h"
#include "handlers.h"
#include "arguments.h"

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
    if(argc == 1){
        help(argv[0]);
        exit_(1);
    }

    Arguments args;
    Arguments_Init(&args);
    Arguments_Process(&args, argc, argv);

    Connection *connection;
    Connection_Init(&connection);

    if(args.server){
        server(connection, &args);
    }
    else if(args.client){
        client(connection, &args);
    }
    else if(args.help){
        help(argv[0]);
        exit_(1);
    }
    else if(args.version){
        version();
        exit_(1);
    }

    Connection_Free(connection);

    return 0;
}
