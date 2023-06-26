//
// Created by Yan LU on 21/05/2020.
//
#include <string.h>
#include <stdlib.h>

#include "ei_geometrymanager.h"
#include "ei_application.h"
#include "ei_utils.h"
#include "ei_toplevel.h"

#define SET_ATTR(attr,cls) if (attr != NULL) { cls->attr = *attr; }

ei_geometrymanager_t* head_manager = NULL;
ei_geometrymanager_t* tail_manager = NULL;

void ei_geometrymanager_register (ei_geometrymanager_t* geometrymanager){
    if (head_manager == NULL){
        head_manager = geometrymanager;
        tail_manager = geometrymanager;
    }else{
        tail_manager->next = geometrymanager;
        tail_manager = geometrymanager;
    }

}

ei_geometrymanager_t* ei_geometrymanager_from_name(ei_geometrymanager_name_t name){
    if (head_manager == NULL){
        return NULL;
    }
    ei_geometrymanager_t* current_manager = head_manager;
    while (current_manager != NULL) {
        if (!strcmp(current_manager->name, name)){
            return current_manager;
        }
        current_manager = current_manager->next;
    }
    return NULL;
}


void    ei_placer_runfunc (struct ei_widget_t*  widget){
    if (widget == NULL || widget->geom_params == NULL || widget->parent == NULL){
        return;
    }
    //ei_app_invalidate_rect(&widget->screen_location);
    ei_placer_t* placer_ptr = (ei_placer_t*) widget->geom_params;
    ei_size_t* size = &widget->screen_location.size;
    ei_point_t* topleft = &widget->screen_location.top_left;

    ei_size_t parent_size;
    ei_point_t parent_topleft;
    if (strcmp(widget->parent->wclass->name, "toplevel") == 0 && widget != widget->parent->children_head){
        ei_toplevel_t* topl_ptr = (ei_toplevel_t*)widget->parent;
        parent_size = widget->parent->screen_location.size;
        parent_topleft = widget->parent->screen_location.top_left;
        extern int default_titlebar_height;
        parent_size.height -= (2 * topl_ptr->border_width + default_titlebar_height);
        parent_size.width -= 2 * topl_ptr->border_width;
        parent_topleft.x += topl_ptr->border_width;
        parent_topleft.y += topl_ptr->border_width + default_titlebar_height;
    } else {
        parent_size = widget->parent->screen_location.size;
        parent_topleft = widget->parent->screen_location.top_left;
    }


    size->width = parent_size.width * placer_ptr->rel_width + placer_ptr->width;
    size->height = parent_size.height * placer_ptr->rel_height + placer_ptr->height;

    switch (placer_ptr->anchor) {
        case(ei_anc_northwest):
            topleft->x = parent_topleft.x + parent_size.width * placer_ptr->rel_x + placer_ptr->x;
            topleft->y = parent_topleft.y + parent_size.height * placer_ptr->rel_y + placer_ptr->y;
            break;
        case(ei_anc_north):
            topleft->x = parent_topleft.x + parent_size.width * placer_ptr->rel_x + placer_ptr->x - size->width/2;
            topleft->y = parent_topleft.y + parent_size.height * placer_ptr->rel_y + placer_ptr->y;
            break;
        case(ei_anc_northeast):
            topleft->x = parent_topleft.x + parent_size.width * placer_ptr->rel_x + placer_ptr->x - size->width;
            topleft->y = parent_topleft.y + parent_size.height * placer_ptr->rel_y + placer_ptr->y;
            break;
        case(ei_anc_center):
            topleft->x = parent_topleft.x + parent_size.width * placer_ptr->rel_x + placer_ptr->x - size->width/2;
            topleft->y = parent_topleft.y + parent_size.height * placer_ptr->rel_y + placer_ptr->y - size->height/2;
            break;
        case(ei_anc_east):
            topleft->x = parent_topleft.x + parent_size.width * placer_ptr->rel_x + placer_ptr->x - size->width;
            topleft->y = parent_topleft.y + parent_size.height * placer_ptr->rel_y + placer_ptr->y - size->height/2;
            break;
        case(ei_anc_south):
            topleft->x = parent_topleft.x + parent_size.width * placer_ptr->rel_x + placer_ptr->x - size->width/2;
            topleft->y = parent_topleft.y + parent_size.height * placer_ptr->rel_y + placer_ptr->y - size->height;
            break;
        case(ei_anc_none):
            break;
        case(ei_anc_west):
            topleft->x = parent_topleft.x + parent_size.width * placer_ptr->rel_x + placer_ptr->x;
            topleft->y = parent_topleft.y + parent_size.height * placer_ptr->rel_y + placer_ptr->y - size->height/2;
            break;
        case(ei_anc_southwest):
            topleft->x = parent_topleft.x + parent_size.width * placer_ptr->rel_x + placer_ptr->x;
            topleft->y = parent_topleft.y + parent_size.height * placer_ptr->rel_y + placer_ptr->y - size->height;
            break;
        case(ei_anc_southeast):
            topleft->x = parent_topleft.x + parent_size.width * placer_ptr->rel_x + placer_ptr->x - size->width;
            topleft->y = parent_topleft.y + parent_size.height * placer_ptr->rel_y + placer_ptr->y - size->height;
            break;
    }
    //ei_app_invalidate_rect(&widget->screen_location);


}

