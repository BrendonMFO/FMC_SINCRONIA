#include "BM_FMC_Eventos.h"
#include "BM_Campo.h"

//==========================================================================
// Prototipos
//==========================================================================
void BM_Evento_jogador(void);
void BM_Hexagono_alvo(void);
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
void BM_Evento_jogador() {
	ALLEGRO_EVENT aux = BM_Eventos_obter_evento();
	switch (aux.type) {
	case ALLEGRO_EVENT_MOUSE_BUTTON_DOWN:
		BM_Player_mover(BM_Player_getJogador(), aux.mouse.x, aux.mouse.y);
		break;
	case ALLEGRO_EVENT_KEY_DOWN:
		switch (aux.keyboard.keycode) {
		case ALLEGRO_KEY_A:
			if (BM_Hexagono_marcar_alvos(BM_Player_getJogador()->hexagonoAtual, MARCAR) > 0) {
				BM_Eventos_Funcoes_remover(BM_Evento_jogador);
				BM_Eventos_Funcoes_adicionar(BM_Hexagono_alvo);
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
void BM_Hexagono_alvo() {
	ALLEGRO_EVENT aux = BM_Eventos_obter_evento();
	BM_EVENTO_MOUSE *mouse;
	BM_Campo *campo = BM_Campo_getCampo();
	int id;
	switch (aux.type) {
	case ALLEGRO_EVENT_MOUSE_BUTTON_DOWN:
		 mouse = BM_Eventos_Mouse_processar(aux.mouse.x, aux.mouse.y);
		 if (mouse != NULL) {
			 sscanf(mouse->opcional, "%d", &id);
			 switch (BM_Hexagono_batalha(id, BM_Player_getJogador()->hexagonoAtual))
			 {
			 case VITORIA_ATAQUE:
				 BM_Hexagono_marcar_alvos(BM_Player_getJogador()->hexagonoAtual, DESMARCAR);
				 campo->hexagonos[id].estado = JOGADOR;
				 BM_Player_getJogador()->hexagonoAtual = id;
				 break;
			 case VITORIA_DEFESA:
				 BM_Hexagono_marcar_alvos(BM_Player_getJogador()->hexagonoAtual, DESMARCAR);
				 campo->hexagonos[BM_Player_getJogador()->hexagonoAtual].estado = ADVERSARIO;
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