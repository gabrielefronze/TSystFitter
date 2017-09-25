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

    TVirtualFitter::SetMaxIterations( 2000 );

    auto *histo = new TH1D("histo","histo",100,-10.,10.);

    cout<<"Histo initialized"<<endl;

    auto form1 = new TFormula("form1","abs(sin(x)/x)");
    auto formula = new TF1("formula","[0]*sin(x)+[1]*cos(x)+[2]*x+[3]*x*x",-10.,10.);
    formula->SetParameters(20.,20.,0.05,0.04);

    for( int i=0; i<histo->GetNbinsX(); i++){
        auto value = formula->Eval(histo->GetBinCenter(i))*(1.+0.1*(gRandom->Uniform(-1.,1.)));
        histo->SetBinContent(i,value);
    }

    histo->Sumw2();

    cout<<"Formula initialized"<<endl;



    auto *systFitSettings = new TSystFitSettings(4);

    cout<<"Settings initialized"<<endl;



    systFitSettings->AddParameter(TSystFitParameter(ParamValue(20.,20.,20.)));

    cout<<"Par0 initialized with "<<systFitSettings->GetParameter(0).GetNValues()<<" values"<<endl;

    std::vector<ParamValue> par1Values = {ParamValue(17.,17.,21.),ParamValue(18.,17.,21.),ParamValue(19.,17.,21.),ParamValue(20.,17.,21.),ParamValue(21.,17.,21.)};

    cout<<par1Values.size()<<endl;
    systFitSettings->AddParameter(TSystFitParameter(par1Values));

    cout<<"Par1 initialized with "<<systFitSettings->GetParameter(1).GetNValues()<<" values"<<endl;

    systFitSettings->AddParameter(TSystFitParameter(new TF1("fa1","sin(x)/x",0,10),10));

    cout<<"Par2 initialized with "<<systFitSettings->GetParameter(2).GetNValues()<<" values"<<endl;

    Double_t par3Values[3] = {1.,0.,2.};
    systFitSettings->AddParameter(TSystFitParameter(par3Values));

    cout<<"Par3 initialized with "<<systFitSettings->GetParameter(3).GetNValues()<<" values"<<endl;

    cout<<"A total of "<<systFitSettings->GetNConfigurations()<<" configurations will be tested"<<endl;
    systFitSettings->GenerateConfigurations();

    auto *systFitter = new TSystFitter(histo);
    systFitter->SetSystFitSettings(systFitSettings);

    systFitter->SystFit(formula,"sreli","",-10.,10.);

    auto canv = new TCanvas("canv","canv");

    systFitter->PrintResults(canv);
}