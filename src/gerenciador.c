#include "gerenciador.h"
#include "fila.h"
#include "log.h"
#include "pilha.h"
#include "utils.h"
#include <assert.h>
#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Node *fila_processos = NULL;
Node *pilha_processos = NULL;
Node *em_atendimento = NULL;
static int pid = 1;

static void menu_busca() {
  printf("------------------------------------------------------------\n");
  printf("[1] Listar chamados em aberto\n");
  printf("[2] Listar chamados em andamento\n");
  printf("[3] Listar chamados finalizados\n");
  printf("[4] Listar todos chamados\n");
  printf("[5] Sair\n");
  printf("------------------------------------------------------------\n");
  printf("Escolha uma opcao: ");
}

static void printar_chamado(Node *inicio) {
  assert(inicio != NULL);
  printf("|                                                            \n");
  printf("|   > %-10s   %-42d \n", "ID:", inicio->chamado.pid);
  printf("|   > %-10s   %-42s \n", "Titulo:", inicio->chamado.nome);
  printf("|   > %-10s  %-42d \n", "Prioridade:", inicio->chamado.prioridade);
  printf("|   > %-10s   %-42s \n", "Status:", inicio->chamado.status);
  printf("|                                                            \n");
  printf("'------------------------------------------------------------\n");
}
static void menu_atendimento() {
  printf("------------------------------------------------------------\n");
  printf("[1] Finalizar atendimento\n");
  printf("[2] Sair para o menu principal\n");
  printf("------------------------------------------------------------\n");
  printf("Escolha uma opcao: ");
}

void entrada_dados() {
  limparTela();
  char nome[50];
  char *nome_validado = NULL;
  char entrada_prioridade[20];
  int prioridade = -1;

  Chamados p;

  // inicializa um loop para validar os dados de entrada
  bool saida = false;
  while (saida == false) {
    if (nome_validado == NULL) {
      printf("%s", "Informe o titulo do chamado: ");
      if (fgets(nome, sizeof(nome), stdin) != NULL) {
        if (strchr(nome, '\n') == NULL) {
          // A linha foi muito longa. Precisamos limpar o buffer.
          int c;
          while ((c = getchar()) != '\n' && c != EOF)
            ;
        }

        // valida se o campo � nulo, avalia o tamanho que est� limitado a 50
        // char e remove espacos a frente e no fim
        nome_validado = valida_nome(nome);

        if (nome_validado != NULL) {
          saida = true;
        } else {
          printf("%s\n", log_callback->msg);
          free_callback();
        }
      }
    }
  }
  saida = false;
  while (saida == false) {
    if (prioridade < 0) {
      printf("%s", "Informe a prioridade do processo (1 a 5): ");
      if (fgets(entrada_prioridade, sizeof(entrada_prioridade), stdin) !=
          NULL) {
        if (strchr(entrada_prioridade, '\n') == NULL) {
          // A linha foi muito longa. Precisamos limpar o buffer.
          int c;
          while ((c = getchar()) != '\n' && c != EOF)
            ;
        }
        // valida se o campo � nulo, avalia o tamanho que est� limitado a 50
        // char e remove espacos a frente e no fim
        prioridade = valida_inteiro(entrada_prioridade);
        if (valida_prioridade(prioridade) != 0) {
          prioridade = -1;
        }
        if (prioridade != -1) {
          saida = true;
          if (log_callback) {
            free_callback();
          }
        } else {
          printf("%s\n", log_callback->msg);
          free_callback();
        }
      }
    }
  }
  p.pid = pid;
  strcpy(p.nome, nome_validado);
  p.prioridade = prioridade;
  strcpy(p.status, "Aberto");

  if (inserir_fim(p) == 0) {
    char *retorno =
        message_handler(LOG_INFO, "Processo cadastrado com sucesso!\n",
                        __func__, __FILE__, __LINE__);
    printf("%s", retorno);
    free(retorno);
    pid++;
  } else {
    printf("%s\n", log_callback->msg);
    free_callback();
  }
}

// busca recursiva
static void busca_rec_processo(Node *inicio) {
  if (inicio == NULL) {
    return;
  }
  printar_chamado(inicio);
  if (inicio->proximo != NULL) {
    return busca_rec_processo(inicio->proximo);
  }
}

