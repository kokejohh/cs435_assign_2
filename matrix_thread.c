#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include <unistd.h>

#define N 800
#define SIZE 8

pthread_t t[SIZE];

typedef struct matrix {
	double **table;
	int row;
	int col;	
} matrix;

typedef struct argument {
	matrix x;
	matrix y;
	matrix ans;
	int thread;
} argument;

matrix matrix_create(double val, int row, int col);
void print_matrix(matrix m);

void *multiply(void *arg)
{
	argument *args = (argument *)arg;
	int start = args->thread;
	int n = N / SIZE;
	for (int i = start * n; i < (start + 1) * n; i++)
	{
		for (int j = 0; j < args->y.col; j++)
		{
			double sum = 0;
			for (int k = 0; k < args->x.col; k++)
				sum += args->x.table[i][k] * args->y.table[k][j];	
			args->ans.table[i][j] = sum;
		}
	}
}

void *add(void *arg)
{
	argument *args = (argument *)arg;
	int start = args->thread;
	int n = N / SIZE;
	for (int i = start * n; i < (start + 1) * n; i++)
		for (int j = 0; j < N; j++)
			args->ans.table[i][j] = args->x.table[i][j] + args->y.table[i][j];
}

int main()
{
	clock_t tt;
	tt = clock();
	srand((unsigned)time(NULL));

	int row = N, col = N, len = N;
	
	matrix a = matrix_create(4, row, col);
	matrix b = matrix_create(3, row, col);
	matrix c = matrix_create(8, row, col);
	matrix d = matrix_create(0, row, col);
	
	argument t_arg[SIZE];
	for (int i = 0; i < SIZE; i++)
	{
		t_arg[i].x = a;
		t_arg[i].y = b;
		t_arg[i].ans = d;
		t_arg[i].thread = i;
		pthread_create(&t[i], NULL, multiply, &t_arg[i]);
	}

	for (int i = 0; i < SIZE; i++)
		pthread_join(t[i], NULL);
/*
	for (int i = 0; i < SIZE; i++)
	{
		t_arg[i].x = d;
		t_arg[i].y = c;
		t_arg[i].ans = d;
		pthread_create(&t[i], NULL, add, &t_arg[i]);
	}
	for (int i = 0; i < SIZE; i++)
		pthread_join(t[i], NULL);
*/
	tt = clock() - tt;
	double time_taken = ((double)tt) / CLOCKS_PER_SEC;	
//	print_matrix(d);
	printf("\n%lf\n", time_taken);

	return (0);
}

matrix matrix_create(double val, int row, int col)
{
	double **arr = (double **)malloc(row * sizeof(double *));
	for (int i = 0; i < N; i++)
	{
		arr[i] = (double *)malloc(col * sizeof(double));
		for (int j = 0; j < col; j++)
			arr[i][j] = val;
	}
	matrix m = {arr, row, col};
	return m;
}

void print_matrix(matrix m)
{
	for (int i = 0; i < m.row; i++)
	{
		for (int j = 0; j < m.col; j++)
			printf("%lf ", m.table[i][j]);
		printf("\n");
	}
}
