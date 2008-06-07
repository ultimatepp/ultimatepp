# Microsoft Developer Studio Generated NMAKE File, Based on crash.dsp
!IF "$(CFG)" == ""
CFG=crash - Win32 Debug
!MESSAGE No configuration specified. Defaulting to crash - Win32 Debug.
!ENDIF

!IF "$(CFG)" != "crash - Win32 Release" && "$(CFG)" != "crash - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE
!MESSAGE NMAKE /f "crash.mak" CFG="crash - Win32 Debug"
!MESSAGE
!MESSAGE Possible choices for configuration are:
!MESSAGE
!MESSAGE "crash - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "crash - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE
!ERROR An invalid configuration is specified.
!ENDIF

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE
NULL=nul
!ENDIF

CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "crash - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release

!IF "$(RECURSE)" == "0"

ALL : "c:\tools\crash.exe"

!ELSE

ALL : "VLib - Win32 Release" "tlib - Win32 Release" "tgui - Win32 Release" "CtrlLib - Win32 Release" "c:\tools\crash.exe"

!ENDIF

!IF "$(RECURSE)" == "1"
CLEAN :"CtrlLib - Win32 ReleaseCLEAN" "tgui - Win32 ReleaseCLEAN" "tlib - Win32 ReleaseCLEAN" "VLib - Win32 ReleaseCLEAN"
!ELSE
CLEAN :
!ENDIF
	-@erase "$(INTDIR)\crash.obj"
	-@erase "$(INTDIR)\crash.res"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "c:\tools\crash.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MT /W3 /GR /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /Fp"$(INTDIR)\crash.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c
MTL_PROJ=/nologo /D "NDEBUG" /mktyplib203 /win32
RSC_PROJ=/l 0x405 /fo"$(INTDIR)\crash.res" /d "NDEBUG"
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\crash.bsc"
BSC32_SBRS= \

LINK32=link.exe
LINK32_FLAGS=/nologo /subsystem:windows /incremental:no /pdb:"$(OUTDIR)\crash.pdb" /machine:I386 /out:"c:/tools/crash.exe"
LINK32_OBJS= \
	"$(INTDIR)\crash.obj" \
	"$(INTDIR)\crash.res" \
	"..\CtrlLib\Release\CtrlLib.lib" \
	"E:\build\tgui\Release\tgui.lib" \
	"E:\build\tlib\Release\tlib.lib" \
	"..\VLib\Release\VLib.lib"

"c:\tools\crash.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "crash - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug
# Begin Custom Macros
OutDir=.\Debug
# End Custom Macros

!IF "$(RECURSE)" == "0"

ALL : "$(OUTDIR)\crash.exe"

!ELSE

ALL : "VLib - Win32 Debug" "tlib - Win32 Debug" "tgui - Win32 Debug" "CtrlLib - Win32 Debug" "$(OUTDIR)\crash.exe"

!ENDIF

!IF "$(RECURSE)" == "1"
CLEAN :"CtrlLib - Win32 DebugCLEAN" "tgui - Win32 DebugCLEAN" "tlib - Win32 DebugCLEAN" "VLib - Win32 DebugCLEAN"
!ELSE
CLEAN :
!ENDIF
	-@erase "$(INTDIR)\crash.obj"
	-@erase "$(INTDIR)\crash.res"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(OUTDIR)\crash.exe"
	-@erase "$(OUTDIR)\crash.ilk"
	-@erase "$(OUTDIR)\crash.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MTd /W3 /Gm /GR /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /Fp"$(INTDIR)\crash.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c
MTL_PROJ=/nologo /D "_DEBUG" /mktyplib203 /win32
RSC_PROJ=/l 0x405 /fo"$(INTDIR)\crash.res" /d "_DEBUG"
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\crash.bsc"
BSC32_SBRS= \

LINK32=link.exe
LINK32_FLAGS=/nologo /subsystem:windows /incremental:yes /pdb:"$(OUTDIR)\crash.pdb" /debug /machine:I386 /out:"$(OUTDIR)\crash.exe" /pdbtype:sept
LINK32_OBJS= \
	"$(INTDIR)\crash.obj" \
	"$(INTDIR)\crash.res" \
	"..\CtrlLib\Debug\CtrlLib.lib" \
	"E:\build\tgui\Debug\tgui.lib" \
	"E:\build\tlib\Debug\tlib.lib" \
	"..\VLib\Debug\VLib.lib"

"$(OUTDIR)\crash.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ENDIF

