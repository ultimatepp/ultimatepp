topic "Application & environment utility";[ $$0,0#00000000000000000000000000000000:Default]
[i448;a25;kKO9;*@(64)2 $$1,0#37138531426314131252341829483370:item]
[l288;2 $$2,0#27521748481378242620020725143825:desc]
[*@(154.0.0)+109 $$3,3#19357814472219620448494467520139:head]
[~~~=_8333; $$4,4#05600065144404261032431302351956:break]
[{_}%EN-US 
[s0;*@(68)2 Upp&]
[s0;*@(154.0.0)+117 Application `& environment utility&]
[s0;2 A number of utility functions to obtain information about your 
application and system environment.&]
[s0;+80 &]
[s0;*2 Implemented in:&]
[s0;@(0.0.255)2 [@0 -|Core`\App.]h&]
[s0;@(0.0.255)2 [@0 -|Core`\App.]cpp&]
[s0;@(0.0.255)2 &]
[ {{10000F(128)G(128)@1 [s0;*@2 Function Detail]}}&]
[s0;*@2 &]
[s3; GetExeFilePath&]
[s0;2 &]
[s1;K:`:`:GetExeFilePath`(`): [^topic`:`/`/Core`/src`/String`$en`-us^ String]_[@0 GetExeF
ilePath]()&]
[s2;# Returns the absolute path towards the application`'s binary.&]
[s4; -|&]
[s3; &]
[s3; GetExeDirFile&]
[s3; &]
[s1;K:`:`:GetExeDirFile`(const char`*`): [^topic`:`/`/Core`/src`/String`$en`-us^ String
]_[@0 GetExeDirFile]([@(0.0.255) const]_[@(0.0.255) char]_`*[@3 fp])&]
[s2;# Returns the absolute path of a file that is found in the folder 
where you application`'s binary resides.&]
[s2; [@(129.0.0) fp..................................................]the 
name of the file&]
[s4; -|&]
[s0; &]
[s3; GetHomeDirectory&]
[s3; &]
[s1;K:`:`:GetHomeDirectory`(`): [^topic`:`/`/Core`/src`/String`$en`-us^ String]_[@0 GetHo
meDirectory]()&]
[s2; Returns the absolute path of current user`'s home folder.&]
[s4; -|&]
[s4; &]
[s3; GetHomeDirFile&]
[s3;*@0;3 &]
[s1;K:`:`:GetHomeDirFile`(const char`*`): [^topic`:`/`/Core`/src`/String`$en`-us^ Strin
g]_[@0 GetHomeDirFile]([@(0.0.255) const]_[@(0.0.255) char]_`*[@3 fp])&]
[s2;# Returns the absolute path of a file that is found in the current 
user`'s home folder.&]
[s2; [@(129.0.0) fp..................................................]the 
name of the file&]
[s4; -|&]
[s4; &]
[s3; GetExeTitle&]
[s0;*2 &]
[s1;K:`:`:GetExeTitle`(`): [^topic`:`/`/Core`/src`/String`$en`-us^ String]_[@0 GetExeTitl
e]()&]
[s2; Returns the title of the application.&]
[s4; -|&]
[s4; &]
[s3; UseHomeDirectoryConfig&]
[s0;*2 &]
[s1;K:`:`:UseHomeDirectoryConfig`(bool`): [@(0.0.255) void]_[@0 UseHomeDirectoryConfig]([@(0.0.255) b
ool]_[@3 b]_`=_true)&]
[s2;# Sets the search path for configuration to either the current 
users`' home folder or to the folder where the application`'s 
binary resides.&]
[s2; [*@(129.0.0) b]...................................................use 
home folder (true) or application folder (false)&]
[s4; -|&]
[s4; &]
[s3; ConfigFile&]
[s0;*2 &]
[s1;K:`:`:ConfigFile`(const char`*`): [^topic`:`/`/Core`/src`/String`$en`-us^ String]_[@0 C
onfigFile]([@(0.0.255) const]_[@(0.0.255) char]_`*[@3 file])&]
[s2;# Returns the absolute path of a configuration file found in 
the configuration files search path.&]
[s2; [@(129.0.0) file ]...............................................the 
name of the file&]
[s4; -|&]
[s4; &]
[s3; ConfigFile&]
[s0; &]
[s1;K:`:`:ConfigFile`(`): [^topic`:`/`/Core`/src`/String`$en`-us^ String]_[@0 ConfigFile](
)&]
[s2;# Returns the default configuration file from the configuration 
files search path. The default configuration file is called [*/ `[application 
title`]][* .cfg].&]
[s4; -|&]
[s4; &]
[s3; CommandLine&]
[s0; &]
[s1;K:`:`:CommandLine`(`): [@(0.0.255) const]_[^topic`:`/`/Core`/src`/Vector`$en`-us^ Vec
tor]<[^`:`:String^ Stri][^topic`:`/`/Core`/src`/String`$en`-us^ n][^`:`:String^ g]>`&_[@0 C
ommandLine]()&]
[s2; Return a vector of strings containing the command line parameters.&]
[s4; -|&]
[s3; &]
[s3; Environment&]
[s0; &]
[s1;K:`:`:Environment`(`): [@(0.0.255) const]_[^topic`:`/`/Core`/src`/VectorMap`$en`-us^ V
ectorMap]<[^topic`:`/`/Core`/src`/String`$en`-us^ String], [^topic`:`/`/Core`/src`/String`$en`-us^ S
tring]>`&_[@0 Environment]()&]
[s2; Returns a vector of strings with system environment variables.&]
[s4; -|&]
[s4; &]
[s3; SetExitCode&]
[s3; &]
[s1;K:`:`:SetExitCode`(int`): [@(0.0.255) void]_[@0 SetExitCode]([@(0.0.255) int]_[@3 code])&]
[s2; Sets the exit code that will be returned on application exit.&]
[s2; [*@(129.0.0) code].............................................the 
exit code&]
[s4; -|&]
[s4; &]
[s3; GetExitCode&]
[s0; &]
[s1;K:`:`:GetExitCode`(`): [@(0.0.255) int]_[@0 GetExitCode]()&]
[s2;# Gets the exit code that will be returned on application exit.&]
[s4; -|&]
[s4;*2 &]
[s3; IsMainRunning&]
[s3; &]
[s1;K:`:`:IsMainRunning`(`): [@(0.0.255) bool]_[@0 IsMainRunning]()&]
[s2;# Return true if the applications execution has reached and not 
yet exited the main function.&]
[s4; -|&]
[s4; &]
[s3; GetDataFile&]
[s3; &]
[s1;K:`:`:GetDataFile`(const char`*`): [^topic`:`/`/Core`/src`/String`$en`-us^ String]_
[@0 GetDataFile]([@(0.0.255) const]_[@(0.0.255) char]_`*[@3 filename])&]
[s2;*_@(85.127.0) TODO&]
[s2; [*@(129.0.0) filename].......................................the 
name of the file&]
[s4; -|&]
[s3; &]
[s3; LaunchWebBrowser&]
[s4; &]
[s1;K:`:`:LaunchWebBrowser`(const`:`:String`&`): [@(0.0.255) void]_[@0 LaunchWebBrowser](
[@(0.0.255) const]_[^topic`:`/`/Core`/src`/String`$en`-us^ String]`&_[@3 url])&]
[s2; Opens an URL in the default Web Browser.&]
[s2; [*@(129.0.0) url................................................]the 
URL that will be opened in the browser&]
[s4; &]
[s4; &]
[s0; &]
[ {{10000F(128)G(128)@(226.85.0) [s0;*@2 Function Detail (POSIX)]}}&]
[s4; &]
[s1;K:`:`:SetHomeDirectory`(const char`*`): [@(0.0.255) void]_[@0 SetHomeDirectory]([@(0.0.255) c
onst]_[@(0.0.255) char]_`*[@3 dir])&]
[s2;# Sets the current user`'s home folder.&]
[s2; [*@(129.0.0) dir][@(129.0.0) ................................................]the 
URL that will be opened in the browser&]
[s0; &]
[s0; ]