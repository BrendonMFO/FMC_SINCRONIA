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
	if (BM_FMC_carregar_recursos() == ERRO)
		return ERRO;
	if (BM_FMC_carregar_campo() == ERRO)
		return ERRO;
	if (BM_Player_iniciar(0, BM_Campo_getCampo()->quantidade - 1) == ERRO)
		return ERRO;
	return SUCESSO;
}
//==========================================================================

//==========================================================================
// Carregar Recursos
//==========================================================================
int BM_FMC_carregar_recursos() {
	if (BM_Allegro_iniciar() == ERRO) {
		return ERRO;
	}
	if (BM_Recursos_carregar_Sprites() == ERRO) {
		return ERRO;
	}
	return SUCESSO;
}
//==========================================================================

//==========================================================================
// Carregar informações do campo
//==========================================================================
int BM_FMC_carregar_campo() {
	BM_Campo_setCampo("MAPA.gf");
	if (BM_Campo_getCampo() == NULL) {
		return ERRO;
	}
	return SUCESSO;
}
//==========================================================================
