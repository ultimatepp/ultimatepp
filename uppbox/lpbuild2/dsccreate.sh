#!/bin/sh

# This script takes some predefined variables and creates .dsc packages

# which version to build
VERSION="$(echo $SRC | sed 's/.*upp-x11-src-\([0-9]*\)[.]tar[.]gz/\1/')"

# copy resources
tmp="$TMP/upp-$VERSION"
mkdir -p "$tmp"
cp -r "$RES_DIR/debian" "$tmp/debian"
for f in Makefile mkfile copyright theide.1 umk.1 theide.desktop; do
    cp "$RES_DIR/$f" "$tmp/$f"
done

# link the original tarball
ln -s "$SRC" "$TMP/upp_$VERSION.orig.tar.gz"

# unpack the original tarball
tar -xzf "$TMP/upp_$VERSION.orig.tar.gz" -C "$tmp"
mv "$tmp/upp-x11-src-$VERSION"/{bazaar,examples,reference,tutorial,uppsrc} "$tmp/"
rm -r "$tmp/upp-x11-src-$VERSION"

# fix version.h
echo '#define IDE_VERSION    "'$VERSION'"' > "$tmp/uppsrc/ide/version.h"

# tweak watch file, if we build stable release
[ "$STABLE" = "yes" ] && sed -i '/ultimatepp.org/s/^/#/;/sf.net/s/^#//;' "$tmp/debian/watch"

workdir="$(pwd)"
cd "$tmp"

for dist in $DISTS; do
    # create changelog
    cat > "debian/changelog" << EOF
upp (${VERSION}-${BUILD}~${dist}0) $dist; urgency=low

  * Updated to SVN VERSION $VERSION
  * See http://www.ultimatepp.org/redmine/projects/upp/repository for complete
    and detailed list of changes.

 -- $MAINTAINER  `date -R`
EOF
    # build source packages
    dpkg-buildpackage -S $BUILDPKGOPTS -m"$MAINTAINER" $skiporig
    skiporig="-sd"
done

cd "$workdir"
