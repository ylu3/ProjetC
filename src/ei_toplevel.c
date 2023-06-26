/*
  Created by YAN LU on 29/05/2020.

*/

#include <stdlib.h>

#include "ei_application.h"
#include "ei_types.h"
#include "ei_draw.h"
#include "ei_toplevel.h"
#include "ei_geometrymanager.h"
#include "ei_event.h"
#include "ei_app_state.h"

#define SET_ATTR(attr,cls) if (attr != NULL) { cls->attr = *attr; }

extern ei_rect_t root_rect;
int default_titlebar_height = 30;
int default_titlebar_radius = 15;
int default_resize_square_size = 10;
int btn_x = 5;
int btn_y = 8;

ei_size_t default_close_btn_size = {20, 20};
ei_color_t default_close_btn_color = { 0xff, 0x00, 0x00, 0xff };
int default_border_width = 5;
int default_close_btn_border_width = 2;
int default_close_btn_radius = 8;
ei_relief_t defautlt_close_btn_relief = ei_relief_raised;
ei_size_t default_topl_size = {320, 240};

ei_bool_t close_toplevel(struct ei_widget_t* widget, struct ei_event_t* event, void* top_level);
ei_callback_t close_callback = close_toplevel;

void* ei_toplevel_allocfunc(void) {
        return calloc(1, sizeof(ei_toplevel_t));
}


void ei_toplevel_releasefunc(struct ei_widget_t*	widget){
        struct ei_toplevel_t* topl_ptr = (ei_toplevel_t*) widget;
        if (topl_ptr == NULL){
                return;
        }

        if(topl_ptr->min_size != NULL){
            free(topl_ptr->min_size);
        }
}


void calculate_content_rect(ei_widget_t* widget, int border_width){
    if (widget->content_rect == NULL){
        return;
    }
    widget->content_rect->size.height = widget->screen_location.size.height - default_titlebar_height - border_width * 2;
    widget->content_rect->size.width = widget->screen_location.size.width - border_width * 2;
    widget->content_rect->top_left.x = widget->screen_location.top_left.x + border_width;
    widget->content_rect->top_left.y = widget->screen_location.top_left.y + default_titlebar_height + border_width;
}


void ei_toplevel_setdefaultsfunc(struct ei_widget_t*  widget){
    widget->requested_size = default_topl_size;
    widget->content_rect = malloc(sizeof(ei_rect_t));
    calculate_content_rect(widget, default_border_width);

    ei_toplevel_t* topl_ptr = (ei_toplevel_t*)widget;
    topl_ptr->bg_color = white;
    topl_ptr->border_width = default_border_width;
    topl_ptr->title = "Window";
    topl_ptr->closable = EI_TRUE;
    topl_ptr->close_btn_ptr = (ei_button_t*)ei_widget_create("button", widget, NULL, NULL);
    ei_button_configure((ei_widget_t*)topl_ptr->close_btn_ptr, &default_close_btn_size, &default_close_btn_color, \
            &default_close_btn_border_width, &default_close_btn_radius, &defautlt_close_btn_relief, NULL, NULL, NULL,\
            NULL, NULL, NULL, NULL, &close_callback, NULL);
    ei_place((ei_widget_t *) topl_ptr->close_btn_ptr, NULL, &btn_x, &btn_y, NULL, NULL, NULL, NULL, NULL, NULL);

    topl_ptr->resizable = ei_axis_none;
    topl_ptr->min_size = malloc(sizeof(ei_size_t));
    int twidth;
    int theight;
    hw_text_compute_size(topl_ptr->title, ei_default_font, &twidth, &theight);
    topl_ptr->min_size->width = twidth + 50;
    topl_ptr->min_size->height = 100;

}


