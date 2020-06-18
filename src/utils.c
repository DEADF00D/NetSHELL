#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdarg.h>

#include <ctype.h>

bool stringIsDigit(char *str){
    for(size_t i=0; i<strlen(str); i++){
        if(isdigit(str[i]) == 0){
            return false;
        }
    }
    return true;
}

int error(const char *fmt, ...){
    va_list args;
    va_start(args, fmt);
    printf("(!) ");
    int rc = vfprintf(stdout, fmt, args);
    printf("\n");
    va_end(args);
    return rc;
}