.c{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $<
<<

.cpp{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $<
<<

.cxx{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $<
<<

.c{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $<
<<

.cpp{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $<
<<

.cxx{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $<
<<


!IF "$(NO_EXTERNAL_DEPS)" != "1"
!IF EXISTS("crash.dep")
!INCLUDE "crash.dep"
!ELSE
!MESSAGE Warning: cannot find "crash.dep"
!ENDIF
!ENDIF


!IF "$(CFG)" == "crash - Win32 Release" || "$(CFG)" == "crash - Win32 Debug"
SOURCE=.\crash.cpp

"$(INTDIR)\crash.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\crash.rc

"$(INTDIR)\crash.res" : $(SOURCE) "$(INTDIR)"
	$(RSC) $(RSC_PROJ) $(SOURCE)


!IF  "$(CFG)" == "crash - Win32 Release"

"CtrlLib - Win32 Release" :
   cd "\SOURCE\CtrlLib"
   $(MAKE) /$(MAKEFLAGS) /F .\CtrlLib.mak CFG="CtrlLib - Win32 Release"
   cd "..\crash"

"CtrlLib - Win32 ReleaseCLEAN" :
   cd "\SOURCE\CtrlLib"
   $(MAKE) /$(MAKEFLAGS) /F .\CtrlLib.mak CFG="CtrlLib - Win32 Release" RECURSE=1 CLEAN
   cd "..\crash"

!ELSEIF  "$(CFG)" == "crash - Win32 Debug"

"CtrlLib - Win32 Debug" :
   cd "\SOURCE\CtrlLib"
   $(MAKE) /$(MAKEFLAGS) /F .\CtrlLib.mak CFG="CtrlLib - Win32 Debug"
   cd "..\crash"

"CtrlLib - Win32 DebugCLEAN" :
   cd "\SOURCE\CtrlLib"
   $(MAKE) /$(MAKEFLAGS) /F .\CtrlLib.mak CFG="CtrlLib - Win32 Debug" RECURSE=1 CLEAN
   cd "..\crash"

!ENDIF

!IF  "$(CFG)" == "crash - Win32 Release"

"tgui - Win32 Release" :
   E:
   cd "E:\V\TLIB"
   $(MAKE) /$(MAKEFLAGS) /F .\tgui.mak CFG="tgui - Win32 Release"
   F:
   cd "F:\SOURCE\crash"

"tgui - Win32 ReleaseCLEAN" :
   E:
   cd "E:\V\TLIB"
   $(MAKE) /$(MAKEFLAGS) /F .\tgui.mak CFG="tgui - Win32 Release" RECURSE=1 CLEAN
   F:
   cd "F:\SOURCE\crash"

!ELSEIF  "$(CFG)" == "crash - Win32 Debug"

"tgui - Win32 Debug" :
   E:
   cd "E:\V\TLIB"
   $(MAKE) /$(MAKEFLAGS) /F .\tgui.mak CFG="tgui - Win32 Debug"
   F:
   cd "F:\SOURCE\crash"

"tgui - Win32 DebugCLEAN" :
   E:
   cd "E:\V\TLIB"
   $(MAKE) /$(MAKEFLAGS) /F .\tgui.mak CFG="tgui - Win32 Debug" RECURSE=1 CLEAN
   F:
   cd "F:\SOURCE\crash"

!ENDIF

!IF  "$(CFG)" == "crash - Win32 Release"

"tlib - Win32 Release" :
   E:
   cd "E:\V\TLIB"
   $(MAKE) /$(MAKEFLAGS) /F .\tlib.mak CFG="tlib - Win32 Release"
   F:
   cd "F:\SOURCE\crash"

"tlib - Win32 ReleaseCLEAN" :
   E:
   cd "E:\V\TLIB"
   $(MAKE) /$(MAKEFLAGS) /F .\tlib.mak CFG="tlib - Win32 Release" RECURSE=1 CLEAN
   F:
   cd "F:\SOURCE\crash"

!ELSEIF  "$(CFG)" == "crash - Win32 Debug"

"tlib - Win32 Debug" :
   E:
   cd "E:\V\TLIB"
   $(MAKE) /$(MAKEFLAGS) /F .\tlib.mak CFG="tlib - Win32 Debug"
   F:
   cd "F:\SOURCE\crash"

"tlib - Win32 DebugCLEAN" :
   E:
   cd "E:\V\TLIB"
   $(MAKE) /$(MAKEFLAGS) /F .\tlib.mak CFG="tlib - Win32 Debug" RECURSE=1 CLEAN
   F:
   cd "F:\SOURCE\crash"

!ENDIF

!IF  "$(CFG)" == "crash - Win32 Release"

"VLib - Win32 Release" :
   cd "\SOURCE\VLib"
   $(MAKE) /$(MAKEFLAGS) /F .\VLib.mak CFG="VLib - Win32 Release"
   cd "..\crash"

"VLib - Win32 ReleaseCLEAN" :
   cd "\SOURCE\VLib"
   $(MAKE) /$(MAKEFLAGS) /F .\VLib.mak CFG="VLib - Win32 Release" RECURSE=1 CLEAN
   cd "..\crash"

!ELSEIF  "$(CFG)" == "crash - Win32 Debug"

"VLib - Win32 Debug" :
   cd "\SOURCE\VLib"
   $(MAKE) /$(MAKEFLAGS) /F .\VLib.mak CFG="VLib - Win32 Debug"
   cd "..\crash"

"VLib - Win32 DebugCLEAN" :
   cd "\SOURCE\VLib"
   $(MAKE) /$(MAKEFLAGS) /F .\VLib.mak CFG="VLib - Win32 Debug" RECURSE=1 CLEAN
   cd "..\crash"

!ENDIF


!ENDIF
