#include <Windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "algoritmo.h"
#include "algoritmo_genetico.h"
#include "utils.h"
#include "main.h"





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
	find_test_files(files,&numfiles);
	init_rand();

	// iter change
	printf("\nIter variation 100, 200, 500, 1000 -----");
	int runs_iter[] = { DEFAULT_RUNS,200, 500, 1000 };
	for (int i = 0; i < 4; i++) {
		printf("\n runs_iter[i] %d-----", runs_iter[i]);
		lunch_threads(files, numfiles, runs_iter[i], 1, 0);
	}
	printf("\n-----------------------------------------");
	printf("\nAccept greater variation iter 500 -----");
	lunch_threads(files, numfiles, 500, 0, 1);
	printf("\nAccept equal variation iter 500 -----");
	lunch_threads(files, numfiles, 500, 1, 1);
	printf("\n-----------------------------------------");
	
	// variation each neighbour
	printf("\nNeighbour variation 1,2,3,4  -----");
	for (int i = 0; i < 4; i++) {
		printf("\nneighbour %d-----", i + 1);
		lunch_threads(files, numfiles, 500, i + 1, 0);
	}
	printf("\n-----------------------------------------");
	

	
	/*
	printf("\n----------------------------------------------------------------------------");
	info EA_param = { 0 };
	EA_param.pm = DEFAULT_PM_RATE;
	EA_param.pr = DEFAULT_PR_RATE;
	EA_param.popsize = DEFAULT_POP_SIZE;
	EA_param.tsize = DEFAULT_TOURNAMENT_SIZE;
	EA_param.maxGenerations = DEFAULT_MAX_GENERATIONS;
	lunch_threads_genetic(files, numfiles, DEFAULT_RUNS, EA_param);*/

	//todo  create hibrid algorithm
	//lunch_threads_hibrid(files, numfiles, DEFAULT_RUNS, EA_param);
	
	
	return 0;
}



// test reverse from random p1 to p2
//void print_v(int* vector, int n) {
//	for (int i = 0; i < n; i++) {
//		printf("%d ", vector[i]);
//	}
//	printf("\n");
//}
//


//int main(int argc, char* argv[])
//{
//	int vector[6];
//	int n = 6;
//	for (int i = 0; i < n; i++) {
//		if (i < 3) {
//			vector[i] = 0;
//		}
//		else {
//			vector[i] = 1;
//		}
//	}
//
//	print_v(vector, n);
//
//	for (int i = 0; i < 10; i++) {
//
//		int novo[6];
//		gera_vizinho4(vector, novo, n);
//		printf("Novo: \n");
//		print_v(novo, n);
//	}
//
//}

