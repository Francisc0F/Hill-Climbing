

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
    // Constante para avalia��o com penaliza��o
    float ro;

    // Number of bits in individual sol
    int numGenes;
    
    // subset number of counts
    int k;
    //int     capacity;
 
    // Number of generations
    int maxGenerations;
}info;

// Individual (solution)
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
    info EA_param;
}thread_arg_genetic;

void tournament(pchrom pop, info d, pchrom parents);

void genetic_operators(pchrom parents, info d, pchrom offspring);

void crossover(pchrom parents, info d, pchrom offspring);

void mutation(pchrom offspring, info d);

void write_best(chrom x, info d);

void run_for_file_genetico(const char* nome_fich, int runs, info EA_param);

DWORD WINAPI process_file_genetico(LPVOID lpParameter);

void lunch_threads_genetic(char** files, int num_files, int runs, info EA_param);
