/* 
 * File:   Stats.cpp
 * Author: Zosia
 * 
 * Created on November 30, 2013, 7:52 PM
 */

#include "../headers/stats.h"
#include <omp.h>

#define NULLCH '\0'

const char * TrigramStats::COUNT_SUFFIX = ".count";
const char * TrigramStats::DAT_SUFFIX = ".dat";

TrigramStats::TrigramStats() {

    this->occurances = new unslong[POSSIBILITIES];
    this->total = 0;
}

TrigramStats::TrigramStats(char* lang) {
    this->lang = lang;
    this->occurances = new unslong[POSSIBILITIES];
    this->total = 0;
}

TrigramStats::~TrigramStats() {

    if (occurances) {
        delete [] occurances;
    }
}

double TrigramStats::frequency(size_t index) {

    return (double) occurances[index] / (double) total;
}

void TrigramStats::process(char** input_fnames, int count) {

    for (int i = 0; i < count; i++) { // For each given file

        const char* input_fname = input_fnames[i];
        process(input_fname);
    }
}

void TrigramStats::process(const char* input_fname) {
    char* text = NULL;
    ifstream ifs;
    ifs.open(input_fname);

    if (ifs.is_open()) {

        // File size
        ifs.seekg(0, ios::end);
        size_t size = (size_t) ifs.tellg();
        ifs.seekg(0, ios::beg);
        size -= (size_t) ifs.tellg();

        text = new char[size];

        ifs.read(text, size);
        if (!(ifs.fail())) {
            total += size;
            process_text(text, size);
        } else {
            cerr << "Cannot read file " << input_fname << endl;
        }
        delete[] text;
        ifs.close();
    } else {
        cerr << "Cannot open file " << input_fname << endl;
    }
}

void TrigramStats::process_text(char* text, size_t size) {

    double time = omp_get_wtime();
    //#pragma omp parallel for
    for (unslong i = 0; i < size - 2; i++) {
        unslong index;
        t2i(text + i, &index);
        //#pragma omp atomic
        occurances[index]++;
        char* trigram = new char[LEN + 1];
        i2t(&index, trigram);
    }
    time = (omp_get_wtime() - time);
    timer->add(time);
}

void TrigramStats::save() {
    ofstream ofs;

    ofs.open(string(lang).append(DAT_SUFFIX).c_str());
    char* trigram = new char[LEN + 1];
    for (unslong i = 0; i < POSSIBILITIES; i++) {
        if (occurances[i] > 0) {
            i2t(&i, trigram);
            ofs << trigram << '\t' << occurances[i] << endl;
        }
    }
    ofs.close();

    //    ofs.open(string(lang).append(COUNT_SUFFIX).c_str());
    //    ofs << total << '\t' << POSSIBILITIES;
    //    ofs.close();
}

void TrigramStats::open() {
    //    ifstream ifs_count(string(lang).append(COUNT_SUFFIX).c_str());
    //    if (ifs_count.is_open()) {
    //        size_t tmp_size = 100;
    //        char tmp[100];
    //        memset(tmp, '\0', tmp_size);
    //        ifs_count.getline(tmp, tmp_size, '\t');
    //        total = a2i(tmp);
    //        memset(tmp, '\0', tmp_size);
    //        ifs_count.getline(tmp, tmp_size, '\t');
    //        POSSIBILITIES = a2i(tmp);
    //    }
    //    ifs_count.close();

    ifstream ifs(string(lang).append(DAT_SUFFIX).c_str());
    if (ifs.is_open()) {

        char trigram[LEN + 1];
        trigram[LEN] = '\0';
        unslong index;
        while (!ifs.eof()) {
            ifs.getline(trigram, LEN + 1, '\t');
            t2i(trigram, &index);
            char occ_str[100];
            memset(occ_str, '\0', 50);
            ifs.getline(occ_str, 50, '\n');
            total += (occurances[index] = a2i(occ_str));

        }
    }

    ifs.close();
}


//TrigramStats::entry TrigramStats::operator[](size_t index) {
//    struct entry e;
//    e.trigram = trigrams[index];
//    e.occurances = occurances[index];
//    return e;
//}