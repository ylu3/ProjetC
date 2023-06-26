//
// Created by Yan LU on 20/05/2020.
//
#include <stdlib.h>
#include <string.h>

#include "ei_application.h"
#include "ei_frame.h"
#include "ei_button.h"
#include "ei_widgetclass.h"
#include "ei_toplevel.h"
#include "ei_geometrymanager.h"
#include "ei_toplevel.h"

#define SET_ATTR(attr,cls) if (attr != NULL) { cls->attr = *attr; }

// Compteur global utilisé pour l'identification offscreen des widgets
uint32_t pick_id_counter = 0;


ei_color_t int_to_color(u_int32_t  n)
{
    ei_color_t color;
    color.red = n % 256;
    n = n / 256;
    color.green	= n % 256;
    n = n / 256;
    color.blue	= n % 256;
    color.alpha	= 255;
    return color;
}

ei_widget_t* get_widget_from_id(ei_widget_t* widget, uint32_t id){
    if (widget == NULL || widget->pick_id == id){
        return widget;
    } else {
        ei_widget_t* picked_widget = NULL;
        widget = widget->children_head;
        while(picked_widget == NULL && widget!= NULL){
            picked_widget = get_widget_from_id(widget, id);
            widget = widget->next_sibling;
        }

        return picked_widget;
    }
}

extern ei_surface_t main_offscreen;

ei_widget_t* ei_widget_pick(ei_point_t* where)
{
    uint8_t* color_ptr = hw_surface_get_buffer(main_offscreen);
    ei_size_t offscr_size = hw_surface_get_size(main_offscreen);
    color_ptr += ((where->y) * offscr_size.width + where->x) * 4;

    int red_position, green_position, blue_position, alpha;
    hw_surface_get_channel_indices(main_offscreen, &red_position, &green_position, &blue_position, &alpha);

    uint8_t* red_ptr =  color_ptr + red_position;
    uint8_t* green_ptr = color_ptr + green_position;
    uint8_t* blue_ptr = color_ptr + blue_position;

    uint32_t n = *red_ptr + *green_ptr * 256 + *blue_ptr * 256 * 256;
    return get_widget_from_id(ei_app_root_widget(), n);
}


// Conversion entier -> Couleur pour l'offscreen:
void add_child(ei_widget_t* parent, ei_widget_t* child){
    if (parent->children_head == NULL){
        parent->children_head = child;
        parent->children_tail = child;
    } else {
        parent->children_tail->next_sibling = child;
        parent->children_tail = child;
    }

}

ei_widget_t*		ei_widget_create		(ei_widgetclass_name_t	class_name,
                                             ei_widget_t*		parent,
                                             void*			user_data,
                                             ei_widget_destructor_t destructor){

    ei_widgetclass_t* ptr_class = ei_widgetclass_from_name(class_name);
    if (ptr_class == NULL)
        return NULL;

    ei_widget_t* widget_ptr = ptr_class->allocfunc();
    widget_ptr->parent = parent;
    widget_ptr->wclass = ptr_class;
    widget_ptr->destructor = destructor;

    // 2.Paramètres offscreen
    widget_ptr->pick_id	= pick_id_counter;
    pick_id_counter	+= 32;
    ei_color_t pick_color	= int_to_color(widget_ptr->pick_id);
    widget_ptr->pick_color	= malloc(sizeof(ei_color_t));
    *(widget_ptr->pick_color) = pick_color;

    widget_ptr->next_sibling = NULL;
    widget_ptr->children_tail = NULL;
    widget_ptr->children_head = NULL;
    widget_ptr->geom_params = NULL;

    widget_ptr->requested_size = ei_size_zero();
    widget_ptr->screen_location.size = ei_size_zero();
    widget_ptr->screen_location.top_left = ei_point_zero();
    widget_ptr->content_rect = &widget_ptr->screen_location;

    widget_ptr->wclass->setdefaultsfunc(widget_ptr);


    // Chainage dans les fils
    if (parent != NULL){
        add_child(widget_ptr->parent, widget_ptr);
    }

    return widget_ptr;
}

