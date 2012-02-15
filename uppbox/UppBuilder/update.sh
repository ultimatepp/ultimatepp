#!/bin/sh
# This script compresses the parser and embeds it into 
# the Makefile, using Makefile.in template
#
# Trick to hide the silly warnings caused by windows line ends
file $0 | grep -q CRLF && sed -i 's/\r//' $0 && exec $SHELL $0; 

echo Updating Makefile

# output file can be overriden by MAKEFILE_OUT variable
[ "$MAKEFILE_OUT" ] || MAKEFILE_OUT="/tmp/Makefile"

{
	cat Makefile.in
	
	echo '### PARSER CODE (gzipped and base64 encoded) ###'
	{
		#we perform the '#include' first
		sed '/#include PROCESS/,$d' Parser.cpp
		cat Make.hpp
		sed '1,/#include PROCESS/d' Parser.cpp
	} | gzip -9 | base64 -w74 | sed 's/.*/#:&/;'
	
	echo '### BRC parser (gzipped and base64 encoded) ###'
	cat brc.sh | gzip -9 | base64 -w74 | sed 's/.*/#~&/;'
	
	echo '### Colorizer (gzipped and base64 encoded) ###'
	cat color.sh | gzip -9 | base64 -w74 | sed 's/.*/#%&/;'
} > "$MAKEFILE_OUT"

# fix line endings
sed -i 's/\r//;' "$MAKEFILE_OUT"

# little trick to ensure Makefile is always "rebuild"
sh -c 'sleep 2 && touch Makefile.in'&

exit 0 # comment this line out to see some compression statistics

stat() {
	printf "%s  %5i -> %5i  %3i -> %3i\n" "$1"\
	`cat $2 | wc -c` `grep $3 "$MAKEFILE_OUT" | wc -c` \
	`cat $2 | wc -l` `grep $3 "$MAKEFILE_OUT" | wc -l`
}

echo "Compression info:"
printf "%s%12s%14s\n" "File      " bytes lines
stat "Parser    " Parser.cpp '^#:'
stat "Brc parser" brc.sh '^#~'
stat "Colorizer " color.sh '^#%'
stat "Total     " "Parser.cpp brc.sh color.sh" '^#[%~:]'
stat "Makefile  " Makefile.in '^'
