#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#define N 400

double a[N][N], b[N][N], c[N][N], d[N][N];

int main(void){

	for(int i=0; i<N; i++){
		for(int j=0; j<N; j++){
			a[i][j] = 4.0;
			b[i][j] = 3.0;
			c[i][j] = 8.0;
			d[i][j] = 0.0;
		}
	}

	for(int i=0; i<N; i++){
		for(int j=0; j<N; j++){
			double sum=0;
			for(int k=0; k<N; k++){
				sum += a[i][k] * b[k][j];
			}
			d[i][j] = sum;
		}
	}

	for(int i=0; i<N; i++){
		for(int j=0; j<N; j++){
			d[i][j] = d[i][j] + c[i][j];
		}
	}

	int correct = 1;
	for(int i=0; i<N; i++){
		for(int j=0; j<N; j++){
			if(d[i][j] != 4808){
				correct = 0;
				break;
			}
		}
	}

	if(correct){
		printf("OK\n");
	}
	else printf("ERROR\n");

	return 0;
}
