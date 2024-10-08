R__LOAD_LIBRARY(libeicsmear);

void conservation_check(){

  //Event Class
  erhic::EventPythia *event(NULL); //Note that I use Pointer

  //Particle Class
  erhic::ParticleMC *particle(NULL); //Also use Pointer
  
  //Load ROOT File
  TFile *f = new TFile("../pythia/outfiles/ep_10_100_norad.root");

  //Get EICTree Tree
  TTree *tree = (TTree*)f->Get("EICTree");

  int nEntries = tree->GetEntries();
  cout<<"-------------------------------"<<endl;
  cout<<"Total Number of Events = "<<nEntries<<endl<<endl;

  //Access event Branch
  tree->SetBranchAddress("event",&event); //Note &event, even with event being a pointer

  //Variables
  double Eout(0),Ein(0);
  double pxin(0),pyin(0),pzin(0);
  double pxout(0),pyout(0),pzout(0);
  int status(0);
  int nParticles(0);
  int orig(0);
  int id(0);

  //Define Histograms
  TH1 *h1 = new TH1D("h1","Energy conservation ",200,-10,10);
  h1->GetXaxis()->SetTitle("E_{out} - E_{in} [GeV]");h1->GetXaxis()->CenterTitle();
  h1->SetLineWidth(3);h1->SetLineColor(kBlue);

  TH1 *h2 = new TH1D("h2","p_{z} momentum component conservation",200,-10,10);
  h2->GetXaxis()->SetTitle("p_{z,out} - p_{z,in} [GeV/c]");h2->GetXaxis()->CenterTitle();
  h2->SetLineWidth(3);h2->SetLineColor(kBlue);

  TH1 *h3 = new TH1D("h3","p_{x} momentum component conservation",200,-10,10);
  h3->GetXaxis()->SetTitle("p_{x,out} - p_{x,in} [GeV/c]");h3->GetXaxis()->CenterTitle();
  h3->SetLineWidth(3);h3->SetLineColor(kBlue);

  TH1 *h4 = new TH1D("h4","p_{y} momentum component conservation",200,-10,10);
  h4->GetXaxis()->SetTitle("p_{y,out} - p_{y,in} [GeV/c]");h4->GetXaxis()->CenterTitle();
  h4->SetLineWidth(3);h4->SetLineColor(kBlue);
  
  //Loop Over Events
  for(int iEvent=0;iEvent<nEntries;iEvent++){
    
    tree->GetEntry(iEvent);

    //Reset Variables
    Ein=0;pzin=0;pxin=0;pyin=0;
    Eout=0;pzout=0;pxout=0;pyout=0;
    
    nParticles = event->GetNTracks();

    // Loop over all particles in event 
    for(int iParticle=0;iParticle<nParticles;iParticle++){

      particle = event->GetTrack(iParticle);
      
      id = (int) particle->Id();  //Particle Id Number
      status = (int) particle->GetStatus();  //Particle status
      orig = (int) particle->GetParentIndex(); //Particle origin (i.e. parent)

      //Get total incoming energy and momentum components
      if(status==21 && orig==0 && (id==11 || id==2212) ){ //N.B. You can also just select the first two particles in the list
        Ein+=particle->GetE();
        pzin+=particle->GetPz();
        pxin+=particle->GetPx();
        pyin+=particle->GetPy();
      }


      //Get total outgoing energy and momentum components
      if(status==1){
        Eout+=particle->GetE();
        pzout+=particle->GetPz();
        pxout+=particle->GetPx();
        pyout+=particle->GetPy();
      }

    }//Finished Loop Over Particles

    //Fill histograms
    h1->Fill(Eout-Ein);
    h2->Fill(pzout-pzin);
    h3->Fill(pxout-pxin);
    h4->Fill(pyout-pyin);

  } //Finished loop over entries

  //Make plots
  gStyle->SetOptStat(0);
  gStyle->SetFrameLineWidth(2);

  TCanvas *c1 = new TCanvas("c1");
  h1->Draw();

  TCanvas *c2 = new TCanvas("c2");
  h2->Draw();

  TCanvas *c3 = new TCanvas("c3");
  h3->Draw();

  TCanvas *c4 = new TCanvas("c4");
  h4->Draw();

  //Print plots to pdf file
  c1->Print("plots/conservation_check.pdf[");
  c1->Print("plots/conservation_check.pdf");
  c2->Print("plots/conservation_check.pdf");
  c3->Print("plots/conservation_check.pdf");
  c4->Print("plots/conservation_check.pdf");
  c4->Print("plots/conservation_check.pdf]");

  cout<<"-------------------------------"<<endl;

}
