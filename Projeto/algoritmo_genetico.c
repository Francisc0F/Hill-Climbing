#include <stdio.h>
#include <stdlib.h>

#include "funcao.h"
#include "algoritmo_genetico.h"
#include "utils.h"

/*
	Creates initial population
	Params: struct with info about the genetic problem
	Returns pointer to the vector with initial population
*/
pchrom init_pop(info d)
{
	int     i, j;
	pchrom  indiv;

	indiv = malloc(sizeof(chrom) * d.popsize);
	if (indiv == NULL)
	{
		printf("Erro na alocacao de memoria\n");
		exit(1);
	}

	// double check this - should we ignore this initializor as it creates invalid solutions?
	for (i = 0; i < d.popsize; i++)
	{
		for (j = 0; j < d.numGenes; j++)
			indiv[i].sol[j] = flip();
	}
	return indiv;
}



float eval_individual(int sol[], info d, int* mat, int* valid)
{
	// subset
	int number_of_group_1_elements = 0;
	for (int i = 0; i < d.numGenes; i++)
	{
		if (sol[i] == 1)
		{
			number_of_group_1_elements++;
		}
	}

	if (number_of_group_1_elements != d.k) {
		*valid = 0;
		return 0;
	}

	int fit = calcula_fit(sol, mat, d.numGenes);
	*valid = 1;
	return fit;
}

// Avaliacao da popula��o
// Par�metros de entrada: populacao (pop), estrutura com parametros (d) e matriz com dados do problema (mat)
// Par�metros de sa�da: Preenche pop com os valores de fitness e de validade para cada solu��o
void evaluate(pchrom pop, info d, int* mat)
{
	int i;

	for (i = 0; i < d.popsize; i++)
		pop[i].fitness = eval_individual(pop[i].sol, d, mat, &pop[i].valido);
}


// Preenche uma estrutura com os progenitores da pr�xima gera��o, de acordo com o resultados do torneio binario (tamanho de torneio: 2)
// Par�metros de entrada: popula��o actual (pop), estrutura com par�metros (d) e popula��o de pais a encher
void tournament(pchrom pop, info d, pchrom parents)
{
	int i, x1, x2;

	// Makes a tournament between two individuals and the winner is added to the parents population
	for (i = 0; i < d.popsize; i++)
	{
		x1 = random_l_h(0, d.popsize - 1);
		do
			x2 = random_l_h(0, d.popsize - 1);
		while (x1 == x2);
		// maximization problem we wan the biggest cost, 
		// meaning the biggest number of edges between the group 1 elements
		if (pop[x1].fitness > pop[x2].fitness)		
			parents[i] = pop[x1]; 
		else
			parents[i] = pop[x2];
	}
}

// Operadores geneticos a usar na gera��o dos filhos
// Par�metros de entrada: estrutura com os pais (parents), estrutura com par�metros (d), estrutura que guardar� os descendentes (offspring)
void genetic_operators(pchrom parents, info d, pchrom offspring)
{
	// Crossover at one point
	crossover(parents, d, offspring);

	// Mutation of single bit
	mutation(offspring, d);
}

// Preenche o vector descendentes com o resultado das opera��es de recombina��o
// Par�metros de entrada: estrutura com os pais (parents), estrutura com par�metros (d), estrutura que guardar� os descendentes (offspring)
void crossover(pchrom parents, info d, pchrom offspring)
{
	int i, j, point;

	for (i = 0; i < d.popsize; i += 2)
	{
		if (rand_01() < d.pr)
		{
			point = random_l_h(0, d.numGenes - 1);
			for (j = 0; j < point; j++)
			{
				offspring[i].sol[j] = parents[i].sol[j];
				offspring[i + 1].sol[j] = parents[i + 1].sol[j];
			}
			for (j = point; j < d.numGenes; j++)
			{
				offspring[i].sol[j] = parents[i + 1].sol[j];
				offspring[i + 1].sol[j] = parents[i].sol[j];
			}
		}
		else
		{
			offspring[i] = parents[i];
			offspring[i + 1] = parents[i + 1];
		}
	}
}

