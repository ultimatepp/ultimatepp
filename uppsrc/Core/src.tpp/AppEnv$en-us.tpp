topic "Application & environment utility";
[2 $$0,0#00000000000000000000000000000000:Default]
[i448;a25;kKO9;2 $$1,0#37138531426314131252341829483380:class]
[l288;2 $$2,0#27521748481378242620020725143825:desc]
[0 $$3,0#96390100711032703541132217272105:end]
[H6;0 $$4,0#05600065144404261032431302351956:begin]
[i448;a25;kKO9;2 $$5,0#37138531426314131252341829483370:item]
[l288;a17;*@5;1 $$6,6#70004532496200323422659154056402:requirement]
[l288;i1121;b17;O9;~~~.1408;2 $$7,0#10431211400427159095818037425705:param]
[{_}%EN-US 
[s0; [@3;4 Application `& environment utility]&]
[s0; A number of utility functions to obtain information about your 
application and system environment.&]
[s0;+80 &]
[s0; [* Implemented in:]&]
[s0;%- [%% -|]Core`\App.[@(0.0.255) h]&]
[s0;%- -|Core`\App.[@(0.0.255) cpp]&]
[s0; &]
[ {{10000F(128)G(128)@1 [s0; [*@2 Function List]]}}&]
[s0; &]
[s5;K:`:`:GetExeFilePath`(`):%- [^topic`:`/`/Core`/src`/String`$en`-us^ String]_GetExeF
ilePath[@(64) ()]&]
[s2;# Returns the absolute path towards the application`'s binary.&]
[s3; &]
[s4; &]
[s5;K:`:`:GetExeDirFile`(const char`*`):%- [^topic`:`/`/Core`/src`/String`$en`-us^ Stri
ng]_GetExeDirFile[@(64) (][@(0.0.255) const][@(64) _][@(0.0.255) char][@(64) _`*][@3 fp][@(64) )
]&]
[s2;# Returns the absolute path of a file that is found in the folder 
where you application`'s binary resides.&]
[s2; [@(129.0.0) fp..................................................]the 
name of the file&]
[s3; &]
[s4; &]
[s5;%- [^topic`:`/`/Core`/src`/String`$en`-us^ String]_GetHomeDirectory[@(64) ()]&]
[s2; Returns the absolute path of current user`'s home folder.&]
[s3; &]
[s4; &]
[s5;%- [^topic`:`/`/Core`/src`/String`$en`-us^ String]_GetHomeDirFile[@(64) (][@(0.0.255) c
onst][@(64) _][@(0.0.255) char][@(64) _`*][@3 fp][@(64) )]&]
[s2;# Returns the absolute path of a file that is found in the current 
user`'s home folder.&]
[s2; [@(129.0.0) fp..................................................]the 
name of the file&]
[s3; &]
[s4; &]
[s5;%- [^topic`:`/`/Core`/src`/String`$en`-us^ String]_GetExeTitle[@(64) ()]&]
[s2; Returns the title of the application.&]
[s3; &]
[s4; &]
[s5;%- [@(0.0.255) void][@(64) _]UseHomeDirectoryConfig[@(64) (][@(0.0.255) bool][@(64) _][@3 b][@(64) _
`=_][@(0.0.255) true][@(64) )]&]
[s2;# Sets the search path for configuration to either the current 
users`' home folder or to the folder where the application`'s 
binary resides.&]
[s2; [*@(129.0.0) b]...................................................use 
home folder (true) or application folder (false)&]
[s3; &]
[s4; &]
[s5;%- [^topic`:`/`/Core`/src`/String`$en`-us^ String]_ConfigFile[@(64) (][@(0.0.255) const
][@(64) _][@(0.0.255) char][@(64) _`*][@3 file][@(64) )]&]
[s2;# Returns the absolute path of a configuration file found in 
the configuration files search path.&]
[s2; [@(129.0.0) file ]...............................................the 
name of the file&]
[s3; &]
[s4; &]
[s5;%- [^topic`:`/`/Core`/src`/String`$en`-us^ String]_ConfigFile[@(64) ()]&]
[s2;# Returns the default configuration file from the configuration 
files search path. The default configuration file is called [*/ `[application 
title`]][* .cfg].&]
[s3;# &]
[s4; &]
[s5;%- [@(0.0.255) const][@(64) _][^topic`:`/`/Core`/src`/Vector`$en`-us^@(64) Vector][@(64) <
][^`:`:String^@(64) Stri][^topic`:`/`/Core`/src`/String`$en`-us^@(64) n][^`:`:String^@(64) g
][@(64) >`&_]CommandLine[@(64) ()]&]
[s2; Return a vector of strings containing the command line parameters.&]
[s3; &]
[s4; &]
[s5;%- [@(0.0.255) const][@(64) _][^topic`:`/`/Core`/src`/VectorMap`$en`-us^@(64) VectorMap
][@(64) <][^topic`:`/`/Core`/src`/String`$en`-us^@(64) String][@(64) , 
][^topic`:`/`/Core`/src`/String`$en`-us^@(64) String][@(64) >`&_]Environment[@(64) ()]&]
[s2; Returns a vector of strings with system environment variables.&]
[s3; &]
[s4; &]
[s5;%- [@(0.0.255) void][@(64) _]SetExitCode[@(64) (][@(0.0.255) int][@(64) _][@3 code][@(64) )]&]
[s2; Sets the exit code that will be returned on application exit.&]
[s2; [*@(129.0.0) code].............................................the 
exit code&]
[s3; &]
[s4; &]
[s5;%- [@(0.0.255) int][@(64) _]GetExitCode[@(64) ()]&]
[s2;# Gets the exit code that will be returned on application exit.&]
[s3;# &]
[s4; &]
[s5;%- [@(0.0.255) bool][@(64) _]IsMainRunning[@(64) ()]&]
[s2;# Return true if the applications execution has reached and not 
yet exited the main function.&]
[s3;# &]
[s4;# &]
[s5;%- [^topic`:`/`/Core`/src`/String`$en`-us^ String]_GetDataFile[@(64) (][@(0.0.255) cons
t][@(64) _][@(0.0.255) char][@(64) _`*][@3 filename][@(64) )]&]
[s2; [*_@(85.127.0) TODO]&]
[s2; [*@(129.0.0) filename].......................................the 
name of the file&]
[s3; &]
[s4; &]
[s5;%- [@(0.0.255) void][@(64) _]LaunchWebBrowser[@(64) (][@(0.0.255) const][@(64) _][^topic`:`/`/Core`/src`/String`$en`-us^@(64) S
tring][@(64) `&_][@3 url][@(64) )]&]
[s2; Opens an URL in the default Web Browser.&]
[s2; [*@(129.0.0) url................................................]the 
URL that will be opened in the browser&]
[s0; &]
[s0; &]
[ {{10000F(128)G(128)@(226.85.0) [s0; [*@2 Function List (POSIX)]]}}&]
[s0; &]
[s5;K:`:`:SetHomeDirectory`(const char`*`):%- [@(0.0.255) void][@(64) _]SetHomeDirectory[@(64) (
][@(0.0.255) const][@(64) _][@(0.0.255) char][@(64) _`*][@3 dir][@(64) )]&]
[s2;# Sets the current user`'s home folder.&]
[s2; [*@(129.0.0) dir][@(129.0.0) ................................................]the 
URL that will be opened in the browser&]
[s0; &]
[s0; ]