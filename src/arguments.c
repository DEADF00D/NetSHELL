#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <ctype.h>
#include <unistd.h>

#include "utils.h"
#include "arguments.h"
#include "terminal.h"

void Arguments_Init(Arguments *a){
    a->client = false;
    a->server = false;

    a->help = false;
    a->version = false;

    a->unixx = false;
    a->windows = false;
    a->autodetect = true;
}

void Arguments_Process(Arguments *a, int argc, char *argv[]){
    int c;
    while((c = getopt(argc, argv, ":l:vhwx")) != -1){
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
                a->server = true;
                break;
            case 'x':
                a->unixx = true;
                a->autodetect = false;
                break;
            case 'w':
                a->windows = true;
                a->autodetect = false;
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
        a->client = true;
    }

    if(a->server && a->client){
        error("Server or client ?");
        exit_(1);
    }

    if((a->unixx && a->windows)){
        error("Unix or Windows ?");
        exit_(1);
    }
}
