#pragma once
#include "Windows.h"
typedef struct 
{
    // population size
    int popsize;
    // probability of mutation
    float pm;
    // probability of re-combination
    float pr;
    // tournament size
    int tsize;

    // Number of bits in individual sol
    int numGenes;
    
    // subset number of counts
    int k;
    //int     capacity;
 
    // Number of generations
    int maxGenerations;
}info;

typedef struct individual chrom, * pchrom;

struct individual
{
    // 0,1 vector defining the 2 groups  with k = 4 - [0, 1, 1, 1, 0]
    int* sol; 
    
    // count of sol elements = 1 that have more(maximization) edges between them
    float  fitness;
    
    // this can happen on mutation to have more than k elements with 1, so this is invalid
    int  valido;
};


typedef struct {
    char* file;
    int runs;
    int recombination_opt;
    info EA_param;
    float avg;
    chrom best_ever;
}thread_arg_genetic;

void tournament(pchrom pop, info d, pchrom parents);

void genetic_operators(pchrom parents, info d, pchrom offspring, int recombination);

void crossover(pchrom parents, info d, pchrom offspring);

void mutation(pchrom offspring, info d);

void write_best(chrom x, info d);

void run_for_file_genetico(thread_arg_genetic* args);

DWORD WINAPI process_file_genetico(LPVOID lpParameter);

void lunch_threads_genetic(char** files, int num_files, int runs, info EA_param, int recombination_opt);

pchrom initialize_parents(info EA_param);

chrom get_best(pchrom pop, info d, chrom best);

void copy_chrom(pchrom a, pchrom b, int numGenes);

void copy_chrom_no_aloc(pchrom a, pchrom b, int numGenes);

pchrom init_pop(info d);

void evaluate(pchrom pop, info d, int* mat);