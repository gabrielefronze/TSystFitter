//
// Created by Gabriele Gaetano Fronzé on 25/06/2017.
//

#include <TMinuit.h>
#include "TFitResultPtr.h"
#include "TFitResult.h"
#include "TSystFitter.h"

void TSystFitter::SystFit(TF1 *f1, Option_t *option, Option_t *goption, Double_t xmin, Double_t xmax) {

    if ( (ULong_t)(f1->GetNpar()) != fSystFitSettings->GetNParams() ){
        std::cout<<"ERROR: Wrong number of parameters. Aborting.";
        return;
    }

    auto nConfig = fSystFitSettings->GetNConfigurations();
    fSystFitSettings->GenerateConfigurations();

    for (unsigned long iConfig = 0; iConfig < nConfig; ++iConfig) {
        SetConfiguration(f1, iConfig);
        fFitResultsVector.emplace_back(fHistToFit->Fit(f1,option,goption,xmin,xmax),gMinuit->fCstatu.Data());
        fFitFunctions.emplace_back(*f1);
    }
}

void TSystFitter::SystFit(const char *formula, Option_t *option, Option_t *goption, Double_t xmin, Double_t xmax){

    auto *f1 = new TF1("f1",formula,xmin,xmax);

    TSystFitter::SystFit(f1,option,goption,xmin,xmax);
}

void TSystFitter::SetConfiguration(TF1 *f1, unsigned long iConfig){

    if ( (ULong_t)(f1->GetNpar()) != fSystFitSettings->GetNParams() ){
        std::cout<<"ERROR: Wrong number of parameters. Aborting.";
        return;
    }

    int iPar=0;
    auto config = fSystFitSettings->GetConfiguration(iConfig);

    std::cout<<"Config entries "<<config.size()<<std::endl;

    for(const auto &itPar : config){
        f1->SetParameter(iPar,itPar.fValue);
        f1->SetParLimits(iPar,itPar.fLowerLimit,itPar.fUpperLimit);
        printf("Par %d set as: %f..%f..%f\n",iPar,itPar.fLowerLimit,itPar.fValue,itPar.fUpperLimit);
        iPar++;
    }
}

void TSystFitter::PrintResults(TVirtualPad *pad){
    auto nParams = fSystFitSettings->GetNParams();

    pad->Divide(2,1);
    auto dataFitPad = pad->cd(1);

    fHistToFit->Draw();
    int iFunc =0;
    for(auto &itFunc : fFitFunctions){

        auto fitStatus = GetFitResults()[iFunc++].second;

        itFunc.SetLineWidth(1);

        if (fitStatus.Contains("SUCC")) {
            itFunc.SetLineColor(kGreen);
        } else if (fitStatus.Contains("FAIL")) {
            itFunc.SetLineColor(kRed);
        } else if (fitStatus.Contains("CALL")) {
                itFunc.SetLineColor(kBlue);
        } else itFunc.SetLineColor(kOrange);

        dataFitPad->cd();
        itFunc.Draw("SAME");
    }

    auto parStatsPad = pad->cd(2);
    parStatsPad->Divide(2,(Int_t)nParams/2+1);

    std::vector<Double_t> parData[nParams];

    for ( const auto &itFitResult : fFitResultsVector ){
        for (int iPar = 0; iPar < nParams; ++iPar) {
            parData[iPar].emplace_back(itFitResult.first.Get()->Parameter(iPar));
            std::cout << parData[iPar].back() << std::endl;
        }
    }

    std::vector<TH1D*> histVect;

    for (int iPar = 0; iPar < nParams; ++iPar) {
        parStatsPad->cd(iPar+1);
        auto minX = *std::min_element(parData[iPar].begin(),parData[iPar].end());
        auto maxX = *std::min_element(parData[iPar].begin(),parData[iPar].end());
        auto deltaX = maxX - minX;
        auto offsetX = deltaX * 0.1;
        auto histBuffer = new TH1D(Form("hPar%d",iPar),Form("%s",fFitResultsVector[0].first.Get()->ParName(iPar).c_str()),10000,minX-offsetX,maxX+offsetX);
        histVect.emplace_back(histBuffer);
        for ( auto const &itValue : parData[iPar] ){
            histBuffer->Fill(itValue);
        }
        histVect[iPar]->Draw();
    }



}