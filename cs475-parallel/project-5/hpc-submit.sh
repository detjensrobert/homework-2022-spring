#!/bin/bash
#SBATCH -J montecarlo
#SBATCH -A cs475-575
#SBATCH -p class
#SBATCH --constraint=v100
#SBATCH --gres=gpu:1
#SBATCH -o hpc.out
#SBATCH -e hpc.err
#SBATCH --mail-type=BEGIN,END,FAIL
#SBATCH --mail-user=detjensr@oregonstate.edu

make montecarlo
make bench
# ./montecarlo
