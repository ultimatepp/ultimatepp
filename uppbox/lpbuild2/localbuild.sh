#!/bin/sh
set -e
# This scripts is meant to create deb packages for stable releases. It will best
# work with ubuntu, but it should work on debian and even other distros, as long
# as pbuilder, debootstrap and devscripts packages are available.

# set desired architectures and distributions
archs="i386 amd64"
dists="karmic lucid maverick natty oneiric squeeze wheezy sid"

# prepare source packages
stable=1
noupload=1
scriptpath="$(cd ${0%/*} 2>/dev/null || true; echo $PWD/${0##*/})"
. "$(dirname $scriptpath)/lpbuild.sh"

for dist in $dists; do
  for arch in $archs; do
    # create the chroot if it doesn't exist yet
    if [ ! -f "/var/cache/pbuilder/$NAME-base.tgz" ]; then
      DIST=$dist ARCH=$arch pbuilder --create --configfile "$scriptdir/pbuilderrc" --distribution $dist --architecture $arch
    fi;
    # build the binary packages
    DIST=$dist ARCH=$arch pbuilder --build --configfile "$scriptdir/pbuilderrc" --distribution $dist --architecture $arch "$TMP/upp_${version}-0~${dist}0.dsc"
  done
done

echo "Done!"
