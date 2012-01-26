#!/bin/sh

# PRODUCTION CONFIGURATION
GPG_PROG="/home/upp/bin/gpg-extpass"   # what program to use for signing (here a wrapper that supplies passphrase)
GPG_KEY="0CE4194D"                     # gpg key fingerprint to use
TMP="~/lpbuild.tmp"                    # path for temporary files
SRCDIR="~/upp.tmp/u"                   # where to look for source tarball

## DEBUG CONFIGURATION
#GPG_PROG="/usr/bin/gpg-extpass"
#GPG_KEY="13A907FC"
#TMP="/tmp/upp"
#SRCDIR="/home/h"
set -e

version="$(ls $SRCDIR/upp-x11-src-*.tar.gz | sed '$s/.*upp-x11-src-\([0-9]*\)[.]tar[.]gz/\1/')"
tmp="$TMP/upp-$version"

if [ ! "$version" ]; then
    echo "Source tarball not found in $SRC!"
    exit 1
elif [ -d "$tmp" ]; then
    echo "Version $version was already built once!"
    echo " (If you are trying to build it again on purpose,"
    echo "  delete directory '$tmp' manually)"
    exit 1
fi

dists="karmic lucid maverick natty oneiric"
scriptpath="$(cd ${0%/*} 2>/dev/null || true; echo $PWD/${0##*/})"
scriptdir="$(dirname $scriptpath)"
src="$SRCDIR/upp-x11-src-$version.tar.gz"

rm -rf "$TMP"
mkdir -p "$tmp"

cp -r "$scriptdir/debian" "$tmp/debian"
for f in Makefile mkfile copyright theide.1 theide.desktop; do
    cp "$scriptdir/$f" "$tmp/$f"
done
[ "$1" = "stable" ] && sed -i '/code.google/s/^/#/;/sf.net/s/^#//;' "$tmp/debian/watch"
ln -s "$src" "$TMP/upp_$version.orig.tar.gz"

cd $tmp
tar -xzf "$TMP/upp_$version.orig.tar.gz"
mv "$tmp/upp-x11-src-$version"/{bazaar,examples,reference,tutorial,uppsrc} "$tmp/"
rm -r "$tmp/upp-x11-src-$version"

set +e
for dist in $dists; do
    distver=$version-0~$dist"0"

    cat > "debian/changelog" << EOF
upp ($distver) $dist; urgency=low

  * Updated to SVN version $version
  * See http://www.ultimatepp.org/redmine/projects/upp/repository for complete
    and detailed list of changes.

 -- Jan Dolinar <dolik.rce@seznam.cz>  `date -R`
EOF

    [ "$skiporig" ] || dpkg-source --commit . packaging packaging.patch
    dpkg-buildpackage -d -S -k$GPG_KEY -m'Jan Dolinar <dolik.rce@seznam.cz>' -sgpg -p$GPG_PROG $skiporig
    dput -c "$scriptdir/dput.cfg" "upp-$dist" "../upp_"$distver"_source.changes"
    skiporig="-sd"
done
