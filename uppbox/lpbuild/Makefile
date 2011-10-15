########################################################################
#     __  __  __   ____  __  __  __   __  ____  ___     _      _       #
#    (  )(  )(  ) (_  _)(  )(  \/  ) (  )(_  _)(  _)  _( )_  _( )_     #
#     )(__)(  )(__  )(   )(  )    (  /__\  )(   ) _) (_   _)(_   _)    #
#     \____/ (____)(__) (__)(_/\/\_)(_)(_)(__) (___)   (_)    (_)      #
#        ___  ___    __   __  __  ___  _    _  __   ___   _  _         #
#       (  _)(  ,)  (  ) (  \/  )(  _)( \/\/ )/  \ (  ,) ( )/ )        #
#        ) _) )  \  /__\  )    (  ) _) \    /( () ) )  \  )  \         #
#       (_)  (_)\_)(_)(_)(_/\/\_)(___)  \/\/  \__/ (_)\_)(_)\_)        #
#              __  __   __   _  _  ___  ___  __  __    ___             #
#             (  \/  ) (  ) ( )/ )(  _)(  _)(  )(  )  (  _)            #
#              )    (  /__\  )  \  ) _) ) _) )(  )(__  ) _)            #
#             (_/\/\_)(_)(_)(_)\_)(___)(_)  (__)(____)(___)            #
#                                                                      #
########################################################################

############################## USAGE ###################################
# This makefile is controlled through a set of variables, similar to   #
# any other makefile. The prefered way to change them is via command   #
# line, e.g.: make "PKG=ide usvn" CC=g++                               #
#                                                                      #
# All of the variables that can control the bahaviour are listed below #
# in DEFAULTS section. Also their default value is shown here.         #
#                                                                      #
# For boolean type of variables, values of y, Y, yes, YES, Yes, true,  #
# True, TRUE, T, t, 1, on, On, ON and @ are considered true. Anything  #
# else is evaluated as false.                                          #
############################# TARGETS ##################################
# This makefile defines four targets that are meant to be used by user #
# from the command line:                                               #
#                                                                      #
# all    - builds all (default if any packages are selected)           #
# help   - shows this help (default if no packages are selected)       #
# clean  - deletes OBJDIR                                              #
# export - exports preparsed Makefile to file EXPORT                   #
############################# EXAMPLES #################################
# Typical usage:                                                       #
#    make PKG=ide FLAGS="GUI MT"                                       #
# More complicated usage:                                              #
#    make PKG="ide UWord Bombs" FLAGS="GUI GCC NOGTK" VERBOSE=y        #
# Building "theide" instead of "ide":                                  #
#    make PKG=ide BINPREFIX=bin/the                                    #
# Export makefile with full dependencies:                              #
#    make export PKG=ide FAST=n                                        #
# Show help:                                                           #
#    make OR make help                                                 #
# Simulation mode:                                                     #
#    make PKG=ide SIMULATE=y                                           #
# Silent mode:                                                         #
#    make PKG=ide SILENT=y                                             #
# Paralel compilation (3 paralel processes):                           #
#    make PKG=ide JOBS=3                                               #
############################# DEFAULTS #################################
# List of packages to build (space separated).                         #
# If empty, this help text will be shown.                              #
PKG=
# Paths where to look for packages.                                    #
NESTS=bazaar examples reference tutorial uppsrc
# Flags as in TheIDE.
# The only difference is, that if you use SPEED or SIZE flag, then     #
# the application will be optimized for Speed or Size, respectively.   #
# Flags STATIC and ALLSHARED will determine the linking mode.          #
FLAGS=GCC
# Additional include paths for compiler (without  leading "-I").       #
INCPATHS:=
# Paths to libraries for linker.                                       #
LIBPATHS:=
# Directory to store intermediate object files                         #
OBJDIR:= _out
# Extension added to resulting executables                             #
BINEXT:=
# Prefix for resulting binaries. Usually used with "/" at the end,     #
# to specify the directory (hence the name)                            #
BINPREFIX:=bin/
# ar command                                                           #
AR:= ar -src
# C compiler command                                                   #
CC:= c++
# C++ compiler command                                                 #
CXX:=$(CC)
# Options for C compiler                                               #
CFLAGS:=
# Options for C++ compiler                                             #
CXXFLAGS:=
# Options for linker                                                   #
LDFLAGS:=-Wl,--gc-sections -Wl,-s -Wl,-O,2
# Additional options for speed optimization                            #
SPEEDFLAGS:=-O3 -ffunction-sections -fdata-sections
# Additional options for size optimization                             #
SIZEFLAGS:=-Os -finline-limit=20 -ffunction-sections -fdata-sections
# Platform flag, added to FLAGS                                        #
PLATFORM:=$(shell uname | tr a-z A-Z) POSIX
# Suppress all messagges                                               #
SILENT:=
# Print each executed command                                          #
VERBOSE:=
# Print each executed command, prepended with "SIMULATE: ", without    #
# actually executing it.                                               #
SIMULATE:=
# Do not make full dependency scan (using gcc -M switch).              #
# This saves a lot of time and usually doesn't make any harm.          #
# FAST mode must by turned on only if you modified include files (*.h, #
# *.lay, ...) without modifying the *.c/cpp files that include them.   # 
FAST:=y
# Add flags from based on mainconfig section (not implemented yet)     #
USEMAINCFG:=
# Number of parallel jobs to use (like "make -jN" option).             #
# The JOBS variable must be used instead of -j option (as -j would not #
# work as expected). Default value is the number of cores (if it is    #
# possible to detect) or 1.                                            #
JOBS:=$(shell echo /sys/devices/system/cpu/cpu[0-9] | wc -w || echo 1)
# External parser to use instead of the contained one. Also used       #
# for manipulation with the internal parser (see update-parser,        #
# and export-parser targets in this Makefile for details).             #
PARSER:=
# Exported makefile (used only with export target)                     #
# If multiple packages are being exported, each produces additional    #
# Makefile, which name is EXPORT.package_name                          #
EXPORT:=Makefile.export
# Awk binary. Used only for export.                                    #
AWK:=awk
########################################################################
# HELP END

