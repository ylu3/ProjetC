//
// Created by Yan LU on 21/05/2020.
//
#include <stdlib.h>

#include "ei_types.h"
#include "ei_utils.h"

#define pi acos(-1)
#define j2h(x) (pi*(x)/180.0)

ei_color_t lighter_color(ei_color_t color)
{
    ei_color_t lighter_color = color;
    lighter_color.alpha	= color.alpha - 100;
    return lighter_color;
}

ei_linked_point_t* get_border(ei_rect_t* screen_location, int border_width, char type){
    int width = screen_location->size.width;
    int height = screen_location->size.height;

    ei_linked_point_t* top_left = malloc(sizeof(ei_linked_point_t));
    top_left->point = screen_location->top_left;

    ei_linked_point_t* top_right = malloc(sizeof(ei_linked_point_t));
    top_right->point = top_left->point;
    top_right->point.x += width;

    ei_linked_point_t* bottom_right = malloc(sizeof(ei_linked_point_t));
    bottom_right->point = top_right->point;
    bottom_right->point.y += height;

    ei_linked_point_t* bottom_left = malloc(sizeof(ei_linked_point_t));
    bottom_left->point = top_left->point;
    bottom_left->point.y += height;

    ei_linked_point_t* center_left = malloc(sizeof(ei_linked_point_t));
    center_left->point = bottom_left->point;
    center_left->point.y -= border_width;
    center_left->point.x += border_width;

    ei_linked_point_t* center_right = malloc(sizeof(ei_linked_point_t));
    center_right->point = top_right->point;
    center_right->point.y += border_width;
    center_right->point.x -= border_width;

    if (type == 'w'){
        top_left->point.x += border_width;
        top_left->point.y += border_width;
        top_right->point.x -= border_width;
        top_right->point.y += border_width;
        bottom_left->point.x += border_width;
        bottom_left->point.y -= border_width;
        bottom_right->point.x -= border_width;
        bottom_right->point.y -= border_width;
    }

    switch(type){
        case('u'):
            top_left->next = top_right;
            top_right->next = center_right;
            center_right->next = center_left;
            center_left->next = bottom_left;
            bottom_left->next = NULL;
            free(bottom_right);
            return top_left;
        case('l'):
            top_right->next = bottom_right;
            bottom_right->next = bottom_left;
            bottom_left->next = center_left;
            center_left->next = center_right;
            center_right->next = NULL;
            free(top_left);
            return top_right;
        case('w'):
            top_left->next = top_right;
            top_right->next = bottom_right;
            bottom_right->next = bottom_left;
            bottom_left->next = NULL;
            free(center_right);
            free(center_left);
            return top_left;
        default:
            return NULL;
    }

}

void free_linked_point_list(ei_linked_point_t* head){
    ei_linked_point_t* current_point = head;
    ei_linked_point_t* point_to_free;
    while(current_point != NULL){
        point_to_free = current_point;
        current_point = current_point->next;
        free(point_to_free);
    }
    head = NULL;
}

ei_rect_t intersected_rect(ei_rect_t rect1, ei_rect_t rect2){

    int min_x1 = rect1.top_left.x;
    int min_y1 = rect1.top_left.y;
    int max_x1 = rect1.top_left.x + rect1.size.width;
    int max_y1 = rect1.top_left.y + rect1.size.height;

    int min_x2 = rect2.top_left.x;
    int min_y2 = rect2.top_left.y;
    int max_x2 = rect2.top_left.x + rect2.size.width;
    int max_y2 = rect2.top_left.y + rect2.size.height;
    int min_x, min_y, max_x, max_y;

    ei_rect_t rect;
    rect.size = ei_size_zero();
    rect.top_left = ei_point_zero();

    if (max_x1 < min_x2 || min_x1 > max_x2 || min_y1 > max_y2 || max_y1 < min_y2 ){
        return rect;
    }

    if (min_x1 < min_x2){
        min_x = min_x2;
        if (max_x2 > max_x1){
            max_x = max_x1;
        }
        else{
            max_x = max_x2;
        }
    }
    else{
        min_x = min_x1;
        if (max_x1 > max_x2){
            max_x = max_x2;
        }
        else{
            max_x = max_x1;
        }
    }

    if (min_y1 < min_y2){
        min_y = min_y2;
        if (max_y2 > max_y1){
            max_y = max_y1;
        }
        else{
            max_y = max_y2;
        }
    }
    else{
        min_y = min_y1;
        if (max_y1 > max_y2){
            max_y = max_y2;
        }
        else{
            max_y = max_y1;
        }
    }

    rect.size.width = max_x - min_x;
    rect.size.height = max_y - min_y;
    rect.top_left.x = min_x;
    rect.top_left.y = min_y;

    return rect;

}