void ei_placer_releasefunc(struct ei_widget_t*  widget){
    if (widget == NULL) {
        return;
    }
    if (widget->geom_params != NULL) {
        free((ei_placer_t*)widget->geom_params);
        widget->geom_params = NULL;
    }
}


void placer_set_default(ei_widget_t* widget)
{
    ei_placer_t* placer_prt = (ei_placer_t*) widget->geom_params;
    placer_prt->geometry_param.manager = ei_geometrymanager_from_name("placer");
    placer_prt->anchor = ei_anc_northwest;
    placer_prt->x = 0;
    placer_prt->y = 0;
    placer_prt->width = widget->requested_size.width;
    placer_prt->height = widget->requested_size.height;
    placer_prt->rel_height = 0;
    placer_prt->rel_width = 0;
    placer_prt->rel_x = 0;
    placer_prt->rel_y = 0;
}



void ei_register_placer_manager(void){
    ei_geometrymanager_t* placer_ptr = malloc(sizeof(ei_placer_t));

    strncpy(placer_ptr->name, "placer", 20);
    placer_ptr->runfunc = ei_placer_runfunc;
    placer_ptr->releasefunc = ei_placer_releasefunc;
    placer_ptr->next = NULL;

    ei_geometrymanager_register(placer_ptr);

}

void    ei_geometrymanager_unmap(ei_widget_t*   widget){
    if(widget == NULL || widget->geom_params == NULL){
        return;
    }
    ei_app_invalidate_rect(&widget->screen_location);
    widget->geom_params->manager->releasefunc(widget);
    widget->screen_location.size = ei_size_zero();
    widget->screen_location.top_left = ei_point_zero();
}


void			placer_set_attr	        (    ei_placer_t*               placer_ptr,
                                                                   ei_anchor_t*		anchor,
                                                                   int*			x,
                                                                   int*			y,
                                                                   int*			width,
                                                                   int*			height,
                                                                   float*			rel_x,
                                                                   float*			rel_y,
                                                                   float*			rel_width,
                                                                   float*			rel_height){
    SET_ATTR(anchor,placer_ptr);
    SET_ATTR(x,placer_ptr);
    SET_ATTR(y,placer_ptr);
    SET_ATTR(width,placer_ptr);
    SET_ATTR(height,placer_ptr);
    SET_ATTR(rel_x,placer_ptr);
    SET_ATTR(rel_y,placer_ptr);
    SET_ATTR(rel_width,placer_ptr);
    SET_ATTR(rel_height,placer_ptr);
}


void add_placer(ei_widget_t* widget){
    ei_placer_t* placer_ptr = malloc(sizeof(ei_placer_t));
    widget->geom_params = (ei_geometry_param_t*) placer_ptr;
    placer_set_default(widget);
}


void			ei_place			(ei_widget_t*		widget,
                                                             ei_anchor_t*		anchor,
                                                             int*			x,
                                                             int*			y,
                                                             int*			width,
                                                             int*			height,
                                                             float*			rel_x,
                                                             float*			rel_y,
                                                             float*			rel_width,
                                                             float*			rel_height){

    if (widget == NULL){
        return;
    }

    if (widget->geom_params != NULL){
        ei_geometrymanager_unmap(widget);
    }

    ei_placer_t* placer_ptr;
    add_placer(widget);
    placer_ptr = (ei_placer_t* )widget->geom_params;

    placer_set_attr(placer_ptr, anchor, x, y, width, height, rel_x, rel_y, rel_width, rel_height);

    widget->geom_params->manager->runfunc(widget);
    ei_app_invalidate_rect(&widget->screen_location);

    if (strcmp(widget->wclass->name, "toplevel") == 0){
        ei_toplevel_t* topl_ptr = (ei_toplevel_t*)widget;
        if (topl_ptr->close_btn_ptr != NULL) {
            extern int btn_x;
            extern int btn_y;
            ei_place((ei_widget_t *) topl_ptr->close_btn_ptr, NULL, &btn_x, &btn_y, NULL, NULL, NULL, NULL, NULL, NULL);
        }
    }

}

void free_manager_list(){
    ei_geometrymanager_t* current_manager = head_manager;
    ei_geometrymanager_t * manager_to_free = NULL;
    while(current_manager != NULL){
        manager_to_free = current_manager;
        current_manager = current_manager->next;
        free(manager_to_free);
    }
    head_manager = NULL;
    tail_manager = NULL;
}

