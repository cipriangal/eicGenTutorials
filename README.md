# eicGenTutorials
This repository provides a starting setup for using our EIC event generators. It also provides example generator-level analysis code, as well as example fast smearing code.

**Start by reading [this document](documents/EIC_EventGen_Tutorial.pdf).**

<hr/>
<hr/>

If you are working on the [Singularity container](https://github.com/ECCE-EIC/Singularity) or [Virtual Box](https://github.com/eic/Singularity/blob/master/VirtualBox.md), do the following:

First clone this repository. 

Then run the following command to enter the singularity container environment:
> ./singularity_shell.sh (Virtual Box)

or

> singularity shell -B /cvmfs:/cvmfs /cvmfs/eic.opensciencegrid.org/singularity/rhic_sl7_ext.simg (Singularity install option 1)  
> singularity shell -B cvmfs:/cvmfs cvmfs/eic.opensciencegrid.org/singularity/rhic_sl7_ext.simg   (Singularity install option 2)  

Once in the container, setup the EIC environment.
> cd eicGenTutorials  
> source SDCC_setup.sh

To run BeAGLE, do the following in addition:
> cd beagle  
> source beagle_setup.sh  

N.B. The above links for the Singularity container and Virtual Box instruct the user to source the Fun4All environment after entering the container (e.g. running <ins>source setup.sh</ins> right after entering the container in the Virtual Box). You should not do this if you want to run the above generators and eic-smear only. Just source the SDCC_setup.sh script as described above.

<hr/>

If you are working on the [SDCC](https://www.sdcc.bnl.gov/) (RCF), simply do the following to setup the environment:
> source SDCC_setup.sh (or SDCC_setup.csh if on C shell)

To run BeAGLE, do the following in addition:
> cd beagle  
> source beagle_setup.sh (or beagle_setup.csh if on C shell)  

<br/>


