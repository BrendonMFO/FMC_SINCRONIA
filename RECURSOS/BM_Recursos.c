#include "BM_Recursos.h"
#include "BM_Recursos_Conteudo.h"
#include <stdio.h>

//==========================================================================
// Macros
//==========================================================================
#define NEW_SPRITE(CAMINHO,LINHA,COLUNA) BM_VETOR_SPRITES[indexSprite] = carregar_sprite((CAMINHO), (LINHA), (COLUNA))
#define NEW_FONT(FONTE,TAMANHO,FLAG) BM_Allegro_carregar_font(&BM_VETOR_FONTES[++indexFonte], (FONTE), (TAMANHO), (FLAG));
//==========================================================================

//==========================================================================
// Variaveis globais com os recursos disponiveis
//==========================================================================
BM_SPRITES *BM_VETOR_SPRITES[QUANTIDADE_SPRITES];
ALLEGRO_FONT *BM_VETOR_FONTES[QUANTIDADE_FONTES];
int indexSprite = -1;
int indexFonte = -1;
//==========================================================================

//==========================================================================
// Prototipos
//==========================================================================
int BM_Recursos_carregar_sprite();
int BM_Recursos_carregar_fonte();
BM_SPRITES *carregar_sprite(char *_arquivo, int _framesLinhas, int _framesColunas);
int checarArquivos();
//==========================================================================

//==========================================================================
// Obter sprite com os recursos visuais carregados
//==========================================================================
BM_SPRITES *BM_Recursos_obter_recurso(int _indice)
{
	return BM_VETOR_SPRITES[_indice];
}
//==========================================================================

//==========================================================================
// Obter fonte
//==========================================================================
ALLEGRO_FONT *BM_Recursos_obter_fonte(int _indice) {
	return BM_VETOR_FONTES[_indice];
}
//==========================================================================

//==========================================================================
// Carregar Sprites
//==========================================================================
int BM_Recursos_carregar_recursos()
{
	return
		BM_Recursos_carregar_sprite() +
		BM_Recursos_carregar_fonte();
}
//==========================================================================

//==========================================================================
// Carregar Sprites
//==========================================================================
int BM_Recursos_carregar_sprite() {
	NEW_SPRITE("Recursos\\Fases\\Mapa_01.bmp", 1, 1);
	NEW_SPRITE("Recursos\\Sprites\\Hexagono_1.png", 7, 3);
	NEW_SPRITE("Recursos\\Sprites\\Player.png", 1, 1);
	NEW_SPRITE("Recursos\\Sprites\\Hexagono_Alvo.png", 1, 5);
	NEW_SPRITE("Recursos\\Sprites\\Elementos.png", 1, 6);
	NEW_SPRITE("Recursos\\Sprites\\IA.png", 1, 1);
	NEW_SPRITE("Recursos\\Sprites\\Animacoes\\light_004.png", 5, 5);
	NEW_SPRITE("Recursos\\Sprites\\Animacoes\\darkness_002.png", 6, 5);
	NEW_SPRITE("Recursos\\Sprites\\Animacoes\\fire_002.png", 5, 5);
	NEW_SPRITE("Recursos\\Sprites\\Animacoes\\water_005.png", 6, 5);
	NEW_SPRITE("Recursos\\Sprites\\Animacoes\\earth_002.png", 4, 5);
	NEW_SPRITE("Recursos\\Sprites\\Animacoes\\wind_003.png", 6, 5);
	return checarArquivos();
}
//==========================================================================

//==========================================================================
// Carregar Fontes
//==========================================================================
int BM_Recursos_carregar_fonte() {
	NEW_FONT("Recursos\\Fontes\\ALBAS___.TTF", 100, 0);
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
		fprintf(stderr, "Erro ao carregar Sprite : %s\n", _arquivo);
		return NULL;
	}
	indexSprite++;
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
	for (i = 0; i < QUANTIDADE_FONTES; i++) {
		if (BM_VETOR_FONTES[i] == NULL)
			return ERRO;
	}
	return SUCESSO;
}
//==========================================================================
