#include "log.h"
#include <assert.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#ifdef _WIN32
#include <direct.h>
#include <windows.h>
#else
#include <sys/stat.h>
#include <sys/types.h>
#endif

Retorno *log_callback = NULL;

// Fun�o que captura a hora atual e devolve na struct DataHora
static DataHora captura_hora() {
  time_t horario_atual;
  struct tm *info;

  time(&horario_atual);             // pega o tempo atual
  info = localtime(&horario_atual); // converte para data/hora local

  DataHora dh;

  // Preenche a data
  dh.data.dia = info->tm_mday;
  dh.data.mes = info->tm_mon + 1;     // m�s come�a em 0
  dh.data.ano = info->tm_year + 1900; // anos desde 1900

  // Preenche a hora
  dh.hora.hora = info->tm_hour;
  dh.hora.minuto = info->tm_min;
  dh.hora.segundo = info->tm_sec;

  return dh;
}

// pasta onde os logs sao armazenados
static int criar_pasta() {
#ifdef _WIN32
  if (_mkdir("logs") == 0) {
    return 0;
  } else if (errno == EEXIST) {
    return 0;
  } else {
    fprintf(stderr, AMARELO "[WARN] Nao foi possivel criar pasta de log\n" FIM);
    return 1;
  }
#else
  if (mkdir("logs", 0777) == 0) {
    return 0;
  } else if (errno == EEXIST) {
    return 0;
  } else {
    fprintf(stderr, AMARELO "[WARN] Nao foi possivel criar pasta de log\n" FIM);
    return 1;
  }
#endif
  return 0;
}

// funcao para escrever os logs
static int registra_log(char *log) {
  assert(log != NULL);
  DataHora agora = captura_hora();
  char data[20];
  char hora[20];

  sprintf(data, "%02d-%02d-%04d", agora.data.dia, agora.data.mes,
          agora.data.ano);

  sprintf(hora, "%02d-%02d-%02d", agora.hora.hora, agora.hora.minuto,
          agora.hora.segundo);

  int verifica_pasta = criar_pasta();
  if (verifica_pasta == 1)
    return 1;

  FILE *arquivo;

  char nomepasta[100];
  sprintf(nomepasta, "logs/log [%s %s].txt", data, hora);

  arquivo = fopen(nomepasta, "w");

  if (arquivo == NULL) {
    fprintf(stderr,
            AMARELO "[WARN] Nao foi possivel criar arquivo de log\n" FIM);
    return 1;
  }

  fprintf(arquivo, "%s\n", log);
  fclose(arquivo);
  return 0;
}

// remove o caminho do nome do arquivo
const char *basename(const char *path) {
  assert(path != NULL);
  const char *slash1 = strrchr(path, '/');
  const char *slash2 = strrchr(path, '\\'); // Windows
  const char *slash = slash1 > slash2 ? slash1 : slash2;
  return slash ? slash + 1 : path;
}

// monta a mensagem de retorno e salva na pasta de log usando um txt
// quem chama a funcao tem a responsabilidade de dar free
char *message_handler(LogLevel level, char *mensagem, const char *funcao,
                      const char *file, int linha) {
  assert(mensagem != NULL);
  assert(funcao != NULL);
  assert(file != NULL);

  DataHora agora = captura_hora();
  char data[20];
  char hora[20];

  const char *filename = basename(file);

  sprintf(data, "%02d/%02d/%04d", agora.data.dia, agora.data.mes,
          agora.data.ano);

  sprintf(hora, "%02d:%02d:%02d", agora.hora.hora, agora.hora.minuto,
          agora.hora.segundo);

  int len_linha = snprintf(NULL, 0, "%d", linha);

  size_t alloc_size = 128 + strlen(data) + strlen(hora) + strlen(mensagem) +
                      strlen(funcao) + strlen(filename) + (size_t)len_linha + 1;

  char *retorno = malloc(alloc_size);

  if (!retorno) {
    fprintf(stderr, AMARELO "[ERRO] Erro ao alocar mem�ria para log" FIM);
    return NULL;
  }

  switch (level) {
  case LOG_INFO:
    sprintf(retorno, VERDE "[INFO] [%s %s] [%s:%d %s] - %s" FIM, data, hora,
            filename, linha, funcao, mensagem);
    break;
  case LOG_WARN:
    sprintf(retorno, AMARELO "[WARN] [%s %s] [%s:%d %s] - %s" FIM, data, hora,
            filename, linha, funcao, mensagem);
    break;
  case LOG_ERROR:
    sprintf(retorno, VERMELHO "[ERROR] [%s %s] [%s:%d %s] - %s" FIM, data, hora,
            filename, linha, funcao, mensagem);
    break;
  default:
    sprintf(retorno, "[WARN] [%s %s] Nivel de log invalido. [INFO] %s", data,
            hora, mensagem);
  }

  registra_log(retorno);
  return retorno;
}

void criar_callback(char *mensagem, LogLevel level) {
  assert(mensagem != NULL);

  if (log_callback != NULL) {
    free(log_callback);
  }

  log_callback = malloc(sizeof *log_callback);
  if (!log_callback) {
    char *retorno =
        message_handler(LOG_INFO, "Nao existem dados para serem calculados\n",
                        __func__, __FILE__, __LINE__);
    printf("%s", retorno);
    free(retorno);
  }
  strcpy(log_callback->msg, mensagem);
  log_callback->level = level;
}

void free_callback(void) {
  free(log_callback);
  log_callback = NULL;
}
