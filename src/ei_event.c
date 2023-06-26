//
// Created by tsouf on 22/05/2020.
//
#include <stdlib.h>
#include "ei_event.h"

ei_bind_t* head_binds = NULL;
ei_bind_t* tail_binds = NULL;

void add_bind(ei_bind_t* bind){
    if (head_binds == NULL){
        head_binds = bind;
        tail_binds = bind;
    } else {
        tail_binds->next = bind;
        tail_binds = bind;
    }
}

void		ei_bind			(ei_eventtype_t		eventtype,
                                ei_widget_t*		widget,
                                ei_tag_t		tag,
                                ei_callback_t		callback,
                                void*			user_param) {
    //Binds a callback to an event type and a widget or a tag
    //Fonction qui ressemble à register
    ei_bind_t* bind = malloc(sizeof(ei_bind_t));
    bind->event_type = eventtype;
    bind->picked_widget = widget;
    bind->tag = tag;
    bind->callback = callback;
    bind->user_param = user_param;
    bind->next = NULL;
    add_bind(bind);
}


void		ei_unbind		(ei_eventtype_t		eventtype,
                              ei_widget_t*		widget,
                              ei_tag_t		tag,
                              ei_callback_t		callback,
                              void*			user_param){
    //Unbinds a callback from an event type and widget or tag.
    ei_bind_t* last_bind = NULL;
    ei_bind_t* current_bind = head_binds;
    while(current_bind != NULL){
        if(current_bind->picked_widget == widget && current_bind->event_type == eventtype \
            && current_bind->callback == callback && current_bind->tag == tag){
            if (last_bind != NULL && current_bind->next != NULL){
                last_bind->next = current_bind->next;
            } else {
                 if (last_bind == NULL){
                     head_binds = current_bind->next;
                 }
                 if (current_bind->next == NULL){
                     tail_binds = last_bind;
                     tail_binds->next = NULL;
                 }
            }
            break;
        }
        last_bind = current_bind;
        current_bind = current_bind->next;
    }

}


void free_bind_list(){
    ei_bind_t* current_bind = head_binds;
    ei_bind_t* bind_to_free = NULL;
    while(current_bind != NULL){
        bind_to_free = current_bind;
        current_bind = current_bind->next;
        free(bind_to_free);
    }
    head_binds = NULL;
    tail_binds = NULL;
}