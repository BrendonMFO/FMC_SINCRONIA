#include "BM_Renders.h"
#include "BM_Recursos.h"
#include "BM_Recursos_Conteudo.h"
#include "BM_Recursos_Animacao.h"
#include "BM_Allegro_janela.h"
#include "BM_Player.h"
#include "BM_Player_IA.h"
#include "BM_Rodadas.h"
#include "BM_Campo.h"
#include <string.h>

//==========================================================================
// Macros
//==========================================================================
#define RENDER_DADOS_REGIAO int _sX, int _sY, int _dX, int _dY
//==========================================================================

//==========================================================================
// Variaveis
//==========================================================================
BM_RENDER_FILA *renderFila = NULL;
int TUTORIAL = 0;
//==========================================================================

//==========================================================================
// Prototipos para funções de renders
//==========================================================================
void BM_Render_normal(BM_SPRITES *_sprite, int _x, int _y, int _flag);
void BM_Render_regiao(BM_SPRITES *_sprite, RENDER_DADOS_REGIAO, int _flag);
void BM_Render_regiao_escalonada(BM_SPRITES *_sprite, RENDER_DADOS_REGIAO, int _dW, int _dH, int _flag);
//==========================================================================

//==========================================================================
// Prototipos
//==========================================================================
void BM_Render_campo();
void BM_Render_animacao();
void BM_Render_player();
void BM_Render_player_ia();
void BM_Render_renderizar_fila();
void BM_Render_hexagono(BM_HEXAGONO _hexagono);
BM_RENDER *BM_Render_procurar_fila(BM_RENDER_FUNCAO _funcao);
//==========================================================================

//==========================================================================
// Processo de renderização de uma imagem completa
//==========================================================================
void BM_Render_normal(BM_SPRITES *_sprite, int _x, int _y, int _flag) {
	al_draw_scaled_bitmap(_sprite->Imagem, 
		0, 0, _sprite->imagem->frame_largura, _sprite->imagem->frame_altura,
		BM_AJUSTE_XW(_x), BM_AJUSTE_YH(_y), _sprite->ajusteW, _sprite->ajusteH, _flag);
}
//==========================================================================

//==========================================================================
// Processo de renderização de uma parte de uma imagem
//==========================================================================
void BM_Render_regiao(BM_SPRITES *_sprite, RENDER_DADOS_REGIAO, int _flag) {
	al_draw_scaled_bitmap(_sprite->Imagem,
		_sX, _sY, 
		_sprite->imagem->frame_largura, _sprite->imagem->frame_altura,
		BM_AJUSTE_XW(_dX), BM_AJUSTE_YH(_dY),
		_sprite->ajusteW, _sprite->ajusteH, _flag);
}
//==========================================================================

//==========================================================================
// Processo de renderização de uma parte de imagem escalonada
//==========================================================================
void BM_Render_regiao_escalonada(BM_SPRITES *_sprite, RENDER_DADOS_REGIAO, int _dW, int _dH, int _flag) {
	al_draw_scaled_bitmap(_sprite->Imagem,
		_sX, _sY, _sprite->imagem->frame_largura, _sprite->imagem->frame_altura,
		BM_AJUSTE_XW(_dX), BM_AJUSTE_YH(_dY),
		BM_AJUSTE_XW(_dW), BM_AJUSTE_YH(_sprite->ajusteH), _flag);
}
//==========================================================================

