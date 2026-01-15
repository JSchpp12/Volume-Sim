#!/bin/bash
#SBATCH --ntasks=1
#SBATCH --time=01:00:00
#SBATCH --output=logs/%x-%j.out

cd /data/js400421/volume-sim
./run_cache.sh