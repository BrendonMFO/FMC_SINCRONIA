#pragma once

#include "BM_Allegro_eventos.h"
#include "BM_Player_IA.h"
#include "BM_Campo.h"
#include "BM_Hexagono.h"
#include "BM_Rodadas.h"
#include "BM_FMC_Eventos.h"
# include <stdarg.h>
#include <stdio.h>

//==========================================================================
// Estrutura de ação da IA
//==========================================================================
typedef struct BM_IA_ACAO_S {
	int tempoFrames;
	int tempoAtual;
}BM_IA_ACAO;
//==========================================================================

//==========================================================================
// Variaveis
//==========================================================================
BM_PLAYER *ia = NULL;
BM_IA_ACAO BM_IA_acao;
int BM_IA_executando = FALSE;
//==========================================================================

//==========================================================================
// Defines
//==========================================================================
#define ERRO 0
#define SUCESSO 1
//==========================================================================

//==========================================================================
// Estrutura de dados para a arvore de inteligencia artificial
//==========================================================================
typedef struct BM_IA_S
{
	int valor;
	int hexagono;
	struct BM_IA_S *direita;
	struct BM_IA_S *centro;
	struct BM_IA_S *esquerda;
}BM_IA;
//==========================================================================

//==========================================================================
// Arvore IA
//==========================================================================
typedef struct BM_IA_ARVORE_S
{
	BM_IA *raiz;
}BM_IA_ARVORE;
//==========================================================================

//==========================================================================
// Prototipos
//==========================================================================
void BM_IA_possibilidade_ataque(BM_IA_ARVORE *_arvore);
void BM_IA_possibilidade_conquista(BM_IA_ARVORE *_arvore);
void BM_IA_iniciar_arvore(BM_IA_ARVORE *_arvore);
void BM_IA_conquistar_territorio(int _indexHexagono);
void BM_IA_preparar_ataque(int _indexHexagono);
void BM_IA_executar_ataque(int _alvo, int _atacante);
void BM_IA_aguardar_ataque(void *_parametro);
void BM_IA_aguardar_escolha(void *_parametro);
void BM_IA_arvore_desalocar(BM_IA **_no);
void BM_IA_iniciar_acao(BM_EVENTO_FUNCAO _funcao, int _quantidade, ...);
int BM_IA_arvore_inserir(BM_IA **_no, int _valor, int _hexagono);
int BM_IA_arvore_vazia(BM_IA_ARVORE *_arvore);
int BM_IA_checar_elemento();
BM_IA *BM_IA_executar(BM_IA **_no);
//==========================================================================

//==========================================================================
// Iniciar arvore
//==========================================================================
void BM_IA_iniciar_arvore(BM_IA_ARVORE *_arvore) {
	_arvore->raiz = NULL;
}
//==========================================================================

//==========================================================================
// Inserir folha
//==========================================================================
int BM_IA_arvore_inserir(BM_IA **_no, int _valor, int _hexagono) {
	if ((*_no) == NULL) {
		(*_no) = (BM_IA*)malloc(sizeof(BM_IA));
		(*_no)->esquerda = NULL;
		(*_no)->centro = NULL;
		(*_no)->direita = NULL;
		(*_no)->valor = _valor;
		(*_no)->hexagono = _hexagono;
		return SUCESSO;
	}
	else {
		if (_valor < (*_no)->valor)
			BM_IA_arvore_inserir(&(*_no)->esquerda, _valor, _hexagono);
		else if (_valor >(*_no)->valor)
			BM_IA_arvore_inserir(&(*_no)->direita, _valor, _hexagono);
		else {
			BM_IA_arvore_inserir(&(*_no)->centro, _valor, _hexagono);
		}
	}
	return ERRO;
}
//==========================================================================

//==========================================================================
// Desalocar arvore
//==========================================================================
void BM_IA_arvore_desalocar(BM_IA **_no) {
	if ((*_no) != NULL) {
		if ((*_no)->direita == NULL && (*_no)->esquerda == NULL && (*_no)->centro == NULL)
			free((*_no));
		else {
			if ((*_no)->esquerda != NULL)
				BM_IA_arvore_desalocar(&(*_no)->esquerda);
			if((*_no)->centro != NULL)
				BM_IA_arvore_desalocar(&(*_no)->centro);
			if ((*_no)->direita != NULL)
				BM_IA_arvore_desalocar(&(*_no)->direita);
		}
	}
}
//==========================================================================