ifeq ($(filter 3.8%, $(MAKE_VERSION)), )
$(error This Makefile only supports GNU make version 3.80 and newer)
endif

# no need to parse dependencies just to clean up or to read help etc.
ifeq ($(MAKECMDGOALS),clean)
skip-parse:=y
endif
ifeq ($(MAKECMDGOALS),help)
skip-parse:=y
endif
ifeq ($(MAKECMDGOALS),export)
skip-parse:=y
endif
ifeq ($(PKG),)
skip-parse:=y
endif

# Determine the name of this Makefile
filename:=$(word $(words $(MAKEFILE_LIST)),$(MAKEFILE_LIST))

# Helper functions to put variables on cmdline
pass-var= "$1=$$($1)"
export-var= "$1=$($1)"
# list of user variables to pass to child processes
cmdline-vars=NESTS FLAGS INCPATHS LIBPATHS OBJDIR BINEXT BINPREFIX AR CC CXX\
 CFLAGS CXXFLAGS LDFLAGS SPEEDFLAGS SIZEFLAGS PLATFORM\
 SILENT VERBOSE SIMULATE FAST USEMAINCFG
L:=(
R:=)
CONFIRM=y Y yes YES Yes true True T TRUE t 1 on On ON @
confirm=$(filter $(strip $1),$(CONFIRM))
# verbose/silent/fast mode switches
S:=$(call confirm,$(SILENT))
M:=$(if $(call confirm,$(VERBOSE)),,@)
M:=$(if $(call confirm,$(SIMULATE)),@echo SIMULATE: ,$M)
F:=$(call confirm,$(FAST))
# echo a string if silent mode is not on
echo=$(if $S,,@echo $1)

ifneq ($(skip-parse),y)

