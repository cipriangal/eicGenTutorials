#include "erhic/EventDjangoh.h"

R__LOAD_LIBRARY(libeicsmear);
R__LOAD_LIBRARY(../detectors/detector_central_C.so);

void make_smeared_central(std::string filstr){

  erhic::DisKinematics::BoundaryWarning=false;
 
  std::string dirstr = "outfiles";
  std:string inputstr = dirstr + "/" + filstr + ".root";
  std::string outputstr = dirstr + "/" + filstr + "_central_smeared.root";

  SmearTree(BuildDetector(),inputstr,outputstr);

}
