#!/bin/bash

export SDL2_VERSION=2.26.3
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
cp ../../FreeImage/Makefile.gnu .
make -f Makefile.gnu
cp Dist/libfreeimage.a ../../external/lib/libfreeimage.a
cd ..

#
# download bass24 and copy to external
#

curl -s https://www.un4seen.com/files/bass24-linux.zip -o bass.zip
unzip bass.zip 
mv libs/aarch64/libbass.so ../external/lib

#
# build SDL2 and copy to external
#

curl -sL https://github.com/libsdl-org/SDL/releases/download/release-${SDL2_VERSION}/SDL2-${SDL2_VERSION}.zip -o SDL2-${SDL2_VERSION}.zip
unzip SDL2-${SDL2_VERSION}.zip
cp -r SDL2-${SDL2_VERSION}/include ../external/include/SDL2
cd SDL2-${SDL2_VERSION}
cmake -DSDL_SHARED=ON \
	-DSDL_STATIC=OFF \
	-DSDL_TEST=OFF \
	-DSDL_X11=OFF \
	-DCMAKE_BUILD_TYPE=Release \
	-B build
cmake --build build
cp build/libSDL2-2.0.so.0 ../../external/lib
cd ..

#
# build SDL2_ttf and copy to external
#

curl -sL https://github.com/libsdl-org/SDL_ttf/releases/download/release-${SDL2_TTF_VERSION}/SDL2_ttf-${SDL2_TTF_VERSION}.zip -o SDL2_ttf-${SDL2_TTF_VERSION}.zip
unzip SDL2_ttf-${SDL2_TTF_VERSION}.zip
cp -r SDL2_ttf-${SDL2_TTF_VERSION}/SDL_ttf.h ../external/include/SDL2
cd SDL2_ttf-${SDL2_TTF_VERSION}
cmake -DBUILD_SHARED_LIBS=ON \
        -DSDL2TTF_SAMPLES=OFF \
        -DSDL2_INCLUDE_DIR=$(pwd)/../SDL2-${SDL2_VERSION}/include \
        -DSDL2_LIBRARY=$(pwd)/../SDL2-${SDL2_VERSION}/build/libSDL2-2.0.so \
        -DSDL2TTF_VENDORED=ON \
        -DSDL2TTF_HARFBUZZ=ON \
        -DCMAKE_BUILD_TYPE=Release \
        -B build
cmake --build build
cp build/libSDL2_ttf.so ../../external/lib
cd ..

#
# build libpinmame and copy to external
#

curl -sL https://github.com/vpinball/pinmame/archive/refs/heads/master.zip -o pinmame.zip
unzip pinmame.zip
cd pinmame-master
cp src/libpinmame/libpinmame.h ../../external/include
cp cmake/libpinmame/CMakeLists_linux-x64.txt CMakeLists.txt
cmake -DCMAKE_BUILD_TYPE=Release -B build
cmake --build build
cp build/libpinmame.so.3.6 ../../external/lib
cd ..

