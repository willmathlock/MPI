#include <stdio.h>
#include <mpi.h>

int main(int argc, char **argv){
    int valor;
    int meurank;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &meurank);

    if (meurank == 0)
       valor = 20;

    printf("Processo %d possui valor valendo ANTES %d\n", meurank, valor);

    MPI_Bcast(&valor, 1, MPI_INT, 0, MPI_COMM_WORLD);

    printf("Processo %d possui valor valendo DEPOIS %d\n", meurank, valor);

    MPI_Finalize();

    return(0);
}
