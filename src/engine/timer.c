
#include "timer.h"

uint64_t ticks_ms() {
    struct timespec time;
    clock_gettime(CLOCK_MONOTONIC, &time);
    // printf("%ld %ld", time.tv_sec, time.tv_nsec);
    return time.tv_sec * 1000 + time.tv_nsec / 1000000;
}

void delay_ms(int ms) {
    struct timespec time;
    time.tv_sec = 0;
    time.tv_nsec = ms * 1000000;
    nanosleep(&time, NULL);
}

void delay_us(int us) {
    struct timespec time;
    time.tv_sec = 0;
    time.tv_nsec = us * 1000;
    nanosleep(&time, NULL);
}