#!/bin/bash

export SDL2_VERSION=2.24.2
export SDL2_TTF_VERSION=2.20.1

rm -rf external
mkdir external
mkdir external/include
mkdir external/lib

rm -rf tmp
mkdir tmp
cd tmp

#
# build freeimage and copy to external
#

curl -sL https://downloads.sourceforge.net/project/freeimage/Source%20Distribution/3.18.0/FreeImage3180.zip -o FreeImage3180.zip
unzip FreeImage3180.zip
cd FreeImage
cp ../../freeimage/Makefile.macos.arm64 .
make -f Makefile.macos.arm64
cp Dist/libfreeimage-arm64.a ../../external/lib/libfreeimage.a
cd ..

#
# download bass24 and copy to external
#

curl -s https://www.un4seen.com/files/bass24-osx.zip -o bass.zip
unzip bass.zip 
mv libbass.dylib ../external/lib

#
# build SDL2 and copy to external
#

curl -sL https://github.com/libsdl-org/SDL/releases/download/release-${SDL2_VERSION}/SDL2-${SDL2_VERSION}.zip -o SDL2-${SDL2_VERSION}.zip
unzip SDL2-${SDL2_VERSION}.zip
cp -r SDL2-${SDL2_VERSION}/include ../external/include/SDL2
cd SDL2-${SDL2_VERSION}
cmake -DSDL_SHARED=OFF \
	-DSDL_STATIC=ON \
        -DSDL_STATIC_PIC=ON \
	-DSDL_TEST=OFF \
	-DCMAKE_OSX_ARCHITECTURES=arm64 \
	-DCMAKE_BUILD_TYPE=Release \
	-B build
cmake --build build
cp build/libSDL2.a ../../external/lib
cd ..

#
# build SDL2_ttf and copy to external
#

curl -sL https://github.com/libsdl-org/SDL_ttf/releases/download/release-${SDL2_TTF_VERSION}/SDL2_ttf-${SDL2_TTF_VERSION}.zip -o SDL2_ttf-${SDL2_TTF_VERSION}.zip
unzip SDL2_ttf-${SDL2_TTF_VERSION}.zip
cp -r SDL2_ttf-${SDL2_TTF_VERSION}/SDL_ttf.h ../external/include/SDL2
cd SDL2_ttf-${SDL2_TTF_VERSION}
cp ../../harfbuzz/hb-coretext.cc external/harfbuzz/src/
cmake -DBUILD_SHARED_LIBS=OFF \
        -DSDL2TTF_SAMPLES=OFF \
        -DSDL2_INCLUDE_DIR=$(pwd)/../SDL2-${SDL2_VERSION}/include \
        -DSDL2_LIBRARY=$(pwd)/../SDL2-${SDL2_VERSION}/build/libSDL2.a \
        -DSDL2TTF_VENDORED=ON \
        -DSDL2TTF_HARFBUZZ=ON \
	-DCMAKE_OSX_ARCHITECTURES=arm64 \
        -DCMAKE_BUILD_TYPE=Release \
        -B build 
cmake --build build
cp build/libSDL2_ttf.a ../../external/lib
cd ..

#
# build libpinmame and copy to external
#

curl -sL https://github.com/vpinball/pinmame/archive/refs/heads/master.zip -o pinmame.zip
unzip pinmame.zip
cd pinmame-master
cp src/libpinmame/libpinmame.h ../../external/include
cp cmake/libpinmame/CMakeLists_osx-arm64.txt CMakeLists.txt
cmake -DCMAKE_BUILD_TYPE=Release -B build
cmake --build build
cp build/libpinmame.3.6.dylib ../../external/lib
cd ..
