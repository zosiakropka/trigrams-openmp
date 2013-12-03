/* 
 * File:   detector.cpp
 * Author: Zosia
 * 
 * Created on December 1, 2013, 11:23 PM
 */

#include <omp.h>
#include <math.h>

#include "../headers/detector.h"

Detector::Detector(char* test_fname, Timer* timer) {

    this->timer = timer;

    tst_stats = new TrigramStats();
    tst_stats->timer = timer;
    tst_stats->process(test_fname);

    char** langs;
    ref_lang_count = get_saved_stats_langs(&langs);
    refer_stats = new TrigramStats[ref_lang_count];
    for (unsigned int i = 0; i < ref_lang_count; i++) {
        refer_stats[i].timer = timer;
        refer_stats[i].lang = new char[5];
        strcpy(refer_stats[i].lang, langs[i]);
        refer_stats[i].open();
    }
    for (unsigned int i = 0; i < ref_lang_count; i++) {
        delete [] langs[i];
    }
    delete [] langs;

    probabilities = new double[ref_lang_count];
}

Detector::~Detector() {
    delete tst_stats;
    delete [] refer_stats;
}

void Detector::process() {

    double time = omp_get_wtime();
    if (ref_lang_count > 1) {
        #pragma omp parallel for
        for (unsigned int lang_i = 0; lang_i < ref_lang_count; lang_i++) {
            process(lang_i);
        }
    } else {
        for (unsigned int lang_i = 0; lang_i < ref_lang_count; lang_i++) {
            process(lang_i);
        }
    }
    time = (omp_get_wtime() - time);
    timer->add(time);
    fill_sorted_indexes();
}

void Detector::process(unsigned int lang_i) {
    TrigramStats* ref_stats = &refer_stats[lang_i];

    double p = 1.0;
    // It will go parallel if nested parallel blocks are set or if there is only one reference file.
    #pragma omp parallel for reduction(*: p)
    for (size_t i = 0; i < TrigramStats::POSSIBILITIES; i++) {

        // Distance for requency of i'th trigram 
        double p_p = (1.0 - fabs(tst_stats->frequency(i) - ref_stats->frequency(i)));
        p *= (p_p * p_p);
    }
    probabilities[lang_i] = p;

}

Detector::entry Detector::get_entry(unsigned int index) {
    entry e;
    unsigned int sorted_index = indexes[index];
    e.lang = refer_stats[sorted_index].lang;
    e.probability = probabilities[sorted_index];
    return e;
}

unsigned int Detector::get_saved_stats_langs(char*** langs) {
    const char* path = ".";
    DIR* directory;
    int count = 0;
    if ((directory = opendir(path))) {
        while ((readdir(directory))) {
            count++;
        }
    }
    if (count > 2 && (directory = opendir(path))) {
        (*langs) = new char*[count - 2];
        string suffix(TrigramStats::DAT_SUFFIX);
        dirent *file;
        unsigned int current = 0;
        while ((file = readdir(directory))) {
            string filename = string(file->d_name);
            int suffix_length = suffix.length(),
                    suffix_start = filename.length() - suffix.length(),
                    suffix_end = filename.length();
            if (filename.length() >= suffix.length() && !(filename.compare(suffix_start, suffix_end, suffix))) {
                filename.replace(suffix_start, suffix_length, string(""));
                (*langs)[current] = new char[5];
                strcpy((*langs)[current], filename.c_str());
                current++;
            }
        }
        return current;
    }
    return 0;
}

void Detector::fill_sorted_indexes() {
    indexes = new unsigned int[ref_lang_count];
    for (unsigned int i = 0; i < ref_lang_count; i++) {
        unsigned int left = 0;
        unsigned int larger = 0;
        for (unsigned int j = left; j < ref_lang_count; j++) {
            if (probabilities[j] > probabilities[i]) {
                larger++;
                left += (larger == left) ? 1 : 0;
            }
        }
        indexes[larger] = i;
    }
}