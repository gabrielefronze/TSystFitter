//
// Created by Gabriele Gaetano Fronzé on 25/06/2017.
//

#include <TMinuit.h>
#include <TPad.h>
#include <TMath.h>
#include "TFitResultPtr.h"
#include "TFitResult.h"
#include "TSystFitter.h"

void TSystFitter::SystFit(TF1 *f1, Option_t *option, Option_t *goption, Double_t xmin, Double_t xmax) {

    TString opt = option;
    if ( !opt.Contains("s") ) opt.Append("s");

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
        std::cout<<iConfig+1<<"/"<<nConfig<<"\r"<<std::flush;
    }
    std::cout<<std::endl;
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

//    std::cout<<"Config entries "<<config.size()<<std::endl;

    for(const auto &itPar : config){
        f1->SetParameter(iPar,itPar.fValue);
        f1->SetParLimits(iPar,itPar.fLowerLimit,itPar.fUpperLimit);
//        printf("Par %d set as: %f..%f..%f\n",iPar,itPar.fLowerLimit,itPar.fValue,itPar.fUpperLimit);
        iPar++;
    }
}

void TSystFitter::PrintResults(TVirtualPad *pad){
    auto nParams = fSystFitSettings->GetNParams();

    pad->Divide(2,1);
    auto dataPad = pad->cd(1);
    auto dataFitPad = new TPad("dataFit","dataFit",0.,0.3,1.,1.);
    auto fitResultsPad = new TPad("fitRes","fitRes",0.,0.,1.,0.3);
    dataPad->cd();
    dataFitPad->Draw();
    fitResultsPad->Draw();

    dataFitPad->cd();
    fHistToFit->SetTitle("Data and fit functions");
    fHistToFit->SetStats(kFALSE);
    fHistToFit->Draw();

    char *labels[5] = {"Success","Fail","Call limit","Problems","Other"};

    auto histoFitResult = new TH1I("fitRes","Fit results statistics",5,-0.5,4.5);
    histoFitResult->GetYaxis()->SetRangeUser(0.,fFitFunctions.size());
    histoFitResult->SetStats(kFALSE);
    for (int iLabel = 0; iLabel < 5; ++iLabel) {
        histoFitResult->GetXaxis()->SetBinLabel(iLabel+1,labels[iLabel]);
    }

    auto histoFitResultS = new TH1I("fitResS","fitResS",5,-0.5,4.5);
    histoFitResultS->SetLineColor(kGreen);
    histoFitResultS->SetFillColor(kGreen);
    histoFitResultS->SetFillStyle(3003);
    histoFitResultS->SetStats(kFALSE);

    auto histoFitResultF = new TH1I("fitResF","fitResF",5,-0.5,4.5);
    histoFitResultF->SetLineColor(kRed);
    histoFitResultF->SetFillColor(kRed);
    histoFitResultF->SetFillStyle(3003);
    histoFitResultF->SetStats(kFALSE);

    auto histoFitResultC = new TH1I("fitResC","fitResC",5,-0.5,4.5);
    histoFitResultC->SetLineColor(kBlue);
    histoFitResultC->SetFillColor(kBlue);
    histoFitResultC->SetFillStyle(3003);
    histoFitResultC->SetStats(kFALSE);

    auto histoFitResultP = new TH1I("fitResP","fitResP",5,-0.5,4.5);
    histoFitResultP->SetLineColor(kViolet);
    histoFitResultP->SetFillColor(kViolet);
    histoFitResultP->SetFillStyle(3003);
    histoFitResultP->SetStats(kFALSE);

    auto histoFitResultO = new TH1I("fitResO","fitResO",5,-0.5,4.5);
    histoFitResultO->SetLineColor(kOrange);
    histoFitResultO->SetFillColor(kOrange);
    histoFitResultO->SetFillStyle(3003);
    histoFitResultO->SetStats(kFALSE);

    std::cout<<"Printing functions...\n";
    int iFunc =0;
    for(auto &itFunc : fFitFunctions){

        auto fitStatus = GetFitResults()[iFunc++].second;

        itFunc.SetLineWidth(1);

        if (fitStatus.Contains("SUCC") || fitStatus.Contains("CONV")) {
            itFunc.SetLineColor(kGreen);
            histoFitResultS->Fill(0.);
        } else if (fitStatus.Contains("FAIL")) {
            itFunc.SetLineColor(kRed);
            histoFitResultF->Fill(1.);
        } else if (fitStatus.Contains("CALL")) {
            itFunc.SetLineColor(kBlue);
            histoFitResultC->Fill(2.);
        } else if ( fitStatus.Contains("PROB")){
            itFunc.SetLineColor(kViolet);
            histoFitResultP->Fill(3.);
        } else {
            itFunc.SetLineColor(kOrange);
            histoFitResultO->Fill(4.);
        }

        dataFitPad->cd();
        itFunc.Draw("SAME");
        std::cout<<iFunc+1<<"/"<<fFitFunctions.size()<<"\r"<<std::flush;
    }

    fitResultsPad->cd();
    histoFitResult->Draw();
    histoFitResultS->Draw("SAME");
    histoFitResultF->Draw("SAME");
    histoFitResultC->Draw("SAME");
    histoFitResultP->Draw("SAME");
    histoFitResultO->Draw("SAME");
    fitResultsPad->RedrawAxis();

    auto parStatsPad = pad->cd(2);
    parStatsPad->Divide(2,(Int_t)nParams);

    std::vector<Double_t> parData[nParams];
    std::vector<Double_t> parErr[nParams];

    for ( const auto &itFitResult : fFitResultsVector ){
        auto fitStatus = itFitResult.second;
        if (fitStatus.Contains("SUCC") || fitStatus.Contains("CONV")){
            for (int iPar = 0; iPar < nParams; ++iPar) {
                parData[iPar].emplace_back(itFitResult.first.Get()->Parameter(iPar));
                parErr[iPar].emplace_back(itFitResult.first.Get()->ParError(iPar));
            }
        }
    }

    std::vector<TH1D*> histVectData;
    std::vector<TH1D*> histVectErr;
    TString paramTypesExtended[4];

    paramTypesExtended[kCustom] = "From custom distribution";
    paramTypesExtended[kFix] = "Fixed value";
    paramTypesExtended[kUniform] = "From uniform distribution";
    paramTypesExtended[kNoType] = "No type";

    for (int iPar = 0; iPar < nParams; ++iPar) {
        TString parName = fFitResultsVector[0].first.Get()->ParName(iPar).c_str();
        auto paramType = paramTypesExtended[fSystFitSettings->GetParameter(iPar).GetType()];

        parStatsPad->cd(iPar*2+1);
        auto minX = *std::min_element(parData[iPar].begin(),parData[iPar].end());
        auto maxX = *std::min_element(parData[iPar].begin(),parData[iPar].end());
        auto deltaX = maxX - minX;
        auto offsetX = deltaX * 0.1;
        auto histBuffer = new TH1D(Form("hPar%dData",iPar),Form("%s values - %s",parName.Data(),paramType.Data()),parData[iPar].size(),minX-offsetX,maxX+offsetX);
        histVectData.emplace_back(histBuffer);
        for ( auto const &itValue : parData[iPar] ){
            histBuffer->Fill(itValue);
        }
        histVectData[iPar]->DrawNormalized();

        auto meanParValue = histVectData[iPar]->GetMean();
        auto sigmaTot = histVectData[iPar]->GetRMS();

        parStatsPad->cd(iPar*2+2);
        minX = *std::min_element(parErr[iPar].begin(),parErr[iPar].end());
        maxX = *std::min_element(parErr[iPar].begin(),parErr[iPar].end());
        deltaX = maxX - minX;
        offsetX = deltaX * 0.1;
        histBuffer = new TH1D(Form("hPar%dErr",iPar),Form("%s stat. error - %s",parName.Data(),paramType.Data()),parErr[iPar].size(),minX-offsetX,maxX+offsetX);
        histVectErr.emplace_back(histBuffer);
        for ( auto const &itValue : parErr[iPar] ){
            histBuffer->Fill(itValue);
        }
        histVectErr[iPar]->SetLineColor(kRed);
        histVectErr[iPar]->DrawNormalized();

        auto sigmaStat = histVectErr[iPar]->GetMean();
        auto sigmaSyst = TMath::Abs(sigmaTot)*TMath::Abs(sigmaStat)/TMath::Sqrt(TMath::Power(sigmaStat,2.)-TMath::Power(sigmaTot,2.));

        printf("###################################\n"
                       "Parameter %d:\n"
                       "\tType: %s\n"
                       "\tName: %s\n"
                       "\tValue: %f\n",
                        iPar,paramType.Data(),parName.Data(),meanParValue);
        if ( !paramType.Contains("Fixed") ){
            printf(     "\tStat. unc.: %f (%f%%)\n"
                                "\tSyst. unc.: %f (%f%%)\n",
                        sigmaStat,sigmaStat/meanParValue*100,sigmaSyst,sigmaSyst/meanParValue*100);
        }

    }



}