flags:=$(sort $(FLAGS) $(PLATFORM))
CINC:=$(foreach d,$(NESTS) $(INCPATHS), -I$d) -I/usr/include/freetype2 -I/usr/include/gtk-2.0 -I/usr/local/include/gtk-2.0 -I/usr/include/glib-2.0 -I/usr/local/include/glib-2.0 -I/usr/lib/glib-2.0/include -I/usr/local/lib/glib-2.0/include -I/usr/lib/gtk-2.0/include -I/usr/local/lib/gtk-2.0/include -I/usr/include/cairo -I/usr/include/pango-1.0 -I/usr/include/atk-1.0 -I/usr/X11R6/include -I/usr/X11R6/include/freetype2 -I/usr/X11R6/include/gtk-2.0 -I/usr/X11R6/include/glib-2.0 -I/usr/X11R6/lib/glib-2.0/include -I/usr/X11R6/lib/gtk-2.0/include -I/usr/X11R6/include/cairo -I/usr/X11R6/include/pango-1.0 -I/usr/X11R6/include/atk-1.0 -I/usr/local/include/cairo -I/usr/local/include/pango-1.0 -I/usr/local/include/atk-1.0 -I/usr/local/include -I/usr/local/include/libpng -I/usr/include/gdk-pixbuf-2.0 -I/usr/lib/i386-linux-gnu/glib-2.0/include -I/usr/lib/x86_64-linux-gnu/glib-2.0/include -I/usr/lib/i386-linux-gnu/gtk-2.0/include -I/usr/lib/x86_64-linux-gnu/gtk-2.0/include -I/usr/include/gtk-3.0/gdk
LIBPATH:=$(foreach d,$(LIBPATHS), -L$d) -L/usr/X11R6/lib -L/usr/lib -L/usr/local/lib
needed-dirs:=
begin-group:=-Wl,--start-group
end-group:=-Wl,--end-group
# MacOsX specific things
ifeq ($(filter $(PLATFORM),DARWIN),DARWIN)
begin-group:=
end-group:=
endif
# Return true, if package $2 is to be compiled with flag $1
has-flag=$(filter $1,$2)
#Join array with "_"
concatenate=$(strip $(if $1,$(if $(filter $(words $1),1),$1,\
     $(firstword $1)_$(call concatenate,$(wordlist 2,$(words $1),$1)))))
# Return output directory for .o files, based on pkg name and flags
objdir=$(OBJDIR)/$1/$(call concatenate,$($1_FLAGS))
# prepare output directories targets
define create-dir
needed-dirs+= $1
$1:
	$(call echo,"    Creating directory $1 ...")
	$Mmkdir -p $1
endef
needs-dir=$(if $(filter $(needed-dirs),$1),,$(eval $(call create-dir,$1))) $1
# get dependencies recursively
get-deps=$(strip $(foreach p,$($1_USES),$(call get-deps,$p)) $1)

# Compiler commands
compiler-cpp=$(CXX) -c -x c++ $(CXXFLAGS)
compiler-c=$(CC) -c -x c $(CFLAGS)
analyze-info=$(if $S,,    Analyzing $1 dependencies ...)
includes-fetcher-c=$(info $(analyze-info))$(CC) -M -x c $(CXXFLAGS)
includes-fetcher-cpp=$(info $(analyze-info))$(CC) -M -x c++ $(CFLAGS)

# file counting utilities for progress reports
C:=$(OBJDIR)/.make_counter
incrementor:=while ! ln -s -T '$C' '$C_lock' 2>/dev/null;do true; done; echo -n $$$$(($$$$(cat '$C')+1))>'$C';cat '$C'; rm $C_lock;
counter-beg:=c=0; for f in 
counter-end:=; do [ -e $$f ] && c=$$(($$c+1)); done; echo $$c;

# general target scheme (args:{deps,target,pkg,lang,flags}
define compile
$2: $(if $F,$($3_NEST)/$1,$(dir $2)$(shell $(includes-fetcher-$4) $5 $(CINC) $($3_MACRO) $($3_NEST)/$1 |tr -d '\\\\' | sed 's/^[^:]*://')) | $(call needs-dir,$(dir $2))
	$(call echo,"    Compiling $1 $Lflags $(strip $($3_FLAGS))$R ["`$(incrementor)`"/$(file-num)] ...") 
	$M$(compiler-$4) $5 $(CINC) $($3_MACRO) $($3_NEST)/$1 -o $2
$(EMPTYLINE)
endef

#compile .cpp/.c/.icpp into .o/.io
compile-cpp=$(call compile,$1,$2,$3,cpp,)
compile-c=$(call compile,$1,$2,$3,c,)
compile-icpp=$(call compile,$1,$2,$3,cpp,)
# ditto with speed optimization
compile-cpp-speed=$(call compile,$1,$2,$3,cpp,$(SPEEDFLAGS))
compile-c-speed=$(call compile,$1,$2,$3,c,$(SPEEDFLAGS))
compile-icpp-speed=$(call compile,$1,$2,$3,cpp,$(SPEEDFLAGS))
# ditto with size optimization
compile-cpp-size=$(call compile,$1,$2,$3,cpp,$(SIZEFLAGS))
compile-c-size=$(call compile,$1,$2,$3,c,$(SIZEFLAGS))
compile-icpp-size=$(call compile,$1,$2,$3,cpp,$(SIZEFLAGS))

