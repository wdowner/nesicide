#!/bin/bash

echo Cleaning NESICIDE...
( cd ide; mingw32-make distclean )
echo Cleaning FamiTracker...
( cd famitracker; mingw32-make distclean )
echo Cleaning FamiPlayer...
( cd famiplayer; mingw32-make distclean )
echo Cleaning NES Emulator...
( cd nes-emulator; mingw32-make distclean )
echo Removing deps...
if [ "$1" == "deps" ]; then
  ( cd ..; rm -rf deps )
fi