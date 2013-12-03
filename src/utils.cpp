#include "../headers/utils.h"

void usage(const char* name, const char* arguments) {
    cout << "Usage: " << name << ' ' << arguments << endl;
    exit(64);
}

int i2a(size_t number, char** str) {
    int length = 0;
    int tmp = number;
    while (tmp > 0) {
        tmp /= 10;
        length++;
    }
    (*str) = new char[length + 1];
    return sprintf((*str), "%zu", number);
}

size_t a2i(char* str) {
    char* endptr;
    return strtoul(str, &endptr, 0);
}

double Timer::get_seconds() {
    return time;
}

void Timer::add(double interval) {
    #pragma omp atomic
    time += interval;
}
