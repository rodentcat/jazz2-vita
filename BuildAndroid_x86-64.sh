#!/usr/bin/env bash

BuildType=Release
#BuildType=Debug

export ANDROID_HOME="$(pwd)/../android-sdk"
export GRADLE_HOME="$(pwd)/../gradle"

cmake -B ../Jazz2-Android-${BuildType}_x86-64 -D CMAKE_BUILD_TYPE=${BuildType} -D NCINE_LINKTIME_OPTIMIZATION=ON -D NCINE_BUILD_ANDROID=ON -D NCINE_ASSEMBLE_APK=ON -D NCINE_NDK_ARCHITECTURES="x86_64" -D NDK_DIR=$(pwd)/../android-ndk
make -j8 -C ../Jazz2-Android-${BuildType}_x86-64