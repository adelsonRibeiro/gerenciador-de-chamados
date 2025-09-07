# Sistema de Gerenciamento de Chamados

> Projeto em C que implementa um sistema completo de helpdesk usando estruturas dinâmicas (fila e pilha), tratamento de erros robusto e sistema de logs detalhado.

## Descrição

Este sistema simula o fluxo de chamados de suporte técnico utilizando duas estruturas de dados principais: **fila para chamados em aberto/andamento** (FIFO) e **pilha para chamados finalizados** (LIFO). O sistema implementa priorização automática, busca recursiva, tratamento de erros com logs em arquivo e gerenciamento completo de memória.

## Funcionalidades

- **Cadastrar Chamado**: Adiciona novos chamados na fila com validação completa
- **Listar Chamados**: Exibe chamados com filtros por status (aberto/em andamento/finalizado/todos)
- **Executar Atendimento**: Prioriza chamados por menor valor de prioridade e mais antigo
- **Buscar Chamado**: Busca recursiva por PID em ambas as estruturas
- **Estatísticas**: Relatórios completos de produtividade e status
- **Sistema de Logs**: Registro detalhado de todas as operações em arquivos
- **Gerenciamento de Memória**: Liberação automática com `atexit`

## Estruturas Implementadas

### Estruturas de Dados
```c
typedef struct {
    int pid;              // ID único auto-incrementado
    char nome[50];        // Título do chamado
    int prioridade;       // Prioridade de 1-5 (1 = mais urgente)
    char status[25];      // "Aberto", "Em andamento", "Finalizado"
} Chamados;

typedef struct Node {
    Chamados chamado;
    struct Node *anterior;
    struct Node *proximo;
} Node;
```

### Algoritmos Aplicados
- **Priorização**: Chamados com menor valor de prioridade executam primeiro
- **Recursividade**: Busca de chamados por ID e traversal de estruturas
- **Alocação Dinâmica**: malloc/free com validação rigorosa de ponteiros
- **FIFO/LIFO**: Fila para ativos, pilha para finalizados

## Estrutura de Arquivos

```
gerenciador-de-chamados/
├── src/
│   ├── main.c          # Função principal e menu interativo
│   ├── gerenciador.c   # Lógica principal do sistema
│   ├── fila.c         # Operações da fila (chamados ativos)
│   ├── pilha.c        # Operações da pilha (chamados finalizados)
│   ├── utils.c        # Funções de validação e utilitários
│   └── log.c          # Sistema de logs e tratamento de erros
├── include/
│   ├── gerenciador.h   # Definições de structs e protótipos
│   ├── fila.h         # Protótipos da fila
│   ├── pilha.h        # Protótipos da pilha
│   ├── utils.h        # Protótipos dos utilitários
│   └── log.h          # Estruturas e protótipos de log
├── makefile           # Automatização de compilação
└── logs/              # Diretório gerado automaticamente para logs
```

## Compilação e Execução

```bash
# Compilar
make all

# Executar
make run

# Ou executar manualmente
./main

# Limpar arquivos compilados
make clean
```

## Documentação Detalhada das Funções

### Módulo Principal (main.c)

#### `int main()`
Função principal que implementa o menu interativo do sistema. Gerencia o loop principal com switch para navegação entre funcionalidades. Registra `limpa_memoria()` com `atexit()` para garantir liberação automática de recursos.

---

### Módulo Gerenciador (gerenciador.c)

#### Variáveis Globais
- `Node *fila_processos`: Ponteiro para o início da fila de chamados ativos
- `Node *pilha_processos`: Ponteiro para o topo da pilha de chamados finalizados  
- `Node *em_atendimento`: Ponteiro para o chamado atualmente sendo atendido
- `static int pid`: Contador auto-incrementado para IDs únicos

#### `void entrada_dados()`
Coleta e valida dados para novos chamados. Implementa loops de validação robustos para título (máx. 49 chars, não vazio, trim de espaços) e prioridade (1-5). Utiliza `valida_nome()` e `valida_inteiro()` para garantir integridade dos dados. Insere o chamado na fila com status "Aberto" e incrementa o PID global.

#### `void listar_processo()`
Interface para visualização de chamados com sistema de filtros. Apresenta submenu permitindo listar por status específico ou todos os chamados. Utiliza `busca_rec_processo_filtro()` para filtrar por status e `exibir_tudo()` para visualização completa das duas estruturas.

#### `void executa_atendimento()`
Gerencia o ciclo completo de atendimento de chamados. Verifica pré-condições (existência de chamados, ausência de atendimento em curso), inicia atendimento com `inicializa_atendimento()`, exibe chamado atual e oferece opção de finalização via `finaliza_atendimento()`.

#### `void buscar_processo()`
Interface de busca por PID com validação de entrada. Verifica existência de dados nas estruturas, valida PID informado e executa busca recursiva primeiro na pilha, depois na fila. Informa resultado da busca ou ausência de dados.

#### `void estatisticas()`
Calcula e exibe métricas do sistema. Conta chamados por status, calcula percentual de finalização, identifica chamado mais antigo em aberto. Apresenta relatório formatado com estatísticas de produtividade do sistema.

