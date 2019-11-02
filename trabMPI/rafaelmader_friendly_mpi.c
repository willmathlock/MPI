/*
	
*/
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <mpi.h>

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
	int c=0, meurank, procs, total,index, alocar;
	long int pedaco, last, i, j, factor, ii, sum, done, n, escravos, range_end = 0, range_init = 0, aux, end_aux, start, end;;
	long int *num, *den, *rec_num, *rec_den, *indices;
	double timer_init, timer_end;
	MPI_Status st;
    
	MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &procs);
    MPI_Comm_rank(MPI_COMM_WORLD, &meurank);
	
	if(argc != 3) {
		if(meurank == 0)
			printf("Erro na quantidade de parâmetros [Ex: nome_executavel 1 662143]\n");
		MPI_Finalize();
		return;
	} else {
		start = atoi(argv[1]);
		end = atoi(argv[2]);
	}

	if(start > end) {
		if(meurank == 0)
		printf("Valor inicial deve ser menor que final\n");
		
		MPI_Finalize();
		return;
	} else if(start < 1) {
		if(meurank == 0)
		printf("Valor inicial deve ser maior que ZERO\n");
		
		MPI_Finalize();
		return;

	}else if(procs !=2 && procs !=4 && procs !=8 && procs !=16 && procs !=32 && procs !=64) {
		if(meurank == 0)
			printf("Erro na quantidade de processos\n");
	
		MPI_Finalize();
		return;
	}

	escravos = procs - 1;
	alocar = end - start + 1;
	pedaco = last = end / escravos;
	total = aux = index = 0;
	end_aux = end;
	
	num = (long int*) malloc(sizeof(long int) * alocar);
	den = (long int*) malloc(sizeof(long int) * alocar);
	indices = (long int*) malloc(sizeof(long int) * escravos + 1);

	indices[0] = start;	//Usado para percorrer o vetor no processo de balanceamento
	
	for(i = 1; i <= escravos; i++){
		aux = end_aux / ((procs + 1 ) - i);
		if(i == (escravos))
			indices[i] = end_aux;
		else 
			indices[i] = (int) end_aux / ((procs + 1 ) - i) + aux;
		end_aux -= indices[i];
	}
	
	if (meurank == 0){

		for(i=1; i < procs; i++) {			
			range_init += indices[i-1];
			range_end += indices[i];
			MPI_Send(&range_init, 1, MPI_INT, i, range_end, MPI_COMM_WORLD);
		}

		index = 0;
		for(i=1; i < procs; i++) {
			MPI_Recv(&num[index], indices[i], MPI_LONG, i, MPI_ANY_TAG, MPI_COMM_WORLD, &st);
			MPI_Recv(&den[index], indices[i], MPI_LONG, i, MPI_ANY_TAG, MPI_COMM_WORLD, &st);		
			index += indices[i];
		}

	} else {
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
	}

	if(meurank == 0) {	
		index = 0;
	
		for (i = 1; i < procs; i ++) {
			if (i == (procs - 1) && alocar % pedaco > 0)
				last += alocar % pedaco;
			
			MPI_Send(num, alocar, MPI_LONG, i, i, MPI_COMM_WORLD);
			MPI_Send(den, alocar, MPI_LONG, i, i, MPI_COMM_WORLD);
			MPI_Send(&index, 1, MPI_INT, i, last, MPI_COMM_WORLD);
			index += last;
		}
	} else {
		
		MPI_Recv(num, alocar, MPI_LONG, 0, MPI_ANY_TAG, MPI_COMM_WORLD, &st);
		MPI_Recv(den, alocar, MPI_LONG, 0, MPI_ANY_TAG, MPI_COMM_WORLD, &st);
		MPI_Recv(&index, 1, MPI_INT, 0, MPI_ANY_TAG, MPI_COMM_WORLD, &st);	

		for (i = index; i < (st.MPI_TAG+index); i++) {
			for (j = i + 1; j < alocar; j++) {
				if ((num[i] == num[j]) && (den[i] == den[j]))
					c++;
			}
		}
	}

	MPI_Reduce(&c, &total, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
	
	if (meurank == 0 )
		printf("Founded number friendly %i\n", total);	

	free(num);
	free(den);
	
	MPI_Finalize();
}

int main(int argc, char **argv) {

	friendly_numbers(argc, argv);
	return EXIT_SUCCESS;
}
