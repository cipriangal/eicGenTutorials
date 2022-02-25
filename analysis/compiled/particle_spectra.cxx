#include "eicsmear/erhic/EventPythia.h"

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
#include "TGraph.h"
#include "TF1.h"
#include "TChain.h"
#include "TPaveText.h"

#include "PadMxN.h"

#include <cstdio>
#include <cstdlib>
#include <cmath>

using namespace std;

//Globals
const double Mp(0.9383);
const double Me(0.511E-3);

//---------------------
int main(int argc, char **argv){

    #ifdef WITHRINT
    TRint *myapp = new TRint("RootSession",&argc,argv,NULL,0);
    #else
    TApplication *myapp = new TApplication("myapp",0,0);
    #endif

    //Eta bins for scattered electrons, negative pions, other electrons, and positrons
    int nEta = 4; //Number of eta bins
    double Eta_low[] = {-1.5, -1.2, -0.9, -0.6};
    double Eta_hi[] =  {-1.2, -0.9, -0.6, -0.43};

    TH1 *h_se[nEta];
    TH1* h_ae[nEta]; 
    TH1* h_pos[nEta]; 
    TH1* h_npi[nEta];

    //Eta bins for positive pions and kaons 
    int nEtap = 4;
    double Etap_low[] = {2.50, 2.75, 3.00, 3.25};
    double Etap_hi[] =  {2.75, 3.00, 3.25, 3.50};

    TH1* h_ppi[nEtap];
    TH1* h_pka[nEtap];

    //use constant log binning
    double p_min = 1E-1;
    double p_max = 60;
	const int nbins = 100;
	double log_bw = (log10(p_max) - log10(p_min))/((double)nbins);
    double log_div;
    double p_bins[nbins+1];
    for(int i=0;i<nbins+1;i++){
		log_div = log10(p_min) + (i*log_bw);
		p_bins[i] = pow(10,log_div);
	}

    for(int ihist=0;ihist<nEta;ihist++){

        h_se[ihist] = new TH1D(Form("h_se[%d]",ihist),"",nbins,p_bins);
        h_se[ihist]->SetLineColor(kRed);h_se[ihist]->SetLineWidth(2);

        h_ae[ihist] = new TH1D(Form("h_ae[%d]",ihist),"",nbins,p_bins);
        h_ae[ihist]->SetLineColor(kBlack);h_ae[ihist]->SetLineWidth(2);

        h_pos[ihist] = new TH1D(Form("h_pos[%d]",ihist),"",nbins,p_bins);
        h_pos[ihist]->SetLineColor(kMagenta);h_pos[ihist]->SetLineWidth(2);

        h_npi[ihist] = new TH1D(Form("h_npi[%d]",ihist),"",nbins,p_bins);
        h_npi[ihist]->SetLineColor(kBlue);h_npi[ihist]->SetLineWidth(2);
    }

    for(int ihist=0;ihist<nEtap;ihist++){
        h_ppi[ihist] = new TH1D(Form("h_ppi[%d]",ihist),"",nbins,p_bins);
        h_ppi[ihist]->SetLineColor(kBlue);h_ppi[ihist]->SetLineWidth(2);

        h_pka[ihist] = new TH1D(Form("h_pka[%d]",ihist),"",nbins,p_bins);
        h_pka[ihist]->SetLineColor(kGreen);h_pka[ihist]->SetLineWidth(2);
    }

    //Load ROOT File
    erhic::EventPythia *event(NULL); //Event Class
    erhic::ParticleMC *particle(NULL); //Particle Class

    TFile *f = new TFile("../../pythia/outfiles/ep_10_100_norad_highstat.root");

    //Get EICTree Tree
    TTree *tree = (TTree*)f->Get("EICTree");

    tree->SetBranchAddress("event",&event);

    //Particle variables
    int nParticles(0);
    int id(0);
    int status(0);
    int orig(0);

    //Calculate Generated Luminosity
    int nevents = tree->GetEntries();

    cout<<"-------------------------------"<<endl;
    cout<<"PYTHIA6 Simulation:"<<endl;
    cout<<"Total Number of Events = "<<nevents<<endl;

    //Loop over events
    for(int iEvent=0;iEvent<nevents;iEvent++){
        
        if(iEvent%10000==0) cout<<"Events Analysed = "<<iEvent<<"!"<<endl;
        tree->GetEntry(iEvent);

        //Get Number of tracks
        nParticles = event->GetNTracks();

        // Loop over all particles in event
        for(int iParticle=0;iParticle<nParticles;iParticle++){
      
            particle = event->GetTrack(iParticle);

            id = (int) particle->Id();
            status = (int) particle->GetStatus();
            orig = (int) particle->GetParentIndex();

            if(id==11 && status==1 && orig==3){ //Scattered Electrons
                for(int iEta=0;iEta<nEta;iEta++){
                    if(particle->GetEta()>Eta_low[iEta] && particle->GetEta()<Eta_hi[iEta])
                        h_se[iEta]->Fill(particle->GetP());
                }
            }

            if(id==11 && status==1 && orig!=3){ //Non-scattered Electrons
                for(int iEta=0;iEta<nEta;iEta++){
                    if(particle->GetEta()>Eta_low[iEta] && particle->GetEta()<Eta_hi[iEta])
                        h_ae[iEta]->Fill(particle->GetP());
                }
            }

            if(id==-11 && status==1){ //Positrons
                for(int iEta=0;iEta<nEta;iEta++){
                    if(particle->GetEta()>Eta_low[iEta] && particle->GetEta()<Eta_hi[iEta])
                        h_pos[iEta]->Fill(particle->GetP());
                }
            }

            if(id==-211 && status==1){ //Negative Pions
                for(int iEta=0;iEta<nEta;iEta++){
                    if(particle->GetEta()>Eta_low[iEta] && particle->GetEta()<Eta_hi[iEta])
                        h_npi[iEta]->Fill(particle->GetP());
                }
            }

            if(id==211 && status==1){ //Positive Pions
                for(int iEta=0;iEta<nEtap;iEta++){
                    if(particle->GetEta()>Etap_low[iEta] && particle->GetEta()<Etap_hi[iEta])
                        h_ppi[iEta]->Fill(particle->GetP());
                }
            }

            if(id==321 && status==1){ //Positive Kaons
                for(int iEta=0;iEta<nEtap;iEta++){
                    if(particle->GetEta()>Etap_low[iEta] && particle->GetEta()<Etap_hi[iEta])
                        h_pka[iEta]->Fill(particle->GetP());
                }
            }

        } //closes particle loop
    } //closes event loop

    //Draw plots
    gStyle->SetPadBorderMode(0);
    gStyle->SetFrameBorderMode(0);
    gStyle->SetFrameLineWidth(2);
    gStyle->SetOptStat(0);
    gStyle->SetOptFit(1);
    gStyle->SetLabelSize(0.035,"X");
    gStyle->SetLabelSize(0.035,"Y");
    //gStyle->SetLabelOffset(0.01,"X");
    //gStyle->SetLabelOffset(0.01,"Y");
    gStyle->SetTitleXSize(0.055);
    gStyle->SetTitleXOffset(0.85);
    gStyle->SetTitleYSize(0.055);
    gStyle->SetTitleYOffset(0.85);

    //Plot 1
    PadMxN *pad2x2_1 = new PadMxN("c1",500,500,150,150,100,125,2,2);
    pad2x2_1->Draw();
	TH2 *hframe_1 = new TH2F("hframe_1","",100,0.101,59.99,100,0.2,2e5);

    for(int iCan=0;iCan<nEta;iCan++){
        pad2x2_1->GetPad(iCan+1);
        gPad->SetLogx();gPad->SetLogy();
        gPad->SetTickx();gPad->SetTicky();

        hframe_1->Draw();
        hframe_1->GetXaxis()->SetTitle("Momentum [GeV/c]");hframe_1->GetYaxis()->SetTitle("Yield");
		hframe_1->GetXaxis()->SetLabelFont(63);hframe_1->GetYaxis()->SetLabelFont(63);
      	hframe_1->GetXaxis()->SetLabelSize(25);hframe_1->GetYaxis()->SetLabelSize(25);
      	hframe_1->GetXaxis()->SetLabelOffset(0.01);hframe_1->GetYaxis()->SetLabelOffset(0.01);
      	hframe_1->GetXaxis()->CenterTitle(1);hframe_1->GetYaxis()->CenterTitle(1);
      	hframe_1->GetXaxis()->SetTitleSize(40);hframe_1->GetXaxis()->SetTitleOffset(2.5); 
      	hframe_1->GetYaxis()->SetTitleSize(40);hframe_1->GetYaxis()->SetTitleOffset(3.0);
        
        h_se[iCan]->Draw("same");
        h_npi[iCan]->Draw("same");
    
    }

    // get the text pad
    pad2x2_1->GetPad(5);

    TPaveText* pave1_1 = new TPaveText(0.25,0.91,0.35,0.96,"NDCNB");
    pave1_1->AddText("10 GeV e^{-} on 100 GeV p"); //Hard code the beam energy for now
	pave1_1->SetFillStyle(4000);
    pave1_1->SetTextFont(63);pave1_1->SetTextSize(35);
    pave1_1->Draw();

    TPaveText* pave1_2 = new TPaveText(0.7,0.95,0.8,0.98,"NDCNB");
    pave1_2->AddText("Negative Pions");
	pave1_2->SetFillStyle(4000);
    pave1_2->SetTextFont(63);pave1_2->SetTextSize(30);
    pave1_2->SetTextColor(kBlue);
    pave1_2->Draw();

    TPaveText* pave1_3 = new TPaveText(0.68,0.91,0.82,0.96,"NDCNB");
    pave1_3->AddText("Scattered Electrons");
	pave1_3->SetFillStyle(4000);
    pave1_3->SetTextFont(63);pave1_3->SetTextSize(30);
    pave1_3->SetTextColor(kRed);
    pave1_3->Draw();

    TPaveText *pave2[nEta];
    double p2_xl[] = {0.37,0.70,0.37,0.70};
    double p2_xh[] = {0.47,0.92,0.47,0.92};
    double p2_yl[] = {0.825,0.825,0.425,0.425};
    double p2_yh[] = {0.925,0.925,0.525,0.525};

    for(int ipave=0;ipave<nEta;ipave++){
        pave2[ipave] = new TPaveText(p2_xl[ipave],p2_yl[ipave],p2_xh[ipave],p2_yh[ipave],"NDCNB");
        pave2[ipave]->AddText(Form("%.1f < #eta < %.1f",Eta_low[ipave],Eta_hi[ipave]));
	    pave2[ipave]->SetFillStyle(4000);
        pave2[ipave]->SetTextFont(63);pave2[ipave]->SetTextSize(35);
        pave2[ipave]->Draw();
    }

    TPaveText* pave3_1 = new TPaveText(0.89,0.08,0.94,0.12,"NDCNB");
    pave3_1->AddText("60");
	pave3_1->SetFillStyle(4000);
    pave3_1->SetTextFont(63);pave3_1->SetTextSize(25);
    pave3_1->Draw();

    TPaveText* pave3_2 = new TPaveText(0.1,0.08,0.15,0.12,"NDCNB");
    pave3_2->AddText("10^{-1}");
	pave3_2->SetFillStyle(4000);
    pave3_2->SetTextFont(63);pave3_2->SetTextSize(25);
    pave3_2->Draw();

    TPaveText* pave3_3 = new TPaveText(0.5,0.08,0.55,0.12,"NDCNB");
    pave3_3->AddText("10^{-1}");
	pave3_3->SetFillStyle(4000);
    pave3_3->SetTextFont(63);pave3_3->SetTextSize(25);
    pave3_3->Draw();

    //Plot 2
    PadMxN *pad2x2_2 = new PadMxN("c2",500,500,150,150,100,125,2,2);
    pad2x2_2->Draw();

    for(int iCan=0;iCan<nEta;iCan++){
        pad2x2_2->GetPad(iCan+1);
        gPad->SetLogx();gPad->SetLogy();
        gPad->SetTickx();gPad->SetTicky();

        hframe_1->Draw();
        h_ae[iCan]->Draw("same");
        h_pos[iCan]->Draw("same");
    
    }

    // get the text pad
    pad2x2_2->GetPad(5);

    pave1_1->Draw();

    for(int ipave=0;ipave<nEta;ipave++){
        pave2[ipave]->Draw();
    }

    pave3_1->Draw();
    pave3_2->Draw();
    pave3_3->Draw();

    TPaveText* pave2_1 = new TPaveText(0.7,0.95,0.8,0.98,"NDCNB");
    pave2_1->AddText("Decay Positrons");
	pave2_1->SetFillStyle(4000);
    pave2_1->SetTextFont(63);pave2_1->SetTextSize(30);
    pave2_1->SetTextColor(kMagenta);
    pave2_1->Draw();

    TPaveText* pave2_2 = new TPaveText(0.68,0.91,0.82,0.96,"NDCNB");
    pave2_2->AddText("Decay Electrons");
	pave2_2->SetFillStyle(4000);
    pave2_2->SetTextFont(63);pave2_2->SetTextSize(30);
    pave2_2->SetTextColor(kBlack);
    pave2_2->Draw();

    //Plot 3
    PadMxN *pad2x2_3 = new PadMxN("c3",500,500,150,150,100,125,2,2);
    pad2x2_3->Draw();
    TH2 *hframe_2 = new TH2F("hframe_2","",100,0.101,59.99,100,0.2,2e5);

    for(int iCan=0;iCan<nEtap;iCan++){
        pad2x2_3->GetPad(iCan+1);
        gPad->SetLogx();gPad->SetLogy();
        gPad->SetTickx();gPad->SetTicky();

        hframe_2->Draw();
        hframe_2->GetXaxis()->SetTitle("Momentum [GeV/c]");hframe_2->GetYaxis()->SetTitle("Yield");
		hframe_2->GetXaxis()->SetLabelFont(63);hframe_2->GetYaxis()->SetLabelFont(63);
      	hframe_2->GetXaxis()->SetLabelSize(25);hframe_2->GetYaxis()->SetLabelSize(25);
      	hframe_2->GetXaxis()->SetLabelOffset(0.01);hframe_2->GetYaxis()->SetLabelOffset(0.01);
      	hframe_2->GetXaxis()->CenterTitle(1);hframe_2->GetYaxis()->CenterTitle(1);
      	hframe_2->GetXaxis()->SetTitleSize(40);hframe_2->GetXaxis()->SetTitleOffset(2.5); 
      	hframe_2->GetYaxis()->SetTitleSize(40);hframe_2->GetYaxis()->SetTitleOffset(3.0);

        h_ppi[iCan]->Draw("same");
        h_pka[iCan]->Draw("same");
    }

    // get the text pad
    pad2x2_3->GetPad(5);

    pave1_1->Draw();
    pave3_2->Draw();
    pave3_3->Draw();

    TPaveText* pave3_4 = new TPaveText(0.89,0.08,0.94,0.12,"NDCNB");
    pave3_4->AddText("60");
	pave3_4->SetFillStyle(4000);
    pave3_4->SetTextFont(63);pave3_4->SetTextSize(25);
    pave3_4->Draw();

    TPaveText *pave4[nEtap];
    double p4_xl[] = {0.37,0.70,0.37,0.70};
    double p4_xh[] = {0.47,0.92,0.47,0.92};
    double p4_yl[] = {0.825,0.825,0.425,0.425};
    double p4_yh[] = {0.925,0.925,0.525,0.525};

    for(int ipave=0;ipave<nEtap;ipave++){
        pave4[ipave] = new TPaveText(p4_xl[ipave],p4_yl[ipave],p4_xh[ipave],p4_yh[ipave],"NDCNB");
        pave4[ipave]->AddText(Form("%.2f < #eta < %.2f",Etap_low[ipave],Etap_hi[ipave]));
	    pave4[ipave]->SetFillStyle(4000);
        pave4[ipave]->SetTextFont(63);pave4[ipave]->SetTextSize(35);
        pave4[ipave]->Draw();
    }

    TPaveText* pave4_1 = new TPaveText(0.7,0.95,0.75,0.98,"NDCNB");
    pave4_1->AddText("#pi^{+}");
	pave4_1->SetFillStyle(4000);
    pave4_1->SetTextFont(63);pave4_1->SetTextSize(30);
    pave4_1->SetTextColor(kBlue);
    pave4_1->Draw();

    TPaveText* pave4_2 = new TPaveText(0.75,0.95,0.80,0.98,"NDCNB");
    pave4_2->AddText("K^{+}");
	pave4_2->SetFillStyle(4000);
    pave4_2->SetTextFont(63);pave4_2->SetTextSize(30);
    pave4_2->SetTextColor(kGreen);
    pave4_2->Draw();

    //Print to file
    gROOT->ProcessLine("c1->Print(\"plots/particle_spectra.pdf[\");");
    gROOT->ProcessLine("c1->Print(\"plots/particle_spectra.pdf\");");
    gROOT->ProcessLine("c2->Print(\"plots/particle_spectra.pdf\");");
    gROOT->ProcessLine("c3->Print(\"plots/particle_spectra.pdf\");");
    gROOT->ProcessLine("c3->Print(\"plots/particle_spectra.pdf]\");");

    myapp->Run();
    return 0;
}
