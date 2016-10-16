#include "BM_Renders.h"

//=========================================================================
// Macros
//=========================================================================
#define RENDER_IMG(IMG,X,Y,FLAG) (al_draw_bitmap( BM_VETOR_SPRITES[(IMG)]->Imagem, (X), (Y), (FLAG)))
#define RENDER_REGION(IMG,SX,SY,SW,SH,DX,DY,F) (al_draw_bitmap_region(BM_VETOR_SPRITES[(IMG)]->Imagem,(SX),(SY),(SW),(SH),(DX),(DY),(F)))
//=========================================================================

//==========================================================================
// Prototipos
//==========================================================================
void BM_Render_campo();
void BM_Render_exagono(BM_Hexagono _hexagono);
void BM_Render_renderizar_regiao();
//==========================================================================

//==========================================================================
// Loop de renderização principal
//==========================================================================
void BM_Render_principal() {
	RENDER_IMG(BM_IMG_MAPA_01, 0, 0, 0);
	BM_Render_campo();
	al_flip_display();
}
//==========================================================================

//==========================================================================
// Renderizar Campo
//==========================================================================
void BM_Render_campo() {
	int i;
	BM_Campo *campo = BM_Campo_getCampo();
	for (i = 0; i < campo->quantidade; i++) {
		BM_Render_exagono(campo->hexagonos[i]);
	}
}
//==========================================================================

//==========================================================================
// Renderizar hexagono do campo
//==========================================================================
void BM_Render_exagono(BM_Hexagono _hexagono)
{
	int sourceW = BM_Allegro_largura_da_imagem(BM_VETOR_SPRITES[BM_IMG_HEXAGONO]->Imagem) / BM_VETOR_SPRITES[BM_IMG_HEXAGONO]->imagem->framesColunas;
	int sourceH = BM_Allegro_altura_da_imagem(BM_VETOR_SPRITES[BM_IMG_HEXAGONO]->Imagem) / BM_VETOR_SPRITES[BM_IMG_HEXAGONO]->imagem->framesLinhas;
	int sourceX = sourceW * _hexagono.estado;
	int sourceY = sourceH * _hexagono.elemento;
	RENDER_REGION(BM_IMG_HEXAGONO, sourceX, sourceY, sourceW, sourceH, _hexagono.posicaoX, _hexagono.posicaoY, 0);
}
//==========================================================================
