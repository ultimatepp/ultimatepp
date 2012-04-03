#!/bin/sh

# This scripts is meant to create deb packages for stable releases. It will best
# work with ubuntu, but it should work on debian and even other distros, as long
# as pbuilder, debootstrap and devscripts packages are available.

# This script expects configuration file as parameter (see e.g. nightly.cfg)

[ ! "$1" ] && echo "Usage: $0 file.cfg" > /dev/stderr && exit 1
set -e

# read the variables from config file
. "$1"

# create the source packages
DISTS="$LOCAL_DISTS"
TMP="$LOCAL_TMP"
. "$RES_DIR/dsccreate.sh"

export PATH=$PATH:/usr/sbin

for dist in $LOCAL_DISTS; do
  for arch in $ARCHS; do
    # create the chroot if it doesn't exist yet
    if [ ! -f "/var/cache/pbuilder/$dist-$arch-base.tgz" ]; then
      sudo DIST=$dist ARCH=$arch pbuilder --create --configfile "$RES_DIR/pbuilderrc" --distribution $dist --architecture $arch
      #import key into newly created chroot
      echo "apt-key adv --keyserver keyserver.ubuntu.com --recv-keys $GPG_KEY" > "$TMP/keyscript.sh"
      chmod a+x "$TMP/keyscript.sh"
      sudo DIST=$dist ARCH=$arch pbuilder --execute --save-after-exec --configfile "$RES_DIR/pbuilderrc" --distribution $dist --architecture $arch -- "$TMP/keyscript.sh"
      rm -f "$TMP/keyscript.sh"
    fi;
    # build the binary packages
    sudo DIST=$dist ARCH=$arch pbuilder --build --configfile "$RES_DIR/pbuilderrc" --distribution $dist --architecture $arch --debbuildopts "-j$JOBS " "$TMP/upp_${VERSION}-${BUILD}~${dist}0.dsc"
  done
done
