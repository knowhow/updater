
ARCH=`uname -m`
UBUNTU_VER=`lsb_release -a 2> /dev/null| grep Release | awk '{print $2;}'`

ART_PKG_EXE=knowhowERP_package_updater
 
ARTIFAKT_VER=`git describe --tags --abbrev=0`
ARTIFAKT_EXT=gz
 
PACKAGE=${ART_PKG_EXE}_ubuntu_${UBUNTU_VER}_${ARCH}_${ARTIFAKT_VER}.${ARTIFAKT_EXT}
 
echo $PACKAGE
mv bin/updater $ART_PKG_EXE

gzip $ART_PKG_EXE
mv $ART_PKG_EXE.gz $PACKAGE

ls -l $PACKAGE