# call appropriate compile functions for all files
define compile-all
$(if $(call has-flag,SPEED,$($1_FLAGS)),\
  $(foreach f,$($1_FILES_CPP) $($1_FILES_CPP_SPEED) $($1_FILES_CPP_SIZE),$(call compile-cpp-speed,$1/$f,$($1_OBJDIR)/$(basename $f).o,$1))\
  $(foreach f,$($1_FILES_C) $($1_FILES_C_SPEED) $($1_FILES_C_SIZE),$(call compile-c-speed,$1/$f,$($1_OBJDIR)/$(basename $f).o,$1))\
  $(foreach f,$($1_FILES_ICPP) $($1_FILES_ICPP_SPEED) $($1_FILES_ICPP_SIZE),$(call compile-icpp-speed,$1/$f,$($1_OBJDIR)/$(basename $f).io,$1))\
,$(if $(call has-flag,SIZE,$($1_FLAGS)),\
  $(foreach f,$($1_FILES_CPP) $($1_FILES_CPP_SPEED) $($1_FILES_CPP_SIZE),$(call compile-cpp-size,$1/$f,$($1_OBJDIR)/$(basename $f).o,$1))\
  $(foreach f,$($1_FILES_C) $($1_FILES_C_SPEED) $($1_FILES_C_SIZE),$(call compile-c-size,$1/$f,$($1_OBJDIR)/$(basename $f).o,$1))\
  $(foreach f,$($1_FILES_ICPP) $($1_FILES_ICPP_SPEED) $($1_FILES_ICPP_SIZE),$(call compile-icpp-size,$1/$f,$($1_OBJDIR)/$(basename $f).io,$1))\
,\
  $(foreach f,$($1_FILES_CPP),$(call compile-cpp,$1/$f,$($1_OBJDIR)/$(basename $f).o,$1))\
  $(foreach f,$($1_FILES_C),$(call compile-c,$1/$f,$($1_OBJDIR)/$(basename $f).o,$1))\
  $(foreach f,$($1_FILES_ICPP),$(call compile-icpp,$1/$f,$($1_OBJDIR)/$(basename $f).io,$1))\
  $(foreach f,$($1_FILES_CPP_SPEED),$(call compile-cpp-speed,$1/$f,$($1_OBJDIR)/$(basename $f).o,$1))\
  $(foreach f,$($1_FILES_C_SPEED),$(call compile-c-speed,$1/$f,$($1_OBJDIR)/$(basename $f).o,$1))\
  $(foreach f,$($1_FILES_ICPP_SPEED),$(call compile-icpp-speed,$1/$f,$($1_OBJDIR)/$(basename $f).io,$1))\
  $(foreach f,$($1_FILES_CPP_SIZE),$(call compile-cpp-size,$1/$f,$($1_OBJDIR)/$(basename $f).o,$1))\
  $(foreach f,$($1_FILES_C_SIZE),$(call compile-c-size,$1/$f,$($1_OBJDIR)/$(basename $f).o,$1))\
  $(foreach f,$($1_FILES_ICPP_SIZE),$(call compile-icpp-size,$1/$f,$($1_OBJDIR)/$(basename $f).io,$1))\
))
endef

ifeq ($(single-package),y)

# prepare some variables neccesary early in the build process
define prep
  $(eval $1_FLAGS:=$(sort $($1_FLAGS) $(if $($1_SPEED),SPEED,$(if $($1_SIZE),SIZE,))))
  $1_MACRO:=$(foreach f,$($1_FLAGS), -Dflag$f)
  $1_OBJDIR:=$(call objdir,$1)
endef

