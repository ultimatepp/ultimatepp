const char *readme =
///////////////////////////////////////////////////////////////////////////////
R"--(U++ POSIX Installation

U++ for POSIX (Linux, MacOS, OpenBSD, FreeBSD, OpenSolaris) comes as single sandboxed
archive. After downloading, you need to unpack this archive to suitable location with

tar xf upp-posix-14411.tar.xz

(replace 14411 with the revision you have downloaded). Of course, you can use
your desktop environment GUI desktop utility as well.

This will produce "upp" folder full of files.

cd upp

to get into it. Now you have two options:

- Use ./install script that should resolve all dependencies and build theide
  as fast as possible (usually the process takes a couple of minutes). Note that
  this does not work for OpenBSD (see bellow).

- Resolve dependecies yourself and use make to build theide. Even if you do this,
  peeking into ./install is a good idea to get a clue about dependecies
  required. You should also build umk command line too with
  make -f uMakefile, also this is not strictly necessarry for the first
  taste of U++. On non-Linux systems, you should run ./configure first.

At the end of this process, you should have two binaries in the folder, theide
and umk. Start theide to have some fun with U++.

As long as you keep theide (or umk) just in the original directory, nothing
will be written outside of it - its "sandboxed" (with one exception described
bellow). If you get bored with U++ and need to clean the space, simply delete
the "upp" folder.

If you move theide e.g. to ~/bin, it will start to write things to ~/.config
and ~/.cache.

The exception to the sanboxing rule is single command invoked from TheIDE menu,
Setup / Install theide.desktop. This will write proper .desktop file to
~/.local/share/applications - the effect of this on most desktop environments
is that TheIDE will appear in the desktop menu somewhere, usually in the Start
menu under Programming category. Sometimes desktop environment needs restart
for this to take effect. (This does not apply to MacOS).


OpenBSD specific notes
======================

In OpenBSD you need to resolve dependencies manually before running the ./install script by
running

pkg_add bash pkgconfig

(this should work as long as you have installed desktop environment before installing U++).

It is also necessary to increase the allowed memory sizes in /etc/login.conf to

default:\
	:datasize-max=4096M:\
	:datasize-cur=4096M:\
	:stacksize-cur=512M:\

otherwise compiler will run out of memory.

)--";

