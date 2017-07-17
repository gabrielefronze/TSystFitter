//
// Created by Gabriele Gaetano Fronzé on 25/06/2017.
//

#include "TSystFitter.h"

bool TSystFitter::SystFit(TF1 *f1, Option_t *option, Option_t *goption, Double_t xmin, Double_t xmax) {

    auto nConfig = fSystFitSettings.GetNConfigurations();

    for (int iConfig = 0; iConfig < nConfig; ++iConfig) {
        SetConfiguration(f1, fSystFitSettings.GetConfiguration(iConfig));
    }
    fFitResultsVector.push_back(fHistToFit->Fit(f1,option,goption,xmin,xmax));
}

void TSystFitter::SetConfiguration(TF1 *f1, std::vector<ParamValue> config){

}