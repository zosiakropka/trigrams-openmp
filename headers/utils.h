/* 
 * File:   utils.h
 * Author: Zosia
 *
 * Created on December 1, 2013, 2:41 AM
 */

#ifndef UTILS_H
#define	UTILS_H

#include <iostream>
#include <cstdlib>
#include <cstdio>

typedef unsigned int unsint;
typedef unsigned long unslong;

using std::cout;
using std::endl;

void usage(const char* name, const char* arguments);

int i2a(size_t number, char** str);
size_t a2i(char* str);

class Timer {
private:
    double time;

public:
    double get_seconds();
    void add(double interval);
};

#endif	/* UTILS_H */

