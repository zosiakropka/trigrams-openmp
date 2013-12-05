/* 
 * File:   timer.cpp
 * Author: Zosia
 * 
 * Created on December 4, 2013, 4:54 AM
 */

#include "../headers/timer.h"

Timer::Timer() {
    const char* testrun = getenv("TESTRUN");
    if (testrun != NULL && (*testrun) == 'y') {
        format = 'm';
        delimiter = '\0';
    } else {
        format = 'h';
        delimiter = '\n';
    }
}

Timer* Timer::timer = NULL;

Timer* Timer::instance() {
    if (timer == NULL)
        timer = new Timer();
    return Timer::timer;
}

void Timer::add(double interval) {
    #pragma omp atomic
    time += interval;
}

std::ostream& operator<<(std::ostream& os, const Timer & timer) {
    switch (timer.format) {
        case 'm':
            os << timer.time * 1000.0;
            break;
        case 'h':
        default:
            os << timer.time*1000.0 << 'ms';
            break;
    }
    return os;
}
