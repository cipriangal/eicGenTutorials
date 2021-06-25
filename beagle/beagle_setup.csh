#!/bin/csh

#Source this file if you want to run BeAGLE
setenv BEAGLESYS ${EICDIRECTORY}/PACKAGES/BeAGLE
setenv LHAPDF5 ${EICDIRECTORY}/gcc-8.3/lib
setenv LD_LIBRARY_PATH ${LHAPDF5}:$LD_LIBRARY_PATH
source /cvmfs/sphenix.opensciencegrid.org/gcc-8.3/opt/sphenix/core/gcc/8.3.0.1-0a5ad/x86_64-centos7/setup.csh