#### `void limpa_memoria()`
Liberação completa de recursos alocados. Percorre ambas as estruturas (fila e pilha) liberando todos os nós com `free()`. Libera também callbacks de log pendentes. Registrada com `atexit()` para execução automática no encerramento.

#### Funções Auxiliares Estáticas

#### `static void menu_busca()`
Exibe submenu para filtros de listagem de chamados.

#### `static void menu_atendimento()`
Exibe opções durante atendimento ativo (finalizar ou voltar ao menu).

#### `static void printar_chamado(Node *inicio)`
Formata e exibe informações completas de um chamado específico com layout padronizado.

#### `static void busca_rec_processo(Node *inicio)`
Função recursiva que percorre estrutura exibindo todos os chamados. Utiliza recursão de cauda para percorrer lista duplamente encadeada.

#### `static void busca_rec_processo_filtro(Node *inicio, char *status, int contador)`
Versão da busca recursiva com filtro por status. O parâmetro `contador` acumula ocorrências encontradas através das chamadas recursivas, permitindo informar ao usuário se nenhum chamado corresponde ao filtro especificado.

#### `static void exibir_tudo()`
Apresenta visualização completa do sistema exibindo separadamente chamados não finalizados (fila) e finalizados (pilha) com formatação estruturada.

#### `static int busca_rec_processo_id(Node *inicio, int id)`
Busca recursiva por ID específico. Retorna 0 quando encontra e exibe o chamado, 1 quando não encontra. Percorre recursivamente a estrutura até encontrar ou esgotar possibilidades.

---

### Módulo Fila (fila.c)

#### `static Node *criar_no(Chamados p)`
Aloca dinamicamente novo nó para lista duplamente encadeada. Inicializa ponteiros anterior/próximo como NULL. Trata falhas de alocação com logs de erro. Retorna ponteiro para nó criado ou NULL em caso de falha.

#### `int inserir_fim(Chamados p)`
Implementa inserção FIFO na fila de chamados. Trata caso especial de fila vazia, caso contrário percorre até o final e insere. Mantém integridade dos ponteiros da lista duplamente encadeada. Retorna 0 em sucesso.

#### `void remover_processo(Node **inicio, int id)`
Remove nó específico por ID da estrutura. Trata casos especiais: primeiro nó, último nó, nó intermediário. Mantém integridade da lista ajustando ponteiros anterior/próximo dos nós adjacentes. Libera memória do nó removido.

#### `void inicializa_atendimento()`
Identifica chamado de maior prioridade (menor valor numérico) para atendimento. Em caso de empate, mantém ordem FIFO (mais antigo primeiro). Altera status para "Em andamento" e define ponteiro global `em_atendimento`.

#### `void finaliza_atendimento()`
Completa ciclo de atendimento alterando status para "Finalizado", transferindo chamado da fila para pilha via `inserir_inicio()`, removendo da fila original e resetando ponteiro de atendimento. Exibe callbacks de log se disponíveis.

---

### Módulo Pilha (pilha.c)

#### `static Node *criar_no(Chamados p)`
Função idêntica à da fila - aloca e inicializa novo nó. Mantém consistência na interface entre módulos de estruturas de dados.

#### `int inserir_inicio(Chamados p)`
Implementa inserção LIFO no topo da pilha. Trata caso de pilha vazia, caso contrário insere no início ajustando ponteiros. Mantém integridade da lista duplamente encadeada. Usado para chamados finalizados.

---

### Módulo Utilitários (utils.c)

#### `char *valida_nome(char *buf)`
Validação completa de strings de entrada. Verifica buffer nulo, trata overflow de entrada (máx. 49 chars), remove quebras de linha, aplica `trim()` para espaços, valida campo não vazio. Retorna ponteiro para string válida ou NULL em caso de erro com logs apropriados.

#### `int valida_inteiro(char valor[20])`
Validação robusta de entrada numérica. Remove quebras de linha e espaços, verifica se contém apenas dígitos via `todos_digitos_int()`, converte para inteiro e valida positividade. Retorna valor válido ou -1 em caso de erro.

#### `int valida_prioridade(int valor)`
Valida range de prioridade (1-5). Função específica para regras de negócio do sistema de chamados. Retorna 0 se válido, -1 se inválido, com logs de erro apropriados.

#### `void limparTela()`
Função multiplataforma para limpeza de terminal. Utiliza comando sistema apropriado (cls/clear) baseado em diretivas de compilação.

#### Funções Auxiliares Estáticas

#### `static char *trim(char *s)`
Remove espaços em branco no início e fim de strings. Implementa algoritmo eficiente com dois ponteiros, modificando string in-place. Essencial para tratamento de entradas do usuário.

#### `static bool todos_digitos_int(const char *str)`
Verifica se string contém exclusivamente dígitos numéricos. Utiliza `isdigit()` para validação robusta de caracteres. Tratamento de casos edge (string nula/vazia).

---

### Módulo Log (log.c)

#### Estruturas de Dados
```c
typedef struct {
    Data data;    // dia, mês, ano
    Hora hora;    // hora, minuto, segundo
} DataHora;

typedef struct {
    char msg[200];
    LogLevel level;
} Retorno;
```

