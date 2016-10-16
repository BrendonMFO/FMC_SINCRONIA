#include "BM_Allegro_Imagens.h"

//==========================================================================
// Função - Carregar imagem teste
//==========================================================================
BM_IMAGEM *BM_Allegro_carregar_imagem(char *img_path, int _framesLinhas, int _framesColunas) {
	BM_IMAGEM *temp = (BM_IMAGEM*)malloc(1 * sizeof(BM_IMAGEM));
	temp->bitmap = al_load_bitmap(img_path);
	if (!temp->bitmap) return NULL;
	temp->framesLinhas = _framesLinhas;
	temp->framesColunas = _framesColunas;
	return temp;
}
//==========================================================================

//==========================================================================
// Funçao - Retornar largura 
//==========================================================================
int BM_Allegro_largura_da_imagem(ALLEGRO_BITMAP *_imagem) {
	return al_get_bitmap_width(_imagem);
}
//==========================================================================

//==========================================================================
// Função - Retornar altura
//==========================================================================
int BM_Allegro_altura_da_imagem(ALLEGRO_BITMAP *_imagem) {
	return al_get_bitmap_height(_imagem);
}
//==========================================================================