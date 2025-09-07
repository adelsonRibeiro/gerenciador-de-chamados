#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <limits.h>
#include <assert.h>
#include "pilha.h"
#include "utils.h"
#include "gerenciador.h"
#include "log.h"



// Cria os nós
static Node *criar_no(Chamados p){
    Node *novo = (Node *) malloc(sizeof(Node));
    if(novo == NULL){
        char *retorno = message_handler(LOG_ERROR, "Erro na alocacao de memoria!\n", __func__, __FILE__, __LINE__);
        criar_callback(retorno, LOG_WARN);
        free(retorno);
        return NULL;
    }
    novo->chamado = p;
    novo->anterior = NULL;
    novo->proximo = NULL;
    return novo;
}

// insere no inicio
int inserir_inicio(Chamados p){
    Node **inicio = &pilha_processos;
    Node *novo = criar_no(p);

    assert(novo != NULL);

    if(*inicio == NULL){ // lista vazia
        *inicio = novo;
        return 0;
    }
    Node *temp = *inicio;

    temp-> anterior = novo;
    novo->proximo = temp;
    *inicio = novo;

    return 0;
}
