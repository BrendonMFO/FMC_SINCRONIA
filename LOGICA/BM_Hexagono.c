#include "BM_Hexagono.h"
#include "BM_Campo.h"
#include "BM_Allegro_eventos_mouse.h"
#include "BM_Allegro.h"
#include "BM_Recursos.h"
#include <stdarg.h>

//==========================================================================
// Grafo de sincronia entre os elementos
//==========================================================================
const int BM_Sincronia[7][7] = { {0,0,0,0,0,0,0},
								 {0,1,-2,-1,1,1,2},
								 {0,-2,1,2,-1,1,1},
								 {0,-1,2,1,-2,1,1},
								 {0,1,-1,2,1,1,-2},
								 {0,1,1,-2,2,1,-1},
								 {0,1,1,2,-1,-2,1} };
//==========================================================================

//==========================================================================
// Prototipos
//==========================================================================
void BM_Hexagono_adicionar_listener_mouse(BM_Hexagono *_alvo, int _acao);
//==========================================================================

//==========================================================================
// Criar Hexagono
//==========================================================================
BM_Hexagono BM_Hexagono_criar(int _id, int _estado, int _elemento)
{
	BM_Hexagono temp = { _id, _estado, _elemento, FALSE, FALSE };
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

//==========================================================================
// Dominar territorio
//==========================================================================
int BM_Hexagono_batalha(int _alvo, int _atacante) {
	int resultadoAtaque = 0, resultadoDefesa = 0;
	BM_Campo *campo = BM_Campo_getCampo();
	BM_Hexagono *hexagonoAtaque, *hexagonoDefesa;

	//======================================================================
	// Definir os hexagonos de ataque e defesa
	//======================================================================
	hexagonoAtaque = &campo->hexagonos[_atacante];
	hexagonoDefesa = &campo->hexagonos[_alvo];
	//======================================================================

	//======================================================================
	// Setar hexagonos visiveis
	//======================================================================
	hexagonoAtaque->visivel = TRUE;
	hexagonoDefesa->visivel = TRUE;
	//======================================================================

	//======================================================================
	// Verificar se ha alguma superioridade de sincronia entre os hexagonos
	//======================================================================
	if (BM_Sincronia[hexagonoAtaque->elemento][hexagonoDefesa->elemento] == 2)
		resultadoAtaque += 4;
	else if (BM_Sincronia[hexagonoAtaque->elemento][hexagonoDefesa->elemento] == -2)
		resultadoDefesa += 4;
	else
		resultadoAtaque += BM_Sincronia[hexagonoAtaque->elemento][hexagonoDefesa->elemento];
	//======================================================================

	//======================================================================
	// Calcular poder das sincronias
	//======================================================================
	resultadoAtaque += BM_Hexagono_calcular_sincronia(*hexagonoAtaque, JOGADOR);
	resultadoDefesa += BM_Hexagono_calcular_sincronia(*hexagonoDefesa, ADVERSARIO);
	//======================================================================

	//======================================================================
	// Determinar resultado
	//======================================================================
	if (resultadoAtaque > resultadoDefesa)
		return VITORIA_ATAQUE;
	if (resultadoDefesa > resultadoAtaque)
		return VITORIA_DEFESA;
	//======================================================================

	return EMPATE;
}
//==========================================================================

//==========================================================================
// Marcar alvos
//==========================================================================
int BM_Hexagono_marcar_alvos(int _centro, int _acao) {
	BM_Campo *campo = BM_Campo_getCampo();
	BM_Hexagono hexagono = campo->hexagonos[_centro], *aux;
	int i, contador = 0;
	for (i = 0; i < 6; i++) {
		if (hexagono.conexoes[i] != -1) {
			aux = &campo->hexagonos[hexagono.conexoes[i]];
			if (aux->estado == ADVERSARIO) {
				BM_Hexagono_adicionar_listener_mouse(aux, _acao);
				aux->alvo = _acao;
				contador++;
			}
		}
	}
	return contador;
}
//==========================================================================

//==========================================================================
// Adicionar/Remover listener de eventos do mouse no hexagono selecionado
//==========================================================================
void BM_Hexagono_adicionar_listener_mouse(BM_Hexagono *_alvo, int _acao) {
	int finalX, finalY, *id;
	finalX = _alvo->posicaoX + BM_Allegro_largura_da_imagem(SPRITES(BM_IMG_HEXAGONO_ALVO)->imagem->bitmap);
	finalY = _alvo->posicaoY + BM_Allegro_altura_da_imagem(SPRITES(BM_IMG_HEXAGONO_ALVO)->imagem->bitmap);
	switch (_acao)
	{
	case MARCAR:
		id = (int*)malloc(1 * sizeof(int));
		*id = _alvo->id;
		BM_Eventos_Mouse_adicionar(NULL, _alvo->posicaoX, _alvo->posicaoY, finalX, finalY, id);
		break;
	case DESMARCAR:
		BM_Eventos_Mouse_remover(_alvo->posicaoX, _alvo->posicaoY, finalX, finalY);
		break;
	}
}
//==========================================================================

//==========================================================================
// Calcular vinculo
//==========================================================================
int BM_Hexagono_calcular_sincronia(BM_Hexagono _alvo, int _player) {
	int resultado = 0, i;
	BM_Campo *campo = BM_Campo_getCampo();
	BM_Hexagono aux;
	for (i = 0; i < 6; i++) {
		if (_alvo.conexoes[i] != -1) {
			aux = campo->hexagonos[_alvo.conexoes[i]];
			if (aux.estado == _player)
				resultado += BM_Sincronia[_alvo.elemento][aux.elemento];
		}
	}
	return resultado;
}
//==========================================================================