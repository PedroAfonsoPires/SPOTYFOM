#ifndef MUSICA_H
#define MUSICA_H

#include "bibliotecas.h"

#define MAX 256

// Definição da estrutura musica
typedef struct musica {
    char titulo[MAX];
    char artista[MAX];
    char letra[MAX];
    int codigo;
    int execucoes;
} Musica;

// Função para criar uma nova música
Musica* criaMusica(char* titulo, char* artista, char* letra, int codigo, int execucoes);

// Função para imprimir uma música
void imprimeMusica(Musica* m);

// Função para liberar a memória de uma música
void destroiMusica(Musica* m);

#endif
