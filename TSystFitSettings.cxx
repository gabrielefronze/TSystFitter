//
// Created by Gabriele Gaetano Fronzé on 25/06/2017.
//

#include <utility>

#include "TSystFitSettings.h"

TSystFitSettings::TSystFitSettings(Int_t nParams){
    fParams.resize((unsigned long long)nParams);
}

TSystFitSettings::TSystFitSettings(TF1 funcky, std::vector<int> nSamples){

    if ( nSamples.size() != funcky.GetNpar() ){
        std::cout<<"Sei un pollo! Precisare nSamples per "<<funcky.GetNpar()<<" parametri!"<<std::endl;
        return;
    }

    fParams.resize(funcky.GetNpar());

    for(Int_t iPar = 0; iPar < funcky.GetNpar(); iPar++){
        auto value = funcky.GetParameter(iPar);
        auto error = funcky.GetParError(iPar);
        auto minValue = value - error;
        auto maxValue = value + error;
        auto nSamplesLocal = nSamples[iPar];
        std::cout<<"required samples "<<nSamplesLocal<<std::endl;
        if ( nSamplesLocal==0 ){
            std::cout<<"Fixed "<<iPar<<" as "<<value<<std::endl;
            this->SetParameter(iPar,TSystFitParameter(ParamValue(value,value,value),1));
        } else {
            this->SetParameter(iPar,TSystFitParameter(ParamValue(value,minValue,maxValue),nSamplesLocal));
            std::cout<<"Parameter "<<iPar<<" set as {"<<value<<","<<minValue<<","<<maxValue<<"}"<<std::endl;
        }
    }
};

void TSystFitSettings::GenerateConfigurations(){
    const unsigned long nPar = fParams.size();

    std::vector<int> state;
    state.resize(nPar);
    std::vector<int> config;
    config.resize(nPar);
    std::vector<int> limits;
    limits.resize(nPar);

    for (unsigned long iPar = 0; iPar < nPar; ++iPar) {
        state[iPar]=0;
        config[iPar]=0;
        limits[iPar]=(int)fParams[iPar].GetNValues();
    }

    bool done = false;
    ULong_t nConfig = 0;

    fConfigurations.push_back(config);

    while (true) {
        for (unsigned long iPar = 0; iPar < nPar; ++iPar) {
            if (config[iPar] + 1 < limits[iPar]){
                state[iPar] = 1;
                break;
            }
        }

        int fillValue = -1;
        for (unsigned long iPar = 0; iPar < nPar; ++iPar) {
            if (state[iPar] == 1) {
                fillValue = 0;
                continue;
            }
            state[iPar] = fillValue;
        }
        if (state[nPar-1]==-1) break;

        for (unsigned long iPar = 0; iPar < nPar; ++iPar) {
            if (state[iPar] == 1) config[iPar]++;
            else if (state[iPar] == -1) config[iPar] = 0;
        }

        nConfig++;

        fConfigurations.push_back(config);

        for (unsigned long iPar = 0; iPar < nPar; ++iPar) {
            state[iPar] = 0;
        }
    }

    std::cout<<nConfig+1<<" configurations "<<((nConfig+1 == GetNConfigurations())?"correctly":"badly")<<" generated!"<<std::endl;
}

Bool_t TSystFitSettings::SetParameter(ULong_t iParam, TSystFitParameter param){

    if ( iParam > fParams.size() ) fParams.resize(iParam);

    fParams[iParam] = std::move(param);
    return kTRUE;
}

std::vector<ParamValue> TSystFitSettings::GetConfiguration(unsigned long iConfig){
    std::vector<ParamValue> returnConfig;

    if(iConfig>this->GetNConfigurations()) return returnConfig;

    auto config = fConfigurations[iConfig];

    returnConfig.reserve(config.size());

    for (unsigned long iPar = 0; iPar < config.size(); ++iPar) {
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
