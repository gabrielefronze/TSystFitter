//
// Created by Gabriele Gaetano Fronzé on 19/09/2017.
//

/*==================================================================*/
/*macro per la compilazione one call di tutte le classi usate       */
#include <TSystem.h>

/*==================================================================*/

void CompileAllClasses(TString myopt="fast"){
    char * opt;
    if(myopt.Contains("force")){
        opt = "kfg";
    }
    else {
        opt = "kg";
    }
    gSystem->CompileMacro("TSystFitParameter.cxx",opt);
    gSystem->CompileMacro("TSystFitSettings.cxx",opt);
    gSystem->CompileMacro("TSystFitter.cxx",opt);
    gSystem->CompileMacro("PerformSystFit.cxx",opt);


}
