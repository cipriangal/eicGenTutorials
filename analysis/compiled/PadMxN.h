#include "TH1D.h"
#include "TCanvas.h"
#include "TPad.h"
#include "TGaxis.h"
#include "TLatex.h"

class PadMxN{

  public:
    PadMxN(TString mcpadin = "padMxN", 
           float FigWidth = 500, float FigHeight = 500, float MargLeft = 150, float MargBot = 150, 
           float MargRig = 100, float MargTop = 100,
           int m = 4, int n = 2);
    ~PadMxN();

    void Draw();
    TPad* GetPad(int padID){ return (TPad*) toppad->cd(padID);}

  private:

    //  M A I N     C A N V A S 
    float FigureWidth; // Size of each figure 
    float FigureHeight;

    float MarginLeft; // Margins around the lattice of nx times ny pads 
    float MarginBottom;
    float MarginRight;
    float MarginTop;

    int nPadx;
    int nPady;

    TString mcpad;
    TCanvas *C;
    TVirtualPad *toppad;
};

PadMxN::PadMxN(TString mcpadin, float FigWidth, float FigHeight, float MargLeft, 
               float MargBot, float MargRig, float MargTop,
               int m, int n){
    mcpad        = mcpadin;

  FigureWidth  = FigWidth;
  FigureHeight = FigHeight;
  MarginLeft   = MargLeft;
  MarginBottom = MargBot; 
  MarginRight  = MargRig;
  MarginTop    = MargTop;
  nPadx = m;
  nPady = n;
}

PadMxN::~PadMxN(){
  if(C)      delete C;
  if(toppad) delete toppad;
}

