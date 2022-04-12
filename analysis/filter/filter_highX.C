//Filter EICTree file to only include high-X events
//...and then convert to HEPMC3 file.
//-----------------

R__LOAD_LIBRARY(libeicsmear)

void filter_highX(){

cout<<"--------------------------"<<endl;
cout<<"Filtering high-x events..."<<endl;

std::string dirstrin = "input_ROOT";
std::string strin = "djangoh.NC.Rad.20x250_evt.root";
std::string inputstr = dirstrin + "/" + strin;
std::string dirstrout = "output";
std::string strout = "NC.Rad.20x250_filtered.root";
std::string outputstr = dirstrout + "/" + strout;

//Make filtered ROOT file
TFile *fin = new TFile(inputstr.c_str());
TTree *tin = (TTree*) fin->Get("EICTree");
TFile *fout = new TFile(outputstr.c_str(),"recreate");
TTree *tout = tin->CopyTree("dtrueX>0.1");
fout->Write();

//Convert filtered filtered file to HEPMC3 format
TreeToHepMC(outputstr,dirstrout);

cout<<endl<<"Done!"<<endl;
cout<<"-------------------------"<<endl;

} 

