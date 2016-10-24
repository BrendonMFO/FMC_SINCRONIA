
#include "BM_Allegro.h"
#include "BM_Allegro_eventos_funcoes.h"

//==========================================================================
// Variaveis
//==========================================================================
BM_EVENTOS_FILA *BM_Fila_eventos = NULL;
//==========================================================================

//==========================================================================
// Prototipos
//==========================================================================
BM_EVENTOS_FUNCAO *BM_Eventos_procurar_fila(BM_EVENTO_FUNCAO _funcao);
//==========================================================================

//==========================================================================
// Iniciar pilha de eventos
//==========================================================================
int BM_Eventos_iniciar_fila_funcoes() {
	BM_Fila_eventos = (BM_EVENTOS_FILA*)malloc(1 * sizeof(BM_EVENTOS_FILA));
	if (BM_Fila_eventos == NULL) {
		fprintf(stderr, "ERRO: Nao foi possivel alocar memoria para a pilha de funcoes\n");
		return ERRO;
	}
	BM_Fila_eventos->inicio = NULL;
	BM_Fila_eventos->fim = NULL;
	return SUCESSO;
}
//==========================================================================

//==========================================================================
// Adicionar funcao na fila
//==========================================================================
int BM_Eventos_Funcoes_adicionar(BM_EVENTO_FUNCAO _funcao, void *_parametro) {
	BM_EVENTOS_FUNCAO *aux = (BM_EVENTOS_FUNCAO*)malloc(1 * sizeof(BM_EVENTOS_FUNCAO));
	if (aux == NULL) {
		fprintf(stderr, "ERRO: Nao foi possivel alocar memoria para a pilha de funcoes\n");
		return ERRO;
	}
	aux->ativo = ATIVO;
	aux->funcao = _funcao;
	aux->parametro = _parametro;
	aux->proximo = NULL;
	if (BM_Fila_eventos->fim == NULL) {
		BM_Fila_eventos->inicio = aux;
		BM_Fila_eventos->fim = aux;
		aux->anterior = NULL;
	}
	else
	{
		BM_Fila_eventos->fim->proximo = aux;
		aux->anterior = BM_Fila_eventos->fim;
		BM_Fila_eventos->fim = aux;
	}
	return SUCESSO;
}
//==========================================================================

//==========================================================================
// Adicionar funcao na fila
//==========================================================================
void BM_Eventos_Funcoes_remover(BM_EVENTO_FUNCAO _funcao) {
	BM_EVENTOS_FUNCAO *aux = BM_Eventos_procurar_fila(_funcao);
	if (aux == NULL)
		return;
	aux->ativo = INATIVO;
	if (BM_Fila_eventos->inicio == BM_Fila_eventos->fim) {
		BM_Fila_eventos->inicio = NULL;
		BM_Fila_eventos->fim = NULL;
	}
	else {
		if (aux->anterior != NULL)
			aux->anterior->proximo = aux->proximo;
		else
			BM_Fila_eventos->inicio = aux->proximo;
		if (aux->proximo != NULL)
			aux->proximo->anterior = aux->anterior;
		else
			BM_Fila_eventos->fim = aux->anterior;
	}
}
//==========================================================================

//==========================================================================
// Procurar na fila uma animação especifica
//==========================================================================
BM_EVENTOS_FUNCAO *BM_Eventos_procurar_fila(BM_EVENTO_FUNCAO _funcao) {
	BM_EVENTOS_FUNCAO *aux;
	for (aux = BM_Fila_eventos->inicio; aux != NULL; aux = aux->proximo) {
		if (aux->funcao == _funcao)
			break;
	}
	return aux;
}
//==========================================================================

//==========================================================================
// Obter fila de eventos
//==========================================================================
BM_EVENTOS_FILA *BM_Eventos_obter_fila_funcao() {
	return BM_Fila_eventos;
}
//==========================================================================