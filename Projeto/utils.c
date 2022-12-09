#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "utils.h"

// Leitura do ficheiro de input
// Recebe: nome do ficheiro, numero de vertices (ptr), numero de iteracoes (ptr)
// Devolve a matriz de adjacencias
int* init_dados(char* nome, int* vert, int* iter, int* nConjunto)
{
	FILE* f = NULL;
	int* p = NULL, * q = NULL;
	int i = 0, j;

	errno_t err = fopen_s(&f, nome, "r");
	if (err != 0)
	{
		printf("Erro no acesso ao ficheiro dos dados\n");
		exit(1);
	}

	int edges = 0;

	fscanf_s(f, "k %d\n", nConjunto);
	fscanf_s(f, "p edge %d %d\n", vert, &edges);

	p = malloc(sizeof(int) * (*vert) * (*vert));
	if (!p)
	{
		printf("Erro na alocacao de memoria\n");
		exit(1);
	}

	// Preenchimento da matriz
	for (i = 0; i < *vert; i++) {
		for (j = 0; j < *vert; j++) {
			p[i * (*vert) + j] = 0;
		}
	}
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


	for (i = 0; i < *vert; i++) {
		for (j = 0; j < *vert; j++) {
			printf("%d ", p[i * (*vert) + j]);
		}
		printf("\n");
	}


	fclose(f);
	return p;
}

// Gera a solucao inicial
// Parametros: solucao, numero de vertices
void gera_sol_inicial(int* sol, int v, int nConjunto)
{
	int i, x;

	for (i = 0; i < v; i++)
		sol[i] = 0;


	for (i = 0; i < nConjunto; i++)
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

	printf("\nConjunto A: ");
	for (i = 0; i < vert; i++)
		if (sol[i] == 0)
			printf("%2d  ", i + 1);
	printf("\nConjunto B: ");
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
