#include <stdio.h>
#include <mpi.h>

#define TAM 4

int main(int argc, char **argv){
   int i;
   int mat[TAM][TAM]= {
       {  1,  2,  3,  4},
       {  5,  6,  7,  8},
       {  9, 10, 11, 12},
       { 13, 14, 15, 16}  };
   int vet[TAM];
   int meurank;

   MPI_Init(&argc, &argv);
   MPI_Comm_rank(MPI_COMM_WORLD, &meurank);

   MPI_Scatter(mat, TAM, MPI_INT, 
               vet, TAM, MPI_INT, 
                 0, MPI_COMM_WORLD);

   printf("Processo %d [ ",meurank );
   for(i=0; i < TAM; i++)
      printf("%d ",vet[i]);
   printf("] Final do processo %d\n", meurank);

   MPI_Finalize();

   return(0);
}
