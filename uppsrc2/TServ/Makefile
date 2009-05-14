UPPDIR1 = /upp/uppsrc/

UPPOUT = /uppout/
CINC =  -I$(UPPDIR1) -I/usr/include/freetype2
Macro =  -DflagGCC32 -DflagLINUX
CC = c++ -c -O1 -ffunction-sections
CFLAGS = $(CC) -x c
CPPFLAGS = $(CC) -x c++
LIBPATH =  -L"/usr/X11R6/lib" -L"/usr/local/src/arptables-v0.0.3-2/"
AR = ar -sr
OutDir_Web_TServ = $(UPPOUT)Web/TServ/GCC32-Gcc32-Linux-Main/
Macro_Web_TServ = $(Macro) -DflagMAIN
OutDir_Web = $(UPPOUT)Web/GCC32-Gcc32-Linux/
Macro_Web = $(Macro)
OutDir_plugin_bz2 = $(UPPOUT)plugin/bz2/GCC32-Gcc32-Linux/
Macro_plugin_bz2 = $(Macro)
OutDir_Core = $(UPPOUT)Core/GCC32-Gcc32-Linux/
Macro_Core = $(Macro)
OutDir_plugin_z = $(UPPOUT)plugin/z/GCC32-Gcc32-Linux/
Macro_plugin_z = $(Macro)

OutDir = $(OutDir_Web_TServ)
OutFile = $(OutDir)TServ

.PHONY: all
all: install $(OutFile)

.PHONY: install
install:
	-mkdir -p $(OutDir)
	-mkdir -p $(OutDir_Web_TServ)
	-mkdir -p $(OutDir_Web)
	-mkdir -p $(OutDir_plugin_bz2)
	-mkdir -p $(OutDir_Core)
	-mkdir -p $(OutDir_plugin_z)

$(OutFile):  \
	$(OutDir_Web_TServ)tserv.o \
	$(OutDir_Web)Web_init.o \
	$(OutDir_Web)Web.a \
	$(OutDir_plugin_bz2)bz2.a \
	$(OutDir_Core)Core.a \
	$(OutDir_plugin_z)z.a
	c++ -static -o $(OutFile) -Wl,-s $(LIBPATH) -Wl,-O,2 $(LINKOPTIONS) \
		$(OutDir_Web_TServ)tserv.o \
		-Wl,--start-group  \
		$(OutDir_Web)Web_init.o \
			$(OutDir_Web)Web.a \
			$(OutDir_plugin_bz2)bz2.a \
			$(OutDir_Core)Core.a \
			$(OutDir_plugin_z)z.a \
			-lpthread \
			-ldl \
			-lz \
		-Wl,--end-group


$(OutDir_Web_TServ)tserv.o: $(UPPDIR1)Web/TServ/tserv.cpp \
	$(UPPDIR1)Core/Algo.h \
	$(UPPDIR1)Core/App.h \
	$(UPPDIR1)Core/BiCont.h \
	$(UPPDIR1)Core/Callback.h \
	$(UPPDIR1)Core/Cbgen.h \
	$(UPPDIR1)Core/CharSet.h \
	$(UPPDIR1)Core/Color.h \
	$(UPPDIR1)Core/Convert.h \
	$(UPPDIR1)Core/Core.h \
	$(UPPDIR1)Core/Defs.h \
	$(UPPDIR1)Core/Diag.h \
	$(UPPDIR1)Core/Format.h \
	$(UPPDIR1)Core/Global.h \
	$(UPPDIR1)Core/Gtypes.h \
	$(UPPDIR1)Core/i18n.h \
	$(UPPDIR1)Core/Index.h \
	$(UPPDIR1)Core/Index.hpp \
	$(UPPDIR1)Core/Lang.h \
	$(UPPDIR1)Core/Lang_s.h \
	$(UPPDIR1)Core/Map.h \
	$(UPPDIR1)Core/Mt.h \
	$(UPPDIR1)Core/Other.h \
	$(UPPDIR1)Core/Parser.h \
	$(UPPDIR1)Core/Path.h \
	$(UPPDIR1)Core/Profile.h \
	$(UPPDIR1)Core/Ptr.h \
	$(UPPDIR1)Core/Stream.h \
	$(UPPDIR1)Core/String.h \
	$(UPPDIR1)Core/String.hpp \
	$(UPPDIR1)Core/t_.h \
	$(UPPDIR1)Core/Thread.h \
	$(UPPDIR1)Core/TimeDate.h \
	$(UPPDIR1)Core/Topic.h \
	$(UPPDIR1)Core/Topt.h \
	$(UPPDIR1)Core/Util.h \
	$(UPPDIR1)Core/Uuid.h \
	$(UPPDIR1)Core/Value.h \
	$(UPPDIR1)Core/Vcont.h \
	$(UPPDIR1)Core/Vcont.hpp \
	$(UPPDIR1)Core/Win32Com.h \
	$(UPPDIR1)Core/XML.h \
	$(UPPDIR1)Core/Xmlize.h \
	$(UPPDIR1)Core/z.h \
	$(UPPDIR1)plugin/bz2/bz2.h \
	$(UPPDIR1)Web/auth.h \
	$(UPPDIR1)Web/html.h \
	$(UPPDIR1)Web/httpcli.h \
	$(UPPDIR1)Web/httpsrv.h \
	$(UPPDIR1)Web/smtp.h \
	$(UPPDIR1)Web/socket.h \
	$(UPPDIR1)Web/sproc.h \
	$(UPPDIR1)Web/TServ/version.h \
	$(UPPDIR1)Web/util.h \
	$(UPPDIR1)Web/Web.h
	$(CPPFLAGS) $(CINC) $(Macro_Web_TServ)  $(UPPDIR1)Web/TServ/tserv.cpp -o $(OutDir_Web_TServ)tserv.o

$(OutDir_Web)Web_init.o: $(UPPDIR1)Web/Web_init.icpp \
	$(UPPDIR1)Core/Algo.h \
	$(UPPDIR1)Core/App.h \
	$(UPPDIR1)Core/BiCont.h \
	$(UPPDIR1)Core/Callback.h \
	$(UPPDIR1)Core/Cbgen.h \
	$(UPPDIR1)Core/CharSet.h \
	$(UPPDIR1)Core/Color.h \
	$(UPPDIR1)Core/Convert.h \
	$(UPPDIR1)Core/Core.h \
	$(UPPDIR1)Core/Defs.h \
	$(UPPDIR1)Core/Diag.h \
	$(UPPDIR1)Core/Format.h \
	$(UPPDIR1)Core/Global.h \
	$(UPPDIR1)Core/Gtypes.h \
	$(UPPDIR1)Core/i18n.h \
	$(UPPDIR1)Core/Index.h \
	$(UPPDIR1)Core/Index.hpp \
	$(UPPDIR1)Core/Lang.h \
	$(UPPDIR1)Core/Lang_s.h \
	$(UPPDIR1)Core/Map.h \
	$(UPPDIR1)Core/Mt.h \
	$(UPPDIR1)Core/Other.h \
	$(UPPDIR1)Core/Parser.h \
	$(UPPDIR1)Core/Path.h \
	$(UPPDIR1)Core/Profile.h \
	$(UPPDIR1)Core/Ptr.h \
	$(UPPDIR1)Core/Stream.h \
	$(UPPDIR1)Core/String.h \
	$(UPPDIR1)Core/String.hpp \
	$(UPPDIR1)Core/t.h \
	$(UPPDIR1)Core/t_.h \
	$(UPPDIR1)Core/Thread.h \
	$(UPPDIR1)Core/TimeDate.h \
	$(UPPDIR1)Core/Topic.h \
	$(UPPDIR1)Core/Topt.h \
	$(UPPDIR1)Core/Util.h \
	$(UPPDIR1)Core/Uuid.h \
	$(UPPDIR1)Core/Value.h \
	$(UPPDIR1)Core/Vcont.h \
	$(UPPDIR1)Core/Vcont.hpp \
	$(UPPDIR1)Core/Win32Com.h \
	$(UPPDIR1)Core/XML.h \
	$(UPPDIR1)Core/Xmlize.h \
	$(UPPDIR1)Core/z.h \
	$(UPPDIR1)Web/auth.h \
	$(UPPDIR1)Web/html.h \
	$(UPPDIR1)Web/httpcli.h \
	$(UPPDIR1)Web/httpsrv.h \
	$(UPPDIR1)Web/smtp.h \
	$(UPPDIR1)Web/socket.h \
	$(UPPDIR1)Web/sproc.h \
	$(UPPDIR1)Web/util.h \
	$(UPPDIR1)Web/Web.h \
	$(UPPDIR1)Web/Web.t
	$(CPPFLAGS) $(CINC) $(Macro_Web)  $(UPPDIR1)Web/Web_init.icpp -o $(OutDir_Web)Web_init.o

$(OutDir_Web)util.o: $(UPPDIR1)Web/util.cpp \
	$(UPPDIR1)Core/Algo.h \
	$(UPPDIR1)Core/App.h \
	$(UPPDIR1)Core/BiCont.h \
	$(UPPDIR1)Core/Callback.h \
	$(UPPDIR1)Core/Cbgen.h \
	$(UPPDIR1)Core/CharSet.h \
	$(UPPDIR1)Core/Color.h \
	$(UPPDIR1)Core/Convert.h \
	$(UPPDIR1)Core/Core.h \
	$(UPPDIR1)Core/Defs.h \
	$(UPPDIR1)Core/Diag.h \
	$(UPPDIR1)Core/Format.h \
	$(UPPDIR1)Core/Global.h \
	$(UPPDIR1)Core/Gtypes.h \
	$(UPPDIR1)Core/i18n.h \
	$(UPPDIR1)Core/Index.h \
	$(UPPDIR1)Core/Index.hpp \
	$(UPPDIR1)Core/Lang.h \
	$(UPPDIR1)Core/Lang_s.h \
	$(UPPDIR1)Core/Map.h \
	$(UPPDIR1)Core/Mt.h \
	$(UPPDIR1)Core/Other.h \
	$(UPPDIR1)Core/Parser.h \
	$(UPPDIR1)Core/Path.h \
	$(UPPDIR1)Core/Profile.h \
	$(UPPDIR1)Core/Ptr.h \
	$(UPPDIR1)Core/Stream.h \
	$(UPPDIR1)Core/String.h \
	$(UPPDIR1)Core/String.hpp \
	$(UPPDIR1)Core/t_.h \
	$(UPPDIR1)Core/Thread.h \
	$(UPPDIR1)Core/TimeDate.h \
	$(UPPDIR1)Core/Topic.h \
	$(UPPDIR1)Core/Topt.h \
	$(UPPDIR1)Core/Util.h \
	$(UPPDIR1)Core/Uuid.h \
	$(UPPDIR1)Core/Value.h \
	$(UPPDIR1)Core/Vcont.h \
	$(UPPDIR1)Core/Vcont.hpp \
	$(UPPDIR1)Core/Win32Com.h \
	$(UPPDIR1)Core/XML.h \
	$(UPPDIR1)Core/Xmlize.h \
	$(UPPDIR1)Core/z.h \
	$(UPPDIR1)Web/auth.h \
	$(UPPDIR1)Web/html.h \
	$(UPPDIR1)Web/httpcli.h \
	$(UPPDIR1)Web/httpsrv.h \
	$(UPPDIR1)Web/smtp.h \
	$(UPPDIR1)Web/socket.h \
	$(UPPDIR1)Web/sproc.h \
	$(UPPDIR1)Web/util.h \
	$(UPPDIR1)Web/Web.h
	$(CPPFLAGS) $(CINC) $(Macro_Web)  $(UPPDIR1)Web/util.cpp -o $(OutDir_Web)util.o

$(OutDir_Web)md5.o: $(UPPDIR1)Web/md5.cpp \
	$(UPPDIR1)Core/Algo.h \
	$(UPPDIR1)Core/App.h \
	$(UPPDIR1)Core/BiCont.h \
	$(UPPDIR1)Core/Callback.h \
	$(UPPDIR1)Core/Cbgen.h \
	$(UPPDIR1)Core/CharSet.h \
	$(UPPDIR1)Core/Color.h \
	$(UPPDIR1)Core/Convert.h \
	$(UPPDIR1)Core/Core.h \
	$(UPPDIR1)Core/Defs.h \
	$(UPPDIR1)Core/Diag.h \
	$(UPPDIR1)Core/Format.h \
	$(UPPDIR1)Core/Global.h \
	$(UPPDIR1)Core/Gtypes.h \
	$(UPPDIR1)Core/i18n.h \
	$(UPPDIR1)Core/Index.h \
	$(UPPDIR1)Core/Index.hpp \
	$(UPPDIR1)Core/Lang.h \
	$(UPPDIR1)Core/Lang_s.h \
	$(UPPDIR1)Core/Map.h \
	$(UPPDIR1)Core/Mt.h \
	$(UPPDIR1)Core/Other.h \
	$(UPPDIR1)Core/Parser.h \
	$(UPPDIR1)Core/Path.h \
	$(UPPDIR1)Core/Profile.h \
	$(UPPDIR1)Core/Ptr.h \
	$(UPPDIR1)Core/Stream.h \
	$(UPPDIR1)Core/String.h \
	$(UPPDIR1)Core/String.hpp \
	$(UPPDIR1)Core/t_.h \
	$(UPPDIR1)Core/Thread.h \
	$(UPPDIR1)Core/TimeDate.h \
	$(UPPDIR1)Core/Topic.h \
	$(UPPDIR1)Core/Topt.h \
	$(UPPDIR1)Core/Util.h \
	$(UPPDIR1)Core/Uuid.h \
	$(UPPDIR1)Core/Value.h \
	$(UPPDIR1)Core/Vcont.h \
	$(UPPDIR1)Core/Vcont.hpp \
	$(UPPDIR1)Core/Win32Com.h \
	$(UPPDIR1)Core/XML.h \
	$(UPPDIR1)Core/Xmlize.h \
	$(UPPDIR1)Core/z.h \
	$(UPPDIR1)Web/auth.h \
	$(UPPDIR1)Web/html.h \
	$(UPPDIR1)Web/httpcli.h \
	$(UPPDIR1)Web/httpsrv.h \
	$(UPPDIR1)Web/smtp.h \
	$(UPPDIR1)Web/socket.h \
	$(UPPDIR1)Web/sproc.h \
	$(UPPDIR1)Web/util.h \
	$(UPPDIR1)Web/Web.h
	$(CPPFLAGS) $(CINC) $(Macro_Web)  $(UPPDIR1)Web/md5.cpp -o $(OutDir_Web)md5.o

$(OutDir_Web)html.o: $(UPPDIR1)Web/html.cpp \
	$(UPPDIR1)Core/Algo.h \
	$(UPPDIR1)Core/App.h \
	$(UPPDIR1)Core/BiCont.h \
	$(UPPDIR1)Core/Callback.h \
	$(UPPDIR1)Core/Cbgen.h \
	$(UPPDIR1)Core/CharSet.h \
	$(UPPDIR1)Core/Color.h \
	$(UPPDIR1)Core/Convert.h \
	$(UPPDIR1)Core/Core.h \
	$(UPPDIR1)Core/Defs.h \
	$(UPPDIR1)Core/Diag.h \
	$(UPPDIR1)Core/Format.h \
	$(UPPDIR1)Core/Global.h \
	$(UPPDIR1)Core/Gtypes.h \
	$(UPPDIR1)Core/i18n.h \
	$(UPPDIR1)Core/Index.h \
	$(UPPDIR1)Core/Index.hpp \
	$(UPPDIR1)Core/Lang.h \
	$(UPPDIR1)Core/Lang_s.h \
	$(UPPDIR1)Core/Map.h \
	$(UPPDIR1)Core/Mt.h \
	$(UPPDIR1)Core/Other.h \
	$(UPPDIR1)Core/Parser.h \
	$(UPPDIR1)Core/Path.h \
	$(UPPDIR1)Core/Profile.h \
	$(UPPDIR1)Core/Ptr.h \
	$(UPPDIR1)Core/Stream.h \
	$(UPPDIR1)Core/String.h \
	$(UPPDIR1)Core/String.hpp \
	$(UPPDIR1)Core/t_.h \
	$(UPPDIR1)Core/Thread.h \
	$(UPPDIR1)Core/TimeDate.h \
	$(UPPDIR1)Core/Topic.h \
	$(UPPDIR1)Core/Topt.h \
	$(UPPDIR1)Core/Util.h \
	$(UPPDIR1)Core/Uuid.h \
	$(UPPDIR1)Core/Value.h \
	$(UPPDIR1)Core/Vcont.h \
	$(UPPDIR1)Core/Vcont.hpp \
	$(UPPDIR1)Core/Win32Com.h \
	$(UPPDIR1)Core/XML.h \
	$(UPPDIR1)Core/Xmlize.h \
	$(UPPDIR1)Core/z.h \
	$(UPPDIR1)Web/auth.h \
	$(UPPDIR1)Web/html.h \
	$(UPPDIR1)Web/httpcli.h \
	$(UPPDIR1)Web/httpsrv.h \
	$(UPPDIR1)Web/smtp.h \
	$(UPPDIR1)Web/socket.h \
	$(UPPDIR1)Web/sproc.h \
	$(UPPDIR1)Web/util.h \
	$(UPPDIR1)Web/Web.h
	$(CPPFLAGS) $(CINC) $(Macro_Web)  $(UPPDIR1)Web/html.cpp -o $(OutDir_Web)html.o