void recalculate_children(ei_widget_t* widget){
    if (widget ==NULL) {
        return;
    }
    if (widget->geom_params != NULL){
        widget->geom_params->manager->runfunc(widget);
    }

    ei_widget_t* child = widget->children_head;
    while(child != NULL){
        recalculate_children(child);
        child = child->next_sibling;
    }
    ei_widget_t* next = widget->next_sibling;
    while(next != NULL){
        recalculate_children(next);
        next = next->next_sibling;
    }

}


void ei_toplevel_geomnotifyfunc(ei_widget_t* widget){
    ei_toplevel_t* topl_ptr = (ei_toplevel_t*)widget;
    recalculate_children(widget->children_head);
}

ei_rect_t get_rect_corner(ei_widget_t* widget){
    ei_rect_t rect;
    rect.top_left.x = widget->screen_location.top_left.x + widget->screen_location.size.width - default_resize_square_size;
    rect.top_left.y = widget->screen_location.top_left.y + widget->screen_location.size.height - default_resize_square_size;
    rect.size.height = default_resize_square_size;
    rect.size.width = default_resize_square_size;
    return rect;
}

void ei_toplevel_drawfunc(struct ei_widget_t* widget,
                          ei_surface_t surface,
                          ei_surface_t pick_surface,
                          ei_rect_t* clipper) {
    ei_linked_point_t *border_toplevel = get_toplevel_border(widget);
    ei_toplevel_t *topl_ptr = (ei_toplevel_t *) widget;
    ei_draw_polygon(surface, border_toplevel, light_gray, clipper);
    calculate_content_rect(widget, topl_ptr->border_width);
    ei_fill(surface, &topl_ptr->bg_color, widget->content_rect);
    if (topl_ptr->resizable) {
        ei_rect_t rect = get_rect_corner(widget);
        ei_fill(surface, &light_gray, &rect);
    }
    ei_point_t where = ei_point(widget->screen_location.top_left.x + 30, widget->screen_location.top_left.y + 4);
    ei_draw_text(surface, &where, topl_ptr->title, ei_default_font, white, clipper);


    if (topl_ptr->closable){
        topl_ptr->close_btn_ptr->widget.wclass->drawfunc((ei_widget_t*)topl_ptr->close_btn_ptr, surface, pick_surface, clipper);
    }

    ei_draw_polygon(pick_surface, border_toplevel, *widget->pick_color, clipper);
    free_linked_point_list(border_toplevel);

}


void ei_toplevel_register_class (void)
{
    ei_widgetclass_t* topl_class_ptr = malloc(sizeof(ei_widgetclass_t));
    strncpy(ei_widgetclass_stringname(topl_class_ptr->name), "toplevel", 20);

    topl_class_ptr->allocfunc = ei_toplevel_allocfunc;
    topl_class_ptr->releasefunc = ei_toplevel_releasefunc;
    topl_class_ptr->drawfunc = ei_toplevel_drawfunc;
    topl_class_ptr->setdefaultsfunc = ei_toplevel_setdefaultsfunc;
    topl_class_ptr->geomnotifyfunc = ei_toplevel_geomnotifyfunc;
    topl_class_ptr->next = NULL;

    ei_widgetclass_register(topl_class_ptr);
}


ei_bool_t close_toplevel(struct ei_widget_t* widget, struct ei_event_t* event, void* user_param){
    if(widget == NULL || widget->parent == NULL){
        return EI_FALSE;
    }
    ei_widget_destroy(widget->parent);
    return EI_TRUE;
}


