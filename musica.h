#ifndef MUSICA_H
#define MUSICA_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_CHAR 1024

typedef struct musica {
    char titulo[MAX_CHAR];
    char artista[MAX_CHAR];
    char letra[MAX_CHAR];
    int codigo;
    int execucoes;
} Musica;

typedef struct no {
    Musica dado;
    struct no *prox;
} No;

typedef struct desc {
    int tam;
    No *inicio;
} Desc;

typedef struct fila {
    No *inicio;
    No *fim;
    int tam;
} Fila;

typedef struct pilha {
    No *topo;
    int tam;
} Pilha;

// Funções auxiliares
Desc *CriaDesc();
No* criaNo(Musica m);
void insereNo(Musica m, Desc *desc);
void carregaMusicas(Desc *desc);
void imprimeMusica(Musica m);
void imprimeBuscaPorTituloOuTodas(Desc *lista, const char *titulo);
void executaMusicaPorCodigo(Desc *desc, int codigo, int execucoes);
void buscaPorCodigo(Desc *lista, int codigo);
void buscaPorTitulo(Desc *lista, const char *titulo);
void buscaPorArtista(Desc *lista, const char *artista);
void busca(Desc *lista);
void geraRelatorioPlaylist(Fila *fila, Pilha *pilha, const char *filename);
void geraRelatorioAcervo(Desc *desc, const char *filename);
void geraRelatorio(Desc *desc, Fila *fila, Pilha *pilha);
void shuffle(No **array, int n);
void playlistAleatoria(Desc *desc, Fila *fila, int n);
void gerenciarPlaylist(Desc *desc, Fila **fila, Pilha **pilha);
void buscaOuImpressao(Desc *desc);

// Funções da Pilha
Pilha* criaPilha();
void empilha(Pilha *p, Musica m, int execucoes);
Musica desempilha(Pilha *p);
void executaPilha(Pilha *pilha, Desc *desc);

// Funções da Fila
Fila* criaFila();
void enfileira(Fila *f, Musica m);
Musica desenfileira(Fila *f);
void executaPlaylist(Fila *fila, Desc *desc);

#endif // MUSICA_H