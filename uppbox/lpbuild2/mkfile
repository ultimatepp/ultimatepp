########################################################################
#  __  __    _      _     __  __   __   _  _  ___  ___  __  __    ___  #
# (  )(  ) _( )_  _( )_  (  \/  ) (  ) ( )/ )(  _)(  _)(  )(  )  (  _) #
#  )(__)( (_   _)(_   _)  )    (  /__\  )  \  ) _) ) _) )(  )(__  ) _) #
#  \____/   (_)    (_)   (_/\/\_)(_)(_)(_)\_)(___)(_)  (__)(____)(___) #
#                                                                      #
############################## USAGE ###################################
# This makefile is controlled through a set of variables, similar to   #
# any other makefile. The prefered way to change them is via command   #
# line, e.g.: make PKG="ide usvn" CXX=g++                              #
#                                                                      #
# All of the variables that can control the bahaviour are listed below #
# in DEFAULTS section. Also their default value is shown here. Notice  #
# that many of them can contain shell commands (enclosed in ``) which  #
# are evaluated before parsing and building each package.
#                                                                      #
# For boolean type of variables value "1" represents true, anything    #
# else is evaluated as false.                                          #
############################# TARGETS ##################################
# This makefile defines five targets that are meant to be used         #
# directly by user from the command line:                              #
#                                                                      #
# all          - builds all (default if any packages are specified)    #
# help         - shows this help (default if no packages are given)    #
# simulate     - prints all commands, without executing them           #
# clean        - deletes all compiled files related to given packages  #
# dist-clean   - deletes the entire OUT directory (including parsers)  #
############################# EXAMPLES #################################
# Typical usage:                                                       #
#    make PKG=ide FLAGS="GCC"                                          #
# More complicated usage:                                              #
#    make PKG="ide UWord Bombs" FLAGS="GCC .NOGTK" ECHO=1              #
# Show help:                                                           #
#    make help                                                         #
# Silent mode:                                                         #
#    make PKG=ide SILENT=1                                             #
############################# DEFAULTS #################################
# List of packages to build (space separated). If empty, this help     #
# text will be shown.                                                  #
PKG:=
# Paths where to look for packages.                                    #
NESTS:=uppsrc bazaar examples reference tutorial $$HOME/MyApps
# Building flags (as in TheIDE).
FLAGS:=GCC
# Additional include paths for compiler (without the leading "-I").    #
INCPATHS:= /usr/include/freetype2 /usr/include/gtk-2.0
INCPATHS+= /usr/local/include/gtk-2.0 /usr/include/glib-2.0
INCPATHS+= /usr/local/include/glib-2.0 /usr/lib/glib-2.0/include
INCPATHS+= /usr/local/lib/glib-2.0/include /usr/lib/gtk-2.0/include
INCPATHS+= /usr/local/lib/gtk-2.0/include /usr/include/cairo
INCPATHS+= /usr/include/pango-1.0 /usr/include/atk-1.0
INCPATHS+= /usr/X11R6/include /usr/X11R6/include/freetype2
INCPATHS+= /usr/X11R6/include/gtk-2.0 /usr/X11R6/include/glib-2.0
INCPATHS+= /usr/X11R6/lib/glib-2.0/include
INCPATHS+= /usr/X11R6/lib/gtk-2.0/include /usr/X11R6/include/cairo
INCPATHS+= /usr/X11R6/include/pango-1.0 /usr/X11R6/include/atk-1.0
INCPATHS+= /usr/local/include/cairo /usr/local/include/pango-1.0
INCPATHS+= /usr/local/include/atk-1.0 /usr/local/include
INCPATHS+= /usr/local/include/libpng /usr/include/gdk-pixbuf-2.0
INCPATHS+= /usr/lib/i386-linux-gnu/glib-2.0/include
INCPATHS+= /usr/lib/x86_64-linux-gnu/glib-2.0/include
INCPATHS+= /usr/lib/i386-linux-gnu/gtk-2.0/include
INCPATHS+= /usr/lib/x86_64-linux-gnu/gtk-2.0/include
INCPATHS+= /usr/lib/gtk-2.0/include
INCPATHS+= /usr/lib/glib-2.0/include
# Paths to libraries for linker.                                       #
LIBPATHS:=/usr/X11R6/lib /usr/lib /usr/local/lib
# Directory to store intermediate object files                         #
OUT:=_out
# Directory where the resulting binaries will be stored                #
BIN:=$(OUT)/bin
# ar command                                                           #
AR:=ar -src
# Command to create directories equivalent to "mkdir -p". The only     #
# good reason to change is if your platform uses mkdir command which   #
# doesn't support the -p option                                        #
MKDIRP:=mkdir -p
# C compiler command                                                   #
CC:=cc
# C++ compiler command                                                 #
CXX:=c++
# Options for C/C++ preprocessor                                       #
CPPFLAGS:=
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
# Optimization, supply SPEED or SIZE to optimize the executables       #
OPT:=
# Platform flag, added to FLAGS, can include shell commands            #
PLATFORM:=`uname | tr a-z A-Z` POSIX
# Suppress all messagges                                               #
SILENT:=0
# Print each executed command                                          #
ECHO:=0
# Use dependency files. This will probably work only with gcc-like     #
# compilers (requires -MD option). The build usually works correctly   #
# even without specifying the dependencies unless you modify included  #
# files (*.h, *.lay, ...) without modifying the *.c/cpp files that     #
# include them.                                                        # 
DEPS:=0
# Run the executable(s) sequntially when compiled                      #
RUN:=0
# Override default name of final executable (absolute or relative path)
TARGET:=
# Add flags from based on mainconfig section (1-based index, set       #
# to 0 to disable completely                                           #
USEMAINCFG:=1
# Number of parallel processes to use (like "make -jN" option).        #
# The JOBS variable must be used instead of -j option (as -j would not #
# work as expected). Default value is the number of cores (if it is    #
# possible to detect) or 1.                                            #
JOBS:=$(shell echo /sys/devices/system/cpu/cpu[0-9] | wc -w || echo 1)
# Ask for some additional details during parsing                       #
INTERACTIVE:=0
# Use colorized output, recognized values are 0, 1 or auto             #
COLOR:=auto
# What color should be used in colorized mode, number from 0 to 7      #
HIGHLIGHT:=4
TIME:=TIMEFORMAT="  time taken: %Es"; time
########################################################################

