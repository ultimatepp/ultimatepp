# U++ POSIX/X11 installation


Standard POSIX/X11 distribution of U++ comes as a source tarball. If you want to run U++ graphical environnement (theide) or run U++ command line tool (umk), you will first need to compile and install U++.


## Build Requires

Before compiling U++ source code, you must install a few developpement packages. Many POSIX/X11 distributions provides developpement packages with the same names. Sometimes, developpement package names don't match. You will have to find the corresponding names for your distribution.


### Build requires per distribution


#### Debian/apt-get based distributions

Build requires: g++  make  libgtk-3-dev  libnotify-dev  libbz2-dev  sox

How to install them:

if sudo is available and enabled on your distribution, copy/paste this in a terminal:
```
sudo apt-get install  g++  make  libgtk-3-dev  libnotify-dev  libbz2-dev  sox
```

if sudo is not available:
```
su -c 'apt-get install  g++  make  libgtk-3-dev  libnotify-dev  libbz2-dev  sox'
```

The 'buildrequires.debian' file in U++ tarball contains apt-get commands to do the same thing. You can use this file by running 'sh buildrequires.debian' as root (e.g. 'sudo sh buildrequires.debian' if sudo is available or 'su -c "sh buildrequires.debian"' if not).


#### Fedora based distributions

Build requires:  gtk3-devel  pango-devel  atk-devel  cairo-devel  libnotify-devel  bzip2-devel xorg-x11-server-devel  freetype-devel  expat-devel

How to install them:

if sudo is available and enabled on your distribution, copy/paste this in a terminal:
```
sudo yum install  gtk3-devel  pango-devel  atk-devel  cairo-devel  libnotify-devel  bzip2-devel xorg-x11-server-devel  freetype-devel  expat-devel
```
if sudo is not available:
```
su -c 'yum install  gtk3-devel  pango-devel  atk-devel  cairo-devel  libnotify-devel  bzip2-devel xorg-x11-server-devel  freetype-devel  expat-devel'
```

The 'buildrequires.fedora' file in U++ tarball contains yum commands to do the same thing. You can use this file by running 'sh buildrequires.fedora' as root (e.g. 'sudo sh buildrequires.fedora' if sudo is available or 'su -c "sh buildrequires.fedora"' if not).


#### Other rpm based distributions

Redhat 7 build requires:  gtk3-devel pango-devel atk-devel cairo-devel libnotify-devel freetype-devel expat-devel bzip2-devel
OpenSuse build requires: gtk3-devel pango-devel atk-devel cairo-devel libnotify-devel xorg-x11-devel freetype2-devel libexpat-devel libbz2-devel


#### BSD based distributions

Build requires: bash gmake gtk3 freetype2 libnotify clang-devel (e.g. clang++)

How to install them:

if sudo is available and enabled on your distribution, copy/paste this in a terminal:
```
sudo pkg install bash gmake gtk3 freetype2 libnotify clang-devel
```
if sudo is not available:
```
su -m root -c 'pkg install bash gmake gtk3 freetype2 libnotify clang-devel'
```


## Compile U++ source code


### Standard U++ compilation

First, uncompress U++ source tarball and change dir to the new created directory.

Example: for upp-x11-src-**10641**.tar.gz
```
version=10641
tar zxvf upp-x11-src-$version.tar.gz
cd upp-x11-src-$version
```

Use 'make' to compile U++ and generate theide (U++ integrated development environment) and umk (commandline tool for building U++ projects) then run 'make install' to prepare standard U++ environment:
```
make
make install
```

Now you can start playing with U++ by invoking ~/theide.
You might want to put theide and umk elsewhere later, e.g. inside ~/bin/ for example

If you only want to build umk or theide, run make with the corresponding target:
```
make umk
# or
make theide
```

Note: 'make install' copy theide and umk in your home directory but it also:

* create ~/upp directory to store U++ library sources and copy the U++ sources inside
* create ~/upp/MyApps to store your application sources
* create ~/upp.out as output for intermediate files
* set up a few variables in the '~/.upp' directory. Those variables are required by umk and theide


## Advanced installation

You can install umk and theide like most other POSIX project do. If make detects that you has defined the prefix variable, it will switch to standard POSIX installation mode.
```
make
make install prefix="/usr"
```
You can also use several other standard installation variables in this installation mode: 'DESTDIR', 'bindir', 'datadir', 'mandir', and 'docdir'.


## Troubleshooting

If your POSIX/X11 distribution use an old gcc version (< 4.9), U++ compilation will fail because of missing gcc c++11 standard implementation. To solve this, you need to install and use clang++ compiler instead of g++.

Make search for g++ first and if gcc version is too old, it will automatically search for clang++ and then for any compiler named 'c++'. If you still need to force clang++ as default compiler or if clang++ is not in your path or if you want to use another compiler, you can run make with the CXX parameter. Example:
```
make CXX="/home/user/my-clang-install-dir/clang++"
make install
```
On BSD based distributions, if you use make instead of gmake, U++ compilation will fail because BSD 'make' needs four dollars ('$$$$') to escape one. To solve this, you can install gmake or you can run make with an extra parameter if you use a recent U++ snapshot. Example (for bash shell):
```
make 'Dollar=$$$$'
make install
```
Those file names are already escaped for gmake. This is why they already use two dollars (Dollar = $$).


## U++ spec file for rpm based distribution

There is an alternative way to build U++ on rpm based distributions. Indead, U++ POSIX/X11 tarball contains a spec file for you to build a standard rpm binary and source file. To do that, first install U++ build requires and rpm-build then build U++.

### Fedora based distributions

if sudo is available and enabled on your distribution, copy/paste this in a terminal:

```
sudo yum install  gtk3-devel  pango-devel  atk-devel  cairo-devel  libnotify-devel  bzip2-devel xorg-x11-server-devel  freetype-devel  expat-devel
sudo yum install  rpm-build
version=10641
rpmbuild -ta upp-x11-src-$version.tar.gz
```

if sudo is not available:
```
su -c 'yum install  gtk3-devel  pango-devel  atk-devel  cairo-devel  libnotify-devel  bzip2-devel xorg-x11-server-devel  freetype-devel  expat-devel'
su -c 'yum install  rpm-build'
version=10641
rpmbuild -ta upp-x11-src-$version.tar.gz
```

Note : the rpm binary doesn't install U++ source in your home directory nor does it create needed configuration for U++ command line tool (umk) after installation. Theide will take care of all this on first start.
