//
// Created by Gabriele Gaetano Fronzé on 25/06/2017.
//

#ifndef TSYSTFITTER_TSYSTFITPARAMETER_H
#define TSYSTFITTER_TSYSTFITPARAMETER_H

#include "TF1.h"
#include <utility>
#include <vector>

enum ParamType{
    kListOfValues,
    kDistribution,
    kFix,
    kStandard,
    kNoType
};

struct ParamValue{
    Double_t fValue;
    Double_t fUpperLimit;
    Double_t fLowerLimit;

    ParamValue(){
        fValue = 0.;
        fUpperLimit = 0.;
        fLowerLimit = 0.;
    }
    explicit ParamValue(Double_t value){
        fValue = value;
        fUpperLimit = value;
        fLowerLimit = value;
    }
    explicit ParamValue(Double_t value, Double_t lowerLimit, Double_t upperLimit){
        fValue = value;
        fUpperLimit = upperLimit;
        fLowerLimit = lowerLimit;
    }
    explicit ParamValue(const Double_t data[3]){
        fValue = data[0];
        fUpperLimit = data[2];
        fLowerLimit = data[1];
    }
};

class TSystFitParameter {
public:

    TSystFitParameter() : fParamType(kNoType){};

    explicit TSystFitParameter(TF1 *funcky, Int_t nSamples);
    explicit TSystFitParameter(ParamValue paramValue, Int_t nSamples);

    inline const ULong_t GetNValues(){ return fParamValues.size(); };
    inline const ParamType GetType(){ return fParamType; };
    inline const ParamValue GetValue(UInt_t index=0){ return (index<fParamValues.size()) ? fParamValues[index] : ParamValue(0.,0.,0.); };

    inline TF1* GetFuncky(){ return new TF1(*fSampledFuncky); };

private:
    std::vector<ParamValue> fParamValues;
    ParamType fParamType;
    TF1 *fSampledFuncky;
};


#endif //TSYSTFITTER_TSYSTFITPARAMETER_H
