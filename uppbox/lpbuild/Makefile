########################################################################
#     __  __  __   ____  __  __  __   __  ____  ___     _      _       #
#    (  )(  )(  ) (_  _)(  )(  \/  ) (  )(_  _)(  _)  _( )_  _( )_     #
#     )(__)(  )(__  )(   )(  )    (  /__\  )(   ) _) (_   _)(_   _)    #
#     \____/ (____)(__) (__)(_/\/\_)(_)(_)(__) (___)   (_)    (_)      #
#        ___  ___    __   __  __  ___  _    _  __   ___   _  _         #
#       (  _)(  ,)  (  ) (  \/  )(  _)( \/\/ )/  \ (  ,) ( )/ )        #
#        ) _) )  \  /__\  )    (  ) _) \    /( () ) )  \  )  \         #
#       (_)  (_)\_)(_)(_)(_/\/\_)(___)  \/\/  \__/ (_)\_)(_)\_)        #
#              __  __   __   _  _  ___  ___  __  __    ___             #
#             (  \/  ) (  ) ( )/ )(  _)(  _)(  )(  )  (  _)            #
#              )    (  /__\  )  \  ) _) ) _) )(  )(__  ) _)            #
#             (_/\/\_)(_)(_)(_)\_)(___)(_)  (__)(____)(___)            #
#                                                                      #
########################################################################

############################## USAGE ###################################
# This makefile is controlled through a set of variables, similar to   #
# any other makefile. The preffered way to change them is via command  #
# line, e.g.: make "PKG=ide usvn" CC=g++                               #
#                                                                      #
# All of the variables that can control the bahaviour are listed below #
# in DEFAULTS section. Also their default value is shown here.         #
#                                                                      #
# For boolean type of variables, values of y, Y, yes, YES, Yes, true,  #
# True, T, t, 1, on, On, ON and @ are considered true, anything else   #
# is evaluated as false.                                               #
############################# TARGETS ##################################
# This makefile defines four targets that are meant to be used by user #
# from the command line:                                               #
#                                                                      #
# all    - builds all (default if any packages are selected)           #
# help   - shows this help (default if no packages are selected)       #
# clean  - deletes OBJDIR                                              #
# export - exports preparsed Makefile to file EXPORT                   #
############################# DEFAULTS #################################
# List of packages to build (space separated)                          #
# If empty, this help text will be shown.                              #
PKG=
# Paths where to look for packages (in U++ terminology DIRS=assembly   #
# and each path is a package nest)                                     #
DIRS=bazaar examples reference tutorial uppsrc
# Flags as in TheIDE.
# The only difference is, that if you use SPEED or SIZE flag, then     #
# the application will be optimized for Speed or Size, respectively.   #
# Flags STATIC and ALLSHARED will determine the linking mode.          #
FLAGS=GCC
# Additional include paths for compiler (without  leading "-I")        #
INCPATHS:=
# Paths to libraries for linker                                        #
LIBPATHS:=
# Directory to store intermediate object files                         #
OBJDIR:= _out
# Extension added to resulting executables                             #
BINEXT:=.out
# Prefix for resulting binaries. Usually used with "/" at the end,     #
# to specify the directory (hence the name)                            #
BINDIR:=
# ar command                                                           #
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
LDFLAGS:=-Wl,--gc-sections -Wl,-s
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
# FAST mode must by turned of only if you modified include files (*.h, #
# *.lay, ...) without modifying the *.c/cpp files that include them.   # 
FAST:=y
# Add flags from first line of mainconfig section                      #
USEMAINCFG:=
# Number of parallel jobs to use (like "make -jN" option).             #
# Please, use JOBS when building multiple packages rather than -j, as  #
# the later leads cluttered output and often also to performance loss. #
# When building a single package, this option is disregarded and you   #
# should use -j option.                                                #
# Empty value means single job (i.e. serial processing).               #
JOBS:=
# Exported makefile (used only with export target)                     #
# If multiple packages are being exported, each produces additional    #
# Makefile, which name is EXPORT.package_name                          #
EXPORT:=Makefile.export
# Awk binary. Used only for export.                                    #
AWK:=awk
########################################################################

