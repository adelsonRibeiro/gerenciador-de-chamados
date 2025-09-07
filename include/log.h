#ifndef LOG_H
#define LOG_H

#define VERMELHO "\x1b[31m"
#define VERDE   "\x1b[32m"
#define AMARELO "\x1b[33m"
#define FIM     "\x1b[0m"


typedef struct {
    int dia;
    int mes;
    int ano;
} Data;

typedef struct {
    int hora;
    int minuto;
    int segundo;
} Hora;

typedef struct {
    Data data;
    Hora hora;
} DataHora;

typedef enum {
    LOG_INFO,
    LOG_WARN,
    LOG_ERROR
} LogLevel;

typedef struct{
    char msg[200];
    LogLevel level;
}Retorno;

extern Retorno *log_callback;

char *message_handler(LogLevel level, char *mensagem, const char *funcao, const char *file, int linha);
void criar_callback(char *mensagem, LogLevel level);
void free_callback(void);

#endif
