#include <Windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "algoritmo.h"
#include "utils.h"
#include "main.h"

#define MAX_FILES 10

typedef struct {
	char* file;
	int runs;
}thread_arg_t;



void print_general_results(const char* nome_fich, int vert, float mbf, int k, int* best, int best_custo)
{
	printf("-----------------------------------------\n");
	printf("Ficheiro: %s\n", nome_fich);
	printf("Vertices: %d\n", vert);
	printf("MBF: %f\n", mbf / k);
	printf("Melhor solucao encontrada\n");
	escreve_sol(best, vert);
	printf("Custo final: %2d\n", best_custo);
	printf("-----------------------------------------\n\n");
}

void run_for_file(const char* nome_fich, int runs) {

	int vert, num_iter = DEFAULT_TREPA_ITER, k, custo, best_custo, nConjunto;
	int* grafo, * sol, * best;
	float mbf = 0.0;

	// Preenche matriz de adjacencias
	grafo = init_dados(nome_fich, &vert, &nConjunto);
	sol = allocate_matrix(1, vert);
	best = allocate_matrix(1, vert);

	for (k = 0; k < runs; k++)
	{
		// Gerar solucao inicial
		gera_sol_inicial(sol, vert, nConjunto);
		//escreve_sol(sol, vert);
		// Trepa colinas
		custo = trepa_colinas(sol, grafo, vert, num_iter);
		// Escreve resultados da repeticao k
		//printf("\nRepeticao %d:", k);
		//escreve_sol(sol, vert);
		//printf("Custo final: %2d\n", custo);
		mbf += custo;
		if (k == 0 || custo >= best_custo)
		{
			best_custo = custo;
			substitui(best, sol, vert);
		}
	}

	// Escreve resultados globais
	print_general_results(nome_fich, vert, mbf, k, best, best_custo);


	free(grafo);
	free(sol);
	free(best);
}

DWORD WINAPI process_file(LPVOID lpParameter) {
	thread_arg_t* thread_arg = (thread_arg_t*)lpParameter;
	printf("Processing file: %s\n", thread_arg->file);
	run_for_file(thread_arg->file, thread_arg->runs);
	return NULL;
}

void lunch_threads(char** files,int num_files, int runs) {
	// Create a separate thread for each file
	HANDLE threads[MAX_FILES] = {0};
	for (int i = 0; i < num_files; i++) {
		thread_arg_t* arg = (thread_arg_t*)malloc(sizeof(thread_arg_t));
		if (arg == NULL) {
			printf("Error Alocating thread_arg_t*)malloc(sizeof(thread_arg_t: %d\n", GetLastError());
			exit(1);
		}

		arg->file = files[i];
		arg->runs = runs;
		threads[i] = CreateThread(NULL, 0, process_file, arg, 0, NULL);
		if (threads[i] == NULL) {
			printf("Error creating thread: %d\n", GetLastError());
			exit(1);
		}
	}

	// Wait for all threads to complete
	WaitForMultipleObjects(num_files, threads, TRUE, INFINITE);

	// Close handles to threads
	for (int i = 0; i < num_files; i++) {
		CloseHandle(threads[i]);
	}
	printf("All threads complete.\n");
}

void find_test_files(char** files,int* num_files) {
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
	1 Best - neigbour 1

	#define DEFAULT_RUNS 100.000
	#define DEFAULT_TREPA_ITER 500
	teste.txt -> 5 = MBF 
	file1.txt -> 20, 19.999 MBF
	file2.text -> 15
	file3.txt -> 112
	file4.txt -> 79
	file5.txt -> 


	2 Best - neigbour 1
	#define DEFAULT_RUNS 10000
	#define DEFAULT_TREPA_ITER 100

	teste.txt -> 5 = MBF
	file1.txt -> 20, 19.759501 MBF
	file2.text -> 15, 14.557300 MBF
	file3.txt -> 112, 105.328499 MBF
	file4.txt -> 62, 30.740400 MBF
	file5.txt -> 48, 20.850700 MBF


	3 Best - neigbour 3
	#define DEFAULT_RUNS 10000
	#define DEFAULT_TREPA_ITER 100

	teste.txt -> 5,  4.155600 = MBF
	file1.txt -> 20, 19.115999 MBF
	file2.text -> 15, 12.213600 MBF
	file3.txt -> 112, 102.703300 MBF
	file4.txt -> 48, 18.335400 MBF
	file5.txt -> 34, 11.516500 MBF


	4 Best - neigbour 2
	#define DEFAULT_RUNS 10000
	#define DEFAULT_TREPA_ITER 100

	teste.txt -> 5,  3.864700 = MBF
	file1.txt -> 20, 18.839199 MBF
	file2.text -> 15, 14.417400 MBF
	file3.txt -> 110, 102.142197 MBF
	file4.txt -> 45, 16.268000 MBF
	file5.txt -> 28, 9.194300 MBF


*/
int main(int argc, char* argv[])
{
	char    nome_fich[100];
	int     runs;
	char** files = (char**)calloc(MAX_FILES, sizeof(char*));
	int numfiles = 0;
	find_test_files(files,&numfiles);
	init_rand();

	lunch_threads(files, numfiles, DEFAULT_RUNS);

	return 0;
}
