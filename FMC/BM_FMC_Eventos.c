#include "BM_FMC_Eventos.h"
#include "BM_Campo.h"
#include "BM_Allegro_eventos_mouse.h"
#include "BM_Recursos.h"
#include "BM_Renders.h"
#include "BM_Elemento.h"
#include "BM_Player.h"
#include "BM_Player_IA.h"
#include "BM_Recursos_Animacao.h"
#include "BM_Rodadas.h"
#include "BM_Hexagono.h"

//==========================================================================
// Prototipos
//==========================================================================
void BM_Evento_tutorial(void *_parametro);
void BM_Evento_jogador(void *_parametro);
void BM_Evento_alvo(void *_parametro);
void BM_Evento_escolha(void *_parametro);
void BM_Evento_reiniciar(void *_parametro);
void BM_Iniciar();
//==========================================================================

//==========================================================================
// Iniciar jogo
//==========================================================================
void BM_Iniciar() {
	BM_Rodada_set(50);
	BM_Player_iniciar_valores(0);
	BM_Render_remover_funcao(BM_Render_tutorial);
	BM_Eventos_Funcoes_adicionar(BM_Evento_jogador, NULL);
	BM_Render_adicionar_funcao(BM_Render_rodada);
}
//==========================================================================

//==========================================================================
// Evento inicial
//==========================================================================
void BM_FMC_Evento_inicial() {
	BM_Eventos_Funcoes_adicionar(BM_Evento_tutorial, NULL);
	BM_Render_adicionar_funcao(BM_Render_tutorial);
}
//==========================================================================

//==========================================================================
// Verificar se o jogo possue um vencedor
//==========================================================================
void BM_Jogo_verificar_fim() {
	int a = BM_Rodada_get_restantes();
	if (BM_Rodada_get_restantes() == 0 || 
		BM_Player_getJogador()->quantidadeTerritorio == 0 ||
		BM_Player_getIAPlayer()->quantidadeTerritorio == 0) {
		BM_Render_adicionar_funcao(BM_Render_resultado);
		BM_Eventos_Funcoes_remover(BM_Evento_jogador);
		BM_Eventos_Funcoes_adicionar(BM_Evento_reiniciar, NULL);
	}
}
//==========================================================================

//==========================================================================
// Eventos disparados na tela de tutorial
//==========================================================================
void BM_Evento_tutorial(void *_parametro) {
	ALLEGRO_EVENT aux = BM_Eventos_obter_evento();
	switch (aux.type) {
	case ALLEGRO_EVENT_KEY_DOWN:
		switch (aux.keyboard.keycode) {
		case ALLEGRO_KEY_I:
			BM_Iniciar();
			break;
		case ALLEGRO_KEY_ENTER:
			if (BM_Render_obter_tutorial() == 9)
				BM_Iniciar();
			else
				BM_Render_avancar_tutorial();
			break;
		}
	}
}
//==========================================================================

//==========================================================================
// Eventos disparados pelo jogador
//==========================================================================
void BM_Evento_jogador(void *_parametro) {
	ALLEGRO_EVENT aux = BM_Eventos_obter_evento();
	if (BM_IA_get_executando() == FALSE) {
		switch (aux.type) {
		case ALLEGRO_EVENT_MOUSE_BUTTON_DOWN:
			BM_Player_mover(BM_Player_getJogador(), aux.mouse.x, aux.mouse.y);
			break;
		case ALLEGRO_EVENT_KEY_DOWN:
			switch (aux.keyboard.keycode) {
			case ALLEGRO_KEY_A:
				if (BM_Campo_getCampo()->hexagonos[BM_Player_getJogador()->hexagonoAtual].estado == JOGADOR) {
					if (BM_Hexagono_marcar_alvos(BM_Player_getJogador()->hexagonoAtual, HEXAGONO_ALVO) > 0) {
						BM_Hexagono_marcar_sincronia();
						BM_Eventos_Funcoes_remover(BM_Evento_jogador);
						BM_Eventos_Funcoes_adicionar(BM_Evento_alvo, NULL);
					}
				}
				break;
			case ALLEGRO_KEY_S:
				if (BM_Campo_getCampo()->hexagonos[BM_Player_getJogador()->hexagonoAtual].estado != ADVERSARIO) {
					BM_Elemento_adicionar_mouse_listener();
					BM_Render_adicionar_funcao(BM_Render_elementos);
					BM_Eventos_Funcoes_remover(BM_Evento_jogador);
					BM_Eventos_Funcoes_adicionar(BM_Evento_escolha, NULL);
				}
				break;
			}
			break;
		}
	}
}
//==========================================================================

