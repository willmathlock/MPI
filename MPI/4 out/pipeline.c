#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

int main(int argc, char **argv){
    int procs, meurank;
    int tag=10, n, aux, primeiro, segundo, terceiro, quarto;
    MPI_Status status;
    
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &procs);
    MPI_Comm_rank(MPI_COMM_WORLD, &meurank);
    

    if(meurank == 0){
        primeiro = 12;
        MPI_Send(&primeiro, 1, MPI_INT, 1, tag, MPI_COMM_WORLD);
    }
    if(meurank == 1){
        MPI_Recv(&segundo, 1, MPI_INT, 0, tag, MPI_COMM_WORLD,&status);
        segundo = segundo * segundo;
        MPI_Send(&segundo, 1, MPI_INT, 2, tag, MPI_COMM_WORLD);
    }
    if(meurank == 2){
        MPI_Recv(&terceiro, 1, MPI_INT, 1, tag, MPI_COMM_WORLD,&status);
        terceiro = terceiro % 100;  
        MPI_Send(&terceiro, 1, MPI_INT, 3, tag, MPI_COMM_WORLD);
    }
    if(meurank == 3){
        MPI_Recv(&quarto, 1, MPI_INT, 2, tag, MPI_COMM_WORLD,&status);
        printf("%d\n", quarto);

    }

    MPI_Finalize();
    return(0); 
} 
