topic "Tutorial";
[l288;i1120;a17;O9;~~~.1408;2 $$1,0#10431211400427159095818037425705:param]
[a83;*R6 $$2,5#31310162474203024125188417583966:caption]
[H4;b83;*4 $$3,5#07864147445237544204411237157677:title]
[i288;O9;C2 $$4,6#40027414424643823182269349404212:item]
[b42;a42;ph2 $$5,5#45413000475342174754091244180557:text]
[l288;b17;a17;2 $$6,6#27521748481378242620020725143825:desc]
[l321;C@5+75 $$7,7#20902679421464641399138805415013:code]
[b2503;2 $$8,0#65142375456100023862071332075487:separator]
[*@(0.0.255)2 $$9,0#83433469410354161042741608181528:base]
[C2 $$10,0#37138531426314131251341829483380:class]
[l288;a17;*1 $$11,11#70004532496200323422659154056402:requirement]
[i417;b42;a42;O9;~~~.416;2 $$12,12#10566046415157235020018451313112:tparam]
[b167;C2 $$13,13#92430459443460461911108080531343:item1]
[i288;a42;O9;C2 $$14,14#77422149456609303542238260500223:item2]
[*@2$(0.128.128)2 $$15,15#34511555403152284025741354420178:NewsDate]
[l321;*C$7;2 $$16,16#03451589433145915344929335295360:result]
[l321;b83;a83;*C$7;2 $$17,17#07531550463529505371228428965313:result`-line]
[l160;*C+117 $$18,5#88603949442205825958800053222425:package`-title]
[2 $$19,0#53580023442335529039900623488521:gap]
[C2 $$20,20#70211524482531209251820423858195:class`-nested]
[b50;2 $$21,21#03324558446220344731010354752573:Par]
[b83;* $$22,22#78EB85B566C7E078B53494D0FD53D992:subtitle]
[2 $$0,0#00000000000000000000000000000000:Default]
[{_} 
[s0;b20;a20;pht300; [*G5 U`+`+/SSH Package Tutorial]&]
[s5; A practical guide to using the SSH package in U`+`+ applications. 
&]
[s0;b20;t300;G &]
[s0; [*4 Table of Contents]&]
[s0; [^`#1^ 1 Introduction]&]
[s0; [^`#2^ 2 Getting Started]&]
[s0; [^`#3^ 3 Command Execution]&]
[s0; [^`#4^ 4 File Transfer with SFTP]&]
[s0; [^`#5^ 5 Interactive Shell]&]
[s0; [^`#6^ 6 Port Forwarding]&]
[s0; [^`#7^ 7 Advanced Features]&]
[s0; [^`#8^ 8 Best Practices]&]
[s0; [^`#9^ 9 Summary]&]
[s0; [^`#10^ 10 Additional Resources]&]
[s0; [^`#11^ 11 Note on Test Server]&]
[s0;N!t300;G &]
[s3;:1: 1. Introduction&]
[s5; The U`+`+/SSH package provides a clean, RAII`-based wrapper 
around libssh2, making SSH operations natural and safe in U`+`+ 
applications. The package handles the complexity of SSH`'s non`-blocking 
state machines, error handling, and resource management so you 
can focus on your application logic. &]
[s3;:2: 2. Getting Started&]
[s22; 2.1 Prerequisites&]
[s5; Before using the SSH package, ensure you have: &]
[s5;l224;i150;O0; U`+`+ IDE or umk installed&]
[s5;l224;i150;O0; Core/SSH package in your workspace&]
[s5;l224;i150;O0; A test SSH server (local or remote)&]
[s0; &]
[s22; 2.2 Basic Application Structure&]
[s5; All examples in this tutorial follow this pattern: &]
[s5; &]
[s7; #include[@0  ][@3 `"SshBasics.h`"]&]
[s7; &]
[s7; CONSOLE`_APP`_MAIN&]
[s7; `{&]
[s7;     StdLogSetup(LOG`_COUT[@(0.0.255) `|][@0 LOG`_FILE);]&]
[s7;     &]
[s7;     SshSession session;&]
[s7;     [*@(0.0.255) if][@0 (session][@(0.0.255) .][@0 Timeout(][@3 30000][@0 )][@(0.0.255) .][@0 Co
nnect(][@3 `"demo:password`@test.rebex.net:22`"][@0 )) `{]&]
[s7;         [/@4 // Your SSH operations here]&]
[s7;         [*_@(128.0.255) return][@0 ;]&]
[s7;     `}&]
[s7;     [@4 LOG][@0 (session][@(0.0.255) .][@0 GetErrorDesc());]&]
[s7; `}&]
[s5; The header file includes the necessary U`+`+ and SSH components:&]
[s5; &]
[s7; #ifndef[@0  `_SshBasic`_SshBasic`_h]&]
[s7; #define[@0  `_SshBasic`_SshBasic`_h]&]
[s7; &]
[s7; #include[@0  ][@(0.0.255) <][@0 Core][@(0.0.255) /][@0 Core][@(0.0.255) .][@0 h][@(0.0.255) >]&]
[s7; #include[@0  ][@(0.0.255) <][@0 Core][@(0.0.255) /][@0 SSH][@(0.0.255) /][@0 SSH][@(0.0.255) .][@0 h
][@(0.0.255) >]&]
[s7; &]
[s7; using namespace[@0  Upp;]&]
[s0; &]
[s22; 2.3 Connection and Authentication&]
[s5; The[@5  ][C^topic`:`/`/Core`/SSH`/src`/Upp`_Ssh`_Session`_en`-us`#Upp`:`:SshSession`:`:Connect`(const Upp`:`:String`&`)^@5 C
onnect()] method accepts various formats for credentials: &]
[s5;l192;i150;O0; Inline credentials: [C@5 `"user:password`@host:port`"]&]
[s5;l192;i150;O0; Host only: [C@5 `"host:port`"] (authenticate separately)&]
[s5; Set a timeout (in milliseconds) before connecting to prevent 
indefinite hangs. The 30`-second timeout used in examples is 
reasonable for most network conditions. &]
[s22; 2.4 Enable Debug Tracing&]
[s5; For troubleshooting, enable SSH tracing in your main function:&]
[s5; &]
[s7; CONSOLE`_APP`_MAIN&]
[s7; `{&]
[s7;     StdLogSetup(LOG`_COUT [@(0.0.255) `|][@0  LOG`_FILE);]&]
[s7;     Ssh[@(0.0.255) `::][@0 Trace();  ][/@4 // Enable basic SSH tracing]&]
[s7;     &]
[s7;     [/@4 // Your code...]&]
[s7; `}&]
[s7; &]
[s3;:3: 3. Command Execution&]
[s22; 3.1 Basic Command Execution&]
[s5; The [C^topic`:`/`/Core`/SSH`/src`/Upp`_Ssh`_Channels`_en`-us`#Upp`:`:SshExec`:`:class^@5 S
shExec] class executes a remote command and captures its output. 
The [C^topic`:`/`/Core`/SSH`/src`/Upp`_Ssh`_Channels`_en`-us`#Upp`:`:SshExec`:`:operator`(`)`(const Upp`:`:String`&`,Upp`:`:String`&`,Upp`:`:String`&`)^@5 o
perator()] takes a command line and returns the exit code, while 
filling two strings with stdout and stderr respectively.&]
[s0;b20;t300;*G &]
[s7; #include [@3 `"SshBasics.h`"]&]
[s7; &]
[s7; // ExecListDir:&]
[s7; // Demonstrates a remote command execution.&]
[s7; &]
[s7; void ExecListDir(SshSession`& session)&]
[s7; `{&]
[s7;     [@(0.0.255) const][@0  ][@(0.0.255) char][@0  ][@(0.0.255) `*][@0 cmdline 
][@(0.0.255) `=][@0  ][@3 `"ls `-l /pub/example`"][@0 ;]&]
[s7;     &]
[s7;     SshExec exec(session);&]
[s7;     String cout, cerr;&]
[s7;     [@(0.0.255) int][@0  exit`_code ][@(0.0.255) `=][@0  exec(cmdline, 
cout, cerr);]&]
[s7;     [@(0.0.255) if][@0 (][@(0.0.255) !][@0 exec][@(0.0.255) .][@0 IsError()) 
`{]&]
[s7;         [@4 DUMP][@0 (exit`_code);]&]
[s7;         [@4 LOG][@0 (][@3 `"Stdout:][@(0.0.255) `\n][@3 `"][@0  ][@(0.0.255) <<][@0  
cout);]&]
[s7;         [@4 LOG][@0 (][@3 `"Stderr:][@(0.0.255) `\n][@3 `"][@0  ][@(0.0.255) <<][@0  
cerr);]&]
[s7;         [_@(128.0.255) return][@0 ;]&]
[s7;     `}&]
[s7;     [@4 LOG][@0 (exec][@(0.0.255) .][@0 GetErrorDesc());]&]
[s7;     &]
[s7;     [/@4 // Or you can use one of the helper functions instead:]&]
[s7;     [/@4 // LOG(`"Stdout:`\n`" << SshExecute(session, cmdline));]&]
[s7; `}&]
[s0;b20;t300;*G &]
[s5; [* Key Points: ]&]
[s5;l192;i150;O0; The [C@(0.0.255) exec(cmdline, cout, cerr)] syntax 
executes the command synchronously&]
[s5;l192;i150;O0; Check [C^topic`:`/`/Core`/SSH`/src`/Upp`_Ssh`_Base`_en`-us`#Upp`:`:Ssh`:`:IsError`(`)const^@(0.0.255) I
sError()] to verify success before using the output&]
[s5;l192;i150;O0; Exit code 0 typically indicates success&]
[s5; The commented line shows a simpler one`-line alternative for 
quick commands&]
[s22; 3.2 Asynchronous Command Execution&]
[s5; For running commands without blocking, use U`+`+`'s [C^topic`:`/`/Core`/src`/AsyncWork`_en`-us`#Upp`:`:Async`(Upp`:`:Function`&`&`,Args`&`&`.`.`.args`)^@(0.0.255) A
sync] system to execute SSH operations in background threads. 
&]
[s5; &]
[s7; #include [@3 `"SshBasics.h`"]&]
[s7; &]
[s7; // ExecAsyncListDir: Demonstrates remote command execution in 
worker threads.&]
[s7; &]
[s7; AsyncWork[@(0.0.255) <void>][@0  AsyncListDir(SshSession][@(0.0.255) `&][@0  
session, ][@(0.0.255) const][@0  String][@(0.0.255) `&][@0  path)]&]
[s7; `{&]
[s7;     [@(0.0.255) auto][@0  worker ][@(0.0.255) `=][@0  Upp][@(0.0.255) `::][@0 Async(][@(0.0.255) `[
`=][@0 , ][@(0.0.255) `&][@0 session][@(0.0.255) `]][@0  `{]&]
[s7;         SshExec exec(session);&]
[s7;         String cout, cerr;&]
[s7;         [@(0.0.255) int][@0  exit`_code ][@(0.0.255) `=][@0  exec(][@3 `"ls 
`-l `"][@0  ][@(0.0.255) `+][@0  path, cout, cerr);]&]
[s7;         [@(0.0.255) if][@0 (exec][@(0.0.255) .][@0 IsError())]&]
[s7;             [_@(128.0.255) throw][@0  Ssh][@(0.0.255) `::][@0 Error(Format(][@3 `"Worker 
#][@(0.0.255) %d][@3 : ][@(0.0.255) %s][@3 `"][@0 , exec][@(0.0.255) .][@0 GetId(), 
exec][@(0.0.255) .][@0 GetErrorDesc()));]&]
[s7;         [@4 LOG][@0 (][@3 `"Directory: `"][@0  ][@(0.0.255) <<][@0  path);]&]
[s7;         [@4 LOG][@0 (][@3 `"Exit code: `"][@0  ][@(0.0.255) <<][@0  exit`_code);]&]
[s7;         [@4 LOG][@0 (][@3 `"Stdout:][@(0.0.255) `\n][@3 `"][@0  ][@(0.0.255) <<][@0  
cout);]&]
[s7;         [@4 LOG][@0 (][@3 `"Stderr:][@(0.0.255) `\n][@3 `"][@0  ][@(0.0.255) <<][@0  
cerr);]&]
[s7;     `});&]
[s7;     [_@(128.0.255) return][@0  pick(worker);]&]
[s7; `}&]
[s7; &]
[s7; void GetResult(AsyncWork<void>`& w)&]
[s7; `{&]
[s7;     [@(0.0.255) try][@0  `{]&]
[s7;         w[@(0.0.255) .][@0 Get();]&]
[s7;     `}&]
[s7;     [@(0.0.255) catch][@0 (][@(0.0.255) const][@0  Ssh][@(0.0.255) `::][@0 Error][@(0.0.255) `&
][@0  e) `{]&]
[s7;         [@4 LOG][@0 (e);]&]
[s7;     `}&]
[s7; `}&]
[s7; &]
[s7; void ExecAsyncListDir(SshSession`& session)&]
[s7; `{&]
[s7;     [@(0.0.255) const][@0  ][@(0.0.255) char][@0  ][@(0.0.255) `*][@0 path1 
][@(0.0.255) `=][@0  ][@3 `"/`"][@0 ;]&]
[s7;     [@(0.0.255) const][@0  ][@(0.0.255) char][@0  ][@(0.0.255) `*][@0 path2 
][@(0.0.255) `=][@0  ][@3 `"/pub/example/`"][@0 ;]&]
[s7;     &]
[s7;     [@(0.0.255) auto][@0  worker1 ][@(0.0.255) `=][@0  AsyncListDir(session, 
path1);]&]
[s7;     [@(0.0.255) auto][@0  worker2 ][@(0.0.255) `=][@0  AsyncListDir(session, 
path2);]&]
[s7;     &]
[s7;     GetResult(worker2);&]
[s7;     GetResult(worker1);&]
[s7; `}&]
[s5; [* Key Points: ]&]
[s5;l192;i150;O0; [C^topic`:`/`/Core`/src`/AsyncWork`_en`-us`#Upp`:`:Async`(Upp`:`:Function`&`&`,Args`&`&`.`.`.args`)^ A
sync()][G  creates a background worker that won`'t block the main 
thread]&]
[s5;l192;i150;O0; The lambda captures the session by reference and 
command by value&]
[s5;l192;i150;O0; [C^topic`:`/`/Core`/src`/AsyncWork`_en`-us`#Upp`:`:AsyncWork`:`:Get`(`)^ G
et][^topic`:`/`/Core`/src`/AsyncWork`_en`-us`#Upp`:`:AsyncWork`:`:Get`(`)^ ()][G  
blocks until the operation completes]&]
[s5;l192;i150;O0; Errors are thrown and must be caught at the call 
site&]
[s5;l192;i150;O0; Each SSH object has a unique ID accessible via 
[C^topic`:`/`/Core`/SSH`/src`/Upp`_Ssh`_Base`_en`-us`#Upp`:`:Ssh`:`:GetId`(`)const^ G
etId()] for tracking&]
[s3;:4: 4. File Transfer with SFTP&]
[s22; 4.1 Simple File Download&]
[s5; The [C^topic`:`/`/Core`/SSH`/src`/Upp`_Ssh`_SFtp`_en`-us`#Upp`:`:SFtp`:`:class^@(0.0.255) S
Ftp] class provides high`-level file operations. [C^topic`:`/`/Core`/SSH`/src`/Upp`_Ssh`_SFtp`_en`-us`#Upp`:`:SFtp`:`:LoadFile`(const char`*`)^ L
oadFile()] downloads a file and returns its contents as a string. 
&]
[s5; &]
[s7; #include [@3 `"SshBasics.h`"]&]
[s7; &]
[s7; // SFtpGet:&]
[s7; // Demonstrates a file download, using sftp.&]
[s7; &]
[s7; void SFtpGet(SshSession`& session)&]
[s7; `{&]
[s7;     [*@(0.0.255) const][@0  ][*@(0.0.255) char][@0  ][@(0.0.255) `*][@0 path 
][@(0.0.255) `=][@0  ][@3 `"/readme.txt`"][@0 ;]&]
[s7;     &]
[s7;     SFtp sftp(session);&]
[s7;     String file [@(0.0.255) `=][@0  sftp][@(0.0.255) .][@0 LoadFile(path);]&]
[s7;     [@4 LOG][@0 ((][@(0.0.255) !][@0 sftp][@(0.0.255) .][@0 IsError() ][@(0.0.255) ?][@0  
file ][@(0.0.255) :][@0  sftp][@(0.0.255) .][@0 GetErrorDesc()));]&]
[s7; `}&]
[s5; [* Key Points: ]&]
[s5;l192;i150;O0; Simple one`-liner for downloading files&]
[s5;l192;i150;O0; Suitable for text files or small binary files&]
[s5;l192;i150;O0; The ternary operator checks for errors and displays 
either content or error message&]
[s22; 4.2 Streaming File Operations&]
[s5; For large files or when you need to process data incrementally, 
use [C^topic`:`/`/Core`/SSH`/src`/Upp`_Ssh`_SFtpStream`_en`-us`#Upp`:`:SFtpFileIn`:`:class^ S
FtpFileIn] for reading. This approach keeps memory usage constant 
regardless of file size. &]
[s5; &]
[s7; #include[@0  ][@3 `"SshBasics.h`"]&]
[s7; &]
[s7; // SFtpStreamGet:&]
[s7; // Demonstrates a basic stream operation on an sftp remote file 
object.&]
[s7; &]
[s7; void[@0  SFtpStreamGet(SshSession]`&[@0  session)]&]
[s7; `{&]
[s7;     [*@(0.0.255) const][@0  ][*@(0.0.255) char][@0  ][@(0.0.255) `*][@0 path 
][@(0.0.255) `=][@0  ][@3 `"/readme.txt`"][@0 ;]&]
[s7;     &]
[s7;     SFtp sftp(session);&]
[s7;     SFtpFileIn fi(sftp, path);&]
[s7;     [*@(0.0.255) while][@0 (][@(0.0.255) !][@0 fi][@(0.0.255) .][@0 IsEof()) 
`{]&]
[s7;         [@(0.128.128) int64][@0   pos  ][@(0.0.255) `=][@0  fi][@(0.0.255) .][@0 GetPos();]&]
[s7;         String line [@(0.0.255) `=][@0  fi][@(0.0.255) .][@0 GetLine();]&]
[s7;         [*@(0.0.255) if][@0 (][@(0.0.255) !][@0 line][@(0.0.255) .][@0 IsEmpty())]&]
[s7;             [@4 LOG][@0 (Format(][@3 `"Offset: ][@(0.0.255) %3d][@3 , Line: 
`[][@(0.0.255) %s][@3 `]`"][@0 , pos, line));]&]
[s7;     `}&]
[s7;     [*@(0.0.255) if][@0 (fi][@(0.0.255) .][@0 IsError())]&]
[s7;         [@4 LOG][@0 (fi][@(0.0.255) .][@0 GetErrorText());]&]
[s7; &]
[s7; `}&]
[s12; [* Key Points: ]&]
[s5;l192;i150;O0; [C^topic`:`/`/Core`/SSH`/src`/Upp`_Ssh`_SFtpStream`_en`-us`#Upp`:`:SFtpFileIn`:`:class^ S
FtpFileIn][G  opens a file for reading in stream mode]&]
[s5;l192;i150;O0; [^topic`:`/`/Core`/src`/Stream`_en`-us`#Stream`:`:GetLine`(`)^ GetLin
e()][G  reads line by line, perfect for text files]&]
[s5;l192;i150;O0; [^topic`:`/`/Core`/src`/Stream`_en`-us`#Stream`:`:GetPos`(`)const^ Ge
tPos()][G  returns the current file position]&]
[s5;l192;i150;O0; Stream automatically closes when object goes out 
of scope&]
[s5;l192;i150;O0; Always check [C^topic`:`/`/Core`/SSH`/src`/Upp`_Ssh`_Base`_en`-us`#Upp`:`:Ssh`:`:IsError`(`)const^ I
sError()] after the loop completes&]
[s22; 4.3 File System Transparency&]
[s5; The SSH package integrates with U`+`+`'s [C^topic`:`/`/Core`/SSH`/src`/Upp`_Ssh`_SFtp`_en`-us`#Upp`:`:SFtpFileSystemInfo`:`:class^@5 F
ileSystemInfo] abstraction, allowing you to work with remote 
and local filesystems using the same interface. &]
[s5; &]
[s7; #include[@0  ][@3 `"SshBasics.h`"]&]
[s7; &]
[s7; // SFtpTransparency:&]
[s7; // Demonstrates  access to sftp directory hierarcy in a file`-system`-agnostic 
(transparent) way.&]
[s7; &]
[s7; void[@0  ReadDirEntries(FileSystemInfo]`&[@0  fsi, ]const[@0  String]`&[@0  
path)]&]
[s7; `{&]
[s7;     [*@(0.0.255) int][@0  maxentry ][@(0.0.255) `=][@0  ][@3 5][@0 ;]&]
[s7;     [*@(0.0.255) for][@0 (FileSystemInfo][@(0.0.255) `::][@0 FileInfo 
entry ][@(0.0.255) :][@0  fsi][@(0.0.255) .][@0 Find(path, maxentry)) 
`{]&]
[s7;         [@4 DUMP][@0 (entry][@(0.0.255) .][@0 filename);]&]
[s7;         [@4 DUMP][@0 (entry][@(0.0.255) .][@0 is`_folder);]&]
[s7;         [@4 DUMP][@0 (entry][@(0.0.255) .][@0 length);]&]
[s7;         [@4 DUMP][@0 (entry][@(0.0.255) .][@0 last`_access`_time);]&]
[s7;         [/@4 //...]&]
[s7;     `}&]
[s7; `}&]
[s7; &]
[s7; void[@0  SFtpTransparency(SshSession]`&[@0  session)]&]
[s7; `{&]
[s7;     [@4 LOG][@0 (][@3 `"Local file system objects`-`-`-`-`-`-`-`-`-`-`-`-`-`-`-`-`-`-`-
`-`-`-`-`-`-`-`-`-`-`-`-`-`-`-`-`-`-`-`-`-`-`-`-`-`-`-`-`-`-`-`-`-`-`-`-`-`-`-`"][@0 )
;]&]
[s7;     ReadDirEntries(StdFileSystemInfo(), GetCurrentDirectory());&]
[s7; &]
[s7;     SFtp sftp(session);&]
[s7;     SFtpFileSystemInfo sfsi(sftp);&]
[s7;     &]
[s7;     [@4 LOG][@0 (][@3 `"Remote file system objects`-`-`-`-`-`-`-`-`-`-`-`-`-`-`-`-`-`-
`-`-`-`-`-`-`-`-`-`-`-`-`-`-`-`-`-`-`-`-`-`-`-`-`-`-`-`-`-`-`-`-`-`-`-`-`-`-`-`"][@0 )
;]&]
[s7;     ReadDirEntries((FileSystemInfo[@(0.0.255) `&][@0 ) sfsi, ][@3 `"/pub/example/`*.pn
g`"][@0 );]&]
[s7;     &]
[s7;     [*@(0.0.255) if][@0 (sftp][@(0.0.255) .][@0 IsError())]&]
[s7;         [@4 LOG][@0 (sftp][@(0.0.255) .][@0 GetErrorDesc());]&]
[s7; `}&]
[s0; &]
[s5; Key Points: &]
[s5;l192;i150;O0; [^topic`:`/`/Core`/SSH`/src`/Upp`_Ssh`_SFtp`_en`-us`#Upp`:`:SFtpFileSystemInfo`:`:class^ S
FtpFileSystemInfo][G  wraps SFtp in a ]FileSystemInfo[G  interface]&]
[s5;l192;i150;O0; The same [C@(0.0.255) ReadDirEntries()] function 
works for both local and remote filesystems&]
[s5;l192;i150;O0; Supports wildcards like [C@(0.0.255) `*.png] for 
filtering&]
[s5;l192;i150;O0; maxentry[G  parameter limits results to prevent overwhelming 
output]&]
[s5;l192;i150;O0; Cast to [C@(0.0.255) FileSystemInfo`&] enables polymorphic 
usage&]
[s22; 4.4 Multi`-threaded SFTP Downloads&]
[s5; For downloading multiple files in parallel, create async workers 
that each perform a download operation. &]
[s5; &]
[s7; #include[@0  ][@3 `"SshBasics.h`"]&]
[s7; &]
[s7; // SFtpAsyncGet: DEmonstrates multiple file downloads, using 
worker threads.&]
[s7; &]
[s7; AsyncWork[@(0.0.255) <][*@(0.0.255) void][@(0.0.255) >][@0  AsyncGet(SshSession][@(0.0.255) `&
][@0  session, ][*@(0.0.255) const][@0  String][@(0.0.255) `&][@0  path)]&]
[s7; `{&]
[s7;     [*@(0.0.255) auto][@0  worker ][@(0.0.255) `=][@0  Upp][@(0.0.255) `::][@0 Async(][@(0.0.255) `[
`=][@0 , ][@(0.0.255) `&][@0 session][@(0.0.255) `]][@0  `{]&]
[s7;         [@4 LOG][@0 (][@3 `"Downloading `"][@0  ][@(0.0.255) <<][@0  path);]&]
[s7;         SFtp sftp(session);&]
[s7;         String file [@(0.0.255) `=][@0  sftp][@(0.0.255) .][@0 LoadFile(path);]&]
[s7;         [*@(0.0.255) if][@0 (sftp][@(0.0.255) .][@0 IsError())]&]
[s7;             [*_@(128.0.255) throw][@0  Ssh][@(0.0.255) `::][@0 Error(Format(][@3 `"Worker 
#][@(0.0.255) %d][@3 : ][@(0.0.255) %s][@3 `"][@0 , sftp][@(0.0.255) .][@0 GetId(), 
sftp][@(0.0.255) .][@0 GetErrorDesc()));]&]
[s7;         [@4 LOG][@0 (][@3 `"File `"][@0  ][@(0.0.255) <<][@0  GetFileName(path) 
][@(0.0.255) <<][@0  ][@3 `" is successfully downloaded.`"][@0 );]&]
[s7;     `});&]
[s7;     [*_@(128.0.255) return][@0  pick(worker);]&]
[s7; `}&]
[s7; &]
[s7; void[@0  CheckError(AsyncWork]<void>`&[@0  w)]&]
[s7; `{&]
[s7;     [*@(0.0.255) try][@0  `{]&]
[s7;         w[@(0.0.255) .][@0 Get();]&]
[s7;     `}&]
[s7;     [*@(0.0.255) catch][@0 (][*@(0.0.255) const][@0  Ssh][@(0.0.255) `::][@0 Error][@(0.0.255) `&
][@0  e) `{]&]
[s7;         [@4 LOG][@0 (e);]&]
[s7;     `}&]
[s7; `}&]
[s7; &]
[s7; void[@0  SFtpAsyncGet(SshSession]`&[@0  session)]&]
[s7; `{&]
[s7;     [*@(0.0.255) const][@0  ][*@(0.0.255) int][@0  MAXDOWNLOADS ][@(0.0.255) `=][@0  
][@3 4][@0 ;]&]
[s7;     [*@(0.0.255) const][@0  ][*@(0.0.255) char][@0  ][@(0.0.255) `*][@0 path 
][@(0.0.255) `=][@0  ][@3 `"/pub/example/`"][@0 ;]&]
[s7;     &]
[s7;     SFtp browser(session);&]
[s7;     SFtp[@(0.0.255) `::][@0 DirList ls;]&]
[s7;     &]
[s7;     [*@(0.0.255) if][@0 (][@(0.0.255) !][@0 browser][@(0.0.255) .][@0 ListDir(path, 
ls)) `{ ][/@4 // Get a dir listing to extract file names on`-the`-fly.]&]
[s7;         [@4 LOG][@0 (browser][@(0.0.255) .][@0 GetErrorDesc());]&]
[s7;         [*_@(128.0.255) return][@0 ;]&]
[s7;     `}&]
[s7; &]
[s7;     Array[@(0.0.255) <][@0 AsyncWork][@(0.0.255) <][*@(0.0.255) void][@(0.0.255) >>][@0  
workers;]&]
[s7;     &]
[s7;     [*@(0.0.255) for][@0 (][*@(0.0.255) const][@0  ][*@(0.0.255) auto][@(0.0.255) `&][@0  
e ][@(0.0.255) :][@0  ls) `{]&]
[s7;         [*@(0.0.255) if][@0 (][@(0.0.255) !][@0 e][@(0.0.255) .][@0 IsFile() 
][@(0.0.255) `|`|][@0  (e][@(0.0.255) .][@0 GetSize() ][@(0.0.255) >][@0  
][@3 65535][@0 ))]&]
[s7;             [*_@(128.0.255) continue][@0 ;]&]
[s7;         [*@(0.0.255) if][@0 (workers][@(0.0.255) .][@0 GetCount() ][@(0.0.255) `=`=][@0  
MAXDOWNLOADS)]&]
[s7;             [*_@(128.0.255) break][@0 ;]&]
[s7;         workers[@(0.0.255) .][@0 Add(AsyncGet(session, AppendFileName(path, 
e][@(0.0.255) .][@0 GetName())));]&]
[s7;     `}&]
[s7;     &]
[s7;     [*@(0.0.255) while][@0 (][@(0.0.255) !][@0 workers][@(0.0.255) .][@0 IsEmpty()) 
`{]&]
[s7;         [*@(0.0.255) for][@0 (][*@(0.0.255) int][@0  i ][@(0.0.255) `=][@0  
][@3 0][@0 ; i ][@(0.0.255) <][@0  workers][@(0.0.255) .][@0 GetCount(); 
i][@(0.0.255) `+`+][@0 ) `{]&]
[s7;             [*@(0.0.255) auto][@(0.0.255) `&][@0  worker ][@(0.0.255) `=][@0  
workers][@(0.0.255) `[][@0 i][@(0.0.255) `]][@0 ;]&]
[s7;             [*@(0.0.255) if][@0 (worker][@(0.0.255) .][@0 IsFinished()) 
`{]&]
[s7;                 CheckError(worker);&]
[s7;                 workers[@(0.0.255) .][@0 Remove(i);]&]
[s7;                 [*_@(128.0.255) break][@0 ;]&]
[s7;             `}&]
[s7;             Sleep([@3 1][@0 );]&]
[s7;         `}&]
[s7;     `}&]
[s7; `}&]
[s5; &]
[s5; [* Key Points: ]&]
[s5;l192;i150;O0; First, use a `"browser`" SFtp instance to list 
directory contents&]
[s5;l192;i150;O0; Filter entries to download only files under a size 
limit&]
[s5;l192;i150;O0; Create up to [@(0.0.255) MAXDOWNLOADS] concurrent 
workers&]
[s5;l192;i150;O0; Poll workers in a loop, removing finished ones&]
[s5;l192;i150;O0; Each worker gets its own [^topic`:`/`/Core`/SSH`/src`/Upp`_Ssh`_SFtp`_en`-us`#Upp`:`:SFtp`:`:class^ S
Ftp] instance for thread safety&]
[s22; 4.5 Alternative: Using CoFor for Parallelization&]
[s5; U`+`+`'s [C^topic`:`/`/Core`/src`/CoWork`_en`-us`#Upp`:`:CoFor`(int`,Fn`)^@(0.0.255) C
oFor] provides a cleaner way to parallelize operations with automatic 
thread pool management. &]
[s5; &]
[s7; #include[@0  ][@3 `"SshBasics.h`"]&]
[s7; &]
[s7; // SFtpAsyncGet2: Demonstrates multiple file downloads, using 
a parallelization loop.&]
[s7; &]
[s7; &]
[s7; void[@0  SFtpAsyncGet2(SshSession]`&[@0  session)]&]
[s7; `{&]
[s7;     [*@(0.0.255) const][@0  ][*@(0.0.255) int][@0  MAXDOWNLOADS ][@(0.0.255) `=][@0  
][@3 4][@0 ;]&]
[s7;     [*@(0.0.255) const][@0  ][*@(0.0.255) char][@0  ][@(0.0.255) `*][@0 path 
][@(0.0.255) `=][@0  ][@3 `"/pub/example/`"][@0 ;]&]
[s7; &]
[s7;     SFtp[@(0.0.255) `::][@0 DirList ls;]&]
[s7;     `{&]
[s7;         [/@4 // Get a remote dir listing.]&]
[s7;         SFtp browser(session);&]
[s7;         [*@(0.0.255) if][@0 (][@(0.0.255) !][@0 browser][@(0.0.255) .][@0 ListDir(path, 
ls)) `{]&]
[s7;             [@4 RLOG][@0 (browser][@(0.0.255) .][@0 GetErrorDesc());]&]
[s7;             [*_@(128.0.255) return][@0 ;]&]
[s7;         `}&]
[s7;     `}&]
[s7; &]
[s7;     [/@4 // Filter the dir list.]&]
[s7;     [*@(0.0.255) auto][@0  files ][@(0.0.255) `=][@0   FilterRange(ls, 
][@(0.0.255) `[`]][@0 (][*@(0.0.255) const][@0  SFtp][@(0.0.255) `::][@0 DirEntry][@(0.0.255) `&
][@0  e) `{ ][*_@(128.0.255) return][@0  e][@(0.0.255) .][@0 IsFile() ][@(0.0.255) `&`&][@0  
e][@(0.0.255) .][@0 GetSize() ][@(0.0.255) <`=][@0  ][@3 65536][@0 ; `});]&]
[s7; &]
[s7;     [/@4 // Loop over.]&]
[s7;     CoFor(min(files[@(0.0.255) .][@0 GetCount(), MAXDOWNLOADS), 
][@(0.0.255) `[`&][@0 files, ][@(0.0.255) `&][@0 path, ][@(0.0.255) `&][@0 session][@(0.0.255) `]
][@0 (][*@(0.0.255) int][@0  i)`{]&]
[s7;         [*@(0.0.255) const][@0  SFtp][@(0.0.255) `::][@0 DirEntry][@(0.0.255) `&][@0  
e ][@(0.0.255) `=][@0  files][@(0.0.255) `[][@0 i][@(0.0.255) `]][@0 ;]&]
[s7;         String fpath [@(0.0.255) `=][@0  AppendFileName(path, e][@(0.0.255) .][@0 GetNam
e());]&]
[s7;         [@4 RLOG][@0 (][@3 `"Downloading `"][@0  ][@(0.0.255) <<][@0  fpath);]&]
[s7;         SFtp sftp(session);&]
[s7;         String file [@(0.0.255) `=][@0  sftp][@(0.0.255) .][@0 LoadFile(fpath);]&]
[s7;         [*@(0.0.255) if][@0 (sftp][@(0.0.255) .][@0 IsError())]&]
[s7;             [@4 RLOG][@0 (Format(][@3 `"Worker #][@(0.0.255) %d][@3 : ][@(0.0.255) %s][@3 `"][@0 ,
 sftp][@(0.0.255) .][@0 GetId(), sftp][@(0.0.255) .][@0 GetErrorDesc()));]&]
[s7;         [*@(0.0.255) else]&]
[s7;             [@4 RLOG][@0 (][@3 `"File `"][@0  ][@(0.0.255) <<][@0  e][@(0.0.255) .][@0 GetName()
 ][@(0.0.255) <<][@0  ][@3 `" is successfully downloaded.`"][@0 );]&]
[s7;     `});&]
[s7; `}&]
[s5; &]
[s5; [* Key Points: ]&]
[s5;l192;i150;O0; [^topic`:`/`/Core`/src`/Range`_en`-us`#Upp`:`:FilterRange`(BaseRange`&`&`,Predicate`)^ F
ilterRange()][G  creates a filtered view without copying]&]
[s5;l192;i150;O0; [^topic`:`/`/Core`/src`/CoWork`_en`-us`#Upp`:`:CoFor`(int`,Fn`)^ CoFo
r()][G  automatically manages the thread pool]&]
[s5;l192;i150;O0; Much cleaner than manual worker array management&]
[s5;l192;i150;O0; Lambda receives index [C@(0.0.255) i] to access the 
file list&]
[s3;:5: 5. Interactive Shell&]
[s22; 5.1 Console Shell&]
[s5; The [C^topic`:`/`/Core`/SSH`/src`/Upp`_Ssh`_Channels`_en`-us`#Upp`:`:SshShell`:`:Console`(const Upp`:`:String`&`)^@5 C
onsole()] method provides a fully interactive terminal session. 
It handles terminal emulation and passes input/output between 
your console and the remote shell. &]
[s5; &]
[s7; #include[@0  ][@3 `"SshBasics.h`"]&]
[s7; &]
[s7; // ShellConsole:&]
[s7; // Demonstrates an interactive shell in console mode.&]
[s7; &]
[s7; void[@0  ShellConsole(SshSession]`&[@0  session)]&]
[s7; `{&]
[s7;     SshShell shell(session);&]
[s7;     shell[@(0.0.255) .][@0 Timeout(][@(0.128.128) Null][@0 );]&]
[s7;     [*@(0.0.255) if][@0 (][@(0.0.255) !][@0 shell][@(0.0.255) .][@0 Console(][@3 `"ansi`"][@0 ))
]&]
[s7;         [@4 LOG][@0 (shell][@(0.0.255) .][@0 GetErrorDesc());]&]
[s7; `}&]
[s5; &]
[s5; Key Points: &]
[s5;l192;i150;O0; Timeout(Null)[G  disables timeout for interactive 
sessions]&]
[s5;l192;i150;O0; Terminal type [C@(0.0.255) `"ansi`"] provides broad 
compatibility&]
[s5;l192;i150;O0; Other options include [C@(0.0.255) `"xterm`"], [C@(0.0.255) `"vt100`"],
 [C@(0.0.255) `"linux`"]&]
[s5;l192;i150;O0; The method blocks until the shell session ends&]
[s5;l192;i150;O0; User types commands directly; output appears immediately&]
[s22; 5.2 X11 Forwarding&]
[s5; To run graphical applications on the remote server with the 
display forwarded to your local machine, enable X11 forwarding.&]
[s5; &]
[s7; #include[@0  ][@3 `"SshBasics.h`"]&]
[s7; &]
[s7; // X11Forwarding:&]
[s7; // Demonstrates an interactive shell with X11 forwarding, in 
console mode.&]
[s7; &]
[s7; // This example requires a running X server.&]
[s7; &]
[s7; void[@0  X11Forwarding(SshSession]`&[@0  session)]&]
[s7; `{&]
[s7;     SshShell x11shell(session);&]
[s7;     x11shell[@(0.0.255) .][@0 Timeout(][@(0.128.128) Null][@0 );]&]
[s7;     session[@(0.0.255) .][@0 WhenX11 ][@(0.0.255) `=][@0  ][@(0.0.255) `[`&][@0 x11shell][@(0.0.255) `]
][@0 (SshX11Handle xhandle)]&]
[s7;     `{&]
[s7;         x11shell[@(0.0.255) .][@0 AcceptX11(xhandle);]&]
[s7; &]
[s7;     `};&]
[s7;     [*@(0.0.255) if][@0 (][@(0.0.255) !][@0 x11shell][@(0.0.255) .][@0 ForwardX11()][@(0.0.255) .
][@0 Console(][@3 `"ansi`"][@0 ))]&]
[s7;         [@4 LOG](x11shell[@(0.0.255) .]GetErrorDesc());&]
[s0; [C0 `}]&]
[s5;C0 &]
[s5; [* Key Points: ]&]
[s5;l192;i150;O0; Requires an X server running locally (Xorg, XQuartz, 
Xming, etc.)&]
[s5;l192;i150;O0; WhenX11[G  callback accepts incoming X11 connection 
requests]&]
[s5;l192;i150;O0; [^topic`:`/`/Core`/SSH`/src`/Upp`_Ssh`_Channels`_en`-us`#Upp`:`:SshShell`:`:ForwardX11`(const Upp`:`:String`&`,int`,int`,int`)^ F
orwardX11()][G  enables X11 forwarding before opening the console]&]
[s5;l192;i150;O0; The shell must remain open while graphical applications 
run&]
[s5;l192;i150;O0; Launch GUI apps in the remote shell; they`'ll display 
locally&]
[s3;:6: 6. Port Forwarding&]
[s22; 6.1 TCP/IP Tunneling&]
[s5; SSH tunneling creates secure channels for network traffic. Set 
up a listener that accepts connections and forwards them through 
the SSH session.&]
[s5; &]
[s7; #include[@0  ][@3 `"SshBasics.h`"]&]
[s7; &]
[s7; // OpenTcpTunnel:&]
[s7; // Demonstrates tcp`-ip and port forwarding feature of the ssh 
protocol.&]
[s7; &]
[s7; // This example requires upp/reference/SocketServer and upp/reference/SocketClie
nt examples.&]
[s7; // SocketClient: Set the port number to 3215.&]
[s7; &]
[s7; bool[@0  ServerSendRecv(SshSession]`&[@0  session, String]`&[@0  data)]&]
[s7; `{&]
[s7;     [/@4 // SshTunnel <`-> SocketServer]&]
[s7;     SshTunnel tunnel(session);&]
[s7;     [*@(0.0.255) if][@0 (][@(0.0.255) !][@0 tunnel][@(0.0.255) .][@0 Connect(][@3 `"127.0.0.1`"
][@0 , ][@3 3214][@0 )) `{]&]
[s7;         [@4 LOG][@0 (][@3 `"ServerSendRecv(): `"][@0  ][@(0.0.255) <<][@0  
tunnel][@(0.0.255) .][@0 GetErrorDesc());]&]
[s7;         [*_@(128.0.255) return][@0  ][*@(0.0.255) false][@0 ;]&]
[s7;     `}&]
[s7;     tunnel[@(0.0.255) .][@0 Put(data ][@(0.0.255) `+][@0  ][@3 `'][@(0.0.255) `\n][@3 `'][@0 );]&]
[s7;     data [@(0.0.255) `=][@0  tunnel][@(0.0.255) .][@0 GetLine();]&]
[s7;     [*_@(128.0.255) return][@0  ][@(0.0.255) !][@0 data][@(0.0.255) .][@0 IsEmpty();]&]
[s7; `}&]
[s7; &]
[s7; void[@0  ForwardTcpIp(SshSession]`&[@0  session)]&]
[s7; `{&]
[s7;     SshTunnel listener(session);&]
[s7;     [*@(0.0.255) if][@0 (][@(0.0.255) !][@0 listener][@(0.0.255) .][@0 Listen(][@3 3215][@0 , 
][@3 5][@0 )) `{]&]
[s7;         [@4 LOG][@0 (][@3 `"ForwardTcpIp(): `"][@0  ][@(0.0.255) <<][@0  
listener][@(0.0.255) .][@0 GetErrorDesc());]&]
[s7;         [*_@(128.0.255) return][@0 ;]&]
[s7;     `}&]
[s7;     [@4 LOG][@0 (][@3 `"SSH tunnel (server mode): Waiting for the requests 
to be tunneled...`"][@0 );]&]
[s7;     [*@(0.0.255) for][@0 (;;) `{]&]
[s7;         SshTunnel tunnel(session);&]
[s7;         [*@(0.0.255) if][@0 (][@(0.0.255) !][@0 tunnel][@(0.0.255) .][@0 Accept(listener)) 
`{]&]
[s7;             [@4 LOG][@0 (][@3 `"ForwardTcpIp(): `"][@0  ][@(0.0.255) <<][@0  
tunnel][@(0.0.255) .][@0 GetErrorDesc());]&]
[s7;             [*_@(128.0.255) return][@0 ;]&]
[s7;         `}&]
[s7;         [/@4 // SocketClient <`-> SshTunnel]&]
[s7;         String data [@(0.0.255) `=][@0  tunnel][@(0.0.255) .][@0 GetLine();]&]
[s7;         [@4 LOG][@0 (][@3 `"Tunneled Request: `"][@0  ][@(0.0.255) <<][@0  
data);]&]
[s7;         [*@(0.0.255) if][@0 (][@(0.0.255) !][@0 data][@(0.0.255) .][@0 IsEmpty() 
][@(0.0.255) `&`&][@0  ServerSendRecv(session, data)) `{]&]
[s7;             [@4 LOG][@0 (][@3 `"Tunneled Response: `"][@0  ][@(0.0.255) <<][@0  
data);]&]
[s7;             tunnel[@(0.0.255) .][@0 Put(data ][@(0.0.255) `+][@0  ][@3 `'][@(0.0.255) `\n][@3 `'
][@0 );]&]
[s7;         `}&]
[s7;     `}&]
[s7; `}&]
[s5; &]
[s5; [* Key Points: ]&]
[s5;l192;i150;O0; [C^topic`:`/`/Core`/SSH`/src`/Upp`_Ssh`_Channels`_en`-us`#Upp`:`:SshTunnel`:`:Listen`(int`,int`)^ L
isten(port, backlog)][G  sets up a remote listener]&]
[s5;l192;i150;O0; [C^topic`:`/`/Core`/SSH`/src`/Upp`_Ssh`_Channels`_en`-us`#Upp`:`:SshTunnel`:`:Accept`(Upp`:`:SshTunnel`&`)^ A
ccept()][G  waits for and accepts incoming connections]&]
[s5;l192;i150;O0; Each accepted connection gets its own [C^topic`:`/`/Core`/SSH`/src`/Upp`_Ssh`_Channels`_en`-us`#Upp`:`:SshTunnel`:`:class^@5 S
shTunnel] object&]
[s5;l192;i150;O0; Use [C@5$(245) Put()] and [C@5$(245) GetLine()] for 
bidirectional communication&]
[s5;l192;i150;O0; The helper function [C@(0.0.255)$(245) ServerSendRecv()] 
forwards to another local service&]
[s5;l192;i150;O0; This creates a chain: RemoteClient → SSH → LocalService&]
[s3;:7: 7. Advanced Features&]
[s22; 7.1 SCP File Transfer&]
[s5; The [C@5$(245) Scp] class provides SCP protocol file transfer, 
which is simpler but less flexible than SFTP. &]
[s5; &]
[s7; #include[@0  ][@3 `"SshBasics.h`"]&]
[s7; &]
[s7; // ScpGet:&]
[s7; // Demonstrates a file download using scp.&]
[s7; &]
[s7; void[@0  ScpGet(SshSession]`&[@0  session)]&]
[s7; `{&]
[s7;     [*@(0.0.255) const][@0  ][*@(0.0.255) char][@0  ][@(0.0.255) `*][@0 path 
][@(0.0.255) `=][@0  ][@3 `"the/full/path/of/the/file/to/downlad`"][@0 ;]&]
[s7;     &]
[s7;     Scp scp(session);&]
[s7;     String file [@(0.0.255) `=][@0  scp][@(0.0.255) .][@0 LoadFile(path);]&]
[s7;     [@4 LOG][@0 ((][@(0.0.255) !][@0 scp][@(0.0.255) .][@0 IsError() ][@(0.0.255) ?][@0  
file ][@(0.0.255) :][@0  scp][@(0.0.255) .][@0 GetErrorDesc()));]&]
[s7; `}&]
[s5; &]
[s5; [* Key Points: ]&]
[s5;l192;i150;O0; SCP is good for simple file uploads and downloads&]
[s5;l192;i150;O0; No directory listing or file management operations&]
[s5;l192;i150;O0; API mirrors [^topic`:`/`/Core`/SSH`/src`/Upp`_Ssh`_SFtp`_en`-us`#Upp`:`:SFtp`:`:LoadFile`(Upp`:`:Stream`&`,const char`*`)^ S
Ftp: ][C^topic`:`/`/Core`/SSH`/src`/Upp`_Ssh`_SFtp`_en`-us`#Upp`:`:SFtp`:`:LoadFile`(Upp`:`:Stream`&`,const char`*`)^@5 L
oadFile()] for downloads&]
[s5;l192;i150;O0; Requires full path to the file&]
[s5;l192;i150;O0; Generally simpler but less capable than SFTP&]
[s22; 7.2 Pick Semantics&]
[s5; U`+`+ objects support pick (move) semantics for efficient transfer 
of ownership without copying internal state. &]
[s5; &]
[s7; #include[@0  ][@3 `"SshBasics.h`"]&]
[s7; &]
[s7; // SshPick:&]
[s7; // Demonstrates the pick (move) semantics for ssh objects.&]
[s7; &]
[s7; void[@0  SshPick(SshSession]`&[@0  session)]&]
[s7; `{&]
[s7;     SshSession psession [@(0.0.255) `=][@0  pick(session);    ][/@4 // 
All Ssh`-based objects are pickable.]&]
[s7;     [*@(0.0.255) if][@0 (][@(0.0.255) !][@0 session)]&]
[s7;         [@4 LOG][@0 (][@3 `"SshSession object is picked.`"][@0 );]&]
[s7;     SFtpGet(psession);&]
[s7; `}&]
[s5; &]
[s5; [* Key Points: ]&]
[s5;l192;i150;O0;~~~0; pick()[G  transfers ownership from one object 
to another]&]
[s5;l192;i150;O0;~~~0; The original object becomes invalid (null 
state)&]
[s5;l192;i150;O0;~~~0; Check validity with [C@5$(245) if(!object)] 
after picking&]
[s5;l192;i150;O0;~~~0; Useful for returning SSH objects from functions&]
[s5;l192;i150;O0;~~~0; Avoids expensive copying of sockets and internal 
state&]
[s22; 7.3 Polymorphism and RTTI&]
[s5; All SSH classes inherit from the [C@5$(245) Ssh] base class, enabling 
polymorphic storage and safe runtime type checking. &]
[s5; &]
[s7; #include[@0  ][@3 `"SshBasics.h`"]&]
[s7; &]
[s7; // SshPolymorphism:&]
[s7; // Demonstrates polymorphism and RTTI for Ssh objects.&]
[s7; &]
[s7; void[@0  SshPolymorphism(SshSession]`&[@0  session)]&]
[s7; `{&]
[s7;     [*@(0.0.255) constexpr][@0  ][*@(0.0.255) const][@0  ][*@(0.0.255) char][@0  
][@(0.0.255) `*][@0 path ][@(0.0.255) `=][@0  ][@3 `"/readme.txt`"][@0 ;]&]
[s7;     &]
[s7;     Array[@(0.0.255) <][@0 Ssh][@(0.0.255) >][@0  channels;]&]
[s7;     &]
[s7;     channels[@(0.0.255) .][@0 Create][@(0.0.255) <][@0 Scp][@(0.0.255) >][@0 (session);]&]
[s7;     channels[@(0.0.255) .][@0 Create][@(0.0.255) <][@0 SFtp][@(0.0.255) >][@0 (session);]&]
[s7;     channels[@(0.0.255) .][@0 Create][@(0.0.255) <][@0 SshExec][@(0.0.255) >][@0 (session);]&]
[s7; &]
[s7;     [*@(0.0.255) for][@0 (Ssh][@(0.0.255) `&][@0  channel ][@(0.0.255) :][@0  
channels)`{]&]
[s7;         [*@(0.0.255) if][@0 (channel][@(0.0.255) .][@0 Is][@(0.0.255) <][@0 Scp][@(0.0.255) >][@0 (
)) `{]&]
[s7;             [@4 LOG][@0 (][@3 `"][@(0.0.255) `\n][@3 Found: Scp object`"][@0 );]&]
[s7;             [@4 LOG][@0 (][@3 `"`-`-`-`-`-`-`-`-`-`-`-`-`-`-`-`-`-][@(0.0.255) `\n][@3 `"][@0 )
;]&]
[s7;             [@4 LOG][@0 (channel][@(0.0.255) .][@0 To][@(0.0.255) <][@0 Scp][@(0.0.255) >][@0 ()
][@(0.0.255) .][@0 LoadFile(path));]&]
[s7;         `}&]
[s7;         [*@(0.0.255) else]&]
[s7;         [*@(0.0.255) if][@0 (channel][@(0.0.255) .][@0 Is][@(0.0.255) <][@0 SFtp][@(0.0.255) >
][@0 ()) `{]&]
[s7;             [@4 LOG][@0 (][@3 `"][@(0.0.255) `\n][@3 Found: Sftp object`"][@0 );]&]
[s7;             [@4 LOG][@0 (][@3 `"`-`-`-`-`-`-`-`-`-`-`-`-`-`-`-`-`-`-][@(0.0.255) `\n][@3 `"
][@0 );]&]
[s7;             [@4 LOG][@0 (channel][@(0.0.255) .][@0 To][@(0.0.255) <][@0 SFtp][@(0.0.255) >][@0 (
)][@(0.0.255) .][@0 GetInfo(path)][@(0.0.255) .][@0 GetName());]&]
[s7;         `}&]
[s7;         [*@(0.0.255) else]&]
[s7;         [*@(0.0.255) if][@0 (channel][@(0.0.255) .][@0 Is][@(0.0.255) <][@0 SshExec][@(0.0.255) >
][@0 ()) `{]&]
[s7;             [@4 LOG][@0 (][@3 `"][@(0.0.255) `\n][@3 Found: Exec object`"][@0 );]&]
[s7;             [@4 LOG][@0 (][@3 `"`-`-`-`-`-`-`-`-`-`-`-`-`-`-`-`-`-`-][@(0.0.255) `\n][@3 `"
][@0 );]&]
[s7;             String out, err;&]
[s7;             channel[@(0.0.255) .][@0 To][@(0.0.255) <][@0 SshExec][@(0.0.255) >][@0 ()][@(0.0.255) .
][@0 Execute(][@3 `"ls `-l`"][@0 , out, err);]&]
[s7;             [@4 LOG][@0 (out);]&]
[s7;             [@4 LOG][@0 (err);]&]
[s7;         `}&]
[s7;         [*@(0.0.255) if][@0 (channel][@(0.0.255) .][@0 IsError()) `{]&]
[s7;             [@4 LOG][@0 (][@3 `"Operation failed. Reason: `"][@0  ][@(0.0.255) <<][@0  
channel][@(0.0.255) .][@0 GetErrorDesc());]&]
[s7;         `}&]
[s7;     &]
[s7;     `}&]
[s7; `}&]
[s5; &]
[s5; [* Key Points: ]&]
[s5;l192;i150;O0; Store different SSH types in the same container&]
[s5;l192;i150;O0; [C Create<T>()][G  constructs objects of specific types]&]
[s5;l192;i150;O0; Is<T>()[G  checks runtime type without casting]&]
[s5;l192;i150;O0; To<T>()[G  performs a safe cast to the specific type]&]
[s5;l192;i150;O0; Each object maintains its error state independently&]
[s5;l192;i150;O0; Useful for managing multiple connections or operations&]
[s22; 7.4 Verbose Logging&]
[s5; For deep debugging of SSH protocol issues, enable libssh2`'s 
internal tracing with fine`-grained control over what to log.&]
[s5; &]
[s7; #include[@0  ][@3 `"SshBasics.h`"]&]
[s7; &]
[s7; // TraceVerbose:&]
[s7; // To activate verbose logging, you need to set the LIBSSH2TRACE 
flag via&]
[s7; // TheIDE`->Main Configuration settings.&]
[s7; &]
[s7; void[@0  TraceVerbose()]&]
[s7; `{&]
[s7;     Ssh[@(0.0.255) `::][@0 TraceVerbose(]&]
[s7; //      LIBSSH2`_TRACE`_SOCKET    `|&]
[s7; //      LIBSSH2`_TRACE`_KEX       `|&]
[s7;         LIBSSH2`_TRACE`_AUTH      [@(0.0.255) `|]&]
[s7;         LIBSSH2`_TRACE`_CONN      [@(0.0.255) `|]&]
[s7; //      LIBSSH2`_TRACE`_SCP       `|&]
[s7; //      LIBSSH2`_TRACE`_SFTP      `|&]
[s7; //      LIBSSH2`_TRACE`_PUBLICKEY `|&]
[s7;         LIBSSH2`_TRACE`_ERROR&]
[s7;     );&]
[s7; `}&]
[s5; &]
[s5; Key Points: &]
[s5;l192;i150;O0; Must compile with [@(0.0.255) LIBSSH2TRACE] flag 
enabled&]
[s5;l192;i150;O0; Combine flags with [C@(0.0.255) `|] to trace multiple 
subsystems&]
[s5;l192;i150;O0; [C@(0.0.255) LIBSSH2`_TRACE`_AUTH][G  shows authentication 
details]&]
[s5;l192;i150;O0; [C@(0.0.255) LIBSSH2`_TRACE`_CONN ][G shows connection 
events]&]
[s5;l192;i150;O0; [C@(0.0.255) LIBSSH2`_TRACE`_ERROR ][G logs all errors]&]
[s5;l192;i150;O0; Comment/uncomment lines to control verbosity&]
[s5;l192;i150;O0; Produces very detailed output (use for troubleshooting 
only)&]
[s3;:8: 8. Best Practices&]
[s22; 8.1 Error Handling&]
[s5; Always check for errors after SSH operations: &]
[s5; &]
[s7; SFtp sftp(session);&]
[s7; String file [@(0.0.255) `=][@0  sftp][@(0.0.255) .][@0 LoadFile(][@3 `"/path`"][@0 );]&]
[s7; if[@0 (sftp].[@0 IsError()) `{]&]
[s7;     [@4 LOG][@0 (][@3 `"Download failed: `"][@0  ][@(0.0.255) <<][@0  sftp][@(0.0.255) .][@0 GetE
rrorDesc());]&]
[s7;     [*_@(128.0.255) return][@0 ;]&]
[s7; `}&]
[s5; &]
[s22; 8.2 Resource Management&]
[s5; The package uses RAII, so resources are automatically cleaned 
up:&]
[s5; &]
[s7; `{&]
[s7;     SshSession session;&]
[s7;     session[@(0.0.255) .][@0 Connect(][@3 `"host:22`"][@0 );]&]
[s7;     &]
[s7;     `{&]
[s7;         SFtp sftp(session);&]
[s7;         sftp[@(0.0.255) .][@0 LoadFile(][@3 `"/file`"][@0 );]&]
[s7;         [/@4 // SFtp automatically closed here]&]
[s7;     `}&]
[s7;     &]
[s7;     [/@4 // Session automatically closed here]&]
[s7; `}&]
[s0; &]
[s22; 8.3 Timeout Configuration&]
[s5; Set appropriate timeouts for different operations: &]
[s5; &]
[s7; session[@(0.0.255) .]Timeout([@3 5000])[@(0.0.255) .]Connect([@3 `"host:22`"]); 
 [/@4 // 5s for connection]&]
[s7; &]
[s7; SFtp sftp(session);&]
[s7; sftp[@(0.0.255) .][@0 Timeout(][@3 300000][@0 );  ][/@4 // 5 minutes for 
large transfers]&]
[s7; &]
[s7; SshShell shell(session);&]
[s7; shell[@(0.0.255) .]Timeout([@(0.128.128) Null]);  [/@4 // No timeout 
for interactive use]&]
[s0;/@4 &]
[s22; 8.4 Thread Safety&]
[s5; Each session is independent and thread`-safe. Multiple threads 
can use separate sessions simultaneously:&]
[s5; &]
[s7; void[@0  ParallelSessions()]&]
[s7; `{&]
[s7;     Array[@(0.0.255) <][@0 Thread][@(0.0.255) >][@0  threads;]&]
[s7;     &]
[s7;     [*@(0.0.255) for][@0 (][*@(0.0.255) int][@0  i ][@(0.0.255) `=][@0  
][@3 0][@0 ; i ][@(0.0.255) <][@0  ][@3 4][@0 ; i][@(0.0.255) `+`+][@0 ) `{]&]
[s7;         threads[@(0.0.255) .][@0 Add()][@(0.0.255) .][@0 Run(][@(0.0.255) `[`=`]][@0  
`{]&]
[s7;             SshSession session;&]
[s7;             session[@(0.0.255) .][@0 Connect(][@3 `"host:22`"][@0 );]&]
[s7;             [/@4 // Each thread has its own session]&]
[s7;         `});&]
[s7;     `}&]
[s7;     &]
[s7;     [*@(0.0.255) for][@0 (Thread][@(0.0.255) `&][@0  t ][@(0.0.255) :][@0  
threads)]&]
[s7;         t[@(0.0.255) .][@0 Wait();]&]
[s7; `}&]
[s22; 8.5 Memory Efficiency&]
[s5; For large files, always use streaming:&]
[s5; &]
[s7; // Bad: loads entire file into memory, fails if larger than 2GB&]
[s7; String huge [@(0.0.255) `=][@0  sftp][@(0.0.255) .][@0 LoadFile(][@3 `"/Min2GB`_file.iso`"
][@0 );]&]
[s7; &]
[s7; // Good: stream in chunks&]
[s7; SFtpFileIn fi(sftp, [@3 `"/Min2GB`_file.iso`"][@0 );]&]
[s7; while[@0 (]![@0 fi].[@0 IsEof()) `{]&]
[s7;     String chunk [@(0.0.255) `=][@0  fi][@(0.0.255) .][@0 Get(][@3 65536][@0 ); 
 ][/@4 // 64KB chunks]&]
[s7;     [/@4 // Process chunk]&]
[s0; [C0 `}]&]
[s5;C0 &]
[s3;:9: 9. Summary&]
[s5; The U`+`+/SSH package provides a robust, safe, and efficient 
way to handle SSH operations in C`+`+. Key takeaways: &]
[s5;l192;i150;O0; RAII design ensures automatic resource cleanup&]
[s5;l192;i150;O0; Type`-safe API prevents common errors&]
[s5;l192;i150;O0; Thread`-safe for parallel operations&]
[s5;l192;i150;O0; Comprehensive error handling with exceptions&]
[s5;l192;i150;O0; Flexible timeout management&]
[s5;l192;i150;O0; Support for all major SSH features: exec, shell, 
SFTP, SCP, tunneling&]
[s5; Start with simple examples and gradually explore more advanced 
features as your application needs grow.&]
[s0;b20;t300;G &]
[s3;:10: 10. Additional Resources&]
[s5;l192;i150;O0; U`+`+ Documentation: [^https`:`/`/www`.ultimatepp`.org`/^ https://www.u
ltimatepp.org/]&]
[s5;l192;i150;O0; U`+`+ Forums: Community support and examples&]
[s5;l192;i150;O0; libssh2 Reference: [^https`:`/`/www`.libssh2`.org`/^ https://www.libssh
2.org/]&]
[s0; &]
[s3;:11: 11. Note on Test Server&]
[s5; The examples use the public test server [C@(0.0.255) test.rebex.net] 
with credentials [C@(0.0.255) demo:password]. Note that SCP, X11 
forwarding, and TCP`-IP/port forwarding examples will not work 
with this public server. To test these features, set up a local 
SSH server (e.g., OpenSSH) for testing purposes. &]
[s0;G ]]