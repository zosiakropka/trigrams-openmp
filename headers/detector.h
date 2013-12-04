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
#include "timer.h"

using std::cout;

class LangDetector {
public:

    /**
     * Name of the file to be tested for language adherence
     * @param test_fname
     */
    LangDetector();
    
    void text_feed(char* test_fname);
    
    virtual ~LangDetector();
    
    /** Structure for accessing detection results */
    struct entry {
        const char* lang;
        double probability;
    };

    /**
     * Run detection test for text detector was feeded with.
     */
    void process();

    u_int ref_lang_count;

    /**
     * Get language and ratio for index'th best result. detector[0] is the 
     * best one
     * @param index
     * @return stats for index'th result
     */
    entry operator[](u_int sorted_index);

private:
    Timer* timer;

    TrigramStats* refer_stats;
    TrigramStats* tst_stats;

    double* probabilities;

    /**
     */
    u_int* indexes;

    /**
     * Process single language no index
     * @param index
     */
    void process(u_int index);

    /**
     * Scans current directory, searching for files named '*.dat'. Extracts language
     * codes from those filenames and creates array of them so that langs points
     * at them.
     * 
     * @param langs where lang codes should be stored
     * @return number of detected lang codes
     */
    static u_int get_saved_stats_langs(char*** langs);

    /**
     * Fills map of sorted indexes
     */
    void fill_sorted_indexes();
};

#endif	/* DETECTOR_H */

