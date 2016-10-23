#include "BM_FMC_Eventos.h"
#include "BM_Campo.h"
#include "BM_Recursos.h"
#include "BM_Renders.h"
#include "BM_Elemento.h"
#include "BM_Player_IA.h"
#include "BM_Recursos_Animacao.h"
#include "BM_Rodadas.h"

//==========================================================================
// Prototipos
//==========================================================================
void BM_Evento_jogador(void);
void BM_Hexagono_alvo(void);
void BM_Elemento_escolha(void);
//==========================================================================

//==========================================================================
// Evento inicial
//==========================================================================
void BM_FMC_Evento_inicial() {
	BM_Rodada_set(50);
	BM_Eventos_Funcoes_adicionar(BM_Evento_jogador);
	BM_Render_adicionar_funcao(BM_Render_rodada);
}
//==========================================================================

//==========================================================================
// Eventos disparados pelo jogador
//==========================================================================
void BM_Evento_jogador(void) {
	ALLEGRO_EVENT aux = BM_Eventos_obter_evento();
	switch (aux.type) {
	case ALLEGRO_EVENT_MOUSE_BUTTON_DOWN:
		BM_Player_mover(BM_Player_getJogador(), aux.mouse.x, aux.mouse.y);
		break;
	case ALLEGRO_EVENT_KEY_DOWN:
		switch (aux.keyboard.keycode) {
		case ALLEGRO_KEY_A:
			if (BM_Campo_getCampo()->hexagonos[BM_Player_getJogador()->hexagonoAtual].estado == JOGADOR) {
				if (BM_Hexagono_marcar_alvos(BM_Player_getJogador()->hexagonoAtual, MARCAR) > 0) {
					BM_Eventos_Funcoes_remover(BM_Evento_jogador);
					BM_Eventos_Funcoes_adicionar(BM_Hexagono_alvo);
				}
			}
			break;
		case ALLEGRO_KEY_S:
			if (BM_Campo_getCampo()->hexagonos[BM_Player_getJogador()->hexagonoAtual].estado != ADVERSARIO) {
				BM_Elemento_adicionar_mouse_listener();
				BM_Render_adicionar_funcao(BM_Render_elementos);
				BM_Eventos_Funcoes_remover(BM_Evento_jogador);
				BM_Eventos_Funcoes_adicionar(BM_Elemento_escolha);
			}
			break;
		}
		break;
	}
}
//==========================================================================

//==========================================================================
// Eventos quando a opção de ataque esta selecionada
//==========================================================================
void BM_Hexagono_alvo(void) {
	ALLEGRO_EVENT aux = BM_Eventos_obter_evento();
	BM_EVENTO_MOUSE *mouse;
	BM_Campo *campo = BM_Campo_getCampo();
	BM_Hexagono *hexagono;
	int id;
	switch (aux.type) {
	case ALLEGRO_EVENT_MOUSE_BUTTON_DOWN:
		 mouse = BM_Eventos_Mouse_processar(aux.mouse.x, aux.mouse.y);
		 if (mouse != NULL) {
			 id = *(int*)mouse->opcional;
			 hexagono = &campo->hexagonos[BM_Player_getJogador()->hexagonoAtual];
			 switch (BM_Hexagono_batalha(id, BM_Player_getJogador()->hexagonoAtual))
			 {
			 case VITORIA_ATAQUE:
				 BM_Hexagono_marcar_alvos(BM_Player_getJogador()->hexagonoAtual, DESMARCAR);
				 campo->hexagonos[id].estado = JOGADOR;
				 BM_Player_getJogador()->hexagonoAtual = id;
				 BM_Player_getJogador()->quantidadeTerritorio++;
				 BM_Player_getIAPlayer()->quantidadeTerritorio--;
				 break;
			 case VITORIA_DEFESA:
				 BM_Hexagono_marcar_alvos(BM_Player_getJogador()->hexagonoAtual, DESMARCAR);
				 campo->hexagonos[BM_Player_getJogador()->hexagonoAtual].estado = ADVERSARIO;
				 BM_Player_getJogador()->quantidadeTerritorio--;
				 BM_Player_getIAPlayer()->quantidadeTerritorio++;
				 break;
			 case EMPATE:
				 BM_Hexagono_marcar_alvos(BM_Player_getJogador()->hexagonoAtual, DESMARCAR);
				 break;
			 }
			 BM_Eventos_Funcoes_remover(BM_Hexagono_alvo);
			 BM_Eventos_Funcoes_adicionar(BM_Evento_jogador);
			 BM_IA_disparar();
		 }
		break;
	case ALLEGRO_EVENT_KEY_DOWN:
		switch (aux.keyboard.keycode) {
		case ALLEGRO_KEY_A:
			BM_Hexagono_marcar_alvos(BM_Player_getJogador()->hexagonoAtual, DESMARCAR);
			BM_Eventos_Funcoes_remover(BM_Hexagono_alvo);
			BM_Eventos_Funcoes_adicionar(BM_Evento_jogador);
			break;
		}
		break;
	}
}
//==========================================================================

//==========================================================================
// Eventos na tela de seleção de elementos
//==========================================================================
void BM_Elemento_escolha(void) {
	ALLEGRO_EVENT aux = BM_Eventos_obter_evento();
	BM_Campo *campo = BM_Campo_getCampo();
	BM_EVENTO_MOUSE *mouse;
	int elemento;
	switch (aux.type) {
	case ALLEGRO_EVENT_MOUSE_BUTTON_DOWN:
		mouse = BM_Eventos_Mouse_processar(aux.mouse.x, aux.mouse.y);
		if (mouse != NULL) {
			elemento = *(int*)mouse->opcional;
			campo->hexagonos[BM_Player_getJogador()->hexagonoAtual].elemento = elemento;
			campo->hexagonos[BM_Player_getJogador()->hexagonoAtual].estado = JOGADOR;
			BM_Elemento_remover_mouse_listener();
			BM_Render_remover_funcao(BM_Render_elementos);
			BM_Eventos_Funcoes_remover(BM_Elemento_escolha);
			BM_Eventos_Funcoes_adicionar(BM_Evento_jogador);
			BM_IA_disparar();
		}
		break;
	case ALLEGRO_EVENT_KEY_DOWN:
		switch (aux.keyboard.keycode) {
		case ALLEGRO_KEY_ESCAPE:
			BM_Elemento_remover_mouse_listener();
			BM_Render_remover_funcao(BM_Render_elementos);
			BM_Eventos_Funcoes_remover(BM_Elemento_escolha);
			BM_Eventos_Funcoes_adicionar(BM_Evento_jogador);
			break;
		}
		break;
	}
}
//==========================================================================