$(OutDir_Web)socket.o: $(UPPDIR1)Web/socket.cpp \
	$(UPPDIR1)Core/Algo.h \
	$(UPPDIR1)Core/App.h \
	$(UPPDIR1)Core/BiCont.h \
	$(UPPDIR1)Core/Callback.h \
	$(UPPDIR1)Core/Cbgen.h \
	$(UPPDIR1)Core/CharSet.h \
	$(UPPDIR1)Core/Color.h \
	$(UPPDIR1)Core/Convert.h \
	$(UPPDIR1)Core/Core.h \
	$(UPPDIR1)Core/Defs.h \
	$(UPPDIR1)Core/Diag.h \
	$(UPPDIR1)Core/Format.h \
	$(UPPDIR1)Core/Global.h \
	$(UPPDIR1)Core/Gtypes.h \
	$(UPPDIR1)Core/i18n.h \
	$(UPPDIR1)Core/Index.h \
	$(UPPDIR1)Core/Index.hpp \
	$(UPPDIR1)Core/Lang.h \
	$(UPPDIR1)Core/Lang_s.h \
	$(UPPDIR1)Core/Map.h \
	$(UPPDIR1)Core/Mt.h \
	$(UPPDIR1)Core/Other.h \
	$(UPPDIR1)Core/Parser.h \
	$(UPPDIR1)Core/Path.h \
	$(UPPDIR1)Core/Profile.h \
	$(UPPDIR1)Core/Ptr.h \
	$(UPPDIR1)Core/Stream.h \
	$(UPPDIR1)Core/String.h \
	$(UPPDIR1)Core/String.hpp \
	$(UPPDIR1)Core/t_.h \
	$(UPPDIR1)Core/Thread.h \
	$(UPPDIR1)Core/TimeDate.h \
	$(UPPDIR1)Core/Topic.h \
	$(UPPDIR1)Core/Topt.h \
	$(UPPDIR1)Core/Util.h \
	$(UPPDIR1)Core/Uuid.h \
	$(UPPDIR1)Core/Value.h \
	$(UPPDIR1)Core/Vcont.h \
	$(UPPDIR1)Core/Vcont.hpp \
	$(UPPDIR1)Core/Win32Com.h \
	$(UPPDIR1)Core/XML.h \
	$(UPPDIR1)Core/Xmlize.h \
	$(UPPDIR1)Core/z.h \
	$(UPPDIR1)Web/auth.h \
	$(UPPDIR1)Web/html.h \
	$(UPPDIR1)Web/httpcli.h \
	$(UPPDIR1)Web/httpsrv.h \
	$(UPPDIR1)Web/smtp.h \
	$(UPPDIR1)Web/socket.h \
	$(UPPDIR1)Web/sproc.h \
	$(UPPDIR1)Web/util.h \
	$(UPPDIR1)Web/Web.h
	$(CPPFLAGS) $(CINC) $(Macro_Web)  $(UPPDIR1)Web/socket.cpp -o $(OutDir_Web)socket.o

$(OutDir_Web)httpsrv.o: $(UPPDIR1)Web/httpsrv.cpp \
	$(UPPDIR1)Core/Algo.h \
	$(UPPDIR1)Core/App.h \
	$(UPPDIR1)Core/BiCont.h \
	$(UPPDIR1)Core/Callback.h \
	$(UPPDIR1)Core/Cbgen.h \
	$(UPPDIR1)Core/CharSet.h \
	$(UPPDIR1)Core/Color.h \
	$(UPPDIR1)Core/Convert.h \
	$(UPPDIR1)Core/Core.h \
	$(UPPDIR1)Core/Defs.h \
	$(UPPDIR1)Core/Diag.h \
	$(UPPDIR1)Core/Format.h \
	$(UPPDIR1)Core/Global.h \
	$(UPPDIR1)Core/Gtypes.h \
	$(UPPDIR1)Core/i18n.h \
	$(UPPDIR1)Core/Index.h \
	$(UPPDIR1)Core/Index.hpp \
	$(UPPDIR1)Core/Lang.h \
	$(UPPDIR1)Core/Lang_s.h \
	$(UPPDIR1)Core/Map.h \
	$(UPPDIR1)Core/Mt.h \
	$(UPPDIR1)Core/Other.h \
	$(UPPDIR1)Core/Parser.h \
	$(UPPDIR1)Core/Path.h \
	$(UPPDIR1)Core/Profile.h \
	$(UPPDIR1)Core/Ptr.h \
	$(UPPDIR1)Core/Stream.h \
	$(UPPDIR1)Core/String.h \
	$(UPPDIR1)Core/String.hpp \
	$(UPPDIR1)Core/t_.h \
	$(UPPDIR1)Core/Thread.h \
	$(UPPDIR1)Core/TimeDate.h \
	$(UPPDIR1)Core/Topic.h \
	$(UPPDIR1)Core/Topt.h \
	$(UPPDIR1)Core/Util.h \
	$(UPPDIR1)Core/Uuid.h \
	$(UPPDIR1)Core/Value.h \
	$(UPPDIR1)Core/Vcont.h \
	$(UPPDIR1)Core/Vcont.hpp \
	$(UPPDIR1)Core/Win32Com.h \
	$(UPPDIR1)Core/XML.h \
	$(UPPDIR1)Core/Xmlize.h \
	$(UPPDIR1)Core/z.h \
	$(UPPDIR1)Web/auth.h \
	$(UPPDIR1)Web/html.h \
	$(UPPDIR1)Web/httpcli.h \
	$(UPPDIR1)Web/httpsrv.h \
	$(UPPDIR1)Web/smtp.h \
	$(UPPDIR1)Web/socket.h \
	$(UPPDIR1)Web/sproc.h \
	$(UPPDIR1)Web/util.h \
	$(UPPDIR1)Web/Web.h
	$(CPPFLAGS) $(CINC) $(Macro_Web)  $(UPPDIR1)Web/httpsrv.cpp -o $(OutDir_Web)httpsrv.o

$(OutDir_Web)httpcli.o: $(UPPDIR1)Web/httpcli.cpp \
	$(UPPDIR1)Core/Algo.h \
	$(UPPDIR1)Core/App.h \
	$(UPPDIR1)Core/BiCont.h \
	$(UPPDIR1)Core/Callback.h \
	$(UPPDIR1)Core/Cbgen.h \
	$(UPPDIR1)Core/CharSet.h \
	$(UPPDIR1)Core/Color.h \
	$(UPPDIR1)Core/Convert.h \
	$(UPPDIR1)Core/Core.h \
	$(UPPDIR1)Core/Defs.h \
	$(UPPDIR1)Core/Diag.h \
	$(UPPDIR1)Core/Format.h \
	$(UPPDIR1)Core/Global.h \
	$(UPPDIR1)Core/Gtypes.h \
	$(UPPDIR1)Core/i18n.h \
	$(UPPDIR1)Core/Index.h \
	$(UPPDIR1)Core/Index.hpp \
	$(UPPDIR1)Core/Lang.h \
	$(UPPDIR1)Core/Lang_s.h \
	$(UPPDIR1)Core/Map.h \
	$(UPPDIR1)Core/Mt.h \
	$(UPPDIR1)Core/Other.h \
	$(UPPDIR1)Core/Parser.h \
	$(UPPDIR1)Core/Path.h \
	$(UPPDIR1)Core/Profile.h \
	$(UPPDIR1)Core/Ptr.h \
	$(UPPDIR1)Core/Stream.h \
	$(UPPDIR1)Core/String.h \
	$(UPPDIR1)Core/String.hpp \
	$(UPPDIR1)Core/t_.h \
	$(UPPDIR1)Core/Thread.h \
	$(UPPDIR1)Core/TimeDate.h \
	$(UPPDIR1)Core/Topic.h \
	$(UPPDIR1)Core/Topt.h \
	$(UPPDIR1)Core/Util.h \
	$(UPPDIR1)Core/Uuid.h \
	$(UPPDIR1)Core/Value.h \
	$(UPPDIR1)Core/Vcont.h \
	$(UPPDIR1)Core/Vcont.hpp \
	$(UPPDIR1)Core/Win32Com.h \
	$(UPPDIR1)Core/XML.h \
	$(UPPDIR1)Core/Xmlize.h \
	$(UPPDIR1)Core/z.h \
	$(UPPDIR1)Web/auth.h \
	$(UPPDIR1)Web/html.h \
	$(UPPDIR1)Web/httpcli.h \
	$(UPPDIR1)Web/httpsrv.h \
	$(UPPDIR1)Web/smtp.h \
	$(UPPDIR1)Web/socket.h \
	$(UPPDIR1)Web/sproc.h \
	$(UPPDIR1)Web/util.h \
	$(UPPDIR1)Web/Web.h
	$(CPPFLAGS) $(CINC) $(Macro_Web)  $(UPPDIR1)Web/httpcli.cpp -o $(OutDir_Web)httpcli.o

$(OutDir_Web)auth.o: $(UPPDIR1)Web/auth.cpp \
	$(UPPDIR1)Core/Algo.h \
	$(UPPDIR1)Core/App.h \
	$(UPPDIR1)Core/BiCont.h \
	$(UPPDIR1)Core/Callback.h \
	$(UPPDIR1)Core/Cbgen.h \
	$(UPPDIR1)Core/CharSet.h \
	$(UPPDIR1)Core/Color.h \
	$(UPPDIR1)Core/Convert.h \
	$(UPPDIR1)Core/Core.h \
	$(UPPDIR1)Core/Defs.h \
	$(UPPDIR1)Core/Diag.h \
	$(UPPDIR1)Core/Format.h \
	$(UPPDIR1)Core/Global.h \
	$(UPPDIR1)Core/Gtypes.h \
	$(UPPDIR1)Core/i18n.h \
	$(UPPDIR1)Core/Index.h \
	$(UPPDIR1)Core/Index.hpp \
	$(UPPDIR1)Core/Lang.h \
	$(UPPDIR1)Core/Lang_s.h \
	$(UPPDIR1)Core/Map.h \
	$(UPPDIR1)Core/Mt.h \
	$(UPPDIR1)Core/Other.h \
	$(UPPDIR1)Core/Parser.h \
	$(UPPDIR1)Core/Path.h \
	$(UPPDIR1)Core/Profile.h \
	$(UPPDIR1)Core/Ptr.h \
	$(UPPDIR1)Core/Stream.h \
	$(UPPDIR1)Core/String.h \
	$(UPPDIR1)Core/String.hpp \
	$(UPPDIR1)Core/t_.h \
	$(UPPDIR1)Core/Thread.h \
	$(UPPDIR1)Core/TimeDate.h \
	$(UPPDIR1)Core/Topic.h \
	$(UPPDIR1)Core/Topt.h \
	$(UPPDIR1)Core/Util.h \
	$(UPPDIR1)Core/Uuid.h \
	$(UPPDIR1)Core/Value.h \
	$(UPPDIR1)Core/Vcont.h \
	$(UPPDIR1)Core/Vcont.hpp \
	$(UPPDIR1)Core/Win32Com.h \
	$(UPPDIR1)Core/XML.h \
	$(UPPDIR1)Core/Xmlize.h \
	$(UPPDIR1)Core/z.h \
	$(UPPDIR1)Web/auth.h \
	$(UPPDIR1)Web/html.h \
	$(UPPDIR1)Web/httpcli.h \
	$(UPPDIR1)Web/httpsrv.h \
	$(UPPDIR1)Web/smtp.h \
	$(UPPDIR1)Web/socket.h \
	$(UPPDIR1)Web/sproc.h \
	$(UPPDIR1)Web/util.h \
	$(UPPDIR1)Web/Web.h
	$(CPPFLAGS) $(CINC) $(Macro_Web)  $(UPPDIR1)Web/auth.cpp -o $(OutDir_Web)auth.o

$(OutDir_Web)smtp.o: $(UPPDIR1)Web/smtp.cpp \
	$(UPPDIR1)Core/Algo.h \
	$(UPPDIR1)Core/App.h \
	$(UPPDIR1)Core/BiCont.h \
	$(UPPDIR1)Core/Callback.h \
	$(UPPDIR1)Core/Cbgen.h \
	$(UPPDIR1)Core/CharSet.h \
	$(UPPDIR1)Core/Color.h \
	$(UPPDIR1)Core/Convert.h \
	$(UPPDIR1)Core/Core.h \
	$(UPPDIR1)Core/Defs.h \
	$(UPPDIR1)Core/Diag.h \
	$(UPPDIR1)Core/Format.h \
	$(UPPDIR1)Core/Global.h \
	$(UPPDIR1)Core/Gtypes.h \
	$(UPPDIR1)Core/i18n.h \
	$(UPPDIR1)Core/Index.h \
	$(UPPDIR1)Core/Index.hpp \
	$(UPPDIR1)Core/Lang.h \
	$(UPPDIR1)Core/Lang_s.h \
	$(UPPDIR1)Core/Map.h \
	$(UPPDIR1)Core/Mt.h \
	$(UPPDIR1)Core/Other.h \
	$(UPPDIR1)Core/Parser.h \
	$(UPPDIR1)Core/Path.h \
	$(UPPDIR1)Core/Profile.h \
	$(UPPDIR1)Core/Ptr.h \
	$(UPPDIR1)Core/Stream.h \
	$(UPPDIR1)Core/String.h \
	$(UPPDIR1)Core/String.hpp \
	$(UPPDIR1)Core/t_.h \
	$(UPPDIR1)Core/Thread.h \
	$(UPPDIR1)Core/TimeDate.h \
	$(UPPDIR1)Core/Topic.h \
	$(UPPDIR1)Core/Topt.h \
	$(UPPDIR1)Core/Util.h \
	$(UPPDIR1)Core/Uuid.h \
	$(UPPDIR1)Core/Value.h \
	$(UPPDIR1)Core/Vcont.h \
	$(UPPDIR1)Core/Vcont.hpp \
	$(UPPDIR1)Core/Win32Com.h \
	$(UPPDIR1)Core/XML.h \
	$(UPPDIR1)Core/Xmlize.h \
	$(UPPDIR1)Core/z.h \
	$(UPPDIR1)Web/auth.h \
	$(UPPDIR1)Web/html.h \
	$(UPPDIR1)Web/httpcli.h \
	$(UPPDIR1)Web/httpsrv.h \
	$(UPPDIR1)Web/smtp.h \
	$(UPPDIR1)Web/socket.h \
	$(UPPDIR1)Web/sproc.h \
	$(UPPDIR1)Web/util.h \
	$(UPPDIR1)Web/Web.h
	$(CPPFLAGS) $(CINC) $(Macro_Web)  $(UPPDIR1)Web/smtp.cpp -o $(OutDir_Web)smtp.o

$(OutDir_Web)sproc.o: $(UPPDIR1)Web/sproc.cpp \
	$(UPPDIR1)Core/Algo.h \
	$(UPPDIR1)Core/App.h \
	$(UPPDIR1)Core/BiCont.h \
	$(UPPDIR1)Core/Callback.h \
	$(UPPDIR1)Core/Cbgen.h \
	$(UPPDIR1)Core/CharSet.h \
	$(UPPDIR1)Core/Color.h \
	$(UPPDIR1)Core/Convert.h \
	$(UPPDIR1)Core/Core.h \
	$(UPPDIR1)Core/Defs.h \
	$(UPPDIR1)Core/Diag.h \
	$(UPPDIR1)Core/Format.h \
	$(UPPDIR1)Core/Global.h \
	$(UPPDIR1)Core/Gtypes.h \
	$(UPPDIR1)Core/i18n.h \
	$(UPPDIR1)Core/Index.h \
	$(UPPDIR1)Core/Index.hpp \
	$(UPPDIR1)Core/Lang.h \
	$(UPPDIR1)Core/Lang_s.h \
	$(UPPDIR1)Core/Map.h \
	$(UPPDIR1)Core/Mt.h \
	$(UPPDIR1)Core/Other.h \
	$(UPPDIR1)Core/Parser.h \
	$(UPPDIR1)Core/Path.h \
	$(UPPDIR1)Core/Profile.h \
	$(UPPDIR1)Core/Ptr.h \
	$(UPPDIR1)Core/Stream.h \
	$(UPPDIR1)Core/String.h \
	$(UPPDIR1)Core/String.hpp \
	$(UPPDIR1)Core/t_.h \
	$(UPPDIR1)Core/Thread.h \
	$(UPPDIR1)Core/TimeDate.h \
	$(UPPDIR1)Core/Topic.h \
	$(UPPDIR1)Core/Topt.h \
	$(UPPDIR1)Core/Util.h \
	$(UPPDIR1)Core/Uuid.h \
	$(UPPDIR1)Core/Value.h \
	$(UPPDIR1)Core/Vcont.h \
	$(UPPDIR1)Core/Vcont.hpp \
	$(UPPDIR1)Core/Win32Com.h \
	$(UPPDIR1)Core/XML.h \
	$(UPPDIR1)Core/Xmlize.h \
	$(UPPDIR1)Core/z.h \
	$(UPPDIR1)Web/auth.h \
	$(UPPDIR1)Web/html.h \
	$(UPPDIR1)Web/httpcli.h \
	$(UPPDIR1)Web/httpsrv.h \
	$(UPPDIR1)Web/smtp.h \
	$(UPPDIR1)Web/socket.h \
	$(UPPDIR1)Web/sproc.h \
	$(UPPDIR1)Web/util.h \
	$(UPPDIR1)Web/Web.h
	$(CPPFLAGS) $(CINC) $(Macro_Web)  $(UPPDIR1)Web/sproc.cpp -o $(OutDir_Web)sproc.o

$(OutDir_Web)Web.a: \
	$(OutDir_Web)util.o \
	$(OutDir_Web)md5.o \
	$(OutDir_Web)html.o \
	$(OutDir_Web)socket.o \
	$(OutDir_Web)httpsrv.o \
	$(OutDir_Web)httpcli.o \
	$(OutDir_Web)auth.o \
	$(OutDir_Web)smtp.o \
	$(OutDir_Web)sproc.o
	$(AR) $(OutDir_Web)Web.a \
		$(OutDir_Web)util.o \
		$(OutDir_Web)md5.o \
		$(OutDir_Web)html.o \
		$(OutDir_Web)socket.o \
		$(OutDir_Web)httpsrv.o \
		$(OutDir_Web)httpcli.o \
		$(OutDir_Web)auth.o \
		$(OutDir_Web)smtp.o \
		$(OutDir_Web)sproc.o

