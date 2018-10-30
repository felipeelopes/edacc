//
// Created by felipe on 19/10/18.
//

#include "felipe.h"
#include <unistd.h>
#include <sys/types.h>
#include <dirent.h>
#include <stdio.h>
#include <string.h>
#include <regex.h>
#include <stdlib.h>
#include <ctype.h>

/*
 * Funcao responsavel por varrer o diretorio e sub-diretorios do caminho informado pelo usuario
 * */
void ler_dir(const char *name) {
    DIR *dir;
    struct dirent *entrada;

    // Aloca espaço para a estrutura do tipo regex_t
    // para usarmos expressoes regulares
    regex_t reg_txt;

    // Compilamos a regex para buscarmos um arquivo .txt
    if (regcomp(&reg_txt , ".*\\.txt", REG_EXTENDED|REG_NOSUB) != 0) {
        printf("[ERRO] erro ao compilar o regex reg_txt\n");
        exit(1);
    }

    // Verofica se conseguimos abrir o diretorio
    if (!(dir = opendir(name))){
        return;
    }

    // Percorrer todos os diretorios e sub-diretorio
    while ((entrada = readdir(dir)) != NULL) {

        // Armazenar os caminhos encontrados
        char caminho[1024];

        // Caso seja diretorio
        if (entrada->d_type == DT_DIR) {

            // Ocultar [.] e [..] do caminho
            if (strcmp(entrada->d_name, ".") == 0 || strcmp(entrada->d_name, "..") == 0)
                continue;

            // Armazenar caminho formatado na variavel
            snprintf(caminho, sizeof(caminho), "%s/%s", name, entrada->d_name);

            // De forma recursiva entramos no novo diretorio encontrado
            ler_dir(caminho);


        // Caso seja arquivo
        // Aqui temos que abrir cada arquivo .txt encontrado e adicionar seu conteudo na fila
        } else {
            snprintf(caminho, sizeof(caminho), "%s/%s", name, entrada->d_name);

            // Caso o arquivo nao seja txt, ignoramos o mesmo e continuamos
            // regexec retorna 0 caso o termo seja correspondente
            if ((regexec(&reg_txt, caminho, 0, (regmatch_t *)NULL, 0)) != 0){
                continue;
            } else {
                ler_txt(caminho); // para cada txt encontrado devemos abrir o mesmo e adicionar seu conteudo na pilha
            }
        }
    }
    // Fecha o diretorio
    closedir(dir);
}

/*
 * Funcao responsavel por ler o conteudo do txt e passar seu conteudo para a fila
 * */
void ler_txt(const char* caminho){

    FILE *txt;
    txt = fopen(caminho, "r");

    // Verifica se o txt eh um arquivp valido
    if(txt == NULL){
        printf("[ERRO] Arquivo no caminho seguinte foi ignorado devido a erro na abertura do mesmo: %s", caminho);
        return;
    }

    // Configuracao dos regex utilizados para validacao das palavras conforme documentacao
    regex_t reg_palavra_valida;

    // Compilar o regex
    if (regcomp(&reg_palavra_valida , "[a-zA-Z0-9_-]{4,}$", REG_EXTENDED) != 0) {
        printf("[ERRO] erro ao compilar o regex reg_palavra_valida\n");
        exit(1);
    }

    char linha[500], *ch, *palavra, caminho_com_linha[1024];
    int linha_atual = 0;

    // Percorrer o arquivo linha a linha
    while ((fgets(linha, sizeof(linha), txt)) != NULL){

        // Como o fgets faz a leitura linha a linha, vamos incrementar o contador para  sabermos em qual linha estamos
        ++linha_atual;

        // Verificamos a existencia de quebra de linha e o substituimos
        // strchr retorna o ponteiro do caractere, entao substituimos diretamente no registro.
        // aplicado para a exibicao do debug nao ficar desconfigurada
        if(ch = strchr(linha, '\n')){
            *ch = '\0';
        }

        // Separamos todas as palavras encontrada para colocar as mesmas na fila
        palavra = strtok(linha, " \t");
        while(palavra){

            // Analisa se a palavra segue os requisitos com regex
            if((regexec(&reg_palavra_valida, palavra, 0, NULL, 0)) == 0){

                // Formatamos o caminho para conter o numero da linha, para seguir os requisitos da documentacao
                snprintf(caminho_com_linha, sizeof(caminho_com_linha), "%s:%d", caminho, linha_atual);
                printf("%s --> %s\n", palavra, caminho_com_linha);

                // Colocamos palavra e caminho:linha onde foi encontrada
                //enqueue(palavra, caminho_com_linha);
            }
            palavra = strtok(NULL, " \t");
        }
    }
    fclose(txt);
    free(txt);
    free(palavra);
}