#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

int main(int argc, char *argv[]){
    int envia, recebe;
    int procs, meurank, i = 1000;
    int tag=10;
    MPI_Status status;
    int arg = atoi(argv[1]);

//   if(arg != 4)
//        return EXIT_FAILURE;

    
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &procs);

    printf("%d\n", procs);
    if(procs != 4){
        return EXIT_FAILURE;
    }

    MPI_Comm_rank(MPI_COMM_WORLD, &meurank);


    srand(313);
    while(i--){
        if(meurank != 0){
    	    MPI_Recv(&recebe, 1, MPI_INT, meurank-1, tag, MPI_COMM_WORLD,&status);
            //printf("Processo %d recebido %d\n", meurank, recebe);
        }
  
        if (meurank == 0){
            envia = rand() % 100 + 1;
        }
        else if (meurank == 1)
        	envia = recebe * recebe;
        else if (meurank == 2)
        	envia = recebe % 100;
        else if (meurank == 3)
        	//printf("Valor final %d\n", recebe);    	
        
        if(meurank != procs-1)        
            MPI_Send(&envia, 1, MPI_INT, meurank+1, tag, MPI_COMM_WORLD);
    }
    
    MPI_Finalize();
    return(0); 
} 