ei_linked_point_t* ei_arc(ei_point_t centre, float radius, float start_angle, float end_angle)
{

    ei_linked_point_t* first_point = malloc(sizeof(ei_linked_point_t));
    first_point->point.x = (int)(radius * cos(j2h(start_angle))) + centre.x;
    first_point->point.y = (int)(radius * sin(j2h(start_angle))) + centre.y;
    float current_angle = start_angle;
    ei_linked_point_t* current_point = first_point;

    while (current_angle < end_angle)
    {
        ei_linked_point_t* new_point = malloc(sizeof(ei_linked_point_t));
        new_point->point = ei_point( (int)(radius * cos(j2h(current_angle))) + centre.x,
                                     (int)(radius * sin(j2h(current_angle))) + centre.y);
        new_point->next = NULL;
        current_point->next = new_point;
        current_point = new_point;

        current_angle += 0.01;
    }

    return first_point;

}


ei_linked_point_t* extend_linked_point_list(ei_linked_point_t * list1, ei_linked_point_t* list2){
    if (list1 == NULL){
        return list2;
    } else {
        if (list2 == NULL){
            return list1;
        }
    }
    ei_linked_point_t* tail_list1 = list1;
    while(tail_list1->next != NULL){
        tail_list1 = tail_list1->next;
    }
    tail_list1->next = list2;
    return list1;
}


ei_linked_point_t* rounded_frame(ei_rect_t* rect, float radius, int border_width, char type){
    int height = rect->size.height;
    int width = rect->size.width;
    int x = rect->top_left.x;
    int y = rect->top_left.y;
    ei_linked_point_t * list;
    int marge = border_width + radius * 0.5;


    switch (type) {
        case('u'):{
            ei_linked_point_t* p1 = ei_arc(ei_point(x + radius, y + radius), radius, 180, 270);
            ei_linked_point_t* p2 = ei_arc(ei_point(x + width - radius, y + radius), radius, 270, 315);
            ei_linked_point_t* p3 = malloc(sizeof(ei_linked_point_t));
            p3->point.x = x + width - marge;
            p3->point.y = y + marge;
            p3->next = NULL;
            ei_linked_point_t* p4 = malloc(sizeof(ei_linked_point_t));
            p4->point.x = x + marge;
            p4->point.y = y + height - marge;
            p4->next = NULL;
            ei_linked_point_t* p5 = ei_arc(ei_point(x + radius, y + height - radius), radius, 135, 180);
            ei_linked_point_t* p6 = malloc(sizeof(ei_linked_point_t));
            p6->point.x = x;
            p6->point.y = y + height - radius;
            p6->next = NULL;
            list = extend_linked_point_list(p1, p2);
            list = extend_linked_point_list(list, p3);
            list = extend_linked_point_list(list, p4);
            list = extend_linked_point_list(list, p5);
            list = extend_linked_point_list(list, p6);

            break;
        }
        case('l'):{
            ei_linked_point_t* p1 = ei_arc(ei_point(x + width - radius, y + radius), radius, 315, 360);
            ei_linked_point_t* p6 = malloc(sizeof(ei_linked_point_t));
            p6->point.x = x + width;
            p6->point.y = y + radius;
            p6->next = NULL;
            ei_linked_point_t* p2 = ei_arc(ei_point(x + width - radius, y + height - radius), radius, 0, 90);
            ei_linked_point_t* p3 = ei_arc(ei_point(x + radius, y + height - radius), radius, 90, 135);
            ei_linked_point_t* p4 = malloc(sizeof(ei_linked_point_t));
            p4->point.x = x + marge;
            p4->point.y = y + height - marge;
            p4->next = NULL;
            ei_linked_point_t* p5 = malloc(sizeof(ei_linked_point_t));
            p5->point.x = x + width - marge;
            p5->point.y = y + marge;
            p5->next = NULL;
            list = extend_linked_point_list(p1, p6);
            list = extend_linked_point_list(list, p2);
            list = extend_linked_point_list(list, p3);
            list = extend_linked_point_list(list, p4);
            list = extend_linked_point_list(list, p5);
            break;
        }
        case('w'):{
            ei_linked_point_t* p1 = ei_arc(ei_point(x + radius, y + radius), radius, 180, 270);
            ei_linked_point_t* p2 = ei_arc(ei_point(x + width - radius, y + radius), radius, 270, 360);
            ei_linked_point_t* p3 = ei_arc(ei_point(x + width - radius, y + height - radius), radius, 0, 90);
            ei_linked_point_t* p4 = ei_arc(ei_point(x + radius, y + height - radius), radius, 90, 180);
            ei_linked_point_t* p5 = malloc(sizeof(ei_linked_point_t));
            p5->point.x = x + width;
            p5->point.y = y + radius;
            p5->next = NULL;
            ei_linked_point_t* p6 = malloc(sizeof(ei_linked_point_t));
            p6->point.x = x ;
            p6->point.y = y + height - radius;
            p6->next = NULL;
            list = extend_linked_point_list(p1, p2);
            list = extend_linked_point_list(list, p5);
            list = extend_linked_point_list(list, p3);
            list = extend_linked_point_list(list, p4);
            list = extend_linked_point_list(list, p6);

            break;
        }

    }

    return list;
}

