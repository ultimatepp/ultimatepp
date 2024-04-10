topic "";
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
[ {{10000@(113.42.0) [s0;%% [*@7;4 Title]]}}&]
[s1;*3 &]
[s1;:Upp`:`:FileMapping: [*3 FileMapping]&]
[s2;%% Encapsulates file mapping of file to memory (Linux mmap, Win32 
CreateFileMapping) in mostly platform independent way.&]
[s3; &]
[ {{10000F(128)G(128)@1 [s0;%% [* Public Method List]]}}&]
[s3; &]
[s5;:Upp`:`:FileMapping`:`:FileMapping`(const char`*`): [* FileMapping]([@(0.0.255) const
] [@(0.0.255) char] [@(0.0.255) `*][*@3 file] [@(0.0.255) `=] [@3 0])&]
[s2;%% Constructs the class; if [%-*@3 file] is not NULL, calls Open([%-*@3 file]).&]
[s3; &]
[s4; &]
[s5;:Upp`:`:FileMapping`:`:Open`(const char`*`,Upp`:`:dword`,Upp`:`:int64`): [@(0.0.255) b
ool] [* Open]([@(0.0.255) const] [@(0.0.255) char] [@(0.0.255) `*][*@3 filename], 
dword [*@3 mode][@3  ][@(0.0.255) `=] FileStream[@(0.0.255) `::]READ, 
int64 [*@3 filesize] [@(0.0.255) `=] [@3 0])&]
[s2;%% Opens [%-*@3 filename] for file mapping. [%-*@3 mode][%-@3  ][%-  
is a combination of flags defined in ][%-^topic`:`/`/Core`/src`/Stream`$en`-us`#`:`:BlockStream`:`:class BlockStream^ B
lockStream][%- . If the mode contrains CREATE flag, ][%-*@3 filesize] 
is a size of newly created file. If any file mapping was open 
before Open open, it is closed first.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:FileMapping`:`:Create`(const char`*`,Upp`:`:int64`): [@(0.0.255) bool] 
[* Create]([@(0.0.255) const] [@(0.0.255) char] [@(0.0.255) `*][*@3 file], 
int64 [*@3 filesize])&]
[s2;%% Same as Open([%-*@3 file], FileStream`::CREATE, [%-*@3 filesize]).&]
[s3; &]
[s4; &]
[s5;:Upp`:`:FileMapping`:`:GetFileSize`(`)const: int64 [* GetFileSize]() 
[@(0.0.255) const]&]
[s2;%% Returns the current file size.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:FileMapping`:`:GetTime`(`)const: Time [* GetTime]() [@(0.0.255) const]&]
[s2;%% Returns the last modification time.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:FileMapping`:`:Map`(Upp`:`:int64`,size`_t`): byte [@(0.0.255) `*][* Map](int6
4 [*@3 mapoffset], size`_t [*@3 maplen])&]
[s2;%% Maps a portion file starting at [%-*@3 mapoffset] with size 
[%-*@3 maplen] to memory. No alignment rules are required for [%-*@3 mapoffset] 
and [%-*@3 maplen] (FileMapping maps bigger chunk of file if necessarry).&]
[s3; &]
[s4; &]
[s5;:Upp`:`:FileMapping`:`:Map`(`): byte [@(0.0.255) `*][* Map]()&]
[s2;%% Same as Map(0, GetFileSize()) `- maps the whole file. Probably 
the best mode with 64 bit platforms with relatively unlimited 
virtual space.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:FileMapping`:`:Unmap`(`): [@(0.0.255) bool] [* Unmap]()&]
[s2;%% Ends file mapping (and writes changes to disk).&]
[s3; &]
[s4; &]
[s5;:Upp`:`:FileMapping`:`:Close`(`): [@(0.0.255) bool] [* Close]()&]
[s2;%% Closes the file.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:FileMapping`:`:IsOpen`(`)const: [@(0.0.255) bool] [* IsOpen]() 
[@(0.0.255) const]&]
[s2;%% Returns true if there is open file in FileMapping.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:FileMapping`:`:GetOffset`(`)const: int64 [* GetOffset]() 
[@(0.0.255) const]&]
[s2;%% Returns [%-*@3 mapoffset] from the Map call.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:FileMapping`:`:GetCount`(`)const: size`_t [* GetCount]() 
[@(0.0.255) const]&]
[s2;%% Returns [%-*@3 maplen] from the Map call.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:FileMapping`:`:operator`~`(`)const: [@(0.0.255) const] 
byte [@(0.0.255) `*][* operator][@(0.0.255) `~]() [@(0.0.255) const]&]
[s5;:Upp`:`:FileMapping`:`:begin`(`)const: [@(0.0.255) const] byte 
[@(0.0.255) `*][* begin]() [@(0.0.255) const]&]
[s5;:Upp`:`:FileMapping`:`:operator`~`(`): byte [@(0.0.255) `*][* operator][@(0.0.255) `~](
)&]
[s5;:Upp`:`:FileMapping`:`:begin`(`): byte [@(0.0.255) `*][* begin]()&]
[s2;%% Returns a pointer to the mapped file content or NULL if there 
is no mapping.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:FileMapping`:`:end`(`)const: [@(0.0.255) const] byte [@(0.0.255) `*][* end]() 
[@(0.0.255) const]&]
[s5;:Upp`:`:FileMapping`:`:end`(`): byte [@(0.0.255) `*][* end]()&]
[s2;%% Same as begin() `+ GetCount().&]
[s3; &]
[s4; &]
[s5;:Upp`:`:FileMapping`:`:operator`[`]`(int`)const: [@(0.0.255) const] 
byte[@(0.0.255) `&] [* operator][@(0.0.255) `[`]]([@(0.0.255) int] i) 
[@(0.0.255) const]&]
[s5;:Upp`:`:FileMapping`:`:operator`[`]`(int`): byte[@(0.0.255) `&] 
[* operator][@(0.0.255) `[`]]([@(0.0.255) int] i)&]
[s2;%% Same as begin()`[i`].&]
[s2;%% ]]