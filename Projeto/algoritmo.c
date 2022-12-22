#include <Windows.h>
#include <stdio.h>
#include <stdlib.h>
#include "algoritmo.h"
#include "funcao.h"
#include "utils.h"

// Gera um vizinho
// Parametros: solucao actual, vizinho, numero de vertices
//swap two vertices
void gera_vizinho(int a[], int b[], int n)
{
	int i, p1, p2;

	//copia elementos
	for (i = 0; i < n; i++)
		b[i] = a[i];

	// Encontra posicao com valor 0
	do
		p1 = random_l_h(0, n - 1);
	while (b[p1] != 0);

	// Encontra posicao com valor 1
	do
		p2 = random_l_h(0, n - 1);
	while (b[p2] != 1);

	// Troca
	b[p1] = 1;
	b[p2] = 0;
}

void gera_vizinho2(int a[], int b[], int n)
{
	int i, p1, p2;

	//copia elementos
	for (i = 0; i < n; i++)
		b[i] = a[i];

	do {
		p1 = random_l_h(0, n - 2);
	} while (b[p1] == b[p1 + 1]);

	swap(&b[p1], &b[p1 + 1]);
}

void gera_vizinho3(int a[], int b[], int n)
{
	int i, p1, p2;

	//copia elementos
	for (i = 0; i < n; i++)
		b[i] = a[i];

	do {
		p1 = random_l_h(0, n - 3);
	} while (b[p1] == b[p1 + 2]);

	swap(&b[p1], &b[p1 + 2]);
}

/* 
	Reverse from random p1 below half and p2 above half
*/
void gera_vizinho4(int a[], int b[], int n)
{
	//copia elementos
	for (int i = 0; i < n; i++)
		b[i] = a[i];


	int p1 = random_l_h(0, n / 2 - 1);
	int p2 = random_l_h(n / 2, n -1);
	//printf("pos %d , %d\n", p1, p2);

	reverse_vector(b, p1, p2);
}

// Trepa colinas first-choice
// Parametros: solucao, matriz de adjacencias, numero de vertices e numero de iteracoes
// Devolve o custo da melhor solucao encontrada
int trepa_colinas(int sol[], int* mat, int vert, int num_iter)
{
	int* nova_sol, custo, custo_viz, i;

	nova_sol = allocate_matrix(1, vert);

	// Avalia solucao inicial
	custo = calcula_fit(sol, mat, vert);
	for (i = 0; i < num_iter; i++)
	{
		// Gera vizinho
		//gera_vizinho(sol, nova_sol, vert);
		//gera_vizinho2(sol, nova_sol, vert);
		//gera_vizinho3(sol, nova_sol, vert);
		gera_vizinho4(sol, nova_sol, vert);

		// Avalia vizinho
		custo_viz = calcula_fit(nova_sol, mat, vert);
		// Aceita vizinho se o custo aumentar (problema de maximizacao) e aceita planaltos
		//queremos apanhar o maior numero de arestas entre os elementos
		if (custo_viz >= custo)
		{
			copy_vector(sol, nova_sol, vert);
			custo = custo_viz;
		}
	}
	free(nova_sol);
	return custo;
}

void run_for_file_trepa_colinas(const char* nome_fich, int runs) {

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
			copy_vector(best, sol, vert);
		}
	}

	// Escreve resultados globais
	print_general_results(nome_fich, vert, mbf, k, best, best_custo);

	free(grafo);
	free(sol);
	free(best);
}

DWORD WINAPI process_file_trepa_colinas(LPVOID lpParameter) {
	thread_arg_t* thread_arg = (thread_arg_t*)lpParameter;
	printf("Processing file: %s\n", thread_arg->file);
	run_for_file_trepa_colinas(thread_arg->file, thread_arg->runs);
	return NULL;
}

void lunch_threads(char** files, int num_files, int runs) {
	// Create a separate thread for each file
	HANDLE threads[MAX_FILES] = { 0 };
	for (int i = 0; i < num_files; i++) {
		thread_arg_t* arg = (thread_arg_t*)malloc(sizeof(thread_arg_t));
		if (arg == NULL) {
			printf("Error Alocating thread_arg_t*)malloc(sizeof(thread_arg_t: %d\n", GetLastError());
			exit(1);
		}

		arg->file = files[i];
		arg->runs = runs;
		threads[i] = CreateThread(NULL, 0, process_file_trepa_colinas, arg, 0, NULL);
		if (threads[i] == NULL) {
			printf("Error creating thread: %d\n", GetLastError());
			exit(1);
		}
	}

	wait_and_close_threads(num_files, threads);
	
	printf("All threads complete.\n");
}