void PadMxN::Draw(){ 
  gStyle->SetTitleFont(63,"X");
  gStyle->SetTitleFont(63,"Y");
  gStyle->SetLabelFont(63,"X");
  gStyle->SetLabelFont(63,"Y");
  gStyle->SetTextFont(63);

  gStyle->SetStatStyle(0);
  gStyle->SetTitleStyle(0);
  gStyle->SetCanvasBorderSize(0);
  gStyle->SetFrameBorderSize(0);
  gStyle->SetLegendBorderSize(0);
  gStyle->SetStatBorderSize(0);
  gStyle->SetTitleBorderSize(0);

  gStyle->SetStatStyle(0);
  gStyle->SetTitleStyle(0);
  gStyle->SetCanvasBorderSize(0);
  gStyle->SetFrameBorderSize(0);
  gStyle->SetLegendBorderSize(0);
  gStyle->SetStatBorderSize(0);
  gStyle->SetTitleBorderSize(0);
  gStyle->SetOptStat(0);
  gStyle->SetFillStyle(4000);
  gStyle->SetStatStyle(0);
  gStyle->SetFrameFillColor(0);
  gStyle->SetLegendBorderSize(0);

  int xpads = nPadx; int ypads = nPady;
  float canvas_x = xpads*FigureWidth+MarginLeft+MarginRight;
  float canvas_y = ypads*FigureHeight+MarginTop+MarginBottom;
  C = new TCanvas(mcpad, mcpad, canvas_x, canvas_y); // the main canvas
  C->Range(0,0,1,1);
  C->SetFillStyle(4000); C->SetFillColor(10);
  gStyle->SetOptStat(0); gStyle->SetOptTitle(0);
  C->SetTopMargin(0.); C->SetBottomMargin(0.);//our pads will have no margin
  C->SetLeftMargin(0.);C->SetRightMargin(0.);
  // C->cd();

  toppad = gPad;
  toppad->Clear();
  
  std::vector<std::vector<TPad*>> pad;
  pad.resize(xpads,std::vector<TPad*>(ypads));

  std::vector<std::vector<double>> pad_x1;
  pad_x1.resize(xpads,std::vector<double>(ypads));

  std::vector<std::vector<double>> pad_x2;
  pad_x2.resize(xpads,std::vector<double>(ypads));
  
  std::vector<std::vector<double>> pad_y1;
  pad_y1.resize(xpads,std::vector<double>(ypads));

  std::vector<std::vector<double>> pad_y2;
  pad_y2.resize(xpads,std::vector<double>(ypads));
  
  int pad_counter = 0;
  
  for (int iypad = 0; iypad < ypads; ++iypad)
  {
    for (int ixpad = 0; ixpad < xpads; ++ixpad)
    {
      pad_counter++;
      if (ixpad==0)
      {
        pad_x1[ixpad][iypad] = 0;
        pad_x2[ixpad][iypad] = pad_x1[ixpad][iypad]+FigureWidth+MarginLeft;
      }
      else if (ixpad!=xpads-1)
      {
        pad_x1[ixpad][iypad] = pad_x2[ixpad-1][iypad];
        pad_x2[ixpad][iypad] = pad_x1[ixpad][iypad]+FigureWidth;
      }
      else
      {
        pad_x1[ixpad][iypad] = pad_x2[ixpad-1][iypad];
        pad_x2[ixpad][iypad] = canvas_x;
      }

      if (iypad==0)
      {
        pad_y2[ixpad][iypad] = canvas_y;
        pad_y1[ixpad][iypad] = pad_y2[ixpad][iypad]-FigureHeight-MarginTop;
      }
      else if (iypad!=ypads-1)
      {
        pad_y2[ixpad][iypad] = pad_y1[ixpad][iypad-1];
        pad_y1[ixpad][iypad] = pad_y2[ixpad][iypad]-FigureHeight;
      }
      else
      {
        pad_y2[ixpad][iypad] = pad_y1[ixpad][iypad-1];
        pad_y1[ixpad][iypad] = 0;
      }

      // set pad position
      pad[ixpad][iypad] = new TPad(Form("Pad%d",pad_counter),Form("Pad%d",pad_counter),pad_x1[ixpad][iypad]/canvas_x,pad_y1[ixpad][iypad]/canvas_y,pad_x2[ixpad][iypad]/canvas_x,pad_y2[ixpad][iypad]/canvas_y);
      pad[ixpad][iypad]->SetNumber(pad_counter); // assign a number to it. Possible to access it via :  toppad->cd(ih);

      pad[ixpad][iypad]->SetBorderMode(0);
      pad[ixpad][iypad]->SetBorderSize(0);
      pad[ixpad][iypad]->SetTicks(0,1);

      // set pad margins
      pad[ixpad][iypad]->SetLeftMargin(0.);
      pad[ixpad][iypad]->SetTopMargin(0.);
      pad[ixpad][iypad]->SetRightMargin(0.);
      pad[ixpad][iypad]->SetBottomMargin(0.);
      if (ixpad==0) pad[ixpad][iypad]->SetLeftMargin(MarginLeft/(pad_x2[ixpad][iypad]-pad_x1[ixpad][iypad]));
      if (ixpad==xpads-1) pad[ixpad][iypad]->SetRightMargin(MarginRight/(pad_x2[ixpad][iypad]-pad_x1[ixpad][iypad]));
      if (iypad==0) pad[ixpad][iypad]->SetTopMargin(MarginTop/(pad_y2[ixpad][iypad]-pad_y1[ixpad][iypad]));
      if (iypad==ypads-1) pad[ixpad][iypad]->SetBottomMargin(MarginBottom/(pad_y2[ixpad][iypad]-pad_y1[ixpad][iypad]));

      pad[ixpad][iypad]->Draw();
    }
  }

  pad_counter++;
  TPad* textpad = new TPad(Form("Pad%d",pad_counter),"textpad",0.0,0.0,1.0,1.0);
  textpad->SetNumber(pad_counter);
  textpad->SetBorderMode(0);
  textpad->SetBorderSize(0);
  textpad->SetFillStyle(4000);

  textpad->SetLeftMargin(MarginLeft/canvas_x);
  textpad->SetTopMargin(0.);
  textpad->SetRightMargin(MarginRight/canvas_y);
  textpad->SetBottomMargin(0.);

  textpad->Draw();
}
