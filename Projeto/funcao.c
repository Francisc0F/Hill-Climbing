#include "funcao.h"

/*
 Calcula fitnesss = total
 Recebe:  A solução, sol, a matriz de adjacências, mat, e o número de vértices, vert
 Devolve: O custo que é o número de ligações que existem na solução

 Restriction it only works with a upper triangular matrix, with main diagonal = 0
*/
int calcula_fit(int sol[], int *mat, int vert)
{
	int fitness =0;
	int i, j;

	for(i=0; i<vert; i++)
		if(sol[i]==1)
		{
			for(j=0; j<vert;j++)
				if(sol[j]==1 && *(mat+i*vert+j)==1)
					fitness++;
		}
	return fitness;
}
