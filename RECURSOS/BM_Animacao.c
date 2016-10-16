
#include "BM_Animacao.h"
#include <stdlib.h>

//==========================================================================
// Fila de animações pendentes
//==========================================================================
BM_ANIMACAO_LISTA_S animacoes;
//==========================================================================

//==========================================================================
// Adicionar animação na lista
//==========================================================================
int BM_Animacao_adicionar(BM_SPRITES *_sprite, int _renderX, int _renderY) {
	BM_ANIMACAO_LISTA_S *aux = (BM_ANIMACAO_LISTA_S*)malloc(1 * sizeof(BM_ANIMACAO_LISTA_S));
	aux->sprite = _sprite;
	aux->renderX = _renderX;
	aux->renderY = _renderY;
	aux->anterior = NULL;
	aux->proximo = &animacoes;
	animacoes.
}
//==========================================================================