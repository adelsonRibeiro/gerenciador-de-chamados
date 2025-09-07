#ifndef GERENCIADOR_H
#define GERENCIADOR_H

#include <stdbool.h>


typedef struct{
    int pid;
    char nome[50];
    int prioridade;
    char status[25];
} Chamados;

typedef struct Node{
    Chamados chamado;
    struct Node *anterior;
    struct Node *proximo;
} Node;


void entrada_dados();
void listar_processo();
void limpa_memoria();
void executa_atendimento();
void buscar_processo();
void estatisticas();

#endif
