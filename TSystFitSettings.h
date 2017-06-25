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
    TSystFitSettings(std::vector<TSystFitParameter> params) : fParams(params){ fNParams = (Int_t)fParams.size(); };

    Bool_t SetParameter(Int_t iParam, TSystFitParameter param);
    TSystFitParameter GetParameter(Int_t iParam){ return fParams[iParam]; };

private:
    Int_t fNParams;
    std::vector<TSystFitParameter> fParams;
};


#endif //TSYSTFITTER_TSYSTFITSETTIGNS_H
