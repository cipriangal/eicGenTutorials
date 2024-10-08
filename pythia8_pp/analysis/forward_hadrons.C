void forward_hadrons(){

    //Define Style
    gStyle->SetOptStat(0);
    gStyle->SetPadBorderMode(0);
    gStyle->SetFrameBorderMode(0);
    gStyle->SetFrameLineWidth(2);
    gStyle->SetLabelSize(0.035,"X");
    gStyle->SetLabelSize(0.035,"Y");
    //gStyle->SetLabelOffset(0.01,"X");
    //gStyle->SetLabelOffset(0.01,"Y");
    gStyle->SetTitleXSize(0.04);
    gStyle->SetTitleXOffset(0.9);
    gStyle->SetTitleYSize(0.04);
    gStyle->SetTitleYOffset(0.9);

    //Define histograms
    TH1 *h1 = new TH1D("h1","Number of charged hadrons: 2.5 < #eta < 4.0",20,0,20);
    h1->GetXaxis()->SetTitle("Number of particles per event");h1->GetXaxis()->CenterTitle();
    //h1->GetXaxis()->CenterLabels();
    h1->SetLineColor(kBlack);h1->SetLineWidth(2);

    TH1 *h2 = new TH1D("h2","Charged hadron spectrum: 2.5 < #eta < 4.0",100,-0.2,12);
    h2->GetXaxis()->SetTitle("Hadron P_{t} [GeV/c]");h2->GetXaxis()->CenterTitle();
    h2->SetLineColor(kBlack);h2->SetLineWidth(2);

    TH1 *h2a = new TH1D("h2a","Charged hadron spectrum: 2.5 < #eta < 4.0",100,-0.2,12); //Charged pions
    h2a->GetXaxis()->SetTitle("Hadron P_{t} [GeV/c]");h2a->GetXaxis()->CenterTitle();
    h2a->SetLineColor(kBlue);h2a->SetLineWidth(2);

    TH1 *h2b = new TH1D("h2b","Charged hadron spectrum: 2.5 < #eta < 4.0",100,-0.2,12); //Charged kaons
    h2b->GetXaxis()->SetTitle("Hadron P_{t} [GeV/c]");h2b->GetXaxis()->CenterTitle();
    h2b->SetLineColor(kRed);h2b->SetLineWidth(2);

    TH1 *h2c = new TH1D("h2c","Charged hadron spectrum: 2.5 < #eta < 4.0",100,-0.2,12); //(Anti-)Protons
    h2c->GetXaxis()->SetTitle("Hadron P_{t} [GeV/c]");h2->GetXaxis()->CenterTitle();
    h2c->SetLineColor(kGreen);h2c->SetLineWidth(2); 

    TH1 *h3 = new TH1D("h3","Charged hadron spectrum: 2.5 < #eta < 4.0",100,2.4,4.1);
    h3->GetXaxis()->SetTitle("Hadron #eta");h3->GetXaxis()->CenterTitle();
    h3->SetLineColor(kBlack);h3->SetLineWidth(2);

    TH1 *h3a = new TH1D("h3a","Charged hadron spectrum: 2.5 < #eta < 4.0",100,2.4,4.1); //Charged pions
    h3a->GetXaxis()->SetTitle("Hadron P_{t} [GeV/c]");h3a->GetXaxis()->CenterTitle();
    h3a->SetLineColor(kBlue);h3a->SetLineWidth(2);

    TH1 *h3b = new TH1D("h3b","Charged hadron spectrum: 2.5 < #eta < 4.0",100,2.4,4.1); //Charged kaons
    h3b->GetXaxis()->SetTitle("Hadron P_{t} [GeV/c]");h3b->GetXaxis()->CenterTitle();
    h3b->SetLineColor(kRed);h3b->SetLineWidth(2);

    TH1 *h3c = new TH1D("h3c","Charged hadron spectrum: 2.5 < #eta < 4.0",100,2.4,4.1); //(Anti-)Protons
    h3c->GetXaxis()->SetTitle("Hadron P_{t} [GeV/c]");h3->GetXaxis()->CenterTitle();
    h3c->SetLineColor(kGreen);h3c->SetLineWidth(2);

    TH1 *h4 = new TH1D("h4","Highest P_{t} Charged hadron in 2.5 < #eta < 4.0",100,-0.2,12);
    h4->GetXaxis()->SetTitle("Hadron P_{t} [GeV/c]");h4->GetXaxis()->CenterTitle();
    h4->SetLineColor(kBlack);h4->SetLineWidth(2);

    TH2 *h5 = new TH2D("h5","Charged hadrons projected to front of FCS",100,-150,150,100,-150,150);
    h5->GetXaxis()->SetTitle("X position [cm]");h5->GetXaxis()->CenterTitle();
    h5->GetYaxis()->SetTitle("Y position [cm]");h5->GetYaxis()->CenterTitle();

    //Open file
    TFile *f = new TFile("../out/510GeV/RHIC_pp_hard_ptmin3.root");
    TTree *tree = (TTree*) f->Get("hepmc3_tree");

    //Create Array Reader
    TTreeReader tr(tree);

    TTreeReaderArray<double> gen_px(tr, "particles.momentum.m_v1");
    TTreeReaderArray<double> gen_py(tr, "particles.momentum.m_v2");
    TTreeReaderArray<double> gen_pz(tr, "particles.momentum.m_v3");
    TTreeReaderArray<double> gen_E(tr, "particles.momentum.m_v4");
    TTreeReaderArray<int> gen_pid(tr, "particles.pid");
    TTreeReaderArray<int> gen_status(tr, "particles.status");

    //Other variables
    int counter(0);
    const int pion_id = 211; //pi+ pdg id
    const int kaon_id = 321; //K+ pdg id
    const int prot_id = 2212; //Proton pdg id
    const double fcs_z = 725.0; //Front face of FCS (in cm)

    //TDatabasePDG *pdg = new TDatabasePDG();

    //Loop over events
    while (tr.Next()) {

	//Reset variables
	int num_fwd_hadrons = 0;
	double pt_max = -9999;

	if(counter%1000==0) cout<<"Analyzing event "<<counter<<endl;
	counter++;

        //Loop over generated particles
        for(int igen=0;igen<gen_px.GetSize();igen++){
		if(gen_status[igen]==1){
	    		//Get particle information from database
	    		//TParticlePDG *particle = pdg->GetParticle(gen_pid[igen]);
			//double charge = particle->Charge();

			//See if particle is a charged hadron
			bool charged_pion = false;
			if( fabs(gen_pid[igen])==pion_id ) charged_pion = true;

			bool charged_kaon = false;
			if( fabs(gen_pid[igen])==kaon_id ) charged_kaon = true;

			bool charged_prot = false;
			if( fabs(gen_pid[igen])==prot_id ) charged_prot = true;

			//Particle 4-momentum
            		TLorentzVector particle_vec(gen_px[igen],gen_py[igen],gen_pz[igen],gen_E[igen]);

			//Look for charged hadrons in forward rapidities
			if( particle_vec.Eta()>2.5 && particle_vec.Eta()<4.0 && 
			    (charged_pion || charged_kaon || charged_prot) ){
				
				num_fwd_hadrons++;
				h2->Fill(particle_vec.Pt());
				h3->Fill(particle_vec.Eta());

				if(charged_pion){
					h2a->Fill(particle_vec.Pt());
                                	h3a->Fill(particle_vec.Eta());
				}
				else if(charged_kaon){
                                        h2b->Fill(particle_vec.Pt());
                                        h3b->Fill(particle_vec.Eta());
                                }
				else if(charged_prot){
                                        h2c->Fill(particle_vec.Pt());
                                        h3c->Fill(particle_vec.Eta());
                                }

				//Find highest Pt charged hadron in 2.5 < eta < 4.0
				pt_max = std::max(particle_vec.Pt(),pt_max);

				//Project to FCS
				auto pos_x = fcs_z * ( particle_vec.Px() / particle_vec.Pz() );
				auto pos_y = fcs_z * ( particle_vec.Py() / particle_vec.Pz() );
				h5->Fill(pos_x,pos_y);	

			}//End if charged-hadrons
		} //End if final-state particles
        } //End loop over generated particles

	//Fill event-level histograms
	h1->Fill(num_fwd_hadrons);
	h4->Fill(pt_max);

    } // End loop over events

    //Make plots
    TCanvas *c1 = new TCanvas("c1");
    h1->Draw("");

    TCanvas *c2 = new TCanvas("c2");
    c2->SetLogy();
    h2->Draw("");
    h2a->Draw("same");h2b->Draw("same");h2c->Draw("same");

    TLegend *legc2 = new TLegend(0.7,0.7,0.85,0.85);
    legc2->SetBorderSize(0);
    legc2->AddEntry("h2","h^{#pm}","l");
    legc2->AddEntry("h2a","#pi^{#pm}","l");
    legc2->AddEntry("h2b","K^{#pm}","l");
    legc2->AddEntry("h2c","p^{#pm}","l");
    legc2->Draw();

    TCanvas *c3 = new TCanvas("c3");
    //c3->SetLogy();
    h3->Draw();
    h3a->Draw("same");h3b->Draw("same");h3c->Draw("same");

    TLegend *legc3 = new TLegend(0.7,0.7,0.85,0.85);
    legc3->SetBorderSize(0);
    legc3->AddEntry("h3","h^{#pm}","l");
    legc3->AddEntry("h3a","#pi^{#pm}","l");
    legc3->AddEntry("h3b","K^{#pm}","l");
    legc3->AddEntry("h3c","p^{#pm}","l");
    legc3->Draw();

    TCanvas *c4 = new TCanvas("c4");
    c4->SetLogy();
    h4->Draw("");

    TCanvas *c5 = new TCanvas("c5");
    h5->Draw("colz");

    TLatex *tex_c51 = new TLatex();
    tex_c51->SetTextSize(0.035);
    tex_c51->DrawLatex(-130,135,"2.5 < #eta < 4.0");
    tex_c51->DrawLatex(-130,120,"Ignoring magnetic field");

    //Print plots to file
    c1->Print("forward_hadrons.pdf[");
    c1->Print("forward_hadrons.pdf");
    c2->Print("forward_hadrons.pdf");
    c3->Print("forward_hadrons.pdf");
    c4->Print("forward_hadrons.pdf");
    c5->Print("forward_hadrons.pdf");
    c5->Print("forward_hadrons.pdf]");

}