void			ei_widget_destroy		(ei_widget_t*		widget){
    if (widget == NULL) {
        return;
    }

    ei_widget_t* child = widget->children_head;
    while(child != NULL){
        ei_widget_destroy(child);
        child = child->next_sibling;
    }

    ei_widget_t* parent = widget->parent;
    if (parent != NULL) {
        if (parent->children_head == widget) {
            if (widget->next_sibling == NULL) {
                parent->children_head = NULL;
                parent->children_tail = NULL;
            } else {
                parent->children_head = widget->next_sibling;
            }
        } else {
            ei_widget_t* last_widget = NULL;
            ei_widget_t* current_widget = widget->children_head;
            while (current_widget != widget) {
                last_widget = current_widget;
                current_widget = current_widget->next_sibling;
            }
            if (widget->next_sibling == NULL) {
                parent->children_tail = last_widget;
                last_widget->next_sibling = NULL;
            } else {
                last_widget->next_sibling = current_widget->next_sibling;
            }
        }
    }

    if (widget->content_rect != &widget->screen_location){
        free(widget->content_rect);
    }

    // Libération des attributs généraux du widget
    ei_geometrymanager_unmap(widget); // Libération du gestionnaire de géométrie
    free(widget->pick_color);

    // Libération des attributs spécifiques à la classe du widget
    ei_widgetclass_t* class = widget->wclass;
    class->releasefunc(widget);

    // Libération définitive du widget
    free(widget);
}

void ei_frame_configure(ei_widget_t*		widget,
                        ei_size_t*			requested_size,
                        const ei_color_t*	bg_color,
                        int*				border_width,
                        ei_relief_t*		relief,
                        char**				text,
                        ei_font_t*			text_font,
                        ei_color_t*			text_color,
                        ei_anchor_t*		text_anchor,
                        ei_surface_t*		img,
                        ei_rect_t**			img_rect,
                        ei_anchor_t*		img_anchor)
{
    if (widget == NULL) {
        return;
    }

    ei_frame_t* frm_ptr = (ei_frame_t*) widget;
    SET_ATTR(requested_size,widget);
    widget->requested_size.height = widget->requested_size.height < 0 ? 0 : widget->requested_size.height;
    widget->requested_size.width = widget->requested_size.width < 0 ? 0 : widget->requested_size.width;
    //widget->screen_location.size = widget->requested_size;
    SET_ATTR(bg_color,frm_ptr);
    SET_ATTR(border_width,frm_ptr);
    SET_ATTR(relief,frm_ptr);
    SET_ATTR(text,frm_ptr);
    SET_ATTR(text_font,frm_ptr);
    SET_ATTR(text_color,frm_ptr);
    SET_ATTR(text_anchor,frm_ptr);
    SET_ATTR(img,frm_ptr);
    if (img_rect != NULL) {
        frm_ptr->img_rect = malloc(sizeof(ei_rect_t));
        *(frm_ptr->img_rect) = **img_rect;
    }
    //SET_ATTR(img_rect,frm_ptr);
    SET_ATTR(img_anchor,frm_ptr);

}


void			ei_button_configure		(ei_widget_t*		widget,
                                            ei_size_t*		requested_size,
                                            const ei_color_t*	color,
                                            int*			border_width,
                                            int*			corner_radius,
                                            ei_relief_t*		relief,
                                            char**			text,
                                            ei_font_t*		text_font,
                                            ei_color_t*		text_color,
                                            ei_anchor_t*		text_anchor,
                                            ei_surface_t*		img,
                                            ei_rect_t**		img_rect,
                                            ei_anchor_t*		img_anchor,
                                            ei_callback_t*		callback,
                                            void**			user_param){
    if (widget == NULL) {
        return;
    }

    ei_button_t* btn_ptr = (ei_button_t*) widget;
    SET_ATTR(requested_size,widget);
    widget->requested_size.height = widget->requested_size.height < 0 ? 0 : widget->requested_size.height;
    widget->requested_size.width = widget->requested_size.width < 0 ? 0 : widget->requested_size.width;
    SET_ATTR(color,btn_ptr);
    SET_ATTR(border_width,btn_ptr);
    SET_ATTR(corner_radius,btn_ptr);
    SET_ATTR(relief,btn_ptr);
    SET_ATTR(text,btn_ptr);
    SET_ATTR(text_font,btn_ptr);
    SET_ATTR(text_color,btn_ptr);
    SET_ATTR(text_anchor,btn_ptr);
    SET_ATTR(img,btn_ptr);
    if (img_rect != NULL) {
        btn_ptr->img_rect = malloc(sizeof(ei_rect_t));
        *(btn_ptr->img_rect) = **img_rect;
    }
    //SET_ATTR(img_rect,btn_ptr);
    SET_ATTR(img_anchor,btn_ptr);
    SET_ATTR(callback,btn_ptr);
    SET_ATTR(user_param,btn_ptr);
}

