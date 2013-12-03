/* 
 * File:   detector.h
 * Author: Zosia
 *
 * Created on December 1, 2013, 11:23 PM
 */

#ifndef DETECTOR_H
#define	DETECTOR_H

#include <iostream>
#include <dirent.h>
#include "stats.h"

using std::cout;

class Detector {
public:

    struct entry {
        const char* lang;
        double probability;
    };

    Timer* timer;

    Detector(char* test_fname, Timer* timer);
    Detector(TrigramStats* tested_stats, TrigramStats* reference_stats, unsigned int reference_count);
    virtual ~Detector();
    void process();

    entry get_entry(unsigned int index);

    unsigned int ref_lang_count;

private:
    TrigramStats* refer_stats;
    TrigramStats* tst_stats;

    double* probabilities;
    
    unsigned int* indexes;

    void process(unsigned int index);
    /**
     * Scans current directory, searching for files named '*.dat'. Extracts language
     * codes from those filenames and creates array of them so that langs points
     * at them.
     * 
     * @param langs where lang codes should be stored
     * @return number of detected lang codes
     */
    static unsigned int get_saved_stats_langs(char*** langs);
    
    void fill_sorted_indexes();
};

#endif	/* DETECTOR_H */

