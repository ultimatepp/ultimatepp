UPPDIR1 = c:\\MyApps\\

UPPOUT = C:\\MyApps\\out\\
CINC =  -I$(UPPDIR1) -IC:\mingw\include
Macro =  -DflagMT -DflagSTUDENT_VER -DflagMAIN -DflagGCC -DflagDEBUG -DflagSO -DflagSHARED -DflagDEBUG_FULL -DflagWIN32
CC = c++ -c -D_DEBUG -O0 -ggdb -g2
CFLAGS = $(CC) -x c
CPPFLAGS = $(CC) -x c++
LIBPATH =  -L"C:\\mingw\\lib"
AR = ar -sr
OutDir_TestXML = $(UPPOUT)TestXML\\MINGW-Debug-Debug_full-Gcc-Main-Mt-Shared-So-Student_ver-Win32\\
Macro_TestXML = $(Macro)

OutDir = $(OutDir_TestXML)
OutFile = $(OutDir)TestXML.exe

.PHONY: all
all: install $(OutFile)

.PHONY: install
install:
	-mkdir -p $(OutDir)
	-mkdir -p $(OutDir_TestXML)

$(OutFile):  \
	$(OutDir_TestXML)TestXML.o
	c++ -mwindows -o $(OutFile) -ggdb $(LIBPATH) -Wl,-O,2 $(LINKOPTIONS) \
		$(OutDir_TestXML)TestXML.o \
		-Wl,--start-group  \
		-Wl,--end-group


$(OutDir_TestXML)TestXML.o: $(UPPDIR1)TestXML\\TestXML.cpp
	$(CPPFLAGS) $(CINC) $(Macro_TestXML)  $(UPPDIR1)TestXML\\TestXML.cpp -o $(OutDir_TestXML)TestXML.o
