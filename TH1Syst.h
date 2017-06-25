//
// Created by Gabriele Gaetano Fronzé on 25/06/2017.
//

#ifndef TSYSTFITTER_TH1SYST_H
#define TSYSTFITTER_TH1SYST_H

#include "TH1.h"
#include "TSystFitSettings.h"
#include <vector>

class TH1Syst : public TH1 {

public:
    TH1Syst() : TH1(){};
    TH1Syst(const char *name, const char *title, Int_t nbinsx, Double_t xlow, Double_t xup) : TH1(name, title, nbinsx, xlow, xup) {};
    TH1Syst(const char *name, const char *title, Int_t nbinsx, const Float_t *xbins) : TH1(name, title, nbinsx, xbins) {};
    TH1Syst(const char *name, const char *title, Int_t nbinsx, const Double_t *xbins) : TH1(name, title, nbinsx, xbins) {};
    TH1Syst(const TH1Syst &hist) : TH1(hist){};

    virtual ~TH1Syst() {};

    void SetSystFitSettings(TSystFitSettings settings){ fSystFitSettings = settings; };
    void SetSystFitSettings(std::vector<TSystFitParameter> params){ fSystFitSettings = TSystFitSettings(params); };

    //TODO: this methods should also get a SystFitSettings object.
    //TODO: To check that the number of parameters of the function and the lenght of settings is the same
    void SystFit(TF1 *f1, Option_t *option, Option_t *goption, Double_t xmin, Double_t xmax);
    void SystFit(const char *formula, Option_t *option, Option_t *goption, Double_t xmin, Double_t xmax);

    std::vector<TFitResultPtr> GetFitResults(){ return fFitResultsVector; };

private:
    std::vector<TFitResultPtr> fFitResultsVector;
    TSystFitSettings fSystFitSettings;


};


#endif //TSYSTFITTER_TH1SYST_H
