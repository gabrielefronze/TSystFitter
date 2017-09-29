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


Double_t FuncBck(Double_t *x, Double_t *par){
    Double_t sigma = par[2]+par[3]*((x[0]-par[1])/par[1]);
    Double_t FitBck = par[0]*TMath::Exp(-(x[0]-par[1])*(x[0]-par[1])/(2.*sigma*sigma));
    return FitBck;
}

Double_t FuncJpsi(Double_t *x, Double_t *par){
    //par[1] = mean; par[2] = sigma; par[3] = alpha; par[4] = n; par[0] = Normalization; par[5] = alpha2; par[6] = n2;
    Double_t t = (x[0]-par[5])/par[6];
    if (par[7] < 0) t = -t;
    Double_t absAlpha = fabs((Double_t)par[7]);
    Double_t absAlpha2 = fabs((Double_t)par[9]);
    if (t >= -absAlpha && t < absAlpha2){ // gaussian core
        return par[4]*(exp(-0.5*t*t));
    }
    if (t < -absAlpha){ // left tail
        Double_t a = TMath::Power(par[8]/absAlpha,par[8])*exp(-0.5*absAlpha*absAlpha);
        Double_t b = par[8]/absAlpha - absAlpha;
        return par[4]*(a/TMath::Power(b - t, par[8]));
    }
    if (t >= absAlpha2){ // right tail
        Double_t c = TMath::Power(par[10]/absAlpha2,par[10])*exp(-0.5*absAlpha2*absAlpha2);
        Double_t d = par[10]/absAlpha2 - absAlpha2;
        return  par[4]*(c/TMath::Power(d + t, par[10]));
    }
    return 0.;
}


Double_t FuncTot(Double_t *x, Double_t *par){
    Double_t sigma = par[2]+par[3]*((x[0]-par[1])/par[1]);
    Double_t FitBck = par[0]*TMath::Exp(-(x[0]-par[1])*(x[0]-par[1])/(2.*sigma*sigma));
    Double_t t = (x[0]-par[5])/par[6];
    if (par[7] < 0) t = -t;
    Double_t absAlpha = fabs((Double_t)par[7]);
    Double_t absAlpha2 = fabs((Double_t)par[9]);
    if (t >= -absAlpha && t < absAlpha2){
        return par[4]*(exp(-0.5*t*t)) + FitBck;
    }
    if (t < -absAlpha){
        Double_t a = TMath::Power(par[8]/absAlpha,par[8])*exp(-0.5*absAlpha*absAlpha);
        Double_t b = par[8]/absAlpha - absAlpha;
        return par[4]*(a/TMath::Power(b - t, par[8])) + FitBck;
    }
    if (t >= absAlpha2){
        Double_t c = TMath::Power(par[10]/absAlpha2,par[10])*exp(-0.5*absAlpha2*absAlpha2);
        Double_t d = par[10]/absAlpha2 - absAlpha2;
        return  par[4]*(c/TMath::Power(d + t, par[10])) + FitBck;
    }
    return 0.;
}

void PerformRealSystFit2(){

    TVirtualFitter::SetMaxIterations( 20000 );

    Double_t par[12] ={2.37707e+05,-1.01476e-07,-6.52638e-01,1.60290e-08, 2.48902e+04,3.09421e+00,8.78494e-02, 7.18255e-01,8.40892e-01,2.07705e+00,-2.44212e+05, 0.};
    // Fit for background
    TFile *f = new TFile("SumHistos_LHC16r.root");
//    TCanvas *c_bk = new TCanvas("c_bk", "Background", 800, 600);
    TH1F *histo_bk = (TH1F*)f->Get("hMassOS_2m");
    for (float i = 2.9; i <= 3.25; i+=0.001){
        histo_bk->SetBinContent(histo_bk->FindBin(i), 0);
    }
    histo_bk->Draw();
    histo_bk->GetXaxis()->SetTitle("m_{#mu^{+}#mu^{-}} [GeV/c^{2}]");
    histo_bk->GetYaxis()->SetTitle("#Deltan / #Deltam [(GeV/c^{2})^{-1}]");
    histo_bk->GetXaxis()->SetRangeUser(2, 5);
    histo_bk->GetYaxis()->SetRangeUser(1, 30000);
    TF1 *f_bk = new TF1("f_bk", FuncBck, 2, 5, 4);
    for (int n_par = 0; n_par  < 4; n_par ++){
        f_bk->SetParameter(n_par,par[n_par]);
    }
    histo_bk->Fit(f_bk, "RL");

    // Fit for peak
    TFile *file_peak = new TFile("SumHistos_LHC16r.root");
//    TCanvas *c_peak = new TCanvas("c_peak", "Jpsi", 800, 600);
    TH1F *histo_pk = (TH1F*)file_peak->Get("hMassOS_2m");
//    histo_pk->Draw();
    histo_pk->GetXaxis()->SetTitle("m_{#mu^{+}#mu^{-}} [GeV/c^{2}]");
    histo_pk->GetYaxis()->SetTitle("#Deltan / #Deltam [(GeV/c^{2})^{-1}]");
    histo_pk->GetXaxis()->SetRangeUser(2, 5);
    histo_pk->GetYaxis()->SetRangeUser(1, 30000);
    TF1 *f_peak = new TF1("f_peak", FuncJpsi, 2.9, 3.25, 11);
    for (int n_par = 0; n_par < 4; n_par++){
        f_peak->FixParameter(n_par,f_bk->GetParameter(n_par));
    }
    for (int n_par = 4; n_par < 7; n_par++){
        f_peak->SetParameter(n_par,par[n_par]);
    }
    for (int n_par = 7; n_par < 12; n_par++){
        f_peak->SetParameter(n_par,par[n_par]);
    }
    histo_pk->Fit(f_peak, "R");

    // Fit all the spectrum
    TFile *file_tot = new TFile("SumHistos_LHC16r.root");
//    TCanvas *c_tot = new TCanvas("c_tot", "Jpsi", 800, 600);
    TH1F *histo_tot = (TH1F*)file_tot->Get("hMassOS_2m");
//    histo_tot->Draw();
    histo_tot->GetXaxis()->SetTitle("m_{#mu^{+}#mu^{-}} [GeV/c^{2}]");
    histo_tot->GetYaxis()->SetTitle("#Deltan / #Deltam [(GeV/c^{2})^{-1}]");
    histo_tot->GetXaxis()->SetRangeUser(2, 5);
    histo_tot->GetYaxis()->SetRangeUser(1, 30000);
    TF1 *f_tot = new TF1("f_tot", FuncTot, 2, 4, 11);
    for (int n_par = 0; n_par < 4; n_par++){
        f_tot->SetParameter(n_par,f_bk->GetParameter(n_par));
    }
    for (int n_par = 4; n_par < 11; n_par++){
        f_tot->SetParameter(n_par,f_peak->GetParameter(n_par));
    }
    histo_tot->Fit(f_tot, "R");

    std::vector<int> nSamples = {10,0,10,0,10,0,10,0,0,0,0};
    auto *systFitSettings = new TSystFitSettings(*f_tot,nSamples);

    cout<<"A total of "<<systFitSettings->GetNConfigurations()<<" configurations will be tested"<<endl;
    systFitSettings->GenerateConfigurations();

//    return;

    auto *systFitter = new TSystFitter(histo_tot);
    systFitter->SetSystFitSettings(systFitSettings);

    systFitter->SystFit(f_tot,"srleq","",2.,5.);

    systFitter->PrintResults(new TCanvas("canv","canv"));
}