# TP Sistemas Operacionais XV6

## Authors:

Marcos Paulo Ferreira de Souza: https://github.com/MP022

João Vítor Santana Depollo: https://github.com/@JoaoVitorSD

### 1. Introdução

O objetivo deste trabalho é implementar alterações na política de escalonamento do sistema operacional xv6. A maior parte das alterações foram feitas pelas chamadas do scheduler e na inicialização do proc, na função `allocproc()`.

### Scheduller

A prempção do scheduler ocorre com a implementação de um loop que por uma variável `prempt` que quando atinge o valor da variável `INTERV`, o valor da variável é zerado e a função `premptProcess()` é chamada, aplicando diferentes políticas de escalonamento, de acordo com a prioridade, para escolher o próximo processo a ser executado.
## Prioridades
As prioridades foram divididas em 4 níveis, sendo eles:
- Low
- Medium
- High
- Realtime

A implementação foi feita via enum, no arquivo `proc.h`:
```c
enum procpriority
{
  LOW,
  MEDIUM,
  HIGH,
  REALTIME
};	
```
## Escalonamento

Cada prioridade possui uma política de escalonamento diferente e a cada tempo X de ticks, o processo é promovido para a próxima prioridade, até chegar na prioridade `REALTIME`. Esta política gera o aging dos processos e evita que um processo fique em starvation. Neste contexto, foram implementadas as seguintes políticas de escalonamento:
### Realtime
Round Robin, a ideia é que o processo que chegou primeiro, seja o primeiro a ser executado, e que os processos sejam executados por um tempo fixo, e depois sejam colocados no final da fila. Para esta implementação, é utilizado o valor `last_cycle` do processo, que é incrementado a cada ciclo em que o processo é escolhido para ser executado. Dessa forma, o round_robin vai buscar o processo com o menor `last_cycle` para ser executado.

### High
Baseado no CFS (Completely Fair Scheduler), a ideia é que o processo que tem a menor quantidade de tempo de CPU, seja o próximo a ser executado. Para isso, é utilizado o valor `rutime` do processo, que é incrementado a cada ciclo em que o processo está como RUNNING ou foi coloado para executar. Dessa forma, o CFS vai buscar o processo com o menor `rutime` para ser executado. É uma política que não gera starvation e dará maior prioridade para os processos que tem menor tempo de CPU, (io-bound).

### Medium

Similar ao escalonador Round Robin, mas considera a quantidade de vezes que o processo foi executado, para que o processo não fique em starvation. A ideia é que o processo que foi executado menos vezes, seja o próximo a ser executado. Dessa forma, se um processo p2 chegar após o processo p1, mas o processo p1 já foi executado 3 vezes, e o processo p2 ainda não foi executado, o processo p2 será executado antes do processo p1 por 3 vezes. Dessa forma, ele recebe um tratamento justo, e não fica em starvation. 

### Low

Utiliza o FCFS (First Come, First Served), utilizando o valor `ctime` do processo para saber qual o processo com o menor `ctime` do array de processo. A ideia é que o processo que chegou primeiro, seja o primeiro a ser executado, o starvation ainda é um problema, se o processo que chegou primeiro, não terminar, os outros processos não serão executados. Mas é uma solução simples e que vai evitar que os processos cheguem e subam para a fila de prioridade alta, sem serem executados.