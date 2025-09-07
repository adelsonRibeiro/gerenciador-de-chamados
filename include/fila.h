#ifndef FILA_H
#define FILA_H

#include "gerenciador.h"

extern Node *fila_processos;
extern Node *em_atendimento;
int inserir_fim(Chamados p);
void remover_processo(Node **inicio, int id);
void inicializa_atendimento();
void finaliza_atendimento();

#endif
