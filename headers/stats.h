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

using namespace std;

#ifndef	STATS_H
#define     STATS_H

#define NULL_CHAR (char) 0

class TrigramStats {
public:

    static const unslong POSSIBILITIES = 265 * 265 * 265 * sizeof (char);

    struct entry {
        char trigram[4];
        unslong occurances;
        double frequency;
    };

    Timer* timer;

    unslong *occurances;
    double frequency(unslong index);

    char* lang;

    size_t total;

    TrigramStats();
    TrigramStats(char* lang);

    ~TrigramStats();

    /**
     * 
     * @param inputs
     * @param count
     */
    void process(char** input_fnames, int count);
    void process(const char* input_fname);

    void open();
    void save();

    static const char * COUNT_SUFFIX;
    static const char * DAT_SUFFIX;

private:

    void t2i(char* trigram, unslong* index);

    void i2t(const unslong* index, char* trigram);

    static const unsigned int LEN = 3;

    /**
     * Process text and store analyze to given stats container.
     * 
     * @param text text to analyze
     * @param size size of the text to analyze
     */
    void process_text(char* text, size_t size);

};

#endif	/* STATS_H */

