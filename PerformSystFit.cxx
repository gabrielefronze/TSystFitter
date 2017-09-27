//
// Created by Gabriele Gaetano Fronzé on 19/09/2017.
//

#include <TRandom.h>
#include "TROOT.h"
#include "TH1D.h"
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

void PerformSystFit(){

    TVirtualFitter::SetMaxIterations( 20000 );

    auto *histo = new TH1D("histo","histo",100,-10.,10.);

    cout<<"Histo initialized"<<endl;

    auto form1 = new TFormula("form1","abs(sin(x)/x)");
    auto formula = new TF1("formula","[0]*sin(x)+[1]*cos(1.5*x)+[2]*x+[3]*x*x",-10.,10.);
    auto formulaFit = new TF1(*formula);
    formula->SetParameters(20.,20.,0.5,0.4);

    for( int i=0; i<histo->GetNbinsX(); i++){
        auto value = formula->Eval(histo->GetBinCenter(i))*(1.+0.1*(gRandom->Uniform(-1.,1.)));
        histo->SetBinContent(i,value);
    }

    histo->Sumw2();

    cout<<"Formula initialized"<<endl;

    formulaFit->SetParameters(20.,20.,0.5,0.4);
    histo->Fit(formulaFit,"rlie");

//    auto *systFitSettings = new TSystFitSettings(4);

    std::vector<int> nSamples = {0,3,3,3};
    auto *systFitSettings = new TSystFitSettings(*formulaFit,nSamples);

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

    systFitter->SystFit(formula,"srliq","",-10.,10.);

    auto canv = new TCanvas("canv","canv");

    systFitter->PrintResults(canv);
}