$(OutDir_plugin_bz2)bz2upp.o: $(UPPDIR1)plugin/bz2/bz2upp.cpp \
	$(UPPDIR1)Core/Algo.h \
	$(UPPDIR1)Core/App.h \
	$(UPPDIR1)Core/BiCont.h \
	$(UPPDIR1)Core/Callback.h \
	$(UPPDIR1)Core/Cbgen.h \
	$(UPPDIR1)Core/CharSet.h \
	$(UPPDIR1)Core/Color.h \
	$(UPPDIR1)Core/Convert.h \
	$(UPPDIR1)Core/Core.h \
	$(UPPDIR1)Core/Defs.h \
	$(UPPDIR1)Core/Diag.h \
	$(UPPDIR1)Core/Format.h \
	$(UPPDIR1)Core/Global.h \
	$(UPPDIR1)Core/Gtypes.h \
	$(UPPDIR1)Core/i18n.h \
	$(UPPDIR1)Core/Index.h \
	$(UPPDIR1)Core/Index.hpp \
	$(UPPDIR1)Core/Lang.h \
	$(UPPDIR1)Core/Lang_s.h \
	$(UPPDIR1)Core/Map.h \
	$(UPPDIR1)Core/Mt.h \
	$(UPPDIR1)Core/Other.h \
	$(UPPDIR1)Core/Parser.h \
	$(UPPDIR1)Core/Path.h \
	$(UPPDIR1)Core/Profile.h \
	$(UPPDIR1)Core/Ptr.h \
	$(UPPDIR1)Core/Stream.h \
	$(UPPDIR1)Core/String.h \
	$(UPPDIR1)Core/String.hpp \
	$(UPPDIR1)Core/t_.h \
	$(UPPDIR1)Core/Thread.h \
	$(UPPDIR1)Core/TimeDate.h \
	$(UPPDIR1)Core/Topic.h \
	$(UPPDIR1)Core/Topt.h \
	$(UPPDIR1)Core/Util.h \
	$(UPPDIR1)Core/Uuid.h \
	$(UPPDIR1)Core/Value.h \
	$(UPPDIR1)Core/Vcont.h \
	$(UPPDIR1)Core/Vcont.hpp \
	$(UPPDIR1)Core/Win32Com.h \
	$(UPPDIR1)Core/XML.h \
	$(UPPDIR1)Core/Xmlize.h \
	$(UPPDIR1)Core/z.h \
	$(UPPDIR1)plugin/bz2/bz2.h \
	$(UPPDIR1)plugin/bz2/lib/bzlib.h
	$(CPPFLAGS) $(CINC) $(Macro_plugin_bz2)  $(UPPDIR1)plugin/bz2/bz2upp.cpp -o $(OutDir_plugin_bz2)bz2upp.o

$(OutDir_plugin_bz2)blocksort.o: $(UPPDIR1)plugin/bz2/lib/blocksort.c \
	$(UPPDIR1)plugin/bz2/lib/bzlib.h \
	$(UPPDIR1)plugin/bz2/lib/bzlib_private.h
	$(CFLAGS) $(CINC) $(Macro_plugin_bz2)  $(UPPDIR1)plugin/bz2/lib/blocksort.c -o $(OutDir_plugin_bz2)blocksort.o

$(OutDir_plugin_bz2)bzlib.o: $(UPPDIR1)plugin/bz2/lib/bzlib.c \
	$(UPPDIR1)plugin/bz2/lib/bzlib.h \
	$(UPPDIR1)plugin/bz2/lib/bzlib_private.h
	$(CFLAGS) $(CINC) $(Macro_plugin_bz2)  $(UPPDIR1)plugin/bz2/lib/bzlib.c -o $(OutDir_plugin_bz2)bzlib.o

$(OutDir_plugin_bz2)compress.o: $(UPPDIR1)plugin/bz2/lib/compress.c \
	$(UPPDIR1)plugin/bz2/lib/bzlib.h \
	$(UPPDIR1)plugin/bz2/lib/bzlib_private.h
	$(CFLAGS) $(CINC) $(Macro_plugin_bz2)  $(UPPDIR1)plugin/bz2/lib/compress.c -o $(OutDir_plugin_bz2)compress.o

$(OutDir_plugin_bz2)crctable.o: $(UPPDIR1)plugin/bz2/lib/crctable.c \
	$(UPPDIR1)plugin/bz2/lib/bzlib.h \
	$(UPPDIR1)plugin/bz2/lib/bzlib_private.h
	$(CFLAGS) $(CINC) $(Macro_plugin_bz2)  $(UPPDIR1)plugin/bz2/lib/crctable.c -o $(OutDir_plugin_bz2)crctable.o

$(OutDir_plugin_bz2)decompress.o: $(UPPDIR1)plugin/bz2/lib/decompress.c \
	$(UPPDIR1)plugin/bz2/lib/bzlib.h \
	$(UPPDIR1)plugin/bz2/lib/bzlib_private.h
	$(CFLAGS) $(CINC) $(Macro_plugin_bz2)  $(UPPDIR1)plugin/bz2/lib/decompress.c -o $(OutDir_plugin_bz2)decompress.o

$(OutDir_plugin_bz2)huffman.o: $(UPPDIR1)plugin/bz2/lib/huffman.c \
	$(UPPDIR1)plugin/bz2/lib/bzlib.h \
	$(UPPDIR1)plugin/bz2/lib/bzlib_private.h
	$(CFLAGS) $(CINC) $(Macro_plugin_bz2)  $(UPPDIR1)plugin/bz2/lib/huffman.c -o $(OutDir_plugin_bz2)huffman.o

$(OutDir_plugin_bz2)randtable.o: $(UPPDIR1)plugin/bz2/lib/randtable.c \
	$(UPPDIR1)plugin/bz2/lib/bzlib.h \
	$(UPPDIR1)plugin/bz2/lib/bzlib_private.h
	$(CFLAGS) $(CINC) $(Macro_plugin_bz2)  $(UPPDIR1)plugin/bz2/lib/randtable.c -o $(OutDir_plugin_bz2)randtable.o

$(OutDir_plugin_bz2)bz2.a: \
	$(OutDir_plugin_bz2)bz2upp.o \
	$(OutDir_plugin_bz2)blocksort.o \
	$(OutDir_plugin_bz2)bzlib.o \
	$(OutDir_plugin_bz2)compress.o \
	$(OutDir_plugin_bz2)crctable.o \
	$(OutDir_plugin_bz2)decompress.o \
	$(OutDir_plugin_bz2)huffman.o \
	$(OutDir_plugin_bz2)randtable.o
	$(AR) $(OutDir_plugin_bz2)bz2.a \
		$(OutDir_plugin_bz2)bz2upp.o \
		$(OutDir_plugin_bz2)blocksort.o \
		$(OutDir_plugin_bz2)bzlib.o \
		$(OutDir_plugin_bz2)compress.o \
		$(OutDir_plugin_bz2)crctable.o \
		$(OutDir_plugin_bz2)decompress.o \
		$(OutDir_plugin_bz2)huffman.o \
		$(OutDir_plugin_bz2)randtable.o

$(OutDir_Core)Mt.o: $(UPPDIR1)Core/Mt.cpp \
	$(UPPDIR1)Core/Algo.h \
	$(UPPDIR1)Core/App.h \
	$(UPPDIR1)Core/BiCont.h \
	$(UPPDIR1)Core/Callback.h \
	$(UPPDIR1)Core/Cbgen.h \
	$(UPPDIR1)Core/CharSet.h \
	$(UPPDIR1)Core/Color.h \
	$(UPPDIR1)Core/Convert.h \
	$(UPPDIR1)Core/Core.h \
	$(UPPDIR1)Core/Defs.h \
	$(UPPDIR1)Core/Diag.h \
	$(UPPDIR1)Core/Format.h \
	$(UPPDIR1)Core/Global.h \
	$(UPPDIR1)Core/Gtypes.h \
	$(UPPDIR1)Core/i18n.h \
	$(UPPDIR1)Core/Index.h \
	$(UPPDIR1)Core/Index.hpp \
	$(UPPDIR1)Core/Lang.h \
	$(UPPDIR1)Core/Lang_s.h \
	$(UPPDIR1)Core/Map.h \
	$(UPPDIR1)Core/Mt.h \
	$(UPPDIR1)Core/Other.h \
	$(UPPDIR1)Core/Parser.h \
	$(UPPDIR1)Core/Path.h \
	$(UPPDIR1)Core/Profile.h \
	$(UPPDIR1)Core/Ptr.h \
	$(UPPDIR1)Core/Stream.h \
	$(UPPDIR1)Core/String.h \
	$(UPPDIR1)Core/String.hpp \
	$(UPPDIR1)Core/t_.h \
	$(UPPDIR1)Core/Thread.h \
	$(UPPDIR1)Core/TimeDate.h \
	$(UPPDIR1)Core/Topic.h \
	$(UPPDIR1)Core/Topt.h \
	$(UPPDIR1)Core/Util.h \
	$(UPPDIR1)Core/Uuid.h \
	$(UPPDIR1)Core/Value.h \
	$(UPPDIR1)Core/Vcont.h \
	$(UPPDIR1)Core/Vcont.hpp \
	$(UPPDIR1)Core/Win32Com.h \
	$(UPPDIR1)Core/XML.h \
	$(UPPDIR1)Core/Xmlize.h \
	$(UPPDIR1)Core/z.h
	$(CPPFLAGS) $(CINC) $(Macro_Core)  $(UPPDIR1)Core/Mt.cpp -o $(OutDir_Core)Mt.o

$(OutDir_Core)Thread.o: $(UPPDIR1)Core/Thread.cpp \
	$(UPPDIR1)Core/Algo.h \
	$(UPPDIR1)Core/App.h \
	$(UPPDIR1)Core/BiCont.h \
	$(UPPDIR1)Core/Callback.h \
	$(UPPDIR1)Core/Cbgen.h \
	$(UPPDIR1)Core/CharSet.h \
	$(UPPDIR1)Core/Color.h \
	$(UPPDIR1)Core/Convert.h \
	$(UPPDIR1)Core/Core.h \
	$(UPPDIR1)Core/Defs.h \
	$(UPPDIR1)Core/Diag.h \
	$(UPPDIR1)Core/Format.h \
	$(UPPDIR1)Core/Global.h \
	$(UPPDIR1)Core/Gtypes.h \
	$(UPPDIR1)Core/i18n.h \
	$(UPPDIR1)Core/Index.h \
	$(UPPDIR1)Core/Index.hpp \
	$(UPPDIR1)Core/Lang.h \
	$(UPPDIR1)Core/Lang_s.h \
	$(UPPDIR1)Core/Map.h \
	$(UPPDIR1)Core/Mt.h \
	$(UPPDIR1)Core/Other.h \
	$(UPPDIR1)Core/Parser.h \
	$(UPPDIR1)Core/Path.h \
	$(UPPDIR1)Core/Profile.h \
	$(UPPDIR1)Core/Ptr.h \
	$(UPPDIR1)Core/Stream.h \
	$(UPPDIR1)Core/String.h \
	$(UPPDIR1)Core/String.hpp \
	$(UPPDIR1)Core/t_.h \
	$(UPPDIR1)Core/Thread.h \
	$(UPPDIR1)Core/TimeDate.h \
	$(UPPDIR1)Core/Topic.h \
	$(UPPDIR1)Core/Topt.h \
	$(UPPDIR1)Core/Util.h \
	$(UPPDIR1)Core/Uuid.h \
	$(UPPDIR1)Core/Value.h \
	$(UPPDIR1)Core/Vcont.h \
	$(UPPDIR1)Core/Vcont.hpp \
	$(UPPDIR1)Core/Win32Com.h \
	$(UPPDIR1)Core/XML.h \
	$(UPPDIR1)Core/Xmlize.h \
	$(UPPDIR1)Core/z.h
	$(CPPFLAGS) $(CINC) $(Macro_Core)  $(UPPDIR1)Core/Thread.cpp -o $(OutDir_Core)Thread.o

$(OutDir_Core)OL_Set.o: $(UPPDIR1)Core/OL_Set.cpp
	$(CPPFLAGS) $(CINC) $(Macro_Core)  $(UPPDIR1)Core/OL_Set.cpp -o $(OutDir_Core)OL_Set.o

$(OutDir_Core)heap.o: $(UPPDIR1)Core/heap.cpp \
	$(UPPDIR1)Core/Algo.h \
	$(UPPDIR1)Core/App.h \
	$(UPPDIR1)Core/BiCont.h \
	$(UPPDIR1)Core/Callback.h \
	$(UPPDIR1)Core/Cbgen.h \
	$(UPPDIR1)Core/CharSet.h \
	$(UPPDIR1)Core/Color.h \
	$(UPPDIR1)Core/Convert.h \
	$(UPPDIR1)Core/Core.h \
	$(UPPDIR1)Core/Defs.h \
	$(UPPDIR1)Core/Diag.h \
	$(UPPDIR1)Core/Format.h \
	$(UPPDIR1)Core/Global.h \
	$(UPPDIR1)Core/Gtypes.h \
	$(UPPDIR1)Core/i18n.h \
	$(UPPDIR1)Core/Index.h \
	$(UPPDIR1)Core/Index.hpp \
	$(UPPDIR1)Core/Lang.h \
	$(UPPDIR1)Core/Lang_s.h \
	$(UPPDIR1)Core/Map.h \
	$(UPPDIR1)Core/Mt.h \
	$(UPPDIR1)Core/Other.h \
	$(UPPDIR1)Core/Parser.h \
	$(UPPDIR1)Core/Path.h \
	$(UPPDIR1)Core/Profile.h \
	$(UPPDIR1)Core/Ptr.h \
	$(UPPDIR1)Core/Stream.h \
	$(UPPDIR1)Core/String.h \
	$(UPPDIR1)Core/String.hpp \
	$(UPPDIR1)Core/t_.h \
	$(UPPDIR1)Core/Thread.h \
	$(UPPDIR1)Core/TimeDate.h \
	$(UPPDIR1)Core/Topic.h \
	$(UPPDIR1)Core/Topt.h \
	$(UPPDIR1)Core/Util.h \
	$(UPPDIR1)Core/Uuid.h \
	$(UPPDIR1)Core/Value.h \
	$(UPPDIR1)Core/Vcont.h \
	$(UPPDIR1)Core/Vcont.hpp \
	$(UPPDIR1)Core/Win32Com.h \
	$(UPPDIR1)Core/XML.h \
	$(UPPDIR1)Core/Xmlize.h \
	$(UPPDIR1)Core/z.h
	$(CPPFLAGS) $(CINC) $(Macro_Core)  $(UPPDIR1)Core/heap.cpp -o $(OutDir_Core)heap.o

$(OutDir_Core)heapdbg.o: $(UPPDIR1)Core/heapdbg.cpp \
	$(UPPDIR1)Core/Algo.h \
	$(UPPDIR1)Core/App.h \
	$(UPPDIR1)Core/BiCont.h \
	$(UPPDIR1)Core/Callback.h \
	$(UPPDIR1)Core/Cbgen.h \
	$(UPPDIR1)Core/CharSet.h \
	$(UPPDIR1)Core/Color.h \
	$(UPPDIR1)Core/Convert.h \
	$(UPPDIR1)Core/Core.h \
	$(UPPDIR1)Core/Defs.h \
	$(UPPDIR1)Core/Diag.h \
	$(UPPDIR1)Core/Format.h \
	$(UPPDIR1)Core/Global.h \
	$(UPPDIR1)Core/Gtypes.h \
	$(UPPDIR1)Core/i18n.h \
	$(UPPDIR1)Core/Index.h \
	$(UPPDIR1)Core/Index.hpp \
	$(UPPDIR1)Core/Lang.h \
	$(UPPDIR1)Core/Lang_s.h \
	$(UPPDIR1)Core/Map.h \
	$(UPPDIR1)Core/Mt.h \
	$(UPPDIR1)Core/Other.h \
	$(UPPDIR1)Core/Parser.h \
	$(UPPDIR1)Core/Path.h \
	$(UPPDIR1)Core/Profile.h \
	$(UPPDIR1)Core/Ptr.h \
	$(UPPDIR1)Core/Stream.h \
	$(UPPDIR1)Core/String.h \
	$(UPPDIR1)Core/String.hpp \
	$(UPPDIR1)Core/t_.h \
	$(UPPDIR1)Core/Thread.h \
	$(UPPDIR1)Core/TimeDate.h \
	$(UPPDIR1)Core/Topic.h \
	$(UPPDIR1)Core/Topt.h \
	$(UPPDIR1)Core/Util.h \
	$(UPPDIR1)Core/Uuid.h \
	$(UPPDIR1)Core/Value.h \
	$(UPPDIR1)Core/Vcont.h \
	$(UPPDIR1)Core/Vcont.hpp \
	$(UPPDIR1)Core/Win32Com.h \
	$(UPPDIR1)Core/XML.h \
	$(UPPDIR1)Core/Xmlize.h \
	$(UPPDIR1)Core/z.h
	$(CPPFLAGS) $(CINC) $(Macro_Core)  $(UPPDIR1)Core/heapdbg.cpp -o $(OutDir_Core)heapdbg.o