m:=[ "$(SILENT)" = "1" ] || echo
e:=@    #comment out for debugging
dbgparser:=-DflagDEBUG -ggdb3

thisfile:=$(MAKEFILE_LIST)$(.MAKEFILE_LIST)
color:=if [ "$(COLOR)" = "auto" ]; then [ -t 1 ] && c=1 || c=0; else c="$(COLOR)"; fi
pass:=$(color); UPP_NESTS="$(NESTS)" UPP_FLAGS="$(FLAGS)" \
    UPP_INCPATHS="$(INCPATHS)" UPP_LIBPATHS="$(LIBPATHS)" UPP_OUT="$(OUT)" \
    UPP_BIN="$(BIN)" UPP_AR="$(AR)" UPP_MKDIRP="$(MKDIRP)" UPP_CC="$(CC)" \
    UPP_CXX="$(CXX)" UPP_CFLAGS="$(CFLAGS)" UPP_CXXFLAGS="$(CXXFLAGS)" \
    UPP_LDFLAGS="$(LDFLAGS)" UPP_SPEEDFLAGS="$(SPEEDFLAGS)" \
    UPP_SIZEFLAGS="$(SIZEFLAGS)" UPP_PLATFORM="$(PLATFORM)" \
    UPP_SILENT="$(SILENT)" UPP_ECHO="$(ECHO)" UPP_DEPS="$(DEPS)" \
    UPP_RUN="$(RUN)" UPP_TARGET="$(TARGET)" UPP_USEMAINCFG="$(USEMAINCFG)" \
    UPP_OPT="$(OPT)" UPP_INTERACTIVE="$(INTERACTIVE)" UPP_COLOR="$$c" \
    UPP_HIGHLIGHT="$(HIGHLIGHT)"

all: no-package-specified build

do-build: $(OUT)/parser $(OUT)/brc.sh $(OUT)/color.sh
	+$e set -e; $(color); B=""; N=""; C=""; color=""; \
	[ $$c -eq 1 ] && color="$(SHELL) $(OUT)/color.sh" && B="\e[1m" && N="\e[0m" && C="\e[1;3$(HIGHLIGHT)m"; \
	for pkg in $(PKG) ; do \
		$m $${B}Parsing package $$C$$pkg$$N $$B...$$N; \
		$(TIME) $(pass) $$color $(OUT)/parser $$pkg; \
		$m $${B}Building package$$N $$C$$pkg$$N $$B...$$N; \
		$(TIME) $$color $(MAKE) -j $(JOBS) -f $(OUT)/Makefile && \
		$m $${B}Package$$N $$C$$pkg$$N$$B finished ...$$N; \
	done; set +e

no-package-specified:
	$e if [ -z "$(PKG)" ]; then sed '/^$$/{s/.*//;q;}' $(thisfile) && false; fi

$(OUT)/parser: $(OUT)/parser.cpp
	@$m Compiling parser...
	+$e $(TIME) $(CXX) -x c++ -o $@ $^ $(dbgparser)

$(OUT)/parser.cpp: $(thisfile)
	@$m "Extracting parser..."
	+$e [ -d $(OUT) ] || $(MKDIRP) $(OUT)
	+$e $(TIME) sed -n 's/^#://p;' $< | base64 -d | gzip -d > $@

$(OUT)/brc.sh: $(thisfile)
	@$m "Extracting brc parser..."
	+$e [ -d $(OUT) ] || $(MKDIRP) $(OUT)
	+$e $(TIME) sed -n 's/^#~//p;' $< | base64 -d | gzip -d > $@

