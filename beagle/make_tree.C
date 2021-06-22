R__LOAD_LIBRARY(libeicsmear)

void make_tree(std::string filstr){

  //erhic::DisKinematics::BoundaryWarning=false; //Need to comment this for eic account
  
  std::string dirstr = "outForPythiaMode";
  std::string inputstr = dirstr + "/" + filstr;
  std::string outputdir = dirstr;
  BuildTree(inputstr,outputdir);
}