//==========================================================================
// Eventos quando a opção de ataque esta selecionada
//==========================================================================
void BM_Evento_alvo(void *_parametro) {
	ALLEGRO_EVENT aux = BM_Eventos_obter_evento();
	BM_EVENTO_MOUSE *mouse;
	BM_Campo *campo = BM_Campo_getCampo();
	BM_HEXAGONO *hexagono;
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
				 BM_Hexagono_marcar_alvos(BM_Player_getJogador()->hexagonoAtual, HEXAGONO_NORMAL);
				 BM_Hexagono_desmarcar_sincronia();
				 campo->hexagonos[id].estado = JOGADOR;
				 BM_Player_getJogador()->hexagonoAtual = id;
				 BM_Player_getJogador()->quantidadeTerritorio++;
				 BM_Player_getIAPlayer()->quantidadeTerritorio--;
				 break;
			 case VITORIA_DEFESA:
				 BM_Hexagono_marcar_alvos(BM_Player_getJogador()->hexagonoAtual, HEXAGONO_NORMAL);
				 BM_Hexagono_desmarcar_sincronia();
				 campo->hexagonos[BM_Player_getJogador()->hexagonoAtual].estado = ADVERSARIO;
				 BM_Player_getJogador()->quantidadeTerritorio--;
				 BM_Player_getIAPlayer()->quantidadeTerritorio++;
				 break;
			 case EMPATE:
				 BM_Hexagono_marcar_alvos(BM_Player_getJogador()->hexagonoAtual, HEXAGONO_NORMAL);
				 BM_Hexagono_desmarcar_sincronia();
				 break;
			 }
			 BM_Eventos_Funcoes_remover(BM_Evento_alvo);
			 BM_Eventos_Funcoes_adicionar(BM_Evento_jogador, NULL);
			 BM_IA_disparar();
		 }
		break;
	case ALLEGRO_EVENT_KEY_DOWN:
		switch (aux.keyboard.keycode) {
		case ALLEGRO_KEY_A:
			BM_Hexagono_marcar_alvos(BM_Player_getJogador()->hexagonoAtual, HEXAGONO_NORMAL);
			BM_Hexagono_desmarcar_sincronia();
			BM_Eventos_Funcoes_remover(BM_Evento_alvo);
			BM_Eventos_Funcoes_adicionar(BM_Evento_jogador, NULL);
			break;
		}
		break;
	}
}
//==========================================================================

//==========================================================================
// Eventos na tela de seleção de elementos
//==========================================================================
void BM_Evento_escolha(void *_parametro) {
	ALLEGRO_EVENT aux = BM_Eventos_obter_evento();
	BM_Campo *campo = BM_Campo_getCampo();
	BM_EVENTO_MOUSE *mouse;
	int elemento;
	switch (aux.type) {
	case ALLEGRO_EVENT_MOUSE_BUTTON_DOWN:
		mouse = BM_Eventos_Mouse_processar(aux.mouse.x, aux.mouse.y);
		if (mouse != NULL) {
			elemento = *(int*)mouse->opcional;
			if (BM_Player_disponibilidade_elemento(elemento) == TRUE) {
				if (campo->hexagonos[BM_Player_getJogador()->hexagonoAtual].estado == NEUTRO)
					BM_Player_getJogador()->quantidadeTerritorio++;
				BM_Player_checar_elemento(elemento);
				campo->hexagonos[BM_Player_getJogador()->hexagonoAtual].elemento = elemento;
				campo->hexagonos[BM_Player_getJogador()->hexagonoAtual].estado = JOGADOR;
				BM_Elemento_remover_mouse_listener();
				BM_Render_remover_funcao(BM_Render_elementos);
				BM_Eventos_Funcoes_remover(BM_Evento_escolha);
				BM_Eventos_Funcoes_adicionar(BM_Evento_jogador, NULL);
				BM_IA_disparar();
			}
		}
		break;
	case ALLEGRO_EVENT_KEY_DOWN:
		switch (aux.keyboard.keycode) {
		case ALLEGRO_KEY_ESCAPE:
			BM_Elemento_remover_mouse_listener();
			BM_Render_remover_funcao(BM_Render_elementos);
			BM_Eventos_Funcoes_remover(BM_Evento_escolha);
			BM_Eventos_Funcoes_adicionar(BM_Evento_jogador, NULL);
			break;
		}
		break;
	}
}
//==========================================================================

//==========================================================================
// Reiniciar jogo
//==========================================================================
void BM_Evento_reiniciar(void *_parametro) {
	ALLEGRO_EVENT aux = BM_Eventos_obter_evento();
	switch (aux.type) {
	case ALLEGRO_EVENT_MOUSE_BUTTON_DOWN:
		break;
	case ALLEGRO_EVENT_KEY_DOWN:
		switch (aux.keyboard.keycode) {
		case ALLEGRO_KEY_R:
			BM_Campo_redefinir();
			BM_IA_iniciar_valores(BM_Campo_getCampo()->quantidade - 1);
			BM_Eventos_Funcoes_remover(BM_Evento_reiniciar);
			BM_Render_remover_funcao(BM_Render_resultado);
			BM_FMC_Evento_inicial();
			break;
		}
		break;
	}
}
//==========================================================================