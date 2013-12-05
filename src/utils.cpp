#include "../headers/utils.h"

void usage(const char* name, const char* arguments) {
    std::cout << "Usage: " << name << ' ' << arguments << std::endl;
    exit(64);
}

int long2a(u_long number, char** str) {
    int length = 0;
    u_long tmp = number;
    while (tmp > 0) {
        tmp /= 10;
        length++;
    }
    (*str) = new char[length + 1];
    return sprintf((*str), "%lu", number);
}

u_long a2long(char* str) {
    char* endptr;
    return strtoul(str, &endptr, 0);
}

//////////////////////////////////////////////////////////////////////////////
