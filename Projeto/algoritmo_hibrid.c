#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>
#include "utils.h"
#include "algoritmo_hibrid.h"
#include "algoritmo.h"
#include "algoritmo_genetico.h"


DWORD WINAPI process_file_genetico_hibrid(LPVOID lpParameter) {
	thread_arg_genetic_hibrid* thread_arg = (thread_arg_genetic_hibrid*)lpParameter;
	run_for_file_genetico_hibrid(thread_arg);
	return NULL;
}

void run_for_file_genetico_hibrid(thread_arg_genetic_hibrid* args) {

	info EA_param = args->EA_param;
	char* nome_fich = args->file;
	int runs = args->runs;
	int recombination_opt = args->recombination_opt;

	pchrom pop = NULL, parents = NULL;
	chrom best_ever = { 0 };
	int vert, nConjunto;

	int* adjacent_matrix = init_dados(nome_fich, &vert, &nConjunto);

	args->EA_param.numGenes = vert;
	args->EA_param.k = nConjunto;
	int hibrid = args->hibrid;
	int trepa_runs = args->trepa_colinas_runs;
	EA_param.numGenes = vert;
	EA_param.k = nConjunto;

	float mbf = 0.0;
	int r, gen_actual;


	// Do genetic runs times
	for (r = 0; r < runs; r++)
	{
		chrom best_run = { 0 };

		// Generate initial population
		pop = init_pop(EA_param);
		
		if (hibrid == 1 || hibrid == 3) {
			for (int i = 0; i < args->EA_param.popsize; i++)
			{
				//printf("running trepa in %d nome_fich %s\n", i, nome_fich);

				trepa_colinas(pop[i].sol, adjacent_matrix, args->EA_param.numGenes, trepa_runs, 1, 1);
			}
		}
		
		evaluate(pop, EA_param, adjacent_matrix);
		gen_actual = 1;

		copy_chrom(&best_run, &pop[0], EA_param.numGenes);
		best_run = get_best(pop, EA_param, best_run);
		
		parents = initialize_parents(EA_param);

		while (gen_actual <= EA_param.maxGenerations)
		{
			tournament(pop, EA_param, parents);
			genetic_operators(parents, EA_param, pop, recombination_opt);
			evaluate(pop, EA_param, adjacent_matrix);
			best_run = get_best(pop, EA_param, best_run);
			gen_actual++;
		}

		// Contagem das solu��es inv�lidas
		int inv, i;
		for (inv = 0, i = 0; i < EA_param.popsize; i++)
			if (pop[i].valido == 0)
				inv++;
		mbf += best_run.fitness;

		if (r == 0 || best_run.fitness >= best_ever.fitness) {
			copy_chrom(&best_ever, &best_run, EA_param.numGenes);
			
			free(best_run.sol);
		}

		if (hibrid == 2 || hibrid == 3) {
			trepa_colinas(best_ever.sol, adjacent_matrix, args->EA_param.numGenes, trepa_runs, 1, 1);
		}
		
		free(parents);

		for (int x = 0; x < EA_param.popsize; x++)
			free(pop[x].sol);

		free(pop);

	}

	copy_chrom(&args->best_ever, &best_ever, EA_param.numGenes);
	args->avg = mbf / r;
	free(adjacent_matrix);
	free(best_ever.sol);
}


void print_general_results_hibrid(const char* nome_fich, chrom best_ever, float avg, int r, info EA_param)
{

	//printf("\nGenetico      - Ficheiro: %10s, Subset k: %3d, MBF: %10f , Custo: %3f", nome_fich, EA_param.k, avg, best_ever.fitness);
	//printf("\nGenetico      - %10s, MBF: %10f , Custo: %3f, Pop: %2d, Pr: %2f, Pm: %2f", nome_fich, avg, best_ever.fitness, EA_param.popsize, EA_param.pr, EA_param.pm);
	//printf("\n%10s,%f,%f,Pop: %d,Pr: %f,Pm: %f", nome_fich, avg, best_ever.fitness, EA_param.popsize, EA_param.pr, EA_param.pm);
	printf("\n%10s,%f,%f", nome_fich, avg, best_ever.fitness);
	//printf("\n best_ever valida = %d", best_ever.valido);
	//escreve_sol(best_ever.sol, EA_param.numGenes);
}

void lunch_threads_hibrid(char** files, int num_files, int runs, info EA_param, int recombination_opt, int hibrid_opt, int trepa_colinas_runs) {
	HANDLE threads[MAX_FILES] = { 0 };
	thread_arg_genetic_hibrid* thread_args[MAX_FILES] = { 0 };
	for (int i = 0; i < num_files; i++) {
		thread_arg_genetic_hibrid* arg = (thread_arg_genetic_hibrid*)malloc(sizeof(thread_arg_genetic_hibrid));
		if (arg == NULL) {
			printf("Error Alocating thread_arg_genetic_hibrid*)malloc(sizeof(thread_arg_genetic_hibrid: %d\n", GetLastError());
			exit(1);
		}

		arg->file = files[i];
		arg->runs = runs;
		arg->runs = runs;
		arg->EA_param = EA_param;
		arg->recombination_opt = recombination_opt;
		arg->hibrid = hibrid_opt;
		arg->trepa_colinas_runs = trepa_colinas_runs;
		thread_args[i] = arg;
		threads[i] = CreateThread(NULL, 0, process_file_genetico_hibrid, arg, 0, NULL);

		if (threads[i] == NULL) {
			printf("Error creating thread: %d\n", GetLastError());
			exit(1);
		}
	}

	wait_and_close_threads(num_files, threads);

	for (size_t i = 0; i < num_files; i++)
	{
		print_general_results_hibrid(thread_args[i]->file, thread_args[i]->best_ever, thread_args[i]->avg, thread_args[i]->runs, thread_args[i]->EA_param);
		free(thread_args[i]);
	}
}

