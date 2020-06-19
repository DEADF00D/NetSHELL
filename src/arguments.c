#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <ctype.h>
#include <unistd.h>

#include "utils.h"
#include "arguments.h"
#include "terminal.h"

void Arguments_Init(Arguments *a){
    a->isClient = false;
    a->isServer = false;
    a->help = false;
    a->version = false;
}

void Arguments_Process(Arguments *a, int argc, char *argv[]){
    int c;
    while((c = getopt(argc, argv, ":l:vh")) != -1){
        switch (c) {
            case 'h':
                a->help = true;
                break;
            case 'v':
                a->version = true;
                break;
            case 'l':
                if(!stringIsDigit(optarg)){
                    error("Invalid bind port.");
                    exit_(1);
                }
                a->serverPort = atoi(optarg);
                a->isServer = true;
                break;
            case 'x':
                printf("Ok, it's gonna be unix.\n");
                break;
            case 'w':
                printf("Ok, it's gonna be windows.\n");
                break;
            case '?':
                error("Unknown option: %c, please check help.", optopt);
                exit_(1);
                break;
        }
    }

    if(optind + 2 <= argc){
        a->clientIp = argv[optind];
        a->clientPort = atoi(argv[optind+1]);
        a->isClient = true;
    }

    if(a->isServer && a->isClient){
        error("Server or client ?");
        exit_(1);
    }
}
