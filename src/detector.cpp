/* 
 * File:   detector.cpp
 * Author: Zosia
 * 
 * Created on December 1, 2013, 11:23 PM
 */

#include <omp.h>
#include <math.h>

#include "../headers/detector.h"

LangDetector::LangDetector() {

    timer = Timer::instance();

    char** langs;
    ref_lang_count = get_saved_stats_langs(&langs);
    refer_stats = new TrigramStats[ref_lang_count];
    for (u_int i = 0; i < ref_lang_count; i++) {
        refer_stats[i].lang = new char[5];
        strcpy(refer_stats[i].lang, langs[i]);
        refer_stats[i].open_stats();
    }
    for (u_int i = 0; i < ref_lang_count; i++) {
        delete [] langs[i];
    }
    delete [] langs;

    probabilities = new double[ref_lang_count];
    indexes = new u_int[ref_lang_count];
}

void LangDetector::text_feed(char* test_fname) {
    tst_stats = new TrigramStats();
    tst_stats->process(test_fname);
}

LangDetector::~LangDetector() {
    delete tst_stats;
    delete [] refer_stats;
}

void LangDetector::process() {

    memset(probabilities, 0.0, ref_lang_count * sizeof (double));
    double time = omp_get_wtime();
    // If there are at least 4 languages to test, let's test them in parallel.
    if (ref_lang_count >= 4) {
        #pragma omp parallel for
        for (u_int lang_i = 0; lang_i < ref_lang_count; lang_i++) {
            process(lang_i);
        }
    } else { // Otherwise inner loop will go parallel.
        for (u_int lang_i = 0; lang_i < ref_lang_count; lang_i++) {
            process(lang_i);
        }
    }
    time = (omp_get_wtime() - time);
    timer->add(time);
    fill_sorted_indexes();
}

void LangDetector::process(u_int lang_i) {
    TrigramStats* ref_stats = &refer_stats[lang_i];

    const TrigramStats::tr_int BINS_COUNT = TrigramStats::POSSIBILITIES - 1;

    // Implementation of X^2 failed, so I came up with SELF-THOUGHT way of 
    // detection; lang stats do not differ much, but it haven't failed for any
    // test provided by me.
    double p = 1.0;
    #pragma omp parallel for reduction(*: p)
    for (TrigramStats::tr_int i = 1; i < BINS_COUNT; i++) {
        // Distance for frequency of i'th trigram;
        double dist_i = fabs(tst_stats->frequency(i) - ref_stats->frequency(i));
        // p is never multiplied by 0 (provided ref stats and tested text
        // contain more than single trigram); For trigrams not existing in any
        // of the stats satement has no effect.
        p *= (1.0 - dist_i);
    }
    probabilities[lang_i] = p;
}

u_int LangDetector::get_saved_stats_langs(char*** langs) {
    const char* path = ".";
    DIR* directory;
    u_int count = 0;
    if ((directory = opendir(path))) {
        while ((readdir(directory))) {
            count++;
        }
    }
    if (count > 2 && (directory = opendir(path))) {
        (*langs) = new char*[count - 2];
        string suffix(TrigramStats::DAT_SUFFIX);
        dirent *file;
        u_int current = 0;
        while ((file = readdir(directory))) { // iterate through the directory
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

void LangDetector::fill_sorted_indexes() {
    memset(indexes, 0, ref_lang_count * sizeof (u_int));
    for (u_int i = 0; i < ref_lang_count; i++) {
        u_int left = 0;
        u_int larger = 0;

        u_int right = ref_lang_count - 1;
        u_int smaller = 0;

        for (u_int j = left; j < ref_lang_count; j++) {
            if (probabilities[j] > probabilities[i]) {
                larger++;
                left += (larger == left) ? 1 : 0;
            }
            if (probabilities[ref_lang_count - j - 1] > probabilities[ref_lang_count - i - 1]) {
                smaller++;
                right -= (smaller == right) ? 1 : 0;
            }
        }
        indexes[larger] = i;
        indexes[smaller] = i;
    }
}


LangDetector::entry LangDetector::operator[](u_int sorted_index) {
    entry e;
    u_int unsorted_index = indexes[sorted_index];
    e.lang = refer_stats[unsorted_index].lang;
    e.probability = probabilities[unsorted_index];
    return e;
}