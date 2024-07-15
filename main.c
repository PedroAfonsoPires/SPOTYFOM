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
    desc->tam = 0;
    return desc;
}

No* criaNo(Musica m) {
    No *novo = (No*) malloc(sizeof(No));
    if (novo != NULL) {
        novo->dado = m;
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
            m.execucoes = 0; // Inicializa execucoes como 0
            insereNo(m, desc);
        }
    }

    fclose(file);
}

void imprimeMusica(Musica m) {
    printf("\nTitulo: %s\n", m.titulo);
    printf("Artista: %s\n", m.artista);
    printf("Letra: %s\n", m.letra);
    printf("Codigo: %d\n", m.codigo);
    printf("------------------------------\n");
}

void imprimeBuscaPorTituloOuTodas(Desc *lista, const char *titulo) {
    No *atual = lista->inicio;
    int count = 0;
    int total = 0;
    int todasImpressas = 0; // Variável para controlar se todas as músicas foram impressas

    if (titulo == NULL || strlen(titulo) == 0) {
        printf("Imprimindo todas as musicas do acervo:\n");
        while (atual != NULL) {
            imprimeMusica(atual->dado);
            count++;
            total++;
            if (count % 200 == 0) {
                char resposta;
                printf("Deseja imprimir mais 200 musicas? (s/n): ");
                scanf(" %c", &resposta);
                getchar();
                if (resposta != 's' && resposta != 'S') {
                    break; // Sai do loop de impressão
                }
            }
            atual = atual->prox;
        }
        if (atual == NULL) { // Verifica se todas as músicas foram impressas
            todasImpressas = 1;
        }
    } else {
        while (atual != NULL) {
            if (strcmp(atual->dado.titulo, titulo) == 0) {
                imprimeMusica(atual->dado);
                return;
            }
            atual = atual->prox;
        }
        printf("Musica com titulo \"%s\" nao encontrada.\n", titulo);
    }

    if (todasImpressas && (titulo == NULL || strlen(titulo) == 0)) {
        printf("\nTodas as musicas foram impressas.\n");
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
    novo->prox = p->topo;
    p->topo = novo;
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
            printf("Musica \"%s\" executada %d vez(es).\n", atual->dado.titulo, execucoes);
            return;
        }
        atual = atual->prox;
    }
    printf("Musica com codigo %d nao encontrada no acervo.\n", codigo);
}

void executaPlaylist(Fila *fila, Desc *desc) {
    printf("Executando playlist aleatoria...\n");
    while (fila->tam > 0) {
        Musica m = desenfileira(fila);
        executaMusicaPorCodigo(desc, m.codigo, 1);
    }
}

void executaPilha(Pilha *pilha, Desc *desc) {
    printf("Executando playlist pessoal...\n");
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
            printf("------------------------------\n");
        }
        atual = atual->prox;
    }
}

void busca(Desc *lista) {
    int opcao, codigo;
    char titulo[MAX_CHAR], artista[MAX_CHAR];

    printf("Busca:\n");
    printf("1. Por Codigo\n");
    printf("2. Por Titulo\n");
    printf("3. Por Artista\n");
    printf("Escolha uma opcao: ");
    scanf("%d", &opcao);
    getchar();

    switch (opcao) {
        case 1:
            printf("Digite o codigo da musica: ");
            scanf("%d", &codigo);
            buscaPorCodigo(lista, codigo);
            break;
        case 2:
            printf("Digite o titulo da musica: ");
            fgets(titulo, MAX_CHAR, stdin);
            titulo[strcspn(titulo, "\n")] = '\0';
            buscaPorTitulo(lista, titulo);
            break;
        case 3:
            printf("Digite o nome do artista: ");
            fgets(artista, MAX_CHAR, stdin);
            artista[strcspn(artista, "\n")] = '\0';
            buscaPorArtista(lista, artista);
            break;
        default:
            printf("Opcao invalida! Tente novamente.\n");
    }
}

