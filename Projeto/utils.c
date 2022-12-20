#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "utils.h"

// Leitura do ficheiro de input
// Recebe: nome do ficheiro, numero de vertices (ptr), numero de iteracoes (ptr)
// Devolve a matriz de adjacencias



typedef void (*FuncPtr)(int*, int, int, int);
typedef struct {
	int* p;
} PrintArgs;

typedef struct {
	int* p;
	int n; // value
} SetValueArgs;


void print_element(void* args, int i, int j, int tam) {
	PrintArgs* a = (PrintArgs*)args;
	printf("%d ", a->p[i * tam + j]);
};

typedef struct {
	void (*func)(void* args, int, int, int);
	int* p; // Pointer to the array
	int i;
	int j;
	void* args;
} LoopCode;


void set_value(void* args, int i, int j, int tam) {
	SetValueArgs* a = (SetValueArgs*)args;
	a->p[i * tam + j] = a->n;
}

void execute_loop(LoopCode loop_code, int tam) {
	for (int i = 0; i < tam; i++) {
		for (int j = 0; j < tam; j++) {
				loop_code.func(loop_code.args, i, j, tam);
		}
		printf("\n");
	}
}

void fill_adjacent_matrix(FILE* f, int* p, int* vert)
{
	int count = 1;
	do {
		int auxI = 0;
		int auxJ = 0;
		count = fscanf_s(f, "e %d %d\n", &auxI, &auxJ);
		if (count > 0) {
			auxI--;
			auxJ--;
			p[auxI * (*vert) + auxJ] = 1;
		}
	} while (count > 0);
}

int* allocate_matrix(int rows, int cols) {
	// Allocate memory for the array
	int* matrix = malloc(sizeof(int) * rows * cols);

	// Check if the allocation was successful
	if (!matrix) {
		printf("Error allocating memory for the matrix\n");
		exit(1);
	}

	return matrix;
}

FILE* open_file(const char* filename) {
	// Open the file for reading
	FILE* f = NULL;
	errno_t err = fopen_s(&f, filename, "r");

	// Check if the file was successfully opened
	if (err != 0) {
		printf("Error accessing file: %s\n", filename);
		exit(1);
	}

	return f;
}


int* init_dados(char* nome, int* vert, int* subsetNum)
{
	FILE* f = open_file(nome);

	int edges = 0;
	fscanf_s(f, "k %d\n", subsetNum);
	fscanf_s(f, "p edge %d %d\n", vert, &edges);

	int* p = allocate_matrix((*vert), (*vert));

	SetValueArgs args = {.n = 0, .p = p};
	LoopCode code = { set_value, .args  = &args };
	execute_loop(code, *vert);

	fill_adjacent_matrix(f, p, vert);

	PrintArgs pargs = {  .p = p };
	LoopCode print = { print_element,.p = p, .args = &pargs };
	execute_loop(print, *vert);

	fclose(f);
	return p;
}




/*
 * Generates an initial solution
 * sol: array to store the solution
 * v: size of the 1D array
 * subsetNum: number of elements to include in the solution
 */
void gera_sol_inicial(int* sol, int v, int subsetNum)
{
	int i, x;

	// Initialize all elements of the solution array to 0
	for (i = 0; i < v; i++)
		sol[i] = 0;

	// Randomly select subsetNum elements and set their value to 1
	for (i = 0; i < subsetNum; i++)
	{
		do
			x = random_l_h(0, v - 1);
		while (sol[x] != 0);
		sol[x] = 1;
	}
}

// Escreve solucao
// Parametros: solucao e numero de vertices
void escreve_sol(int* sol, int vert)
{
	int i;

	printf("\nSubset A: ");
	for (i = 0; i < vert; i++)
		if (sol[i] == 0)
			printf("%2d  ", i + 1);
	printf("\nSubset B: ");
	for (i = 0; i < vert; i++)
		if (sol[i] == 1)
			printf("%2d  ", i +1);
	printf("\n");
}

// copia vector b para a (tamanho n)
void substitui(int a[], int b[], int n)
{
	int i;
	for (i = 0; i < n; i++)
		a[i] = b[i];
}

// Inicializa o gerador de numeros aleatorios
void init_rand()
{
	srand((unsigned)time(NULL));
}

// Devolve valor inteiro aleatorio entre min e max
int random_l_h(int min, int max)
{
	return min + rand() % (max - min + 1);
}

// Devolve um valor real aleatorio do intervalo [0, 1]
float rand_01()
{
	return ((float)rand()) / RAND_MAX;
}


void process_args(int argc, char* argv[], char* filename, int* runs) {
	// Set default values
	*runs = DEFAULT_RUNS;

	if (argc == 3) {
		// Set the number of runs from the command-line argument
		*runs = atoi(argv[2]);
		// Set the filename from the command-line argument
		strcpy_s(filename, sizeof(filename), argv[1]);
	}
	else if (argc == 2) {
		// Set the number of runs to the default value
		*runs = DEFAULT_RUNS;
		// Set the filename from the command-line argument
		strcpy_s(filename, sizeof(filename), argv[1]);
	}
	else {
		// Prompt the user for the filename
		printf("Nome ficheiro: ");
		gets(filename);
	}

	// Check if the number of runs is valid
	if (*runs <= 0) {
		exit(0);
	}
}