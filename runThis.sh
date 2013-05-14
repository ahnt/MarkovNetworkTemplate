#!/bin/bash -login
#PBS -l nodes=1:ppn=1,walltime=24:00:00
#PBS -j oe
cd GCP/
./GCP LOD_"$localN".txt GEN_"$localN".txt
