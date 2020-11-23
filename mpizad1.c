#include <stdio.h>
#include <math.h>
#include "mpi.h"

int main(int argc, char **argv)
{
    int tag = 2115;
    int proces;
    int suma_procesow;               
    MPI_Init(&argc, &argv); 
    MPI_Comm_size(MPI_COMM_WORLD, &suma_procesow);
    MPI_Comm_rank(MPI_COMM_WORLD, &proces);
    MPI_Status status;
    double data = 0;
    int potega = 1;
    if(proces == 0)
    {
	printf("Proces o numerze %d zaczyna prace\n", proces);
	data += 4 * powf(-1, potega - 1) / (2 * potega - 1);
	potega += 1;
        MPI_Send(&data, 1, MPI_DOUBLE, proces+1, tag, MPI_COMM_WORLD);
	MPI_Send(&potega, 1, MPI_DOUBLE, proces+1, tag, MPI_COMM_WORLD);
    }

    if(proces > 0 && proces <= suma_procesow-1)
    {
	MPI_Recv(&data, 1, MPI_DOUBLE, proces-1, tag, MPI_COMM_WORLD, &status);
	MPI_Recv(&potega, 1, MPI_DOUBLE, proces-1, tag, MPI_COMM_WORLD, &status);
        printf("Proces o numerze %d odebral wartosc funkcji: %f\n", proces, data);
	data += 4 * powf(-1, potega - 1) / (2 * potega - 1);
	potega += 1;
	if(proces != suma_procesow-1)
	{
		MPI_Send(&data, 1, MPI_DOUBLE, proces+1, tag, MPI_COMM_WORLD);
		MPI_Send(&potega, 1, MPI_DOUBLE, proces+1, tag, MPI_COMM_WORLD);
	}
    }

    MPI_Finalize();
    return 0;
}