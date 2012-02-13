#!/bin/sh
# this script converts .brc file into .c file suitable for compilation
# usage: brc.sh file.brc > output.c

set -e
cd "`dirname $1`"
f="`basename $1`"

tmp="`mktemp -d brc_XXXXXX`"
sed 's/[(),]/ /g;s/\r//' $f > $tmp/preparsed

toBin() {
    case $2 in
        ZIP) obj=$tmp/obj.gz; cat "$1" | gzip -9 > $obj ;;
        BZ2) obj=$tmp/obj.bz2; cat "$1" | bzip2 -9 > $obj ;;
        *)   obj="$1" ;;
    esac
    hexdump -v -e '"\t" 16/1 "%i, " "\n"' $obj | sed '$s/[ ,]\+$/, 0/g'
    len="`cat $obj | wc -c`"
}

BINARY() {
    echo "static char $1_[] = {"
    toBin "$2" $3
    echo "};"
    echo "char *$1 = $1_;"
    echo "int $1_length = $len;";
}

BINARY_MASK() {
    c=0
    for file in $2; do
        BINARY_ARRAY $1 $c $file $3
        c=$(( $c + 1 ))
        files="$files\n\t\"$file\","
    done
    echo -e "char *bin_all_files[] = {$files\n};"
}

BINARY_ARRAY() {
    echo "static char $1_$2[] = {"
    toBin "$3" $4
    echo "};"
    [ -z "`echo $arrays | grep $1`" ] && arrays="$1 $arrays"
    echo "$1_$2 $len" >> $tmp/len
}

. $tmp/preparsed | tee $tmp/out

for a in $arrays; do
    c="`grep -c $a'_[0-9]\+' $tmp/out`"
    echo "int $a""_count = $c;"
    echo "int $a""_length[] = {"
    n=0
    while [ $n -lt $c ]; do
        sed -n "/$a""_$n"' /s/.* \([0-9]*\)/\t\1,/p' $tmp/len
        n=$(( $n + 1 ))
    done
    echo "};"

    echo "char *$a[] = { "
    grep -o $a'_[0-9]\+' $tmp/out | sed 's/^/\t/;s/$/,/;'
    echo " };"
done 

rm -r $tmp
