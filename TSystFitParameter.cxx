//
// Created by Gabriele Gaetano Fronzé on 25/06/2017.
//

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

    for (int iStep = 0; iStep < nSamples; ++iStep) {

        Double_t x = xMin + iStep*stepSize;
        Double_t xMinus = x - stepSize/2.;
        Double_t xPlus = x + stepSize/2.;

        Double_t value = fSampledFuncky->Eval( x );
        Double_t bounds[2] = { fSampledFuncky->Eval( xMinus ), fSampledFuncky->Eval( xPlus )};
        Double_t lowerBound = (bounds[0]<bounds[1])?bounds[0]:bounds[1];
        Double_t upperBound = (bounds[1]>bounds[0])?bounds[1]:bounds[0];

        printf("\t%f<%f<%F\n",lowerBound,value,upperBound);

        localParamValues.emplace_back(value,lowerBound,upperBound);
    }

    fParamValues = std::move(localParamValues);
}
