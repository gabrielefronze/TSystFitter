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
    kNone
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
    ParamValue(Double_t value){
        fValue = value;
        fUpperLimit = value;
        fLowerLimit = value;
    }
    ParamValue(Double_t value, Double_t lowerLimit, Double_t upperLimit){
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

    TSystFitParameter() : fParamType(kNone),fIndex(0){};

    TSystFitParameter(TF1 funcky, Int_t nSamples, Bool_t adaptive = kFALSE);
    explicit TSystFitParameter(std::vector<ParamValue> paramValues) : fParamType(kListOfValues),fIndex(0){ fParamValues.insert(fParamValues.end(), paramValues.begin(), paramValues.end()); };
    explicit TSystFitParameter(Double_t fixedValue) : fParamType(kFix),fIndex(0){ fParamValues.push_back(ParamValue(fixedValue,fixedValue,fixedValue)); };
    explicit TSystFitParameter(Double_t data[3]) : fParamType(kStandard),fIndex(0){ fParamValues.push_back(ParamValue(data)); };
    explicit TSystFitParameter(ParamValue paramValue) : fParamType(kStandard),fIndex(0){ fParamValues.push_back(paramValue); };

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
