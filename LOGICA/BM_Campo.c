#include "BM_Campo.h"

//==========================================================================
// Variavel que ira conter os dados do campo
//==========================================================================
BM_Campo *campo = NULL;
char *nome_campo;
//==========================================================================

//==========================================================================
// Prototipos
//==========================================================================
void carregarHexagonos(BM_HEXAGONO *_hexagonos, FILE *_arquivo, int _quantidadeLinhas);
int numeroAleatorio(int _numero);
//==========================================================================

//==========================================================================
// Criar campo apartir de um arquivo
//==========================================================================
BM_Campo *BM_Campo_getCampo()
{
	if (campo == NULL) {

		//==================================================================
		// Checar se existe um arquivo de campo setado
		//==================================================================
		if (nome_campo == NULL) {
			fprintf(stderr, "ERRO: nao ha um arquivo de campo definido\n");
			return NULL;
		}
		//==================================================================

		//==================================================================
		// Alocar campo na memoria
		//==================================================================
		campo = (BM_Campo*)malloc(1 * sizeof(BM_Campo));
		if (campo == NULL) {
			fprintf(stderr, "ERRO: nao foi possivel alocar memoria de campo\n");
			return NULL;
		}
		//==================================================================

		//==================================================================
		// Abrir arquivo e obter quantidade de elementos
		//==================================================================
		FILE *arquivo = fopen(nome_campo, "r");
		if (arquivo == NULL) {
			fprintf(stderr, "ERRO: nao foi possivel abrir arquivo de campo\n");
			return NULL;
		}
		fscanf(arquivo, "Grafos = %d\n", &campo->quantidade);
		//==================================================================

		//==================================================================
		// Criar e Popular lista de hexagonos
		//==================================================================
		campo->hexagonos = (BM_HEXAGONO*)malloc(campo->quantidade * sizeof(BM_HEXAGONO));
		if (campo->hexagonos == NULL) {
			fprintf(stderr, "ERRO: nao foi possivel montar os hexagonos do campo\n");
			return NULL;
		}
		carregarHexagonos(campo->hexagonos, arquivo, campo->quantidade);
		//==================================================================
	}

	return campo;
}
//==========================================================================

//==========================================================================
// Setar arquivo de campo
//==========================================================================
void BM_Campo_setCampo(char *_campo) {
	nome_campo = (char*)malloc(strlen(_campo)*sizeof(char));
	strcpy(nome_campo, _campo);
}
//==========================================================================

//==========================================================================
// Popular vetor de hexagonos
//==========================================================================
void carregarHexagonos(BM_HEXAGONO *_hexagonos, FILE *_arquivo, int _quantidadeLinhas) {
	int contador;
	time_t t;
	srand((unsigned)time(&t));
	for (contador = 0; contador < _quantidadeLinhas; contador++) {
		_hexagonos[contador] = BM_Hexagono_criar(contador, NEUTRO, numeroAleatorio(7));
		fscanf(_arquivo, "X : %d Y : %d Conexoes : [ %d %d %d %d %d %d ]\n", 
			&_hexagonos[contador].posicaoX,
			&_hexagonos[contador].posicaoY,
			&_hexagonos[contador].conexoes[0],
			&_hexagonos[contador].conexoes[1],
			&_hexagonos[contador].conexoes[2],
			&_hexagonos[contador].conexoes[3],
			&_hexagonos[contador].conexoes[4],
			&_hexagonos[contador].conexoes[5]);
	}
}
//==========================================================================

//==========================================================================
// Sortear um elemento aleatorio
//==========================================================================
int numeroAleatorio(int _numero) {
	return rand() % _numero;
}
//==========================================================================

//==========================================================================
// Redefinir campo
//==========================================================================
void BM_Campo_redefinir() {
	int contador;
	for (contador = 0; contador < campo->quantidade; contador++) {
		campo->hexagonos[contador].estado = NEUTRO;
		campo->hexagonos[contador].elemento = 0;
	}
}
//==========================================================================