static void busca_rec_processo_filtro(Node *inicio, char *status,
                                      int contador) {
  if (inicio == NULL) {
    if (contador == 0) {
      char *retorno =
          message_handler(LOG_INFO, "Nao existem chamados com este status\n",
                          __func__, __FILE__, __LINE__);
      printf("%s", retorno);
      free(retorno);
    }
    return;
  }
  if (strcmp(inicio->chamado.status, status) == 0) {
    printar_chamado(inicio);
    contador++;
  }
  if (inicio->proximo != NULL) {
    return busca_rec_processo_filtro(inicio->proximo, status, contador);
  }
  if (contador == 0) {
    char *retorno =
        message_handler(LOG_INFO, "Nao existem chamados com este status\n",
                        __func__, __FILE__, __LINE__);
    printf("%s", retorno);
    free(retorno);
  }
}

static void exibir_tudo() {
  if (fila_processos != NULL) {
    printf(".------------------------------------------------------------\n");
    printf("| :: CHAMADOS NAO FINALIZADOS ::                             \n");
    printf("+------------------------------------------------------------\n");
    busca_rec_processo(fila_processos);
  } else {
    char *retorno =
        message_handler(LOG_INFO, "Nao existem chamados nao finalizados\n",
                        __func__, __FILE__, __LINE__);
    printf("%s", retorno);
    free(retorno);
  }
  printf(".------------------------------------------------------------\n");
  printf("                                                             \n");
  printf("+------------------------------------------------------------\n");
  if (pilha_processos != NULL) {
    printf(".------------------------------------------------------------\n");
    printf("| :: CHAMADOS FINALIZADOS ::                                 \n");
    printf("+------------------------------------------------------------\n");
    busca_rec_processo(pilha_processos);
  } else {
    char *retorno =
        message_handler(LOG_INFO, "Nao existem chamados finalizados\n",
                        __func__, __FILE__, __LINE__);
    printf("%s", retorno);
    free(retorno);
  }
}

void listar_processo() {
  limparTela();
  char escolha[3];
  bool sair = false;
  while (!sair) {
    menu_busca();
    if (fgets(escolha, sizeof(escolha), stdin) != NULL) {
      long convertido = strtol(escolha, NULL, 10);

      switch (convertido) {
      case 1:
        limparTela();
        busca_rec_processo_filtro(fila_processos, "Aberto", 0);
        break;

      case 2:
        limparTela();
        busca_rec_processo_filtro(fila_processos, "Em andamento", 0);
        break;

      case 3:
        limparTela();
        busca_rec_processo_filtro(pilha_processos, "Finalizado", 0);
        break;

      case 4:
        limparTela();
        exibir_tudo();
        break;
      case 5:
        limparTela();
        sair = true;
        break;
      default:
        char *retorno = message_handler(LOG_WARN, "Opcao invalida\n", __func__,
                                        __FILE__, __LINE__);
        printf("%s", retorno);
        free(retorno);
        break;
      }
    }
  }
}

// o chamado de maior prioridade e mais antigo muda para "Em atendimento"
// s� � permitido realizar um atendimento por vez
void executa_atendimento() {
  limparTela();

  // verifica se existem chamados em aberto
  if (fila_processos == NULL) {
    char *retorno =
        message_handler(LOG_INFO, "Nao existem chamados nao finalizados\n",
                        __func__, __FILE__, __LINE__);
    printf("%s", retorno);
    free(retorno);
    return;
  }

  // verifica se ja nao foi iniciado um chamado
  else if (em_atendimento != NULL) {
    char *retorno = message_handler(
        LOG_INFO,
        "Ja existe um atendimento inicializado que n�o foi finalizado\n",
        __func__, __FILE__, __LINE__);
    printf("%s", retorno);
    free(retorno);
  }

  // inicia um chamado e coloca como em atendimento
  else {
    inicializa_atendimento();
  }

  printar_chamado(em_atendimento);

  char escolha[3];
  bool sair = false;
  while (!sair) {
    menu_atendimento();
    if (fgets(escolha, sizeof(escolha), stdin) != NULL) {
      long convertido = strtol(escolha, NULL, 10);

      switch (convertido) {
      case 1:
        limparTela();
        finaliza_atendimento();
        sair = true;
        break;
      case 2:
        limparTela();
        sair = true;
        break;
      default:
        char *retorno = message_handler(LOG_WARN, "Opcao invalida\n", __func__,
                                        __FILE__, __LINE__);
        printf("%s", retorno);
        free(retorno);
        break;
      }
    }
  }
}

