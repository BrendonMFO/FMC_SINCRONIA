#include "BM_Renders.h"
#include "BM_Recursos.h"
#include "BM_Recursos_Conteudo.h"
#include "BM_Recursos_Animacao.h"
#include "BM_Player.h"
#include "BM_Player_IA.h"
#include "BM_Rodadas.h"
#include "BM_Campo.h"
#include <string.h>

//=========================================================================
// Macros
//=========================================================================
#define RENDER_IMG(IMG,X,Y,FLAG) (al_draw_bitmap(SPRITES((IMG))->Imagem, (X), (Y), (FLAG)))
#define RENDER_REGION(IMG,SX,SY,SW,SH,DX,DY,F) (al_draw_bitmap_region(SPRITES((IMG))->Imagem,(SX),(SY),(SW),(SH),(DX),(DY),(F)))
#define RENDER_REGION_SCALED(IMG,SX,SY,SW,SH,DX,DY,DW,DH,F) (al_draw_scaled_bitmap((IMG),(SX),(SY),(SW),(SH),(DX),(DY),(DW),(DH),(F)))
//=========================================================================

//==========================================================================
// Variaveis
//==========================================================================
BM_RENDER_FILA *renderFila = NULL;
int TUTORIAL = 0;
//==========================================================================

//==========================================================================
// Prototipos
//==========================================================================
void BM_Render_campo();
void BM_Render_animacao();
void BM_Render_player();
void BM_Render_player_ia();
void BM_Render_hexagono(BM_HEXAGONO _hexagono);
void BM_Render_renderizar_fila();
BM_RENDER *BM_Render_procurar_fila(BM_RENDER_FUNCAO _funcao);
//==========================================================================

