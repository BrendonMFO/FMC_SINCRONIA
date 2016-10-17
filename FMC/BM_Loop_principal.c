#include "BM_Loop_principal.h"
#include "BM_Eventos.h"

//==========================================================================
// Macros
//==========================================================================
#define BOOL int
#define TRUE 1
#define FALSE 0
//==========================================================================

//==========================================================================
// Prototipos
//==========================================================================
void BM_Loop_iniciarTimer();
double BM_obterTempoTimer();
//==========================================================================

//==========================================================================
// Variavel para o controle do tempo de Atualização
//==========================================================================
double BM_Loop_tempoInicial = 0;
int BM_Loop_frame = 0;
BOOL BM_Loop_limitado = TRUE;
BOOL BM_Loop_finalizar = FALSE;
//==========================================================================

//==========================================================================
// Iniciar timer para travar o tempo de atualização
//==========================================================================
void BM_Loop_iniciarTimer()
{
	BM_Loop_tempoInicial = al_get_time();
}
//==========================================================================

//==========================================================================
// Obter tempo decorrido desde o inicio da aplicação 
//==========================================================================
double BM_obterTempoTimer()
{
	return al_get_time() - BM_Loop_tempoInicial;
}
//==========================================================================

//==========================================================================
// Loop principal
//==========================================================================
void BM_Loop(ALLEGRO_DISPLAY *_janela) {
	if (BM_Eventos_janela(_janela) == ERRO)
		return;
	BM_Loop_iniciarListenerEventos();
	while (!BM_Loop_finalizar) {
		BM_Loop_iniciarTimer();
		if (BM_Eventos_processar() == FECHAR_JOGO)
			BM_Loop_finalizar = TRUE;
		BM_Render_principal();
		BM_Loop_frame++;
		if (BM_Loop_limitado && (BM_obterTempoTimer() < 1.0 / FPS))
		{
			al_rest((1.0 / FPS) - BM_obterTempoTimer());
		}
	}
}
//==========================================================================

