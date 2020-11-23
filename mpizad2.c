#include <stdio.h>
#include <math.h>
#include "mpi.h"

double funkcja(double x)
{
	return x*x;
}

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
    double aktualna_wartosc;
    double a = 5;
    double b = 10;  
    double dx = (b-a)/suma_procesow;

    if(proces == 0)
    {
	MPI_Recv(&data, 1, MPI_DOUBLE, proces+1, tag, MPI_COMM_WORLD, &status);
	printf("Proces o numerze %d odebral wartosc funkcji: %f\n", proces, data);
	data += funkcja(a) / 2 * dx;
	printf("Koncowa wartosc procesu %d to: %f\n", proces, data);
	
    }

    if(proces == suma_procesow-1)
    {
	data += funkcja(b) / 2 * dx;  
	MPI_Send(&data, 1, MPI_DOUBLE, proces-1, tag, MPI_COMM_WORLD);
	printf("Proces o numerze %d zaczyna prace\n", proces);
	
    }

    if(proces > 0 && proces < suma_procesow-1)
    {
	MPI_Recv(&data, 1, MPI_DOUBLE, proces+1, tag, MPI_COMM_WORLD, &status);
        printf("Proces o numerze %d odebral wartosc funkcji: %f\n", proces, data);
	data += funkcja(b - (dx * proces)) * dx;
	MPI_Send(&data, 1, MPI_DOUBLE, proces-1, tag, MPI_COMM_WORLD);
    }

    MPI_Finalize();
    return 0;
}