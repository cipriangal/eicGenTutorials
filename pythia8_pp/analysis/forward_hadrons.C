// forward_hadrons.C
//
// Forward (2.5 < eta < 4.0) charged-hadron analysis of Pythia8 events that
// were written to HepMC3 and converted to a ROOT tree ("hepmc3_tree").
//
// Input file is built as:  <indir>/<energy>/RHIC_pp_<sample>.root
//
// Usage (interpreted):
//   root -l -b -q 'forward_hadrons.C("510GeV","hard_ptmin3")'
//   root -l -b -q 'forward_hadrons.C("510GeV","soft")'
//   root -l -b -q 'forward_hadrons.C("200GeV","soft","../out")'
// Usage (compiled with ACLiC -- noticeably faster for large files):
//   root -l -b -q 'forward_hadrons.C+("510GeV","soft")'
//
// Output: forward_hadrons_<energy>_<sample>.pdf
//
// sqrt(s) is taken event-by-event from the two beam particles (HepMC status 4).
// If they are not found, it falls back to the number at the start of <energy>
// (e.g. "510GeV" -> 510).

#include "TFile.h"
#include "TTree.h"
#include "TTreeReader.h"
#include "TTreeReaderArray.h"
#include "TH1D.h"
#include "TH2D.h"
#include "TCanvas.h"
#include "TLegend.h"
#include "TLatex.h"
#include "TLine.h"
#include "TStyle.h"
#include "TString.h"
#include "TAxis.h"

#include <array>
#include <cmath>
#include <cstdlib>
#include <initializer_list>
#include <iostream>

namespace {

// Species index used for all per-species histogram arrays
enum Species { kAll = 0, kPion, kKaon, kProton, kNSpecies };

const std::array<const char*, kNSpecies> kSpeciesTag   = {"h", "pi", "K", "p"};
const std::array<const char*, kNSpecies> kSpeciesLabel = {"h^{#pm}", "#pi^{#pm}", "K^{#pm}", "p/#bar{p}"};
const std::array<int, kNSpecies>         kSpeciesColor = {kBlack, kBlue, kRed, kGreen + 2};

// Returns pion/kaon/proton index for charged pi, K, p (and antiparticles), -1 otherwise
int speciesFromPid(int pid)
{
    switch (std::abs(pid)) {
        case 211:  return kPion;
        case 321:  return kKaon;
        case 2212: return kProton;
        default:   return -1;
    }
}

// Pseudorapidity from 3-momentum (no TLorentzVector construction needed)
double pseudorapidity(double px, double py, double pz)
{
    const double p = std::sqrt(px * px + py * py + pz * pz);
    if (p == std::abs(pz)) return pz >= 0 ? 1e10 : -1e10; // along the beam
    return 0.5 * std::log((p + pz) / (p - pz));
}

// Common histogram styling
template <class H>
H* style(H* h, const char* xtitle, const char* ytitle = nullptr, int color = kBlack)
{
    h->GetXaxis()->SetTitle(xtitle);
    h->GetXaxis()->CenterTitle();
    if (ytitle) {
        h->GetYaxis()->SetTitle(ytitle);
        h->GetYaxis()->CenterTitle();
    }
    h->SetLineColor(color);
    h->SetLineWidth(2);
    return h;
}

} // namespace

