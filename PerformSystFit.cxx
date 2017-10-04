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

    TVirtualFitter::SetMaxIterations( 100000 );

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
    histo->Fit(formulaFit,"ril");

    std::vector<int> nSamples = {7,7,7,7};
    auto *systFitSettings = new TSystFitSettings(*formulaFit,nSamples);

    cout<<"A total of "<<systFitSettings->GetNConfigurations()<<" configurations will be tested"<<endl;
    systFitSettings->GenerateConfigurations();

    auto *systFitter = new TSystFitter(histo);
    systFitter->SetSystFitSettings(systFitSettings);

    systFitter->SystFit(formula,"srle","",-10.,10.);

    auto canv = new TCanvas("canv","canv");

    systFitter->PrintResults(canv);
}