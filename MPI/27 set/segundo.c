#include <stdio.h>
#include <mpi.h>

int main(int argc, char **argv){
    int i, envia, recebe;
    int procs, meurank;
    int tag=10;
    MPI_Status status;
    
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &procs);
    MPI_Comm_rank(MPI_COMM_WORLD, &meurank);
    
    if (meurank == 0){
        for(i=1; i< procs; i++){
            MPI_Recv(&recebe, 1, MPI_INT, i, tag, MPI_COMM_WORLD, &status);
          //MPI_Recv(mensagem recebida, tipo da mensagem, de quem, variavel, comunicador, status)
            printf("Recebido o valor %d do processo %d\n", recebe, i);
        }
    }
    else {
        envia = meurank * 2;
        MPI_Send(&envia, 1, MPI_INT, 0, tag, MPI_COMM_WORLD);
        //MPI_Recv(mensagem enviada, tipo da mensagem, para quem, variavel, comunicador, status)
    }

    MPI_Finalize();
    return(0); 
} 

