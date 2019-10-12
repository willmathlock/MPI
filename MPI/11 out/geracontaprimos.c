#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <mpi.h>

#define TAM 480000000
#define MAX 100

int ehprimo(int num);

int main(int argc, char **argv){
	int i, j, k, ind = 0;
    int *vet;
    int envia, recebe, len, parte, answ;
    int procs, meurank;
    MPI_Status st;
    char name[MPI_MAX_PROCESSOR_NAME];
    
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &procs);
    MPI_Comm_rank(MPI_COMM_WORLD, &meurank);
    MPI_Get_processor_name(name, &len);

    srand(313);
    vet = (int *)malloc(sizeof(int) * TAM);
    if (vet == NULL){
        printf("Nao foi\n");
        exit(1);
    }

    parte = (int)(TAM / (procs-1));
    if(meurank == 0){
        answ = 0;
        for(i=0; i < TAM; i++)
        vet[i] = rand() % MAX;
        //printf("Debug 1\n");
        
        for(i=1; i < procs; i++){
            MPI_Send(&vet[ind], parte, MPI_INT, i, i, MPI_COMM_WORLD);
            ind = ind + parte;
            //printf("Debug send1\n"); 
        }

        //printf("Debug 2\n");      
        for(i=1; i < procs; i++){
            //printf("Debug 3\n");
            MPI_Recv(&recebe, 1, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &st);
            answ = answ + recebe;
            //printf("Debug 4\n");
        }
        //answ += 1;
        printf("Primos %d\n Indice final: %d \n", answ, ind);  
    }else{
        j=0;
        MPI_Recv(vet, parte, MPI_INT, 0, MPI_ANY_TAG, MPI_COMM_WORLD, &st);
        //printf("Debug 5\n");
        for(i=0; i < parte; i++){
            if (ehprimo(vet[i])){
                j++;
            }
        }
        //printf("Debug 6\n");
        MPI_Send(&j, 1, MPI_INT, 0, meurank, MPI_COMM_WORLD);   
    }
    MPI_Finalize();    
}

int ehprimo(int num){
    int i,c=0;    

    for(i=3; i < num/2+1; i++)
        if ((num % i ) == 0){
            c++;
            break;
        }
    return (c == 0 ? 1 : 0);
}
