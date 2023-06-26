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

        ei_color_t	button_color_P	= {0x00, 0xaa, 0x00, 0xff};
        ei_color_t	button_color_f	= {0x00, 0x00, 0xaa, 0xff};
        ei_color_t	button_color_ff	= {0xaa, 0x00, 0x00, 0xff};
        ei_color_t      text_color      = {0x00, 0x00, 0x00, 0xff};
        ei_relief_t     relief          = ei_relief_raised;
        int             button_border_width    = 1;
        char*           button_title_P   = "P";
        char*           button_title_f    = "f";
        char*           button_title_ff    = "f.f";
        char*           button_title_autre    = "autre";
        ei_anchor_t	button_anchor	= ei_anc_center;

        float           button_rel_x    = 0.5;
        float           button_rel_y    = 0.5;
        int		button_x	= 0;
        int		button_y	= 0;
        float           button_rel_size_y = 0.5;
        float           button_rel_size_x = 0.5;
        int              button_size_x = 150;
        int button_size_y = 100;
        ei_size_t       window_size     = {300,300};
        ei_size_t       window_size_2    ={600,600};
        char*           window_title    = "G.G : descendants";
        ei_color_t      window_color    = {0xaa,0x99,0x99, 0xff};
        int             window_border_width    = 2;
        ei_bool_t       closable        = EI_FALSE;
        ei_axis_set_t   window_resizable = ei_axis_both;
        ei_point_t	window_position	 = {50, 50};

        ei_widget_t* P;
        ei_widget_t* f;
        ei_widget_t* ff;
        ei_widget_t* autre;
        ei_widget_t*    toplevel;

        ei_app_create(screen_size, EI_FALSE);
        ei_frame_configure(ei_app_root_widget(), NULL, &root_bgcol, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);

        toplevel = ei_widget_create("toplevel", ei_app_root_widget(), NULL, NULL);

        P = ei_widget_create("button", toplevel,NULL,NULL);
        f = ei_widget_create("button", P,NULL,NULL);
        ff = ei_widget_create("button", f,NULL,NULL);

         ei_toplevel_configure(toplevel, &window_size, &window_color, &window_border_width, &window_title, &closable, NULL, NULL);

         ei_button_configure(P, NULL, &button_color_P,
                            &button_border_width, NULL, &relief, &button_title_P, NULL, &text_color, NULL,
                            NULL, NULL, NULL, &button_callback, NULL);
        ei_button_configure(f, NULL, &button_color_f,
                            &button_border_width, NULL, &relief, &button_title_f, NULL, &text_color, NULL,
                            NULL, NULL, NULL, &button_callback, NULL);
        ei_button_configure(ff, NULL, &button_color_ff,
                            &button_border_width, NULL, &relief, &button_title_ff, NULL, &text_color, NULL,
                            NULL, NULL, NULL, &button_callback, NULL);

        ei_place(toplevel, NULL, &(window_position.x), &(window_position.y), NULL, NULL, NULL, NULL, NULL, NULL);

        ei_place(P, &button_anchor, NULL, NULL, NULL,NULL, &button_rel_x,&button_rel_y, &button_rel_size_x, &button_rel_size_y);
        ei_place(f, &button_anchor, NULL, NULL, NULL,NULL, &button_rel_x, &button_rel_y, &button_rel_size_x, &button_rel_size_y);
        ei_place(ff, &button_anchor, NULL, NULL, NULL,NULL, &button_rel_x, &button_rel_y, &button_rel_size_x, &button_rel_size_y);

       //  2. A decommenter !
     //  ei_geometrymanager_unmap(f);

      ei_bind(ei_ev_keydown, NULL, "all", process_key, NULL);

        ei_app_run();

        ei_unbind(ei_ev_keydown, NULL, "all", process_key, NULL);

        ei_app_free();

        return (EXIT_SUCCESS);
}