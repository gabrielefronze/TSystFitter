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
#include <vector>
#include <iostream>

using namespace std;

void PerformSystFit(){

    TVirtualFitter::SetMaxIterations( 2000 );

    auto *histo = new TH1D("histo","histo",100,-10.,10.);

    cout<<"Histo initialized"<<endl;

    auto form1 = new TFormula("form1","abs(sin(x)/x)");
    auto formula = new TF1("formula","[0]*sin(x)+[1]*cos(x)+[2]*x+[3]*x*x",-10.,10.);
    formula->SetParameters(20.,20.,0.001,0.04);

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

    std::vector<ParamValue> par1Values = {ParamValue(19.,17.,21.),ParamValue(1.,-1.5,3.5),ParamValue(4.,0.,10.),ParamValue(2.,0.,20.),ParamValue(-3.,-40.,23.)};

    cout<<par1Values.size()<<endl;
    systFitSettings->AddParameter(TSystFitParameter(par1Values));

    cout<<"Par1 initialized with "<<systFitSettings->GetParameter(1).GetNValues()<<" values"<<endl;

    systFitSettings->AddParameter(TSystFitParameter(TF1("gaus","gaus",-2.,2.),10));

    cout<<"Par2 initialized with "<<systFitSettings->GetParameter(2).GetNValues()<<" values"<<endl;

    Double_t par3Values[3] = {27.,0.,50.};
    systFitSettings->AddParameter(TSystFitParameter(par3Values));

    cout<<"Par3 initialized with "<<systFitSettings->GetParameter(3).GetNValues()<<" values"<<endl;

    cout<<"A total of "<<systFitSettings->GetNConfigurations()<<" configurations will be tested"<<endl;
    systFitSettings->GenerateConfigurations();

    auto *systFitter = new TSystFitter(histo);
    systFitter->SetSystFitSettings(systFitSettings);

    systFitter->SystFit(formula,"sreli","",-10.,10.);

    histo->Draw();
//    formula->Draw("SAME");
    int iFunc =0;
    for(auto &itFunc : systFitter->fFitFunctions){

        auto fitStatus = systFitter->GetFitResults()[iFunc++].second;

        itFunc.SetLineWidth(1);

        if (fitStatus.Contains("SUCC")) {
            itFunc.SetLineColor(kGreen);
        } else if (fitStatus.Contains("FAIL")) {
            itFunc.SetLineColor(kRed);
        } else if (fitStatus.Contains("CALL")) {
                itFunc.SetLineColor(kBlue);
        } else itFunc.SetLineColor(kOrange);

        itFunc.Draw("SAME");
    }
}