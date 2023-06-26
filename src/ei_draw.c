//
// Created by Yan LU on 22/05/2020.
//
#include <stdlib.h>

#include "ei_draw.h"


int			ei_copy_surface		(ei_surface_t		destination,
                                       const ei_rect_t*	dst_rect,
                                       const ei_surface_t	source,
                                       const ei_rect_t*	src_rect,
                                       const ei_bool_t	alpha) {

    ei_size_t source_size = hw_surface_get_size(source);
    ei_size_t destination_size = hw_surface_get_size(destination);

    ei_rect_t source_rect = src_rect != NULL ? *src_rect : hw_surface_get_rect(source);
    ei_rect_t destination_rect = dst_rect != NULL ? *dst_rect : hw_surface_get_rect(destination);


    if (destination_rect.size.width != source_rect.size.width || destination_rect.size.height != source_rect.size.height \
        || destination_rect.top_left.x < 0 || destination_rect.top_left.y < 0 \
        || source_rect.top_left.x < 0 || source_rect.top_left.y < 0 \
        || destination_rect.top_left.x + destination_rect.size.width > destination_size.width\
        || destination_rect.top_left.y + destination_rect.size.height > destination_size.height \
        || source_rect.top_left.x + source_rect.size.width > source_size.width \
        || source_rect.top_left.y + source_rect.size.height > source_size.height) {
        return 1;
    }

    uint32_t *pixel_tab_src = (uint32_t*) hw_surface_get_buffer(source);
    uint32_t *pixel_tab_dst = (uint32_t*) hw_surface_get_buffer(destination);

    uint8_t * src_pixel, *dst_pixel;

    int src_ir, src_ig, src_ib, src_alpha;
    hw_surface_get_channel_indices(source, &src_ir, &src_ig, &src_ib, &src_alpha);

    int dst_ir, dst_ig, dst_ib, dst_alpha;
    hw_surface_get_channel_indices(destination, &dst_ir, &dst_ig, &dst_ib, &dst_alpha);

    if(alpha == EI_TRUE && src_alpha != -1) {

        int src_start_index = source_rect.top_left.y * source_size.width;
        int dst_start_index = destination_rect.top_left.y * destination_size.width;

        for (int i = 0; i < source_rect.size.height; i++){
            for(int j = 0; j < source_rect.size.width; j++){
                int src_index = src_start_index + i * source_size.width + source_rect.top_left.x + j;
                int dst_index = dst_start_index + i * destination_size.width + destination_rect.top_left.x + j;

                src_pixel= (uint8_t *)&(pixel_tab_src[src_index]);
                dst_pixel = (uint8_t *)&(pixel_tab_dst[dst_index]);

                dst_pixel[dst_ir] = (src_pixel[src_alpha] * src_pixel[src_ir] + (255 - src_pixel[src_alpha]) * dst_pixel[dst_ir])/255;
                dst_pixel[dst_ig] = (src_pixel[src_alpha] * src_pixel[src_ig] + (255 - src_pixel[src_alpha]) * dst_pixel[dst_ig])/255;
                dst_pixel[dst_ib] = (src_pixel[src_alpha] * src_pixel[src_ib] + (255 - src_pixel[src_alpha]) * dst_pixel[dst_ib])/255;
            }
        }

    } else {
        int src_start_index = source_rect.top_left.y * source_size.width;
        int dst_start_index = destination_rect.top_left.y * destination_size.width;

        for (int i = 0; i < source_rect.size.height; i++){
            for(int j = 0; j < source_rect.size.width; j++){
                int src_index = src_start_index + i * source_size.width + source_rect.top_left.x + j;
                int dst_index = dst_start_index + i * destination_size.width + destination_rect.top_left.x + j;
                pixel_tab_dst[dst_index] = pixel_tab_src[src_index];
            }
        }
    }
    return 0;
}


void			ei_draw_text		(ei_surface_t		surface,
                                     const ei_point_t*	where,
                                     const char*		text,
                                     const ei_font_t	font,
                                     ei_color_t		color,
                                     const ei_rect_t*	clipper) {


    ei_surface_t txt_surface = hw_text_create_surface(text, font, color);
    ei_rect_t src_rct = hw_surface_get_rect(txt_surface);
    ei_rect_t dst_rct;
    dst_rct.top_left = *where;
    dst_rct.size = src_rct.size;
    ei_copy_surface(surface, &dst_rct, txt_surface, &src_rct, EI_TRUE);
    hw_surface_free(txt_surface);
}



