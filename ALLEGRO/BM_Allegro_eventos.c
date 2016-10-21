#include "BM_Allegro_eventos.h"
#include "BM_Allegro_eventos_mouse.h"
#include "BM_Recursos_Animacao.h"
#include "BM_Recursos.h"

//==========================================================================
// Variaveis para controle de eventos
//==========================================================================
ALLEGRO_EVENT_QUEUE *BM_Loop_filaEventos;
ALLEGRO_EVENT BM_Loop_evento;
ALLEGRO_TIMEOUT BM_Loop_timeout;
//==========================================================================

//==========================================================================
// Prototipos
//==========================================================================
void BM_Eventos_iniciarListenerEventos();
void BM_Eventos_executarFilaFuncoes();
int BM_Eventos_janela(ALLEGRO_DISPLAY *_janela);
//==========================================================================

//==========================================================================
// Obter fila de eventos
//==========================================================================
ALLEGRO_EVENT_QUEUE *BM_Eventos_obter_fila() {
	return BM_Loop_filaEventos;
}
//==========================================================================

//==========================================================================
// Get eventos
//==========================================================================
ALLEGRO_EVENT BM_Eventos_obter_evento() {
	return BM_Loop_evento;
}
//==========================================================================

//==========================================================================
// Iniciar eventos
//==========================================================================
int BM_Eventos_iniciar(ALLEGRO_DISPLAY *_janela)
{
	if (BM_Eventos_janela(_janela) == ERRO)
		return ERRO;
	BM_Eventos_iniciarListenerEventos();
	return SUCESSO;
}
//==========================================================================

//==========================================================================
// Iniciar fila de eventos da aplicação
//==========================================================================
int BM_Eventos_janela(ALLEGRO_DISPLAY *_janela) {
	if (BM_Allegro_criar_eventos(&BM_Loop_filaEventos) == ERRO) {
		fprintf(stderr, "ERRO: Nao foi possivel iniciar o listener de eventos da janela\n");
		return ERRO;
	}
	al_register_event_source(BM_Loop_filaEventos, al_get_display_event_source(_janela));
	return SUCESSO;
}
//==========================================================================

//==========================================================================
// Iniciar variaveis responsaveis por escutar os eventos do jogo
//==========================================================================
void BM_Eventos_iniciarListenerEventos() {
	al_init_timeout(&BM_Loop_timeout, 0.05);
	al_register_event_source(BM_Loop_filaEventos, al_get_keyboard_event_source());
	al_register_event_source(BM_Loop_filaEventos, al_get_mouse_event_source());
}
//==========================================================================

//==========================================================================
// Eventos do jogo
//==========================================================================
int BM_Eventos_processar() {
	int tem_eventos = al_wait_for_event_until(BM_Loop_filaEventos, &BM_Loop_evento, &BM_Loop_timeout);
	if (tem_eventos && BM_Loop_evento.type == ALLEGRO_EVENT_DISPLAY_CLOSE)	return FECHAR_JOGO;
	if (tem_eventos) BM_Eventos_executarFilaFuncoes();
	return LOOP;
}
//==========================================================================

//==========================================================================
// Executar fila de funções
//==========================================================================
void BM_Eventos_executarFilaFuncoes() {
	BM_EVENTOS_FUNCAO *aux, *aux2;
	if (BM_Eventos_obter_fila_funcao() != NULL) {
		for (aux = BM_Eventos_obter_fila_funcao()->inicio; aux != NULL;) {
			aux->funcao();
			if (aux->ativo == INATIVO) {
				if (aux->proximo == NULL) aux = NULL;
				else {
					aux2 = aux->proximo;
					free(aux);
					aux = aux2;
				}
			}
			else aux = aux->proximo;
		}
	}
}
//==========================================================================