$(OutDir_Core)String.o: $(UPPDIR1)Core/String.cpp \
	$(UPPDIR1)Core/Algo.h \
	$(UPPDIR1)Core/App.h \
	$(UPPDIR1)Core/BiCont.h \
	$(UPPDIR1)Core/Callback.h \
	$(UPPDIR1)Core/Cbgen.h \
	$(UPPDIR1)Core/CharSet.h \
	$(UPPDIR1)Core/Color.h \
	$(UPPDIR1)Core/Convert.h \
	$(UPPDIR1)Core/Core.h \
	$(UPPDIR1)Core/Defs.h \
	$(UPPDIR1)Core/Diag.h \
	$(UPPDIR1)Core/Format.h \
	$(UPPDIR1)Core/Global.h \
	$(UPPDIR1)Core/Gtypes.h \
	$(UPPDIR1)Core/i18n.h \
	$(UPPDIR1)Core/Index.h \
	$(UPPDIR1)Core/Index.hpp \
	$(UPPDIR1)Core/Lang.h \
	$(UPPDIR1)Core/Lang_s.h \
	$(UPPDIR1)Core/Map.h \
	$(UPPDIR1)Core/Mt.h \
	$(UPPDIR1)Core/Other.h \
	$(UPPDIR1)Core/Parser.h \
	$(UPPDIR1)Core/Path.h \
	$(UPPDIR1)Core/Profile.h \
	$(UPPDIR1)Core/Ptr.h \
	$(UPPDIR1)Core/Stream.h \
	$(UPPDIR1)Core/String.h \
	$(UPPDIR1)Core/String.hpp \
	$(UPPDIR1)Core/t_.h \
	$(UPPDIR1)Core/Thread.h \
	$(UPPDIR1)Core/TimeDate.h \
	$(UPPDIR1)Core/Topic.h \
	$(UPPDIR1)Core/Topt.h \
	$(UPPDIR1)Core/Util.h \
	$(UPPDIR1)Core/Uuid.h \
	$(UPPDIR1)Core/Value.h \
	$(UPPDIR1)Core/Vcont.h \
	$(UPPDIR1)Core/Vcont.hpp \
	$(UPPDIR1)Core/Win32Com.h \
	$(UPPDIR1)Core/XML.h \
	$(UPPDIR1)Core/Xmlize.h \
	$(UPPDIR1)Core/z.h
	$(CPPFLAGS) $(CINC) $(Macro_Core)  $(UPPDIR1)Core/String.cpp -o $(OutDir_Core)String.o

$(OutDir_Core)StrUtil.o: $(UPPDIR1)Core/StrUtil.cpp \
	$(UPPDIR1)Core/Algo.h \
	$(UPPDIR1)Core/App.h \
	$(UPPDIR1)Core/BiCont.h \
	$(UPPDIR1)Core/Callback.h \
	$(UPPDIR1)Core/Cbgen.h \
	$(UPPDIR1)Core/CharSet.h \
	$(UPPDIR1)Core/Color.h \
	$(UPPDIR1)Core/Convert.h \
	$(UPPDIR1)Core/Core.h \
	$(UPPDIR1)Core/Defs.h \
	$(UPPDIR1)Core/Diag.h \
	$(UPPDIR1)Core/Format.h \
	$(UPPDIR1)Core/Global.h \
	$(UPPDIR1)Core/Gtypes.h \
	$(UPPDIR1)Core/i18n.h \
	$(UPPDIR1)Core/Index.h \
	$(UPPDIR1)Core/Index.hpp \
	$(UPPDIR1)Core/Lang.h \
	$(UPPDIR1)Core/Lang_s.h \
	$(UPPDIR1)Core/Map.h \
	$(UPPDIR1)Core/Mt.h \
	$(UPPDIR1)Core/Other.h \
	$(UPPDIR1)Core/Parser.h \
	$(UPPDIR1)Core/Path.h \
	$(UPPDIR1)Core/Profile.h \
	$(UPPDIR1)Core/Ptr.h \
	$(UPPDIR1)Core/Stream.h \
	$(UPPDIR1)Core/String.h \
	$(UPPDIR1)Core/String.hpp \
	$(UPPDIR1)Core/t_.h \
	$(UPPDIR1)Core/Thread.h \
	$(UPPDIR1)Core/TimeDate.h \
	$(UPPDIR1)Core/Topic.h \
	$(UPPDIR1)Core/Topt.h \
	$(UPPDIR1)Core/Util.h \
	$(UPPDIR1)Core/Uuid.h \
	$(UPPDIR1)Core/Value.h \
	$(UPPDIR1)Core/Vcont.h \
	$(UPPDIR1)Core/Vcont.hpp \
	$(UPPDIR1)Core/Win32Com.h \
	$(UPPDIR1)Core/XML.h \
	$(UPPDIR1)Core/Xmlize.h \
	$(UPPDIR1)Core/z.h
	$(CPPFLAGS) $(CINC) $(Macro_Core)  $(UPPDIR1)Core/StrUtil.cpp -o $(OutDir_Core)StrUtil.o

$(OutDir_Core)CharSet.o: $(UPPDIR1)Core/CharSet.cpp \
	$(UPPDIR1)Core/Algo.h \
	$(UPPDIR1)Core/App.h \
	$(UPPDIR1)Core/BiCont.h \
	$(UPPDIR1)Core/Callback.h \
	$(UPPDIR1)Core/Cbgen.h \
	$(UPPDIR1)Core/CharSet.h \
	$(UPPDIR1)Core/Color.h \
	$(UPPDIR1)Core/Convert.h \
	$(UPPDIR1)Core/Core.h \
	$(UPPDIR1)Core/Defs.h \
	$(UPPDIR1)Core/Diag.h \
	$(UPPDIR1)Core/Format.h \
	$(UPPDIR1)Core/Global.h \
	$(UPPDIR1)Core/Gtypes.h \
	$(UPPDIR1)Core/i18n.h \
	$(UPPDIR1)Core/Index.h \
	$(UPPDIR1)Core/Index.hpp \
	$(UPPDIR1)Core/Lang.h \
	$(UPPDIR1)Core/Lang_s.h \
	$(UPPDIR1)Core/Map.h \
	$(UPPDIR1)Core/Mt.h \
	$(UPPDIR1)Core/Other.h \
	$(UPPDIR1)Core/Parser.h \
	$(UPPDIR1)Core/Path.h \
	$(UPPDIR1)Core/Profile.h \
	$(UPPDIR1)Core/Ptr.h \
	$(UPPDIR1)Core/Stream.h \
	$(UPPDIR1)Core/String.h \
	$(UPPDIR1)Core/String.hpp \
	$(UPPDIR1)Core/t_.h \
	$(UPPDIR1)Core/Thread.h \
	$(UPPDIR1)Core/TimeDate.h \
	$(UPPDIR1)Core/Topic.h \
	$(UPPDIR1)Core/Topt.h \
	$(UPPDIR1)Core/Util.h \
	$(UPPDIR1)Core/Uuid.h \
	$(UPPDIR1)Core/Value.h \
	$(UPPDIR1)Core/Vcont.h \
	$(UPPDIR1)Core/Vcont.hpp \
	$(UPPDIR1)Core/Win32Com.h \
	$(UPPDIR1)Core/XML.h \
	$(UPPDIR1)Core/Xmlize.h \
	$(UPPDIR1)Core/z.h
	$(CPPFLAGS) $(CINC) $(Macro_Core)  $(UPPDIR1)Core/CharSet.cpp -o $(OutDir_Core)CharSet.o

$(OutDir_Core)Path.o: $(UPPDIR1)Core/Path.cpp \
	$(UPPDIR1)Core/Algo.h \
	$(UPPDIR1)Core/App.h \
	$(UPPDIR1)Core/BiCont.h \
	$(UPPDIR1)Core/Callback.h \
	$(UPPDIR1)Core/Cbgen.h \
	$(UPPDIR1)Core/CharSet.h \
	$(UPPDIR1)Core/Color.h \
	$(UPPDIR1)Core/Convert.h \
	$(UPPDIR1)Core/Core.h \
	$(UPPDIR1)Core/Defs.h \
	$(UPPDIR1)Core/Diag.h \
	$(UPPDIR1)Core/Format.h \
	$(UPPDIR1)Core/Global.h \
	$(UPPDIR1)Core/Gtypes.h \
	$(UPPDIR1)Core/i18n.h \
	$(UPPDIR1)Core/Index.h \
	$(UPPDIR1)Core/Index.hpp \
	$(UPPDIR1)Core/Lang.h \
	$(UPPDIR1)Core/Lang_s.h \
	$(UPPDIR1)Core/Map.h \
	$(UPPDIR1)Core/Mt.h \
	$(UPPDIR1)Core/Other.h \
	$(UPPDIR1)Core/Parser.h \
	$(UPPDIR1)Core/Path.h \
	$(UPPDIR1)Core/Profile.h \
	$(UPPDIR1)Core/Ptr.h \
	$(UPPDIR1)Core/Stream.h \
	$(UPPDIR1)Core/String.h \
	$(UPPDIR1)Core/String.hpp \
	$(UPPDIR1)Core/t_.h \
	$(UPPDIR1)Core/Thread.h \
	$(UPPDIR1)Core/TimeDate.h \
	$(UPPDIR1)Core/Topic.h \
	$(UPPDIR1)Core/Topt.h \
	$(UPPDIR1)Core/Util.h \
	$(UPPDIR1)Core/Uuid.h \
	$(UPPDIR1)Core/Value.h \
	$(UPPDIR1)Core/Vcont.h \
	$(UPPDIR1)Core/Vcont.hpp \
	$(UPPDIR1)Core/Win32Com.h \
	$(UPPDIR1)Core/XML.h \
	$(UPPDIR1)Core/Xmlize.h \
	$(UPPDIR1)Core/z.h
	$(CPPFLAGS) $(CINC) $(Macro_Core)  $(UPPDIR1)Core/Path.cpp -o $(OutDir_Core)Path.o

$(OutDir_Core)App.o: $(UPPDIR1)Core/App.cpp \
	$(UPPDIR1)Core/Algo.h \
	$(UPPDIR1)Core/App.h \
	$(UPPDIR1)Core/BiCont.h \
	$(UPPDIR1)Core/Callback.h \
	$(UPPDIR1)Core/Cbgen.h \
	$(UPPDIR1)Core/CharSet.h \
	$(UPPDIR1)Core/Color.h \
	$(UPPDIR1)Core/Convert.h \
	$(UPPDIR1)Core/Core.h \
	$(UPPDIR1)Core/Defs.h \
	$(UPPDIR1)Core/Diag.h \
	$(UPPDIR1)Core/Format.h \
	$(UPPDIR1)Core/Global.h \
	$(UPPDIR1)Core/Gtypes.h \
	$(UPPDIR1)Core/i18n.h \
	$(UPPDIR1)Core/Index.h \
	$(UPPDIR1)Core/Index.hpp \
	$(UPPDIR1)Core/Lang.h \
	$(UPPDIR1)Core/Lang_s.h \
	$(UPPDIR1)Core/Map.h \
	$(UPPDIR1)Core/Mt.h \
	$(UPPDIR1)Core/Other.h \
	$(UPPDIR1)Core/Parser.h \
	$(UPPDIR1)Core/Path.h \
	$(UPPDIR1)Core/Profile.h \
	$(UPPDIR1)Core/Ptr.h \
	$(UPPDIR1)Core/Stream.h \
	$(UPPDIR1)Core/String.h \
	$(UPPDIR1)Core/String.hpp \
	$(UPPDIR1)Core/t_.h \
	$(UPPDIR1)Core/Thread.h \
	$(UPPDIR1)Core/TimeDate.h \
	$(UPPDIR1)Core/Topic.h \
	$(UPPDIR1)Core/Topt.h \
	$(UPPDIR1)Core/Util.h \
	$(UPPDIR1)Core/Uuid.h \
	$(UPPDIR1)Core/Value.h \
	$(UPPDIR1)Core/Vcont.h \
	$(UPPDIR1)Core/Vcont.hpp \
	$(UPPDIR1)Core/Win32Com.h \
	$(UPPDIR1)Core/XML.h \
	$(UPPDIR1)Core/Xmlize.h \
	$(UPPDIR1)Core/z.h
	$(CPPFLAGS) $(CINC) $(Macro_Core)  $(UPPDIR1)Core/App.cpp -o $(OutDir_Core)App.o

$(OutDir_Core)Stream.o: $(UPPDIR1)Core/Stream.cpp \
	$(UPPDIR1)Core/Algo.h \
	$(UPPDIR1)Core/App.h \
	$(UPPDIR1)Core/BiCont.h \
	$(UPPDIR1)Core/Callback.h \
	$(UPPDIR1)Core/Cbgen.h \
	$(UPPDIR1)Core/CharSet.h \
	$(UPPDIR1)Core/Color.h \
	$(UPPDIR1)Core/Convert.h \
	$(UPPDIR1)Core/Core.h \
	$(UPPDIR1)Core/Defs.h \
	$(UPPDIR1)Core/Diag.h \
	$(UPPDIR1)Core/Format.h \
	$(UPPDIR1)Core/Global.h \
	$(UPPDIR1)Core/Gtypes.h \
	$(UPPDIR1)Core/i18n.h \
	$(UPPDIR1)Core/Index.h \
	$(UPPDIR1)Core/Index.hpp \
	$(UPPDIR1)Core/Lang.h \
	$(UPPDIR1)Core/Lang_s.h \
	$(UPPDIR1)Core/Map.h \
	$(UPPDIR1)Core/Mt.h \
	$(UPPDIR1)Core/Other.h \
	$(UPPDIR1)Core/Parser.h \
	$(UPPDIR1)Core/Path.h \
	$(UPPDIR1)Core/Profile.h \
	$(UPPDIR1)Core/Ptr.h \
	$(UPPDIR1)Core/Stream.h \
	$(UPPDIR1)Core/String.h \
	$(UPPDIR1)Core/String.hpp \
	$(UPPDIR1)Core/t_.h \
	$(UPPDIR1)Core/Thread.h \
	$(UPPDIR1)Core/TimeDate.h \
	$(UPPDIR1)Core/Topic.h \
	$(UPPDIR1)Core/Topt.h \
	$(UPPDIR1)Core/Util.h \
	$(UPPDIR1)Core/Uuid.h \
	$(UPPDIR1)Core/Value.h \
	$(UPPDIR1)Core/Vcont.h \
	$(UPPDIR1)Core/Vcont.hpp \
	$(UPPDIR1)Core/Win32Com.h \
	$(UPPDIR1)Core/XML.h \
	$(UPPDIR1)Core/Xmlize.h \
	$(UPPDIR1)Core/z.h
	$(CPPFLAGS) $(CINC) $(Macro_Core)  $(UPPDIR1)Core/Stream.cpp -o $(OutDir_Core)Stream.o

$(OutDir_Core)BlockStream.o: $(UPPDIR1)Core/BlockStream.cpp \
	$(UPPDIR1)Core/Algo.h \
	$(UPPDIR1)Core/App.h \
	$(UPPDIR1)Core/BiCont.h \
	$(UPPDIR1)Core/Callback.h \
	$(UPPDIR1)Core/Cbgen.h \
	$(UPPDIR1)Core/CharSet.h \
	$(UPPDIR1)Core/Color.h \
	$(UPPDIR1)Core/Convert.h \
	$(UPPDIR1)Core/Core.h \
	$(UPPDIR1)Core/Defs.h \
	$(UPPDIR1)Core/Diag.h \
	$(UPPDIR1)Core/Format.h \
	$(UPPDIR1)Core/Global.h \
	$(UPPDIR1)Core/Gtypes.h \
	$(UPPDIR1)Core/i18n.h \
	$(UPPDIR1)Core/Index.h \
	$(UPPDIR1)Core/Index.hpp \
	$(UPPDIR1)Core/Lang.h \
	$(UPPDIR1)Core/Lang_s.h \
	$(UPPDIR1)Core/Map.h \
	$(UPPDIR1)Core/Mt.h \
	$(UPPDIR1)Core/Other.h \
	$(UPPDIR1)Core/Parser.h \
	$(UPPDIR1)Core/Path.h \
	$(UPPDIR1)Core/Profile.h \
	$(UPPDIR1)Core/Ptr.h \
	$(UPPDIR1)Core/Stream.h \
	$(UPPDIR1)Core/String.h \
	$(UPPDIR1)Core/String.hpp \
	$(UPPDIR1)Core/t_.h \
	$(UPPDIR1)Core/Thread.h \
	$(UPPDIR1)Core/TimeDate.h \
	$(UPPDIR1)Core/Topic.h \
	$(UPPDIR1)Core/Topt.h \
	$(UPPDIR1)Core/Util.h \
	$(UPPDIR1)Core/Uuid.h \
	$(UPPDIR1)Core/Value.h \
	$(UPPDIR1)Core/Vcont.h \
	$(UPPDIR1)Core/Vcont.hpp \
	$(UPPDIR1)Core/Win32Com.h \
	$(UPPDIR1)Core/XML.h \
	$(UPPDIR1)Core/Xmlize.h \
	$(UPPDIR1)Core/z.h
	$(CPPFLAGS) $(CINC) $(Macro_Core)  $(UPPDIR1)Core/BlockStream.cpp -o $(OutDir_Core)BlockStream.o

