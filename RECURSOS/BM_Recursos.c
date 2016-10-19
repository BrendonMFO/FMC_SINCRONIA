#include "BM_Recursos.h"
#include "BM_Recursos_Sprites.h"
#include <stdio.h>

//==========================================================================
// Variaveis globais com os recursos disponiveis
//==========================================================================
BM_SPRITES *BM_VETOR_SPRITES[QUANTIDADE_SPRITES];
//==========================================================================

//==========================================================================
// Prototipos
//==========================================================================
BM_SPRITES *carregar_sprite(char *_arquivo, int _framesLinhas, int _framesColunas);
int checarArquivos();
//==========================================================================

//==========================================================================
// Obter array com os recursos visuais carregados
//==========================================================================
BM_SPRITES *BM_Recursos_obter_recurso(int _indice)
{
	return BM_VETOR_SPRITES[_indice];
}
//==========================================================================

//==========================================================================
// Carregar Sprites
//==========================================================================
int BM_Recursos_carregar_Sprites()
{
	BM_VETOR_SPRITES[0] = carregar_sprite("Mapa_01.bmp", 1, 1);
	BM_VETOR_SPRITES[1] = carregar_sprite("Hexagono_1.png", 7, 3);
	BM_VETOR_SPRITES[2] = carregar_sprite("Recursos\\Sprites\\Animacoes\\water_005.png", 6, 5);
	return checarArquivos();
}
//==========================================================================

//==========================================================================
// Carregar Sprites
//==========================================================================
BM_SPRITES *carregar_sprite(char *_arquivo, int _framesLinhas, int _framesColunas) {
	BM_SPRITES *temp = (BM_SPRITES*)malloc(1 * sizeof(BM_SPRITES));
	temp->imagem = BM_Allegro_carregar_imagem(_arquivo, _framesLinhas, _framesColunas);
	if (temp->imagem == NULL) {
		fprintf(stderr, "Erro ao carregar Sprite : %s", _arquivo);
		return NULL;
	}
	return temp;
}
//==========================================================================

//==========================================================================
// Checar se todos os arquivos foram carregados
//==========================================================================
int checarArquivos() {
	int i;
	for (i = 0; i < QUANTIDADE_SPRITES; i++) {
		if (BM_VETOR_SPRITES[i] == NULL)
			return ERRO;
	}
	return SUCESSO;
}
//==========================================================================