// busca recursiva
static int busca_rec_processo_id(Node *inicio, int id) {
  if (inicio == NULL) {
    return 1;
  }
  if (inicio->chamado.pid == id) {
    printar_chamado(inicio);
    return 0;
  } else if (inicio->proximo != NULL) {
    return busca_rec_processo_id(inicio->proximo, id);
  } else {
    return 1;
  }
}

// busca de processo por id
void buscar_processo() {
  limparTela();
  if (fila_processos == NULL && pilha_processos == NULL) {
    char *retorno =
        message_handler(LOG_INFO, "Nao existem processos para serem buscados\n",
                        __func__, __FILE__, __LINE__);
    printf("%s", retorno);
    free(retorno);
    return;
  }
  char entrada_pid[20];
  int id = -1;

  // inicializa um loop para validar os dados de entrada
  bool saida = false;
  while (saida == false) {
    if (id < 0) {
      printf("%s", "Informe o pid do processo: ");
      if (fgets(entrada_pid, sizeof(entrada_pid), stdin) != NULL) {
        // valida se o campo � nulo, avalia o tamanho que est� limitado a 50
        // char e remove espacos a frente e no fim
        id = valida_inteiro(entrada_pid);

        if (id != -1) {
          saida = true;
        } else {
          printf("%s\n", log_callback->msg);
          free_callback();
        }
      }
    }
  }
  int retorno = busca_rec_processo_id(pilha_processos, id);
  if (retorno == 1) {
    retorno = busca_rec_processo_id(fila_processos, id);
  }
  if (retorno == 1) {
    char *retorno =
        message_handler(LOG_INFO, "Nao existem chamados com o id informado\n",
                        __func__, __FILE__, __LINE__);
    printf("%s", retorno);
    free(retorno);
  }
}

// funcao para calcular estatisticas
void estatisticas() {
  limparTela();
  if (fila_processos == NULL && pilha_processos == NULL) {
    char *retorno =
        message_handler(LOG_INFO, "Nao existem dados para serem calculados\n",
                        __func__, __FILE__, __LINE__);
    printf("%s", retorno);
    free(retorno);
    return;
  }
  int quantidade_processos_abertos = 0;
  int quantidade_processos_finalizados = 0;

  if (pilha_processos != NULL) {
    Node *temp = pilha_processos;
    while (temp != NULL) {
      quantidade_processos_finalizados += 1;
      temp = temp->proximo;
    }
  }
  if (fila_processos != NULL) {
    Node *temp = fila_processos;
    while (temp != NULL) {
      quantidade_processos_abertos += 1;
      temp = temp->proximo;
    }
  }

  double porcentagem_finalizado =
      (double)quantidade_processos_finalizados /
      (quantidade_processos_finalizados + quantidade_processos_abertos) * 100.0;

  printf("Estatisticas:\n");
  printf("Total de chamados abertos e em atendimento | %d\n",
         quantidade_processos_abertos);
  printf("Total de chamados finalizados              | %d\n",
         quantidade_processos_finalizados);
  printf("Chamado mais antigo aberto:                    \n");
  if (fila_processos != NULL) {
    printar_chamado(fila_processos);
  } else {
    printf("Nao existem chamados em aberto\n");
  }
  printf("Percentual de finalizados                  | %.2f%% \n",
         porcentagem_finalizado);
}

// funcao para limpar memoria antes de sair
void limpa_memoria() {
  Node *atual = fila_processos;
  Node *proximo_no;

  while (atual != NULL) {
    proximo_no = atual->proximo;
    free(atual);
    atual = proximo_no;
  }
  fila_processos = NULL;

  atual = pilha_processos;

  while (atual != NULL) {
    proximo_no = atual->proximo;
    free(atual);
    atual = proximo_no;
  }
  pilha_processos = NULL;
  if (log_callback != NULL) {
    free_callback();
  }

  printf("Memoria liberada com sucesso.\n");
}