#### Variáveis Globais
- `Retorno *log_callback`: Ponteiro global para sistema de callback de mensagens

#### `char *message_handler(LogLevel level, char *mensagem, const char *funcao, const char *file, int linha)`
Função central do sistema de logs. Formata mensagens com timestamp, informações de debug (arquivo:linha função), aplica cores ANSI por nível de log. Calcula dinamicamente tamanho necessário, aloca memória, registra em arquivo via `registra_log()`. Retorna string formatada (caller deve liberar).

#### `void criar_callback(char *mensagem, LogLevel level)`
Sistema de callback para comunicação entre módulos. Aloca estrutura `Retorno`, copia mensagem e define nível. Permite que módulos baixo nível comuniquem erros para interface sem acoplamento direto.

#### `void free_callback(void)`
Libera memória do sistema de callback e reseta ponteiro global. Essencial para evitar vazamentos de memória no sistema de mensagens.

#### Funções Auxiliares Estáticas

#### `static DataHora captura_hora()`
Captura timestamp atual do sistema usando `time()` e `localtime()`. Preenche estrutura `DataHora` com conversões apropriadas (mês +1, ano +1900). Fornece base temporal para todos os logs.

#### `static int criar_pasta()`
Cria diretório "logs" de forma multiplataforma. Utiliza `_mkdir()` (Windows) ou `mkdir()` (Unix/Linux). Trata erro EEXIST (pasta já existe) como sucesso. Retorna 0 em sucesso, 1 em falha.

#### `static int registra_log(char *log)`
Persiste logs em arquivos individuais com timestamp no nome. Cria arquivo único por operação no formato "log [DD-MM-YYYY HH-MM-SS].txt". Garante criação do diretório via `criar_pasta()`, trata falhas de abertura de arquivo.

#### `const char *basename(const char *path)`
Extrai nome de arquivo de path completo. Identifica última ocorrência de separadores (/ ou \\), retorna ponteiro para nome ou path original se não houver separadores. Multiplataforma para Windows e Unix.

---

## Fluxo de Execução

### Ciclo de Vida dos Chamados
1. **Cadastro**: `entrada_dados()` → validação → `inserir_fim()` → fila (status "Aberto")
2. **Atendimento**: `inicializa_atendimento()` → identifica prioridade → status "Em andamento"  
3. **Finalização**: `finaliza_atendimento()` → status "Finalizado" → `inserir_inicio()` → pilha

### Sistema de Priorização
- Menor valor numérico = maior prioridade (1 > 2 > 3 > 4 > 5)
- Em empates: ordem de chegada (FIFO)
- **Limitação**: Apenas um atendimento simultâneo permitido pelo sistema

### Sistema de Logs
- Arquivo único por operação com timestamp no nome
- Formato: `log [DD-MM-YYYY HH-MM-SS].txt`
- Níveis: INFO (verde), WARN (amarelo), ERROR (vermelho)
- Callback system para comunicação entre módulos

## Tratamento de Erros

- **assert()**: Pontos críticos (ponteiros nulos, pré-condições)
- **Códigos de retorno**: Funções retornam 0 (sucesso) ou -1 (erro)
- **Validação de malloc**: Toda alocação é verificada
- **Logs estruturados**: Registro de erros, avisos e informações
- **Cleanup automático**: `atexit()` registra liberação de memória

## Requisitos

- Compilador C (gcc) com suporte a C11
- make
- Sistema operacional: Linux, macOS ou Windows (MinGW)

## Exemplo de Uso

```bash
# Compilar e executar
make run

# Menu interativo:
# [1] Cadastrar chamado     → entrada_dados()
# [2] Listar chamados      → listar_processo() 
# [3] Executar atendimento → executa_atendimento()
# [4] Buscar chamado       → buscar_processo()
# [5] Estatísticas         → estatisticas()
# [6] Sair                 → limpa_memoria()
```

## Características Técnicas

- **Modularização**: Código separado por responsabilidades em arquivos específicos
- **Memória Dinâmica**: Alocação sob demanda com liberação automática
- **Estruturas Dinâmicas**: Lista duplamente encadeada para fila e pilha
- **Recursão**: Busca e traversal de estruturas
- **Multiplataforma**: Compatível Windows/Unix via diretivas de compilação
- **Logs Persistentes**: Sistema completo de auditoria em arquivos
- **Interface Robusta**: Validação completa de entradas do usuário

## Estatísticas Disponíveis

- Total de chamados por status (aberto/andamento/finalizado)
- Percentual de conclusão de chamados
- Identificação do chamado mais antigo em aberto
- Relatórios formatados de produtividade

## Observações

- **IDs únicos**: Sistema auto-incrementado iniciando em 1
- **Status válidos**: "Aberto", "Em andamento", "Finalizado"  
- **Prioridades**: 1 (máxima) a 5 (mínima)
- **Títulos**: Máximo 49 caracteres, não podem ficar vazios
- **Memória**: Liberação automática no encerramento do programa
- **Logs**: Criados no diretório `logs/` com timestamps únicos