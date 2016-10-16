#pragma once

#include "BM_Allegro_Imagens.h"

//==========================================================================
// Definir constantes
//==========================================================================
#define Imagem imagem->bitmap
//==========================================================================

//==========================================================================
// Estrutura de dados dos sprites
//==========================================================================
typedef struct BM_SPRITES_S {
	BM_IMAGEM *imagem;
	int frameAtualLinha;
	int frameAtualColuna;
}BM_SPRITES;
//==========================================================================

//==========================================================================
// ENUM - Enumeração dos sprites disponiveis
//==========================================================================
enum ENUM_SPRITES
{
	BM_IMG_MAPA_01,
	BM_IMG_HEXAGONO,
	BM_IMG_ANI_AGUA
};
//==========================================================================

//==========================================================================
// Retornar sprite
//==========================================================================
BM_SPRITES *BM_Recursos_obter_sprite(int _indice);
//==========================================================================

//==========================================================================
// Alterar frame atual do sprite
//==========================================================================
void BM_Recursos_alterar_frame_sprite(int _indice, int _frameLinha, int _frameColuna);
//==========================================================================