ifeq ($(filter 3.8%, $(MAKE_VERSION)), )
$(error This Makefile only supports GNU make version 3.80 and newer)
endif

# if only one package is given, we can parse and compile,
# otherwise we call make on each given package 
ifeq ($(words $(PKG)),1)
single-package:=y
endif

# no need to parse dependencies just to delete a directory 
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

# Return last word of argument
lastword=$(word $(words $1),$1)

# Determine the name of this Makefile
filename:=$(call lastword,$(MAKEFILE_LIST))

# Helper functions to put variables on cmdline
pass-var= "$1=$$($1)"
export-var= "$1=$($1)"
# list of user variables to pass to child processes
cmdline-vars=DIRS FLAGS INCPATHS LIBPATHS OBJDIR BINEXT BINDIR AR CC CXX\
 CFLAGS CXXFLAGS LDFLAGS SPEEDFLAGS SIZEFLAGS PLATFORM\
 SILENT VERBOSE SIMULATE FAST USEMAINCFG


CONFIRM=y Y yes YES Yes true True T t 1 on On ON @
confirm=$(filter $(strip $1),$(CONFIRM))
# verbose/silent/fast mode switches
S:=$(call confirm,$(SILENT))
M:=$(if $(call confirm,$(VERBOSE)),,@)
M:=$(if $(call confirm,$(SIMULATE)),@echo SIMULATE: ,$M)
F:=$(call confirm,$(FAST))

# echo a string if silent mode is not on
echo=$(if $S,,@echo $1)

ifneq ($(skip-parse),y)

CINC:=$(foreach d,$(DIRS) $(INCPATHS), -I$d) -I/usr/include/freetype2 -I/usr/include/gtk-2.0 -I/usr/local/include/gtk-2.0 -I/usr/include/glib-2.0 -I/usr/local/include/glib-2.0 -I/usr/lib/glib-2.0/include -I/usr/local/lib/glib-2.0/include -I/usr/lib/gtk-2.0/include -I/usr/local/lib/gtk-2.0/include -I/usr/include/cairo -I/usr/include/pango-1.0 -I/usr/include/atk-1.0 -I/usr/X11R6/include -I/usr/X11R6/include/freetype2 -I/usr/X11R6/include/gtk-2.0 -I/usr/X11R6/include/glib-2.0 -I/usr/X11R6/lib/glib-2.0/include -I/usr/X11R6/lib/gtk-2.0/include -I/usr/X11R6/include/cairo -I/usr/X11R6/include/pango-1.0 -I/usr/X11R6/include/atk-1.0 -I/usr/local/include/cairo -I/usr/local/include/pango-1.0 -I/usr/local/include/atk-1.0 -I/usr/local/include -I/usr/local/include/libpng
LIBPATH:=$(foreach d,$(LIBPATHS), -L$d) -L/usr/X11R6/lib -L/usr/lib -L/usr/local/lib
needed-dirs:=$(BINDIR)

