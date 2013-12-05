/* 
 * File:   Stats.cpp
 * Author: Zosia
 * 
 * Created on November 30, 2013, 7:52 PM
 */

#include "../headers/stats.h"
#include <omp.h>

const char * TrigramStats::DAT_SUFFIX = ".dat";

TrigramStats::TrigramStats() {

    init();
}

TrigramStats::TrigramStats(char* lang) {
    init();
    this->lang = lang;
}

void TrigramStats::init() {
    this->occurances = new tr_int[POSSIBILITIES];
    this->total = 0;
    timer = Timer::instance();

}

TrigramStats::~TrigramStats() {

    if (occurances) {
        delete [] occurances;
    }
}

void TrigramStats::process(char** input_fnames, u_int count) {

    for (u_int i = 0; i < count; i++) { // For each given file

        const char* input_fname = input_fnames[i];
        process(input_fname);
    }
}

void TrigramStats::process(const char* input_fname) {
    char* text = NULL;
    ifstream ifs;
    ifs.open(input_fname);

    if (ifs.is_open()) {

        // Find file len
        ifs.seekg(0, ios::end);
        u_long txt_len = (u_long) ifs.tellg();
        ifs.seekg(0, ios::beg);
        txt_len -= (u_long) ifs.tellg();

        text = new char[txt_len];

        ifs.read(text, txt_len);
        if (!(ifs.fail())) {
            total += txt_len;
            process_text(text, txt_len);
        } else {
            cerr << "Cannot read file " << input_fname << endl;
        }
        delete[] text;
        ifs.close();
    } else {
        cerr << "Cannot open file " << input_fname << endl;
    }
}

void TrigramStats::trigram2index(char* trigram, tr_int* index) {

    trigram[0] = (trigram[0] < 32) ? ' ' : trigram[0];
    trigram[1] = (trigram[1] < 32) ? ' ' : trigram[1];
    trigram[2] = (trigram[2] < 32) ? ' ' : trigram[2];
    (*index) = 0;
    memcpy(index, trigram, LEN * sizeof (char));
}

void TrigramStats::process_text(char* text, u_long txt_len) {

    double time = omp_get_wtime();
    #pragma omp parallel for
    for (tr_int tr_i = 0; tr_i < txt_len - 2; tr_i++) {

        tr_int index;
        trigram2index(text + tr_i, &index);
        if (index != 0x202000 && index != 0x002020) {
            #pragma omp atomic
            occurances[index]++;
        }
    }
    time = (omp_get_wtime() - time);
    timer->add(time);
}

void TrigramStats::save_stats() {
    ofstream ofs;

    ofs.open(string(lang).append(DAT_SUFFIX).c_str());
    for (tr_int i = 0; i < POSSIBILITIES; i++) {
        if (occurances[i] > 0) {
            // content of `(char*) &i` equals to string of i'th available trigram
            ofs << (char*) &i << TAB_CH << occurances[i] << endl;
        }
    }
    ofs.close();

}

void TrigramStats::open_stats() {

    ifstream ifs(string(lang).append(DAT_SUFFIX).c_str());
    if (ifs.is_open()) {

        char trigram[LEN + 1];
        trigram[LEN] = NULL_CH;
        tr_int index;
        while (!ifs.eof()) {
            ifs.getline(trigram, LEN + 1, TAB_CH);
            trigram2index(trigram, &index);
            char occ_str[100];
            memset(occ_str, NULL_CH, 50);
            ifs.getline(occ_str, 50, NL_CH);
            total += (occurances[index] = a2long(occ_str));

        }
    }

    ifs.close();
}

