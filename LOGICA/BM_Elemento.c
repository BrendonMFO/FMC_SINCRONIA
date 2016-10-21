#include "BM_Elemento.h"
#include "BM_Recursos.h"
#include "BM_Allegro_eventos_mouse.h"

//==========================================================================
// Adicionar elementos para a fila de eventos do mouse
//==========================================================================
void BM_Elemento_adicionar_mouse_listener() {
	int sourceW, sourceH, sourceX, destinoX, i;
	for (i = 0; i <= 6; i++) {
		int *id = (int*)malloc(1 * sizeof(int));
		*id = i;
		sourceW = BM_Allegro_largura_da_imagem(SPRITES(BM_IMG_ELEMENTOS)->Imagem) / SPRITES(BM_IMG_ELEMENTOS)->imagem->framesColunas;
		sourceH = BM_Allegro_altura_da_imagem(SPRITES(BM_IMG_ELEMENTOS)->Imagem) / SPRITES(BM_IMG_ELEMENTOS)->imagem->framesLinhas;
		sourceX = sourceW * i;
		destinoX = 196 + (sourceW * i);
		BM_Eventos_Mouse_adicionar(NULL, destinoX, 266, destinoX + sourceX, 266 + sourceH, id);
	}
}
//==========================================================================