void geraRelatorioPlaylist(Fila *fila, Pilha *pilha, const char *filename) {
    FILE *file = fopen(filename, "w");
    if (!file) {
        perror("Nao foi possivel criar o arquivo");
        return;
    }

    if (fila != NULL) {
        fprintf(file, "Relatorio da Playlist Aleatoria:\n\n");
        No *atual = fila->inicio;
        while (atual != NULL) {
            fprintf(file, "Titulo: %s\n", atual->dado.titulo);
            fprintf(file, "Artista: %s\n", atual->dado.artista);
            fprintf(file, "Letra: %s\n", atual->dado.letra);
            fprintf(file, "Codigo: %d\n", atual->dado.codigo);
            fprintf(file, "Execucoes: %d\n", atual->dado.execucoes);
            fprintf(file, "------------------------------\n");
            atual = atual->prox;
        }
    }

    if (pilha != NULL) {
        fprintf(file, "Relatorio da Playlist Pessoal:\n\n");
        No *atual = pilha->topo;
        while (atual != NULL) {
            fprintf(file, "Titulo: %s\n", atual->dado.titulo);
            fprintf(file, "Artista: %s\n", atual->dado.artista);
            fprintf(file, "Letra: %s\n", atual->dado.letra);
            fprintf(file, "Codigo: %d\n", atual->dado.codigo);
            fprintf(file, "Execucoes: %d\n", atual->dado.execucoes);
            fprintf(file, "------------------------------\n");
            atual = atual->prox;
        }
    }

    fclose(file);
    printf("Relatorio gerado com sucesso em %s.\n", filename);
}

