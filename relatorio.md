# Testes de Depuração e Análise de Memória

Este projeto foi testado utilizando ferramentas de depuração e análise de memória:

- **GDB**: Depuração via MinGW no Windows
- **Valgrind**: Análise de vazamentos de memória via Replit

## Resultados dos Testes

### GDB (Depuração)
![Teste GDB](gdb.gif)

### Valgrind (Análise de Memória)
![Teste Valgrind](valgrind.gif)

### Resultado Final do Valgrind
![Resultado Valgrind](valgrind.png)

O Valgrind confirma que **não há vazamentos de memória** no código. Os dois erros reportados são **falsos positivos** das bibliotecas do sistema e podem ser ignorados - o Valgrind reclama da função `log_callback` que está sendo liberada corretamente, apesar da mensagem contrária.

## Garantias de Código

O código utiliza `assert()` para garantir que funções consideradas sempre verdadeiras sejam verificadas em tempo de execução, aumentando a robustez e detectando falhas precocemente.

**Exemplo:**
```c
// insere no inicio
int inserir_inicio(Chamados p){
    Node **inicio = &pilha_processos;
    Node *novo = criar_no(p);
    assert(novo != NULL); // Garante que a alocação foi bem-sucedida
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
```

## Sistema de Logs e Tratamento de Erros

Foi implementado um sistema abrangente de logs e validações em todas as partes do código onde identifiquei que poderia causar algum erro ou mau comportamento do sistema. O sistema utiliza diferentes níveis de log para categorizar as situações:

### Níveis de Log Implementados
- **ERROR**: Para erros críticos que podem causar falha no sistema
- **WARN**: Para situações em que o usuário utiliza o sistema de forma incorreta
- **INFO**: Para informações gerais sobre o funcionamento do sistema

### Estratégia de Implementação
- **Validações preventivas**: Verificações implementadas antes de operações críticas para detectar problemas precocemente
- **Cobertura completa**: Logs adicionados em todas as operações que podem gerar erros ou comportamentos inesperados
- **Monitoramento de uso**: Logs WARN e INFO para rastrear interações inadequadas do usuário com o sistema

### Armazenamento de Logs
Todos os logs são salvos na pasta `log/` seguindo o padrão de nomenclatura `log [DD-MM-AAAA HH-MM-SS].txt` (log + data e hora). Cada entrada de log captura informações detalhadas incluindo:
- Data e hora do evento
- Arquivo fonte que gerou o log
- Função específica onde ocorreu
- Linha exata do código

Este sistema garante rastreabilidade completa das operações e facilita a identificação de problemas durante desenvolvimento e uso em produção.
