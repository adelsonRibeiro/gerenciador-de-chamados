#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <limits.h>
#include <assert.h>
#include "fila.h"
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

// insere no fim da lista
int inserir_fim(Chamados p){
    Node **inicio = &fila_processos;
    Node *novo = criar_no(p);

    if(*inicio == NULL){ // lista vazia
        *inicio = novo;
        return 0;
    }
    Node *temp = *inicio;
    while(temp->proximo != NULL){ // percorre a list node até o fim para achar o ultimo item
        temp = temp->proximo;
    }

    temp-> proximo = novo;
    novo->anterior = temp;

    return 0;
}


void remover_processo(Node **inicio, int id){
    assert(inicio != NULL);
    Node *temp = *inicio;
    while(temp != NULL){
        if(temp->chamado.pid == id){
            if(temp->anterior != NULL){ // verifica se nao é o primeiro no
                temp->anterior->proximo = temp->proximo;
            }
            else{
                *inicio = temp->proximo;
            }
            if(temp->proximo !=NULL){ // verifica se nao é ultimo no
                temp->proximo->anterior = temp->anterior;
            }
            free(temp);
            return;
        }
        temp = temp->proximo;
    }
    char *retorno = message_handler(LOG_ERROR, "Processo nao encontrado\n", __func__, __FILE__, __LINE__);
    criar_callback(retorno, LOG_WARN);
    free(retorno);
}

void inicializa_atendimento(){
    assert(fila_processos != NULL);
    Node *inicio = fila_processos;
    Node *maior_prioridade = fila_processos;
    while(inicio != NULL){
        if(inicio->chamado.prioridade < maior_prioridade->chamado.prioridade){
            maior_prioridade = inicio;
        }
        inicio = inicio->proximo;
    }
    strcpy(maior_prioridade->chamado.status, "Em andamento");
    em_atendimento = maior_prioridade;
}

// muda o chamado da fila para a pilha
void finaliza_atendimento(){
    assert(em_atendimento != NULL);

    strcpy(em_atendimento->chamado.status, "Finalizado");
    inserir_inicio(em_atendimento->chamado);
    remover_processo(&fila_processos, em_atendimento->chamado.pid);
    em_atendimento = NULL;
    if(log_callback){
        printf("%s\n", log_callback->msg);
        free_callback();
    }
}
