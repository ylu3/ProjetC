//
// Created by tsouf on 02/06/2020.
//

#include "ei_types.h"
#include "ei_draw.h"
#include "ei_event.h"
#include "hw_interface.h"

// TEST DE EI_FILL ET EI_COPY_SURFACE SUR UN TEXTE

int main(int argc, char** argv)
{
        ei_color_t vert = {0, 255, 0, 255};
        ei_color_t bleu = {0, 0, 255, 255};

        ei_point_t origine = {0, 0};

        ei_surface_t			surface_principale;
        ei_size_t			surface_principale_size;


        hw_init();

        surface_principale_size.width	= 640;
        surface_principale_size.height	= 480;

        surface_principale = hw_create_window(surface_principale_size, EI_FALSE);
        hw_surface_lock(surface_principale);

        // Test ei_fill
        ei_fill(surface_principale, &bleu, NULL);

        // Test de copie surface sur un texte
        ei_surface_t surface_txt;
        ei_font_t font = hw_text_font_create("misc/font.ttf", 0, 100);
        surface_txt = hw_text_create_surface("EQUIPE 30 IG", font, vert);
        hw_surface_lock(surface_txt);
        ei_rect_t src_rect = hw_surface_get_rect(surface_txt);

        ei_rect_t dst_rect;
        dst_rect.top_left = origine;
        dst_rect.size = hw_surface_get_size(surface_txt);

        ei_copy_surface(surface_principale, &dst_rect, surface_txt, &src_rect, EI_TRUE);
        hw_surface_unlock(surface_txt);

        hw_surface_free(surface_txt);

        hw_surface_unlock(surface_principale);
        hw_surface_update_rects(surface_principale, NULL);

        // Boucle evenementielle
        ei_event_t			event;

        event.type = ei_ev_none;
        while (event.type != ei_ev_keydown)
                hw_event_wait_next(&event);

        // Free hardware resources.
        hw_quit();

        // Terminate program with no error code.
        return 0;

}