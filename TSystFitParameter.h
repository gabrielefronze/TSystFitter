//
// Created by Gabriele Gaetano Fronzé on 25/06/2017.
//

#ifndef TSYSTFITTER_TSYSTFITPARAMETER_H
#define TSYSTFITTER_TSYSTFITPARAMETER_H

#include "TF1.h"
#include <vector>

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

class TSystFitParameter {
public:

    TSystFitParameter() : fParamType(kNone),fIndex(0){};
    TSystFitParameter(std::vector<ParamValue> paramValues) : fParamType(kListOfValues),fIndex(0){ fParamValues = paramValues; };
    TSystFitParameter(Double_t fixedValue) : fParamType(kFix),fIndex(0){ fParamValues.push_back(ParamValue(fixedValue,0.,0.)); };
    TSystFitParameter(Double_t data[3]) : fParamType(kStandard),fIndex(0){ fParamValues.push_back(ParamValue(data)); };
    TSystFitParameter(ParamValue paramValue) : fParamType(kStandard),fIndex(0){ fParamValues.push_back(paramValue); };
    TSystFitParameter(TF1 funcky, Int_t nSamples, Bool_t adaptive = kFALSE);

    inline const ULong_t GetNValues(){ return fParamValues.size(); };
    inline const ParamType GetType(){ return fParamType; };
    inline const ParamValue GetValue(UInt_t index=0){ return (index<fParamValues.size()) ? fParamValues[index] : ParamValue(0.,0.,0.); };
    inline const ParamValue GetNextValue(){ return (fIndex+1<fParamValues.size()) ? fParamValues[fIndex++] : ParamValue(0.,0.,0.); };


private:
    std::vector<ParamValue> fParamValues;
    UInt_t fIndex;
    ParamType fParamType;
};


#endif //TSYSTFITTER_TSYSTFITPARAMETER_H
