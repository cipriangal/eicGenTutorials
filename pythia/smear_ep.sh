#!/usr/bin/bash

echo "-----------------------------------"
echo "Smearing PYTHIA Simulation for ep Collider!!!"
echo "..."
echo ""

echo "-----------------------------------"

echo "Making First Smeared ROOT File..."
root -l -b -q 'make_smeared_perfect.C("ep_10_100_norad")'
echo "Done!!!"

echo "Making Second Smeared ROOT File..."
root -l -b -q 'make_smeared_central.C("ep_10_100_norad")'
echo "Done!!!"

echo "-----------------------------------"