//==========================================================================
// Checar se a arvore esta vazia
//==========================================================================
int BM_IA_arvore_vazia(BM_IA_ARVORE *_arvore) {
	return _arvore->raiz == NULL;
}
//==========================================================================

//==========================================================================
// Executar escolha
//==========================================================================
BM_IA *BM_IA_executar(BM_IA **_no) {
	int escolha;
	if ((*_no) != NULL) {
		escolha = (rand() % 100) + 1;
		if ((*_no)->direita != NULL && escolha >= 0 && escolha <= 40)
			return BM_IA_executar(&(*_no)->direita);
		else if((*_no)->centro != NULL && escolha > 40 && escolha >= 70)
			return BM_IA_executar(&(*_no)->centro);
		else if ((*_no)->esquerda != NULL && escolha > 70 && escolha <= 90)
			return BM_IA_executar(&(*_no)->esquerda);
		else
			return (*_no);
	}
	return NULL;
}
//==========================================================================

//==========================================================================
// Iniciar player IA
//==========================================================================
int BM_Player_IA_iniciar(int _hexagonoAtual) {

	//======================================================================
	// Alocar memoria para o player da IA
	//======================================================================
	ia = (BM_PLAYER*)malloc(1 * sizeof(BM_PLAYER));
	if (ia == NULL) {
		fprintf(stderr, "ERRO: Nao foi possivel alocar memoria para um player");
		return ERRO;
	}
	BM_IA_iniciar_valores(_hexagonoAtual);
	//======================================================================

	return SUCESSO;

}
//==========================================================================

//==========================================================================
// Get IA
//==========================================================================
BM_PLAYER *BM_Player_getIAPlayer() {
	return ia;
}
//==========================================================================

//==========================================================================
// Iniciar valores da estrutura de jogadores
//==========================================================================
void BM_IA_iniciar_valores(int _hexagono) {
	ia->hexagonoAtual = _hexagono;
	ia->quantidadeTerritorio = 1;
	ia->elementosDisponivel.luz = TRUE;
	ia->elementosDisponivel.trevas = TRUE;
	ia->elementosDisponivel.fogo = TRUE;
	ia->elementosDisponivel.agua = TRUE;
	ia->elementosDisponivel.terra = TRUE;
	ia->elementosDisponivel.ar = TRUE;
	ia->elementosTempo.luz = 0;
	ia->elementosTempo.trevas = 0;
	ia->elementosTempo.fogo = 0;
	ia->elementosTempo.agua = 0;
	ia->elementosTempo.terra = 0;
	ia->elementosTempo.ar = 0;
	BM_Campo_getCampo()->hexagonos[_hexagono].estado = ADVERSARIO;
}
//==========================================================================

//==========================================================================
// Disparar inteligencia artificial
//==========================================================================
void BM_IA_disparar() {
	int ataque = FALSE, conquista = FALSE, decisao;
	BM_IA_ARVORE arvoreAtaque, arvoreConquista;
	time_t t; srand((unsigned)time(&t));
	BM_IA_executando = TRUE;

	//======================================================================
	// Iniciar arvores
	//======================================================================
	BM_IA_iniciar_arvore(&arvoreAtaque);
	BM_IA_iniciar_arvore(&arvoreConquista);
	//======================================================================

	//======================================================================
	// Montar arvore de decisão
	//======================================================================
	BM_IA_possibilidade_ataque(&arvoreAtaque);
	BM_IA_possibilidade_conquista(&arvoreConquista);
	//======================================================================

	//======================================================================
	// Checar possibilidades de ações
	//======================================================================
	if (!BM_IA_arvore_vazia(&arvoreAtaque)) ataque = TRUE;
	if (!BM_IA_arvore_vazia(&arvoreConquista)) conquista = TRUE;
	//======================================================================

	//======================================================================
	// Definir ação
	//======================================================================
	if (ataque == TRUE && conquista == TRUE) {
		decisao = (rand() % 100) + 1;
		if (decisao >= 0 && decisao <= 55) {
			BM_IA_iniciar_acao(BM_IA_aguardar_ataque, 1, BM_IA_executar(&arvoreAtaque.raiz)->hexagono);
		}
		else
			BM_IA_conquistar_territorio(BM_IA_executar(&arvoreConquista.raiz)->hexagono);
	}
	else {
		if(conquista == TRUE)
			BM_IA_conquistar_territorio(BM_IA_executar(&arvoreConquista.raiz)->hexagono);
		if (ataque == TRUE) {
			BM_IA_iniciar_acao(BM_IA_aguardar_ataque, 1, BM_IA_executar(&arvoreAtaque.raiz)->hexagono);
		}
	}
	//======================================================================

	//======================================================================
	// Desalocar arvores
	//======================================================================
	BM_IA_arvore_desalocar(&arvoreAtaque.raiz);
	BM_IA_arvore_desalocar(&arvoreConquista.raiz);
	//======================================================================

}
//==========================================================================

