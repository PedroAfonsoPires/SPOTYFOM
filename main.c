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

typedef struct desc{
    int tam;
    No *inicio;
}Desc;

Desc *CriaDesc(){
    Desc *desc = (Desc*)malloc(sizeof(Desc));
    desc->inicio = NULL;
    desc->tam = 0;

    return desc;
}

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
void insereNo(Musica m, Desc *desc) {
    No *novo = criaNo(m);
    if (desc->inicio == NULL) {
        desc->inicio = novo;
        desc->tam++;
    } else {
        No *atual = desc->inicio;
        while (atual->prox != NULL) {
            atual = atual->prox;
        }
        atual->prox = novo;
        desc->tam++;
    }
}

// Função para ler o arquivo e carregar as músicas
void carregaMusicas(Desc *desc) {
    FILE *file = fopen("musicas copy.txt", "r");
    if (!file) {
        perror("Não foi possível abrir o arquivo");
        return;
    }

    int quantidadeMusicas;
    fscanf(file, "%d\n", &quantidadeMusicas);

    Musica m;
    for (int i = 0; i < quantidadeMusicas; i++) {
        if (fscanf(file, "%[^;];%[^;];%[^;];%d;\n",
               m.artista, m.titulo, m.letra, &m.codigo) == 4) {
            insereNo(m, desc);
        }
    }

    fclose(file);
}

// Função para imprimir as músicas
void imprimeMusicas(Desc *lista) {
    No *atual = lista->inicio;

    while (atual != NULL) {
        printf("Título: %s\n", atual->dado.titulo);
        printf("Artista: %s\n", atual->dado.artista);
        printf("Letra: %s\n", atual->dado.letra);
        printf("Código: %d\n", atual->dado.codigo);
        printf("------------------------------\n");
        atual = atual->prox;
        lista->tam++;
    }
}

int main() {
    Desc *desc = NULL;
    desc=CriaDesc();

    carregaMusicas(desc);
    imprimeMusicas(desc);

    // Liberação de memória
    No *atual = desc->inicio;
    while (atual != NULL) {
        No *temp = atual;
        atual = atual->prox;
        free(temp);
    }

    return 0;
}