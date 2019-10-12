#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

int main(int argc, char **argv){
    int i, envia, recebe, len;
    int procs, meurank;
    MPI_Status st;
    char name[MPI_MAX_PROCESSOR_NAME];

    
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &procs);
    MPI_Comm_rank(MPI_COMM_WORLD, &meurank);
    MPI_Get_processor_name(name, &len);
    
    srand(meurank);

    if (meurank == 0){
       for(i=1; i < procs; i++)
          MPI_Send(&i, 1, MPI_INT, i, i, MPI_COMM_WORLD);
       for(i=1; i < procs; i++){
          MPI_Recv(&recebe, 1, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &st);
          printf("Recebido %d do proc. %d com tag %d\n", recebe, st.MPI_SOURCE, st.MPI_TAG);
       }
    }   

    else {
       MPI_Recv(&recebe, 1, MPI_INT, 0, MPI_ANY_TAG, MPI_COMM_WORLD, &st);
       envia = recebe * st.MPI_TAG;
       sleep(rand() % 5);
       MPI_Send(&envia, 1, MPI_INT, 0, meurank, MPI_COMM_WORLD);
    }
    MPI_Finalize();
    
    return(0);
} 