$(OutDir_Core)Log.o: $(UPPDIR1)Core/Log.cpp \
	$(UPPDIR1)Core/Algo.h \
	$(UPPDIR1)Core/App.h \
	$(UPPDIR1)Core/BiCont.h \
	$(UPPDIR1)Core/Callback.h \
	$(UPPDIR1)Core/Cbgen.h \
	$(UPPDIR1)Core/CharSet.h \
	$(UPPDIR1)Core/Color.h \
	$(UPPDIR1)Core/Convert.h \
	$(UPPDIR1)Core/Core.h \
	$(UPPDIR1)Core/Defs.h \
	$(UPPDIR1)Core/Diag.h \
	$(UPPDIR1)Core/Format.h \
	$(UPPDIR1)Core/Global.h \
	$(UPPDIR1)Core/Gtypes.h \
	$(UPPDIR1)Core/i18n.h \
	$(UPPDIR1)Core/Index.h \
	$(UPPDIR1)Core/Index.hpp \
	$(UPPDIR1)Core/Lang.h \
	$(UPPDIR1)Core/Lang_s.h \
	$(UPPDIR1)Core/Map.h \
	$(UPPDIR1)Core/Mt.h \
	$(UPPDIR1)Core/Other.h \
	$(UPPDIR1)Core/Parser.h \
	$(UPPDIR1)Core/Path.h \
	$(UPPDIR1)Core/Profile.h \
	$(UPPDIR1)Core/Ptr.h \
	$(UPPDIR1)Core/Stream.h \
	$(UPPDIR1)Core/String.h \
	$(UPPDIR1)Core/String.hpp \
	$(UPPDIR1)Core/t_.h \
	$(UPPDIR1)Core/Thread.h \
	$(UPPDIR1)Core/TimeDate.h \
	$(UPPDIR1)Core/Topic.h \
	$(UPPDIR1)Core/Topt.h \
	$(UPPDIR1)Core/Util.h \
	$(UPPDIR1)Core/Uuid.h \
	$(UPPDIR1)Core/Value.h \
	$(UPPDIR1)Core/Vcont.h \
	$(UPPDIR1)Core/Vcont.hpp \
	$(UPPDIR1)Core/Win32Com.h \
	$(UPPDIR1)Core/XML.h \
	$(UPPDIR1)Core/Xmlize.h \
	$(UPPDIR1)Core/z.h
	$(CPPFLAGS) $(CINC) $(Macro_Core)  $(UPPDIR1)Core/Log.cpp -o $(OutDir_Core)Log.o

$(OutDir_Core)Debug.o: $(UPPDIR1)Core/Debug.cpp \
	$(UPPDIR1)Core/Algo.h \
	$(UPPDIR1)Core/App.h \
	$(UPPDIR1)Core/BiCont.h \
	$(UPPDIR1)Core/Callback.h \
	$(UPPDIR1)Core/Cbgen.h \
	$(UPPDIR1)Core/CharSet.h \
	$(UPPDIR1)Core/Color.h \
	$(UPPDIR1)Core/Convert.h \
	$(UPPDIR1)Core/Core.h \
	$(UPPDIR1)Core/Defs.h \
	$(UPPDIR1)Core/Diag.h \
	$(UPPDIR1)Core/Format.h \
	$(UPPDIR1)Core/Global.h \
	$(UPPDIR1)Core/Gtypes.h \
	$(UPPDIR1)Core/i18n.h \
	$(UPPDIR1)Core/Index.h \
	$(UPPDIR1)Core/Index.hpp \
	$(UPPDIR1)Core/Lang.h \
	$(UPPDIR1)Core/Lang_s.h \
	$(UPPDIR1)Core/Map.h \
	$(UPPDIR1)Core/Mt.h \
	$(UPPDIR1)Core/Other.h \
	$(UPPDIR1)Core/Parser.h \
	$(UPPDIR1)Core/Path.h \
	$(UPPDIR1)Core/Profile.h \
	$(UPPDIR1)Core/Ptr.h \
	$(UPPDIR1)Core/Stream.h \
	$(UPPDIR1)Core/String.h \
	$(UPPDIR1)Core/String.hpp \
	$(UPPDIR1)Core/t_.h \
	$(UPPDIR1)Core/Thread.h \
	$(UPPDIR1)Core/TimeDate.h \
	$(UPPDIR1)Core/Topic.h \
	$(UPPDIR1)Core/Topt.h \
	$(UPPDIR1)Core/Util.h \
	$(UPPDIR1)Core/Uuid.h \
	$(UPPDIR1)Core/Value.h \
	$(UPPDIR1)Core/Vcont.h \
	$(UPPDIR1)Core/Vcont.hpp \
	$(UPPDIR1)Core/Win32Com.h \
	$(UPPDIR1)Core/XML.h \
	$(UPPDIR1)Core/Xmlize.h \
	$(UPPDIR1)Core/z.h
	$(CPPFLAGS) $(CINC) $(Macro_Core)  $(UPPDIR1)Core/Debug.cpp -o $(OutDir_Core)Debug.o

$(OutDir_Core)Util.o: $(UPPDIR1)Core/Util.cpp \
	$(UPPDIR1)Core/Algo.h \
	$(UPPDIR1)Core/App.h \
	$(UPPDIR1)Core/BiCont.h \
	$(UPPDIR1)Core/Callback.h \
	$(UPPDIR1)Core/Cbgen.h \
	$(UPPDIR1)Core/CharSet.h \
	$(UPPDIR1)Core/Color.h \
	$(UPPDIR1)Core/Convert.h \
	$(UPPDIR1)Core/Core.h \
	$(UPPDIR1)Core/Defs.h \
	$(UPPDIR1)Core/Diag.h \
	$(UPPDIR1)Core/Format.h \
	$(UPPDIR1)Core/Global.h \
	$(UPPDIR1)Core/Gtypes.h \
	$(UPPDIR1)Core/i18n.h \
	$(UPPDIR1)Core/Index.h \
	$(UPPDIR1)Core/Index.hpp \
	$(UPPDIR1)Core/Lang.h \
	$(UPPDIR1)Core/Lang_s.h \
	$(UPPDIR1)Core/Map.h \
	$(UPPDIR1)Core/Mt.h \
	$(UPPDIR1)Core/Other.h \
	$(UPPDIR1)Core/Parser.h \
	$(UPPDIR1)Core/Path.h \
	$(UPPDIR1)Core/Profile.h \
	$(UPPDIR1)Core/Ptr.h \
	$(UPPDIR1)Core/Stream.h \
	$(UPPDIR1)Core/String.h \
	$(UPPDIR1)Core/String.hpp \
	$(UPPDIR1)Core/t_.h \
	$(UPPDIR1)Core/Thread.h \
	$(UPPDIR1)Core/TimeDate.h \
	$(UPPDIR1)Core/Topic.h \
	$(UPPDIR1)Core/Topt.h \
	$(UPPDIR1)Core/Util.h \
	$(UPPDIR1)Core/Uuid.h \
	$(UPPDIR1)Core/Value.h \
	$(UPPDIR1)Core/Vcont.h \
	$(UPPDIR1)Core/Vcont.hpp \
	$(UPPDIR1)Core/Win32Com.h \
	$(UPPDIR1)Core/XML.h \
	$(UPPDIR1)Core/Xmlize.h \
	$(UPPDIR1)Core/z.h
	$(CPPFLAGS) $(CINC) $(Macro_Core)  $(UPPDIR1)Core/Util.cpp -o $(OutDir_Core)Util.o

$(OutDir_Core)mathutil.o: $(UPPDIR1)Core/mathutil.cpp \
	$(UPPDIR1)Core/Algo.h \
	$(UPPDIR1)Core/App.h \
	$(UPPDIR1)Core/BiCont.h \
	$(UPPDIR1)Core/Callback.h \
	$(UPPDIR1)Core/Cbgen.h \
	$(UPPDIR1)Core/CharSet.h \
	$(UPPDIR1)Core/Color.h \
	$(UPPDIR1)Core/Convert.h \
	$(UPPDIR1)Core/Core.h \
	$(UPPDIR1)Core/Defs.h \
	$(UPPDIR1)Core/Diag.h \
	$(UPPDIR1)Core/Format.h \
	$(UPPDIR1)Core/Global.h \
	$(UPPDIR1)Core/Gtypes.h \
	$(UPPDIR1)Core/i18n.h \
	$(UPPDIR1)Core/Index.h \
	$(UPPDIR1)Core/Index.hpp \
	$(UPPDIR1)Core/Lang.h \
	$(UPPDIR1)Core/Lang_s.h \
	$(UPPDIR1)Core/Map.h \
	$(UPPDIR1)Core/Mt.h \
	$(UPPDIR1)Core/Other.h \
	$(UPPDIR1)Core/Parser.h \
	$(UPPDIR1)Core/Path.h \
	$(UPPDIR1)Core/Profile.h \
	$(UPPDIR1)Core/Ptr.h \
	$(UPPDIR1)Core/Stream.h \
	$(UPPDIR1)Core/String.h \
	$(UPPDIR1)Core/String.hpp \
	$(UPPDIR1)Core/t_.h \
	$(UPPDIR1)Core/Thread.h \
	$(UPPDIR1)Core/TimeDate.h \
	$(UPPDIR1)Core/Topic.h \
	$(UPPDIR1)Core/Topt.h \
	$(UPPDIR1)Core/Util.h \
	$(UPPDIR1)Core/Uuid.h \
	$(UPPDIR1)Core/Value.h \
	$(UPPDIR1)Core/Vcont.h \
	$(UPPDIR1)Core/Vcont.hpp \
	$(UPPDIR1)Core/Win32Com.h \
	$(UPPDIR1)Core/XML.h \
	$(UPPDIR1)Core/Xmlize.h \
	$(UPPDIR1)Core/z.h
	$(CPPFLAGS) $(CINC) $(Macro_Core)  $(UPPDIR1)Core/mathutil.cpp -o $(OutDir_Core)mathutil.o

$(OutDir_Core)Vcont.o: $(UPPDIR1)Core/Vcont.cpp \
	$(UPPDIR1)Core/Algo.h \
	$(UPPDIR1)Core/App.h \
	$(UPPDIR1)Core/BiCont.h \
	$(UPPDIR1)Core/Callback.h \
	$(UPPDIR1)Core/Cbgen.h \
	$(UPPDIR1)Core/CharSet.h \
	$(UPPDIR1)Core/Color.h \
	$(UPPDIR1)Core/Convert.h \
	$(UPPDIR1)Core/Core.h \
	$(UPPDIR1)Core/Defs.h \
	$(UPPDIR1)Core/Diag.h \
	$(UPPDIR1)Core/Format.h \
	$(UPPDIR1)Core/Global.h \
	$(UPPDIR1)Core/Gtypes.h \
	$(UPPDIR1)Core/i18n.h \
	$(UPPDIR1)Core/Index.h \
	$(UPPDIR1)Core/Index.hpp \
	$(UPPDIR1)Core/Lang.h \
	$(UPPDIR1)Core/Lang_s.h \
	$(UPPDIR1)Core/Map.h \
	$(UPPDIR1)Core/Mt.h \
	$(UPPDIR1)Core/Other.h \
	$(UPPDIR1)Core/Parser.h \
	$(UPPDIR1)Core/Path.h \
	$(UPPDIR1)Core/Profile.h \
	$(UPPDIR1)Core/Ptr.h \
	$(UPPDIR1)Core/Stream.h \
	$(UPPDIR1)Core/String.h \
	$(UPPDIR1)Core/String.hpp \
	$(UPPDIR1)Core/t_.h \
	$(UPPDIR1)Core/Thread.h \
	$(UPPDIR1)Core/TimeDate.h \
	$(UPPDIR1)Core/Topic.h \
	$(UPPDIR1)Core/Topt.h \
	$(UPPDIR1)Core/Util.h \
	$(UPPDIR1)Core/Uuid.h \
	$(UPPDIR1)Core/Value.h \
	$(UPPDIR1)Core/Vcont.h \
	$(UPPDIR1)Core/Vcont.hpp \
	$(UPPDIR1)Core/Win32Com.h \
	$(UPPDIR1)Core/XML.h \
	$(UPPDIR1)Core/Xmlize.h \
	$(UPPDIR1)Core/z.h
	$(CPPFLAGS) $(CINC) $(Macro_Core)  $(UPPDIR1)Core/Vcont.cpp -o $(OutDir_Core)Vcont.o

$(OutDir_Core)Hash.o: $(UPPDIR1)Core/Hash.cpp \
	$(UPPDIR1)Core/Algo.h \
	$(UPPDIR1)Core/App.h \
	$(UPPDIR1)Core/BiCont.h \
	$(UPPDIR1)Core/Callback.h \
	$(UPPDIR1)Core/Cbgen.h \
	$(UPPDIR1)Core/CharSet.h \
	$(UPPDIR1)Core/Color.h \
	$(UPPDIR1)Core/Convert.h \
	$(UPPDIR1)Core/Core.h \
	$(UPPDIR1)Core/Defs.h \
	$(UPPDIR1)Core/Diag.h \
	$(UPPDIR1)Core/Format.h \
	$(UPPDIR1)Core/Global.h \
	$(UPPDIR1)Core/Gtypes.h \
	$(UPPDIR1)Core/i18n.h \
	$(UPPDIR1)Core/Index.h \
	$(UPPDIR1)Core/Index.hpp \
	$(UPPDIR1)Core/Lang.h \
	$(UPPDIR1)Core/Lang_s.h \
	$(UPPDIR1)Core/Map.h \
	$(UPPDIR1)Core/Mt.h \
	$(UPPDIR1)Core/Other.h \
	$(UPPDIR1)Core/Parser.h \
	$(UPPDIR1)Core/Path.h \
	$(UPPDIR1)Core/Profile.h \
	$(UPPDIR1)Core/Ptr.h \
	$(UPPDIR1)Core/Stream.h \
	$(UPPDIR1)Core/String.h \
	$(UPPDIR1)Core/String.hpp \
	$(UPPDIR1)Core/t_.h \
	$(UPPDIR1)Core/Thread.h \
	$(UPPDIR1)Core/TimeDate.h \
	$(UPPDIR1)Core/Topic.h \
	$(UPPDIR1)Core/Topt.h \
	$(UPPDIR1)Core/Util.h \
	$(UPPDIR1)Core/Uuid.h \
	$(UPPDIR1)Core/Value.h \
	$(UPPDIR1)Core/Vcont.h \
	$(UPPDIR1)Core/Vcont.hpp \
	$(UPPDIR1)Core/Win32Com.h \
	$(UPPDIR1)Core/XML.h \
	$(UPPDIR1)Core/Xmlize.h \
	$(UPPDIR1)Core/z.h
	$(CPPFLAGS) $(CINC) $(Macro_Core)  $(UPPDIR1)Core/Hash.cpp -o $(OutDir_Core)Hash.o

$(OutDir_Core)Callback.o: $(UPPDIR1)Core/Callback.cpp \
	$(UPPDIR1)Core/Algo.h \
	$(UPPDIR1)Core/App.h \
	$(UPPDIR1)Core/BiCont.h \
	$(UPPDIR1)Core/Callback.h \
	$(UPPDIR1)Core/Cbgen.h \
	$(UPPDIR1)Core/CharSet.h \
	$(UPPDIR1)Core/Color.h \
	$(UPPDIR1)Core/Convert.h \
	$(UPPDIR1)Core/Core.h \
	$(UPPDIR1)Core/Defs.h \
	$(UPPDIR1)Core/Diag.h \
	$(UPPDIR1)Core/Format.h \
	$(UPPDIR1)Core/Global.h \
	$(UPPDIR1)Core/Gtypes.h \
	$(UPPDIR1)Core/i18n.h \
	$(UPPDIR1)Core/Index.h \
	$(UPPDIR1)Core/Index.hpp \
	$(UPPDIR1)Core/Lang.h \
	$(UPPDIR1)Core/Lang_s.h \
	$(UPPDIR1)Core/Map.h \
	$(UPPDIR1)Core/Mt.h \
	$(UPPDIR1)Core/Other.h \
	$(UPPDIR1)Core/Parser.h \
	$(UPPDIR1)Core/Path.h \
	$(UPPDIR1)Core/Profile.h \
	$(UPPDIR1)Core/Ptr.h \
	$(UPPDIR1)Core/Stream.h \
	$(UPPDIR1)Core/String.h \
	$(UPPDIR1)Core/String.hpp \
	$(UPPDIR1)Core/t_.h \
	$(UPPDIR1)Core/Thread.h \
	$(UPPDIR1)Core/TimeDate.h \
	$(UPPDIR1)Core/Topic.h \
	$(UPPDIR1)Core/Topt.h \
	$(UPPDIR1)Core/Util.h \
	$(UPPDIR1)Core/Uuid.h \
	$(UPPDIR1)Core/Value.h \
	$(UPPDIR1)Core/Vcont.h \
	$(UPPDIR1)Core/Vcont.hpp \
	$(UPPDIR1)Core/Win32Com.h \
	$(UPPDIR1)Core/XML.h \
	$(UPPDIR1)Core/Xmlize.h \
	$(UPPDIR1)Core/z.h
	$(CPPFLAGS) $(CINC) $(Macro_Core)  $(UPPDIR1)Core/Callback.cpp -o $(OutDir_Core)Callback.o

$(OutDir_Core)Color.o: $(UPPDIR1)Core/Color.cpp \
	$(UPPDIR1)Core/Algo.h \
	$(UPPDIR1)Core/App.h \
	$(UPPDIR1)Core/BiCont.h \
	$(UPPDIR1)Core/Callback.h \
	$(UPPDIR1)Core/Cbgen.h \
	$(UPPDIR1)Core/CharSet.h \
	$(UPPDIR1)Core/Color.h \
	$(UPPDIR1)Core/Convert.h \
	$(UPPDIR1)Core/Core.h \
	$(UPPDIR1)Core/Defs.h \
	$(UPPDIR1)Core/Diag.h \
	$(UPPDIR1)Core/Format.h \
	$(UPPDIR1)Core/Global.h \
	$(UPPDIR1)Core/Gtypes.h \
	$(UPPDIR1)Core/i18n.h \
	$(UPPDIR1)Core/Index.h \
	$(UPPDIR1)Core/Index.hpp \
	$(UPPDIR1)Core/Lang.h \
	$(UPPDIR1)Core/Lang_s.h \
	$(UPPDIR1)Core/Map.h \
	$(UPPDIR1)Core/Mt.h \
	$(UPPDIR1)Core/Other.h \
	$(UPPDIR1)Core/Parser.h \
	$(UPPDIR1)Core/Path.h \
	$(UPPDIR1)Core/Profile.h \
	$(UPPDIR1)Core/Ptr.h \
	$(UPPDIR1)Core/Stream.h \
	$(UPPDIR1)Core/String.h \
	$(UPPDIR1)Core/String.hpp \
	$(UPPDIR1)Core/t_.h \
	$(UPPDIR1)Core/Thread.h \
	$(UPPDIR1)Core/TimeDate.h \
	$(UPPDIR1)Core/Topic.h \
	$(UPPDIR1)Core/Topt.h \
	$(UPPDIR1)Core/Util.h \
	$(UPPDIR1)Core/Uuid.h \
	$(UPPDIR1)Core/Value.h \
	$(UPPDIR1)Core/Vcont.h \
	$(UPPDIR1)Core/Vcont.hpp \
	$(UPPDIR1)Core/Win32Com.h \
	$(UPPDIR1)Core/XML.h \
	$(UPPDIR1)Core/Xmlize.h \
	$(UPPDIR1)Core/z.h
	$(CPPFLAGS) $(CINC) $(Macro_Core)  $(UPPDIR1)Core/Color.cpp -o $(OutDir_Core)Color.o

