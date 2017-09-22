//
// Created by Gabriele Gaetano Fronzé on 25/06/2017.
//

#ifndef TSYSTFITTER_TSYSTFITSETTIGNS_H
#define TSYSTFITTER_TSYSTFITSETTIGNS_H

#include "Rtypes.h"
#include "TSystFitParameter.h"
#include <utility>
#include <vector>

class TSystFitSettings {
public:
    explicit TSystFitSettings(Int_t fNParams = 0);

    explicit TSystFitSettings(std::vector<TSystFitParameter> params) : fParams(std::move(params)){};

    void GenerateConfigurations();

    inline void AddParameter(const TSystFitParameter &param){ fParams.push_back(param); };

    Bool_t SetParameter(ULong_t iParam, TSystFitParameter param);

    TSystFitParameter GetParameter(Int_t iParam){ return fParams[iParam]; };
    inline ULong_t GetNParams(){ return (ULong_t)fParams.size(); };
    ULong_t GetNConfigurations();
    std::vector<ParamValue> GetConfiguration(unsigned long iConfig);

private:
    std::vector<TSystFitParameter> fParams;
    std::vector<std::vector<int>> fConfigurations;
};


#endif //TSYSTFITTER_TSYSTFITSETTIGNS_H
