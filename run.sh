#!/bin/bash
mpicc mpi_gol.c -lm -O3 && mpirun --use-hwthread-cpus -np $1 a.out