//==========================================================================
// Loop de renderização principal
//==========================================================================
void BM_Render_principal() {
	RENDER_IMG(BM_IMG_MAPA_01, 0, 0, 0);
	BM_Render_campo();
	BM_Render_player();
	BM_Render_player_ia();
	BM_Render_renderizar_fila();
	BM_Render_animacao();
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
void BM_Render_hexagono(BM_HEXAGONO _hexagono)
{
	int sourceW, sourceH, sourceX, sourceY;
	sourceW = BM_Allegro_largura_da_imagem(SPRITES(BM_IMG_HEXAGONO)->Imagem) / SPRITES(BM_IMG_HEXAGONO)->imagem->framesColunas;
	sourceH = BM_Allegro_altura_da_imagem(SPRITES(BM_IMG_HEXAGONO)->Imagem) / SPRITES(BM_IMG_HEXAGONO)->imagem->framesLinhas;
	sourceX = sourceW * _hexagono.estado;
	if (_hexagono.visivel == TRUE || _hexagono.estado == JOGADOR) sourceY = sourceH * _hexagono.elemento;
	else sourceY = 0;
	RENDER_REGION(BM_IMG_HEXAGONO, sourceX, sourceY, sourceW, sourceH, _hexagono.posicaoX, _hexagono.posicaoY, 0);
	sourceW = BM_Allegro_largura_da_imagem(SPRITES(BM_IMG_HEXAGONO_ALVO)->Imagem) / SPRITES(BM_IMG_HEXAGONO_ALVO)->imagem->framesColunas;
	sourceH = BM_Allegro_altura_da_imagem(SPRITES(BM_IMG_HEXAGONO_ALVO)->Imagem) / SPRITES(BM_IMG_HEXAGONO_ALVO)->imagem->framesLinhas;
	if(_hexagono.alvo != HEXAGONO_NORMAL)
		RENDER_REGION(BM_IMG_HEXAGONO_ALVO, sourceW * (_hexagono.alvo - 1), 0, sourceW, sourceH, _hexagono.posicaoX, _hexagono.posicaoY, 0);
}
//==========================================================================

//==========================================================================
// Renderizar animações pendentes
//==========================================================================
void BM_Render_animacao() {
	BM_ANIMACAO *aux;
	int sourceW, sourceH, sourceX, sourceY, destinoW, destinoH;
	for (aux = BM_Animacao_obter_fila()->inicio; aux != NULL; aux = aux->proximo) {
		sourceW = BM_Allegro_largura_da_imagem(aux->sprite->Imagem) / aux->sprite->imagem->framesColunas;
		sourceH = BM_Allegro_altura_da_imagem(aux->sprite->Imagem) / aux->sprite->imagem->framesLinhas;
		sourceX = sourceW * aux->frameAtualColuna;
		sourceY = sourceH * aux->frameAtualLinha;
		RENDER_REGION_SCALED(aux->sprite->Imagem, sourceX, sourceY, sourceW, sourceH, aux->destinoX, aux->destinoY, aux->destinoW, aux->destinoH, 0);
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
// Renderizar players
//==========================================================================
void BM_Render_player_ia() {
	int sourceW, sourceH, destinoX, destinoY;
	sourceW = BM_Allegro_largura_da_imagem(SPRITES(BM_IMG_PLAYER_IA)->Imagem);
	sourceH = BM_Allegro_altura_da_imagem(SPRITES(BM_IMG_PLAYER_IA)->Imagem);
	destinoX = (sourceW / 2) + BM_Campo_getCampo()->hexagonos[BM_Player_getIAPlayer()->hexagonoAtual].posicaoX;
	destinoY = (sourceH / 6) + BM_Campo_getCampo()->hexagonos[BM_Player_getIAPlayer()->hexagonoAtual].posicaoY;
	RENDER_REGION(BM_IMG_PLAYER_IA, 0, 0, sourceW, sourceH, destinoX, destinoY, 0);
}
//==========================================================================

//==========================================================================
// Renderizar Elementos
//==========================================================================
void BM_Render_elementos(void *_parametro, ...) {
	int sourceW, sourceH, sourceX, destinoX, i, *elemento;
	al_draw_filled_rectangle(0, 0, 1600, 920, al_map_rgba(0, 0, 0, 150));
	elemento = &BM_Player_getJogador()->elementosDisponivel.luz;
	for (i = 0; i <= 6; i++, elemento++) {
		sourceW = BM_Allegro_largura_da_imagem(SPRITES(BM_IMG_ELEMENTOS)->Imagem) / SPRITES(BM_IMG_ELEMENTOS)->imagem->framesColunas;
		sourceH = BM_Allegro_altura_da_imagem(SPRITES(BM_IMG_ELEMENTOS)->Imagem) / SPRITES(BM_IMG_ELEMENTOS)->imagem->framesLinhas;
		sourceX = sourceW * i;
		destinoX = 196 + (sourceW * i);
		if(*elemento == TRUE)
			al_draw_bitmap_region(SPRITES(BM_IMG_ELEMENTOS)->Imagem, sourceX, 0, sourceW, sourceH, destinoX, 266, 0);
		else
			al_draw_tinted_bitmap_region(SPRITES(BM_IMG_ELEMENTOS)->Imagem, al_map_rgb(100, 100, 100), sourceX, 0, sourceW, sourceH, destinoX, 266, 0);
	}
	al_draw_textf(BM_Recursos_obter_fonte(BM_FONTE_ALBA_MENOR), al_map_rgb(255, 200, 200), 1300, 800, ALLEGRO_ALIGN_LEFT, "ESC - Voltar", BM_Rodada_get_restantes());
}
//==========================================================================

//==========================================================================
// Renderizar Rodada
//==========================================================================
void BM_Render_rodada(void *_parametro, ...) {
	al_draw_textf(BM_Recursos_obter_fonte(BM_FONTE_ALBA_MENOR), al_map_rgb(225, 225, 225), 1100, 850, ALLEGRO_ALIGN_LEFT, "A - Atacar S - Adicionar", BM_Rodada_get_restantes());
	al_draw_textf(BM_Recursos_obter_fonte(BM_FONTE_ALBA), al_map_rgb(255, 200, 200), 100, 10, ALLEGRO_ALIGN_CENTRE, "%d", BM_Rodada_get_restantes());
}
//==========================================================================

//==========================================================================
// Renderizar texto jogo
//==========================================================================
void BM_Render_texto(void *_parametro, ...) {
	al_draw_text(BM_Recursos_obter_fonte(BM_FONTE_ALBA), al_map_rgb(255, 200, 200), 0, 0, ALLEGRO_ALIGN_LEFT, "A - atacar S - Adicionar");
}
//==========================================================================

//==========================================================================
// Renderizar resultado
//==========================================================================
void BM_Render_resultado(void *_parametro, ...) {
	int jogador = BM_Player_getJogador()->quantidadeTerritorio;
	int ia = BM_Player_getIAPlayer()->quantidadeTerritorio;
	char texto[10];
	al_draw_filled_rectangle(0, 0, 1600, 920, al_map_rgba(0, 0, 0, 150));
	if (jogador > ia) strcpy(texto, "Vitoria");
	else if (jogador < ia) strcpy(texto, "Derrota");
	else strcpy(texto, "Empate");
	al_draw_textf(BM_Recursos_obter_fonte(BM_FONTE_ALBA), al_map_rgb(220, 220, 220), 800, 125, ALLEGRO_ALIGN_CENTRE, "%s", texto);
	RENDER_REGION(BM_IMG_HEXAGONO, 238, 0, 238, 206, 323, 326, 0);
	RENDER_REGION(BM_IMG_HEXAGONO, 476, 0, 238, 206, 1065, 326, 0);
	al_draw_textf(BM_Recursos_obter_fonte(BM_FONTE_ALBA), al_map_rgb(255, 255, 255), 435, 350, ALLEGRO_ALIGN_CENTRE, "%d", jogador);
	al_draw_textf(BM_Recursos_obter_fonte(BM_FONTE_ALBA), al_map_rgb(255, 255, 255), 1185, 350, ALLEGRO_ALIGN_CENTRE, "%d", ia);
	al_draw_text(BM_Recursos_obter_fonte(BM_FONTE_ALBA), al_map_rgb(255, 255, 255), 800, 700, ALLEGRO_ALIGN_CENTRE, "R - Reiniciar");

}
//==========================================================================

//==========================================================================
// Renderizar tutorial
//==========================================================================
void BM_Render_tutorial(void *_parametro, ...) {
	al_draw_filled_rectangle(0, 0, 1600, 920, al_map_rgba(0, 0, 0, 220));
	al_draw_bitmap(BM_Recursos_obter_tutorial(TUTORIAL), 0, 0, 0);
}
//==========================================================================

//==========================================================================
// Avançar render
//==========================================================================
void BM_Render_avancar_tutorial() {
	TUTORIAL++;
}
//==========================================================================

//==========================================================================
// Obter tutorial
//==========================================================================
int BM_Render_obter_tutorial() {
	return TUTORIAL;
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

