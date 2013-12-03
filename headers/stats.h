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

    void t2i(char* trigram, unslong* index) {
        (*index) = 0;
        for (unsint i = 0; i < LEN; i++) {
            if ((short) trigram[i] < 32) {
                trigram[i] = ' ';
            }
        }
        memcpy(index, trigram, LEN * sizeof (char));
    }

    void i2t(const unslong* index, char* trigram) {
        memcpy(trigram, index, LEN * sizeof (char));
        trigram[LEN] = '\0';
    }

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