void	ei_toplevel_configure		(ei_widget_t*		widget,
                                      ei_size_t*		requested_size,
                                      ei_color_t*		bg_color,
                                      int*			border_width,
                                      char**			title,
                                      ei_bool_t*		closable,
                                      ei_axis_set_t*		resizable,
                                      ei_size_t**		min_size){
    if (widget == NULL) {
        return;
    }

    ei_toplevel_t* topl_ptr = (ei_toplevel_t*) widget;
    SET_ATTR(requested_size, widget);
    widget->requested_size.height = widget->requested_size.height < 0 ? 0 : widget->requested_size.height;
    widget->requested_size.width = widget->requested_size.width < 0 ? 0 : widget->requested_size.width;

    SET_ATTR(bg_color, topl_ptr);
    SET_ATTR(border_width, topl_ptr);
    SET_ATTR(title, topl_ptr);
    SET_ATTR(closable, topl_ptr);
    if (topl_ptr->closable == EI_FALSE && topl_ptr->close_btn_ptr != NULL) {
        ei_widget_destroy((ei_widget_t *) topl_ptr->close_btn_ptr);
    }
    SET_ATTR(resizable, topl_ptr);
    /*
    if (min_size != NULL){
        if (topl_ptr->min_size != NULL){
            free(topl_ptr);
            if (*min_size != NULL){
                topl_ptr->min_size = malloc(sizeof(ei_size_t));
                *topl_ptr->min_size = **min_size;
            }
        } else {
            if (*min_size != NULL){
                topl_ptr->min_size = malloc(sizeof(ei_size_t));
                *topl_ptr->min_size = **min_size;
            }
        }
    }
*/
    SET_ATTR(min_size, topl_ptr);

    if(widget->wclass->geomnotifyfunc != NULL) {
        widget->wclass->geomnotifyfunc(widget);
    }

    widget->requested_size.width += 2 * topl_ptr->border_width;
    widget->requested_size.height += 2 * topl_ptr->border_width + default_titlebar_height;


}


ei_bool_t ei_move_toplevel(struct ei_widget_t* widget, struct ei_event_t* event, void* user_param){
    ei_point_t new_mouse_position = event->param.mouse.where;
    int delta_x = new_mouse_position.x - last_mouse_position.x;
    int delta_y = new_mouse_position.y - last_mouse_position.y;
    int old_x = clicked_toplevel->screen_location.top_left.x;
    int old_y = clicked_toplevel->screen_location.top_left.y;
    int height = clicked_toplevel->screen_location.size.height;
    int width = clicked_toplevel->screen_location.size.width;

    int new_position_x = old_x + delta_x;
    if (new_position_x < 0 ){
        new_position_x = 0;
    }
    if (new_position_x + width > root_rect.size.width){
        new_position_x = root_rect.size.width - width;
    }

    int new_position_y = old_y + delta_y;
    if (new_position_y < 0 ){
        new_position_y = 0;
    }
    if (new_position_y + height > root_rect.size.height){
        new_position_y = root_rect.size.height - height;
    }

    ei_rect_t rect_to_update;
    if (new_position_x < old_x){
        rect_to_update.top_left.x = new_position_x;
        rect_to_update.size.width = old_x + width - new_position_x;
    } else {
        rect_to_update.top_left.x = old_x;
        rect_to_update.size.width = new_position_x + width - old_x;
    }
    if (new_position_y < old_y){
        rect_to_update.top_left.y = new_position_y;
        rect_to_update.size.height = old_y + height - new_position_y;
    } else {
        rect_to_update.top_left.y = old_y;
        rect_to_update.size.height = new_position_y + height - old_y;
    }

    clicked_toplevel->screen_location.top_left.x = new_position_x;
    clicked_toplevel->screen_location.top_left.y = new_position_y;

    ei_app_invalidate_rect(&rect_to_update);

    if (clicked_toplevel->wclass->geomnotifyfunc != NULL){
        clicked_toplevel->wclass->geomnotifyfunc(clicked_toplevel);
    }

    last_mouse_position = new_mouse_position;

    return EI_TRUE;
}


ei_bool_t ei_release_toplevel(struct ei_widget_t* widget, struct ei_event_t* event, void* user_param){
    ei_unbind(ei_ev_mouse_move, NULL, "all", ei_move_toplevel, NULL);
    ei_unbind(ei_ev_mouse_buttonup, NULL, "all", ei_release_toplevel, NULL);

    clicked_toplevel = NULL;

    return EI_TRUE;
}


