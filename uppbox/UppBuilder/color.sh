#!/bin/sh
s='s/error:/\x1B[1;31m&\x1B[0m/ig;
s/warning:/\x1B[1;35m&\x1B[0m/ig;
s/note:/\x1B[1;32m&\x1B[0m/ig;
s/info:/\x1B[1;32m&\x1B[0m/ig;'
# redirection magic makes the sed command work on stdout 
# as well as on stderr, without mixing them up
f="`mktemp -t status.XXXXXX`"
{ { {
  $@
} ; echo $? > $f; } 3>&1 1>&2 2>&3 | sed "$s"; } 3>&1 1>&2 2>&3 | sed "$s"
read ret < $f
rm -f $f
exit $ret
