#!/bin/bash
if [[ $1 > 4 ]] 
then
    mpicc mpi_gol.c -lm -O3 && mpirun --use-hwthread-cpus --oversubscribe -np $1 a.out
elif [[ $1 > 2 ]]
then
    echo "mai2"
    mpicc mpi_gol.c -lm -O3 && mpirun --use-hwthread-cpus -np $1 a.out
else
    mpicc mpi_gol.c -lm -O3 && mpirun  -np $1 a.out
fi
