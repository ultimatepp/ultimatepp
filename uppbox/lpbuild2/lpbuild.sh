#!/bin/sh

# Launchpad.net ubuntu packages builds
# This script expects configuration file as parameter (see e.g. nightly.cfg)

[ ! "$1" ] && echo "Usage: $0 file.cfg" > /dev/stderr && exit 1
set -e

# read the variables from config file
. "$1"

# create the source packages
DISTS="$LP_DISTS"
TMP="$LP_TMP"
. "$RES_DIR/dsccreate.sh"

for dist in $LP_DISTS; do
  dput $DPUT_OPTS "upp-$dist" "$TMP/upp_${VERSION}-${BUILD}~${dist}0_source.changes"
done
