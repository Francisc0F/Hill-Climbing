#include <Windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "algoritmo.h"
#include "algoritmo_genetico.h"
#include "algoritmo_hibrid.h"
#include "utils.h"
#include "main.h"



void hill_climbing_results(char** files, int numfiles) {
	// iter change
	printf("\nIter variation 100, 200, 500, 1000 -----");
	int runs_iter[] = { DEFAULT_RUNS,200, 500, 1000 };
	for (int i = 0; i < 4; i++) {
		printf("\n%d", runs_iter[i]);
		lunch_threads(files, numfiles, runs_iter[i], 1, 1);
	}


	printf("\n-----------------------------------------");
	printf("\nAccept greater variation iter 500");
	lunch_threads(files, numfiles, 500, 1, 0);
	printf("\nAccept equal variation iter 500");
	lunch_threads(files, numfiles, 500, 1, 1);
	printf("\n-----------------------------------------");

	// variation each neighbour
	printf("\nNeighbour variation 1,2,3,4  -----");
	for (int i = 0; i < 4; i++) {
		printf("\nneighbour %d", i + 1);
		lunch_threads(files, numfiles, 500, i + 1, 1);
	}
	printf("\n-----------------------------------------");

}

void genetic_results(char** files, int numfiles) {
	printf("\nIter variation 100, 200, 500, 1000 -----");
	int runs_iter[] = { DEFAULT_RUNS,200, 500, 1000 };
	for (int i = 0; i < 4; i++) {
		info EA_param = { 0 };
		EA_param.pm = DEFAULT_PM_RATE;
		EA_param.pr = DEFAULT_PR_RATE;
		EA_param.popsize = DEFAULT_POP_SIZE;
		printf("\nIter variation %d, pm: %f, pr: %f, popsize: %d", runs_iter[i], EA_param.pm, EA_param.pr, EA_param.popsize);
		EA_param.tsize = DEFAULT_TOURNAMENT_SIZE;
		EA_param.maxGenerations = DEFAULT_MAX_GENERATIONS;
		lunch_threads_genetic(files, numfiles, runs_iter[i], EA_param, 1);
		printf("\n--------------------------");
	}


	printf("\nVariation pr 0.5  500");
	info paramsPr = { 0 };
	paramsPr.pm = DEFAULT_PM_RATE;
	paramsPr.pr = 0.5;
	paramsPr.popsize = DEFAULT_POP_SIZE;
	paramsPr.tsize = DEFAULT_TOURNAMENT_SIZE;
	paramsPr.maxGenerations = DEFAULT_MAX_GENERATIONS;
	lunch_threads_genetic(files, numfiles, 500, paramsPr, 1);
	printf("\n--------------------------");

	printf("\nVariation increase pm  500 -----");
	info paramsPm = { 0 };
	paramsPm.pm = 0.2;
	paramsPm.pr = DEFAULT_PR_RATE;
	paramsPm.popsize = DEFAULT_POP_SIZE;
	paramsPm.tsize = DEFAULT_TOURNAMENT_SIZE;
	paramsPm.maxGenerations = DEFAULT_MAX_GENERATIONS;
	lunch_threads_genetic(files, numfiles, 500, paramsPm, 1);
	printf("\n--------------------------");


	printf("\nVariation increase popsize 3x  500 -----");
	info paramsPop = { 0 };
	paramsPop.pm = DEFAULT_PM_RATE;
	paramsPop.pr = DEFAULT_PR_RATE;
	paramsPop.popsize = DEFAULT_POP_SIZE*3;
	paramsPop.tsize = DEFAULT_TOURNAMENT_SIZE;
	paramsPop.maxGenerations = DEFAULT_MAX_GENERATIONS;
	lunch_threads_genetic(files, numfiles, 500, paramsPop, 1);
	printf("\n--------------------------");


	printf("\nRecombination Change iter 500  -----");
	for (int i = 1; i < 4; i++) {
		printf("\nRecombination variation %d", i);
		info EA_param = { 0 };
		EA_param.pm = DEFAULT_PM_RATE;
		EA_param.pr = DEFAULT_PR_RATE;
		EA_param.popsize = DEFAULT_POP_SIZE;
		EA_param.tsize = DEFAULT_TOURNAMENT_SIZE;
		EA_param.maxGenerations = DEFAULT_MAX_GENERATIONS;
		lunch_threads_genetic(files, numfiles, 500, EA_param, i);
		printf("\n--------------------------");
	}

}

void genetic_hibrid_results(char** files, int numfiles) {
	
	printf("\nhibrid results -----");
	for (int i = 0; i < 4; i++) {
		info paramsPr = { 0 };
		paramsPr.pm = DEFAULT_PM_RATE;
		paramsPr.pr = DEFAULT_PR_RATE;
		paramsPr.popsize = DEFAULT_POP_SIZE;
		paramsPr.tsize = DEFAULT_TOURNAMENT_SIZE;
		paramsPr.maxGenerations = DEFAULT_MAX_GENERATIONS;
		printf("\nThreads variation %d", i);
		lunch_threads_hibrid(files, numfiles, 500, paramsPr, 1, i, 500);
		printf("\n--------------------------");
	}
}


void best_results(char** files, int numfiles) {

	printf("\nhibrid results -----");
	for (int i = 0; i < 4; i++) {
		info paramsPr = { 0 };
		paramsPr.pm = DEFAULT_PM_RATE + 0.05;
		paramsPr.pr = 0.8;
		paramsPr.popsize = DEFAULT_POP_SIZE * 10;
		paramsPr.tsize = DEFAULT_TOURNAMENT_SIZE;
		paramsPr.maxGenerations = DEFAULT_MAX_GENERATIONS * 2;
		printf("\nThreads variation %d", i);
		lunch_threads_hibrid(files, numfiles, 700, paramsPr, 1, i, 700);
		printf("\n--------------------------");
	}
}

/*
	[hibrid - config 3 ]
	[1] Best - PM:0.06 PR:0.8 POPSIZE: 100 MAX_GEN: 200, genetic_runs:700, inner_trepa: 700
			  Média    Custo
	 file1.txt,20.000000,20.000000
	 file2.txt,15.000000,15.000000
	 file3.txt,112.000000,112.000000
	 file4.txt,78.978569,79.000000
	 file5.txt,83.481430,98.000000
	 teste.txt,5.000000,5.000000

	[hibrid - config 3 ]
	[2] Best - PM:0.06 PR:0.7 POPSIZE: 50 MAX_GEN: 180, genetic_runs:500, inner_trepa: 500
			  Média    Custo
	teste.txt,5.000000,5.000000
	file1.txt,20.000000,20.000000
	file2.txt,15.000000,15.000000
	file3.txt,112.000000,112.000000
	file4.txt,76.954002,79.000000
	file5.txt,72.650002,91.000000

*/
int main(int argc, char* argv[])
{
	char nome_fich[100];
	int runs;
	char** files = (char**)calloc(MAX_FILES, sizeof(char*));
	int numfiles = 0;
	find_test_files(files, &numfiles);
	init_rand();


	//hill_climbing_results(files, numfiles);

	//printf("\n----------------------------------------------------------------------------");
	//genetic_results(files, numfiles);
	
	//todo  create hibrid algorithm
	//genetic_hibrid_results(files, numfiles);

	// best
	best_results(files, numfiles);
	return 0;
}
