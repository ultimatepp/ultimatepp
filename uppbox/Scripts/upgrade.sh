# This is generic upgrade script. It should be never used directly,
# always source it from another script that sets up the necessary
# variables. See plugin/jpg or plugin/png for example of usage.

# run configure
(cd "$CONFIGURE"; ./configure $OPTS)
# get information about necessary files
echo 'print_sources:' >> "$SRC/Makefile"
echo '	'$SOURCES >> "$SRC/Makefile"
FILES="$(make --no-print-directory -C $SRC print_sources) $ADD_FILES"
H_FILES="$(echo $FILES" " | grep -o '[^ ]*[.]h ' | sort -u)"
C_FILES="$(echo $FILES" " | grep -o '[^ ]*[.]c ' | sort -u)"

# clean destination
rm -r "$DST" || true
mkdir -p "$DST"

# copy headers
for f in $H_FILES; do
    echo ", $DST/$f" >> "$UPP"
  {
    GUARD="${GUARD_PREFIX}${DST}_$(echo $f | tr '.' '_')_"
    echo '#ifndef '$GUARD
    echo '#define '$GUARD
    echo '#if defined(flag'$FLAG') || defined(flagWIN32)'
    cat "$SRC/$f"
    echo '#endif'
    echo '#endif'
  } > "$DST/$f"
done

# copy source files
for f in $C_FILES; do
    echo ", $DST/$f" >> "$UPP"
  {
    echo '#if defined(flag'$FLAG') || defined(flagWIN32)'
    cat "$SRC/$f"
    echo '#endif'
  } > "$DST/$f"
done

# copy additional files
for f in $COPY_FILES; do
    cp "$SRC/$f" "$DST/$f"
done

# add documentation
[ -n "$DOC_FILES" ] && echo ",Info readonly separator" >> "$UPP"
for f in $DOC_FILES; do
    echo ", $DST/$(basename $f)" >> "$UPP"
    cp "$SRC/$f" "$DST/$(basename $f)"
done

# close the .upp file
echo ";" >> "$UPP"
