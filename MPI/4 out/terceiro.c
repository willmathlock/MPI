#include <stdio.h>
#include <mpi.h>

#define TAM 12

int main(int argc, char **argv){
    int i, meurank, procs, parte, ind=0;
    int vet[TAM];
    int tag=10;
    MPI_Status status;
    
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &procs);
    MPI_Comm_rank(MPI_COMM_WORLD, &meurank);
    
    parte = (int)(TAM / (procs-1));
    if (meurank == 0){
       for(i=0; i<TAM; i++)
           vet[i] = i;
           
       for(i=1; i < procs; i++){
          MPI_Send(&vet[ind], parte, MPI_INT, i, tag, MPI_COMM_WORLD);
         ind = ind + parte;
       }
    }
    else {
       MPI_Recv(vet, parte, MPI_INT, 0, tag, MPI_COMM_WORLD, &status);
       printf("Proc. %d recebeu [ ", meurank);
       for(i=0; i<parte; i++)
          printf("%d ", vet[i]);
          
    printf("] Final do proc. %d\n",meurank);
    }
    
    MPI_Finalize();
    return(0);
} 

