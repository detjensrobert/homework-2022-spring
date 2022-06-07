#!/bin/bash
#SBATCH -J mpi-correlation
#SBATCH -A cs475-575
#SBATCH -p class
#SBATCH -N 16 # number of nodes
#SBATCH -n 16 # number of tasks
#SBATCH --constraint=ib
#SBATCH -o hpc.out
#SBATCH -e hpc.err
#SBATCH --mail-type=BEGIN,END,FAIL
#SBATCH --mail-user=detjensr@oregonstate.edu

module load openmpi

make bench
