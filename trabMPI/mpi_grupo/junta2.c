#include <stdio.h>
#include <mpi.h>

#define TAM 4

int main(int argc, char **argv){
   int vet[TAM*2];
   int ped[TAM/2];
   int i, meurank;

   MPI_Init(&argc, &argv);
   MPI_Comm_rank(MPI_COMM_WORLD, &meurank);

   ped[0] = meurank;
   ped[1] = meurank * 10;
   
   MPI_Gather(ped, TAM/2, MPI_INT, 
              vet, TAM/2, MPI_INT, 
                0, MPI_COMM_WORLD);

   if (meurank == 0){
      printf("Processo %d [ ",meurank);
      for(i=0; i < TAM*2; i++)
         printf("%d ",vet[i]);
      printf("]\nFinal processo %d\n", meurank);
   }
    
   MPI_Finalize();

   return(0);
}
