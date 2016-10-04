topic "Application & environment utility";[ $$0,0#00000000000000000000000000000000:Default]
[i448;a25;kKO9;*@(64)2 $$1,0#37138531426314131252341829483370:item]
[l288;2 $$2,0#27521748481378242620020725143825:desc]
[{_}%EN-US 
[ {{10000@(170.127.200) [s0;4 Application `& environment utility]}}&]
[s0; A number of utility functions to obtain information about your 
application and system environment.&]
[s0; &]
[ {{10000@(204) [s0;@(0.0.255)2 [%00-00@0 Location: ][%00-00^topic`:`/`/Core`/^@0 Core][%00-00@0 `\App.][%00-00 h
][%00-00@0 , ][%00-00^topic`:`/`/Core`/^@0 Core][%00-00@0 `\App.][%00-00 cpp]]}}&]
[s0;* &]
[ {{10000@(170.170.255) [s0;* Functions]}}&]
[s1;l333;K:`:`:GetExeFilePath`(`): [^topic`:`/`/Core`/src`/String`$en`-us^ String]_[@0 Ge
tExeFilePath]()&]
[s0;l667;2 Returns the absolute path towards the application`'s binary 
executable&]
[s0;l333;2 &]
[s1;l333;i150;K:`:`:GetExeDirFile`(const char`*`): [^topic`:`/`/Core`/src`/String`$en`-us^ S
tring]_[@0 GetExeDirFile]([@(0.0.255) const]_[@(0.0.255) char]_`*[@3 fp])&]
[s0;l667;2 Returns the absolute path of a file that is found in the 
folder where you application`'s binary resides.&]
[s2;l500;* Parameters:&]
[s2;l667; [*@3 fp]: the name of the file&]
[s0;l667; &]
[s1;l333;K:`:`:GetHomeDirectory`(`): [^topic`:`/`/Core`/src`/String`$en`-us^ String]_[@0 G
etHomeDirectory]()&]
[s0;l667;2 Returns the absolute path of current user`'s home folder.&]
[s0;l667;2 &]
[s1;l333;K:`:`:GetHomeDirFile`(const char`*`): [^topic`:`/`/Core`/src`/String`$en`-us^ S
tring]_[@0 GetHomeDirFile]([@(0.0.255) const]_[@(0.0.255) char]_`*[@3 fp])&]
[s0;l667;2 Returns the absolute path of a file that is found in the 
current user`'s home folder.&]
[s2;l500;* Parameters:&]
[s0;l667;2 [*@3 fp]: the name of the file&]
[s0;l667; &]
[s1;l333;K:`:`:GetHomeDirectory`(`): [^`:`:String^ String]_[@0 GetHomeDirectory]()&]
[s2;l333;   &]
[s0;l333; &]
[s1;l333;K:`:`:SetHomeDirectory`(const char`*`): [@(0.0.255) void]_[@0 SetHomeDirectory](
[@(0.0.255) const]_[@(0.0.255) char]_`*[@3 dir])&]
[s2;l333;   [*@3 dir] &]
[s0;l333; &]
[s1;l333;K:`:`:GetExeTitle`(`): [^`:`:String^ String]_[@0 GetExeTitle]()&]
[s2;l333;   &]
[s0;l333; &]
[s1;l333;K:`:`:UseHomeDirectoryConfig`(bool`): [@(0.0.255) void]_[@0 UseHomeDirectoryConf
ig]([@(0.0.255) bool]_[@3 b]_`=_true)&]
[s2;l333;   [*@3 b] &]
[s0;l333; &]
[s1;l333;K:`:`:ConfigFile`(const char`*`): [^`:`:String^ String]_[@0 ConfigFile]([@(0.0.255) c
onst]_[@(0.0.255) char]_`*[@3 file])&]
[s2;l333;   [*@3 file] &]
[s0;l333; &]
[s1;l333;K:`:`:ConfigFile`(`): [^`:`:String^ String]_[@0 ConfigFile]()&]
[s2;l333;   &]
[s0;l333; &]
[s1;l333;K:`:`:CommandLine`(`): [@(0.0.255) const]_[^`:`:Vector^ Vector]<[^`:`:String^ Stri
ng]>`&_[@0 CommandLine]()&]
[s2;l333;   &]
[s0;l333; &]
[s1;l333;K:`:`:Environment`(`): [@(0.0.255) const]_[^`:`:VectorMap^ VectorMap]<[^`:`:String^ S
tring], [^`:`:String^ String]>`&_[@0 Environment]()&]
[s2;l333;   &]
[s0;l333; &]
[s1;l333;K:`:`:SetExitCode`(int`): [@(0.0.255) void]_[@0 SetExitCode]([@(0.0.255) int]_[@3 co
de])&]
[s2;l333;   [*@3 code] &]
[s0;l333; &]
[s1;l333;K:`:`:GetExitCode`(`): [@(0.0.255) int]_[@0 GetExitCode]()&]
[s2;l333;   &]
[s0;l333; &]
[s1;l333;K:`:`:IsMainRunning`(`): [@(0.0.255) bool]_[@0 IsMainRunning]()&]
[s2;l333;   &]
[s0;l333; &]
[s1;l333;K:`:`:GetDataFile`(const char`*`): [^`:`:String^ String]_[@0 GetDataFile]([@(0.0.255) c
onst]_[@(0.0.255) char]_`*[@3 filename])&]
[s2;l333;   [*@3 filename] &]
[s0;l333; &]
[s1;l333;K:`:`:LaunchWebBrowser`(const`:`:String`&`): [@(0.0.255) void]_[@0 LaunchWebBrow
ser]([@(0.0.255) const]_[^`:`:String^ String]`&_[@3 url])&]
[s2;l333;   [*@3 url] &]
[s0;l333; &]
[s0; ]