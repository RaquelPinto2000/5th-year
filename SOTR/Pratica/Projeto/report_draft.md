# Serial Comunication
## Image Encoding
A imagem e um array de pixeis de tamanho 1 Byte.
Sao usados os seguintes valores:

- 0x00, para representar o background;
- 0x80 para representar objetos;
- 0xFF para representar a guideline.

Portanto são usados 3 dos possiveis 256 valores para um pixel de forma a representar o mundo que rodeia o robot.

De forma a conseguir informar o robot de que se vai tranferir uma imagem e que a tranferencia da imagem ja foi finalizada, decidimos introduzir mais 2 palavras:

- 0x40, para sinalizar o começo do envio de uma imagem;
- 0xC0, para sinalizar o fim do envio de uma imagem.

A escolha destas duas palavras foi feita de forma a que as palavras transmitidas com significado continuassem equidistantes.

# WCET
A medição do WCET das varias tarefas é facilitada pelo facto de o nRF52840 ser um SoC unicore.

O scheduling do Zephyr OS é baseado em prioridades e em 2 tipos de tarefas:^[https://docs.zephyrproject.org/3.0.0/reference/kernel/threads/index.html#thread-priorities]

- Tarefas cooperativas, prioridade < 0: 
assumem controlo do CPU durante a sua execução sendo responsaveis por libertar o CPU. São apenas suplantadas por ISRs

- Tarefas Preemtable prioridade >= 0:
Podem ser suplantadas por tarefas cooperativas ou tarefas preemtable com maior prioridade.


## Metodologia

1. A medição do WCET foio feito tarefa a tarefa.

1. São usados dados (imagens), representativas das imagens que se espera que o robot venha a processar, mas geradas localmente de forma a poder desligar rotinas de interupção durante a execução da tarefa.

1. A prioridade da tarefa em analise é definida com o valor -15, menor valor possivel, de forma a tornar a tarefa em uma tarefa cooperativa de maior prioridade.

1. São usadas timestamps (diferença).

1. São recolhidos, no minimo, N valores por tarefa.

1. Para cada tarefa é atribuido como WCET a soma do pior valor obtido com o desvio padrao (? TBD).

## Resultados

! Apresentar tabela com WCET !



