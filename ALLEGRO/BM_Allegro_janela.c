#include "BM_Allegro_janela.h"

//==========================================================================
// Variaveis
//==========================================================================
BM_JANELA dados_janela;
//==========================================================================

//==========================================================================
// Obter dados da janela
//==========================================================================
BM_JANELA BM_Janela_obter_dados() {
	return dados_janela;
}
//==========================================================================

//==========================================================================
// Setar dados da janela
//==========================================================================
void BM_Janela_setar_dados(int _largura, int _altura) {
	dados_janela.largura = _largura;
	dados_janela.altura = _altura;
}
//==========================================================================