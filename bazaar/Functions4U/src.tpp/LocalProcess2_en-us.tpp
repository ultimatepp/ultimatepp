topic "LocalProcess2";
[ $$0,0#00000000000000000000000000000000:Default]
[i448;a25;kKO9; $$1,0#37138531426314131252341829483380:structitem]
[l288;2 $$2,0#27521748481378242620020725143825:desc]
[0 $$3,0#96390100711032703541132217272105:end]
[H6;0 $$4,0#05600065144404261032431302351956:begin]
[i448;a25;kKO9;2 $$5,0#37138531426314131252341829483370:codeitem]
[{_}%EN-US 
[ {{10000@(113.42.0) [s0; [*@7;4 Functions4U Reference. LocalProcess2]]}}&]
[s4;2 &]
[s1;:Upp`:`:LocalProcess2`:`:class: [@(0.0.255) class]_[* LocalProcess2]_:_[@(0.0.255) publi
c]_[*@3 AProcess]&]
[s0;2 &]
[s0; [2 LocalProcess2 permits to launch and handle new process from 
a host application.]&]
[s0;2 &]
[s0; [2 These processes are defined by a classical command line like 
the used in the command window, although it can add default directory 
and environment variables.]&]
[s0;2 &]
[s0; [2 LocalProcess2 is a fork of LocalProcess adding some new abilities 
like:]&]
[s0;i150;O0; [2 Process pauses (suspends) and resumes including all 
children processes and threads.]&]
[s0;i150;O0; [2 Process killing includes all children processes and 
threads.]&]
[s0;i150;O0; [2 Process starting includes default directory.]&]
[s3; &]
[ {{10000F(128)G(128)@1 [s0; [* Public Method List]]}}&]
[s4; &]
[s5;:Upp`:`:LocalProcess2`:`:Start`(const char`*`,const char`*`,const char`*`): [@(0.0.255) b
ool]_[* Start]([@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 cmdline], 
[@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 envptr]_`=_(([@(0.0.255) void]_`*)[@3 0]), 
[@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 dir]_`=_(([@(0.0.255) void]_`*)[@3 0]))&]
[s2; Starts a new process defined by the command line [*@3 cmdline], 
using [*@3 envptr] environment variables and running the process 
in [*@3 dir] directory.&]
[s2; All text output from process to [^https`:`/`/en`.wikipedia`.org`/wiki`/Standard`_streams^ s
tandard streams] stdout and stderr is mixed and can be got using 
Read() method.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:LocalProcess2`:`:Start2`(const char`*`,const char`*`,const char`*`): [@(0.0.255) b
ool]_[* Start2]([@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 cmdline], 
[@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 envptr]_`=_(([@(0.0.255) void]_`*)[@3 0]), 
[@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 dir]_`=_(([@(0.0.255) void]_`*)[@3 0]))&]
[s2; Starts a new process defined by the command line [*@3 cmdline], 
using [*@3 envptr] environment variables and running the process 
in [*@3 dir] directory.&]
[s2; All text output from process to [^https`:`/`/en`.wikipedia`.org`/wiki`/Standard`_streams^ s
tandard streams] stdout and stderr is separated and can be got 
using Read2() method.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:LocalProcess2`:`:Start`(const char`*`,const Upp`:`:Vector`<Upp`:`:String`>`&`,const char`*`,const char`*`): [@(0.0.255) b
ool]_[* Start]([@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 cmd], [@(0.0.255) const]_[_^Upp`:`:Vector^ V
ector]<[_^Upp`:`:String^ String]>`&_[*@3 arg], [@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 e
nvptr]_`=_(([@(0.0.255) void]_`*)[@3 0]), [@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 dir]_
`=_(([@(0.0.255) void]_`*)[@3 0]))&]
[s2; Starts a new process defined by the command [*@3 cmd] with arguments 
list [*@3 arg], using [*@3 envptr] environment variables and running 
the process in [*@3 dir] directory.&]
[s2; All text output from process to [^https`:`/`/en`.wikipedia`.org`/wiki`/Standard`_streams^ s
tandard streams] stdout and stderr is mixed and can be got using 
Read() method.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:LocalProcess2`:`:Start2`(const char`*`,const Upp`:`:Vector`<Upp`:`:String`>`&`,const char`*`,const char`*`): [@(0.0.255) b
ool]_[* Start2]([@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 cmd], [@(0.0.255) const]_[_^Upp`:`:Vector^ V
ector]<[_^Upp`:`:String^ String]>`&_[*@3 arg], [@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 e
nvptr]_`=_(([@(0.0.255) void]_`*)[@3 0]), [@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 dir]_
`=_(([@(0.0.255) void]_`*)[@3 0]))&]
[s2; Starts a new process defined by the command [*@3 cmd] with arguments 
list [*@3 arg], using [*@3 envptr] environment variables and running 
the process in [*@3 dir] directory.&]
[s2; All text output from process to [^https`:`/`/en`.wikipedia`.org`/wiki`/Standard`_streams^ s
tandard streams] stdout and stderr is separated and can be got 
using Read2() method.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:LocalProcess2`:`:Kill`(`): [@(0.0.255) void]_[* Kill]()&]
[s2; Kills a process and all children processes and threads.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:LocalProcess2`:`:IsRunning`(`): [@(0.0.255) bool]_[* IsRunning]()&]
[s2; Returns true if process is still running.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:LocalProcess2`:`:Write`(Upp`:`:String`): [@(0.0.255) void]_[* Write]([_^Upp`:`:String^ S
tring]_[*@3 s])&]
[s2; Sends a text string [*@3 s] to process.&]
[s2; In a command window, this text would be entered using the keyboard.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:LocalProcess2`:`:Read`(Upp`:`:String`&`): [@(0.0.255) bool]_[* Read]([_^Upp`:`:String^ S
tring][@(0.0.255) `&]_[*@3 res])&]
[s2; Reads text [*@3 res] from process. This process should have to 
be started using Start() function.&]
[s2; In a command window, this text would be output to the screen.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:LocalProcess2`:`:Read2`(Upp`:`:String`&`,Upp`:`:String`&`): [@(0.0.255) boo
l]_[* Read2]([_^Upp`:`:String^ String][@(0.0.255) `&]_[*@3 reso], [_^Upp`:`:String^ String][@(0.0.255) `&
]_[*@3 rese])&]
[s2; Reads text [*@3 reso] from stdout and [*@3 rese] from stderr. This 
process should have to be started using Start2() function.&]
[s2; In a command window, this text would be output to the screen.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:LocalProcess2`:`:GetExitMessage`(`): [_^Upp`:`:String^ String]_[* GetExitMess
age]()&]
[s2; Returns the string defined by the operating system to describe 
the error code.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:LocalProcess2`:`:GetExitCode`(`): [@(0.0.255) int]_[* GetExitCode]()&]
[s2; Returns the process exit code.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:LocalProcess2`:`:Pause`(`): [@(0.0.255) void]_[* Pause]()&]
[s2; Pauses/freezes/suspends process and all children processes and 
threads.&]
[s2; If process is already paused, it resumes it.&]
[s2; [*@(0.0.255)1 Only defined in Windows.]&]
[s3; &]
[s4; &]
[s5;:Upp`:`:LocalProcess2`:`:IsPaused`(`): [@(0.0.255) bool]_[* IsPaused]()&]
[s2; Returns true if process is paused.&]
[s2; [*@(0.0.255)1 Only defined in Windows.]&]
[s3;%- ]]