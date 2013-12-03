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


void detect(char* text_fname);

int main(int argc, char** argv) {

    if (argc != 3) {
        string args = "<count> <file>";
        usage(argv[0], args.c_str());
    }

    int threads = atoi(argv[1]);
    char* text_fname = argv[2];

    if (threads != 0) {
        omp_set_num_threads(threads);
    }

    detect(text_fname);

    return 0;
}

void detect(char* text_fname) {

    Timer* timer = new Timer();
    Detector detector(text_fname, timer);
    detector.process();

    for (unsigned int i = 0; i < detector.ref_lang_count; i++) {
        Detector::entry entry = detector.get_entry(i);
        cout << entry.lang << ": " << entry.probability * 100 << '%' << endl;
    }
}
