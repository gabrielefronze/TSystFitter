//
// Created by Gabriele Gaetano Fronzé on 25/06/2017.
//

#ifndef TSYSTFITTER_H
#define TSYSTFITTER_H

#include "TH1.h"
#include "TSystFitSettings.h"
#include <vector>

class TSystFitter {

public:
    TSystFitter(TH1 *histToFit) : fHistToFit((TH1*)histToFit->Clone("histToFit")){};

    ~TSystFitter() { delete fHistToFit; };

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
    TH1 *fHistToFit;
    void SetConfiguration(TF1 *f1, std::vector<ParamValue> config);


};


#endif //TSYSTFITTER_H
