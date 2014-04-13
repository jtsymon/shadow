/* 
 * File:   def.h
 * Author: jts
 *
 * Created on 11 January 2014, 2:42 PM
 */


#ifndef DEF_H
#define	DEF_H

#include <math.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>

#include <GL/glew.h>
#include <GL/glxew.h>
#include <GLFW/glfw3.h>

#define MATH_CONSTANTS
#ifdef MATH_CONSTANTS
#ifndef M_E
    #define M_E         2.7182818284590452354	/* e */
#endif
#ifndef M_LOG2E
    #define M_LOG2E     1.4426950408889634074	/* log_2 e */
#endif
#ifndef M_LOG10E
    #define M_LOG10E	0.43429448190325182765	/* log_10 e */
#endif
#ifndef M_LN2
    #define M_LN2		0.69314718055994530942	/* log_e 2 */
#endif
#ifndef M_LN10
    #define M_LN10		2.30258509299404568402	/* log_e 10 */
#endif
#ifndef M_PI
    #define M_PI		3.14159265358979323846	/* pi */
#endif
#ifndef M_PI_2
    #define M_PI_2		1.57079632679489661923	/* pi/2 */
#endif
#ifndef M_PI_4
    #define M_PI_4		0.78539816339744830962	/* pi/4 */
#endif
#ifndef M_1_PI
    #define M_1_PI		0.31830988618379067154	/* 1/pi */
#endif
#ifndef M_2_PI
    #define M_2_PI		0.63661977236758134308	/* 2/pi */
#endif
#ifndef M_2_SQRTPI
    #define M_2_SQRTPI	1.12837916709551257390	/* 2/sqrt(pi) */
#endif
#ifndef M_SQRT2
    #define M_SQRT2     1.41421356237309504880	/* sqrt(2) */
#endif
#ifndef M_SQRT1_2
    #define M_SQRT1_2	0.70710678118654752440	/* 1/sqrt(2) */
#endif
#endif

typedef struct {
    double x, y;
} DPoint;

extern double vector_to_angle(double w, double h);

#define delta 0.00001

typedef unsigned char bool;
#define true    1
#define false   0

extern int max(int a, int b);
extern int min(int a, int b);
extern double maxd(double a, double b);
extern double mind(double a, double b);
extern double absd(double d);
extern int equald(double a, double b);
extern double angle_sanify(double a);

extern char* read_file(char* filename);

#endif	/* DEF_H */

