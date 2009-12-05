topic "SysInfo";
[ $$0,0#00000000000000000000000000000000:Default]
[0 $$1,0#96390100711032703541132217272105:end]
[i448;a25;kKO9;2 $$2,0#37138531426314131252341829483370:item]
[l288;2 $$3,0#27521748481378242620020725143825:desc]
[H6;0 $$4,0#05600065144404261032431302351956:begin]
[{_}%EN-US 
[s0;%- [*R6 SysInfo]&]
[s0; &]
[s0; &]
[ {{10000@1 [s0; [* Running files and commands]]}}&]
[s1; &]
[s2;:GetExtExecutable`(const String`):%- [_^String^ String]_[* GetExtExecutable]([@(0.0.255) c
onst]_[_^String^ String]_[*@3 ext])&]
[s3; Gets the program that will open by default the files with extension 
[%-*@3 ext].&]
[s0;2 &]
[s0; [2 -|][/2 Example:]&]
[s0; [/2 -|GetExtExecutable(`"html`") `-> `"Firefox.exe`"]&]
[s1; &]
[s4;%- &]
[s2;:LaunchFile`(const String`):%- [@(0.0.255) bool]_[* LaunchFile]([@(0.0.255) const]_[_^String^ S
tring]_[*@3 file])&]
[s3; Opens the file [%-*@3 file ]with the adecuated program defined 
in the OS by default.&]
[s0;2 &]
[s0; [/2 -|Example:]&]
[s0; [/2 -|LaunchFile(`"c:`\`\My spreadsheet.txt`"). It will open default 
program (probably Notepad) with document `"My spreadsheet.txt`".]&]
[s1; &]
[s0; &]
[ {{10000@1 [s0; [* Obtaining special folders]]}}&]
[s0;%- &]
[s2;:GetDesktopFolder`(`):%- [_^String^ String]_[* GetDesktopFolder]()&]
[s3; Gets the default Desktop folder path.&]
[s1;%- &]
[s4;%- &]
[s2;:GetProgramsFolder`(`):%- [_^String^ String]_[* GetProgramsFolder]()&]
[s3; Gets the default programs folder path.&]
[s1;%- &]
[s4;%- &]
[s2;:GetAppDataFolder`(`):%- [_^String^ String]_[* GetAppDataFolder]()&]
[s3; Gets the default application data folder path.&]
[s1;%- &]
[s4;%- &]
[s2;:GetMusicFolder`(`):%- [_^String^ String]_[* GetMusicFolder]()&]
[s3; Gets the default music files folder path.&]
[s1;%- &]
[s4;%- &]
[s2;:GetPicturesFolder`(`):%- [_^String^ String]_[* GetPicturesFolder]()&]
[s3; Gets the default picture files folder path.&]
[s1;%- &]
[s4;%- &]
[s2;:GetVideoFolder`(`):%- [_^String^ String]_[* GetVideoFolder]()&]
[s3; Gets the default video files folder path.&]
[s1;%- &]
[s4;%- &]
[s2;:GetPersonalFolder`(`):%- [_^String^ String]_[* GetPersonalFolder]()&]
[s3; Gets the default personal files folder path.&]
[s1;%- &]
[s4;%- &]
[s2;:GetTemplatesFolder`(`):%- [_^String^ String]_[* GetTemplatesFolder]()&]
[s3; Gets the default templates files folder path.&]
[s1;%- &]
[s4;%- &]
[s2;:GetDownloadFolder`(`):%- [_^String^ String]_[* GetDownloadFolder]()&]
[s3; Gets the default file download folder path.&]
[s1;%- &]
[s4;%- &]
[s2;:GetRootFolder`(`):%- [_^String^ String]_[* GetRootFolder]()&]
[s3; Gets the default root folder path.&]
[s1;%- &]
[s4;%- &]
[s2;:GetTempFolder`(`):%- [_^String^ String]_[* GetTempFolder]()&]
[s3; Gets the default temp files folder path.&]
[s1;%- &]
[s4;%- &]
[s2;:GetOsFolder`(`):%- [_^String^ String]_[* GetOsFolder]()&]
[s3; Gets the default operating system files folder path.&]
[s1;%- &]
[s4;%- &]
[s2;:GetSystemFolder`(`):%- [_^String^ String]_[* GetSystemFolder]()&]
[s3; Gets the default system files folder path.&]
[s1;%- &]
[s0; &]
[ {{10000@1 [s0; [* Hardware, BIOS, OS and Distro info]]}}&]
[s0;%- &]
[s2;:GetSystemInfo`(String`&`,String`&`,String`&`,int`&`):%- [@(0.0.255) void]_[* GetSyst
emInfo]([_^String^ String]_`&[*@3 manufacturer], [_^String^ String]_`&[*@3 productName], 
[_^String^ String]_`&[*@3 version], [@(0.0.255) int]_`&[*@3 numberOfProcessors])&]
[s3; Returns hardware information including computer [%-*@3 manufacturer, 
] [%-*@3 productName, ] [%-*@3 version ]and [%-*@3 numberOfProcessors].&]
[s1; &]
[s4;%- &]
[s2;:GetBiosInfo`(String`&`,Date`&`):%- [@(0.0.255) void]_[* GetBiosInfo]([_^String^ String
]_`&[*@3 biosVersion], [_^Date^ Date]_`&[*@3 biosReleaseDate])&]
[s3; Returns bios information including [%-*@3 biosVersion] and [%-*@3 biosReleaseDate].&]
[s1; &]
[s4;%- &]
[s2;:GetProcessorInfo`(int`,String`&`,String`&`,String`&`,int`&`):%- [@(0.0.255) bool]_
[* GetProcessorInfo]([@(0.0.255) int]_[*@3 number], [_^String^ String]_`&[*@3 vendor], 
[_^String^ String]_`&[*@3 identifier], [_^String^ String]_`&[*@3 architecture], 
[@(0.0.255) int]_`&[*@3 speed])&]
[s3; Returns information about the different cpu cores.&]
[s3; [%-*@3 number] is the core number to get the information&]
[s3; [%-*@3 vendor] is the core vendor&]
[s3; [%-*@3 identifier] is the core identifier&]
[s3; [%-*@3 architecture] is the core architecture (32, 64)&]
[s3; [%-*@3 speed].is the core speed in MHz.&]
[s0; &]
[s4;%- &]
[s2;:GetCpuSpeed`(`):%- [@(0.0.255) int]_[* GetCpuSpeed]()&]
[s3; Gets the real time main CPU speed in MHz.&]
[s0; [2 -|This data is directly calculated by the function.]&]
[s1; &]
[s4;%- &]
[s2;:GetVideoInfo`(Array`<Value`>`&`,Array`<Value`>`&`,Array`<Value`>`&`,Array`<Value`>`&`,Array`<Value`>`&`):%- [@(0.0.255) b
ool]_[* GetVideoInfo]([_^Array^ Array]_<[_^Value^ Value]>_`&[*@3 name], 
Array_<Value>_`&[*@3 description], Array_<Value>_`&[*@3 videoProcessor], 
Array_<Value>_`&[*@3 ram], Array_<Value>_`&[*@3 videoMode])&]
[s3; If true gets information about video systems installed:&]
[s3; [%-*@3 name] is the video system name&]
[s3; [%-*@3 description] is the video description&]
[s3; [%-*@3 videoProcessor] is the processor used&]
[s3; [%-*@3 ram ]is the available memory in the video hardware in Mb&]
[s3; [%-*@3 videoMode] is the video mode actually selected including 
screen resolution and number of colors&]
[s1; &]
[s4;%- &]
[s2;:GetPackagesInfo`(Array`<Value`>`&`,Array`<Value`>`&`,Array`<Value`>`&`,Array`<Value`>`&`,Array`<Value`>`&`,Array`<Value`>`&`,Array`<Value`>`&`):%- [@(0.0.255) b
ool]_[* GetPackagesInfo]([_^Array^ Array]_<[_^Value^ Value]>_`&[*@3 name], 
Array_<Value>_`&[*@3 version], Array_<Value>_`&[*@3 vendor], Array_<Value>_`&[*@3 insta
llDate], Array_<Value>_`&[*@3 caption], Array_<Value>_`&[*@3 description], 
Array_<Value>_`&[*@3 state])&]
[s3; If true gets information about installed software:&]
[s3;%- [*@3 name]&]
[s3;%- [*@3 version]&]
[s3;%- [*@3 vendor]&]
[s3;%- [*@3 installDate]&]
[s3; [%-*@3 caption] is a summary of the software description&]
[s3;%- [*@3 description]&]
[s3; [%-*@3 state] is the status of the software. Values are:&]
[s0; [2 -|-|`- `"Bad Configuration`"]&]
[s0; [2 -|-|`- `"Invalid Argument`"]&]
[s0; [2 -|-|`- `"Unknown Package`"]&]
[s0; [2 -|-|`- `"Advertised`"]&]
[s0; [2 -|-|`- `"Absent`"]&]
[s0; [2 -|-|`- `"Ok`"]&]
[s1; &]
[s4;%- &]
[s2;:GetMemoryInfo`(int`&`,uint64`&`,uint64`&`,uint64`&`,uint64`&`,uint64`&`,uint64`&`):%- [@(0.0.255) b
ool]_[* GetMemoryInfo]([@(0.0.255) int]_`&[*@3 memoryLoad], [_^uint64^ uint64]_`&[*@3 total
Phys], [_^uint64^ uint64]_`&[*@3 freePhys], [_^uint64^ uint64]_`&[*@3 totalPageFile], 
[_^uint64^ uint64]_`&[*@3 freePageFile], [_^uint64^ uint64]_`&[*@3 totalVirtual], 
[_^uint64^ uint64]_`&[*@3 freeVirtual])&]
[s3; Gets information about the system memory:&]
[s3; [%-*@3 memoryLoad ]is the percent of memory in use&]
[s3; [%-*@3 totalPhys ]is the total physical memory&]
[s3; [%-*@3 freePhys] is the free physical memory&]
[s3; [%-*@3 totalPageFile ]is the total paging file&]
[s3; [%-*@3 freePageFile ]is the free paging file&]
[s3; [%-*@3 totalVirtual ]is the total virtual memory&]
[s3; [%-*@3 freeVirtual ]is the free virtual memory.&]
[s1; &]
[s4;%- &]
[s2;:GetOsInfo`(String`&`,String`&`,String`&`,String`&`,String`&`,String`&`,String`&`):%- [@(0.0.255) b
ool]_[* GetOsInfo]([_^String^ String]_`&[*@3 kernel], [_^String^ String]_`&[*@3 kerVersion],
 [_^String^ String]_`&[*@3 kerArchitecture], [_^String^ String]_`&[*@3 distro], 
[_^String^ String]_`&[*@3 distVersion], [_^String^ String]_`&[*@3 desktop], 
[_^String^ String]_`&[*@3 deskVersion])&]
[s3; Gets many information to identify the operating system and Desktop 
where the application is being run. &]
[s3; [%-*@3 kernel]: Kernel name&]
[s3; [%-*@3 kerVersion]: Kernel version&]
[s3; [%-*@3 kerArchitecture]: Kernel architecture&]
[s3; [%-*@3 distro]: Distro name&]
[s3; [%-*@3 distVersion]: Distro version&]
[s3; [%-*@3 desktop]: Desktop manager name&]
[s3; [%-*@3 deskVersion].: Desktop manager version&]
[s1; &]
[s4;%- &]
[s2;:GetDesktopManagerNew`(`):%- [_^String^ String]_[* GetDesktopManagerNew]()&]
[s3; A more complete version of GetDesktopManager() based on GetOsInfo().&]
[s0; &]
[s4;%- &]
[s2;:GetDriveList`(`):%- [_^Array^ Array]<[_^String^ String]>_[* GetDriveList]()&]
[s3; Returns an array with the paths to all drives, internal or external, 
identified in the system.&]
[s1;%- &]
[s4;%- &]
[s2;:GetDriveSpace`(String`,uint64`&`,uint64`&`,uint64`&`):%- [@(0.0.255) bool]_[* GetDri
veSpace]([_^String^ String]_[*@3 drive], [_^uint64^ uint64]_`&[*@3 freeBytesUser], 
[_^uint64^ uint64]_`&[*@3 totalBytesUser], [_^uint64^ uint64]_`&[*@3 totalFreeBytes])&]
[s3; Gets [%-*@3 drive] space.&]
[s3; [%-*@3 freeBytesUser]: Amount of free bytes available to the user&]
[s3; [%-*@3 totalBytesUser]: Size of drive visible for the user&]
[s3; [%-*@3 totalFreeBytes]: Amount of free bytes.&]
[s0; [2 -|Returns false if drive is not mounted or it is not accessible]&]
[s1; &]
[s4;%- &]
[s2;:GetDriveInformation`(String`,String`&`,String`&`,int`&`,String`&`):%- [@(0.0.255) b
ool]_[* GetDriveInformation]([_^String^ String]_[*@3 drive], [_^String^ String]_`&[*@3 type
], [_^String^ String]_`&[*@3 volume], [@(0.0.255) int]_`&[*@3 maxName], 
[_^String^ String]_`&[*@3 fileSystem])&]
[s3; Gets [%-*@3 drive] information&]
[s3;%- [*@3 type]: Gets the type of the drive.&]
[s3;%- Available types are `"Hard`", `"Network`", `"Optical`", `"RAM`", 
`"Removable`".&]
[s3;%- [*@3 volume]: Gets the name of the drive&]
[s3;%- [*@3 maxName]: Gets the maximum length permitted for a file name&]
[s3;%- [*@3 fileSystem]: Gets the drive formatting system.&]
[s0; [2 -|Returns false if drive is not mounted or it is not accessible]&]
[s1; &]
[s4;%- &]
[s2;:GetCompilerInfo`(String`&`,int`&`,String`&`):%- [@(0.0.255) void]_[* GetCompilerInfo
]([_^String^ String]_`&[*@3 name], [@(0.0.255) int]_`&[*@3 version], 
[_^String^ String]_`&[*@3 date])&]
[s3; Returns compiling information, like compiler [%-*@3 name, ]compiler 
[%-*@3 version] and program compilation [%-*@3 date].&]
[s1; &]
[s4;%- &]
[s2;:GetBatteryStatus`(bool`&`,int`&`,int`&`):%- [@(0.0.255) bool]_[* GetBatteryStatus]([@(0.0.255) b
ool]_`&[*@3 discharging], [@(0.0.255) int]_`&[*@3 percentage], [@(0.0.255) int]_`&[*@3 rema
iningMin])&]
[s3; Gets battery information like if it is [%-*@3 discharging] or 
connected to the grid, [%-*@3 percentage] of charging where 100% 
means full charge, and number of expected computer running minutes 
in [%-*@3 remainingMin].&]
[s0; [2 -|Returns true if the values got are valid.]&]
[s1; &]
[s4;%- &]
[s2;:GetBatteryInfo`(bool`&`):%- [@(0.0.255) bool]_[* GetBatteryInfo]([@(0.0.255) bool]_`&[*@3 p
resent]_)&]
[s3; Gets if battery is [%-*@3 present ]or not.&]
[s0; [2 -|Returns true if the values got are valid.]&]
[s1; &]
[s4;%- &]
[s2;:OpenCDTray`(`):%- [@(0.0.255) void]_[* OpenCDTray]()&]
[s3; Ejects CD/DVD tray.&]
[s1; &]
[s4;%- &]
[s2;:CloseCDTray`(`):%- [@(0.0.255) void]_[* CloseCDTray]()&]
[s3; Closes CD/DVD tray if possible&]
[s1;@(0.0.255)%- &]
[ {{10000@1 [s0; [* Process handling]]}}&]
[s1;%- &]
[s2;:GetWindowsList`(Array`<long`>`&`,Array`<long`>`&`,Array`<String`>`&`,Array`<String`>`&`,Array`<String`>`&`):%- [@(0.0.255) v
oid]_[* GetWindowsList]([_^Array^ Array]<[@(0.0.255) long]>_`&[*@3 wid], 
[_^Array^ Array]<[@(0.0.255) long]>_`&[*@3 pid], [_^Array^ Array]<[_^String^ String]>_`&[*@3 n
ame], [_^Array^ Array]<[_^String^ String]>_`&[*@3 fileName], [_^Array^ Array]<[_^String^ St
ring]>_`&[*@3 title])&]
[s3; Gets arrays with handles to all the opened windows with additional 
info as:&]
[s3; [%-*@3 wid]: Handle to the the window&]
[s3; [%-*@3 pid]: Handle to the process that manages the window&]
[s3; [%-*@3 name]: Window name&]
[s3; [%-*@3 fileName]: Window process program file name&]
[s3; [%-*@3 title]: Window title (caption)&]
[s1; &]
[s4;%- &]
[s2;:GetWindowsList`(`):%- [_^Array^ Array]<[@(0.0.255) long]>_[* GetWindowsList]()&]
[s3; Gets an array with handles to all the opened windows.&]
[s1;%- &]
[s4;%- &]
[s2;:GetProcessList`(Array`<long`>`&`,Array`<String`>`&`):%- [@(0.0.255) bool]_[* GetProc
essList]([_^Array^ Array]<[@(0.0.255) long]>_`&[*@3 pid], [_^Array^ Array]<[_^String^ Strin
g]>_`&[*@3 pNames])&]
[s3; Gets arrays with handles to all the opened processes [%-*@3 pid 
]and process names [%-*@3 pNames].&]
[s1; &]
[s4;%- &]
[s2;:GetProcessList`(`):%- [_^Array^ Array]<[@(0.0.255) long]>_[* GetProcessList]()&]
[s3; Gets an array with handles to all the opened processes&]
[s1;%- &]
[s4;%- &]
[s2;:GetProcessName`(long`):%- [_^String^ String]_[* GetProcessName]([@(0.0.255) long]_[*@3 p
id])&]
[s3; Returns the process name for a process with handle [%-*@3 pid].&]
[s1; &]
[s4;%- &]
[s2;:GetProcessFileName`(long`):%- [_^String^ String]_[* GetProcessFileName]([@(0.0.255) lo
ng]_[*@3 processID])&]
[s3; Gets the program file name of a process with handle [%-*@3 processID].&]
[s1; &]
[s4;%- &]
[s2;:GetProcessIdFromWindowCaption`(String`,bool`):%- [@(0.0.255) long]_[* GetProcessIdFr
omWindowCaption]([_^String^ String]_[*@3 windowCaption], [@(0.0.255) bool]_[*@3 exactMatc
h]_`=_[@(0.0.255) false])&]
[s3;%- [%% Gets the process handle of a program with a window with 
title ][*@3 windowCaption.]&]
[s3;%- [%% If ][*@3 exactMatch][%% .is true it only returns the process 
handle of a process with a window title that is equal to ][*@3 windowCaption. 
][%% If it is false then the handle is returned if only part of 
the window title matches with ][*@3 windowCaption.]&]
[s1; &]
[s4;%- &]
[s2;:GetWindowIdFromCaption`(String`,bool`):%- [@(0.0.255) long]_[* GetWindowIdFromCaptio
n]([_^String^ String]_[*@3 windowCaption], [@(0.0.255) bool]_[*@3 exactMatch]_`=_[@(0.0.255) f
alse])&]
[s3;%- [%% Gets the window handle of a program with a window with title 
][*@3 windowCaption.]&]
[s3;%- [%% If ][*@3 exactMatch][%% .is true it only returns the process 
handle of a process with a window title that is equal to ][*@3 windowCaption. 
][%% If it is false then the handle is returned if only part of 
the window title matches with ][*@3 windowCaption.]&]
[s1; &]
[s4;%- &]
[s2;:GetProcessIdFromWindowId`(long`):%- [@(0.0.255) long]_[* GetProcessIdFromWindowId]([@(0.0.255) l
ong]_[*@3 wid])&]
[s3;%- [%% Returns the process handle of a program with window handle 
][*@3 wid.]&]
[s1; &]
[s4;%- &]
[s2;:GetWindowIdFromProcessId`(long`):%- [@(0.0.255) long]_[* GetWindowIdFromProcessId]([@(0.0.255) l
ong]_[*@3 pid])&]
[s3;%- [%% Returns the window handle of a program with process handle 
][*@3 pid.]&]
[s1; &]
[s4;%- &]
[s2;:ProcessTerminate`(long`,int`):%- [@(0.0.255) bool]_[* ProcessTerminate]([@(0.0.255) lo
ng]_[*@3 pid], [@(0.0.255) int]_[*@3 timeout]_`=_[@3 500])&]
[s3;%- [%% Ends the program with handle ][*@3 pid.]&]
[s3; If after asking the process to end [%-*@3 timeout ]is over, it 
will kill the process by different means in order of increasing 
`"aggressivity`".&]
[s0; [2 -|For example in Posix it will send the process first a SIGTERM, 
if the process does not stop it will send a SIGKILL, and if the 
process remains running it will simply call WindowKill() to do 
the dirty job.]&]
[s1; &]
[s4;%- &]
[s2;:GetProcessPriority`(long`):%- [@(0.0.255) int]_[* GetProcessPriority]([@(0.0.255) long
]_[*@3 pid])&]
[s3; Gets the process priority of the process with handle [%-*@3 pid] 
as a number from 0 (minimum) to 10 (maximum), if possible.&]
[s1; &]
[s4;%- &]
[s2;:SetProcessPriority`(long`,int`):%- [@(0.0.255) bool]_[* SetProcessPriority]([@(0.0.255) l
ong]_[*@3 pid], [@(0.0.255) int]_[*@3 priority])&]
[s3; Sets the process priority to [%-*@3 priority ]of the process with 
handle [%-*@3 pid] as a number from 0 (minimum) to 10 (maximum), 
if possible.&]
[s1; &]
[s4;%- &]
[s2;:ProcessExists`(long`):%- [@(0.0.255) bool]_[* ProcessExists]([@(0.0.255) long]_[*@3 pid])
&]
[s3; Returns true if a process with handle [%-*@3 pid ]exists.&]
[s4;%- &]
[s2;:GetProcessId`(`):%- [@(0.0.255) long]_[* GetProcessId]()&]
[s3; Gets actual running process handle.&]
[s1;%- &]
[s4;%- &]
[s2;:Shutdown`(String`):%- [@(0.0.255) bool]_[* Shutdown]([_^String^ String]_[*@3 action])&]
[s3; Tries to logoff, reboot or shutdown the actual running session.&]
[s0; [2 -|Actual valid ][%-*@3;2 action ][2 values are `"logoff`", `"reboot`" 
and `"shutdown`".]&]
[s1; &]
[s0; &]
[ {{10000@1 [s0; [* Windows handling]]}}&]
[s0; &]
[s2;:Window`_GetRect`(long`,long`&`,long`&`,long`&`,long`&`):%- [@(0.0.255) bool]_[* Wind
ow`_GetRect]([@(0.0.255) long]_[*@3 windowId], [@(0.0.255) long]_`&[*@3 left], 
[@(0.0.255) long]_`&[*@3 top], [@(0.0.255) long]_`&[*@3 right], [@(0.0.255) long]_`&[*@3 bott
om])&]
[s3; Giving this function the [%-*@3 windowId], it returns the window 
location in the screen in [%-*@3 left], [%-*@3 top], [%-*@3 right] 
and [%-*@3 bottom].&]
[s0; [2 -|Returns true if the values got are valid.]&]
[s1; &]
[s4;%- &]
[s2;:Window`_SetRect`(long`,long`,long`,long`,long`):%- [@(0.0.255) void]_[* Window`_SetR
ect]([@(0.0.255) long]_[*@3 windowId], [@(0.0.255) long]_[*@3 left], 
[@(0.0.255) long]_[*@3 top], [@(0.0.255) long]_[*@3 right], [@(0.0.255) long]_[*@3 bottom])&]
[s3; Giving this function the [%-*@3 windowId], it sets the window 
location in the screen in [%-*@3 left], [%-*@3 top], [%-*@3 right] 
and [%-*@3 bottom].&]
[s3; -|Returns true if the window is relocated correctly..&]
[s0; &]
[ {{10000@1 [s0; [* Mouse and keyboard handling]]}}&]
[s1; &]
[s2;:Mouse`_GetPos`(long`&`,long`&`):%- [@(0.0.255) bool]_[* Mouse`_GetPos]([@(0.0.255) lon
g]_`&[*@3 x], [@(0.0.255) long]_`&[*@3 y])&]
[s3; Gets the mouse position [%-*@3 x, y].in screen pixels where upper 
left corner is (0, 0).&]
[s0; [2 -|Returns true if the operation has been done successfully].&]
[s1; &]
[s4;%- &]
[s2;:Mouse`_SetPos`(long`,long`,long`):%- [@(0.0.255) bool]_[* Mouse`_SetPos]([@(0.0.255) l
ong]_[*@3 x], [@(0.0.255) long]_[*@3 y], [@(0.0.255) long]_[*@3 windowId])&]
[s3; Sets the mouse position to [%-*@3 x] [%-*@3 y] referenced to the 
upper left vertex of window with window handle [%-*@3 windowId].&]
[s0; [2 -|Returns true if the operation has been done successfully.]&]
[s1; &]
[s4;%- &]
[s2;:Mouse`_LeftClick`(`):%- [@(0.0.255) void]_[* Mouse`_LeftClick]()&]
[s3; Simulates by software a mouse click with the left button as 
if it would have been done with the mouse.&]
[s1;%- &]
[s4;%- &]
[s2;:Mouse`_MiddleClick`(`):%- [@(0.0.255) void]_[* Mouse`_MiddleClick]()&]
[s3; Simulates by software a mouse click with the middle button as 
if it would have been done with the mouse.&]
[s1;%- &]
[s4;%- &]
[s2;:Mouse`_RightClick`(`):%- [@(0.0.255) void]_[* Mouse`_RightClick]()&]
[s3; Simulates by software a mouse click with the right button as 
if it would have been done with the mouse.&]
[s1;%- &]
[s4;%- &]
[s2;:Mouse`_LeftDblClick`(`):%- [@(0.0.255) void]_[* Mouse`_LeftDblClick]()&]
[s3; Simulates by software a mouse double click with the left button 
as if it would have been done with the mouse.&]
[s1;%- &]
[s4;%- &]
[s2;:Mouse`_MiddleDblClick`(`):%- [@(0.0.255) void]_[* Mouse`_MiddleDblClick]()&]
[s3; Simulates by software a mouse double click with the middle button 
as if it would have been done with the mouse.&]
[s1;%- &]
[s4;%- &]
[s2;:Mouse`_RightDblClick`(`):%- [@(0.0.255) void]_[* Mouse`_RightDblClick]()&]
[s3; Simulates by software a mouse double click with the right button 
as if it would have been done with the mouse.&]
[s1;%- &]
[s4;%- &]
[s2;:Keyb`_SendKeys`(String`,long`,long`):%- [@(0.0.255) void]_[* Keyb`_SendKeys]([_^String^ S
tring]_[*@3 text], [@(0.0.255) long]_[*@3 finalDelay]_`=_[@3 100], [@(0.0.255) long]_[*@3 del
ayBetweenKeys]_`=_[@3 50])&]
[s3; Simulates by software a text entered using the keyboard as if 
it would have been entered with the keyboard.&]
[s3; To really simulate manual key typing the function lets to enter 
delays between keys and after entering the text.&]
[s3; [%-*@3 text]: Is the text to be entered&]
[s3; [%-*@3 finalDelay]: Is the delay in ms that is forced after entering 
text&]
[s3; [%-*@3 delayBetweenKeys]: Is the delay in ms that is forced between 
text keys.&]
[s1; &]
[s4;%- &]
[s2;:GetKeyLockStatus`(bool`&`,bool`&`,bool`&`):%- [@(0.0.255) void]_[* GetKeyLockStatus](
[@(0.0.255) bool]_`&[*@3 caps], [@(0.0.255) bool]_`&[*@3 num], [@(0.0.255) bool]_`&[*@3 scrol
l])&]
[s3; Gets the status of keys [%-*@3 caps ]lock,  [%-*@3 num ]lock and 
[%-*@3 scroll] [%-*@3  ]lock.&]
[s1; &]
[s4;%- &]
[s2;:SetKeyLockStatus`(bool`,bool`,bool`):%- [@(0.0.255) void]_[* SetKeyLockStatus]([@(0.0.255) b
ool]_[*@3 caps], [@(0.0.255) bool]_[*@3 num], [@(0.0.255) bool]_[*@3 scroll])&]
[s3; Sets the status of keys [%-*@3 caps ]lock,  [%-*@3 num ]lock and 
[%-*@3 scroll] [%-*@3  ]lock.&]
[s1; &]
[ {{10000@1 [s0; [* Screen recording]]}}&]
[s1; &]
[s2;:Snap`_Desktop`(String`):%- [@(0.0.255) bool]_[* Snap`_Desktop]([_^String^ String]_[*@3 f
ileName])&]
[s3; Saves the desktop in [%-*@3 fileName].as an image file.&]
[s0; [2 -|Allowed formats are:]&]
[s0; [2 -|-|`- Posix: .xwd]&]
[s0; [2 -|-|`- Windows: .bmp]&]
[s1; &]
[s4;%- &]
[s2;:Snap`_DesktopRectangle`(String`,int`,int`,int`,int`):%- [@(0.0.255) bool]_[* Snap`_D
esktopRectangle]([_^String^ String]_[*@3 fileName], [@(0.0.255) int]_[*@3 left], 
[@(0.0.255) int]_[*@3 top], [@(0.0.255) int]_[*@3 width], [@(0.0.255) int]_[*@3 height])&]
[s3; Saves a rectangle of the desktop defined by [%-*@3 left], [%-*@3 top], 
[%-*@3 width ]and [%-*@3 height].in [%-*@3 fileName] as an image file.&]
[s0; [2 -|Allowed formats are:]&]
[s0; [2 -|-|`- Posix: .xwd]&]
[s0; [2 -|-|`- Windows: .bmp]&]
[s1; &]
[s4;%- &]
[s2;:Snap`_Window`(String`,long`):%- [@(0.0.255) bool]_[* Snap`_Window]([_^String^ String]_
[*@3 fileName], [@(0.0.255) long]_[*@3 handle])&]
[s3; Saves a window defined by its [%-*@3 handle] in [%-*@3 fileName] 
as an image file.&]
[s0; [2 -|Allowed formats are:]&]
[s0; [2 -|-|`- Posix: .xwd]&]
[s0; [2 -|-|`- Windows: .bmp]&]
[s1; &]
[s4;%- &]
[s2;:Record`_Desktop`(String`,int`,int`,bool`):%- [@(0.0.255) bool]_[* Record`_Desktop]([_^String^ S
tring]_[*@3 fileName], [@(0.0.255) int]_[*@3 duration], [@(0.0.255) int]_[*@3 secsFrame]_`=
_[@3 1], [@(0.0.255) bool]_[*@3 viewMouse]_`=_[@(0.0.255) true])&]
[s3; Records the desktop activity in [%-*@3 fileName] as a video file 
of [%-*@3 duration] in seconds with [%-*@3 secsFrame] seconds between 
frames and recording also mouse movement if [%-*@3 viewMouse].is 
true.&]
[s0; [2 -|Allowed formats are:]&]
[s0; [2 -|-|`- Windows: Uncompressed .avi ]&]
[s1; &]
[s4;%- &]
[s2;:Record`_DesktopRectangle`(String`,int`,int`,int`,int`,int`,int`,bool`):%- [@(0.0.255) b
ool]_[* Record`_DesktopRectangle]([_^String^ String]_[*@3 fileName], 
[@(0.0.255) int]_[*@3 duration], [@(0.0.255) int]_[*@3 left], [@(0.0.255) int]_[*@3 top], 
[@(0.0.255) int]_[*@3 width], [@(0.0.255) int]_[*@3 height], [@(0.0.255) int]_[*@3 secsFrame]_
`=_[@3 1], [@(0.0.255) bool]_[*@3 viewMouse]_`=_[@(0.0.255) true])&]
[s3; Records the desktop activity in the rectangle defined by [%-*@3 left], 
[%-*@3 top], [%-*@3 width ]and [%-*@3 height]. in [%-*@3 fileName] as 
a video file of [%-*@3 duration] in seconds with [%-*@3 secsFrame] 
seconds between frames and recording also mouse movement if [%-*@3 viewMouse].is 
true.&]
[s0; [2 -|Allowed formats are:]&]
[s3; -|-|`- Windows: Uncompressed .avi&]
[s1; &]
[s4;%- &]
[s2;:Record`_Window`(String`,int`,long`,int`,bool`):%- [@(0.0.255) bool]_[* Record`_Windo
w]([_^String^ String]_[*@3 fileName], [@(0.0.255) int]_[*@3 duration], 
[@(0.0.255) long]_[*@3 handle], [@(0.0.255) int]_[*@3 secsFrame]_`=_[@3 1], 
[@(0.0.255) bool]_[*@3 viewMouse]_`=_[@(0.0.255) true])&]
[s3; Records the activity of a window defined by its [%-*@3 handle] 
in [%-*@3 fileName] as a video file of [%-*@3 duration] in seconds 
with [%-*@3 secsFrame] seconds between frames and recording also 
mouse movement if [%-*@3 viewMouse].is true.&]
[s0; [2 -|Allowed formats are:]&]
[s3; -|-|`- Windows: Uncompressed .avi&]
[s1; &]
[ {{10000@1 [s0; [* Miscellaneous functions]]}}&]
[s0; &]
[s2;:LoadFile`_Safe`(String`):%- [_^String^ String]_[* LoadFile`_Safe]([_^String^ String]_[*@3 f
ileName])&]
[s3; Same as LoadFile([%-*@3 fileName]) but it works in Posix for files 
automatically generated by the OS.&]
[s0;2 &]
[s0; [2 U`+`+ LoadFile() functions prior to loading the file into a 
String, get the length of the file to dimension the String that 
will get the file. This is not valid for OS generated virtual 
files where the file length returned by the OS is 0 (for example 
files under folder /proc)]&]
[s0;2 &]
[s0; [2 LoadFile`_Safe() just get the file bytes returned by the OS 
until the file end.]&]
[s1; &]
[s4;%- &]
[s2;:FileToTrashBin`(const char`*`):%- [@(0.0.255) void]_[* FileToTrashBin]([@(0.0.255) con
st]_[@(0.0.255) char]_`*[*@3 path])&]
[s3; Deletes file [%-*@3 path].by sending it to the Trash Bin.&]
[s1; &]
[s4;%- &]
[s2;:TrashBinGetCount`(`):%- [_^int64^ int64]_[* TrashBinGetCount]()&]
[s3; Returns the number of items (files and directories) located 
in the Trash Bin.&]
[s1;%- &]
[s4;%- &]
[s2;:TrashBinClear`(`):%- [@(0.0.255) void]_[* TrashBinClear]()&]
[s3; Removes all the items (files and directories) located in the 
Trash Bin.&]
[s1;%- &]
[s4;%- &]
[s2;:SetDesktopWallPaper`(const char`*`):%- [@(0.0.255) void]_[* SetDesktopWallPaper]([@(0.0.255) c
onst]_[@(0.0.255) char]_`*[*@3 path])&]
[s3; Sets [%-*@3 path].file as desktop wallpaper. Supports Gnome, Kde 
v3 and Windows desktops.&]
[s0;2 &]
[s0;i150;O0; [2 In Gnome, ][%-*@3;2 path][2  has to be .png]&]
[s0;i150;O0; [2 In Kde, ][%-*@3;2 path][2  has to be .png, .gif or .jpg]&]
[s0;i150;O0; [2 In Windows, ][%-*@3;2 path][2  has to be .bmp]&]
[s0;2 &]
[s0; [2 If ][%-*@3;2 path][2 .is empty, the desktop wallpaper is removed. 
]&]
[s0; &]
[s4;%- &]
[s2;:FileCat`(const char`*`,const char`*`):%- [@(0.0.255) bool]_[* FileCat]([@(0.0.255) con
st]_[@(0.0.255) char]_`*[*@3 file], [@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 appendFile])
&]
[s3; Appends at the end of [%-*@3 file] the contents of file [%-*@3 appendFile]. 
[%-*@3 file] will be modified and [%-*@3 appendFile ]remains unchanged.&]
[s0; [2 -|Returns true if the operations has been done succesfully.]&]
[s1;%- &]
[s4;%- &]
[s2;:Replace`(String`,String`,String`):%- [_^String^ String]_[* Replace]([_^String^ String]_
[*@3 str], [_^String^ String]_[*@3 find], [_^String^ String]_[*@3 replace])&]
[s3; Returns the resulting String obtained by replacing in [%-*@3 str] 
String [%-*@3 find] with [%-*@3 replace ]all the times that [%-*@3 find 
]appears in [%-*@3 str].&]
[s0; &]
[s4;%- &]
[s2;:ReverseFind`(const String`&`,const String`&`,int`):%- [@(0.0.255) int]_[* ReverseFin
d]([@(0.0.255) const]_[_^String^ String][@(0.0.255) `&]_[*@3 s], [@(0.0.255) const]_[_^String^ S
tring][@(0.0.255) `&]_[*@3 toFind], [@(0.0.255) int]_[*@3 from]_`=_[@3 0])&]
[s3; Find in [%-*@3 s] the string [%-*@3 toFind] beginning in [%-*@3 from 
]in reverse order.&]
[s3; Similar to Find() but searching to the beginning of the String.&]
[s1; &]
[s4;%- &]
[s2;:FormatLong`(long`):%- [_^String^ String]_[* FormatLong]([@(0.0.255) long]_[*@3 a])&]
[s3; Returns the long number [%-*@3 a ]converted into a String.&]
[s0; &]
[s4;%- &]
[s2;:Sign`(T`):%- [@(0.0.255) template]_<[@(0.0.255) class]_[*@4 T]>_[@(0.0.255) int]_[* Sign](
[*@4 T]_[*@3 a])&]
[s3; Returns&]
[s3;i150;O0;  `+1 if [%-*@3 a]. is positive&]
[s3;i150;O0; 0 if [%-*@3 a] is 0 and &]
[s3;i150;O0; `-1 if [%-*@3 a] is negative.&]
[s1; &]
[s4;%- &]
[s2;:GetUpperFolder`(String`):%- [_^String^ String]_[* GetUpperFolder]([_^String^ String]_[*@3 f
olderName])&]
[s3; Returns the name of the upper folder over [%-*@3 folderName].&]
[s1; &]
[s4;%- &]
[s2;:CreateFolderDeep`(const char`*`):%- [@(0.0.255) bool]_[* CreateFolderDeep]([@(0.0.255) c
onst]_[@(0.0.255) char]_`*[*@3 dir])&]
[s3; Creates folder [%-*@3 dir], creating intermediate folders if necessary&]
[s1; &]
[s4;%- &]
[s2;:SecondsToString`(double`,bool`):%- [_^String^ String]_[* SecondsToString]([@(0.0.255) d
ouble]_[*@3 seconds], [@(0.0.255) bool]_[*@3 decimals]_`=_[@(0.0.255) false])&]
[s3; Converts an amount of [%-*@3 seconds] to a String formatted as 
HH:MM:SS.&]
[s0; [2 -|If ][%-*@3;2 decimals ][2 it includes decimals in seconds]&]
[s0; [2 -|It is opposite to StringToSeconds()]&]
[s1; &]
[s4;%- &]
[s2;:StrToTime`(struct Time`&`,const char`*`):%- [@(0.0.255) const]_[@(0.0.255) char]_`*[* S
trToTime]([@(0.0.255) struct]_[_^Time^ Upp`::Time][@(0.0.255) `&]_[*@3 d], 
[@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 s])&]
[s3;%- Scans a string for a Time. Order of day, month and year is 
specified using SetDateScan. In place of month both number and 
text is accepted `- text must match abbreviated or full name 
of month.&]
[s0;%- [2 -|Similar to StrToDate() but including both date and time.]&]
[s3;%- [*@3 d]-|Found time.&]
[s3;%- [*@3 s]-|String to scan.&]
[s3;%- Return value-|NULL if no time is found in [*@3 s] or pointer 
to character right after the date.&]
[s1; &]
[s4;%- &]
[s2;:StringToSeconds`(String`):%- [@(0.0.255) double]_[* StringToSeconds]([_^String^ String
]_[*@3 str])&]
[s3; It converts a String [%-*@3 str ]formatted as HH:MM:SS to a returned 
amount of seconds.&]
[s0; [2 -|It is opposite to SecondsToString()]&]
[s1; &]
[s4;%- &]
[s2;:BytesToString`(uint64`):%- [_^String^ String]_[* BytesToString]([_^uint64^ uint64]_[*@3 b
ytes])&]
[s3; Converts an amount of [%-*@3 bytes ]to a short String.&]
[s1; &]
[s4;%- &]
[s2;:DoEvents`(`):%- [@(0.0.255) void]_[* DoEvents]()&]
[s3; A ProcessEvents just for old Visual Basic programmers.&]
[s1; &]
[s4;%- &]
[s2;:SearchFile`(String`,String`,String`):%- [_^Array^ Array]<[_^String^ String]>_[* Search
File]([_^String^ String]_[*@3 dir], [_^String^ String]_[*@3 condFile], 
[_^String^ String]_[*@3 text]_`=_`"`")&]
[s3; Returns an Array of Strings containing the file names with full 
path of the files under folder [%-*@3 dir] that comply with condition 
(with wildcards) [%-*@3 condFile] and that contain inside the text 
[%-*@3 text].&]
[s0; ]