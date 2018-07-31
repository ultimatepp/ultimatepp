topic "SFtp";
[i448;a25;kKO9;2 $$1,0#37138531426314131252341829483380:class]
[l288;2 $$2,2#27521748481378242620020725143825:desc]
[0 $$3,0#96390100711032703541132217272105:end]
[H6;0 $$4,0#05600065144404261032431302351956:begin]
[i448;a25;kKO9;2 $$5,0#37138531426314131252341829483370:item]
[l288;a4;*@5;1 $$6,6#70004532496200323422659154056402:requirement]
[l288;i1121;b17;O9;~~~.1408;2 $$7,0#10431211400427159095818037425705:param]
[i448;b42;O9;2 $$8,8#61672508125594000341940100500538:tparam]
[b42;2 $$9,9#13035079074754324216151401829390:normal]
[2 $$0,0#00000000000000000000000000000000:Default]
[{_} 
[ {{10000@(113.42.0) [s0;%% [*@7;4 SFtp]]}}&]
[s3; &]
[s1;:Upp`:`:SFtp`:`:class: [@(0.0.255)3 class][3 _][*3 SFtp ][3 :_][@(0.0.255)3 public][3 _][*@3;3 S
sh]&]
[s0;#l288;%% This class encapsulates an SSH2 SFTP subsystem. It allows 
for a range of operations on remote file system objects, such 
as file/directory creation, deletion, renaming, file transfers, 
directory listings etc. SFtp class is derived from Ssh base class, 
and has pick semantics.&]
[s3; &]
[ {{10000F(128)G(128)@1 [s0;%% [* Public Method List]]}}&]
[s3; &]
[s5;:Upp`:`:SFtp`:`:Timeout`(int`): [_^Upp`:`:SFtp^ SFtp][@(0.0.255) `&]_[* Timeout]([@(0.0.255) i
nt]_[*@3 ms])&]
[s2;%% Sets timeout value in miliseconds. Setting the timeout value 
to 0 puts the SFtp object into non`-blocking mode (same as NonBlocking(true)). 
Returns `*this for methods chaining.&]
[s3;%% &]
[s4; &]
[s5;:Upp`:`:SFtp`:`:NonBlocking`(bool`): [_^Upp`:`:SFtp^ SFtp][@(0.0.255) `&]_[* NonBlockin
g]([@(0.0.255) bool]_[*@3 b]_`=_[@(0.0.255) true])&]
[s2;%% Activates or deactivates non`-blocking mode. Default is blocking 
mode. Returns `*this for methods chaining.&]
[s3;%% &]
[s4; &]
[s5;:Upp`:`:SFtp`:`:WaitStep`(int`): [_^Upp`:`:SFtp^ SFtp][@(0.0.255) `&]_[* WaitStep]([@(0.0.255) i
nt]_[*@3 ms])&]
[s2;%% Sets the periodicity of calling [^topic`:`/`/SSH`/src`/Upp`_Ssh`_Session`_en`-us`#Upp`:`:SshSession`:`:WhenWait^ W
henWait] in millisecond between calls. Default value is 10ms 
(100hz). Returns `*this for method chaining.&]
[s3;%% &]
[s4; &]
[s5;:Upp`:`:SFtp`:`:ChunkSize`(int`): [_^Upp`:`:SFtp^ SFtp][@(0.0.255) `&]_[* ChunkSize]([@(0.0.255) i
nt]_[*@3 sz])&]
[s2;%% Sets the chunk size to [%-*@3 sz] for data reads and writes. 
Returns `*this for methods chaining.&]
[s3;%% &]
[s4; &]
[s5;:Upp`:`:SFtp`:`:GetHandle`(`)const: [_^LIBSSH2`_SFTP`_HANDLE^ LIBSSH2`_SFTP`_HANDLE
][@(0.0.255) `*]_[* GetHandle]()_[@(0.0.255) const]&]
[s2;%% Returns the libssh2 sftp session handle on success, NULL on 
failure.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:SFtp`:`:GetResult`(`)const: [_^Upp`:`:Value^ Value]_[* GetResult]()_[@(0.0.255) c
onst]&]
[s2;%% This method returns the result of last operation if that operation 
returns a value (e.g. this method will return a boolean value 
for SFtp`::FileExists(), and int64 for SFtp`::GetSize(), DirEntry 
for SFtp`::GetInfo(), etc.). Intented to be used in non`-blocking 
mode.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:SFtp`:`:Open`(const Upp`:`:String`&`,Upp`:`:dword`,long`): [_^Upp`:`:SFtpHandle^ S
FtpHandle][@(0.0.255) `*]_[* Open]([@(0.0.255) const]_[_^Upp`:`:String^ String][@(0.0.255) `&
]_[*@3 path], [_^Upp`:`:dword^ dword]_[*@3 flags], [@(0.0.255) long]_[*@3 mode])&]
[s0;l288;%% Opens the remote file or directory at [%-*@3 path] with 
access [%-*@3 flags] and [%-*@3 mode]. Returns a file handle on success, 
and NULL on failure. File handles obtained using this method 
must be released using the Close() method when finished.&]
[s2;%% &]
[s2;%% Filesystem access [%-*@3 flags] :&]
[s2;%% &]
[s7;i1440;a17;:Ctrl`:`:CENTER:~~~.1728;%% [%-*C@3 READ]-|Opens file 
in read mode.&]
[s7;i1440;a17;:Ctrl`:`:LEFT:~~~.1728~1728;%% [%-*C@3 WRITE]-|Opens file 
in write mode.&]
[s7;i1440;a17;:Ctrl`:`:RIGHT:~~~.1728~1728;%% [%-*C@3 APPEND]-|Opens 
file in append mode.&]
[s7;i1440;a17;:Ctrl`:`:TOP:~~~.1728~1728;%% [%-*C@3 CREATE]-|Creates 
file if it doesn`'t exist.&]
[s7;i1440;~~~.1728~1728;%% [%-*C@3 EXCLUSIVE]-|Causes  the request to 
fail if the named file already exists. CREATE flag must also 
be specified if this flag is used.&]
[s7;i1440;a17;:Ctrl`:`:BOTTOM:~~~.1728~1728;%% [%-*C@3 TRUNCATE]-|Opens 
file and truncates it to zero (0) bytes. Must be used with CREATE 
flag.&]
[s3;%% &]
[s2;%% Filesystem access [%-*@3 mode]s (permissions):&]
[s2;%% &]
[s7;i1440;~~~.1728~1440~1440~1440;%% [*C@3 IRUSR, IWUSR, IXUSR, IRWXU]-|User 
permissions.&]
[s7;i1440;~~~.1728;%% [%-*C@3 IRGRP, IWGRP, IXGRP, IRWXG]-|Group permissions&]
[s7;i1440;~~~.1728;%% [%-*C@3 IROTH, IWOTH, IXOTH, IRWXO]-|Permissions 
for others.&]
[s7;i1440;~~~.1728;%% [%-*C@3 IRALL]-|Read access for all.&]
[s7;i1440;~~~.1728;%% [%-*C@3 IWALL]-|Write access for all.&]
[s7;i1440;~~~.1728;%% [%-*C@3 IXALL]-|Executable by all.&]
[s7;i1440;~~~.1728;%% [%-*C@3 RWXA]-|Unrestricted access for all.&]
[s3;%% &]
[s4; &]
[s5;:Upp`:`:SFtp`:`:OpenRead`(const Upp`:`:String`&`): [_^Upp`:`:SFtpHandle^ SFtpHandle
][@(0.0.255) `*]_[* OpenRead]([@(0.0.255) const]_[_^Upp`:`:String^ String][@(0.0.255) `&]_[*@3 p
ath])&]
[s2;%% Opens remote [%-*@3 path] for reading. Returns a file handle 
on success, and NULL on failure. File handles obtained using 
this method must be released using the Close() method when finished.&]
[s3;%% &]
[s4; &]
[s5;:Upp`:`:SFtp`:`:OpenWrite`(const Upp`:`:String`&`): [_^Upp`:`:SFtpHandle^ SFtpHandl
e][@(0.0.255) `*]_[* OpenWrite]([@(0.0.255) const]_[_^Upp`:`:String^ String][@(0.0.255) `&]_
[*@3 path])&]
[s2;%% Opens remote [%-*@3 path] for writing. File will be created 
if it doesn`'t exist. Returns a file handle on success, and NULL 
on failure. File handles obtained using this method must be released 
using the Close() method when finished.&]
[s3;%% &]
[s4; &]
[s5;:Upp`:`:SFtp`:`:Close`(Upp`:`:SFtpHandle`*`): [@(0.0.255) bool]_[* Close]([_^Upp`:`:SFtpHandle^ S
FtpHandle][@(0.0.255) `*]_[*@3 handle])&]
[s0;l288;%% Closes the file [%-*@3 handle]. Return true on success. 
File handles obtained with Open() or OpenDir() methods must be 
released using this method when finished.&]
[s3;%% &]
[s4; &]
[s5;:Upp`:`:SFtp`:`:Rename`(const Upp`:`:String`&`,const Upp`:`:String`&`): [@(0.0.255) b
ool]_[* Rename]([@(0.0.255) const]_[_^Upp`:`:String^ String][@(0.0.255) `&]_[*@3 oldpath], 
[@(0.0.255) const]_[_^Upp`:`:String^ String][@(0.0.255) `&]_[*@3 newpath])&]
[s2;%% Renames a file or directory from [%-*@3 oldpath] to [%-*@3 newpath]. 
Returns true on success. &]
[s3;%% &]
[s4; &]
[s5;:Upp`:`:SFtp`:`:Delete`(const Upp`:`:String`&`): [@(0.0.255) bool]_[* Delete]([@(0.0.255) c
onst]_[_^Upp`:`:String^ String][@(0.0.255) `&]_[*@3 path])&]
[s2;%% Deletes the file at [%-*@3 path]. Returns true on success.&]
[s3;%% &]
[s4; &]
[s5;:Upp`:`:SFtp`:`:Sync`(Upp`:`:SFtpHandle`*`): [@(0.0.255) bool]_[* Sync]([_^Upp`:`:SFtpHandle^ S
FtpHandle][@(0.0.255) `*]_[*@3 handle])&]
[s2;%% Synchronizes the file pointed by [%-*@3 handle] to the remote 
storage device. Returns true on success.&]
[s3;%% &]
[s4; &]
[s5;:Upp`:`:SFtp`:`:Seek`(Upp`:`:SFtpHandle`*`,Upp`:`:int64`): [_^Upp`:`:SFtp^ SFtp][@(0.0.255) `&
]_[* Seek]([_^Upp`:`:SFtpHandle^ SFtpHandle][@(0.0.255) `*]_[*@3 handle], 
[_^Upp`:`:int64^ int64]_[*@3 position])&]
[s2;%% Seeks to [%-*@3 position] in a remote file pointed by [%-*@3 handle]. 
Returns `*this for methods chaining.&]
[s3;%% &]
[s4; &]
[s5;:Upp`:`:SFtp`:`:GetPos`(Upp`:`:SFtpHandle`*`): [_^Upp`:`:int64^ int64]_[* GetPos]([_^Upp`:`:SFtpHandle^ S
FtpHandle][@(0.0.255) `*]_[*@3 handle])&]
[s2;%% Returns the current position (in bytes) from the beginning 
of a remote file pointed by [%-*@3 handle].&]
[s3;%% &]
[s4; &]
[s5;:Upp`:`:SFtp`:`:OpenDir`(const Upp`:`:String`&`): [_^Upp`:`:SFtpHandle^ SFtpHandle][@(0.0.255) `*
]_[* OpenDir]([@(0.0.255) const]_[_^Upp`:`:String^ String][@(0.0.255) `&]_[*@3 path])&]
[s0;l288;%% Opens the directory at [%-*@3 path]. Returns a file handle 
on success, and NULL on failure. File handles obtained using 
this method must be released using Close() method when finished.&]
[s3;%% &]
[s4; &]
[s5;:Upp`:`:SFtp`:`:MakeDir`(const Upp`:`:String`&`,long`): [@(0.0.255) bool]_[* MakeDir](
[@(0.0.255) const]_[_^Upp`:`:String^ String][@(0.0.255) `&]_[*@3 path], 
[@(0.0.255) long]_[*@3 mode])&]
[s2;%% Creates a directory at [%-*@3 path ]with access [%-*@3 mode]. 
Returns true on success.&]
[s3;%% &]
[s4; &]
[s5;:Upp`:`:SFtp`:`:RemoveDir`(const Upp`:`:String`&`): [@(0.0.255) bool]_[* RemoveDir]([@(0.0.255) c
onst]_[_^Upp`:`:String^ String][@(0.0.255) `&]_[*@3 path])&]
[s2;%% Deletes a remote directory at [%-*@3 path]. Return true on success.&]
[s3;%% &]
[s4; &]
[s5;:Upp`:`:SFtp`:`:ListDir`(Upp`:`:SFtpHandle`*`,Upp`:`:SFtp`:`:DirList`&`): [@(0.0.255) b
ool]_[* ListDir]([_^Upp`:`:SFtpHandle^ SFtpHandle][@(0.0.255) `*]_[*@3 handle], 
[_^Upp`:`:SFtp`:`:DirList^ DirList][@(0.0.255) `&]_[*@3 list])&]
[s2;%% [%-*@3 list]s the content of remote directory pointed by file 
[%-*@3 handle]. Returns true on success. This method will return 
true even when the remote directory is empty. &]
[s3;%% &]
[s4; &]
[s5;:Upp`:`:SFtp`:`:ListDir`(const Upp`:`:String`&`,Upp`:`:SFtp`:`:DirList`&`): [@(0.0.255) b
ool]_[* ListDir]([@(0.0.255) const]_[_^Upp`:`:String^ String][@(0.0.255) `&]_[*@3 path], 
[_^Upp`:`:SFtp`:`:DirList^ DirList][@(0.0.255) `&]_[*@3 list])&]
[s2;%% Overload. [%-*@3 list]s the content of remote [%-*@3 path]. Returns 
true on success. This method will return true even when the remote 
directory is empty.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:SFtp`:`:GetWorkDir`(`): [_^Upp`:`:String^ String]_[* GetWorkDir]()&]
[s2;%% Returns the working directory on success.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:SFtp`:`:MakeLink`(const Upp`:`:String`&`,const Upp`:`:String`&`): [@(0.0.255) b
ool]_[* MakeLink]([@(0.0.255) const]_[_^Upp`:`:String^ String][@(0.0.255) `&]_[*@3 orig], 
[@(0.0.255) const]_[_^Upp`:`:String^ String][@(0.0.255) `&]_[*@3 link])&]
[s2;%% Creates a symbolic [%-*@3 link] to the [%-*@3 orig]inating file 
system object. Returns true on success.&]
[s3;%% &]
[s4; &]
[s5;:Upp`:`:SFtp`:`:ReadLink`(const Upp`:`:String`&`,Upp`:`:String`&`): [@(0.0.255) boo
l]_[* ReadLink]([@(0.0.255) const]_[_^Upp`:`:String^ String][@(0.0.255) `&]_[*@3 path], 
[_^Upp`:`:String^ String][@(0.0.255) `&]_[*@3 target])&]
[s2;%% Resoves a symbolic link at [%-*@3 path] to its next [%-*@3 target]. 
Returns true on success.&]
[s3;%% &]
[s4; &]
[s5;:Upp`:`:SFtp`:`:RealizePath`(const Upp`:`:String`&`,Upp`:`:String`&`): [@(0.0.255) b
ool]_[* RealizePath]([@(0.0.255) const]_[_^Upp`:`:String^ String][@(0.0.255) `&]_[*@3 path],
 [_^Upp`:`:String^ String][@(0.0.255) `&]_[*@3 target])&]
[s2;%% Resolves a complex, relative or symlinked [%-*@3 path] to its 
effective [%-*@3 target]. Returns true on success.&]
[s3;%% &]
[s4; &]
[s5;:Upp`:`:SFtp`:`:GetAttrs`(Upp`:`:SFtpHandle`*`,Upp`:`:SFtpAttrs`&`): [@(0.0.255) bo
ol]_[* GetAttrs]([_^Upp`:`:SFtpHandle^ SFtpHandle][@(0.0.255) `*]_[*@3 handle], 
[_^Upp`:`:SFtpAttrs^ SFtpAttrs][@(0.0.255) `&]_[*@3 attrs])&]
[s2;%% Gets the attributes of file system object pointed by [%-*@3 handle 
]to [%-*@3 attrs]. Returns true on success.&]
[s3;%% &]
[s4; &]
[s5;:Upp`:`:SFtp`:`:GetAttrs`(const Upp`:`:String`&`,Upp`:`:SFtpAttrs`&`): [@(0.0.255) b
ool]_[* GetAttrs]([@(0.0.255) const]_[_^Upp`:`:String^ String][@(0.0.255) `&]_[*@3 path], 
[_^Upp`:`:SFtpAttrs^ SFtpAttrs][@(0.0.255) `&]_[*@3 attrs])&]
[s2;%% Gets the attributes of file system object at [%-*@3 path ]to 
[%-*@3 attrs]. Returns true on success.&]
[s3;%% &]
[s4; &]
[s5;:Upp`:`:SFtp`:`:SetAttrs`(Upp`:`:SFtpHandle`*`,const Upp`:`:SFtpAttrs`&`): [@(0.0.255) b
ool]_[* SetAttrs]([_^Upp`:`:SFtpHandle^ SFtpHandle][@(0.0.255) `*]_[*@3 handle], 
[@(0.0.255) const]_[_^Upp`:`:SFtpAttrs^ SFtpAttrs][@(0.0.255) `&]_[*@3 attrs])&]
[s2;%% Sets the attributes of the file system object pointed by [%-*@3 handle].to 
[%-*@3 attrs]. Returns true on success.&]
[s3;%% &]
[s4; &]
[s5;:Upp`:`:SFtp`:`:SetAttrs`(const Upp`:`:String`&`,const Upp`:`:SFtpAttrs`&`): [@(0.0.255) b
ool]_[* SetAttrs]([@(0.0.255) const]_[_^Upp`:`:String^ String][@(0.0.255) `&]_[*@3 path], 
[@(0.0.255) const]_[_^Upp`:`:SFtpAttrs^ SFtpAttrs][@(0.0.255) `&]_[*@3 attrs])&]
[s2;%% Sets the attributes of the file system object at [%-*@3 path] 
to [%-*@3 attrs]. Returns true on success.&]
[s3;%% &]
[s4; &]
[s5;:Upp`:`:SFtp`:`:GetInfo`(const Upp`:`:String`&`): [_^Upp`:`:SFtp`:`:DirEntry^ SFtp`:
:DirEntry]_[* GetInfo]([@(0.0.255) const]_[_^Upp`:`:String^ String][@(0.0.255) `&]_[*@3 path
])&]
[s2;%% Returns a DirEntry object that contains information about 
[%-*@3 path] on success, and Null on failure.&]
[s2;%% In non`-blocking mode, the result of this operation can be 
obtained using GetResult() method.&]
[s3;%% &]
[s4; &]
[s5;:Upp`:`:SFtp`:`:SetInfo`(const Upp`:`:SFtp`:`:DirEntry`&`): [@(0.0.255) bool]_[* SetI
nfo]([@(0.0.255) const]_[_^Upp`:`:SFtp`:`:DirEntry^ DirEntry][@(0.0.255) `&]_[*@3 entry])
&]
[s2;%% Sets the attributes of file system object represented by [%-*@3 entry]. 
Returns true on succes, and Null on failure.&]
[s3;%% &]
[s4; &]
[s5;:Upp`:`:SFtp`:`:GetSize`(const Upp`:`:String`&`): [_^Upp`:`:int64^ int64]_[* GetSize](
[@(0.0.255) const]_[_^Upp`:`:String^ String][@(0.0.255) `&]_[*@3 path])&]
[s2;%% Returns the size of [%-*@3 path] on success, `-1 on failure.&]
[s2;%% In non`-blocking mode, the result of this operation can be 
obtained using GetResult() method.&]
[s3;%% &]
[s4; &]
[s5;:Upp`:`:SFtp`:`:SetSize`(const Upp`:`:String`&`,Upp`:`:int64`): [@(0.0.255) bool]_[* S
etSize]([@(0.0.255) const]_[_^Upp`:`:String^ String][@(0.0.255) `&]_[*@3 path], 
[_^Upp`:`:int64^ int64]_[*@3 size])&]
[s2;%% Sets the [%-*@3 size ]of [%-*@3 path]. Returns true on success.&]
[s3;%% &]
[s4; &]
[s5;:Upp`:`:SFtp`:`:GetLastModifyTime`(const Upp`:`:String`&`): [_^Upp`:`:Time^ Time]_[* G
etLastModifyTime]([@(0.0.255) const]_[_^Upp`:`:String^ String][@(0.0.255) `&]_[*@3 path])
&]
[s2;%% Returns the last modification time[%-*@3  ]of [%-*@3 path] on 
success, and Null on failure.&]
[s2;%% In non`-blocking mode, the result of this operation can be 
obtained using GetResult() method.&]
[s3;%% &]
[s4; &]
[s5;:Upp`:`:SFtp`:`:SetLastModifyTime`(const Upp`:`:String`&`,const Upp`:`:Time`&`): [@(0.0.255) b
ool]_[* SetLastModifyTime]([@(0.0.255) const]_[_^Upp`:`:String^ String][@(0.0.255) `&]_[*@3 p
ath], [@(0.0.255) const]_[_^Upp`:`:Time^ Time][@(0.0.255) `&]_[*@3 time])&]
[s2;%% Sets the last modification [%-*@3 time ]of [%-*@3 path]. Returns 
true on success.&]
[s3;%% &]
[s4; &]
[s5;:Upp`:`:SFtp`:`:GetLastAccessTime`(const Upp`:`:String`&`): [_^Upp`:`:Time^ Time]_[* G
etLastAccessTime]([@(0.0.255) const]_[_^Upp`:`:String^ String][@(0.0.255) `&]_[*@3 path])
&]
[s2;%% Returns the last access time of [%-*@3 path] on success, and 
Null on failure.&]
[s2;%% In non`-blocking mode, the result of this operation can be 
obtained using GetResult() method.&]
[s3;%% &]
[s4; &]
[s5;:Upp`:`:SFtp`:`:SetLastAccessTime`(const Upp`:`:String`&`,const Upp`:`:Time`&`): [@(0.0.255) b
ool]_[* SetLastAccessTime]([@(0.0.255) const]_[_^Upp`:`:String^ String][@(0.0.255) `&]_[*@3 p
ath], [@(0.0.255) const]_[_^Upp`:`:Time^ Time][@(0.0.255) `&]_[*@3 time])&]
[s2;%% Sets the last access time[%-*@3  ]of [%-*@3 path]. Returns true 
on success.&]
[s3;%% &]
[s4; &]
[s5;:Upp`:`:SFtp`:`:FileExists`(const Upp`:`:String`&`): [@(0.0.255) bool]_[* FileExists](
[@(0.0.255) const]_[_^Upp`:`:String^ String][@(0.0.255) `&]_[*@3 path])&]
[s2;%% Returns true if [%-*@3 path] represents a file.&]
[s2;%% In non`-blocking mode, the result of this operation can be 
obtained using GetResult() method.&]
[s3;%% &]
[s4; &]
[s5;:Upp`:`:SFtp`:`:DirectoryExists`(const Upp`:`:String`&`): [@(0.0.255) bool]_[* Direct
oryExists]([@(0.0.255) const]_[_^Upp`:`:String^ String][@(0.0.255) `&]_[*@3 path])&]
[s2;%% Returns true if [%-*@3 path] represents a directory.&]
[s2;%% In non`-blocking mode, the result of this operation can be 
obtained using GetResult() method.&]
[s3;%% &]
[s4; &]
[s5;:Upp`:`:SFtp`:`:SymLinkExists`(const Upp`:`:String`&`): [@(0.0.255) bool]_[* SymLinkE
xists]([@(0.0.255) const]_[_^Upp`:`:String^ String][@(0.0.255) `&]_[*@3 path])&]
[s2;%% Returns true if [%-*@3 path] represents a symbolic link.&]
[s2;%% In non`-blocking mode, the result of this operation can be 
obtained using GetResult() method.&]
[s3;%% &]
[s4; &]
[s5;:Upp`:`:SFtp`:`:SocketExists`(const Upp`:`:String`&`): [@(0.0.255) bool]_[* SocketExi
sts]([@(0.0.255) const]_[_^Upp`:`:String^ String][@(0.0.255) `&]_[*@3 path])&]
[s2;%% Returns true if [%-*@3 path] represents a socket.&]
[s2;%% In non`-blocking mode, the result of this operation can be 
obtained using GetResult() method. &]
[s3;%% &]
[s4; &]
[s5;:Upp`:`:SFtp`:`:PipeExists`(const Upp`:`:String`&`): [@(0.0.255) bool]_[* PipeExists](
[@(0.0.255) const]_[_^Upp`:`:String^ String][@(0.0.255) `&]_[*@3 path])&]
[s2;%% Returns true if [%-*@3 path] represents a named pipe.&]
[s2;%% In non`-blocking mode, the result of this operation can be 
obtained using GetResult() method.&]
[s3;%% &]
[s4; &]
[s5;:Upp`:`:SFtp`:`:BlockExists`(const Upp`:`:String`&`): [@(0.0.255) bool]_[* BlockExist
s]([@(0.0.255) const]_[_^Upp`:`:String^ String][@(0.0.255) `&]_[*@3 path])&]
[s2;%% Returns true if [%-*@3 path] represents a block special file.&]
[s2;%% In non`-blocking mode, the result of this operation can be 
obtained using GetResult() method.&]
[s3;%% &]
[s4; &]
[s5;:Upp`:`:SFtp`:`:SpecialFileExists`(const Upp`:`:String`&`): [@(0.0.255) bool]_[* Spec
ialFileExists]([@(0.0.255) const]_[_^Upp`:`:String^ String][@(0.0.255) `&]_[*@3 path])&]
[s2;%% Returns true if [%-*@3 path] represents a character special 
file.&]
[s2;%% In non`-blocking mode, the result of this operation can be 
obtained using GetResult() method.&]
[s3;%% &]
[s4; &]
[s5;:Upp`:`:SFtp`:`:WhenContent: [_^Upp`:`:Event^ Event]<[@(0.0.255) const]_[@(0.0.255) voi
d`*], [@(0.0.255) int]>_[* WhenContent]&]
[s2;%% Defines a consumer function for SFtp data content. If defined, 
SFtp class uses this output event instead of storing the output 
content into the string or stream that is provided with the data 
transfer methods.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:SFtp`:`:WhenProgress: [_^Upp`:`:Gate^ Gate]<[_^Upp`:`:int64^ int64], 
[_^Upp`:`:int64^ int64]>_[* WhenProgress]&]
[s2;%% If defined, this gate allows tracking of data transfers. The 
first parameter provides the amount of data that has already 
been transferred. The second parameter may provide the total 
amount of data to be transferred, but is allowed to be 0. Returning 
true will abort the current data transfer.&]
[s3; &]
[ {{10000F(128)G(128)@1 [s0;%% [* Transfer Methods]]}}&]
[s3;  &]
[s5;:Upp`:`:SFtp`:`:Get`(Upp`:`:SFtpHandle`*`,Upp`:`:Stream`&`): [@(0.0.255) bool]_[* Get
]([_^Upp`:`:SFtpHandle^ SFtpHandle][@(0.0.255) `*]_[*@3 handle], [_^Upp`:`:Stream^ Stream
][@(0.0.255) `&]_[*@3 out])&]
[s5;:Upp`:`:SFtp`:`:Get`(const Upp`:`:String`&`,Upp`:`:Stream`&`): [@(0.0.255) bool]_[* G
et]([@(0.0.255) const]_[_^Upp`:`:String^ String][@(0.0.255) `&]_[*@3 path], 
[_^Upp`:`:Stream^ Stream][@(0.0.255) `&]_[*@3 out])&]
[s5;:Upp`:`:SFtp`:`:Get`(const Upp`:`:String`&`,Upp`:`:Stream`&`,Upp`:`:int64`): [@(0.0.255) b
ool]_[* Get]([@(0.0.255) const]_[_^Upp`:`:String^ String][@(0.0.255) `&]_[*@3 path], 
[_^Upp`:`:Stream^ Stream][@(0.0.255) `&]_[*@3 out], [_^Upp`:`:int64^ int64]_[*@3 offset])&]
[s2;%% Downloads the content of remote file pointed by  [%-*@3 handle] 
or [%-*@3 path] to [%-*@3 out]. Returns true on success. [%-*@3 offset] 
can be used to start or restart a partial download. Setting [%-*@3 offset] 
to zero or a negative value will result in full download.&]
[s3;%% &]
[s4; &]
[s5;:Upp`:`:SFtp`:`:Get`(Upp`:`:SFtpHandle`*`): [_^Upp`:`:String^ String]_[* Get]([_^Upp`:`:SFtpHandle^ S
FtpHandle][@(0.0.255) `*]_[*@3 handle])&]
[s5;:Upp`:`:SFtp`:`:Get`(const Upp`:`:String`&`): [_^Upp`:`:String^ String]_[* Get]([@(0.0.255) c
onst]_[_^Upp`:`:String^ String][@(0.0.255) `&]_[*@3 path])&]
[s2;%% Downloads the content of remote file pointed by [%-*@3 handle] 
or [%-*@3 path ]as string. Returns an empty string on failure.&]
[s2;%% In non`-blocking mode, the result of this operation can be 
obtained using GetResult() method.&]
[s3;%% &]
[s4; &]
[s5;:Upp`:`:SFtp`:`:Put`(Upp`:`:SFtpHandle`*`,Upp`:`:Stream`&`): [@(0.0.255) bool]_[* Put
]([_^Upp`:`:SFtpHandle^ SFtpHandle][@(0.0.255) `*]_[*@3 handle], [_^Upp`:`:Stream^ Stream
][@(0.0.255) `&]_[*@3 in])&]
[s5;:Upp`:`:SFtp`:`:Put`(Upp`:`:Stream`&`,const Upp`:`:String`&`): [@(0.0.255) bool]_[* P
ut]([_^Upp`:`:Stream^ Stream][@(0.0.255) `&]_[*@3 in], [@(0.0.255) const]_[_^Upp`:`:String^ S
tring][@(0.0.255) `&]_[*@3 path])&]
[s5;:Upp`:`:SFtp`:`:Put`(Upp`:`:Stream`&`,const Upp`:`:String`&`,Upp`:`:dword`,long`): [@(0.0.255) b
ool]_[* Put]([_^Upp`:`:Stream^ Stream][@(0.0.255) `&]_[*@3 in], [@(0.0.255) const]_[_^Upp`:`:String^ S
tring][@(0.0.255) `&]_[*@3 path], [_^Upp`:`:dword^ dword]_[*@3 flags], 
[@(0.0.255) long]_[*@3 mode])&]
[s5;:Upp`:`:SFtp`:`:Put`(Upp`:`:Stream`&`,const Upp`:`:String`&`,Upp`:`:int64`): [@(0.0.255) b
ool]_[* Put]([_^Upp`:`:Stream^ Stream][@(0.0.255) `&]_[*@3 in], [@(0.0.255) const]_[_^Upp`:`:String^ S
tring][@(0.0.255) `&]_[*@3 path], [_^Upp`:`:int64^ int64]_[*@3 offset])&]
[s2;%% Uploads [%-*@3 in] to remote file pointed by [%-*@3 handle]. or 
[%-*@3 path], optionally with access [%-*@3 flags] and [%-*@3 mode]. 
Returns true on success. [%-*@3 offset ]can be used to start or 
restart a partial upload. Setting [%-*@3 offset] to zero or a negative 
value will result in full upload. Returns true on success.&]
[s3;%% &]
[s4; &]
[s5;:Upp`:`:SFtp`:`:Append`(Upp`:`:Stream`&`,const Upp`:`:String`&`): [@(0.0.255) bool]_
[* Append]([_^Upp`:`:Stream^ Stream][@(0.0.255) `&]_[*@3 in], [@(0.0.255) const]_[_^Upp`:`:String^ S
tring][@(0.0.255) `&]_[*@3 path])&]
[s5;:Upp`:`:SFtp`:`:Append`(Upp`:`:Stream`&`,const Upp`:`:String`&`,long`): [@(0.0.255) b
ool]_[* Append]([_^Upp`:`:Stream^ Stream][@(0.0.255) `&]_[*@3 in], [@(0.0.255) const]_[_^Upp`:`:String^ S
tring][@(0.0.255) `&]_[*@3 path], [@(0.0.255) long]_[*@3 mode])&]
[s2;%% Appends [%-*@3 in] to remote file at [%-*@3 path], optionally 
with access [%-*@3 mode]. Returns true on success.&]
[s3;%% &]
[s4; &]
[s5;:Upp`:`:SFtp`:`:Peek`(const Upp`:`:String`&`,Upp`:`:int64`,Upp`:`:int64`): [_^Upp`:`:String^ S
tring]_[* Peek]([@(0.0.255) const]_[_^Upp`:`:String^ String][@(0.0.255) `&]_[*@3 path], 
[_^Upp`:`:int64^ int64]_[*@3 offset], [_^Upp`:`:int64^ int64]_[*@3 length])&]
[s2;%% Peeks at the content of remote file at [%-*@3 path]. The file 
block to be peeked at can be specified using the [%-*@3 offset] 
and [%-*@3 length] variables. Returns peeked data block as string 
on success, and an empty string on failure.&]
[s2;%% In non`-blocking mode, the result of this operation can be 
obtained using GetResult() method..&]
[s3;%% &]
[s4; &]
[s5;:Upp`:`:SFtp`:`:Poke`(const Upp`:`:String`&`,const Upp`:`:String`&`,Upp`:`:int64`,Upp`:`:int64`): [@(0.0.255) b
ool]_[* Poke]([@(0.0.255) const]_[_^Upp`:`:String^ String][@(0.0.255) `&]_[*@3 data], 
[@(0.0.255) const]_[_^Upp`:`:String^ String][@(0.0.255) `&]_[*@3 path], 
[_^Upp`:`:int64^ int64]_[*@3 offset], [_^Upp`:`:int64^ int64]_[*@3 length])&]
[s2;%% Pokes [%-*@3 data ]at the remote file at [%-*@3 path]. The file 
block to be poked at can be specified using the [%-*@3 offset] 
and [%-*@3 length] variables. Note that this method simply overwrites 
a given section of a remote file. It does not change the size 
of it.&]
[s3;%% &]
[ {{10000F(128)G(128)@1 [s0;%% [* Multithreaded Transfer Methods.]]}}&]
[s2;# &]
[s2;#%% [%- The following convenience methods use high performance 
worker threads to transfer data. They all require a valid ssh 
][%-*@3 session][%-  during the operation, and throw ][%-^topic`:`/`/SSH`/src`/Upp`_Ssh`_Base`_en`-us`#Upp`:`:Ssh`:`:Error`:`:struct^ S
Sh`::Error][%-  on failures. In all of these methods, except AsyncConsumerGet(), 
][%-*@3 progress] gate can be used to track the progress of the 
transfer: The first parameter of this gate indicates a unique 
id. The second parameter provides the amount of data that has 
already been transferred. The third parameter may provide the 
total amount of data to be transferred, but is allowed to be 
0. Returning true will abort the current data transfer. Note 
that these worker threads will use their session`'s waitstep 
value to wait.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:SFtp`:`:AsyncGet`(Upp`:`:SshSession`&`,const Upp`:`:String`&`,Upp`:`:Gate`<Upp`:`:int64`,Upp`:`:int64`,Upp`:`:int64`>`): [@(0.0.255) s
tatic] [_^Upp`:`:AsyncWork^ AsyncWork]<[_^Upp`:`:String^ String]>_[* AsyncGet]([_^Upp`:`:SshSession^ S
shSession][@(0.0.255) `&]_[*@3 session], [@(0.0.255) const]_[_^Upp`:`:String^ String][@(0.0.255) `&
]_[*@3 path], [_^Upp`:`:Gate^ Gate]<[_^Upp`:`:int64^ int64], [_^Upp`:`:int64^ int64], 
[_^Upp`:`:int64^ int64]>_[*@3 progress]_`=_Null)&]
[s2;%% Downloads the remote file pointed by the [%-*@3 path] as string.&]
[s3;%% &]
[s4; &]
[s5;:Upp`:`:SFtp`:`:AsyncGet`(Upp`:`:SshSession`&`,const Upp`:`:String`&`,Upp`:`:Stream`&`,Upp`:`:Gate`<Upp`:`:int64`,Upp`:`:int64`,Upp`:`:int64`>`): [@(0.0.255) s
tatic] [_^Upp`:`:AsyncWork^ AsyncWork]<[@(0.0.255) void]>_[* AsyncGet]([_^Upp`:`:SshSession^ S
shSession][@(0.0.255) `&]_[*@3 session], [@(0.0.255) const]_[_^Upp`:`:String^ String][@(0.0.255) `&
]_[*@3 path], [_^Upp`:`:Stream^ Stream][@(0.0.255) `&]_[*@3 out], [_^Upp`:`:Gate^ Gate]<[_^Upp`:`:int64^ i
nt64], [_^Upp`:`:int64^ int64], [_^Upp`:`:int64^ int64]>_[*@3 progress]_`=_Null)&]
[s2;%% Downloads the remote file pointed by the [%-*@3 path] to [%-*@3 out].&]
[s3;%% &]
[s4; &]
[s5;:Upp`:`:SFtp`:`:AsyncPut`(Upp`:`:SshSession`&`,Upp`:`:String`&`,const Upp`:`:String`&`,Upp`:`:Gate`<Upp`:`:int64`,Upp`:`:int64`,Upp`:`:int64`>`): [@(0.0.255) s
tatic] [_^Upp`:`:AsyncWork^ AsyncWork]<[@(0.0.255) void]>_[* AsyncPut]([_^Upp`:`:SshSession^ S
shSession][@(0.0.255) `&]_[*@3 session], [_^Upp`:`:String^ String][@(0.0.255) `&]_[*@3 in],
 [@(0.0.255) const]_[_^Upp`:`:String^ String][@(0.0.255) `&]_[*@3 path], 
[_^Upp`:`:Gate^ Gate]<[_^Upp`:`:int64^ int64], [_^Upp`:`:int64^ int64], 
[_^Upp`:`:int64^ int64]>_[*@3 progress]_`=_Null)&]
[s5;:Upp`:`:SFtp`:`:AsyncPut`(Upp`:`:SshSession`&`,Upp`:`:Stream`&`,const Upp`:`:String`&`,Upp`:`:Gate`<Upp`:`:int64`,Upp`:`:int64`,Upp`:`:int64`>`): [@(0.0.255) s
tatic] [_^Upp`:`:AsyncWork^ AsyncWork]<[@(0.0.255) void]>_[* AsyncPut]([_^Upp`:`:SshSession^ S
shSession][@(0.0.255) `&]_[*@3 session], [_^Upp`:`:Stream^ Stream][@(0.0.255) `&]_[*@3 in],
 [@(0.0.255) const]_[_^Upp`:`:String^ String][@(0.0.255) `&]_[*@3 path], 
[_^Upp`:`:Gate^ Gate]<[_^Upp`:`:int64^ int64], [_^Upp`:`:int64^ int64], 
[_^Upp`:`:int64^ int64]>_[*@3 progress]_`=_Null)&]
[s2;%% Uploads [%-*@3 in] to the [%-*@3 path].&]
[s3;%% &]
[s4; &]
[s5;:Upp`:`:SFtp`:`:AsyncAppend`(Upp`:`:SshSession`&`,Upp`:`:String`&`,const Upp`:`:String`&`,Upp`:`:Gate`<Upp`:`:int64`,Upp`:`:int64`,Upp`:`:int64`>`): [@(0.0.255) s
tatic] [_^Upp`:`:AsyncWork^ AsyncWork]<[@(0.0.255) void]>_[* AsyncAppend]([_^Upp`:`:SshSession^ S
shSession][@(0.0.255) `&]_[*@3 session], [_^Upp`:`:String^ String][@(0.0.255) `&]_[*@3 in],
 [@(0.0.255) const]_[_^Upp`:`:String^ String][@(0.0.255) `&]_[*@3 path], 
[_^Upp`:`:Gate^ Gate]<[_^Upp`:`:int64^ int64], [_^Upp`:`:int64^ int64], 
[_^Upp`:`:int64^ int64]>_[*@3 progress]_`=_Null)&]
[s5;:Upp`:`:SFtp`:`:AsyncAppend`(Upp`:`:SshSession`&`,Upp`:`:Stream`&`,const Upp`:`:String`&`,Upp`:`:Gate`<Upp`:`:int64`,Upp`:`:int64`,Upp`:`:int64`>`): [@(0.0.255) s
tatic] [_^Upp`:`:AsyncWork^ AsyncWork]<[@(0.0.255) void]>_[* AsyncAppend]([_^Upp`:`:SshSession^ S
shSession][@(0.0.255) `&]_[*@3 session], [_^Upp`:`:Stream^ Stream][@(0.0.255) `&]_[*@3 in],
 [@(0.0.255) const]_[_^Upp`:`:String^ String][@(0.0.255) `&]_[*@3 path], 
[_^Upp`:`:Gate^ Gate]<[_^Upp`:`:int64^ int64], [_^Upp`:`:int64^ int64], 
[_^Upp`:`:int64^ int64]>_[*@3 progress]_`=_Null)&]
[s2;%% Appends [%-*@3 in] to the object pointed by the [%-*@3 path].&]
[s3;%% &]
[s4; &]
[s5;:Upp`:`:SFtp`:`:AsyncGetToFile`(Upp`:`:SshSession`&`,const Upp`:`:String`&`,const Upp`:`:String`&`,Upp`:`:Gate`<Upp`:`:int64`,Upp`:`:int64`,Upp`:`:int64`>`): [@(0.0.255) s
tatic] [_^Upp`:`:AsyncWork^ AsyncWork]<[@(0.0.255) void]> [* AsyncGetToFile]([_^Upp`:`:SshSession^ S
shSession][@(0.0.255) `&]_[*@3 session], [@(0.0.255) const]_[_^Upp`:`:String^ String][@(0.0.255) `&
]_[*@3 src], [@(0.0.255) const]_[_^Upp`:`:String^ String][@(0.0.255) `&]_[*@3 dest], 
[_^Upp`:`:Gate^ Gate]<[_^Upp`:`:int64^ int64], [_^Upp`:`:int64^ int64], 
[_^Upp`:`:int64^ int64]>_[*@3 progress]_`=_Null)&]
[s2;%% Downloads the remote file pointed by [%-*@3 src ]to local file 
pointed by [%-*@3 dest]. Note that, in case of failure this method 
[/ does not] automatically delete the partially downloaded file.&]
[s3;%% &]
[s4; &]
[s5;:Upp`:`:SFtp`:`:AsyncPutFromFile`(Upp`:`:SshSession`&`,const Upp`:`:String`&`,const Upp`:`:String`&`,Upp`:`:Gate`<Upp`:`:int64`,Upp`:`:int64`,Upp`:`:int64`>`): [@(0.0.255) s
tatic] [_^Upp`:`:AsyncWork^ AsyncWork]<[@(0.0.255) void]> [* AsyncPutFromFile]([_^Upp`:`:SshSession^ S
shSession][@(0.0.255) `&]_[*@3 session], [@(0.0.255) const]_[_^Upp`:`:String^ String][@(0.0.255) `&
]_[*@3 src], [@(0.0.255) const]_[_^Upp`:`:String^ String][@(0.0.255) `&]_[*@3 dest], 
[_^Upp`:`:Gate^ Gate]<[_^Upp`:`:int64^ int64], [_^Upp`:`:int64^ int64], 
[_^Upp`:`:int64^ int64]>_[*@3 progress]_`=_Null)&]
[s2;%% Uploads the local file pointed by [%-*@3 src] to  the remote 
path pointed by [%-*@3 dest].&]
[s3;%% &]
[s4; &]
[s5;:Upp`:`:SFtp`:`:AsyncAppendFromFile`(Upp`:`:SshSession`&`,const Upp`:`:String`&`,const Upp`:`:String`&`,Upp`:`:Gate`<Upp`:`:int64`,Upp`:`:int64`,Upp`:`:int64`>`): [@(0.0.255) s
tatic ][_^Upp`:`:AsyncWork^ AsyncWork]<[@(0.0.255) void]> [* AsyncAppendFromFile]([_^Upp`:`:SshSession^ S
shSession][@(0.0.255) `&]_[*@3 session], [@(0.0.255) const]_[_^Upp`:`:String^ String][@(0.0.255) `&
]_[*@3 src], [@(0.0.255) const]_[_^Upp`:`:String^ String][@(0.0.255) `&]_[*@3 dest], 
[_^Upp`:`:Gate^ Gate]<[_^Upp`:`:int64^ int64], [_^Upp`:`:int64^ int64], 
[_^Upp`:`:int64^ int64]>_[*@3 progress]_`=_Null)&]
[s2;%% Appends the local file pointed by [%-*@3 src] to the remote 
path pointed by [%-*@3 dest].&]
[s3;%% &]
[s4; &]
[s5;:Upp`:`:SFtp`:`:AsyncConsumerGet`(Upp`:`:SshSession`&`,const Upp`:`:String`&`,Upp`:`:Event`<Upp`:`:int64`,const void`*`,int`>`): [@(0.0.255) s
tatic ][_^Upp`:`:AsyncWork^ AsyncWork]<[@(0.0.255) void]>_[* AsyncConsumerGet]([_^Upp`:`:SshSession^ S
shSession][@(0.0.255) `&]_[*@3 session], [@(0.0.255) const]_[_^Upp`:`:String^ String][@(0.0.255) `&
]_[*@3 path], [_^Upp`:`:Event^ Event]<[_^Upp`:`:int64^ int64], [@(0.0.255) const]_[@(0.0.255) v
oid`*], [@(0.0.255) int]>_[*@3 consumer])&]
[s2;%% Downloads the remote file pointed by [%-*@3 path], using a user`-defined 
[%-*@3 consumer] function. The first parameter of the consumer 
function is the unique id of the given sftp worker.&]
[s3;%% &]
[s3;%% &]
[ {{10000F(128)G(128)@1 [s0;%% [* Constructor detail]]}}&]
[s3; &]
[s5;:Upp`:`:SFtp`:`:SFtp`(Upp`:`:SshSession`&`): [* SFtp]([_^Upp`:`:SshSession^ SshSessio
n][@(0.0.255) `&]_[*@3 session])&]
[s2;%% Constructor. Binds the SFtp instance to [%-*@3 session].&]
[s3;%% &]
[s0; &]
[ {{10000@(113.42.0) [s0;%% [*@7;4 SFtp`::DirEntry]]}}&]
[s3;%% &]
[s1;:Upp`:`:SFtp`:`:DirEntry`:`:class: [@(0.0.255)3 class][3 _][*3 DirEntry][3 _:_][@(0.0.255)3 p
ublic][3 _][*@3;3 Moveable][3 <][*3 DirEntry][3 >_]&]
[s0;l288;%% This nested helper class is intended to simplify the 
parsing of file system objects (files, directories, symbolic 
links, etc.) returned by the SFtp class. DirEntry class has pick 
semantics.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:SFtp`:`:DirEntry`:`:GetName`(`)const: [_^Upp`:`:String^ String]_[* GetName]()
_[@(0.0.255) const]&]
[s2;%% Returns the name (path) of the file system object on success, 
and Null on failure..&]
[s3; &]
[s4; &]
[s5;:Upp`:`:SFtp`:`:DirEntry`:`:GetUid`(`)const: [_^Upp`:`:int64^ int64]_[* GetUid]()_[@(0.0.255) c
onst]&]
[s2;%% Returns the user id associated with the file system object 
on success, and `-1 on failure.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:SFtp`:`:DirEntry`:`:GetGid`(`)const: [_^Upp`:`:int64^ int64]_[* GetGid]()_[@(0.0.255) c
onst]&]
[s2;%% Returns the group id associated with the file system object 
on success, and `-1 on failure.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:SFtp`:`:DirEntry`:`:GetSize`(`)const: [_^Upp`:`:int64^ int64]_[* GetSize]()_[@(0.0.255) c
onst]&]
[s2;%% Returns the size of file system object in bytes on success, 
and `-1 on falure.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:SFtp`:`:DirEntry`:`:GetLastModified`(`)const: [_^Upp`:`:Time^ Time]_[* GetLas
tModified]()_[@(0.0.255) const]&]
[s2;%% Returns the last modification time of the file system object 
on success, and Null on failure.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:SFtp`:`:DirEntry`:`:GetLastAccessed`(`)const: [_^Upp`:`:Time^ Time]_[* GetLas
tAccessed]()_[@(0.0.255) const]&]
[s2;%% Returns the last access time of the file system object on 
success, and Null on failure.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:SFtp`:`:DirEntry`:`:GetAttrs`(`): [_^Upp`:`:SFtpAttrs^ SFtpAttrs][@(0.0.255) `&
]_[* GetAttrs]()&]
[s5;:Upp`:`:SFtp`:`:DirEntry`:`:GetAttrs`(`): [_^Upp`:`:SFtpAttrs^ SFtpAttrs][@(0.0.255) `&
]_[* operator`*]()&]
[s2;%% Returns the libssh2 file attributes structure for the file 
system object.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:SFtp`:`:DirEntry`:`:operator`~`(`)const: [@(0.0.255) const]_[_^Upp`:`:SFtpAttrs^ S
FtpAttrs][@(0.0.255) `&]_[* operator`~]()_[@(0.0.255) const]&]
[s2;%% Allows constant access  to the file attributes structure.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:SFtp`:`:DirEntry`:`:IsFile`(`)const: [@(0.0.255) bool]_[* IsFile]()_[@(0.0.255) c
onst]&]
[s2;%% Returns true if the file system object is a file.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:SFtp`:`:DirEntry`:`:IsDirectory`(`)const: [@(0.0.255) bool]_[* IsDirectory]()
_[@(0.0.255) const]&]
[s2;%% Returns true if the file system object is a directory.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:SFtp`:`:DirEntry`:`:IsSymLink`(`)const: [@(0.0.255) bool]_[* IsSymLink]()_[@(0.0.255) c
onst]&]
[s2;%% Returns true if the file system object is a symbolic link.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:SFtp`:`:DirEntry`:`:IsSpecial`(`)const: [@(0.0.255) bool]_[* IsSpecial]()_[@(0.0.255) c
onst]&]
[s2;%% Returns true if the file system object is a character special 
file.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:SFtp`:`:DirEntry`:`:IsBlock`(`)const: [@(0.0.255) bool]_[* IsBlock]()_[@(0.0.255) c
onst]&]
[s2;%% Returns true if the file system object is a block special 
file.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:SFtp`:`:DirEntry`:`:IsPipe`(`)const: [@(0.0.255) bool]_[* IsPipe]()_[@(0.0.255) c
onst]&]
[s2;%% Returns true if the file system object is a named pipe.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:SFtp`:`:DirEntry`:`:IsSocket`(`)const: [@(0.0.255) bool]_[* IsSocket]()_[@(0.0.255) c
onst]&]
[s2;%% Returns true if the file system object is a socket.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:SFtp`:`:DirEntry`:`:IsReadable`(`)const: [@(0.0.255) bool]_[* IsReadable]()_[@(0.0.255) c
onst]&]
[s2;%% Returns true if the file system object is readable.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:SFtp`:`:DirEntry`:`:IsWriteable`(`)const: [@(0.0.255) bool]_[* IsWriteable]()
_[@(0.0.255) const]&]
[s2;%% Returns true if the file system object is writable.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:SFtp`:`:DirEntry`:`:IsReadOnly`(`)const: [@(0.0.255) bool]_[* IsReadOnly]()_[@(0.0.255) c
onst]&]
[s2;%% Returns true if the file system object has read`-only access.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:SFtp`:`:DirEntry`:`:IsExecutable`(`)const: [@(0.0.255) bool]_[* IsExecutable](
)_[@(0.0.255) const]&]
[s2;%% Returns true if the file system object is executable.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:SFtp`:`:DirEntry`:`:ToString`(`)const: [_^Upp`:`:String^ String]_[* ToString](
)_[@(0.0.255) const]&]
[s2;%% Returns the information of file system object as a formattad 
string. Output of this method is similar to Unix ls command`'s 
output.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:SFtp`:`:DirEntry`:`:ToXml`(`)const: [_^Upp`:`:String^ String]_[* ToXml]()_[@(0.0.255) c
onst]&]
[s2;%% Returns the information of file system object as an xml string. 
Returned string will be a single xml tag, with the name of the 
file system object as its text:&]
[s0; &]
[ {{4133:5867<288;>544;l/26r/26t/14b/14@1-1 [s0;=%% [* sftp:direntry]]
::l/25r/25t/15b/15@2 [s0; ]
::l/26r/26t/14b/14@(178) [s0;=%% Attributes]
:: [s0;=%% Possible Value(s)]
::l/25r/25t/15b/15@2 [s0;=%% [*C type]]
:: [s0;%% [C file, directory, symlink, socket, pipe, block`-special, character`-special, 
other]]
:: [s0;=%% [*C uid]]
:: [s0;%% [C A string representing an int64 value]]
:: [s0;=%% [*C gid]]
:: [s0;%% [C A string representing an int64 value]]
:: [s0;=%% [*C size]]
:: [s0;%% [C A string representing an int64 value]]
:: [s0;=%% [*C modified]]
:: [s0;%% [C Last modification time in YYYY:MM`::DD HH:MM:SS format]]
:: [s0;=%% [*C accessed]]
:: [s0;%% [C Last access time in YYYY:MM`::DD HH:MM:SS format]]
:: [s0;=%% [*C permissions]]
:: [s0;%% [C rwxrwxrwx]]}}&]
[s3; &]
[s3; &]
[ {{10000F(128)G(128)@1 [s0;%% [* Constructor detail]]}}&]
[s3; &]
[s5;:Upp`:`:SFtp`:`:DirEntry`:`:DirEntry`(`): [* DirEntry]()&]
[s2;%% Default constructor.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:SFtp`:`:DirEntry`:`:DirEntry`(const Upp`:`:String`&`): [* DirEntry]([@(0.0.255) c
onst]_[_^Upp`:`:String^ String][@(0.0.255) `&]_[*@3 path])&]
[s2;%% Creates an empty directory entry representing [%-*@3 path].&]
[s3;%% &]
[s4; &]
[s5;:Upp`:`:SFtp`:`:DirEntry`:`:DirEntry`(const Upp`:`:String`&`,const Upp`:`:SFtpAttrs`&`): [* D
irEntry]([@(0.0.255) const]_[_^Upp`:`:String^ String][@(0.0.255) `&]_[*@3 path], 
[@(0.0.255) const]_[_^Upp`:`:SFtpAttrs^ SFtpAttrs][@(0.0.255) `&]_[*@3 attrs])&]
[s2;%% Cretas a directory entry representing [%-*@3 path] with [%-*@3 attrs].&]
[s3;%% &]
[s0; &]
[ {{10000@(113.42.0) [s0;%% [*@7;4 SFtp`::DirList]]}}&]
[s4; &]
[s5;:Upp`:`:SFtp`:`:DirList`:`:typedef: [@(0.0.255) typedef]_[_^Upp`:`:Vector^ Vector]<[_^Upp`:`:SFtp`:`:DirEntry^ D
irEntry]>_[* DirList]&]
[s2;%% SFtp`::DirList is a Vector containing SFtp`::DirEntry elements.&]
[s3; &]
[s3;%% ]]