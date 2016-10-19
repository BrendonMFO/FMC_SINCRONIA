#include "BM_Allegro.h"

//==========================================================================
// Funçao - Iniciar Allegro
//==========================================================================
int BM_Allegro_iniciar() {
	if (!al_init()) {
		fprintf(stderr, "Falha ao iniciar modo grafico Allegro\n");
		return FALSE;
	}
	if (!al_install_mouse()) {
		fprintf(stderr, "Falha ao iniciar mouse Allegro\n");
		return FALSE;
	}
	if (!al_install_keyboard()) {
		fprintf(stderr, "Falha ao iniciar teclado Allegro\n");
		return FALSE;
	}
	if (!al_init_image_addon()) {
		fprintf(stderr, "Falha ao iniciar Addon de imagem Allegro\n");
		return FALSE;
	}
	if (!al_init_ttf_addon()) {
		fprintf(stderr, "Falha ao iniciar Addon de fonte ttf\n");
		return FALSE;
	}
	if (!al_init_primitives_addon())
	{
		fprintf(stderr, "Falha ao inicializar add-on de primitivas.\n");
		return false;
	}
	al_init_font_addon();
	return TRUE;
}
//==========================================================================

//==========================================================================
// Funçao - Iniciar janela
//==========================================================================
int BM_Allegro_criar_janela(ALLEGRO_DISPLAY **_window, int x, int y) {
	al_set_new_bitmap_flags(ALLEGRO_MEMORY_BITMAP);
	*_window = al_create_display(x, y);
	if (!*_window) {
		fprintf(stderr, "Erro ao criar janela");
		return FALSE;
	}
	return TRUE;
}
//==========================================================================

//==========================================================================
// Funçao - Instanciar mouse do sistema na aplicação
//==========================================================================
int BM_Allegro_iniciar_mouse(ALLEGRO_DISPLAY **_mouse, int flags) {
	if (!al_set_system_mouse_cursor(*_mouse, flags)) {
		fprintf(stderr, "Falha ao instanciar mouse do sistema Allegro\n");
		return FALSE;
	}
	return TRUE;
}
//==========================================================================

//==========================================================================
// Funçao - Iniciar evento
//==========================================================================
int BM_Allegro_criar_eventos(ALLEGRO_EVENT_QUEUE **_evento) {
	*_evento = al_create_event_queue();
	if (!*_evento) {
		fprintf(stderr, "Falha ao criar lista de eventos\n");
		return FALSE;
	}
	return TRUE;
}
//==========================================================================

//==========================================================================
// Funçao - Carregar font
//==========================================================================
int BM_Allegro_carregar_font(ALLEGRO_FONT **_font, char *fonte, int tamanho, int flags) {
	*_font = al_load_ttf_font(fonte, tamanho, flags);
	if (!*_font) {
		fprintf(stderr, "Erro ao carregar fonte - %s\n", fonte);
		return FALSE;
	}
	return TRUE;
}
//==========================================================================

