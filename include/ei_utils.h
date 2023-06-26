/**
 * @file	ei_utils.h
 *
 * @brief	General purpose utility functions: creation of points and sizes, and arithmetics on
 *		them.
 */

#ifndef EI_UTILS_H
#define EI_UTILS_H

#include "ei_types.h"
#include "ei_widget.h"
#include <math.h>



/**
 * @brief	Returns a \ref ei_size_t with width = 0 and height = 0;
 */
static inline ei_size_t ei_size_zero()
{
	ei_size_t	s	= {0, 0};
	return s;
}

/**
 * @brief	Returns a \ref ei_size_t initialized with the width and height passed as parameters.
 */
static inline ei_size_t ei_size(int width, int height)
{
	ei_size_t	s;
	s.width		= width;
	s.height	= height;
	return s;
}

/**
 * @brief	Returns a \ref ei_point_t with x = 0 and y = 0;
 */
static inline ei_point_t ei_point_zero()
{
	ei_point_t	p	= {0, 0};
	return p;
}

/**
 * @brief	Returns a \ref ei_point_t initialized with the x and y passed as parameters.
 */
static inline ei_point_t ei_point(int x, int y)
{
	ei_point_t	pt;
	pt.x	= x;
	pt.y	= y;
	return pt;
}

/**
 * @brief	Returns a \ref ei_point_t which coordinates are opposite from the coordinate of the
 *		point passed as a parameter.
 */
static inline ei_point_t ei_point_neg(ei_point_t point)
{
	point.x 	= - point.x;
	point.y 	= - point.y;
	return point;
}

/**
 * @brief	Returns a \ref ei_point_t which coordinates are the sum of the coordinates of the
 *		two points passed as parameters.
 */
static inline ei_point_t ei_point_add(ei_point_t p1, ei_point_t p2)
{
	p1.x		+= p2.x;
	p1.y		+= p2.y;
	return p1;
}

/**
 * @brief	Returns a \ref ei_point_t which coordinates are the coordinates of the first point
 *		parameter minus the coordinates of the second point parameter.
 */
static inline ei_point_t ei_point_sub(ei_point_t p1, ei_point_t p2)
{
	p1.x		-= p2.x;
	p1.y		-= p2.y;
	return p1;
}

/**
 * @brief	Returns a \ref ei_size_t which components are the same as the point
 *		passed as parameter.
 */
static inline ei_size_t ei_point_as_size(ei_point_t p)
{
	return ei_size(p.x, p.y);
}

/**
 * @brief	Returns a \ref ei_size_t which components are the sum of the components of the
 *		two sizes passed as parameters.
 */
static inline ei_size_t ei_size_add(ei_size_t s1, ei_size_t s2)
{
	s1.width	+= s2.width;
	s1.height	+= s2.height;
	return s1;
}

/**
 * @brief	Returns a \ref ei_size_t which components are the components of the first size
 *		parameter minus the components of the second size parameter.
 */
static inline ei_size_t ei_size_sub(ei_size_t s1, ei_size_t s2)
{
	s1.width	-= s2.width;
	s1.height	-= s2.height;
	return s1;
}

/**
 * @brief	Returns a \ref ei_size_t which is the size passed as the first parameter
 *		scaled by the second parameter.
 */
static inline ei_size_t ei_size_scale(ei_size_t s, float scale)
{
	s.width		= (int)roundf(s.width * scale);
	s.height	= (int)roundf(s.height * scale);
	return s;
}

/**
 * @brief	Returns a \ref ei_point_t which components are the same as the size
 *		passed as parameter.
 */
static inline ei_point_t ei_size_as_point(ei_size_t s)
{
	return ei_point(s.width, s.height);
}

/**
 * @brief	Returns a \ref ei_rect_t located in (0, 0) and of size (0, 0).
 */
static inline ei_rect_t ei_rect_zero()
{
	ei_rect_t	r	= {{0, 0}, {0, 0}};
	return r;
}

/**
 * @brief	Returns a \ref ei_rect_t which position and size are passed as parameters.
 */
static inline ei_rect_t ei_rect(ei_point_t top_left, ei_size_t size) {
    ei_rect_t r;
    r.top_left = top_left;
    r.size = size;
    return r;
}

extern ei_linked_point_t* get_border(ei_rect_t* screen_location, int border_width, char type);
extern void free_linked_point_list(ei_linked_point_t* head);
extern ei_rect_t intersected_rect(ei_rect_t rect1, ei_rect_t rect2);
extern ei_linked_point_t* ei_arc(ei_point_t centre, float radius, float start_angle, float end_angle);
extern ei_linked_point_t* rounded_frame(ei_rect_t* rect, float  radius, int border_width, char type);
extern ei_linked_point_t* extend_linked_point_list(ei_linked_point_t * list1, ei_linked_point_t* list2);
extern ei_color_t lighter_color(ei_color_t color);
extern ei_linked_point_t* get_toplevel_border(ei_widget_t* widget);
extern void ei_draw_img(ei_surface_t surface, ei_surface_t img, const ei_rect_t* img_rect, const ei_rect_t* clipper);
#endif
