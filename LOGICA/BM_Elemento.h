#pragma once

//==========================================================================
// Enum - Elementos disponiveis
//==========================================================================
enum BM_ELEMENTO
{
	NAO_ELEMENTAL = 0,
	LUZ,
	TREVAS,
	FOGO,
	AGUA,
	TERRA,
	AR
};
//==========================================================================

//==========================================================================
// Estrutura de dados de nivel dos elementos
//==========================================================================
typedef struct BM_ELEMENTOS_INFO_S {
	int luz;
	int trevas;
	int fogo;
	int agua;
	int terra;
	int ar;
}BM_ELEMENTOS;
//==========================================================================

//==========================================================================
// Grafo de Sincronia dos elementos
//==========================================================================

//==========================================================================