# compile all .c/cpp files in package $1 and create .a
define build
$(eval $1_OBJFILES:=$(foreach f,$($1_FILES_CPP) $($1_FILES_CPP_SPEED) $($1_FILES_CPP_SIZE) $($1_FILES_C) $($1_FILES_C_SPEED) $($1_FILES_C_SIZE),$($1_OBJDIR)/$(basename $f).o))
$(eval $1_IOBJFILES:=$(foreach f,$($1_FILES_ICPP) $($1_FILES_ICPP_SPEED) $($1_FILES_ICPP_SIZE),$($1_OBJDIR)/$(basename $f).io))

$($1_OBJDIR)/$(notdir $1.a): $($1_OBJFILES)
	$(call echo,"    Creating archive $$@ ...")
	$M$(AR) $$@ $($1_OBJFILES)

$(eval $(call compile-all,$1))
endef

# create target to link libraries into executable
define link
$(eval ARCHIVES:=$(strip $(foreach u,$(ALLDEPS),$($u_IOBJFILES)) $(foreach u,$(ALLDEPS_),$($u_OBJDIR)/$(notdir $u.a))))
$(eval ALLLIBS:=$(sort $(foreach u,$(ALLDEPS_),$(foreach l,$($u_LIBS), -l$l))))
$(shell mkdir -p $(OBJDIR);rm -f $C_lock; echo -n `$(counter-beg) $(sort $(IOBJFILES) $(foreach u,$(ALLDEPS),$($u_OBJFILES))) $(counter-end)` > '$C')

$(BINPREFIX)$1$(BINEXT): $(ARCHIVES) $(call needs-dir,$(dir $(BINPREFIX)))
	$(call echo,"  Linking $$@ ...")
	$M$(CXX) -o $$@ $(if $(call has-flag,ALLSHARED,$($1_FLAGS)),-shared,$(if $(call has-flag,STATIC,$1),-static,)) $(LIBPATH) $(LDFLAGS) $(begin-group) $(ARCHIVES) $(ALLLIBS) $(end-group)
endef

endif # single-package

# call make to parse, compile and build package $1
define make-pkg
.PHONY: $1
$1:
	$(if $(call confirm,$(SIMULATE)),@echo -n "SIMULATE: ";,)
	$(if $(call confirm,$(SIMULATE)),,$M)+-\
      $(MAKE) -Rrf $(filename) $(if $(JOBS),-j$(JOBS) ,)--no-p PKG=$1 single-package=y $(foreach v,$(cmdline-vars),$(call pass-var,$v))
endef

.PHONY: default
default: all

# create targets for the main tasks
define main-targets
.PHONY: all start export

all: $(if $(single-package),start $(BINPREFIX)$(PKG)$(BINEXT),$(PKG))
	$(call echo,"$(if $(single-package),  Package $(PKG),Everything) done.")

start:
	$(call echo,"  Building package $(PKG)$(if $($(PKG)_DESC), [$($(PKG)_DESC)],) ($(file-num) files in $(words $(ALLDEPS)) packages, flags: $($(PKG)_FLAGS))")
endef

endif # if skip-parse

# create target to export makefile for package $1
define export-pkg
export_$1:
	$(if $(call confirm,$(SIMULATE)),@echo -n "SIMULATE: ";,)
	$(if $(call confirm,$(SIMULATE)),,$M)+-\
      $(MAKE) -rRf $(filename) --no-p single-package=y "EXPORT=$(EXPORT).$(subst /,_,$1)" PKG=$1 $(foreach v,$(cmdline-vars),$(call pass-var,$v)) export
endef

# create targets for help, clean up, export and developper targets
define additional-targets
.PHONY: help clean
help:
	@sed '/HELP.END/{s/.*//;q;}' $(filename)

clean:
	$(call echo,Deleting $(OBJDIR))
	$M rm -rf $(OBJDIR) || true

#For developers:
# print the parser to stdout (usage: make export-parser)
print-parser:
	@sed -n 's/^#://p;' $(filename)

# print the parser to PARSER (usage: make export-parser PARSER=parser.sh)
export-parser:
	@sed -n 's/^#://p;' $(filename) > $(PARSER)

# load $(PARSER) into this Makefile (usage: make update-parser PARSER=parser.new > Makefile.new)
update-parser:
	@sed -n '/^#:/!s/.*/&/p;' $(filename)
	@sed 's/.*/#:&/;' $(PARSER)

endef

