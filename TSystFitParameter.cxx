//
// Created by Gabriele Gaetano Fronzé on 25/06/2017.
//

#include <TRandom.h>
#include "TSystFitParameter.h"
#include "TH1D.h"

//TODO: make the method adaptive. It will scan the input distribution to adapt the x values to its derivative.
//TODO: this will grant to keep track of fast oscillations
TSystFitParameter::TSystFitParameter(TF1 *funcky, Int_t nSamples, Bool_t adaptive)  : fParamType(kDistribution),fIndex(0) {
    std::vector<ParamValue> localParamValues;

    Double_t xMin, xMax;
    xMin = xMax = 0.;
    fSampledFuncky = new TF1(*funcky);
    fSampledFuncky->GetRange(xMin, xMax);

    Double_t stepSize = (xMax - xMin)/nSamples;

    printf("Sampled values:\n");

    for (int iSample = 0; iSample < nSamples; ++iSample) {
        auto value = fSampledFuncky->GetRandom(xMin,xMax);

        localParamValues.emplace_back(value,xMin,xMax);
        printf("%f\n",value);
    }

    fParamValues = std::move(localParamValues);
}
