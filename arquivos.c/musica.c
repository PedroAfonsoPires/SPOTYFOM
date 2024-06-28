#include "../arquivos.h/musica.h"

// Função para criar uma nova música
Musica* criaMusica(char* titulo, char* artista, char* letra, int codigo, int execucoes) {
    Musica* m = (Musica*)malloc(sizeof(Musica));
    if (m != NULL) {
        strncpy(m->titulo, titulo, MAX);
        strncpy(m->artista, artista, MAX);
        strncpy(m->letra, letra, MAX);
        m->codigo = codigo;
        m->execucoes = execucoes;
    }
    return m;
}

// Função para imprimir uma música
void imprimeMusica(Musica* m) {
    if (m != NULL) {
        printf("Titulo: %s\n", m->titulo);
        printf("Artista: %s\n", m->artista);
        printf("Letra: %s\n", m->letra);
        printf("Codigo: %d\n", m->codigo);
        printf("Execucoes: %d\n", m->execucoes);
    }
}

// Função para liberar a memória de uma música
void destroiMusica(Musica* m) {
    if (m != NULL) {
        free(m);
    }
}
