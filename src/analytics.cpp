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

void analyze(char* lang, char** inputs, int count);

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
        string args = "<count> <lang> <file1> [<file2> ...]";
        usage(argv[0], args.c_str());
    }

    int threads = atoi(argv[1]);
    char* language = argv[2];
    char** inputs = &argv[3];
    int inputs_count = argc - 3;

    if (threads != 0) {
        omp_set_num_threads(threads);
    }

    analyze(language, inputs, inputs_count);

    return 0;
}

////////////////////////////////////////////////////////////////////////////////

void analyze(char* lang, char** inputs, int count) {

    Timer* timer = new Timer();
    TrigramStats stats(lang);
    stats.timer = timer;
    stats.process(inputs, count);
    stats.save();
    
    cout << timer->get_seconds() << "s" << endl;
}


