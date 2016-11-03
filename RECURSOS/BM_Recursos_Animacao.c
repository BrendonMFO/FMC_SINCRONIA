
#include "BM_Recursos_Animacao.h"
#include "BM_Allegro_eventos.h"
#include <stdio.h>
#include <stdlib.h>

//==========================================================================
// Fila de animações pendentes
//==========================================================================
BM_ANIMACAO_FILA *animacoes = NULL;
int animacaoAdicionada = 0;
//==========================================================================

//==========================================================================
// Prototipos
//==========================================================================
BM_ANIMACAO *BM_Animacao_procurar_fila(BM_ANIMACAO *_animacao);
void BM_Animacao_processar(void *_parametro, ...);
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
int BM_Animacao_adicionar(BM_SPRITES *_sprite, int _renderW, int _renderH, int _renderX, int _renderY, double _tempo) {
	BM_ANIMACAO *aux = (BM_ANIMACAO*)malloc(1 * sizeof(BM_ANIMACAO));
	if (aux == NULL) {
		fprintf(stderr, "ERRO: Nao foi possivel alocar memoria para uma animacao\n");
		return ERRO;
	}
	aux->sprite = _sprite;
	aux->frameAtualColuna = 0;
	aux->frameAtualLinha = 0;
	aux->destinoW = _renderW;
	aux->destinoH = _renderH;
	aux->destinoX = _renderX;
	aux->destinoY = _renderY;
	aux->finalizado = NAO;
	aux->render = NAO;
	aux->anterior = NULL;
	aux->tempoAtualizacao = (_tempo / (aux->sprite->imagem->sprite_colunas * aux->sprite->imagem->sprite_linhas)) * 100;
	aux->tempoAtual = 0;
	if (animacoes->inicio == NULL) {
		animacoes->inicio = aux;
		animacoes->fim = aux;
		aux->proximo = NULL;
		BM_Eventos_Funcoes_adicionar(BM_Animacao_processar, NULL);
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
		BM_Eventos_Funcoes_remover(BM_Animacao_processar);
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
// Avançar animação
//==========================================================================
void BM_Animacao_avancar(BM_ANIMACAO *_animacao) {
	if (_animacao != NULL) {
		_animacao->frameAtualColuna++;
		if (_animacao->frameAtualColuna > _animacao->sprite->imagem->sprite_colunas - 1) {
			_animacao->frameAtualLinha++;
			_animacao->frameAtualColuna = 0;
			if (_animacao->frameAtualLinha > _animacao->sprite->imagem->sprite_linhas - 1) {
				_animacao->finalizado = SIM;
				_animacao->frameAtualLinha = 0;
			}
		}
	}
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

//==========================================================================
// Processar animações
//==========================================================================
void BM_Animacao_processar(void *_parametro, ...) {
	BM_ANIMACAO *aux;
	for (aux = animacoes->inicio; aux != NULL; aux = aux->proximo) {
		if (aux->tempoAtual >= aux->tempoAtualizacao) {
			aux->render = SIM;
			aux->tempoAtual = 0;
		}
		else {
			aux->tempoAtual++;
		}
	}
}
//==========================================================================
