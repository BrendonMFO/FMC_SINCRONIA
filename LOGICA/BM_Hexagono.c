#include "BM_Hexagono.h"

//==========================================================================
// Criar Hexagono
//==========================================================================
BM_Hexagono BM_Hexagono_criar(int _id, int _estado, int _elemento)
{
	BM_Hexagono temp = { _id, _estado, _elemento, FALSE };
	return temp;
}
//==========================================================================

//==========================================================================
// Alterar estado e elemento do hexagono
//==========================================================================
void BM_Hexagono_alterar(BM_Hexagono *_hexagono, int _estado, int _elemento) {
	_hexagono->estado = _estado;
	_hexagono->elemento = _elemento;
}
//==========================================================================