/*
 Author: Yunjiang Qiu <serein927@gmail.com>
 File: deltasvm.cpp
 Create Date: 2019-03-01 10:20:28
*/

#include <string>
#include <iostream>
#include <algorithm>
#include <map>
#include <string.h>
#include <iomanip>
#include <fstream>
#include <chrono>
#include <set>
#include <vector>
#include <stdlib.h>
using namespace std;
using namespace std::chrono;

string rev_com(string &seq){
    map<char,char> rev;
    rev['A'] = 'T';
    rev['T'] = 'A';
    rev['C'] = 'G';
    rev['G'] = 'C';
    rev['N'] = 'N';

    string seq_rev;
    long int len = seq.length() - 1;
    for (long int i = len; i >= 0; i--){seq_rev += rev[seq[i]];}
    return seq_rev;
}

int main(int argc, char* argv[]){
    const int kmer_len = 10;
    const int tf_n = 94;

    if (argc != 6){
        cerr<<"usage:deltasvm <ref_fasta> <alt_fastq> <svm_weight> <output> <threshold>"<<endl;
        exit(1);
    }

    FILE * fout = fopen (argv[4], "w");
    if (fout == NULL) {
        cerr<<"Failed to open output file."<<endl;
        exit(1);
    }

    cerr<<"Reading thresholds."<<endl;
    ifstream fin_threshold (argv[5]);
    if (fin_threshold){
    }
    else{
        cerr<<"Failed to open threshold file."<<endl;
        exit(1);
    }
    vector<string> tf_name(tf_n);
    vector<double> threshold(tf_n);
    for (int i = 0; i < tf_n; i++) fin_threshold>>tf_name[i]>>threshold[i];
    cerr<<"Loaded all thresholds."<<endl;

    auto start = high_resolution_clock::now();
    std::set<string> kmers;
    cerr<<"Scanning possible 10-mers."<<endl;
    ifstream fin1 (argv[1]);
    ifstream fin2 (argv[2]);
    string seqid_ref, seqid_alt, seq_ref, seq_alt, kmer_ref, kmer_alt;
    if (fin1 and fin2){
        while (fin1>>seqid_ref>>seq_ref){
            fin2>>seqid_alt>>seq_alt;

            std::transform(seq_ref.begin(), seq_ref.end(), seq_ref.begin(), [](char c){ return std::toupper(c); });
            std::transform(seq_alt.begin(), seq_alt.end(), seq_alt.begin(), [](char c){ return std::toupper(c); });

            int seqlen = seq_ref.length() > seq_alt.length() ? seq_ref.length() : seq_alt.length();

            for (int i = 0; i < seqlen - kmer_len + 1; i++){
                kmer_ref = seq_ref.substr(i, kmer_len);
                kmer_alt = seq_alt.substr(i, kmer_len);
                kmers.insert(kmer_alt);
                kmers.insert(kmer_ref);
            }
        }
    }
    fin1.close();
    fin2.close();
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop - start);
    cerr<<"Read in "<<kmers.size()<<" unique 10-mers using "<<duration.count()<<" microseconds"<<endl;

    start = high_resolution_clock::now();
    cerr<<"Reading SVM weights."<<endl;
    map< string, vector<double> > svm_scores;
    ifstream fin (argv[3]);
    if (fin){
        vector<double> score(tf_n);
        string kmer, kmer_rev;
        while (fin>>kmer){
            for (int i = 0; i < tf_n; i++){
                fin>>score[i];
            }
            if (kmers.find(kmer) != kmers.end()) svm_scores[kmer] = score;
            kmer_rev = rev_com(kmer);
            if (kmers.find(kmer_rev) != kmers.end()) svm_scores[kmer_rev] = score;
        }
    }
    else{
        cerr<<"Failed to open weight file."<<endl;
        exit(1);
    }
    fin.close();
    stop = high_resolution_clock::now();
    duration = duration_cast<microseconds>(stop - start);
    cerr<<"Finished reading SVM weights using "<<duration.count()<<" microseconds"<<endl;

    start = high_resolution_clock::now();
    cerr<<"Scanning sequences."<<endl;
    fin1.open(argv[1]);
    fin2.open(argv[2]);
    if (fin1 and fin2){
        while (fin1>>seqid_ref>>seq_ref){
            fin2>>seqid_alt>>seq_alt;
            seqid_ref = seqid_ref.erase(0, 1);
            seqid_alt = seqid_alt.erase(0, 1);
            if (seqid_ref != seqid_alt){
                cerr<<"The sequence id in "<<seqid_ref<<" is not the same as "<<seqid_alt<<" and is skipped."<<endl;
                continue;
            }

            //std::transform(seq_ref.begin(), seq_ref.end(), seq_ref.begin(), [](char c){ return std::toupper(c); });
            //std::transform(seq_alt.begin(), seq_alt.end(), seq_alt.begin(), [](char c){ return std::toupper(c); });

            //cerr<<seqid_ref<<"\t"<<seq_ref<<"\t"<<seqid_alt<<"\t"<<seq_alt<<endl;
            vector<double> deltasvm_score(tf_n, 0);
            int seqlen = seq_ref.length() > seq_alt.length() ? seq_ref.length() : seq_alt.length();
            for (int i = 0; i < seqlen - kmer_len + 1; i++){
                kmer_ref = seq_ref.substr(i, kmer_len);
                kmer_alt = seq_alt.substr(i, kmer_len);
                for (int j = 0; j < tf_n; j++){
                    deltasvm_score[j] += svm_scores[kmer_alt][j] - svm_scores[kmer_ref][j];
                }
                //cerr<<kmer_ref<<"\t"<<kmer_alt<<"\t"<<svm_scores[kmer_ref]<<"\t"<<svm_scores[kmer_alt]<<endl;
            }
            for (int j = 0; j < tf_n; j++){
                string pred("None");
                if (abs(deltasvm_score[j]) > threshold[j]){
                    pred = deltasvm_score[j] > 0 ? "Loss" : "Gain";
                }
                fprintf (fout, "%s\t%.6f\t%s\t%s\n", seqid_ref.c_str(), deltasvm_score[j], tf_name[j].c_str(), pred.c_str());
            }
        }
    }
    fclose(fout);
    fin1.close();
    fin2.close();
    stop = high_resolution_clock::now();
    duration = duration_cast<microseconds>(stop - start);
    cerr<<"Finished Scanning sequences using "<<duration.count()<<" microseconds"<<endl;

    return 0;
}

