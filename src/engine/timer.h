/* 
 * File:   time.h
 * Author: jts
 *
 * Created on 13 April 2014, 3:09 PM
 */

#ifndef TIME_H
#define	TIME_H

#define _GNU_SOURCE
#include <time.h>
#include <stdint.h>

extern uint64_t ticks_ms();
extern void delay_ms(int ms);
extern void delay_us(int us);

#endif	/* TIME_H */