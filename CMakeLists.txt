cmake_minimum_required(VERSION 3.7)
project(TSystFitter)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES
        LICENSE.md
        README.md TSystFitter.cxx TSystFitter.h TSystFitSettings.cxx TSystFitSettings.h TSystFitParameter.cxx TSystFitParameter.h testCombinations.cxx PerformSystFit.cxx CompileAllClasses.cxx)

include_directories(.
        /usr/include
        /Users/Gabriele/alice_sw/sw/osx_x86-64/ROOT/v5-34-30-alice7-1/include
        /usr/local/opt/libiomp/include/libiomp)

add_executable(TSystFitter ${SOURCE_FILES} TSystFitter.cxx TSystFitter.h TSystFitSettings.cxx TSystFitSettings.h)