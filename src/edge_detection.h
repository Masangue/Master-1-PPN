#ifndef EDGE_DETECTION_H
#define EDGE_DETECTION_H

#include <stdio.h>
#include <stdlib.h>

/*
    Marching square is a graphical algorithm to draw a contour line in an image (or any set of points of same format).
    Instead of drawing lines, this implementation draws 3x3 pixels from 2x2 to make the contour line more visible.

    @param in : pixels: the set of points, each point is stored in an unsigned char (8 bits)
    @param in : width: the width of the image
    @param in : height: the height of the image
    @param in : threshold: the threshold to draw the contour line (if the pixel value is greater or equal than the threshold, the pixel is considered as "on")
    @param in : off_value: the value of the new_pixel when it is off (color of the background)
    @param in : on_value: the value of the new_pixel when it is on (color of the contour line)
    @param out : out_pixels: the new set of points (allocated as new_width*new_height unsigned char (8 bits))
    @param out : out_width: the width of the new image
    @param out : out_height: the height of the new image
*/
void marchingSquare(u_int8_t *pixels, size_t width, size_t height, u_int8_t threshold, u_int8_t off_value, u_int8_t on_value, u_int8_t **out_pixels, size_t *out_width, size_t *out_height);

#endif