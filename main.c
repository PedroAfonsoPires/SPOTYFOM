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

// Funcoes auxiliares
Desc *CriaDesc() {
    Desc *desc = (Desc*)malloc(sizeof(Desc));
    desc->inicio = NULL;
    desc->tam = 1;
    return desc;
}

No* criaNo(Musica m) {
    No *novo = (No*) malloc(sizeof(No));
    if (novo != NULL) {
        novo->dado = m;
        novo->dado.execucoes = 0;
        novo->prox = NULL;
    }
    return novo;
}

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

void carregaMusicas(Desc *desc) {
    FILE *file = fopen("musicas.txt", "r");
    if (!file) {
        perror("Nao foi possivel abrir o arquivo");
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

void imprimeMusicas(Desc *lista) {
    No *atual = lista->inicio;
    int count = 0;

    while (atual != NULL) {
        printf("Titulo: %s\n", atual->dado.titulo);
        printf("Artista: %s\n", atual->dado.artista);
        printf("Letra: %s\n", atual->dado.letra);
        printf("Codigo: %d\n", atual->dado.codigo);
        printf("Execucoes: %d\n", atual->dado.execucoes);
        printf("------------------------------\n");

        count++;
        if (count % 200 == 0) {
            if (atual->prox == NULL) {
                printf("Todas as musicas foram impressas.\n");
                break;
            }

            char resposta;
            printf("Deseja imprimir mais 200 musicas? (s/n): ");
            scanf(" %c", &resposta);
            if (resposta != 's' && resposta != 'S') {
                break;
            }
        }

        atual = atual->prox;
    }

    if (atual == NULL && count % 200 != 0) {
        printf("Todas as musicas foram impressas.\n");
    }
}

// Funcoes da Fila
Fila* criaFila() {
    Fila *f = (Fila*)malloc(sizeof(Fila));
    f->inicio = NULL;
    f->fim = NULL;
    f->tam = 0;
    return f;
}

void enfileira(Fila *f, Musica m) {
    No *novo = criaNo(m);
    if (f->fim == NULL) {
        f->inicio = novo;
    } else {
        f->fim->prox = novo;
    }
    f->fim = novo;
    f->tam++;
}

Musica desenfileira(Fila *f) {
    if (f->inicio == NULL) {
        printf("Fila vazia!\n");
        Musica m = {"", "", "", 0, 0};
        return m;
    }
    No *remover = f->inicio;
    Musica m = remover->dado;
    f->inicio = f->inicio->prox;
    if (f->inicio == NULL) {
        f->fim = NULL;
    }
    free(remover);
    f->tam--;
    return m;
}

// Funcoes da Pilha
Pilha* criaPilha() {
    Pilha *p = (Pilha*)malloc(sizeof(Pilha));
    p->topo = NULL;
    p->tam = 0;
    return p;
}

void empilha(Pilha *p, Musica m) {
    No *novo = criaNo(m);
    if (p->topo == NULL) {
        p->topo = novo;
    } else {
        novo->prox = p->topo;
        p->topo = novo;
    }
    p->tam++;
}

Musica desempilha(Pilha *p) {
    if (p->topo == NULL) {
        printf("Pilha vazia!\n");
        Musica m = {"", "", "", 0, 0};
        return m;
    }
    No *remover = p->topo;
    Musica m = remover->dado;
    p->topo = p->topo->prox;
    free(remover);
    p->tam--;
    return m;
}

void executaMusicaPorCodigo(Desc *desc, int codigo, int execucoes) {
    No *atual = desc->inicio;
    while (atual != NULL) {
        if (atual->dado.codigo == codigo) {
            atual->dado.execucoes += execucoes;
            printf("Musica \"%s\" executada. Total de execucoes: %d\n", atual->dado.titulo, atual->dado.execucoes);
            return;
        }
        atual = atual->prox;
    }
    printf("Musica com codigo %d nao encontrada no acervo.\n", codigo);
}

void executaPlaylist(Fila *fila, Desc *desc) {
    printf("Executando playlist...\n");
    while (fila->tam > 0) {
        Musica m = desenfileira(fila);
        executaMusicaPorCodigo(desc, m.codigo, m.execucoes);  
    }
}

void executaPilha(Pilha *pilha, Desc *desc) {
    printf("Executando playlist...\n");
    while (pilha->tam > 0) {
        Musica m = desempilha(pilha);
        executaMusicaPorCodigo(desc, m.codigo, m.execucoes);
    }
}

void buscaPorCodigo(Desc *lista, int codigo) {
    No *atual = lista->inicio;
    while (atual != NULL) {
        if (atual->dado.codigo == codigo) {
            printf("Titulo: %s\n", atual->dado.titulo);
            printf("Artista: %s\n", atual->dado.artista);
            printf("Letra: %s\n", atual->dado.letra);
            printf("Codigo: %d\n", atual->dado.codigo);
            printf("Execucoes: %d\n", atual->dado.execucoes);
            printf("------------------------------\n");
            return;
        }
        atual = atual->prox;
    }
    printf("Musica com codigo %d nao encontrada.\n", codigo);
}

void buscaPorTitulo(Desc *lista, const char *titulo) {
    No *atual = lista->inicio;
    while (atual != NULL) {
        if (strcmp(atual->dado.titulo, titulo) == 0) {
            printf("Titulo: %s\n", atual->dado.titulo);
            printf("Artista: %s\n", atual->dado.artista);
            printf("Letra: %s\n", atual->dado.letra);
            printf("Codigo: %d\n", atual->dado.codigo);
            printf("Execucoes: %d\n", atual->dado.execucoes);
            printf("------------------------------\n");
            return;
        }
        atual = atual->prox;
    }
    printf("Musica com titulo \"%s\" nao encontrada.\n", titulo);
}

void buscaPorArtista(Desc *lista, const char *artista) {
    No *atual = lista->inicio;
    while (atual != NULL) {
        if (strcmp(atual->dado.artista, artista) == 0) {
            printf("Titulo: %s\n", atual->dado.titulo);
            printf("Artista: %s\n", atual->dado.artista);
            printf("Letra: %s\n", atual->dado.letra);
            printf("Codigo: %d\n", atual->dado.codigo);
            printf("Execucoes: %d\n", atual->dado.execucoes);
            printf("------------------------------\n");
        }
        atual = atual->prox;
    }
}

void busca(Desc *desc) {
    int opcao;
    int codigo;
    char titulo[MAX_CHAR];
    char artista[MAX_CHAR];

    printf("\nBuscar Musica:\n");
    printf("1. Por codigo\n");
    printf("2. Por titulo\n");
    printf("3. Por artista\n");
    printf("Escolha uma opcao: ");
    scanf("%d", &opcao);
    getchar();

    switch (opcao) {
        case 1:
            printf("Digite o codigo da musica: ");
            scanf("%d", &codigo);
            getchar();
            buscaPorCodigo(desc, codigo);
            break;
        case 2:
            printf("Digite o titulo da musica: ");
            fgets(titulo, MAX_CHAR, stdin);
            titulo[strcspn(titulo, "\n")] = '\0';
            buscaPorTitulo(desc, titulo);
            break;
        case 3:
            printf("Digite o artista da musica: ");
            fgets(artista, MAX_CHAR, stdin);
            artista[strcspn(artista, "\n")] = '\0';
            buscaPorArtista(desc, artista);
            break;
        default:
            printf("Opcao invalida! Tente novamente.\n");
    }
}

void filaAleatoria(Desc *desc, Fila *fila, int n) {
    No *atual;
    int contador = 0;

    srand(time(NULL));

    while (contador < n) {
        int aleatorio = rand() % desc->tam;
        atual = desc->inicio;

        for (int i = 0; i < aleatorio; i++) {
            atual = atual->prox;
        }
        Musica m = atual->dado;
        m.execucoes = 1;
        enfileira(fila, m);
        contador++;
    }

    printf("Playlist aleatoria criada.\n");
}

void playlistPessoal(Desc *desc, Pilha *pilha, int n) {
    char titulo[MAX_CHAR];
    int contador = 0;

    while (contador < n) {
        printf("Digite o titulo da musica para adicionar a playlist (ou 'sair' para finalizar): ");
        fgets(titulo, MAX_CHAR, stdin);
        titulo[strcspn(titulo, "\n")] = '\0';

        if (strcmp(titulo, "sair") == 0) {
            break;
        } else {
            No *atual = desc->inicio;
            while (atual != NULL) {
                if (strcmp(atual->dado.titulo, titulo) == 0) {
                    printf("Digite o numero de execucoes para a musica \"%s\": ", titulo);
                    int execucoes;
                    scanf("%d", &execucoes);
                    getchar();
                    Musica m = atual->dado;
                    m.execucoes = execucoes;
                    empilha(pilha, m);
                    printf("Musica \"%s\" adicionada a playlist.\n", titulo);
                    contador++;
                    break;
                }
                atual = atual->prox;
            }
            if (atual == NULL) {
                printf("Musica \"%s\" nao encontrada.\n", titulo);
            }
        }
    }
}

void gerenciarPlaylist(Desc *desc, Fila **fila, Pilha **pilha) {
    int opcao;
    int n;

    printf("\nGerenciar Playlist:\n");
    printf("1. Criar playlist aleatoria\n");
    printf("2. Criar playlist pessoal\n");
    printf("Escolha uma opcao: ");
    scanf("%d", &opcao);
    getchar();

    switch (opcao) {
        case 1:
            printf("Digite o numero de musicas para a playlist aleatoria: ");
            scanf("%d", &n);
            getchar();
            *fila = criaFila();
            filaAleatoria(desc, *fila, n);
            break;
        case 2:
            printf("Digite o numero de musicas para a playlist pessoal: ");
            scanf("%d", &n);
            getchar();
            *pilha = criaPilha();
            playlistPessoal(desc, *pilha, n);
            break;
        default:
            printf("Opcao invalida! Tente novamente.\n");
    }
}

void exibirMenu() {
    printf("\nMenu:\n");
    printf("1. Execucao\n");
    printf("2. Playlist\n");
    printf("3. Busca\n");
    printf("4. Impressao\n");
    printf("5. Relatorio\n");
    printf("6. Back-up\n");
    printf("7. Sair\n");
    printf("Escolha uma opcao: ");
}

int main() {
    Desc *desc = CriaDesc();
    int opcao;
    Fila *fila = NULL;
    Pilha *pilha = NULL;

    carregaMusicas(desc);

    do {
        exibirMenu();
        scanf("%d", &opcao);
        getchar();

        switch (opcao) {
            case 1:
                if (fila == NULL && pilha == NULL) {
                    printf("Nenhuma playlist criada. Crie uma playlist primeiro.\n");
                } else {
                    int opcaoExec;
                    printf("\nExecucao:\n");
                    printf("1. Executar playlist aleatoria\n");
                    printf("2. Executar playlist pessoal\n");
                    printf("Escolha uma opcao: ");
                    scanf("%d", &opcaoExec);
                    getchar();

                    if (opcaoExec == 1 && fila != NULL) {
                        executaPlaylist(fila, desc);
                        free(fila);
                        fila = NULL;
                    } else if (opcaoExec == 2 && pilha != NULL) {
                        executaPilha(pilha, desc);
                        free(pilha);
                        pilha = NULL;
                    } else {
                        printf("Opcao invalida ou playlist nao criada. Tente novamente.\n");
                    }
                }
                break;
            case 2:
                gerenciarPlaylist(desc, &fila, &pilha);
                break;
            case 3:
                busca(desc);
                break;
            case 4:
                imprimeMusicas(desc);
                break;
            case 5:
                printf("Relatorio nao implementado.\n");
                break;
            case 6:
                printf("Back-up nao implementado.\n");
                break;
            case 7:
                printf("Saindo...\n");
                break;
            default:
                printf("Opcao invalida! Tente novamente.\n");
        }
    } while (opcao != 7);

    No *atual = desc->inicio;
    while (atual != NULL) {
        No *temp = atual;
        atual = atual->prox;
        free(temp);
    }
    free(desc);

    return 0;
}