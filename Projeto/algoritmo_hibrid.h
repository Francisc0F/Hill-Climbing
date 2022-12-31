#include "algoritmo_genetico.h"
#pragma once

typedef struct {
    char* file;
    int runs;
    int recombination_opt;
    int trepa_colinas_runs;
    int hibrid;
    info EA_param;
    float avg;
    chrom best_ever;
}thread_arg_genetic_hibrid;

DWORD WINAPI process_file_genetico_hibrid(LPVOID lpParameter);

void run_for_file_genetico_hibrid(thread_arg_genetic_hibrid* args);

void lunch_threads_hibrid(char** files, int num_files, int runs, info EA_param, int recombination_opt, int hibrid_opt, int trepa_colinas_runs);