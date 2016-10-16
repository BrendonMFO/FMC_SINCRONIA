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
typedef struct BM_ELEMENTOS_NIVEL_S {
	int luz;
	int trevas;
	int fogo;
	int agua;
	int terra;
	int ar;
}BM_ELEMENTOS_NIVEL;
//==========================================================================

//==========================================================================
// Grafo de Sincronia dos elementos
//==========================================================================
const int BM_Sincronia_Elementos[7][6] = {	{ 0,0,0,0,0,0 },
											{ 1,-2,-1,1,1,2 },
											{ -2,1,2,-1,1,1 },
											{ -1,2,1,-1,1,1 },
											{ 1,-1,2,1,1,-2 },
											{ 1,1,-2,2,1,-1 },
											{ 1,1,2,-1,-2,1 } };
//==========================================================================