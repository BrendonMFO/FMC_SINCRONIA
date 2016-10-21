#include "BM_Allegro_eventos_mouse.h"
#include <string.h>

//==========================================================================
// Variaveis
//==========================================================================
BM_EVENTO_MOUSE_FILA *BM_Fila_eventos_mouse = NULL;
//==========================================================================

//==========================================================================
// Prototipos
//==========================================================================
BM_EVENTO_MOUSE *BM_Eventos_procurar_fila_mouse(int _posicaoX, int _posicaoY, int _finalX, int _finalY);
//==========================================================================

//==========================================================================
// Iniciar fila de eventos
//==========================================================================
int BM_Eventos_Mouse_iniciar_fila_mouse() {
	BM_Fila_eventos_mouse = (BM_EVENTO_MOUSE_FILA*)malloc(1 * sizeof(BM_EVENTO_MOUSE_FILA));
	if (BM_Fila_eventos_mouse == NULL) {
		fprintf(stderr, "ERRO: Nao foi possivel alocar memoria para a pilha de funcoes do mouse\n");
		return ERRO;
	}
	BM_Fila_eventos_mouse->inicio = NULL;
	BM_Fila_eventos_mouse->fim = NULL;
	return SUCESSO;
}
//==========================================================================

//==========================================================================
// Adicionar funcao na fila
//==========================================================================
int BM_Eventos_Mouse_adicionar(BM_MOUSE_FUNCAO _funcao, int _iX, int _iY, int _fX, int _fY, void *_opcional) {
	BM_EVENTO_MOUSE *aux = (BM_EVENTO_MOUSE*)malloc(1 * sizeof(BM_EVENTO_MOUSE));
	if (aux == NULL) {
		fprintf(stderr, "ERRO: Nao foi possivel alocar memoria para um evento do mouse\n");
		return ERRO;
	}
	aux->inicial_X = _iX;
	aux->inicial_Y = _iY;
	aux->final_X = _fX;
	aux->final_Y = _fY;
	aux->funcao = _funcao;
	aux->opcional = _opcional;
	aux->anterior = NULL;
	if (BM_Fila_eventos_mouse->inicio == NULL) {
		BM_Fila_eventos_mouse->inicio = aux;
		BM_Fila_eventos_mouse->fim = aux;
		aux->proximo = NULL;
	}
	else
	{
		BM_Fila_eventos_mouse->inicio->anterior = aux;
		aux->proximo = BM_Fila_eventos_mouse->inicio;
		BM_Fila_eventos_mouse->inicio = aux;
	}
	return SUCESSO;
}
//==========================================================================

//==========================================================================
// Adicionar funcao na fila
//==========================================================================
void BM_Eventos_Mouse_remover(int _posicaoX, int _posicaoY, int _finalX, int _finalY) {
	BM_EVENTO_MOUSE *aux = BM_Eventos_procurar_fila_mouse(_posicaoX, _posicaoY, _finalX, _finalY);
	if (aux == NULL)
		return;
	if (BM_Fila_eventos_mouse->inicio == BM_Fila_eventos_mouse->fim) {
		BM_Fila_eventos_mouse->inicio = NULL;
		BM_Fila_eventos_mouse->fim = NULL;
	}
	else {
		if (aux->anterior != NULL)
			aux->anterior->proximo = aux->proximo;
		else
			BM_Fila_eventos_mouse->inicio = aux->proximo;
		if (aux->proximo != NULL)
			aux->proximo->anterior = aux->anterior;
		else
			BM_Fila_eventos_mouse->fim = aux->anterior;
	}
	free(aux->opcional);
	free(aux);
}
//==========================================================================

//==========================================================================
// Obter fila de eventos
//==========================================================================
BM_EVENTO_MOUSE_FILA *BM_Eventos_Mouse_obter_fila_funcao() {
	return BM_Fila_eventos_mouse;
}
//==========================================================================

//==========================================================================
// Processar fila
//==========================================================================
BM_EVENTO_MOUSE *BM_Eventos_Mouse_processar(int _clickX, int _clickY) {
	BM_EVENTO_MOUSE *aux;
	for (aux = BM_Fila_eventos_mouse->inicio; aux != NULL; aux = aux->proximo) 
		if (_clickX >= aux->inicial_X && _clickX <= aux->final_X &&
			_clickY >= aux->inicial_Y && _clickY <= aux->final_Y)
			break;
	return aux;
}
//==========================================================================

//==========================================================================
// Procurar na fila uma animação especifica
//==========================================================================
BM_EVENTO_MOUSE *BM_Eventos_procurar_fila_mouse(int _posicaoX, int _posicaoY, int _finalX, int _finalY) {
	BM_EVENTO_MOUSE *aux;
	for (aux = BM_Fila_eventos_mouse->inicio; aux != NULL; aux = aux->proximo) {
		if (aux->inicial_X == _posicaoX && aux->inicial_Y == _posicaoY && 
			aux->final_X == _finalX && aux->final_Y == _finalY)
			break;
	}
	return aux;
}
//==========================================================================