#create export target
ifeq ($(words $(strip $(PKG))),1)
define export-targets
export:
	$(call echo,"  Exporting makefile for package $(PKG) to $(EXPORT) ...")
	$Mecho ".PHONY:default" > $(EXPORT)
	$Mecho "default: all" >>  $(EXPORT)
	$M$(MAKE) -RrpqBf $(filename) default "PKG=$(PKG)" single-package=y exporting=y $(foreach v,$(cmdline-vars),$(call export-var,$v)) \
        |sed -n '/^. Files/,$$$${/^[\x23]/!{/.c[p]*:$$$$/!H}};$$$${g;p;}'\
        >> '$(EXPORT)'
#        |sed -n '/^. Files/,$$$${/^[\x23]/!{/:$$$$/!H}};$$$${g;p;}'\

endef
else
define export-targets
export: $(foreach p,$(PKG),$(eval $(call export-pkg,$p)) export_$p)
	$(call echo,"Exporting control makefile to $(EXPORT)")
	$Mecho -n all:>'$(EXPORT)'
	$Mecho -n "$(foreach p,$(PKG),\n	$M+-$(MAKE) --no-p -f $(EXPORT).$(subst /,_,$p);)">>'$(EXPORT)'
endef
endif

# clean-out the default rules SUFFIXES, we manage everything based on upp files
.SUFFIXES:

ifneq ($(skip-parse),y)

ifeq ($(single-package),y)

# Parse the upp files
JOBS:=$(strip $(JOBS))
ifeq ($(JOBS),1)
$(if $S,,$(info Using 1 parallel job ...))
else
$(if $S,,$(info Using $(JOBS) parallel jobs ...))
endif
$(if $S,,$(info Parsing package files needed for $(PKG) ...))
ifeq ($(PARSER),)
$(eval $(shell sed -n 's/^#://p;' $(filename) | sh -s '$(PKG)' '$(NESTS)' '$(flags)'))
else
$(eval $(shell sh $(PARSER) '$(PKG)' '$(NESTS)' '$(flags)'))
endif
$(eval $(PKG)_FLAGS+=MAIN)
$(eval ALLDEPS_:=$(call get-deps,$(PKG)))
$(eval ALLDEPS:=$(sort $(ALLDEPS_)))
$(eval file-num:=$(words $(foreach p,$(ALLDEPS),$($p_FILES_CPP) $($p_FILES_ICPP) $($p_FILES_C) $($p_FILES_CPP_SPEED) $($p_FILES_ICPP_SPEED) $($p_FILES_C_SPEED) $($p_FILES_CPP_SIZE) $($p_FILES_ICPP_SIZE) $($p_FILES_C_SIZE))))
$(foreach p,$(ALLDEPS),$(eval $(call prep,$p)))

endif # single-package

# Call link on main package and create all directories
# and build targets needed for current task
# OR
# run makefile for each package from $(PKG)
$(if $(single-package),\
  $(foreach p,$(ALLDEPS),$(eval $(call build,$p)))\
  $(eval $(call link,$(PKG))),\
 $(foreach p,$(PKG),$(eval $(call make-pkg,$p))))

endif # not skip-parse

$(eval $(main-targets))
ifneq ($(exporting),y)
$(eval $(additional-targets))
$(eval $(export-targets))
endif

