R__LOAD_LIBRARY(libeicsmear);

void access_smeared(){
  
  //Smeared Event Class
  Smear::Event *event_s(NULL);
  
  //Smeared Particle Class
  Smear::ParticleMCS *particle_s(NULL);
  
  //Load ROOT File
  TFile *f = new TFile("../pythia/outfiles/ep_10_100_norad_perfect_smeared.root");
  
  //Get EICTree Tree
  TTree *tree = (TTree*)f->Get("Smeared");
  
  Int_t nEntries = tree->GetEntries();
  cout<<"-------------------------------"<<endl;
  cout<<"Total Number of Events = "<<nEntries<<endl<<endl;
  
  //Access event Branch
  tree->SetBranchAddress("eventS",&event_s);
  
  //Define Some Variables
  Float_t Q2(0);
  Int_t nParticles(0);
  Float_t Px[500],Py[500],Pz[500];
  Int_t Status[500];
  Int_t id[500];
  Float_t theta[500];
  Float_t phi[500];
  Int_t beam_count(0),det_count(0);

  //Loop Over Events
  for(Int_t i=0;i<10;i++){
    
    tree->GetEntry(i);
    
    //Write Out Q2
    Q2 = (Float_t) event_s->GetQ2();
    printf("For Event %d, Q^2 = %.3f GeV^2!\n",i,Q2);
    
    //Get Total Number of Particles
    nParticles = event_s->GetNTracks();
    printf("For Event %d, we have %d particles!\n",i,nParticles);

    //Reset Counters
    beam_count=0;det_count=0;
    
    //Loop Over Each Particle
    for(Int_t j=0;j<nParticles;j++){
      
      particle_s = event_s->GetTrack(j);
      
      if(particle_s){ //make sure not null pointer
	Status[j] = (Int_t) particle_s->GetStatus();
	id[j] = (Int_t) particle_s->Id();
	Px[j] = (Float_t) particle_s->GetPx();
	Py[j] = (Float_t) particle_s->GetPy();
	Pz[j] = (Float_t) particle_s->GetPz();
	theta[j] = (Float_t) ( particle_s->GetTheta() * TMath::RadToDeg() );
	phi[j] = (Float_t) ( particle_s->GetPhi() * TMath::RadToDeg() );

	if(Status[j]==21) beam_count++;
	if(Status[j]==1) det_count++;
      }
      else{
	Status[j] = 0;
	id[j] = 0;
	Px[j] = 0;
	Py[j] = 0;
	Pz[j] = 0;
	theta[j] = 0;
      }
      
      if(particle_s){
	printf("For Event %d, particle %d is detected with Status %d\n",i,j,Status[j]);
	printf("For Event %d, particle %d is detected with Id %d\n",i,j,id[j]);
	printf("For Event %d, particle %d is detected with (Px,Py,Pz) = (%.3f,%.3f,%.3f) GeV\n",i,j,Px[j],Py[j],Pz[j]);
	printf("For Event %d, particle %d is detected with theta = %.3f degrees\n",i,j,theta[j]);
	printf("For Event %d, particle %d is detected with phi = %.3f degrees\n",i,j,phi[j]);
      }
      
    }
    printf("For Event %d, there are %d beam particles and %d detected particles\n\n",i,beam_count,det_count);
    
  }
  
  cout<<"-------------------------------"<<endl;

}
