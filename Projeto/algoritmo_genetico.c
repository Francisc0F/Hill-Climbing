#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>
#include "funcao.h"
#include "algoritmo_genetico.h"
#include "utils.h"



void copy_chrom(pchrom a, pchrom b, int numGenes) {
	a->fitness = b->fitness;
	a->valido = b->valido;
	a->sol = allocate_matrix(1, numGenes);
	memcpy(a->sol, b->sol, numGenes * sizeof(int));
}

void copy_chrom_no_aloc(pchrom a, pchrom b, int numGenes) {
	a->fitness = b->fitness;
	a->valido = b->valido;
	memcpy(a->sol, b->sol, numGenes * sizeof(int));
}

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
		indiv[i].sol = allocate_matrix(1, d.numGenes);

		gera_sol_inicial(indiv[i].sol, d.numGenes, d.k);

		// this was allowing to start with invalid values, and was not allowing performance increase over time
		/*for (j = 0; j < d.numGenes; j++)
			indiv[i].sol[j] = flip();*/
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
			copy_chrom_no_aloc(&parents[i], &parents[x1], d.numGenes);
		else
			copy_chrom_no_aloc(&parents[i], &parents[x2], d.numGenes);
		
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

void crossover_2_points(pchrom parents, info d, pchrom offspring)
{
	int i, j, point1, point2;

	for (i = 0; i < d.popsize; i += 2)
	{
		if (rand_01() < d.pr)
		{
			point1 = random_l_h(0, d.numGenes - 1);
			point2 = random_l_h(point1 + 1, d.numGenes);
			for (j = 0; j < point1; j++)
			{
				offspring[i].sol[j] = parents[i].sol[j];
				offspring[i + 1].sol[j] = parents[i + 1].sol[j];
			}
			for (j = point1; j < point2; j++)
			{
				offspring[i].sol[j] = parents[i + 1].sol[j];
				offspring[i + 1].sol[j] = parents[i].sol[j];
			}
			for (j = point2; j < d.numGenes; j++)
			{
				offspring[i].sol[j] = parents[i].sol[j];
				offspring[i + 1].sol[j] = parents[i + 1].sol[j];
			}
		}
		else
		{
			copy_chrom_no_aloc(&offspring[i], &parents[i], d.numGenes);
			copy_chrom_no_aloc(&offspring[i + 1], &parents[i + 1], d.numGenes);
		}
	}
	
}


void crossover_uniform(pchrom parents, info d, pchrom offspring)
{
	int i, j;
	for (i = 0; i < d.popsize; i += 2)
	{
		if (rand_01() < d.pr)
		{
			for (j = 0; j < d.numGenes; j++)
			{
				if (flip() == 1)
				{
					offspring[i].sol[j] = parents[i].sol[j];
					offspring[i + 1].sol[j] = parents[i + 1].sol[j];
				}
				else
				{
					offspring[i].sol[j] = parents[i + 1].sol[j];
					offspring[i + 1].sol[j] = parents[i].sol[j];
				}
			}
		}
		else
		{
			copy_chrom_no_aloc(&offspring[i], &parents[i], d.numGenes);
			copy_chrom_no_aloc(&offspring[i + 1], &parents[i + 1], d.numGenes);
		}
	}
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
			int* p = parents[i].sol;
			int* p1 = parents[i + 1].sol;

			int* o = offspring[i].sol;
			int* o1 = offspring[i + 1].sol;

			for (j = 0; j < point; j++)
			{
				o[j] = p[j];
				o1[j] = p1[j];
			}


			for (j = point; j < d.numGenes; j++)
			{
				o[j] = p1[j];
				o1[j] = p[j];
			}
		}
		else
		{
			copy_chrom_no_aloc(&offspring[i], &parents[i], d.numGenes);
			copy_chrom_no_aloc(&offspring[i + 1], &parents[i + 1], d.numGenes);
		}
	}
}

// Single bit mutation
void mutation(pchrom offspring, info d)
{
	for (int i = 0; i < d.popsize; i++)
		for (int j = 0; j < d.numGenes; j++)
			if (rand_01() < d.pm)
				offspring[i].sol[j] = !(offspring[i].sol[j]);
}

chrom get_best(pchrom pop, info d, chrom best)
{
	for (int i = 0; i < d.popsize; i++)
	{
		if (best.fitness <= pop[i].fitness)
			copy_chrom_no_aloc(&best, &pop[i], d.numGenes);
	}
	return best;
}

