#!/bin/bash
#PBS -N p
#PBS -l nodes=1:ppn=3
#PBS -l walltime=0:05:00
#PBS -o /dev/null
#PBS -e /dev/null

cd $PBS_O_WORKDIR

# Load required modules
ml icc
ml openmpi

mpirun -np 1 prod >output/res-1
mpirun -np 2 prod >output/res-2
mpirun -np 3 prod >output/res-3
mpirun -np 6 prod >output/res-6
