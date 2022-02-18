#!/bin/sh

echo "-----------------------------------"
echo "Running PYTHIA Simulation for ep Collider!!!"
echo "..."
echo ""

./DIS_18275 > logfiles/DIS_18275.log

echo "Completed Simulation!!!"
echo ""

echo "Making Output ROOT File..."
mv hepmcout.dat outfiles/
root -l -b -q 'make_tree.C("hepmcout.dat")'
echo "Done!!!"

