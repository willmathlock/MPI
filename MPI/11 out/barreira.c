#include <stdio.h>
#include <mpi.h>

int main(int argc, char **argv){
    int i, j, c=0, meurank;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &meurank);

    printf("Processo %d antes\n", meurank);
    if (meurank == 0)
       for(i=1; i< 50000; i++)
            for(j=1; j < 50000; j++)
                if (i * j % 313 == 0)
                    c++;

    //MPI_Barrier(MPI_COMM_WORLD);   
    printf("Processo %d depois\n", meurank);

    MPI_Finalize();

    return(0); 
} 
