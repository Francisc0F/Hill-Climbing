#include <Windows.h>
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

	if (j == tam - 1) {
		printf("\n");
	}
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

	/*
	PrintArgs pargs = {.p = p};
	LoopCode print = { print_element,.p = p, .args = &pargs };
	execute_loop(print, *vert);
	*/
	fclose(f);
	return p;
}

void reverse_vector(int* vector, int start, int end) {
	while (start < end) {
		swap(&vector[start], &vector[end]);
		start++;
		end--;
	}
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

void escreve_sol_bin(int* sol, int vert)
{
	for (int i = 0; i < vert; i++)
		printf("%d", sol[i]);
	putchar('\n');
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
void copy_vector(int a[], int b[], int n)
{	
	if (a == NULL) {
		a = allocate_matrix(1, n);
	}

	int i;
	for (i = 0; i < n; i++)
		a[i] = b[i];
}

void copy_vector_no_aloc(int a[], int b[], int n)
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

void swap(int* a, int* b) {
	int temp = *a;
	*a = *b;
	*b = temp;
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

void print_general_results(const char* nome_fich, int runs, float mbf, int k, int* best, int best_custo)
{
	//printf("\nTrepa colinas - Ficheiro: %10s, Subset k: %3d, MBF: %10f , Custo: %3d", nome_fich, k, (mbf / runs), best_custo);

	printf("\nTrepa colinas - %10s, MBF: %10f , Custo: %3d", nome_fich, (mbf / runs), best_custo);

	//printf("Vertices: %d ", vert);
	//printf("MBF: %f ", mbf / k);
	//escreve_sol(best, vert);
}

void find_test_files(char** files, int* num_files) {
	char currentDir[MAX_PATH];
	GetCurrentDirectoryA(MAX_PATH, currentDir);

	// Set the search pattern to look for .txt files
	char* searchPattern = "*.txt";

	// Combine the current directory path and search pattern into a full search string
	char searchString[MAX_PATH];
	sprintf_s(searchString, MAX_PATH, "%s\\%s", currentDir, searchPattern);

	// Find the first file in the directory that matches the search pattern
	WIN32_FIND_DATA findData = { 0 };
	//Restriction needs to be set as multibyte in carater set
	HANDLE hFind = FindFirstFile(searchString, &findData);
	if (hFind == INVALID_HANDLE_VALUE)
	{
		printf("Error finding first file: %d\n", GetLastError());
		exit(1);
	}


	*num_files = 0;

	do {
		if (num_files == MAX_FILES) {
			fprintf(stderr, "Too many files in directory\n");
			break;
		}

		if (strlen(findData.cFileName) == 0 || strspn(findData.cFileName, " \t\r\n") == strlen(findData.cFileName)) {
			continue;
		}

		size_t s = strlen(findData.cFileName) + 1;
		files[(*num_files)] = (char*)malloc(s);

		if (strcpy_s(files[(*num_files)], s, findData.cFileName) != 0)
		{
			printf("Error copying string: %d\n", errno);
			return 1;
		}
		(*num_files)++;
	} while (FindNextFile(hFind, &findData) != 0);


	// Close the search handle
	FindClose(hFind);
}


/*
	Coin flip 0 or 1
*/
int flip()
{
	if ((((float)rand()) / RAND_MAX) < 0.5)
		return 0;
	else
		return 1;
}

void wait_and_close_threads(int num_files, const HANDLE* threads) {
	// Wait for all threads to complete
	WaitForMultipleObjects(num_files, threads, TRUE, INFINITE);

	// Close handles to threads
	for (int i = 0; i < num_files; i++) {
		CloseHandle(threads[i]);
	}
}