uint32_t		ei_map_rgba		(ei_surface_t surface, const ei_color_t* color) {
    int red_position, green_position, blue_position, alpha;
    hw_surface_get_channel_indices(surface, &red_position, &green_position, &blue_position, &alpha);
    uint32_t new_color = color->red * (int)pow(256, red_position) + color->green * (int)pow(256, green_position) + \
        color->blue * (int)pow(256, blue_position);
    new_color = alpha == -1 ? new_color : new_color + color->alpha * pow(256, alpha);
    return new_color;
}




void			ei_fill			(ei_surface_t		surface,
                                    const ei_color_t*	color,
                                    const ei_rect_t*	clipper) {
    uint32_t *pixel_ptr = (uint32_t *)hw_surface_get_buffer(surface);
    ei_rect_t surface_rect = hw_surface_get_rect(surface);
    clipper = (clipper != NULL) ? clipper : &surface_rect;
    if (color == NULL){
        color = &black;
    }
    uint32_t int_color = ei_map_rgba(surface, color);
    if (clipper->top_left.x < 0 || clipper->top_left.y < 0 || \
        clipper->top_left.x + clipper->size.width > surface_rect.size.width \
        || clipper->top_left.y + clipper->size.height > surface_rect.size.height){
        return;
    }
    int ir, ig, ib, ia;
    hw_surface_get_channel_indices(surface, &ir, &ig, &ib, &ia);

    u_int32_t start_index = clipper->top_left.y * surface_rect.size.width;
    if (ia == -1 || color->alpha == 255){
        for(int i  = 0; i < clipper->size.height; i++){
            for(int j = 0; j < clipper->size.width; j++){
                pixel_ptr[start_index + i * surface_rect.size.width + clipper->top_left.x + j] = int_color;
            }
        }
    } else {
        for(int i  = 0; i < clipper->size.height; i++){
            for(int j = 0; j < clipper->size.width; j++){
                uint8_t* color_ptr = (uint8_t*)(pixel_ptr + start_index + i * surface_rect.size.width + clipper->top_left.x + j);
                color_ptr[ir] = (color->alpha * color->red + (255 - color->alpha)* color_ptr[ir]) / 255;
                color_ptr[ig] = (color->alpha * color->green + (255 - color->alpha)* color_ptr[ig]) / 255;
                color_ptr[ib] = (color->alpha * color->blue + (255 - color->alpha)* color_ptr[ib]) / 255;
            }
        }
    }

}


