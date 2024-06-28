#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_CHAR 256

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

// Função para criar um novo nó
No* criaNo(Musica m) {
    No *novo = (No*) malloc(sizeof(No));
    if (novo != NULL) {
        novo->dado = m;
        novo->prox = NULL;
    }
    return novo;
}

// Função para inserir um nó no final da lista
void insereNo(No **lista, Musica m) {
    No *novo = criaNo(m);
    if (*lista == NULL) {
        *lista = novo;
    } else {
        No *atual = *lista;
        while (atual->prox != NULL) {
            atual = atual->prox;
        }
        atual->prox = novo;
    }
}

// Função para ler o arquivo e carregar as músicas
void carregaMusicas(No **lista) {
    FILE *file = fopen("musicas.txt", "r");
    if (!file) {
        perror("Não foi possível abrir o arquivo");
        return;
    }

    int quantidadeMusicas;
    fscanf(file, "%d\n", &quantidadeMusicas);

    Musica m;
    for (int i = 0; i < quantidadeMusicas; i++) {
        if (fscanf(file, "%255[^;];%255[^;];%255[^;];%d;%d\n",
               m.titulo, m.artista, m.letra, &m.codigo, &m.execucoes) == 5) {
            insereNo(lista, m);
        }
    }

    fclose(file);
}

// Função para imprimir as músicas
void imprimeMusicas(No *lista) {
    No *atual = lista;
    int contador = 0;

    while (atual != NULL && contador < 5000) {
        printf("Título: %s\n", atual->dado.titulo);
        printf("Artista: %s\n", atual->dado.artista);
        printf("Letra: %s\n", atual->dado.letra);
        printf("Código: %d\n", atual->dado.codigo);
        printf("Execuções: %d\n", atual->dado.execucoes);
        printf("------------------------------\n");
        atual = atual->prox;
        contador++;
    }
}

int main() {
    No *listaMusicas = NULL;
    carregaMusicas(&listaMusicas);
    imprimeMusicas(listaMusicas);

    // Liberação de memória
    No *atual = listaMusicas;
    while (atual != NULL) {
        No *temp = atual;
        atual = atual->prox;
        free(temp);
    }

    return 0;
}