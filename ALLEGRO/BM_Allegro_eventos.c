#include "BM_Allegro_eventos.h"
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
void BM_Eventos_teclado();
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
}
//==========================================================================

//==========================================================================
// Eventos do jogo
//==========================================================================
int BM_Eventos_processar() {
	int tem_eventos = al_wait_for_event_until(BM_Loop_filaEventos, &BM_Loop_evento, &BM_Loop_timeout);
	if (tem_eventos && BM_Loop_evento.type == ALLEGRO_EVENT_DISPLAY_CLOSE)	return FECHAR_JOGO;
	BM_Eventos_teclado();
	BM_Eventos_executarFilaFuncoes();
}
//==========================================================================

//==========================================================================
// Executar fila de funções
//==========================================================================
void BM_Eventos_executarFilaFuncoes() {
	BM_EVENTOS_FUNCAO *aux;
	if (BM_Eventos_obter_fila_funcao() != NULL) {
		for (aux = BM_Eventos_obter_fila_funcao()->inicio; aux != NULL; aux = aux->proximo) {
			aux->funcao();
		}
	}
}
//==========================================================================

//==========================================================================
// Processar eventos do teclado
//==========================================================================
void BM_Eventos_teclado() {
	if (BM_Loop_evento.type == ALLEGRO_EVENT_KEY_DOWN)
	{
		switch (BM_Loop_evento.keyboard.keycode) {
			case ALLEGRO_KEY_UP:
				BM_Animacao_adicionar(SPRITES(BM_IMG_ANI_AGUA), 50, 50, 0.5);
				break;
		}
	}
}
//==========================================================================