$(OutDir_Core)Gtypes.o: $(UPPDIR1)Core/Gtypes.cpp \
	$(UPPDIR1)Core/Algo.h \
	$(UPPDIR1)Core/App.h \
	$(UPPDIR1)Core/BiCont.h \
	$(UPPDIR1)Core/Callback.h \
	$(UPPDIR1)Core/Cbgen.h \
	$(UPPDIR1)Core/CharSet.h \
	$(UPPDIR1)Core/Color.h \
	$(UPPDIR1)Core/Convert.h \
	$(UPPDIR1)Core/Core.h \
	$(UPPDIR1)Core/Defs.h \
	$(UPPDIR1)Core/Diag.h \
	$(UPPDIR1)Core/Format.h \
	$(UPPDIR1)Core/Global.h \
	$(UPPDIR1)Core/Gtypes.h \
	$(UPPDIR1)Core/i18n.h \
	$(UPPDIR1)Core/Index.h \
	$(UPPDIR1)Core/Index.hpp \
	$(UPPDIR1)Core/Lang.h \
	$(UPPDIR1)Core/Lang_s.h \
	$(UPPDIR1)Core/Map.h \
	$(UPPDIR1)Core/Mt.h \
	$(UPPDIR1)Core/Other.h \
	$(UPPDIR1)Core/Parser.h \
	$(UPPDIR1)Core/Path.h \
	$(UPPDIR1)Core/Profile.h \
	$(UPPDIR1)Core/Ptr.h \
	$(UPPDIR1)Core/Stream.h \
	$(UPPDIR1)Core/String.h \
	$(UPPDIR1)Core/String.hpp \
	$(UPPDIR1)Core/t_.h \
	$(UPPDIR1)Core/Thread.h \
	$(UPPDIR1)Core/TimeDate.h \
	$(UPPDIR1)Core/Topic.h \
	$(UPPDIR1)Core/Topt.h \
	$(UPPDIR1)Core/Util.h \
	$(UPPDIR1)Core/Uuid.h \
	$(UPPDIR1)Core/Value.h \
	$(UPPDIR1)Core/Vcont.h \
	$(UPPDIR1)Core/Vcont.hpp \
	$(UPPDIR1)Core/Win32Com.h \
	$(UPPDIR1)Core/XML.h \
	$(UPPDIR1)Core/Xmlize.h \
	$(UPPDIR1)Core/z.h
	$(CPPFLAGS) $(CINC) $(Macro_Core)  $(UPPDIR1)Core/Gtypes.cpp -o $(OutDir_Core)Gtypes.o

$(OutDir_Core)TimeDate.o: $(UPPDIR1)Core/TimeDate.cpp \
	$(UPPDIR1)Core/Algo.h \
	$(UPPDIR1)Core/App.h \
	$(UPPDIR1)Core/BiCont.h \
	$(UPPDIR1)Core/Callback.h \
	$(UPPDIR1)Core/Cbgen.h \
	$(UPPDIR1)Core/CharSet.h \
	$(UPPDIR1)Core/Color.h \
	$(UPPDIR1)Core/Convert.h \
	$(UPPDIR1)Core/Core.h \
	$(UPPDIR1)Core/Defs.h \
	$(UPPDIR1)Core/Diag.h \
	$(UPPDIR1)Core/Format.h \
	$(UPPDIR1)Core/Global.h \
	$(UPPDIR1)Core/Gtypes.h \
	$(UPPDIR1)Core/i18n.h \
	$(UPPDIR1)Core/Index.h \
	$(UPPDIR1)Core/Index.hpp \
	$(UPPDIR1)Core/Lang.h \
	$(UPPDIR1)Core/Lang_s.h \
	$(UPPDIR1)Core/Map.h \
	$(UPPDIR1)Core/Mt.h \
	$(UPPDIR1)Core/Other.h \
	$(UPPDIR1)Core/Parser.h \
	$(UPPDIR1)Core/Path.h \
	$(UPPDIR1)Core/Profile.h \
	$(UPPDIR1)Core/Ptr.h \
	$(UPPDIR1)Core/Stream.h \
	$(UPPDIR1)Core/String.h \
	$(UPPDIR1)Core/String.hpp \
	$(UPPDIR1)Core/t_.h \
	$(UPPDIR1)Core/Thread.h \
	$(UPPDIR1)Core/TimeDate.h \
	$(UPPDIR1)Core/Topic.h \
	$(UPPDIR1)Core/Topt.h \
	$(UPPDIR1)Core/Util.h \
	$(UPPDIR1)Core/Uuid.h \
	$(UPPDIR1)Core/Value.h \
	$(UPPDIR1)Core/Vcont.h \
	$(UPPDIR1)Core/Vcont.hpp \
	$(UPPDIR1)Core/Win32Com.h \
	$(UPPDIR1)Core/XML.h \
	$(UPPDIR1)Core/Xmlize.h \
	$(UPPDIR1)Core/z.h
	$(CPPFLAGS) $(CINC) $(Macro_Core)  $(UPPDIR1)Core/TimeDate.cpp -o $(OutDir_Core)TimeDate.o

$(OutDir_Core)Value.o: $(UPPDIR1)Core/Value.cpp \
	$(UPPDIR1)Core/Algo.h \
	$(UPPDIR1)Core/App.h \
	$(UPPDIR1)Core/BiCont.h \
	$(UPPDIR1)Core/Callback.h \
	$(UPPDIR1)Core/Cbgen.h \
	$(UPPDIR1)Core/CharSet.h \
	$(UPPDIR1)Core/Color.h \
	$(UPPDIR1)Core/Convert.h \
	$(UPPDIR1)Core/Core.h \
	$(UPPDIR1)Core/Defs.h \
	$(UPPDIR1)Core/Diag.h \
	$(UPPDIR1)Core/Format.h \
	$(UPPDIR1)Core/Global.h \
	$(UPPDIR1)Core/Gtypes.h \
	$(UPPDIR1)Core/i18n.h \
	$(UPPDIR1)Core/Index.h \
	$(UPPDIR1)Core/Index.hpp \
	$(UPPDIR1)Core/Lang.h \
	$(UPPDIR1)Core/Lang_s.h \
	$(UPPDIR1)Core/Map.h \
	$(UPPDIR1)Core/Mt.h \
	$(UPPDIR1)Core/Other.h \
	$(UPPDIR1)Core/Parser.h \
	$(UPPDIR1)Core/Path.h \
	$(UPPDIR1)Core/Profile.h \
	$(UPPDIR1)Core/Ptr.h \
	$(UPPDIR1)Core/Stream.h \
	$(UPPDIR1)Core/String.h \
	$(UPPDIR1)Core/String.hpp \
	$(UPPDIR1)Core/t_.h \
	$(UPPDIR1)Core/Thread.h \
	$(UPPDIR1)Core/TimeDate.h \
	$(UPPDIR1)Core/Topic.h \
	$(UPPDIR1)Core/Topt.h \
	$(UPPDIR1)Core/Util.h \
	$(UPPDIR1)Core/Uuid.h \
	$(UPPDIR1)Core/Value.h \
	$(UPPDIR1)Core/Vcont.h \
	$(UPPDIR1)Core/Vcont.hpp \
	$(UPPDIR1)Core/Win32Com.h \
	$(UPPDIR1)Core/XML.h \
	$(UPPDIR1)Core/Xmlize.h \
	$(UPPDIR1)Core/z.h
	$(CPPFLAGS) $(CINC) $(Macro_Core)  $(UPPDIR1)Core/Value.cpp -o $(OutDir_Core)Value.o

$(OutDir_Core)Convert.o: $(UPPDIR1)Core/Convert.cpp \
	$(UPPDIR1)Core/Algo.h \
	$(UPPDIR1)Core/App.h \
	$(UPPDIR1)Core/BiCont.h \
	$(UPPDIR1)Core/Callback.h \
	$(UPPDIR1)Core/Cbgen.h \
	$(UPPDIR1)Core/CharSet.h \
	$(UPPDIR1)Core/Color.h \
	$(UPPDIR1)Core/Convert.h \
	$(UPPDIR1)Core/Core.h \
	$(UPPDIR1)Core/Defs.h \
	$(UPPDIR1)Core/Diag.h \
	$(UPPDIR1)Core/Format.h \
	$(UPPDIR1)Core/Global.h \
	$(UPPDIR1)Core/Gtypes.h \
	$(UPPDIR1)Core/i18n.h \
	$(UPPDIR1)Core/Index.h \
	$(UPPDIR1)Core/Index.hpp \
	$(UPPDIR1)Core/Lang.h \
	$(UPPDIR1)Core/Lang_s.h \
	$(UPPDIR1)Core/Map.h \
	$(UPPDIR1)Core/Mt.h \
	$(UPPDIR1)Core/Other.h \
	$(UPPDIR1)Core/Parser.h \
	$(UPPDIR1)Core/Path.h \
	$(UPPDIR1)Core/Profile.h \
	$(UPPDIR1)Core/Ptr.h \
	$(UPPDIR1)Core/Stream.h \
	$(UPPDIR1)Core/String.h \
	$(UPPDIR1)Core/String.hpp \
	$(UPPDIR1)Core/t_.h \
	$(UPPDIR1)Core/Thread.h \
	$(UPPDIR1)Core/TimeDate.h \
	$(UPPDIR1)Core/Topic.h \
	$(UPPDIR1)Core/Topt.h \
	$(UPPDIR1)Core/Util.h \
	$(UPPDIR1)Core/Uuid.h \
	$(UPPDIR1)Core/Value.h \
	$(UPPDIR1)Core/Vcont.h \
	$(UPPDIR1)Core/Vcont.hpp \
	$(UPPDIR1)Core/Win32Com.h \
	$(UPPDIR1)Core/XML.h \
	$(UPPDIR1)Core/Xmlize.h \
	$(UPPDIR1)Core/z.h
	$(CPPFLAGS) $(CINC) $(Macro_Core)  $(UPPDIR1)Core/Convert.cpp -o $(OutDir_Core)Convert.o

$(OutDir_Core)Format.o: $(UPPDIR1)Core/Format.cpp \
	$(UPPDIR1)Core/Algo.h \
	$(UPPDIR1)Core/App.h \
	$(UPPDIR1)Core/BiCont.h \
	$(UPPDIR1)Core/Callback.h \
	$(UPPDIR1)Core/Cbgen.h \
	$(UPPDIR1)Core/CharSet.h \
	$(UPPDIR1)Core/Color.h \
	$(UPPDIR1)Core/Convert.h \
	$(UPPDIR1)Core/Core.h \
	$(UPPDIR1)Core/Defs.h \
	$(UPPDIR1)Core/Diag.h \
	$(UPPDIR1)Core/Format.h \
	$(UPPDIR1)Core/Global.h \
	$(UPPDIR1)Core/Gtypes.h \
	$(UPPDIR1)Core/i18n.h \
	$(UPPDIR1)Core/Index.h \
	$(UPPDIR1)Core/Index.hpp \
	$(UPPDIR1)Core/Lang.h \
	$(UPPDIR1)Core/Lang_s.h \
	$(UPPDIR1)Core/Map.h \
	$(UPPDIR1)Core/Mt.h \
	$(UPPDIR1)Core/Other.h \
	$(UPPDIR1)Core/Parser.h \
	$(UPPDIR1)Core/Path.h \
	$(UPPDIR1)Core/Profile.h \
	$(UPPDIR1)Core/Ptr.h \
	$(UPPDIR1)Core/Stream.h \
	$(UPPDIR1)Core/String.h \
	$(UPPDIR1)Core/String.hpp \
	$(UPPDIR1)Core/t_.h \
	$(UPPDIR1)Core/Thread.h \
	$(UPPDIR1)Core/TimeDate.h \
	$(UPPDIR1)Core/Topic.h \
	$(UPPDIR1)Core/Topt.h \
	$(UPPDIR1)Core/Util.h \
	$(UPPDIR1)Core/Uuid.h \
	$(UPPDIR1)Core/Value.h \
	$(UPPDIR1)Core/Vcont.h \
	$(UPPDIR1)Core/Vcont.hpp \
	$(UPPDIR1)Core/Win32Com.h \
	$(UPPDIR1)Core/XML.h \
	$(UPPDIR1)Core/Xmlize.h \
	$(UPPDIR1)Core/z.h
	$(CPPFLAGS) $(CINC) $(Macro_Core)  $(UPPDIR1)Core/Format.cpp -o $(OutDir_Core)Format.o

$(OutDir_Core)t.o: $(UPPDIR1)Core/t.cpp \
	$(UPPDIR1)Core/Algo.h \
	$(UPPDIR1)Core/App.h \
	$(UPPDIR1)Core/BiCont.h \
	$(UPPDIR1)Core/Callback.h \
	$(UPPDIR1)Core/Cbgen.h \
	$(UPPDIR1)Core/CharSet.h \
	$(UPPDIR1)Core/Color.h \
	$(UPPDIR1)Core/Convert.h \
	$(UPPDIR1)Core/Core.h \
	$(UPPDIR1)Core/Core.t \
	$(UPPDIR1)Core/Defs.h \
	$(UPPDIR1)Core/Diag.h \
	$(UPPDIR1)Core/Format.h \
	$(UPPDIR1)Core/Global.h \
	$(UPPDIR1)Core/Gtypes.h \
	$(UPPDIR1)Core/i18n.h \
	$(UPPDIR1)Core/Index.h \
	$(UPPDIR1)Core/Index.hpp \
	$(UPPDIR1)Core/Lang.h \
	$(UPPDIR1)Core/Lang_s.h \
	$(UPPDIR1)Core/Map.h \
	$(UPPDIR1)Core/Mt.h \
	$(UPPDIR1)Core/Other.h \
	$(UPPDIR1)Core/Parser.h \
	$(UPPDIR1)Core/Path.h \
	$(UPPDIR1)Core/Profile.h \
	$(UPPDIR1)Core/Ptr.h \
	$(UPPDIR1)Core/Stream.h \
	$(UPPDIR1)Core/String.h \
	$(UPPDIR1)Core/String.hpp \
	$(UPPDIR1)Core/t.h \
	$(UPPDIR1)Core/t_.h \
	$(UPPDIR1)Core/Thread.h \
	$(UPPDIR1)Core/TimeDate.h \
	$(UPPDIR1)Core/Topic.h \
	$(UPPDIR1)Core/Topt.h \
	$(UPPDIR1)Core/Util.h \
	$(UPPDIR1)Core/Uuid.h \
	$(UPPDIR1)Core/Value.h \
	$(UPPDIR1)Core/Vcont.h \
	$(UPPDIR1)Core/Vcont.hpp \
	$(UPPDIR1)Core/Win32Com.h \
	$(UPPDIR1)Core/XML.h \
	$(UPPDIR1)Core/Xmlize.h \
	$(UPPDIR1)Core/z.h
	$(CPPFLAGS) $(CINC) $(Macro_Core)  $(UPPDIR1)Core/t.cpp -o $(OutDir_Core)t.o

$(OutDir_Core)Lang.o: $(UPPDIR1)Core/Lang.cpp \
	$(UPPDIR1)Core/Algo.h \
	$(UPPDIR1)Core/App.h \
	$(UPPDIR1)Core/BiCont.h \
	$(UPPDIR1)Core/Callback.h \
	$(UPPDIR1)Core/Cbgen.h \
	$(UPPDIR1)Core/CharSet.h \
	$(UPPDIR1)Core/Color.h \
	$(UPPDIR1)Core/Convert.h \
	$(UPPDIR1)Core/Core.h \
	$(UPPDIR1)Core/Defs.h \
	$(UPPDIR1)Core/Diag.h \
	$(UPPDIR1)Core/Format.h \
	$(UPPDIR1)Core/Global.h \
	$(UPPDIR1)Core/Gtypes.h \
	$(UPPDIR1)Core/i18n.h \
	$(UPPDIR1)Core/Index.h \
	$(UPPDIR1)Core/Index.hpp \
	$(UPPDIR1)Core/Lang.h \
	$(UPPDIR1)Core/Lang_s.h \
	$(UPPDIR1)Core/Map.h \
	$(UPPDIR1)Core/Mt.h \
	$(UPPDIR1)Core/Other.h \
	$(UPPDIR1)Core/Parser.h \
	$(UPPDIR1)Core/Path.h \
	$(UPPDIR1)Core/Profile.h \
	$(UPPDIR1)Core/Ptr.h \
	$(UPPDIR1)Core/Stream.h \
	$(UPPDIR1)Core/String.h \
	$(UPPDIR1)Core/String.hpp \
	$(UPPDIR1)Core/t_.h \
	$(UPPDIR1)Core/Thread.h \
	$(UPPDIR1)Core/TimeDate.h \
	$(UPPDIR1)Core/Topic.h \
	$(UPPDIR1)Core/Topt.h \
	$(UPPDIR1)Core/Util.h \
	$(UPPDIR1)Core/Uuid.h \
	$(UPPDIR1)Core/Value.h \
	$(UPPDIR1)Core/Vcont.h \
	$(UPPDIR1)Core/Vcont.hpp \
	$(UPPDIR1)Core/Win32Com.h \
	$(UPPDIR1)Core/XML.h \
	$(UPPDIR1)Core/Xmlize.h \
	$(UPPDIR1)Core/z.h
	$(CPPFLAGS) $(CINC) $(Macro_Core)  $(UPPDIR1)Core/Lang.cpp -o $(OutDir_Core)Lang.o

