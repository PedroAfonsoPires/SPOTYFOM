#include "musica.h"

// Função para criar uma pilha vazia
Pilha* criaPilha() {
    Pilha *p = (Pilha*)malloc(sizeof(Pilha)); 
    p->topo = NULL; 
    p->tam = 0; 
    return p;
}

// Função para empilhar uma música com número de execuções especificado
void empilha(Pilha *p, Musica m, int execucoes) {
    No *novo = criaNo(m); 
    novo->dado.execucoes = execucoes; 
    novo->prox = p->topo; 
    p->topo = novo; 
    p->tam++; 
}

// Função para desempilhar uma música da pilha
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

// Função para executar todas as músicas da pilha
void executaPilha(Pilha *pilha, Desc *desc) {
    printf("Executando playlist pessoal...\n"); 
    while (pilha->tam > 0) { 
        Musica m = desempilha(pilha); 
        executaMusicaPorCodigo(desc, m.codigo, m.execucoes); 
    }
}
