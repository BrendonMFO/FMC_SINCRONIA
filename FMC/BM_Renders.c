#include "BM_Renders.h"
#include "BM_Recursos.h"
#include "BM_Recursos_Sprites.h"
#include "BM_Recursos_Animacao.h"
#include "BM_Player.h"
#include "BM_Campo.h"

//=========================================================================
// Macros
//=========================================================================
#define RENDER_IMG(IMG,X,Y,FLAG) (al_draw_bitmap(SPRITES((IMG))->Imagem, (X), (Y), (FLAG)))
#define RENDER_REGION_SPRITE (SPRITE,SX,SY,SW,SH,DX,DY,F) (al_draw_bitmap_region((SPRITE),(SX),(SY),(SW),(SH),(DX),(DY),(F)))
#define RENDER_REGION(IMG,SX,SY,SW,SH,DX,DY,F) (al_draw_bitmap_region(SPRITES((IMG))->Imagem,(SX),(SY),(SW),(SH),(DX),(DY),(F)))
//=========================================================================

//==========================================================================
// Prototipos
//==========================================================================
void BM_Render_campo();
void BM_Render_animacao();
void BM_Render_player();
void BM_Render_exagono(BM_Hexagono _hexagono);
//==========================================================================

//==========================================================================
// Loop de renderização principal
//==========================================================================
void BM_Render_principal() {
	RENDER_IMG(BM_IMG_MAPA_01, 0, 0, 0);
	BM_Render_campo();
	BM_Render_animacao();
	BM_Render_player();
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
	int sourceW = BM_Allegro_largura_da_imagem(SPRITES(BM_IMG_HEXAGONO)->Imagem) / SPRITES(BM_IMG_HEXAGONO)->imagem->framesColunas;
	int sourceH = BM_Allegro_altura_da_imagem(SPRITES(BM_IMG_HEXAGONO)->Imagem) / SPRITES(BM_IMG_HEXAGONO)->imagem->framesLinhas;
	int sourceX = sourceW * _hexagono.estado;
	int sourceY = sourceH * _hexagono.elemento;
	RENDER_REGION(BM_IMG_HEXAGONO, sourceX, sourceY, sourceW, sourceH, _hexagono.posicaoX, _hexagono.posicaoY, 0);
}
//==========================================================================

//==========================================================================
// Renderizar animações pendentes
//==========================================================================
void BM_Render_animacao() {
	BM_ANIMACAO *aux;
	int sourceW, sourceH, sourceX, sourceY;
	for (aux = BM_Animacao_obter_fila()->inicio; aux != NULL; aux = aux->proximo) {
		sourceW = BM_Allegro_largura_da_imagem(aux->sprite->Imagem) / aux->sprite->imagem->framesColunas;
		sourceH = BM_Allegro_altura_da_imagem(aux->sprite->Imagem) / aux->sprite->imagem->framesLinhas;
		sourceX = sourceW * aux->frameAtualColuna;
		sourceY = sourceH * aux->frameAtualLinha;
		RENDER_REGION(aux->sprite->Imagem, sourceX, sourceY, sourceW, sourceH, aux->renderX, aux->renderY, 0);
		if (aux->render == SIM) {
			aux->render = NAO;
			BM_Animacao_avancar(aux);
		}
	}
	for (aux = BM_Animacao_obter_fila()->inicio; aux != NULL;) {
		if (aux->finalizado == SIM) {
			if (aux->proximo != NULL) {
				aux = aux->proximo;
				BM_Animacao_remover(aux->anterior);
			}
			else {
				BM_Animacao_remover(aux);
				break;
			}
		}
		else
			aux = aux->proximo;
	}
}
//==========================================================================

//==========================================================================
// Renderizar players
//==========================================================================
void BM_Render_player() {
	int sourceW, sourceH, destinoX, destinoY;
	sourceW = BM_Allegro_largura_da_imagem(SPRITES(BM_IMG_PLAYER)->Imagem);
	sourceH = BM_Allegro_altura_da_imagem(SPRITES(BM_IMG_PLAYER)->Imagem);
	destinoX = (sourceW / 2) + BM_Campo_getCampo()->hexagonos[BM_Player_getJogador()->hexagonoAtual].posicaoX;
	destinoY = (sourceH / 6) + BM_Campo_getCampo()->hexagonos[BM_Player_getJogador()->hexagonoAtual].posicaoY;
	RENDER_REGION(BM_IMG_PLAYER, 0, 0, sourceW, sourceH, destinoX, destinoY, 0);
}
//==========================================================================