//==========================================================================
// Checar possiblidade de ataque
//==========================================================================
void BM_IA_possibilidade_ataque(BM_IA_ARVORE *_arvore) {
	int ataqueAtual, i, j;
	BM_HEXAGONO *hexagono = NULL;
	BM_Campo *campo = BM_Campo_getCampo();
	for (j = 0; j < campo->quantidade; j++) {
		hexagono = &campo->hexagonos[j];
		if (hexagono->estado == ADVERSARIO) {
			for (i = 0; i < 6; i++) {
				if (hexagono->conexoes[i] != -1) {
					if (campo->hexagonos[hexagono->conexoes[i]].estado == JOGADOR) {
						ataqueAtual = BM_Hexagono_calcular_sincronia(*hexagono, ADVERSARIO);
						BM_IA_arvore_inserir(&_arvore->raiz, ataqueAtual, hexagono->id);
					}
				}
			}
		}
	}
}
//==========================================================================

//==========================================================================
// Checar possibilidade de conquista
//==========================================================================
void BM_IA_possibilidade_conquista(BM_IA_ARVORE *_arvore) {
	int i, j, k, sincronia;
	BM_HEXAGONO *hexagono, *aux = NULL;
	BM_Campo *campo = BM_Campo_getCampo();
	hexagono = &campo->hexagonos[0];
	for (k = 0; k < campo->quantidade;k++) {
		hexagono = &campo->hexagonos[k];
		if (hexagono->estado == ADVERSARIO) {
			for (i = 0, sincronia = 0; i < 6; i++) {
				if (hexagono->conexoes[i] != -1) {
					for (j = 0; j < 6; j++) {
						if (hexagono->conexoes[j] != -1) {
							aux = &campo->hexagonos[hexagono->conexoes[j]];
							if (aux->estado == NEUTRO)
								BM_IA_arvore_inserir(&_arvore->raiz, sincronia, aux->id);
						}
					}
				}
			}
		}
	}
}
//==========================================================================

//==========================================================================
// Conquistar hexagono
//==========================================================================
void BM_IA_conquistar_territorio(int _indexHexagono) {
	BM_Campo *campo = BM_Campo_getCampo();
	campo->hexagonos[_indexHexagono].estado = ADVERSARIO;
	campo->hexagonos[_indexHexagono].visivel = FALSE;
	campo->hexagonos[_indexHexagono].elemento = BM_IA_checar_elemento();
	ia->hexagonoAtual = campo->hexagonos[_indexHexagono].id;
	ia->quantidadeTerritorio++;
	BM_Rodada_avancar();
	BM_Jogo_verificar_fim();
	BM_IA_executando = FALSE;
}
//==========================================================================

//==========================================================================
// Dispara ataque
//==========================================================================
void BM_IA_preparar_ataque(int _indexHexagono) {
	BM_Campo *campo = BM_Campo_getCampo();
	BM_HEXAGONO *hexagono = &campo->hexagonos[_indexHexagono];
	int i, possibilidade[6], contador = 0;
	ia->hexagonoAtual = hexagono->id;
	for (i = 0; i < 6; i++) {
		if (hexagono->conexoes[i] != -1)
			if (campo->hexagonos[hexagono->conexoes[i]].estado == JOGADOR) {
				possibilidade[contador] = campo->hexagonos[hexagono->conexoes[i]].id;
				contador++;
			}
	}
	i = rand() % contador;
	campo->hexagonos[possibilidade[i]].alvo = HEXAGONO_ALVO;
	BM_IA_iniciar_acao(BM_IA_aguardar_escolha, 2, possibilidade[i], _indexHexagono);
}
//==========================================================================

