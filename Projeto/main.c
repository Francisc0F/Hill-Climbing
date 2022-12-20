#include <Windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "algoritmo.h"
#include "utils.h"

#define MAX_FILES 10

typedef struct {
	char* file;
	int runs;
}thread_arg_t;


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
		escreve_sol(sol, vert);
		// Trepa colinas
		custo = trepa_colinas(sol, grafo, vert, num_iter);
		// Escreve resultados da repeticao k
		printf("\nRepeticao %d:", k);
		escreve_sol(sol, vert);
		printf("Custo final: %2d\n", custo);
		mbf += custo;
		if (k == 0 || custo >= best_custo)
		{
			best_custo = custo;
			substitui(best, sol, vert);
		}
	}

	// Escreve resultados globais
	printf("-----------------------------------------");
	printf("Ficheiro: %s\n", nome_fich);
	printf("Vertices: %d\n", vert);
	printf("\n\nMBF: %f\n", mbf / k);
	printf("\nMelhor solucao encontrada");
	escreve_sol(best, vert);
	printf("Custo final: %2d\n", best_custo);
	printf("-----------------------------------------\n");
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

void lunch_threads(char** files, int runs) {
	// Create a separate thread for each file
	HANDLE threads[sizeof(files) / sizeof(char*)];
	int num_files = sizeof(files) / sizeof(char*);
	for (int i = 0; i < num_files; i++) {
		thread_arg_t* arg = (thread_arg_t*)malloc(sizeof(thread_arg_t));
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


void find_test_files(char** files) {
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


	int num_files = 0;

	//do {
	//	if (num_files == MAX_FILES) {
	//		fprintf(stderr, "Too many files in directory\n");
	//		break;
	//	}

	//	if (strlen(findData.cFileName) == 0 || strspn(findData.cFileName, " \t\r\n") == strlen(findData.cFileName)) {
	//		continue;
	//	}


	//
	//	
	//	//size_t s = strlen(findData.cFileName) + 1;
	//	//files[num_files] = (char*)malloc(s);
	//	//
	//	//if (strcpy_s(&files[num_files], s, findData.cFileName) != 0)
	//	//{
	//	//	printf("Error copying string: %d\n", errno);
	//	//	return 1;
	//	//}
	//	num_files++;
	//} while (FindNextFile(hFind, &findData) != 0);



	// Close the search handle
	FindClose(hFind);
}

int main(int argc, char* argv[])
{
	char    nome_fich[100];
	int     runs;
	//process_args(argc, argv, nome_fich, &runs);

	/*if (argc < 2) {
		fprintf(stderr, "Nao foi especificada a diretoria de ficheiros\n");
		return 1;
	}*/

	// allocating memory for 1st dimension
	char** files = (char**)malloc(MAX_FILES, sizeof(char*));



	//todo complete the thread luncher for each file
	for (int i = 0; i < MAX_FILES; i++)
		// allocating memory for 2nd dimension
	{
		files[i] = (char*)calloc(30, sizeof(char));
		scanf_s("%s", files[i]);
	}

	for (int i = 0; i < MAX_FILES; i++)
		printf("%s\n", files[i]);

	for (int i = 0; i < 10; i++) {
		char* str = "some string";
		size_t str_len = strlen(str) + 1;
		strcpy_s(files[i], str_len, str);
	}

	//find_test_files(files);

	//lunch_threads(files, 10);


	init_rand();

	return 0;
}
