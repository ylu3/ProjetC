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
        ei_point_t origine = {0, 0};

        ei_surface_t			surface_principale;
        ei_size_t			surface_principale_size;


        hw_init();

        surface_principale_size.width	= 640;
        surface_principale_size.height	= 420;

        surface_principale = hw_create_window(surface_principale_size, EI_FALSE);
        hw_surface_lock(surface_principale);

        // Test de copie surface sur une image
        ei_surface_t surface_img;
        surface_img = hw_image_load("misc/Logo_ENSIMAG.png", surface_principale);
        hw_surface_lock(surface_img);
        ei_rect_t src_rect = hw_surface_get_rect(surface_img);

        ei_rect_t dst_rect;
        dst_rect.top_left = origine;
        dst_rect.size = hw_surface_get_size(surface_img);

        ei_copy_surface(surface_principale, &dst_rect, surface_img, &src_rect, EI_FALSE);

        hw_surface_unlock(surface_img);
        hw_surface_free(surface_img);

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
