

#include <stdio.h>
#include <stdlib.h>
#include <TString.h>
#include <TH2.h>
#include <TCanvas.h>
#include <TF1.h>

#include <fstream>
#include <string>
#include <iostream>

#include <fstream>
#include <string.h>

#include "TMath.h"
#include "TError.h"
#include "TVirtualFitter.h"
#include "TFile.h"
#include "TF1.h"
#include "TH1.h"
#include <math.h>



using namespace std;


void read_dati(){
    TVirtualFitter::SetMaxIterations(6000);
    
    // Read the raw data from the file .root and plot the histogram
    TFile *f = new TFile("dati.root");
    f->ls();
    TCanvas *c = new TCanvas("c", "Signal charge", 800, 600);
    TH1F *histo = (TH1F*)f->Get("h");
    histo->Draw();
    histo->GetXaxis()->SetTitle("charge [pC/4]");
    histo->GetYaxis()->SetTitle("counts");
    histo->GetYaxis()->SetRangeUser(0, 120);
    histo->GetXaxis()->SetRangeUser(-50, 370);
    
    // Gaussian function for baseline
    TF1 *f1 = new TF1("f1","[0]*Gaus(x, [1], [2])", -10, 15);
    f1->SetParameter(0, 70);
    f1->SetParameter(1, 0);
    f1->SetParameter(2, 3);
    histo->Fit("f1", "r");
    
    // Gaussian function for avalanche peak
    TF1 *f2 = new TF1("f2","[0]*Landau(x, [1], [2])", 20, 80);
    f2->SetParameter(0, 533);
    f2->SetParameter(1, 25);
    f2->SetParameter(2, 5.2);
    histo->Fit("f2", "r");
    
    // Gaussian function for streamer peak
    TF1 *f3 = new TF1("f3","[0]*Gaus(x, [1], [2])", 150, 350);
    f3->SetParameter(0, 10);
    f3->SetParameter(1, 250);
    f3->SetParameter(2, 25);
    histo->Fit("f3", "r");
    
    // Global function: that's the sum of the previous functions
    TF1 *f4 = new TF1("f4","[0]*Gaus(x, [1], [2]) + [3]*Landau(x, [4], [5]) + [6]*Gaus(x, [7], [8])", -100, 400);
    f4->SetParameter(0, f1->GetParameter(0));
    f4->FixParameter(1, f1->GetParameter(1)); // fixed parameter
    f4->SetParameter(2, f1->GetParameter(2));
    f4->SetParameter(3, f2->GetParameter(0));
    f4->SetParameter(4, f2->GetParameter(1));
    f4->SetParameter(5, f2->GetParameter(2));
    f4->SetParameter(6, f3->GetParameter(0));
    f4->SetParameter(7, f3->GetParameter(1));
    f4->SetParameter(8, f3->GetParameter(2));
    f4->SetNpx(1000);
    histo->Fit("f4", "re");
    
    
}