void print_general_results_genetico(const char* nome_fich, chrom best_ever, float avg, int r, info EA_param)
{
	
	//printf("\nGenetico      - Ficheiro: %10s, Subset k: %3d, MBF: %10f , Custo: %3f", nome_fich, EA_param.k, avg, best_ever.fitness);
	printf("\nGenetico      - %10s, MBF: %10f , Custo: %3f, Pop: %2d, Pr: %2f, Pm: %2f", nome_fich, avg, best_ever.fitness, EA_param.popsize, EA_param.pr, EA_param.pm);
	//printf("\n best_ever valida = %d", best_ever.valido);
	//escreve_sol(best_ever.sol, EA_param.numGenes);
}
pchrom initialize_parents(info EA_param) {
	pchrom parents = calloc(EA_param.popsize, sizeof(chrom) * EA_param.popsize);

	if (parents == NULL)
	{
		printf("Erro na alocacao de memoria\n");
		exit(1);
	}

	for (int i = 0; i < EA_param.popsize; i++)
	{
		parents[i].sol = allocate_matrix(1, EA_param.numGenes);
		
		for (int j = 0; j < EA_param.numGenes; j++)
			parents[i].sol[j] = 0;
	}

	return parents;
}

void run_for_file_genetico(thread_arg_genetic* args) {
	
	info EA_param = args->EA_param;
	char* nome_fich = args->file;
	int runs  = args->runs;
	
	pchrom pop = NULL, parents = NULL;
	chrom best_ever = { 0 };
	int vert, nConjunto;

	int* adjacent_matrix = init_dados(nome_fich, &vert, &nConjunto);

	args->EA_param.numGenes = vert;
	args->EA_param.k = nConjunto;
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
		//printf("best_ever\n");
		//escreve_sol(best_ever.sol, EA_param.numGenes);

		// Evaluate initial population
		evaluate(pop, EA_param, adjacent_matrix);
		gen_actual = 1;

		//Choose a first best individual 
		//best_run = pop[0];
		copy_chrom(&best_run, &pop[0], EA_param.numGenes);
		best_run = get_best(pop, EA_param, best_run);
		//printf("best_run\n");
		//escreve_sol(best_run.sol, EA_param.numGenes);
		// Reserva espa�o para os pais da popula��o seguinte
		parents = initialize_parents(EA_param);

		// Ciclo de optimiza��o
		while (gen_actual <= EA_param.maxGenerations)
		{
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
		//printf("\nRepeticao %d:", r);
		//write_best(best_run, EA_param);
		//printf("\nPercentagem Invalidos: %f\n", 100 * (float)inv / EA_param.popsize);
		mbf += best_run.fitness;
		
		if (r == 0 || best_run.fitness >= best_ever.fitness) {
			copy_chrom(&best_ever, &best_run, EA_param.numGenes);
			free(best_run.sol);
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

void write_best(chrom x, info d)
{
	printf("\nBest individual: %4.1f\n", x.fitness);
	for (int i = 0; i < d.numGenes; i++)
		printf("%d", x.sol[i]);
	putchar('\n');
}

DWORD WINAPI process_file_genetico(LPVOID lpParameter) {
	thread_arg_genetic* thread_arg = (thread_arg_genetic*)lpParameter;
	//printf("Genetico Processing file: %s\n", thread_arg->file);
	run_for_file_genetico(thread_arg);
	//printf("DONE - Genetico Processing file : % s\n", thread_arg->file);
	return NULL;
}

void lunch_threads_genetic(char** files, int num_files, int runs, info EA_param) {
	// Create a separate thread for each file
	HANDLE threads[MAX_FILES] = { 0 };
	thread_arg_genetic* thread_args[MAX_FILES] = { 0 };
	for (int i = 0; i < num_files; i++) {
		thread_arg_genetic* arg = (thread_arg_genetic*)malloc(sizeof(thread_arg_genetic));
		if (arg == NULL) {
			printf("Error Alocating thread_arg_genetic*)malloc(sizeof(thread_arg_genetic: %d\n", GetLastError());
			exit(1);
		}
		
		arg->file = files[i];
		arg->runs = runs;
		arg->EA_param = EA_param;
		thread_args[i] = arg;
		threads[i] = CreateThread(NULL, 0, process_file_genetico, arg, 0, NULL);
		
		if (threads[i] == NULL) {
			printf("Error creating thread: %d\n", GetLastError());
			exit(1);
		}
	}
	
	wait_and_close_threads(num_files, threads);

	for (size_t i = 0; i < num_files; i++)
	{	
		print_general_results_genetico(thread_args[i]->file, thread_args[i]->best_ever, thread_args[i]->avg, thread_args[i]->runs, thread_args[i]->EA_param);
		free(thread_args[i]);
	}
	//printf("All genetic threads complete.\n");
}