# Everything marked with '#:' is a shell script used to parse .upp files
#:NESTS="$2"
#:FLAGS="$3"
#:
#:# GENERAL FUNCTIONS
#:FindUpp(){
#:	pkg=`/bin/echo "$1" | sed -e 's|.*/||;'`
#:	for n in $NESTS
#:	do
#:		if [ -f "$n/$1/$pkg.upp" ]
#:		then
#:			FILE="$n/$1/$pkg.upp"
#:			NEST="$n"
#:			return
#:		fi
#:	done
#:}
#:PrintFiles() {
#:	local files="$1"
#:	shift
#:	local s
#:	while [ "x" != "x$1" ]
#:	do
#:		s=$s"/[.]"$2"\$/ {s/[.]"$2"\$/."$3"/;s|\"||g;s|^[ ]*||g;s|[ ]*\$||g;s|.*|\$(eval "$PKG"_"$1"+=&)|p;};"
#:		shift 3
#:	done
#:	/bin/echo "$files" | sed -n -e "$s"
#:}
#:Pop(){
#:	/bin/echo "$1"
#:}
#:Shift(){
#:	shift
#:	/bin/echo "$@"
#:}
#:
#:# SED PARSERS
#:# WARNING: Don't try to fix the ugly looking syntax of sed scripts - it is 
#:#          neccesary to make it work with BSD sed!
#:#
#:# Parser for booleans
#:p="/^description/! {"
#:# change (..(..)..) to (..[..]..)
#:p=$p"
#::a;
#:s/(\(.*\)(\([^()]*\))\(.*\))/(\1[\2]\3)/
#:t a;
#:"
#:# change (...) to ([...])
#:p=$p"s/(\(.*\))/([\1])/g;"
#:# delete spaces on left ...
#:p=$p":b;
#:s/\[\([^ ]*\)[ ][ ]*\([&|]\)\(.*\)\]/[\1\2\3]/;"
#:# ... and right side of operator
#:p=$p"s/\[\([^ ]*\)\([&|]\)[ ][ ]*\(.*\)\]/[\1\2\3]/;
#:t b;
#:"
#:# insert implicit &
#:p=$p":c;
#:s/\[\([a-zA-Z0-9_!&|]*\)[ ][ ]*\(.*\)\]/[\1\&\2]/;
#:t c;
#:"
#:# place {} around flags
#:p=$p"s/\([![&|]\)\([a-zA-Z0-9_][a-zA-Z0-9_]*\)/\1{\2}/g;"
#:# evalualte flags
#:p=$p"s/{\([^{}]*\)}/{\$(call has-flag,\1,$FLAGS)}/g;"
#:# evaluate negations
#:p=$p":d;
#:s/!{\([^{}]*\)}/{\$(if \1,,T)}/g;"
#:# evaluate ANDs
#:p=$p"s/{\([^{}]*\)}&{\([^{}]*\)}/{\$(call and,\1,\2)}/g;"
#:# evaluate ORs
#:p=$p"s/{\([^{}]*\)}|{\([^{}]*\)}/{\$(call or,\1,\2)}/g;"
#:# remove [] around already evaluated statements
#:p=$p"s/\[\({[^{}]*}\)\]/\1/g;
#:t d;
#:"
#:p=$p"s/({\([^{}]*\)})\(.*\)/\$(if \1,\2,)/g;
#:}"
#:
#:# File list parser
#:# strip quotes
#:f="s/\"//g;"
#:# remove separators
#:f=$f"s/,[ ]*[^,]*separator[ ]*,/,/g;"
#:# file.cpp optimize_xyz -> file.cpp_xyz
#:f=$f"s/,[ ]*\([^ ,]*\)[ ]*optimize_/,\1_/g;"
#:# drop modificators (charset, readonly, highlight, tabsize, font, ...)
#:f=$f"s/,[ ]*\([^ ,][^ ,]*\)[^,]*/,\1/g;"
#:# convert \ -> / and print
#:f=$f's/\\/\//g;p;'
#:
#:# Simple sections parser
#:# note: charset and blitz are ignored
#:Simples() {
#:	/bin/echo "$2" | sed -n -e '
#:	/^library[ ]*/ {s/^library[ ]*//;'"s|\"||g;s|^[ ]*||g;s|[ ]*\$||g;s|.*|\$(eval $1"_"LIBS+=&)|;p;};"'
#:	/^link[ ]*/ {s/^link[ ]*//;'"s|\"||g;s|^[ ]*||g;s|[ ]*\$||g;s|.*|\$(eval $1"_"LINK+=&)|;p;};"'
#:	/^acceptflags[ ]*/ {s/^acceptflags[ ]*//;'"s|\"||g;s|^[ ]*||g;s|[ ]*\$||g;s|.*|\$(eval $1"_"ACCEPT+=&)|;p;};"'
#:	/^mainconfig[ ]*/ {s/^mainconfig[ ]*//;'"s|\"||g;s|^[ ]*||g;s|[ ]*\$||g;s|.*|\$(eval $1"_"CFG+=&)|;p;};"'
#:	/^target[ ]*/ {s/^target[ ]*//;'"s|\"||g;s|^[ ]*||g;s|[ ]*\$||g;s|.*|\$(eval $1"_"TARGET+=&)|;p;};"'
#:	/^include[ ]*/ {s/^include[ ]*//;'"s|\"||g;s|^[ ]*||g;s|[ ]*\$||g;s|.*|\$(eval $1"_"INC+=&)|;p;};"'
#:	/^options[ ]*/ {s/^options[ ]*//;'"s|\"||g;s|^[ ]*||g;s|[ ]*\$||g;s|.*|\$(eval $1"_"OPTS+=&)|;p;};"'
#:	/^flags[ ]*/ {s/^flags[ ]*//;'"s|\"||g;s|^[ ]*||g;s|[ ]*\$||g;s|.*|\$(eval $1"_"FLAGS+=&)|;p;};"
#:}
#:
#:# THE MAIN PARSER FUNCTION
#:Parse(){
#:	PKG="$1"
#:	FindUpp "$PKG"
#:	/bin/echo '$(eval '$package'_NEST:='$NEST')'
#:	/bin/echo '$(eval '$package'_FLAGS+='$FLAGS')'
#:
#:	if [ -z "$FILE" ]
#:	then
#:		/bin/echo "ERROR: Package $PKG not found!"
#:		exit 1
#:	fi
#:	# get the .upp contents, parse the boolean expresions and translate them to the language of make
#:	UPP=`cat $FILE | tr "\n\r;" "  \n" | sed -e "s/^[ 	]*//g;s/[ 	]*\$//g;s/[ 	][ 	]*/ /g; $p"`
#:# FILE SECTION PARSER
#:	FILES=`/bin/echo "$UPP" | sed -n -e '/^'file'[ ]*/ {s/^'file'[ ]*//;s/.*/, & ,/;'"$f}" | tr "," "\n";`
#:	PrintFiles "$FILES" \
#:		FILES_CPP        cpp        cpp \
#:		FILES_CPP_SPEED  cpp_speed  cpp \
#:		FILES_CPP_SIZE   cpp_size   cpp \
#:		FILES_ICPP       icpp       icpp \
#:		FILES_ICPP_SPEED icpp_speed icpp \
#:		FILES_ICPP_SIZE  icpp_size  icpp \
#:		FILES_C          c          c   \
#:		FILES_C_SPEED    c_speed    c   \
#:		FILES_C_SIZE     c_size     c
#:# DESCRIPTION SECTION PARSER
#:	/bin/echo "$UPP" | sed -n -e '/^'description'[ ]*/{s/^'description'[ ]*//;s/^"\(.*\)"$/\1/;s/\\[^ ]*$//;'"s|\"||g;s|^[ ]*||g;s|[ ]*\$||g;s|.*|\$(eval "$PKG"_DESC=&)|p;}"
#:# USES SECTION PARSER
#:	USES=$( /bin/echo "$UPP" | sed -n -e '/^'uses'[ ]*/{s/^'uses'[ ]*//;s/\\/\//g;/^\$/!s/[ ]*,[ ]*/ /g;p;}' )
#:	/bin/echo "$USES" | sed -e "s|\"||g;s|^[ ]*||g;s|[ ]*\$||g;s|.*|\$(eval "$PKG"_USES+=&)|;"
#:# GLOBAL OPTIMIZATION FLAGS PARSER
#:	/bin/echo "$UPP" | sed -n -e 's|^optimize_speed[ ]*|$(eval '"$PKG"'_SPEED=T)|p;
#:	s|^optimize_size[ ]*|$(eval '"$PKG"'_SIZE=T)|p;'
#:# SIMPLE SECTIONS PARSERS
#:	Simples "$PKG" "$UPP"
#:}
#:
#:# MAIN LOOP
#:NEEDED="$1"
#:PARSED=""
#:while [ -n "$NEEDED" ]
#:do
#:	package=`Pop $NEEDED`
#:	NEEDED=`Shift $NEEDED`
#:	Parse $package;
#:	PARSED="$PARSED $package"
#:	uses=$( /bin/echo "$USES" | sed -e 's/\\/\//g;/^\$(/{s/.*, \([^, ]*\),[)]*$/\1/;}' )
#:	for dep in $uses
#:	do
#:		if [ -z "`/bin/echo " $NEEDED $PARSED " | grep " $dep "`" ]
#:		then
#:			NEEDED="$NEEDED $dep"
#:		fi
#:	done
#:done;
#:#
#:/bin/echo '$(eval PARSED:='"$PARSED"')'
