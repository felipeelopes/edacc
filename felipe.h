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
//void inserir(Lista*, TipoChave);

void inserir(Lista*, const char* palavra, const char* caminho_com_linha);
void inserir_sublista(Lista*, const char* caminho_com_linha);
void salvar(const Lista*);

#endif //FELIPE_H
