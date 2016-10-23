#include "BM_FMC_Carregar.h"

//==========================================================================
// Prototipos
//==========================================================================
int BM_FMC_carregar_recursos();
int BM_FMC_carregar_campo();
//==========================================================================

//==========================================================================
// Carregar dados
//==========================================================================
int BM_FMC_carregar() {
	return
		BM_FMC_carregar_recursos() &&
		BM_FMC_carregar_campo() &&
		BM_Player_iniciar(0) &&
		BM_Player_IA_iniciar(BM_Campo_getCampo()->quantidade - 1);
}
//==========================================================================

//==========================================================================
// Carregar Recursos
//==========================================================================
int BM_FMC_carregar_recursos() {
	return
		BM_Allegro_iniciar() &&
		BM_Recursos_carregar_recursos() &&
		BM_Eventos_iniciar_fila_funcoes() &&
		BM_Eventos_Mouse_iniciar_fila_mouse() &&
		BM_Animacao_iniciar_fila() &&
		BM_Render_iniciar_fila();
}
//==========================================================================

//==========================================================================
// Carregar informações do campo
//==========================================================================
int BM_FMC_carregar_campo() {
	BM_Campo_setCampo("Recursos\\Fases\\Mapa_01.gf");
	if (BM_Campo_getCampo() == NULL) {
		return ERRO;
	}
	return SUCESSO;
}
//==========================================================================
