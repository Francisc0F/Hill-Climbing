#include <Windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "algoritmo.h"
#include "algoritmo_genetico.h"
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

/*
	[Trepa colinas]
	[1] Best - neigbour 1 [GUINESS]

	#define DEFAULT_RUNS 100.000
	#define DEFAULT_TREPA_ITER 500
	teste.txt -> 5 = MBF
	file1.txt -> 20, 19.999 MBF
	file2.text -> 15
	file3.txt -> 112
	file4.txt -> 79
	file5.txt ->

	[Trepa colinas]
	[2] Best - neigbour 1
	#define DEFAULT_RUNS 10000
	#define DEFAULT_TREPA_ITER 100

	teste.txt -> 5 = MBF
	file1.txt -> 20, 19.759501 MBF
	file2.text -> 15, 14.557300 MBF
	file3.txt -> 112, 105.328499 MBF
	file4.txt -> 62, 30.740400 MBF
	file5.txt -> 48, 20.850700 MBF

	[Trepa colinas]
	3 Best - neigbour 4
	#define DEFAULT_RUNS 10000
	#define DEFAULT_TREPA_ITER 100

	teste.txt -> 5 , 4.923800 MBF
	file1.txt -> 20, 19.236300 MBF
	file2.text -> 15, 12.623200 MBF
	file3.txt -> 110, 101.429497 MBF
	file4.txt -> 42, 19.503401 MBF
	file5.txt -> 32, 13.949200 MBF

	[Trepa colinas]
	4 Best - neigbour 3
	#define DEFAULT_RUNS 10000
	#define DEFAULT_TREPA_ITER 100

	teste.txt -> 5,  4.155600 = MBF
	file1.txt -> 20, 19.115999 MBF
	file2.text -> 15, 12.213600 MBF
	file3.txt -> 112, 102.703300 MBF
	file4.txt -> 48, 18.335400 MBF
	file5.txt -> 34, 11.516500 MBF

	[Trepa colinas]
	5 Best - neigbour 2
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
	char nome_fich[100];
	int runs;
	char** files = (char**)calloc(MAX_FILES, sizeof(char*));
	int numfiles = 0;
	find_test_files(files, &numfiles);
	init_rand();


	//hill_climbing_results(files, numfiles);

	printf("\n----------------------------------------------------------------------------");
	//genetic_results(files, numfiles);
	
	//todo  create hibrid algorithm
	//lunch_threads_hibrid(files, numfiles, DEFAULT_RUNS, EA_param);


	return 0;
}
