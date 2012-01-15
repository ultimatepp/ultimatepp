OCE -- OpenCascade Community Edition

BEWARE -- THIS PACKAGE REQUIRES (TO DATE 15/01/2012) A PATCHED IDE TO WORK !
Up to the time they'll applied, you can find the patches on forum.

This package MUST be completed with OCE git repository in 'oce' folder, as
sources are too big for Bazaar.
OCE can be fetched with git as following :

cd into OCE package main folder (the one that containe OCE.h file)
issue following command :

	git clone git://github.com/tpaviot/oce.git

As an alternative, sources can be fetched as a zip file here :

	https://github.com/tpaviot/oce/zipball/master

The important is that after fetching/unpacking the source tree a
folder named 'oce' inside the main Upp OCE folder appears.
Once done, the package is ready for compiling

NOTES :

- by now, for OCE you'll need USEMALLOC flag; OCE still has many memory allocation problems
- The bazaar OCETest package shows some very basic usage
- be *very* patient; compilation must be done without Blitz, so is quite slow !

Max

