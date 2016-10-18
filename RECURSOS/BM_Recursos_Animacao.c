
#include "BM_Recursos_Animacao.h"
#include <stdio.h>
#include <stdlib.h>

//==========================================================================
// Fila de animações pendentes
//==========================================================================
BM_ANIMACAO_FILA *animacoes;
//==========================================================================

//==========================================================================
// Prototipos
//==========================================================================
BM_ANIMACAO *BM_Animacao_procurar_fila(BM_ANIMACAO *_animacao);
//==========================================================================

//==========================================================================
// Obter fila de animações pendentes
//==========================================================================
BM_ANIMACAO_FILA *BM_Animacao_obter_fila() {
	return animacoes;
}
//==========================================================================

//==========================================================================
// Iniciar fila de animações
//==========================================================================
int BM_Animacao_iniciar_fila() {
	animacoes = (BM_ANIMACAO_FILA*)malloc(1 * sizeof(BM_ANIMACAO_FILA));
	if (animacoes == NULL) {
		fprintf(stderr, "ERRO: Nao foi possivel alocar memoria para a fila de animacoes\n");
		return ERRO;
	}
	animacoes->inicio = NULL;
	animacoes->fim = NULL;
	return SUCESSO;
}
//==========================================================================

//==========================================================================
// Adicionar animação na lista
//==========================================================================
int BM_Animacao_adicionar(BM_SPRITES *_sprite, int _renderX, int _renderY) {
	BM_ANIMACAO *aux = (BM_ANIMACAO*)malloc(1 * sizeof(BM_ANIMACAO));
	if (aux == NULL) {
		fprintf(stderr, "ERRO: Nao foi possivel alocar memoria para uma animacao");
		return ERRO;
	}
	aux->sprite = _sprite;
	aux->renderX = _renderX;
	aux->renderY = _renderY;
	aux->anterior = NULL;
	if (animacoes->inicio == NULL) {
		animacoes->inicio = aux;
		animacoes->fim = aux;
		aux->proximo = NULL;
	}
	else
	{
		animacoes->inicio->anterior = aux;
		aux->proximo = animacoes->inicio;
		animacoes->inicio = aux;
	}
	return SUCESSO;
}
//==========================================================================

//==========================================================================
// Remover animacao da lista
//==========================================================================
void BM_Animacao_remover(BM_ANIMACAO *_animacao) {
	BM_ANIMACAO *aux = BM_Animacao_procurar_fila(_animacao);
	if (aux == NULL)
		return;
	if (animacoes->inicio == animacoes->fim) {
		animacoes->inicio = NULL;
		animacoes->fim = NULL;
	}
	else {
		if (aux->anterior != NULL) 
			aux->anterior->proximo = aux->proximo;
		else 
			animacoes->inicio = aux->proximo;
		if (aux->proximo != NULL)
			aux->proximo->anterior = aux->anterior;
		else
			animacoes->fim = aux->anterior;
	}
	free(aux);
}
//==========================================================================

//==========================================================================
// Procurar na fila uma animação especifica
//==========================================================================
BM_ANIMACAO *BM_Animacao_procurar_fila(BM_ANIMACAO *_animacao) {
	BM_ANIMACAO *aux;
	for (aux = animacoes->inicio; aux != _animacao; aux = aux->proximo) {
		if (aux == NULL)
			return NULL;
	}
	return aux;
}
//==========================================================================