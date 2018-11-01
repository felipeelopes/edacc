//
// Created by felipe on 19/10/18.
//

#include "felipe.h"

int main(int argc, char *argv[]) {
    if(argc != 2){
        printf("[ERRO] Eh necessario passar o caminho do diretorio onde o programa fara a leitura dos arquivos\n");
        return EXIT_FAILURE;
    } else {
        Lista* lista = cria();
        ler_dir(lista, argv[1]);
        imprime(lista);
    }

    return EXIT_SUCCESS;
}