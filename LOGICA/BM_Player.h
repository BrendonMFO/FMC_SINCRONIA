#pragma once

#include "BM_Elemento.h"

//==========================================================================
// Estrutura de dados de um player
//==========================================================================
typedef struct BM_PLAYER_S {
	int quantidadeTerritorio;
	int hexagonoAtual;
	BM_ELEMENTOS elementosNivel;
	BM_ELEMENTOS elementosQuantidade;
}BM_PLAYER;
//==========================================================================

//==========================================================================
// Iniciar player
//==========================================================================
int BM_Player_iniciar(int _hexagonoInicialJogador);
//==========================================================================

//==========================================================================
// Get jogador
//==========================================================================
BM_PLAYER *BM_Player_getJogador();
//==========================================================================

