//
// Created by Gabriele Gaetano Fronzé on 19/09/2017.
//

#include <TRandom.h>
#include "TROOT.h"
#include "TH1D.h"
#include "TFile.h"
#include "TMath.h"
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

void PerformRealSystFit2(){

    TVirtualFitter::SetMaxIterations(6000);
    Double_t par[12] ={2.37707e+05,-1.01476e-07,-6.52638e-01,1.60290e-08, 2.48902e+04,3.09421e+00,8.78494e-02, 7.18255e-01,8.40892e-01,2.07705e+00,-2.44212e+05, 0.};
    // Fit for background
    TFile *f = new TFile("SumHistos_LHC16r.root");
    TCanvas *c_bk = new TCanvas("c_bk", "Background", 800, 600);
    TH1F *histo_bk = (TH1F*)f->Get("hMassOS_2m");
    for (float i = 2.9; i <= 3.25; i+=0.001){
        histo_bk->SetBinContent(histo_bk->FindBin(i), 0);
    }
    histo_bk->Draw();
    histo_bk->GetXaxis()->SetTitle("m_{#mu^{+}#mu^{-}} [GeV/c^{2}]");
    histo_bk->GetYaxis()->SetTitle("#Deltan / #Deltam [(GeV/c^{2})^{-1}]");
    histo_bk->GetXaxis()->SetRangeUser(2, 5);
    histo_bk->GetYaxis()->SetRangeUser(1, 30000);
    TF1 *f_bk = new TF1("f_bk", "expo(0)*expo(0)", 2, 5);
    f_bk->SetParameter(0,14.);
    f_bk->SetParameter(1,-2.);
    histo_bk->Fit(f_bk, "R");


    // Fit for background
    TFile *file_peak = new TFile("SumHistos_LHC16r.root");
    TCanvas *c_peak = new TCanvas("c_peak", "Jpsi", 800, 600);
    TH1F *histo_pk = (TH1F*)file_peak->Get("hMassOS_2m");
    histo_pk->Draw();
    histo_pk->GetXaxis()->SetTitle("m_{#mu^{+}#mu^{-}} [GeV/c^{2}]");
    histo_pk->GetYaxis()->SetTitle("#Deltan / #Deltam [(GeV/c^{2})^{-1}]");
    histo_pk->GetXaxis()->SetRangeUser(2, 5);
    histo_pk->GetYaxis()->SetRangeUser(1, 30000);
    TF1 *f_peak = new TF1("f_peak", "gaus(0)", 2.98, 3.20);
    f_peak->SetParameter(0, 2.48103e+04);
    f_peak->SetParameter(1, 3.08974e+00);
    f_peak->SetParameter(2, 9.13342e-02);
    histo_pk->Fit(f_peak, "REI");

    // Fit all the spectrum
    TFile *file_tot = new TFile("SumHistos_LHC16r.root");
    TCanvas *c_tot = new TCanvas("c_tot", "Jpsi", 800, 600);
    TH1F *histo_tot = (TH1F*)file_tot->Get("hMassOS_2m");
    histo_tot->Draw();
    histo_tot->GetXaxis()->SetTitle("m_{#mu^{+}#mu^{-}} [GeV/c^{2}]");
    histo_tot->GetYaxis()->SetTitle("#Deltan / #Deltam [(GeV/c^{2})^{-1}]");
    histo_tot->GetXaxis()->SetRangeUser(2, 5);
    histo_tot->GetYaxis()->SetRangeUser(1, 30000);
    TF1 *f_tot = new TF1("f_tot", "expo(0)*expo(0) + gaus(2)", 2, 5);
    for (int k = 0; k < 1; k++){
        f_tot->SetParameter(k, f_bk->GetParameter(k));
    }
    for (int k = 1; k < 2; k++){
        f_tot->SetParameter(k, f_bk->GetParameter(k));
    }
    for (int j = 0; j < 1; j++){
        f_tot->SetParameter(j+2, f_peak->GetParameter(j)*10.);
    }
    for (int j = 1; j < 3; j++){
        f_tot->FixParameter(j+2, f_peak->GetParameter(j));
    }
    f_tot->FixParameter(3, 3.08974e+00);
    f_tot->FixParameter(4, 9.13342e-02);
    histo_tot->Fit(f_tot, "REI");

    std::vector<int> nSamples = {20,20,10,0,0};
    auto *systFitSettings = new TSystFitSettings(*f_tot,nSamples);

    cout<<"A total of "<<systFitSettings->GetNConfigurations()<<" configurations will be tested"<<endl;
    systFitSettings->GenerateConfigurations();

//    return;

    auto *systFitter = new TSystFitter(histo_tot);
    systFitter->SetSystFitSettings(systFitSettings);

    systFitter->SystFit(f_tot,"sreq","",2.,5.);

    systFitter->PrintResults(new TCanvas("canv","canv"));
}