/*
int			ei_copy_surface_old		(ei_surface_t		destination,
                                           const ei_rect_t*	dst_rect,
                                           const ei_surface_t	source,
                                           const ei_rect_t*	src_rect,
                                           const ei_bool_t	alpha) {

    hw_surface_lock(source);
    hw_surface_lock(destination);
    // Pixels

    int ir;
    int ig;
    int ib;
    int ia;
    // On met à jour les Channel indices (le même pour toutes les surfaces)
    hw_surface_get_channel_indices(source,  &ir,  &ig,  &ib,  &ia);

    uint32_t*			pixel_source;
    uint32_t*			pixel_destination;

    // Composantes Couleurs
    int component_rouge;
    int component_green;
    int component_blue;
    int component_alpha;
                        // Rectangles de la source et de la destination

                        ei_rect_t rectangle_source = hw_surface_get_rect(source);
    ei_rect_t rectangle_destination = hw_surface_get_rect(destination);
// Rectangles après clipping

    ei_rect_t clipping_source;
    ei_rect_t clipping_destination;

// 1er cas dst_rect NULL et src_rect non NULL donc on ne fait pas de clipping sur la surface de destination

    if (dst_rect == NULL && src_rect != NULL){
        clipping_source = intersected_rect(*src_rect, rectangle_source);
        clipping_destination = rectangle_destination;}

// 2éme cas dst_rect non NULL et src_rect NULL donc on ne fait pas de clipping sur la surface source

    else if (dst_rect != NULL && src_rect == NULL){
        clipping_source = rectangle_source;
        clipping_destination = intersected_rect(*dst_rect, rectangle_destination);}

// 3éme cas dst_rect NULL et src_rect NULL donc on ne fait pas de clipping

    else if (dst_rect == NULL && src_rect == NULL){
        clipping_source = rectangle_source;
        clipping_destination = rectangle_destination;}

// Dernier cas si les deux ne sont pas nulles et donc on fait le clipping sur les deux

    else if (dst_rect != NULL && src_rect != NULL){
        clipping_source = intersected_rect(*src_rect, rectangle_source);
        clipping_destination = intersected_rect(*dst_rect, rectangle_destination);}

    if (clipping_destination.size.height != clipping_source.size.height || clipping_destination.size.width != clipping_source.size.width){
        return 1;
    }
// On met à jour les coordonnées des pixels d'origine
    hw_surface_set_origin(source, clipping_source.top_left);
    ei_point_t point_dst = clipping_destination.top_left;
    point_dst.y = -(point_dst.y + (70*clipping_destination.size.height/27));
    point_dst.x = point_dst.x - (350*clipping_destination.size.width/206);
    hw_surface_set_origin(destination, point_dst);
//hw_surface_set_origin(destination, clipping_destination.top_left);

// On récupère les pixels source et destination

    pixel_source = (uint32_t *)hw_surface_get_buffer(source);
    pixel_destination = (uint32_t *)hw_surface_get_buffer(destination);
// hw_surface_set_origin(destination, clipping_destination.top_left);

    for (int y = 0; y <  dst_rect->size.height; y++) {
        for (int x = 0; x < dst_rect->size.width; x++) {
            if (!alpha) {
                *pixel_destination = *pixel_source;
                pixel_source++;
                pixel_destination++;
            } else {
// On calcule les composantes de la source
// Composante rouge
//((uint8_t*)(&pixel_source) + *ir);
//((uint8_t*)(&pixel_source))[*ir];
                uint8_t* Scomponent_rouge = (uint8_t*)pixel_source+ ir;
// Composante verte
//((uint8_t*)(&pixel_source) + *ig);
//((uint8_t*)(&pixel_source))[*ig];
                uint8_t* Scomponent_verte = (uint8_t*)pixel_source + ig;
//Composante bleue
//((uint8_t*)(&pixel_source) + *ib);
//((uint8_t*)(&pixel_source))[*ib];
                uint8_t* Scomponent_blue = (uint8_t*)pixel_source + ib;
// Composante alpha
//((uint8_t*)(&pixel_source) + *ia);
//((uint8_t*)(&pixel_source))[*ia];
                uint8_t* Scomponent_alpha = (uint8_t*)pixel_source + ia;

// On calcule les composantes de la destination

// Composante rouge
//((uint8_t*)(&pixel_destination) + *ir);
//((uint8_t*)(&pixel_destination))[*ir];
                uint8_t* Dcomponent_rouge = (uint8_t*)pixel_destination + ir;
// Composante verte
//((uint8_t*)(&pixel_destination) + *ig);
//((uint8_t*)(&pixel_destination))[*ig];
                uint8_t* Dcomponent_verte = (uint8_t*)pixel_destination + ig;
//composante bleue
//((uint8_t*)(&pixel_destination) + *ib);
//((uint8_t*)(&pixel_destination))[*ib];
                uint8_t* Dcomponent_blue = (uint8_t*)pixel_destination + ib;
// composante alpha
//((uint8_t*)(&pixel_destination) + *ia);
//((uint8_t*)(&pixel_destination))[*ia];
                uint8_t* Dcomponent_alpha = (uint8_t*)pixel_destination + ia;

// Pour le rouge
                *Dcomponent_rouge = (*Scomponent_rouge * *Scomponent_alpha + *Dcomponent_rouge * (255 - *Scomponent_alpha)) / 255;
// Pour le vert
                *Dcomponent_verte = (*Scomponent_verte * *Scomponent_alpha + *Dcomponent_verte * (255 - *Scomponent_alpha)) / 255;
// Pour le bleu
                *Dcomponent_blue = (*Scomponent_blue * *Scomponent_alpha + *Dcomponent_blue * (255 - *Scomponent_alpha)) / 255;
// Inutile de calculer alpha car la transparence résultante n'est jamais utilisée

// ei_color_t couleur_pixel_destination = {Dcomponent_rouge, Dcomponent_verte, Dcomponent_blue, Dcomponent_alpha};
// *pixel_destination = ei_map_rgba(destination, &couleur_pixel_destination);
                pixel_destination++;
                pixel_source++;

            }
        }
        pixel_destination += abs(rectangle_destination.size.width - clipping_destination.size.width);
        pixel_source += abs(rectangle_source.size.width - clipping_source.size.width);
    }
    hw_surface_unlock(source);
    hw_surface_unlock(destination);
    hw_surface_unlock(destination);
//hw_surface_update_rects(source, NULL);
    hw_surface_update_rects(destination, NULL);
    return 0;
}
*/