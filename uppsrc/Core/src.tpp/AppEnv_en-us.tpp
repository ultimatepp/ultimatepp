topic "Application & environment utility";
[i448;a25;kKO9;2 $$1,0#37138531426314131252341829483380:class]
[l288;2 $$2,0#27521748481378242620020725143825:desc]
[0 $$3,0#96390100711032703541132217272105:end]
[H6;0 $$4,0#05600065144404261032431302351956:begin]
[i448;a25;kKO9;2 $$5,0#37138531426314131252341829483370:item]
[l288;a4;*@5;1 $$6,6#70004532496200323422659154056402:requirement]
[l288;i1121;b17;O9;~~~.1408;2 $$7,0#10431211400427159095818037425705:param]
[i448;b42;O9;2 $$8,8#61672508125594000341940100500538:tparam]
[b42;2 $$9,9#13035079074754324216151401829390:normal]
[2 $$0,0#00000000000000000000000000000000:Default]
[{_} 
[ {{10000@(113.42.0) [s0;%% [*@7;4 Application `& environment utility]]}}&]
[s9;%% A number of utility functions to obtain information about 
your application and system environment.&]
[s3; &]
[s0;%% &]
[ {{10000F(128)G(128)@1 [s0;%% [* Function List]]}}&]
[s3; &]
[s5;:GetExeFilePath`(`): [_^topic`:`/`/Core`/src`/String`$en`-us`#String`:`:class^ Stri
ng]_[* GetExeFilePath]()&]
[s2;%% Returns the absolute path towards the application`'s binary.&]
[s3; &]
[s4; &]
[s5;:GetEnv`(const char`*`): [_^topic`:`/`/Core`/src`/String`$en`-us`#String`:`:class^ S
tring]_[* GetEnv]([@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 id])&]
[s2;%% Returns the value of the environment variable [*@3 id].&]
[s3;%% &]
[s4; &]
[s5;:Upp`:`:SetEnv`(const char`*`,const char`*`): [@(0.0.255) bool] 
[* SetEnv]([@(0.0.255) const] [@(0.0.255) char] [@(0.0.255) `*][*@3 name], 
[@(0.0.255) const] [@(0.0.255) char] [@(0.0.255) `*][*@3 value])&]
[s2;%% Sets the value of the environment variable [%-*@3 name] to [%-*@3 value].&]
[s3; &]
[s4; &]
[s5;:GetExeDirFile`(const char`*`): [_^topic`:`/`/Core`/src`/String`$en`-us`#String`:`:class^ S
tring]_[* GetExeDirFile]([@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 fp])&]
[s2;%% Returns the absolute path of a file that is found in the folder 
where you application`'s binary resides.&]
[s2;%% [*@(129.0.0) fp][@(129.0.0) ..................................................]the
 name of the file&]
[s3; &]
[s4; &]
[s5;:GetHomeDirectory`(`): [_^topic`:`/`/Core`/src`/String`$en`-us`#String`:`:class^ St
ring]_[* GetHomeDirectory]()&]
[s2;%% Returns the absolute path of current user`'s home folder.&]
[s3; &]
[s4; &]
[s5;:GetHomeDirFile`(const char`*`): [_^topic`:`/`/Core`/src`/String`$en`-us`#String`:`:class^ S
tring]_[* GetHomeDirFile]([@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 fp])&]
[s2;%% Returns the absolute path of a file that is found in the current 
user`'s home folder.&]
[s7;%% [*@3 fp]..................................................the 
name of the file&]
[s3; &]
[s4; &]
[s5;:GetExeTitle`(`): [_^topic`:`/`/Core`/src`/String`$en`-us`#String`:`:class^ String]_
[* GetExeTitle]()&]
[s2;%% Returns the title of the application.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:GetAppFolder`(`): [_^Upp`:`:String^ String]_[* GetAppFolder]()&]
[s6; `[MacOS only`]&]
[s2;%% This funtion returns the path to the application folder, if 
the the executable is in one.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:GetAppName`(`): [_^Upp`:`:String^ String]_[* GetAppName]()&]
[s2;%% Returns the name of the application. Default value is GetExeTitle, 
but it can be changed with SetAppName. This values is used e.g. 
as the name of configuration folder.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:SetAppName`(const Upp`:`:String`&`): [@(0.0.255) void]_[* SetAppName]([@(0.0.255) c
onst]_[_^Upp`:`:String^ String][@(0.0.255) `&]_[*@3 name])&]
[s2;%% Overrides the name of the application.&]
[s3;%% &]
[s4; &]
[s5;:Upp`:`:SetConfigGroup`(const char`*`): [@(0.0.255) void]_[* SetConfigGroup]([@(0.0.255) c
onst]_[@(0.0.255) char]_`*[*@3 group])&]
[s2;%% In POSIX systems, ConfigFile function default path is inside 
`".config`" directory. This function allows to set a name of 
subdirectory of this directory where the another subdirectory 
named GetAppName() specific to the application will store the 
configuration. Setting group empty just removes whole level. 
Default value is `"u`+`+`".&]
[s3;%% &]
[s4; &]
[s5;:Upp`:`:GetConfigGroup`(`): [_^Upp`:`:String^ String]_[* GetConfigGroup]()&]
[s2;%% Returns current config group.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:GetTempDirectory`(`): [_^Upp`:`:String^ String]_[* GetTempDirectory]()&]
[s2;%% Return the temporary directory.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:TempFile`(const char`*`): [_^Upp`:`:String^ String]_[* TempFile]([@(0.0.255) co
nst]_[@(0.0.255) char]_`*[*@3 filename])&]
[s2;%% Returns the absolute path of a file in temporary directory.&]
[s3;%% &]
[s4; &]
[s5;:Upp`:`:GetExeFolder`(`): [_^topic`:`/`/Core`/src`/String`$en`-us`#String`:`:class^ S
tring]_[* GetExeFolder]()&]
[s2;%% Return the directory of application executable.&]
[s3; &]
[s4; &]
[s5;:UseHomeDirectoryConfig`(bool`): [@(0.0.255) void]_[* UseHomeDirectoryConfig]([@(0.0.255) b
ool]_[*@3 b]_`=_[@(0.0.255) true])&]
[s2;%% In Win32 sets the search path for configuration to either 
the current users`' home folder or to the folder where the application`'s 
binary resides.&]
[s2;%% [*@(129.0.0) b]...................................................use 
home folder (true) or application folder (false)&]
[s3; &]
[s4; &]
[s5;:Upp`:`:SetConfigDirectory`(const Upp`:`:String`&`): [@(0.0.255) void]_[* SetConfigDi
rectory]([@(0.0.255) const]_[_^Upp`:`:String^ String][@(0.0.255) `&]_[*@3 s])&]
[s2;%% Overrides the directory where configuration files are stored.&]
[s3;%% &]
[s4; &]
[s5;:Upp`:`:SetConfigName`(const String`&`): [@(0.0.255) void] [* SetConfigName]([@(0.0.255) c
onst] String[@(0.0.255) `&] [*@3 s])&]
[s2;%% Sets the name of configuration directory if applicable. If 
not specified, GetAppName() is used.&]
[s3; &]
[s4; &]
[s5;:ConfigFile`(const char`*`): [_^topic`:`/`/Core`/src`/String`$en`-us`#String`:`:class^ S
tring]_[* ConfigFile]([@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 file])&]
[s2;%% Returns the absolute path of a configuration file. The directory 
depends on host platform and other settings. If one was set with 
SetConfigDirectory, it is used. Otherwise, in POSIX, function 
searches for .config folder starting with directory of binary, 
then goes up until root. If none is found, XDG`_CONFIG`_HOME 
is used. If that does not exist, `~/.config is used. Function 
then appends GetAppName() to this directory and makes sure it 
exists (with RealizeDirectory). In Win32, this is either the 
directory of .exe file, or if UseHomeDirectoryConfig is set, 
user`'s home directory.&]
[s3; &]
[s4; &]
[s5;:ConfigFile`(`): [_^topic`:`/`/Core`/src`/String`$en`-us`#String`:`:class^ String]_
[* ConfigFile]()&]
[s2;%% Returns the default configuration file from the configuration 
files search path. The default configuration file is called [*/ `[GetAppName()`]][* .
cfg].&]
[s3; &]
[s4; &]
[s5;:CommandLine`(`): [@(0.0.255) const]_[_^topic`:`/`/Core`/src`/Vector`$en`-us`#Vector`:`:class^ V
ector][@(0.0.255) <][_^topic`:`/`/Core`/src`/String`$en`-us`#String`:`:class^ String][@(0.0.255) >
`&]_[* CommandLine]()&]
[s2;%% Return a vector of strings containing the command line parameters. 
This array does not contain main`'s argv`[0`], so the number 
of elements is argc `- 1.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:CommandLineRemove`(int`,int`): [@(0.0.255) void] [* CommandLineRemove]([@(0.0.255) i
nt] [*@3 i], [@(0.0.255) int] [*@3 count] [@(0.0.255) `=] [@3 1])&]
[s2;%% Removes commandline argument(s).&]
[s3; &]
[s4; &]
[s5;:Upp`:`:GetArgv0`(`): [_^Upp`:`:String^ String]_[* GetArgv0]()&]
[s2;%% Returns the argv`[0`] argument to main (contains the name 
of executable).&]
[s3; &]
[s4; &]
[s5;:Environment`(`): [@(0.0.255) const]_[_^topic`:`/`/Core`/src`/VectorMap`$en`-us`#VectorMap`:`:class^ V
ectorMap][@(0.0.255) <][_^topic`:`/`/Core`/src`/String`$en`-us`#String`:`:class^ Stri
ng], [_^topic`:`/`/Core`/src`/String`$en`-us`#String`:`:class^ String][@(0.0.255) >`&
]_[* Environment]()&]
[s2;%% Returns a vector of strings with system environment variables.&]
[s3; &]
[s4; &]
[s5;:SetExitCode`(int`): [@(0.0.255) void]_[* SetExitCode]([@(0.0.255) int]_[*@3 code])&]
[s2;%% Sets the exit code that will be returned on application exit.&]
[s2;%% [*@(129.0.0) code].............................................the 
exit code&]
[s3; &]
[s4; &]
[s5;:GetExitCode`(`): [@(0.0.255) int]_[* GetExitCode]()&]
[s2;%% Gets the exit code that will be returned on application exit.&]
[s3; &]
[s4; &]
[s5;:IsMainRunning`(`): [@(0.0.255) bool]_[* IsMainRunning]()&]
[s2;%% Return true if the applications execution has reached and 
not yet exited the main function.&]
[s3; &]
[s4; &]
[s5;:GetDataFile`(const char`*`): [_^topic`:`/`/Core`/src`/String`$en`-us`#String`:`:class^ S
tring]_[* GetDataFile]([@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 filename])&]
[s2;%% This function is designed to retrieve the path of file that 
contains application specific data. Behaviour depends on whether 
application was started from `'theide`' `- in that case theide 
passes a reference to main package directory through environment 
variable `"UPP`_MAIN`_`_`" and the resulting file is in this 
directory. When started standalone, the function is equivalent 
to GetExeDirFile. Alternatively, application can call SetDataPath 
to define the directory.&]
[s3; &]
[s4; &]
[s5;:LoadDataFile`(const char`*`): [_^topic`:`/`/Core`/src`/String`$en`-us`#String`:`:class^ S
tring]_[* LoadDataFile]([@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 filename])&]
[s2;%% Same as LoadFile(GetDataFile([%-*@3 filename])).&]
[s3;%% &]
[s4; &]
[s5;:SetDataPath`(const char`*`): [@(0.0.255) void]_[* SetDataPath]([@(0.0.255) const]_[@(0.0.255) c
har]_`*[*@3 path])&]
[s2;%% Defines explicit directory to be used with GetDataFile.&]
[s3;%% &]
[s4; &]
[s5;:GetComputerName`(`): [_^topic`:`/`/Core`/src`/String`$en`-us`#String`:`:class^ Str
ing]_[* GetComputerName]()&]
[s2;%% Returns the name of computer.&]
[s3; &]
[s4; &]
[s5;:GetUserName`(`): [_^topic`:`/`/Core`/src`/String`$en`-us`#String`:`:class^ String]_
[* GetUserName]()&]
[s2;%% Returns current user.&]
[s3; &]
[s4; &]
[s5;:GetDesktopManager`(`): [_^topic`:`/`/Core`/src`/String`$en`-us`#String`:`:class^ S
tring]_[* GetDesktopManager]()&]
[s2;%% Returns current GUI identifier. Predefined values are `"windows`", 
`"gnome`", `"kde`". If X11 desktop manager is not in this list, 
the value of `"DESKTOP`_SESSION`" environment variable is returned.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:IsUserAdmin`(`): [@(0.0.255) bool] [* IsUserAdmin]()&]
[s2;%% Return true if the current process is running as administrator 
(Windows) or root (POSIX).&]
[s3; &]
[s4; &]
[s5;:LaunchWebBrowser`(const String`&`): [@(0.0.255) void]_[* LaunchWebBrowser]([@(0.0.255) c
onst]_[_^topic`:`/`/Core`/src`/String`$en`-us`#String`:`:class^ String][@(0.0.255) `&
]_[*@3 url])&]
[s2;%% Opens an URL in the default Web Browser.&]
[s2;%% [*@(129.0.0) url]................................................the 
URL that will be opened in the browser&]
[s3; &]
[s4; &]
[s5;:SetHomeDirectory`(const char`*`): [@(0.0.255) void]_[* SetHomeDirectory]([@(0.0.255) c
onst]_[@(0.0.255) char]_`*[*@3 dir])&]
[s2;%% Sets the current user`'s home folder.&]
[s2;%% [*@(129.0.0) dir]................................................the 
URL that will be opened in the browser&]
[s3;%% &]
[s4; &]
[s5;:CpuMMX`(`): [@(0.0.255) bool]_[* CpuMMX]()&]
[s2;%% Returns true if CPU has MMX support.&]
[s3;%% &]
[s3;%% &]
[s4; &]
[s5;:CpuSSE`(`): [@(0.0.255) bool]_[* CpuSSE]()&]
[s2;%% Returns true if CPU has SSE support.&]
[s3; &]
[s4; &]
[s5;:CpuSSE2`(`): [@(0.0.255) bool]_[* CpuSSE2]()&]
[s2;%% Returns true if CPU has SSE2 support.&]
[s3; &]
[s4; &]
[s5;:CpuSSE3`(`): [@(0.0.255) bool]_[* CpuSSE3]()&]
[s2;%% Returns true if CPU has SSE3 support.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:CpuHypervisor`(`): [@(0.0.255) bool]_[* CpuHypervisor]()&]
[s2;%% Checks whether CPU has hypervisor flag set. If it has, the 
program is running in virtual machine. Unfortunately, opposite 
is not true `- some VMs run without this flag set.&]
[s3; &]
[s4; &]
[s5;:CPU`_Cores`(`): [@(0.0.255) int]_[* CPU`_Cores]()&]
[s2;%% Returns the number of cores the CPU has.&]
[s3; &]
[s0; ]]