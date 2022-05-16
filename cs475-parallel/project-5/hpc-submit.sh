#!/bin/bash
#SBATCH -J montecarlo
#SBATCH -A cs475-575
#SBATCH -p class
#SBATCH --constraint=v100
#SBATCH --gres=gpu:1
#SBATCH -o montecarlo.out
#SBATCH -e montecarlo.err
#SBATCH --mail-type=BEGIN,END,FAIL
#SBATCH --mail-user=detjensr@oregonstate.edu

make montecarlo
make bench
# ./montecarlo
