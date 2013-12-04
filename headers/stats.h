/* 
 * File:   Stats.h
 * Author: Zosia
 *
 * Created on November 30, 2013, 7:52 PM
 */

#include <iostream>
#include <fstream>
#include <cstring>
#include <sstream>

#include "utils.h"
#include "timer.h"

using namespace std;

#ifndef	STATS_H
#define     STATS_H

#define NULL_CH '\0'
#define NL_CH '\n'
#define TAB_CH '\t'

/**
 * Class collecting trigram stats
 */
class TrigramStats {
public:

    typedef u_int32_t tr_int;
    static const tr_int POSSIBILITIES = 265 * 265 * 265 * sizeof (char);

    /**
     * Frequency getter of the trigram with given index
     * @param index
     * @return frequency (occurances / total)
     */
    double inline frequency(tr_int index) {
        return (double) occurances[index] / (double) total;
    }

    char* lang;

    TrigramStats();
    TrigramStats(char* lang);

    ~TrigramStats();

    /**
     * 
     * @param inputs
     * @param count
     */
    void process(char** input_fnames, u_int count);
    void process(const char* input_fname);

    void open_stats();
    void save_stats();

    /** Suffix of the files containing language trigram stats */
    static const char * DAT_SUFFIX;


private:

    /** Common timer */
    Timer* timer;

    /** Convert 3-character trigram into tr_int number 
     * (further used as index in occurances counter) */
    void trigram2index(char* trigram, tr_int* index);

    /** Occurances counter. Its indexes are strings numberized by t2index() */
    tr_int *occurances;

    /** Total number of non-unique trigrams in analyzed texts*/
    u_long total;

    /**
     * Process text and store analyze
     * 
     * @param text text to analyze
     * @param txt_len length of the text to analyze
     */
    void process_text(char* text, u_long txt_len);

    /** Initialize stuff*/
    void init();

    /** Length of trigram, just for convenience */
    static const u_int LEN = 3;
};

#endif	/* STATS_H */

