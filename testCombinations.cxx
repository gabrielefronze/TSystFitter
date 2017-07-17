//
// Created by Gabriele Gaetano Fronzé on 17/07/2017.
//

#include <iostream>
#include <vector>

void testCombinations(){
    const int nPar = 4;

    std::vector<int> state;
    state.reserve(nPar);
    std::vector<int> config;
    config.reserve(nPar);
    int limits[nPar] = {3,4,2,2};

    for (int iPar = 0; iPar < nPar; ++iPar) {
        state[iPar]=0;
        config[iPar]=0;
//        limits[iPar]=(int)fParams[iPar].GetNValues();
    }

    bool done = false;
    int nConfig = 0;

    std::cout<<nConfig++<<"\t{";
    for (int iPar = 0; iPar < nPar; ++iPar) {
        std::cout<<config[iPar]<<" ";
    }
    std::cout<<"}"<<std::endl;

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

        std::cout<<nConfig++<<"\t{";
        for (int iPar = 0; iPar < nPar; ++iPar) {
            std::cout<<config[iPar]<<" ";
        }
        std::cout<<"}"<<std::endl;

        for (int iPar = 0; iPar < nPar; ++iPar) {
            state[iPar] = 0;
        }

    }
}