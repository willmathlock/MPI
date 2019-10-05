#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <mpi.h>

#define TAM 12
#define MAX 100


int ehprimo(int num);

int main(int argc, char **argv){
	int i, j, k, tag = 10;
    int vet[TAM];
    int meurank, procs, parte, ind=0, answ = 0, aux;
    MPI_Status status;

    srand(313);
    if (vet == NULL){
        printf("Nao foi\n");
        exit(1);
    }

    MPI_Init(&argc, &argv); 
    MPI_Comm_size(MPI_COMM_WORLD, &procs);
    MPI_Comm_rank(MPI_COMM_WORLD, &meurank);

    parte = (int)(TAM / (procs-1));
    if(meurank == 0){
        for(i=0; i < TAM; i++)
            vet[i] = rand() % MAX;
        for(i = 1; i < procs; i++){
            MPI_Send(&vet[ind], parte, MPI_INT, i, tag, MPI_COMM_WORLD);
            ind+=parte;
        }
        for(i = 1; i < procs; i++){
            MPI_Recv(aux, 1, MPI_INT, i, tag, MPI_COMM_WORLD, &status);
            answ += aux;
        }
        printf("%d\n", answ);

    }
    if(meurank != 0){
        MPI_Recv(vet, parte, MPI_INT, 0, tag, MPI_COMM_WORLD, &status);
        j = 0;
        for(i=0; i< parte; i++){
            if(ehprimo(vet[i])){
                j++;
            }
        }
        MPI_Send(&j, 1, MPI_INT, 0, tag, MPI_COMM_WORLD);
    }
        
    j=0;
    for(i=0; i < TAM; i++){
        if (ehprimo(vet[i])){
            j++;
            //printf("%d\n", vet[i]);
        }

    }
    MPI_Finalize();

    printf("Primos %d\n", j);
}


int ehprimo(int num){
    int i,c=0;    
    for(i=3; i < num/2+1; i++)
        if ((num % i ) == 0){
            c++;
            break;
        }
    return (c == 0 ? 1 : 0);
}
