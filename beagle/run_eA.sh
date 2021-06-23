#!/usr/bin/bash

echo "-----------------------------------"
echo "Running BeAGLE Simulation for eA Collider!!!"
echo "..."
echo ""


$BEAGLESYS/BeAGLE < input/eAu.inp > logs/eAu.log

echo "Making Output ROOT File..."
root -l -b -q 'make_tree.C("eAu.txt")'
echo "-----------------------------------"


echo "Done!!!"


