#include "arquivos.h/bibliotecas.h"
#include "arquivos.h/musica.h"

int main() {
    FILE *file = fopen("musicas.txt", "r");
    if (file == NULL) {
        printf("Erro ao abrir o arquivo de entrada.\n");
        return 1;
    }

    int quantidade;
    fscanf(file, "%d\n", &quantidade); // Leitura da quantidade de músicas

    // Alocação dinâmica da lista de músicas
    Musica **musicas = (Musica **)malloc(quantidade * sizeof(Musica *));
    if (musicas == NULL) {
        printf("Erro ao alocar memória.\n");
        fclose(file);
        return 1;
    }

    // Leitura dos dados das músicas
    for (int i = 0; i < quantidade; i++) {
        char titulo[MAX], artista[MAX], letra[MAX];
        int codigo, execucoes;
        fscanf(file, "%[^;];%[^;];%[^;];%d;%d\n", titulo, artista, letra, &codigo, &execucoes);
        musicas[i] = criaMusica(titulo, artista, letra, codigo, execucoes);
    }

    fclose(file);

    // Impressão da lista de músicas
    for (int i = 0; i < quantidade; i++) {
        imprimeMusica(musicas[i]);
        destroiMusica(musicas[i]);
    }

    // Liberação da memória alocada para a lista de músicas
    free(musicas);

    return 0;
}