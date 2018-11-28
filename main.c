//
// Created by felipe on 19/10/18.
//

#include "felipe.h"

int main(int argc, char *argv[]) {

    if(argc != 2){
        printf("[ERRO] Eh necessario passar o caminho do diretorio onde o programa fara a leitura dos arquivos\n");
        return EXIT_FAILURE;
    } else {
        puts("Aguarde, adicionando elementos do diretorio...");
        Lista *lista = cria();
        ler_dir(lista, argv[1]);
        //imprime(lista);

        int opcao_escolhida;

        do {
            printf("1. Busca palavra\n");
            printf("2. Atualizar o Indice\n");
            printf("3. Salvar o indice em arquivo\n");
            printf("4. Recuperar indice remissivo\n");


            scanf("%d", &opcao_escolhida);
            //system("clear");

            char palavra[1024];
            char caminho;
            switch(opcao_escolhida){

                case 1:
                    printf("Digite uma palavra para buscar\n");

                    // Capturar a entrada do usuario
                    scanf("%s", palavra);

                    // Buscar a palavra desejada pelo usuario mostrando os caminhos encontrados e numero de ocorrencias
                    busca_palavra(lista, palavra);

                    puts("\n\nDigite qualquer tecla para continuar...\n");
                    getchar();getchar();
                    break;

                case 2:
                    sincronizar(lista, argv[1]);
                    break;

                case 3:
                    printf("Digite o caminho e nome do arquivo, ex: /home/felipe/salvo.txt \n");
                    scanf("%s", palavra);
                    salvar(lista, palavra);
                    puts("Digite qualquer tecla para continuar...\n");
                    getchar();getchar();
                    break;

                case 4:
                    printf("Digite o caminho e nome do arquivo, ex: /home/felipe/salvo.txt \n");
                    scanf("%s", palavra);
                    carregar_arquivo(lista, palavra);
                    puts("Digite qualquer tecla para continuar...\n");
                    getchar();getchar();

                    break;

                case 5:
                    printf("Digite o caminho para inserir\n");
                    scanf('%c', &caminho);
                    //inserir_sublista(lista, caminho);
                    break;

                case 6:
                    printf("Digite um caminho para excluir \n");
                    scanf('%c', &caminho);
                    //excluir_ocorrencias();
                    break;

                case 7:
                    //salvar();
                    break;

                case 8:
                    //recuperar();
                    break;

                default:
                    printf("Digite uma opcao valida\n");
            }
        } while (opcao_escolhida);
    }
    return EXIT_SUCCESS;
}