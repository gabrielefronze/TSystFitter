//
// Created by Gabriele Gaetano Fronzé on 25/06/2017.
//

#ifndef TSYSTFITTER_H
#define TSYSTFITTER_H

#include "TH1.h"
#include "TSystFitSettings.h"
#include <utility>
#include <vector>
#include <TVirtualPad.h>

class TSystFitter {

public:
    explicit TSystFitter(TH1 *histToFit) : fHistToFit((TH1*)histToFit->Clone("histToFit")){};

    ~TSystFitter() { delete fHistToFit; };

    void SetSystFitSettings(TSystFitSettings *settings){ fSystFitSettings = settings; };
    void SetSystFitSettings(std::vector<TSystFitParameter> params){ fSystFitSettings = new TSystFitSettings(std::move(params)); };

    void SystFit(TF1 *f1, Option_t *option, Option_t *goption, Double_t xmin, Double_t xmax);
    void SystFit(const char *formula, Option_t *option, Option_t *goption, Double_t xmin, Double_t xmax);

    std::vector<std::pair<TFitResultPtr,TString>> GetFitResults(){ return fFitResultsVector; };

    std::vector<TF1> fFitFunctions;

    void PrintResults(TVirtualPad *pad);

private:
    std::vector<std::pair<TFitResultPtr,TString>> fFitResultsVector;
    TSystFitSettings* fSystFitSettings;
    TH1 *fHistToFit;
    void SetConfiguration(TF1 *f1, unsigned long iConfig);


};

//TODO: maybe the fitting function should be placed inside TSystFitSettings
//TODO: several TSystFitSettings will contain different functions
//TODO: the extrapolation at a certain value will be given with stat and syst error taking into account all fit results

#endif //TSYSTFITTER_H
