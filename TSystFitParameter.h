//
// Created by Gabriele Gaetano Fronzé on 25/06/2017.
//

#ifndef TSYSTFITTER_TSYSTFITPARAMETER_H
#define TSYSTFITTER_TSYSTFITPARAMETER_H

#include "TF1.h"
#include <vector>

class TSystFitParameter {
public:
    enum ParamType{
        kListOfValues,
        kDistribution,
        kFix,
        kStandard,
        kNone
    };

    struct ParamValue{
        Double_t fValue;
        Double_t fUpperLimit;
        Double_t fLowerLimit;
        ParamValue(Double_t value, Double_t upperLimit, Double_t lowerLimit){
            fValue = value;
            fUpperLimit = upperLimit;
            fLowerLimit = lowerLimit;
        }
        ParamValue(Double_t data[3]){
            fValue = data[0];
            fUpperLimit = data[1];
            fLowerLimit = data[2];
        }
    };

    TSystFitParameter() : fParamType(kNone){};
    TSystFitParameter(std::vector<ParamValue> paramValues) : fParamType(kListOfValues){ fParamValues = paramValues; };
    TSystFitParameter(Double_t fixedValue) : fParamType(kFix){ fParamValues.push_back(ParamValue(fixedValue,0.,0.)); };
    TSystFitParameter(Double_t data[3]) : fParamType(kStandard){ fParamValues.push_back(ParamValue(data)); };
    TSystFitParameter(ParamValue paramValue) : fParamType(kStandard){ fParamValues.push_back(paramValue); };
    TSystFitParameter(TF1 funcky, Int_t nSamples, Bool_t adaptive = kFALSE);


private:
    std::vector<ParamValue> fParamValues;
    ParamType fParamType;
};


#endif //TSYSTFITTER_TSYSTFITPARAMETER_H
