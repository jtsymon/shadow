/* 
 * File:   image.h
 * Author: jts
 *
 * Created on 13 April 2014, 2:02 PM
 */

#ifndef IMAGE_H
#define	IMAGE_H

#include <png.h>
#include "../main.h"
#include "../misc.h"

extern GLubyte *load_png(char *name, int *width, int *height, int *bit_depth, int *colour_type);

#endif	/* IMAGE_H */

