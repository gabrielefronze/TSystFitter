//
// Created by Gabriele Gaetano Fronzé on 25/06/2017.
//

#include "TSystFitSettings.h"
#include "TSystFitParameter.h"

TSystFitSettings::TSystFitSettings(Int_t nParams){
    fParams.reserve((unsigned long long)nParams);
}

Bool_t TSystFitSettings::SetParameter(Int_t iParam, TSystFitParameter param){
    if ( iParam > fParams.size() ) return kFALSE;
    else{
        fParams[iParam] = param;
        return kTRUE;
    }
}

Bool_t TSystFitSettings::SetParameters(TF1 *f1){
    auto nPar = fParams.size();
    auto nParOK = (f1->GetNpar() == nPar);

    if ( !nParOK ) return false;

    std::vector<int> parWithMultipleValues;

    for (int iPar = 0; iPar < nPar; ++iPar) {

        auto parValue = fParams[iPar].GetValue();

        switch (fParams[iPar].GetType()){
            case ParamType::kListOfValues: parWithMultipleValues.push_back(iPar);
            case ParamType::kDistribution: parWithMultipleValues.push_back(iPar);
            case ParamType::kFix: {
                f1->SetParameter(iPar,parValue.fValue);
                f1->SetParLimits(iPar,parValue.fValue,parValue.fValue);
            };
            case ParamType::kStandard:{
                f1->SetParameter(iPar,parValue.fValue);
                f1->SetParLimits(iPar,parValue.fLowerLimit,parValue.fUpperLimit);
            };
            case ParamType::kNone:{
                f1->SetParameter(iPar,0.);
                f1->SetParLimits(iPar,0.,0.);
            };
            default:{
                f1->SetParameter(iPar,0.);
                f1->SetParLimits(iPar,0.,0.);
            };
        }
    }

    //TODO: We have to handle every possible combination of the multi-valued parameters: recursive function?
    auto nComb = GetNCombinations();
}

ULong_t TSystFitSettings::GetNCombinations() {
    ULong_t nComb = 1;

    for(auto &itPar : fParams){
        nComb *= itPar.GetNValues();
    }

    return nComb;
}
