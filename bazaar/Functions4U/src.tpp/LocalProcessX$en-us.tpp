topic "LocalProcessX";
[2 $$0,0#00000000000000000000000000000000:Default]
[i448;a25;kKO9;2 $$1,0#37138531426314131252341829483380:class]
[l288;2 $$2,2#27521748481378242620020725143825:desc]
[0 $$3,0#96390100711032703541132217272105:end]
[H6;0 $$4,0#05600065144404261032431302351956:begin]
[i448;a25;kKO9;2 $$5,0#37138531426314131252341829483370:item]
[l288;a4;*@5;1 $$6,6#70004532496200323422659154056402:requirement]
[l288;i1121;b17;O9;~~~.1408;2 $$7,0#10431211400427159095818037425705:param]
[i448;b42;O9;2 $$8,8#61672508125594000341940100500538:tparam]
[b42;2 $$9,9#13035079074754324216151401829390:normal]
[{_}%EN-US 
[ {{10000@(113.42.0) [s0; [*@7;4 Functions4U Reference. LocalProcessX]]}}&]
[s4;2 &]
[s1;:Upp`:`:LocalProcessX`:`:class: [@(0.0.255) class]_[* LocalProcessX]&]
[s0; &]
[s0; LocalProcessX permits to launch child executables and batch 
files managing its input and output.&]
[s0; It is based in LocalProcess2 including some elements to ease 
its use.like WhenTimer callback that lets user to manage program 
status.&]
[s3; &]
[ {{10000F(128)G(128)@1 [s0; [* Public Method List]]}}&]
[s4;%- &]
[s5;:Upp`:`:LocalProcessX`:`:Start`(const char`*`,const char`*`,const char`*`,double`,double`,double`,bool`):%- [@(0.0.255) b
ool]_[* Start]([@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 cmd], [@(0.0.255) const]_[@(0.0.255) c
har]_`*[*@3 envptr]_`=_[@3 0], [@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 dir]_`=_[@3 0], 
[@(0.0.255) double]_[*@3 refreshTime]_`=_`-[@3 1], [@(0.0.255) double]_[*@3 maxTimeWithoutO
utput]_`=_`-[@3 1], [@(0.0.255) double]_[*@3 maxRunTime]_`=_`-[@3 1], 
[@(0.0.255) bool]_[*@3 convertcharset]_`=_[@(0.0.255) true])&]
[s2; Starts child process defined by command line [%-*@3 cmd], with 
[%-*@3 envptr] environment variables, in folder [%-*@3 dir].&]
[s2; If [%-*@3 refreshTime] is defined, the Perform function will be 
called periodically.&]
[s2; [%-*@3 maxTimeWithoutOutput] is the maximum time permitted without 
child program text output (by functions like printf) and [%-*@3 maxRunTime] 
is the maximum child program run time. [%-*@3 convertcharset] .defines 
if character set has to be converted.&]
[s3; &]
[s4;%- &]
[s5;:Upp`:`:LocalProcessX`:`:Perform`(`):%- [@(0.0.255) void]_[* Perform]()&]
[s2; Perform manages child program and calls WhenTimer user function. 
&]
[s3;%- &]
[s4;%- &]
[s5;:Upp`:`:LocalProcessX`:`:Stop`(ProcessStatus`):%- [@(0.0.255) void]_[* Stop]([_^ProcessStatus^ P
rocessStatus]_[*@3 status]_`=_STOP`_USER)&]
[s2; Stops child program, subprograms and threads, with [%-*@3 status].&]
[s3; &]
[s4;%- &]
[s5;:Upp`:`:LocalProcessX`:`:Pause`(`):%- [@(0.0.255) void]_[* Pause]()&]
[s2; Pauses child program, subprograms and threads.&]
[s3;%- &]
[s4;%- &]
[s5;:Upp`:`:LocalProcessX`:`:IsPaused`(`):%- [@(0.0.255) bool]_[* IsPaused]()&]
[s2; Returns true if child program is paused.&]
[s3;%- &]
[s4;%- &]
[s5;:Upp`:`:LocalProcessX`:`:Seconds`(`):%- [@(0.0.255) double]_[* Seconds]()&]
[s2; Returns number of child program running elapsed seconds.&]
[s3;%- &]
[s4;%- &]
[s5;:Upp`:`:LocalProcessX`:`:Write`(Upp`:`:String`):%- [@(0.0.255) void]_[* Write]([_^Upp`:`:String^ S
tring]_[*@3 str])&]
[s2; Sends [%-*@3 str] string to child program.&]
[s3; &]
[s4;%- &]
[s5;:Upp`:`:LocalProcessX`:`:GetStatus`(`):%- [@(0.0.255) int]_[* GetStatus]()&]
[s2; Returns child program status. &]
[s2; It is defined in [* ProcessStatus] and it can be:&]
[s2;i150;O0; [* RUNNING]&]
[s2;i150;O0; [* STOP`_OK]&]
[s2;i150;O0; [* STOP`_TIMEOUT]&]
[s2;i150;O0; [* STOP`_USER]&]
[s3;%- &]
[s4;%- &]
[s5;:Upp`:`:LocalProcessX`:`:IsRunning`(`):%- [@(0.0.255) bool]_[* IsRunning]()&]
[s2; Returns true if child program is running.&]
[s3;%- &]
[s4;%- &]
[s5;:Upp`:`:LocalProcessX`:`:WhenTimer:%- [_^Upp`:`:Gate4^ Gate4]<[@(0.0.255) double], 
String[@(0.0.255) `&], [@(0.0.255) bool], [@(0.0.255) bool`&]>_[* WhenTimer]&]
[s2; Permits to include user function to handle:&]
[s2;i150;O0; Inputs&]
[s2;l416;i150;O0; Running time elapsed&]
[s2;l416;i150;O0; String output by child program&]
[s2;l416;i150;O0; Child program status&]
[s2;i150;O0; Output&]
[s2;l416;i150;O0; To reset time without program output timeout&]
[s3; ]]