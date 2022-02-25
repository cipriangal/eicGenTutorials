#!/usr/bin/bash

echo "-----------------------------------"
echo "Running DJANGOH Simulation for ep Collider!!!"
echo "..."
echo ""


OUTFILE1=outfiles/djangoh.NC.Rad.20x250_evt.dat
if test -f "$OUTFILE1"; then
	rm -f "$OUTFILE1"
fi
OUTFILE2=outfiles/djangoh.NC.Rad.20x250_out.dat
if test -f "$OUTFILE2"; then
	rm -f "$OUTFILE2"
fi
OUTFILE3=outfiles/djangoh.NC.Rad.20x250_smp.dat
if test -f "$OUTFILE3"; then
        rm -f "$OUTFILE3"
fi

djangoh < ep.Rad=1.NC.in > logfiles/ep.Rad=1.NC.log

echo "Completed Simulation!!!"
echo ""

echo "Making Output ROOT File..."
root -l -b -q 'make_tree.C("djangoh.NC.Rad.20x250_evt.dat")'
echo "Done!!!"

echo "-----------------------------------"

