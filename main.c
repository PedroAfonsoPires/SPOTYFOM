#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

Desc *CriaDesc() {
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
    } else {
        No *atual = desc->inicio;
        while (atual->prox != NULL) {
            atual = atual->prox;
        }
        atual->prox = novo;
    }
    desc->tam++;
}

// Função para ler o arquivo e carregar as músicas
void carregaMusicas(Desc *desc) {
    FILE *file = fopen("musicas.txt", "r");
    if (!file) {
        perror("Não foi possível abrir o arquivo");
        return;
    }

    Musica m;
    char buffer[MAX_CHAR];
    while (fgets(buffer, MAX_CHAR, file) != NULL) {
        if (sscanf(buffer, "%[^;];%d;%[^;];%[^;];%*s",
                   m.artista, &m.codigo, m.titulo, m.letra) == 4) {
            insereNo(m, desc);
        }
    }

    fclose(file);
}

// Função para imprimir as músicas paginadas
void imprimeMusicas(Desc *lista) {
    No *atual = lista->inicio;
    int count = 0;

    while (atual != NULL) {
        printf("Título: %s\n", atual->dado.titulo);
        printf("Artista: %s\n", atual->dado.artista);
        printf("Letra: %s\n", atual->dado.letra);
        printf("Código: %d\n", atual->dado.codigo);
        printf("------------------------------\n");

        count++;
        if (count % 200 == 0) {
            if (atual->prox == NULL) {
                printf("Todas as músicas foram impressas.\n");
                break;
            }

            char resposta;
            printf("Deseja imprimir mais 200 músicas? (s/n): ");
            scanf(" %c", &resposta);
            if (resposta != 's' && resposta != 'S') {
                break;
            }
        }

        atual = atual->prox;
    }

    if (atual == NULL && count % 200 != 0) {
        printf("Todas as músicas foram impressas.\n");
    }
}

// Funções placeholder para as opções do menu
void executaMusicas() {
    printf("Executar músicas.\n");
    // Implementar função
}

void playlist() {
    printf("Gerenciar playlist.\n");
    // Implementar função
}

void busca() {
    printf("Buscar músicas.\n");
    // Implementar função
}

void relatorio() {
    printf("Gerar relatório.\n");
    // Implementar função
}

void backup() {
    printf("Fazer back-up.\n");
    // Implementar função
}

void exibirMenu() {
    printf("\nMenu:\n");
    printf("1. Execução\n");
    printf("2. Playlist\n");
    printf("3. Busca\n");
    printf("4. Impressão\n");
    printf("5. Relatório\n");
    printf("6. Back-up\n");
    printf("7. Sair\n");
    printf("Escolha uma opção: ");
}

int main() {
    Desc *desc = CriaDesc();
    int opcao;

    carregaMusicas(desc);

    do {
        exibirMenu();
        scanf("%d", &opcao);

        switch (opcao) {
            case 1:
                executaMusicas();
                break;
            case 2:
                playlist();
                break;
            case 3:
                busca();
                break;
            case 4:
                imprimeMusicas(desc);
                break;
            case 5:
                relatorio();
                break;
            case 6:
                backup();
                break;
            case 7:
                printf("Saindo...\n");
                break;
            default:
                printf("Opção inválida! Tente novamente.\n");
        }
    } while (opcao != 7);

    // Liberação de memória
    No *atual = desc->inicio;
    while (atual != NULL) {
        No *temp = atual;
        atual = atual->prox;
        free(temp);
    }
    free(desc);

    return 0;
}