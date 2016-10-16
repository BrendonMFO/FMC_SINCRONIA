#include "BM_Recursos.h"

//==========================================================================
// Retornar sprite
//==========================================================================
BM_SPRITES * BM_Recursos_obter_sprite(int _indice)
{
	return &BM_VETOR_SPRITES[_indice];
}
//==========================================================================

//==========================================================================
// Alterar frame atual do sprite
//==========================================================================
void BM_Recursos_alterar_frame_sprite(int _indice, int _frameLinha, int _frameColuna) {
	BM_VETOR_SPRITES[_indice]->frameAtualLinha = _frameLinha;
	BM_VETOR_SPRITES[_indice]->frameAtualColuna = _frameColuna;
}
//==========================================================================