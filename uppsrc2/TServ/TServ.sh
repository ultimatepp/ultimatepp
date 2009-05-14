# Project modules: Web\TServ Web TCore Core plugin\bz2 plugin
# Configurations:
mkdir -p /uppout/uppnew/Web/CONSOLE-GNU3-IA32-LINUX-RELEASE-ST
echo /src/uppsrc/Web/util.cpp
/usr/local/bin/g++ -c -O2 -ffunction-sections -x c++ -fuse-cxa-atexit -I"/src/uppsrc" -DflagCONSOLE -DflagST -DflagLINUX -DflagGNU3 -DflagIA32 -DflagRELEASE   /src/uppsrc/Web/util.cpp -o /uppout/uppnew/Web/CONSOLE-GNU3-IA32-LINUX-RELEASE-ST/util.o
echo /src/uppsrc/Web/html.cpp
/usr/local/bin/g++ -c -O2 -ffunction-sections -x c++ -fuse-cxa-atexit -I"/src/uppsrc" -DflagCONSOLE -DflagST -DflagLINUX -DflagGNU3 -DflagIA32 -DflagRELEASE   /src/uppsrc/Web/html.cpp -o /uppout/uppnew/Web/CONSOLE-GNU3-IA32-LINUX-RELEASE-ST/html.o
echo /src/uppsrc/Web/socket.cpp
/usr/local/bin/g++ -c -O2 -ffunction-sections -x c++ -fuse-cxa-atexit -I"/src/uppsrc" -DflagCONSOLE -DflagST -DflagLINUX -DflagGNU3 -DflagIA32 -DflagRELEASE   /src/uppsrc/Web/socket.cpp -o /uppout/uppnew/Web/CONSOLE-GNU3-IA32-LINUX-RELEASE-ST/socket.o
echo /src/uppsrc/Web/http.cpp
/usr/local/bin/g++ -c -O2 -ffunction-sections -x c++ -fuse-cxa-atexit -I"/src/uppsrc" -DflagCONSOLE -DflagST -DflagLINUX -DflagGNU3 -DflagIA32 -DflagRELEASE   /src/uppsrc/Web/http.cpp -o /uppout/uppnew/Web/CONSOLE-GNU3-IA32-LINUX-RELEASE-ST/http.o
echo /src/uppsrc/Web/auth.cpp
/usr/local/bin/g++ -c -O2 -ffunction-sections -x c++ -fuse-cxa-atexit -I"/src/uppsrc" -DflagCONSOLE -DflagST -DflagLINUX -DflagGNU3 -DflagIA32 -DflagRELEASE   /src/uppsrc/Web/auth.cpp -o /uppout/uppnew/Web/CONSOLE-GNU3-IA32-LINUX-RELEASE-ST/auth.o
echo /src/uppsrc/Web/smtp.cpp
/usr/local/bin/g++ -c -O2 -ffunction-sections -x c++ -fuse-cxa-atexit -I"/src/uppsrc" -DflagCONSOLE -DflagST -DflagLINUX -DflagGNU3 -DflagIA32 -DflagRELEASE   /src/uppsrc/Web/smtp.cpp -o /uppout/uppnew/Web/CONSOLE-GNU3-IA32-LINUX-RELEASE-ST/smtp.o
echo /src/uppsrc/Web/sproc.cpp
/usr/local/bin/g++ -c -O2 -ffunction-sections -x c++ -fuse-cxa-atexit -I"/src/uppsrc" -DflagCONSOLE -DflagST -DflagLINUX -DflagGNU3 -DflagIA32 -DflagRELEASE   /src/uppsrc/Web/sproc.cpp -o /uppout/uppnew/Web/CONSOLE-GNU3-IA32-LINUX-RELEASE-ST/sproc.o
/usr/local/bin/ar -sr /uppout/uppnew/Web/CONSOLE-GNU3-IA32-LINUX-RELEASE-ST/Web.a /uppout/uppnew/Web/CONSOLE-GNU3-IA32-LINUX-RELEASE-ST/util.o /uppout/uppnew/Web/CONSOLE-GNU3-IA32-LINUX-RELEASE-ST/html.o /uppout/uppnew/Web/CONSOLE-GNU3-IA32-LINUX-RELEASE-ST/socket.o /uppout/uppnew/Web/CONSOLE-GNU3-IA32-LINUX-RELEASE-ST/http.o /uppout/uppnew/Web/CONSOLE-GNU3-IA32-LINUX-RELEASE-ST/auth.o /uppout/uppnew/Web/CONSOLE-GNU3-IA32-LINUX-RELEASE-ST/smtp.o /uppout/uppnew/Web/CONSOLE-GNU3-IA32-LINUX-RELEASE-ST/sproc.o
mkdir -p /uppout/uppnew/TCore/CONSOLE-GNU3-IA32-LINUX-RELEASE-ST
echo /src/uppsrc/TCore/util.cpp
/usr/local/bin/g++ -c -O2 -ffunction-sections -x c++ -fuse-cxa-atexit -I"/src/uppsrc" -DflagCONSOLE -DflagST -DflagLINUX -DflagGNU3 -DflagIA32 -DflagRELEASE   /src/uppsrc/TCore/util.cpp -o /uppout/uppnew/TCore/CONSOLE-GNU3-IA32-LINUX-RELEASE-ST/util.o
echo /src/uppsrc/TCore/dbf.cpp
/usr/local/bin/g++ -c -O2 -ffunction-sections -x c++ -fuse-cxa-atexit -I"/src/uppsrc" -DflagCONSOLE -DflagST -DflagLINUX -DflagGNU3 -DflagIA32 -DflagRELEASE   /src/uppsrc/TCore/dbf.cpp -o /uppout/uppnew/TCore/CONSOLE-GNU3-IA32-LINUX-RELEASE-ST/dbf.o
echo /src/uppsrc/TCore/globcfg.cpp
/usr/local/bin/g++ -c -O2 -ffunction-sections -x c++ -fuse-cxa-atexit -I"/src/uppsrc" -DflagCONSOLE -DflagST -DflagLINUX -DflagGNU3 -DflagIA32 -DflagRELEASE   /src/uppsrc/TCore/globcfg.cpp -o /uppout/uppnew/TCore/CONSOLE-GNU3-IA32-LINUX-RELEASE-ST/globcfg.o
echo /src/uppsrc/TCore/atexit.cpp
/usr/local/bin/g++ -c -O2 -ffunction-sections -x c++ -fuse-cxa-atexit -I"/src/uppsrc" -DflagCONSOLE -DflagST -DflagLINUX -DflagGNU3 -DflagIA32 -DflagRELEASE   /src/uppsrc/TCore/atexit.cpp -o /uppout/uppnew/TCore/CONSOLE-GNU3-IA32-LINUX-RELEASE-ST/atexit.o
echo /src/uppsrc/TCore/help.cpp
/usr/local/bin/g++ -c -O2 -ffunction-sections -x c++ -fuse-cxa-atexit -I"/src/uppsrc" -DflagCONSOLE -DflagST -DflagLINUX -DflagGNU3 -DflagIA32 -DflagRELEASE   /src/uppsrc/TCore/help.cpp -o /uppout/uppnew/TCore/CONSOLE-GNU3-IA32-LINUX-RELEASE-ST/help.o
echo /src/uppsrc/TCore/CalcType.cpp
/usr/local/bin/g++ -c -O2 -ffunction-sections -x c++ -fuse-cxa-atexit -I"/src/uppsrc" -DflagCONSOLE -DflagST -DflagLINUX -DflagGNU3 -DflagIA32 -DflagRELEASE   /src/uppsrc/TCore/CalcType.cpp -o /uppout/uppnew/TCore/CONSOLE-GNU3-IA32-LINUX-RELEASE-ST/CalcType.o
echo /src/uppsrc/TCore/CalcNode.cpp
/usr/local/bin/g++ -c -O2 -ffunction-sections -x c++ -fuse-cxa-atexit -I"/src/uppsrc" -DflagCONSOLE -DflagST -DflagLINUX -DflagGNU3 -DflagIA32 -DflagRELEASE   /src/uppsrc/TCore/CalcNode.cpp -o /uppout/uppnew/TCore/CONSOLE-GNU3-IA32-LINUX-RELEASE-ST/CalcNode.o
echo /src/uppsrc/TCore/CalcBasic.cpp
/usr/local/bin/g++ -c -O2 -ffunction-sections -x c++ -fuse-cxa-atexit -I"/src/uppsrc" -DflagCONSOLE -DflagST -DflagLINUX -DflagGNU3 -DflagIA32 -DflagRELEASE   /src/uppsrc/TCore/CalcBasic.cpp -o /uppout/uppnew/TCore/CONSOLE-GNU3-IA32-LINUX-RELEASE-ST/CalcBasic.o
echo /src/uppsrc/TCore/datafile.cpp
/usr/local/bin/g++ -c -O2 -ffunction-sections -x c++ -fuse-cxa-atexit -I"/src/uppsrc" -DflagCONSOLE -DflagST -DflagLINUX -DflagGNU3 -DflagIA32 -DflagRELEASE   /src/uppsrc/TCore/datafile.cpp -o /uppout/uppnew/TCore/CONSOLE-GNU3-IA32-LINUX-RELEASE-ST/datafile.o
echo /src/uppsrc/TCore/database.cpp
/usr/local/bin/g++ -c -O2 -ffunction-sections -x c++ -fuse-cxa-atexit -I"/src/uppsrc" -DflagCONSOLE -DflagST -DflagLINUX -DflagGNU3 -DflagIA32 -DflagRELEASE   /src/uppsrc/TCore/database.cpp -o /uppout/uppnew/TCore/CONSOLE-GNU3-IA32-LINUX-RELEASE-ST/database.o
echo /src/uppsrc/TCore/datatest.cpp
/usr/local/bin/g++ -c -O2 -ffunction-sections -x c++ -fuse-cxa-atexit -I"/src/uppsrc" -DflagCONSOLE -DflagST -DflagLINUX -DflagGNU3 -DflagIA32 -DflagRELEASE   /src/uppsrc/TCore/datatest.cpp -o /uppout/uppnew/TCore/CONSOLE-GNU3-IA32-LINUX-RELEASE-ST/datatest.o
/usr/local/bin/ar -sr /uppout/uppnew/TCore/CONSOLE-GNU3-IA32-LINUX-RELEASE-ST/TCore.a /uppout/uppnew/TCore/CONSOLE-GNU3-IA32-LINUX-RELEASE-ST/util.o /uppout/uppnew/TCore/CONSOLE-GNU3-IA32-LINUX-RELEASE-ST/dbf.o /uppout/uppnew/TCore/CONSOLE-GNU3-IA32-LINUX-RELEASE-ST/globcfg.o /uppout/uppnew/TCore/CONSOLE-GNU3-IA32-LINUX-RELEASE-ST/atexit.o /uppout/uppnew/TCore/CONSOLE-GNU3-IA32-LINUX-RELEASE-ST/help.o /uppout/uppnew/TCore/CONSOLE-GNU3-IA32-LINUX-RELEASE-ST/CalcType.o /uppout/uppnew/TCore/CONSOLE-GNU3-IA32-LINUX-RELEASE-ST/CalcNode.o /uppout/uppnew/TCore/CONSOLE-GNU3-IA32-LINUX-RELEASE-ST/CalcBasic.o /uppout/uppnew/TCore/CONSOLE-GNU3-IA32-LINUX-RELEASE-ST/datafile.o /uppout/uppnew/TCore/CONSOLE-GNU3-IA32-LINUX-RELEASE-ST/database.o /uppout/uppnew/TCore/CONSOLE-GNU3-IA32-LINUX-RELEASE-ST/datatest.o
mkdir -p /uppout/uppnew/Core/CONSOLE-GNU3-IA32-LINUX-RELEASE-ST
echo /src/uppsrc/Core/Mt.cpp
/usr/local/bin/g++ -c -O2 -ffunction-sections -x c++ -fuse-cxa-atexit -I"/src/uppsrc" -DflagCONSOLE -DflagST -DflagLINUX -DflagGNU3 -DflagIA32 -DflagRELEASE   /src/uppsrc/Core/Mt.cpp -o /uppout/uppnew/Core/CONSOLE-GNU3-IA32-LINUX-RELEASE-ST/Mt.o
echo /src/uppsrc/Core/Thread.cpp
/usr/local/bin/g++ -c -O2 -ffunction-sections -x c++ -fuse-cxa-atexit -I"/src/uppsrc" -DflagCONSOLE -DflagST -DflagLINUX -DflagGNU3 -DflagIA32 -DflagRELEASE   /src/uppsrc/Core/Thread.cpp -o /uppout/uppnew/Core/CONSOLE-GNU3-IA32-LINUX-RELEASE-ST/Thread.o
echo /src/uppsrc/Core/heap.cpp
/usr/local/bin/g++ -c -O2 -ffunction-sections -x c++ -fuse-cxa-atexit -I"/src/uppsrc" -DflagCONSOLE -DflagST -DflagLINUX -DflagGNU3 -DflagIA32 -DflagRELEASE   /src/uppsrc/Core/heap.cpp -o /uppout/uppnew/Core/CONSOLE-GNU3-IA32-LINUX-RELEASE-ST/heap.o
echo /src/uppsrc/Core/nheap.cpp
/usr/local/bin/g++ -c -O2 -ffunction-sections -x c++ -fuse-cxa-atexit -I"/src/uppsrc" -DflagCONSOLE -DflagST -DflagLINUX -DflagGNU3 -DflagIA32 -DflagRELEASE   /src/uppsrc/Core/nheap.cpp -o /uppout/uppnew/Core/CONSOLE-GNU3-IA32-LINUX-RELEASE-ST/nheap.o
echo /src/uppsrc/Core/OString.cpp
/usr/local/bin/g++ -c -O2 -ffunction-sections -x c++ -fuse-cxa-atexit -I"/src/uppsrc" -DflagCONSOLE -DflagST -DflagLINUX -DflagGNU3 -DflagIA32 -DflagRELEASE   /src/uppsrc/Core/OString.cpp -o /uppout/uppnew/Core/CONSOLE-GNU3-IA32-LINUX-RELEASE-ST/OString.o
echo /src/uppsrc/Core/OWString.cpp
/usr/local/bin/g++ -c -O2 -ffunction-sections -x c++ -fuse-cxa-atexit -I"/src/uppsrc" -DflagCONSOLE -DflagST -DflagLINUX -DflagGNU3 -DflagIA32 -DflagRELEASE   /src/uppsrc/Core/OWString.cpp -o /uppout/uppnew/Core/CONSOLE-GNU3-IA32-LINUX-RELEASE-ST/OWString.o
echo /src/uppsrc/Core/String.cpp
/usr/local/bin/g++ -c -O2 -ffunction-sections -x c++ -fuse-cxa-atexit -I"/src/uppsrc" -DflagCONSOLE -DflagST -DflagLINUX -DflagGNU3 -DflagIA32 -DflagRELEASE   /src/uppsrc/Core/String.cpp -o /uppout/uppnew/Core/CONSOLE-GNU3-IA32-LINUX-RELEASE-ST/String.o
echo /src/uppsrc/Core/StrUtil.cpp
/usr/local/bin/g++ -c -O2 -ffunction-sections -x c++ -fuse-cxa-atexit -I"/src/uppsrc" -DflagCONSOLE -DflagST -DflagLINUX -DflagGNU3 -DflagIA32 -DflagRELEASE   /src/uppsrc/Core/StrUtil.cpp -o /uppout/uppnew/Core/CONSOLE-GNU3-IA32-LINUX-RELEASE-ST/StrUtil.o
echo /src/uppsrc/Core/CharSet.cpp
/usr/local/bin/g++ -c -O2 -ffunction-sections -x c++ -fuse-cxa-atexit -I"/src/uppsrc" -DflagCONSOLE -DflagST -DflagLINUX -DflagGNU3 -DflagIA32 -DflagRELEASE   /src/uppsrc/Core/CharSet.cpp -o /uppout/uppnew/Core/CONSOLE-GNU3-IA32-LINUX-RELEASE-ST/CharSet.o
echo /src/uppsrc/Core/Path.cpp
/usr/local/bin/g++ -c -O2 -ffunction-sections -x c++ -fuse-cxa-atexit -I"/src/uppsrc" -DflagCONSOLE -DflagST -DflagLINUX -DflagGNU3 -DflagIA32 -DflagRELEASE   /src/uppsrc/Core/Path.cpp -o /uppout/uppnew/Core/CONSOLE-GNU3-IA32-LINUX-RELEASE-ST/Path.o
echo /src/uppsrc/Core/App.cpp
/usr/local/bin/g++ -c -O2 -ffunction-sections -x c++ -fuse-cxa-atexit -I"/src/uppsrc" -DflagCONSOLE -DflagST -DflagLINUX -DflagGNU3 -DflagIA32 -DflagRELEASE   /src/uppsrc/Core/App.cpp -o /uppout/uppnew/Core/CONSOLE-GNU3-IA32-LINUX-RELEASE-ST/App.o
echo /src/uppsrc/Core/Stream.cpp
/usr/local/bin/g++ -c -O2 -ffunction-sections -x c++ -fuse-cxa-atexit -I"/src/uppsrc" -DflagCONSOLE -DflagST -DflagLINUX -DflagGNU3 -DflagIA32 -DflagRELEASE   /src/uppsrc/Core/Stream.cpp -o /uppout/uppnew/Core/CONSOLE-GNU3-IA32-LINUX-RELEASE-ST/Stream.o
echo /src/uppsrc/Core/Debug.cpp
/usr/local/bin/g++ -c -O2 -ffunction-sections -x c++ -fuse-cxa-atexit -I"/src/uppsrc" -DflagCONSOLE -DflagST -DflagLINUX -DflagGNU3 -DflagIA32 -DflagRELEASE   /src/uppsrc/Core/Debug.cpp -o /uppout/uppnew/Core/CONSOLE-GNU3-IA32-LINUX-RELEASE-ST/Debug.o
echo /src/uppsrc/Core/Util.cpp
/usr/local/bin/g++ -c -O2 -ffunction-sections -x c++ -fuse-cxa-atexit -I"/src/uppsrc" -DflagCONSOLE -DflagST -DflagLINUX -DflagGNU3 -DflagIA32 -DflagRELEASE   /src/uppsrc/Core/Util.cpp -o /uppout/uppnew/Core/CONSOLE-GNU3-IA32-LINUX-RELEASE-ST/Util.o
echo /src/uppsrc/Core/mathutil.cpp
/usr/local/bin/g++ -c -O2 -ffunction-sections -x c++ -fuse-cxa-atexit -I"/src/uppsrc" -DflagCONSOLE -DflagST -DflagLINUX -DflagGNU3 -DflagIA32 -DflagRELEASE   /src/uppsrc/Core/mathutil.cpp -o /uppout/uppnew/Core/CONSOLE-GNU3-IA32-LINUX-RELEASE-ST/mathutil.o
echo /src/uppsrc/Core/Cont.cpp
/usr/local/bin/g++ -c -O2 -ffunction-sections -x c++ -fuse-cxa-atexit -I"/src/uppsrc" -DflagCONSOLE -DflagST -DflagLINUX -DflagGNU3 -DflagIA32 -DflagRELEASE   /src/uppsrc/Core/Cont.cpp -o /uppout/uppnew/Core/CONSOLE-GNU3-IA32-LINUX-RELEASE-ST/Cont.o
echo /src/uppsrc/Core/Callback.cpp
/usr/local/bin/g++ -c -O2 -ffunction-sections -x c++ -fuse-cxa-atexit -I"/src/uppsrc" -DflagCONSOLE -DflagST -DflagLINUX -DflagGNU3 -DflagIA32 -DflagRELEASE   /src/uppsrc/Core/Callback.cpp -o /uppout/uppnew/Core/CONSOLE-GNU3-IA32-LINUX-RELEASE-ST/Callback.o
echo /src/uppsrc/Core/Color.cpp
/usr/local/bin/g++ -c -O2 -ffunction-sections -x c++ -fuse-cxa-atexit -I"/src/uppsrc" -DflagCONSOLE -DflagST -DflagLINUX -DflagGNU3 -DflagIA32 -DflagRELEASE   /src/uppsrc/Core/Color.cpp -o /uppout/uppnew/Core/CONSOLE-GNU3-IA32-LINUX-RELEASE-ST/Color.o
echo /src/uppsrc/Core/Gtypes.cpp
/usr/local/bin/g++ -c -O2 -ffunction-sections -x c++ -fuse-cxa-atexit -I"/src/uppsrc" -DflagCONSOLE -DflagST -DflagLINUX -DflagGNU3 -DflagIA32 -DflagRELEASE   /src/uppsrc/Core/Gtypes.cpp -o /uppout/uppnew/Core/CONSOLE-GNU3-IA32-LINUX-RELEASE-ST/Gtypes.o
echo /src/uppsrc/Core/TimeDate.cpp
/usr/local/bin/g++ -c -O2 -ffunction-sections -x c++ -fuse-cxa-atexit -I"/src/uppsrc" -DflagCONSOLE -DflagST -DflagLINUX -DflagGNU3 -DflagIA32 -DflagRELEASE   /src/uppsrc/Core/TimeDate.cpp -o /uppout/uppnew/Core/CONSOLE-GNU3-IA32-LINUX-RELEASE-ST/TimeDate.o
echo /src/uppsrc/Core/Value.cpp
/usr/local/bin/g++ -c -O2 -ffunction-sections -x c++ -fuse-cxa-atexit -I"/src/uppsrc" -DflagCONSOLE -DflagST -DflagLINUX -DflagGNU3 -DflagIA32 -DflagRELEASE   /src/uppsrc/Core/Value.cpp -o /uppout/uppnew/Core/CONSOLE-GNU3-IA32-LINUX-RELEASE-ST/Value.o
echo /src/uppsrc/Core/Convert.cpp
/usr/local/bin/g++ -c -O2 -ffunction-sections -x c++ -fuse-cxa-atexit -I"/src/uppsrc" -DflagCONSOLE -DflagST -DflagLINUX -DflagGNU3 -DflagIA32 -DflagRELEASE   /src/uppsrc/Core/Convert.cpp -o /uppout/uppnew/Core/CONSOLE-GNU3-IA32-LINUX-RELEASE-ST/Convert.o
echo /src/uppsrc/Core/Format.cpp
/usr/local/bin/g++ -c -O2 -ffunction-sections -x c++ -fuse-cxa-atexit -I"/src/uppsrc" -DflagCONSOLE -DflagST -DflagLINUX -DflagGNU3 -DflagIA32 -DflagRELEASE   /src/uppsrc/Core/Format.cpp -o /uppout/uppnew/Core/CONSOLE-GNU3-IA32-LINUX-RELEASE-ST/Format.o
echo /src/uppsrc/Core/Lang.cpp
/usr/local/bin/g++ -c -O2 -ffunction-sections -x c++ -fuse-cxa-atexit -I"/src/uppsrc" -DflagCONSOLE -DflagST -DflagLINUX -DflagGNU3 -DflagIA32 -DflagRELEASE   /src/uppsrc/Core/Lang.cpp -o /uppout/uppnew/Core/CONSOLE-GNU3-IA32-LINUX-RELEASE-ST/Lang.o
echo /src/uppsrc/Core/parser.cpp
/usr/local/bin/g++ -c -O2 -ffunction-sections -x c++ -fuse-cxa-atexit -I"/src/uppsrc" -DflagCONSOLE -DflagST -DflagLINUX -DflagGNU3 -DflagIA32 -DflagRELEASE   /src/uppsrc/Core/parser.cpp -o /uppout/uppnew/Core/CONSOLE-GNU3-IA32-LINUX-RELEASE-ST/parser.o
echo /src/uppsrc/Core/Uuid.cpp
/usr/local/bin/g++ -c -O2 -ffunction-sections -x c++ -fuse-cxa-atexit -I"/src/uppsrc" -DflagCONSOLE -DflagST -DflagLINUX -DflagGNU3 -DflagIA32 -DflagRELEASE   /src/uppsrc/Core/Uuid.cpp -o /uppout/uppnew/Core/CONSOLE-GNU3-IA32-LINUX-RELEASE-ST/Uuid.o
echo /src/uppsrc/Core/Ptr.cpp
/usr/local/bin/g++ -c -O2 -ffunction-sections -x c++ -fuse-cxa-atexit -I"/src/uppsrc" -DflagCONSOLE -DflagST -DflagLINUX -DflagGNU3 -DflagIA32 -DflagRELEASE   /src/uppsrc/Core/Ptr.cpp -o /uppout/uppnew/Core/CONSOLE-GNU3-IA32-LINUX-RELEASE-ST/Ptr.o
echo /src/uppsrc/Core/Win32Com.cpp
/usr/local/bin/g++ -c -O2 -ffunction-sections -x c++ -fuse-cxa-atexit -I"/src/uppsrc" -DflagCONSOLE -DflagST -DflagLINUX -DflagGNU3 -DflagIA32 -DflagRELEASE   /src/uppsrc/Core/Win32Com.cpp -o /uppout/uppnew/Core/CONSOLE-GNU3-IA32-LINUX-RELEASE-ST/Win32Com.o
echo /src/uppsrc/Core/Dli.cpp
/usr/local/bin/g++ -c -O2 -ffunction-sections -x c++ -fuse-cxa-atexit -I"/src/uppsrc" -DflagCONSOLE -DflagST -DflagLINUX -DflagGNU3 -DflagIA32 -DflagRELEASE   /src/uppsrc/Core/Dli.cpp -o /uppout/uppnew/Core/CONSOLE-GNU3-IA32-LINUX-RELEASE-ST/Dli.o
echo /src/uppsrc/Core/Test.cpp
/usr/local/bin/g++ -c -O2 -ffunction-sections -x c++ -fuse-cxa-atexit -I"/src/uppsrc" -DflagCONSOLE -DflagST -DflagLINUX -DflagGNU3 -DflagIA32 -DflagRELEASE   /src/uppsrc/Core/Test.cpp -o /uppout/uppnew/Core/CONSOLE-GNU3-IA32-LINUX-RELEASE-ST/Test.o
/usr/local/bin/ar -sr /uppout/uppnew/Core/CONSOLE-GNU3-IA32-LINUX-RELEASE-ST/Core.a /uppout/uppnew/Core/CONSOLE-GNU3-IA32-LINUX-RELEASE-ST/Mt.o /uppout/uppnew/Core/CONSOLE-GNU3-IA32-LINUX-RELEASE-ST/Thread.o /uppout/uppnew/Core/CONSOLE-GNU3-IA32-LINUX-RELEASE-ST/heap.o /uppout/uppnew/Core/CONSOLE-GNU3-IA32-LINUX-RELEASE-ST/nheap.o /uppout/uppnew/Core/CONSOLE-GNU3-IA32-LINUX-RELEASE-ST/OString.o /uppout/uppnew/Core/CONSOLE-GNU3-IA32-LINUX-RELEASE-ST/OWString.o /uppout/uppnew/Core/CONSOLE-GNU3-IA32-LINUX-RELEASE-ST/String.o /uppout/uppnew/Core/CONSOLE-GNU3-IA32-LINUX-RELEASE-ST/StrUtil.o /uppout/uppnew/Core/CONSOLE-GNU3-IA32-LINUX-RELEASE-ST/CharSet.o /uppout/uppnew/Core/CONSOLE-GNU3-IA32-LINUX-RELEASE-ST/Path.o /uppout/uppnew/Core/CONSOLE-GNU3-IA32-LINUX-RELEASE-ST/App.o /uppout/uppnew/Core/CONSOLE-GNU3-IA32-LINUX-RELEASE-ST/Stream.o /uppout/uppnew/Core/CONSOLE-GNU3-IA32-LINUX-RELEASE-ST/Debug.o /uppout/uppnew/Core/CONSOLE-GNU3-IA32-LINUX-RELEASE-ST/Util.o /uppout/uppnew/Core/CONSOLE-GNU3-IA32-LINUX-RELEASE-ST/mathutil.o /uppout/uppnew/Core/CONSOLE-GNU3-IA32-LINUX-RELEASE-ST/Cont.o /uppout/uppnew/Core/CONSOLE-GNU3-IA32-LINUX-RELEASE-ST/Callback.o /uppout/uppnew/Core/CONSOLE-GNU3-IA32-LINUX-RELEASE-ST/Color.o /uppout/uppnew/Core/CONSOLE-GNU3-IA32-LINUX-RELEASE-ST/Gtypes.o /uppout/uppnew/Core/CONSOLE-GNU3-IA32-LINUX-RELEASE-ST/TimeDate.o /uppout/uppnew/Core/CONSOLE-GNU3-IA32-LINUX-RELEASE-ST/Value.o /uppout/uppnew/Core/CONSOLE-GNU3-IA32-LINUX-RELEASE-ST/Convert.o /uppout/uppnew/Core/CONSOLE-GNU3-IA32-LINUX-RELEASE-ST/Format.o /uppout/uppnew/Core/CONSOLE-GNU3-IA32-LINUX-RELEASE-ST/Lang.o /uppout/uppnew/Core/CONSOLE-GNU3-IA32-LINUX-RELEASE-ST/parser.o /uppout/uppnew/Core/CONSOLE-GNU3-IA32-LINUX-RELEASE-ST/Uuid.o /uppout/uppnew/Core/CONSOLE-GNU3-IA32-LINUX-RELEASE-ST/Ptr.o /uppout/uppnew/Core/CONSOLE-GNU3-IA32-LINUX-RELEASE-ST/Win32Com.o /uppout/uppnew/Core/CONSOLE-GNU3-IA32-LINUX-RELEASE-ST/Dli.o /uppout/uppnew/Core/CONSOLE-GNU3-IA32-LINUX-RELEASE-ST/Test.o
mkdir -p /uppout/uppnew/plugin/bz2/CONSOLE-GNU3-IA32-LINUX-RELEASE-ST
echo /src/uppsrc/plugin/bz2/bz2upp.cpp
/usr/local/bin/g++ -c -O2 -ffunction-sections -x c++ -fuse-cxa-atexit -I"/src/uppsrc" -DflagCONSOLE -DflagST -DflagLINUX -DflagGNU3 -DflagIA32 -DflagRELEASE   /src/uppsrc/plugin/bz2/bz2upp.cpp -o /uppout/uppnew/plugin/bz2/CONSOLE-GNU3-IA32-LINUX-RELEASE-ST/bz2upp.o
echo /src/uppsrc/plugin/bz2/blocksort.c
/usr/local/bin/g++ -c -O2 -ffunction-sections -x c -I"/src/uppsrc" -DflagCONSOLE -DflagST -DflagLINUX -DflagGNU3 -DflagIA32 -DflagRELEASE   /src/uppsrc/plugin/bz2/blocksort.c -o /uppout/uppnew/plugin/bz2/CONSOLE-GNU3-IA32-LINUX-RELEASE-ST/blocksort.o
echo /src/uppsrc/plugin/bz2/bzlib.c
/usr/local/bin/g++ -c -O2 -ffunction-sections -x c -I"/src/uppsrc" -DflagCONSOLE -DflagST -DflagLINUX -DflagGNU3 -DflagIA32 -DflagRELEASE   /src/uppsrc/plugin/bz2/bzlib.c -o /uppout/uppnew/plugin/bz2/CONSOLE-GNU3-IA32-LINUX-RELEASE-ST/bzlib.o
echo /src/uppsrc/plugin/bz2/compress.c
/usr/local/bin/g++ -c -O2 -ffunction-sections -x c -I"/src/uppsrc" -DflagCONSOLE -DflagST -DflagLINUX -DflagGNU3 -DflagIA32 -DflagRELEASE   /src/uppsrc/plugin/bz2/compress.c -o /uppout/uppnew/plugin/bz2/CONSOLE-GNU3-IA32-LINUX-RELEASE-ST/compress.o
echo /src/uppsrc/plugin/bz2/crctable.c
/usr/local/bin/g++ -c -O2 -ffunction-sections -x c -I"/src/uppsrc" -DflagCONSOLE -DflagST -DflagLINUX -DflagGNU3 -DflagIA32 -DflagRELEASE   /src/uppsrc/plugin/bz2/crctable.c -o /uppout/uppnew/plugin/bz2/CONSOLE-GNU3-IA32-LINUX-RELEASE-ST/crctable.o
echo /src/uppsrc/plugin/bz2/decompress.c
/usr/local/bin/g++ -c -O2 -ffunction-sections -x c -I"/src/uppsrc" -DflagCONSOLE -DflagST -DflagLINUX -DflagGNU3 -DflagIA32 -DflagRELEASE   /src/uppsrc/plugin/bz2/decompress.c -o /uppout/uppnew/plugin/bz2/CONSOLE-GNU3-IA32-LINUX-RELEASE-ST/decompress.o
echo /src/uppsrc/plugin/bz2/huffman.c
/usr/local/bin/g++ -c -O2 -ffunction-sections -x c -I"/src/uppsrc" -DflagCONSOLE -DflagST -DflagLINUX -DflagGNU3 -DflagIA32 -DflagRELEASE   /src/uppsrc/plugin/bz2/huffman.c -o /uppout/uppnew/plugin/bz2/CONSOLE-GNU3-IA32-LINUX-RELEASE-ST/huffman.o
echo /src/uppsrc/plugin/bz2/randtable.c
/usr/local/bin/g++ -c -O2 -ffunction-sections -x c -I"/src/uppsrc" -DflagCONSOLE -DflagST -DflagLINUX -DflagGNU3 -DflagIA32 -DflagRELEASE   /src/uppsrc/plugin/bz2/randtable.c -o /uppout/uppnew/plugin/bz2/CONSOLE-GNU3-IA32-LINUX-RELEASE-ST/randtable.o
/usr/local/bin/ar -sr /uppout/uppnew/plugin/bz2/CONSOLE-GNU3-IA32-LINUX-RELEASE-ST/bz2.a /uppout/uppnew/plugin/bz2/CONSOLE-GNU3-IA32-LINUX-RELEASE-ST/bz2upp.o /uppout/uppnew/plugin/bz2/CONSOLE-GNU3-IA32-LINUX-RELEASE-ST/blocksort.o /uppout/uppnew/plugin/bz2/CONSOLE-GNU3-IA32-LINUX-RELEASE-ST/bzlib.o /uppout/uppnew/plugin/bz2/CONSOLE-GNU3-IA32-LINUX-RELEASE-ST/compress.o /uppout/uppnew/plugin/bz2/CONSOLE-GNU3-IA32-LINUX-RELEASE-ST/crctable.o /uppout/uppnew/plugin/bz2/CONSOLE-GNU3-IA32-LINUX-RELEASE-ST/decompress.o /uppout/uppnew/plugin/bz2/CONSOLE-GNU3-IA32-LINUX-RELEASE-ST/huffman.o /uppout/uppnew/plugin/bz2/CONSOLE-GNU3-IA32-LINUX-RELEASE-ST/randtable.o
mkdir -p /uppout/uppnew/plugin/CONSOLE-GNU3-IA32-LINUX-RELEASE-ST
echo /src/uppsrc/plugin/dummy.cpp
/usr/local/bin/g++ -c -O2 -ffunction-sections -x c++ -fuse-cxa-atexit -I"/src/uppsrc" -DflagCONSOLE -DflagST -DflagLINUX -DflagGNU3 -DflagIA32 -DflagRELEASE   /src/uppsrc/plugin/dummy.cpp -o /uppout/uppnew/plugin/CONSOLE-GNU3-IA32-LINUX-RELEASE-ST/dummy.o
/usr/local/bin/ar -sr /uppout/uppnew/plugin/CONSOLE-GNU3-IA32-LINUX-RELEASE-ST/plugin.a /uppout/uppnew/plugin/CONSOLE-GNU3-IA32-LINUX-RELEASE-ST/dummy.o
mkdir -p /uppout/uppnew/Web/TServ/CONSOLE-GNU3-IA32-LINUX-MAIN-RELEASE-ST
echo /src/uppsrc/Web/TServ/tserv.cpp
/usr/local/bin/g++ -c -O2 -ffunction-sections -x c++ -fuse-cxa-atexit -I"/src/uppsrc" -DflagCONSOLE -DflagST -DflagMAIN -DflagLINUX -DflagGNU3 -DflagIA32 -DflagRELEASE   /src/uppsrc/Web/TServ/tserv.cpp -o /uppout/uppnew/Web/TServ/CONSOLE-GNU3-IA32-LINUX-MAIN-RELEASE-ST/tserv.o
/usr/local/bin/g++ -Wl,-s -static  -o /uppout/tserv -Wl,--start-group   /uppout/uppnew/Web/TServ/CONSOLE-GNU3-IA32-LINUX-MAIN-RELEASE-ST/tserv.o /uppout/uppnew/Web/CONSOLE-GNU3-IA32-LINUX-RELEASE-ST/Web.a /uppout/uppnew/TCore/CONSOLE-GNU3-IA32-LINUX-RELEASE-ST/TCore.a /uppout/uppnew/Core/CONSOLE-GNU3-IA32-LINUX-RELEASE-ST/Core.a -lpthread -ldl -luuid /uppout/uppnew/plugin/bz2/CONSOLE-GNU3-IA32-LINUX-RELEASE-ST/bz2.a /uppout/uppnew/plugin/CONSOLE-GNU3-IA32-LINUX-RELEASE-ST/plugin.a -Wl,--end-group
