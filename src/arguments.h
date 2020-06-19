#ifndef _ARGUMENTS_H_
#define _ARGUMENTS_H_

typedef struct{
    // Server part
    bool isServer;
    int serverPort;

    // Client part
    bool isClient;
    char *clientIp;
    int clientPort;

    // Misc
    bool help;
    bool version;
} Arguments;

void Arguments_Init(Arguments *a);
void Arguments_Process(Arguments *a, int argc, char *argv[]);

#endif
