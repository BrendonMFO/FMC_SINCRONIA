#pragma once

#include "BM_Player_IA.h"
#include "BM_Campo.h"
#include "BM_Hexagono.h"
#include <stdio.h>

//==========================================================================
// Variaveis
//==========================================================================
BM_JOGADOR_IA *ia = NULL;
//==========================================================================

//==========================================================================
// Defines
//==========================================================================
#define ERRO 0
#define SUCESSO 1
//==========================================================================

//==========================================================================
// Prototipos
//==========================================================================
void iniciarValores_IA(int _hexagono);
void adicionarHexagonos_IA();
//==========================================================================

//==========================================================================
// Iniciar player IA
//==========================================================================
int BM_Player_IA_iniciar(int _hexagonoAtual) {

	//======================================================================
	// Alocar memoria para o player da IA
	//======================================================================
	ia = (BM_JOGADOR_IA*)malloc(1 * sizeof(BM_JOGADOR_IA));
	if (ia == NULL) {
		fprintf(stderr, "ERRO: Nao foi possivel alocar memoria para um player");
		return ERRO;
	}
	//======================================================================

	//======================================================================
	// Alocar memoria para os dados de player da IA
	//======================================================================
	ia->ia = (BM_PLAYER*)malloc(1 * sizeof(BM_PLAYER));
	if (ia->ia == NULL) {
		fprintf(stderr, "ERRO: Nao foi possivel alocar memoria para um player");
		return ERRO;
	}
	iniciarValores_IA(ia, _hexagonoAtual);
	//======================================================================

}
//==========================================================================

//==========================================================================
// Get IA
//==========================================================================
BM_JOGADOR_IA *BM_Player_getIAPlayer() {
	return ia;
}
//==========================================================================

//==========================================================================
// Iniciar valores da estrutura de jogadores
//==========================================================================
void iniciarValores_IA(int _hexagono) {
	ia->hexagonos = BM_Hexagono_iniciar_fila();
	ia->ia->hexagonoAtual = _hexagono;
	ia->ia->quantidadeTerritorio = 0;
	ia->ia->elementosQuantidade.luz = ia->ia->elementosNivel.luz = 0;
	ia->ia->elementosQuantidade.trevas = ia->ia->elementosNivel.trevas = 0;
	ia->ia->elementosQuantidade.fogo = ia->ia->elementosNivel.fogo = 0;
	ia->ia->elementosQuantidade.agua = ia->ia->elementosNivel.agua = 0;
	ia->ia->elementosQuantidade.terra = ia->ia->elementosNivel.terra = 0;
	ia->ia->elementosQuantidade.ar = ia->ia->elementosNivel.ar = 0;
	adicionarHexagonos_IA();
}
//==========================================================================

//==========================================================================
// Adicionar hexagonos IA
//==========================================================================
void adicionarHexagonos_IA() {
	int i;
	BM_Campo *campo = BM_Campo_getCampo();
	for (i = 0; i < campo->quantidade; i++) {
		if (campo->hexagonos[i].estado == ADVERSARIO)
			BM_Hexagono_fila_inserir(ia->hexagonos, campo->hexagonos[i]);
	}
}
//==========================================================================

//==========================================================================
// Checar possiblidade de ataque
//==========================================================================
int BM_IA_possibilidade_ataque() {
	int hexagonoAtaque, resultadoAtaque = 0, ataqueAtual, i = 0;
	BM_Hexagono *hexagono;
	for (hexagono = ia->hexagonos->inicio; hexagono != NULL; hexagono = hexagono->proximo, i++) {
		ataqueAtual = BM_Hexagono_calcular_sincronia(*hexagono, ADVERSARIO);
		if (ataqueAtual > resultadoAtaque) {
			hexagonoAtaque = 1;
			resultadoAtaque = ataqueAtual;
		}
	}
	return resultadoAtaque;
}
//==========================================================================