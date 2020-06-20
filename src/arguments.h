#ifndef _ARGUMENTS_H_
#define _ARGUMENTS_H_

typedef struct{
    // Server part
    bool server;
    int serverPort;

    // Client part
    bool client;
    char *clientIp;
    int clientPort;

    // Misc
    bool help;
    bool version;

    // 'unix' name was having problem with an another defined constant.
    // arguments.h:18:10: error: expected identifier or ‘(’ before numeric constant
    bool unixx;
    bool windows;
    bool autodetect;
} Arguments;

void Arguments_Init(Arguments *a);
void Arguments_Process(Arguments *a, int argc, char *argv[]);

#endif
