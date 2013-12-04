/* 
 * File:   timer.h
 * Author: Zosia
 *
 * Created on December 4, 2013, 4:54 AM
 */

#include <cstdlib>
#include <iostream>

#ifndef TIMER_H
#define	TIMER_H

class Timer {
private:
    static Timer* timer;
    double time;
    char format;
    static const char default_format;
    Timer();

public:
    char delimiter;
    static Timer* instance();
    void add(double interval);

    friend std::ostream& operator<<(std::ostream& os, const Timer& timer);
};

#endif	/* TIMER_H */

