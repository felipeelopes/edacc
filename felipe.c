//
// Created by felipe on 19/10/18.
//

#include "felipe.h"


struct Noh {
    char palavra[MAX_SIZE];
    struct Noh* proximo;
    struct Lista* sublista; // Armazena uma nova lista, para criar a sublista
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
        char caminho[MAX_SIZE];

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

    char linha[MAX_SIZE], *ch, *palavra, caminho_com_linha[MAX_SIZE];
    int linha_atual = 0;

    // Percorrer o arquivo linha a linha
    while ((fgets(linha, sizeof(linha), txt)) != NULL){

        // Como o fgets faz a leitura linha a linha, vamos incrementar o contador para  sabermos em qual linha estamos
        ++linha_atual;

        // Necessario para nao pular uma linha desnecessaria.
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

/*
 * Insere novas palavras na lista.
 * Para cada palavra criada, uma sublista tambem eh criada, para conter o caminho:linha
 * Caso a palavra seja repetida, somente seu caminho:linha eh adicionado na sublista
 * */
Noh* inserir(Lista* l, const char* palavra, const char* caminho_com_linha){

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
                if(strcasecmp(u->proximo->palavra, palavra) > 0){
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
        return n;
    } else {
        // Caso a palavra ja exista na lista, devemos somente adicionar o caminho em que sua repetida foi achada.
        inserir_sublista(busca_resultado->sublista, caminho_com_linha);
        return NULL;
    }
}

/*
 * Adiciona caminho:linha na sublista de um Noh(Palavra)
 * */
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

void destroi(Lista* l) {
    while (!underflow(l)) {
        Noh* n = l->cabeca;
        l->cabeca = n->proximo;
        free(n);
    }
    free(l);
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


/*
 * Funcao para procurar uma palavra e retornar o Noh caso ache.
 * Essa funcao eh usada ao inserir uma nova palavra
 * */
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

/*
 * Funcao utilizada para imprimir palavras encontradas e o numero de ocorrencias da mesma
 * Chamado no menu principal
 * */
void busca_palavra(const Lista* l, char* palavra){
    Noh* np = l->cabeca;
    while (np != NULL){

        // Lembrando, nao estamos considerando casesensitive
        if(strcasecmp(np->palavra, palavra) == 0){

            Noh* ns = np->sublista->cabeca;
            printf("Palavra encontrada nos seguintes locais: \n");

            // Imprime a sublista do Noh, contendo todos os caminhos:linhas encontrados.
            while(ns != NULL){
                printf("%s\n", ns->palavra);
                ns = ns->proximo;
            }
            printf("Total de ocorrencias dessa palavra: %d\n", np->sublista->total_palavras);

            // Caso chegue aqui, entao achamos a palavra, vamos encessar o laco
            return;
        }
        np = np->proximo;
    }
    printf(" A palavra digitada nao foi encontrada.\n");
}

void salvar(const Lista* l, const char* caminho_arquivo){
    FILE *arq;

    arq = fopen(caminho_arquivo, "wb");

    // Verifica se o arquivo foi criado e aberto
    if (arq != NULL) {
        // Cria ponteiro para lista
        Noh *np = l->cabeca;

        // Faz leitura da lista
        while (np != NULL) {
            //Escrever o noh atual no arquivo
            fprintf(arq, "%s;", np->palavra);

            // Faz a leitura da sub-lista caso exista
            if(!underflow(np->sublista)) {
                // Cria ponteiro para sublista do noh atual
                Noh *ns = np->sublista->cabeca;

                while (ns != NULL) {

                    // Se eu estiver no ultimo noh nao coloco o token ;
                    if (ns->proximo == NULL) {
                        fprintf(arq, "%s\n", ns->palavra);
                    } else {
                        // Caso eu nao seja o ultimo Noh eu adiciono o token ; para separar os Nohs da sublista
                        fprintf(arq, "%s;", ns->palavra);
                    }
                    ns = ns->proximo;
                }
                free(ns);
            } else {
                fputs("\n", arq);
            }
            np = np->proximo;
        }
    } else {
        printf("[ERRO] Falha ao salvar o arquivo, verifique as permissoes do diretorio\n");
    }
    printf("\n\n[INFO] Arquivo salvo com sucesso no caminho: %s\n", caminho_arquivo);
    fclose(arq);
}

void carregar_arquivo(Lista* l, const char* caminho_arquivo){
    Noh* n = l->cabeca;

    // Vamos apagar as sublistas
    while (n != NULL) {
         //Vamos apagar as informacoes da sublista caso exista alguma
        if(!underflow(n->sublista)){
            destroi(n->sublista);
        }
        n = n->proximo;
    }

    // Apagamos somente os nohs da lista mantendo a lista preservada
    // para adicionarmos os valores contidos no banco de dados.
    while (!underflow(l)) {
        Noh* n = l->cabeca;
        l->cabeca = n->proximo;
        free(n);
    }

    // Agora inserimos na lista as informacoes conforme lido do arquivo salvo anteriormente
    // Token ; separa as palavreas
    // Toda primeira palavra encontrada sera o noh principal, se nao for a primeira palavra entra na sublista do Noh
    // Caractere \n significa final de um registro, ir para o proximo e repetir

    printf("Comecando a adicionar do arquivo\n");

    FILE *arq;

    // Abre o arquivo passado para leitura
    arq = fopen(caminho_arquivo, "r");

    char* palavra;
    char* caminho;
    char linha[MAX_SIZE];

    if(arq == NULL){
        printf("[ERRO] Falha ao abrir o arquivo informado\n");
        exit(-1);
    } else {

        // Variavel para verificar em que palavra estamos
        // Caso seja a primeira palavra entoa eh o Noh principal
        // Caso seja maior que 1 adicionamos na sublista
        int palavra_atual = 0;
        char *caminho_linha, *ch;
        char *palavra_tmp;


        while ((fgets(linha, sizeof(linha), arq)) != NULL) {

            // Necessario para nao pular uma linha no arquivo!
            if(ch = strchr(linha, '\n')){
                *ch = '\0';
            }

            palavra = strtok(linha, ";");
            palavra_atual = 0;

            Noh* tmp = cria();

            //Le as palavras separadas
            while (palavra) {
                ++palavra_atual;

                if(palavra_atual == 1){
                    palavra_tmp = palavra;
                    tmp = inserir(l, palavra_tmp, caminho_linha);
                }
                if(palavra_atual >= 2){
                    inserir_sublista(tmp->sublista, palavra);
                }

                palavra = strtok(NULL, ";");
            }
        }
    }
}