# Generation of proton-proton events with Pythia8

The files in this directory will allow you to simulate proton-proton collisions with Pythia8. The executable can be created by typing \```make```. The simulation can then be run by doing the following:

./RHIC_pp RHIC_pp.cmnd RHIC_pp.hepmc

The simulation configuration is set in the file <i>RHIC_pp.cmnd</i>. (The energy is currently set to RHIC energies, hence the file names.) You can make changes to the parameters in the configuration file and run a new simulation without recompiling.

The output file is in HepMC3 format and is called <i>RHIC_pp.hepmc</i>. This file can be converted to a ROOT file for easier analysis.

Comments on process selection
-----------------------------
You should not turn on both hard and soft processes at the same time. When running hard processes, you must set a minimum transverse momentum limit. See [here](https://pythia.org/latest-manual/QCDSoftProcesses.html) and [here](https://pythia.org/latest-manual/QCDHardProcesses.html) for details.

Running on SDCC
---------------
If running on SDCC, you may wish to use the most up-to-date environment instead:

```
setenv EIC_LEVEL EIC2022a
source /cvmfs/eic.opensciencegrid.org/gcc-8.3/MCEG/releases/etc/eic_cshrc.csh
setenv LD_LIBRARY_PATH ${LHAPDF6}lib:$LD_LIBRARY_PATH
setenv PATH ${LHAPDF6}bin:${PATH}
```

