#pragma once

#include "BM_Recursos_Sprites.h"
#include "BM_Animacao.h"

//==========================================================================
// Constantes
//==========================================================================
#define ERRO 0
#define SUCESSO 1
#define QUANTIDADE_SPRITES 3
#define SPRITES BM_Recursos_array()
//==========================================================================

//==========================================================================
// Obter array com os recursos visuais carregados
//==========================================================================
BM_SPRITES *BM_Recursos_array();
//==========================================================================

//==========================================================================
// Carregar Sprites
//==========================================================================
int BM_Recursos_carregar_Sprites();
//==========================================================================