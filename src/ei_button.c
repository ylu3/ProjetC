//
// Created by Yan LU on 22/05/2020.
//
#include <stdlib.h>
#include <string.h>

#include "ei_application.h"
#include "ei_event.h"
#include "ei_geometrymanager.h"
#include "ei_utils.h"
#include "ei_button.h"
#include "ei_draw.h"
#include "ei_app_state.h"
#include "hw_interface.h"



void* ei_button_allocfunc(void) {
        return calloc(1, sizeof(ei_button_t));
}

void ei_button_releasefunc(struct ei_widget_t*	widget) {
    if (widget == NULL) {
        return;
    }

    ei_button_t* btn_ptr = (ei_button_t*) widget;
    if(btn_ptr->img_rect != NULL){
        free(btn_ptr->img_rect);
    }

}

void ei_button_setdefaultsfunc(struct ei_widget_t* widget)
{
        ei_button_t* btn_ptr = (ei_button_t*) widget;
        btn_ptr->callback = NULL;
        btn_ptr->user_param = NULL;

        btn_ptr->color = ei_default_background_color;
        btn_ptr->border_width = 0;
        btn_ptr->corner_radius = k_default_button_corner_radius;
        btn_ptr->relief = ei_relief_none;

        btn_ptr->text = NULL;
        btn_ptr->text_font = ei_default_font;
        btn_ptr->text_color = ei_font_default_color;
        btn_ptr->text_anchor = ei_anc_center;

        btn_ptr->img = NULL;
        btn_ptr->img_rect = NULL;
        btn_ptr->img_anchor = ei_anc_center;

}


void calculate_text_location(ei_button_t* btn_ptr, ei_point_t* where){

    ei_size_t txt_size;
    hw_text_compute_size(btn_ptr->text, btn_ptr->text_font,&(txt_size.width),&(txt_size.height));

    ei_size_t btn_size = btn_ptr->widget.screen_location.size;
    ei_point_t btn_topleft = btn_ptr->widget.screen_location.top_left;

    if (txt_size.width > btn_size.width || txt_size.height> btn_size.height){
        return;
    }

    switch (btn_ptr->text_anchor) {
        case ei_anc_none :
            break;
        case ei_anc_center : 
            where->x = btn_topleft.x + btn_size.width/2 - txt_size.width/2;
            where->y = btn_topleft.y + btn_size.height/2 - txt_size.height/2;
            break;
        case ei_anc_north : 
            where->x = btn_topleft.x + btn_size.width/2 - txt_size.width/2;
            where->y = btn_topleft.y;
            break;
        case ei_anc_east :
            where->x = btn_topleft.x + btn_size.width - txt_size.width;
            where->y = btn_topleft.y + btn_size.height/2 - txt_size.height/2;
            break;
        case ei_anc_south :
            where->x = btn_topleft.x + btn_size.width/2 - txt_size.width/2;
            where->y = btn_topleft.y + btn_size.height - txt_size.height;
            break;
        case ei_anc_west :
            where->x = btn_topleft.x;
            where->y = btn_topleft.y + btn_size.height/2 - txt_size.height/2;
            break;
        case ei_anc_northeast : 
            where->x = btn_topleft.x + btn_size.width - txt_size.width;
            where->y = btn_topleft.y;
            break;
        case ei_anc_southeast : 
            where->x = btn_topleft.x + btn_size.width - txt_size.width;
            where->y = btn_topleft.y + btn_size.height - txt_size.height;
            break;
        case ei_anc_southwest : 
            where->x = btn_topleft.x;
            where->y = btn_topleft.y + btn_size.height - txt_size.height;
            break;
        case ei_anc_northwest : 
            where->x = btn_topleft.x;
            where->y = btn_topleft.y;
            break;
    }


}


