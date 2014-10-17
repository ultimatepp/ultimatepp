topic "Function reference";
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
[{_}%EN-US 
[ {{10000@(113.42.0) [s0; [*@7;4 SysInfo]]}}&]
[s0; &]
[s0; &]
[ {{10000@1 [s0; [* Hardware, BIOS, OS and Distro info]]}}&]
[s4;%- &]
[s5;:GetSystemInfo`(String`&`,String`&`,String`&`,int`&`,String`&`):%- [@(0.0.255) void
]_[* GetSystemInfo]([_^String^ String]_`&[*@3 manufacturer], [_^String^ String]_`&[*@3 prod
uctName], [_^String^ String]_`&[*@3 version], [@(0.0.255) int]_`&[*@3 numberOfProcessors],
 [_^String^ String]_`&[*@3 mbSerial])&]
[s2; Returns hardware information including computer [%-*@3 manufacturer, 
] [%-*@3 productName, ] [%-*@3 version], [%-*@3 numberOfProcessors 
]and [%-*@3 mbSerial ](motherboard serial number).&]
[s3; &]
[s4;%- &]
[s5;:GetBiosInfo`(String`&`,Date`&`,String`&`):%- [@(0.0.255) void]_[* GetBiosInfo]([_^String^ S
tring]_`&[*@3 biosVersion], [_^Date^ Date]_`&[*@3 biosReleaseDate][_^String^ , 
String]_`&[*@3 biosSerial])&]
[s2;%- [%% Returns bios information including ][*@3 biosVersion][%% , ][*@3 biosReleaseDate][%% .
and ][*@3 biosSerial.]&]
[s3; &]
[s4;%- &]
[s5;:GetProcessorInfo`(int`,String`&`,String`&`,String`&`,int`&`):%- [@(0.0.255) bool]_
[* GetProcessorInfo]([@(0.0.255) int]_[*@3 number], [_^String^ String]_`&[*@3 vendor], 
[_^String^ String]_`&[*@3 identifier], [_^String^ String]_`&[*@3 architecture], 
[@(0.0.255) int]_`&[*@3 speed])&]
[s2; Returns information about the different cpu cores.&]
[s2; [%-*@3 number] is the core number to get the information&]
[s2; [%-*@3 vendor] is the core vendor&]
[s2; [%-*@3 identifier] is the core identifier&]
[s2; [%-*@3 architecture] is the core architecture (32, 64)&]
[s2; [%-*@3 speed].is the core speed in MHz.&]
[s3; &]
[s4;%- &]
[s5;:GetCpuSpeed`(`):%- [@(0.0.255) int]_[* GetCpuSpeed]()&]
[s2; Gets the real time main CPU speed in MHz.&]
[s0;l288; This data is directly calculated by the function.&]
[s3; &]
[s4;%- &]
[s5;:GetVideoInfo`(Array`<Value`>`&`,Array`<Value`>`&`,Array`<Value`>`&`,Array`<Value`>`&`,Array`<Value`>`&`):%- [@(0.0.255) b
ool]_[* GetVideoInfo]([_^Array^ Array]_<[_^Value^ Value]>_`&[*@3 name], 
Array_<Value>_`&[*@3 description], Array_<Value>_`&[*@3 videoProcessor], 
Array_<Value>_`&[*@3 ram], Array_<Value>_`&[*@3 videoMode])&]
[s2; If true gets information about video systems installed:&]
[s2; [%-*@3 name] is the video system name&]
[s2; [%-*@3 description] is the video description&]
[s2; [%-*@3 videoProcessor] is the processor used&]
[s2; [%-*@3 ram ]is the available memory in the video hardware in Mb&]
[s2; [%-*@3 videoMode] is the video mode actually selected including 
screen resolution and number of colors&]
[s6; Only available in Windows&]
[s3; &]
[s4;%- &]
[s5;:GetPackagesInfo`(Array`<Value`>`&`,Array`<Value`>`&`,Array`<Value`>`&`,Array`<Value`>`&`,Array`<Value`>`&`,Array`<Value`>`&`,Array`<Value`>`&`):%- [@(0.0.255) b
ool]_[* GetPackagesInfo]([_^Array^ Array]_<[_^Value^ Value]>_`&[*@3 name], 
Array_<Value>_`&[*@3 version], Array_<Value>_`&[*@3 vendor], Array_<Value>_`&[*@3 insta
llDate], Array_<Value>_`&[*@3 caption], Array_<Value>_`&[*@3 description], 
Array_<Value>_`&[*@3 state])&]
[s2; If true gets information about installed software:&]
[s2;%- [*@3 name]&]
[s2;%- [*@3 version]&]
[s2;%- [*@3 vendor]&]
[s2;%- [*@3 installDate]&]
[s2; [%-*@3 caption] is a summary of the software description&]
[s2;%- [*@3 description]&]
[s2; [%-*@3 state] is the status of the software. Values are:&]
[s0;l640; `- `"Bad Configuration`"&]
[s0;l640; `- `"Invalid Argument`"&]
[s0;l640; `- `"Unknown Package`"&]
[s0;l640; `- `"Advertised`"&]
[s0;l640; `- `"Absent`"&]
[s0;l640; `- `"Ok`"&]
[s6; Only available in Windows&]
[s3; &]
[s4;%- &]
[s5;:GetCpuTemperature`(`):%- [@(0.0.255) double]_[* GetCpuTemperature]()&]
[s2; Returns CPU temperature in degrees Celsius.&]
[s6; It does not work in all computers.&]
[s3;%- &]
[s4;%- &]
[s5;:GetNetworkInfo`(String`&`,String`&`,String`&`,String`&`):%- [@(0.0.255) bool]_[* Get
NetworkInfo]([_^String^ String]_`&[*@3 name], [_^String^ String]_`&[*@3 domain], 
[_^String^ String]_`&[*@3 ip4], [_^String^ String]_`&[*@3 ip6])&]
[s2; Returns the local computer network [%-*@3 name], [%-*@3 domain], 
[%-*@3 ip4] and [%-*@3 ip6].&]
[s3; &]
[s4;%- &]
[s5;:GetMacAddress`(`):%- [@(0.0.255) String]_[* GetMacAddress]()&]
[s2; Returns the [%-@3 MAC ]address in uppercase hexadecimal formated 
like `"0A:0B:0C:0C:0D:0E`" or Null.&]
[s0;l288; This function is deprecated by GetAdapterInfo().&]
[s3; &]
[s4;%- &]
[s5;:GetAdapterInfo`(`):%- [_^Array^ Array]_<[_^NetAdapter^ NetAdapter]>_[* GetAdapterInfo](
)&]
[s0;l288; Gets an array of type NetAdapter with all the network adapters 
installed.&]
[s0;l288; NetAdapter contains:&]
[s0;l288;i150;O0;~~~1280; String description:-|Adapter description&]
[s0;l288;i150;O0;~~~1280; String fullname:-|Adapter name&]
[s0;l288;i150;O0;~~~1280; String mac:-|Adapter MAC address if available&]
[s0;l288;i150;O0;~~~1280; String type;-|Adapter type. Some of the 
included types are:&]
[s0;l448;i150;O1;~~~1280~1856; `"ETHERNET`"-|-|Ethernet&]
[s0;l448;i150;O1;~~~1280~1856; `"TOKENRING`"-|-|Token Ring&]
[s0;l448;i150;O1;~~~1280~1856; `"MODEM`"-|-|Point to Point Protocol&]
[s0;l448;i150;O1;~~~1280~1856; `"SOFTWARE`_LOOPBACK`"-|Software loopback&]
[s0;l448;i150;O1;~~~1280~1856; `"ATM`"-|-|Asynchronous Transfer Mode&]
[s0;l448;i150;O1;~~~1280~1856; `"IEEE80211`"-|-|WIFI&]
[s0;l448;i150;O1;~~~1280~1856; `"TUNNEL`"-|-|Tunnel encapsulation&]
[s0;l448;i150;O1;~~~1280~1856; `"IEEE1394`"-|-|Firewire&]
[s3; &]
[s4;%- &]
[s5;:GetNetworkInfo`(String`&`,String`&`):%- [@(0.0.255) bool]_[* GetNetworkInfo]([_^String^ S
tring]_`&[*@3 name], [_^String^ String]_`&[*@3 domain])&]
[s2; Returns the local computer network [%-*@3 name] and [%-*@3 domain].&]
[s3; &]
[s4;%- &]
[s5;:GetAvailableSocketPort`(int`):%- [@(0.0.255) int]_[* GetAvailableSocketPort]([@(0.0.255) i
nt]_[*@3 from]_`=_[@3 49152])&]
[s2; Returns the first available socket port from [%-*@3 from] or Null 
if there is an error.&]
[s3; &]
[s4;%- &]
[s5;:GetHDSerial`(`):%- [@(0.0.255) String]_[* GetHDSerial]()&]
[s2; Returns the hard disk physical [%-*@3 serial ]number or Null. 
This code is not affected by disk formatting.&]
[s6; Only available in Windows&]
[s3; &]
[s4;%- &]
[s5;:GetMemoryInfo`(int`&`,uint64`&`,uint64`&`,uint64`&`,uint64`&`,uint64`&`,uint64`&`):%- [@(0.0.255) b
ool]_[* GetMemoryInfo]([@(0.0.255) int]_`&[*@3 memoryLoad], [_^uint64^ uint64]_`&[*@3 total
Phys], [_^uint64^ uint64]_`&[*@3 freePhys], [_^uint64^ uint64]_`&[*@3 totalPageFile], 
[_^uint64^ uint64]_`&[*@3 freePageFile], [_^uint64^ uint64]_`&[*@3 totalVirtual], 
[_^uint64^ uint64]_`&[*@3 freeVirtual])&]
[s2; Gets information about the system memory:&]
[s2; [%-*@3 memoryLoad ]is the percent of memory in use&]
[s2; [%-*@3 totalPhys ]is the total physical memory&]
[s2; [%-*@3 freePhys] is the free physical memory&]
[s2; [%-*@3 totalPageFile ]is the total paging file&]
[s2; [%-*@3 freePageFile ]is the free paging file&]
[s2; [%-*@3 totalVirtual ]is the total virtual memory&]
[s2; [%-*@3 freeVirtual ]is the free virtual memory.&]
[s3; &]
[s4;%- &]
[s5;:GetOsInfo`(String`&`,String`&`,String`&`,String`&`,String`&`,String`&`,String`&`):%- [@(0.0.255) b
ool]_[* GetOsInfo]([_^String^ String]_`&[*@3 kernel], [_^String^ String]_`&[*@3 kerVersion],
 [_^String^ String]_`&[*@3 kerArchitecture], [_^String^ String]_`&[*@3 distro], 
[_^String^ String]_`&[*@3 distVersion], [_^String^ String]_`&[*@3 desktop], 
[_^String^ String]_`&[*@3 deskVersion])&]
[s2; Gets many information to identify the operating system and Desktop 
where the application is being run. &]
[s2; [%-*@3 kernel]: Kernel name&]
[s2; [%-*@3 kerVersion]: Kernel version&]
[s2; [%-*@3 kerArchitecture]: Kernel architecture&]
[s2; [%-*@3 distro]: Distro name&]
[s2; [%-*@3 distVersion]: Distro version&]
[s2; [%-*@3 desktop]: Desktop manager name&]
[s2; [%-*@3 deskVersion].: Desktop manager version&]
[s3;%- &]
[s4;%- &]
[s5;:GetDriveSpace`(String`,uint64`&`,uint64`&`,uint64`&`):%- [@(0.0.255) bool]_[* GetDri
veSpace]([_^String^ String]_[*@3 drive], [_^uint64^ uint64]_`&[*@3 freeBytesUser], 
[_^uint64^ uint64]_`&[*@3 totalBytesUser], [_^uint64^ uint64]_`&[*@3 totalFreeBytes])&]
[s2; Gets [%-*@3 drive] space.&]
[s2; [%-*@3 freeBytesUser]: Amount of free bytes available to the user&]
[s2; [%-*@3 totalBytesUser]: Size of drive visible for the user&]
[s2; [%-*@3 totalFreeBytes]: Amount of free bytes.&]
[s0; -|Returns false if drive is not mounted or it is not accessible&]
[s3; &]
[s4;%- &]
[s5;:GetDriveInformation`(String`,String`&`,String`&`,int`&`,String`&`):%- [@(0.0.255) b
ool]_[* GetDriveInformation]([_^String^ String]_[*@3 drive], [_^String^ String]_`&[*@3 type
], [_^String^ String]_`&[*@3 volume], [@(0.0.255) int]_`&[*@3 maxName], 
[_^String^ String]_`&[*@3 fileSystem])&]
[s2; Gets [%-*@3 drive] information&]
[s2;%- [*@3 type]: Gets the type of the drive.&]
[s2;%- Available types are `"Hard`", `"Network`", `"Optical`", `"RAM`", 
`"Removable`".&]
[s2;%- [*@3 volume]: Gets the name of the drive&]
[s2;%- [*@3 maxName]: Gets the maximum length permitted for a file name&]
[s2;%- fileSystem: Gets the drive formatting system.&]
[s2; Returns false if drive is not mounted or it is not accessible&]
[s3; &]
[s4; &]
[s5;:GetCompilerInfo`(String`&`,int`&`,Time`&`,String`&`,int`&`):%- [@(0.0.255) void]_[* G
etCompilerInfo]([_^String^ String]_`&[*@3 name], [@(0.0.255) int]_`&[*@3 version], 
[_^Time^ Time]_`&[*@3 time], [_^String^ String]_`&[*@3 mode], [_^String^ int]_`&[*@3 bits])&]
[s2; Returns compiling information, like compiler [%-*@3 name, ]compiler 
[%-*@3 version], program compilation [%-*@3 time], compiling [%-*@3 mode] 
and [%-*@3 bits].&]
[s3; &]
[s4; &]
[s5;:GetBatteryStatus`(bool`&`,int`&`,int`&`):%- [@(0.0.255) bool]_[* GetBatteryStatus]([@(0.0.255) b
ool]_`&[*@3 discharging], [@(0.0.255) int]_`&[*@3 percentage], [@(0.0.255) int]_`&[*@3 rema
iningMin])&]
[s2; Gets battery information like if it is [%-*@3 discharging] or 
connected to the grid, [%-*@3 percentage] of charging where 100% 
means full charge, and number of expected computer running minutes 
in [%-*@3 remainingMin].&]
[s2; Returns true if the values got are valid.&]
[s3; &]
[s4;%- &]
[s5;:GetBatteryInfo`(bool`&`):%- [@(0.0.255) bool]_[* GetBatteryInfo]([@(0.0.255) bool]_`&[*@3 p
resent]_)&]
[s2; Gets if battery is [%-*@3 present ]or not.&]
[s2; Returns true if the values got are valid.&]
[s3; &]
[s4;%- &]
[s5;:OpenCDTray`(`):%- [@(0.0.255) void]_[* OpenCDTray]()&]
[s2; Ejects CD/DVD tray.&]
[s3; &]
[s4;%- &]
[s5;:CloseCDTray`(`):%- [@(0.0.255) void]_[* CloseCDTray]()&]
[s2; Closes CD/DVD tray if possible&]
[s3;@(0.0.255)%- &]
[s0;%- &]
[ {{10000@1 [s0; [* Process handling]]}}&]
[s3;%- &]
[s5;:GetWindowsList`(Array`<int64`>`&`,Array`<int64`>`&`,Array`<String`>`&`,Array`<String`>`&`,Array`<String`>`&`):%- [@(0.0.255) v
oid]_[* GetWindowsList]([_^Array^ Array]<[@(0.0.255) int64]>_`&[*@3 wid], 
[_^Array^ Array]<[@(0.0.255) int64]>_`&[*@3 pid], [_^Array^ Array]<[_^String^ String]>_`&[*@3 n
ame], [_^Array^ Array]<[_^String^ String]>_`&[*@3 fileName], [_^Array^ Array]<[_^String^ St
ring]>_`&[*@3 title])&]
[s2; Gets arrays with handles to all the opened windows with additional 
info as:&]
[s2; [%-*@3 wid]: Handle to the the window&]
[s2; [%-*@3 pid]: Handle to the process that manages the window&]
[s2; [%-*@3 name]: Window name&]
[s2; [%-*@3 fileName]: Window process program file name&]
[s2; [%-*@3 title]: Window title (caption)&]
[s3; &]
[s4;%- &]
[s5;:GetWindowsList`(`):%- [_^Array^ Array]<[@(0.0.255) int64]>_[* GetWindowsList]()&]
[s2; Gets an array with handles to all the opened windows.&]
[s3;%- &]
[s4;%- &]
[s5;:GetProcessList`(Array`<int64`>`&`,Array`<String`>`&`):%- [@(0.0.255) bool]_[* GetPro
cessList]([_^Array^ Array]<[@(0.0.255) int64]>_`&[*@3 pid], [_^Array^ Array]<[_^String^ Str
ing]>_`&[*@3 pNames])&]
[s2; Gets arrays with handles to all the opened processes [%-*@3 pid 
]and process names [%-*@3 pNames].&]
[s3; &]
[s4;%- &]
[s5;:GetProcessList`(`):%- [_^Array^ Array]<[@(0.0.255) int64]>_[* GetProcessList]()&]
[s2; Gets an array with handles to all the opened processes&]
[s3;%- &]
[s4;%- &]
[s5;:GetProcessName`(int64`):%- [_^String^ String]_[* GetProcessName]([@(0.0.255) int64]_[*@3 p
id])&]
[s2; Returns the process name for a process with handle [%-*@3 pid].&]
[s3; &]
[s4;%- &]
[s5;:GetProcessFileName`(int64`):%- [_^String^ String]_[* GetProcessFileName]([@(0.0.255) i
nt64]_[*@3 pid])&]
[s2; Gets the program file name of a process with handle [%-*@3 pid].&]
[s3; &]
[s4; &]
[s5;:GetProcessCPUUsage`(int64`):%- [@(0.0.255) int]_[* GetProcessCPUUsage]([@(0.0.255) int
64]_[*@3 pid])&]
[s2; Gets the percentage of CPU usage for process [%-*@3 pid].&]
[s6; Only available in Windows&]
[s3; &]
[s4;%- &]
[s5;:GetProcessIdFromName`(String`):%- [_^int64^ int64]_[* GetProcessIdFromName]([_^String^ S
tring]_[*@3 name])&]
[s2; Gets the process handle of a program named [%-*@3 name].&]
[s3; &]
[s4;%- &]
[s5;:GetProcessIdFromWindowCaption`(String`,bool`):%- [@(0.0.255) int64]_[* GetProcessIdF
romWindowCaption]([_^String^ String]_[*@3 windowCaption], [@(0.0.255) bool]_[*@3 exactMat
ch]_`=_[@(0.0.255) false])&]
[s2;%- [%% Gets the process handle of a program with a window with 
title ][*@3 windowCaption.]&]
[s2;%- [%% If ][*@3 exactMatch][%% .is true it only returns the process 
handle of a process with a window title that is equal to ][*@3 windowCaption. 
][%% If it is false then the handle is returned if only part of 
the window title matches with ][*@3 windowCaption.]&]
[s3; &]
[s4;%- &]
[s5;:GetWindowIdFromCaption`(String`,bool`):%- [@(0.0.255) int64]_[* GetWindowIdFromCapti
on]([_^String^ String]_[*@3 windowCaption], [@(0.0.255) bool]_[*@3 exactMatch]_`=_[@(0.0.255) f
alse])&]
[s2;%- [%% Gets the window handle of a program with a window with title 
][*@3 windowCaption.]&]
[s2;%- [%% If ][*@3 exactMatch][%% .is true it only returns the process 
handle of a process with a window title that is equal to ][*@3 windowCaption. 
][%% If it is false then the handle is returned if only part of 
the window title matches with ][*@3 windowCaption.]&]
[s3; &]
[s4;%- &]
[s5;:GetProcessIdFromWindowId`(int64`):%- [@(0.0.255) int64]_[* GetProcessIdFromWindowId](
[@(0.0.255) int64]_[*@3 wid])&]
[s2;%- [%% Returns the process handle of a program with window handle 
][*@3 wid.]&]
[s3; &]
[s4;%- &]
[s5;:GetWindowIdFromProcessId`(int64`):%- [@(0.0.255) int64]_[* GetWindowIdFromProcessId](
[@(0.0.255) int64]_[*@3 pid])&]
[s2;%- [%% Returns the window handle of a program with process handle 
][*@3 pid.]&]
[s3; &]
[s4;%- &]
[s5;:ProcessTerminate`(int64`,int`):%- [@(0.0.255) bool]_[* ProcessTerminate]([@(0.0.255) i
nt64]_[*@3 pid], [@(0.0.255) int]_[*@3 timeout]_`=_[@3 500])&]
[s2;%- [%% Ends the program with handle ][*@3 pid.]&]
[s2; If after asking the process to end [%-*@3 timeout ]is over, it 
will kill the process by different means in order of increasing 
`"agressivity`".&]
[s0;l288; For example in Posix it will send the process first a SIGTERM, 
if the process does not stop it will send a SIGKILL, and if the 
process remains running it will simply call WindowKill() to do 
the dirty job.&]
[s3; &]
[s4;%- &]
[s5;:GetProcessPriority`(int64`):%- [@(0.0.255) int]_[* GetProcessPriority]([@(0.0.255) int
64]_[*@3 pid])&]
[s2; Gets the process priority of the process with handle [%-*@3 pid] 
as a number from 0 (minimum) to 10 (maximum), if possible.&]
[s3; &]
[s4;%- &]
[s5;:SetProcessPriority`(int64`,int`):%- [@(0.0.255) bool]_[* SetProcessPriority]([@(0.0.255) i
nt64]_[*@3 pid], [@(0.0.255) int]_[*@3 priority])&]
[s2; Sets the process priority to [%-*@3 priority ]of the process with 
handle [%-*@3 pid] as a number from 0 (minimum) to 10 (maximum), 
if possible.&]
[s3; &]
[s4;%- &]
[s5;:ProcessExists`(int64`):%- [@(0.0.255) bool]_[* ProcessExists]([@(0.0.255) int64]_[*@3 pi
d])&]
[s2; Returns true if a process with handle [%-*@3 pid ]exists.&]
[s3; &]
[s4;%- &]
[s5;:GetProcessId`(`):%- [@(0.0.255) int64]_[* GetProcessId]()&]
[s2; Gets actual running process handle.&]
[s3;%- &]
[s4;%- &]
[s5;:Shutdown`(String`):%- [@(0.0.255) bool]_[* Shutdown]([_^String^ String]_[*@3 action])&]
[s2; Tries to logoff, reboot or shutdown the actual running session.&]
[s0; -|Actual valid [%-*@3 action ]values are `"logoff`", `"reboot`" 
and `"shutdown`".&]
[s3; &]
[s0; &]
[ {{10000@1 [s0; [* Windows handling]]}}&]
[s0; &]
[s5;:Window`_GetRect`(int64`,long`&`,long`&`,long`&`,long`&`):%- [@(0.0.255) bool]_[* Win
dow`_GetRect]([@(0.0.255) int64]_[*@3 windowId], [@(0.0.255) long]_`&[*@3 left], 
[@(0.0.255) long]_`&[*@3 top], [@(0.0.255) long]_`&[*@3 right], [@(0.0.255) long]_`&[*@3 bott
om])&]
[s2; Giving this function the [%-*@3 windowId], it returns the window 
location in the screen in [%-*@3 left], [%-*@3 top], [%-*@3 right] 
and [%-*@3 bottom].&]
[s2; Returns true if the values got are valid.&]
[s3; &]
[s4;%- &]
[s5;:Window`_SetRect`(int64`,long`,long`,long`,long`):%- [@(0.0.255) bool]_[* Window`_Set
Rect]([_^int64^ int64]_[*@3 windowId], [@(0.0.255) long]_[*@3 left], 
[@(0.0.255) long]_[*@3 top], [@(0.0.255) long]_[*@3 right], [@(0.0.255) long]_[*@3 bottom])&]
[s2; Giving this function the [%-*@3 windowId], it sets the window 
location in the screen in [%-*@3 left], [%-*@3 top], [%-*@3 right] 
and [%-*@3 bottom].&]
[s2; Returns true if the values got are valid.&]
[s3; &]
[s4;%- &]
[s5;:Window`_Bottom`(int64`):%- [@(0.0.255) void]_[* Window`_Bottom]([_^int64^ int64]_[*@3 wi
ndowId])&]
[s2; Moves [%-*@3 windowId] to the bottom of all windows.&]
[s3; &]
[s4;%- &]
[s5;:Window`_Top`(int64`):%- [@(0.0.255) void]_[* Window`_Top]([_^int64^ int64]_[*@3 windowId
])&]
[s2; Moves [%-*@3 windowId] to the top of the windows.&]
[s3; &]
[s4;%- &]
[s5;:Window`_TopMost`(int64`):%- [@(0.0.255) void]_[* Window`_TopMost]([_^int64^ int64]_[*@3 w
indowId])&]
[s2; Moves [%-*@3 windowId] as the topmost window.&]
[s3; &]
[s0; &]
[ {{10000@1 [s0; [* Mouse and keyboard handling]]}}&]
[s3; &]
[s5;:Mouse`_GetPos`(long`&`,long`&`):%- [@(0.0.255) bool]_[* Mouse`_GetPos]([@(0.0.255) lon
g]_`&[*@3 x], [@(0.0.255) long]_`&[*@3 y])&]
[s2; Gets the mouse position [%-*@3 x, y].in screen pixels where upper 
left corner is (0, 0).&]
[s2; Returns true if the operation has been done successfully.&]
[s3; &]
[s4; &]
[s5;:Mouse`_SetPos`(long`,long`,int64`):%- [@(0.0.255) bool]_[* Mouse`_SetPos]([@(0.0.255) l
ong]_[*@3 x], [@(0.0.255) long]_[*@3 y], [@(0.0.255) int64]_[*@3 windowId])&]
[s2; Sets the mouse position to [%-*@3 x] [%-*@3 y] referenced to the 
upper left vertex of window with window handle [%-*@3 windowId].&]
[s2; Returns true if the operation has been done successfully.&]
[s3; &]
[s4;%- &]
[s5;:Mouse`_LeftClick`(`):%- [@(0.0.255) void]_[* Mouse`_LeftClick]()&]
[s2; Simulates by software a mouse click with the left button as 
if it would have been done with the mouse.&]
[s3;%- &]
[s4;%- &]
[s5;:Mouse`_MiddleClick`(`):%- [@(0.0.255) void]_[* Mouse`_MiddleClick]()&]
[s2; Simulates by software a mouse click with the middle button as 
if it would have been done with the mouse.&]
[s3;%- &]
[s4;%- &]
[s5;:Mouse`_RightClick`(`):%- [@(0.0.255) void]_[* Mouse`_RightClick]()&]
[s2; Simulates by software a mouse click with the right button as 
if it would have been done with the mouse.&]
[s3;%- &]
[s4;%- &]
[s5;:Mouse`_LeftDblClick`(`):%- [@(0.0.255) void]_[* Mouse`_LeftDblClick]()&]
[s2; Simulates by software a mouse double click with the left button 
as if it would have been done with the mouse.&]
[s3;%- &]
[s4;%- &]
[s5;:Mouse`_MiddleDblClick`(`):%- [@(0.0.255) void]_[* Mouse`_MiddleDblClick]()&]
[s2; Simulates by software a mouse double click with the middle button 
as if it would have been done with the mouse.&]
[s3;%- &]
[s4;%- &]
[s5;:Mouse`_RightDblClick`(`):%- [@(0.0.255) void]_[* Mouse`_RightDblClick]()&]
[s2; Simulates by software a mouse double click with the right button 
as if it would have been done with the mouse.&]
[s3;%- &]
[s4;%- &]
[s5;:Mouse`_LeftDown`(`):%- [@(0.0.255) void]_[* Mouse`_LeftDown]()&]
[s2; Simulates by software a mouse press with the left button as 
if it would have been done with the mouse.&]
[s2; [@6 Caution:] The button will remain pressed after this function.&]
[s3;%- &]
[s4;%- &]
[s5;:Mouse`_LeftUp`(`):%- [@(0.0.255) void]_[* Mouse`_LeftUp]()&]
[s2; Simulates by software a mouse release with the left button as 
if it would have been done with the mouse.&]
[s3;%- &]
[s4;%- &]
[s5;:Mouse`_RightDown`(`):%- [@(0.0.255) void]_[* Mouse`_RightDown]()&]
[s2; Simulates by software a mouse press with the right button as 
if it would have been done with the mouse.&]
[s2; [@6 Caution:] The button will remain pressed after this function.&]
[s3;%- &]
[s4;%- &]
[s5;:Mouse`_RightUp`(`):%- [@(0.0.255) void]_[* Mouse`_RightUp]()&]
[s2; Simulates by software a mouse release with the right button 
as if it would have been done with the mouse.&]
[s3;%- &]
[s4;%- &]
[s5;:Mouse`_MiddleDown`(`):%- [@(0.0.255) void]_[* Mouse`_MiddleDown]()&]
[s2; Simulates by software a mouse press with the middle button as 
if it would have been done with the mouse.&]
[s2; [@6 Caution:] The button will remain pressed after this function.&]
[s3;%- &]
[s4;%- &]
[s5;:Mouse`_MiddleUp`(`):%- [@(0.0.255) void]_[* Mouse`_MiddleUp]()&]
[s2; Simulates by software a mouse release with the middle button 
as if it would have been done with the mouse.&]
[s3;%- &]
[s4;%- &]
[s5;:Keyb`_SendKeys`(String`,long`,long`):%- [@(0.0.255) void]_[* Keyb`_SendKeys]([_^String^ S
tring]_[*@3 text], [@(0.0.255) long]_[*@3 finalDelay]_`=_[@3 100], [@(0.0.255) long]_[*@3 del
ayBetweenKeys]_`=_[@3 50])&]
[s2; Simulates by software a text entered using the keyboard as if 
it would have been entered with the keyboard.&]
[s2; To really simulate manual key typing the function lets to enter 
delays between keys and after entering the text.&]
[s2; [%-*@3 text]: Is the text to be entered&]
[s2; [%-*@3 finalDelay]: Is the delay in ms that is forced after entering 
text&]
[s2; [%-*@3 delayBetweenKeys]: Is the delay in ms that is forced between 
text keys.&]
[s3; &]
[s4;%- &]
[s5;:GetKeyLockStatus`(bool`&`,bool`&`,bool`&`):%- [@(0.0.255) void]_[* GetKeyLockStatus](
[@(0.0.255) bool]_`&[*@3 caps], [@(0.0.255) bool]_`&[*@3 num], [@(0.0.255) bool]_`&[*@3 scrol
l])&]
[s2; Gets the status of keys [%-*@3 caps ]lock,  [%-*@3 num ]lock and 
[%-*@3 scroll] [%-*@3  ]lock. Not fully functional in Linux.&]
[s3; &]
[s4;%- &]
[s5;:SetKeyLockStatus`(bool`,bool`,bool`):%- [@(0.0.255) void]_[* SetKeyLockStatus]([@(0.0.255) b
ool]_[*@3 caps], [@(0.0.255) bool]_[*@3 num], [@(0.0.255) bool]_[*@3 scroll])&]
[s2; Sets the status of keys [%-*@3 caps ]lock,  [%-*@3 num ]lock and 
[%-*@3 scroll] [%-*@3  ]lock. Not fully functional in Linux.&]
[s3; &]
[s0; &]
[ {{10000@1 [s0; [* Screen recording]]}}&]
[s3; &]
[s5;:Snap`_Desktop`(String`):%- [@(0.0.255) bool]_[* Snap`_Desktop]([_^String^ String]_[*@3 f
ileName])&]
[s2; Saves the desktop in [%-*@3 fileName].as an image file.&]
[s2; Allowed formats are:&]
[s2;i150;O0; Posix:-|xwd&]
[s2;i150;O0; Windows:-|bmp, png, jpg&]
[s3; &]
[s4;%- &]
[s5;:GetDesktopRect`(`):%- [_^Rect^ Rect]_[* GetDesktopRect]()&]
[s2; Gets the Desktop dimensions.&]
[s3;%- &]
[s4;%- &]
[s5;:Snap`_DesktopRectangle`(String`,int`,int`,int`,int`):%- [@(0.0.255) bool]_[* Snap`_D
esktopRectangle]([_^String^ String]_[*@3 fileName], [@(0.0.255) int]_[*@3 left], 
[@(0.0.255) int]_[*@3 top], [@(0.0.255) int]_[*@3 width], [@(0.0.255) int]_[*@3 height])&]
[s2; Saves a rectangle of the desktop defined by [%-*@3 left], [%-*@3 top], 
[%-*@3 width ]and [%-*@3 height].in [%-*@3 fileName] as an image file.&]
[s2; Allowed formats are:&]
[s2;i150;O0; Posix:-|xwd&]
[s2;i150;O0; Windows:-|bmp, png, jpg&]
[s3; &]
[s4;%- &]
[s5;:Snap`_Window`(String`,int64`):%- [@(0.0.255) bool]_[* Snap`_Window]([_^String^ String]_
[*@3 fileName], [@(0.0.255) int64]_[*@3 handle])&]
[s2; Saves a window defined by its [%-*@3 handle] in [%-*@3 fileName] 
as an image file.&]
[s2; Allowed formats are:&]
[s2;i150;O0; Posix:-|xwd&]
[s2;i150;O0; Windows:-|bmp, png, jpg&]
[s3; &]
[s4;%- &]
[s5;:Snap`_Desktop`(`):%- [_^Image^ Image]_[* Snap`_Desktop]()&]
[s2; Returns the desktop image.&]
[s3;%- &]
[s4;%- &]
[s5;:Snap`_DesktopRectangle`(int`,int`,int`,int`):%- [_^Image^ Image]_[* Snap`_DesktopRec
tangle]([@(0.0.255) int]_[*@3 left], [@(0.0.255) int]_[*@3 top], [@(0.0.255) int]_[*@3 width],
 [@(0.0.255) int]_[*@3 height])&]
[s2; Returns the image of a desktop rectangle defined by [%-*@3 left], 
[%-*@3 top], [%-*@3 width ]and [%-*@3 height].&]
[s3; &]
[s4;%- &]
[s5;:Snap`_Window`(int64`):%- [_^Image^ Image]_[* Snap`_Window]([_^int64^ int64]_[*@3 handle])
&]
[s2; Returns a window image defined by its [%-*@3 handle].&]
[s3; &]
[s4;%- &]
[s5;:Record`_Desktop`(String`,int`,int`,bool`):%- [@(0.0.255) bool]_[* Record`_Desktop]([_^String^ S
tring]_[*@3 fileName], [@(0.0.255) int]_[*@3 duration], [@(0.0.255) int]_[*@3 secsFrame]_`=
_[@3 1], [@(0.0.255) bool]_[*@3 viewMouse]_`=_[@(0.0.255) true])&]
[s2; Records the desktop activity in [%-*@3 fileName] as a video file 
of [%-*@3 duration] in seconds with [%-*@3 secsFrame] seconds between 
frames and recording also mouse movement if [%-*@3 viewMouse].is 
true.&]
[s0;l288; Allowed formats are:&]
[s0;l288;i150;O0; Windows: Uncompressed .avi &]
[s6; Only available in Windows&]
[s3; &]
[s4;%- &]
[s5;:Record`_DesktopRectangle`(String`,int`,int`,int`,int`,int`,int`,bool`):%- [@(0.0.255) b
ool]_[* Record`_DesktopRectangle]([_^String^ String]_[*@3 fileName], 
[@(0.0.255) int]_[*@3 duration], [@(0.0.255) int]_[*@3 left], [@(0.0.255) int]_[*@3 top], 
[@(0.0.255) int]_[*@3 width], [@(0.0.255) int]_[*@3 height], [@(0.0.255) int]_[*@3 secsFrame]_
`=_[@3 1], [@(0.0.255) bool]_[*@3 viewMouse]_`=_[@(0.0.255) true])&]
[s2; Records the desktop activity in the rectangle defined by [%-*@3 left], 
[%-*@3 top], [%-*@3 width ]and [%-*@3 height]. in [%-*@3 fileName] as 
a video file of [%-*@3 duration] in seconds with [%-*@3 secsFrame] 
seconds between frames and recording also mouse movement if [%-*@3 viewMouse].is 
true.&]
[s2; Allowed formats are:&]
[s2;i150;O0; Windows: Uncompressed .avi&]
[s6; Only available in Windows&]
[s3; &]
[s4;%- &]
[s5;:Record`_Window`(String`,int`,long`,int`,bool`):%- [@(0.0.255) bool]_[* Record`_Windo
w]([_^String^ String]_[*@3 fileName], [@(0.0.255) int]_[*@3 duration], 
[@(0.0.255) int64]_[*@3 handle], [@(0.0.255) int]_[*@3 secsFrame]_`=_[@3 1], 
[@(0.0.255) bool]_[*@3 viewMouse]_`=_[@(0.0.255) true])&]
[s2; Records the activity of a window defined by its [%-*@3 handle] 
in [%-*@3 fileName] as a video file of [%-*@3 duration] in seconds 
with [%-*@3 secsFrame] seconds between frames and recording also 
mouse movement if [%-*@3 viewMouse].is true.&]
[s0;l288; Allowed formats are:&]
[s2;i150;O0; Windows: Uncompressed .avi&]
[s6; Only available in Windows&]
[s3; &]
[s0; &]
[ {{10000@1 [s0; [* Miscellaneous functions]]}}&]
[s4;%- &]
[s5;:SetDesktopWallPaper`(const char`*`):%- [@(0.0.255) void]_[* SetDesktopWallPaper]([@(0.0.255) c
onst]_[@(0.0.255) char]_`*[*@3 path])&]
[s2; Sets [%-*@3 path].file as desktop wallpaper. Supports Gnome, Kde 
v3 and Windows desktops.&]
[s0;l288; &]
[s0;l288;i150;O0; In Gnome, [%-*@3 path] has to be .png&]
[s0;l288;i150;O0; In Kde, [%-*@3 path] has to be .png, .gif or .jpg&]
[s0;l288;i150;O0; In Windows, [%-*@3 path] has to be .bmp&]
[s0;l288; &]
[s0;l288; If [%-*@3 path].is empty, the desktop wallpaper is removed. 
&]
[s3; ]]