ei_linked_point_t* get_toplevel_border(ei_widget_t* widget){
    extern int default_titlebar_radius;
    int x = widget->screen_location.top_left.x;
    int y = widget->screen_location.top_left.y;
    int height = widget->screen_location.size.height;
    int width = widget->screen_location.size.width;

    ei_linked_point_t* list = NULL;
    ei_linked_point_t* p1 = ei_arc(ei_point(x + default_titlebar_radius, y + default_titlebar_radius ), \
        default_titlebar_radius, 180, 270);
    ei_linked_point_t* p2 = malloc(sizeof(ei_linked_point_t));
    p2->point.x = x + default_titlebar_radius;
    p2->point.y = y;
    p2->next = NULL;
    ei_linked_point_t* p3 = ei_arc(ei_point(x + width - default_titlebar_radius, y + default_titlebar_radius ), \
        default_titlebar_radius, 270, 360);
    ei_linked_point_t* p4 = malloc(sizeof(ei_linked_point_t));
    p4->point.x = x + width;
    p4->point.y = y + default_titlebar_radius;
    p4->next = NULL;
    ei_linked_point_t* p5 = malloc(sizeof(ei_linked_point_t));
    p5->point.x = x + width;
    p5->point.y = y + height;
    p5->next = NULL;
    ei_linked_point_t* p6 = malloc(sizeof(ei_linked_point_t));
    p6->point.x = x;
    p6->point.y = y + height;
    p6->next = NULL;

    list = extend_linked_point_list(p1,p2);
    list = extend_linked_point_list(list, p3);
    list = extend_linked_point_list(list, p4);
    list = extend_linked_point_list(list, p5);
    list = extend_linked_point_list(list, p6);

    return list;
}



void ei_draw_img(ei_surface_t surface, ei_surface_t img, const ei_rect_t* img_rect, const ei_rect_t* clipper){
    ei_rect_t src_rect = *img_rect;
    ei_rect_t dst_rect = *clipper;
    int width = src_rect.size.width > dst_rect.size.width ? dst_rect.size.width : src_rect.size.width;
    int height = src_rect.size.height > dst_rect.size.height ? dst_rect.size.height : src_rect.size.height;
    src_rect.size.height = height;
    dst_rect.size.height = height;
    src_rect.size.width = width;
    dst_rect.size.width = width;
    ei_copy_surface(surface, &dst_rect, img, &src_rect, EI_TRUE);
}










/*
// On dessine le texte d'un widget en utilisant la fonction draw_text du module ei_draw.h
// On a besoin en paramètre du widget en question, la surface de dessin, le texte à écrire et le font et la couleur
// et la zone d'écriture : l'anchor
void    ei_draw_text_of_widget	        (ei_surface_t		surface,
                                               ei_widget_t* widget,
                                               const char*		text,
                                               const ei_font_t	font,
                                               ei_color_t		color){
// origine du repère

        ei_point_t origine = ei_point_zero();

// La fonction hw_text_compute_size permet de définir la taille du texte
        ei_size_t taille_texte;
        hw_text_compute_size(text, font, &taille_texte.width, &taille_texte.height);

// La taille du texte dépend de la taille du widget : elle ne doit pas la dépasser et elle ne doit pas
// non plus être trop petite relativement à la taille du widget
// On récupère alors les paramètres du widget
// taille :
        ei_size_t taille_widget = widget->screen_location.size;


// Si le texte est plus grand que le widget :
        if (taille_texte.width >= taille_widget.width || taille_texte.height >= taille_widget.height) {
                printf("Texte grand, ne peut pas être affiché. \n");
                return;
        }
// Sinon on définit le clipper qui définit la limite de la zone d'écriture
// C'est l'intersection entre la fenêtre du widget et de son parent
        ei_geometrymanager_t * placer_ptr = (ei_geometrymanager_t *) widget;
        placer_ptr->runfunc(widget);
        ei_rect_t clipper_text;

        if (widget->parent == NULL) {
                clipper_text = widget->screen_location;
        }
        else {
                clipper_text = intersected_rect(widget->screen_location, widget->parent->screen_location);
        }
        ei_draw_text(surface, &origine, text, font, color, &clipper_text);


}
*/
