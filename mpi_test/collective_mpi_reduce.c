#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define SIZE 4

float *create_rand_nums(int num_elements)
{
  float *rand_nums = (float *)malloc(sizeof(float)*num_elements);
  int i;
  for (i=0;i<num_elements;i++)
    {
      rand_nums[i] = (rand()/(float)RAND_MAX);
    }
  return rand_nums;
}


void main(int argc, char *argv[]) {


  float *rand_nums = NULL;
  float local_sum=0;

  int numtasks, rank, sendcount,recvcount,source;

  MPI_Init(&argc,&argv);
  MPI_Comm_rank(MPI_COMM_WORLD,&rank);
  MPI_Comm_size(MPI_COMM_WORLD,&numtasks);

  srand(time(NULL)*numtasks); 
  rand_nums = create_rand_nums(2);
  // sum local
  int i;
  for (i=0; i< 2;i++)
    {
      local_sum += rand_nums[i];
    }

  printf("Local sum for processes %d - %f, avg = %f \n",
	  rank,local_sum, local_sum/2);
  float global_sum;
  MPI_Reduce(&local_sum,&global_sum,1,MPI_FLOAT,MPI_SUM,0,MPI_COMM_WORLD);

  if(rank==0)
    {
      printf("Total sum = %f, avg= %f/n", global_sum, global_sum/(2*numtasks));
    }
  free(rand_nums);
  MPI_Barrier(MPI_COMM_WORLD);
  MPI_Finalize();
}
