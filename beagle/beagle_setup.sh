#!/bin/bash

#Source this file if you want to run BeAGLE
export BEAGLESYS="$EICDIRECTORY/PACKAGES/BeAGLE"
export LHAPDF5="$EICDIRECTORY/gcc-8.3/lib"
export LD_LIBRARY_PATH=$LHAPDF5:$LD_LIBRARY_PATH
source /cvmfs/sphenix.opensciencegrid.org/gcc-8.3/opt/sphenix/core/gcc/8.3.0.1-0a5ad/x86_64-centos7/setup.sh

