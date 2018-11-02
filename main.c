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


    int menu;

        do
        {
            printf("\n\t 1.Busca palavra\n");
            printf("2. Atualizar o Indice\n");
            printf("3. Adicionar palavras\n");
            printf("4. Excluir palavras\n");
            printf("5. Adicionar ocorrencias\n");
            printf("6. Excluir ocorrencias\n");
            printf("7. Salvar o indice em arquivo\n");
            printf("8. Recuperar indice remissivo\n");


            scanf("%d", &menu);
            system("cls || clear");

            char word[1024];
            char caminho;
            switch(menu)
            {

                case 1:
                    printf("Digite uma palavra para buscar\n");
                    scanf('%c', &word);
                    busca(word);
                    break;

                case 2:
                    atualizar();
                    break;

                case 3:
                    printf("Digite uma palavra para inserir e o caminho \n");
                    scanf('%c', '%c', &word, &caminho);
                    inserir(word, caminho);
                    break;

                case 4:
                    printf("Digite uma palavra para excluir\n");
                    scanf('%c', &word);
                    excluir_palavras();
                    break;

                case 5:
                    printf("Digite o caminho para inserir\n");
                    scanf('%c', &caminho);
                    inserir_sublista(caminho);
                    break;

                case 6:
                    printf("Digite um caminho para excluir \n");
                    scanf('%c', &caminho);
                    excluir_ocorrencias();
                    break;

                case 7:
                    salvar();
                    break;

                case 8:
                    recuperar();
                    break;

                default:
                    printf("Digite uma opcao valida\n");
            }
        } while(menu);

    return EXIT_SUCCESS;
}