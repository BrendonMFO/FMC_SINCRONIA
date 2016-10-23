#pragma once

#include "BM_Player_IA.h"
#include "BM_Campo.h"
#include "BM_Hexagono.h"
#include "BM_Rodadas.h"
#include <stdio.h>

//==========================================================================
// Variaveis
//==========================================================================
BM_PLAYER *ia = NULL;
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
void iniciarValores_IA(int _hexagono);
void BM_IA_possibilidade_ataque(BM_IA_ARVORE *_arvore);
void BM_IA_possibilidade_conquista(BM_IA_ARVORE *_arvore);
void BM_IA_conquistar_territorio(int _indexHexagono);
void BM_IA_atacar(int _indexHexagono);
void BM_IA_iniciar_arvore(BM_IA_ARVORE *_arvore);
int BM_IA_arvore_inserir(BM_IA **_no, int _valor, int _hexagono);
void BM_IA_arvore_desalocar(BM_IA **_no);
int BM_IA_arvore_vazia(BM_IA_ARVORE *_arvore);
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
}
//==========================================================================

//==========================================================================
// Desalocar arvore
//==========================================================================
void BM_IA_arvore_desalocar(BM_IA **_no) {
	if ((*_no) != NULL) {
		if ((*_no)->direita == NULL && (*_no)->esquerda == NULL && (*_no)->centro == NULL)
			return free((*_no));
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
		else if ((*_no)->esquerda != NULL && escolha > 60 && escolha <= 90)
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
	iniciarValores_IA(_hexagonoAtual);
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
void iniciarValores_IA(int _hexagono) {
	ia->hexagonoAtual = _hexagono;
	ia->quantidadeTerritorio = 0;
	ia->elementosQuantidade.luz = ia->elementosNivel.luz = 0;
	ia->elementosQuantidade.trevas = ia->elementosNivel.trevas = 0;
	ia->elementosQuantidade.fogo = ia->elementosNivel.fogo = 0;
	ia->elementosQuantidade.agua = ia->elementosNivel.agua = 0;
	ia->elementosQuantidade.terra = ia->elementosNivel.terra = 0;
	ia->elementosQuantidade.ar = ia->elementosNivel.ar = 0;
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
		if (decisao >= 0 && decisao <= 70)
			BM_IA_atacar(BM_IA_executar(&arvoreAtaque.raiz)->hexagono);
		else
			BM_IA_conquistar_territorio(BM_IA_executar(&arvoreConquista.raiz)->hexagono);
	}
	else {
		if(conquista == TRUE)
			BM_IA_conquistar_territorio(BM_IA_executar(&arvoreConquista.raiz)->hexagono);
		if(ataque == TRUE)
			BM_IA_atacar(BM_IA_executar(&arvoreAtaque.raiz)->hexagono);
	}
	//======================================================================

	//======================================================================
	// Desalocar arvores
	//======================================================================
	BM_IA_arvore_desalocar(&arvoreAtaque.raiz);
	BM_IA_arvore_desalocar(&arvoreConquista.raiz);
	//======================================================================

	BM_Rodada_set(BM_Rodada_get_restantes() - 1);
}
//==========================================================================

//==========================================================================
// Checar possiblidade de ataque
//==========================================================================
void BM_IA_possibilidade_ataque(BM_IA_ARVORE *_arvore) {
	int ataqueAtual, i, j;
	BM_Hexagono *hexagono = NULL;
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
	BM_Hexagono *hexagono, *aux = NULL;
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
	ia->hexagonoAtual = campo->hexagonos[_indexHexagono].id;
}
//==========================================================================

//==========================================================================
// Dispara ataque
//==========================================================================
void BM_IA_atacar(int _indexHexagono) {
	BM_Campo *campo = BM_Campo_getCampo();
	BM_Hexagono *hexagono = &campo->hexagonos[_indexHexagono];
	int i;
	ia->hexagonoAtual = hexagono->id;
	for (i = 0; i < 6; i++) {
		if (hexagono->conexoes[i] != -1)
			if (campo->hexagonos[hexagono->conexoes[i]].estado == JOGADOR)
				break;
	}
	switch (BM_Hexagono_batalha(hexagono->conexoes[i], _indexHexagono))
	{
	case VITORIA_ATAQUE:
		campo->hexagonos[hexagono->conexoes[i]].estado = ADVERSARIO;
		BM_Player_getIAPlayer()->quantidadeTerritorio++;
		BM_Player_getJogador()->quantidadeTerritorio--;
		break;
	case VITORIA_DEFESA:
		campo->hexagonos[ia->hexagonoAtual].estado = JOGADOR;
		BM_Player_getIAPlayer()->quantidadeTerritorio--;
		BM_Player_getJogador()->quantidadeTerritorio++;
	}
}
//==========================================================================