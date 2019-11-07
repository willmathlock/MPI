#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <mpi.h>
#include <time.h>

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

void friendly_numbers(int argc, char **argv) {
	//double startT, endT, spentT, totalTime = 0.00, auxTime = 0.00;
	int c=0, meurank, procs, total,index, size;
	long int slice, last, i, j, factor, ii, sum, done, n, slave, range_end = 0, range_init = 0, aux, end_aux, start, end;
	long int *num, *den, *rec_num, *rec_den, *indexes;

	MPI_Status st;
    
	MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &procs);
    MPI_Comm_rank(MPI_COMM_WORLD, &meurank);
	
	if(argc != 3) {
		if(meurank == 0)
			printf("Wrong number of parameters. Do something like:\ntime mpirun -np 8 -npernode 4 -hostfile 'hostname' 'executavel' 1 662143\n");
		MPI_Finalize();
		return;
	} else {
		start = atoi(argv[1]);
		end = atoi(argv[2]);
	}
	if(procs !=2 && procs !=4 && procs !=8 && procs !=16 && procs !=32 && procs !=64) {
		if(meurank == 0)
			printf("Wrong number of processors. Must be 2, 4, 8, 16, 32 or 64 Like\n");
	
		MPI_Finalize();
		return;
	}

	slave = procs - 1;
	size = end - start + 1;
	slice = last = end / slave;
	total = aux = index = 0;
	end_aux = end;
	
	num = (long int*) malloc(sizeof(long int) * size);
	den = (long int*) malloc(sizeof(long int) * size);
	indexes = (long int*) malloc(sizeof(long int) * slave + 1);

	indexes[0] = start;

	for(i = 1; i <= slave; i++){
		aux = end_aux / ((procs + 1 ) - i);
		if(i == (slave))
			indexes[i] = end_aux;
		else 
			indexes[i] = (int) end_aux / ((procs + 1 ) - i) + aux;
		end_aux -= indexes[i];
	}
	
	if (meurank == 0){
		//startT = MPI_Wtime();
		for(i=1; i < procs; i++) {			
			range_init += indexes[i-1];
			range_end += indexes[i];
			MPI_Send(&range_init, 1, MPI_INT, i, range_end, MPI_COMM_WORLD);
		}

		index = 0;
		for(i=1; i < procs; i++) {
			MPI_Recv(&num[index], indexes[i], MPI_LONG, i, MPI_ANY_TAG, MPI_COMM_WORLD, &st);
			MPI_Recv(&den[index], indexes[i], MPI_LONG, i, MPI_ANY_TAG, MPI_COMM_WORLD, &st);		
			index += indexes[i];
		}
		//endT = MPI_Wtime();
		//spentT = endT - startT;
	} else {
		//startT = MPI_Wtime();
		MPI_Recv(&start, 1, MPI_INT, 0, MPI_ANY_TAG, MPI_COMM_WORLD, &st);
		int tamanho = st.MPI_TAG - start + 1;
		rec_num = (long int*) malloc(sizeof(long int) * tamanho);
		rec_den = (long int*) malloc(sizeof(long int) * tamanho);

		for (i = start; i <= st.MPI_TAG; i++) {
			ii = i - start;
			sum = 1 + i;
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
			rec_num[ii] = sum;
			rec_den[ii] = i;
			n = gcd(rec_num[ii], rec_den[ii]);
			rec_num[ii] /= n;
			rec_den[ii] /= n;
		} // end for	
		MPI_Send(rec_num, tamanho, MPI_LONG, 0, 1, MPI_COMM_WORLD);
		MPI_Send(rec_den, tamanho, MPI_LONG, 0, 1, MPI_COMM_WORLD);

		free(rec_num);
		free(rec_den);
		//endT = MPI_Wtime();
		//spentT = endT - startT;
	}
	//somando tempo até agora
	//MPI_Reduce(&spentT, &totalTime, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);


	// comparando os números e somando
	if(meurank == 0) {
		//startT = MPI_Wtime();	
		index = 0;
	
		for (i = 1; i < procs; i ++) {
			if (i == (procs - 1) && size % slice > 0)
				last += size % slice;
			
			MPI_Send(num, size, MPI_LONG, i, i, MPI_COMM_WORLD);
			MPI_Send(den, size, MPI_LONG, i, i, MPI_COMM_WORLD);
			MPI_Send(&index, 1, MPI_INT, i, last, MPI_COMM_WORLD);
			index += last;
		}
	} else {
		//startT = MPI_Wtime();
		
		MPI_Recv(num, size, MPI_LONG, 0, MPI_ANY_TAG, MPI_COMM_WORLD, &st);
		MPI_Recv(den, size, MPI_LONG, 0, MPI_ANY_TAG, MPI_COMM_WORLD, &st);
		MPI_Recv(&index, 1, MPI_INT, 0, MPI_ANY_TAG, MPI_COMM_WORLD, &st);	

		for (i = index; i < (st.MPI_TAG+index); i++) {
			for (j = i + 1; j < size; j++) {
				if ((num[i] == num[j]) && (den[i] == den[j]))
					c++;
			}
		}
		//endT = MPI_Wtime();
		//spentT = endT - startT;
	}

	//somando tempo até agora
	//MPI_Reduce(&spentT, &auxTime, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);


	MPI_Reduce(&c, &total, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
	if (meurank == 0 ){
		printf("Found %i friendly numbers.\n", total);	
	}

	free(num);
	free(den);
	
	MPI_Finalize();
}

int main(int argc, char **argv) {

	friendly_numbers(argc, argv);
	return EXIT_SUCCESS;
}
