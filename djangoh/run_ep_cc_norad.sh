#!/usr/bin/bash

echo "-----------------------------------"
echo "Running DJANGOH Simulation for e+p (positron-proton) Collider!!!"
echo "..."
echo ""


OUTFILE1=outfiles/djangoh.CC.18x275_evt.dat
if test -f "$OUTFILE1"; then
	rm -f "$OUTFILE1"
fi
OUTFILE2=outfiles/djangoh.CC.18x275_out.dat
if test -f "$OUTFILE2"; then
	rm -f "$OUTFILE2"
fi
OUTFILE3=outfiles/djangoh.CC.18x275_smp.dat
if test -f "$OUTFILE3"; then
        rm -f "$OUTFILE3"
fi

djangoh < ep.Rad=0.CC.in > logfiles/ep.Rad=0.CC.log

echo "Completed Simulation!!!"
echo ""

echo "Making Output ROOT File..."
root -l -b -q 'make_tree.C("djangoh.CC.18x275_evt.dat")'
echo "Done!!!"

echo "-----------------------------------"

