#include "edge_detection.h"

void marchingSquare(u_int8_t *pixels, size_t width, size_t height, u_int8_t threshold, u_int8_t off_value, u_int8_t on_value, u_int8_t **out_pixels, size_t *out_width, size_t *out_height)
{
    int a,b,c,d;
    u_int8_t state;
    size_t new_width = (width-1)*3;
    size_t new_height = (height-1)*3;

    u_int8_t *new_pixels = malloc(new_width*new_height*sizeof(u_int8_t));

    for (int i=0;i<height-1;i++)
    {
        for (int j=0;j<width-1;j++)
        {
            state = 0;

            a = pixels[i*width+j];
            b = pixels[i*width+j+1];
            c = pixels[(i+1)*width+j+1];
            d = pixels[(i+1)*width+j];

            if (a >= threshold) state |= 1 << 3;
            if (b >= threshold) state |= 1 << 2;
            if (c >= threshold) state |= 1 << 1;
            if (d >= threshold) state |= 1;

            switch (state) {
            case 0: // 0000 (rien)
                new_pixels[(i*new_width+j)*3] = off_value;
                new_pixels[(i*new_width+j)*3+1] = off_value;
                new_pixels[(i*new_width+j)*3+2] = off_value;

                new_pixels[(i*new_width+j)*3+new_width] = off_value;
                new_pixels[(i*new_width+j)*3+new_width+1] = off_value;
                new_pixels[(i*new_width+j)*3+new_width+2] = off_value;

                new_pixels[(i*new_width+j)*3+new_width*2] = off_value;
                new_pixels[(i*new_width+j)*3+new_width*2+1] = off_value;
                new_pixels[(i*new_width+j)*3+new_width*2+2] = off_value;
                break;   
            case 1 : // 0001 (bg)
                new_pixels[(i*new_width+j)*3] = off_value;
                new_pixels[(i*new_width+j)*3+1] = off_value;
                new_pixels[(i*new_width+j)*3+2] = off_value;

                new_pixels[(i*new_width+j)*3+new_width] = on_value;
                new_pixels[(i*new_width+j)*3+new_width+1] = off_value;
                new_pixels[(i*new_width+j)*3+new_width+2] = off_value;

                new_pixels[(i*new_width+j)*3+new_width*2] = on_value;
                new_pixels[(i*new_width+j)*3+new_width*2+1] = on_value;
                new_pixels[(i*new_width+j)*3+new_width*2+2] = off_value;
                break;   
            case 2 : // 0010 (bd)
                new_pixels[(i*new_width+j)*3] = off_value;
                new_pixels[(i*new_width+j)*3+1] = off_value;
                new_pixels[(i*new_width+j)*3+2] = off_value;

                new_pixels[(i*new_width+j)*3+new_width] = off_value;
                new_pixels[(i*new_width+j)*3+new_width+1] = off_value;
                new_pixels[(i*new_width+j)*3+new_width+2] = on_value;

                new_pixels[(i*new_width+j)*3+new_width*2] = off_value;
                new_pixels[(i*new_width+j)*3+new_width*2+1] = on_value;
                new_pixels[(i*new_width+j)*3+new_width*2+2] = on_value;            
                break;
            case 3 : // 0011 (bd, bg)
                new_pixels[(i*new_width+j)*3]   = off_value;
                new_pixels[(i*new_width+j)*3+1] = off_value;
                new_pixels[(i*new_width+j)*3+2] = off_value;

                new_pixels[(i*new_width+j)*3+new_width]   = on_value;
                new_pixels[(i*new_width+j)*3+new_width+1] = on_value;
                new_pixels[(i*new_width+j)*3+new_width+2] = on_value;

                new_pixels[(i*new_width+j)*3+new_width*2]   = off_value;
                new_pixels[(i*new_width+j)*3+new_width*2+1] = off_value;
                new_pixels[(i*new_width+j)*3+new_width*2+2] = off_value;
                break;   
            case 4 : // 0100 (hd)
                new_pixels[(i*new_width+j)*3]   = off_value;
                new_pixels[(i*new_width+j)*3+1] = on_value;
                new_pixels[(i*new_width+j)*3+2] = on_value;

                new_pixels[(i*new_width+j)*3+new_width]   = off_value;
                new_pixels[(i*new_width+j)*3+new_width+1] = off_value;
                new_pixels[(i*new_width+j)*3+new_width+2] = on_value;

                new_pixels[(i*new_width+j)*3+new_width*2]   = off_value;
                new_pixels[(i*new_width+j)*3+new_width*2+1] = off_value;
                new_pixels[(i*new_width+j)*3+new_width*2+2] = off_value;
                break;   
            case 5 : // 0101 (hd, bg)
                new_pixels[(i*new_width+j)*3]   = off_value;
                new_pixels[(i*new_width+j)*3+1] = on_value;
                new_pixels[(i*new_width+j)*3+2] = on_value;

                new_pixels[(i*new_width+j)*3+new_width]   = on_value;
                new_pixels[(i*new_width+j)*3+new_width+1] = off_value;
                new_pixels[(i*new_width+j)*3+new_width+2] = on_value;

                new_pixels[(i*new_width+j)*3+new_width*2]   = on_value;
                new_pixels[(i*new_width+j)*3+new_width*2+1] = on_value;
                new_pixels[(i*new_width+j)*3+new_width*2+2] = off_value;
                break;   
            case 6 : // 0110 (hd, bd)
                new_pixels[(i*new_width+j)*3]   = off_value;
                new_pixels[(i*new_width+j)*3+1] = on_value;
                new_pixels[(i*new_width+j)*3+2] = off_value;

                new_pixels[(i*new_width+j)*3+new_width]   = off_value;
                new_pixels[(i*new_width+j)*3+new_width+1] = on_value;
                new_pixels[(i*new_width+j)*3+new_width+2] = off_value;

                new_pixels[(i*new_width+j)*3+new_width*2]   = off_value;
                new_pixels[(i*new_width+j)*3+new_width*2+1] = on_value;
                new_pixels[(i*new_width+j)*3+new_width*2+2] = off_value;
                break;   
            case 7 : // 0111 (hd, bd, bg)
                new_pixels[(i*new_width+j)*3]   = on_value;
                new_pixels[(i*new_width+j)*3+1] = on_value;
                new_pixels[(i*new_width+j)*3+2] = off_value;

                new_pixels[(i*new_width+j)*3+new_width]   = on_value;
                new_pixels[(i*new_width+j)*3+new_width+1] = off_value;
                new_pixels[(i*new_width+j)*3+new_width+2] = off_value;

                new_pixels[(i*new_width+j)*3+new_width*2]   = off_value;
                new_pixels[(i*new_width+j)*3+new_width*2+1] = off_value;
                new_pixels[(i*new_width+j)*3+new_width*2+2] = off_value;
                break;   
            case 8 : // 1000 (hg)
                new_pixels[(i*new_width+j)*3]   = on_value;
                new_pixels[(i*new_width+j)*3+1] = on_value;
                new_pixels[(i*new_width+j)*3+2] = off_value;

                new_pixels[(i*new_width+j)*3+new_width]   = on_value;
                new_pixels[(i*new_width+j)*3+new_width+1] = off_value;
                new_pixels[(i*new_width+j)*3+new_width+2] = off_value;

                new_pixels[(i*new_width+j)*3+new_width*2]   = off_value;
                new_pixels[(i*new_width+j)*3+new_width*2+1] = off_value;
                new_pixels[(i*new_width+j)*3+new_width*2+2] = off_value;
                break;   
            case 9 : // 1001 (hg, bg)
                new_pixels[(i*new_width+j)*3]   = off_value;
                new_pixels[(i*new_width+j)*3+1] = on_value;
                new_pixels[(i*new_width+j)*3+2] = off_value;

                new_pixels[(i*new_width+j)*3+new_width]   = off_value;
                new_pixels[(i*new_width+j)*3+new_width+1] = on_value;
                new_pixels[(i*new_width+j)*3+new_width+2] = off_value;

                new_pixels[(i*new_width+j)*3+new_width*2]   = off_value;
                new_pixels[(i*new_width+j)*3+new_width*2+1] = on_value;
                new_pixels[(i*new_width+j)*3+new_width*2+2] = off_value;
                break;
            case 10 : // 1010 (hg, bd)
                new_pixels[(i*new_width+j)*3] = on_value;
                new_pixels[(i*new_width+j)*3+1] = on_value;
                new_pixels[(i*new_width+j)*3+2] = off_value;

                new_pixels[(i*new_width+j)*3+new_width] = on_value;
                new_pixels[(i*new_width+j)*3+new_width+1] = off_value;
                new_pixels[(i*new_width+j)*3+new_width+2] = on_value;

                new_pixels[(i*new_width+j)*3+new_width*2] = off_value;
                new_pixels[(i*new_width+j)*3+new_width*2+1] = on_value;
                new_pixels[(i*new_width+j)*3+new_width*2+2] = on_value;
                break;   
            case 11 : // 1011 (hg, bd, bg) 
                new_pixels[(i*new_width+j)*3]   = off_value;
                new_pixels[(i*new_width+j)*3+1] = on_value;
                new_pixels[(i*new_width+j)*3+2] = on_value;

                new_pixels[(i*new_width+j)*3+new_width]   = off_value;
                new_pixels[(i*new_width+j)*3+new_width+1] = off_value;
                new_pixels[(i*new_width+j)*3+new_width+2] = on_value;

                new_pixels[(i*new_width+j)*3+new_width*2]   = off_value;
                new_pixels[(i*new_width+j)*3+new_width*2+1] = off_value;
                new_pixels[(i*new_width+j)*3+new_width*2+2] = off_value;
                break;   
            case 12 : // 1100 (hg, hd)
                new_pixels[(i*new_width+j)*3]   = off_value;
                new_pixels[(i*new_width+j)*3+1] = off_value;
                new_pixels[(i*new_width+j)*3+2] = off_value;

                new_pixels[(i*new_width+j)*3+new_width]   = on_value;
                new_pixels[(i*new_width+j)*3+new_width+1] = on_value;
                new_pixels[(i*new_width+j)*3+new_width+2] = on_value;

                new_pixels[(i*new_width+j)*3+new_width*2]   = off_value;
                new_pixels[(i*new_width+j)*3+new_width*2+1] = off_value;
                new_pixels[(i*new_width+j)*3+new_width*2+2] = off_value;
                break; 
            case 13 : // 1101 (hg, hd, bg)  
                new_pixels[(i*new_width+j)*3] = off_value;
                new_pixels[(i*new_width+j)*3+1] = off_value;
                new_pixels[(i*new_width+j)*3+2] = off_value;

                new_pixels[(i*new_width+j)*3+new_width] = off_value;
                new_pixels[(i*new_width+j)*3+new_width+1] = off_value;
                new_pixels[(i*new_width+j)*3+new_width+2] = on_value;

                new_pixels[(i*new_width+j)*3+new_width*2] = off_value;
                new_pixels[(i*new_width+j)*3+new_width*2+1] = on_value;
                new_pixels[(i*new_width+j)*3+new_width*2+2] = on_value;            
                break;
            case 14 : // 1110 (hg, hd, bd)
                new_pixels[(i*new_width+j)*3] = off_value;
                new_pixels[(i*new_width+j)*3+1] = off_value;
                new_pixels[(i*new_width+j)*3+2] = off_value;

                new_pixels[(i*new_width+j)*3+new_width] = on_value;
                new_pixels[(i*new_width+j)*3+new_width+1] = off_value;
                new_pixels[(i*new_width+j)*3+new_width+2] = off_value;

                new_pixels[(i*new_width+j)*3+new_width*2] = on_value;
                new_pixels[(i*new_width+j)*3+new_width*2+1] = on_value;
                new_pixels[(i*new_width+j)*3+new_width*2+2] = off_value;
                break; 
            case 15 : // 1111 (rien)
                new_pixels[(i*new_width+j)*3] = off_value;
                new_pixels[(i*new_width+j)*3+1] = off_value;
                new_pixels[(i*new_width+j)*3+2] = off_value;

                new_pixels[(i*new_width+j)*3+new_width] = off_value;
                new_pixels[(i*new_width+j)*3+new_width+1] = off_value;
                new_pixels[(i*new_width+j)*3+new_width+2] = off_value;

                new_pixels[(i*new_width+j)*3+new_width*2] = off_value;
                new_pixels[(i*new_width+j)*3+new_width*2+1] = off_value;
                new_pixels[(i*new_width+j)*3+new_width*2+2] = off_value;
                break;               
            default:
                break;
            }
        }
    }
    *out_width = new_width;
    *out_height = new_height;
    *out_pixels = new_pixels;
}
