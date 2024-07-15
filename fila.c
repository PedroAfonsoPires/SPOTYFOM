#include "musica.h"

// Função para criar uma fila vazia
Fila* criaFila() {
    Fila *f = (Fila*)malloc(sizeof(Fila)); 
    f->inicio = NULL; 
    f->fim = NULL; 
    f->tam = 0; 
    return f; 
}

// Função para enfileirar uma música na fila
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

// Função para desenfileirar uma música da fila
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

// Função para executar a playlist aleatória
void executaPlaylist(Fila *fila, Desc *desc) {
    printf("Executando playlist aleatoria...\n");  
    while (fila->tam > 0) { 
        Musica m = desenfileira(fila);  
        executaMusicaPorCodigo(desc, m.codigo, 1);  
    }
}