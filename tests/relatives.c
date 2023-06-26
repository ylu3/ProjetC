#include <stdio.h>
#include <stdlib.h>

#include "ei_application.h"
#include "ei_event.h"
#include "hw_interface.h"
#include "ei_widget.h"
#include "ei_geometrymanager.h"


ei_bool_t button_press(ei_widget_t* widget, ei_event_t* event, void* user_param);
ei_bool_t process_key(ei_widget_t* widget, ei_event_t* event, void* user_param);
ei_callback_t button_callback = button_press;


ei_bool_t button_press(ei_widget_t* widget, ei_event_t* event, void* user_param)
{
        printf("Click !\n");
        return EI_TRUE;
}

ei_bool_t process_key(ei_widget_t* widget, ei_event_t* event, void* user_param)
{
        if (event->param.key.key_code == SDLK_ESCAPE) {
                ei_app_quit_request();
                return EI_TRUE;
        }

        return EI_FALSE;
}

// main --

int main(int argc, char** argv)
{
        ei_size_t	screen_size	= {1000, 1000};
        ei_color_t      root_bgcol      = {0x52, 0x7f, 0xb4, 0xff};

        ei_color_t	button_color	= {0x88, 0x88, 0x88, 0xff};
        ei_color_t      text_color      = {0xFF, 0x00, 0x00, 0xff};
        ei_relief_t     relief          = ei_relief_raised;
        int             button_border_width    = 1;
        char*           button_title_c   = "c";
        char*           button_title_nw    = "nw";
        char*           button_title_se    = "se";
        char*           button_title_fils    = "Child";
        ei_anchor_t	button_anchor_c	= ei_anc_center;
        ei_anchor_t	button_anchor_nw	= ei_anc_northwest;
        ei_anchor_t	button_anchor_se	= ei_anc_southeast;
        ei_anchor_t	button_anchor_fils	= ei_anc_center;
        float           button_rel_x    = 0.5;
        float           button_rel_y    = 0.5;
        int		button_x	= 150;
        int		button_y	= 150;
        float           button_rel_size_y = 0.1;
        float           button_rel_size_x = 0.1;
        int              button_size_x = 40;
        int button_size_y = 40;
        ei_size_t       window_size     = {300,300};
        ei_size_t       window_size_2    ={600,600};
        char*           window_title    = "Gestion de geometrie";
        ei_color_t      window_color    = {0xaa,0x99,0x99, 0xff};
        int             window_border_width    = 2;
        ei_bool_t       closable        = EI_FALSE;
        ei_axis_set_t   window_resizable = ei_axis_both;
        ei_point_t	window_position	 = {10, 10};


        ei_widget_t*    button;
        ei_widget_t* c;
        ei_widget_t* nw;
        ei_widget_t* se;
        ei_widget_t* fils;
        ei_widget_t*    toplevel;

        ei_app_create(screen_size, EI_FALSE);
        ei_frame_configure(ei_app_root_widget(), NULL, &root_bgcol, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);

        toplevel = ei_widget_create("toplevel", ei_app_root_widget(), NULL, NULL);
        button = ei_widget_create("button", toplevel, NULL, NULL);

        c = ei_widget_create("button", toplevel,NULL,NULL);
        nw = ei_widget_create("button", toplevel,NULL,NULL);
        se = ei_widget_create("button", toplevel,NULL,NULL);
        fils = ei_widget_create("button", toplevel,NULL,NULL);

        // 1 -3  cas : A commenter !

        ei_toplevel_configure(toplevel, &window_size, &window_color, &window_border_width, &window_title, &closable, NULL, NULL);
        ei_button_configure(nw, NULL, &button_color,
                            &button_border_width, NULL, &relief, &button_title_nw, NULL, &text_color, NULL,
                            NULL, NULL, NULL, &button_callback, NULL);
        ei_button_configure(se, NULL, &button_color,
                            &button_border_width, NULL, &relief, &button_title_se, NULL, &text_color, NULL,
                            NULL, NULL, NULL, &button_callback, NULL);
        ei_button_configure(fils, NULL, &button_color,
                            &button_border_width, NULL, &relief, &button_title_fils, NULL, &text_color, NULL,
                            NULL, NULL, NULL, &button_callback, NULL);
        ei_button_configure(c, NULL, &button_color,
                            &button_border_width, NULL, &relief, &button_title_c, NULL, &text_color, NULL,
                            NULL, NULL, NULL, &button_callback, NULL);

        ei_place(toplevel, NULL, &(window_position.x), &(window_position.y), NULL, NULL, NULL, NULL, NULL, NULL);

        // Role du releasefunc
        // se->geom_params->manager->releasefunc(se);

        // 3eme cas : a commenter !
        ei_place(c, &button_anchor_c, NULL, NULL, NULL,NULL, &button_rel_x,&button_rel_y, &button_rel_size_x, &button_rel_size_y);
        ei_place(nw, &button_anchor_nw, NULL, NULL, NULL,NULL, &button_rel_x, &button_rel_y, &button_rel_size_x, &button_rel_size_y);
        ei_place(se, &button_anchor_se, NULL, NULL, NULL,NULL, &button_rel_x, &button_rel_y, &button_rel_size_x, &button_rel_size_y);


        // 3eme cas : a decommenter !
/*
        ei_place(c, &button_anchor_c, &button_x, &button_y, &button_size_x,&button_size_y, NULL,NULL, NULL, NULL);
        ei_place(nw, &button_anchor_nw,&button_x, &button_y,&button_size_x,&button_size_y, NULL, NULL, NULL, NULL);
        ei_place(se, &button_anchor_se, &button_x, &button_y, &button_size_x,&button_size_y, NULL, NULL, NULL, NULL);
*/

        // Role de unmap
       // ei_geometrymanager_unmap(se);


        ei_bind(ei_ev_keydown, NULL, "all", process_key, NULL);

        ei_app_run();

        ei_unbind(ei_ev_keydown, NULL, "all", process_key, NULL);

        ei_app_free();

        return (EXIT_SUCCESS);
}