topic "";
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
[ {{10000@(113.42.0) [s0;%% [*@7;4 File path utilities]]}}&]
[s9; &]
[ {{10000F(128)G(128)@1 [s0;%% [* Function List]]}}&]
[s3; &]
[s5;:PatternMatch`(const char`*`,const char`*`): [@(0.0.255) bool]_[* PatternMatch]([@(0.0.255) c
onst]_[@(0.0.255) char]_`*[*@3 p], [@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 s])&]
[s2; [%% Matches filename pattern ][*@3 p][%%  with filename ][*@3 s][%% . 
Pattern can use wildcard characters `'][%%@(0.0.255) ?][%% `', matching 
any single character or `'][%%@(0.0.255) `*][%% `' matching any number 
of any characters (these wildcards are consistent with unix/windows). 
If pattern ends with `'][%%@(0.0.255) .][%% `', there must be no 
`'.`' in ][*@3 s] (filename cannot have extension).&]
[s3;%% &]
[s4; &]
[s5;:PatternMatchMulti`(const char`*`,const char`*`): [@(0.0.255) bool]_[* PatternMatchMu
lti]([@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 p], [@(0.0.255) const]_[@(0.0.255) char]_`*
[*@3 s])&]
[s2;%% Matches multiple patterns separated by semicolon (`';`') in 
[%-*@3 p] with filename [%-*@3 s], using [^topic`:`/`/Core`/src`/Path`$en`-us`#PatternMatch`(const char`*`,const char`*`)^ P
atternMatch].&]
[s3;%% &]
[s4; &]
[s5;:GetFileNamePos`(const char`*`): [@(0.0.255) const]_[@(0.0.255) char]_`*[* GetFileNameP
os]([@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 path])&]
[s2;%% Returns the pointer to the first character of filename in 
[%-*@3 path] `- the character after the last path separator (`'/`' 
in POSIX, `'`\`\`' or `'/`' in Win32). If there is no such character, 
returns [%-*@3 path].&]
[s3;%% &]
[s4; &]
[s5;:GetFileExtPos`(const char`*`): [@(0.0.255) const]_[@(0.0.255) char]_`*[* GetFileExtPos
]([@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 path])&]
[s2;%% If filename in [%-*@3 path ]has any extension (in fact, contains 
one or more `'.`' charactera), returns the pointer to the last 
`'.`', otherwise returns pointer to terminating `'`\0`'.&]
[s3;%% &]
[s4; &]
[s5;:HasFileExt`(const char`*`): [@(0.0.255) bool]_[* HasFileExt]([@(0.0.255) const]_[@(0.0.255) c
har]_`*[*@3 path])&]
[s2;%% Returns true if [%-*@3 path] ends with filename with extension 
(in other words, if filename contains `'.`').&]
[s3;%% &]
[s4; &]
[s5;:HasWildcards`(const char`*`): [@(0.0.255) bool]_[* HasWildcards]([@(0.0.255) const]_[@(0.0.255) c
har]_`*[*@3 path])&]
[s2;%% Returns true if [%-*@3 path] contains `'`*`' or `'?`' characters 
(wildcards).&]
[s3;%% &]
[s4; &]
[s5;:IsFullPath`(const char`*`): [@(0.0.255) bool]_[* IsFullPath]([@(0.0.255) const]_[@(0.0.255) c
har]_`*[*@3 path])&]
[s2;%% Returns true if [%-*@3 path] is absolute `- starting with `'/`' 
in POSIX system or drive letter in Win32.&]
[s3;%% &]
[s4; &]
[s5;:GetFileDirectory`(const char`*`): [_^String^ String]_[* GetFileDirectory]([@(0.0.255) c
onst]_[@(0.0.255) char]_`*[*@3 path])&]
[s2;%% Extracts a directory path from [%-*@3 path] `- a substring that 
ends with last path separator (`'/`' in POSIX, `'/`' or `'`\`\`' 
in Win32). Separator [*/ is] included at the end of result. If 
there is none, returns empty string.&]
[s2;%% Example (POSIX): GetFileDirectory([@3 `"/home/user/test.ext`"]) 
returns [@3 `"/home/user/`"].&]
[s0;%% &]
[s3;%% &]
[s4;%% &]
[s5;:GetFileFolder`(const char`*`): [_^String^ String]_[* GetFileFolder]([@(0.0.255) const]_
[@(0.0.255) char]_`*[*@3 path])&]
[s2;%% Extracts a directory path from [%-*@3 path] `- a substring that 
ends with last path separator (`'/`' in POSIX, `'/`' or `'`\`\`' 
in Win32). Separator [*/ is not] included at the end of result. 
If there is none, returns empty string.&]
[s2;%% Example (POSIX): GetFileFolder([@3 `"/home/user/test.ext`"]) 
returns [@3 `"/home/user`"].&]
[s3;%% &]
[s4;%% &]
[s5;:GetFileTitle`(const char`*`): [_^String^ String]_[* GetFileTitle]([@(0.0.255) const]_[@(0.0.255) c
har]_`*[*@3 path])&]
[s2;%% Gets the name of file in [%-*@3 path] without extension (if 
any).&]
[s2;%% Example (POSIX): GetFileTitle([@3 `"/home/user/test.ext`"]) returns 
[@3 `"test`"].&]
[s3;%% &]
[s4;%% &]
[s5;:GetFileExt`(const char`*`): [_^String^ String]_[* GetFileExt]([@(0.0.255) const]_[@(0.0.255) c
har]_`*[*@3 path])&]
[s2;%% Gets file extension, including `'.`' character. If there is 
no extension, returns empty string.&]
[s2;%% Example (POSIX): GetFileExt([@3 `"/home/user/test.ext`"]) returns 
[@3 `".ext`"].&]
[s3;%% &]
[s4;%% &]
[s5;:GetFileName`(const char`*`): [_^String^ String]_[* GetFileName]([@(0.0.255) const]_[@(0.0.255) c
har]_`*[*@3 path])&]
[s2;%% Gets file extension, including `'.`' character. If there is 
no extension, returns empty string.&]
[s2;%% Example (POSIX): GetFileName([@3 `"/home/user/test.ext`"]) returns 
[@3 `".ext`"].&]
[s3;%% &]
[s4; &]
[s5;:AppendFileName`(const String`&`,const char`*`): [_^String^ String]_[* AppendFileName
]([@(0.0.255) const]_[_^String^ String][@(0.0.255) `&]_[*@3 path], [@(0.0.255) const]_[@(0.0.255) c
har]_`*[*@3 filename])&]
[s2;%% Appends a [%-*@3 filename] to directory [%-*@3 path]. The main 
purpose of this operation is to maintain correct path separator. 
[%-*@3 path] is allowed but not required to end with separator.&]
[s3;%% &]
[s4; &]
[s5;:WinPath`(const char`*`): [_^String^ String]_[* WinPath]([@(0.0.255) const]_[@(0.0.255) c
har]_`*[*@3 path])&]
[s2;%% Converts `'`\`\`' in [%-*@3 path] to `'/`'.&]
[s3;%% &]
[s4; &]
[s5;:UnixPath`(const char`*`): [_^String^ String]_[* UnixPath]([@(0.0.255) const]_[@(0.0.255) c
har]_`*[*@3 path])&]
[s2;%% Converts `'/`' in [%-*@3 path] to `'`\`\`'.&]
[s3;%% &]
[s4; &]
[s5;:NativePath`(const char`*`): [_^String^ String]_[* NativePath]([@(0.0.255) const]_[@(0.0.255) c
har]_`*[*@3 path])&]
[s2;%% Same as [^topic`:`/`/Core`/src`/Path`$en`-us`#WinPath`(const char`*`)^ WinPath] 
in Win32 or [^topic`:`/`/Core`/src`/Path`$en`-us`#UnixPath`(const char`*`)^ UnixPat
h] in POSIX.&]
[s3;%% &]
[s4; &]
[s5;:AppendExt`(const char`*`,const char`*`): [_^String^ String]_[* AppendExt]([@(0.0.255) c
onst]_[@(0.0.255) char]_`*[*@3 path], [@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 ext])&]
[s2;%% If [%-*@3 path] does not have extension, appends [%-*@3 ext]. 
Returns result.&]
[s3;%% &]
[s4; &]
[s5;:ForceExt`(const char`*`,const char`*`): [_^String^ String]_[* ForceExt]([@(0.0.255) co
nst]_[@(0.0.255) char]_`*[*@3 path], [@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 ext])&]
[s2;%% Changes file extension of [%-*@3 path] to [%-*@3 ext]. If there 
is no extension, appends it. Returns result.&]
[s3;%% &]
[s4; &]
[s5;:PathIsEqual`(const char`*`,const char`*`): [@(0.0.255) bool]_[* PathIsEqual]([@(0.0.255) c
onst]_[@(0.0.255) char]_`*[*@3 p1], [@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 p2])&]
[s2;%% Returns true if [%-*@3 p1] and [%-*@3 p2] identify the same file 
or directory.&]
[s3;%% &]
[s0; ]