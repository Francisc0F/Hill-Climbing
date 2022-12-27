#define MAX_FILES 10

//#define DEFAULT_RUNS 10000

#define DEFAULT_TREPA_ITER 100

//#define DEFAULT_RUNS 100
#define DEFAULT_RUNS 100

#define DEFAULT_PM_RATE 0.01
#define DEFAULT_PR_RATE 0.1

#define DEFAULT_POP_SIZE 30 // pair number only
#define DEFAULT_TOURNAMENT_SIZE 2
#define DEFAULT_MAX_GENERATIONS 100



void fill_adjacent_matrix(FILE* f, int* p, int* vert);
int* init_dados(char* nome, int* n, int* k);
int* allocate_matrix(int rows, int cols);
void gera_sol_inicial(int *sol, int v, int nConjunto);
void escreve_sol(int *sol, int vert);
void copy_vector(int a[], int b[], int n);
void init_rand();
int random_l_h(int min, int max);
float rand_01();
void process_args(int argc, char* argv[], char* filename, int* runs);
void swap(int* a, int* b);
void reverse_vector(int* vector, int start, int end);
void print_general_results(const char* nome_fich, int runs, float mbf, int k, int* best, int best_custo);
void find_test_files(char** files, int* num_files);
int flip();
void wait_and_close_threads(int num_files, const HANDLE* threads);
void copy_vector_no_aloc(int a[], int b[], int n);
void escreve_sol_bin(int* sol, int vert);