//
// Created by Yan LU on 20/05/2020.
//
#include "ei_widget.h"
#include "ei_utils.h"

#ifndef PROJETC_IG_EI_WIDGET_FRAME_H
#define PROJETC_IG_EI_WIDGET_FRAME_H

typedef struct ei_frame_t{
    ei_widget_t		widget;
    ei_color_t  	bg_color;      //struct
    int			border_width;

    ei_relief_t		relief;     //enum

    char*		text;
    ei_font_t		text_font;      //pointer
    ei_color_t		text_color;     //struct
    ei_anchor_t		text_anchor;    //enum

    ei_surface_t	img;    //pointer
    ei_rect_t*		img_rect;       //pointer
    ei_anchor_t		img_anchor;     //enum

} ei_frame_t;

extern void ei_frameclass_register();
extern void ei_root_configure(struct ei_widget_t* widget, ei_surface_t display_surface);

#endif //PROJETC_IG_EI_WIDGET_FRAME_H
