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
#include "../headers/detector.h"


using namespace std;


void detect(char* text_fname, bool print_results, u_int iterations);

int main(int argc, char** argv) {

    if (argc != 3) {
        const char* args = "<count> <file>";
        usage(argv[0], args);
    } else {
        int threads = atoi(argv[1]);
        char* text_fname = argv[2];

        if (threads != 0) {
            omp_set_num_threads(threads);
        }

        bool print_results = true;
        u_int iterations = 1;
        const char* testrun = getenv("TESTRUN");
        if (testrun != NULL && (*testrun) == 'y') {
            print_results = false;
            const char* iterations_ch = getenv("TESTITERATIONS");
            iterations = atoi(iterations_ch);
            iterations = (iterations > 0) ? iterations : 10;
        }

        detect(text_fname, print_results, iterations); // print results only first time
        Timer* timer = Timer::instance();
        cout << (*timer) << timer->delimiter;

        return 0;
    }

}

void detect(char* text_fname, bool print_results, u_int iterations) {

    LangDetector detector;
    detector.text_feed(text_fname);
    for (u_int i = 0; i < iterations; i++)
        detector.process();

    if (print_results) {
        for (u_int i = 0; i < detector.ref_lang_count; i++) {
            LangDetector::entry entry = detector[i];
            cout << "[" << entry.lang << "] compliance ratio: " << ": " << entry.probability << " / 1.0" << endl;
        }
    }
}
