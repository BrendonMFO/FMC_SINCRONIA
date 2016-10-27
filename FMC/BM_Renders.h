#pragma once

#include "BM_Allegro.h"

//==========================================================================
// Typedef's
//==========================================================================
typedef void(*BM_RENDER_FUNCAO)(void*, ...);
//==========================================================================

//==========================================================================
// Estrutura de dados de fun��es para renderizar elementos
//==========================================================================
typedef struct BM_RENDER_S {
	int ativo;
	BM_RENDER_FUNCAO funcao;
	void *parametro;
	struct BM_RENDER_S *anterior;
	struct BM_RENDER_S *proximo;
}BM_RENDER;
//==========================================================================

//==========================================================================
// Fila de dados de fun��es para renderiza��o
//==========================================================================
typedef struct BM_RENDER_FILA_S
{
	BM_RENDER *inicio;
	BM_RENDER *fim;
}BM_RENDER_FILA;
//==========================================================================

//==========================================================================
// Iniciar fila de renderiza��o
//==========================================================================
int BM_Render_iniciar_fila();
//==========================================================================

//==========================================================================
// Adicionar fun��o na fila de renderiza��o
//==========================================================================
int BM_Render_adicionar_funcao(BM_RENDER_FUNCAO _funcao, void *_parametro);
//==========================================================================

//==========================================================================
// Remover fun��o da fila de renderiza��o
//==========================================================================
int BM_Render_remover_funcao(BM_RENDER_FUNCAO _funcao);
//==========================================================================

//==========================================================================
// Renderizar Campo
//==========================================================================
void BM_Render_principal();
//==========================================================================

//==========================================================================
// Renderizar elementos
//==========================================================================
void BM_Render_elementos(void *_parametro);
//==========================================================================

//==========================================================================
// Renderizar Rodada
//==========================================================================
void BM_Render_rodada(void *_parametro);
//==========================================================================

//==========================================================================
// Renderizar resultado
//==========================================================================
void BM_Render_resultado(void *_parametro);
//==========================================================================

//==========================================================================
// Renderizar tutorial
//==========================================================================
void BM_Render_tutorial(void *_parametro);
//==========================================================================

//==========================================================================
// Avan�ar render
//==========================================================================
void BM_Render_avancar_tutorial();
//==========================================================================

//==========================================================================
// Obter tutorial
//==========================================================================
int BM_Render_obter_tutorial();
//==========================================================================