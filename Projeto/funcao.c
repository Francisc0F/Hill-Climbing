#include "funcao.h"
#include <assert.h>
/*
 Calcula fitnesss = total
 Recebe:  A solução, sol, a matriz de adjacências, mat, e o número de vértices, vert
 Devolve: O custo que é o número de ligações que existem na solução
 
*/
int calcula_fit(int sol[], int* mat, int vert)
{
	int fitness = 0;
	int i, j;

	for (i = 0; i < vert; i++)
		if (sol[i] == 1)
		{
			for (j = i + 1; j < vert; j++)
				if (sol[j] == 1 && *(mat + i * vert + j) == 1)
					fitness++;
		}
	return fitness;
}


void test_calcula_fit() {
	int mat[3][3] = {
	  {0, 1, 1},
	  {1, 0, 0},
	  {1, 0, 0}
	};

	int sol1[3] = { 1, 0, 1 };
	int fitness = calcula_fit(sol1, (int*)mat, 3);
	assert(fitness == 1);

	int sol2[3] = { 0, 1, 0 };
	fitness = calcula_fit(sol2, (int*)mat, 3);
	assert(fitness == 0);

	int sol3[3] = { 1, 1, 0 };
	fitness = calcula_fit(sol3, (int*)mat, 3);
	assert(fitness == 1);

}

void test_calcula_fit2() {
	int mat[6][6] = {
	  {0, 1, 1, 1, 0, 0},
	  {0, 0, 0, 1, 1, 0},
	  {0, 0, 0, 1, 0, 0},
	  {0, 0, 0, 0, 0, 1},
	  {0, 0, 0, 0, 0, 0},
	  {0, 0, 0, 0, 0, 0},
	};

	int sol1[6] = { 1, 0, 1, 1, 0, 1 };
	int fitness = calcula_fit(sol1, (int*)mat, 6);
	assert(fitness == 4);
	fitness = -1;

	int sol2[6] = { 0, 0, 1, 1, 1, 1 };
	fitness = calcula_fit(sol2, (int*)mat, 6);
	assert(fitness == 2);


	fitness = -1;
	int sol3[6] = { 1, 1, 0, 1, 1, 0 };
	fitness = calcula_fit(sol3, (int*)mat, 6);
	assert(fitness == 4);
}



void test_calcula_fit3() {
	int mat[6][6] = {
	  {0, 1, 1, 1, 0, 0},
	  {1, 0, 0, 1, 1, 0},
	  {1, 0, 0, 1, 0, 0},
	  {1, 1, 1, 0, 0, 1},
	  {0, 1, 0, 0, 0, 0},
	  {0, 0, 0, 1, 0, 0},
	};

	int sol1[6] = { 1, 0, 1, 1, 0, 1 };
	int fitness = calcula_fit(sol1, (int*)mat, 6);
	assert(fitness == 4);
	fitness = -1;

	int sol2[6] = { 0, 0, 1, 1, 1, 1 };
	fitness = calcula_fit(sol2, (int*)mat, 6);
	assert(fitness == 2);
}

void calcula_fit_tests() {
	test_calcula_fit();
	test_calcula_fit2();
	test_calcula_fit3();
	printf(" All tests pass!\n");
}
