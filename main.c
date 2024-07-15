#include "musica.h"

// Função para exibir o menu
void exibirMenu() {
    printf("\nMenu:\n");
    printf("1. Execucao\n");
    printf("2. Playlist\n");
    printf("3. Busca\n");
    printf("4. Impressao\n");
    printf("5. Relatorio\n");
    printf("6. Sair\n");
    printf("Escolha uma opcao: ");
}

// Função principal
int main() {
    Desc *desc = CriaDesc();  
    int opcao; 
    Fila *fila = NULL; 
    Pilha *pilha = NULL;  

    carregaMusicas(desc);  

    // Loop do menu principal
    do {
        exibirMenu();  
        scanf("%d", &opcao); 
        getchar();  

        switch (opcao) {
            case 1:  // Opção 1: Execução
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
            case 2:  // Opção 2: Gerenciar Playlist
                gerenciarPlaylist(desc, &fila, &pilha);  
                break;
            case 3:  // Opção 3: Busca
                busca(desc);  
                break;
            case 4:  // Opção 4: Impressão
                buscaOuImpressao(desc);  
                break;
            case 5:  // Opção 5: Relatório
                geraRelatorio(desc, fila, pilha);  
                break;
            case 6:  // Opção 6: Sair
                printf("Saindo\n");  
                break;
            default:  // Opção inválida
                printf("Opcao invalida! Tente novamente.\n");
        }
    } while (opcao != 6);  

    // Libera memória das playlists se ainda existirem
    if (fila != NULL) {
        free(fila);
    }
    if (pilha != NULL) {
        free(pilha);
    }

    return 0;  
}
