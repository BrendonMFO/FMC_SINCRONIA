#include "BM_Player.h"
#include "BM_Allegro_eventos.h"
#include "BM_Allegro_eventos_funcoes.h"
#include "BM_Campo.h"
#include "BM_Recursos.h"
#include <stdio.h>
#include <stdlib.h>

//==========================================================================
// Constantes
//==========================================================================
#define ERRO 0
#define SUCESSO 1
//==========================================================================

//==========================================================================
// Variaveis
//==========================================================================
BM_PLAYER *jogador = NULL;
BM_PLAYER *ia = NULL;
//==========================================================================

//==========================================================================
// Prototipos
//==========================================================================
void iniciarValores(BM_PLAYER *_player, int _hexagono);
void BM_Player_mover(BM_PLAYER *_player, int _mouseX, int _mouseY);
void BM_Player_eventos();
//==========================================================================

//==========================================================================
// Iniciar player
//==========================================================================
int BM_Player_iniciar(int _hexagonoInicialJogador, int _hexagonoInicialIA)
{
	//======================================================================
	// Alocar memoria para o player do jogador
	//======================================================================
	jogador = (BM_PLAYER*)malloc(1 * sizeof(BM_PLAYER));
	if (jogador == NULL) {
		fprintf(stderr, "ERRO: Nao foi possivel alocar memoria para um player");
		return ERRO;
	}
	iniciarValores(jogador, _hexagonoInicialJogador);
	BM_Eventos_Funcoes_adicionar(BM_Player_eventos);
	//======================================================================

	//======================================================================
	// Alocar memoria para o player da IA
	//======================================================================
	ia = (BM_PLAYER*)malloc(1 * sizeof(BM_PLAYER));
	if (ia == NULL) {
		fprintf(stderr, "ERRO: Nao foi possivel alocar memoria para um player");
		return ERRO;
	}
	iniciarValores(ia, _hexagonoInicialIA);
	//======================================================================

	return SUCESSO;
}
//==========================================================================

//==========================================================================
// Get jogador
//==========================================================================
BM_PLAYER *BM_Player_getJogador() {
	return jogador;
}
//==========================================================================

//==========================================================================
// Get IA
//==========================================================================
BM_PLAYER *BM_Player_getIAPlayer() {
	return ia;
}
//==========================================================================

//==========================================================================
// Iniciar valores da estrutura de jogadores
//==========================================================================
void iniciarValores(BM_PLAYER *_player, int _hexagono) {
	_player->hexagonoAtual = _hexagono;
	_player->quantidadeTerritorio = 0;
	_player->elementosQuantidade.luz = _player->elementosNivel.luz = 0;
	_player->elementosQuantidade.trevas = _player->elementosNivel.trevas = 0;
	_player->elementosQuantidade.fogo = _player->elementosNivel.fogo = 0;
	_player->elementosQuantidade.agua = _player->elementosNivel.agua = 0;
	_player->elementosQuantidade.terra = _player->elementosNivel.terra = 0;
	_player->elementosQuantidade.ar = _player->elementosNivel.ar = 0;
}
//==========================================================================

//==========================================================================
// Alterar hexagono que o player esta atualmente
//==========================================================================
void BM_Player_mover(BM_PLAYER *_player, int _mouseX, int _mouseY) {
	BM_Campo *campo = BM_Campo_getCampo();
	BM_Hexagono *hexagono;
	int i, largura, altura;
	for (i = 0; i < campo->quantidade; i++) {
		hexagono = &campo->hexagonos[i];
		largura = BM_Allegro_largura_da_imagem(SPRITES(BM_IMG_HEXAGONO)) / 2;
		altura = BM_Allegro_altura_da_imagem(SPRITES(BM_IMG_HEXAGONO)) / 2;
		if (_mouseX >= 800 - largura &&
			_mouseX <= 460 + largura &&
			_mouseY >= 800 - altura &&
			_mouseY <= 460 + altura) {
			break;
		}
	}

}
//==========================================================================

//==========================================================================
// Eventos de teclado do jogador
//==========================================================================
void BM_Player_eventos() {
	ALLEGRO_EVENT aux = BM_Eventos_obter_evento();
	if (aux.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP) {
		BM_Player_mover(jogador, aux.mouse.x, aux.mouse.y);
	}
}
//==========================================================================