#include <stdio.h>
#include <mpi.h>

int main(int argc, char ** argv){
    int meurank, procs;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &meurank);
    MPI_Comm_size(MPI_COMM_WORLD, &procs);
    
    printf("Processo %d de %d executando\n", meurank, procs);
    MPI_Finalize();
}
