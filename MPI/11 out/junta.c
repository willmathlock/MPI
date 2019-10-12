#include <stdio.h>
#include <mpi.h>

#define TAM 4

int main(int argc, char **argv){
   int vet[TAM];
   int i, meurank;

   MPI_Init(&argc, &argv);
   MPI_Comm_rank(MPI_COMM_WORLD, &meurank);
   
   MPI_Gather(&meurank, 1, MPI_INT, 
                   vet, 1, MPI_INT, 
                     0, MPI_COMM_WORLD);

   if (meurank == 0){
      printf("Processo %d [ ",meurank);
      for(i=0; i < TAM; i++)
         printf("%d ",vet[i]);
      printf("] Final processo %d\n", meurank);
   }
   
   MPI_Finalize();

   return(0);
}
