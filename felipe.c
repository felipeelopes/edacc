//
// Created by felipe on 19/10/18.
//

#include "felipe.h"


struct Noh {
    char palavra[1024];
    struct Noh* proximo;
    struct Lista* sublista;
};
typedef struct Noh Noh;

struct Lista {
    int total_palavras;
    Noh* cabeca;
};

Lista* cria(void) {
    Lista* l = malloc(sizeof(Lista));
    l->total_palavras = 0;
    l->cabeca = NULL;
    return l;
}

/*
 * Funcao responsavel por varrer o diretorio e sub-diretorios do caminho informado pelo usuario
 * */
void ler_dir(Lista* l, const char *name) {
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
            ler_dir(l, caminho);


        // Caso seja arquivo
        // Aqui temos que abrir cada arquivo .txt encontrado e adicionar seu conteudo na fila
        } else {
            snprintf(caminho, sizeof(caminho), "%s/%s", name, entrada->d_name);

            // Caso o arquivo nao seja txt, ignoramos o mesmo e continuamos
            // regexec retorna 0 caso o termo seja correspondente
            if ((regexec(&reg_txt, caminho, 0, (regmatch_t *)NULL, 0)) != 0){
                continue;
            } else {
                ler_txt(l, caminho); // para cada txt encontrado devemos abrir o mesmo e adicionar seu conteudo na pilha
            }
        }
    }
    // Fecha o diretorio
    closedir(dir);
}

/*
 * Funcao responsavel por ler o conteudo do txt e passar seu conteudo para a fila
 * */
void ler_txt(Lista* l, const char* caminho){

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
                //printf("%s --> %s\n", palavra, caminho_com_linha);

                // Colocamos palavra e caminho:linha onde foi encontrada
                inserir(l, palavra, caminho_com_linha);
            }
            palavra = strtok(NULL, " \t");
        }
    }
    fclose(txt);
    free(txt);
}

bool underflow(const Lista* l) {
    return l->cabeca == NULL;
}

bool overflow(const Lista* l) {
    Noh* f = malloc(sizeof(Noh));
    if (f == NULL) {
        return true;
    }
    free(f);
    return false;
}

void inserir(Lista* l, const char* palavra, const char* caminho_com_linha){

    // Fazemos uma busca para verificar se a palavra ja existe na lista
    Noh* busca_resultado = busca(l, palavra);

    // Caso nao exista, adicionamos a mesma na lista. O retorno eh NULL se nao existir, se existir o retorno eh o ponteiro para o Noh que contem a palavra
    if(busca_resultado == NULL){
        // cria um novo noh que guardara a nova palavra
        Noh* n = malloc(sizeof(Noh));

        // cria uma sub-lista para cada Noh adicionado.
        n->sublista = cria();

        // Copia a palavra passada pelo parametro para o noh criado
        strcpy(n->palavra, palavra);

        // Define o proximo Noh do novo Noh como NULL
        n->proximo = NULL;
        ++l->total_palavras;

        // Caso a lista nao esteja vazia temos que buscar a posicao em ordem alfabetica para inserir o novo Noh
        if (!underflow(l)) {
            Noh *u = l->cabeca;
            while (u->proximo != NULL) {

                // Aqui comparamos se a palavra atual deve estar antes que a proxima palavra armazenada na lista
                // A funcao strcasecmp ignora o casesensitive, entao A e a eh a mesma coisa.
                // Retorno > 0 a string eh 'menor' que a proxima
                // Retorno < 0 a string eh 'maior' que a proxima
                // Retorno = 0 a string eh igual a proxima
                if(strcasecmp(u->proximo->palavra, palavra) > 0 && u->proximo != NULL){
                    // Adiciono o caminho no noh que estou inserindo
                    inserir_sublista(n->sublista, caminho_com_linha);

                    // Marco o proximo elemento do noh que estou adicionando como a prox palavra
                    n->proximo = u->proximo;
                    u->proximo = n;

                    // Paro o loop pois ja adicionei a palavra na lista
                    break;
                } else if(u->proximo == NULL){ // Se chegar aqui significa que estou no ultimo Noh, entao so adicionamos a palavra
                    u->proximo = n;
                }

                // Andamos para o proximo elemento do da lista
                u = u->proximo;
            }

            u->proximo = n;
        } else {
            n->proximo = l->cabeca;
            l->cabeca = n;
        }
    } else {

        // Caso a palavra ja exista na lista, devemos somente adicionar o caminho em que sua repetida foi achada.
        inserir_sublista(busca_resultado->sublista, caminho_com_linha);
    }
}

void inserir_sublista(Lista* l, const char* caminho_com_linha){

    // cria um novo noh que guardara a nova palavra
    Noh* n = malloc(sizeof(Noh));

    // Copia a palavra passada pelo parametro para o noh criado
    strcpy(n->palavra, caminho_com_linha);

    // Define o proximo Noh do novo Noh como NULL
    n->proximo = NULL;
    ++l->total_palavras;

    // Caso a lista nao esteja vazia temos que buscar a posicao em ordem alfabetica para inserir o novo Noh
    if (!underflow(l)) {
        Noh* u = l->cabeca;
        while (u->proximo != NULL) {
            u = u->proximo;
        }
        u->proximo = n;
    } else {
        n->proximo = l->cabeca;
        l->cabeca = n;
    }
}

void imprime(const Lista* l) {
    if (underflow(l)) {
        printf("(VAZIA)");
        return;
    }
    printf("(CABECA) ");
    Noh* n = l->cabeca;
    while (n != NULL) {
        printf("%s", n->palavra);
        printf("-->");
        n = n->proximo;
    }
    printf("NULL (CAUDA)\n");
    printf("Total de palavras armazenadas: %d\n", l->total_palavras);
}

Noh* busca(const Lista* l, char* palavra){
    Noh* n = l->cabeca;
    while (n != NULL) {

        // Caso eu encontre a palavra eu retorno o noh onde a mesma se encontra caso contrario retorno NULL
        // Optei por usar o strcasecmp para ignorar o casesensitive
        if (strcasecmp(n->palavra, palavra) == 0) {
            return n;
        }
        n = n->proximo;
    }
    return NULL;
}

void salvar(const Lista* lista){
    int salv;
    FILE *arq;
    char nomearq[1024];
    printf("Digite o nome do arquivo para ser salvo\n");
    scanf('%c', &nomearq);

    arq = fopen(nomearq, "wb");
    if (arq != NULL) {
        salv = fwrite(lista, sizeof(Lista),1, arq);
        }
    else
        puts("Erro: abertura do arquivo");
}


