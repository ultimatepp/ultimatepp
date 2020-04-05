const char *readme =
R"--(U++ is rapid C++ based development framework and environment.

To start working with U++, you will have to install dependecies for your
platform and compile TheIDE.

You can do this manually - install dependencies and run "make". This will
create 'ide' executable in current directory.

Alternatively, there is 'install' script which will attempt to identify
your distro's packaging system, install required packages and then do the
required compilation. It will even try to use precompiled utility 'umks32'
to accelerate the build, if you allow it.
	
After the installation, you can start using U++ be starting TheIDE.
The framework is designed to stay sandboxed - as long you do not copy binaries
from the unpacked archive, it would not write anything outside it.
)--";