// Muta��o bin�ria com v�rios pontos de muta��o
// Par�metros de entrada: estrutura com os descendentes (offspring) e estrutura com par�metros (d)
void mutation(pchrom offspring, info d)
{
	for (int i = 0; i < d.popsize; i++)
		for (int j = 0; j < d.numGenes; j++)
			if (rand_01() < d.pm)
				offspring[i].sol[j] = !(offspring[i].sol[j]);
}


chrom get_best(pchrom pop, info d, chrom best)
{
	int i;

	for (i = 0; i < d.popsize; i++)
	{
		if (best.fitness < pop[i].fitness)
			best = pop[i];
	}
	return best;
}

void run_for_file_genetico(const char* nome_fich, int runs) {

	pchrom pop = NULL, parents = NULL;
	chrom best_run, best_ever;
	// Preenche a matriz com dados dos objectos (peso e valor) e a estrutura EA_param que foram definidos no ficheiro de input
	info EA_param = { 0 };
	int vert, nConjunto;
	int* adjacent_matrix = init_dados(nome_fich, &vert, &nConjunto);
	float       mbf = 0.0;
	// todo rest of genetic

	int r, gen_actual;
	// Do genetic runs times
	for (r = 0; r < runs; r++)
	{
		printf("Genetic Repetition %d\n", r + 1);
		// Generate initial population
		pop = init_pop(EA_param);
		// Evaluate initial population
		evaluate(pop, EA_param, adjacent_matrix);
		gen_actual = 1;
		// Como ainda n�o existe, escolhe-se como melhor solu��o a primeira da popula��o (poderia ser outra qualquer)
		best_run = pop[0];
		// Encontra-se a melhor solu��o dentro de toda a popula��o
		best_run = get_best(pop, EA_param, best_run);
		// Reserva espa�o para os pais da popula��o seguinte
		parents = malloc(sizeof(chrom) * EA_param.popsize);
		// Caso n�o consiga fazer a aloca��o, envia aviso e termina o programa
		if (parents == NULL)
		{
			printf("Erro na alocacao de memoria\n");
			exit(1);
		}
		// Ciclo de optimiza��o
		while (gen_actual <= EA_param.numGenerations)
		{	
			printf("Gen: %d\n", gen_actual);
			// Torneio bin�rio para encontrar os progenitores (ficam armazenados no vector parents)
			tournament(pop, EA_param, parents);
			// Aplica os operadores gen�ticos aos pais (os descendentes ficam armazenados na estrutura pop)
			genetic_operators(parents, EA_param, pop);
			// Avalia a nova popula��o (a dos filhos)
			evaluate(pop, EA_param, adjacent_matrix);
			// Actualiza a melhor solu��o encontrada
			best_run = get_best(pop, EA_param, best_run);
			gen_actual++;
		}
		// Contagem das solu��es inv�lidas
		int inv, i;
		for (inv = 0, i = 0; i < EA_param.popsize; i++)
			if (pop[i].valido == 0)
				inv++;
		// Escreve resultados da repeti��o que terminou
		printf("\nRepeticao %d:", r);
		write_best(best_run, EA_param);
		printf("\nPercentagem Invalidos: %f\n", 100 * (float)inv / EA_param.popsize);
		mbf += best_run.fitness;
		if (r == 0 || best_run.fitness > best_ever.fitness)
			best_ever = best_run;
		// Liberta a mem�ria usada
		free(parents);
		free(pop);
	}
	// Escreve resultados globais
	printf("\n\nMBF: %f\n", mbf / r);
	printf("\nMelhor solucao encontrada");
	write_best(best_ever, EA_param);
}


// Escreve uma solu��o na consola
// Par�metro de entrada: populacao actual (pop) e estrutura com par�metros (d)
void write_best(chrom x, info d)
{
	int i;

	printf("\nBest individual: %4.1f\n", x.fitness);
	for (i = 0; i < d.numGenes; i++)
		printf("%d", x.sol[i]);
	putchar('\n');
}
