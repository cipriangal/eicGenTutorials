# eicGenTutorials
This repository provides a starting setup for eic generator tutorials.
<br/>
<br/>

If you are working on the [Singularity container](https://github.com/eic/Singularity) or [Virtual Box](https://github.com/eic/Singularity/blob/master/VirtualBox.md), do the following.

First clone this repository. 

Run the following command to enter the singularity container environment:
> ./singularity_shell.sh (Virtual Box)

or

> singularity shell -B /cvmfs:/cvmfs /cvmfs/eic.opensciencegrid.org/singularity/rhic_sl7_ext.simg (Singularity install)

Once in the container, setup the EIC environment.
> cd eicGenTutorials  
> source SDCC_setup.sh

To run BeAGLE, do the following in addition:
> cd beagle  
> source beagle_setup.sh  

N.B. The above links for the Singularity container and Virtual Box instruct the user to source the Fun4All environment after entering the container (e.g. running <ins>source setup.sh</ins> right after entering the container in the Virtual Box). You should not do this if you want to run the above generators and eic-smear only. Just source the SDCC_setup.sh script as descibed above.

<hr/>

If you are working on the [SDCC](https://www.sdcc.bnl.gov/) (RCF), simply do the following to setup the environment:
> source SDCC_setup.sh (or SDCC_setup.csh if on C shell)

To run BeAGLE, do the following in addition:
> cd beagle  
> source beagle_setup.sh (or beagle_setup.csh if on C shell)  

To run the compiled analysis, use Makefile.SDCC
<br/>


