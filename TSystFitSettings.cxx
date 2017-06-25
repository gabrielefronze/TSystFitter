//
// Created by Gabriele Gaetano Fronzé on 25/06/2017.
//

#include "TSystFitSettings.h"

TSystFitSettings::TSystFitSettings(Int_t nParams) : fNParams(nParams){
    fParams.reserve((unsigned long long)fNParams);
}

Bool_t TSystFitSettings::SetParameter(Int_t iParam, TSystFitParameter param){
    if ( iParam > fNParams ) return kFALSE;
    else{
        fParams[iParam] = param;
        return kTRUE;
    }
}