ei_bool_t ei_drag_toplevel(struct ei_widget_t* widget, struct ei_event_t* event, void* user_param){
    ei_point_t new_mouse_position = event->param.mouse.where;
    int delta_width = new_mouse_position.x - last_mouse_position.x;
    int delta_height = new_mouse_position.y - last_mouse_position.y;
    //printf("%d\n", delta_height);
    int old_width = clicked_toplevel->screen_location.size.width;
    int old_height = clicked_toplevel->screen_location.size.height;

    clicked_toplevel->screen_location.size.width += delta_width;
    clicked_toplevel->screen_location.size.height += delta_height;
    ei_toplevel_t* topl_ptr = (ei_toplevel_t*)clicked_toplevel;
    if (clicked_toplevel->screen_location.size.height < topl_ptr->min_size->height){
        clicked_toplevel->screen_location.size.height = topl_ptr->min_size->height;
    }
    if (clicked_toplevel->screen_location.size.width < topl_ptr->min_size->width){
        clicked_toplevel->screen_location.size.width = topl_ptr->min_size->width;
    }
    if (clicked_toplevel->screen_location.size.height + clicked_toplevel->screen_location.top_left.y > root_rect.size.height){
        clicked_toplevel->screen_location.size.height = root_rect.size.height - clicked_toplevel->screen_location.top_left.y;
    }
    if (clicked_toplevel->screen_location.size.width + clicked_toplevel->screen_location.top_left.x > root_rect.size.width){
        clicked_toplevel->screen_location.size.width = root_rect.size.width - clicked_toplevel->screen_location.top_left.x;
    }

    ei_rect_t rect_to_update;
    rect_to_update.top_left = clicked_toplevel->screen_location.top_left;
    rect_to_update.size.width = delta_width > 0 ? clicked_toplevel->screen_location.size.width : old_width;
    rect_to_update.size.height = delta_height > 0 ? clicked_toplevel->screen_location.size.height : old_height;
    ei_app_invalidate_rect(&rect_to_update);

    if (clicked_toplevel->wclass->geomnotifyfunc != NULL){
        clicked_toplevel->wclass->geomnotifyfunc(clicked_toplevel);
    }
    last_mouse_position = new_mouse_position;

    calculate_content_rect(clicked_toplevel, topl_ptr->border_width);

    return EI_TRUE;
}


ei_bool_t ei_release_drag_toplevel(struct ei_widget_t* widget, struct ei_event_t* event, void* user_param){
    ei_unbind(ei_ev_mouse_move, NULL, "all", ei_drag_toplevel, NULL);
    ei_unbind(ei_ev_mouse_buttonup, NULL, "all", ei_release_drag_toplevel, NULL);

    clicked_toplevel = NULL;

    return EI_TRUE;
}


ei_bool_t ei_click_toplevel(struct ei_widget_t* widget, struct ei_event_t* event, void* user_param){
    ei_point_t mouse_position = event->param.mouse.where;
    clicked_toplevel = widget;
    if (mouse_position.y < widget->content_rect->top_left.y){
        ei_bind(ei_ev_mouse_move, NULL, "all", ei_move_toplevel, NULL);
        ei_bind(ei_ev_mouse_buttonup, NULL, "all", ei_release_toplevel, NULL);
        last_mouse_position = mouse_position;
    } else {
        ei_rect_t rect = get_rect_corner(widget);
        if(mouse_position.x >= rect.top_left.x && mouse_position.x <= rect.top_left.x + rect.size.width && \
            mouse_position.y >= rect.top_left.y && mouse_position.y <= rect.top_left.y + rect.size.height){
            ei_bind(ei_ev_mouse_move, NULL, "all", ei_drag_toplevel, NULL);
            ei_bind(ei_ev_mouse_buttonup, NULL, "all", ei_release_drag_toplevel, NULL);
            last_mouse_position = mouse_position;
        }
    }
    return EI_TRUE;
}