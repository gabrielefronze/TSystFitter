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
    explicit TSystFitter(TH1 *histToFit) : fHistToFit((TH1*)histToFit->Clone("histToFit")){};

    ~TSystFitter() { delete fHistToFit; };

    void SetSystFitSettings(TSystFitSettings settings){ fSystFitSettings = settings; };
    void SetSystFitSettings(std::vector<TSystFitParameter> params){ fSystFitSettings = TSystFitSettings(params); };
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
