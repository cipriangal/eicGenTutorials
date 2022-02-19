#!/usr/bin/bash

echo "-----------------------------------"
echo "Smearing DJANGOH Simulation for ep Collider!!!"
echo "..."
echo ""

echo "-----------------------------------"

echo "Making First Smeared ROOT File..."
root -l -b -q 'make_smeared_perfect.C("djangoh.NC.20x250_evt")'
echo "Done!!!"

echo "Making Second Smeared ROOT File..."
root -l -b -q 'make_smeared_central.C("djangoh.NC.20x250_evt")'
echo "Done!!!"

echo "-----------------------------------"
