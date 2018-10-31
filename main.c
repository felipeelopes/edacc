//
// Created by felipe on 19/10/18.
//

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "felipe.h"

int main(int argc, char *argv[]) {

    if(argc != 2){
        printf("[ERRO] Eh necessario passar o caminho do diretorio onde o programa fara a leitura dos arquivos\n");
        return EXIT_FAILURE;
    } else {
        ler_dir(argv[1]);
    }

    return EXIT_SUCCESS;
}