//==========================================================================
// Executar ataque
//==========================================================================
void BM_IA_executar_ataque(int _alvo, int _atacante) {
	BM_Campo *campo = BM_Campo_getCampo();
	switch (BM_Hexagono_batalha(campo->hexagonos[_alvo].id, _atacante, ADVERSARIO, JOGADOR))
	{
	case VITORIA_ATAQUE:
		campo->hexagonos[_alvo].estado = ADVERSARIO;
		BM_Player_getIAPlayer()->quantidadeTerritorio++;
		BM_Player_getJogador()->quantidadeTerritorio--;
		break;
	case VITORIA_DEFESA:
		campo->hexagonos[ia->hexagonoAtual].estado = JOGADOR;
		BM_Player_getIAPlayer()->quantidadeTerritorio--;
		BM_Player_getJogador()->quantidadeTerritorio++;
	}
	campo->hexagonos[_alvo].alvo = HEXAGONO_NORMAL;
	BM_Rodada_avancar();
	BM_Jogo_verificar_fim();
	BM_IA_executando = FALSE;
}
//==========================================================================

//==========================================================================
// Checar elemento
//==========================================================================
int BM_IA_checar_elemento() {
	int elementosDisponiveis = 0, ordemElementos[6], vazio = TRUE, *elemento, i, resultado;
	elemento = &ia->elementosDisponivel.luz;
	for (i = 0; i < 6; i++, elemento++) {
		if (*elemento == TRUE) {
			ordemElementos[elementosDisponiveis++] = i + 1;
			vazio = FALSE;
		}
	}
	if (vazio == TRUE) {
		elemento = &ia->elementosDisponivel.luz;
		for (i = 0; i < 6; i++, elemento++) {
			*elemento = TRUE;
		}
	}
	resultado = ordemElementos[rand() % elementosDisponiveis];
	elemento = &ia->elementosDisponivel.luz;
	elemento[resultado] = FALSE;
	return ordemElementos[rand() % elementosDisponiveis];
}
//==========================================================================

//==========================================================================
// Checar tempo elemento
//==========================================================================
void BM_IA_checar_tempo() {
	int *elementoTempo, *elemento, i;
	elementoTempo = &ia->elementosTempo.luz;
	for (i = 0; i < 6; i++, elementoTempo++) {
		*elementoTempo++;
	}
	elementoTempo = &ia->elementosTempo.luz;
	elemento = &ia->elementosDisponivel.luz;
	for (i = 0; i < 6; i++, elementoTempo++, elemento++) {
		if (*elementoTempo == 4) {
			*elementoTempo = 0;
			*elemento = TRUE;
		}
	}
}
//==========================================================================

//==========================================================================
// Inserir ordem de ação da IA
//==========================================================================
void BM_IA_iniciar_acao(BM_EVENTO_FUNCAO _funcao, int _quantidade, ...) {
	va_list ap;
	BM_Campo *campo = BM_Campo_getCampo();
	int *parametro = (int*)malloc(_quantidade * sizeof(int)), i;
	va_start(ap, _quantidade);
	for (i = 0; i < _quantidade; i++) {
		parametro[i] = va_arg(ap, int);
	}
	BM_IA_acao.tempoFrames = 60;
	BM_IA_acao.tempoAtual = 0;
	BM_Eventos_Funcoes_adicionar(_funcao, parametro);
	va_end(ap);
}
//==========================================================================

//==========================================================================
// Aguardar para ação de ataque
//==========================================================================
void BM_IA_aguardar_ataque(void *_parametro) {
	if (BM_IA_acao.tempoAtual >= BM_IA_acao.tempoFrames) {
		BM_Eventos_Funcoes_remover(BM_IA_aguardar_ataque);
		BM_IA_preparar_ataque(*(int*)_parametro);
	}
	else
		BM_IA_acao.tempoAtual++;
}
//==========================================================================

//==========================================================================
// Aguardar para ação de escolha
//==========================================================================
void BM_IA_aguardar_escolha(void *_parametro) {
	int *parametro;
	if (BM_IA_acao.tempoAtual >= BM_IA_acao.tempoFrames) {
		parametro = (int*)_parametro;
		BM_Eventos_Funcoes_remover(BM_IA_aguardar_escolha);
		BM_IA_executar_ataque(parametro[0], parametro[1]);
	}
	else
		BM_IA_acao.tempoAtual++;
}
//==========================================================================

//==========================================================================
// Retornar estado atual da IA
//==========================================================================
int BM_IA_get_executando() {
	return BM_IA_executando;
}
//==========================================================================
