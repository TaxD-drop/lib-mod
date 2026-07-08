#!/bin/bash
# Certifique-se de ter o NDK e definir a variável ANDROID_NDK_HOME
mkdir -p build_android && cd build_android

cmake .. \
    -DCMAKE_TOOLCHAIN_FILE=$ANDROID_NDK_HOME/build/cmake/android.toolchain.cmake \
    -DANDROID_ABI=arm64-v8a \
    -DANDROID_PLATFORM=android-21 \
    -DANDROID_BUILD=ON

cmake --build . -j$(nproc)