void geraRelatorioAcervo(Desc *desc, const char *filename) {
    FILE *file = fopen(filename, "w");
    if (!file) {
        perror("Nao foi possivel criar o arquivo");
        return;
    }

    No *atual = desc->inicio;
    fprintf(file, "Relatorio do Acervo Completo:\n\n");
    while (atual != NULL) {
        fprintf(file, "Titulo: %s\n", atual->dado.titulo);
        fprintf(file, "Artista: %s\n", atual->dado.artista);
        fprintf(file, "Letra: %s\n", atual->dado.letra);
        fprintf(file, "Codigo: %d\n", atual->dado.codigo);
        fprintf(file, "Execucoes: %d\n", atual->dado.execucoes);
        fprintf(file, "------------------------------\n");
        atual = atual->prox;
    }

    fclose(file);
    printf("Relatorio do acervo gerado com sucesso em %s.\n", filename);
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

void geraRelatorio(Desc *desc, Fila *fila, Pilha *pilha) {
    int opcao;
    char filename[MAX_CHAR];

    printf("\nGerar Relatorio:\n");
    printf("1. Playlist aleatoria\n");
    printf("2. Playlist pessoal\n");
    printf("3. Acervo completo\n");
    printf("Escolha uma opcao: ");
    scanf("%d", &opcao);
    getchar();

    printf("Digite o nome do arquivo para salvar o relatorio: ");
    fgets(filename, MAX_CHAR, stdin);
    filename[strcspn(filename, "\n")] = '\0';

    switch (opcao) {
        case 1:
            if (fila != NULL && fila->tam > 0) {
                geraRelatorioPlaylist(fila, NULL, filename);
            } else {
                printf("Nenhuma playlist aleatoria foi criada.\n");
            }
            break;
        case 2:
            if (pilha != NULL && pilha->tam > 0) {
                geraRelatorioPlaylist(NULL, pilha, filename);
            } else {
                printf("Nenhuma playlist pessoal foi criada.\n");
            }
            break;
        case 3:
            geraRelatorioAcervo(desc, filename);
            break;
        default:
            printf("Opcao invalida! Tente novamente.\n");
    }
}

void shuffle(No **array, int n) {
    if (n > 1) {
        srand(time(NULL)); // Inicializa o gerador de números aleatórios
        for (int i = 0; i < n - 1; i++) {
            int j = i + rand() / (RAND_MAX / (n - i) + 1);
            No *temp = array[j];
            array[j] = array[i];
            array[i] = temp;
        }
    }
}

void playlistAleatoria(Desc *desc, Fila *fila, int n) {
    if (n > desc->tam) {
        printf("Numero de musicas solicitado (%d) maior que o total disponivel (%d). Criando playlist com todas as musicas disponiveis.\n", n, desc->tam);
        n = desc->tam;
    }

    // Cria um array de ponteiros para os nós da lista
    No **array = (No**)malloc(desc->tam * sizeof(No*));
    No *atual = desc->inicio;
    int i = 0;

    // Preenche o array com ponteiros para os nós
    while (atual != NULL) {
        array[i++] = atual;
        atual = atual->prox;
    }

    // Embaralha o array
    shuffle(array, desc->tam);

    // Adiciona as primeiras n músicas embaralhadas na fila
    for (i = 0; i < n; i++) {
        enfileira(fila, array[i]->dado);
    }

    free(array);
    printf("Playlist aleatoria criada com %d musicas.\n", n);
}

void gerenciarPlaylist(Desc *desc, Fila **fila, Pilha **pilha) {
    int opcaoPlaylist, numMusicas, i, execucoes;
    No *musicaAtual;
    Musica musica;
    char titulo[MAX_CHAR];

    printf("\nPlaylist:\n");
    printf("1. Criar playlist aleatoria\n");
    printf("2. Criar playlist pessoal\n");
    printf("Escolha uma opcao: ");
    scanf("%d", &opcaoPlaylist);
    getchar();

    switch (opcaoPlaylist) {
        case 1:
            printf("Quantas musicas deseja colocar na playlist aleatoria? ");
            scanf("%d", &numMusicas);
            getchar();

            if (*fila != NULL) {
                // Libere a memória alocada para a fila antiga
                No *atual = (*fila)->inicio;
                while (atual != NULL) {
                    No *temp = atual;
                    atual = atual->prox;
                    free(temp);
                }
                free(*fila);
            }
            *fila = criaFila();

            playlistAleatoria(desc, *fila, numMusicas); // Gera a playlist aleatória

            break;

        case 2:
            printf("Quantas musicas deseja colocar na playlist pessoal? ");
            scanf("%d", &numMusicas);
            getchar();

            if (*pilha != NULL) {
                // Libere a memória alocada para a pilha antiga
                No *atual = (*pilha)->topo;
                while (atual != NULL) {
                    No *temp = atual;
                    atual = atual->prox;
                    free(temp);
                }
                free(*pilha);
            }
            *pilha = criaPilha();

            int contador = 0;
            while (contador < numMusicas) {
                printf("Digite o titulo da musica para adicionar a playlist (ou 'sair' para finalizar): ");
                fgets(titulo, MAX_CHAR, stdin);
                titulo[strcspn(titulo, "\n")] = '\0';

                if (strcmp(titulo, "sair") == 0) {
                    break;
                }

                musicaAtual = desc->inicio;
                while (musicaAtual != NULL) {
                    if (strcmp(musicaAtual->dado.titulo, titulo) == 0) {
                        printf("Digite o numero de execucoes para a musica '%s': ", titulo);
                        scanf("%d", &execucoes);
                        getchar();

                        musica = musicaAtual->dado;
                        execucoes = execucoes; // Atualiza o número de execuções

                        empilha(*pilha, musica);
                        contador++;
                        break;
                    }
                    musicaAtual = musicaAtual->prox;
                }

                if (musicaAtual == NULL) {
                    printf("Musica '%s' nao encontrada.\n", titulo);
                }
            }

            if (contador < numMusicas) {
                printf("Menos musicas do que o solicitado. Adicionadas %d musicas.\n", contador);
            } else {
                printf("Playlist pessoal criada com %d musicas.\n", numMusicas);
            }
            break;

        default:
            printf("Opcao invalida! Tente novamente.\n");
    }
}

void buscaOuImpressao(Desc *desc) {
    int opcao;
    char titulo[MAX_CHAR];

    printf("\nImpressao:\n");
    printf("1. Busca por Titulo\n");
    printf("2. Imprimir todas as musicas\n");
    printf("Escolha uma opcao: ");
    scanf("%d", &opcao);
    getchar();

    switch (opcao) {
        case 1:
            printf("Digite o titulo da musica: ");
            fgets(titulo, MAX_CHAR, stdin);
            titulo[strcspn(titulo, "\n")] = '\0';
            imprimeBuscaPorTituloOuTodas(desc, titulo);
            break;
        case 2:
            imprimeBuscaPorTituloOuTodas(desc, NULL);
            break;
        default:
            printf("Opcao invalida! Tente novamente.\n");
    }
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
                buscaOuImpressao(desc);
                break;
            case 5:
                geraRelatorio(desc, fila, pilha);
                break;
            case 6:
                // Implementar função de back-up se necessário
                printf("Funcao de back-up nao implementada.\n");
                break;
            case 7:
                printf("Saindo do programa...\n");
                break;
            default:
                printf("Opcao invalida! Tente novamente.\n");
        }
    } while (opcao != 7);

    // Libera memória das playlists se ainda existirem
    if (fila != NULL) {
        free(fila);
    }
    if (pilha != NULL) {
        free(pilha);
    }

    return 0;
}