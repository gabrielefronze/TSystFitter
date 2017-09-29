//
// Created by Gabriele Gaetano Fronzé on 19/09/2017.
//

#include <TRandom.h>
#include "TROOT.h"
#include "TH1D.h"
#include "TFile.h"
#include "TSystFitParameter.h"
#include "TSystFitSettings.h"
#include "TSystFitter.h"
#include "TFitResult.h"
#include "TFitResultPtr.h"
#include "TVirtualFitter.h"
#include "TCanvas.h"
#include <vector>
#include <iostream>

using namespace std;

void PerformRealSystFit(){

    TVirtualFitter::SetMaxIterations( 20000 );

    auto canv = new TCanvas("canv","canv");

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

    std::vector<int> nSamples = {0,0,0,0,7,7,0,7,7};
    auto *systFitSettings = new TSystFitSettings(*f4,nSamples);

//    cout<<"Settings initialized"<<endl;
//
//    systFitSettings->AddParameter(TSystFitParameter(ParamValue(20.,10.,30.),1));
//
//    cout<<"Par0 initialized with "<<systFitSettings->GetParameter(0).GetNValues()<<" values"<<endl;
//
//    systFitSettings->AddParameter(TSystFitParameter(ParamValue(17.,5.,25.),5));
//
//    cout<<"Par1 initialized with "<<systFitSettings->GetParameter(1).GetNValues()<<" values"<<endl;
//
//    systFitSettings->AddParameter(TSystFitParameter(new TF1("fa1","sin(x)/x",0.,50.),5));
//
//    cout<<"Par2 initialized with "<<systFitSettings->GetParameter(2).GetNValues()<<" values"<<endl;
//
//    Double_t par3Values[3] = {1.,0.,2.};
//    systFitSettings->AddParameter(TSystFitParameter(ParamValue(par3Values),5));
//
//    cout<<"Par3 initialized with "<<systFitSettings->GetParameter(3).GetNValues()<<" values"<<endl;

    cout<<"A total of "<<systFitSettings->GetNConfigurations()<<" configurations will be tested"<<endl;
    systFitSettings->GenerateConfigurations();

//    return;

    auto *systFitter = new TSystFitter(histo);
    systFitter->SetSystFitSettings(systFitSettings);

    systFitter->SystFit(f4,"srliq","",-100.,400.);

    systFitter->PrintResults(canv);
}