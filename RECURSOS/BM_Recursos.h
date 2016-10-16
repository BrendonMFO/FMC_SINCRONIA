#pragma once

#include "BM_Recursos_Sprites.h"

//==========================================================================
// Constantes
//==========================================================================
#define ERRO 0
#define SUCESSO 1
#define QUANTIDADE_SPRITES 2
//==========================================================================

//==========================================================================
// Variaveis globais com os recursos disponiveis
//==========================================================================
BM_SPRITES *BM_VETOR_SPRITES[QUANTIDADE_SPRITES];
//==========================================================================

//==========================================================================
// Carregar Sprites
//==========================================================================
int BM_Recursos_carregar_Sprites();
//==========================================================================