$(OutDir_Core)parser.o: $(UPPDIR1)Core/parser.cpp \
	$(UPPDIR1)Core/Algo.h \
	$(UPPDIR1)Core/App.h \
	$(UPPDIR1)Core/BiCont.h \
	$(UPPDIR1)Core/Callback.h \
	$(UPPDIR1)Core/Cbgen.h \
	$(UPPDIR1)Core/CharSet.h \
	$(UPPDIR1)Core/Color.h \
	$(UPPDIR1)Core/Convert.h \
	$(UPPDIR1)Core/Core.h \
	$(UPPDIR1)Core/Defs.h \
	$(UPPDIR1)Core/Diag.h \
	$(UPPDIR1)Core/Format.h \
	$(UPPDIR1)Core/Global.h \
	$(UPPDIR1)Core/Gtypes.h \
	$(UPPDIR1)Core/i18n.h \
	$(UPPDIR1)Core/Index.h \
	$(UPPDIR1)Core/Index.hpp \
	$(UPPDIR1)Core/Lang.h \
	$(UPPDIR1)Core/Lang_s.h \
	$(UPPDIR1)Core/Map.h \
	$(UPPDIR1)Core/Mt.h \
	$(UPPDIR1)Core/Other.h \
	$(UPPDIR1)Core/Parser.h \
	$(UPPDIR1)Core/Path.h \
	$(UPPDIR1)Core/Profile.h \
	$(UPPDIR1)Core/Ptr.h \
	$(UPPDIR1)Core/Stream.h \
	$(UPPDIR1)Core/String.h \
	$(UPPDIR1)Core/String.hpp \
	$(UPPDIR1)Core/t_.h \
	$(UPPDIR1)Core/Thread.h \
	$(UPPDIR1)Core/TimeDate.h \
	$(UPPDIR1)Core/Topic.h \
	$(UPPDIR1)Core/Topt.h \
	$(UPPDIR1)Core/Util.h \
	$(UPPDIR1)Core/Uuid.h \
	$(UPPDIR1)Core/Value.h \
	$(UPPDIR1)Core/Vcont.h \
	$(UPPDIR1)Core/Vcont.hpp \
	$(UPPDIR1)Core/Win32Com.h \
	$(UPPDIR1)Core/XML.h \
	$(UPPDIR1)Core/Xmlize.h \
	$(UPPDIR1)Core/z.h
	$(CPPFLAGS) $(CINC) $(Macro_Core)  $(UPPDIR1)Core/parser.cpp -o $(OutDir_Core)parser.o

$(OutDir_Core)XML.o: $(UPPDIR1)Core/XML.cpp \
	$(UPPDIR1)Core/Algo.h \
	$(UPPDIR1)Core/App.h \
	$(UPPDIR1)Core/BiCont.h \
	$(UPPDIR1)Core/Callback.h \
	$(UPPDIR1)Core/Cbgen.h \
	$(UPPDIR1)Core/CharSet.h \
	$(UPPDIR1)Core/Color.h \
	$(UPPDIR1)Core/Convert.h \
	$(UPPDIR1)Core/Core.h \
	$(UPPDIR1)Core/Defs.h \
	$(UPPDIR1)Core/Diag.h \
	$(UPPDIR1)Core/Format.h \
	$(UPPDIR1)Core/Global.h \
	$(UPPDIR1)Core/Gtypes.h \
	$(UPPDIR1)Core/i18n.h \
	$(UPPDIR1)Core/Index.h \
	$(UPPDIR1)Core/Index.hpp \
	$(UPPDIR1)Core/Lang.h \
	$(UPPDIR1)Core/Lang_s.h \
	$(UPPDIR1)Core/Map.h \
	$(UPPDIR1)Core/Mt.h \
	$(UPPDIR1)Core/Other.h \
	$(UPPDIR1)Core/Parser.h \
	$(UPPDIR1)Core/Path.h \
	$(UPPDIR1)Core/Profile.h \
	$(UPPDIR1)Core/Ptr.h \
	$(UPPDIR1)Core/Stream.h \
	$(UPPDIR1)Core/String.h \
	$(UPPDIR1)Core/String.hpp \
	$(UPPDIR1)Core/t_.h \
	$(UPPDIR1)Core/Thread.h \
	$(UPPDIR1)Core/TimeDate.h \
	$(UPPDIR1)Core/Topic.h \
	$(UPPDIR1)Core/Topt.h \
	$(UPPDIR1)Core/Util.h \
	$(UPPDIR1)Core/Uuid.h \
	$(UPPDIR1)Core/Value.h \
	$(UPPDIR1)Core/Vcont.h \
	$(UPPDIR1)Core/Vcont.hpp \
	$(UPPDIR1)Core/Win32Com.h \
	$(UPPDIR1)Core/XML.h \
	$(UPPDIR1)Core/Xmlize.h \
	$(UPPDIR1)Core/z.h
	$(CPPFLAGS) $(CINC) $(Macro_Core)  $(UPPDIR1)Core/XML.cpp -o $(OutDir_Core)XML.o

$(OutDir_Core)Xmlize.o: $(UPPDIR1)Core/Xmlize.cpp \
	$(UPPDIR1)Core/Algo.h \
	$(UPPDIR1)Core/App.h \
	$(UPPDIR1)Core/BiCont.h \
	$(UPPDIR1)Core/Callback.h \
	$(UPPDIR1)Core/Cbgen.h \
	$(UPPDIR1)Core/CharSet.h \
	$(UPPDIR1)Core/Color.h \
	$(UPPDIR1)Core/Convert.h \
	$(UPPDIR1)Core/Core.h \
	$(UPPDIR1)Core/Defs.h \
	$(UPPDIR1)Core/Diag.h \
	$(UPPDIR1)Core/Format.h \
	$(UPPDIR1)Core/Global.h \
	$(UPPDIR1)Core/Gtypes.h \
	$(UPPDIR1)Core/i18n.h \
	$(UPPDIR1)Core/Index.h \
	$(UPPDIR1)Core/Index.hpp \
	$(UPPDIR1)Core/Lang.h \
	$(UPPDIR1)Core/Lang_s.h \
	$(UPPDIR1)Core/Map.h \
	$(UPPDIR1)Core/Mt.h \
	$(UPPDIR1)Core/Other.h \
	$(UPPDIR1)Core/Parser.h \
	$(UPPDIR1)Core/Path.h \
	$(UPPDIR1)Core/Profile.h \
	$(UPPDIR1)Core/Ptr.h \
	$(UPPDIR1)Core/Stream.h \
	$(UPPDIR1)Core/String.h \
	$(UPPDIR1)Core/String.hpp \
	$(UPPDIR1)Core/t_.h \
	$(UPPDIR1)Core/Thread.h \
	$(UPPDIR1)Core/TimeDate.h \
	$(UPPDIR1)Core/Topic.h \
	$(UPPDIR1)Core/Topt.h \
	$(UPPDIR1)Core/Util.h \
	$(UPPDIR1)Core/Uuid.h \
	$(UPPDIR1)Core/Value.h \
	$(UPPDIR1)Core/Vcont.h \
	$(UPPDIR1)Core/Vcont.hpp \
	$(UPPDIR1)Core/Win32Com.h \
	$(UPPDIR1)Core/XML.h \
	$(UPPDIR1)Core/Xmlize.h \
	$(UPPDIR1)Core/z.h
	$(CPPFLAGS) $(CINC) $(Macro_Core)  $(UPPDIR1)Core/Xmlize.cpp -o $(OutDir_Core)Xmlize.o

$(OutDir_Core)Uuid.o: $(UPPDIR1)Core/Uuid.cpp \
	$(UPPDIR1)Core/Algo.h \
	$(UPPDIR1)Core/App.h \
	$(UPPDIR1)Core/BiCont.h \
	$(UPPDIR1)Core/Callback.h \
	$(UPPDIR1)Core/Cbgen.h \
	$(UPPDIR1)Core/CharSet.h \
	$(UPPDIR1)Core/Color.h \
	$(UPPDIR1)Core/Convert.h \
	$(UPPDIR1)Core/Core.h \
	$(UPPDIR1)Core/Defs.h \
	$(UPPDIR1)Core/Diag.h \
	$(UPPDIR1)Core/Format.h \
	$(UPPDIR1)Core/Global.h \
	$(UPPDIR1)Core/Gtypes.h \
	$(UPPDIR1)Core/i18n.h \
	$(UPPDIR1)Core/Index.h \
	$(UPPDIR1)Core/Index.hpp \
	$(UPPDIR1)Core/Lang.h \
	$(UPPDIR1)Core/Lang_s.h \
	$(UPPDIR1)Core/Map.h \
	$(UPPDIR1)Core/Mt.h \
	$(UPPDIR1)Core/Other.h \
	$(UPPDIR1)Core/Parser.h \
	$(UPPDIR1)Core/Path.h \
	$(UPPDIR1)Core/Profile.h \
	$(UPPDIR1)Core/Ptr.h \
	$(UPPDIR1)Core/Stream.h \
	$(UPPDIR1)Core/String.h \
	$(UPPDIR1)Core/String.hpp \
	$(UPPDIR1)Core/t_.h \
	$(UPPDIR1)Core/Thread.h \
	$(UPPDIR1)Core/TimeDate.h \
	$(UPPDIR1)Core/Topic.h \
	$(UPPDIR1)Core/Topt.h \
	$(UPPDIR1)Core/Util.h \
	$(UPPDIR1)Core/Uuid.h \
	$(UPPDIR1)Core/Value.h \
	$(UPPDIR1)Core/Vcont.h \
	$(UPPDIR1)Core/Vcont.hpp \
	$(UPPDIR1)Core/Win32Com.h \
	$(UPPDIR1)Core/XML.h \
	$(UPPDIR1)Core/Xmlize.h \
	$(UPPDIR1)Core/z.h
	$(CPPFLAGS) $(CINC) $(Macro_Core)  $(UPPDIR1)Core/Uuid.cpp -o $(OutDir_Core)Uuid.o

$(OutDir_Core)Ptr.o: $(UPPDIR1)Core/Ptr.cpp \
	$(UPPDIR1)Core/Algo.h \
	$(UPPDIR1)Core/App.h \
	$(UPPDIR1)Core/BiCont.h \
	$(UPPDIR1)Core/Callback.h \
	$(UPPDIR1)Core/Cbgen.h \
	$(UPPDIR1)Core/CharSet.h \
	$(UPPDIR1)Core/Color.h \
	$(UPPDIR1)Core/Convert.h \
	$(UPPDIR1)Core/Core.h \
	$(UPPDIR1)Core/Defs.h \
	$(UPPDIR1)Core/Diag.h \
	$(UPPDIR1)Core/Format.h \
	$(UPPDIR1)Core/Global.h \
	$(UPPDIR1)Core/Gtypes.h \
	$(UPPDIR1)Core/i18n.h \
	$(UPPDIR1)Core/Index.h \
	$(UPPDIR1)Core/Index.hpp \
	$(UPPDIR1)Core/Lang.h \
	$(UPPDIR1)Core/Lang_s.h \
	$(UPPDIR1)Core/Map.h \
	$(UPPDIR1)Core/Mt.h \
	$(UPPDIR1)Core/Other.h \
	$(UPPDIR1)Core/Parser.h \
	$(UPPDIR1)Core/Path.h \
	$(UPPDIR1)Core/Profile.h \
	$(UPPDIR1)Core/Ptr.h \
	$(UPPDIR1)Core/Stream.h \
	$(UPPDIR1)Core/String.h \
	$(UPPDIR1)Core/String.hpp \
	$(UPPDIR1)Core/t_.h \
	$(UPPDIR1)Core/Thread.h \
	$(UPPDIR1)Core/TimeDate.h \
	$(UPPDIR1)Core/Topic.h \
	$(UPPDIR1)Core/Topt.h \
	$(UPPDIR1)Core/Util.h \
	$(UPPDIR1)Core/Uuid.h \
	$(UPPDIR1)Core/Value.h \
	$(UPPDIR1)Core/Vcont.h \
	$(UPPDIR1)Core/Vcont.hpp \
	$(UPPDIR1)Core/Win32Com.h \
	$(UPPDIR1)Core/XML.h \
	$(UPPDIR1)Core/Xmlize.h \
	$(UPPDIR1)Core/z.h
	$(CPPFLAGS) $(CINC) $(Macro_Core)  $(UPPDIR1)Core/Ptr.cpp -o $(OutDir_Core)Ptr.o

$(OutDir_Core)z.o: $(UPPDIR1)Core/z.cpp \
	$(UPPDIR1)Core/Algo.h \
	$(UPPDIR1)Core/App.h \
	$(UPPDIR1)Core/BiCont.h \
	$(UPPDIR1)Core/Callback.h \
	$(UPPDIR1)Core/Cbgen.h \
	$(UPPDIR1)Core/CharSet.h \
	$(UPPDIR1)Core/Color.h \
	$(UPPDIR1)Core/Convert.h \
	$(UPPDIR1)Core/Core.h \
	$(UPPDIR1)Core/Defs.h \
	$(UPPDIR1)Core/Diag.h \
	$(UPPDIR1)Core/Format.h \
	$(UPPDIR1)Core/Global.h \
	$(UPPDIR1)Core/Gtypes.h \
	$(UPPDIR1)Core/i18n.h \
	$(UPPDIR1)Core/Index.h \
	$(UPPDIR1)Core/Index.hpp \
	$(UPPDIR1)Core/Lang.h \
	$(UPPDIR1)Core/Lang_s.h \
	$(UPPDIR1)Core/Map.h \
	$(UPPDIR1)Core/Mt.h \
	$(UPPDIR1)Core/Other.h \
	$(UPPDIR1)Core/Parser.h \
	$(UPPDIR1)Core/Path.h \
	$(UPPDIR1)Core/Profile.h \
	$(UPPDIR1)Core/Ptr.h \
	$(UPPDIR1)Core/Stream.h \
	$(UPPDIR1)Core/String.h \
	$(UPPDIR1)Core/String.hpp \
	$(UPPDIR1)Core/t_.h \
	$(UPPDIR1)Core/Thread.h \
	$(UPPDIR1)Core/TimeDate.h \
	$(UPPDIR1)Core/Topic.h \
	$(UPPDIR1)Core/Topt.h \
	$(UPPDIR1)Core/Util.h \
	$(UPPDIR1)Core/Uuid.h \
	$(UPPDIR1)Core/Value.h \
	$(UPPDIR1)Core/Vcont.h \
	$(UPPDIR1)Core/Vcont.hpp \
	$(UPPDIR1)Core/Win32Com.h \
	$(UPPDIR1)Core/XML.h \
	$(UPPDIR1)Core/Xmlize.h \
	$(UPPDIR1)Core/z.h \
	$(UPPDIR1)plugin/z/lib/zconf.h \
	$(UPPDIR1)plugin/z/lib/zlib.h \
	$(UPPDIR1)plugin/z/lib/zutil.h
	$(CPPFLAGS) $(CINC) $(Macro_Core)  $(UPPDIR1)Core/z.cpp -o $(OutDir_Core)z.o

$(OutDir_Core)Topic.o: $(UPPDIR1)Core/Topic.cpp \
	$(UPPDIR1)Core/Algo.h \
	$(UPPDIR1)Core/App.h \
	$(UPPDIR1)Core/BiCont.h \
	$(UPPDIR1)Core/Callback.h \
	$(UPPDIR1)Core/Cbgen.h \
	$(UPPDIR1)Core/CharSet.h \
	$(UPPDIR1)Core/Color.h \
	$(UPPDIR1)Core/Convert.h \
	$(UPPDIR1)Core/Core.h \
	$(UPPDIR1)Core/Defs.h \
	$(UPPDIR1)Core/Diag.h \
	$(UPPDIR1)Core/Format.h \
	$(UPPDIR1)Core/Global.h \
	$(UPPDIR1)Core/Gtypes.h \
	$(UPPDIR1)Core/i18n.h \
	$(UPPDIR1)Core/Index.h \
	$(UPPDIR1)Core/Index.hpp \
	$(UPPDIR1)Core/Lang.h \
	$(UPPDIR1)Core/Lang_s.h \
	$(UPPDIR1)Core/Map.h \
	$(UPPDIR1)Core/Mt.h \
	$(UPPDIR1)Core/Other.h \
	$(UPPDIR1)Core/Parser.h \
	$(UPPDIR1)Core/Path.h \
	$(UPPDIR1)Core/Profile.h \
	$(UPPDIR1)Core/Ptr.h \
	$(UPPDIR1)Core/Stream.h \
	$(UPPDIR1)Core/String.h \
	$(UPPDIR1)Core/String.hpp \
	$(UPPDIR1)Core/t_.h \
	$(UPPDIR1)Core/Thread.h \
	$(UPPDIR1)Core/TimeDate.h \
	$(UPPDIR1)Core/Topic.h \
	$(UPPDIR1)Core/Topt.h \
	$(UPPDIR1)Core/Util.h \
	$(UPPDIR1)Core/Uuid.h \
	$(UPPDIR1)Core/Value.h \
	$(UPPDIR1)Core/Vcont.h \
	$(UPPDIR1)Core/Vcont.hpp \
	$(UPPDIR1)Core/Win32Com.h \
	$(UPPDIR1)Core/XML.h \
	$(UPPDIR1)Core/Xmlize.h \
	$(UPPDIR1)Core/z.h
	$(CPPFLAGS) $(CINC) $(Macro_Core)  $(UPPDIR1)Core/Topic.cpp -o $(OutDir_Core)Topic.o

