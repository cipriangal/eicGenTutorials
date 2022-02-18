#!/usr/bin/bash

echo "-----------------------------------"
echo "Running PYTHIA Simulation number for ep Fixed Target!!!"
echo "..."
echo ""

pythiaeRHIC < ep_fixed.inp > logfiles/ep_fixed.log

echo "Completed Simulation!!!"
echo ""

echo "Making Output ROOT File..."
root -l -b -q 'make_tree.C("ep_fixed_10.out")'
echo "-----------------------------------"

echo "Making Output HEPMC File..."
root -l -b -q 'make_hepmc.C("ep_fixed_10.root")'
echo "-----------------------------------"

echo "Done!!!"