$(OUT)/color.sh: $(thisfile)
	@$m "Extracting colorizer script..."
	+$e [ -d $(OUT) ] || $(MKDIRP) $(OUT)
	+$e $(TIME) sed -n 's/^#%//p;' $< | base64 -d | gzip -d > $@

build: do-build
	$e for pkg in $(PKG); do \
		if [ "$(RUN)" = "1" ]; then \
			for target in "$(TARGET)" "$(BIN)/$(TARGET)" "$(BIN)/$$pkg"; do \
				[ -f "$$target" ] && break; \
			done; \
			[ -e "$$target" ] && { $m "Running $$target"; } && $$target; \
		fi; \
	done

simulate: $(OUT)/Makefile
	+$e for pkg in $(PKG) ; do \
		$m Parsing package $$pkg ... && \
		$(pass) $(OUT)/parser $$pkg && \
		$m Building package $$pkg ... && \
		$(MAKE) -nf $(OUT)/Makefile; \
	done

help:
	$e sed '/^$$/{s/.*//;q;}' $(thisfile);

clean:
	+$e for pkg in $(PKG); do \
		$m Parsing package $$pkg... && \
		$(pass) $(OUT)/parser $$pkg && \
		$m Cleaning package $$pkg && \
		$(MAKE) -f $(OUT)/Makefile clean; \
	done

dist-clean:
	@$m Deleting $(OUT) ...
	$e rm -rf $(OUT)

parser-clean:
	@$m Deleting parser files ...
	$e rm -f $(OUT)/parser $(OUT)/parser.cpp

.SUFFIXES:

.PHONY: all help simulate clean dist-clean parser-clean \
        build do-build no-package-specified