$(OutDir_Core)Dli.o: $(UPPDIR1)Core/Dli.cpp \
	$(UPPDIR1)Core/Algo.h \
	$(UPPDIR1)Core/App.h \
	$(UPPDIR1)Core/BiCont.h \
	$(UPPDIR1)Core/Callback.h \
	$(UPPDIR1)Core/Cbgen.h \
	$(UPPDIR1)Core/CharSet.h \
	$(UPPDIR1)Core/Color.h \
	$(UPPDIR1)Core/Convert.h \
	$(UPPDIR1)Core/Core.h \
	$(UPPDIR1)Core/Defs.h \
	$(UPPDIR1)Core/Diag.h \
	$(UPPDIR1)Core/Format.h \
	$(UPPDIR1)Core/Global.h \
	$(UPPDIR1)Core/Gtypes.h \
	$(UPPDIR1)Core/i18n.h \
	$(UPPDIR1)Core/Index.h \
	$(UPPDIR1)Core/Index.hpp \
	$(UPPDIR1)Core/Lang.h \
	$(UPPDIR1)Core/Lang_s.h \
	$(UPPDIR1)Core/Map.h \
	$(UPPDIR1)Core/Mt.h \
	$(UPPDIR1)Core/Other.h \
	$(UPPDIR1)Core/Parser.h \
	$(UPPDIR1)Core/Path.h \
	$(UPPDIR1)Core/Profile.h \
	$(UPPDIR1)Core/Ptr.h \
	$(UPPDIR1)Core/Stream.h \
	$(UPPDIR1)Core/String.h \
	$(UPPDIR1)Core/String.hpp \
	$(UPPDIR1)Core/t_.h \
	$(UPPDIR1)Core/Thread.h \
	$(UPPDIR1)Core/TimeDate.h \
	$(UPPDIR1)Core/Topic.h \
	$(UPPDIR1)Core/Topt.h \
	$(UPPDIR1)Core/Util.h \
	$(UPPDIR1)Core/Uuid.h \
	$(UPPDIR1)Core/Value.h \
	$(UPPDIR1)Core/Vcont.h \
	$(UPPDIR1)Core/Vcont.hpp \
	$(UPPDIR1)Core/Win32Com.h \
	$(UPPDIR1)Core/XML.h \
	$(UPPDIR1)Core/Xmlize.h \
	$(UPPDIR1)Core/z.h
	$(CPPFLAGS) $(CINC) $(Macro_Core)  $(UPPDIR1)Core/Dli.cpp -o $(OutDir_Core)Dli.o

$(OutDir_Core)Win32Com.o: $(UPPDIR1)Core/Win32Com.cpp \
	$(UPPDIR1)Core/Algo.h \
	$(UPPDIR1)Core/App.h \
	$(UPPDIR1)Core/BiCont.h \
	$(UPPDIR1)Core/Callback.h \
	$(UPPDIR1)Core/Cbgen.h \
	$(UPPDIR1)Core/CharSet.h \
	$(UPPDIR1)Core/Color.h \
	$(UPPDIR1)Core/Convert.h \
	$(UPPDIR1)Core/Core.h \
	$(UPPDIR1)Core/Defs.h \
	$(UPPDIR1)Core/Diag.h \
	$(UPPDIR1)Core/Format.h \
	$(UPPDIR1)Core/Global.h \
	$(UPPDIR1)Core/Gtypes.h \
	$(UPPDIR1)Core/i18n.h \
	$(UPPDIR1)Core/Index.h \
	$(UPPDIR1)Core/Index.hpp \
	$(UPPDIR1)Core/Lang.h \
	$(UPPDIR1)Core/Lang_s.h \
	$(UPPDIR1)Core/Map.h \
	$(UPPDIR1)Core/Mt.h \
	$(UPPDIR1)Core/Other.h \
	$(UPPDIR1)Core/Parser.h \
	$(UPPDIR1)Core/Path.h \
	$(UPPDIR1)Core/Profile.h \
	$(UPPDIR1)Core/Ptr.h \
	$(UPPDIR1)Core/Stream.h \
	$(UPPDIR1)Core/String.h \
	$(UPPDIR1)Core/String.hpp \
	$(UPPDIR1)Core/t_.h \
	$(UPPDIR1)Core/Thread.h \
	$(UPPDIR1)Core/TimeDate.h \
	$(UPPDIR1)Core/Topic.h \
	$(UPPDIR1)Core/Topt.h \
	$(UPPDIR1)Core/Util.h \
	$(UPPDIR1)Core/Uuid.h \
	$(UPPDIR1)Core/Value.h \
	$(UPPDIR1)Core/Vcont.h \
	$(UPPDIR1)Core/Vcont.hpp \
	$(UPPDIR1)Core/Win32Com.h \
	$(UPPDIR1)Core/XML.h \
	$(UPPDIR1)Core/Xmlize.h \
	$(UPPDIR1)Core/z.h
	$(CPPFLAGS) $(CINC) $(Macro_Core)  $(UPPDIR1)Core/Win32Com.cpp -o $(OutDir_Core)Win32Com.o

$(OutDir_Core)Core.a: \
	$(OutDir_Core)Mt.o \
	$(OutDir_Core)Thread.o \
	$(OutDir_Core)OL_Set.o \
	$(OutDir_Core)heap.o \
	$(OutDir_Core)heapdbg.o \
	$(OutDir_Core)String.o \
	$(OutDir_Core)StrUtil.o \
	$(OutDir_Core)CharSet.o \
	$(OutDir_Core)Path.o \
	$(OutDir_Core)App.o \
	$(OutDir_Core)Stream.o \
	$(OutDir_Core)BlockStream.o \
	$(OutDir_Core)Log.o \
	$(OutDir_Core)Debug.o \
	$(OutDir_Core)Util.o \
	$(OutDir_Core)mathutil.o \
	$(OutDir_Core)Vcont.o \
	$(OutDir_Core)Hash.o \
	$(OutDir_Core)Callback.o \
	$(OutDir_Core)Color.o \
	$(OutDir_Core)Gtypes.o \
	$(OutDir_Core)TimeDate.o \
	$(OutDir_Core)Value.o \
	$(OutDir_Core)Convert.o \
	$(OutDir_Core)Format.o \
	$(OutDir_Core)t.o \
	$(OutDir_Core)Lang.o \
	$(OutDir_Core)parser.o \
	$(OutDir_Core)XML.o \
	$(OutDir_Core)Xmlize.o \
	$(OutDir_Core)Uuid.o \
	$(OutDir_Core)Ptr.o \
	$(OutDir_Core)z.o \
	$(OutDir_Core)Topic.o \
	$(OutDir_Core)Dli.o \
	$(OutDir_Core)Win32Com.o
	$(AR) $(OutDir_Core)Core.a \
		$(OutDir_Core)Mt.o \
		$(OutDir_Core)Thread.o \
		$(OutDir_Core)OL_Set.o \
		$(OutDir_Core)heap.o \
		$(OutDir_Core)heapdbg.o \
		$(OutDir_Core)String.o \
		$(OutDir_Core)StrUtil.o \
		$(OutDir_Core)CharSet.o \
		$(OutDir_Core)Path.o \
		$(OutDir_Core)App.o \
		$(OutDir_Core)Stream.o \
		$(OutDir_Core)BlockStream.o \
		$(OutDir_Core)Log.o \
		$(OutDir_Core)Debug.o \
		$(OutDir_Core)Util.o \
		$(OutDir_Core)mathutil.o \
		$(OutDir_Core)Vcont.o \
		$(OutDir_Core)Hash.o \
		$(OutDir_Core)Callback.o \
		$(OutDir_Core)Color.o \
		$(OutDir_Core)Gtypes.o \
		$(OutDir_Core)TimeDate.o \
		$(OutDir_Core)Value.o \
		$(OutDir_Core)Convert.o \
		$(OutDir_Core)Format.o \
		$(OutDir_Core)t.o \
		$(OutDir_Core)Lang.o \
		$(OutDir_Core)parser.o \
		$(OutDir_Core)XML.o \
		$(OutDir_Core)Xmlize.o \
		$(OutDir_Core)Uuid.o \
		$(OutDir_Core)Ptr.o \
		$(OutDir_Core)z.o \
		$(OutDir_Core)Topic.o \
		$(OutDir_Core)Dli.o \
		$(OutDir_Core)Win32Com.o

$(OutDir_plugin_z)adler32.o: $(UPPDIR1)plugin/z/lib/adler32.c \
	$(UPPDIR1)plugin/z/lib/zconf.h \
	$(UPPDIR1)plugin/z/lib/zlib.h
	$(CFLAGS) $(CINC) $(Macro_plugin_z)  $(UPPDIR1)plugin/z/lib/adler32.c -o $(OutDir_plugin_z)adler32.o

$(OutDir_plugin_z)compress.o: $(UPPDIR1)plugin/z/lib/compress.c \
	$(UPPDIR1)plugin/z/lib/zconf.h \
	$(UPPDIR1)plugin/z/lib/zlib.h
	$(CFLAGS) $(CINC) $(Macro_plugin_z)  $(UPPDIR1)plugin/z/lib/compress.c -o $(OutDir_plugin_z)compress.o

$(OutDir_plugin_z)crc32.o: $(UPPDIR1)plugin/z/lib/crc32.c \
	$(UPPDIR1)plugin/z/lib/zconf.h \
	$(UPPDIR1)plugin/z/lib/zlib.h
	$(CFLAGS) $(CINC) $(Macro_plugin_z)  $(UPPDIR1)plugin/z/lib/crc32.c -o $(OutDir_plugin_z)crc32.o

$(OutDir_plugin_z)deflate.o: $(UPPDIR1)plugin/z/lib/deflate.c \
	$(UPPDIR1)plugin/z/lib/deflate.h \
	$(UPPDIR1)plugin/z/lib/zconf.h \
	$(UPPDIR1)plugin/z/lib/zlib.h \
	$(UPPDIR1)plugin/z/lib/zutil.h
	$(CFLAGS) $(CINC) $(Macro_plugin_z)  $(UPPDIR1)plugin/z/lib/deflate.c -o $(OutDir_plugin_z)deflate.o

$(OutDir_plugin_z)gzio.o: $(UPPDIR1)plugin/z/lib/gzio.c \
	$(UPPDIR1)plugin/z/lib/zconf.h \
	$(UPPDIR1)plugin/z/lib/zlib.h \
	$(UPPDIR1)plugin/z/lib/zutil.h
	$(CFLAGS) $(CINC) $(Macro_plugin_z)  $(UPPDIR1)plugin/z/lib/gzio.c -o $(OutDir_plugin_z)gzio.o

$(OutDir_plugin_z)infblock.o: $(UPPDIR1)plugin/z/lib/infblock.c \
	$(UPPDIR1)plugin/z/lib/infblock.h \
	$(UPPDIR1)plugin/z/lib/infcodes.h \
	$(UPPDIR1)plugin/z/lib/inftrees.h \
	$(UPPDIR1)plugin/z/lib/infutil.h \
	$(UPPDIR1)plugin/z/lib/zconf.h \
	$(UPPDIR1)plugin/z/lib/zlib.h \
	$(UPPDIR1)plugin/z/lib/zutil.h
	$(CFLAGS) $(CINC) $(Macro_plugin_z)  $(UPPDIR1)plugin/z/lib/infblock.c -o $(OutDir_plugin_z)infblock.o

$(OutDir_plugin_z)infcodes.o: $(UPPDIR1)plugin/z/lib/infcodes.c \
	$(UPPDIR1)plugin/z/lib/infblock.h \
	$(UPPDIR1)plugin/z/lib/infcodes.h \
	$(UPPDIR1)plugin/z/lib/inffast.h \
	$(UPPDIR1)plugin/z/lib/inftrees.h \
	$(UPPDIR1)plugin/z/lib/infutil.h \
	$(UPPDIR1)plugin/z/lib/zconf.h \
	$(UPPDIR1)plugin/z/lib/zlib.h \
	$(UPPDIR1)plugin/z/lib/zutil.h
	$(CFLAGS) $(CINC) $(Macro_plugin_z)  $(UPPDIR1)plugin/z/lib/infcodes.c -o $(OutDir_plugin_z)infcodes.o

$(OutDir_plugin_z)inffast.o: $(UPPDIR1)plugin/z/lib/inffast.c \
	$(UPPDIR1)plugin/z/lib/infblock.h \
	$(UPPDIR1)plugin/z/lib/infcodes.h \
	$(UPPDIR1)plugin/z/lib/inffast.h \
	$(UPPDIR1)plugin/z/lib/inftrees.h \
	$(UPPDIR1)plugin/z/lib/infutil.h \
	$(UPPDIR1)plugin/z/lib/zconf.h \
	$(UPPDIR1)plugin/z/lib/zlib.h \
	$(UPPDIR1)plugin/z/lib/zutil.h
	$(CFLAGS) $(CINC) $(Macro_plugin_z)  $(UPPDIR1)plugin/z/lib/inffast.c -o $(OutDir_plugin_z)inffast.o

$(OutDir_plugin_z)inflate.o: $(UPPDIR1)plugin/z/lib/inflate.c \
	$(UPPDIR1)plugin/z/lib/infblock.h \
	$(UPPDIR1)plugin/z/lib/zconf.h \
	$(UPPDIR1)plugin/z/lib/zlib.h \
	$(UPPDIR1)plugin/z/lib/zutil.h
	$(CFLAGS) $(CINC) $(Macro_plugin_z)  $(UPPDIR1)plugin/z/lib/inflate.c -o $(OutDir_plugin_z)inflate.o

$(OutDir_plugin_z)inftrees.o: $(UPPDIR1)plugin/z/lib/inftrees.c \
	$(UPPDIR1)plugin/z/lib/inffixed.h \
	$(UPPDIR1)plugin/z/lib/inftrees.h \
	$(UPPDIR1)plugin/z/lib/zconf.h \
	$(UPPDIR1)plugin/z/lib/zlib.h \
	$(UPPDIR1)plugin/z/lib/zutil.h
	$(CFLAGS) $(CINC) $(Macro_plugin_z)  $(UPPDIR1)plugin/z/lib/inftrees.c -o $(OutDir_plugin_z)inftrees.o

$(OutDir_plugin_z)infutil.o: $(UPPDIR1)plugin/z/lib/infutil.c \
	$(UPPDIR1)plugin/z/lib/infblock.h \
	$(UPPDIR1)plugin/z/lib/infcodes.h \
	$(UPPDIR1)plugin/z/lib/inftrees.h \
	$(UPPDIR1)plugin/z/lib/infutil.h \
	$(UPPDIR1)plugin/z/lib/zconf.h \
	$(UPPDIR1)plugin/z/lib/zlib.h \
	$(UPPDIR1)plugin/z/lib/zutil.h
	$(CFLAGS) $(CINC) $(Macro_plugin_z)  $(UPPDIR1)plugin/z/lib/infutil.c -o $(OutDir_plugin_z)infutil.o

$(OutDir_plugin_z)trees.o: $(UPPDIR1)plugin/z/lib/trees.c \
	$(UPPDIR1)plugin/z/lib/deflate.h \
	$(UPPDIR1)plugin/z/lib/trees.h \
	$(UPPDIR1)plugin/z/lib/zconf.h \
	$(UPPDIR1)plugin/z/lib/zlib.h \
	$(UPPDIR1)plugin/z/lib/zutil.h
	$(CFLAGS) $(CINC) $(Macro_plugin_z)  $(UPPDIR1)plugin/z/lib/trees.c -o $(OutDir_plugin_z)trees.o

$(OutDir_plugin_z)uncompr.o: $(UPPDIR1)plugin/z/lib/uncompr.c \
	$(UPPDIR1)plugin/z/lib/zconf.h \
	$(UPPDIR1)plugin/z/lib/zlib.h
	$(CFLAGS) $(CINC) $(Macro_plugin_z)  $(UPPDIR1)plugin/z/lib/uncompr.c -o $(OutDir_plugin_z)uncompr.o

$(OutDir_plugin_z)zutil.o: $(UPPDIR1)plugin/z/lib/zutil.c \
	$(UPPDIR1)plugin/z/lib/zconf.h \
	$(UPPDIR1)plugin/z/lib/zlib.h \
	$(UPPDIR1)plugin/z/lib/zutil.h
	$(CFLAGS) $(CINC) $(Macro_plugin_z)  $(UPPDIR1)plugin/z/lib/zutil.c -o $(OutDir_plugin_z)zutil.o

$(OutDir_plugin_z)z.a: \
	$(OutDir_plugin_z)adler32.o \
	$(OutDir_plugin_z)compress.o \
	$(OutDir_plugin_z)crc32.o \
	$(OutDir_plugin_z)deflate.o \
	$(OutDir_plugin_z)gzio.o \
	$(OutDir_plugin_z)infblock.o \
	$(OutDir_plugin_z)infcodes.o \
	$(OutDir_plugin_z)inffast.o \
	$(OutDir_plugin_z)inflate.o \
	$(OutDir_plugin_z)inftrees.o \
	$(OutDir_plugin_z)infutil.o \
	$(OutDir_plugin_z)trees.o \
	$(OutDir_plugin_z)uncompr.o \
	$(OutDir_plugin_z)zutil.o
	$(AR) $(OutDir_plugin_z)z.a \
		$(OutDir_plugin_z)adler32.o \
		$(OutDir_plugin_z)compress.o \
		$(OutDir_plugin_z)crc32.o \
		$(OutDir_plugin_z)deflate.o \
		$(OutDir_plugin_z)gzio.o \
		$(OutDir_plugin_z)infblock.o \
		$(OutDir_plugin_z)infcodes.o \
		$(OutDir_plugin_z)inffast.o \
		$(OutDir_plugin_z)inflate.o \
		$(OutDir_plugin_z)inftrees.o \
		$(OutDir_plugin_z)infutil.o \
		$(OutDir_plugin_z)trees.o \
		$(OutDir_plugin_z)uncompr.o \
		$(OutDir_plugin_z)zutil.o
