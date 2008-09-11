topic "Application & environment utility";[ $$0,0#00000000000000000000000000000000:Default]
[i448;a25;kKO9;*@(64)2 $$1,0#37138531426314131252341829483370:item]
[l288;2 $$2,0#27521748481378242620020725143825:desc]
[*@(154.0.0)+109 $$3,3#19357814472219620448494467520139:head]
[H6;0 $$4,4#05600065144404261032431302351956:breakline]
[0 $$5,4#96390100711032703541132217272105:breakhead]
[{_}%EN-US 
[s3; [@3;4 Application `& environment utility]&]
[s0; [2 A number of utility functions to obtain information about your 
application and system environment.]&]
[s0;+80 &]
[s0; [*2 Implemented in:]&]
[s0;%- [%%2 -|][2 Core`\App.][@(0.0.255)2 h]&]
[s0;%- [2 -|Core`\App.][@(0.0.255)2 cpp]&]
[s0; &]
[ {{10000F(128)G(128)@1 [s0; [*@2 Function List]]}}&]
[s0;2 &]
[s1;K:`:`:GetExeFilePath`(`):%- [^topic`:`/`/Core`/src`/String`$en`-us^ String]_[@0 GetEx
eFilePath]()&]
[s2;# Returns the absolute path towards the application`'s binary.&]
[s5; &]
[s4; &]
[s1;K:`:`:GetExeDirFile`(const char`*`):%- [^topic`:`/`/Core`/src`/String`$en`-us^ Stri
ng]_[@0 GetExeDirFile]([@(0.0.255) const]_[@(0.0.255) char]_`*[@3 fp])&]
[s2;# Returns the absolute path of a file that is found in the folder 
where you application`'s binary resides.&]
[s2; [@(129.0.0) fp..................................................]the 
name of the file&]
[s5; &]
[s4; &]
[s1;%- [^topic`:`/`/Core`/src`/String`$en`-us^ String]_[@0 GetHomeDirectory]()&]
[s2; Returns the absolute path of current user`'s home folder.&]
[s5; &]
[s4; &]
[s1;%- [^topic`:`/`/Core`/src`/String`$en`-us^ String]_[@0 GetHomeDirFile]([@(0.0.255) cons
t]_[@(0.0.255) char]_`*[@3 fp])&]
[s2;# Returns the absolute path of a file that is found in the current 
user`'s home folder.&]
[s2; [@(129.0.0) fp..................................................]the 
name of the file&]
[s5; &]
[s4; &]
[s1;%- [^topic`:`/`/Core`/src`/String`$en`-us^ String]_[@0 GetExeTitle]()&]
[s2; Returns the title of the application.&]
[s5; &]
[s4; &]
[s1;%- [@(0.0.255) void]_[@0 UseHomeDirectoryConfig]([@(0.0.255) bool]_[@3 b]_`=_[@(0.0.255) tr
ue])&]
[s2;# Sets the search path for configuration to either the current 
users`' home folder or to the folder where the application`'s 
binary resides.&]
[s2; [*@(129.0.0) b]...................................................use 
home folder (true) or application folder (false)&]
[s5; &]
[s4; &]
[s1;%- [^topic`:`/`/Core`/src`/String`$en`-us^ String]_[@0 ConfigFile]([@(0.0.255) const]_[@(0.0.255) c
har]_`*[@3 file])&]
[s2;# Returns the absolute path of a configuration file found in 
the configuration files search path.&]
[s2; [@(129.0.0) file ]...............................................the 
name of the file&]
[s5; &]
[s4; &]
[s1;%- [^topic`:`/`/Core`/src`/String`$en`-us^ String]_[@0 ConfigFile]()&]
[s2;# Returns the default configuration file from the configuration 
files search path. The default configuration file is called [*/ `[application 
title`]][* .cfg].&]
[s5;# &]
[s4; &]
[s1;%- [@(0.0.255) const]_[^topic`:`/`/Core`/src`/Vector`$en`-us^ Vector]<[^`:`:String^ Str
i][^topic`:`/`/Core`/src`/String`$en`-us^ n][^`:`:String^ g]>`&_[@0 CommandLine]()&]
[s2; Return a vector of strings containing the command line parameters.&]
[s5; &]
[s4; &]
[s1;%- [@(0.0.255) const]_[^topic`:`/`/Core`/src`/VectorMap`$en`-us^ VectorMap]<[^topic`:`/`/Core`/src`/String`$en`-us^ S
tring], [^topic`:`/`/Core`/src`/String`$en`-us^ String]>`&_[@0 Environment]()&]
[s2; Returns a vector of strings with system environment variables.&]
[s5; &]
[s4; &]
[s1;%- [@(0.0.255) void]_[@0 SetExitCode]([@(0.0.255) int]_[@3 code])&]
[s2; Sets the exit code that will be returned on application exit.&]
[s2; [*@(129.0.0) code].............................................the 
exit code&]
[s5; &]
[s4; &]
[s1;%- [@(0.0.255) int]_[@0 GetExitCode]()&]
[s2;# Gets the exit code that will be returned on application exit.&]
[s5;# &]
[s4; &]
[s1;%- [@(0.0.255) bool]_[@0 IsMainRunning]()&]
[s2;# Return true if the applications execution has reached and not 
yet exited the main function.&]
[s5;# &]
[s4;# &]
[s1;%- [^topic`:`/`/Core`/src`/String`$en`-us^ String]_[@0 GetDataFile]([@(0.0.255) const]_
[@(0.0.255) char]_`*[@3 filename])&]
[s2; [*_@(85.127.0) TODO]&]
[s2; [*@(129.0.0) filename].......................................the 
name of the file&]
[s5; &]
[s4; &]
[s1;%- [@(0.0.255) void]_[@0 LaunchWebBrowser]([@(0.0.255) const]_[^topic`:`/`/Core`/src`/String`$en`-us^ S
tring]`&_[@3 url])&]
[s2; Opens an URL in the default Web Browser.&]
[s2; [*@(129.0.0) url................................................]the 
URL that will be opened in the browser&]
[s0; &]
[s0; &]
[ {{10000F(128)G(128)@(226.85.0) [s0; [*@2 Function List (POSIX)]]}}&]
[s0; &]
[s1;K:`:`:SetHomeDirectory`(const char`*`):%- [@(0.0.255) void]_[@0 SetHomeDirectory]([@(0.0.255) c
onst]_[@(0.0.255) char]_`*[@3 dir])&]
[s2;# Sets the current user`'s home folder.&]
[s2; [*@(129.0.0) dir][@(129.0.0) ................................................]the 
URL that will be opened in the browser&]
[s0; &]
[s0; ]