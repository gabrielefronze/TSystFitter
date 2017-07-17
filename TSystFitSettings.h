//
// Created by Gabriele Gaetano Fronzé on 25/06/2017.
//

#ifndef TSYSTFITTER_TSYSTFITSETTIGNS_H
#define TSYSTFITTER_TSYSTFITSETTIGNS_H

#include "Rtypes.h"
#include "TSystFitParameter.h"
#include <vector>

class TSystFitSettings {
public:
    TSystFitSettings(Int_t fNParams = 0);
    TSystFitSettings(std::vector<TSystFitParameter> params) : fParams(params){};

    void GenerateConfigurations();

    void AddParameter(TSystFitParameter param){ fParams.push_back(param); };
    Bool_t SetParameter(Int_t iParam, TSystFitParameter param);
    Bool_t SetParameters(TF1 *f1);
    TSystFitParameter GetParameter(Int_t iParam){ return fParams[iParam]; };
    inline ULong_t GetNParams(){ return (ULong_t)fParams.size(); };
    ULong_t GetNConfigurations();
    std::vector<ParamValue> GetConfiguration(int iConfig);

private:
    std::vector<TSystFitParameter> fParams;
    std::vector<std::vector<int>> fConfigurations;
};


#endif //TSYSTFITTER_TSYSTFITSETTIGNS_H
