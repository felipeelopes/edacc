//
// Created by felipe on 19/10/18.
//

#include <stdio.h>
#include <string.h>
#include "felipe.h"

int main(void) {
    ler_dir("/home/felipe/txt");

    const char* a = "felape";
    const char* b = "felipe";

    /*
    if(strcmp(a, b) > 0){
        printf("%s\n", b);
        printf("%s\n", a);
    } else {
        printf("%s\n", a);
        printf("%s\n", b);
    }
     */

    //printf("%d", strcmp(&a, &b));
    return 0;
}