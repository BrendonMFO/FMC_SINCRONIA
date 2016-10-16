#include "BM_Recursos.h"

//==========================================================================
// Retornar sprite
//==========================================================================
BM_SPRITES * BM_Recursos_obter_sprite(int _indice)
{
	return &SPRITES[_indice];
}
//==========================================================================

//==========================================================================
// Alterar frame atual do sprite
//==========================================================================
void BM_Recursos_alterar_frame_sprite(int _indice, int _frameLinha, int _frameColuna) {
	SPRITES[_indice].frameAtualLinha = _frameLinha;
	SPRITES[_indice].frameAtualColuna = _frameColuna;
}
//==========================================================================
