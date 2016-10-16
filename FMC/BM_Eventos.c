#include "BM_Eventos.h"

//==========================================================================
// Variaveis para controle de eventos
//==========================================================================
ALLEGRO_EVENT_QUEUE *BM_Loop_filaEventos;
ALLEGRO_EVENT BM_Loop_evento;
ALLEGRO_TIMEOUT BM_Loop_timeout;
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
void BM_Loop_iniciarListenerEventos() {
	al_init_timeout(&BM_Loop_timeout, 0.05);
}
//==========================================================================

//==========================================================================
// Eventos do jogo
//==========================================================================
int BM_Eventos_processar() {
	int tem_eventos = al_wait_for_event_until(BM_Loop_filaEventos, &BM_Loop_evento, &BM_Loop_timeout);
	if (tem_eventos && BM_Loop_evento.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
	{
		return FECHAR_JOGO;
	}
}
//==========================================================================