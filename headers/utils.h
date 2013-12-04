/* 
 * File:   utils.h
 * Author: Zosia
 *
 * Created on December 1, 2013, 2:41 AM
 */

#ifndef UTILS_H
#define	UTILS_H

#include <iostream>
#include <cstdio>
#include <cstdlib>


void usage(const char* name, const char* arguments);

int long2a(u_long number, char** str);
u_long a2long(char* str);



#endif	/* UTILS_H */

