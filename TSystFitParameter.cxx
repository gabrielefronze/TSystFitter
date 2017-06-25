//
// Created by Gabriele Gaetano Fronzé on 25/06/2017.
//

#include "TSystFitParameter.h"
#include "TH1D.h"

TSystFitParameter::TSystFitParameter(TF1 funcky, Int_t nSamples, Bool_t adaptive)  : fParamType(kDistribution) {
    std::vector<ParamValue> localParamValues;

    Double_t xMin, xMax;
    xMin = xMax = 0.;
    funcky.GetRange(xMin, xMax);

    Double_t stepSize = (xMax - xMin)/nSamples;

    for (int iStep = 0; iStep < nSamples; ++iStep) {

        Double_t x = xMin + iStep*stepSize;
        Double_t xMinus = x - stepSize/2.;
        Double_t xPlus = x + stepSize/2.;

        Double_t value = funcky.Eval( x );
        Double_t bounds[2] = { funcky.Eval( xMinus ), funcky.Eval( xPlus )};
        Double_t lowerBound = (bounds[0]<bounds[1])?bounds[0]:bounds[1];
        Double_t upperBound = (bounds[1]>bounds[0])?bounds[1]:bounds[0];

        localParamValues.push_back(ParamValue(value,lowerBound,upperBound));
    }

    TSystFitParameter::TSystFitParameter(localParamValues);
}
