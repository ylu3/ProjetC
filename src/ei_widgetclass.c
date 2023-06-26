//
// Created by Yan LU on 20/05/2020.
//
#include "ei_widgetclass.h"
#include "ei_widget.h"
#include <string.h>
#include <stdlib.h>

ei_widgetclass_t* head_widgetclass = NULL;
ei_widgetclass_t* tail_widgetclass = NULL;

void widget_set_parent(ei_widget_t* widget, ei_widget_t* parent)
{
    widget->parent = parent;
}

ei_widgetclass_t* ei_widgetclass_from_name(ei_widgetclass_name_t name) {
    if (head_widgetclass == NULL) {
        return NULL;
    }
    ei_widgetclass_t* current_class = head_widgetclass;
    while (current_class != NULL) {
        if (!strcmp(ei_widgetclass_stringname(current_class->name), ei_widgetclass_stringname(name))){
            return current_class;
        }
        current_class = current_class->next;
    }
    return NULL;
}


void	ei_widgetclass_register		(ei_widgetclass_t* widgetclass) {
    if (head_widgetclass == NULL) {
        head_widgetclass = widgetclass;
        tail_widgetclass = widgetclass;
    }else {
        tail_widgetclass->next = widgetclass;
        tail_widgetclass = widgetclass;
    }

};


void free_list_wigdetclass(){
    ei_widgetclass_t* current_widgetclass = head_widgetclass;
    ei_widgetclass_t* widgetclass_to_free = NULL;
    while(current_widgetclass != NULL){
        widgetclass_to_free = current_widgetclass;
        current_widgetclass = current_widgetclass->next;
        free(widgetclass_to_free);
    }
    head_widgetclass = NULL;
    tail_widgetclass = NULL;
}
    
