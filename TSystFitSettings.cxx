//
// Created by Gabriele Gaetano Fronzé on 25/06/2017.
//

#include <utility>

#include "TSystFitSettings.h"
#include "TSystFitParameter.h"

TSystFitSettings::TSystFitSettings(Int_t nParams){
    fParams.reserve((unsigned long long)nParams);
}

void TSystFitSettings::GenerateConfigurations(){
    const unsigned long nPar = fParams.size();

    std::vector<int> state;
    state.reserve(nPar);
    std::vector<int> config;
    config.reserve(nPar);
    std::vector<int> limits;
    limits.reserve(nPar);

    for (int iPar = 0; iPar < nPar; ++iPar) {
        state[iPar]=0;
        config[iPar]=0;
        limits[iPar]=(int)fParams[iPar].GetNValues();
    }

    bool done = false;
    int nConfig = 0;

    fConfigurations.push_back(config);

    while (true) {
        for (int iPar = 0; iPar < nPar; ++iPar) {
            if (config[iPar] + 1 < limits[iPar]){
                state[iPar] = 1;
                break;
            }
        }

        int fillValue = -1;
        for (int iPar = 0; iPar < nPar; ++iPar) {
            if (state[iPar] == 1) {
                fillValue = 0;
                continue;
            }
            state[iPar] = fillValue;
        }
        if (state[nPar-1]==-1) break;

        for (int iPar = 0; iPar < nPar; ++iPar) {
            if (state[iPar] == 1) config[iPar]++;
            else if (state[iPar] == -1) config[iPar] = 0;
        }

        fConfigurations.push_back(config);

        for (int iPar = 0; iPar < nPar; ++iPar) {
            state[iPar] = 0;
        }
    }

    std::cout<<nConfig+1<<" configurations "<<((nConfig+1 == GetNConfigurations())?"correctly":"badly")<<" generated!"<<std::endl;
}

Bool_t TSystFitSettings::SetParameter(Int_t iParam, TSystFitParameter param){
    if ( iParam > fParams.size() ) return kFALSE;
    else{
        fParams[iParam] = std::move(param);
        return kTRUE;
    }
}

std::vector<ParamValue> TSystFitSettings::GetConfiguration(int iConfig){
    std::vector<ParamValue> returnConfig;

    auto config = fConfigurations[iConfig];

    returnConfig.reserve(config.size());

    for (int iPar = 0; iPar < config.size(); ++iPar) {
        returnConfig.emplace_back(fParams[iPar].GetValue((UInt_t)config[iPar]));
    }

    return returnConfig;
}

ULong_t TSystFitSettings::GetNConfigurations() {
    ULong_t nComb = 1;

    for(auto &itPar : fParams){
        nComb *= itPar.GetNValues();
    }

    return nComb;
}
