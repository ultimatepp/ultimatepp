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
[s0;%% [@3;4 Application `& environment utility]&]
[s0;%% A number of utility functions to obtain information about 
your application and system environment.&]
[s0;+80%% &]
[s0;%% [* Implemented in:]&]
[s0; [%% -|]Core`\App.[@(0.0.255) h]&]
[s0; -|Core`\App.[@(0.0.255) cpp]&]
[s0;%% &]
[ {{10000F(128)G(128)@1 [s0;%% [* Function List]]}}&]
[s3; &]
[s5;:GetExeFilePath`(`): String_[* GetExeFilePath]()&]
[s2;#%% Returns the absolute path towards the application`'s binary.&]
[s3; &]
[s4; &]
[s5;:GetExeDirFile`(const char`*`): String_[* GetExeDirFile]([@(0.0.255) const]_[@(0.0.255) c
har]_`*[*@3 fp])&]
[s2;#%% Returns the absolute path of a file that is found in the 
folder where you application`'s binary resides.&]
[s2;%% [@(129.0.0) fp..................................................]the 
name of the file&]
[s3; &]
[s4; &]
[s5;:GetHomeDirectory`(`): String_[* GetHomeDirectory]()&]
[s2;%% Returns the absolute path of current user`'s home folder.&]
[s3; &]
[s4; &]
[s5;:GetHomeDirFile`(const char`*`): String_[* GetHomeDirFile]([@(0.0.255) const]_[@(0.0.255) c
har]_`*[*@3 fp])&]
[s2;#%% Returns the absolute path of a file that is found in the 
current user`'s home folder.&]
[s2;%% [@(129.0.0) fp..................................................]the 
name of the file&]
[s3; &]
[s4; &]
[s5;:GetExeTitle`(`): String_[* GetExeTitle]()&]
[s2;%% Returns the title of the application.&]
[s3; &]
[s4; &]
[s5;:UseHomeDirectoryConfig`(bool`): [@(0.0.255) void]_[* UseHomeDirectoryConfig]([@(0.0.255) b
ool]_[*@3 b]_`=_[@(0.0.255) true])&]
[s2;#%% Sets the search path for configuration to either the current 
users`' home folder or to the folder where the application`'s 
binary resides.&]
[s2;%% [*@(129.0.0) b]...................................................use 
home folder (true) or application folder (false)&]
[s3; &]
[s4; &]
[s5;:ConfigFile`(const char`*`): String_[* ConfigFile]([@(0.0.255) const]_[@(0.0.255) char]_
`*[*@3 file])&]
[s2;#%% Returns the absolute path of a configuration file found in 
the configuration files search path.&]
[s2;%% [@(129.0.0) file ]...............................................the 
name of the file&]
[s3; &]
[s4; &]
[s5;:ConfigFile`(`): String_[* ConfigFile]()&]
[s2;#%% Returns the default configuration file from the configuration 
files search path. The default configuration file is called [*/ `[application 
title`]][* .cfg].&]
[s3; &]
[s4; &]
[s5;:CommandLine`(`): [@(0.0.255) const]_Vector[@(0.0.255) <]String[@(0.0.255) >`&]_[* Comman
dLine]()&]
[s2;%% Return a vector of strings containing the command line parameters.&]
[s3; &]
[s4; &]
[s5;:Environment`(`): [@(0.0.255) const]_VectorMap[@(0.0.255) <]String, 
String[@(0.0.255) >`&]_[* Environment]()&]
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
[s2;#%% Gets the exit code that will be returned on application exit.&]
[s3; &]
[s4; &]
[s5;:IsMainRunning`(`): [@(0.0.255) bool]_[* IsMainRunning]()&]
[s2;#%% Return true if the applications execution has reached and 
not yet exited the main function.&]
[s3; &]
[s4; &]
[s5;:GetDataFile`(const char`*`): String_[* GetDataFile]([@(0.0.255) const]_[@(0.0.255) cha
r]_`*[*@3 filename])&]
[s2;%% This function is designed to retrieve the path of file that 
contains application specific data. Behaviour depends on whether 
application was started from `'theide`' `- in that case theide 
passes a reference to main package directory through environment 
variable and the resulting file is in this directory. When started 
standalone, the function is equivalent to ConfigFile.&]
[s2;%% [*@(129.0.0) filename].......................................the 
name of the file&]
[s3; &]
[s4; &]
[s5;:LaunchWebBrowser`(const String`&`): [@(0.0.255) void]_[* LaunchWebBrowser]([@(0.0.255) c
onst]_String[@(0.0.255) `&]_[*@3 url])&]
[s2;%% Opens an URL in the default Web Browser.&]
[s2;%% [*@(129.0.0) url................................................]the 
URL that will be opened in the browser&]
[s3; &]
[s4; &]
[s5;:SetHomeDirectory`(const char`*`): [@(0.0.255) void]_[* SetHomeDirectory]([@(0.0.255) c
onst]_[@(0.0.255) char]_`*[*@3 dir])&]
[s2;#%% Sets the current user`'s home folder.&]
[s2;%% [*@(129.0.0) dir][@(129.0.0) ................................................]the 
URL that will be opened in the browser&]
[s0; ]