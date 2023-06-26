#include "ei_widget.h"
#include "ei_utils.h"
#include "ei_button.h"

#ifndef PROJETC_IG_EI_WIDGET_TOPLEVEL_H
#define PROJETC_IG_EI_WIDGET_TOPLEVEL_H



typedef struct ei_toplevel_t {
        ei_widget_t	widget;
        ei_color_t          bg_color;
        int			border_width;
        char*			title;
        ei_bool_t		closable;
        ei_axis_set_t	resizable;
        ei_size_t*		min_size;
        ei_button_t*    close_btn_ptr;
}ei_toplevel_t;



extern void calculate_content_rect(ei_widget_t* widget, int border_width);
extern ei_bool_t ei_click_toplevel(struct ei_widget_t* widget, struct ei_event_t* event, void* user_param);

#endif //PROJETC_IG_EI_WIDGET_TOPLEVEL_H