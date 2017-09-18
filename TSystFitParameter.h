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
    ParamValue(Double_t value, Double_t upperLimit, Double_t lowerLimit){
        fValue = value;
        fUpperLimit = upperLimit;
        fLowerLimit = lowerLimit;
    }

    explicit ParamValue(Double_t data[3]){
        fValue = data[0];
        fUpperLimit = data[1];
        fLowerLimit = data[2];
    }
};

class TSystFitParameter {
public:

    TSystFitParameter() = default;

    TSystFitParameter(TF1 funcky, Int_t nSamples, Bool_t adaptive = kFALSE);
    explicit TSystFitParameter(std::vector<ParamValue> paramValues) : fParamType(kListOfValues),fIndex(0){ fParamValues = std::move(paramValues); };
    explicit TSystFitParameter(Double_t fixedValue) : fParamType(kFix),fIndex(0){ fParamValues.emplace_back(fixedValue,fixedValue,fixedValue); };
    explicit TSystFitParameter(Double_t data[3]) : fParamType(kStandard),fIndex(0){ fParamValues.emplace_back(data); };
    explicit TSystFitParameter(ParamValue paramValue) : fParamType(kStandard),fIndex(0){ fParamValues.push_back(paramValue); };

    inline const ULong_t GetNValues(){ return fParamValues.size(); };
    inline const ParamType GetType(){ return fParamType; };
    inline const ParamValue GetValue(UInt_t index=0){ return (index<fParamValues.size()) ? fParamValues[index] : ParamValue(0.,0.,0.); };
    inline const ParamValue GetNextValue(){ return (fIndex+1<fParamValues.size()) ? fParamValues[fIndex++] : ParamValue(0.,0.,0.); };


private:
    std::vector<ParamValue> fParamValues;
    UInt_t fIndex{0};
    ParamType fParamType{kNone};
};


#endif //TSYSTFITTER_TSYSTFITPARAMETER_H
