topic "SFtpStream";
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
[ {{10000@(113.42.0) [s0;%% [*@7;4 SFtpStream]]}}&]
[s3; &]
[s1;:Upp`:`:SFtpStream`:`:class: [@(0.0.255)3 class][3 _][*3 SFtpStream][3 _:_][@(0.0.255)3 publ
ic][3 _][*@3;3 BlockStream]&]
[s2;%% This class implements the operations needed to manage streams 
that are able to read and write blocks of data from the [^topic`:`/`/Core`/SSH`/src`/Upp`_Ssh`_SFtp`_en`-us`#Upp`:`:SFtp`:`:class^ S
Ftp] file system objects.at random position. &]
[s3; &]
[ {{10000F(128)G(128)@1 [s0;%% [* Public Method List]]}}&]
[s3; &]
[s5;:Upp`:`:SFtpStream`:`:operator bool`(`)const: [* operator_bool]()_[@(0.0.255) const]&]
[s2;%% Returns true if the stream is open.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:SFtpStream`:`:Open`(Upp`:`:SFtp`&`,const char`*`,Upp`:`:dword`,int`): [@(0.0.255) b
ool]_[* Open]([_^Upp`:`:SFtp^ SFtp][@(0.0.255) `&]_[*@3 sftp], [@(0.0.255) const]_[@(0.0.255) c
har]_`*[*@3 filename], [_^Upp`:`:dword^ dword]_[*@3 mode], [@(0.0.255) int]_[*@3 acm]_`=_[@3 0
644])&]
[s2;%% Opens a remote file system object pointed by [%-*@3 filename] 
in specified [%-*@3 mode] (as defined in [^topic`:`/`/Core`/src`/Stream`_en`-us`#BlockStream`:`:class^ B
lockStream]), and with specific POSIX access rights. Returns 
true on success. Requires a valid [%-*@3 sftp] object. &]
[s3;%% &]
[s4; &]
[s5;:Upp`:`:SFtpStream`:`:GetHandle`(`)const: [_^Upp`:`:SFtpHandle^ SFtpHandle]_[* GetHan
dle]()_[@(0.0.255) const]&]
[s2;%% Returns the associated sftp file handle.&]
[s3; &]
[s3; &]
[ {{10000F(128)G(128)@1 [s0;%% [* Constructor detail]]}}&]
[s3; &]
[s5;:Upp`:`:SFtpStream`:`:SFtpStream`(Upp`:`:SFtp`&`,const char`*`,Upp`:`:dword`,int`): [* S
FtpStream]([_^Upp`:`:SFtp^ SFtp][@(0.0.255) `&]_[*@3 sftp], [@(0.0.255) const]_[@(0.0.255) c
har]_`*[*@3 filename], [_^Upp`:`:dword^ dword]_[*@3 mode], [@(0.0.255) int]_[*@3 acm]_`=_[@3 0
644])&]
[s2;%% Creates a block stream representing a remote file system object 
pointed by [%-*@3 filename] in specified [%-*@3 mode] (as defined 
in [^topic`:`/`/Core`/src`/Stream`_en`-us`#BlockStream`:`:class^ BlockStream]), 
and with specific POSIX access rights. Requires a valid [%-*@3 sftp] 
object. &]
[s3; &]
[s4; &]
[s5;:Upp`:`:SFtpStream`:`:SFtpStream`(`): [* SFtpStream]()&]
[s2;%% Default constructor.&]
[s3; &]
[ {{10000@(113.42.0) [s0;%% [*@7;4 SFtpFileIn]]}}&]
[s3;%% &]
[s1;:Upp`:`:SFtpFileIn`:`:class: [@(0.0.255)3 class][3 _][*3 SFtpFileIn][3 _:_][@(0.0.255)3 publ
ic][3 _][*@3;3 SFtpStream]&]
[s2;%% A simple helper class that represents SFtpStream in read mode.&]
[s3; &]
[ {{10000F(128)G(128)@1 [s0;%% [* Public Method List]]}}&]
[s3; &]
[s5;:Upp`:`:SFtpFileIn`:`:Open`(Upp`:`:SFtp`&`,const char`*`): [@(0.0.255) bool]_[* Open](
[_^Upp`:`:SFtp^ SFtp][@(0.0.255) `&]_[*@3 sftp], [@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 f
ilename])&]
[s2;%% Opens a remote file system object with [%-*@3 filename] for 
reading with specific POSIX access rights. Returns true on success. 
Requires a valid [%-*@3 sftp] object.&]
[s3;%% &]
[ {{10000F(128)G(128)@1 [s0;%% [* Constructor detail]]}}&]
[s3; &]
[s5;:Upp`:`:SFtpFileIn`:`:SFtpFileIn`(Upp`:`:SFtp`&`,const char`*`): [* SFtpFileIn]([_^Upp`:`:SFtp^ S
Ftp][@(0.0.255) `&]_[*@3 sftp], [@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 filename])&]
[s2;%% Opens a remote file system object with [%-*@3 filename] for 
reading with specific POSIX access rights. Requires a valid [%-*@3 sftp] 
object.&]
[s3;%% &]
[s4; &]
[s5;:Upp`:`:SFtpFileIn`:`:SFtpFileIn`(`): [* SFtpFileIn]()&]
[s2;%% Default constructor.&]
[s0; &]
[ {{10000@(113.42.0) [s0;%% [*@7;4 SFtpFileOut]]}}&]
[s3; &]
[s1;:Upp`:`:SFtpFileOut`:`:class: [@(0.0.255)3 class][3 _][*3 SFtpFileOut][3 _:_][@(0.0.255)3 pu
blic][3 _][*@3;3 SFtpStream]&]
[s2;%% A simple helper class that represents SFtpStream in write 
mode.&]
[s3;%% &]
[ {{10000F(128)G(128)@1 [s0;%% [* Public Method List]]}}&]
[s3; &]
[s5;:Upp`:`:SFtpFileOut`:`:Open`(Upp`:`:SFtp`&`,const char`*`,int`): [@(0.0.255) bool]_
[* Open]([_^Upp`:`:SFtp^ SFtp][@(0.0.255) `&]_[*@3 sftp], [@(0.0.255) const]_[@(0.0.255) char
]_`*[*@3 filename], [@(0.0.255) int]_[*@3 acm]_`=_[@3 0644])&]
[s2;%% Opens a remote file system object with [%-*@3 filename] for 
writing with specific POSIX access rights. Returns true on success. 
Requires a valid [%-*@3 sftp] object.&]
[s3;%% &]
[ {{10000F(128)G(128)@1 [s0;%% [* Constructor detail]]}}&]
[s3; &]
[s5;:Upp`:`:SFtpFileOut`:`:SFtpFileOut`(Upp`:`:SFtp`&`,const char`*`): [* SFtpFileOut](
[_^Upp`:`:SFtp^ SFtp][@(0.0.255) `&]_[*@3 sftp], [@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 f
ilename])&]
[s2;%% Opens a remote file system object with [%-*@3 filename] for 
writing with specific POSIX access rights. Requires a valid [%-*@3 sftp] 
object.&]
[s3;%% &]
[s4; &]
[s5;:Upp`:`:SFtpFileOut`:`:SFtpFileOut`(`): [* SFtpFileOut]()&]
[s2;%% Default constructor.&]
[s3; &]
[ {{10000@(113.42.0) [s0;%% [*@7;4 SFtpFileAppend]]}}&]
[s3; &]
[s1;:Upp`:`:SFtpFileAppend`:`:class: [@(0.0.255)3 class][3 _][*3 SFtpFileAppend][3 _:_][@(0.0.255)3 p
ublic][3 _][*@3;3 SFtpStream]&]
[s2;%% A simple helper class that represents SFtpStream in append 
mode.&]
[s0; &]
[ {{10000F(128)G(128)@1 [s0;%% [* Public Method List]]}}&]
[s3; &]
[s5;:Upp`:`:SFtpFileAppend`:`:Open`(Upp`:`:SFtp`&`,const char`*`): [@(0.0.255) bool]_[* O
pen]([_^Upp`:`:SFtp^ SFtp][@(0.0.255) `&]_[*@3 sftp], [@(0.0.255) const]_[@(0.0.255) char]_
`*[*@3 filename])&]
[s2;%% Opens a remote file system object with [%-*@3 filename] for 
appending with specific POSIX access rights. Returns true on 
success. Requires a valid [%-*@3 sftp] object.&]
[s3;%% &]
[ {{10000F(128)G(128)@1 [s0;%% [* Constructor detail]]}}&]
[s3; &]
[s5;:Upp`:`:SFtpFileAppend`:`:SFtpFileAppend`(Upp`:`:SFtp`&`,const char`*`): [* SFtpFil
eAppend]([_^Upp`:`:SFtp^ SFtp][@(0.0.255) `&]_[*@3 sftp], [@(0.0.255) const]_[@(0.0.255) ch
ar]_`*[*@3 filename])&]
[s2;%% Opens a remote file system object with [%-*@3 filename] for 
appending with specific POSIX access rights. Requires a valid 
[%-*@3 sftp] object.&]
[s3;%% &]
[s4; &]
[s5;:Upp`:`:SFtpFileAppend`:`:SFtpFileAppend`(`): [* SFtpFileAppend]()&]
[s2;%% Default constructor.&]
[s0;%% ]]