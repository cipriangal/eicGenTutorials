//-----------------------
//Simple Detector: perfect resolution, efficiency, and PID.
//Acceptance from [-15,15] in eta
//-----------------------

//To Build with ROOT6:
//>>root
//>>.L detector_perfect.C+
//-----------------------

#include "eicsmear/erhic/VirtualParticle.h"
#include "eicsmear/smear/Acceptance.h"
#include "eicsmear/smear/Device.h"
#include "eicsmear/smear/Detector.h"
#include "eicsmear/smear/Smearer.h"
#include "eicsmear/smear/ParticleMCS.h"
#include "eicsmear/smear/PerfectID.h"


//Convert pseudorapidity (eta) to polar angle (theta) in radians.
//Make use of TLorentzVector to do eta-to-theta conversion.
double etaToTheta(const double eta) {
  TLorentzVector v;
  v.SetPtEtaPhiM(1., eta, 0., 0.);
  return v.Theta();
}

//Convert and angle in degrees to one in radians.
double degreesToRadians(double degrees) {
  return degrees / 180. * TMath::Pi();
}

//Build Detector
Smear::Detector BuildDetector(){

  //Create Devices
  Smear::Device energy(Smear::kE,"0"); //"0":Perfect Resolution                                                                               
  Smear::Device momentum(Smear::kP,"0");
  Smear::Device theta(Smear::kTheta,"0");
  Smear::Device phi(Smear::kPhi,"0");

  //Detector Acceptance
  Smear::Acceptance::Zone central(etaToTheta(15.),etaToTheta(-15.));

  energy.Accept.AddZone(central);
  momentum.Accept.AddZone(central);
  theta.Accept.AddZone(central);
  phi.Accept.AddZone(central);

  //PID performance is unparameterised as of now
  Smear::PerfectID pid;
  pid.Accept.AddZone(central);
  
  //Create the detector and add devices
  Smear::Detector det;
  det.AddDevice(energy);
  det.AddDevice(momentum);
  det.AddDevice(theta);
  det.AddDevice(phi);
  det.AddDevice(pid);
  det.SetEventKinematicsCalculator("NM JB DA");

  return det;

}