void ei_button_drawfunc(struct ei_widget_t *widget,
                        ei_surface_t surface,
                        ei_surface_t pick_surface,
                        ei_rect_t *clipper) {

    ei_button_t *btn_ptr = (ei_button_t *) widget;
    ei_color_t btn_color = lighter_color(btn_ptr->color);
    int border_width = btn_ptr->border_width;
    if (border_width != 0) {
        ei_linked_point_t *frame_u = rounded_frame(&widget->screen_location, btn_ptr->corner_radius,
                                                   border_width, 'u');
        ei_linked_point_t *frame_l = rounded_frame(&widget->screen_location, btn_ptr->corner_radius,
                                                   border_width, 'l');
        switch (btn_ptr->relief) {
            case (ei_relief_raised):
                ei_draw_polygon(surface, frame_u, white, clipper);
                ei_draw_polygon(surface, frame_u, btn_color, clipper);
                ei_draw_polygon(surface, frame_l, black, clipper);
                ei_draw_polygon(surface, frame_l, btn_color, clipper);
                break;
            case (ei_relief_sunken):
                ei_draw_polygon(surface, frame_u, black, clipper);
                ei_draw_polygon(surface, frame_u, btn_color, clipper);
                ei_draw_polygon(surface, frame_l, white, clipper);
                ei_draw_polygon(surface, frame_l, btn_color, clipper);
                break;
            case (ei_relief_none):
                break;
        }
        free_linked_point_list(frame_u);
        free_linked_point_list(frame_l);
    }

    ei_rect_t rect_inside = widget->screen_location;
    rect_inside.top_left.x += border_width;
    rect_inside.top_left.y += border_width;
    rect_inside.size.width -= border_width * 2 + 1;
    rect_inside.size.height -= border_width * 2 + 1;

    // Construction des points
    ei_linked_point_t *frame_w = rounded_frame(&rect_inside, btn_ptr->corner_radius, border_width, 'w');

    // Dessin du bouton dans la surface principale
    ei_draw_polygon(surface, frame_w, btn_ptr->color, clipper);
    free_linked_point_list(frame_w);

    // Dessin du bouton dans l'offscreen
    frame_w = rounded_frame(&widget->screen_location, btn_ptr->corner_radius, border_width, 'w');
    ei_draw_polygon(pick_surface, frame_w, *(widget->pick_color), clipper);
    free_linked_point_list(frame_w);
    

    // Dessin de l'image
    if (btn_ptr->img != NULL) {
        ei_draw_img(surface, btn_ptr->img, btn_ptr->img_rect, clipper);
    }


    // Dessin du texte
    if (btn_ptr->text != NULL) {
        ei_point_t where;
        calculate_text_location(btn_ptr, &where);
        ei_draw_text(surface, &where, btn_ptr->text, btn_ptr->text_font, btn_ptr->text_color, clipper);
    }
}


void ei_button_geomnotifyfunc(ei_widget_t* widget){
    widget->geom_params->manager->runfunc(widget);
}


void ei_button_register_class () {

        ei_widgetclass_t *button = malloc(sizeof(ei_widgetclass_t));
        strcpy(button->name, "button");

        button->allocfunc = ei_button_allocfunc;
        button->releasefunc = ei_button_releasefunc;
        button->drawfunc = ei_button_drawfunc;
        button->setdefaultsfunc = ei_button_setdefaultsfunc;
        button->geomnotifyfunc = ei_button_geomnotifyfunc;
        button->next = NULL;

        ei_widgetclass_register(button);
}

ei_bool_t ei_press_button(ei_widget_t* widget, ei_event_t* event, void* user_param){

    ei_bind(ei_ev_mouse_buttonup, NULL, "all", ei_release_button, NULL);
    ei_bind(ei_ev_mouse_move, NULL, "all", ei_toggle_relief, NULL);

    pressed_button = widget;
    button_pressed = EI_TRUE;

    ei_button_t* btn_ptr = (ei_button_t*)widget;
    btn_ptr->relief = ei_relief_sunken;
    ei_app_invalidate_rect(&btn_ptr->widget.screen_location);

    return EI_TRUE;
}


ei_bool_t ei_click_button(ei_widget_t* widget, ei_event_t* event, void* user_param){
    ei_button_t* btn_ptr = (ei_button_t*)widget;
    if (btn_ptr->callback != NULL){
        btn_ptr->callback(widget, event, user_param);
    }
    return EI_TRUE;
}


ei_bool_t ei_release_button(ei_widget_t* widget, ei_event_t* event, void* user_param){

    ei_unbind(ei_ev_mouse_move, NULL, "all", ei_toggle_relief, NULL);
    ei_unbind(ei_ev_mouse_buttonup, NULL, "all", ei_release_button, NULL);

    if (widget != pressed_button){
        return EI_FALSE;
    }

    ei_button_t* btn_ptr = (ei_button_t*)pressed_button;
    ei_click_button(widget, event, btn_ptr->user_param);

    button_pressed = EI_FALSE;
    pressed_button = NULL;

    btn_ptr->relief = ei_relief_raised;
    ei_app_invalidate_rect(&btn_ptr->widget.screen_location);

    return EI_TRUE;
}



ei_bool_t ei_toggle_relief(struct ei_widget_t* widget, struct ei_event_t* event, void* user_param){
    if (pressed_button == NULL || (button_pressed == EI_FALSE && widget != pressed_button)){
        return EI_FALSE;
    }

    ei_button_t* btn_ptr = (ei_button_t*)pressed_button;
    if (widget != pressed_button){
        btn_ptr->relief = ei_relief_raised;
        button_pressed = EI_FALSE;
    } else {
        btn_ptr->relief = ei_relief_sunken;
        button_pressed = EI_TRUE;
    }
    ei_app_invalidate_rect(&btn_ptr->widget.screen_location);
    return EI_TRUE;
}
