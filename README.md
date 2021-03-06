# FMC_SINCRONIA
Projeto de jogo em C utilizando ALLEGRO 5
Brendon Mike Feliciano de Oliveira

Informações das pastas - 
ALLEGRO - Codigos relacionados a API grafica utilizada
RECURSOS - Codigos relacionados ao controle e manipulação de recursos graficos ( Imagens, sons, ... )
LOGICA - Codigos relacionados a logica em si do jogo, como as regras, o jogador, e a inteligência artificial
FMC - Codigos relacionados ao inicio e loop do jogo

Este trabalho é focado em um jogo de dominação de territorio apartir de um campo, onde cada territorio é um nó em
um grafo, representado graficamente como um hexagono onde cada um de seus lados representa uma conexão com o hexagono
conectado. A logica de movimentação do jogador utliza uma busca em largura para verificar se o territorio selecionado
pelo jogador é valido ou não. As funções do grafo são altamente utilizadas quando a Inteligência Artificial é executado. 

A Inteligência Artificial do jogo faz um processamento de todas as possibilidades de ação da "maquina" em duas etapas,
cada uma dessas etapas gera um grafo, um para a possibilidade de ataque e outro para a possibilidade de conquista,
esses grafos são gerados apartir do grafo do campo, é analisado se o territorio em questão primeiramente pode ser 
acessado pelo jogador controlado pela maquina, uma busca em largura é aplicada nesta etapa, uma vez que é detectado a
possibilidade de acesso da maquina aquele nó, é calculado o que no jogo é chamado de Sincronia, onde um elemento que 
está vinculado ao nó possue vantagens e desvantagens em relação a outros, ou seja, nessa etapa é acessado todos os
nós conectados ao nó origem para se fazer o calculo. Após os dois grafos de possibilidades serem gerados, a maquina
escolhe atraves de calculos pseudo-aleatorios a ação que deseja executar, sendo que apartir do nó de origem no
grafo de possibilidades a maquina tem 50 porcento de chance de escolher a melhor jogada, 35 porcento de escolher a
jogada intermediaria, e 25 de executar uma jogada pior.

Toda a interface grafica da aplicação foi escrita em linguagem C utilizando a bliblioteca grafica ALLEGRO 5, toda a
logica de manipulação de recursos graficos, como animação e alocação dos mesmos na memoria, foram de minha autoria,

Todo o projeto pode ser totalmente portado para o sistema operacional LINUX.

Para rodar o jogo em resoluções de telas diferentes, por favor iniciar o projeto pelo launcher, ele automaticamente
detectara todas as resoluções de tela suportado por sua maquina.
