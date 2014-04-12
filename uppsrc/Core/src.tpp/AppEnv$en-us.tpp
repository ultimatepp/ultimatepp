topic "Application & environment utility";
[2 $$0,0#00000000000000000000000000000000:Default]
[i448;a25;kKO9;2 $$1,0#37138531426314131252341829483380:class]
[l288;2 $$2,0#27521748481378242620020725143825:desc]
[0 $$3,0#96390100711032703541132217272105:end]
[H6;0 $$4,0#05600065144404261032431302351956:begin]
[i448;a25;kKO9;2 $$5,0#37138531426314131252341829483370:item]
[l288;a4;*@5;1 $$6,6#70004532496200323422659154056402:requirement]
[l288;i1121;b17;O9;~~~.1408;2 $$7,0#10431211400427159095818037425705:param]
[i448;b42;O9;2 $$8,8#61672508125594000341940100500538:tparam]
[b42;2 $$9,9#13035079074754324216151401829390:normal]
[{_} 
[ {{10000@(113.42.0) [s0;%% [*@7;4 Application `& environment utility]]}}&]
[s9;%% A number of utility functions to obtain information about 
your application and system environment.&]
[s3; &]
[s0;%% &]
[ {{10000F(128)G(128)@1 [s0;%% [* Function List]]}}&]
[s3; &]
[s5;:GetExeFilePath`(`): [_^String^ String]_[* GetExeFilePath]()&]
[s2;%% Returns the absolute path towards the application`'s binary.&]
[s3; &]
[s4; &]
[s5;:GetEnv`(const char`*`): [_^String^ String]_[* GetEnv]([@(0.0.255) const]_[@(0.0.255) cha
r]_`*[*@3 id])&]
[s2;%% Returns the value of the environment variable [*@3 id].&]
[s3;%% &]
[s4; &]
[s5;:GetExeDirFile`(const char`*`): [_^String^ String]_[* GetExeDirFile]([@(0.0.255) const]_
[@(0.0.255) char]_`*[*@3 fp])&]
[s2;%% Returns the absolute path of a file that is found in the folder 
where you application`'s binary resides.&]
[s2;%% [*@(129.0.0) fp][@(129.0.0) ..................................................]the
 name of the file&]
[s3; &]
[s4; &]
[s5;:GetHomeDirectory`(`): [_^String^ String]_[* GetHomeDirectory]()&]
[s2;%% Returns the absolute path of current user`'s home folder.&]
[s3; &]
[s4; &]
[s5;:GetHomeDirFile`(const char`*`): [_^String^ String]_[* GetHomeDirFile]([@(0.0.255) cons
t]_[@(0.0.255) char]_`*[*@3 fp])&]
[s2;%% Returns the absolute path of a file that is found in the current 
user`'s home folder.&]
[s7;%% [*@3 fp]..................................................the 
name of the file&]
[s3; &]
[s4; &]
[s5;:GetExeTitle`(`): [_^String^ String]_[* GetExeTitle]()&]
[s2;%% Returns the title of the application.&]
[s3; &]
[s4; &]
[s5;:UseHomeDirectoryConfig`(bool`): [@(0.0.255) void]_[* UseHomeDirectoryConfig]([@(0.0.255) b
ool]_[*@3 b]_`=_[@(0.0.255) true])&]
[s2;%% Sets the search path for configuration to either the current 
users`' home folder or to the folder where the application`'s 
binary resides.&]
[s2;%% [*@(129.0.0) b]...................................................use 
home folder (true) or application folder (false)&]
[s3; &]
[s4; &]
[s5;:ConfigFile`(const char`*`): [_^String^ String]_[* ConfigFile]([@(0.0.255) const]_[@(0.0.255) c
har]_`*[*@3 file])&]
[s2;%% Returns the absolute path of a configuration file found in 
the configuration files search path.&]
[s2;%% [*@(129.0.0) file]...............................................the 
name of the file&]
[s3; &]
[s4; &]
[s5;:ConfigFile`(`): [_^String^ String]_[* ConfigFile]()&]
[s2;%% Returns the default configuration file from the configuration 
files search path. The default configuration file is called [*/ `[application 
title`]][* .cfg].&]
[s3; &]
[s4; &]
[s5;:CommandLine`(`): [@(0.0.255) const]_[_^Vector^ Vector][@(0.0.255) <][_^String^ String][@(0.0.255) >
`&]_[* CommandLine]()&]
[s2;%% Return a vector of strings containing the command line parameters.&]
[s3; &]
[s4; &]
[s5;:Environment`(`): [@(0.0.255) const]_[_^VectorMap^ VectorMap][@(0.0.255) <][_^String^ Str
ing], [_^String^ String][@(0.0.255) >`&]_[* Environment]()&]
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
[s5;:GetDataFile`(const char`*`): [_^String^ String]_[* GetDataFile]([@(0.0.255) const]_[@(0.0.255) c
har]_`*[*@3 filename])&]
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
[s5;:LoadDataFile`(const char`*`): [_^String^ String]_[* LoadDataFile]([@(0.0.255) const]_[@(0.0.255) c
har]_`*[*@3 filename])&]
[s2;%% Same as LoadFile(GetDataFile([%-*@3 filename])).&]
[s3;%% &]
[s4; &]
[s5;:SetDataPath`(const char`*`): [@(0.0.255) void]_[* SetDataPath]([@(0.0.255) const]_[@(0.0.255) c
har]_`*[*@3 path])&]
[s2;%% Defines explicit directory to be used with GetDataFile.&]
[s3;%% &]
[s4; &]
[s5;:GetComputerName`(`): [_^String^ String]_[* GetComputerName]()&]
[s2;%% Returns the name of computer.&]
[s3; &]
[s4; &]
[s5;:GetUserName`(`): [_^String^ String]_[* GetUserName]()&]
[s2;%% Returns current user.&]
[s3; &]
[s4; &]
[s5;:GetDesktopManager`(`): [_^String^ String]_[* GetDesktopManager]()&]
[s2;%% Returns current GUI identifier. Predefined values are `"windows`", 
`"gnome`", `"kde`". If X11 desktop manager is not in this list, 
the value of `"DESKTOP`_SESSION`" environment variable is returned.&]
[s3; &]
[s4; &]
[s5;:LaunchWebBrowser`(const String`&`): [@(0.0.255) void]_[* LaunchWebBrowser]([@(0.0.255) c
onst]_[_^String^ String][@(0.0.255) `&]_[*@3 url])&]
[s2;%% Opens an URL in the default Web Browser.&]
[s2;%% [*@(129.0.0) url]................................................the 
URL that will be opened in the browser&]
[s3; &]
[s4; &]
[s5;:SetHomeDirectory`(const char`*`): [@(0.0.255) void]_[* SetHomeDirectory]([@(0.0.255) c
onst]_[@(0.0.255) char]_`*[*@3 dir])&]
[s2; [*@3 POSIX specific]&]
[s2;%% Sets the current user`'s home folder.&]
[s2;%% [*@(129.0.0) dir]................................................the 
URL that will be opened in the browser&]
[s3;%% &]
[s4; &]
[s5;:CpuMMX`(`): [@(0.0.255) bool]_[* CpuMMX]()&]
[s2;%% Returns true if CPU has MMX support.&]
[s3; &]
[s4; &]
[s5;:min`(const T`&`,const T`&`): [@(0.0.255) template]_<[@(0.0.255) class]_[*@4 T]>_[@(0.0.255) c
onst]_[*@4 T][@(0.0.255) `&]_[* min]([@(0.0.255) const]_[*@4 T][@(0.0.255) `&]_[*@3 a], 
[@(0.0.255) const]_[*@4 T][@(0.0.255) `&]_[*@3 b])&]
[s5;:min`(const T`&`,const T`&`,const T`&`): [@(0.0.255) template]_<[@(0.0.255) class]_[*@4 T
]>_[@(0.0.255) const]_[*@4 T][@(0.0.255) `&]_[* min]([@(0.0.255) const]_[*@4 T][@(0.0.255) `&]_
[*@3 a], [@(0.0.255) const]_[*@4 T][@(0.0.255) `&]_[*@3 b], [@(0.0.255) const]_[*@4 T][@(0.0.255) `&
]_[*@3 c])&]
[s5;:min`(const T`&`,const T`&`,const T`&`,const T`&`): [@(0.0.255) template]_<[@(0.0.255) c
lass]_[*@4 T]>_[@(0.0.255) const]_[*@4 T][@(0.0.255) `&]_[* min]([@(0.0.255) const]_[*@4 T][@(0.0.255) `&
]_[*@3 a], [@(0.0.255) const]_[*@4 T][@(0.0.255) `&]_[*@3 b], [@(0.0.255) const]_[*@4 T][@(0.0.255) `&
]_[*@3 c], [@(0.0.255) const]_[*@4 T][@(0.0.255) `&]_[*@3 d])&]
[s2;%% Returns the least of all arguments.&]
[s3;%% &]
[s4; &]
[s5;:max`(const T`&`,const T`&`): [@(0.0.255) template]_<[@(0.0.255) class]_[*@4 T]>_[@(0.0.255) c
onst]_[*@4 T][@(0.0.255) `&]_[* max]([@(0.0.255) const]_[*@4 T][@(0.0.255) `&]_[*@3 a], 
[@(0.0.255) const]_[*@4 T][@(0.0.255) `&]_[*@3 b])&]
[s5;:max`(const T`&`,const T`&`,const T`&`): [@(0.0.255) template]_<[@(0.0.255) class]_[*@4 T
]>_[@(0.0.255) const]_[*@4 T][@(0.0.255) `&]_[* max]([@(0.0.255) const]_[*@4 T][@(0.0.255) `&]_
[*@3 a], [@(0.0.255) const]_[*@4 T][@(0.0.255) `&]_[*@3 b], [@(0.0.255) const]_[*@4 T][@(0.0.255) `&
]_[*@3 c])&]
[s5;:max`(const T`&`,const T`&`,const T`&`,const T`&`): [@(0.0.255) template]_<[@(0.0.255) c
lass]_[*@4 T]>_[@(0.0.255) const]_[*@4 T][@(0.0.255) `&]_[* max]([@(0.0.255) const]_[*@4 T][@(0.0.255) `&
]_[*@3 a], [@(0.0.255) const]_[*@4 T][@(0.0.255) `&]_[*@3 b], [@(0.0.255) const]_[*@4 T][@(0.0.255) `&
]_[*@3 c], [@(0.0.255) const]_[*@4 T][@(0.0.255) `&]_[*@3 d])&]
[s2;%% Returns the greatest of all arguments.&]
[s3;%% &]
[s4; &]
[s5;:minmax`(T`,T`,T`): [@(0.0.255) template]_<[@(0.0.255) class]_[*@4 T]>_[*@4 T]_[* minmax]([*@4 T
]_[*@3 x], [*@4 T]_[*@3 `_min], [*@4 T]_[*@3 `_max])&]
[s5;:clamp`(T`,T`,T`): [@(0.0.255) template]_<[@(0.0.255) class]_[*@4 T]>_[*@4 T]_[* clamp]([*@4 T
]_[*@3 x], [*@4 T]_[*@3 `_min], [*@4 T]_[*@3 `_max])&]
[s2;%% Returns [%-*@3 x] if it is between [%-*@3 `_min] and [%-*@3 `_max].&]
[s2; [%% If ][*@3 x][%%  is lower than ][*@3 `_min][%%  it returns ][*@3 `_min.]&]
[s0;l288; [%% If ][*@3 x][%%  is higher than ][*@3 `_max][%%  it returns ][*@3 `_max.]&]
[s3;%% &]
[s4; &]
[s5;:findarg`(const T`&`,const T1`&`,`.`.`.`): [@(0.0.255) template]_<[@(0.0.255) class]_
[*@4 T], [@(0.0.255) class]_[*@4 T1], ...>_[@(0.0.255) int]_[* findarg]([@(0.0.255) const]_[*@4 T
][@(0.0.255) `&]_[*@3 x], [@(0.0.255) const]_[*@4 T1][@(0.0.255) `&]_[*@3 p0], 
...)&]
[s2;%% Searches the list of arguments for the value of [%-*@3 x]. If 
it is found, function returns the index of argument (starting 
with 0). If not found, `-1 is returned.&]
[s3;%% &]
[s4; &]
[s5;:decode`(const T`&`,const T1`&`,const V1`&`,`.`.`.`): [@(0.0.255) template]_<[@(0.0.255) c
lass]_[*@4 T], [@(0.0.255) class]_[*@4 T1], [@(0.0.255) class]_[*@4 V1], 
..., [@(0.0.255) class]_[*@4 D]>&]
[s5; [*@4 D]_[* decode]([@(0.0.255) const]_[*@4 T][@(0.0.255) `&]_[*@3 x], [@(0.0.255) const]_[*@4 T1
][@(0.0.255) `&]_[*@3 p0], [@(0.0.255) const]_[*@4 V1][@(0.0.255) `&]_[*@3 v0], 
..., [@(0.0.255) const]_[*@4 D][@(0.0.255) `&]_[*@3 def])&]
[s5;:decode`(const T`&`,const T1`&`,const char`*`,`.`.`.`): [@(0.0.255) template]_<[@(0.0.255) c
lass]_[*@4 T], [@(0.0.255) class]_[*@4 T1], ...>&]
[s5; [@(0.0.255) const]_[@(0.0.255) char]_`*[* decode]([@(0.0.255) const]_[*@4 T][@(0.0.255) `&]_
[*@3 x], [@(0.0.255) const]_[*@4 T1][@(0.0.255) `&]_[*@3 p0], [@(0.0.255) const]_[@(0.0.255) ch
ar]_`*[*@3 v0], ..., [@(0.0.255) const]_[*@4 D][@(0.0.255) `&]_[*@3 def])&]
[s2;%% Searches the list of argument pairs for the value of [%-*@3 x] 
to be equal to the first argument in the pair. If found, returns 
the second argument of pair. If no match is found, returns the 
last argument [%-*@3 def].&]
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
[s5;:CPU`_Cores`(`): [@(0.0.255) int]_[* CPU`_Cores]()&]
[s2;%% Returns the number of cores the CPU has.&]
[s3; &]
[s0; ]]