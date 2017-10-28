#!/bin/bash

LIBDEPS="../deps/rtmidi/release/rtmidi \
     ../deps/qscintilla2/Qt4Qt5/release/qscintilla2_qt5 \
     ../deps/Windows/SDL/SDL.dll \
     ../libs/nes/release/nes-emulator \
     ../libs/c64/release/c64-emulator \
     ../libs/famitracker/release/famitracker"

DEPLOYS_SRC="../apps/ide/release/nesicide.exe \
        ../apps/famitracker/release/famitracker.exe \
        ../apps/famiplayer/release/famiplayer.exe \
        ../apps/nes-emulator/release/nes-emulator.exe"

DEPLOYS_DEST="./dist/nesicide.exe \
        ./dist/famitracker.exe \
        ./dist/famiplayer.exe \
        ./dist/nes-emulator.exe"

rm -rf ./dist
mkdir -pv ./dist
for DEPLOY in ${DEPLOYS_SRC}
do
   cp -v ${DEPLOY} ./dist/
done
for f in ${LIBDEPS}
do 
   cp -v ${f}* dist/ 
done
for DEPLOY in ${DEPLOYS_DEST}
do
   DIST=$(basename $DEPLOY) 
   echo Deploying ${DIST}
   windeployqt ${DEPLOY} ${TARGARGS}
done
tar cjvf nesicide-win.tar.bz2 dist