# UPP FILES PARSER
# read whole file to hold space end execute everything on the last line (multiline matching)
header:=1h;1!H;$${
footer:=}
# helpers:
s:=\t \x0A\x0D
del-section=g;s/;[$s]*$1[^;]*;/;/g;s/^$1[^;]*;/;/g;h;
sel:=s/^[^@]*@</@</; s/>@[^@]*$$/>@/; s/>@[^@]*@</>@\n@</g;
slash:=s/\\/\//g;
# PREPARSE:
# change (..(..)..) to (..[..]..)
p1:=g;:a;s/(\([^\n]*\)(\([^()]*\))\([^\n]*\))/(\1[\2]\3)/;t a;
# change (...) to ([...])
p2:=s/(\([^\n]*\))/([\1])/g;
# delete spaces on left ...
p3:=:b;s/\[\([^$s]*\)[$s][$s]*\([&|]\)\([^\n]*\)\]/[\1\2\3]/;
# ... and right side of operator
p4:=s/\[\([^$s]*\)\([&|]\)[$s][$s]*\([^\n]*\)\]/[\1\2\3]/;t b;
# insert implicit &
p5:=:c;s/\[\([a-zA-Z0-9_!&|]*\)[$s][$s]*\([^\n]*\)\]/[\1\&\2]/;t c;
# place {} around flags
p6:=s/\([![&|]\)\([a-zA-Z0-9_][a-zA-Z0-9_]*\)/\1{\2}/g;
# save preparsed to hold space
p7:=h;
# EVALUATION
# filter only library clauses
e1:=$(sel)
# evalualte flags
e2:=s/{\([^{}]*\)}/{$$(call has-flag,\1,$$1)}/g;
# fix unconditional cases
e3:=s/@<\([^()@]*\)>@/@<({T}) \1>@/g;
# evaluate negations
e4:=s/!{\([^{}]*\)}/{$$(if \1,,T)}/g;
# evaluate ANDs
e5:=s/{\([^{}]*\)}&{\([^{}]*\)}/{$$(call and,\1,\2)}/g;
# evaluate ORs
e6:=s/{\([^{}]*\)}|{\([^{}]*\)}/{$$(call or,\1,\2)}/g;
# remove [] around already evluated statements
e7:=s/\[\({[^{}]*}\)\]/\1/g;
# PARSER:
# mark contents of library clauses with @<..>@
u1=:u1$1;s/;[$s]*$1\([$s(][^;]*\);/;@<\1>@;/;
u1+=s/^$1\([$s(][^;]*\);/@<\1>@;/;t u1$1;
# delete commas, newlines and unnecessary spaces
u2:=s/[$s,][$s,]*/ /g;
# remove quotations, we do not need them
u3:=s/"\([^"@]*\)"/\1/g;
# evaluate boolean expressions
u4=$(e1)$(e2)$(e3):$1;$(e4)$(e5)$(e6)$(e7)t $1;
# print final evals
u5=$(slash)s/@<({\([^{}]*\)})\([^@]*\)>@/$$(eval $$1_$1+=$$(if \1,\2,))/gp;
# function that returns full parser
get-key=g;$(call u1,$1)$(u2)$(u3)$(call u4,$1)$(call u5,$2)$(call del-section,$1)

# Specialized parsers follow:
# DESCRIPTION:   (note: last description found in file is used)
d1=g;s/;[$s]*description[$s]*"\([^\n]*\)".*/$$(eval $$1_DESC:=\1 )/p;
d1+=s/^description[$s]*"\([^\n]*\)".*/$$(eval $$1_DESC:=\1 )/p;
# FILE:
# mark contents of files clauses with @<..>@
f1=g;s/;[$s]*file[$s]*\([^;]*\);/@<,\1,>@/g;
f1+=s/^[$s]*file[$s]*\([^;]*\);/@<,\1,>@/g;
# filter only files clauses
f2:=$(sel)
# remove separators
f3:=s/,[$s]*[^,]*separator[$s]*,/,/g;
# file.cpp optimize_size -> file.cpp_size
f4:=s/,[$s]*\([^$s,]*\)[$s]*optimize_size/,\n\1_size/g;
# file.cpp optimize_speed -> file.cpp_speed
f5:=s/,[$s]*\([^$s,]*\)[$s]*optimize_speed/,\n\1_speed/g;
# drop modificators (charset, readonly, highlight, tabsize, font, ...)
f6:=s/,[$s]*\([^$s,@][^$s,@]*\)[^,@]*/,\n\1/g;
# filter only files with extension (to remove licenses etc.)
f7:=s/,[$s]*\([^.,@]*\)[$s]*,/,/g;
# delete commas, newlines, unnecessary spaces and quotation marks
f8:=s/[$s,][$s,]*/ /g; s/"//g;
# write final eval to output
f9:=$(slash)s/@<[$s,]*\([^@]*\)[$s,]*>@/$$(eval $$1_FILES+= \1)/gp;
# ACCEPT
# mark contents of files clauses with @<..>@
a1:=g;s/;[$s]*acceptflags\([$s][^;]*\);/@<,\1,>@/g;
# filter only files clauses
a2:=$(sel)
# delete commas, newlines, unnecessary spaces and quotation marks
a3:=s/[$s,][$s,]*/ /g; s/"//g;
# write final eval to output
a4:=s/@<[$s,]*\([^@]*\)[$s,]*>@/$$(eval $$1_ACCEPT+= \1)/gp;
# OPTIMIZE SPEED/SIZE:
o1=g;s/.*;[$s]*optimize_speed[$s]*;.*/$$(eval $$1_SPEED:=TRUE)/gp;
o2=g;s/.*;[$s]*optimize_size[$s]*;.*/$$(eval $$1_SIZE:=TRUE)/gp;
# MAINCONFIG:
m1:=g;s/;[$s]*mainconfig\([$s][^;]*\);/@<,\1,>@/g;
# filter only mainconfig clauses
m2:=$(sel)
# "a" = "b" -> "a":"b"
m3:=s/"[$s]*=[$s]*"/":"/g;
# replace spaces with underscores
m4:=:m;s/"\([^$s:,"][^$s:,"]*\)[$s][$s]*\([^":,]*\)"/"\1_\2"/g;t m;
# delete commas, newlines and unnecessary spaces
m5:=s/[$s,][$s,]*/ /g;
# drop quotations
m6:=s/"*\([^":]*\)"*:"*\(\([^":]*\)\)"*/\1:\2/g;
# write final eval to output
m7:=s/@<[$s,]*\([^@]*\)[$s,]*>@/$$(eval $$1_CFG+= \1)/gp;

# put it all together
PARSER:='$(header)\
$(d1)$(call del-section,description)\
$(f1)$(f2)$(f3)$(f4)$(f5)$(f6)$(f7)$(f8)$(f9)$(call del-section,file)\
$(a1)$(a2)$(a3)$(a4)$(call del-section,acceptflags)\
$(o1)$(o2)$(call del-section,optimize_speed)$(call del-section,optimize_size)\
$(m1)$(m2)$(m3)$(m4)$(m5)$(m6)$(m7)$(call del-section,mainconfig)\
$(p1)$(p2)$(p3)$(p4)$(p5)$(p6)$(p7)\
$(call get-key,flags,FLAGS)\
$(call get-key,library,LIBS)\
$(call get-key,uses,USES)\
$(call get-key,link,LINK)\
$(call get-key,options,OPTS)\
$(call get-key,include,INC)\
$(call get-key,target,TARG)\
$(footer)'

#Return true, if package $2 is to be compiled flag $1
has-flag=$(filter $1,$($2_FLAGS))
#Join array with "_"
concatenate=$(strip $(if $1,$(if $(filter $(words $1),1),$1,\
     $(firstword $1)_$(call concatenate,$(wordlist 2,$(words $1),$1)))))
# return output directory for .o files, based on pkg name and flags
objdir=$(OBJDIR)/$1/$(call concatenate,$($1_FLAGS))
# source files needed to compile package $1
get-src=$(filter %.cpp,$($1_FILES))\
        $(filter %.icpp,$($1_FILES))\
        $(filter %.c,$($1_FILES))
get-src-speed=$(patsubst %_speed,%,$(filter %_speed,$($1_FILES)))
get-src-size=$(patsubst %_size,%,$(filter %_size,$($1_FILES)))

# Find all files in DIRS
available-files:=$(sort \
  $(foreach d,$(DIRS),\
    $(shell find $d -name .svn -prune -o -type f -print)\
   )\
)
available-upps:=$(filter %.upp,$(available-files))
assert-file=$(if $1,$1,$(error File $2 not found!))
# Find exact location of given .upp file
find-upp=$(call assert-file,$(filter $(foreach d,$(DIRS),$d/$1/$(notdir $1.upp)),$(available-upps)),$1/$1.upp)
find-file=$(call assert-file,$(filter $(foreach d,$(DIRS),$d/$1/$2),$(available-files)),$1/$2)

# add directory of file $2 to the list of required directories
needs-dir=needed-dirs+= $(dir $2)

#TODO: improve
compiler=$(if $(filter-out .c,$(suffix $1)),\
 $(CXX) -c -x c++ $(CXXFLAGS),\
 $(CC) -c -x c $(CFLAGS))
 analyze-info=$(if $S,,  Analyzing $1 dependencies ...)
includes-fetcher=$(info $(analyze-info))$(compiler) -M

#compile .cpp into .o
define compile
$(eval $(needs-dir))
$(if $F,$2: $1,$(dir $2)$(shell $(includes-fetcher) $(CINC) $($3_MACRO) $1 |tr -d '\\'))
	$(call echo,"    Compiling $1 with flags $($3_FLAGS) ...") 
	$M$(compiler) $(CINC) $($3_MACRO) $1 -o $2
$(ENDLINE)
endef
# ditto with speed optimization
define compile-speed
$(eval $(needs-dir))
$(if $F,$2: $1,$(dir $2)$(shell $(includes-fetcher) $(SPEEDFLAGS) $(CINC) $($3_MACRO) $1 |tr -d "\\\\"))
	$(call echo,"    Compiling $1 with flags $($3_FLAGS) and speed optimizations ...") 
	$M$(compiler) $(SPEEDFLAGS) $(CINC) $($3_MACRO) $1 -o $2
$(ENDLINE)
endef
# ditto with size optimization
define compile-size
$(eval $(needs-dir))
$(if $F,$2: $1,$(dir $2)$(shell $(includes-fetcher) $(SIZEFLAGS) $(CINC) $($3_MACRO) $1 |tr -d "\\\\"))
	$(call echo,"    Compiling $1 with flags $($3_FLAGS) and size optimizations ...") 
	$M $(compiler) $(SIZEFLAGS) $(CINC) $($3_MACRO) $1 -o $2
$(ENDLINE)
endef

# call appropriate compile functions for all files
define compile-all
$(if $(call has-flag,SPEED,$1),\
  $(foreach f,$($1_SRC),$(call compile-speed,$(call find-file,$1,$f),$($1_OBJDIR)/$(basename $f)$(if $(filter $(suffix $f),.icpp),.i,).o,$1))\
  $(foreach f,$($1_SRC_SPEED),$(call compile-speed,$(call find-file,$1,$f),$($1_OBJDIR)/$(basename $f)$(if $(filter $(suffix $f),.icpp_speed),.i,).o,$1))\
  $(foreach f,$($1_SRC_SIZE),$(call compile-speed,$(call find-file,$1,$f),$($1_OBJDIR)/$(basename $f)$(if $(filter $(suffix $f),.icpp_size),.i,).o,$1)),\
$(if $(call has-flag,SIZE,$1),\
  $(foreach f,$($1_SRC),$(call compile-size,$(call find-file,$1,$f),$($1_OBJDIR)/$(basename $f)$(if $(filter $(suffix $f),.icpp),.i,).o,$1))\
  $(foreach f,$($1_SRC_SPEED),$(call compile-size,$(call find-file,$1,$f),$($1_OBJDIR)/$(basename $f)$(if $(filter $(suffix $f),.icpp_speed),.i,).o,$1))\
  $(foreach f,$($1_SRC_SIZE),$(call compile-size,$(call find-file,$1,$f),$($1_OBJDIR)/$(basename $f)$(if $(filter $(suffix $f),.icpp_size),.i,).o,$1))\
,\
  $(foreach f,$($1_SRC),$(call compile,$(call find-file,$1,$f),$($1_OBJDIR)/$(basename $f)$(if $(filter $(suffix $f),.icpp),.i,).o,$1))\
  $(foreach f,$($1_SRC_SPEED),$(call compile-speed,$(call find-file,$1,$f),$($1_OBJDIR)/$(basename $f)$(if $(filter $(suffix $f),.icpp_speed),.i,).o,$1))\
  $(foreach f,$($1_SRC_SIZE),$(call compile-size,$(call find-file,$1,$f),$($1_OBJDIR)/$(basename $f)$(if $(filter $(suffix $f),.icpp_size),.i,).o,$1))\
))
endef

# parse .upp file and process obtained variables
define prep
  $(eval $(shell cat $(call find-upp,$1) |sed -n $(PARSER)))
  $(eval $1_FLAGS:=$(sort $($1_FLAGS) $(if $($1_SPEED),SPEED,$(if $($1_SIZE),SIZE,))))
  $1_USES:=$(sort $($1_USES))
  $1_FILES:=$(sort $($1_FILES))
  $1_MACRO:=$(foreach f,$($1_FLAGS), -Dflag$f)
  $1_OBJDIR:=$(call objdir,$1)
  $1_SRC:=$(sort $(call get-src,$1))
  $1_SRC_SPEED:=$(sort $(call get-src-speed,$1))
  $1_SRC_SIZE:=$(sort $(call get-src-size,$1))
endef

# compile all .c/cpp files in package $1 and create .a
define build
$(eval $1_OBJFILES:=$(foreach f,$($1_SRC),$($1_OBJDIR)/$(basename $f)$(if $(filter $(suffix $f),.icpp),.i,).o)\
  $(foreach f,$($1_SRC_SPEED),$($1_OBJDIR)/$(basename $f)$(if $(filter $(suffix $f),.icpp_speed),.i,).o)\
  $(foreach f,$($1_SRC_SIZE),$($1_OBJDIR)/$(basename $f)$(if $(filter $(suffix $f),.icpp_size),.i,).o))
$(eval $1_icpp-files+=$(foreach f,$($1_OBJFILES),$(if $(findstring .i.o,$f),$f,)))

$($1_OBJDIR)/$(notdir $1.a): \
  $(foreach u,$($1_USES), $($u_OBJDIR)/$(notdir $u.a)) $($1_OBJFILES)
	$(call echo,"    Creating archive $$@ ...")
	$M$(AR) $$@ $(filter-out %.i.o,$($1_OBJFILES))

$(eval $(call compile-all,$1))
endef

# create target to link libraries into executable
define link
$(eval ARCHIVES:=$(strip $(foreach u,$(ALLDEPS),$($u_icpp-files)) $(foreach u,$(ALLDEPS),$($u_OBJDIR)/$(notdir $u.a))))
$(eval ALLLIBS:=$(sort $(foreach u,$(ALLDEPS),$(foreach l,$($u_LIBS), -l$l))))
$(eval file-num=$(words $(foreach d,$(ALLDEPS), $($d_OBJFILES))))

prep-dirs:
	$(call echo,"  Preparing output directory structure ...")
	$Mmkdir -p $(sort $(needed-dirs))

$(BINDIR)$1$(BINEXT): $(ARCHIVES)
	$(call echo,"  Linking $$@ ...")
	$M$(CXX) -o $$@ $(if $(call has-flag,ALLSHARED,$1),-shared,$(if $(call has-flag,STATIC,$1),-static,)) $(LIBPATH) $(LDFLAGS) -Wl,--start-group $(ARCHIVES) $(ALLLIBS) -Wl,--end-group
endef

# Recursively parse all necessary packages
parse-deps=\
  $(if $(filter $1,$(PKG)),$(if $(call confirm,$(USEMAINCFG)),\
    $(info CMD:cat $(call find-upp,$(PKG)) |sed -n $(PARSER) OUT:$(shell cat $(call find-upp,$(PKG)) |sed -n $(PARSER)))$(eval $(shell cat $(call find-upp,$(PKG)) |sed -n $(PARSER)))\
    $(eval CFG_FLAGS:=$(subst _, ,$(call lastword,$(subst :, ,$(firstword $($(PKG)_CFG))))))\
    $(if $($(PKG)_CFG),,$(warning Package $(PKG) does not appear to be a main package!)$(warning Trying to continue anyway...)),),)\
  $(foreach p,$1,\
    $(eval $p_FLAGS:=$(sort $($p_FLAGS) $(FLAGS) $(PLATFORM) $(CFG_FLAGS)))\
    $(if $(filter $p,$(ALLDEPS)),,\
      $(if $S,,$(info Parsing package $p ...))\
      $(eval ALLDEPS+= $p)\
      $(eval $(call prep,$p))\
      $(call parse-deps,$($p_USES))\
     )\
    $(eval ALLDEPS:=$(sort $(ALLDEPS)))\
   )

define make-pkg
.PHONY: $1
$1:
	$(if $(call confirm,$(SIMULATE)),@echo -n "SIMULATE: ";,)
	$(if $(call confirm,$(SIMULATE)),,$M)+-\
      $(MAKE) -f $(filename) $(if $(JOBS),-j$(JOBS) ,)--no-p PKG=$1 $(foreach v,$(cmdline-vars),$(call pass-var,$v))
endef

.PHONY: default
default: all

define main-targets
.PHONY: all start prep-dirs export

all: $(if $(single-package),start $(BINDIR)$(PKG)$(BINEXT),$(PKG))
	$(call echo,"$(if $(single-package),  Package $(PKG),Everything) done.")

start: prep-dirs
	$(call echo,"  Building package $(PKG) ($(file-num) files in $(words $(ALLDEPS)) packages, flags: $($(PKG)_FLAGS))")
endef

endif # if skip-parse

define export-pkg
export_$1:
	$(if $(call confirm,$(SIMULATE)),@echo -n "SIMULATE: ";,)
	$(if $(call confirm,$(SIMULATE)),,$M)+-\
      $(MAKE) -f $(filename) --no-p -r "EXPORT=$(EXPORT).$(subst /,_,$1)" PKG=$1 $(foreach v,$(cmdline-vars),$(call pass-var,$v)) export
endef

define additional-targets
.PHONY: help clean
help:
	@head -n 104 $(filename)

clean:
	$(call echo,Deleting $(OBJDIR))
	$M rm -rf $(OBJDIR) || true

ifeq ($(single-package),y)
export:
	$(call echo,"  Exporting makefile to $(EXPORT) ...")
	$M$(MAKE) -f $(filename) -p -q -B -r "PKG=$(PKG)" $(foreach v,$(cmdline-vars),$(call export-var,$v)) \
		|$(AWK) 'BEGIN{c=0;print ".PHONY:default";print "default: all";}{if(c==1&&substr($$$$0,0,1)!="#"){print $$$$0;}if(c==0&&$$$$0!="# Files"){next;}else{c=1;}}' \
		|sed -n '1h;1!H;$$$${g;:d;s/[^\x0A\x0D]*:[\x0A\x0D][\x0A\x0D]*\([^\t]\)/\1/;t d;p;}'\
		> '$(EXPORT)'
else
export: $(foreach p,$(PKG),$(eval $(call export-pkg,$p)) export_$p)
	$(call echo,"Exporting control makefile to $(EXPORT)")
	$Mecho all:>'$(EXPORT)'
	$Mecho "	$M+-$(foreach p,$(PKG),make --no-p -f $(EXPORT).$(subst /,_,$p); )">>'$(EXPORT)'
endif

endef

# clean-out the default rules SUFFIXES, we manage it ourselves
.SUFFIXES:

# call parse-upp and link on main package and create all directories
# and build targets needed for current task
# OR
# run makefile for each package from $(PKG)
$(if $(skip-parse),,\
  $(if $(single-package),\
    $(eval $(PKG)_FLAGS:=MAIN)\
     $(call parse-deps,$(PKG))\
     $(foreach p,$(ALLDEPS),$(eval $(call build,$p)))\
     $(eval $(call link,$(PKG)))\
    $(foreach p,$(PKG),$(eval $(call make-pkg,$p)))))

$(eval $(main-targets))
$(eval $(additional-targets))

# For each package, folowing variables are defined: 
#	$(PKG)_DESC
#	$(PKG)_FILES
#	$(PKG)_CFG
#	$(PKG)_FLAGS
#	$(PKG)_USES
#	$(PKG)_LIBS
#	$(PKG)_LINK
#	$(PKG)_OPTS
#	$(PKG)_TARGET
#	$(PKG)_INC
#	$(PKG)_ACCEPT
#	$(PKG)_SPEED
#	$(PKG)_SIZE
# Global variables defined when invoked for single package:
#	ALLDEPS
#	ARCHIVES
#	ALLLIBS
#	CFG_FLAGS
