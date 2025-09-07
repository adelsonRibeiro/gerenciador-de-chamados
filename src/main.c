#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "gerenciador.h"

int main() {
    char escolha[3];
    bool sair = false;
    // free antes de encerrar o sistema
    atexit(limpa_memoria);

    while (!sair) {
        printf("\n");
        printf("Gerenciador de chamados:\n");
        printf("------------------------------------------------------------\n");
        printf("[1] Cadastrar chamado\n");
        printf("[2] Listar chamados\n");
        printf("[3] Executar atendimento\n");
        printf("[4] Buscar chamado\n");
        printf("[5] Estatisticas\n");
        printf("[6] Sair\n");
        printf("------------------------------------------------------------\n");
        printf("Escolha uma opcao: ");

        if (fgets(escolha, sizeof(escolha), stdin) != NULL) {
            long convertido = strtol(escolha, NULL, 10);

            switch (convertido) {
                case 1:
                    entrada_dados();
                    break;

                case 2:
                    listar_processo();
                    break;

                case 3:
                    executa_atendimento();
                    break;

                case 4:
                    buscar_processo();
                    break;

                case 5:
                    estatisticas();
                    break;

                case 6:
                    sair = true;
                    printf("Estatisticas: .\n");
                    estatisticas();
                    printf("Sistema encerrado.\n");
                    break;

                default:
                    printf("Opcao invalida.\n");
                    break;
            }
        }
    }

    return 0;
}
