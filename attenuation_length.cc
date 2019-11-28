#ifndef __CINT__
#include "TApplication.h"
#include "TAxis.h"
#include "TCanvas.h"
#include "TGraph.h"
#include "TH1.h"
#include "TROOT.h"
#include "TStyle.h"
#include "TMath.h"
#endif
#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
using namespace std;

void attenuation_length()
{
  //---------------------------------------------------------------------//
  // material
#if 1
  TString fin_name = "file/H2O.txt";
  const Double_t DENSITY = 1.0;
  const Double_t YMAX = 50;
  TString title = "H_{2}O (#rho = 1.0 g/cm^{3})";
#endif
#if 0
  TString fin_name = "file/C18H30.txt";
  const Double_t DENSITY = 0.852;
  const Double_t YMAX = 60;
  TString title = "C_{18}H_{30} (#rho = 0.852 g/cm^{3})";
#endif
#if 0
  TString fin_name = "file/Zn.txt";
  const Double_t DENSITY = 11.3;
  const Double_t YMAX = 3;
  TString title = "Zn (#rho = 11.3 g/cm^{3})";
#endif
#if 0
  TString fin_name = "file/NaI.txt";
  const Double_t DENSITY = 3.67;
  const Double_t YMAX = 10;
  TString title = "NaI (#rho = 3.67 g/cm^{3})";
#endif
  //---------------------------------------------------------------------//

  // set canvas style
  //
  gStyle -> SetLabelSize(0.05, "XYZ");
  gStyle -> SetTitleSize(0.06, "XYZ");
  gStyle -> SetTitleOffset(1.0, "X");
  gStyle -> SetTitleOffset(1.1, "Y");
  gStyle -> SetPadLeftMargin(0.14);
  gStyle -> SetPadBottomMargin(0.14);

  TCanvas *c1 = new TCanvas("c1", "", 800, 600);
  c1 -> SetFillStyle(4001);
  c1 -> SetGridx();
  c1 -> SetGridy();

  // read file
  //
  ifstream fin(fin_name.Data());
  if (!fin) {
    cerr << "Error: don't exist such a data file, " << fin_name << " !" << endl;
    exit(-1);
  }

  const Int_t A = 10000;
  Double_t x[A], y[A];
  Double_t PhotonEnergy, ScatteringWithCoherent, ScatteringWithIncoherent, PhotoelectricAbsorption, PairProductionInNuclearField, PairProductionInElectronField, TotalAttenuationWithCoherentScattering, TotalAttenuationWithoutCoherentScattering;
  Int_t N = 0;
  string line;
  while (getline(fin, line)) {
    if (line.empty() || (line[0] == '#')) {
      continue;
    }

    if (!(istringstream(line) >> PhotonEnergy >> ScatteringWithCoherent >> ScatteringWithIncoherent >> PhotoelectricAbsorption >> PairProductionInNuclearField >> PairProductionInElectronField >> TotalAttenuationWithCoherentScattering >> TotalAttenuationWithoutCoherentScattering)) {
      cerr << "Error: badly formatted data line: " << line << endl;
      exit(8);
    }

    x[N] = PhotonEnergy; // MeV
    y[N] = 1.0 / (TotalAttenuationWithCoherentScattering * DENSITY);
    //    y[N] = 1.0 / (PhotoelectricAbsorption * DENSITY);
    //    cout << setw(2) << N << "  " << setw(6) << x[N] << "  " << setw(9) << y[N] << endl;

    N++;
  }

  // draw graph
  //
  TGraph* graph = new TGraph(N, x, y);
  graph -> SetLineColor(4);
  graph -> SetLineWidth(3);
  graph -> Draw("AL");

  const Double_t XMIN = 0;
  const Double_t XMAX = 10;
  const Double_t YMIN = 0;
  graph -> GetXaxis() -> SetLimits(XMIN, XMAX);
  graph -> GetYaxis() -> SetRangeUser(YMIN, YMAX);
  //  graph -> SetTitle("");
  graph -> SetTitle(title.Data());
  graph -> GetXaxis() -> SetTitle("E_{#gamma} [MeV]");
  graph -> GetYaxis() -> SetTitle("Attenuation Length [cm]");

  // make plot
  //
  c1 -> Print("plot_attenuation_length.pdf");
}

#ifndef __CINT__
Int_t main(Int_t argc, char** argv) {
  TApplication app("App", &argc, argv);
  attenuation_length();
  cerr << "Finished." << endl;
  app.Run();
  return 0;
}
#endif
