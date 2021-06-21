#!/usr/bin/bash

echo "-----------------------------------"
echo "Running PYTHIA Simulation for ep Collider!!!"
echo "..."
echo ""

pythiaeRHIC --out=outfiles/ep_10_100_norad_def.root < input.data_noradcor.eic > logfiles/ep_10_100_norad.log #default root file only created on sPhenix account

echo "Completed Simulation!!!"
echo ""

echo "Making Output ROOT File..."
root -l -b -q 'make_tree.C("ep_10_100_norad.out")'
echo "Done!!!"

echo "-----------------------------------"

echo "Making First Smeared ROOT File..."
root -l -b -q 'make_smeared_perfect.C("ep_10_100_norad")'
echo "Done!!!"

echo "Making Second Smeared ROOT File..."
root -l -b -q 'make_smeared_central.C("ep_10_100_norad")'
echo "Done!!!"

echo "-----------------------------------"
