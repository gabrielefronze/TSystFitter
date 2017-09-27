//
// Created by Gabriele Gaetano Fronzé on 25/06/2017.
//

#include <TRandom.h>
#include "TSystFitParameter.h"
#include "TH1D.h"

TSystFitParameter::TSystFitParameter(TF1 *funcky, Int_t nSamples)  : fParamType(kDistribution) {
    std::vector<ParamValue> localParamValues;

    Double_t xMin, xMax;
    xMin = xMax = 0.;
    this->fSampledFuncky = new TF1(*funcky);
    this->fSampledFuncky->GetRange(xMin, xMax);

    std::cout<<"Range is "<<xMin<<" "<<xMax<<std::endl;

    Double_t stepSize = (xMax - xMin)/nSamples;

    printf("Sampled values:\n");

    for (int iSample = 0; iSample < nSamples; ++iSample) {
        auto value = this->fSampledFuncky->GetRandom(xMin,xMax);

        localParamValues.emplace_back(value,xMin,xMax);
        printf("%f\n",value);
    }

    this->fParamValues = std::move(localParamValues);
}

TSystFitParameter::TSystFitParameter(ParamValue paramValue, Int_t nSamples) : fParamType(kStandard),fSampledFuncky(0x0) {

    if (paramValue.fLowerLimit==paramValue.fUpperLimit) {
        fParamType = kFix;
        fSampledFuncky = 0x0;
        fParamValues.emplace_back(paramValue);
    }
    else {
        Double_t xMin = (paramValue.fLowerLimit<paramValue.fUpperLimit)?paramValue.fLowerLimit:paramValue.fUpperLimit;
        Double_t xMax = (paramValue.fLowerLimit>paramValue.fUpperLimit)?paramValue.fLowerLimit:paramValue.fUpperLimit;

        fParamType = kStandard;

        std::vector<ParamValue> localParamValues;

        fSampledFuncky = new TF1("unidist","1.",xMin,xMax);
        fSampledFuncky->GetRange(xMin, xMax);

        std::cout<<"Range is "<<xMin<<" "<<xMax<<std::endl;

        Double_t stepSize = (xMax - xMin)/nSamples;

        printf("Sampled values:\n");

        for (int iSample = 0; iSample < nSamples; ++iSample) {
            auto value = fSampledFuncky->GetRandom(xMin,xMax);

            localParamValues.emplace_back(value,xMin,xMax);
            printf("%f\n",value);
        }

        fParamValues = std::move(localParamValues);
    }
}
