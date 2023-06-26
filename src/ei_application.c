//
// Created by Yan LU on 20/05/2020.
//

#include <stdlib.h>

#include "ei_application.h"
#include "ei_frame.h"
#include "ei_button.h"
#include "ei_event.h"
#include "ei_geometrymanager.h"
#include "ei_app_state.h"
#include "ei_toplevel.h"


ei_widget_t * root_widget;
ei_surface_t	main_surface = NULL;
ei_surface_t    main_offscreen = NULL;

ei_rect_t main_rect;
ei_linked_rect_t* head_rects_to_update = NULL;
ei_linked_rect_t* tail_rects_to_update = NULL;

extern ei_bind_t* head_binds;
extern ei_bind_t* tail_binds;


void free_rects_list(){
    ei_linked_rect_t* current_rect = head_rects_to_update;
    ei_linked_rect_t* rect_to_free;
    while(current_rect != NULL){
        rect_to_free = current_rect;
        current_rect = current_rect->next;
        free(rect_to_free);
    }
    head_rects_to_update = NULL;
}


void ei_app_create(ei_size_t main_window_size, ei_bool_t fullscreen) {
    hw_init();

    main_surface = hw_create_window(main_window_size, fullscreen);
    main_offscreen = hw_surface_create(main_surface, main_window_size, EI_TRUE);
    main_rect = hw_surface_get_rect(main_surface);
    extern ei_rect_t root_rect;
    root_rect = main_rect;

    ei_frame_register_class();
    ei_button_register_class();
    ei_toplevel_register_class();
    ei_register_placer_manager();


    root_widget = ei_widget_create("frame", NULL, NULL, NULL);
    ei_root_configure(root_widget, main_surface);

}



void ei_app_free(void) {

    ei_widget_destroy(root_widget);
    root_widget = NULL;

    free_bind_list();
    free_list_wigdetclass();
    free_rects_list();
    free_manager_list();

    hw_surface_free(main_offscreen);
    hw_quit();
}




void ei_app_invalidate_rect(ei_rect_t* rect)
{

    if (head_rects_to_update ==  NULL){
        head_rects_to_update = malloc(sizeof(ei_linked_rect_t));
        head_rects_to_update->rect = *rect;
        head_rects_to_update->next = NULL;
        tail_rects_to_update = head_rects_to_update;
    } else {
        tail_rects_to_update->next = malloc(sizeof(ei_linked_rect_t));
        tail_rects_to_update = tail_rects_to_update->next;
        tail_rects_to_update->rect = *rect;
        tail_rects_to_update->next = NULL;
    }

}


void draw_widget_recursive(ei_widget_t* widget, ei_rect_t* clipper){

    if (widget == NULL){
        return;
    }

    ei_rect_t new_clipper = intersected_rect(*clipper, widget->screen_location);
    if (widget->parent != NULL) {
        new_clipper = intersected_rect(new_clipper, widget->parent->screen_location);
    }

    if (new_clipper.size.height == 0 || new_clipper.size.width == 0){
        return;
    }

    widget->wclass->drawfunc(widget, main_surface, main_offscreen, &new_clipper);

    ei_widget_t* child = widget->children_head;
    while(child != NULL){
        draw_widget_recursive(child, &new_clipper);
        child = child->next_sibling;
    }

}


void draw_all_widgets(){
    ei_linked_rect_t*  current_rect = head_rects_to_update;
    while (current_rect != NULL){
        draw_widget_recursive(root_widget, &current_rect->rect);
        current_rect = current_rect->next;
    }

}


void ei_app_quit_request(){
    app_run = EI_FALSE;
}


void ei_execute_callbacks(ei_widget_t* widget, ei_event_t* event){
    ei_bind_t* current_bind = head_binds;

    while(current_bind != NULL){
        if (current_bind->event_type == event->type){
            if (current_bind->picked_widget == widget || \
                (current_bind->tag != NULL && (strcmp(current_bind->tag, widget->wclass->name) == 0 || \
                strcmp(current_bind->tag, "all") == 0)))  {
                current_bind->callback(widget, event, current_bind->user_param);
            }
        }
        current_bind = current_bind->next;
    }
}


void bind_all_events(){
    ei_bind(ei_ev_mouse_buttondown, NULL, "button", ei_press_button, NULL);
    ei_bind(ei_ev_mouse_buttondown, NULL, "toplevel", ei_click_toplevel, NULL);
}


void ei_app_run(void) {
    ei_event_t		event;
    ei_widget_t* picked_widget;

    hw_surface_lock(main_surface);
    hw_surface_lock(main_offscreen);


    ei_app_invalidate_rect(&main_rect);
    draw_all_widgets();
    free_rects_list();
    //ei_copy_surface(main_surface, &main_rect, main_offscreen, &main_rect, EI_TRUE);

    hw_surface_unlock(main_surface);
    hw_surface_unlock(main_offscreen);

    hw_surface_update_rects(main_surface, NULL);

    bind_all_events();
    while (app_run == EI_TRUE){
        hw_event_wait_next(&event);

        hw_surface_lock(main_surface);
        hw_surface_lock(main_offscreen);
        picked_widget = ei_widget_pick(&(event.param.mouse.where));
        ei_execute_callbacks(picked_widget, &event);

        draw_all_widgets();

        hw_surface_unlock(main_surface);
        hw_surface_unlock(main_offscreen);

        if (head_rects_to_update != NULL) {
            hw_surface_update_rects(main_surface, head_rects_to_update);
        }
        free_rects_list();
    }

}


ei_widget_t* ei_app_root_widget(void){
    return root_widget;
}


ei_surface_t ei_app_root_surface()
{
    return main_surface;
}

ei_surface_t ei_app_root_offscreen()
{
    return main_offscreen;
}