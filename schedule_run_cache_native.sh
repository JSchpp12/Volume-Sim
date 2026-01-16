#!/bin/bash
#SBATCH --ntasks=1
#SBATCH --time=01:00:00
#SBATCH --output=logs/volume-sim/%x-%j.out

cd /data/js400421/volume-sim
./run_cache_native.sh