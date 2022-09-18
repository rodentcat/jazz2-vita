#!/bin/bash

BuildType=Release
#BuildType=Debug

export OS=emscripten
export CC=emcc

source ../emsdk/emsdk_env.sh

# Use SDL2 as default for Emscripten for now, it has better touch support
emcmake cmake -B ../Jazz2-Emscripten-${BuildType}-Demo -D CMAKE_BUILD_TYPE=${BuildType} -D NCINE_PREFERRED_BACKEND=SDL2 -D SHAREWARE_DEMO_ONLY=ON
make -j2 -C ../Jazz2-Emscripten-${BuildType}-Demo