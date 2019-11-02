#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <sys/time.h>

int main(int argc, char **argv){
   int numero, maior;
   int meurank;
   struct timeval tempo;

   MPI_Init(&argc, &argv);
   MPI_Comm_rank(MPI_COMM_WORLD, &meurank);

   gettimeofday(&tempo, NULL);
   srandom(tempo.tv_usec);
   numero = random() % 50000;
 
   printf("Processo %d gerou numero %d\n", meurank, numero);

   MPI_Reduce(&numero, &maior, 
                    1, MPI_INT, MPI_MAX, 
                    0, MPI_COMM_WORLD);

   //MPI_Barrier(MPI_COMM_WORLD);

   if (meurank == 0)
       printf("Processo %d encontrou maior valor %d\n", meurank, maior);

   MPI_Finalize();

   return(0);
}
