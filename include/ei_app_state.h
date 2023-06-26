//
// Created by Yan LU on 29/05/2020.
//

#ifndef PROJETC_IG_EI_STATE_H
#define PROJETC_IG_EI_STATE_H

ei_rect_t root_rect;
static ei_bool_t app_run = EI_TRUE;
static ei_bool_t button_pressed = EI_FALSE;
static ei_widget_t * pressed_button = NULL;
static ei_widget_t * clicked_toplevel = NULL;
static ei_point_t last_mouse_position;

#endif //PROJETC_IG_EI_STATE_H
