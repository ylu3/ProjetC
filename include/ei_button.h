//
// Created by tsouf on 22/05/2020.
//
#include "ei_widget.h"
#include "ei_utils.h"

#ifndef PROJETC_IG_EI_BUTTON_H
#define PROJETC_IG_EI_BUTTON_H

typedef struct ei_button_t {
        ei_widget_t		widget;
        void*			user_param;
        ei_callback_t	callback;

        ei_color_t		color;
        int				border_width;
        int				corner_radius;
        ei_relief_t		relief;

        char*			text;
        ei_font_t		text_font;
        ei_color_t		text_color;
        ei_anchor_t		text_anchor;

        ei_surface_t	img;
        ei_rect_t*		img_rect;
        ei_anchor_t		img_anchor;

} ei_button_t;

extern ei_bool_t ei_press_button(struct ei_widget_t* widget, struct ei_event_t* event, void* user_param);
extern ei_bool_t ei_release_button(struct ei_widget_t* widget, struct ei_event_t* event, void* user_param);
extern ei_bool_t ei_toggle_relief(struct ei_widget_t* widget, struct ei_event_t* event, void* user_param);

#endif //PROJETC_IG_EI_BUTTON_H