void forward_hadrons(TString energy = "510GeV",
                     TString sample = "hard_ptmin3",
                     TString indir  = "../out")
{
    //---------------------------------------------------------------- Style
    gStyle->SetOptStat(0);
    gStyle->SetPadBorderMode(0);
    gStyle->SetFrameBorderMode(0);
    gStyle->SetFrameLineWidth(2);
    gStyle->SetLabelSize(0.035, "X");
    gStyle->SetLabelSize(0.035, "Y");
    gStyle->SetTitleXSize(0.04);
    gStyle->SetTitleXOffset(0.9);
    gStyle->SetTitleYSize(0.04);
    gStyle->SetTitleYOffset(0.9);
    gStyle->SetPadRightMargin(0.13); // room for colz palettes

    //---------------------------------------------------------------- Settings
    const double eta_min = 2.5;   // forward acceptance
    const double eta_max = 4.0;
    const double fcs_z   = 725.0; // front face of FCS [cm]

    // Wider eta window for the Pt vs eta plots, so the forward acceptance
    // can be seen in context (marked with lines on the plot)
    const double eta_wide_min = -1.0;
    const double eta_wide_max = 6.0;

    //---------------------------------------------------------------- Open file
    const TString infile  = indir + "/" + energy + "/RHIC_pp_" + sample + ".root";
    const TString outfile = "forward_hadrons_" + energy + "_" + sample + ".pdf";

    TFile* f = TFile::Open(infile);
    if (!f || f->IsZombie()) {
        std::cerr << "ERROR: cannot open " << infile << std::endl;
        return;
    }
    TTree* tree = f->Get<TTree>("hepmc3_tree");
    if (!tree) {
        std::cerr << "ERROR: no hepmc3_tree in " << infile << std::endl;
        return;
    }
    std::cout << "Reading " << infile << " (" << tree->GetEntries() << " events)" << std::endl;

    // Fallback sqrt(s) from the folder name, e.g. "510GeV" -> 510
    const double sqrts_fallback = std::atof(energy.Data());

    //---------------------------------------------------------------- Histograms
    // Keep histograms out of the input file's directory so they survive f->Close()
    TH1::AddDirectory(kFALSE);

    const TString acc = Form("%.1f < #eta < %.1f", eta_min, eta_max);

    TH1D* h1 = style(new TH1D("h1", "Number of charged hadrons: " + acc, 20, 0, 20),
                     "Number of particles per event");

    TH1D* h4 = style(new TH1D("h4", "Highest P_{t} charged hadron in " + acc, 100, -0.2, 12),
                     "Hadron P_{t} [GeV/c]");

    TH2D* h5 = style(new TH2D("h5", "Charged hadrons projected to front of FCS",
                              100, -150, 150, 100, -150, 150),
                     "X position [cm]", "Y position [cm]");

    // Per-species histograms: index with kAll, kPion, kKaon, kProton
    std::array<TH1D*, kNSpecies> hPt{}, hEta{};
    std::array<TH2D*, kNSpecies> hPtEta{}, hPtXf{};

    for (int s = 0; s < kNSpecies; ++s) {
        const char* tag = kSpeciesTag[s];
        const char* lab = kSpeciesLabel[s];

        hPt[s] = style(new TH1D(Form("hPt_%s", tag), "Charged hadron spectrum: " + acc, 100, -0.2, 12),
                       "Hadron P_{t} [GeV/c]", nullptr, kSpeciesColor[s]);

        hEta[s] = style(new TH1D(Form("hEta_%s", tag), "Charged hadron spectrum: " + acc, 100, 2.4, 4.1),
                        "Hadron #eta", nullptr, kSpeciesColor[s]);

        // NEW: Pt vs eta (wide eta window)
        hPtEta[s] = style(new TH2D(Form("hPtEta_%s", tag), Form("%s: P_{t} vs. #eta", lab),
                                   140, eta_wide_min, eta_wide_max, 120, 0, 12),
                          "Hadron #eta", "Hadron P_{t} [GeV/c]");

        // NEW: Pt vs xF (forward acceptance)
        hPtXf[s] = style(new TH2D(Form("hPtXf_%s", tag), Form("%s in %s: P_{t} vs. x_{F}", lab, acc.Data()),
                                  100, 0, 1, 120, 0, 12),
                         "x_{F} = 2p_{z}/#sqrt{s}", "Hadron P_{t} [GeV/c]");
    }

    //---------------------------------------------------------------- Tree reader
    TTreeReader tr(tree);
    TTreeReaderArray<double> gen_px(tr, "particles.momentum.m_v1");
    TTreeReaderArray<double> gen_py(tr, "particles.momentum.m_v2");
    TTreeReaderArray<double> gen_pz(tr, "particles.momentum.m_v3");
    TTreeReaderArray<double> gen_E (tr, "particles.momentum.m_v4");
    TTreeReaderArray<int>    gen_pid(tr, "particles.pid");
    TTreeReaderArray<int>    gen_status(tr, "particles.status");

    //---------------------------------------------------------------- Event loop
    long counter = 0;
    double sqrts_first = -1; // first value found, for printing / labels
    bool warned_fallback = false;

    while (tr.Next()) {

        if (counter % 1000 == 0) std::cout << "Analyzing event " << counter << std::endl;
        counter++;

        const int npart = gen_px.GetSize();

        // --- sqrt(s) from the two beam particles (status 4)
        double bE = 0, bpx = 0, bpy = 0, bpz = 0;
        int nbeam = 0;
        for (int i = 0; i < npart && nbeam < 2; ++i) {
            if (gen_status[i] != 4) continue;
            bE += gen_E[i]; bpx += gen_px[i]; bpy += gen_py[i]; bpz += gen_pz[i];
            ++nbeam;
        }
        double sqrts = sqrts_fallback;
        if (nbeam == 2) {
            sqrts = std::sqrt(bE * bE - bpx * bpx - bpy * bpy - bpz * bpz);
        } else if (!warned_fallback) {
            std::cerr << "WARNING: beam particles not found; using sqrt(s) = "
                      << sqrts_fallback << " GeV from the folder name" << std::endl;
            warned_fallback = true;
        }
        if (sqrts_first < 0) {
            sqrts_first = sqrts;
            std::cout << "sqrt(s) = " << sqrts << " GeV" << std::endl;
        }

        // --- particle loop
        int num_fwd_hadrons = 0;
        double pt_max = -1;

        for (int i = 0; i < npart; ++i) {
            if (gen_status[i] != 1) continue;               // final state only
            const int sp = speciesFromPid(gen_pid[i]);
            if (sp < 0) continue;                           // charged pi/K/p only

            const double px = gen_px[i], py = gen_py[i], pz = gen_pz[i];
            const double pt  = std::hypot(px, py);
            const double eta = pseudorapidity(px, py, pz);

            // Pt vs eta over the wide window
            hPtEta[kAll]->Fill(eta, pt);
            hPtEta[sp]->Fill(eta, pt);

            if (eta <= eta_min || eta >= eta_max) continue; // forward acceptance

            const double xF = 2.0 * pz / sqrts;             // collider: lab = CM frame

            num_fwd_hadrons++;
            pt_max = std::max(pt, pt_max);

            for (int s : {static_cast<int>(kAll), sp}) {
                hPt[s]->Fill(pt);
                hEta[s]->Fill(eta);
                hPtXf[s]->Fill(xF, pt);
            }

            // Straight-line projection to the FCS (no magnetic field)
            h5->Fill(fcs_z * px / pz, fcs_z * py / pz);
        }

        h1->Fill(num_fwd_hadrons);
        if (num_fwd_hadrons > 0) h4->Fill(pt_max);
    }

    f->Close();
    std::cout << "Processed " << counter << " events" << std::endl;

    //---------------------------------------------------------------- Plots
    // Add sqrt(s) and sample name to every histogram title
    const TString runLabel = Form(" (#sqrt{s} = %.0f GeV, %s)", sqrts_first, sample.Data());
    for (TH1* h : std::initializer_list<TH1*>{h1, h4, h5}) h->SetTitle(TString(h->GetTitle()) + runLabel);
    for (int s = 0; s < kNSpecies; ++s) {
        for (TH1* h : std::initializer_list<TH1*>{hPt[s], hEta[s], hPtEta[s], hPtXf[s]})
            h->SetTitle(TString(h->GetTitle()) + runLabel);
        // Larger axis text for the 2x2 panels
        for (TH1* h : std::initializer_list<TH1*>{hPtEta[s], hPtXf[s]}) {
            for (TAxis* ax : {h->GetXaxis(), h->GetYaxis()}) {
                ax->SetTitleSize(0.05);
                ax->SetLabelSize(0.045);
            }
            h->GetXaxis()->SetTitleOffset(0.95);
            h->GetYaxis()->SetTitleOffset(0.85);
        }
    }

    auto makeLegend = [](const std::array<TH1D*, kNSpecies>& h) {
        TLegend* leg = new TLegend(0.7, 0.7, 0.85, 0.85);
        leg->SetBorderSize(0);
        for (int s = 0; s < kNSpecies; ++s) leg->AddEntry(h[s], kSpeciesLabel[s], "l");
        return leg;
    };

    TCanvas* c1 = new TCanvas("c1");
    h1->Draw();

    TCanvas* c2 = new TCanvas("c2");
    c2->SetLogy();
    for (int s = 0; s < kNSpecies; ++s) hPt[s]->Draw(s == 0 ? "" : "same");
    makeLegend(hPt)->Draw();

    TCanvas* c3 = new TCanvas("c3");
    for (int s = 0; s < kNSpecies; ++s) hEta[s]->Draw(s == 0 ? "" : "same");
    makeLegend(hEta)->Draw();

    TCanvas* c4 = new TCanvas("c4");
    c4->SetLogy();
    h4->Draw();

    TCanvas* c5 = new TCanvas("c5");
    h5->Draw("colz");
    {
        TLatex tex;
        tex.SetTextSize(0.035);
        tex.DrawLatex(-130, 135, acc);
        tex.DrawLatex(-130, 120, "Ignoring magnetic field");
    }

    // NEW: Pt vs eta, 2x2 (all, pi, K, p), forward acceptance marked
    TCanvas* c6 = new TCanvas("c6", "Pt vs eta", 1000, 800);
    c6->Divide(2, 2);
    for (int s = 0; s < kNSpecies; ++s) {
        c6->cd(s + 1);
        gPad->SetLogz();
        gPad->SetRightMargin(0.14);
        gPad->SetLeftMargin(0.12);
        gPad->SetBottomMargin(0.12);
        hPtEta[s]->Draw("colz");
        for (double e : {eta_min, eta_max}) {
            TLine* l = new TLine(e, 0, e, hPtEta[s]->GetYaxis()->GetXmax());
            l->SetLineStyle(2);
            l->SetLineWidth(2);
            l->Draw();
        }
    }

    // NEW: Pt vs xF, 2x2 (all, pi, K, p), forward acceptance only
    TCanvas* c7 = new TCanvas("c7", "Pt vs xF", 1000, 800);
    c7->Divide(2, 2);
    for (int s = 0; s < kNSpecies; ++s) {
        c7->cd(s + 1);
        gPad->SetLogz();
        gPad->SetRightMargin(0.14);
        gPad->SetLeftMargin(0.12);
        gPad->SetBottomMargin(0.12);
        hPtXf[s]->Draw("colz");
    }

    //---------------------------------------------------------------- Output
    TCanvas* canvases[] = {c1, c2, c3, c4, c5, c6, c7};
    canvases[0]->Print(outfile + "[");
    for (TCanvas* c : canvases) c->Print(outfile);
    canvases[0]->Print(outfile + "]");
    std::cout << "Wrote " << outfile << std::endl;
}
