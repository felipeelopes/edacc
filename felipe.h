//
// Created by felipe on 19/10/18.
//
#ifndef FELIPE_H
#define FELIPE_H

#include <unistd.h>
#include <sys/types.h>
#include <dirent.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <regex.h>
#include <stdlib.h>
#include <ctype.h>

#define MAX_SIZE 1024

struct Noh;
typedef  struct Noh Noh;
struct Lista;
typedef struct Lista Lista;


// Operacoes com diretorios e arquivos
void ler_dir(Lista*, const char*);
void ler_txt(Lista*, const char*);

// Operacoes TAD
Lista* cria(void);
void destroi(Lista*);
bool underflow(const Lista*);
bool overflow(const Lista*);
void imprime(const Lista*);
Noh* busca(const Lista*, char*);
void busca_palavra(const Lista*, char*);
Noh* inserir(Lista*, const char* palavra, const char*);
void inserir_sublista(Lista*, const char*);
void salvar(const Lista*, const char*);
void carregar_arquivo(Lista*, const char*);
void sincronizar(Lista*, const char*);

#endif //FELIPE_H
