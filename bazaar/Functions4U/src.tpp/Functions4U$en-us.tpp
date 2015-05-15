topic "Functions4U. Reference";
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
[ {{10000@(113.42.0) [s0; [*@7;4 Functions4U. Reference]]}}&]
[s0;*@3;4 &]
[s0; List of functions in package.&]
[s0; &]
[ {{10000@1 [s0; [* Running files and commands]]}}&]
[s4;%- &]
[s5;:LaunchFile`(const char`*`):%- [@(0.0.255) bool]_[* LaunchFile]([@(0.0.255) const]_[@(0.0.255) c
har]_`*[*@3 file])&]
[s2; Opens the file [%-*@3 file ]with the adecuated program defined 
in the OS by default.&]
[s0; &]
[s0;l288; [/ Example:]&]
[s0;l288; [/ LaunchFile(`"c:`\`\My spreadsheet.txt`"). It will open 
default program (like gedit or notepad) with document `"My spreadsheet.txt`".]&]
[s3; &]
[s0; &]
[ {{10000@1 [s0; [* File Handling]]}}&]
[s4; &]
[s5;:FileCat`(const char`*`,const char`*`):%- [@(0.0.255) bool]_[* FileCat]([@(0.0.255) con
st]_[@(0.0.255) char]_`*[*@3 file], [@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 appendFile])
&]
[s2; Appends at the end of [%-*@3 file] the contents of file [%-*@3 appendFile]. 
[%-*@3 file] will be modified and [%-*@3 appendFile ]will remain 
unchanged.&]
[s0;l288; Returns true in case of success.&]
[s3; &]
[s4;%- &]
[s5;:FileStrAppend`(const char`*`,const char`*`):%- [@(0.0.255) bool]_[* FileStrAppend]([@(0.0.255) c
onst]_[@(0.0.255) char]_`*[*@3 file], [@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 str])&]
[s2; Appends at the end of [%-*@3 file] the text [%-*@3 str].&]
[s0;l288; Returns true in case of success.&]
[s4;%- &]
[s5;:AppendFile`(const char`*`,const char`*`):%- [@(0.0.255) bool]_[* AppendFile]([@(0.0.255) c
onst]_[@(0.0.255) char]_`*[*@3 filename], [@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 str])
&]
[s2; Appends at the end of file [%-*@3 filename] the text [%-*@3 str].&]
[s2; Returns true in case of success.&]
[s3; &]
[s4;%- &]
[s5;:AppendFileName`(const String`&`,const char`*`,const char`*`):%- [_^String^ String]_
[* AppendFileName]([@(0.0.255) const]_[_^String^ String][@(0.0.255) `&]_[*@3 path1], 
[@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 path2], [@(0.0.255) const]_[@(0.0.255) char]_`*
[*@3 path3])&]
[s2; Like AppendFileName() but joining three paths. &]
[s3; &]
[s4;%- &]
[s5;:GetUpperFolder`(const char`*`):%- [_^String^ String]_[* GetUpperFolder]([@(0.0.255) co
nst]_[@(0.0.255) char]_`*[*@3 folderName])&]
[s2; Returns the name of the folder over [%-*@3 folderName].&]
[s3; &]
[s4;%- &]
[s5;:IsRootFolder`(const char`*`):%- [@(0.0.255) bool]_[* IsRootFolder]([@(0.0.255) const]_
[@(0.0.255) char]_`*[*@3 folderName])&]
[s0;l288; Returns false if there is a folder over [%-*@3 folderName]. 
It does not check if [%-*@3 folderName] exists.&]
[s3; &]
[s4;%- &]
[s5;:GetUpperFolder`(const String`&`):%- [_^String^ String]_[* GetUpperFolder]([@(0.0.255) c
onst]_[_^String^ String]_`&[*@3 folderName])&]
[s2; Returns the folder over [%-*@3 folderName].&]
[s3; &]
[s4;%- &]
[s5;:GetRelativePath`(String`&`,String`&`):%- [_^String^ String]_[* GetRelativePath]([_^String^ S
tring]_`&[*@3 from], [_^String^ String]_`&[*@3 path])&]
[s2; Returns the relative path to go from [%-*@3 from] to [%-*@3 path].&]
[s0;l288; For example, if:&]
[s0;l288;i150;O0; [%-*@3 from] `= `"/books/technology/computers`"&]
[s0;l288;i150;O0; [%-*@3 path ]`= `"/books/biology/mammals`"&]
[s0;l288; GetRelativePath() would return `"../../biology/mammals`".&]
[s0;l288; If there is no match between both paths it returns Null.&]
[s3; &]
[s4;%- &]
[s5;:FileRealName`(const char`*`):%- [_^String^ String]_[* FileRealName]([@(0.0.255) const]_
[@(0.0.255) char]_`*[*@3 path])&]
[s2; Returns the real name of [%-*@3 path].(directory or file) with 
the right letter cases or `"`" if [%-*@3 path].does not exist.&]
[s3; &]
[s4;%- &]
[s5;:GetNextFolder`(const char`*`,const char`*`):%- [_^String^ String]_[* GetNextFolder](
[@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 folder], [@(0.0.255) const]_[@(0.0.255) char]_`*
[*@3 lastFolder])&]
[s2; Considering [%-*@3 folder ]as a parent folder of [%-*@3 lastFolder], 
it returns the folder under [%-*@3 folder ]that is also parent 
of [%-*@3 lastFolder].&]
[s2; For example: GetNextFolder(`"/home`", `"/home/user/documents`") 
returns `"/home/user`".&]
[s3; &]
[s4; &]
[s5;:FileCompare`(const char`*`,const char`*`):%- [@(0.0.255) int]_[* FileCompare]([@(0.0.255) c
onst]_[@(0.0.255) char]_`*[*@3 path1], [@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 path2])&]
[s2; Compares files [%-*@3 path1] and [%-*@3 path2]. &]
[s2; It returns:&]
[s2;i150;O0; `-2 if there are problems opening or reading from the 
files.&]
[s2;i150;O0; `-1 if files are different.&]
[s2;i150;O0; 1 if files are equal.&]
[s3; &]
[s4;%- &]
[s5;:FindStringInFile`(const char`*`,const String`,int64`):%- [_^int64^ int64]_[* FindStr
ingInFile]([@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 file], [@(0.0.255) const]_[_^String^ S
tring]_[*@3 text], [_^int64^ int64]_[*@3 pos0]_`=_[@3 0])&]
[s2; Search in [%-*@3 file] for string [%-*@3 text] from byte position 
[%-*@3 pos0]. &]
[s2; It returns &]
[s2;i150;O0; `-2 if there are problems opening or reading from the 
[%-*@3 file].&]
[s2;i150;O0; `-1 if [%-*@3 text] is not found in [%-*@3 file].&]
[s2;i150;O0; An int64 value indicating the byte where [%-*@3 text] 
is found.&]
[s3; &]
[s4;%- &]
[s5;:GetDirectoryLength`(const char`*`):%- [_^int64^ Upp`::int64]_[* GetDirectoryLength]([@(0.0.255) c
onst]_[@(0.0.255) char]_`*[*@3 directoryName])&]
[s2; Returns the size in bytes of the sum of all file sizes under 
[%-*@3 directoryName].&]
[s3; &]
[s4;%- &]
[s5;:GetLength`(const char`*`):%- [_^int64^ Upp`::int64]_[* GetLength]([@(0.0.255) const]_[@(0.0.255) c
har]_`*[*@3 fileDirName])&]
[s2; Returns the size of [%-*@3 fileDirName].&]
[s0;l288;i150;O0; If it is a file, it returns its size.&]
[s0;l288;i150;O0; If it is a folder, it returns the size in bytes 
of the sum of all file sizes under [%-*@3 fileDirName].&]
[s3;%- &]
[s4;%- &]
[s5;:DirectoryCopy`(const char`*`,const char`*`):%- [@(0.0.255) bool]_[* DirectoryCopy]([@(0.0.255) c
onst]_[@(0.0.255) char]_`*[*@3 dir], [@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 newPlace])
&]
[s2; Copy folder [%-*@3 dir] including all files and folders to [%-*@3 newPlace].&]
[s0;l288; Returns true in case of success.&]
[s3; &]
[s4;%- &]
[s5;:SetReadOnly`(const char`*`,bool`):%- [@(0.0.255) bool]_[* SetReadOnly]([@(0.0.255) con
st]_[@(0.0.255) char]_`*[*@3 path], [@(0.0.255) bool]_[*@3 readOnly])&]
[s2; Sets [%-*@3 path ](file or folder) as read only or write permitted 
depending on the value of[%-*@3  readOnly].&]
[s0;l288; In Linux the permissions are set for user, group and others.&]
[s2; -|Returns true in case of success.&]
[s3; &]
[s4;%- &]
[s5;:SetReadOnly`(const char`*`,bool`,bool`,bool`):%- [@(0.0.255) bool]_[* SetReadOnly]([@(0.0.255) c
onst]_[@(0.0.255) char]_`*[*@3 path], [@(0.0.255) bool]_[*@3 user], [@(0.0.255) bool]_[*@3 gr
oup], [@(0.0.255) bool]_[*@3 others])&]
[s2; Sets [%-*@3 path] (file or folder) as read only or write permitted 
for[%-*@3  user], [%-*@3 group ]and [%-*@3 others].&]
[s0;l288; -|Returns true in case of success.&]
[s3; &]
[s4;%- &]
[s5;:IsReadOnly`(const char`*`,bool`&`,bool`&`,bool`&`):%- [@(0.0.255) bool]_[* IsReadOnl
y]([@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 path], [@(0.0.255) bool]_`&[*@3 user], 
[@(0.0.255) bool]_`&[*@3 group], [@(0.0.255) bool]_`&[*@3 others])&]
[s2; Returns true in [%-*@3 user], [%-*@3 group] or [%-*@3 others] if [%-*@3 path 
](file or folder) is read only for some of those user groups.&]
[s0; -|Returns true in case of success.&]
[s3;%- &]
[s4;%- &]
[s5;:LoadFile`_Safe`(const String`):%- [_^String^ String]_[* LoadFile`_Safe]([@(0.0.255) co
nst]_[_^String^ String]_[*@3 fileName])&]
[s2; Same as LoadFile([%-*@3 fileName]) but it works in POSIX for files 
automatically generated by the OS.&]
[s2; &]
[s2; U`+`+ LoadFile() functions prior to loading the file into a 
String, get the length of the file to dimension the String that 
will get the file. This is not valid for OS generated virtual 
files where the file length returned by the OS is 0 (for example 
files under folder /proc in Linux)&]
[s2; LoadFile`_Safe() just get the file bytes returned by the OS 
until the file end.&]
[s3; &]
[s4;%- &]
[s5;:IsFile`(const char`*`):%- [@(0.0.255) bool]_[* IsFile]([@(0.0.255) const]_[@(0.0.255) ch
ar]_`*[*@3 fileName])&]
[s2; Returns true if [%-*@3 fileName] is a file.&]
[s3; &]
[s4; &]
[s5;:IsFolder`(const char`*`):%- [@(0.0.255) bool]_[* IsFolder]([@(0.0.255) const]_[@(0.0.255) c
har]_`*[*@3 fileName])&]
[s2; Returns true id [%-*@3 fileName] is a folder.&]
[s3; &]
[s4; &]
[s5;:SearchFile`(String`,String`,String`,Array`<String`>`&`):%- [_^Array^ Array]<[_^String^ S
tring]>_[* SearchFile]([_^String^ String]_[*@3 dir], [_^String^ String]_[*@3 condFile], 
[_^String^ String]_[*@3 text], [_^Array^ Array]<[_^String^ String]>_`&[*@3 errorList])&]
[s2; Returns an Array of Strings containing the file names with full 
path of the files under folder [%-*@3 dir] that comply with condition 
(with wildcards) [%-*@3 condFile] and that contain inside the text 
[%-*@3 text].&]
[s2; [%-*@3 errorList] contains the errors.&]
[s3; &]
[s4;%- &]
[s5;:SearchFile`(String`,String`,String`):%- [_^Array^ Array]<[_^String^ String]>_[* Search
File]([_^String^ String]_[*@3 dir], [_^String^ String]_[*@3 condFile], 
[_^String^ String]_[*@3 text]_`=_`"`")&]
[s2; Returns an Array of Strings containing the file names with full 
path of the files under folder [%-*@3 dir] that comply with condition 
(with wildcards) [%-*@3 condFile ]and that contain inside the text 
[%-*@3 text].&]
[s3; &]
[s4; &]
[s5;:FileToTrashBin`(const char`*`):%- [@(0.0.255) void]_[* FileToTrashBin]([@(0.0.255) con
st]_[@(0.0.255) char]_`*[*@3 path])&]
[s2; Deletes file [%-*@3 path].by sending it to the Trash Bin.&]
[s3; &]
[s4;%- &]
[s5;:TrashBinGetCount`(`):%- [_^int64^ int64]_[* TrashBinGetCount]()&]
[s2; Returns the number of items (files and directories) located 
in the Trash Bin.&]
[s3;%- &]
[s4;%- &]
[s5;:TrashBinClear`(`):%- [@(0.0.255) void]_[* TrashBinClear]()&]
[s0; Removes all the items (files and directories) located in the 
Trash Bin.ç&]
[s3; &]
[s0; &]
[s0;%- &]
[ {{10000@1 [s0; [* File/Folder Handling `"X`" functions]]}}&]
[s0; &]
[s0;%- [%% File and folder handling functions with eXtras defined in 
]EXT`_FILE`_FLAGS&]
[s3; &]
[s4;%- &]
[s5;:USE`_TRASH`_BIN:%- [@(0.0.255) enum]_EXT`_FILE`_FLAGS_[* USE`_TRASH`_BIN]_`=_[@3 1]&]
[s2; If true, removed fildes and fodlers will be moved to the Trash 
(recycle) Bin.&]
[s3;%- &]
[s4;%- &]
[s5;:BROWSE`_LINKS:%- [@(0.0.255) enum]_EXT`_FILE`_FLAGS_[* BROWSE`_LINKS]_`=_[@3 2]&]
[s2; If true, folder functions will follow symbolic links.&]
[s3;%- &]
[s4;%- &]
[s5;:DELETE`_READ`_ONLY:%- [@(0.0.255) enum]_EXT`_FILE`_FLAGS_[* DELETE`_READ`_ONLY]_`=_[@3 4
]&]
[s2; If true, file/folder deletion functions will not consider read 
only flag.&]
[s0; &]
[s4;%- &]
[s5;:DeleteFolderDeepWildcardsX`(const char`*`,EXT`_FILE`_FLAGS`):%- [@(0.0.255) bool]_
[* DeleteFolderDeepWildcardsX]([@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 path], 
EXT`_FILE`_FLAGS_[*@3 flags]_`=_[@3 0])&]
[s2;%- [%% Delete all folders under ][*@3 path ][%% using ][*@3 flags][%%  
that comply the condition in wildcards included in ][*@3 path.]&]
[s2; Returns true in case of success.&]
[s3; &]
[s4;%- &]
[s5;:DeleteFileDeepWildcardsX`(const char`*`,EXT`_FILE`_FLAGS`):%- [@(0.0.255) bool]_[* D
eleteFileDeepWildcardsX]([@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 path], 
EXT`_FILE`_FLAGS_[*@3 flags]_`=_[@3 0])&]
[s2;%- [%% Delete all files under ][*@3 path ][%% using ][*@3 flags][%%  that 
comply the condition in wildcards included in ][*@3 path.]&]
[s2; Returns true in case of success.&]
[s3; &]
[s4;%- &]
[s5;:DeleteFolderDeepX`(const char`*`,EXT`_FILE`_FLAGS`):%- [@(0.0.255) bool]_[* DeleteFo
lderDeepX]([@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 path], EXT`_FILE`_FLAGS_[*@3 flags
]_`=_[@3 0])&]
[s2; Delete [%-*@3 path] folder and all files and folders under it 
using [%-*@3 flags].&]
[s2; Returns true in case of success.&]
[s3;%- &]
[s4;%- &]
[s5;:RenameDeepWildcardsX`(const char`*`,const char`*`,const char`*`,bool`,bool`,EXT`_FILE`_FLAGS`):%- [@(0.0.255) b
ool]_[* RenameDeepWildcardsX]([@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 path], 
[@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 namewc], [@(0.0.255) const]_[@(0.0.255) char]_`*
[*@3 newname], [@(0.0.255) bool]_[*@3 forfile], [@(0.0.255) bool]_[*@3 forfolder], 
EXT`_FILE`_FLAGS_[*@3 flags]_`=_NO`_FLAG)&]
[s2; Rename to [%-*@3 newname] the files (if [%-*@3 forfile] is set to 
true) and/or folders (if [%-*@3 forfolder] is set to true) under 
[%-*@3 path] that comply the condition in wildcards included in 
[%-*@3 namewc] using [%-*@3 flags] .&]
[s2; Returns true in case of success.&]
[s3; &]
[s4;%- &]
[s5;:FileMoveX`(const char`*`,const char`*`,EXT`_FILE`_FLAGS`):%- [@(0.0.255) bool]_[* Fi
leMoveX]([@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 oldpath], [@(0.0.255) const]_[@(0.0.255) c
har]_`*[*@3 newpath], EXT`_FILE`_FLAGS_[*@3 flags]_`=_NO`_FLAG)&]
[s2; Moves or renames [%-*@3 oldpath] file to [%-*@3 newpath] using [%-*@3 flags].&]
[s2; Returns true in case of success.&]
[s3; &]
[s4; &]
[s5;:FileDeleteX`(const char`*`,EXT`_FILE`_FLAGS`):%- [@(0.0.255) bool]_[* FileDeleteX]([@(0.0.255) c
onst]_[@(0.0.255) char]_`*[*@3 path], EXT`_FILE`_FLAGS_[*@3 flags]_`=_NO`_FLAG)&]
[s2; Deletes [%-*@3 path] file using [%-*@3 flags].&]
[s2; Returns true in case of success.&]
[s3; &]
[s0; &]
[ {{10000@1 [s0; [* String functions]]}}&]
[s4;%- &]
[s5;:Trim`(const String`&`):%- [_^String^ String]_[* Trim]([@(0.0.255) const]_[_^String^ Stri
ng][@(0.0.255) `&]_[*@3 s])&]
[s2; Like TrimBoth(), it removes left and right spaces in [%-*@3 s]. 
Spaces are chars like, `' `', `'`\t`', `'`\n`', ...&]
[s3; &]
[s4;%- &]
[s5;:Tokenize`(const String`&`,const String`&`,int`&`):%- [_^String^ String]_[* Tokenize](
[@(0.0.255) const]_[_^String^ String]_`&[*@3 str], [@(0.0.255) const]_[_^String^ String]_`&
[*@3 token], [@(0.0.255) int]_`&[*@3 pos])&]
[s2; Finds the next token in [%-*@3 str] beginning from [%-*@3 pos]. 
[%-*@3 token] is a String that contains token identifier chars.&]
[s0;l288; Returns the String from original [%-*@3 pos] to the char 
after found token or until the end of the String if no token 
is found. [%-*@3 pos] is updated to the next char after returned 
String.&]
[s3; &]
[s4;%- &]
[s5;:Tokenize`(const String`&`,const String`&`):%- [_^String^ String]_[* Tokenize]([@(0.0.255) c
onst]_[_^String^ String]_`&[*@3 str], [@(0.0.255) const]_[_^String^ String]_`&[*@3 tokenEnd
])&]
[s2; Finds a token in [%-*@3 str]. [%-*@3 tokenEnd] is a String that 
contains token identifier chars.&]
[s2; Returns the String to the char after found token or until the 
end of the String if no token is found.&]
[s3; &]
[s4;%- &]
[s5;:ReadCSV`(const String`,char`,bool`,bool`):%- [_^Vector^ Vector]<[_^Vector^ Vector]_<
[_^Value^ Value]>_>_[* ReadCSV]([@(0.0.255) const]_[_^String^ String]_[*@3 strFile], 
[@(0.0.255) char]_[*@3 separator], [@(0.0.255) bool]_[*@3 bycols], [@(0.0.255) bool]_[*@3 rem
oveRepeated])&]
[s2; Read a .csv file in String [%-*@3 strFile] where [%-*@3 separator] 
can be defined.&]
[s2; If [%-*@3 bycols] is set, the data is saved in `[column`]`[rows`] 
order.&]
[s2; If [%-*@3 removeRepeated ]then repeated rows will be deleted.&]
[s3; &]
[s4;%- &]
[s5;:WriteCSV`(Vector`<Vector`<Value`>`>`&`,char`,bool`):%- [_^String^ String]_[* WriteCS
V]([_^Vector^ Vector]<[_^Vector^ Vector]_<[_^Value^ Value]>_>_`&[*@3 data], 
[@(0.0.255) char]_[*@3 separator], [@(0.0.255) bool]_[*@3 bycols])&]
[s2; Save a .csv file to a String where [%-*@3 separator] can be defined.&]
[s2; If [%-*@3 bycols] is set, the data is supposed to be stored in 
`[column`]`[rows`] order.&]
[s3; &]
[s4; &]
[s5;:ReverseFind`(const String`&`,const String`&`,int`):%- [@(0.0.255) int]_[* ReverseFin
d]([@(0.0.255) const]_[_^String^ String][@(0.0.255) `&]_[*@3 s], [@(0.0.255) const]_[_^String^ S
tring][@(0.0.255) `&]_[*@3 toFind], [@(0.0.255) int]_[*@3 from]_`=_[@3 0])&]
[s2; Find in [%-*@3 s] the String [%-*@3 toFind] beginning in [%-*@3 from 
]in reverse order.&]
[s2; Similar to Find() but searching from the end of the String.&]
[s3; &]
[s4;%- &]
[s5;:FormatLong`(long`):%- [_^String^ String]_[* FormatLong]([@(0.0.255) long]_[*@3 a])&]
[s2; Returns the long number [%-*@3 a ]converted into a String.&]
[s3; &]
[s4;%- &]
[s5;:SecondsToString`(double`,bool`):%- [_^String^ String]_[* SecondsToString]([@(0.0.255) d
ouble]_[*@3 seconds], [@(0.0.255) bool]_[*@3 units]_`=_[@(0.0.255) false])&]
[s2; Converts an amount of [%-*@3 seconds] to a String formatted as 
HH:MM:SS.&]
[s0; -|If [%-*@3 units ]is true the units (hours, mins, seconds) will 
be part of the text.&]
[s0; -|It is opposite to StringToSeconds()&]
[s3; &]
[s4;%- &]
[s5;:StringToSeconds`(String`):%- [@(0.0.255) double]_[* StringToSeconds]([_^String^ String
]_[*@3 str])&]
[s2; Converts a String [%-*@3 str ]formatted as HH:MM:SS to a returned 
amount of seconds.&]
[s0; -|It is opposite to SecondsToString()&]
[s3; &]
[s4;%- &]
[s5;:HMSToString`(int`,int`,double`,bool`):%- [_^String^ String]_[* HMSToString]([@(0.0.255) i
nt]_[*@3 hour], [@(0.0.255) int]_[*@3 min], [@(0.0.255) double]_[*@3 seconds], 
[@(0.0.255) bool]_[*@3 units ]`=_[@(0.0.255) false])&]
[s2; Returns a String containing [%-*@3 hour], [%-*@3 min ]and [%-*@3 seconds 
]formatted as HH:MM:SS.&]
[s0;l288; If [%-*@3 units ]is true the units (hours, mins, seconds) 
will be part of the text.&]
[s0; -|It is opposite to StringToHMS().&]
[s0; &]
[s4;%- &]
[s5;:StringToHMS`(String`,int`&`,int`&`,double`&`):%- [@(0.0.255) void]_[* StringToHMS]([_^String^ S
tring]_[*@3 durat], [@(0.0.255) int]_`&[*@3 hour], [@(0.0.255) int]_`&[*@3 min], 
[@(0.0.255) double]_`&[*@3 seconds])&]
[s2; It converts a String [%-*@3 durat ]formatted as HH:MM:SS to get 
[%-*@3 hour], [%-*@3 min ]and [%-*@3 seconds].&]
[s0; -|It is opposite to HMSToString()&]
[s3; &]
[s4; &]
[s5;:BytesToString`(uint64`,bool`):%- [_^String^ String]_[* BytesToString]([_^uint64^ uint6
4]_[*@3 bytes, ][_^uint64^ bool]_[*@3 units])&]
[s2; Converts an amount of [%-*@3 bytes ]to a short String. If [%-*@3 units] 
is true, they will appear like in 123.4 Kb&]
[s3; &]
[s4;%- &]
[s5;:StrToTime`(const char`*`):%- [_^Time^ `::Time]_[* StrToTime]([@(0.0.255) const]_[@(0.0.255) c
har]_`*[*@3 s])&]
[s2;%- [%% Like ][@(0.0.255) const]_[@(0.0.255) char]_`*[* StrToTime]([@(0.0.255) struct]_[_^Time^ U
pp`::Time][@(0.0.255) `&]_[*@3 d], [@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 s]), 
but returning directly a Time.&]
[s3; &]
[s4;%- &]
[s5;:StrToDate`(const char`*`):%- [_^Date^ `::Date]_[* StrToDate]([@(0.0.255) const]_[@(0.0.255) c
har]_`*[*@3 s])&]
[s2;%- [%% Like ][@(0.0.255) const]_[@(0.0.255) char]_`*[* StrToDate]([@(0.0.255) struct]_[_^Time^ U
pp`::Date][@(0.0.255) `&]_[*@3 d], [@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 s]), 
but returning directly a Date.&]
[s3; &]
[s4;%- &]
[s5;:SeasonName`(int`):%- [_^String^ String]_[* SeasonName]([@(0.0.255) int]_[*@3 i])&]
[s2; It returns the name of the season beginning from `"winter`" 
([%-*@3 i] `= 0).&]
[s3; &]
[s4;%- &]
[s5;:FormatDoubleAdjust`(double`,double`):%- [_^String^ String]_[* FormatDoubleAdjust]([@(0.0.255) d
ouble]_[*@3 d], [@(0.0.255) double]_[*@3 range])&]
[s2; Returns number [%-*@3 d] in [%-*@3 range ]converted to a String, 
so that it has the right number of digits.&]
[s0;l288; This function serves to show numbers in graph axis and 
other elements that require to show the minimum right quantity 
of digits from a double.&]
[s3; &]
[s4;%- &]
[s5;:RemoveAccents`(String`):%- [_^String^ String]_[* RemoveAccents]([_^String^ String]_[*@3 s
tr])&]
[s2; Returns [%-*@3 str ]with the accented characters substituted with 
the most approximated ascii characters.&]
[s0;l288; If the original letter is uppercase, begins a word and 
the second letter is lowercase, the substituted first letter 
will be uppercase and the second will be lowercase.&]
[s0;l288; An accented letter can be substituted with one or some 
characters.&]
[s0;l288; For example `"Århus Ørsted Ñandú crème brûlée`" 
is converted in `"Aarhus Oersted Nandu creme brulee`"&]
[s3;%- &]
[s4;%- &]
[s5;:FitFileName`(String`,int`):%- [_^String^ String]_[* FitFileName]([@(0.0.255) const]_[_^String^ S
tring]_[*@3 fileName], [@(0.0.255) int]_[*@3 len])&]
[s2; Fits String [%-*@3 fileName] length until [%-*@3 len] removing just 
intermediate folder names replacing them with `"...`".&]
[s3; &]
[s4; &]
[s5;:IsPunctuation`(wchar`):%- [@(0.0.255) bool]_[* IsPunctuation]([_^wchar^ wchar]_[*@3 c])&]
[s2; Returns true if [%-*@3 c ]is a punctuation character.&]
[s3;%- &]
[s4;%- &]
[s5;:Replace`(String`,String`,String`):%- [_^String^ String]_[* Replace]([_^String^ String]_
[*@3 str], [_^String^ String]_[*@3 find], [_^String^ String]_[*@3 replace])&]
[s0;l288; Replaces in [%-*@3 str] substring [%-*@3 find] with [%-*@3 replace] 
string for all times [%-*@3 find] string appears.&]
[s3; &]
[s4;%- &]
[s5;:Replace`(String`,char`,char`):%- [_^String^ String]_[* Replace]([_^String^ String]_[*@3 s
tr], [@(0.0.255) char]_[*@3 find], [@(0.0.255) char]_[*@3 replace])&]
[s2; Replaces in [%-*@3 str] char [%-*@3 find] with [%-*@3 replace] char 
for all times [%-*@3 find] char appears.&]
[s3; &]
[s0; &]
[ {{10000@1 [s0; [* Special folders]]}}&]
[s4; &]
[s5;:GetTempFolder`(`):%- [_^String^ String]_[* GetTempFolder]()&]
[s2; Gets the default temp files folder path.&]
[s3;%- &]
[s0; &]
[ {{10000@1 [s0; [* Math functions]]}}&]
[s4; &]
[s5;:Sign`(T`):%- [@(0.0.255) template]_<[@(0.0.255) class]_[*@4 T]>_[@(0.0.255) int]_[* Sign](
[*@4 T]_[*@3 a])&]
[s2;%- [%% Returns the sign of ][*@3 a:]&]
[s2;i150;O0; `+1 if [%-*@3 a]. is positive&]
[s2;i150;O0; 0 if [%-*@3 a] is 0&]
[s2;i150;O0; `-1 if [%-*@3 a] is negative.&]
[s3; &]
[s4;%- &]
[s5;:Average`(T`,T`):%- [@(0.0.255) template]_<[@(0.0.255) class]_[*@4 T]>_[*@4 T]_[* Average](
[*@4 T]_[*@3 a], [*@4 T]_[*@3 b])&]
[s2; Returns the average of [%-*@3 a] and [%-*@3 b].&]
[s3; &]
[s4;%- &]
[s5;:Average`(T`,T`,T`):%- [@(0.0.255) template]_<[@(0.0.255) class]_[*@4 T]>_[*@4 T]_[* Averag
e]([*@4 T]_[*@3 a], [*@4 T]_[*@3 b], [*@4 T]_[*@3 c])&]
[s2; Returns the average of [%-*@3 a], [%-*@3 b] and [%-*@3 c].&]
[s3; &]
[s4;%- &]
[s5;:Average`(T`,T`,T`,T`):%- [@(0.0.255) template]_<[@(0.0.255) class]_[*@4 T]>_[*@4 T]_[* Ave
rage]([*@4 T]_[*@3 a], [*@4 T]_[*@3 b], [*@4 T]_[*@3 c], [*@4 T]_[*@3 d])&]
[s2; Returns the average of [%-*@3 a], [%-*@3 b], [%-*@3 c] and [%-*@3 d].&]
[s3; &]
[s4;%- &]
[s5;:pow2`(T`):%- [@(0.0.255) template]_<[@(0.0.255) class]_[*@4 T]>_[*@4 T]_[* pow2]([*@4 T]_[*@3 a
])&]
[s2; Raises [%-*@3 a] to 2. Similar to sqr().&]
[s3; &]
[s4;%- &]
[s5;:pow3`(T`):%- [@(0.0.255) template]_<[@(0.0.255) class]_[*@4 T]>_[*@4 T]_[* pow3]([*@4 T]_[*@3 a
])&]
[s2; Raises [%-*@3 a] to 3.&]
[s3; &]
[s4;%- &]
[s5;:pow4`(T`):%- [@(0.0.255) template]_<[@(0.0.255) class]_[*@4 T]>_[*@4 T]_[* pow4]([*@4 T]_[*@3 a
])&]
[s2; Raises [%-*@3 a] to 4.&]
[s3; &]
[s4;%- &]
[s5;:Between`(const T`&`,const T`&`,const T`&`):%- [@(0.0.255) template]_<[@(0.0.255) cla
ss]_[*@4 T]>_[@(0.0.255) bool]_[* Between]([@(0.0.255) const]_[*@4 T][@(0.0.255) `&]_[*@3 val],
 [@(0.0.255) const]_[*@4 T][@(0.0.255) `&]_[*@3 min], [@(0.0.255) const]_[*@4 T][@(0.0.255) `&]_
[*@3 max])&]
[s2; Returns true if [%-*@3 val] is between [%-*@3 min] and [%-*@3 max].&]
[s3; &]
[s4;%- &]
[s5;:AngleAdd360`(T`,T`):%- [@(0.0.255) template]_<[@(0.0.255) class]_[*@4 T]>_[*@4 T]_[* Angle
Add360]([*@4 T]_[*@3 ang], [*@4 T]_[*@3 val])&]
[s2; Adds [%-*@3 val] to [%-*@3 ang] taking care that result is between 
0 and 360º.&]
[s3; &]
[s4;%- &]
[s5;:Distance`(const T`&`,const T`&`,const T`&`,const T`&`):%- [@(0.0.255) template]_<[@(0.0.255) c
lass]_[*@4 T]>_[@(0.0.255) const]_[*@4 T]_[* Distance]([@(0.0.255) const]_[*@4 T][@(0.0.255) `&
]_[*@3 x1], [@(0.0.255) const]_[*@4 T][@(0.0.255) `&]_[*@3 y1], [@(0.0.255) const]_[*@4 T][@(0.0.255) `&
]_[*@3 x2], [@(0.0.255) const]_[*@4 T][@(0.0.255) `&]_[*@3 y2])&]
[s2; Returns the distance ([^http`:`/`/en`.wikipedia`.org`/wiki`/Euclidean`_distance^ e
uclidean]) between ([%-*@3 x1], [%-*@3 y1]) and ([%-*@3 x2], [%-*@3 y2]).&]
[s3; &]
[s4;%- &]
[s5;:Distance`(const Point`_`<T`>`&`,const Point`_`<T`>`&`):%- [@(0.0.255) template]_<[@(0.0.255) c
lass]_[*@4 T]>_[@(0.0.255) const]_[*@4 T]_[* Distance]([@(0.0.255) const]_[_^Point`_^ Point`_
]<[*@4 T]>`&_[*@3 p1], [@(0.0.255) const]_[_^Point`_^ Point`_]<[*@4 T]>`&_[*@3 p2])&]
[s2; Returns the distance ([^http`:`/`/en`.wikipedia`.org`/wiki`/Euclidean`_distance^ e
uclidean]) between [%-*@3 p1] and [%-*@3 p2].&]
[s3; &]
[s4;%- &]
[s5;:Distance`(const T`&`,const T`&`,const T`&`,const T`&`,const T`&`,const T`&`):%- [@(0.0.255) t
emplate]_<[@(0.0.255) class]_[*@4 T]>_[@(0.0.255) const]_[*@4 T]_[* Distance]([@(0.0.255) con
st]_[*@4 T][@(0.0.255) `&]_[*@3 x1], [@(0.0.255) const]_[*@4 T][@(0.0.255) `&]_[*@3 y1], 
[@(0.0.255) const]_[*@4 T][@(0.0.255) `&]_[*@3 z1], [@(0.0.255) const]_[*@4 T][@(0.0.255) `&]_[*@3 x
2], [@(0.0.255) const]_[*@4 T][@(0.0.255) `&]_[*@3 y2], [@(0.0.255) const]_[*@4 T][@(0.0.255) `&
]_[*@3 z2])&]
[s2; Returns the distance ([^http`:`/`/en`.wikipedia`.org`/wiki`/Euclidean`_distance^ e
uclidean]) between ([%-*@3 x1], [%-*@3 y1] [%-*@3 z1]), and ([%-*@3 x2], 
[%-*@3 y2], [%-*@3 z2]).&]
[s3; &]
[s4;%- &]
[s5;:Angle`(const T`&`,const T`&`,const T`&`,const T`&`):%- [@(0.0.255) template]_<[@(0.0.255) c
lass]_[*@4 T]>_[@(0.0.255) const]_[@(0.0.255) double]_[* Angle]([@(0.0.255) const]_[*@4 T][@(0.0.255) `&
]_[*@3 x1], [@(0.0.255) const]_[*@4 T][@(0.0.255) `&]_[*@3 y1], [@(0.0.255) const]_[*@4 T][@(0.0.255) `&
]_[*@3 x2], [@(0.0.255) const]_[*@4 T][@(0.0.255) `&]_[*@3 y2])&]
[s2; Returns the angle between ([%-*@3 x1], [%-*@3 y1]) and ([%-*@3 x2], 
[%-*@3 y2]).&]
[s3; &]
[s4;%- &]
[s5;:Angle`(const Point`_`<T`>`&`,const Point`_`<T`>`&`):%- [@(0.0.255) template]_<[@(0.0.255) c
lass]_[*@4 T]>_[@(0.0.255) const]_[@(0.0.255) double]_[* Angle]([@(0.0.255) const]_[_^Point`_^ P
oint`_]<[*@4 T]>`&_[*@3 p1], [@(0.0.255) const]_[_^Point`_^ Point`_]<[*@4 T]>`&_[*@3 p2])&]
[s2; Return the angle between [%-*@3 p1] and [%-*@3 p2].&]
[s3; &]
[s4; &]
[s5;:Odd`(int`):%- [@(0.0.255) bool]_[* Odd]([@(0.0.255) int]_[*@3 val])&]
[s2; Returns true if [%-*@3 val ]is odd.&]
[s3; &]
[s4;%- &]
[s5;:Even`(int`):%- [@(0.0.255) bool]_[* Even]([@(0.0.255) int]_[*@3 val])&]
[s2; Returns true if [%-*@3 val ]is even.&]
[s3; &]
[s4;%- &]
[s5;:RoundEven`(int`):%- [@(0.0.255) int]_[* RoundEven]([@(0.0.255) int]_[*@3 val])&]
[s2; Returns [%-*@3 val ]rounded to the upper even number or itself 
if it is even.&]
[s3; &]
[s4;%- &]
[s5;:ToRad`(double`):%- [@(0.0.255) double]_[* ToRad]([@(0.0.255) double]_[*@3 angle])&]
[s2; Returns [%-*@3 angle ]in degrees converted to radians.&]
[s3; &]
[s4; &]
[s5;:ToDeg`(double`):%- [@(0.0.255) double]_[* ToDeg]([@(0.0.255) double]_[*@3 angle])&]
[s0;l288; Returns [%-*@3 angle ]in radians converted to degrees.&]
[s3; &]
[ {{10000@1 [s0; [* Graphical functions]]}}&]
[s4;%- &]
[s5;:FitInFrame`(const Size`_`<T`>`&`,const Size`_`<T`>`&`):%- [@(0.0.255) template]_<[@(0.0.255) c
lass]_[*@4 T]>_[_^Rect`_^ Rect`_]<[*@4 T]>_[* FitInFrame]([@(0.0.255) const]_[_^Size`_^ Size`_
]<[*@4 T]>_`&[*@3 frame], [@(0.0.255) const]_[_^Size`_^ Size`_]<[*@4 T]>_`&[*@3 object])&]
[s2; Returns the rectangle in a Rect to fit inside [%-*@3 frame] the 
[%-*@3 object ]maintaining the aspect ratio.&]
[s0; &]
[s0;= 
@@image:3268&2037
(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)
&]
[s3; &]
[s4;%- &]
[s5;:SaveImage`(const Image`&`,int`,const String`&`,String`):%- [@(0.0.255) bool]_[* Save
Image]([@(0.0.255) const]_[_^Image^ Image]_`&[*@3 img], [@(0.0.255) int]_[*@3 qualityBpp], 
[@(0.0.255) const]_[_^String^ String]_`&[*@3 fileName], [_^String^ String]_[*@3 ext]_`=_`"`"
)&]
[s2; Saves Image [%-*@3 img] in file [%-*@3 fileName] with format [%-*@3 ext]. 
If ext is `"`", the format is taken from [%-*@3 fileName] extension.&]
[s2; [%-*@3 qualityBpp] is the quality (for jpg) or the bits per pixel 
(for other formats):&]
[s3; &]
[s4;%- &]
[s5;:PrintImage`(const Image`&`,int`,int`,int`,int`):%- [@(0.0.255) bool]_[* PrintImage](
[@(0.0.255) const]_[_^Image^ Image]_`&[*@3 img], [@(0.0.255) int]_[*@3 x]_`=_[@3 0], 
[@(0.0.255) int]_[*@3 y]_`=_[@3 0], [@(0.0.255) int]_[*@3 width]_`=_Null, 
[@(0.0.255) int]_[*@3 height]_`=_Null)&]
[s2; Prints image [%-*@3 img] in coords. [%-*@3 x,] [%-*@3 y,] [%-*@3 width,] 
[%-*@3 height].&]
[s3; &]
[s4;%- &]
[s5;:DrawRectLine`(Draw`&`,int`,int`,int`,int`,int`,const Color`&`):%- [@(0.0.255) void
]_[* DrawRectLine]([_^Draw^ Draw][@(0.0.255) `&]_[*@3 w], [@(0.0.255) int]_[*@3 x], 
[@(0.0.255) int]_[*@3 y], [@(0.0.255) int]_[*@3 width], [@(0.0.255) int]_[*@3 height], 
[@(0.0.255) int]_[*@3 lineWidth], [@(0.0.255) const]_[_^Color^ Color]_`&[*@3 color])&]
[s5;:DrawRectLine`(Draw`&`,Point`&`,Size`&`,int`,const Color`&`):%- [@(0.0.255) void]_[* D
rawRectLine]([_^Draw^ Draw][@(0.0.255) `&]_[*@3 w], [_^Point^ Point]_`&[*@3 pos], 
[_^Size^ Size]_`&[*@3 s], [@(0.0.255) int]_[*@3 lineWidth], [@(0.0.255) const]_[_^Color^ Colo
r]_`&[*@3 color])&]
[s5;:DrawRectLine`(Draw`&`,Rect`&`,int`,const Color`&`):%- [@(0.0.255) void]_[* DrawRectL
ine]([_^Draw^ Draw][@(0.0.255) `&]_[*@3 w], [_^Rect^ Rect]_`&[*@3 r], [@(0.0.255) int]_[*@3 lin
eWidth], [@(0.0.255) const]_[_^Color^ Color]_`&[*@3 color])&]
[s2; Draws a rectangle in Draw [%-*@3 w] defined by either [%-*@3 x,] 
[%-*@3 y,] [%-*@3 width,] [%-*@3 height][%- , or Point ][%-*@3 pos][%-  and] 
Size [%-*@3 s][%-  or Rect ][%-*@3 r][%- , with line width] [%-*@3 lineWidth] 
and Color [%-*@3 color].&]
[s3; &]
[s4; &]
[s2;l0;:RandomColor`(`):%- [_^Color^ Color]_[* RandomColor]()&]
[s2; Returns a random color.&]
[s3; &]
[s4;%- &]
[s5;:GetRainbowColor`(int`):%- [_^Color^ Color]_[* GetRainbowColor]([@(0.0.255) int]_[*@3 val
16])&]
[s2; Returns a rainbow color from an input value [%-*@3 val16] from 
0 (Red) to 0xFFFF.&]
[s3; &]
[s4; &]
[s5;:GetRect`(const Image`&`,const Rect`&`):%- [_^Image^ Image]_[* GetRect]([@(0.0.255) con
st]_[_^Image^ Image][@(0.0.255) `&]_[*@3 orig], [@(0.0.255) const]_[_^Rect^ Rect]_`&[*@3 r])&]
[s2; Returns a subimage delimited by Rect [%-*@3 r] taken from [%-*@3 orig].&]
[s3; &]
[s4;%- &]
[s5;:GetPixel`(const Image`&`,int`,int`):%- [@(0.0.255) const]_[_^RGBA^ RGBA]_`*[* GetPixel
](const Image_`&[*@3 img], [@(0.0.255) int]_[*@3 x], [@(0.0.255) int]_[*@3 y])&]
[s2; Gets the color of a pixel in image [%-*@3 img] in coordinates 
[%-*@3 x], [%-*@3 y].&]
[s2; This pixel is read`-only.&]
[s3;%- &]
[s4;%- &]
[s5;:GetPixel`(ImageBuffer`&`,int`,int`):%- [_^RGBA^ RGBA]_`*[* GetPixel](ImageBuffer_`&[*@3 i
mg], [@(0.0.255) int]_[*@3 x], [@(0.0.255) int]_[*@3 y])&]
[s2; Gets the color of a pixel in image [%-*@3 img] in coordinates 
[%-*@3 x], [%-*@3 y]. &]
[s2; This pixel is modifiable.&]
[s3; &]
[s4; &]
[s5;:IsValid`(const Image`&`,int`,int`):%- [@(0.0.255) bool]_[* IsValid]([@(0.0.255) const]_
[_^Image^ Image]_`&[*@3 img], [@(0.0.255) int]_[*@3 x], [@(0.0.255) int]_[*@3 y])&]
[s2; Returns true if coordinates [%-*@3 x] and [%-*@3 y] are inside Image 
[%-*@3 img].&]
[s3; &]
[s4; &]
[s5;:IsValid`(ImageBuffer`&`,int`,int`):%- [@(0.0.255) bool]_[* IsValid]([_^ImageBuffer^ Im
ageBuffer]_`&[*@3 img], [@(0.0.255) int]_[*@3 x], [@(0.0.255) int]_[*@3 y])&]
[s2; Returns true if coordinates [%-*@3 x] and [%-*@3 y] are inside ImageBuffer 
[%-*@3 img].&]
[s3; &]
[s4;%- &]
[s5;:IsValid`(const Image`&`,T`&`):%- [@(0.0.255) template]_<[@(0.0.255) class]_[*@4 T]>_[@(0.0.255) b
ool]_[* IsValid]([@(0.0.255) const]_[_^Image^ Image]_`&[*@3 img], [*@4 T]_`&[*@3 t])&]
[s2; Returns true if point [%-*@3 t] is inside Image [%-*@3 img].&]
[s3; &]
[s4;%- &]
[s5;:IsValid`(ImageBuffer`&`,T`&`):%- [@(0.0.255) template]_<[@(0.0.255) class]_[*@4 T]>_[@(0.0.255) b
ool]_[* IsValid]([_^ImageBuffer^ ImageBuffer]_`&[*@3 img], [*@4 T]_`&[*@3 t])&]
[s2; Returns true if point [%-*@3 t] is inside ImageBuffer [%-*@3 img].&]
[s3; &]
[ {{10000@1 [s0; [* Other functions]]}}&]
[s3; &]
[s5;:GetExtExecutable`(const String`):%- [_^String^ String]_[* GetExtExecutable]([@(0.0.255) c
onst]_[_^String^ String]_[*@3 ext])&]
[s2; Gets the program that will open by default the files with extension 
[%-*@3 ext].&]
[s0; &]
[s0; -|[/ Example:]&]
[s0; [/ -|GetExtExecutable(`"html`") `-> `"Firefox.exe`"]&]
[s3; &]
[s4;%- &]
[s5;:GetDriveList`(`):%- [_^Array^ Array]<[_^String^ String]>_[* GetDriveList]()&]
[s2; Returns an array with the paths to all drives, internal or external, 
identified in the system.&]
[s3; &]
[s4;%- &]
[s5;:Getcwd`(`):%- [_^String^ String]_[* Getcwd]()&]
[s2; Returns an absolute file name representing the current process 
working directory.&]
[s6; Replaced by GetCurrentDurectory(.)&]
[s3;%- &]
[s4;%- &]
[s5;:Chdir`(const String`&`):%- [@(0.0.255) bool]_[* Chdir]_([@(0.0.255) const]_[_^String^ St
ring]_`&[*@3 folder])&]
[s2; Sets the process`'s working directory to [%-*@3 folder].&]
[s0;l288; Returns true if successful.&]
[s6; Replaced by SetCurrentDurectory().&]
[s3; &]
[s4;%- &]
[s5;:GetDesktopManagerNew`(`):%- [_^String^ String]_[* GetDesktopManagerNew]()&]
[s2; A more complete version of GetDesktopManager() based on GetOsInfo().&]
[s3; &]
[s4;%- &]
[s5;:NON`_REENTRANT`_V:%- [* NON`_REENTRANT`_V]&]
[s5;:NON`_REENTRANT`(v`):%- [* NON`_REENTRANT]([*@3 v])&]
[s2; It returns [%-*@3 v] if the function has been called in the same 
thread and it has not returned yet.&]
[s0;l288; It is useful to avoid a function to be called twice in 
different situations, for example:&]
[s0;#l288;i150;O0; The function calls Ctrl`::ProcessEvents(); If the 
function has been called when a button has been pressed, this 
button can be pressed again so the function can be called twice.&]
[s0;#l288;i150;O0; The function is called by ChildLostFocus() and 
it opens a new window using, lets say, PromptOK(). This way ChildLostFocus 
will be called twice: the second one because PromptOK() makes 
the field to loose the focus.&]
[s3; ]]