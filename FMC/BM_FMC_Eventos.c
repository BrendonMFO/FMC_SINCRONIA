#include "BM_FMC_Eventos.h"
#include "BM_Campo.h"
#include "BM_Recursos.h"
#include "BM_Renders.h"
#include "BM_Elemento.h"

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
	BM_Eventos_Funcoes_adicionar(BM_Evento_jogador);
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
			switch (BM_Campo_getCampo()->hexagonos[BM_Player_getJogador()->hexagonoAtual].estado) {
			case JOGADOR:
				if (BM_Hexagono_marcar_alvos(BM_Player_getJogador()->hexagonoAtual, MARCAR) > 0) {
					BM_Eventos_Funcoes_remover(BM_Evento_jogador);
					BM_Eventos_Funcoes_adicionar(BM_Hexagono_alvo);
				}
				break;
			case NEUTRO:
				BM_Elemento_adicionar_mouse_listener();
				BM_Render_adicionar_funcao(BM_Render_elementos);
				BM_Eventos_Funcoes_remover(BM_Evento_jogador);
				BM_Eventos_Funcoes_adicionar(BM_Elemento_escolha);
				break;
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
	int id;
	switch (aux.type) {
	case ALLEGRO_EVENT_MOUSE_BUTTON_DOWN:
		 mouse = BM_Eventos_Mouse_processar(aux.mouse.x, aux.mouse.y);
		 if (mouse != NULL) {
			 id = *(int*)mouse->opcional;
			 switch (BM_Hexagono_batalha(id, BM_Player_getJogador()->hexagonoAtual))
			 {
			 case VITORIA_ATAQUE:
				 BM_Hexagono_marcar_alvos(BM_Player_getJogador()->hexagonoAtual, DESMARCAR);
				 campo->hexagonos[id].estado = JOGADOR;
				 BM_Player_getJogador()->hexagonoAtual = id;
				 BM_Player_getJogador()->quantidadeTerritorio++;
				 BM_Player_getIAPlayer()->ia->quantidadeTerritorio--;
				 break;
			 case VITORIA_DEFESA:
				 BM_Hexagono_marcar_alvos(BM_Player_getJogador()->hexagonoAtual, DESMARCAR);
				 campo->hexagonos[BM_Player_getJogador()->hexagonoAtual].estado = ADVERSARIO;
				 BM_Player_getJogador()->quantidadeTerritorio--;
				 BM_Player_getIAPlayer()->ia->quantidadeTerritorio++;
				 break;
			 case EMPATE:
				 BM_Hexagono_marcar_alvos(BM_Player_getJogador()->hexagonoAtual, DESMARCAR);
				 break;
			 }
			 BM_Eventos_Funcoes_remover(BM_Hexagono_alvo);
			 BM_Eventos_Funcoes_adicionar(BM_Evento_jogador);
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
	switch (aux.type) {
	case ALLEGRO_EVENT_MOUSE_BUTTON_DOWN:
		break;
	case ALLEGRO_EVENT_KEY_DOWN:
		switch (aux.keyboard.keycode) {
		case ALLEGRO_KEY_ESCAPE:
			BM_Render_remover_funcao(BM_Render_elementos);
			BM_Eventos_Funcoes_remover(BM_Elemento_escolha);
			BM_Eventos_Funcoes_adicionar(BM_Evento_jogador);
			break;
		}
		break;
	}
}
//==========================================================================
