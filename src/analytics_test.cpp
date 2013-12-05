/* 
 * File:   analytics.cpp
 * Author: Zosia
 *
 * Created on November 30, 2013, 2:59 PM
 */

#include <cstdlib>
#include <iostream>
#include <fstream>
#include <omp.h>

#include "../headers/stats.h"
#include "../headers/utils.h"

using namespace std;

void analyze(char* lang, char** inputs, u_int count, u_int iterations);

////////////////////////////////////////////////////////////////////////////////
/**
 * Save data from stats container to specific language file.
 * 
 * @param stats 
 * @param lang
 */
void save_stats(TrigramStats* stats, char* lang);

int main(int argc, char** argv) {


    if (argc < 4) {
        const char* args = "<count> <lang> <file1> [<file2> ...]";
        usage(argv[0], args);
    }

    int threads = atoi(argv[1]);
    char* language = argv[2];
    char** inputs = &argv[3];
    u_int inputs_count = (u_int) argc - 3;

    if (threads != 0) {
        omp_set_num_threads(threads);
    }

    u_int iterations = 1;
    const char* testrun = getenv("TESTRUN");
    if (testrun != NULL && (*testrun) == 'y') {
        iterations = 1000;
    }

    analyze(language, inputs, inputs_count, iterations);
    Timer* timer = Timer::instance();
    cout << (*timer) << timer->delimiter;

    return 0;
}

////////////////////////////////////////////////////////////////////////////////

void analyze(char* lang, char** inputs, u_int count, u_int iterations) {

    TrigramStats stats(lang);
    for (u_int i = 0; i < iterations; i++)
        stats.process(inputs, count);

    stats.save_stats();

}


