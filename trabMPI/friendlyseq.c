#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>
#include <mpi.h>



int procs, meurank;
MPI_Status st;

/* Recursiva
int gcd(int u, int v) {
	if (v == 0)
		return u;
	return gcd(v, u % v);
}
*/

/* Standard C Function: Greatest Common Divisor */
int gcd ( int a, int b ){
  	int c;
	while ( a != 0 ) {
     	c = a; 
		a = b % a;  
		b = c;
	}
  	return b;
}

void friendly_numbers(long int start, long int end) {
	double inicio;	
	int c=0, aux;
	long int last = end - start + 1;

	long int *the_num;
	the_num = (long int*) malloc(sizeof(long int) * last);
	long int *num;
	num = (long int*) malloc(sizeof(long int) * last);
	long int *den;
	den = (long int*) malloc(sizeof(long int) * last);

	long int i, j, factor, ii, sum, done, n;

	int parte = last / procs; // 662143
	if(meurank == 0){
		for (i = start; i <= end; i++) {
			ii = i - start;
			sum = 1 + i;
			the_num[ii] = i;
			done = i;
			factor = 2;
			while (factor < done) {
				if ((i % factor) == 0) {
					sum += (factor + (i / factor));
					if ((done = i / factor) == factor)
						sum -= factor;
				}
				factor++;
			}
			num[ii] = sum;
			den[ii] = i;
			n = gcd(num[ii], den[ii]);
			num[ii] /= n;
			den[ii] /= n;
			printf("%ld\n", i);
		} // end for
		/*for(i = 1; i < procs; i++){
			MPI_Send(num, parte, MPI_INT, i, 10, MPI_COMM_WORLD);
			MPI_Send(den, parte, MPI_INT, i, 11, MPI_COMM_WORLD);
		}	
	}else{
		MPI_Recv(&num, parte, MPI_INT, 0, 10, MPI_COMM_WORLD, &st);
		MPI_Recv(&den, parte, MPI_INT, 0, 11, MPI_COMM_WORLD, &st);*/
		for (i = 0; i < parte; i++) {
			for (j = i + 1; j < parte; j++) {
				printf("i = %ld j = %ld\n", i, j);
				if ((num[i] == num[j]) && (den[i] == den[j]))
					c++;
			}
		}
		printf("%d\n", c);
	//}
		

	free(the_num);
	free(num);
	free(den);
}

int main(int argc, char **argv) {
	MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &procs);
    MPI_Comm_rank(MPI_COMM_WORLD, &meurank);

	long int start;
	long int end; 

	scanf("%ld %ld", &start, &end);
	printf("Number %ld to %ld\n", start, end);
	friendly_numbers(start, end);

	return EXIT_SUCCESS;
}
