------------------------------------------------------------------------

 simpleG4MC: a Geant4 framework for simulating the JLab experiments

------------------------------------------------------------------------

	http://github.com/dhamilton-glasgow/simpleG4MC
	(developed and tested with Geant4.10.4 and root6.22 on centos7)

	Default geometry:
	Target is 10cm LH2, windows are 0.1mm thik Al
	No scattering chamber or exit beamline
	Detector 1 is 20x20x20cm3 PbWO4 at -25.5 deg and 250 cm (to center)
	Detector 2 is 40x40x40cm3 plastic scint at -15.5 deg and 150 cm (to center)
	Default units: MeV, cm, ns

------------------------------------------------------------------------
 Compilation

	mkdir build
  	cd build
  	cmake ../ (or cmake3 ../ on some systems)
  	make -jX (X specifies number of cores to use)

------------------------------------------------------------------------
 Running the simulation in visualisation mode

  	./simpleG4MC
  	then in the gui: 
	/control/execute macros/vis_beam.mac 

------------------------------------------------------------------------
 Running the simulation in batch mode

  	./simple4MC macros/batch_beam.mac 