//==========================================================================
// Loop de renderização principal
//==========================================================================
void BM_Render_principal() {
	BM_Render_normal(SPRITES(BM_IMG_MAPA_01), 0, 0, 0);
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
int BM_Render_adicionar_funcao(BM_RENDER_FUNCAO _funcao, void *_parametro) {
	BM_RENDER *aux = (BM_RENDER*)malloc(1 * sizeof(BM_RENDER));
	if (aux == NULL) {
		printf("ERRO: Nao foi possivel alocar memoria para uma funcao de renderizacao\n");
		return ERRO;
	}
	aux->funcao = _funcao;
	aux->parametro = _parametro;
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
		return ERRO;
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
	return SUCESSO;
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
	int sourceX, sourceY, sourceW;
	sourceW = BM_Allegro_largura_da_imagem(SPRITES(BM_IMG_HEXAGONO)->Imagem) / SPRITES(BM_IMG_HEXAGONO)->imagem->sprite_colunas;
	sourceX = SPRITES(BM_IMG_HEXAGONO)->imagem->frame_largura * _hexagono.estado;
	if (_hexagono.visivel == TRUE || _hexagono.estado == JOGADOR) sourceY = SPRITES(BM_IMG_HEXAGONO)->imagem->frame_altura * _hexagono.elemento;
	else sourceY = 0;
	BM_Render_regiao(SPRITES(BM_IMG_HEXAGONO), sourceX, sourceY, _hexagono.posicaoX, _hexagono.posicaoY, 0);
	if(_hexagono.alvo != HEXAGONO_NORMAL)
		BM_Render_regiao(SPRITES(BM_IMG_HEXAGONO_ALVO), sourceW * (_hexagono.alvo - 1), 0, _hexagono.posicaoX, _hexagono.posicaoY, 0);
}
//==========================================================================

//==========================================================================
// Renderizar animações pendentes
//==========================================================================
void BM_Render_animacao() {
	BM_ANIMACAO *aux;
	int sourceX, sourceY;
	for (aux = BM_Animacao_obter_fila()->inicio; aux != NULL; aux = aux->proximo) {
		sourceX = aux->sprite->imagem->frame_largura * aux->frameAtualColuna;
		sourceY = aux->sprite->imagem->frame_altura * aux->frameAtualLinha;
		BM_Render_regiao_escalonada(aux->sprite, sourceX, sourceY, aux->destinoX, aux->destinoY, aux->destinoW, aux->destinoH, 0);
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
	int destinoX, destinoY;
	destinoX = (SPRITES(BM_IMG_PLAYER)->imagem->frame_largura / 2) + BM_Campo_getCampo()->hexagonos[BM_Player_getJogador()->hexagonoAtual].posicaoX;
	destinoY = (SPRITES(BM_IMG_PLAYER)->imagem->frame_altura / 6) + BM_Campo_getCampo()->hexagonos[BM_Player_getJogador()->hexagonoAtual].posicaoY;
	BM_Render_normal(SPRITES(BM_IMG_PLAYER), destinoX, destinoY, 0);
}
//==========================================================================

//==========================================================================
// Renderizar players
//==========================================================================
void BM_Render_player_ia() {
	int destinoX, destinoY;
	destinoX = (SPRITES(BM_IMG_PLAYER_IA)->imagem->frame_largura / 2) + BM_Campo_getCampo()->hexagonos[BM_Player_getIAPlayer()->hexagonoAtual].posicaoX;
	destinoY = (SPRITES(BM_IMG_PLAYER_IA)->imagem->frame_altura / 6) + BM_Campo_getCampo()->hexagonos[BM_Player_getIAPlayer()->hexagonoAtual].posicaoY;
	BM_Render_normal(SPRITES(BM_IMG_PLAYER_IA), destinoX, destinoY, 0);
}
//==========================================================================

//==========================================================================
// Renderizar Elementos
//==========================================================================
void BM_Render_elementos(void *_parametro) {
	int sourceW, sourceH, sourceX, destinoX, i, *elemento;
	al_draw_filled_rectangle(0, 0, BM_Janela_obter_dados().largura, BM_Janela_obter_dados().altura, al_map_rgba(0, 0, 0, 150));
	elemento = &BM_Player_getJogador()->elementosDisponivel.luz;
	sourceW = SPRITES(BM_IMG_ELEMENTOS)->imagem->frame_largura;
	sourceH = SPRITES(BM_IMG_ELEMENTOS)->imagem->frame_altura;
	for (i = 0; i <= 6; i++, elemento++) {
		sourceX = sourceW * i;
		destinoX = (196) + (sourceW * i);
		if(*elemento == TRUE)
			al_draw_scaled_bitmap(SPRITES(BM_IMG_ELEMENTOS)->Imagem, sourceX, 0, sourceW, sourceH, BM_AJUSTE_XW(destinoX), BM_AJUSTE_YH(266), SPRITES(BM_IMG_ELEMENTOS)->ajusteW, SPRITES(BM_IMG_ELEMENTOS)->ajusteH, 0);
		else
			al_draw_tinted_scaled_bitmap(SPRITES(BM_IMG_ELEMENTOS)->Imagem, al_map_rgb(100, 100, 100), sourceX, 0, sourceW, sourceH, BM_AJUSTE_XW(destinoX), BM_AJUSTE_YH(266), SPRITES(BM_IMG_ELEMENTOS)->ajusteW, SPRITES(BM_IMG_ELEMENTOS)->ajusteH, 0);
	}
	al_draw_textf(BM_Recursos_obter_fonte(BM_FONTE_ALBA_MENOR), al_map_rgb(255, 200, 200), BM_AJUSTE_XW(1300), BM_AJUSTE_YH(800), ALLEGRO_ALIGN_LEFT, "ESC - Voltar", BM_Rodada_get_restantes());
}
//==========================================================================

//==========================================================================
// Renderizar Rodada
//==========================================================================
void BM_Render_rodada(void *_parametro) {
	al_draw_textf(BM_Recursos_obter_fonte(BM_FONTE_ALBA_MENOR), al_map_rgb(225, 225, 225), BM_AJUSTE_XW(1000), BM_AJUSTE_YH(850), ALLEGRO_ALIGN_LEFT, "A - Atacar S - Adicionar T - Tutorial", BM_Rodada_get_restantes());
	al_draw_textf(BM_Recursos_obter_fonte(BM_FONTE_ALBA), al_map_rgb(255, 200, 200), BM_AJUSTE_XW(100), BM_AJUSTE_YH(10), ALLEGRO_ALIGN_CENTRE, "%d", BM_Rodada_get_restantes());
}
//==========================================================================

//==========================================================================
// Renderizar resultado
//==========================================================================
void BM_Render_resultado(void *_parametro) {
	int jogador = BM_Player_getJogador()->quantidadeTerritorio;
	int ia = BM_Player_getIAPlayer()->quantidadeTerritorio;
	char texto[10];
	al_draw_filled_rectangle(0, 0, BM_Janela_obter_dados().largura, BM_Janela_obter_dados().altura, al_map_rgba(0, 0, 0, 150));
	if (jogador > ia) strcpy(texto, "Vitoria");
	else if (jogador < ia) strcpy(texto, "Derrota");
	else strcpy(texto, "Empate");
	al_draw_textf(BM_Recursos_obter_fonte(BM_FONTE_ALBA), al_map_rgb(220, 220, 220), BM_AJUSTE_XW(800), BM_AJUSTE_YH(125), ALLEGRO_ALIGN_CENTRE, "%s", texto);
	al_draw_scaled_bitmap(SPRITES(BM_IMG_HEXAGONO)->Imagem, 238, 0, SPRITES(BM_IMG_HEXAGONO)->imagem->frame_largura, SPRITES(BM_IMG_HEXAGONO)->imagem->frame_altura, BM_AJUSTE_XW(323), BM_AJUSTE_YH(326), SPRITES(BM_IMG_HEXAGONO)->ajusteW, SPRITES(BM_IMG_HEXAGONO)->ajusteH, 0);
	al_draw_scaled_bitmap(SPRITES(BM_IMG_HEXAGONO)->Imagem, 476, 0, SPRITES(BM_IMG_HEXAGONO)->imagem->frame_largura, SPRITES(BM_IMG_HEXAGONO)->imagem->frame_altura, BM_AJUSTE_XW(1065), BM_AJUSTE_YH(326), SPRITES(BM_IMG_HEXAGONO)->ajusteW, SPRITES(BM_IMG_HEXAGONO)->ajusteH, 0);
	al_draw_textf(BM_Recursos_obter_fonte(BM_FONTE_ALBA), al_map_rgb(255, 255, 255), BM_AJUSTE_XW(435), BM_AJUSTE_YH(350), ALLEGRO_ALIGN_CENTRE, "%d", jogador);
	al_draw_textf(BM_Recursos_obter_fonte(BM_FONTE_ALBA), al_map_rgb(255, 255, 255), BM_AJUSTE_XW(1185), BM_AJUSTE_YH(350), ALLEGRO_ALIGN_CENTRE, "%d", ia);
	al_draw_text(BM_Recursos_obter_fonte(BM_FONTE_ALBA), al_map_rgb(255, 255, 255), BM_AJUSTE_XW(800), BM_AJUSTE_YH(700), ALLEGRO_ALIGN_CENTRE, "R - Reiniciar");
}
//==========================================================================

//==========================================================================
// Renderizar tutorial
//==========================================================================
void BM_Render_tutorial(void *_parametro) {
	al_draw_filled_rectangle(0, 0, BM_Janela_obter_dados().largura, BM_Janela_obter_dados().altura, al_map_rgba(0, 0, 0, 220));
	BM_Render_normal(BM_Recursos_obter_tutorial(TUTORIAL), 0, 0, 0);
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
// Reiniciar tutorial
//==========================================================================
void BM_Render_reiniciar_tutorial() {
	TUTORIAL = 0;
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
		aux->funcao(aux->parametro);
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

