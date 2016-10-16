
#include "BM_Allegro.h"
#include "BM_Allegro_Imagens.h"
#include "BM_FMC_Carregar.h"
#include "BM_Loop_principal.h"

int main()
{
	//==========================================================================
	// Variaveis
	//==========================================================================
	ALLEGRO_DISPLAY *janela = NULL;
	//==========================================================================

	//==========================================================================
	// Carregar dados
	//==========================================================================
	if (BM_FMC_carregar() == ERRO) {
		return -1;
	}
	//==========================================================================

	//==========================================================================
	// Criar e configurar janela
	//==========================================================================
	BM_Allegro_criar_janela(&janela, 1600, 920);
	al_clear_to_color(al_map_rgb(255, 255, 255));
	//==========================================================================

	//==========================================================================
	// Loop principal
	//==========================================================================
	BM_Loop(janela);
	//==========================================================================

	al_destroy_display(janela);

	return 0;

}
