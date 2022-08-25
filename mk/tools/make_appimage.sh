#!/usr/bin/env bash

set -e

cd "$(dirname "$0")"/../..

mkdir -p build.appimage/
cd build.appimage/

cmake -DCMAKE_INSTALL_PREFIX=/usr ..
make -j$(nproc)
make install DESTDIR=AppDir

ln -s usr/share/applications/stmeltdown.desktop AppDir/stmeltdown.desktop
ln -s usr/share/icons/hicolor/scalable/apps/stmeltdown.svg AppDir/stmeltdown.svg

echo '#!/bin/sh' > AppDir/AppRun
echo 'APPDIR="$(dirname "$(readlink -f "$0")")"' >> AppDir/AppRun
echo -n '"$APPDIR"/usr/bin/stmeltdown --data' >> AppDir/AppRun
echo ' "$APPDIR"/usr/share/stmeltdown $@' >> AppDir/AppRun
chmod +x AppDir/AppRun

#wget https://github.com/AppImage/AppImageKit/releases/download/continuous/appimagetool-x86_64.AppImage
#chmod +x appimagetool-x86_64.AppImage

#./appimagetool-x86_64.AppImage AppDir

wget https://github.com/linuxdeploy/linuxdeploy/releases/download/continuous/linuxdeploy-x86_64.AppImage
chmod +x linuxdeploy-x86_64.AppImage

./linuxdeploy-x86_64.AppImage --appdir AppDir --output appimage
