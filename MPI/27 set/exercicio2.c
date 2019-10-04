/**Exercícios de MPI
a) Modificar o 2 o . exemplo passado em aula, de forma a manter suas características e
funcionalidades originais:
I)
Criar um novo programa e retirar uma variável do programa (envia)
II)
Criar outro novo programa e retirar mais uma variável (recebe)
**/

// Parte I

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
        MPI_Send(&envia, 1, MPI_INT, 0, tag, MPI_COMM_WORLD);
        //MPI_Recv(mensagem enviada, tipo da mensagem, para quem, variavel, comunicador, status)
    }

    MPI_Finalize();
    return(0); 
} 
