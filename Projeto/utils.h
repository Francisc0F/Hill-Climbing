#define DEFAULT_RUNS 10000
#define DEFAULT_TREPA_ITER 100

void fill_adjacent_matrix(FILE* f, int* p, int* vert);
int* init_dados(char* nome, int* n, int* k);
int* allocate_matrix(int rows, int cols);
void gera_sol_inicial(int *sol, int v, int nConjunto);
void escreve_sol(int *sol, int vert);
void substitui(int a[], int b[], int n);
void init_rand();
int random_l_h(int min, int max);
float rand_01();
void process_args(int argc, char* argv[], char* filename, int* runs);
void swap(int* a, int* b);
