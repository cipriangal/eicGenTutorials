#include "eicsmear/erhic/EventPythia.h"
#include "eicsmear/smear/EventSmear.h"

#include "Riostream.h"
#include "TApplication.h"
#include "TH1.h"
#include "TH2.h"
#include "TFile.h"
#include "TTree.h"
#include "TCanvas.h"
#include "TRint.h"
#include "TLatex.h"
#include "TStyle.h"
#include "TLegend.h"
#include "TMath.h"
#include "TLorentzVector.h"
#include "TLine.h"

#include <cstdio>
#include <cstdlib>
#include <cmath>

using namespace std;

int main(int argc, char **argv){

  #ifdef WITHRINT
  TRint *myapp = new TRint("RootSession",&argc,argv,NULL,0);
  #else
  TApplication *myapp = new TApplication("myapp",0,0);
  #endif
  
  //Event Class
  erhic::EventPythia *event(NULL);
  
  //Particle Class
  erhic::ParticleMC *particle(NULL);

  //Smeared Event Class
  Smear::Event *event_s(NULL);
  
  //Smeared Particle Class
  Smear::ParticleMCS *particle_s(NULL);
  
  TFile *f = new TFile("../../pythia/outfiles/ep_10_100_norad.root");

  TTree *tree = (TTree*)f->Get("EICTree");
  tree->AddFriend("Smeared","../../pythia/outfiles/ep_10_100_norad_central_smeared.root");

  tree->SetBranchAddress("event",&event);
  tree->SetBranchAddress("eventS",&event_s);
  Int_t nevents = tree->GetEntries();
  
  //MC Variables
  Int_t nfinal(0); // Number of Particles in final-state
  Int_t nParticles(0);
  Int_t Status[500];
  Int_t id[500];
  TLorentzVector ef;

  //Smeared Variables
  Int_t nfinal_s(0);
  Int_t nParticles_s(0);
  Int_t Status_s[500];
  Int_t id_s[500];
  TLorentzVector ef_s;

  //Histograms
  TH1 *h1 = new TH1D("h1","Smeared Energy minus True Energy",100,-1,1);
  h1->SetLineWidth(3);h1->SetLineColor(kBlue);
  h1->GetXaxis()->SetTitle("Difference [GeV]");h1->GetXaxis()->CenterTitle();

  TH1 *h2 = new TH1D("h2","Smeared Momentum minus True Momentum",100,-1,1);
  h2->SetLineWidth(3);h2->SetLineColor(kBlue);
  h2->GetXaxis()->SetTitle("Difference [GeV/c]");h2->GetXaxis()->CenterTitle();

  TH1 *h3 = new TH1D("h3","Smeared Transverse Momentum minus True Transverse Momentum",100,-1,1);
  h3->SetLineWidth(3);h3->SetLineColor(kBlue);
  h3->GetXaxis()->SetTitle("Difference [GeV/c]");h3->GetXaxis()->CenterTitle();

  TH1 *h4 = new TH1D("h4","Smeared Longitudinal Momentum minus True Longitudinal Momentum",100,-1,1);
  h4->SetLineWidth(3);h4->SetLineColor(kBlue);
  h4->GetXaxis()->SetTitle("Difference [GeV/c]");h4->GetXaxis()->CenterTitle();

  TH1 *h5 = new TH1D("h5","Smeared Polar Angle minus True Polar Angle",100,-1,1);
  h5->SetLineWidth(3);h5->SetLineColor(kBlue);
  h5->GetXaxis()->SetTitle("Difference [rad]");h5->GetXaxis()->CenterTitle();

  TH1 *h6 = new TH1D("h6","Smeared Azimuthal Angle minus True Azimuthal Angle",100,-1,1);
  h6->SetLineWidth(3);h6->SetLineColor(kBlue);
  h6->GetXaxis()->SetTitle("Difference [rad]");h6->GetXaxis()->CenterTitle();

  //Begin Analysis
  cout<<"Starting Analysis..."<<endl;
  cout<<"Total Number of Events = "<<nevents<<endl;
  
  // Loop over all events
  for(int i=0;i<nevents;i++){
    if(i%10000==0) cout<<"Events Analysed = "<<i<<"!"<<endl;
    
    tree->GetEntry(i);    

    //MC Tree First
    //Reset Variables
    nfinal=0;

    nParticles = event->GetNTracks();
    
    // Loop over all particles in event 
    for(int j=0;j<nParticles;j++){
      
      particle = event->GetTrack(j);
      Status[j] = (Int_t) particle->GetStatus();
      id[j] = (Int_t) particle->Id();

      // Get final state election
      if(id[j]==11 && Status[j]==1){
      	ef.SetPxPyPzE(particle->GetPx(),particle->GetPy(),particle->GetPz(),particle->GetE());
        nfinal++;
      } 
    }//Finished Loop Over Particles

    //----------------------------------------
    //Finished MC Tree...Now Do Smeared Tree
    //Reset Variables
    nfinal_s=0;

    //Get Total Number of Particles
    nParticles_s = event_s->GetNTracks();

    //Loop Over particles to find scattered electron
    for(Int_t j=0;j<nParticles_s;j++){
      
      particle_s = event_s->GetTrack(j);
      
      if(particle_s){ //make sure not null pointer
        Status_s[j] = (Int_t) particle_s->GetStatus();
	id_s[j] = (Int_t) particle_s->Id();
	
	if(Status_s[j]==1 && id_s[j]==11){
          ef_s.SetPxPyPzE(particle_s->GetPx(),particle_s->GetPy(),particle_s->GetPz(),particle_s->GetE());
          nfinal_s++;
	}//Final State Particles
      }//Particle NULL check
    }//Finished Smeared Particle Loop

 
    //Fill MC Histograms
    if(nfinal_s==1){ //avoid events with more than 1 detected electron
    	h1->Fill(ef_s.E()-ef.E());
    	h2->Fill(ef_s.P()-ef.P());
    	h3->Fill(ef_s.Pt()-ef.Pt());
    	h4->Fill(ef_s.Pz()-ef.Pz());
    	h5->Fill(ef_s.Theta()-ef.Theta());
    	h6->Fill(ef_s.Phi()-ef.Phi());
    }

  }//Finished Event Loop


  //Make Plots
  gStyle->SetOptStat(0);

  TCanvas *c1 = new TCanvas("c1");
  h1->Draw("");

  TCanvas *c2 = new TCanvas("c2");
  h2->Draw("");

  TCanvas *c3 = new TCanvas("c3");
  h3->Draw("");

  TCanvas *c4 = new TCanvas("c4");
  h4->Draw("");

  TCanvas *c5 = new TCanvas("c5");
  h5->Draw("");

  TCanvas *c6 = new TCanvas("c6");
  h6->Draw("");

  //Print Plots to File
  c1->Print("plots/analyze_smeared.pdf[");
  c1->Print("plots/analyze_smeared.pdf");
  c2->Print("plots/analyze_smeared.pdf");
  c3->Print("plots/analyze_smeared.pdf");
  c4->Print("plots/analyze_smeared.pdf");
  c5->Print("plots/analyze_smeared.pdf");
  c6->Print("plots/analyze_smeared.pdf");
  c6->Print("plots/analyze_smeared.pdf]");
  
  myapp->Run();
  return 0;

}

