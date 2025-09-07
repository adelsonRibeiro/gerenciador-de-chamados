#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <ctype.h>
#include <stdbool.h>
#include <assert.h>
#include "utils.h"
#include "log.h"


#define MAX_LEN 49

// remove espacos
static char *trim(char *s) {
    assert(s != NULL);
    char *end;
    while (*s == ' ') s++;
    if (*s == '\0') return s;
    end = s + strlen(s) - 1;
    while (end > s && *end == ' ') end--;
    *(end + 1) = '\0';
    return s;
}

// valida e retorna o nome sem espacos
char *valida_nome(char *buf) {
    size_t len;

    if (buf == NULL) {
        char *retorno = message_handler(LOG_ERROR, "Buffer invalido", __func__, __FILE__, __LINE__);
        criar_callback(retorno, LOG_WARN);
        free(retorno);
        return NULL;
    }

    len = strlen(buf);

    if (len == MAX_LEN && buf[len-1] != '\n') {
        char *retorno = message_handler(LOG_WARN, "O texto deve ter ate 49 caracteres\n", __func__, __FILE__, __LINE__);
        criar_callback(retorno, LOG_WARN);
        free(retorno);
        return NULL;
    }

    buf[strcspn(buf, "\n")] = '\0';

    char *p = trim(buf);

    if (p[0] == '\0') {
        char *retorno = message_handler(LOG_WARN, "A descricao nao pode ficar vazia\n", __func__, __FILE__, __LINE__);
        criar_callback(retorno, LOG_WARN);
        free(retorno);
        return NULL;
    }

    return p;
}

// verifica se a prioridade é valida
int valida_prioridade(int valor){
    if(valor < 1 || valor > 5){
        char *retorno = message_handler(LOG_WARN, "Prioridade invalida! Use um valor entre 1 e 5\n", __func__, __FILE__, __LINE__);
        criar_callback(retorno, LOG_WARN);
        free(retorno);
        return -1;
    }
    return 0;
}

// validacao para verificar se tem algum tipo char na entrada
static bool todos_digitos_int(const char *str) {
    if (str == NULL || *str == '\0')
        return false;
    for (const char *p = str; *p; p++) {
        if (!isdigit((unsigned char)*p))
            return false;
    }
    return true;
}

// validacao de inteiro: remove espacos, verifica se tem algum tipo char ou se a entrada é nula
int valida_inteiro(char valor[20]){
    valor[strcspn(valor, "\n")] = '\0';
    valor = trim(valor);

    if(valor[0] == '\0') {
        char *retorno = message_handler(LOG_WARN, "Campo vazio, informe apenas numeros inteiros\n", __func__, __FILE__, __LINE__);
        criar_callback(retorno, LOG_WARN);
        free(retorno);
        return -1;
    }
    else{
        if(todos_digitos_int(valor)){
            int convertido = atoi(valor);
            if(convertido > 0){
                return convertido;
            }
            else{
                char *retorno = message_handler(LOG_WARN, "Campo nulo ou caractere invalido, Informe apenas numeros inteiros positivos\n", __func__, __FILE__, __LINE__);
                criar_callback(retorno, LOG_WARN);
                free(retorno);
                return -1;
            }
        }
        else{
            char *retorno = message_handler(LOG_WARN, "Campo nulo ou caractere invalido, Informe apenas numeros inteiros positivos\n", __func__, __FILE__, __LINE__);
            criar_callback(retorno, LOG_WARN);
            free(retorno);
            return -1;
        }
    }
}

// funcao basica para limpar a tela do terminal
void limparTela(){
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}