### PARSER CODE (gzipped and base64 encoded) ###
#:H4sIAOIi808CA7Q7+1PcRtI/QxX/w6BgkFixu+TqkguL8BG8TqjjVTwSO8DnCGkWZLSSLGnXJj
#:b87V93z0Mj7cM4dedygTTT093T028N30VJEI9CzrZHSVSUYftuZ/E7PRaUDxmvDyX8o/lalHmU
#:3NZBAE0c3UyMRSkOLZZ8mMV+ybeD2C8Kdr6zGCVxlHA2TqOQHfV/65/a56xwPi8uRAObfi8MMq
#:BSwlsZ8jx3rf7p6fHpFjtLh7y8A/qsvPNLVtylozhkCR/znN35WcYTHsJDoV/a7fZVYjk9QMk/
#:RaW9iY+Pi4+9aWyJh994UKY5MpMAhegv7vpxnAaw7nydZb3FbHQTR8HW4oIAtJ0tBLK7jssIkJ
#:4y+Mnkv8+nnCC+d3qPiwtPahmb/0+t6tIqKTFk6Rde7qWjpLSdIE2KsrEq5+UoT4hvY935Kksz
#:nvtA+PLaRiyJM3VddplcG+sEhamrxVRzHYDuhqFNZ4iHCWzseCQWh8HQghLFuhjDc5EY1m1QND
#:tr4Xyr5bBzcVIKo+bkk8M+M7mGSHmfekzDXSTRhxGvoMeCE2A5SwvvdZSE9pioAnMwsuPBMekt
#:wMC1wRFiH0suEAOtNjALEZC2wnkiROR1e9E2yT6CPeAUEhp7XnYZXWtKERJ5rAhtbAoiZA9SQo
#:gu5klNkOrdoAdDJkWcXAAhRs7GztO5TTtdwFkClFQz5jH7fN3JudDXzIXJdYRKB/a5Q4toRi56
#:XARjHIOlaJ3fPt+pVMKTMjGmQOrCxoRFnZHLYMB6cOfndSMSc2ANW00DyMD483CU2ZblwJk/al
#:hBTrysgt+ApRq2aGeOMwEryBaCSAULaAnvk2ZiwggHOed2RiYouK+ZgsBeM4jZJnWTpjHbL/rD
#:rHywJ8HZdENeh9PyWJd9+cKAbTgQ4IZGEKW5O8XW00zcmiFcqsCZgWM2V7WlSvJNBTBOZMrSCh
#:rPSAtED3vNs2qwovAsgxiCYQZaC0Cz0RjM1BBNQyP5IfPbi7n/dedsqgZDe6pUtSe9ReWmn3vU
#:+nynCbk1uTF0BZUGwlLJiETSnC2qWdwxazF0OwtikV9GwbvAL8ptkv+OXTkHgblFKIRzECF7Wd
#:AY8uEwHYMkWghH0nQFrOncIWgXwslN2xiQfNZ+ns3r91MYzS5x8hpRVK+tTRzoPo/VpooK38zs
#:jC17qGjkm2kz60GaPVRqIWSxoDQGn3ErCNQIBQZ1FMJeOsz8nNfpuko+3samjsMVCph4CXQTod
#:0FOndIUZS2F42AdgByNNCzwMRYxRl9DBCroh0McxsbjgxvGNs8L6iHt0Zoa8TPhpGrGI1qo0gJ
#:mSlN8Gp6UI+3OL+BS3UIxICpJNCKhAzQ289hUMaowyg0vDroWl66rKaUt+VdJXeDcVNXN2ihTD
#:NgaLsr43Y9q2VWP8/TfIvtJ2M/BucDZ+1DgsvzgpUp8rIsUlcjd1147EmtIxpodDuCK7a6Kvnb
#:6ZIUBFNiCGLxgtojqT/pInk+eIHf0y1rKAwLcLQ2ayalVoL1B9mDTUaPhiclhjonEaOZoY2tXX
#:XXDLlLJV8ERiKehCpH0F6h4e3iu0KdgxrK70hF5Ho8BbV4+7mLQZYy0f175OFQ5XZkBgFw4Fg9
#:nATsi0bW/o28VZgBoK3cwJNke3FR8nvK/fA3P4aMV4cFl8w45HE03ESxu2vGyPdiRJVangcBS1
#:mFZfW0GeQ9Wf7cR5mH3vHjXRRz2y4ucQSsXeLHrEQMYU4iSVSD294/vndQL+X7skeaic+tFmCl
#:kNRGm8MhPE0K5h+8gR8XvCeYvOwCvbUri7g2lghz+ADvZT7iQp1M7xJIT4/OogceuccCmZMD1v
#:XA3IN+I+Zt+wPyjINEFl+Cyw2IFMuoxldr4EluIObc9xSubc/+8LK7BZslv1BGCTG0kKMurAfS
#:BQ7sDw7yIDeOw1L0OaniIm1+D7YwGtYO1HSZkGRHKZUBKggofylnpNS0zshxCgOe120IURmqZK
#:SSpByQB4Hc8WQ0ZMdZ+fno+KjvsrOTfv8V/Nr/ow+zavIMfAiHAu3i6D9Hx78fuezo2GVv+2cM
#:MQCnowDjQMyBC4AGqygrpUvA94Hr8JNbGEMgKK0BwEZ6kNMbMtovztJRHnAboVbZQIkDa7xBG/
#:G0dWxba69JmSzDtFPtVemYpM4/lSAVuRpFA9At0rF6tAG4dpXbqZADoxgFyzROP/LcFq+CLsM3
#:fRxWkGWWyOAXFzD7A4WrzUdfAwiC+dOfPsl50Uhpo0A9Gp5y0IyDENgkk1NRBN+A4SafigOHn6
#:lvlI6fQQF5xwu7lvWaB8Kg+jXPg0WVkReUmJDFMnpe66zp9o9URtDC/ZIPP2s9+HhHRa98i2AO
#:3oaj0r+JeaXhGCuRc7kcm0A4Yku9r6vrQeqHAGjswjXKLrZ+zx9cVT5LhFREM+GmlUcoXAB0RJ
#:pZOUa2RsG+6RfruzHgbeFHF3DGo1TAyGC61fs41u8o75pn9Qit9qki8cV8g5zbgvSKePpIDnGp
#:CaksataRzMDicbyxIWAoaULyqDyVpxV5sviB9PWGu67yYlMcG4KHqeBQynaVDb1xm3pHtGjYlu
#:iUsGAEzx1GqvhauBg3aVbppIASVER8RBHTviToM7RcntPPfsGPwPNMK/KUZysMp9aZ4dSKKqgU
#:NS9m0HoV5f8NUpQv1Gh1XYQQtOp5FVoAefQ6zQEFMiPnIHu4udzsdrvYiXu9f9BfZwNvANlTYj
#:8NEtfKLcXNoJ65UBfGyFWWB9jOGjikYEnLg4TXD+0bd9NF7O5AHFDLu8HfnQ7WXQz+++zOD+7R
#:PvMcLJJFwyzmQ55gbpwm7KNoOKdAi3PIOktE4uVy+4lTO+acUrW6JMBV9ZNxzZySeoTHnKySSm
#:ZbFycn72jXWctD/TShc+8W6gVASNmTovsy3yp6ppe7yLLP9Tgb8iLIo4xSgzDKe2Y8brgiHEUw
#:Bvu4x583uZ+Dx/LDcBD7t+DMIOkHNlw2KjhVp/QJAGQ4Ksp0WCEU9HeYHwQc3CEADG7v6efYZY
#:SqgkVt2WED+ImDiwuwA2oS1hICGq2rVAan599yBMV92urdfe5Kt9bNlCyvwnEXJWm/wQL7jHqt
#:SLgCZHZLCkiuspFTb5wjk543orQDlxJQs+EgV34WxRe6Ewh0tY5BVcFiLIx9OAlbWg8ZgGFjwr
#:2TpTTTbWk6m7JzDXCYRA/gbANbGAxaHuTF/ePXlCkvU6aMiXKvlhUTGGX/n1U8WM7bVfMTFuWX
#:uRG3Ibte1rFMJs7wKvy+mVLLhEPQYLYII5ClU/WrE3QZkT54yx96OmjEMumW6ING6wVlIzVjlR
#:zW39AP8jV4ohKYmnDo0/Bj0yUeyNHuYf/d4e6bayNIBx9DYA2l/DG0EdJlJqho5QEMMG11LGPh
#:gMKVPGf76OLgYKI9AvkRsTY1R1oAy8ePAQQBCZLIEDprLy1rCwhij1BNwSPQhp/GzhYGGDURh5
#:rUYUybIcy0R5DRCnjUJvLllSsn5RgYuvMo0pXlwfyeyYmgwF4U5I4HsLdw+SqxXPakiE+0Tho9
#:kJq2Y+pQ2U6l7bbO3sBhWcIVFpYrnaIDZoCAmInXANFXWsJlzoEhT2ppnzoTkpyjVbncmYDCE1
#:vKJc+EQ09tCYc9E0b6cUt7dEcE05oJGtmpZcQUy1GWX8sobOvq6h8//igPXn6EkzZfGAkdjqLq
#:XFlWbx5FEUmEdBxl9WEqc0oZZyjVMzI5R1KfkbTNJJakN3FU/qW31umw6DZJc8gDwOJACz/OWz
#:30QYppMohuTU7lwwJGwll8Sj01cDELD40ZI1f4kUyCG8OekjQSGP+XCHyr3NBShtFf/F2RQc4E
#:aAgLjMKBUxdB2OlX1sKPxtL9P/pzVmLqoI9KqwSmFWwgty2K/Waaz9aUWCg6Fhy7shiMdZTUEl
#:svRHOri8HL1bWMdjJlPht5jd0ynyUmYrMpK6OQmoHCkJaJQYlsFgK9WWNxJQBZx+hSTPhpPY+y
#:0J2ZgVNJQ9ZLQtx6/5TbkUoOnBrWb9UvkWZWHqdZGk8Wx7VSUCw36sGJenhK9TxZJE8rk4uJGr
#:lWJc+pk+dUyvVaeXa1rA9J/tJl7kTVLCF01VuvoOtyV1UVrajF7EEQpzpyNj9n1YpC0dnJwJna
#:ylLcZj5FFzekIhi5i5nMjv14hnnhnQs/Npchu+T/5AURmJY3gXRZ/BqiCJTG9vT8jvhp1DGYMX
#:Y6BZWIECMLnlONWKQ5lIV5W97kGJt8TLQTx1OaiTkp41h2EDG20IL33maPva+vYK3We6Fbzdb3
#:0/jy/bVK9yPvPX3okhLFL3giB4+udcsucHa6Uk1hvLV57eG0SIOEulGyLKaC2lezAkCpYK/2tg
#:l7y6fsrWh5VttqSdRV1+JR3hU58XOQYyVryMZ3WHZPpaGkhpF04hqWPhRKWGU56TJVoUKCVYhs
#:DDwh4hIfu5jUSNlV3g+J+8Axv6dCjbEDBuhTiRGwbXj+a43Co5zZNWb+EDPCZt+tGVBdA+qnNd
#:koEBHm0C+DO1Q/o0FIedP9rWplGu5uWfpmfZGhWl64uKQ3scTWKQcRvEEbp1XHubHGTBwcmThI
#:IjdGYl5UdZzJhwoO6rNEt1I3EmxBHwId9WVSODrVZQ2nuKXJXp6gTycqsskodMwbXIoFpYXvgY
#:n320IFTFV8X30mtmHzntf98gVV7BJ+XLdV1kgUxGKwJEEITjCcpE++sMGDUh6Bf3U1Cj3POtzd
#:P7Ic9eXTOPxdQDb97BsH1jhmIV8mBdy9dsTHNwpVy2vGi73WyOdWVydSvFWpWFOp4dZvjI3dTN
#:kFaNOzNoG7be7B4OPLlwnWvsxlDWBns9YHVw/RXrp1HffBDTRtDIba1MVf9lQb39BvPet5b/tn
#:vebn9GbsB0ZxRb1RvqhD/g52NxxW6A+E6nus7Vgi0Ff0JLmmhj0uzjdnys0MW33WDYTff+0fMX
#:HFdwtKa6ynn9Q+ptbTFZs3L4HRraPjyZOgeA/294pnRbOpUokfLVCYFfbmcEIbl0CHiKk5A1Uw
#:R49BKyhva4tG3P2tKxsxCNsIDKPJmyOEqI1F8JRIhTwp5akAIW65RNfsXcl0aETM1GCrTwR1yq
#:NpBJWIRjLyVwEW+ZD3hmvpifG1SGmz2xBwzgfRp4ke4FeSkM+N/Y/ltuVGcwjizGoJ3K2x2ue0
#:hpo8fUSEJlvYJiujYSC6LtgSty7O+ugg917/YrnWpqXv/9DBF0XgJwP7CZa41ovQclcTfY8XXX
#:f3uk01dLURTypP8yr5736eSPVmYcqLZK2EIg8S3zJlN5z5lF+oNldbdpQkCV3AqO68bqICCZ77
#:QRmNubGl/aPz/unu3vn+b33YU9cCpmBn6gbPsrFIxufES14mG5tbIlvTH1dtE/Kzan7O2LgsFO
#:S2rcNRXEaYoYruwyinLxkF88d+FONXTRe2H+O3jjTh6rK8ShtlbUN3KKLtGQSrq8eaKGMvIoet
#:vSjWtugnihESR/fJRIFKZQ6ojLeOaN1he1SiiZyO2fgnCiBv5hesgGPiqkQHnxBHRelUe1BI3k
#:JNKjeJCfrli/B6CzmqFTisLjYwZfkBQ9GdRY9dXllXFmF0qm7zYBCPiju8vq9vBd6MBpff//MH
#:kVNjV72wYchlMAZnUIZRorqOeLuLbnXAvFNdABY2QGvIBhgYgaosE1kS1jcDugjgQhpY+wm9Aq
#:Aqn0swPaVyIGHbbJZOqTuLteNKrpUJ0rwo6TJXtCjpkqVnXRTUqaYW3ZTm9ERyxpQyZcLJiARM
#:FCLqbGRgypxvQqhyR93obquaWuZ01IuQELJbUB2o2ngFWqVzDXfaiA+VR31uYoqsRuC/8NCJbj
#:Q3LZUWIwErBiuo3iR1jTecw4fy/01gwOlGAmuzJuZD9SXvWynq9s18qo5uPU2Zpzik8l/VwpJ6
#:OQNanrSrWVcuSHU9vorBZTXZm06gJoD5u1YolW7i4SuWxKnXa5HaV7Epp18//Edh8GY8pka/GY
#:5V+9BT4ev45Pyb9Ns4E8DlefiVFB2LQjwZpNDVabKeRS1Oq6HQiMrofer7Rua6/T/6U5ephufj
#:bHWU316erY0SvqaMiFn3QiYBRXZUk1mzVzz25vSJx3hRE/UBH7A3X2vzYW/DbG3VuqlzN57cz/
#:I+k5tO7ms7Vq0UUrUGlHEvhjRO9HRq9RHmWFS7bdGjrd5JG8nYlA4e9c/OzyB5GmVZkQfsxv/L
#:h1AKmSfPeQLBuByB+4n8mPFPPjbiCta5S4e8c9c5fNiFNZZj1gR13K8Pdn9B3L/s7bFfLvYRtL
#:LgOujJwe756+PTQ4A+OT7bf8MO9o8u3tRXLFYpPO6HxozEV70Ku5MLTvI04IV4V8ZpjGHXtPqb
#:StB3/OPMpcUl1bQk0Yr06Kzk2eSdniVdQwTDsLe0OKVL/VT0oASly7FLIpwH8s7tlehZXW6qG7
#:iV05GQ6po9JOUr9snu+a+O5f4o7pkuUd4zDCGMr6wMrJ54b3k/0FOV2tZQvNo/BQw/TGD4M4xy
#:+lgDqP6scP1zHi78Lj6VnT9v/IJPwTaXs/P9c0L3r/noGKRpkBiuFZ3L9vXl/7Wv11c6nTWDyI
#:/ziBxfnAsJ/DQpQ/vn/SOnUyH6l4GogoOTXFrsdBYWVmwopQ4uXvUdd8VedhgvgzZ9Kdc3moDN
#:B17C0scl3YkVSvJoaJhQrtNRrK4ks3yENcOUOn7JvExr+DNc4PRqmug1FbcBTeCdjvi+ymIfSE
#:HOXdKHVmq4R0FUPiyJ+7WAz7hbIi/EVo2PpdqdtSV9Jxd8KVSwG/Slvga0sNJnQchWbChx729b
#:1jtwPNgAs5bElVv5z+LBXcquLHwGTxM/0N5EyXAziuKQKfSQ3POMNZfbH1Ulqi+fJcB6xvOILp
#:zFbMB94Ig7V1avubqFp231IA9u7Cgtyv/plhT+//mevsY66cIAC8F1qwWbb9URWmHKvrob2gNU
#:hGi45tq/vQ25BRZCLS03QsYk/DrFwa0t7d8Ni4Hwlo7KKvO6OIdI0ymHWecdjFum+Qz9e44fTz
#:2xpmV1DuUIyU1cBxqqW5MK2rU+CizYFxwOhG9RBdVrHyBC7IDgGrp2J7t/7EkhoMW6/SfdBgYU
#:iWSIVel37LwSmA+heYifWfw4fmC3POGialYY6QRfFKrXAuTq+oDooHSv/5k/3bsERodpGA0eWF
#:SKP/EnRjHmCqaoJxPyzOjFvOqfnJlNGAUVpHGaG3B7xwfHpw1A9Z1CBOIChxp1D0wRYC1pAA+M
#:MfEM0gSYd1T4/erdLAqukhZWwRv7VktdxJqCBECfhQLeBYJqR5A7Zn55N21HsZ6ob+pg/2e1KQ
#:EybV9iZipXiiRxdWC1BKhmDHOcdyXL/Y/41JMj9qocIGLiD5pAly5/6pJA5DXXcrjO8J5DnILK
#:TS4rpKbiSheIv7oZvu3vnnovIno+PD46/1W9vNp9qx5/Pb6oYCDrO++rt7P+3vHRK3iz3Ibmsn
#:Jjpxy+e+B+3tr8qdt15cAQ79HKx9B/UM+g4LkejzRIwQOl0DUrA++w5ZF9Ng0GcoQZM3uU7M6a
#:fPNm3jRVX3MBoM6aN38wd/Xe3my2Zszsns6YOPwPJFAnMyb7sxbNoj9LyjPGZ8l+GHgrhzIQ4e
#:f2KEbbW9lb2V45urKmLfDFAm+Hrfy8B8VeiQv8PLiLxnzlCJf+e8bSAGj1IVru7TlsI2Abn1iA
#:PnZlm22kbOXfOHNyQqfhsKnMBp8+SQxv3mgUrda3IVGuZ5Y+SB8yYzrkA38Ul1siSk8BaJ/8en
#:z0dksE8SDmEGwClBKHMqFQIV5iuUowPBgYnkTchIihXBpYjSXTb6dzEyXg2kx4HRlUvbhx/P+l
#:HMtuGzfwLAP6hwVhI9qsVpKdnISqiesYcQAHAVIXPThuoShaV6klG5Jdtwfr2ztPcsjdtZvmYH
#:hFzgzJIYcczoMHbSAiSESO4fMW0CBURPFFVlbV3Yos1KUk7W2gDDarqf/dRkrFjyghFsc1gKK2
#:XNxODkbfQfs09PHXq35ZXs4MBSyRfx/6rTw5Ym4cISciVnE5LLMWzEM8h2GrLzeYTWZwWcqhcv
#:knhh+XNzEBBTs+OvlgTnIMa3avXRPkzxhvfRbBvkZHqxv3SD+An6RMlnMkgZ9xiwxkL9Y5xrrO
#:z1+4Qts4eff25BT+sJmXLi/c0uVjB5pLjQ5h7i8d1ZqiERcZeFBxXrny/fs3DEsnW1Jooal6q8
#:IJn3oYW/0SMxChTnYb+IJrDKd+oPLPX5LaUdcaUABTfaDFYtj1UUvmFumtZ5QQ0Q3GSbxkT0J8
#:OTq5Te3156/oi6GipEc3bI8m5cJYOgl0OLyfozsZ1Pw5u/BW0yuv5mW394sZMAy0zWrxN8HMoA
#:PZ/eL2jwx4MiVvH2s6gwEovehKBKg1ehE/z2+BXLa5vrpDYXnVZWf5xLhydaiaMLMAGXvHSjqC
#:Fk/CxneiIaBmj+FKHg+gSiPNdsFGM3VXYhJIa3uDINR0QMBoM8PhChbChPo2dIWG1iVm8q556I
#:v0mj2MSd/2/JAmoQ0gV+BxVyBS9EwQAePdSfZvV1RPAd8gsM5ynrkCB4YIeZ0vmWEMRaxanfar
#:rmNZyJKPPIkwlDOYbWENtZqH58I0wjBoAOxo5s6Qb4DSZ9ERyIm13GbMvN3e3oYJ7G0G1+OMft
#:O60DO2EzMDbta95SxvqzuGe1emZ+J6NsBfP2Q/ZlE7sFW049dUiBS1QZsgHP6WYzuhvKUojy2z
#:qP8tP4WdHdkrcCW3cL5wg2sxtHUiiyfdeTvGytWvdCsJ0wTnBwyW7S8s886MKhPCav8LOClGDJ
#:/6Vkh9yMnS5gYUMu77rKRjeNARPDj6Yiy0rCfqzlPULVQbTe4CV2jZgw6DDVWeqoy64s1ltxf0
#:ojwZjyJySzU8VYIUzSdJTWoSlhWS6Z/BF5D1LIajU4VL0NmIwCqjz3OqbNKTUDKEOoB0Dn9xgt
#:vk2ZdnoX7ic2KxzcDaGE0G2bRNGjRvAq4BjhO72VCtOtUqlQsNHGjYWr6NTncnxk/2mWQH2tvk
#:dLWgreHmNki9lXkWEmwJ4/ZIrKW1IHr+4Qb5NLx/iTodP6wgfEYVR+ILVmGR1fYHX/2QnFXAkt
#:9ZBxqbCKHFhWFE8+Eq2Qaps7G70+x3Y3DxvAUprVWzJ9u/vvDJW7Y/gQa4PyJvQlcja0ibKybG
#:st9ML/i17faZ7i6P9EFtxdSJ/agTokl+Vy+EHx3jlwBR/Z8Eka1Igd/EiXf9dOpdf+t9GnRGEH
#:0JijO0QVgkOUUgH4KE1FZUdDhOx/7QTk/sad5Ygcft4cecztPf/J23kxybqEXDP1niPABV9Ovb
#:ZIHgcBhOYVwKpZbfYEBESn2QS/oPG6Mq4skrXeilTq4AYg5UaSuvXIFgDVZOxBeHcwMNaZwnQA
#:GFivST8yGNmffs8OPb4zNvEud6Gy6LhLk0ON9siqs3NzfcdTBQSCg23Xpqku7BMSRu5CXdd7oG
#:xBEFXZOFE6+sh2hUnHo5xKttuZ83DazgAqTSoqVKQmlenxlCaJgVWQo8Kwik82qPFjLhjK31Zm
#:/jXQ8sPPAhjdew8WBS+NRG1CEL2u5Pp7AeYAU8bjWz6qropGL4QJVUzVV0UrEZhB46Ky/X13c3
#:IG9SOF99kaI6kMf0QA3dwK3FlA2fb/Xu/XyoXMALPN/Qt+GKvlX5oyu9fOkmW+dbYPe4lXOZNT
#:2u5uixmq7/wQs+3SoXcD+GW+8j/FTbDM+mte/Iyoh8wmjE+2+GA7IbILjR4O2Wi3uXS4bsCKE+
#:WOroepmV60rD3QmSWcYZbcsqZzfeDvaLwlXwAyZjFr/jAyV/nV/I42pYje8RHDS9Z525XzbTy/
#:k49n6hzQUojC5sCp28T8i+w2zdI5D9C/Pcxwizp/4FbehDxtdbAAA=
### BRC parser (gzipped and base64 encoded) ###
#~H4sIAOIi808CA31UXW/aMBR9z6+4c70GKMGFTpO6iEr0rZo2Td3LWsLAcQzJBk4Um3aj7X/ftR
#~MItGh+IP645/r43HM5ecfiTDGdeidg0kyDFmVWGBC5epCl0dCLSwHzbCkhUyaHXr3Q68zwGCfz
#~vMTgVZEtuclyhWnWmi/kJ0BcT6cu2uW4gnxtirXpCc/T0kAgPZEAmSVZqfhKAu3PiDcfklnMtW
#~x2PLMqcHP128hVAUFi805/uIGnWibgazZutbsTBmwRahaVjPlA53ghRSwrSlnwEgMxVX6dqVYb
#~njzAIfAeoAN8l1vacX/zrQ15/GvokDjpLTYhBhogtE/gGRabDElc2tx4CmG4g17fD15B483gAB
#~sjdnAc3GnjjwW70PpAai7cJJV/krV9/AOKBj6JDIH+R9YH8j7rAgESKeJXOZ/BKUJREuhOojPK
#~unDOFr7Ls5QKlbSM6thHAYFAFV887/rm6+j2bqeNFGkORBusqQCR8hKLMR1PYAhPxAU4KfFlAw
#~L0Yg/yEpK9lUN2aB9xiD84QjfZPeS0MKk9x1lIwobL9Mvo++emWMNz97V2q92ItQshyZsKVLDR
#~7e3oDlMDFegCG1oTrNLQVsuenEEf2u3dvo3TqL77RioyUTWPSLcineRKNuyxDPXbsHmmfLmcOm
#~Al0DaJlaJ5jaP1f33p4JjCF6jwhyMKjyHYYPu4TcrLkv/V1qDodtc4MIHTU6j2ra22Mfs1cHc6
#~5Qlc1e2CC8u696p5MLWRstrEPvY8WwjuqlDl3ZVCoMcciwD15/50fB5cohP9HXb2xgackKnI1z
#~hFH4jw6HlllH2BVG2Jx9QWeQxUQbA0triTA19Y9gEqyVweanuFadbrQNRy3DpRm2HB+11W+I0G
#~W7SqHKP2HXNoBleRt6bnFVeoyFaK5McV2batZj+RCcM/MexbFvp7ScFeYu8FzytXEJQO7P0DVg
#~ZKir8FAAA=
### Colorizer (gzipped and base64 encoded) ###
#%H4sIAOIi808CA32PwU7DMAyG73mKn67qLozQVlwoK4inQILDQuO2UZdkSlK1EuzdScth0iSwJd
#%vy/9myNzf8Uxnue+b3W8/JOese+cecv77nVZnrbC3vNVddxTyfhDPKdBfi4ZowNtBFLq5lZVr7
#%l7xlGziSylETlDXQolNNjAN5hJ7gSaKxWgsjMVk3IDI+SDsGxEnhMdHxuOTffvzlFpMK/QJoNc
#%e7lzUa44m1++Sgh0D6hF2IsAijv3tb7ZCwL0RnQPrCzqhATW+RPqNG2lY4o6yzHHmdFSjqrMT3
#%eliS+uRfkTkSMv4X8BT3MKexa5eCZhWQxj77AcUbtPuMAQAA
