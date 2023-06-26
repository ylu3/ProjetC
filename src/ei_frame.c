//
// Created by Yan LU on 15/05/2020.
//

#include <stdlib.h>

#include "ei_application.h"
#include "ei_utils.h"
#include "ei_types.h"
#include "ei_draw.h"
#include "ei_frame.h"
#include <string.h>
#include "ei_geometrymanager.h"

// Variables Globales


void* ei_frame_allocfunc(void) {
        return calloc(1, sizeof(ei_frame_t));
}

void ei_frame_releasefunc(struct ei_widget_t*	widget) {
        ei_frame_t* frm_ptr = (ei_frame_t*) widget;
        if (frm_ptr == NULL) {
                return;
        }
        if(frm_ptr->img_rect != NULL){
            free(frm_ptr->img_rect);
        }
}



void ei_frame_drawfunc(struct ei_widget_t* widget,
                         ei_surface_t surface,
                         ei_surface_t pick_surface,
                         ei_rect_t* clipper) {

    ei_frame_t* frm_ptr = (ei_frame_t*) widget;
    ei_color_t frm_color = lighter_color(frm_ptr->bg_color);
    if (frm_ptr->border_width != 0){
        ei_linked_point_t* first_point_u = get_border(&widget->screen_location, frm_ptr->border_width, 'u');
        ei_linked_point_t* first_point_l = get_border(&widget->screen_location, frm_ptr->border_width, 'l');
        switch (frm_ptr->relief) {
            case (ei_relief_raised):
                ei_draw_polygon(surface, first_point_u, white, clipper);
                ei_draw_polygon(surface, first_point_u, frm_color, clipper);
                ei_draw_polygon(surface, first_point_l, black, clipper);
                ei_draw_polygon(surface, first_point_l, frm_color, clipper);
                break;
            case (ei_relief_sunken):
                ei_draw_polygon(surface, first_point_u, black, clipper);
                ei_draw_polygon(surface, first_point_u, frm_color, clipper);
                ei_draw_polygon(surface, first_point_l, white, clipper);
                ei_draw_polygon(surface, first_point_l, frm_color, clipper);
                break;
            case(ei_relief_none):
                break;
        }
        free_linked_point_list(first_point_l);
        free_linked_point_list(first_point_u);
    }
        // Surface d'affichage

        ei_linked_point_t* first_point = get_border(&widget->screen_location, frm_ptr->border_width, 'w');
        ei_draw_polygon(surface, first_point, frm_ptr->bg_color, clipper);
        free_linked_point_list(first_point);

        // Offscreen

        first_point = get_border(&widget->screen_location, 0, 'w');
        ei_draw_polygon(pick_surface, first_point, *(widget->pick_color), clipper);
        free_linked_point_list(first_point);



    // Affichage du texte
    if (frm_ptr->text != NULL && strcmp(frm_ptr->text,"") != 0) {
            ei_point_t where = frm_ptr->widget.screen_location.top_left;
            ei_draw_text(surface, &where, frm_ptr->text, frm_ptr->text_font, frm_ptr->text_color, NULL);
    }

    if (frm_ptr->img != NULL) {
        ei_draw_img(surface, frm_ptr->img, frm_ptr->img_rect, clipper);
    }

}


void ei_frame_setdefaultsfunc(struct ei_widget_t*  widget){

    ei_frame_t* frm_ptr = (ei_frame_t*) widget;
    frm_ptr->bg_color = ei_default_background_color;
    frm_ptr->border_width = 0;
    ei_relief_t relief_none = ei_relief_none;
    frm_ptr->relief = relief_none;
    frm_ptr->text = NULL;
    frm_ptr->text_font = ei_default_font;
    frm_ptr->text_color = ei_font_default_color;
    frm_ptr->text_anchor = ei_anc_center;
    frm_ptr->img = NULL;
    frm_ptr->img_rect = NULL;
    frm_ptr->img_anchor = ei_anc_center;
}


void ei_root_configure(struct ei_widget_t* root, ei_surface_t main_surface){

    ei_size_t main_size = hw_surface_get_size(main_surface);
    root->requested_size.width = main_size.width;
    root->requested_size.height = main_size.height;
    ei_rect_t main_rect = hw_surface_get_rect(main_surface);
    root->screen_location = main_rect;
    root->content_rect = &(root->screen_location);

}


void ei_frame_geomnotifyfunc(ei_widget_t* widget){
    widget->geom_params->manager->runfunc(widget);
}



void ei_frame_register_class()
{
    ei_widgetclass_t* frm_ptr = malloc(sizeof(ei_widgetclass_t));
    strncpy(ei_widgetclass_stringname(frm_ptr->name), "frame", 20);
    frm_ptr->allocfunc = ei_frame_allocfunc;
    frm_ptr->releasefunc = ei_frame_releasefunc;
    frm_ptr->drawfunc = ei_frame_drawfunc;
    frm_ptr->setdefaultsfunc =ei_frame_setdefaultsfunc;
    frm_ptr->geomnotifyfunc = ei_frame_geomnotifyfunc;
    frm_ptr->next = NULL;

    ei_widgetclass_register(frm_ptr);
}



