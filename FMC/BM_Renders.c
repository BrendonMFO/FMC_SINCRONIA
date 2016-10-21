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
// Variaveis
//==========================================================================
BM_RENDER_FILA *renderFila = NULL;
//==========================================================================

//==========================================================================
// Prototipos
//==========================================================================
void BM_Render_campo();
void BM_Render_animacao();
void BM_Render_player();
void BM_Render_hexagono(BM_Hexagono _hexagono);
void BM_Render_renderizar_fila();
BM_RENDER *BM_Render_procurar_fila(BM_RENDER_FUNCAO _funcao);
//==========================================================================

//==========================================================================
// Loop de renderização principal
//==========================================================================
void BM_Render_principal() {
	RENDER_IMG(BM_IMG_MAPA_01, 0, 0, 0);
	BM_Render_campo();
	BM_Render_animacao();
	BM_Render_player();
	BM_Render_renderizar_fila();
	al_flip_display();
}
//==========================================================================

//==========================================================================
// Iniciar fila de renderização
//==========================================================================
int BM_Render_iniciar_fila() {
	renderFila = (BM_RENDER_FILA*)malloc(1 * sizeof(BM_RENDER_FILA));
	if (renderFila == NULL) {
		printf("ERRO: Nao foi possivel alocar memoria para a fila de renderizacao\n");
		return ERRO;
	}
	renderFila->inicio = NULL;
	renderFila->fim = NULL;
	return SUCESSO;
}
//==========================================================================

//==========================================================================
// Adicionar função na fila de renderização
//==========================================================================
int BM_Render_adicionar_funcao(BM_RENDER_FUNCAO _funcao) {
	BM_RENDER *aux = (BM_RENDER*)malloc(1 * sizeof(BM_RENDER));
	if (aux == NULL) {
		printf("ERRO: Nao foi possivel alocar memoria para uma funcao de renderizacao\n");
		return ERRO;
	}
	aux->funcao = _funcao;
	aux->ativo = SIM;
	aux->proximo = NULL;
	if (renderFila->inicio == NULL) {
		renderFila->inicio = aux;
		renderFila->fim = aux;
		aux->anterior = NULL;
	}
	else {
		renderFila->fim->proximo = aux;
		aux->anterior = renderFila->fim;
		renderFila->fim = aux;
	}
	return SUCESSO;
}
//==========================================================================

//==========================================================================
// Remover função da fila de renderização
//==========================================================================
int BM_Render_remover_funcao(BM_RENDER_FUNCAO _funcao) {
	BM_RENDER *aux = BM_Render_procurar_fila(_funcao);
	if (aux == NULL)
		return;
	aux->ativo = NAO;
	if (renderFila->inicio == renderFila->fim) {
		renderFila->inicio = NULL;
		renderFila->fim = NULL;
	}
	else {
		if (aux->anterior != NULL)
			aux->anterior->proximo = aux->proximo;
		else
			renderFila->inicio = aux->proximo;
		if (aux->proximo != NULL)
			aux->proximo->anterior = aux->anterior;
		else
			renderFila->fim = aux->anterior;
	}
}
//==========================================================================

//==========================================================================
// Renderizar Campo
//==========================================================================
void BM_Render_campo() {
	int i;
	BM_Campo *campo = BM_Campo_getCampo();
	for (i = 0; i < campo->quantidade; i++) {
		BM_Render_hexagono(campo->hexagonos[i]);
	}
}
//==========================================================================

//==========================================================================
// Renderizar hexagono do campo
//==========================================================================
void BM_Render_hexagono(BM_Hexagono _hexagono)
{
	int sourceW = BM_Allegro_largura_da_imagem(SPRITES(BM_IMG_HEXAGONO)->Imagem) / SPRITES(BM_IMG_HEXAGONO)->imagem->framesColunas;
	int sourceH = BM_Allegro_altura_da_imagem(SPRITES(BM_IMG_HEXAGONO)->Imagem) / SPRITES(BM_IMG_HEXAGONO)->imagem->framesLinhas;
	int sourceX = sourceW * _hexagono.estado;
	int sourceY = sourceH * _hexagono.elemento;
	RENDER_REGION(BM_IMG_HEXAGONO, sourceX, sourceY, sourceW, sourceH, _hexagono.posicaoX, _hexagono.posicaoY, 0);
	if (_hexagono.alvo == TRUE) 
		RENDER_IMG(BM_IMG_HEXAGONO_ALVO, _hexagono.posicaoX, _hexagono.posicaoY, 0);
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

//==========================================================================
// Renderizar Elementos
//==========================================================================
void BM_Render_elementos(void *_parametro, ...) {
	int sourceW, sourceH, sourceX, destinoX, i;
	al_draw_filled_rectangle(0, 0, 1600, 920, al_map_rgba(0, 0, 0, 150));
	for (i = 0; i <= 6; i++) {
		sourceW = BM_Allegro_largura_da_imagem(SPRITES(BM_IMG_ELEMENTOS)->Imagem) / SPRITES(BM_IMG_ELEMENTOS)->imagem->framesColunas;
		sourceH = BM_Allegro_altura_da_imagem(SPRITES(BM_IMG_ELEMENTOS)->Imagem) / SPRITES(BM_IMG_ELEMENTOS)->imagem->framesLinhas;
		sourceX = sourceW * i;
		destinoX = 196 + (sourceW * i);
		al_draw_bitmap_region(SPRITES(BM_IMG_ELEMENTOS)->Imagem, sourceX, 0, sourceW, sourceH, destinoX, 266, 0);
	}
}
//==========================================================================

//==========================================================================
// Renderizar dados na fila de renderização
//==========================================================================
void BM_Render_renderizar_fila() {
	BM_RENDER *aux;
	for (aux = renderFila->inicio; aux != NULL; aux = aux->proximo)
		aux->funcao(NULL);
}
//==========================================================================

//==========================================================================
// Procurar função especifica na fila de renderiação
//==========================================================================
BM_RENDER *BM_Render_procurar_fila(BM_RENDER_FUNCAO _funcao) {
	BM_RENDER *aux;
	for (aux = renderFila->inicio; aux != NULL; aux = aux->proximo) {
		if (aux->funcao == _funcao)
			break;
	}
	return aux;
}
//==========================================================================

