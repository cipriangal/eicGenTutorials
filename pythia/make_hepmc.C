R__LOAD_LIBRARY(libeicsmear)

void make_hepmc(std::string filstr){

  std::string dirstr = "outfiles";
  std::string inputstr = dirstr + "/" + filstr;
  TreeToHepMC(inputstr,dirstr);
}
