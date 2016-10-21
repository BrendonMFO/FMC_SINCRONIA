#pragma once

#include "BM_Recursos_Sprites.h"

//==========================================================================
// Constantes
//==========================================================================
#define ERRO 0
#define SUCESSO 1
#define SIM 1
#define NAO 2
//==========================================================================

//==========================================================================
// Estrutura de dados das anima��es pendentes
//==========================================================================
typedef struct BM_ANIMACAO_S
{
	BM_SPRITES *sprite;
	int frameAtualLinha;
	int frameAtualColuna;
	int renderX;
	int renderY;
	int render;
	int tempoAtualizacao;
	int tempoAtual;
	int finalizado;
	struct BM_ANIMACAO_S *anterior;
	struct BM_ANIMACAO_S *proximo;
}BM_ANIMACAO;
//==========================================================================

//==========================================================================
// Lista de anima��es pendentes
//==========================================================================
typedef struct BM_ANIMACAO_FILA_S
{
	BM_ANIMACAO *inicio;
	BM_ANIMACAO *fim;
}BM_ANIMACAO_FILA;
//==========================================================================

//==========================================================================
// Obter fila de anima��es pendentes
//==========================================================================
BM_ANIMACAO_FILA *BM_Animacao_obter_fila();
//==========================================================================

//==========================================================================
// Iniciar fila
//==========================================================================
int BM_Animacao_iniciar_fila();
//==========================================================================

//==========================================================================
// Adicionar anima��o
//==========================================================================
int BM_Animacao_adicionar(BM_SPRITES *_sprite, int _renderX, int _renderY, double _tempo);
//==========================================================================

//==========================================================================
// Remover anima��o
//==========================================================================
void BM_Animacao_remover(BM_ANIMACAO *_animacao);
//==========================================================================

//==========================================================================
// Avan�ar animacao
//==========================================================================
void BM_Animacao_avancar(BM_ANIMACAO *_animacao);
//==========================================================================