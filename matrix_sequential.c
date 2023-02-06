#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define N 800

typedef struct matrix {
	double **table;
	int row;
	int col;	
} matrix;

matrix matrix_create(double val, int row, int col);
void print_matrix(matrix m);
void multiply(matrix x, matrix y, matrix ans);
void add(matrix x, matrix y, matrix ans);

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

	multiply(a, b, d);
	add(d, c, d);
	
	tt = clock() - tt;
	double time_taken = ((double)tt) / CLOCKS_PER_SEC;
	printf("\n(%lf)\n", time_taken);

	//print_matrix(d);

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

void multiply(matrix x, matrix y, matrix ans)
{	
	if (x.col != y.row)
		return ;
	for (int i = 0; i < x.row; i++)
	{
		for (int j = 0; j < y.col; j++)
		{
			double sum = 0;
			for (int k = 0; k < x.col; k++)
				sum += x.table[i][k] * y.table[k][j];	
			ans.table[i][j] = sum;
		}
	}
}

void add(matrix x, matrix y, matrix ans)
{
	if (x.row != y.row || x.col != y.col)
		return ;
	for (int i = 0; i < x.row; i++)
		for (int j = 0; j < y.col; j++)
			ans.table[i][j] = x.table[i][j] + y.table[i][j];
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
