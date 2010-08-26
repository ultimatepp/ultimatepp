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
[s5;:LaunchFile`(const String`):%- [@(0.0.255) bool]_[* LaunchFile]([@(0.0.255) const]_[_^String^ S
tring]_[*@3 file])&]
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
[s5;:AppendFileName`(const String`&`,const char`*`,const char`*`):%- [_^String^ String]_
[* AppendFileName]([@(0.0.255) const]_[_^String^ String][@(0.0.255) `&]_[*@3 path1], 
[@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 path2], [@(0.0.255) const]_[@(0.0.255) char]_`*
[*@3 path3])&]
[s2; Like AppendFileName() but joining three paths. &]
[s3; &]
[s4;%- &]
[s5;:GetUpperFolder`(String`):%- [_^String^ String]_[* GetUpperFolder]([_^String^ String]_[*@3 f
olderName])&]
[s2; Returns the name of the folder over [%-*@3 folderName].&]
[s3; &]
[s4;%- &]
[s5;:ThereIsUpperFolder`(String`):%- [@(0.0.255) bool]_[* ThereIsUpperFolder]([_^String^ St
ring]_[*@3 folderName])&]
[s0;l288; Returns true if there is a folder over [%-*@3 folderName].&]
[s3; &]
[s4;%- &]
[s5;:GetRealName`(String`):%- [_^String^ String]_[* GetRealName]([_^String^ String]_[*@3 path
])&]
[s2; Returns the real name of [%-*@3 path].(directory or file) with 
the right letter cases or `"`" if [%-*@3 path].does not exist.&]
[s3; &]
[s4;%- &]
[s5;:GetNextFolder`(String`,String`):%- [_^String^ String]_[* GetNextFolder]([_^String^ Str
ing]_[*@3 folder], [_^String^ String]_[*@3 lastFolder])&]
[s2; Considering [%-*@3 folder ]as a parent folder of [%-*@3 lastFolder], 
it returns the folder under [%-*@3 folder ]that is also parent 
of [%-*@3 lastFolder].&]
[s0;l288; For example: GetNextFolder(`"/home`", `"/home/user/documents`") 
returns `"/home/user`".&]
[s3; &]
[s4;%- &]
[s5;:CreateFolderDeep`(const char`*`):%- [@(0.0.255) bool]_[* CreateFolderDeep]([@(0.0.255) c
onst]_[@(0.0.255) char]_`*[*@3 dir]) [*@6 DEPRECATED]&]
[s2; Creates folder [%-*@3 dir], creating intermediate folders if necessary.&]
[s0;l288; Superseded by Core RealizePath()&]
[s3; &]
[s4;%- &]
[s5;:GetDirectoryLength`(String`):%- [_^int64^ int64]_[* GetDirectoryLength]([_^String^ Str
ing]_[*@3 directoryName])&]
[s2; Returns the size in bytes of the sum of all file sizes under 
[%-*@3 directoryName].&]
[s3; &]
[s4;%- &]
[s5;:GetLength`(String`):%- [_^int64^ int64]_[* GetLength]([_^String^ String]_[*@3 fileDirNam
e])&]
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
[s5;:FileSetReadOnly`(String`,bool`):%- [@(0.0.255) bool]_[* FileSetReadOnly]([_^String^ St
ring]_[*@3 fileName], [@(0.0.255) bool]_[*@3 readOnly])&]
[s2; Sets [%-*@3 fileName ]as read only or write permitted depending 
on the value of[%-*@3  readOnly].&]
[s0;l288; In Linux the permissions are set for user, group and others.&]
[s2; -|Returns true in case of success.&]
[s3; &]
[s4; &]
[s5;:DeleteFolderDeepWildcards`(const char`*`):%- [@(0.0.255) bool]_[* DeleteFolderDeepWi
ldcards]([@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 dir])&]
[s2;%- [%% Delete all files under ][*@3 dir ][%% folder that comply the 
condition in wildcards included in ][*@3 dir.]&]
[s0;l288; Returns true in case of success.&]
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
[s4;%- &]
[s0;:LoadFile`_Safe`(String`):%- [_^String^ String]_[* LoadFile`_Safe]([_^String^ String]_[*@3 f
ileName])&]
[s2; Same as LoadFile([%-*@3 fileName]) but it works in Posix for files 
automatically generated by the OS.&]
[s0; &]
[s0;l288; U`+`+ LoadFile() functions prior to loading the file into 
a String, get the length of the file to dimension the String 
that will get the file. This is not valid for OS generated virtual 
files where the file length returned by the OS is 0 (for example 
files under folder /proc in Linux)&]
[s0;l288; LoadFile`_Safe() just get the file bytes returned by the 
OS until the file end.&]
[s3;%- &]
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
Trash Bin.√ß&]
[s3; &]
[s0; &]
[ {{10000@1 [s0; [* String functions]]}}&]
[s4; &]
[s5;:Replace`(String`,String`,String`):%- [_^String^ String]_[* Replace]([_^String^ String]_
[*@3 str], [_^String^ String]_[*@3 find], [_^String^ String]_[*@3 replace])&]
[s2; Returns the resulting String obtained by replacing in [%-*@3 str] 
String [%-*@3 find] with [%-*@3 replace ]all the times that [%-*@3 find 
]appears in [%-*@3 str].&]
[s0; &]
[s4;%- &]
[s5;:Replace`(String`,char`,char`):%- [_^String^ String]_[* Replace]([_^String^ String]_[*@3 s
tr], [@(0.0.255) char]_[*@3 find], [@(0.0.255) char]_[*@3 replace])&]
[s2; Returns the resulting String obtained by replacing in [%-*@3 str] 
char [%-*@3 find] with [%-*@3 replace ]all the times that [%-*@3 find 
]appears in [%-*@3 str].&]
[s3; &]
[s4; &]
[s5;:Trim`(const String`&`):%- [_^String^ String]_[* Trim]([@(0.0.255) const]_[_^String^ Stri
ng][@(0.0.255) `&]_[*@3 s])&]
[s2; Like TrimBoth(), it removes left and right spaces in [%-*@3 s]. 
Spaces are chars like, `' `', `'`\t`', `'`\n`', ...&]
[s3; &]
[s4;%- &]
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
ouble]_[*@3 seconds], [@(0.0.255) bool]_[*@3 decimals]_`=_[@(0.0.255) false])&]
[s2; Converts an amount of [%-*@3 seconds] to a String formatted as 
HH:MM:SS.&]
[s0; -|If [%-*@3 decimals ]it includes decimals in seconds&]
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
[s5;:BytesToString`(uint64`):%- [_^String^ String]_[* BytesToString]([_^uint64^ uint64]_[*@3 b
ytes])&]
[s2; Converts an amount of [%-*@3 bytes ]to a short String.&]
[s3; &]
[s4;%- &]
[s5;:StrToTime`(struct Time`&`,const char`*`):%- [@(0.0.255) const]_[@(0.0.255) char]_`*[* S
trToTime]([@(0.0.255) struct]_[_^Time^ Upp`::Time][@(0.0.255) `&]_[*@3 d], 
[@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 s])&]
[s2;%- Scans a string for a Time. Order of day, month and year is 
specified using SetDateScan. In place of month both number and 
text is accepted `- text must match abbreviated or full name 
of month.&]
[s2;%- -|Similar to StrToDate() but including both date and time.&]
[s7;~~~.928;%- [*@3 s]-|String to scan.&]
[s7;~~~.928;%- [*@3 d][%% -|]Found time.&]
[s7;~~~.928;%- Return value-|NULL if no time is found in [*@3 s] or 
pointer to character right after the date.&]
[s3;%- &]
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
[s0;l288; For example `"√Örhus √òrsted √ëand√∫ cr√®me br√ªl√©e`" 
is converted in `"Aarhus Oersted Nandu creme brulee`"&]
[s3;%- &]
[s4;%- &]
[s5;:IsPunctuation`(wchar`):%- [@(0.0.255) bool]_[* IsPunctuation]([_^wchar^ wchar]_[*@3 c])&]
[s2; Returns true if [%-*@3 c ]is a punctuation character.&]
[s3;%- &]
[s0;%- &]
[ {{10000@1 [s0; [* Special folders]]}}&]
[s4;%- &]
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
ÄÉãÇ∆ÅÄÄÄÄÄÄÄÄÄ¯áúÏΩá˜‚Ÿ˚íÆ˝ˇ€‹ÈèÈÈÓ™Ù ˘πÔç†‰Ω˜ñòÅÚ»»Ä¿¯ÊÔÖº…”ÛßôÓóÊü√¡¶¥ª˚æÈ©™Á
—«˙◊ÆÑÇ¬§ûùë˚«‹•‹»ì¶Õö¥ñÈ“§…ì¶ÕŸö¥È“§…ì‰¶Õö¥È“§í…ì¶Õö¥ÈÀ“§…ì¶Õö¨¥È“§…ì¶≤Õö¥È“§…
…ì¶Õö¥È“•§…ì¶Õö¥ñÈ“§…ì¶ÕŸö¥È“§…ì‰¶Õö¥È“§í…ì¶Õö¥ÈÀ“§…ì¶Õö¨¥È“§…ì¶≤Õö¥È“§……ì¶Õö¥È“
•§…ì¶Õö¥ñÈ“§…ì¶ÕŸö¥È“§…ì‰¶Õö¥È“§ﬁ…˚ÉŸˇ˘¡ˇ¥ø˝˘˚˛ˇÿ˘ÆÈ“§…ìÔˆª≤ü¡¿è¶Ô¸ÛŒﬁ‚¿Ÿö¥È“˛¿ˆ
¿èπÌâàÈ“ç§…˚ﬂ„øüÊ¡ºîó˙È«Õß∑˛¡ÛΩΩ€◊£ﬂΩÙ–œ∆ﬁˇËŸﬁ˙Â´“§…ì¶Ìá∑ˆÛÄ¯È∆Ø˜˘Ûß∑¯ªÄË¯ñÑœæ◊è
ˇπ“Âø˚˜“ ì¶Õö¥’ıû˙Êˇ‘ª˛€›€ºıÍœü≠Îß˝ˆÀ∑ˇÀÈ«üºòÈ“Ì§…ìˆ„˚±ì√¸ÁÄ‚ü˙»ôøâîüÉ‚•Ÿ«ê¶Õö¥
åÈø∂ﬂ≥†˛”˛°˛åàÁüÚ—ö¥È“§˝©ÿ§†§…ì¶ﬁÕ⁄≥ˆØçàùóû‰Ÿ◊†Õπö¥È“¥ˆèÙÅ¬Ω∆Î•µøˆÉª¸¯πﬁü˙’”ß¯«Å
”ÒÙ¡ﬁ˙„•í…ì¶Õö¥ÈÀ“§…ì¶Õö¨¥È“§…ì¶≤Õö¥È“§……ì¶Õö¥È“’§…ì¶çÕÓíµí¶Õö¥Èœœö‹è™Õö§¥È“ﬁ≤…à
ˆÈ“§…ìˆí»…∆»ì¶Õö£¥óÃ≤¬ö¥ÆÈ“§Ω‰íë´“§…ì¶Ì•ßìåê¶Õö¥îÈØô‰Ñ¥Èì“§…˚…§£•§…ì¶Õ⁄Àê¶ô°Õö¥È
…“ﬁ≤…àÈ“ù§…ìˆí…∆Ú»ì¶Õö¥ó»Ã≤¬ö¥È“Î§Ω‰íë“§Í…ì¶Ì•ìåâê¶Õö¥ÈØÂô‰Ñ¥È“§§…˚…§£§…âì¶Õ⁄À¶ô
§°Õö¥È“ﬁ≤≤…àÈ“§…«ìˆí…∆»ìú¶Õö¥óÃ≤Ú¬ö¥È“§Ω⁄‰íë“§…ì∫¶Ì•ìåê¶¢Õö¥ÈØô‰ôÑ¥È“§…˚Ÿ…§£ﬁ‹⁄Á
Áº¨∂˘‘«ÇÊ◊øËµ¨˘∫˙÷¸Ω´˛æµì¿Ä©∞∏ë‰‡ç€¬”ªëÖˇ˝ò…ëúŸç éöÇ∫˘œ”€î´ÒÚ§·¬¥ŸøÓ¿¡ÖâˆÁ√‰…„î´
ÜÈ⁄›ºÛäÍôôΩÌù”Ë÷Ì®˝Ó¥Û…€ãªÍü⁄àÃÏÛÖ·±óÌ±Â∑É¯íΩæ∏üÙ„≠ë÷ÏÒ’ßÉ¯⁄·¸Â†⁄∂øóùﬁâèÜŸÉ†—ãÍÚ
§´¥ªõŸ¥«ˆ◊è‚¬áÒÖá˝ËÃÓ¯¸≥Ó∏∞·ÔﬁıÈ´ó‡èÙË’æﬂÔÈò˜Ëñº˘∫≈Ø¸÷∞úé◊ÁäèÂ‘Ñ∞ÔàÍï’Ωñ∫‡ıË‘ªÆ™
≤àÍ€èˆõ–ÑÉõ®¡≥®Ñ∂ö≤ÑŸ√÷Áìƒøö¥£Ü∂∞¬¸ﬂ’˝¶¿éüàû•«˘çÙœﬂâ÷⁄€¿¨´∑Æ†∫ó˝™†√≤ºß§≈ÔîÓâ‡ ¯ü
¸‡‹Üø›◊≥«ÊáÁù∞ÖáÉÃ°ëÎËÏ≥∫º±ùô‹éÃ˜Ì¿õúù€ãˇ¡ˇäÂÉÿ⁄º—¸√ë∞û§Á∑›©›œ⁄Ê«ÕÏ˚≤ˆˇˆ„¿üΩå‰èÇ
ÊÁ™é√¬é·ô≥ÚˆíÕπŒ„ÁßÒÛÔ‚ó¬üºà¶Æ√ãÈÀ»ˆ™ˆˆ«ç °íó∑…áÀ€‘ïäÆ§ºÆÓ„§∑˛éÍ…˚ßöÑƒ€’öÑÑÑÑÑÑ
Á¥ﬂ≥…∆º°´µœ˚Öñêé≈‰™—˚à¿êÄœ‡∫åÌÑ«≥≠°ÈÇƒ˙ÒÄÚˆ∑∫ÏéöØ¡ÎŒ∫€¶›åÕ”»àÜ¬É´”°Ò çÄË°öæ’ô—†ô
Ó‘€‹©·—€õΩ‚É÷åŸ√≤”€ë˛¿à·–íÀÉà†√È˙—Ç¥ÚÄ¯„ò≥≈Ää†ô°ù∞‹Ü∫¨úè∆÷é„®‚ÑÖ‹Ê÷ÜŒ¨¥¨ÏÜèäﬁ±„
á¡”ÆàÆã¥ê†êÅä¯◊ø±ÇÇÒ¡†°˚”ïÙúë‡›Í®≥°ëÑÃ®ß‡ÙàéŸ¬§∫∞—·‰âû∞‰¢¥¯å£ËÒ£ïßñØ¶œ¶Éº¸¢¶÷Éå
Æ∏∆Ñë°»ˇì¯íÚ·ŸÔ‹ø∆ﬂÚüåˇÅ¥·Äü˝¯÷Íòà∞Ωõùµ¨ëë‡”πàØäü¬€ùÿøŒ¸Ï∏íÊì‰¶ñªµ„…Ô˘≥¿ƒÑÅÇ«∏Á
ÄÑ≈Úß§ó®—äÚ≥îàãûŸÑ¿√Ä–êŒ‘ﬁ≈âéâÛ–éç¢·‚∂ı˘Áõæ∫ÕﬂﬂÅË“◊®¿ÑË≠ùø√“˛π¶ìËÕÔË Ú—ÀÁ¬Ö¥ÿ¥˙’
å≈£“¡åÓ∂ê¥ÊçœÔƒÅ‘êÎ«âí‡ê¥˙÷˝∫Öè◊¯˘ÊÈå£à¶ãÆ∆È˜„¥È”ÅÅ°≤‚©Ê†à–Ø¯ªñŒ˝˝Î˛ëÀêáÉàÖΩäú†
Øî∞«ôë£º¿·Ìàºê≥»âÎ‘”Ò‚çë¬Ñà®ñÄçÿá±†¬˙Ò‹ï‰»¨ª–ﬂ°˛†Ï±πΩË‘åŒ¯‘äÎÔçı˙Âœîˇ¨Ö≈ Ïòâá˛
ﬁÌò–µ™¿ÅŒ§†∞ñèêò–Öäà¬˝¶çÁ–±£∏Í†¿ñ¡°É’–·‘≠’¶˛õÁÑ≈äÂÑÚ˜ç°˛®Ã˛∆∆êøò√Òíäˇ‰Ω∆…ƒ™´ˇ∑ä≠
ßàø∑Áïˇ∆ß¯‹˚©ÚÑ‡‹ÅæîÈ›àºùìã¯°ƒ◊éÁ·ù«∑˘√ô˚¥ú§≠Æ‘¶ÍªΩ˝öñÒÇàøÛòÃÇîŒﬁ¢ÁÆËπõ§»¢†˛·Ö•Ñ
˙¸†§Ë“ü∫ﬁåêÇÈÀ–ÊÆä–ÄÅ√Ó∫Åû√¥§ØÓÑˆóÆÓ≤◊®ÏËç™√ò‡Ωıî˜»⁄ø›§£ßﬁ ¿√ÍÑÎÜÈˆñ⁄¿åÌ†ù∆¨·ÒÕ
êà∫Äƒ—Çˆº∆¯ñ¯´ÄûÒ≈ãåòΩÌ““°∞Û ‹¯£ªÇ†¬∑Ô±πä¨”êÉàπäõ˚¢ÑÇ≤‚úøÜ»ÄÀêª∆ÿú…Õ◊ÍÀ¶≠¥È ⁄àó
¬†≥≥Å´ÿ–≈Üì›óÄ‚ÈÙ†∂èŸ¶»¯‘ÊË»úÏ›ÛÄ˜™ˇ¶Û–¨—òÈπÕÓ—ı≠üÅìΩœ‰°êòäœ®Ä††àèÓ“Ë•¬¥Ñ‘¥∞ùÄ
µ∂„õ¬£õ˘¥ÃË‚à∫£Ÿìàü¬˝ñ˛Ö‘…˝èïÏ’•∂‚¢êîœä√É°˛ıﬁèÑ«¬¨ò«Ç¥¨Ïê√Ëñ◊à†‹Ä·ÇÕá∏∞™á≤áîª‚‰
†ëÑà¿ÑäÖìÑ€èË¸ø¢Ñ¿ï∞ÄÒç«äê¢ ∂©Œ∆°√•¿É¬·œ˙ÓÏÊûıõ∫◊ˆ∆Ë≠ø√Â“ﬁ√§£ﬁƒÿäÅ°÷“›∏ñ‡ˆÛ í¢à
êïêê¡†Éáôøçëƒ‰ˆòãñ˝˝Î˛£ÂòÔ⁄¥°¡œ≤¥ÇÓŸæÁ¡¨–ìÜëÅÖë·ÄŸ‚Ñï£∞¸®Ä¿¿∞Ä·ÉÖ–¡…Ï„⁄˚ÔﬂóÅÀªß‡
Õ≤“€Ñåÿ–¬⁄êë…à¢¿©≠Å£‡Ò‡µÉ√√éÏÑ±ñ·†ò°üØ¨™¬â¸‡¡Ë–ò¢©ÍóôûÖÖ€ÊêãÛ≈ñì»à•≤åäèåÈ¥¨âó∆∂
Ñ¡ë–®ÈÏìÉ∞•Ëãˇú”¬˛•ç•ÿ√ ±”çÍ¥∞Òÿâ˝®Çáπ¥‡òéÈÈ•î≥‘•úëπàﬁ√¨ÎààÖüÔ‰¸÷ÑﬁÆŸ›Ëâ¸ﬁîﬁ¸¯õ
ö≠Æ§‡ÄŸ§à†‡‰ëÑÜ¿òî†ã†êí´Å¿∞çæøî˙ˆì∏ø’Ùù£™©’≈ÌäÈ≥∆˜¨¥∞ÄªîÅ»·äâÁóªΩøá›Å‘ûÆÓûŒïÂ
“ÿ∑¬Ô˝ùíÒˆ ¶ôÒ˙÷∆¡•áéµõØä¶óÃò‰†ÖÇ≈ñÈ£¥ÍâêœÄ£ËëèÓ∞‰‚ãñΩ°Å˙ì¶¨¿…∑Õ‰˘⁄Áº≠≥ûÕÊŸœœÉÇÓ
Ω•€¬⁄æÁü≈ﬂøé’ÍóÊìòÎä¨–Ó⁄ÉÎ‘ê≥òÛù…Ì∑Ê∑±˝Ë…˘Ì√¿ô±äâÄáê‚ë™£‡ﬁÄâıœì€‡√±πÉÌŸ˚Œ¥Ë∏†
π∆ÖÉ¶Çıˇ√ﬁœóîé¢™»∆µéü¸˚‰ùÈÔ«∂û»ã˘à≠°˛’≥å“ò≤ºÉàÁıπ®Çæ¢¶∞∏ö‘Ÿäè£¢ Ä˝ôãä≥ÍÕ‡·üÎä´áâ
…ı„à ∞ÿÿ‰Ú£ã°¿Ä–—¿ïΩø•Ñë¿øıÖîàÂÆÃ£Á∏ÊáñƒÄÚ∑Òñ“Ä˝Ãæ˘¯˚≥≈πˆõ–√˘ÅÓÒ€ﬂ€¬›‹ﬂ‹ æÄÆÔûÆÔ
ïø‹›ˇπ˛ê≠∫¢¡ïó˜ﬂøÚåêÄ¯ﬂÁıíëØÏ≠‰’ì€î¢µÓﬂ⁄∑ı˜Æ„˛ôÀ◊¢˝òˆ†∏¬–îú∆∆Ñ‡ùÄπÖûÊã·±îÓÖ«ïÏ€ã
¸‡êÅ˚†—˛ñê‹Üò√ı›ƒ¿à„íØæ˚≈◊∑Üåòå’ò¡È¶àÿ Ã«…Î…Í«¡ÉÙÄÆúá±ªÓÈàÂéÉÁÄ«öÃÏì£∂âëë©™÷§Õ
ëéÑƒ¢‡ÉÅ∏Á√Êë√◊î‚ëàãÇä˘†î¿§Ç¥Ó˝ºçØËÅ∂ïÌ≈Ω«¿õäîﬂŒ∞Ï«ß˜ﬂâÂÂÊ‚Ò∂µèõ˘òë£è íÙØÄ‚—¿¬°
¢äøÏ≠à¶ˆì¢»Çõ≥†∫ìøè∏ÿ¥ã¢Ú‡ïôÀÃæÃ°‰˜ºŸ˛ì£·ÌÖÆ¯‚Ä‚ÖÃà≈∞ˇ√á≈‚ª¥°¿À‡ÅŒÅÅã¿’ñﬂ‹Å˚ˇ˙ç
“˙¯˙√∫˝¯ÜÜ∫£ˇ…ﬂÔóøÄæâ·çî˙˝˘˙˜Â€›ï◊Ô˜ﬂøΩ¸›˝éÇÀ∑ﬂøµ›¸˘˙ı’⁄ÂÑÑƒˇ⁄ìëå¯ÕÎöÅÁ•éÆΩıé˜≤
–Å†‡ı›Á¬÷ù≠ÉÊåù˙êƒÍ∫∞∞ÆòìÅŸÈ´¶£¶¯®£Åó·˙ﬁæ’ÃÉüáá¯ÈÅ∆Ã†ö¶ÓﬂñÚº≠≥Ç‡–ﬂà√åÀõﬁﬁµˇ†©∞
ê‚âŒÓáÁ±≥é¿ÇÑ∏˙≠¿óØËÖù‹ë·Ûì–òÖ◊ÄÊ√€˜Â±„‰ò…ì¶“õƒä‰à®÷ùâÑı‡û¡ÁéêÀ⁄’”≤™õ•Ô¯äÓπ¢ÒÆË
Ûÿßˆ‚ä¢¨«…ó≈î–µ…∂˜‹çÕì‡≤⁄‹£‡ÇÇàï®éäÂ¥°∆®ΩÖ°√÷àñëÜ√Êß©¨äÿ·îËÂ¸õπ¿Üƒ‡–ÄÒ¿õ„√ñëáÒ™Ê
§ßÃÖµÖéî¡é√π¯ÀΩ±¥ãﬂ‚≤êºå÷ÁÆ¢ŸÑ¬‚Å‚¶˝ı˚ˆÛà˜˛˚œã˚ˇ◊Ï∑…√≥∑¡ˇ˙√æü˝˚€∑Ôèﬂæˇ˘ﬂ˛€◊Ôø˝˚
ﬂÌ∑Ôø˝óËà∑íﬂÜ´üãˆÓ¸Ìª¸˚Ì†ªﬂ£ÓâÇáùÅ«ÉÁ∫¶∆êà¿êöò Ôù˝ùíˆ⁄¶ôΩÒÍ∆éöº¥ßûÉ–≠‚›ñﬁò—Ø¶ﬂ©ø
¯¥Ïñ±Éö£ƒ÷Ö—Ñ∞‚ÿ≈å®ÈŸçÙ≠°¯ç»äüí‚á÷”¸úöòüË˘∫êõÙãò˙ËÍ⁄ˆı≠˚®°™ô£≠µê ¡ÄÁ„ÇÑ∏‡˙Â˚æ±æ
¿ÖÔÜÀ‹†≈‹…∏Á§À«¡“≥Úˆå¶≤‚˚òÙ‚ÿ§Ãﬁß…µ£ƒí’™Õ¿ÛÇ√ŒÁ˘¯©Â§ÊÇ∆∑ÖÉçŒ‰›˛¬Ãë¬Åçà∆Å∑ã¢à’åó
êïê†˛Ñâë¢Ëà»°üõºŒ˛úÇò¬≈ÜΩê∂¢Ÿéåóƒ†©™âñ‚Ç‡éî¨ËÅ±ñ¯áÑó˝±ËºâÁ„∆úÅ˘œ£©–ÃªŒØ¢‘Ÿ∫Éé•„
øÃ’›Ÿıùíö¶†ÆÖæ“åàú¯ı„Ãàÿ“¸•Áœóæã†æóË€ﬁè™ö›ˇ˝éèÜÅëàºÖì°ÛÃ˝èÒã÷Ô˝∆ùíˆÜ¶ôÒÇ∫÷º´úıã
ˆùóÎÿÈó‡ßºÔÎ·ùœ¸√¥°ÂÚõ‚ÎóÿîÿÊ˝»≈‘¡Ê˙Ñ∞úÉ¢ã°çêÖ∞ù¡¯Ñ˝¢èúºˇãÇÉ¶≈™ö⁄ÊΩ≠≥§Ù∆èÄÅ∑«
ä¿¨‡µö≥»Œ€ﬁÁ±±Œ◊«ØÑ∑€—äó‘ºÕû∏í·Œã»ÅÆéÀ ì˚˛∆ƒöÓ∞Âˆä∆ÄœÑÚó Æ◊√Ö®êñäò©ÏäÈíÅ°ÕìçÌ≈
Õ«äãÀÙôìàÀ™†êãÇç¯±¯Âé˘ ¬≈ûëäù¶ûÕÍ…ú¢˝˘†¶ÈçÄã≥∂òŸ†·ˆèË¿ïÈÖüÄ¬¨ˆ©ç¶∏à†˜µãÀÔ°´íÉ∆¯
«Úå¿£âÖ˚àŒØÔ≥◊˜Ÿ±‚Ö◊ô∞àÄÉ»∏«“‡Êâ∆∞Òï±°»ÒΩ†Ò˛˜§áÇƒ·ŒâıßÇìÔ∑èﬂØÓø™ﬁ‹˝ÅááÁåÅº•¯¸ú◊
ΩΩØÑ§˝Ô¥√…à◊∞ÖêﬁºñÒû⁄áå–≥‚àæìûˇÖÑ∫Ã¡‚˛èΩëÃ™ñ±±ﬁÏ≤∫∆≥Î∞∂á∑Øª∞ó√≈πú¯≥Ùô·—À—∏ˇΩà°Ñƒ
ˆë≠≥ÍŸ⁄√ùœ•ûÆêéåÆà„≤Ø˚≈èıàÅ÷ÄèÊËØ–‹Ú°∏‡‡ú÷÷ººÎﬁ›øÒ‚÷ Ë˘©âÍÇÑó·çåÅëÖáî≥ÆÍﬁ√˝·µ√ü
åÄˇ»£ÙêπõÛË¥êŒñÉøÕ–ø”˘ïøç∏˜≈√„ñ®€™îâÚıËŒÕÛÜÍ©†‡±Ó¸«—À÷ËòÙãµ¶°√û∂Ñª‚’±€«◊Ì°ÁÁÁø∑
ßˇåÊµ⁄ÿØ≥†√£¿–∂’·‹òÃÁôËÄúõ±¬ÓÀúÖÖ≤éå£ÚûÊ–ƒíôë¶›©êëÃ∫›Ò˛‘Ö¡ãà¨ÆÓ≥«ó∏Ô®˚Ûüà”∞Òı†≤—
ë∑ﬂÖ©Ú“ ñè‚«Ô˘ﬂÒ∑ûäπâ©§≈åö≤î≤Äò≠Äãºë‹ô•ãÒç“•ø˛œÎÔê¥∑≥…ùàˇ∂µŒ˚ïô˘¿ÄàˆÖ‡ﬁÑ¨ˇ˝∆ï‹
ïÍÉåò¬àÇá”⁄∞¢Öè„Ÿ¬ÚÄ•˜œ∞‡ñåÎ˚ÿúÉÒÛÅﬂ¥è§à£¡ üÇäà¶ ˙±¯Ì∑¨˘˛ıã¯∞ÌÙ¯ˇ≠Û‹÷ÉûãÑ¯Ñ“°ùà…
úòæ‰ëƒÓÌåÕÕõ”¡π´ÊùÖ†Ú™ûÿ§Ωéûπ©éÄ‚Æ∏£Ù∏ãŒûÁπ¢Á¿é†¥Ö¿‰∞àÖÏ«∏˝ñªç´ÁÂï£¨ ¨ÿ„øÛ˚¿ã∏ÍÔ
–Áïèº£ÄÇ–ì ™öòé¬µΩÛÅ¿ë∫Û∫ÈåÎ”‡‰˜∆’∏ƒä å™€¯ë˜ƒç≈µ•ÇŸ¿—Äª˙¥˚‹´≈÷Å¢â˚Û∫àØ©⁄¯Ê±öË‘≥©
”‡ò€ÍÎîòõõ≤˘”˚æ¸Æøè˝º„†ç¬õø”úë™‰—ëÜ∞Ò¢Ûí•äÖíÇ∂Ω∞ÚÒ˙öˆØ¨°©ƒﬁ‘ŒæŒïËÓã¶••¥ë›√∑∏∏˚
ò–ì ∞êµ›ƒÚÑÜÃïº˜ÜÄ¢†¸òπæ˙ÎÔê¥∑¥€…àˇ™µ≠¯â¥ÍﬂÉù∫Æñ¡•£„Ô‹ÏâÙ≠‚Å±Å◊ç–∞Ö≥Ã‘‘Á£Å«•»ª
ÛßËÚ≠ŒîÈÙ∏âÏê©ß˝ï∏⁄õò‹à¡ñ¥—≥ä§àê∆©‡ÒâñãÇîƒç˛…ÔêÚÙ¿≥ó£÷ÄÑ÷∫¥˚Øï·œ∆≠ù∞î∞ïû§âëä€ã
ú‰·›ã§‡Ãˆƒœ’√ªæ‰Ó•¸Îâﬂú∆®ÆŒ¢Á¿äΩÑﬂˆÎ¥çË≥ﬁ°⁄íÀáàã≈çØÊ˜‚‡πü©¸–ÊîËÁµØ–·¯≥êÇé⁄≥é ô‚
∂¬°√Ñ’¶üÅ’¨¶Ñó∏Ã‰ê‹ò≈⁄·À¶ÿ–¯˘¯ÖœÂµÑ±õ´œÇïÑêﬁ†Œ⁄πã‰®°°¡√¬›õŸ⁄Ò§˜Ωô‚ê¥ΩçÁê®æÄÌ›—å†
≈‡Ì‰†¢ˇﬁö≠ïøÔˆÇ¶˙Ó”‚ß´Ä≈”≥ ëƒƒ…áÉ€«€ﬁÛ∑»Äáëª‹‚Œâ∞¢ÖàúÇÈ´ÓˇÍíé‡£ºá§∑˛é…˚„Ôìå¯ÁŸ˚¢
ﬁü‘ÒúÿıÉÇ∫óëìΩÎò‚ ÄìÉÔ≥„Ø…Ïâ”Í∏˜†à»ÅÅÖÓ©∏ΩŒÍâ⁄•ø≈Á¿Õ¬≤ôü∑¬Ü¿˘ÉÇƒ†™—ì¶‘—ì„„ŸÊ⁄€´
ÒË∞•˙˛‡˙æô¸‰Ú°íõ°°˛ö¸Ó⁄Ú√íÿÙ·”∏ Ωç‡∆…ÓœúÊÖ≥¿àÿ˙˙¿ìÙË´√õı≈œÓ¢Ù÷∑îÅæÇÿ¿‰Å±≥≈æøãæ— 
ÇØõ⁄Ûê¶î∆∞†∏≠§÷ÀîÛË–Ç‚≥ÈÄéåà±±˝Üï∏†ËßœÑ™í≈ãÃÖŒöø¡ÛÓ«Áâãì‡‰ÅÅºÅıïÌ∏©ÍïÖÔ¿ØÈôï«È¿
˛Å˜‚ﬂÆ¢òùìË†”çÊ§•ÿ¢∫Ï˙¯≥˚æÅÜ√†Ñ∏ê≠¬÷ÖÑ˚ûÉ”§¬Ç˚ıÈÒˆ©Ç§†ˇ£–ÍÅàã€Ä§…πä˜€®‰—ﬂŒ–Éç¨
ªê≤Ôñ–êï‡≈áéüÓ„ÈÀ‹Öàéûƒµ≈©…ù«Û˙∫Ô˝ùíˆˆ¶íôÒœ≤›ÚÄΩµ§‘±®ä¬Êâíé≤Œ∞˛‰á∆åÔ≈¿ÉøÅá¡çë˛≥
˛∫âê≥˚≤ª•ßÈÎ… ïô‰·¡©ßπ‡∆Ü¡”⁄£√Ê⁄ø©‘ΩöÑÖ¿Ñ†„†ô◊Èòæ»áë¶√àŒÙ⁄≥ÀÛÅÜù¶∫ø≥Ó“‡öÎÿâê‰Ë
¡¯¯µ≤ÇÎ¨ÇÑà‡ä›ñ∏Ã´ﬁÕ≤“ó∑‡ç∏¥ÿÖ∫‡ôìæº∫ÅòÖ†™ˆä≈ìã£»àò±¬ÏŸπ ü“ÿäåËÄ§™à¿ÑÌËüÄ¬®ÅÎóÍ
≈‚»≈ÅëÅ„‡î¶ÇË¡‰ã¿íÇç«ÇçÎÂ«…ì√Ëù¥ÅÊ‡“§ÿ´¨©Åå‡á¿…∫Æ™°ÕÀ¡ƒ‰∑•ƒ‡ñı”„÷¥ÿï¶´é‚¿ÇÓâ”˙®¡
ñ˚Ëâ—˜§Õ¥É•ÔÛ˝îä«®ëÊ„Ù◊‘∆…„öªÈ®∫∏ä©“ØåúâòÒµ°ö√„˜ã™‡«¯ïÚ°¬˝ÂÀ›¢ó‚èÁÆÿ˘…îœ∏Ûª¢π„ô
óË≠ø√“˛ó¨¶ôÒœ∞˝ÚóÄıÑë˛Ç∆¡–“üºπ∏˘É…ù®¯—∏•¿à‹Ëö ùÁ¯≥ºŸ»ïı“§©≤‚ÂÄ∑ÓèÕÔ≈°öÄ–Ä·‡£‘ã—
≠ƒ⁄∞ãÇäÓÍˇ∑‡ Ôï˛íÄ–ÙëÜß¶Éá…ıÍÄÇÃÒ°πÉ‹»Ÿ¬ºÈ˝º€≠÷π¿ÿ›•¸íäŒàﬁú…ã‘ï‚ë´ﬂ≤œà‘ù˙ÒÙ¯ö◊
óÁ‡¢π„ÑéÙºﬂ∞÷ˆ∏éèÔà™Å¶«‘‘¿Å™†¿ù„±úØ¬ä∞òÖ—⁄ÍÇ√°Òõø™®ÔÍ’¥∆î“°–äöà¯¶ÄÖ‹§‰ƒ†ﬁ’÷∫Åá˚
æŸ±∏ü·› Âº’¯»ÈµäåÕ÷£¢¸Ûöâª‡˘îã§¨˜Î∏£œƒΩ¯Îîé¸ßÒÃÊŸ°íÖÇÎ¡∏Ô‡è°á◊Â÷¶»Û„Öâ£ÉãÅÉ®‡ÄŸ
†ò∫ªàÊÌÅ…‘˛†ª«˙ÎüÔê¥ﬂã…∆≥¸èõØ„ü„ßÑøÄë‹ïÿ–∆Áæƒ≈àÓ‡∞ˆÊ›¶∞öäµ©Äÿ¢±Æ£®ìûê¿‚†ÒøéÓˇîã
äì¶†ƒ£ÿÑ’ÁÇáôìÀ„±ò‹¬¥–ë¥‰∫°¥Â‚≠å”‹õ∆¯Å˚Ê ßÌ‹‘•Í¶ÉØÅ›—±øëÛâ†ƒﬁ€Ïïïº«∏ÇôÅÌÎÁÃí—∫…
“¸í≤π—èØÄê¶‡ΩØäì˘˚Ò—πÜ∞ÇÕÑ¨ÿ”Ï´⁄ÏØÄ¡ÇùÓòîÿ‘äÇ˘îêÉÁÖƒë¡Ñã¥àÖ ù≠˝Ω˘≠æ«áµ„ÑÒ ëìÒÅ˜Ø
Ù⁄®î”–Äß÷¬ˆºŸÅÜ•ñã∑ ÖéóºÈ™˚ØÑÇñ˚„ïäøû‡ˇÅê¢ö©◊Ç„áííëÔóÓ˝ﬂ∏´ø®≥«ÚÈØ‰œÀÊÉ•˜¯˝ÿÆŸÒ¶∑
∏Ω¬€§∑˛éÍ…˚ùô‰ƒˇï¨·∂ÁÂô∆Æî¶õÑÙãÅçÆ«ÙÆáÊÌ‹±ß«∏Á˛∆ú÷¢∂œ∆–€¡ç˝Û∞ıûÉ¬ãÌ∑≠‹Ω“ô≥≥ÖÆû‚
¬Çàî†ﬁòÿÈ¿Ó¯É‘˛∆òÅ¥€ë…ÌëÇª‡Ú˝õ´ë¯™†Ã‘©œÊ∆—¶œêóúº≈ﬂ¶úÛ«Ëõ‰Îå˚¬Œú¶Å¡≠°≤‘ã˚îœˆùﬂäÍ
ÜÁ‚¸ñÓê–â‚àªñ§≤€•ßÈ´ªÂÜËÈ‚§â§Äóª¯˙§‡±»∞Äè∞ÇÑ·ÄƒôÊáâ≈¬Ñ®©Óû ˆ§¿ÎéÂîâ‰îí·ÄÓÊ¡èé›˘≠
Ä»•È´éÖ˙ˇäÜƒ∫íÉ⁄ˆ¶‡ù∂–÷Ã≤æ˛∂ÕË∞§©Ë†Ü÷π„àº›¯Œ’Ü˛Â«ƒ≤®Ò…Ú¨¶Æπ¶Ø–ê»π«ßüø‡Ç’Ì€˘¿‚£±
œ´ﬂ◊∏˙ÚÏﬁôÙÍ‡…œÒ¨ÏªºΩñó¯ëáŸ¶Á¬ƒçà¯¡Î«öπ–©ÎŒî‹‡„ƒ”´§÷Ô˝ùíªˆ˚≤…àˇ¡î”Å°≤‚ë¶ –ã¬˙•ÑÑ
Òë Üì€ëÂ·ÉÑ¯°º∂Ïˇ√öµç⁄ÂÂã—‡˛¥ôµá¿Óä‹ñÑÇ‚∫∞¬ˆÑõ»·‘ﬂãµÒ–∂ûÎßò‘öà©Ä·Ä’±Ñ¨‡ÖÀ®ä €Ï¢π
î”™§Ú™úºŒõ∑ò∆ú¯˙Œ™íπé—œ—¸¥Ó—«˝˘‘⁄†àöæ›˝πµ∏˝»ﬁ›ßÆÍÓ·∂Äí∏ª·ã¬âªùØ€”≈§”Êõ¯±°ÈŒ€‹Èä¨
é“ååµß⁄◊“ƒéöÔπ¡Çå˚Ω“∆âﬁè≥èÉÚ§˘¬Üˆ›üøçﬁ∑†ØÓ∞˛Ë©¯ﬁ∏¯«Ï†ƒ≈Ö…´ãﬂÍñÈ˚ëîªÚ„‘¿πç•¨ºÛ˚æé
íüŒÄì⁄—æâÍ¥üß¶¿∏≠¶„Èÿãë¿ﬂ∞Áœíü«ÌßÍ˝îù˛ìÔ•û’ÊÈä—ÅÄ˝î›ÇêÊÉ≠ì˜í¶úâû±ŒÉƒ‡∞©Ù–∞É‹†§Ö
ÈÂŒ⁄˘±›©Û∑˛é…˚›ôŒ‰ƒˇîÈÅÒ–àì⁄“ËŸÀ–Ñà™´èÿ±Ü©˜Í—Çœ∑‰´î‡ˆ˘‰Ø∏ƒ®·Û¶±ùºËàœ≤Ò·µº∏‡ƒ¿—≠
ù⁄±öò∆÷Úîë¶¬£ÂÅÒ∞òáœàæÓ∞¬¯∆…§œúÛÄ»˛ñÛ°ı≈»ªÍË≤ÄÓº›º…ûﬂ¶Æ∞∂∂ù£∂°‘ƒüË´–˝◊ãæ°ú˚˘«ÂÎÎ
ÿ¯ê∑ÌÉÖ˜Éàá¶ÇÇñ˚õÿﬂ©ª„’¯é¡»∞Ç‹àéóã‚Ò›ŒóÕ‡˙÷¶ªØæ¬∆¯∆¡äˆÔüË·ÕÍÉ®èÃ•‡°Ä¶ØÂä◊ÙƒËõëä
ãÑÉÌÏ´¢¸‡∏àﬁ≥ÎÔë˙¸Çî¥⁄ßï¡øíëú–‚üúÉÅ˙´’ﬁ•ù¯◊„ ü‚´æ—  ˇ©ËÁ©üá◊≤ÔœÍ›∏˚ ¢˛‰Ò«“±—†ì¸
áÉòÛ–¿ÆÏÙíÙ∂‰äÈ˜∆ÌˇÔÕÕÂƒ˜€æåˇÜΩÊ√á¯îêœ±°π◊‹èÄÄ€Éò£Ü∂∞¶åûùûô±ÔÀ∏∏≤—É«åË»≥çÌØ‚Î˘€—
Ã†˘ÈßåÜ Ç¸èÑí¥”Ùß∂ªóù⁄ç‚ÃÓ‡ê±°¥ñ°ÂéÛ≥∂Ïî¥áÄÄóàÒ˘ä£¸ÊçüÛâÃ‚æ€ãíªÙ≠Èõ•˜ùÜ˘¿°˘°Ó∆«
ó‰“Âù“¡ú´‡•≥º∑àâ™ºìÖòÄéÄ‚–‡ÒÄìû¯Õ•ªä§Äπâ‡ó«°ïô¬ØÖÓÒÉÒ‹ŒÛ˘¨°ñÌ€ßôÒù”Ò•ØﬁÒÀ∑ßô•˘†ì
–¬Â¡íˇ’ñÔ¡‰ÌÛ´ÌóﬂƒƒŸ“ƒà¨ ÏòœëØÌΩﬂˇ¶ëˆ˚ÙÑãÂ••üï‡öóÏ™Ø¿¢©Ä‡ùÎ≤â’ÊáÓò⁄æéÛœﬁ⁄•√òÎÒ
ñÄµƒ∫ƒèÎÖÂ®æÚúﬂÛÈƒπ«§Ö«≥ëÄ¯ﬁÖ¡Ç ±·¬äﬁÍé∑ÓçÅﬁÆüıö¡ﬂóÈøÔÔÌ≤«ˇÂôÒöüÔ¢ëºΩ÷ù´˘èÂ‘‘•àª
¡˘ç€Œ±¨ß·ãœªö∞‹£·√” ‚Î„Èû∑˝ò»û≠ÑÛﬁ¯≈à÷œ¯ÏÃ·«ºÖ¿é¬¶ª¬«ÒÖöèäÒ∑¬£Œ¬Û ñû¶ä‡ÎŒ¢
¸πü–Å∆Ÿ«¯ÛÇá‚à÷πçÿıØÓÎ’˚ÿÑ°ùÓëé˜ûﬂ±˘õ“ÖŒÕÙˆÄ»¢˜Û√¿°Ñ–Ü¿ÊÂ”§·œ†¡ÁÌîπ£æé≠‚‚¬ß∞·ÛÊ
œ◊Ì⁄•ª©£≥¨√≥Ãê≠¸⁄ÿ°êÅ£ííßö´—∆üÇà¢¨Öãú¨ÿ¸Ëúë‰‡˝ﬁÛù¶ÏûÑÌ¥ÓêÛ«˜›ëùÁäË€ë‹ºéÏúûá∑ùëù◊
€ÃÜªæ√Ÿ°’¬Åﬂ““˙ í∑∏¢ñÁÃû˙í∞∫íÍÆÂ»¨íøÄàÑ∞ÄÑ≤¢Œåÿˆ∫äÁèÎëƒõ—º£‘™ñê≤ ’ÄÀÁÑ≈êéˇûÈÛâ´
˙∑’‡Å¥≠¢´ÅË‡ÛÁ‰Æü¶ﬂ≥Ωµü≈À˚ó‰ƒÎ˛à‰›ÎÅ¬Öü™‹˚ù’£àßØäçò√º±ıÓ≤èœ÷√‡¿ï¯Ãª≠∞˜Èÿ√⁄ÅøÛúƒØ
óÓâùáíÍñƒàóÍê”∏√∞π™ç„ƒ±¶≤Ü¿°©∏ïêí˜ô–∂∏‚Å¢ÇËÉèÖ·É¿ºàÑã‚§Ÿ Öç°„‡¡ùÚ”ìüÃ¿Ωœ„∏Ûœ√·À
Â–˚öµÄøΩã–ƒëúÓ‚¿£∏àéîƒ–é úÖé¢Ç€àπÜßÙ»ÊÒ¡‘Æ˝Á‘â¶˛êÒÌø¬É◊çºôõÆÕìÑ–ÑÏπ¬õ«æﬂÕÉ”Õ˝ìÛ
ˇÓÒ˙ˆ·Í¶¸˝ıÎ¿ºÁ⁄æ∫ÚÔù∆ˆº—âΩœÍèéåÖô‡±ÂäÓŸé√¶Éﬂ∆Œ—–ÍåÇØ∂ÑΩ™ßî“–⁄·…ƒ⁄ø¡üøÒÊî≈‰≠÷ï
Éö≤⁄ŸñïÑ≈ë£ ªˇÚ¯›Öˇ†ÁÔÎ«˜´°ÉÑƒÔﬁ∑ﬁ⁄èä˚◊„˘ƒÎ˛à›ÎÅÿ¬Ö’ıòÿ°ƒﬁÂﬁ±è‚Êø°ñÉ ú ô≠™ƒ¿˙ñË
®»”©è»«§Éµ´è˜ÏÄ∆¿–ªù∫íÀáøö·ˆ‡Ó¨º¿ÉéÁ…£©ñÅïéíÃáãó¿†ÇŸóÇÇìöÀÍâ∏¨òÿäˆè√ ¥Õõ¥òç‘≤ÆÄé
éﬂøÂ˚Œ≥¶∑‡∑ÿ≤Ùˆã‘ºîÑ°—¨ï·≠ìÇêé•ï±ì®…–πˆ©éöû£ÉÄÑáîëÍ∆…… ©‰∫’””øÚñ¸¢Á•†ÅËÄäØÈÄß‡‰ˆ
››¿á†Ÿ˛∫¸·ªﬁÿ∂õ¨∏ª´¶À¬äÈ¿ÊÒıÊÈ˝≈∂‚≥Ïª˜›·ÈªßÊ˜¶Ìæß¥∫Ôà‡±ıàí±ü˙◊ˆœ¨ÃˆØ’ëﬁπàÕÚΩö”Á‹
⁄˙Üµºï≈ïÆàãõÍÇö™£≥∂Ã‡Òâü∆’âÇ≤æéñÚå∂üÂ∑ÿÁÁ√ª‰£Ω˜ﬁü™øµ©ÚÀ¿ﬂ¸ﬁÌ≠˝ËﬁÏû±‚èéã◊©¸ê˚µ≥ÓŸ
Æõ®æç–üô¢¡–Ôéèêä¶∆òÒƒÀöÂäâÄÄóéÀÕ≤ªı˘ÛÑÛ«˜Æ‹‘Á’£ˆà¸”Ä°ä∞Å—ÏÄèåû ÌÉπ ˚¯π˙„åâÎ´Ìî›
É˜··á¯¥¯ıê»Ê∫ëª·‡§í¿¡≥ïÄ—ß’ä©›ƒÅÇéﬂï∫¸Á∫‰¸”ßí¸˚Œ£ıàÀ ¥•Æ‹≈±¬ó¿óÊ•ä¬¯ˆ§ÖÔÑ∆ÉÌî÷Ö
ö˜üÉÑ¯ï¶‰ˆû”·ã´ﬁã¨¬¯°Äç¯ÅÀùÔ°ö∏ˆŸ¿∞≠ΩÔÕÌÔ∫¢õáæìÕµπÏ∂≠≠ôóπó◊ñÊÁ¶¶Äß∆««ÜÜÜéÜáÜßÁó÷
˙Õñ›˚Òû√†„˜ÖèΩ±£ÆˇÚ–ü∫ÙÁõˆΩ…Î°Ôı°˚ËˆÌÛ…Íãﬂÿø›ˆ≈≈ö¡ˇ£ó™¨éûÃ¸É∂Ωäà¨Ñ©î˙ä›Â÷¿§êì£
†Üê˘∆¯üÑÑÖø‘ûÅ∫ØÃ˘–Ó˜ÌÔÌÏ«ãÏˇº±∑§˛≈ˇ˝˚Õè°±ﬂ–†Øú˜§Ô·Öá…ÄÉ˙¶Çò ºâ‚∫Å«ì€ëŸΩÒà†êê‡≤é
·®›ûìÅ∑û„Êπ¿≠¿”¢Úå°äÄ·â„∂â¨£‡÷Œ≥Ü√›ÈÕ—°≈âœ£ÊŸ¯°¸Ç∏„ûöóÄàÑ«êŸ´ûﬁÛˇÒÀ„åµÙ£ÏÓü¥·√
◊Ω¬àØƒà¶†Éè¶êëƒù‹≈ê¥ÒïÅ·œ°ΩØÿå÷©”≥£ÏÖÄÿ§°áŒ‚Ã‰âÔ‚Õ®ıﬂë¥ÿ—∑¡π”ñÂïÇ÷˘‚ƒ”¿·®ïáﬂö¥Î
∑éÔúƒ∂éÉ˜ÊΩì’Îﬁ‚¡ö˘ÊÓ˛ä¿–∞∂º∂‘ª–˙„Ë∂¥Î⁄öµıÌ≠ÉΩ›”ª„£ÎÀãõ÷ªÎÆÌˇ‡ÿı…‰éúû∆éº™ÿ∞ÿﬂπö›
πﬁòöôòúﬂ∂Ÿ≤ÏŸé¸ªÛû‰éÂ˚∑ƒÕ∆≠ÙÌ¢†ùÿ·√ïœµÒÌ∫…§ïŒÑ÷’ú¬´Œ¨‹¨ÒÚÌ®√µ°çÃ©∏„ƒ•Üè–®±îÊìä¢
âÜ«Ë¿√ˆ¡Ò‘Î˛ﬂ§˝º£Ωµü˝Ïˇ≤•å¯ÕèÒ–öà˛™§¬¶Íÿàë√·îÑË¿¸∫Ï„æ£©›˚»¸æ∂ÔÉï©ˇûÅ¬ï…©«Ÿ§ÀêÒ°í
∏ëçø£‘Æôï‹Øƒë‹æö÷ÓÊ≈Ïˇπ‹ìÎò∫Ú€∞◊™œÁ©ËﬂÓã»ÒêêÅâ∏ÌÙ¿äœ∏ƒœƒ¿òÇöˆ˜ÆÙ¥Ö’‚‚Í—ü‰öØ˘Ò¥™
Ñùƒ¯¿Ûá†ÿ§…‡äÈ∫§Á‘©› çÇ˜‰≤ù–û›“å™†Úô¢Ôû‹ìª˘Ö∑Ú⁄‚◊Ïî≈®ò˘çòÿ°¯ ã”Ç√≠ŸÎÄ·ì∞È•Æ…ßˆº
©…ÌÁÿÚ‡µÂÌ∫ñ∑ˆÊñÓ÷ß¶ß«á˙áá˙åÉ›ù˝Å˙Ê√ÀÕÎ›¿≈˝≈Ò√Ââõ°πˆ∞≥ıˆÅ∏˚˝˛ÚÎ„πÂ‡Ô˜ÿ·º⁄ºÒû∏√ªŒ
√–≤È˚¸‚⁄£®ÎòúöùŸ˛≤≠Ôπ∂‹ÒÙ≠˜ÇãË Ó¯÷ÅÖÊ¬öìäã÷‘≠˛ŒÀÔ¨£¯‚—åÖÙïÌ¸ïﬁëÅÑ¢™◊ØÔ·≠ë®«ÎÅ¡Ù†
˙¥ÿ√®◊¸æ¬»˚˘˚Îø˙ÎÿûÇ‚è àª◊¸êá∑¬⁄âçË¥™ËÏÀ…ÖÇÍÇÑËò¡∂≈°¸ÿºà¡≠ù£†Ïﬁ¨Ç”É”ç¶Ø£‰ÈÅÖƒŒÃî
‹…ﬂﬁ¬Â®µÄ‘ç˚∑Óêà ÁôÄ©«⁄áµı¢‰âó’π˘¥®∆éÓé®üí∆Û¸êàÆ˘˛õÜ∆ñ‹›û›ﬂÇ£»Ú ·ö±¡ÄÆ¯∏æˇÚ£Ñ
–ëºÑ”ùÂ∆ Ù¿Òñ¸¶‚ëÊæı˘˚Ë£ùìªÜ—Õâùƒ¢ÛàâƒÁ‚£åÄ◊√áÖëê¨ıˇæ˚˘´ì…Ï∆êòŒ“˜ÎŒ‘⁄°‡öØıìîâ…°∆
íñÇëåà¨ÇÇ¬ÏŸÕÉùÏÕßÒÎ°˜Ì˚ÄËÒ˝˚ÍˆÒ¯ÏÙÑ––£´ ∑∂‘Èõ €ÿÎæµî˛“ıóÒœ≈ˇ˝∑–Ú˝—ﬂÙïÔáıöÀÁá⁄Õ≥
º√ˆÀí√æ˚íÙ‰∏∏ÒÌù˙Áó÷˙∫ª÷õÀäåÌÌ£Æì”Àõˆã≥î‚πâŸ‘öıøÊÄ¬í√ÅÏï«Ç˝ø¯¸œÖ£ÿ¸·Ï˛à†∂◊®Ÿ≈≥
ßˆ®™Ÿí¸ˆ≥·ê˘ëÉ«Å´ƒ¨∫‚ÎÖÁÅ•π´•¢ãœñ ÀàÁÈ†ÅˇÒªµ∑ˆ£è¨ÕÇ‚ø·„ª≥ëó¥Å⁄çÃß¢ƒÿÇ±‡íÇáú·ÊÈÓø
Õ∫‡è≠ú·á˛’ÍÔ Ä»·±º˚Çƒ¸Ï∫£ôª—∑öÅÀ€áïø∏˜¸Æ†…â•¿¯úπäÚÓàûùÖ»Ë¡±—î€å∫Ñ≈õçÀû¶¸é √Éüﬂ‡
ıÉﬂ›ñåø§ÖìÒ˘†¬ΩÅ¢¡ıàÊÑÑïÆ∑¥«ı‹ôŸßñ¿ÅÂ∑‹ï´ú⁄ „†â”ÎﬁÔöª∏âû∫üá±àÿßàèÇ√á¸¶’œÊ˛∑™’»…“
â√ä‹•É˙⁄¡‘™ÈÕëÇÇ“ƒå»ŸîÙ”à÷·ç¬Üéìøñ˜Œ∞ı»è¿Ùÿ∞Ìœ≠¨ÛéåÕåÙ˜ˆ ˆ∂˜∂‘ı‘„ó∂÷óµïøµ´©˙À≈ªø
ËÅù ˛ÜÄ¯¸œ›Âª¡ÊíÔôû∆’±„÷ü‚Ëˆ˙ú˘Ì¥˘ÌÕ¥ª∞∫ßÿﬂØÔÓ®≠ç˙≈ﬂ⁄¥≤æ£µ¥˘‡πâ⁄äŒ‚Ê‹˝§ﬁÜ≥ûÉê—√¬
¥çêòë‡ôÓì¨ßò‡ÒïµÛ»á∏£·Èêú·ˆ•∞¯—ºê«§∂üÏ‹≥¢⁄¥©òë©ü¶ﬂ≥Ωµü˝˙ÏœÇ‚è»à´◊¸êßÏ—üæ»Ãª‹ã™›
—ÈÊÒÉ™¿√º¢ùåÇ¡Ã««‚ˆ¸ôÿÌúÉ†¸…ãº†∑îÅª©ÇÀ€ÁÈå—ÃıÑãƒµâêü∑é◊ùüÑÛ‹°÷¨ô“∫åÍªÜü≈πœí àë
„°§ïÜÂØÓÛ’„èºŒ¨Äºë‚¥Â˜ªÔÕﬂÔ®Ù˛ßó–Åáé‡‘ö˘ÏØÕ˚¨Æ¸ù©´¬ƒ•ßÒ£Èéﬂ∏Ëﬁêüˆ‚‡É’û≠Óé“ñ”¥Âà
òî®®â£ç◊êÅù÷ã•∞¢õµÆÇ¬•ÇÇÓãŸ€ö⁄∆éñª±Èàõè˝ıÎ˚ÆÖÛÓÙˇ“˙Ë‰Ï¿¿¸‡œó¡ÿ—§∏Î¨Ô≠˘‹‘è˛°Æ¯Ô’ü
Â˛‹˛´Ç¢Ïﬂóˇ€˘Ø˙√™˜√≠•”∆◊∫≈¡∂”‰◊˝⁄Ùˇ¸¯¿ƒ˝»†Ä≈–⁄‘¯“›⁄˝·œ∫èñÜ·±…ÇƒèÃåÄ¿û†¶é¡á‰™Üê⁄
¢∆†éëì¨˘Ä≈Û¨çöñéß„ãá¯ﬂŒ≤≤ÈÍë‹å†î›”©◊≤±öŒÿÂÒÄƒ∂⁄À·Ê˛˝˛îì¬‹ñ¿ΩÊ˜≈⁄€œ€€˚—ÁÏ⁄ﬂÉëØ˘°Ç
©√◊•∆ÓíÊ’îÎ‘ÇΩñïÔ”çù™Öö£ñè∆ÉÑèÑñ◊íèÕü„·∑ÌÒ‘Åõ©£™â†™®ßØÑÖ£‡˙÷¨ã°∫¶ﬁ‡®·Éí∆–ƒÈ∑Ê¯
ã®⁄ÕöÅ¡ù≤∂ÚÒƒîü•€«çµÚå–Î¯ëœôÅò¯ˆõ˝ÍÅË•˚íÛØù…√√ä¬¯ú¡®ÉóÆÉÉôÎö∆ÖÎåÄ†‚®î√Ä…√ﬁëæÿÖ
Çî“Ê‘·Ò¡∞âäàçœ”¡È√à•—§Çà‡Ç¿ÏÛ≈√ùÂ‹ºâÌú˙◊˛ÏÆ≈ÕÉÈÂˇç—È˘Å·—ŸÆÆ„ßÆ’◊““◊÷€“ÿÒˇ±∂‰◊ èˇ
∏âÙ®˝ÂﬂÀ˚˛ˆˇÀﬂ˝©ü±‰ØÜÍè√˘≠•ìÜ™ôÓˇ∫≈ÅÊôﬁÊÄ·√√◊ÀÌÀ¯Õ≈√Ÿß⁄¢çø÷˛˙ÔùÕ‰ı√£ìÀõáƒñóÖèÿÒÄ
 ◊•Ä¬—ÄàÖÇÚÉ“Ëÿ¸Øåâ∏™Ö¨«ÿ‡’‚ïŒ ¿©É€ƒÖ∫≠‰≠‰“œäÅ°ä˙…·˚¨◊¸æ¬»˚˘˚Îø˙∑¢˝—ôÒöüÙ¢∑†ﬁ—àØ
æŸ ∑ßÊ÷äûâçöï„∫Ë≠˚úﬂ‚Ò∞€ÄÑó©ÒÿÏúˆ√»ßÇ‚† ôÇ¸¯ÇéÜæ«ŒîÈÄëË ·…ÇÍ÷à§â˜Ùë†∞áÂµ∫¬∆ÈŸ 
èœ¬— ç≈ìÀ¢√Ü«ë¥—ñ“√àØﬂ‘Åúù≤◊˜ÄÜú‚…ìÂä‹Ç¸î¬∏§Ü‰œﬂÓ˘ìóß¿áÿâ≈—∏∑ü∏É¿∫®Ë¯‹Îﬁª–ƒãèƒå¯
≥¢¸ç∂î¡°î°Œ≥Í©˙£ã–‡ıƒÃáõ˛’›Á¢ÂˇíÊ’∫∂≥∞∏≤∫ﬁÉ°ÄÆç≤ñˇÌçïÕ’ÀüÎ ﬂ◊˛˙ÓÎÈØˇ—ÚÀøøó˛Úß“ﬂ
Õˇ‘ıÒø€ ÆˇÌ¨˝∏‹÷Œ∫ÆØò∂÷œìı÷Œö´á⁄âÀçı≈ÕÂü¯ö ˛≠ˇ¸◊—⁄œˇ∂¥µè†ÃÃØÌüõ›⁄â±œ…©¶ƒ·ËåµÉ¬’Û
™’ÈÁ¥Ùúÿ°¬ûù¯˜éù¯¥’ÜÕûﬁß‡¨¯¬ÑµÇ–’ÍµøØ“˛ﬁˇﬁ⁄è˛»˛–ıå¯ÕèÒŸ∂åÈÍ˚µıß∑ﬁë•£≤ö∆”ΩÇéé¯∫£Ä
ÊêÿóØ¸∆€°ç¸…úˆØ∂¢ ‚›Ãº˙”√ïíË¬ßü¢‡ÕÕæö∆‡¶Ë∏¢ø≈¶¶ôÃ‚ìœãÀí‹œóâûÇ·ƒ°‡Ñçâ‚Ã®Áµ©Ôæ‡ßæ´
ÜºıµÀÙàëü›´à∏ê£îÏä•ñûË£“ç¿É¶ó¬°πΩˇññÛÿöÒŒÁçÑÃÆß°óê—†™ âêƒÄ¢ …ÿÿÂÑ†‹Ò“õÍå⁄éÉõá
õﬁı›ÁíÂÔöÓÕ∫µ∞º≤æπ∞∏ÿ›’°©ÔÔÓÔÆÌô≠´Ô¨¸‘”›¸Î˘áø¸˛˚€ü¿•ø˛πÎ¸˝ˇ÷˛˛•£≠Ú˝◊˝—Ó˚ÈÑ°Ú∫´
∂˙æØÓ¶´Êü§£≤∑©‘◊˙›‘ﬁı©≠Ú‹”À≈«ûŒ˝ﬂ¯Ã‚“éìÈÒ‡ Í¬ûƒäâÛâùûç•ä€ïˇ≠›ó∑ªîÚÊ…¬Ÿ§ ‚•©ëƒ¡å∞
´´ ØÍéú–Ÿ⁄˚ŒØ˘˝ëÔˆÛˆ÷˛ÙÔµÿüóëØ˘°Âä≈öÊ°ß∂˛É≤™›˚’¶ƒÌ‚¥Ÿ¡˜Ö¨çÍ‡≥ıäÉ˚ÊÏÙí⁄…é¥›≥ù°†ÃØ
±÷™‹®ﬁıäÈ∑Á¿–¯ö≥ﬁìÂ°‹è∆õá≈ƒ„˚ìöíˆ©˝˙íö¬«„ô¶Ÿü÷•È˛Ã‹˘Ìó‘Â∆›‰ÏÜ¢Ö¿≠¡…Ï‹ë…ùˇÖœéË‘ÉÉ
±€ﬂÛ£¶Ëπ∏‹ÈèûÌ˙≤Îõûå¯‡ÇÁ∂–åπ¢•Êß˘ù»‚Á™î¬Åã÷ß®ƒ˘Äßçè∂…«ÕÂ¥⁄‘äÓÈ˘¬∆…Ê„–Ü±›ÉÙÿ±Ó≠
·¨ŸÊñ÷∆ßﬂ¶áÜá˙˙ª˝ªåÌ∫÷˙∂ˇ∆™Ê⁄“˙Ú˙èï≈øîæˇøÀ«ø¸©ËÔçˇ—˙Óø´øœ˝≠°Ï˝«ÕˇßÓ∆‚ëé≤ˇ…Œ ôÓöÖ
ü˛˙πû⁄â√Å’–€˘œ„…¸◊√ô±°‹–Ï–π–◊∑π∞ô∂ÏÛ√ãﬂ˜≠·ü•“·ŸŸÕàÜ°¯À⁄—àËï§Œ¡–…à⁄ΩáëØ∑î™Ç¬ß˙¨Ÿ¯
ÛÆÑ¬ÅÌŒ•›É¬ΩÊ˜≈⁄˚œ€€˚—ü≤êø¢£ﬁÛ√ºåÂ—¡°¬ûãœ∏ÛÜá¯—åß“—â⁄ì‚€‡Óƒ¿áïª«Û—ôÖøæÇòò‰ô…ùƒÛ
˛ºííéŒù´ïÉÈ‘©∆Ù÷í¬¿Ç¢à‡“üúå†ﬁ‡ºê˘ÛÃùîÃ§†ñôîÁºøÄûıﬂ≈ìÆ·ë˝Ë¥â¬ç÷‚Á˙∑…ã‹‡ƒá≈ÈÆîùÕó‚
√»√˝¥˜É±£Éãí«á°≥ú˚–úΩ¬øüÜáü·Ñ¥ºüúó∫„¿Åì £∞ÆﬁÒ¶óù¬˘≠∏Ñ¶∞î∆–Éà¬∫®ç¬¶Ë§≤∏√Ù©Ñ‘˝ˆ©˝•
‚»£÷∑ºò√êﬂºÖ¥¿‡∫ê⁄∏ÙØ€ŒË÷∑èñ∂∂ÁØó◊ßÁñ∆ß¯ß«∆Ü˙˚∫‚˙åù∆›ìÆˇπ¶µæº©∫ß∏ÆÏ˝≈—ØÚ•Ôàêø¸π
∑Ï√ﬂÍäﬂ∑ÿ◊ïôÍã˚õËÀ«ıÂ”∆™êŸÄƒﬂ›¸Ø¸Üëì˙™—éπ ë››ê°ÈÉ∞≥˝¸‚ÚÓ»ÌÛıﬂÎÚ„ñü≤Ê ÆÇ£êŒ®¸–Â®î
ÆüÖ”Í§±∆¡‚ìÛ¶ÅÖ‚öÇÌ–≤ú™Ëç®ÔÜ¯√–Äê≤Éç‰ÅÅâëòæÉùÙ≈–«œÙöﬂ†óÈøÔÔÌ«¬ˇ÷§†˛‡ÆÜ®éÈº·©õ
ï˛µÕÀ≥ÍˇËâÓ… ≥◊î“Ó’®´ıﬁ√ÉÛŒæÖê¡úäü¨Œ¥‘˝ª–À€ÓñÕ∑®∆⁄ÅÕ∑àì‚àÃƒò«”‚Á¸ßÚ‘∏∞ß˛ã€˘ñ‹°˙ñ
ù∆–¸‹√’âùå£∞ïüñê‡Í«éê◊•„ﬂÁ∑ÅÙ’È‚•‚∏¬Ù∏≥ã•èÙ∆Ù–µüªÄÿ°ª≈Ä‡ïœ˘Ú¬≠ô¢Ïã‚ƒ∫∂›§•í¬ƒé©äù
—¿•Ê¿€æ¡Êàµøê¯»Ìá”¯ñ´‚ıÑæ∂ÏÆıù√ïèÎÓ‚ö˘ÓÈÄı˙Ó˛‚‚ÚÄÙÙ‰¯¯‡˜†ß∑€ﬂ›Ÿ⁄ÊÏË‘µ‘∑äµî∑◊î√Ñ
º—›¸Æ‚”Ø∂•Ôˇ⁄ˆÒÔû’üﬂ’ï˝Ë—≠Æ≤∂ñå¥¡ïÕÙ÷Œõ´æÊ∫™Ê∫´¡ı≥›’≥∆ÍÈøÆ∫Èæ∂ô¡ˇ„Ãÿ‡¬“ è ÊæÈ®‡æ
ÙÕÔ–üãâ¶ıÁ‰“Ë√ùóçôı™˘„ïêæ‡≈Ÿõ⁄ˆ™¢≈Á¸Ÿ¡°§ÕÖå˚Åå√Åâ¢π˛πáçÂ§∆≈ÚêËùÛƒœ›œÙöﬂ†óÈøÔÔÌ«
•ˇ±˚É±‚µøƒÿ˚´¬¨¬ªüÎÆÏÛçíßàã»ëüü™Ü´‚Í≤ «ÅûÈû®É¬ç¨¢Ë∞Ç∆πì∏ÌÇ ÈŒƒïöä∆ÑÕÓπáﬂÁœ≥ÈÌ¥ª
…á»°‡ı»áÖö·µèÑπ¢®Ã∞£‡œ¿‹ËìÖëèÄ¡«≥Ë¯¥‹Ï¸¶óîπÜø÷ôΩàá∫®Î’¢¬»»à˛‹ß∫ÿà®¸ÄóÓñæıìêÄ¨†Ãí
≠Ê¨É∏∆ëö‚–ò‘Ãàœô∞÷ô°Ä·œÁ‹‘Ïæ‰é´º´ÂŸÏßÎ€Ö∂ïì˘·ÈÈÄÊÊÍ‚¸ÙÙÏ¯‡®ø˚†Øˇ´ø∑≥Ø«–Ñ„ÏÔ≤¥˜ˆ
†¥∞†⁄Íãõ¨™æ÷ñ˝®Æ¯˘ﬂŸÙÆ™¯ûΩ–°±‚”˚ˇÌÁÓÊ“°éßäÈÆÍÄƒ¥∂‡¢≥‚¶≥íÙÄ±ﬂ›ªÿﬂë¥¥Í‹ôûﬁ≤€ö≥Ÿ∑Ã˚
¢éã≥åØœÇ‚ˇÔÈ™Ø·˘ú—ñ≠ˇ˜àÌ∞˙—êà∞¥à‘– …Ö•Ñ≠ÏƒÜâƒËÄ†ÚÄàíÀâíﬁ–‚‚î‰…¢˝º©çí®◊¸Öæ»˚˘˚Îø
’˙è€üÖëØç˘°äôªô—®óàñûë„¸ñéªÂ„Ó«§Ç∞¡ù∏ëç ∞üπ±≠˙Ä˙…∫í£∏…Ö‹¶ùº˝πø¬—Øàèøâ∫Öá¸›∫éÅãã›
äÕòùÇóÄÓËÅúÖ∂Œ≈Ó„©Ÿ›∫«åò£¿¿°áóˆË≈Ø©≠πëìÃíàÏôÀ”¢†√à–Ç¢˚≠îˇæ∫çÂÆÉôö´≥‡ÇÄÑâ∂¿ˆóÚöˆ
≠ ©©Çü˜†àˆêÙ»ÿï¿˝®‚œœõëòˆàîê…Æê¯π≤ö”‡çïª«öÈƒÑ≤ÇÇ∏àïê™ìÙ•ƒˆœ¸€áŒÛÅéÌÂÌı·˛¥Í˙ºˇ¨ﬂ∞
Á‘›ﬁÁËÓÓÊËË÷µıÎõπªª€ªÙÕ∆›„Á˚ùæ•”™ΩëÅ—ÿ˘Á±°‚”˝≈ßˇ∫Ú¢˙ä¢¶è ¢ñÍ¢›˝∏ÒﬂÎŸ®æÚ€æ∑ˆ∂´Í 
§–æ——∫•´∑õÎ™⁄Í´€ÃüÌ∑ÃçÓ≠„œŸˆ∂Ìá«®ªŒü∆ØßÔ£“¬˘´⁄Úœ≤–˘Á√Ä’«ˇÁΩ˝ˆØòù˝¡…¿˛ƒ¢Ìèî ™ËÄˇ
†Ä¿ÑÔ“Ÿ◊ÀñæåÄ≤◊ﬂŸÖ≤äƒÓ‰Ø´◊¸æ»˚˘ò˚Îø˙ﬂ±â¡à«ñ§®à•Õêé∞Äá¿Ä—Îˆ”®°ùó•√ÒÑò√Ø©ÄÌì¶Ñ©Ïä
ﬁØˆ¸ ¸µ—Œ•õï°“≈ÜÄÀï¿ü†ëºÖµ∫˙áÑﬂ˘‡¯ˆ«§áŒﬂç∆ì¢ëÖŸÿ·œ„È†®¬áƒΩûó…π®∆ŸÕ©ÔØÕ‡äê©ÒÒÉ¡Ç∑
µÙ„·…ΩÀ€ô¯’†çÖÈ«»¥˜òÜ∞°´˚“·òÎ‘òπ§ÇÏ£†Äù‹Ÿ‡€îé⁄π§Âø÷éΩ‹ƒ∆ßØŒ«ù©¬˙õµÒö¬áπ’ê°Ì÷Ó
∫‡ıàëÔ≈áòÑ∫Ï˚≤ª˛˙¸ñ›¥û‰…ŒÈ¸”ë∞ŸøéñÕõ≥ãã·ì”„„£˝√˛˝˙ÅÆ÷æŒ˘ñﬁÄÑÆ©«ü–“”Ÿ⁄„ÏõÎÓÏÈ“∑ˆ
÷ˆ‘È€´ªö∆™⁄ÍÀ€Íä‡…•≠µπ§ﬂµ¶∏≠¶ÿﬂ¶–“ﬂ÷æ—ŸìΩ‘ﬂøﬂ”Ωæ„¨ò“ïŒòÄ™ñ˚Í÷áõ˝Õì∫Õ˘¡ùﬂµÈΩ≠ı˝ª
Ó˝ﬂÈ±˚„˚¥îΩÌÖπØ≈ô¡˛°ÌöüÔœ†Ä†êÖÁ¢à‰ê˛¶‰‘í®º≤˙∆˘§îµ‰§Œ°ÆÑ‡¥Ê„∆√¨Åù≤òÀ‰Ûˆ´Ø˘˝ëˆÛ
ï÷∫ÁÖµØ¯ÃçÀæÓ’ˇøÑ©∞∏ëú≤á“óˆ„≥ª—›€ºßÒÚ§˛‹î—ø·ÏúÚµ¬Õª€Êœ€ÁÓ»≥®Óïﬂò∞µÑ∑ﬁ≤«óﬂ·ﬂ˝ÿäΩæ
Ïˆòü∑≈«¨¡¡—ÀÚ´¥êÙê€ΩäØüÔƒß∂√˝Î˛©ˆÖœ€º™„˚ﬁ£€ÙÍóºôÜÂÔÁäè‰∫‹Òı≠¯ª÷£˝›ÎÅ–œ⁄ï†˜ùïÏ€è
œˆõÇÉÊ–à—ª’è´Ω…·ìÚ†πº∞ë‰ﬂıôéÍ£®≠‚Î‡¡ªó˝µÜÂü˛…ßﬂ˚ÙÈî¡üﬂµì‚Î€èÄù≥ö§ú€¥„òŸùÕ◊ Ê…´
πâó•˜ÇÀ›ò∫¨¡Ÿïÿõ÷¢√ª∫Õ‹£ﬁ∆ØÓﬂÛ√õÙ´ªˇì¬æêŒ∑Òß¢Ë˛ ‹éΩêüﬁø€ìÔƒ«≠∏ïøÕä˘…ı¢üÛ©∂´ëÔ€ƒ
ïä∂≈ùÄâ¯∞‰∑µËç”é±’ÑÌ¿ÇØºå¨ùù—ÛÆúÔÔíãﬁ•‚†‡ßßÒ∫àÎ»ßî©ÊÍ∏≈£≠óêèƒÚÇéêôçù¿…ÀºÀı‡óÄ∏óÂ
ÄÈÙËπÃ˜´áÄ†°Î”Äü±£îèÈ¬÷Í£ºÊõÑﬁûÌÀä∞ÊÒè«¢÷‡ï∑”î†Â¥à∫¿úÇÿÀÙ◊Ã‚à·˛ëÈçÌ©˘•ˇ°ë°Å˛Óæ
∏û˝Ø‡¿–å‚‹ËÖ∆Ù∂ı˙ö€˙ª⁄˚∫Ñª˙åêƒ¥ô≤ªÍÅê∫Ê §à¢ÄëÌıÂ•ÌçÂù†∏–Ç◊⁄ﬁ€‚Ë®ÃôÏØò¥÷Ø∏å∂¨ˆ÷Ãı
¯’Œõ ÊåïëÀêæå∑ÏŒŒô∂Á˚˜÷∆ºÌõÛˆΩÌ√◊ßÎÿüá˜Êú∏Úé∏‘¯Ú´ˆ˘á·¨ä®±ÄäãùÜµ–Ö»ÒÌ†ﬂæ–À‰–™ÜÂÖ¨
í¨ƒÛÆÃ£¡°»¿‹»ﬁ‰√–Ü•ÍàæÑà≈≥Î–Î˛ﬂ§√˝ºı¨¯¡‡À®¡Ìˆê†Ü˛∂¿±£œüµæ≈¡ˇ∑Œ∫õ¥¶œÄéç”ÓÏÄƒÛÃ©®Ö
îÑóåÀﬁ·Û¿Ê∑ÇÙÿ∂¨£†ñÌÒ”Òä‚˝®±¸ê€˚‹èÆŸ£Î«ÖÒıöµÜ∑õ†≤áåÀﬁ›ÇÜ øà‘≤¢ó”¸Çã§‚ÇŒ¬ªÎ≥°îÿ‡
Ÿ˘ï∫Ñ∏·ı¿®âÆå€¯Ã†¡ËêÅëÇõπ√ù˚◊ÛÍŸÇè†òì∏Ìì›üãûîüóû·±˜≠ÄÃú‹Æé√üºËè”„¢ú≥•¥Öû–˜¸“Ò÷
óÏÃÙ´ôê¿æ√í◊∫‡«Ó˜Ó“◊˜©´ªÎ◊¸êµµàÇƒ ƒ¸¬âü∞òÿÃÙ†ù∂îª∞ö∂üåí¡·ŒÎõßÖ‚ÿ£◊ö·ûñ∆˚Ñò¢ö∏‰∆
‡ ºπîñ˘‡®¢‡±…ÖÇ›Íø¯ÿ¨Ê⁄¸Ë‚õ≠’ù–üúª¢¬Û–ﬁ¶é¯Ò‘ Ø˚çªèì—††“Å√ﬁ·§ü¸»·©Ê•ˆÉ¿Ñ£—¢ÉÔÍÖãá
»°ë÷ÿ∏˛·Ï¥◊æ—¥¶ûˆ†ñ·Â∂≤ÿ∫¶Ò›ûïÚ†ÅªÅ’Á∂Ã·´±≤€ß…≥£≈ºÓ˜≠ÿè∆·Á◊á∆∆ıÜ∏ˆ˝«≥æΩ—˝ﬂ÷ﬂŸﬁ–•
ﬁÔÜ®°ø«◊èﬂ£ÔÔ≈Å¶Ö∫çÕÄà√Î◊çæ• ◊‘ŸÈ—ÿÅˇöö´Ù∫€´åÕï˙ˇ∆Úé∫“ŒÜˇ“°é ÈÓöˇı·∆Â˛⁄ÖˇûÍŸŒÚÖÓ
◊™’ÅÜÛÿ€ìˆåÒÁæÔ˜ÊÈ»∂∫˛∞≥Ÿ·∫‹¡∏ùá˛¿úøùâë˘Ö‡„‡Ã¡Å´ü«—«†§¬àæı¸çÀâúÖ≥ŸìÍ≤â≤¿ï¸à–¿
êÆ§πﬁ∏‡®åÑ≥óú∏äæ…ßÇ¿ú…¿¸çÆç∏Ä´Í⁄Œø˜¨˘Ò»±Ùπ¥æôôﬂ√Ô˘¬√ñ‡Áíóó°–Ñö˛¨ó˝Ç∑ˇ›è¡Àƒú≠∫˙ò«
‚à‚†ŒΩﬂ∞«‹â√êÈ∫·ÅÜ≥â¡Ñ¿Á‡¸≥åò≤“ì⁄ö—πÉ”éˆ¡≤®Ã¿®ÏÑÿÜ∂ËÆçôú–ø∞•ø„∆†»·≥≤∂ôôßñˇ·Úàáíñ
áº˝ê≤ìáûá°üÑ√Û∂¯πòöΩæ ùç£‘êπ¥Ê≈¯ÅÇú¯©≤®°Ãê…•‡ÌäÁ¡ŒÓ¢îÓ⁄«Ωó‚ó˛˙ÌÇ≤‰¿ÖÃœı›ŸÕùû¯
…Ω˚ÄÂÓ—Øë˘°æÖÑ◊Ö Ñı≤Ö¯á≥¬Æ¡¨•¸ËîÛåí‚íò‰†±°⁄√´„©ıπë®∏êÎ»ùèâ†≥¥¯Øòõ±îûã£©ÍæÊÜÎˇÛ
∑‡¶¿È£ªéæ‰’ΩÒ˚ú”ª‚ÕÀÚÿë‰Òﬁ¢Äê∑Çéé◊˜ä¥ëÑ¿ÅÇû‹Ÿ™„òÖÑƒÏú∑µ•Ìá≤⁄ÔØÏ–ˇΩì˚úÒÙå∑À¡‹°Í
ﬁ¿ËÏ±Ú’Ôé¨’†·˝ÀÓ®˚í€Ó∏ÑåØ÷ìà’ô∂∫‰¬¶ª˜ı«πƒËΩò⁄∂èçºœ˜˜ˆ∂µ∂ø¥µ‘µ∑÷ı£¥÷ô∫öªåá≠Ω›ùÉ˝Ü
Ç‘ØûÑëÑÌÇ·™Èá¥¥◊≠È€™ÅîùÌ˛µ∆∂öÆ∂ÍôŒñÑÑÆûÅò¡ëƒÕ ‡Ω¯≈ê’”ÜÚ˘øÓöµ·ÊÕ©€˝ﬁ‚ÔÓ¸Ôıˆ‹Øı¶«∂
Ÿ±Ë∑ù¯É˜ûÿôßûÛ¬à˙æıÈÕ›¯Ê»ß…ãÊÆÇÑÓ£Çπ¡±Óîˇ®›‹‰÷ûîîÈ∞°®±¡üíé‘Åó∏≈‚¨–Ä‹–‚ÓÇ¯å∞©÷Ÿ
≠’ÀäÄÚ◊—’–πÍµøØ“∂˛ﬁ⁄®ø§ﬂ–Ñ∫ÑÜ–Û‚˚ø√õ°âÕ·‘ˆÑºˇÔö≠ø˝ﬁ”≤Î˘¯Üê≥ß´ŒÑ•ﬂﬂö≤‚ˆò∑ÇàÄÿ·á
∞«è„èœ£ÄóµÕƒù¯©≥¥¨ªèì¿ùë‹ãÆÚï¨ë∞¸†¥àìëÛûˆüµÖöﬂäÇ˝∆∂££õ‚««âÌË∏ËãÈèÄˇ“Õ‘∆åéŸÚÄ–≤ÂÙ
º≥å∑ú†Ÿá£”ª—àì‡Âå¨·§ê∞ª”ÂÍ·≈∫¬ÜÌ∫Äµè¯∂Óÿ£¶ï•ÎŸÔı˝≈ÌóÍÛﬂı˙˛Îç˝ ˚˝¿¢üÔæ÷¸Ω£ñ£·
œÚ·ÀõÄ¢ü∞‚íŸ¶åÒô‚ìßÒÒ‡çä©‘Çìˆ†Ô¨ß Óã›∑∏≥í”ÑïﬁŒ¶È„∆çÒ™ﬁÙ¨ÚÀ™≤´Û≈¿ôÌü‚˚¶Û ‹ç—Ì‡¸∆Ù
”‹÷∞¬ùØâﬂÄ†Å£Ü◊»„¿–ä©À„√ÑÆ¥ç∑Ëˇúí∂ÂªºÉﬁäá“◊Æ∏ÇÇÊô¡Ä†∆¿Í˚óìŒ˚Å¸¥ò≤Â∞çñéÁî⁄¥¬˙á∫
Â¶¢Œ¡∞≥Áç√™Éı§Ïæ⁄éòéº´∂˜ìÍÆÛı˚¯Ö›œ∏ÏŒØÌŒÜ¨ô∆ÁñÜ«™∆∫˚∫ÙÌÕ√∫é∆√«”ßØÆ©À–ÇÒÇÂÑèÉƒá∏–
À◊›Ó‘∑ˆ¥Ì‘¥◊ó∑’ñ…µµ÷Í€Î∫éÙı∆é∫ÄÑåƒê˙Êäù≈˛ê˙˙“æñÚ‰—›’‹˜Ìå•±Í“–±÷—‰∂ãÄòÈ›ô—Î∫‹ù‹›Ë
âµŒô◊∆∫¨Æã„ªõõáé©Á…†ÊäßŒà„ÁÑàÀèä‘Å¡å¿π–Ñ§ÉÙı¯çˇ÷ΩŸÂÑ˜‚æ•Ó¸Ÿ´¸ ’©©≤ü…•√êÄâ¡Ÿ£†√¥◊
∑†ÿ˘£›ƒØ˙–âÀî¥ﬂß≤µÕ¯⁄Êºù»ãæÿÃì†ÇÄ√õ¡‹‘Ô«åÊˆ¢†—ã∞ò¢Äã˛ˆ˘à»•ºÕ”¬π∂˘÷”∫Á—≠ê¯°¢ò‹àÃ
 Ìáó˜„õé‘§Ç¬Øöº√”øáÇì∏ÉﬂΩ‰»ÃÌ≈¿›¿Éëàäîã†´˜éŒÓƒßˆÄ‚”ª¯Å˛‰¸ëé¢˘ﬂÓ∆–¶àêåëÒÂ∞Ü™©ÿ‚∑¯
∫ÉÄ¬î†Äç˝ÎñÌ±Â√¯Òö≤ÑÛ«ìÜù◊ÜûÕó–«Ó§ﬂäÆé‘˘∫áèÂÅˇß—´»⁄±óﬂïÈˇ»®Ô¯Ô˝◊ˇÔÄåÛ€áÃ±ı˝Ü˜⁄ºÍ
ıä…≠–ãîØ‡¬äı£ìï™π’£í∏ßÙÿœô¬…©ß¶ÖÄÇã±´ßøì–°øñ°ïóÔÉÛÙ†††¢Ü‡˚¶ãı‘úó∂›Œ¡ÿ•∂≠£“µö∑˘î
ÇÄÇì†®∫´„«Õ±¸‘§¶®öÑÅ‚∆ù“óçòÄÎ‡◊‹É§Ü¯¡â¢Úı™é√‹°ŒÚ ÛÌÎÈê“Èíˆàç⁄¥ã«ÅÊ‹ëìÚ“ù≥†ÊÇÈΩÔ
ä¸Àïﬁ∏Ñ¥±ú¨ÌùŒôÏÛÎ€ÛÀÊŸˇ≈µôπ•©ôˇ˘â…Èë——øÅÅæûûŒŒ˛˝ã—–“’ŸŸ⁄ÌÏÉÿÅ†Ê∞–ßß¿Ë˙´ª€˚ª⁄ıÌ
Ëı≠çïıï≈◊µïü€Í´Ù˛ÌµΩùÕ›¿√àˆö˝À•“ı°ÅÀê∫∫íøû¶≤°∂ä…Ó˝’òÆ‚§Ω∂¸®µÙ™´ˆæÌ¨›∂‘Á¥—èÇ†∂∆ıÛ
‚Ωç´ì˝Ê’˚Ö›Ω€—©ﬂÜôà∫£Èç†»ÆΩ…‘© ‡≈‡î£Ä¡Ñƒ–¡öãá®å÷Öî¡¸£‹ﬂÄÊãç¯Æƒá´£†»öâ∫ƒàêäÒÇ¡Ç≈¨
î»ä›¶ØÓ”≈†Î‘Î˛ﬂ§Ø˝ºÅìÔ¡∫»µ÷Ïëëõ¡Ø…Ì»¢Ωæˆßî‡¬Å¨∂ß˚¡˜⁄√ÄâˆÙπØÑíåÜñåŸÔÎ¨óÇêπ›¢÷ñáÕ°
¬ÿôØü–ﬂ–˙ì‹◊èÀÚ–ŸôªÚŒΩΩè≠·ªÎœÏπ”¿¿êà¥‡ΩèÓàÿ°ÒÑ∂ÿπÙÆ∫ˇ…ùò–‹Éæﬂ…ù°“∆ë†¯≥…¢âÉàô‡ÜÄÖ
°Êà±Â Ã∏„àÅÊ±ø¶à˛˚ôÓ«∞µµ¥Çàììá„Ïê“Œâá®≈Ò¡ÄöµØï…íõ‘≈ÕÊÚ∂˚•ÉŸ£´öã¸“ß∫Ãˇ„¯·„Ó€∑û’ÉÅ
…√ΩûÏÖ®¿∂˜ÅÚ—¯Ì¯ŸÕè±¨¥∞≈œ≈õ≤ﬂ’¥ªß∆Çı•ÁúºáËÅ≈…∑óü°î†‰∞¨é¬¸úﬂ®€úƒﬁ¯‘“∆≠ÛÊ´ó¥ Ü˘∏íº
‚∞‹óúæóò££ŸÎ™∆‹¬ò¨ñøøÖ◊ÇÈûøüº®€çø¿ΩüÚ∞Ü¬—¡‚µ»ë»íÂ⁄±ùâ‰”ò„π€áà®‡ëß·è≈Ω⁄î√¨ÉáŒ±É∫˜
Æ“ÛﬁàÍ‡àˆÖçˆ–¡µÚ∂èÜ•Òéπ€˜ÿ˘°Ä‚¿ÉæÏÒ®˝îÃ”Ìù∆∂è˝¶ã˝ì≈´˝ˆÌÄÎ‚Ò„¯ÍÒÄ‰¸ˆ‰Ù‚‰¿Ë‰¯¯†
ƒˇ¯†èœ∫ÙöˆˆıÎÅãΩí‘å∆¥Ù≤ùõÙÉ»á∂ﬁ„˛À∑°±Ωπ™üæ∫§¢‰√ÂèÈ«¶∫Ú√˚Ò›¿œ€è°±Î≥Ω÷◊‹—÷í–⁄⁄◊“¬õ
ıôåı•›ç•πÉ≠Â˝Õ•˝ Õ•Ωç•„Ü◊öï—ˆ€Úüµ”≤∫Ω◊µ≤≈‘≤——πª† €ôù∑⁄∂ˆ€èù«ûÄ´úÁÙ•Øﬁêπ˝ø¥∫¯Á¢¿µ
´ª˜»Ó∫Ã∫¡ªé·£ƒÙ˛∏‹¡à®Ï†ˇòôä°•îêë≥¡¬çàà¿†‘Ää§¡◊†¨¡–ÂîÂÔ»∑ﬂ˜†◊¸æ»˚∏˘Î€Å⁄óÒ¸ùã˛√íø
¯ãôëöÿäÄéóÂ©ëñÁää∞±‰è√¥±´Ç—ƒ∞Û…è‘–Ë«˚˘Ùã⁄ÿÁÖ¨åÔÖ¶·ïæÙ†»‚ºÒ¶±¬ÎÖ•ƒ˙ñ€««Ë¿ä±π©Àˆˇ
À÷‚ ≠ùß·ùˆãˆ¯‹˛úÎº˙§£¡ÒˇÖ√÷‘à«Ñ€æ»Åâùòà©Ã»∏˛Ü∞∑ŒóüﬂèØûƒãóπ˘È·‚âÏﬁ∞ËÄÖ◊¿∑Ø‚◊¨“Î
‹≥´·ÉÕãë¯é—¨öÉóóÏÔúëÜÂïŒËı¿•Á•Àú°Ù˚¿ÂÜﬁ£ŸÇá„∫‡Åê±•∞ˇÕèë‚÷‘¬‘•âûˆ©˝Ù≤˘Ò†ÀßÖîÌŒæ
ì‰ÖØﬂß∏˜Åã”ıîﬂƒäûœ’˘Û≈ŸºÍ£Ïó›§ÚÄ©†ûã∏Ωâ™ºãŒ¨›à¬ÕÄÃ·Ñ∏Ûüï·¨ùëæ¢áÌÔí¿•ªËœíÄ»ÒóÇ¢¿Õ
≥ô‚∏»à≥¸¶ò”±ÆËÆ®¸ßÑÔ∂Ë—äá§–ÖÈì™†Ì∏‘ﬁ®Å√œ·éù‹°≥‚®â†Œ‚Ùﬂ›Ê∞ò‹Û∆∑Ωù˛ã›µ∂˝Õ∏‡⁄ôﬁ∂è
Ûœ≠éŒÃ˜è Ãıååô˚ÜÖå›˝∆„œ◊¸Á◊è±´Øß¶˚∞øÁË†Áã∞ﬂ∏–ÎÄ¥ÒÙıÉ†⁄˙∫ê€—›≠†ﬁ„æÛóæÅ∂∂‚˛Ú©Æ™∏¢Ù
î„’Ÿß∏å◊ééı∂ˆòö∫Ω€Î⁄õ õÎ‹Àö´äÎ™äÔÍ‡∑ï≈Ì’˘üçı≈Ü∫œˇ¨Å∂äôûÜÔıÒéÌπ„˙íË€¬Ô√¯˚»ÖÅ‚ÃÍ∂Ÿ
ëèùŒÔ¿õÕìÖœÓ¢πß∫√ª·ö¡ƒıÈ©πõÂ©ˇãÎ ˚çí†ã‘ú˛œäëÚÖ…¯Ì¬π˝äâ¡Ä¡ÙÅÖ–˜∫Ÿ ìû›¢Ú’¨ª–ÉËµø
™Ø“˛ﬁÙÀÅ•›í™„±–¡ıòÄüè°ƒ¬ÿ˛ú÷◊é–Œ√ÓÀ⁄œ“†≥˙£π˙òò›ç˝ç⁄¬ΩÎ¡Œ√ÂÄ·©Ä¿È–∑Åù¢”ª—Îô›®‹ã¸
˘˙Ò¸√ë∞ûä§Äëªûà•‚≤∞“Ö∑ï‰ó≤„Âù∏ØÿπÕûÑ≤õÆî∏¯Ï„‡‰°»ÑêÅ¬÷‰Ñ ¥™§ë®Ãà™ÕÌ‡†Å·Üäçæ¡¨Ä‹ñ
õ—ˇß»ÛßÕÈ¢ç›í⁄ïäá∆◊´«…†ëÄºãƒÆÄª¯Ç∆Ï∏ƒΩÀ˘Œé¶ãö—≥øÆ‘©ÌÈ‡ıãÈé®Ã˛∂¨Ÿ—∂ù·ø¨¯Ê∆àÌÃá«á¡
∑àÉÚïØ˘°™¶Œ’›ä—ˆÜ”—îª´é∏…ã÷ò‚ºùçùìŒÒÃ–º¨å¡Æ∏Òªùˆ’‡”ä™Úå®†¿ÏäÉ≈ÈÇÓ∂¿ÒÑ¯ÕÏÄœí¿¿‹
ÚÒ¢âºπõ™ﬁÜ«¥„Ë§äÃç◊Ω ®üíŒÚÿñö≤·ñÛâÜ÷ÁØÓÚ«ÌæÔÒ˛‹ÓïÊÅ¿° úäÃ¶¸ Å¨Ã¨·°Æƒáø⁄´æÛ¯§ÙÃÄ∏
ò¸âêëÏ¡†ˇπÁÅê¡ª∫Ã˝∏∑∂π∂≥â‘ø∫’Ω∞ÍÈËûË◊˜∑µ˛˜∂∂Íõõ€ñ€ö€⁄ö€€Ÿõ€ıÌÌ›ùˇÜ˛æÓ·¡ﬁü°ÅÆ¡˛ŒÃ
é•ı¥ı˜Éó±ö˙ªõ¿Û˜èÎÎç∫⁄˝€Äı«ÛÂÎ„˘ê√Õ…ÕÂÁÄ¿ƒı≈—À√Ö∏—◊∑‹◊⁄Îã®ÔÏ´ÓÎ¨ Î®ÆÆ≠Ø™Ú¨˘ÿ—Ùæ¶
˘‰√˝Ÿ«Êäøè≠ïü⁄™æ¡µ◊ó¡¿±¢è´öÎ©ﬂùÍæﬁË≠üÈ¨û‹ÎØüÌÔÿùÓôµö◊ééè“›æ¿†ë∆˜Ã˛˘óº“Ù©ñ™èÄîâ›∫
‹ÓÜ»ªÒá¸≥ó´ø¨Ã†ñ¡àÔœí†∏ªä·ÁØñœ‚©‘•ÙØﬁ∆¯ﬁæØ◊¸æ»˚ê˘≥¨áÙ˘∆‘††‡ﬂ∏Ú»êú⁄πÃ¸·‚™€ÆÙ≠∞
Éˆﬁ†ÃÃÓ«¶†–Ë¶∫å⁄Ø√ì€—©¨ëûƒÊ˜Ò‡ÄÓ–áÇÆ¿¯¿Î¨Û¬‡¸¿©çÅáÀ‚¯˙‹⁄ÔÉ˜ıƒÆ˚¡›È‹„Ñò∞»¿†∞ªáà
∂¿—Ï≥†¿ºÖÇ†±Ëœ–¨¬¿≥å®Ã¿Á÷ˇåŒﬂ´’î†ËÁç¡êÇì®Å°ÉÓŸÕ…Ô˙≤˚æúë≠Ï–∏¿ùßœçπ∑µËæ™ÜÅ‹øí÷¯˘›
ˆ≠ñ÷¯éÃ¶õÈ¿„Û·¡≥¶ı∆˝Î˛àÈ‰ºØÄ«Ü‘êÄâØﬂÄÓ”µÁ∏¡°…«ìƒîÍƒ˘«÷ˆÊ™öΩíÌ∫©¿ë´¯◊ØÌå˛º°µ˜°⁄∞
Ü¬ìÃÁ∆ËÅï¡ÁÕÿ´˚äàœÃÅˇé·Ä±ÇﬂïÄã‹»˝∏ÛÌÀ‚ÆÔâÀ Æò∞Ä•º∫˚Û‚Ññ÷ê˘ΩÃó˜Á‡∑è◊˜»á€©áÔ†∏å◊Ä
¶∏í £ì«Å≤ˇ¢É˘òÚû◊˛Ù•ßâÅ≈ÁÒÛå®Äƒ±¨ÇàﬁœÉ·ÉÒƒé€Œˇ⁄Œ∑—‘“∆ˇË¥˛¿°±©≠∂∂ˇ±™≤¶º¥¢–Ç‘Ç™Ø¨≠
Ω®™Ø¨™®ÍÙÏË–ı¥ıˆˆòáá˚áá∫˝ááå√˝˙¡ˇﬁ∂˛Æ¶æŒ˘˙Ó˝ç±£ ≈–÷©Î©áÁÈﬂ‹◊“–˝πü∂≤®∫¸”≈ˇÈßö ‚∂¶
â Ó˝›»ﬂãñÄ¬–÷’“ﬂ‡÷€—‘—Ú±ó¯„ªœÔ˛©€˛ÎŸßﬂ´Úæˇ⁄€ÚææâÙ√”˘«›Õ»—œ√ÒÎÈ€§ÆÏ¢£ÏºùΩÏ¨≠åÄ±
¬‚›ÿΩºæÚƒÑÇë‘∫ÀÀ˘ı≥⁄•≥»ÌºøàÁØÆ‰Ô´ï˘˝˜ﬂÆûö–∏ç„ã∆èÔ¢À∫¢ØœÈø·Ú·ÅÓ˚≈¬Ï‰–Ó˘â∫⁄áÂüˇëﬁÛ
ù˚¢ÌÁ≠Û•«¿å–ØâÃÄ£‡≤Ò•∞±ïŒÅß∫á”Ù”Å€ÓÑÑ¯˛ªîπÿ∏˛…ûüÅﬂ“ˆáÌ«ß¡ÒﬂÑÑ¯˚ª≤øÎå„ﬁ∆ì∂ªÁ•¯àÜ◊
°¬Íê÷ó∞Ç„Ñ’†‡Çæ˚Ì´Ωˆûˇﬁ¯ö±∫“∏√°À©ÇÅˆÔ¶åÁÍ√âç»’„¡èΩº—ÜëÊ«éüÆŒ·¸ï‹ÔÅ®π´ÒÿóÑåÄÜ
›õ∑ªÁ√ûß—á¥´ôªå±ïÁ◊∫œÿ¸¥àÂÃÈîÕ âπÒ·ÅÓõùÔ”ùù∞—ØÏûı’Ó¶ŸÌ‹¯ÊéÙöü¢º±≈ÿô˚¬‡¢£ë–¡Åƒûéä
Ç†êëä¶ƒæ–™Ã≥∫ôëß¢ ˜Ïµ≈√“∫ª¶™å»‰ôø·¡≥∆€îÎòÇ◊åîÅ†Çö‚áîç±Ê∆„Ñàë‘∏¿Â¡Ω«ÅÑ‹âÚ≥äïøﬁ£∂ı
°•˙êÑëòÂÙÇÆ‹º‡û±“õà£®◊Ñ◊…ßÆ∞›Ñœƒ´°ú∏ ÕÒî∏¨¬†æÄ∞Ç˝Áƒ‡ñŒú∂≥ñùØΩƒÚ‹óÄ¿·®îæàÏ§ˆúÅı€Ò
°ºÈÁÙÊ©ÁŒ¨√–”ﬂﬂÄ‹’Ÿ€—÷Ÿ¯÷‹÷˙π§ºˇ∏¥¢§¨™¨ˇ¢¶≤™Æ∫¶ˇ°ÆÆ≠µ≈ÿœ©ÎÔÎöùÈƒùùÍûô¥éÁÙÎá˙⁄˚ª
àö˚çµ›ù’˘ùÌï˙ñÚˆ°∆“¨Æ¥î∂€◊ó∂’ï∂÷ùïµ‘ïµ¥÷‘¿ÿ—„®õÍéÈÏ÷’Ù¥◊÷¥÷î‘ó˝™˙®˛ÿÚÒ˝Á◊˜Ôäøº´˝
ˇ¯Æ¸”ª œøÔ™ãﬂ◊ñ¸éË©¯ÿŸ€¥È–‘º—éÄ®”ò‘’ÕÍÎñœ«∫≠´Ûá˚‰ªûøØúèß ÚÁó˜ﬂ¿óÁËÑ©Õ∫Õ—÷†±«ñõ±®
ÓÕÆË¶˛˚ıÛÎ≠⁄Ò˚˙ÌæûÙç§πÚπÓ˝’àÓ≈◊”·¯ø¸ÿú‰Ò¡Âã…ÀúâàÉﬂµ±†ÿ˙íÊﬂÜÎ°∫Ä†É¿Í ¡îÄÓÀ”Í Ó¯“ò
Ó¨¬Ñˆï¯ÿπµ£ÿÛ«‡∂çÄí–‰Œî˛Ñóäﬁó≤–™ ﬂÊ ≈√óÁôæÊﬂÓæà–“Ø¨à·ö™Œ‡ê‹—¨Õù¿ù’Ó£∂Ô˙Üº∫çÒÕê
§˚óù»≈œÍ∆óœ¿ïéúÆÄ—Ö∑‡¢Äéﬂˆ©˘è›ˇ∂’ÔøÉüÅë¸Æ†™¯æÊÍ˝˘Ω˙Ã√‹è‚¥∏ıÒÄã⁄ºÂ≤ôáÚû«◊π©≠∂°Á±¨
Ω‹õﬂùƒÆ˜•ûî∑ª˛ÕèÃ±˘Œ∫’…¡¯ó‘∫‰«Éæä†Œ¡—úÁö⁄âßÌ‚§˙‘‘Œ€’õô±ë„£∆®òÄ¿°ÿÇêƒ‰ÅØ≥‡ô£Á◊¸ 
ıíî»ƒ≤∏·£ä´Üû¨ùﬂÇ™©°ÃìÈï…†¢ªÆ´”õÏ≠“â¿†ÇéÅÒÄˇ˘ƒúºÒ≠Êç¢àƒ•…ãë∞…“ê≠“ñ∆â•Æ„›âãÌÅöè
Ó‡àÕˇâ±˘©”ëŒ€˝±›»◊˝˜ËÊ≈∏≤¥±ßÔÍÔ‚ÌÔÏÏÏ≠Æã™≠¨´¨Ø©ÚØ˚‹⁄˛π¥ﬂ≤¥º∫ºí–˘–ÿ◊ﬂ“‘‘˜ﬁ⁄“ÈËËÌ
…ÈúöÏûõÈ§ùõÍöùÏú≥Ì◊çı∑ÙõÏõ˚ıµ›Ìï√Üñä›”ô–ˇ°Ω±¨Ω©¢ˇΩπ≤£•™£Øµ∫£µ∆ﬂﬁø€•´ÌÌ¨õ™∞÷ı∂’µµ
ìî’◊˝Æ™≈‚¿îï˝®˘¯ﬂ°Ù”˚≤œÔ≠ÀøΩØ®¢æî˛≈ÔöÀæòêÍÄê•ìùï˛”˙ÍÈŒ∫ŸˇÓ∆çŸ·˝Õ’ı”∑≥öÃ¶ëŒÚÁ◊◊◊ 
ú†Ûıõˆòÿü˚Ï≠Ú‹¯˛¶àÌ˚ÉΩ∫’ÔÔÎ€óªºÀ∑ÌÌ˘ë√¯—àñﬂÔïœßìÄ¯√ÿ◊‚⁄Äî‰ı„πƒ„∫ÄÏäéòª±ÈÑ¨ÃèË
©ÅÕÎÔ¨ƒÖúº ÿó∏‰ÃÄ¶∞÷ÃªÖÇ÷èíç◊úÈ‘ÈØïÊùªä∞‹ÅˇôèˆﬂøÛÜ—˚ØΩﬂ·›íπ∫á≈à¯©¯Õ€Õ”œ‘©Üª§
ßÍäêëªß‚˚ëˇ‘ËÛπÔˆú¢·ÏœõﬁˆˆÜ≠áßÊ›•√ïãÌÌ˚Õ˜§¥Ô…‚ã¯°ÎÆ¯∫›ËÎÒ∆‡±π˝ˇµô∂˜”ë·¢Ñœﬂ„®àÄ
˙Å£ÉÑëˆπ˝∂µçåîôÏÊ‚∑ˇÎ˛àº¥ÏŒ‘–Û€öñ˚‰¶Éòâí™Ùú†ŒÓ∏Ëπì“‚à— ∫…¡ÑÌÛ‰∆ƒñÁ˘„ËÍ∫á…áÈ¶ÇÑÊ
ßôˆÇŸÿô˚Ωíó·Ïµ¬¨ÑåÍ‚π‡ÑñíÎ‘‡¡âóò™®ıÏπéäßâçªº„ééÀÕ›”¢âÅÅ⁄„©π◊Ñ„ÉíìÄü≤Œ—¥Í››‘¿á¶
åË†Èçö º¶£±•ü∑Çûíáì∂◊–‚˜ìÕ≠Ÿá«Ê†æ®ÆÆØÇ®ˇ°¨™¢∏ºú˝––ˆπ§¢∏ˇ¥¶º≤æ∫¶˚π°Å–†◊µ»˜ôç˝˝∆†
€√ËœƒËÔ›¯◊ÿ†·¥˘ØÉÅ—Ÿ–£ı´±∂÷ÍõÀòªö €ÍÀ∫„ö ı≠’Üé˛∫£æ°ª≥±ˇß´©Ø´±œ˜ÿ–◊Ÿ◊´ØÒ—µ◊¥◊ñ‘
Ìîˇ¬Í√Ò«èäœü ¡≈üƒ…üÄëÂüﬂ˝ï˝¯µÆ¯ΩüÆ™®ø±Ù ∏◊µ„¨õÎÈ®⁄úË€⁄ôÚ ûÏƒ¢·¿√ˇ˘ÌÌ’˝√Õ√◊¸àÔñ¸Ô
∫äÙ≠ØáÔ¬ﬂ™áˇ˙˝ªÓˇÓ˛Ì∑Ôˇ∞˚˛ÎõÍ∑Øøü—ûÔÇîú†Ó†¶§†˛à£÷∑ë‘îÍ∑ÌáÜÃ·¡ıÃÅÚÊ¯Ï´åæ©ü“∆ÿÊÛ¥
Œ˙ÓãÁÁ–Ç·ÄÖ∂ÿΩ∫«¨Ù∫£âó‡á∆ÓËπ¨«≥é¥‰Âí∂®ﬁ«ﬁ„®óÿ∂‹‰ìù¯úÅ¸á∑π°≥É‡‹Ä‘∑ààêÑè∏ß‘Á…ÖªÇë
î»áÜÖµèÇÆˇûŸ§·é€äº◊¯ÍÍá≤«Œˇüº¥ß•±˘‰ÉÆÃØ¯ÆÕÕåˆ˜ı¸åèèòÃ÷Ω¶£◊ë∑ˆËÀ¸Ÿèº”ã¶µ∂›ßç∫‘—î
Ñ∂ÒöÿΩëÊ±¶â©õ ±‚ì∏ﬁ ÕäÑîúàáªﬁ¸¨Äõ•ì√ˆïÅ°º∆ˇÚêù≥õ´≠‘ñ‹‘°ñÎäà‹˝çâˆ≠«“¯†ˆÇïè•ﬁƒÂÈ‹∞
£Ç•Óç“ÀìÜˆÍˆÊ”Çä˚—‘¥òÖ€úå“áøÑ≤øî∆Äë†‹êºóÃòï≠ôúùÃÑ‹áÒµËƒØê∆ù∞áÆæ˜”ç•Íƒ√ó⁄∂Ïå°Ñ÷¨Ø
ªÓàë‘·ÑÅ°¡ãíÊƒÜ¬ıâËÓ¶Ñ°Ñ„≤üﬂ•‘¡Æ•˛ãªßÏŸ˜éÆò∂Äé∫√¯ÔÌÌ”Áè≈Á≈üäÀøïî†ï°ó™  †Óˆ®≠¨Í¨≠
∏ÌÌÍ‘∑∑öçùΩ›˙˛æªŒ°Å„»†ÒÚÙ»∏∂Ñ±√§»Ô˚˜À¿¸Á„üæÆªΩß™≥•‹áÅ√Ä˝…ÎÌ±ÑàûùÕï∫∂⁄Äæƒè±©øßÂ
Û†ßôÄ—£ØÚµ∂’¬à—ÄΩ·É“°≤¯”’…—’…Á Î‚¢ä¬ƒŸ«˚˜•üﬁïº˚á•∫Ë˝€≈—§Ô√…ƒ«’Ï‰◊£–·ÌÃ∏¸ÔŸÚ∫è”Ÿ
öåÜå˛∞ö¡æíó‘äº’‡Û‹‡±ö–˛≥Îø˛ øÂØÑˇß¡£¸”òæ¡ò†‘”óÔΩ∑¢é‡‘ñÑØƒÔ‘Ü∂√®ö´˛ªºâ–ò•Åö¶Ë‹Ò
·⁄ÒÇ‹Æ¯¸¸– Ÿ¸ô√˝°úº‹í÷˛ß˛·‹È¥ãâ†ä¥£ÑÆÑó§ﬂˆü‹–úó‹ÉÖÚÄãÜÃŒ™™ˇ≠·≤ƒîõΩÜß·œ‹£∫πÎ–»î§
º∞ó‰…–Æôº¿Õ‰ƒ®·ı∆ºÕá∑∫‡˘ÆºÇÇ‡≤’ÁÀ€Œ„õ∂ˇìπ•ı—·ÅÉæŒñ√”Â›ﬂŸ«¶äœ£ùˆÕÊ≈©«—¡ıÅ´∞Ï⁄÷∑
§∂œŒ≠„∑ÑßúìÖ–ÿ«π§≤∏á—ª∫˝•·®∂òõøøå¡ÖÂ„¯™Ë≈ñ¢¶”®ÜùÍÉ™Äëê»›±ﬂÛ√å¶»£´Á‰∞≠≠ó«Ä‡Ç˘€
Ü°Ì˙º†“∫≠⁄‚ÒÖˆÎ©Å◊ã‹ €û˜†ºøåÆﬂ°àä´ÂâÕÓ“á’Ì˜ı´ç ∏†π…ºß¬ï–Ωñ©¿≠ûØﬂ©ñí¡‰ä≤Ã≠¿Ö∆Ñäàú
‰ÅÇËêøßÊºßÙ˛˙ÏöÅ‘¡π®Û¡à‹œ…üŒÔ∆÷Œ⁄ˇÒ‘ÇÛ›á∆˜Ÿ≤ª≤µØí∑ˆµµ∑◊’Å÷ïˇ™©©™Ë≠´Æ´®©®˛™Ø™©¨Æ´
ˇ©≈¿°•Ÿ–õ—⁄ÎÏ†„éÙùÉΩùÉΩÌÀÉΩÌÉΩÌÉÂ›≠É›ÕÄÜ¿˛˝˝Ô«ç–À¡ÿ⁄°ÔÆ√ø°Æ∏πÊ≥‘ÑàÕı•ÕıÒÂÌ≠’˙é⁄
ßÆŒ∆Ó„£—¯◊ÁË´Ó†›ﬂ≠ıïü´ Ä»éü´âéåàÿ ïêÔﬂïøˇ˚µÍ”ªñÚ˛œ∆⁄‚—∂ ØôÆ¶•°Ω˘£∫ˇË˚Ω‡π´…úÂ’‘íà
»üòì§«—√«£·Ÿ¿¸ˇòπ¿¿‘†∆êŸﬂî˝’–¡ç¶£Óï∏¬Õ∏˜ΩÊ˜≈⁄œ€±ƒÓÙÇœ†â°ÒÒ≥ºÒ∂ó«ù€èÆû‡—àôÄÅ·É÷å
˜œƒò‡¯Œ¢çÁ°è‰œ†£«é©«©Ñî∏ºÂîñé©´ÿ±ˆ∫˘Œ«Õ¯‚ﬁ‰≠Öë’≠∆öƒ˜€¢ÉÑ¸ˇ˘ô¯™≈ÃÈêª∫åû¥Å†∏˝®ÿû
Æ£ö»â˘ë‚Õ¯ó∂å√Ã‡£¬ﬁ≠ÄÅé˙©Î§‚⁄˜œ¨ÄÔåééˆı¥˚∂÷¸Æ˝¸◊Ω“ﬂ˛Ω˙√ˇ£Ù‘ó≠å∂Ì∫ÆçØŒÕˆıä˜’˝˛Â¯Ù
·¬Úí≥”ÄÄ∫ìäÈå˚“±ê¡¿·˛†∂ö«öΩ¿ÀÄé°∏ç≈ì ÈÅØ˚†¶∂µ˘•ì€´è·˜“æªﬂó‘«ÄÊ˘¢ë“ÙÇè§î†¢‡—òÒ‡ë
ﬂ˝‹ƒŸÅÇ≈ëÛ—É≤Ç‡Ò›™–√≠∆îî‡˙í˝æ£˘ÇÓÖùí∆›¨‡ ¬ƒ´µóÖŸå∏”∂‚´ÅÏÃÿàÿ¡‚Ì·ƒ™‹òêÁ¯óøË´‡ÑÇà
◊ﬁÛê—äé‘û£ã€¿ﬁÁ…Úô™ÂÛ°ú”öË»àÂ‘∞ùÄì∆∫¯íÁ÷±ªÛ˙È›ÿªÜÿﬂ–⁄››ﬂêﬁ÷’Ç±√ÒÄ˘˘È≈Â˘ıÄÌıÌ√›√Û
‡””áÜéÌΩø›∫·ÅŒ°æÙùÒ°Â†´©˜ﬂÿ–Á®Ô’ª◊ˆÙ‘Ù¥◊Ç˜∑’Çö†ˆ‹∞¥óÈö €àõ ⁄Îæ∑’êóµ‘ï’’ïÒ’‘îµ‘ï∂
‘∂◊È∫Íå˙¿˙›õˆ‰Í©ﬂÉ°∏∂ú†øâ√áÍ“‚Ñê≤‚Û—ÖÇ˘‚ÉÉ¢Ú„ªˇ∆“è˙Í¢¡øÊÚÈÆÊïëØ„÷¬®ÛœíØÚüûÂÚ⁄Ú
ˇ√≠™óÍÀﬂ∫ƒ›Åñ˜èò—¡Œû˝˛œ—«·øÚ‹–û·¢—©¬â€Ú’‚Ï÷Î˛ﬂ§˝çº≠ÿ„ãá¯Ÿ ÏûÑ«äËƒôùµ€πä˝Øù«≠Æ§›
¿œëƒ⁄Ä¢√ç˚“î‰∂‡Ç¶¬Ü”®¶†ËÿÚ±é¨˜•îÈÆãÁò·–Œ∆îÊØ∂ƒìô√Î˚òçéàÚÑ ≤À«âÆèÄ√πàô∞ÿßãŸÒúØ‡¯
èØì¸†˘öÅÖÜ◊¬ç¨Ñà˛úªÏäØÌùìŒ¨ÔçéŒò®ÙÌÌıïµ•≥ÔÄé•ø¸˚œ˝ÁøÙµ˝ûˇÏ¨öÓ™—◊Íñ◊¸˙ÎÂØÅˇ∂¥∫ÓÒ•
µ∏’È™çÎïø√‹è„À«ú¬æ® ¨ù•Ä¿éêËÏ™Ÿ¶ÇÌÔ„©√òﬂ∆ıõ´≤èî¸†∂∫Ó˘«„ì£ÅÖÍÂ◊Ïöñ‚ç•ƒ‚ÍÏ’—∏∑«€Á
§πﬂœïÙº—Ìô€¯∆Õ‘æ¨∏∑í“ùï¿ﬁÉÌ…¬ñ ˆ£¯Ó÷ëüáÔ„»¬Åƒ∆©–°¬Âí∏ô–ÔÚÄøØûöß„·‘¸¨ÇÅ¨°Œ≤üâ¶§
°˘Æ∏À¬ƒëíÈ˙ˇõß√´¶°Õ°‡‡Ñ∑üôÛƒ≥ÓË˙‡ÌÎÔË¸∫°ø©µ§∏¨‰Ûäôƒé•≈Âï”•ï’ï’µ’ªıççÕ≠≠Ìû∫ùÆªªì¬
€”ë°„‰Ë◊Âÿ‡«ü–°ôÆ◊ﬂ››ﬁ·∞Ï≠≥¥ïËõ¸ÀÙç≈∫Üœºùç≈Ìı≈Ìˇı≈ÄÜÊ⁄¢€¶Íœıïü´”Àﬁ◊îø´˘˘ÙÀ≈Ÿ«∫ö
ˇíÊ¶ ÷ñÍˇÊÜ ∆⁄≤˙¬ §√ıÈë–äÅ¢Ü‡¬ÕÈ†ÒÕŸ…µƒë¿êƒî˝¢¿ºﬁØ˛ı◊ÚèÔÀÍãø¥’¸ÍåÌ¨õÍÍŸù◊Ô±≠ŒüûÓ
ﬁƒÇæ‹˘π£ˇ·˙·õ∫›Ù„Üı¸Ìö•©ƒÕˇÀó†îáÂˇ›Õµ©ˆ®ãËÄ∑„ñ¥ Ø≈·Åâ£Ù®ÕØ•Æø◊»Ò√öﬂóÈøÔºÌÅ”ŸÃ£ˆÛ
’¨πíÎ«‘•∞ı§ÄêÿÚ∫∫¸ñüıï·Ç§ÑÁ∂ÒÄ±ü¯úéŒ®ÁûÿŒ¯¨‡îºï‹’û∑∏¶»ÏÏ≠˛ªÔû‡ˇø‰’√◊ÏÕÉ¥¯Äπ¢∑∞
ñùõÇ•Ëﬁ†∆ãÅÉ≤éí˝€øòãÃ˚ÀÛã£Å±≤–¶î≥–ó˜“‚àÛõˆ‹ãÎõ„Ÿ”≥˝∞ºÎ©˙ÔÆ©©¯Ù∑ºœˇ˘˜Ú˜ˇñÈ©¯∞–“≤
…›’Ωﬂ€∑ÿ¥⁄Ó®´Ó®©Éôò›“≥À´⁄ŒÖé®∏êë¬‚≈Åí≠Ãï”®ÅÊ•®—É¡üöçÖ—ˆ“‹Î˛à‚∞Üˆ€Õ¿•Ω˙©–Ñ©èıòö©˛
É©ÑÄ™†ƒŸ‚¡†¡Ãê¶™Ø¥≤’á˜•ÚÛ£§ÇΩÈí°∞≠ÿ∞†∏∑íÑÜµÈÎç£äÇÄ¡±Öƒê°ë™ƒÑ®Ôî÷—Çãé∫îÛŸ®˜¢¥¿≤ÙÉ
≤àèÒ›Á√á⁄ÒÜ±ó«äÌ…˚≈∫¿ÂÇÑ’à£ò¡¥¨ÅµÂÕ⁄ëàŸœ¯ÉãÎÊæ¡—ˇ˙˙Êíœ•†É“œÂêæÄùƒÍÎÍõõ€⁄„⁄⁄çÜΩ“°
¡ﬂ∏¥‡òô–¥ÉùÜ˙ö˙∫ŸÎÄçÜÊ“Äº¬˚˝Áñöœ±ˇü˚Ô¡‹»œ≤Ì˚ﬁØˆÔÂû±âôä≈Ã˛å∂ÃÃÃå≤‰•Àñ¨∞µªÏÁÊÔô˝Œ˚
‡÷Ø≥ã‘í›‰ΩΩÔ≈¯œâ’€ïˇ»≤ı‘Ã’ÊÔ≠‹‰‹≤Èù¥∆∆™ﬁ≠Ñ å©Ë£Åí¥∞“ê˙Ö£¶ìõ¥Ú≠ ÜÊ‘©»‹ÎÜê–∞„÷⁄Ë¡
úË¿æÉñ≤ËƒÈÉÎ¡Ä∆¡Ä†Å†¥Í£‡ ≈È‘∂£∆¿ÀÍìˆ‚§ÊàŸò Ó≠Œ‹éùÔﬁïäœØ∑üæ†ƒ÷◊ˇÌ∂âñ¢≥©≠ßøÛ≈éÏ“©
∫œäˇ˛˚ﬂÔº∞ÅèâÑåË∆¡ÅÌÀÜÉãœø≥·Ô÷ò˙ÃΩ±æÃªﬂ˙ˇ»ıÚ„˙˛ã∞πÃÊá †˘úÅ∞´ÁÍ·ù∞•ˇÇÌÂ·Ã“Ñ•∏øûç
àÙ·á÷ÛÖéñ˜®∆äº˘‹ÁÇµ…Áïˆå≈˙Ñù∏›Ûçòó‡˚ˇœ®ÏΩçŒñ¿Ü¿å ãÄÕòÀœÃøÏòΩ‚”˙ïÁÑ¬¬˘„·åîù¢œ≤Ç¡É
…”¡îÕÇÊÁ˝§óÄ„–ÿºÏÄË∏òﬂ€ô°ú›òûôŒÊ÷íÒ∞∑‡≤π¿Ã‡Õ©å âªŸ®ä⁄åπÄ¿πë≥Õß¨„ÍëÛËÃµíçœœ≠Ì¨›ΩÄ
”ﬂøÙâ˛÷ù§”Î™ªø∆˝˛Ωﬂ‡Ê∂≥„ê˙˜ãè‡ˇó´¯«ü£âºÁÅÏÃˆ÷ü˘ìÖ˘∫Öì¥◊î“çíÊ»ÒäÖƒºã°ÆŸ∫º›ûÛÄ‡ƒ∂Õ
¿ÊÜ´à˙Ò≥ØÄ¯ÜÆ⁄‹ÈÑáÑÎƒ¿∆∞Ω€æ‹ÌüﬂÖÚÑ¢Éº¸˘¬¢Ñ¨ÖÅÙ¶˙¿ù˛õÍäÅìû⁄õüˇ…ã¸∂˙è‹áÆõêÖã°Ü©ÕŒÃ
–≈Üìä©ÚΩ¯¶ èØóÂ˙ö—˛ˆÛË¸Ï¨˙æñÃÿ≠éëËª†¿êòÂ™±Ÿú¿˜Ö£¬ë¢ëÕ‰«‚π˛ÿÆõ œ•˝π§ªù±Û∆¸î¿Éœ™
íÛõÅó‘∂í∞¿¡¡õ‰ÑŒ†Ê–ÀÙ ±⁄§Æ“…ƒÍπÿ©£êÎîíµ¬£¢ñÀ‘Úâ∆ö©”ÎîòﬁÕàòµ§ê¡√°…¡Ï•ΩÜ≤¬—®≥£ßÇÖÑ
‹¡«Í¥∏∆ÉÖ©’î÷Ì≥ËÚºÑ∂Ê√á£ˇû˘°‘Ó“‘˚ÂÒﬁ˘∂â¸ÇâıïûÊÍ†ü˘ñ±ôø±’Ã®∏¬»Äãƒäø˛Öƒ‚±¡û∫Òå
€Ë»Â‚¸ÖÑ˙Ü∂«Ÿø˘‹ú§æÙ‹¶ˇâ∑ƒ«¶—£≥üŒ±àøÌ’¬ÕîøÔôí⁄˚ˆ‘„ø∫˝ﬁ´∞ﬁì«–‘∞ß∂Ë±≈Öù≥£‚∞≤òÓ¿ı—Ã
‚ñ˝ıçöˇ˚ñË¶‰âö«èù€ŸîûŒë£±Ä«ﬂÁ˚Í¿∏’ﬂ˛π™øû±¢—≤«‹¯‡¸¿á∞Ì∑∆ Ä£ß‚—∞ö¯¿ôçÖ∆ûÑú∆öπÕéó¯‹
ÌΩ¨≠ÔùÕŒ°Õåè§≥Ò‡Ú≤‚Àƒ˝—†Ó≥Ë¶›ñÉùâ‘⁄∞Ïı—–óæñµè«ËŸ±†±ì∞Æè‹•∂◊ñ∂ŒÚ˝õ∑è®Œ´…≤œÔÌÕÏÂÜ
Õı≤°Áé±√øŸôñ§Ï‡›Ñ¥ºÎñùÒˇ≈‰Õ‰ª⁄≥®í≈„¯ﬁ–∏È∏∏Àö¿Ç—Ö©¢∆ÀÇæ•¬˚Òﬂ†ÿÍŸ˝ËøﬂÎÎ˙ÙéË∏¿‹±‚˝é
éõîÀ ü¥ÅÑ¿åËÅÇöèÛèø¿æÚ‹¥ﬁìäó¿®∫‹˚ ÜÿÅöƒã±†Åû¬ï¯–ÑØ∞ÄÕ†¶íïø‡ÈÚõÚ∞•Äöò˝∆≥á¢∆µ…©ø¸
Ä∫´÷ˆŒÆó∞∑ˆÚ£„°»≈‹‚≤‚∫å◊Ô„ÑÅ§ìÕìÍÙ¯¸°»¥˘ôÃ‰≥…°¡ì‘Ë–¸®ø∏óòÚÙ«≠Òø‡ôÚöºÊµù®‘ÿÒÖôì”
¥˙©¶ëÎî¢€ï¥◊⁄◊≠ÍˇÓÍ˛ÈßÆü˛˛ÍÈ˙πØßˆ€‹◊£ÈÎïóä˙§¢ë‹¢¿÷ ë©î≤ù† Ó–´Ò≠ä®â’ΩËµ¨ì≠ÅôÑÄ†¥Ä
–êÍÄà´¡„√∆µë´æîÒµÃç•˜ó¶Ø˚é˜À∑Ö∆”ÔÛÛØøø˝ å∂ñÊÖ∫£°–ÿ†ˆ∞˝Æ¯Ù§√ñî®ñÌ˛◊¡°”Ôˇ≈∂±ÇÛ£ò¨§
§¶ëÒﬂ¸Œ™√™∂˚Â˙Ëºõ´ﬂ∏Ω˜˜ø˛ÚΩ¸∏æˇ‚Œ˚∏ÒÄ∏∆ï“à»≤ÁªΩ∑É≤˝—¬ë”à⁄Ä¿Åˆˇ¬„Ê≥ìÅèﬁÄ›≠âÈ‘√Ù—‰
´Ü¬ÄßÉ¸Êœ∞“ô‹í∆Ä¿ó‡£‚≤ﬁŒ¥–èÑÉ¡Ï‡ëæﬂËÎ∫ªöÇíõ˛»çÇ°ﬁÅÔﬁ‘Ê∂©ˆÜ«Ü≤©»Ü≤‚Ü£ù—ÎıÂÿ»ôÚ¿Ç
¨£∏¿—Œ˙Ì¬ÿ“√ÃßÌ”™Ò”ƒò‹Í˜‹ÌÕ•ˆ÷Êé≠ŒØôë‰Ï¥≥∆÷êˆ∂•õ‹≥çÛ¨⁄©‡ú¨‹¬¡ª¸≥◊¸÷¥¨¶ü˛ñ«ﬁƒ›‘∆è
®ûÛï€¶„¿äÿîﬂÄŒò°óˇÇï†‚¢…åæ∞Éïò¶Ωº°≈ª∫Í¯ˇß ©˝†©Éºë’ÓÏàèæ™∞ÓÉ†√Å≠≥ãÄÙ‡ﬁπÈœ∞≠™ô¸‘¶
⁄ËÇÕãÖƒ¯õï·Ç ïÂ≥∏ÊÀÏÀ¨∂ì√≠˙Àõöòï”öüŒÔ €á∂Á≥ÀõŸ‹»∏∞îÖÄ¿ıò¬ÛæÑÅ·Ò∏”›ﬁ‡à∏è±êÏ∂±ºîöù
Öéè˜áÚ…◊¥∂‚ä˘Ò†√ÖÔ∂©ÌÑ√á∆ô¶ëËï¢•˝§◊“€›€ı∂≥†√œˇ˚üÊø˝üˇÙ˝Ù∏Ûœ◊ó®ß«˚‘€´ÓÎïÙ”ı…Ä£ƒ¢Ö
Æƒ¨ñÅüëô»‘Ú‹£ƒ‚Í·¶°†Äö¨Çà”¶äû≤ÅßåÇÇ¬¢◊ª±‚Õ»Ïúà∫¶Êá‚õÛ„ß˚˜€Öõ¸Ì·ë¥¡√ÛÛ´áﬁ˝ÉÎ∏Ñ˙©≤
ñæ≈ß÷áç÷¢ﬂ¯ﬁôˇ∞ÕπåÅÇ¯⁄•ø›∏∆˜ÃÅíì¢È¸ãåÉ±à∂¿øΩ˝⁄¶ΩÓœˇã«««€Æóü◊˜ﬂ◊πÇÅ‰∞ﬂÊùπÃÏÁ“˚ôŸ
∂„Ãà±¶≥Ñ¯≥÷ÏÒåç’ÑÇÃÄ˝ÄÂ´ı˝Üñ™ÿÍ˜ã—‡∞∞¡¡¥ï†Éä¸Ìì‡Ãáÿ§‹Ë∞¯ì‹ñê†ò©‡˝‡˚éÀÏ®«†˝˛Á´ãÜ¸
ªïÖ£Ωû·°ÒÿÕÛªﬂÿ∫ø¨≠ùñÊ∂ŒËá∆ßΩﬁáﬂ¸„ã˚ùÃƒÆ†Ù≤îç∏ÇËÙƒ◊ƒÎé˘Ãº°áëÚê≥©¥õú◊ò•◊«ˆÇª”…¿àì
«£˝õöÏúƒ∂Ã›∫¬√∂⁄Øï∂®Ã∂ˇÊ´ú÷ı„ì‡ÇÙ≥øãùõı‘äí”ûÛ£Ô¢ˆ”—Ãú∏°Ä‹àöØ…∫ïœ‡º¨⁄™˜Íòﬁõë¿éÔ‰é
õ™⁄±ÒÆ¯ú´ºˇçÏ§†◊¥æò¡∞¢Ã◊¨‰∞ÅÄ«÷”Ñ¿â®ﬂä‹±§µœá¸ùâ„ûƒÅ´û∏˝Ëë’–ëÁ÷øµÍí¬«Âπ≈ı¢˚¡Î˛˝€
Á∆À…ı˘Û‡ËÙÊÒçÅÑ’ÊÏ√πÄÑâõÔò¥œüà∆¬¢ôÙÏË∞âÄºàÄàÁ¶ÏÀä˚‰¡ËÏÁÑ“å©»ù‹Á†í™Â¢ô®ˇ∑Øª´ÁÁü
»Åó˛˛øø˝Ô¸èüªˇÓÍ∑ÌÓÜÙËÖÙ‰‡Ä°êÉîàƒµ≤âﬁ°µ™À‰Ñ∆¡∞Ç”¿®Å†Ã∆ù†«Éùƒ†£∞£≈ŸËÄ·àÙ •ƒÄ¬·–˚È
™˝∆Ôü»á◊ºßáè∂◊Ú◊∆ó’∆É”íÇ™⁄∞Ú≥ò∏Œˇ≠Õœˇ‰˚ÚøΩÀàœ≠ŒõÆˇ¸ÜÊ·°∏ÒÚˇÉÃ∞ï—ÑÏ°√ˇÒ…Ç¸àçø˛˛È◊
∑ˆ¶«ÔıÀøåôΩ£ûÏ‹Ô∆‰®è‹ØøÆÀÔøÓô©≥ÌŒÑøÕÊÄõñ‚Ãî°”„◊÷—ºò‡‹ÄûÌ§ÿ¸å“Õ¡†ï˜ﬁ˝·Üè¬Ä˜˘©∞‡
åÁÄõ¿É ‰˙‚Íﬁ¡ß®‰œäùÛ¬ƒ¢ÿΩ©Æ‹¥Øˆﬁ°˝ËÙã¨µ˛´ﬁ…◊óèÚ≥ªÎá”ãõì≥ûã£„ìŸÏ¶ñíÚ¿˘é±ˇÙ¶ï §Éâ®
Ö«ÌßúÊ¨ö˚∞«Ç∂êõÚœ€Ò®ìòä“Ä√ÅÚ∏ƒÔœ”«˜Á”≥È˜˜˙‚Ã…˛ˆ™Ö˙¯√Õ‘¡”¿‘‡È≠øå•…àí›¯È©◊ÖË˝ÿ¬ü˘ä
√Ã®¨˛ô∂ÄΩ–òîèºâÏ√µ»ÕÎÍÉã≤åËŸ≥òÔˇﬂ®˝ƒåü∂õÜ¬ô–„∏Âñ„ﬂ≠√™ÛéÚôÏ·‘••÷¨°’˚∑Òå‡Ú≥öò∆‹≤ô
¡∑¸›Ü¿É££Œûõåù‡Ñ≈¥öö˙÷«åíËΩ∫∏∂ùèËÙâôÙ–¸êêú∆Ω∂≤Ê∞´œ¸´øñ™Õ∞ÜÁÂ∏ﬂ⁄Â‹ªôŸÿ…ˆ§èáÉêêÑë
√“Å∞“§ŒõƒÕ÷ªç∏ëûÑ∏ò ∆ÜÛÔÒë°ÿ–÷è–Ä¡£¿¿ÄÑ•ù‘õ±•°”ÀËî“Öƒ§ÈøÌÈ˛ÈÁÆˇˇ∞Í˛©´ß´˚ªØá–°è°
ËÉèàïƒ¨ï…åä©¶íìÑöÖ√àÃç˛òïÑÑ¯ß∏®¡‹Œ¿∆†¿Ëˆ©ùñÄÄ–®ıÕ∫ïÕØÚíß⁄î«≤ö˜Ø◊ç§˜∑ñÆŒˆœÀµÜ◊†˚
‡◊Ê¥É£ÿ¢ÓäΩœîìÚ”ß˙Ññº“´«»ì‰ÑÍˇ•‘ŒÔøˇõÛú˛Ü‡–Ç˛è™Æº˘∫˛©‘ìå¥ó°¯˜æÚﬂΩ¸∏æˇ‚ÜÄà∞ìÑ‡◊Ã
Ì∞¯ôëÅ»ßã∏∫«çΩ—áËå˛¥¿ÆﬂÅÉÁ˙Å¸äúáœ˝˜®≠õÄâ≈ô™≥ÖªÉ≥€ΩôŸè¿ÂÓËº˛Üä´·à√Öﬁﬂ∂·∆›Îïó˜¬œä
¡‰€±≥Ù‡åü˘≥ Û—Ì˝ìÛÙÊ˛„‡Ù¿ˆ•≥¥ôÀÂ‚‚±‰¢öÀ§Í‡Ô˝‘¯ﬂ«œ¥‹èÖú¡èôŸ¬õ¨¥±ëñˆ∆É∂Øù˜â–Üß•èŸç
äëõ°È¶ñáﬂ˝´√˛ë†ΩÄ∞±¯¯˙ˆÛÖﬂÍõèﬁ∞Ûäœ–»ƒ÷À¸±¶ôæÍÉœÕÒ»í∂…˛ıü∫¸ì‰‘Â˛√–›Êûê¬É∫ËÌûìú€±€
–Æ¯∑π«ÌüÆÎ÷ ˚öÇ¿“∆ª™≠ù≥∫‡˚√ê°«⁄≥Üﬁô≥¶ñìﬂÄ—´Øóº§ú⁄†∏©…üª⁄îÙ∞å‡’ÅàŸñëﬂÑµïÂ˙∑•ñ≈˙Û—
®ÂÒÌ˚êÿõ—ÈÃÆêàÿÕ‡ø¶âåÇ¬¿ÈìÌ¡ŸÃÆñ‡–ó»ÑÚÉØ±ë¡Ë–÷∑±êÙ†¿¿º÷—ü’Ôß¡˛Æâ¬¥äµ‹™óÌâƒ›Ω›ˇË
ˇÇ˙©´˚ÁÓüûÆûæÓﬁë£§É»ä§Ç“ßà≈Äéö©ò€–Å‘ÀÕ˙•»É”€çöìå¯”ÎÈ£ﬁÇ‹†áßÕÄ∆ÿ†î£∞»àÉû∏åëî¿áé∏ë
Ï ∑ÆâªÈ°—à˚Æó€Ÿô∏ªªπæ≠÷Î»åî‡å∆∏˜°ì•Ö◊™¬Ù·¬Èçı˝·√‰ì»¸Ë≈¶èÑù∑ÀÙ ‰ÀÇ‡®Öå’ö£¸ÛÎ˙Í”Ô
∆¿èüπ˝ÈÈæ£◊ÀèÎ˚Ø÷·ˇœ¶◊ó∫ô∆–ÉıÇÆµ†·πïç¯Å¬Å∞—õÇÑ”ëÁôÏƒ¿ãÄ’ãë¸ûÄâ∞üÆœ·˘Û´◊…äÖõ‡†Ø„
ûó‡“Çáç¯ﬂ¶Øø˝—¸˝¡çâ¡≤ñù»‚ÆÎüÄîÑ˚≠òœπˆÀ«ˇ∑çç„Î‹¯ìúÀì∞õÏ§ç”Êô†ù∂˚¸¢ûÅ≠ï”·≤ÙÃïèö√æ
Ô⁄Î¿Ì¶ΩâÉ‘Ÿ≠¯∫äÄú·ıòçÑﬁÌ¯–´®Ω“Ü´˘˝¶˝÷çç†Îˆ»ã„É«ßÓß’á«Á¥õˇãÔÜÃÙøﬂ˛◊Îø–˘¨Û†„Ò˚£„ﬂ
ŸøÌçø∞›õåˇ√Ìã˜Óí‡ë†ˇ §îÔõÇÀùªÌÒ®ËËªÜ‚¬ñà‹∫ô¢ÉÇÑÕ‘ªå∏„≥ﬂÇ∏¡–¯´Ú°åÄƒÎî…àà˛Ù÷Ñ ˙æã
¢ë≠ÄÑ∑Ò¡é†æâ”˚ÍÖ±Ü⁄∏êàÈ¥çÚÎ±ˆ¸¯˛«≥≤±ª≤æï∏»‰¸æ†≈‰∂„È¿É¬ Åò¢ûúËèéèî∆Ü≥æ‡¡‰Ó£¶ÑüÑÄ
Ç⁄ı∂“√õ‘≤∆µ¬•ï ƒ–‚—˜ﬂ˜œ›®¿Ωø˜ÄıÇé∫ÙÙâ˚Åùßƒ‡´¿ÚñéííΩÃ¬™Âˆã£ œÎ‚ˆº÷‡¡›¶¬∆ë∂÷ä¥—ÑÉ¶
Öú¥∞àªÖÒﬁ§–ÕÑëÜØò–ôÙöãﬁ£Âƒ¥—áïãÈªÊá¢€À≥Àß««≈ çÖÅÔ°ù™Çâ±≠¡∞¢…ÙâÁ»∂Ûˆ–õöÒ≠ãõ˚ÑºÉ
±¶çÇ∑∞âΩûÕ‚€ò¿ÆÿËÏÆô◊¢Úµ¡’¥íıò∆ÕˇÁŸéÙÔàô˛∆ÒëÎÂ«ıΩ˝ó”ﬁÌÅÎèπ˘∂·ÁÖÜÓä·È”•◊ˇÉôã©√õıì
ÄçÖ∏®Ω›∑Êﬁôﬂ–˝˛ïØ·¬ç†ÀÁ≠»ÍÅÅæäû≠≠ﬂ˛áÅ≠¿à°Ë±ù∏ôßÔ§Ú¸ÀÕÛÛ∑Ö∞Õ¶Í“¸î›ô€ﬂÀ‰°á›ÓøÕö»
øÿÙÄ∞ôÃû—ß≥íéÙÁ‚Ã√É±¡Ùê∞Ê†Í£ÉÆ¬ÔÑ§—ìÃÀ§‡ÄÕ≈Ï¡∑Â¶⁄Ù§¡ãÎ¥®éô¶ ÜÈ“éÿÏ¨‰ûåª˜æ∑óÓõÕóﬂ
˛ÌÔ˜â“¬πôàùì≈ÒﬂﬂÔ‘Ñ˛¡¶äÀ√≤†∂Ÿ•◊∂ﬁÊÉÅ—¬¡áÅ≈¡˚Íõèü«∑ç‚ÈÑõÈÙÃ—¡ÄîàŒ¬ò∑¿¨Ç†Ó∫Å°å
ƒﬂâ≤ﬁªò¡ãóÚ˝õî¬ñçî∫Ù¯≠±µøòÔ¢ÅâïÉŒ¿ÏüÊó◊∑ÏÚ√£—Ë¬Î˝Ò€Ë∑Ä∞ÒÔÄËÔèåÁ¬πÉôàà˛à•ó·ß¬Æ‹Îµ
∏ÿâ‡ˆ´Ù™˘Ö“¢ìıâ≈†Ω¢ñç›Ω¿Ü˝›êéí»áÅâ‹¨—†Å∫…Ë‰í£¬‚—∂©¸î∂È¿«ÉÇÊ¥¯Ù›÷Øè›Ê¥⁄úÊÉì”ƒ∫Ä†Ã
≈ÑÉº∞—∂ö◊¥ê∏ô«‡èÆ¶É÷¨◊Ÿ±≤ıƒ¡éÑú≥ˇ˝·Õ•ô„£õ√¬›çüô˜êßêø≈éü°¿áœƒﬂˇ„Ñıñ°õ∂¸¿€ÁˇËÄà≈ñπ
¨êÉ€¿Ñµ∆⁄¸˝ßˇ„ç∂§ø§◊€ò’™≈≤˙Ì‰áØ˛˘«øÚΩ¸∏æ˝ˇ‚öºÒç›ÕÑ≠›òîÙ¶¸˚õ”€ÛíÎÕÌ˘ µ˝Ê∫ë¥Å¿‹’æ±
ØÂ´Ãº‡Ê∞ã˘ˆ¡ÿÈá¶ëƒÇÔîé˝¡‹ëÆ‡•Çß≠£˛˘∑¶ÿ∆åÎ£áß…Õæãˇ⁄Î’˝”“ˆƒ·ºï£â≥¶·√ÄùÃÑÈ∞¬⁄Éæœ¢ñÃ
•¶£πÏ§óéﬂÙ«‹…†µÏÿ¶˝∂ÃÁ†Ç®æÎ¢ÍÃ≈›◊±ÄŸÌ≈®£èÜ∆Ù¿·áÓ¬ªÆ——⁄’ÃˆÊúà∫ñÜ¢‚∑óßåö˛˘çÔ∑≠ÃÑçá
üæ⁄ãÊˇ¡ù≈…–√ôñìàÑØü‹¸Ïóï∞Â∞Ë∏¯ö‡¯Ïà÷ÛÌﬂæ†Òú⁄ÙË”âÁØÚÛ˚‚ì£®∏Ã∞ıó¨⁄à°Ï¡˝Öﬂ¥Â¡≤ÇÎè¬Ë
î›¿î‡ö∑Ò˝ˇ—€–°ù¸é¿Ö˚ß›„èãπ≈’ÙÊ†éòå∏úﬁ≥ÌπÂÅ¡‚îâ∂π€††ú¡Ä∑ƒûèÙÁ√√πÑêÇÑÎ†ÍÕü¯Èêãÿå
ÿñ‹ÀË‘∫Ö‘¬°ñ…˚§‚ÄûÒﬂ◊ü‘∑”î®«§ÈïÉ©˜ëù§ â‘Ü•ëÍ‰“Ã°–•’“ó¶ ÿ´ç£¡ÛÚ∆ΩŒë˜Ùá˝·ø€ﬂÎÚªÌ÷ß
ùôÚ–ò√Ã∏ñ≠î”Í≤É˙ëô®ã≈òâ£òÿí¶É⁄Ê‘„áÏƒ∂Ëü ÿÜ∑ñ¶éé˜ˇØÀ˜≠ˇ‰Á÷˜ƒÃ√º«ò˝‡’˚ƒ™é ∞Ã∏Í¿˚üø
∞£¬ì°Ÿçùøû∏∂ÁÉÒ√§Ôô±ù£π÷ç√ıÆ·∞ØÊ‰å¢ÛçøÆøÕÁıç÷Èëı°ÃÈµÎ∞Õ—˚Ø˘˚û”´Ù°èÛŒï≠¥ôÇÉ‚öüÅ–
˚™œùÈãøñâπê·ÌÖ∂ÌÁ˛¸€üÃ·ˇÄóÿ±áÎ¢öÂ®˝«ìÍÜÄ≠öÔ¸¬∂…Èà¸∫´’Ü∆ßëÕñá◊≤‰ı»ª›—Ë∏ƒ˙‚”˛Ãåˆ⁄è
Ÿ˚Õ±‘Ò¶‡ﬂÑÜú›§≥ë÷∫Ø€îÚ€ΩÊé‡§õË£÷„å√ÃÚ⁄Ø√ËµÍ¬≠Ñæ¸ñË‘ππù´ó€Ø⁄⁄÷¸€–√‹å´¶Â€ñ€Ω◊¸ÜÈπ”
ÌﬁÁÜÌ∞Ûë ∑ëÈÓÅôÒ«∆É¯ö†é⁄®ÅÅô¡·¢√¯Ç‰†œ¢‹°°∂Ñ⁄æ»¡ªƒë¸±ûÀäÆ«ûÇÔá“ËçÄç˙«≈¥ºàË•„˜˙é∑ﬂ
âÌƒÆƒ€ßö¿ﬁê∫ÆºÓ™ùúÕÕ≠«ìÍÈüØËÙ∏ÕÅîÖºÏ§ÉàÅå≤—¶´úâæ‰˜˚‹±®ØÅõâçç¿¿å·¿¥◊Îå¯≠∫é¬ÈÇÌı
Ö≠⁄ß‘™•ÚÒπ»ä¿”Å‰ÅÑÙ†˙≈¿ßπêã§ ±ÑûÄ¬®êô¥ÚÅß¡ë±Èáº˝‰‰ƒ∂úıçÏß¸Ÿ∞ø‰É…ÄÄ˛Ñõ¡›ÑªÕ§√ÃŸ
º≠¶õô∂ò®Ã≥Õô°üÅû™Í≠ç◊ÜÌƒÅ⁄ÔôÀ¯÷Êæ«˜˜∂Û∑˜ﬂÂ«œàåﬂﬁ±ı†òß¢⁄é„àñæâ‡Ò⁄˙ÈˇÉßÉŸ’¡Ôﬂ˛ïÆˆ∂
µˆ–êå–Í∞áœËΩ’π˝Â‰˙˘Ò˝ˇÖÈ≤ô—ßã÷¿œ˚åƒˆ·∏´π∂˙ØÄËËõ≥¿ƒôØ–˘Çµöò∆∞µò–’‘ªƒÙŒË‡Ï∂ÿ∞·ı
ã∑áØÏπß≥◊—æäö˛≥∂  „ì€∞ÅË«ÚÆ±˘¯ñéÊ¶ô¶áñ≥Õ«ÎÛ«„ë¿á±¡Ã†ï∂Ü‹óòœ¶˜“öãÇéõò‡⁄πé≈õò»¥ ôÅ
öÈÕî””ÜˆÑ≠∞¡“££Ì—–ëöç°÷πöâÌ–ÿµ≥øæﬁæΩ˘¸Øˇ‡ˇ¬ë∑ÿ¬√è’ñ˛Úª€“ã ˜ú¯Ø¡çøÈøˇ˚øπí˜°Øµî¯– 
––ÊîÖ√àÑ¸â÷˚∞÷˚ÅÎÖ≈≠Àà·¸Ä–°†¯Ãú˛ÿÕ¨≤⁄“ÂŸÇäÓ∑≈ñ¸»«‚¢Ú˙—≈‚ä™∏€çªê£ò¯Ã∞ÛÉÓÑ¨ì»Øπ¶Ñº
Ó©¯Ñ•‚´€Ëﬁ€òÇÁ«ªê°âÆ™Òã´Òî¿ıı˜∞±®˜∫æ≥òâ¶‹Ωûø√ÊÇÄë†êö»åÑ≤Ωë¥”ƒ’ÁÒÖ∆¢ﬁÏ∫àÄ•ëÀ˚ÜíÆÃ
Õƒöı“˛ª·ƒÄˆàÅÇ¬Øñ‘©§Ú‡£»à˚•¢¿ÖﬁΩ–˝‚¶Ü—´íÉâƒ®º≠°‘Ç≤ª¡ë†’ô±á·§¿œ∆ùå´ÉÅÒ‰†õÒÒá˝˛è”Ö
§Ä¡ªê†ÏÊë Í∂ŸÄø„Å¢Õ¥ÌÇŒÑ´âÅß‹ŒÍÉÑ∂¢·µç≈ùﬂãìÉª€õóˇÖ€“√ßÚ„èÎ›„ÁŒÆº´ˇ“ë∂Ê≈Ô˚ÚÌﬂ∫Á∆Û
ã±÷æáÜ˛ÔÙÁˇÛ”´ÏùÂ”óæßœ´…ë¸„õ¡ÓÑøˇÚëÎÂ«ı˝‰ó≥›Ü¨ˇ„òí¥ÖπñæÏ§∑ûæ≠œ«·Ì≤–≠˜Îü¿îÚÜñû◊„ç
‡æÔ‡ôñ∏≥é¶òµà≤≈Ô•°ÌÉ ﬂ—∑√†ˆà‹„ÑÆ˘–ïºÑ«˘€Îâπ•Ø∑ÄÎ≤±¬»πùû¯Ë≤±¥òö…Öù˚„∂ê”‡£Î‰¥¡™◊â‰
Ô¢ûÓü˛˙«√ˇ˛øΩ›øãú˙˚§®˛»≠°ìò‘äÃ£Çãéøâµ¨∞ô∫·˙á°ÅÂ≈‹É¡◊÷¸ƒ±ûÛÑ˙º±Ò˘ê«ˆ∂—øò˚¥Øøˇ˛À
òüˇ∞òÄ·à›Ê®Èø˛õçÀ√ÉÅª˜ÎÇˇòü˝»õ¯∂Ä€‡õ©ò∆¸¯ü†é˘Ÿ”Ì˝é∆ÁèîûŸë¥ÿ°¶∏≤äÍàãä™ÛÅå∂¯Ö™¢°˛û
‚óÌÕÖ˘µ⁄ãå◊€∂áˇûì≈á≠–¬“Î›àÉ◊≤ªïÌñí™¯Ì°üπêØµ◊˜é«¶˝ÁÇ°òÀÂµÅ¡√ù≥øÚöÌ•√ÄœÈ´…ç‰ıŸ£°èÄ
·ƒ†Çƒˇ¬ïîé€√û⁄Î•Ø¨ÑÅÇáòö≠É“•ë©î‚ıôåû≤âÑÄ¯‡ò°ÈÈìıÅâî¢àà⁄£œ˜‚™§’ªÍª√ß£ñÖî˚¿•óöœÜ≥ë
Ü¿¿Î˜£¿¯«¨¥«ÃÄ¿ÿ£Ä„‡‚Ø¿á†ì¢®Ç∏∫ÅÄÕ˛õûÛ”ç˝¡ÛÏÿˆÎæ„ı¸˙¶‘¯í∆åËÅ‚çö»ÿäùıˇ≠«¥£§¢∫™
‘Çââ†ÃôÊÅãŸ∫£ƒˇ¶©ÊŒ˝‰˙˘Òˆ˝ˇ≈ßÃÔÒÆ≈∏‹⁄ø‰∫π¸ü£˚êÔÖƒß¨í‚ﬂ◊ÿ˙¸ï´¥‰ŸÆíªèÃ•ÏØ¸ØêÒöôµ…ˆ
Ò•Ë≠·¶÷âùØ˚ÂûœﬂèúØã„”ÛÄêàÑ¨Ÿ¢Ï÷«ò∞îúËèèè˚Ñ˙„ñ∞ÀËÙ†’§ﬁÆíâÆ˙ª∫∫˛Çï¥…˝›Ω‚ﬁæŸî¿®“ §∏
Åö©Õ’Ÿ∞¿àìå¸§Ù⁄“´£∂ô‘…∑Â‡È¨˚˘Ò÷‰èÍâÄ¯˘‰¸∂Ê¯Áˇ±Â¬‰¸˜‚ﬂˇˇ˜åÁ¥ÏÚÌëê˜õ‘˝´¨¯¯ü»õ∏ÖöƒÌ
Ø√Æ„¨ˆêˆÚÔö‹¯Ç¡ƒ˜√˚ßœÔ©Ì“ªﬂÁÑÁ”îÈﬂ„ÑÃÁ¯‘€™ÀÓÔ„Ò…ˆ˜·‡Ù√Ω»ù∑∑Ù»∑¸ç€§ˆ¥◊é¸◊„‚€úß¡ïƒ
ò€∫∞…±Äêò´Ç¿ÿ≠ééÏê§Ãà∞≤É˚ßÌñ≥ﬂ„àπÖë†˙É˝…ÑÔ¶ÊÃÑ¨¡¢ßÌ∂ìñ‹¿ÖÍÙ∏∆Î–ÈÕ¥ÚﬁâêÀ†∏ƒ»˝å§Ω˝
≤Úë»©Ü˘À¬ñù¢»ÈÜú„˙…†µùµÕÆ¶‹≥¿ΩâÖ¸í¡øœ„Û∫ı‹Åó≥Ï∂Öº›ñü√‚≤Ÿ‘¨Ê‡¿ò®ä§¶»ä§…‡ƒ¥ÿÈ‹Â¡Åã
ÑìÒë…üΩπ·ú⁄ﬂ€ºΩº£∫≠ΩÇ∆îüËﬁª˜¯ˆ«°êçáÔíΩ∫ ﬁ»⁄æ ˘Ö¸ëëùªäêÈµÌœµúˇÅàˇØÈó∞∫ ò∞’œ§…≥»‡˚
ˆÊîüªí·œΩÔﬁ⁄íﬁë´ô¡˜¯Ó»≈·“Û⁄õ≠´œ∏ÿÚë£⁄≤ÕÄ õ≤˜öüöôÁ∏˝Ã“∫≈íÏı˙ΩôÃ‰Ïà¨œò•‚∆™ùîñ”™‰¢
⁄Ò˜ø∫∫ˇÍœÓÌÍÅÆº ÿ°‘À≈§⁄Óä¶¥Œ“¿„∆Üõâ√ÃÚí∑¿ô°£¨˙’ƒ¶ŒŒÙ«ŒèèÔÖπ‚å√›ö˙∏√µÆÅ”éÒÿ÷≥ˇº¸˙
¯ªìÆ·ı»ä€ÕÓÊí˜æÚê¶Ó›∑êê£ÉÆ€òÅ±—‡≠‘ˆ˙‘∫É¬…˙∂¨ﬁ˜∫¯¬∏Ö†§ÒÚ˜®¨Äƒ€Ÿ”õ‰¿¿ó”ààõà⁄€˚ÿé¿à
˛òÒÆ∏ì®øÚÇà¢áà’›œì—©Ö–§·ñˆ˚Ì∂‰¡ÄÀﬂ¡»ê∆ú»ÿÃ“∂≥ŸÂ∑ƒÖ˝Æ∏∫‚ éÂÉôÑßôÒ∏∆˝‡Î¶ú¶”¬åô…ùû◊
ÎÍåÍçﬁ©÷Û»î™îâêãê§Ã“´ö¯Ë¡∫»ƒ∫ôƒ£é¢è√Ó„Ùˇàûåÿ∆£ÆìëòøìÚ≈˝ŸÄéæü€õÒ‚˘í¡˜™Ëä⁄¯ùﬁß›Êµ
ß—¥ô†®Éëı«∞åÜè¥ô€Ùÿ®øÉäπ«®òáöõÃ¨ﬂÆÃüúüóÔæõ•˙”›√Îøµ›˛ÁºÄ˜«ºÜ∑Ù‡€ØÊøæÉÉÓÍüòäï†√ﬂÄ
Ëƒ«¢¡«Æó∆ü◊˜ﬂåî⁄∞Õ«∂çíœ˛€‰ìûﬂ¶ªÚ∑Ω‡¯’œùâÖ∞ôåÌ…©‹æº±„Ÿ‡”ëŸÖ˘¶’ßﬂ∑¸èŒì©¨”ÓÇÊÚ…Ä†¸ﬁ
êœ∫ùöÍèå¡ƒÏë∑·£ÅÎ¡¶Ñæ¸ÔœÍ∞ì–‘ÉìÃ‘ï®¸ÁêÀ›ë∏˙§‡Íè∫åΩŒóÀ¬ªÃÀÑ”¬€Ëíéåﬂ±®É÷¸™¯ìº∫‹Øº…
ÒÓ‡è›πö”∆¶ ‚‡ä≈ôó∂“ıœå–‚Õ∆≥ìõÅè«ÁöüÿÏ·’Áèºâú††∂‰˘Ãº°≈À≠…Ä¬”Ó⁄ò†∏®˛ùìÔÜìµøäà‘‘÷
¡ÿø‹Ë˘—êﬁúÅãÌë¶ÚÓﬁ∏ë˘∑ÄÎ®µÕ£¯ãâÆ¬„µ–∆äø¿áÑº‚™∑◊Ãˆœ∆ßó£—Éî◊ì¿±à≥†ÀáÉÜ‰°®ßá≈‚ı∫¬Å
åœ¢ÊÎœáÜä≤ÅÙ‹ìèπ∏¬Ó≥ÿüÌ¥å·ÇÄ–√¿‡◊»â–©‡¡ìò ¨”ä•¿®Å©’å¨ô†Ö—∞ﬁÄù§í£“‹‚≤™„ñ›˝®èüâò∆√
Á÷°ê≥ìƒüºﬁ†€Îˆ˙–Éﬁﬂ¢‰üà™áÜíÅ‰»óÉÙ∫ù∂ßÄÑãÜì⁄ùÅ‹¥õâ€≠îﬂÈä¶∏È†ã∞Çú”ÓèØØÕùüÈûñ´µ“˝Ò
ÉÓ·ìüæÓª¬†˙≥¿¸Ì∆éﬂÈ·ÎÍ˛óÚ†ò°÷≠Âœ˛µ®˘»ıÚ„”˙˛Î«ê°…ëùßºˆÏÜ«Ç•¡˘ÇﬂÀ•ŸËØ˝ ûæ¡Ïÿ§ì¨Çñ≥
∞∞¡≈∞û˛ÛÌ«©Òõîî÷ûÏ‚’Ì√‡®ÍÍ≥∫ò°ôé∂πùé‘≤∞üòı˜«Ï¡ßÜÈ∆¿‚‘ÅªµçøÊÖæºú©∫è“ªﬁëÒ˛òÁìÆ·òﬂ
ÆãñõÕåÃºÇªÃÄÅÆ∫‡˚ÑﬁÔ≈á¢é€›Ìç‡Ü≥…÷û¬ÏéﬁáÁ¿≥Äõ‹ ﬁï∏øá˚Íåª±¸πõÃÎèºâ≤€˘∫“É—∂≈–ıßˆ⁄ÍÚ
∆Ç§ÿ√í∞Çá’°êó®¯ÒŒ®–—Ì„´«˜©ÃÍÑµ∑Ö…ÌçπùÅ’ûÎŸƒ◊òã¡Èø˛ÂÚ˛ÂÍû–ç∫ˇÉàÎÿÁóÉ—õ¿¿ÁÁ™•˜ò°»Ä
ÆÍæ©∞êÜßª∏ØﬁîõÎ˚—ßì≥À±¯⁄ÅÁç∏ÌÆãÈ·¢†ùê†åò˜ï¶ù∂´ﬂ„ﬂäÜº…òØõæäÙß¸âà´Î‚≥˘Ì¶ßôÉ¥í∏Ì¥í∫
¶É°’„ö‡ƒ∫ËÅì°ì´¥Ç“Ö⁄™◊ÀÂ„‡Å‘Å…ò‰Ù•‘˚†áŸ≠‹û‹ï∞Í∆ºï¯ƒêûÙ€“úﬁ€»ÂÙª›‚ûáã™‡Òé¶√Å°‘Ë¨
ÆõÈèá‚°†Ø¨—∞ÿú“ËÎÙ⁄ùñ¯’»éÄàóô∞¬„Ô§¸é≤óç¥≠ÆÃûùúîŒ´˜•⁄ÉÄƒÈ˝∑ÅÒ˛†˘œÈ´Œ¬˜ÄÇ¢”˚◊˘ﬂÉœ
±°Á◊ç˜◊ø˛ÚΩ¸∏æˇ⁄Œíÿôª‹¿Ûƒõß“≠ˆ∞˚ÑÉ˜Ï¸™∏ÍåÊÚ¢ÆÖÜ°ÃÛ‹¿ûíàè·éªŒÉ∏Îé∞πúﬁÿﬂÛ¯›É÷≥ù¬©“
Ë»ÄœﬂÂ∑¶œ‚˛Å¥øù∂ö˚≠§•◊ô’≤Íï‘ÆÈÖı∞…‚îÑîã‡ÈÙ°‚“ÃÖË£Ö¥Øóç∏›˛ﬁø”Êµô¨ËÊ≥Õõ»ä≤◊–∏œ∆§¸Ù
˜˙Í‚Ì˝©¸›¯™à‚±Á≥êùû˜«“ÖÎ‚ÕΩÁ©˝¸ô‡™áàñì€ºë˛îêÌåËäåôÒ»™ùäÇÙùãË¬ÈÙóÓ©ÖÜ§ÛàãàÁã®ñ†Æ
Ëπ¿∞„≈Ú≠´≥¶œÌ˙ΩÓ°è”¸ÉêÖˆ‚∫¸ı¿¨∆‚¬©üáÄÇÏ˘–¥ÄÑ‰Ñë£˙∞úÅÑœ„äÏÑΩâ®ØìÙîÁíæÿ»ôÙ¿€›ñä÷∑É
Ê≠€¿ÇÇ◊Ë‡Å†åÄê»âß–»‰»≤çÃ¨ä∞°ñ»‘“ÄâÔâïÃﬁÖÄàò¥ôª”°ﬂ≈§ùÙÿÓ·ˆ⁄ÏÆ´›Í·±ŸÕ∂è”Íôı˚˙„˛·
‚Ù®øô…≈£˘—Ëú–É¢Õ˙öùﬁ´’í∏È¡Ê¶ºé⁄ÎÅ†ºˆíË†ìƒﬂÿúªæªªˇ≠÷Àıß √œ´†Ëï…´ÌÏÃÃÌî∞ÀËΩ±”Âáø¨ª
≤∆Ç¨¶∆éÍﬁ•ª¬£›üÄ´¯õﬁ≠Ûâ∑ò«ê˚ÔûºÙÜîﬁñ€«äÏŸˆª˜»Ã€Ü‡Ìô©ÔŒí€¡÷Ú˚ÄÒÈÛÃ†êå⁄≠ˆ‡âÔìÜÃÍ≤Ö
õºÆÙ¬üÕ˙Õ£æ´”ÃÄßîû¯Ù⁄π ©ÈñÅØûÒ·Å´ˇé©§©õ˛∞™Ïûäπ£æ–ØÄƒ™ÏÀá«ÅãÓ§à ß˛áÀäÙÉıË≥¶ññÁã’∫
”Úƒ˘”äıΩ›ﬂı∑ﬂòÃùÎÄ‡ùÁèΩâñÌÄ®Ô≥Íàßµ√Ë≠†ÆŸƒ¿åËûîõ‰º†éÓü⁄††î”Ã«£ÅîëÑƒ”ãé˜¥ºùûØÅ™¿
ŒØóÌÄê∞¢Ä”∑ˇçêÌÂˆ˜≈øÇàçΩ‰ìààòÄÑﬂîÈıòâä≤‡ñî†ÉëßÒÇ†º¯Äêﬁ∑≥÷úƒ£ﬁÙ¬„óä˘£˛áœöÈ±—§Ö†Ã
¥ò∆È€Ä¿†à†¥˙Ö ´◊Ü†…ë†£¥“ô©÷¶≈î≤ é£∑Èïû‹ùà†µ—ì∂Í±ÃÑ≠∏◊¬ªÃ∂ñõ…‚°Õ¶µ å˛Ë–„»áÖ›π‰‡†ï
óÌœ§≤ŸÅˇ‹†‰±⁄¿≥¨°’ÍàÃÔ•±É∑çÙ°πÌ‰í∂ìÿÿúøæÌø®’ö˜çÁÛÍ„´”Ü÷÷õ‰¸Õ∑Ñˆ°Ÿéà¸· „õ≈ﬁ“„õ‚˛§
˘ à¿·©≠Ä˜é≠û√æ˘è√ÎÔ˘Ò€ á˜©…ÓûÃ¶—â≤ßÖñ§„Ωï¢”Öß··ÇúÔ¢¢ àœÇè≥∞µÃãﬂÅŒÑøÑôœ◊∂Ñé¬°òÇÑÙ
≥ˆÌñÛ»ÔÅ˘Ä»∆‹¿µ∑¥í∏©≥Äıà∏ñ£„™©ëó èÃ≥≥÷û∑óË˙›‰»·Öèˆò›Ê»≈¸√É©¨íµôÌ≠Íç∆Í¬Åπ›Ö¨Æ˘˘ô
ÿ‡ÎÒÜ÷Øùïñ’ªùıßÊ†í„ùÒ∆—«óﬁƒÌ¸≠›¨ƒ£ˆ⁄§‰Ñ´Ä›Œ¬∆ú†∆¨úñõÄé«»àß•ﬁèÌ™√‡±–¿‹ËÈúÔ°√ùÄéïÁ
…´™ßñéØ¬Í¸È¸ãã¨‡ÃÛ“ÎîêêÖ¥ÜØæ÷¢≥ü€Ï◊Ä¡ø˘â®Ä±π≥òè•Ã˝ûü”Íá¶ÇáàÄ†ËåŒ∑ë§ÕÏˆ⁄ƒ°†ÇÅ˜¥Ï
ﬁ…∆ºâàßËæÅ¶æŸÑ·¥É‡àû£ı∆¬¢ãü∆Ñàù¡£•—´’∫Ö“ïß–ËŸ…ıÚñô†Éé∏èÍïπ≈ßÙ·ÍèÔî É©‹ò∆≈éõ¿£ÕÉÁ
ÀÖ†»Ç§†òˆ®Ø√íıŸ≥Ä±ﬂÆìüöúûòõòå≈ìé”óÀ‡ÑÄ–„Ü∆≠ô¥∏¨∏óîï∞€…Ã¶áπ∞¥¯Ú˘ı˝◊ÄÓ√À˝”çÁ∫öÇ—ÁÁ
ÿ·‚∂Ëˆ’ˆπ’‡ûŒô˜Æœ¨™Ìß®ﬂû”ıÕß‡ÁÎœå–ØÖÅª§˚Êß∂˝∏˛ûï◊€õìÌ°Ì•Ê•ÿ¥Ñ€Ö§Î¿¢éå–Ë‡ÏÌà≠´Ò˘
°í∑é€ÀæÌÉçÛÙêö˚ÔêÅëé∆ÏÊ´í‹™∏¯˛≠Å±ü´ëÖ∆õì‰ÿÓ¥Å„ñ§öë∆Ñôï®áêãê¶Ø·ÍØ°Æˆì∫´¯∂≥í¢…≤‰±ç
ﬂ§¬√√√ÓØ˙œ≠—ô–≤≤ﬁ€âπÖ˘Ú˝∑Ò˛ÒÛÀ ¬äˆî‡·Üï≥≤Ç™Ô¨£ª˜Ù˛˝”Î·ˇÖ¯á˛ç†ÍÔ√Ë¿±¬ÄÖ»»Ö∆∑ç˝ó
ò˘∫∞∑˚˝¿°¥òÒä‘ÜÍÖÁÇ‘Ö‘õöíÆ´Ç∫¥É¢Éœ´ùêÖÑàΩ®öß˜—¿ÇèÇÔ◊Ò’∫¸¯Öê◊ÂÊÊ˛ßÈÙÏ“≤ëéê¯ÉÆõ†»
§íÄÇ·Ñé€∂Õ¥”ÊÒ∏ù≤¡ø´íÚÄßÿ∞ûˆ˚›ˆß©≈ƒÉ”Ö˘éàòºŸ“ô’⁄¿†ƒáµ¯¶ÖÀÓÉñπèö©∏ƒã€Û…≠˙áïá–ÑÈÌò
Ò∏èÊ›ÿ§Ûà—˙á°∑ôçí¶Ã¿£π¨‰›ÿÌ…Üù„Ÿ˚‰»ˇÙ‘ƒ„‹‚ Ê†ÁÛπ˚ÒÇ◊Î’§∑Æµ–Ü∑ùàÏ˙Ì±∞∑ùä≥ŒÃŒÔÔÌ›
†ïÀ’¿á‡æ¿º˝ëÇ¬¿ä˝øåà∑§Ë–Õ€Ô„—–ˇÊÀÙÚÀÉáÑØŸˇ´ÅÒøÑ◊˛ˇ˙ﬂÚ≠ãﬂ‚“òΩ‰óË∫ãŒÔû˚∑ç˛Ö«êÓï‡Û∏
ÿ´√Òª¡”ÁÜ¶¿ƒüŸßÇ⁄Æˆ‚˚“Æ≤È˙¸´§É∑Ñøô£õıù£ÿ¶¥·∞ÿ…ΩÅº¨ÿõŸ‰˜∫Ÿ£°œ–Î∞„æ§¶’ÎïÃ ¸‘È‰¿Éê©
••íµƒÃ©ïåÅ⁄ó¥ôí÷áΩ√®∂õ¥ˆÚ“üçπÊ∆˙ú◊÷◊‚…Ñÿ†Ÿñ⁄üãªß¸ß«éŒŒ†ï∞Öü›˛˘¥÷Ú ùÉÑÛ®ﬂ»ÛæÚª—Ñø
∑≠Ò¸´ˇ‰£Å˝ÁΩà∑Îº¬¥Ï∂ﬂŸñ‚ö–≠∆‚—¿£‹ÎÔÇÇﬁ˝©¯ÄœÄœÖó¡√Ù˘µ¥ÃàÄëÁûí¯”ú‹ãœñò˜àÅæ≥≈˙ªó
ÿÑË®∂È≥Û‹¨ı¥¯ÎÏÓ‹§Ãç‡íÌÚƒ˜è€áÁ≥Û“´ÈÙ∂Ëè∫πÏàêëÉœíùÖ·ñöÚÿïÕûóΩËÛ≈ÀÇÓ¯ƒìç∫∞˝ê§¶ Äì
¨∫£†É¡•Â¡û∆®Ö√á•Ç∆Œƒü‰ÄçüË·î¿—è“¥ààõ¡Â£Ù“—ãûº®Ÿá†±Ì∑Ä∆∏ÒµçŸß≤Î’Êá£∆ÇÈ˝÷ºª·≥å«›≥√
ˇÒÖ©·Â≈πøµÕù°—±–¸§Ã—Ñ—†¶ûà≠’§˜∫©á¿¨¡‹ÍÊÊ¯‚Û˚Î‰‚¢Óÿ©Åßö“·¨ôÏ»ÔÄÇΩÅ€˘˛ﬂå†ÍºÇ¿‘Ñœø
‹°à◊Ç¿Õ®öÒÁ»ı˜◊«˘€ ·ÙëÎÂÕÔ˝ØÔﬁˇ˘Èˇ›Î⁄π∫Í«Ç€ Óæ∂òáÙ›ﬂÁÙ±”∏¨¥éÄÄË††£öß∑Õ∑”€ÊŸÈ·ú
ß–°¸¯Ò˜àÍˆÉ´†™£ÁÕ¸Ω£óÎ§ﬁ∞ö‰ñªø¢Ô¯Ÿ˝ÊÖ˙Ìîºûÿï°æÄÂ€õÕé∏Ìé¥¯‘ç∂öÚ∫ÏñÅè√Ì∑–∆ﬁ‹Ø“™•äï
ﬁ‘Æî»îí©à¬¨÷àƒ —Óü°í˘©›∞ƒóü˜„— ·…√ÂÅìÊ¢ÈÂﬁŸöºΩøò´ôüèÜ¸â¯à‡ˆˆ¸≤èºïÒáÊ¡≠¥ﬁáÚ•˚Úç¯ì
˜Øï∆≥¯Ù˛ÿÅö‡¿π®’øû ıß˚áœÓË≈°§÷∞ÓñòµŸ¬øÄµƒòÀÈ„„êÔª—ÅÂƒÆÄéÍ˚¨∫ÈÄÃ¡Ä√Œ«≈÷«µÔÅ‚îâ°É
ıÛå‹‡–ÄÁûá ˚‹‡îæÁÈ¬¢·ç†äÇ¿¥Ñû¡ú∑ËËŸö∑ªÇéˇÔ∑áï˘¶ì¥øîëÆ±ÔûˆéœÁñÊ÷¡√ñäÑ‹†éßÉà¬Øá
åÇèûÖ¢ùñ„ì«Èè¯ù·ıÄãò®‡£◊ÆÈ≥ôÄßä·Ï⁄É†É÷°÷·»°î∏©†—Ü¢ó´Â≤Æˇ¿…å•íÈ§∞Ç¬«Ó–áË˝ë“Óú„„˛
Ü‚ÿÉ•≠⁄øÌÆ∞´≈÷≠‘Á¶ó€±Â»ÜÂıá≠”ÈÔÍÄƒ˙„ÒÚ˚ÛÜÌÔËÏ‚˙¶ßûåõ≠îéÈ∂Ãî÷Ê÷˚ΩÅÙ¿ˇÙÓˆ¸ÖÔ‡Á¢ˇ’®
§◊™ÕÁ‡´Ûïáπø±™ÛØèêÿ˝˘œÅ—‡˚˙œ≠ä∞Æ™¡„ŸË¶¸⁄¨ó††˘ïµñ÷ë“á≠ñÔÔ˝Ãˇ€Î√˛ˇ†Ù‹º†ˆØêÓùêˆØÎ˚˘
ı⁄Å–ıÌ¶Ñ˛‘Ä∫Æ∂¯¸¿åÄÇå∏÷˚º‚∞¡Å‚—˘ç†∏µÅÇ¸”ì™ÙäËüäÙ˘‡¨øÿæπì˘Ë‹⁄Ë¬±õâÚŸ◊Ã‡£â«âΩ
Ü—Ø“Ä∫»˚Ü≈‚Óﬁ†…œ˜è¨∑◊Ÿ◊´´óäÈçÏ¿Ì¬‹ä”Û°Ïÿ’≠À⁄ı≥ôﬂã·∆¬©Úˇ”˝æøª∫π»ﬂ€‹î™’ÊÛÌï˘ãÛòü›ï
⁄ßßÁ˜èŒãÔÂÍ’’Õ˛∫Ó–∫´ﬁ∑üÀˇµßìÛÎÛ´Õ¬≠∞ ûﬁÿØ«æ˝ŸÊà–»ˇ∆˙˙“√˙¶œ‹ƒ˚°Óôè‚Ô∂∏ﬂ„Äè±ﬂÆÉõ‰Ü
Ñ†ÀÄ≥Üö¨∫†ê…¿‹ÙìÉ˛÷√∫º˜áˇë∑ìòâÄÉ¡€åœÇÆà”ı¬€ë¬«Â£ÊœÄ°‘˚”ÎÙπ”ÕŒ√£˝›Ç˘Ã‰‰ﬂú‘û˜éØñ◊
Ê∑áÜ«„—∏«œÌ±“¥„ìÿÄ—∆£Ö¢ÏÿÄêé´œÂ˜ê˚Ä¶ú¿˝Í⁄Õñí¢Ùò‡—ÉΩŸç¨ûóÜÊä ¬⁄¨¡¨–ÖÑ∞›ê´˙ÄÅ£ñã
á•Ã¢”˙àÕΩ‹Í»πÒ©êËπù§Ü‹˙§‚ŸÂ∑ ¸òÃÔáÀΩÑ≤ÓƒˇÜ√∂π˛¿÷∞ÃÓÔ˝·Ë‡áÔ¸˙ˆÌÎÁ®‚˙∫ë Ñ£©˛ÿ¨êÃÑÜ
˛Ûâ˘˘©ù›Ì≠¸·∫‘ΩØê¥ûÎœüÖà∑¯•“¸πΩˇÆÄˆœÕ›ΩÙÈˇÄæ≠Ωæﬁ‡∑ø’õØï˙¸ì∞á™ÕóÍÒ√ß¯§ı¯ôå¡¿µ˘˙◊ˇ
”æΩˇ¨’û¡™ã£¿ﬁÇßˇ±åçÙÃñˇõ‘€óÇ–Í˙çèéø∆‘ÎëÌº˝·Ù±´˝ÂøΩÕˇˇΩﬁæÊˇíÇ˚µ…ÆßΩ§˝º…‘Å‘ÌêÇ¢
Ò∆ÇÇ±‚§ÿî∏Ö∫Ìú¡∆®¥°† èó¨à£†¶Ú’∆Äíêœ∑’√ï°†æïÎØ∑¿—ıœ≠µ† çç◊“≥•≤¯∏Úü†ÆäïµÌÅΩÿ¨Ê¶Òã
…â€…‹Ã‡§æ¡èÎÙË‘◊Â´”™‘í©¥Ì∑◊‘’•ÈÓé÷Ùˆ®≈Äê⁄˝§ ú¥È¶øÇÿÑœêµÎ‘¢ÊÕƒ¬Û∞ïæ–≠ø∞¡±˜ﬁå·∂¸ΩÉ
ÅìË∏≤¥∞∞ê≥±ææÿø≥‚≤∞∂ªˆ∞Ω«±∑≥≥ΩµΩ´µΩ€©Ω‘ü∏øÒœ¯ó∆ì“î˜¯ˇÜ´´Ä›ü¸ìªÅÒ≈’Œáñ£ÍùÖÄ¿å∏‰ÑÔ
¢ÙÉó¥âäÔéÄ—…àá°£åÆ˘è≤ììÔÓÅÒ¸óÅÒÜ¬ëùÊ∆á¶˛úê–∑’«Éˇ≥Î’Õ›Ò…ÒÈ‘™ù˘ÅºÎÄ∏¿à∆∞¨´≈⁄Õ¥€
â›é´¸Ë∑ª»≠¶´Öåïùá—®—°¿®ôÙÁññŒ˜âëá¯“Èæ†˘ﬂóü¢ØœŸáÅÚ·îÀ®µÃ¶™◊ÜÕ∫˛óë‡Ä‚æ¬Õ˙±Çë∑ÆÎ’•≠
„™òŸï∂©“‡Ó¸¥Íﬂ»Ö◊∂ÁÜé∑÷Œœœä˜ı„Ûˇ¸ÊÓ˛¸ÚÚˇÃ‹Ï‹¬‹¬ˇ‚¸Í⁄Úﬁ¡Ø˛˘ÂÂÌ•⁄êÈºÄ≥Ü∑˚ÁÄ¿ñé˛ÍÙ
É™∞Û¸¸í˛ûØñÓÍ˜Àı«“µÒ€æºØîÔäÂ˚“Ö•÷æÔ÷öœª˜êù‡È˝ÇãÏ¢¬π˛Ò”âŒó´ÖÚç¸†≥¿…ï˝µÆœÏ›˝Úû†æ
ûœı÷Ú˘∏¿Õ¸˝Æøπó¯ŒÎ˜≤∫áÙ’úí«á˚√Î§ÓÄàõ⁄±ßì∆¬Ì¥ÿá∫ÉùåËúóöó¨à£†¶Æ∏◊Ç¯∞ÇÅ¢•¬˝ûÛÒ˛˘ñ·ƒ
”Ì•‘ÁÙâ™ÁÒ©åÙá‘ÑÈ™¥ﬂÅ µß”≥¸Ì∂€ÔÅ√≥¥ìÊ†¥åöÃ≠¡µ⁄Äîà“ß«›Ô‘⁄⁄ôƒ“›•ÔµÌ÷â˙µíﬁ⁄‹—Á—à√§
ËºÈ÷Ü»ÖÀ¿®ÛòïÉ°«ÔˆÚÏ›˝„ÕÍ£ÕÓı€€˚ﬁûª›ÇÈ©çΩå–ˆŸ¥¢àÿ©õâ€«“°∑∞◊±∞ø–¸˜˙ÇÙô®ÒÙÔπ˛»Îï®ˆ
˚¿Îì£èºâùÔÉ‚™ùéú¨£ò˜êëêÆôÅªÃ∞Ç±ÃÜ∏ìÄ·ƒ£Ñìê»ˇÏÁÑâÎÀ˛õëÔÄ´Ò¸ÁÄ¯´ø‘Öƒß€ ˘¶—ØúùﬁÀÌ˙
Ç‹®æû›î∂¸ˆèßÁó˙˚ÍÛ·–»·±–˜òë∆±‡∑ö¬â‹ √¿ÄÔ≤ïé„∞”‡”¡¶ÄáÏ¯°ƒ≥ò’¨†–ó¨ÔâÔ“Úé≠Ìîîπ∑ä‰·
π™≈˝¿…≈î¯í •ıìÍå©√ÄÄ±îßóÒ¢§¯ß˝˙öëèÓ¿©Õ⁄°’Éﬁ‚¢Ó‹ÙúåÔÆåûÏ¿Ì‹ﬂ›ÅÀæƒÿÌ‰Ô´Á˘õ¬…Â˛Ù≤ˇ
†ˆı˝˘›∏©◊›ï™ıËë√√¿ÙÁ·¨òò˝˝ó◊•ÌΩ£ˇŸ˘≈•ïı›œÉ„ãÎ¢á™Ôä˜¿Õæ∏ìøÃÉÔ∫€ﬁ¨›ïÔÎ’‡‹˝ÄÁ‚·ÛÒ
¡ÆÒ˘›æºæûÑÓÛÂı≠Á∑À˜ç»ê«€◊Œà¬∑©¿ÉÉ‡Ÿ†ÅóÍ›ˇâè˚Ô˝¸ΩÒˆ⁄π¨ïÏÇµÉ‚›∆„∞ò®Ä∆‘ø˜¬à÷Ÿì¿â
êËÅÇ≤¢Üò—Å∞—Á≈≤¢ÈﬁÜàƒàÈ∆À˛ˆ˛ËƒÙÊ∂Ÿ®ÃÇ√êë⁄‚ª÷áÄ¨∑í≤¬«ÍÙµ˙Ö‘ﬁ◊≠àî˜Î§Ω∫Ÿûè©Í≥Î•Œ
ŸΩòÏÚ∏úË¬‰µ®Ü„˛üΩÕïª˚ÊÉíΩ¢≠∫Äœüåø◊ÎœÎÎõÓŒ≥…®“™ƒ—Ω⁄ÈØ•íπ«å“ÑÕøú¥‡Ã§ΩáõÀÕ´áÁ¶†ûﬁ
≠Åöèºæ’ü˘†∏Æº˙–´õÿâà∂ó–©Ëòä≈∏âÇñ¿¥º≈÷·î≥∏çß≠ÁëÁ‚Ñ´é∆‹∂ãëÒ´ô∆‰€˙ƒÊÙÕü≈ÚŒí¶Ú∑øÒ™Ú
¯ÍÛºﬂ™¡ãı•√á€⁄·¢ÒÚﬁ™–Ò˛ÈÁÏ∫º˚¸æº·æΩº∫ûè«ú›ˆõâÄËÄ≥≤êÑÑÑâÚÄËÄÓÖåœ®¿∑÷èÎı∆Õç÷Ø“Ëï∞
∏¡üÄÑ§∑œˇ‘’”◊’›˜‹≥–ó®´€‹Ö’ç‹€ŸœèåÉàç®œ´ÅπåÄ˛Ñ»ç©ïí∂›ƒêÿŒÉ–ÀÒÍ•∆¨Ó«Ò©Ä·ÿ≠¬À€µπüÉà
ø»Ó‚ÿŸ¡ˆÈ±–¿˛ØÍË¢¢Ãë˝˘„ë¿ã‘Ì˛∫ºΩ‹ÿ⁄ÖÜ÷–ˇ»¸¬‚ŒÓˇ˛…È˘··—ˇﬁŒÓˆ⁄⁄˛ˇˆˆ—˛ﬁŸ…Æ—˛Í≤–∏≠
ç›Åá¢˛ÒŸ·ÿø∏Ÿ››‹Òﬁπ∏∫π∏∫—πÖﬁ∆•é√Òâø©Í≤Ò†ÂÊ©Ω˝È–‹õÖ‡Ÿ‘õ˝ΩÁºÓø…ı∑˜ƒ¡€ÎÁÚé≤ÂƒÂÂÔ™Ç
÷Å¡È†èÙÕÜ‚‚ËÜäπëÇıÜëÁ≈éêÕàë¿âÄü¡—˚á◊˜˚ó≈ˇÉÛõ£À‚ŸæÕÂ™‹ºÆææñÓüÄäåÿ∂Ã∞∞‡ºâ≤í–Éï¿√›ì
⁄˛ºÆ◊Ü«∆‹ÏÊ⁄ÑÈ¨∏ç÷∆Ë¨∏ÃÖ˘ƒı∞∑ê“ÎûÈΩ÷®í©ƒÈ›Í…óŒ÷≠çì˜—öæèÆﬂµÈÅë÷´Î©Ì“À˙ÏòÊ—¨øÒ¸∞áè
£âÄ˝•êÖÀ¿∆ﬁÆ∂◊ñÈ◊‹ñã÷°êπˆÙÈ§Ω∏ ÚÏ÷À˝Ñºﬂ˚•õœŸéó«ÒÓÓÆÍ˜µ∆¨É√Ñó˙ì’˜ç’≥–˜ÕÁéèΩâÌ‹¯À
«á∑Ä∏êÄ‚ó€ÄÄ∑€à§àÙ¿àÅ¡±›‚‹¿àã∑åí¿Ä‡ôÒï¿ß†£»∂¿∞Á—íú†ﬁﬁπú±¸∫† ºæ¡àáÅ†¿√¯˘ú€Ωæ∏Ÿæ∏
∑ôúôÀß”Óõß√ÑË¿ùÏ≈Ú≤„òÌ¥¿ÅûÀ∫ùÉàÙÅ‘â˚Îò‘∞®Û…≠ì©≈íƒŸﬂüî∏é›éΩ¢üªò∫Ä¸è¬ﬁ€É‹á∫ﬁ≠ò–°œ®
ÏìÅ”≈Ìî« ≠ö·è·∫Ñ‘œá±©øÒ¬ë´üÙÍ”∂ı˚ƒ§Ãπ∞·àä˝∂ø∏µ∑Úõ≤∑Ÿ∏πÈˆ›îèºç¸äÖ˘∂ÓÄàÉ„ÛßŸπ≈‰¢ô 
¯§ÜÛπ©âÒˇ˘ôÈôâÒ©˜—·Ò‹ˇÃËˇ‡¬‰»ÍÏ¯ˇˆ ¸¡÷˙…ˇ¡˛ÈÒÒˆÊˇ÷“‚“ƒ¯ƒüÚÚ⁄˛¡Òûï∏êµá‘É¬ÁπçêÇü¸
º›¥‘ëØ›Çô¡˚Ë¯øñêëˇ£ÆÄà›ŒÑàêï˝Œ¨†èÚ’√¸◊ÅÒ¢˛€¿ÙË¢£¸¶»áÌ”‚Ùˇ⁄¡ƒ¸ÍÃÏ˜¸¸“ä‹œáÆÔÚ˜ÕÜê
Ä¨£ÍœÇ¿º´óÍ≠µÄ—¯∆©’ûÆã˜ÉÄ√√®Â√¡õà‘Œ⁄Ô∂„∂Íä«—Úπú€¢Ó–‡ÍÄàÆ€ï‰Á≠ÏÁΩÕ¸ÁãÆØƒÀ˛ΩÑÇ“ ‰¢
äû≠Ã‰§ı≥¯„√óÁÁï⁄Œ„Ì·ïπîÇÊ˙Ä∫áˇ®“æÄ∆íﬂÌ≈ÙÜ∫•§è”…®∑≠î÷À›Ñºî·—ç¯¥ÎÉøÆÛ’ë“ı≈√≠Ò¸ﬂá˙™
Ø∂Î˛çæÚ¶ÅÓ˛ìçÔÅÒè¿ã°√õ¿ú¥˜ÄÇ—ëÒúë˘†îóáí∞ ¬ËÀ∂ÉÇ≈Ÿß™ÌÄ∏ÓÛ¿±ﬂ¯ïòÒ§‰ËÅ‚Ω∆ßáîú⁄Ä—˛œ
¿˜Ç¿‘ûﬁÅÿÂÚÛ◊∏®‹Ëù›‹œØÆÙÜè‰ºÓóÀá™ä Çºâ‹Ë•¬åî¡œÍııÑ÷ãÎ¥∏ñ¬·ÑÅ∏ëÃèäæ£ ≠◊™ı¢Úπ⁄¢—àƒ
£≤ﬁﬁ±äç…Í˚∫ÂΩΩäæÊûï®◊£Ó’ä…˙å≤æ‹Æñ¢ï¿Àï≈Ãµ™˛áˆƒ£üÚÚÎÄö˙ÌöπÑ≈ù¶ï¿∏£â⁄åÉÅÀ‰„⁄øæΩÙ¥≥
¿ÒÛ˝ç¨ˆé⁄ã¸Æ›ûƒ˜ØÊÃß⁄¯ŸøæºùûûçÜ£í¡ü∑öÙß£È°Ù¥îå˘‚ƒ˛˜‘„œáúÛπàÁ¥·õÕ„≈óßÚõÛìÜ€ÀÛã”ì„
¶√Éô‰íê‰å˚ÁÔ∫–Ü÷¨ê∞çÄ¡’¸è˛¬´ÉéÌ‹∆‡Ùÿéà÷Ø˛ÔıëÎÂ«ıÔ˝◊Œ≈ù–ÓêÖÇÑØ‡Ç¿Ä°Ã‹’éÅ£ÎÆ´«Äë◊˜
§¸ò‚∞ﬂŸæ®©ÏúÊ◊˜ØÀÅÊ∂è«ó∂æÛ”Î…ë≈¿Ñ˙¢ëùôôû¯öôûôôô€´⁄ŸªΩ™ﬁ‘ƒüÔ†üÅ’ÑÙ‚…Å‡¡©˛ƒÑôÅÙ◊ˇæ
´π´üû›ˆÁÅ”Ê¬Ô–À±áµ‘ÇÈ¢æ«Û–Î¢‡˘™ŒùÜÈ›§∆…®ÃÕ˙âç–˙¨üÜêõ ≈ΩãÛ£…‹¢Ë∂ô§‘Ú©¡©Ò€Ø∞ï˘ô¬±Ñ
äˆÚ⁄ìªéÍ˜≤∫ªˇº∏∫ßùúòµëî¡Ñçñ∏ò◊ÀÅƒ‹®òÍ®è—„ä¨›±ó≈ˆµ«˝◊«õïÍô≠⁄˚®÷û´√Ì™äÑÏ≠†øèºâﬂ‡¬€
∞¿úàÙ¯óÄÅ¡ù≠äèòôöóíÁƒ≈õ∆åàÄÒòÁò—ÃÈÅ‚¶≠–’Óéà˚˜Ä—Œìà“∑ÅÒ≠∫¥°öò›Åü—™Ωï™Õ¸È¿˛ı„˚Ë¯ƒ
áÁıòÅóêùâÄµÇË¿õÄÑã¥∏Üä”≥¡Å∞¿äΩ∫ÁÇ‰¬·˙“ãÁ —·Íï¡©◊¬»≈äë¢∫ŸÄ√™ÎÌÊËÖ†ñ√òôüò∞˘ü†É…≠
‰Úô§¡“û¥®≥∂ı“¨äª— ê©Ù·Ú‘ÄéõﬁÍ∑È¥“û”Ë‹≥≤º±∞µñœﬂúµÑ÷˚ª˙õÍÖ®∂û˜˜éÍ¶∆ßÉﬁØ√∫ƒŸ…ÃäÎ¨
íÜï…ßßµ“ﬂœ∞Èƒû£§¥Ã ≤é√µ–∫ïäÃÊí≤√â–≤ËÒ∫˛Ì√ÉπııÕÛõ‚ùóÉ˚“ÜÜàØ˘çÇ∫•ºÅƒªÙ¯ÅèàøÌµÛ—Ü
Ö⁄≈⁄„ÙŸÜÉÄ‡òÒ˜Äô Å®—ﬁΩØÌÛú›¢ï∑œäà´˚óìÀª√ˇ”ÎŸÒÂÃÿ∞Ú†Ω…∫”∆¯˚‹Ôç‚ˆò∂Øú çÙ˜Áˇ“√πÅ˘È…
Ô›˝ÉÀ“Ì∫ÜÇ¸àáŒ¬ÿÄºñÎ–å¶ ∑úŒØÔ◊∑ˆœíÒàôÄÜõÇ‘Øê·⁄˘¢¥í»∆¢¿á∞‚õ¥û¬ìµÅ÷≈Ñ•Ç¶‹“îª±í⁄úôæ
…ÿŸ©Êœ◊ñ¨ñ¢°ê÷≠§æ¥≤ÑõﬁﬁÿÎªªÔﬁÉ˚øØ˘¬™ÒÇ÷éæ¯ÁˆÆæ∑ˇ˘íŒÊï∞Ò¶√ÿ†ê„ ±°ëëÇ¿ÿåäØ›©»⁄îìÅÌ
ß÷ÄÈÏÛÍ¶¯ˇÒ”∏ΩπΩÍ´î™≈Úıˆ©ÆÜ¸Ô“•Æ¥–çºâÙë∑ﬁÒ˜≤ ´£Âç†£¥Ùû£⁄ïë¶ñëÌÈâﬁ‹‡â—ü∞‚õ¢çëÇ‘Ö¡
˙ÇÉ†’Ì©›ÀâË‘Åè›±∑—´óŒ’˘Ïôá÷∞ãÁ≈Ÿ◊ò¸–û·íÊ‹¬‘öü ®•Äì∞‚„ÁˇÙÃÏ¢‡˜€ÕîÅï¶Ìî·£±†ãÓ∞ôıîﬁ
äÀÍ¥Ñ÷√ËÊ–öÑñÇÇã„Ö±©ì§¸™†ö”©Ù≤â œ‹«—°€©Í“—ã˚¥íû≠À¥«†É˘îî‹ ‰—äùÜâ‚óìÜâŸàîô˘∞ô‡ÖÕ´
ı±Í•ñ≠‘¨ªï˙ÃÜîœ∫›ñä¨ŒåúÓ†ÏﬁÉèÇ¶ß„µ¬ÁÔ•Å•ßò‹ÉéÚüπ˙∞ΩΩ∑∂ö≤Ê˜πÄ´Ñ„Á¢Íî∆Öƒâ®êôÂ˝∆˘
µØ…’Á◊âº¸∆…‘¨Œ∏”°†ıºÓôé≠∫á¬Æ‘–πúªæºªµŸ¨œñõèØ®Ä˜ Ÿ®Ü£«÷¬ç¥ç°üﬁ˚¯¶ßã£√üπﬁ‘˛‹ﬂˇÌ‹î
†ˆŸ˝Öê˘äà¯Áô‡‡˘ÔÄÊ˝‡ÚµΩÌ∂îÕÕÜ“„áæ¯∞≥ºËçµ‰¨¡úÌÕ‚±éÑÄÑÓå€ÛΩâ–¨’üÕ°ˆÁ≥√É˝”≥£ÉÀÀÀ€«
èÁÁÖ ◊ÅÒ’œÄ‚ë¨”ı±¯ˇÄº˛Ωﬂ£≥…ÈÈ–¶Î£Ù∂Ç¨ ©©Œ©ı∫Ì˛≥∑õ≤ö≠ò∂ö‡“Â›¯‹Æß≤ª±ÙÙ¥ˇÄ˛ı˘ıÛ”¨
ˇﬂ››◊◊÷◊ê£·ÄàÖÖ”Íè«ºá˚ªïë˙£†Ç´∫î¥ÃÇØø›‹÷æ◊◊∂√¡®‹æ¢ïã˙î‚ﬁã•∏«ßêÅ¿Ãÿ±•è÷ßÌ˜ö©ê˛≥ü⁄
õôâÔÆåØ¨ÑÕ≠≠ÆÏÏÔÒÏùûúüùü»›ﬁ›‰ÔÆØ¸ãó˘¬˘’ıØÈ≈’Ÿ’˛ºˆˇÛ°∑Ò≤ÇêÇ…ú¶˛åêíˇôëÖûëˇıÓ«º·ƒ„Â
Ω◊ÅÒÊ†È€≠ÄêÑÆ⁄Äêî‰ª¥≥¢£ªëÓΩ¿∞πêÃ∫Ñ‡ƒ©ˆ∏Åµ˜¥æπùä˘Ö‡ﬁÑâ≤—†Äê∆´·∞‡¬∫ì—ãˆ◊Ñâ¿á≠¢ÉÅ
Ü¿±∞ñ†Ù⁄î“ãúà•ﬁ°î’À˙‰Ω›∞à˝ÇÜ»˙¥“¿»áùÑƒè°ÀÏ£µ–Ù˜Ë‚•ÄëÓÉÃ©â∞Íƒ¥ä∫óÈÑ“ƒ™§¿ºó˜öå©ø„º
ÏõŸüü∏⁄€ﬂæææ·Ú´ÿ¬Ù†ˆ∆¸È”¥£åÆ¡˚ß‹«ﬂ‹‹ôôöˆú∫ù¥—ÄÎ‘ä∫πî¯≤öâéÃ∆–À˙…ÂÅüŸ£Úò§Åå ô≥¬Öµú
⁄¥èƒÏ£±◊∆ÿ“ª∂ª˙ºÖ∑’∫≠µüﬂ ‡íÜãâ÷çÉ∂Ñç÷úóæ≈Ò˙èàØù˙ø»ıÚ„˙˛Î≥ÛÙñËã—Ÿ¨âË˚ú—ôÒ¬Æ∂Ø Î«◊
ªÉ≥õ¡Ï§´Ü∫Ëéå∏√Ÿß ù¿ù— ô´≈Ï±É≠Üπ£‚Ñ´Œ˚‚¶∆Ô¬Êœ˙£…úÿ≤›ﬂüÖöñú»éçˆœÕÿŒ¨¨ØÓûûÉûÊÔÅ¶ÆÀ
˝ıºËëÆÔﬂ†‡ã √µ¸Î¥ëØ◊Øä’Ô„´‚ ¶˜∏á√æá€¨èã⁄åÊÉ¬£óıõ»ÿõá‘ÎÚú∏¶Ê”çáﬂËùπ·‚Ì©†‹ˇ‡Œå™è
›Ø‡·Ø¨ÌÑÙÇ˛ù⁄·§ı‹≥Éë£√Éà‹ûﬁ’«¬›„Ô˙ˆ˛¶‘ª∏ÆÆØ¨Ìƒ¬Ò’Ö‘¶êı©˙Æ˙¿∫÷»˚®øù‹Ü´º¥∫ÙÏ÷Â‹Íπ∞
òÓ±„ùŒ˘≥êôÔÊ†îõŒ≠éÕÊ¶Ê«Ê±ÊßÁó¶ÁñÁÁóÊ¶Á¶˘ßÊßßÊ¶ß…ÁÁæÚ¶≤Äë‡∞ÅÛ“ò˝¨Éêﬂ…àñî™Öûëﬂı÷Ù
ÀÒÓŸ”◊ÄÒ∏ç∆îÑå®ıÜ∞‚πÉêù…êÒÏÍú“∑Ä˜¡ÊÀµŸ§àøùÉ”…Èπ»î§Í∑⁄ÏÊö≤Å¬‡•ÙÿÆÜ◊öç⁄ì¡‡À√∆ÄñÄ
≤‡ùà ß£Ù†⁄ä¯ôö•¡Ç©’»≈åù‘ÕíÓç¥€´ÎÈÅ…î≤ÓÉº›õ–ıöµ¢ªà¡‚«πˆñÕñü•Ïµ ˙¥Ú‚‡ÓÙÅ≠€Ù†óëÍ©◊å
ˇßÉÆ…ÅƒÊØ“ÃÒ¡û∞˝Ç∫Û≈€©˘ﬂ§∫Ñ∂Ë¿á ªÌ±–¨¨ØÌÚéÂÚûªç–‡Îı π¯»ÅÑÎÉ˛Ì™±‡Ë÷ØÜ‹ôµ»ß¢ì∏±Ÿê“
ï∆≠Í˛ó·¥í‰ö…∫óá√ª˚Àìóá€˜Õ•‚Û·â•Ú£Ω¥ÅÇ•Úı„Í·”√¿á˙¶◊àÉêˇ£◊ÆÂ£¢£ï®ª€’„§ì€µÁ®“˙ÈÈÂË
¸ÊÍ„ƒóû¿Öù±£≠Ïˆ«ÀÏæ®Èå„éûà·å‚éò¯èﬂ‰â€Éâœ¶§·ç¶ÉÒ‰Ç§ôÃÂ≤ÉÉ˘˝ÄêπÅ˛ëˇ¡‹ÿ–‰ƒˇÿ¬‚“˙ˆ˛
˜ˆÒ’—˛Óº˘‘∏´ÅΩÖŸÓﬁ‡ãÛ“˝¸ßˆ˛¨–πæÆ€Ôù›ØÌûŒ‡¨ÆÅ¿ƒ˝∂ù†’óˆ‚Œ“∑•ñâ±ùÜ¥ÁÇÖ©ÒìÍå¶”∑ê∞ùÆÃ
∞î™çòÀ®ø÷óÎœ‡óÄ€ˇ»¸‚äœ„÷”´‰ﬁã∂∞öàº∏∏º™∑ØÀÀÕ´®Ü†‘˜Èı›æ¯óú óéØä€˚ßóôÏ†∆Æ–Àûƒ¿∏í˜ˆÈ
Ö•˝¥ŒÓß‘úÓ Â¬§¡‹Ìú±â«¸⁄—ÿàëçëÅÄÈıÛìæÄÈıìúûüÈˆíÆØßÂêˆ∑Â°”ˆ∑ÌÛ—üê˘ì—™⁄›ã¬êÌÈÈ¨†ﬁ
˛ìüá¿§´ÒåôÎ¬ºµ¶≥«Å´Ç˘ßû—úÇÑ«àãÚ”˝ËƒØ¡˜úÓî¡Ôπø∫ÆâÄÁÄË∏ﬂæ˚πæÉ˜‚Ï¶¥Úﬁºá˜Âä¸òåï«ÎÍ”
∆ÕıâœÏËÂªø±Ò´∑∞¯ˆÓØØÈ©‹ûÆ†◊ îÎ∂ã≈Â≈Ï¢¿ƒ£ºêê¿É∞ê∆≤≥¶ÖË∞¢å¸∫õÆÉ§¿Ù∆¶£û∂ò»ΩäëÁÅ°∑–˙
®¯å≈Îµ§÷¨Â‘ ¥≤ë¬ì‘≠î˜©Å†‰êöò¨ªÉ¢≈õ“˜⁄ı¢î‹πçí‡«Ä»Éû◊ÙÍ•êﬁé¢Óüï¢Äºíë°ñ˘¨‰¶∂Íüôôÿ›
ÄﬂæªπÆîÀÒÄ«¯»ˇÙﬂπ„‰ø¡âÏØ…’˙”›µ˘Ã›®Ø≠ÓåÂ‚ÜΩéáÈ–Îµîä‡¶ãªˇâñ°¿Ë‰˝∆¬•¯€‚⁄´±‚õ•ﬁ‹ñ¢‰
˙©´˙ÿáœƒ⁄¨´πÔ¡¸Ó¸¸˚·ˆ‚æ¥¸€´÷ÎıÊ√≥ìÖØ¿ı°˘öÔÛ¸ﬁ…ÙÊì¶¸˝÷ﬂøèÎ˚ØÈΩÎ˝†ô®õŸΩõ∫π†˝†¬¸˛
§·ÊÁÍ¸Ì∏˙ªµñ»åõ˝⁄â‹û¿Ì˛⁄¬ï¥¯¬¶˜»≠‰âõΩë´ØïÍàƒº·¯†°öà°¥ƒì©·†ƒä†¢ì…ÊÊ≤Ÿ¨Ä≈»Ä–Ó‚Ï¯ÚÇ
ª∏Ô≥ì≥Äì⁄ª€«ó˚◊Âì√õ˚®∏ù’¢èØ´˚‡Øæ£æÿ∏ºﬂ›ªöŸ⁄æòöﬂò„Ã•ìÅÀ÷á√Õ˜ªíûú◊≥ ÕÑ∆„ò¡˘•Ù‡ åÀ∞
ìÿÔπÿüÉ˚îã†Œ≈©Åø∑¬›©±ªºû∞åÜçö•◊‚¸‹òâÔÔÔüóÎß∑’ı›ø”…ôÂ‹–ÿÚ™ôŒƒ¢ÉÈœ‘ƒ‡¿ÿÆõ†ä˙ù∆’Ä“
ó≈ÃÍ†©åÅæ˜€¢é¬◊äùßŒÏ–Ù˙ıÒΩ— ñ≥™˝¶÷çﬂÂà˙Øüè¯∑á™Û∫±ªã∑π≤±õì¸ƒ≠–Öƒ«ﬁƒéê¸Ç¬öêêïÆπ
≥ÌœÄÅıóÅÀ¡§∫≥ÙËÂ‘ºãÀß·ãŒæ¯û√¸ı˜ìº¥˘¸≥ˆ€πæˇæ±ˇ∫•π˜ºı≤˝Ç¯∂˚Ù˛˛‚˝íØ›îÔÛ∂˜¿ó¸∏·“
¡£ªô—Î±‚◊ˆçﬁÇ‚Îåç∏Î¥—Åù±ƒΩÌÔüœŒÕÿƒ¢ëª≈‰°Ïö”ë˙ï…„®∂‡öª°≥—ì˙ªÈ∞√üÈ¬àÁåöåò¶é÷ ±ô◊Ä
êê¥ÇÑ•”ô»’å†∫ê†ÄÓ√⁄•ïıÈÄ·‰›∆≈∑≠ÈÂµôƒﬁ¬Í∆¥Å£§∂Ωî“óãıÚæ‡Ó¿ ¿à˚ıÚ±©ÖëÖÏÙÆë⁄πòúπÿŸº∏
Ïª´ﬁñÎèÇ™äÕ∑±‡°Íı¿≥™Ωï´ÕÅ¸Õ˘ÌÌÛ‰ﬁËƒÔˆò‹â†—´•‚ÈØ‘†◊ØÜ“¡åÂ´•§∆¬Î§û∂ÉÃçÀÇ§øÃ‹Àπı„¡
®‚°·ŸöãúËÆåﬁÓ¨‹û⁄ü‹ïÆÓ´ï˚ÜÄÇÔÔ¸çÀÔ“˝ªæÚΩØ¸∏æˇöŸ∏ÇõÖ∫üÉ⁄øÇõ€∏ù›ªô¿ŸΩûﬁæúòù€ åŒª„É
Ò§≥¿∫ÄâÏõ•ùæìÀÔˆåáÌæê≥êˆ«á¢ﬁ¥êçáÖ„±Ë¢ûÀ¬ÇœÅ®ïÀ¶”‚‡’™ï ß≠”ô†Ã‡ÆˇÄÏ¯¯®∑∞Æ∞êŒˆèèŒ
ΩŒ¨≠ØÓùÌãù›Ó£¥™¯âúı˛ªÒÙâ∑∏∞∞ı∞∞˝É∏¥Ûºµ∑ø≥êâÊ√¶â∏ëπøìòç€Ì”Ñ÷Ìá¶ù≠û’»¿ÛúÒ„˛À£ÒΩùç√
±¸ùáö¥ßï∑ÓÁ◊ˆ¢Ò§˘¶”˚ÕÜŸ°üËˆŒŒ˛˘·¢ı˚∏∑¥·Ú‰¯Ω¡ßóÍÙÂ¢ã÷Óß¥Ç·ã±Í±ƒËÛ’êÀøÉ“—÷ß˝Ü…ü˛ÿ
ë°…“äè÷‰ﬁ”ıÿîΩûù≈ÿÁî˙∞È¿˚¢â˙Ò†èÄ£åíÆè·Ù∞ìé§ªí√Õí˛‰ÕÏß√ã˛ìÌÄË–ªÄÃ‡”öˇ™†Œ‡πÒÙËäÇ‘
¸¨ù¿èÏúó›÷éÆÊ∑Œ¶¯◊˜Áñ∑Êñ«◊Áó÷ñ◊÷„ó÷∂◊∂∂∑˜ˆè˜œÛß◊ﬁÂ‡‚Â°üÒ≈–Åà¬õ†ûΩæÇàØ™Éë“˜ÇÚõÜé≈
ˇÍ√÷Ó·ƒ‰ò‘§ú≤ô©ìåÅê†¡¥Ú °Ø¥„Íª©Û°“˙óÌÙô∞íáçŸàå≤ö…çå†ÙÃ…é¡é∂ËÕÜç≠ê”·Ç¿®§ø¬Î§∞á€á”
Ô∞ªå Óì∂È◊·î˚©ôèÎîªâπììõ‘Ù≤∞◊´§ΩÕ‚û˝˝ÜÖßÑ÷ √ÆÛ–¬∫∂ºª˘Ú∏‡˜ı˘ÖÎ’ò∏∏Ø ˚ÈåÖ‘ÛÈ€¸Úˆ•
≈ˆÿ™¥Ä†÷€◊∑«á«¬æóÄÖ¢ùŒ≠œó…≈ΩÍÈüàﬁ°¶‘““£µÖ…›∫©’†ÚÛ·Úü©àöıïôáÊ»£õ¯äö◊≤éÌ±«Ò¬¿˛˜±
¿àòÒ€î“Äﬂ€ﬁü⁄Ùﬁ÷˜˜üÍ£◊ÀèﬂÎ˚Ø¡≈Ω†ˇ°≈›°Ö›¡è˘Ì‹‹ÊÊÚ∞πæºùŒéÑë‚ôﬂ§ÊäÑ¸Ìûü’Ì≥∏‡˝Ê¿á◊¿‚
Ñé∫˝·ˇ∏öüç«ÉëòØúÃÖü√—¯¥ñ†Ã¿∆Ä¥§ì…©Ù≤âÑﬁ§Œ”¿ïÄáŒ˝ÊÉôÏ¿™›üÌÓüÃçÃåœ”≠ŒÔû≠ù›∂Ïúñ∂œ õ
˛ß≈’√´•Ωø≥•›≥ÂΩÛ—˙˙˛°ø·èÀÖ…ŸîıºíüúÃª‹ñºß‰˜∏ù∂Ω äÊ”À“û¸˘éÊ¯‡Ëà¯ü»º†òØ‰˘˚Ê®ùÕ·˙ç€
Ô§ü Ö÷◊÷˘Á◊˜‚â¨Õ∂Ï”´µ®†…Ø§ñàåäâ…•ß·Íø©äÊíäîŸµÏ‘ÑŒ˘çÛÅÏ°Í‡úÚÎÛ¿è¿∞ Ç˙—¿ﬂ¶ó≈•ë‹Ê≥
ñ»¬î≤·∞Ññ›ò«√∏ë†Û†‡¡ìÈÙêºŒ∑˘—Ä‡Û˜ïπåÂ∂∞Â≤ΩΩ°ÖìÃ‹àÂÑ”ö—÷≥ÔüÿÃ‰∂ÛçË÷Œ⁄ëàÙ˙⁄∆®˚ÊÕ”ˆ
á€Ó—È“ıüı¸„ŒÀ€ß≈ˇ•ùÛ©ï˝¡ˇôç˛ÒÖ‹ÿˇÃ–ÿ‰ËËËΩ¯ÑÿíßßÊäÊó÷÷∑˜èÜèœÚ∞ ı€õÀﬂµò‘Ë·ÈÇÙ€éÙ˚π
ü”Ì◊’éâÆÉèöÁÑ ·™˛÷ö◊Â˙∆ˆÁﬁÿ¯¯∏‰∑€“–˝¿¥ ΩÍ“ÇÎ‰¶É“∆Ë‹¶É◊ä≠˚ùîœŒ€Õå§∞»ç∫Ééœœó†˚‡≤Ë
ùÃ∆≠≈®±õÊ‘¿˛∑®≈Ï∑·íº«ØÏ±·™˙±ıØ°È¡£ı‚ß°èò‰’ÒáÆÊ◊áï¨˙üôÊ‡ê¢⁄π⁄¶ñ˜ÙÀ´•¢£ ‚÷ ´¢ﬁ€ÿÆΩ
˚æºÄ‹áõõæ¬ÈÓ˛æ˘˛àä–«Ê∂–ïŸ¸∑÷õ≈ÚÌœ‚™∏∫äºà£±†ØËÖ–≥»òÙ π ™“÷©ƒòä•ÛÎÄÿŸå≤áÜãÂà¬¥¢·ÅŸ
â›£Æ≈Ïò€∞Õ€ˆ∆ºßÀÔπ¸˛ Ì˛¸Ôæ˛àÙ‘≠µ¸‘˘ì¯›ë˚ˇ˝ß˙»ıÚ„ã˙˛´ûõœåìé•Ü«ë∆íƒÉ√ëÙ∂êç¯˚√°èﬂÁˆ
∏˘‹æè”Îı¯¯˝ˆèﬂÍÒ´€Ω¡ßØ‰çÖÜ√—Ë¢ëèƒ≈„˛–¥êé≈Ö¢Ò¥ôãÖ√˙Õ¯íÇ¢∑ïÜáÃ¿É»ü¶°¿ß©Ãø§–¢›üÕˆ«
˛”πÏ–ƒ‘ˇ‚ÿ¸ÊÙ⁄·ˇ¸ˆ˘‚Ó≈Úø˛Â ¡’ ¡åûËı‡Í˙ˆ∞Ê†ÓüãëÀ˛É¶£≈ÙÄ¶∑‡≤¶‚ë–Å¿Àõµæ⁄π‘î≤≠≠Õúú
¿ﬁ∞â◊ÉÑòª‰˘˚¸ˆ˘±ÁÙ‡¯Ç¬å“ûã·º„üŒ«ı«‚©≥ÖåÇªï“¢Ü“……ƒ∫∞“Üº¨ëó¶Úì Äû¶çí≤¥…±∂Ë’åπ¥ìŒ
ÉÕ®›≥Ë’ÁåË≈íî¶à…ßÜñ•®π¿ƒ˝ú“§¨⁄∏”‚êÙ‡Æ∑Œ–ø¡ÌÉÇò–ÃŒ∑ıë∑Òáìı¬∫ôÒÆ†πÑ≈ÆÌå®¸Ãø†ÄÔÄ∂ˇ
ˆ√ÉóÂ›≥ü€ù”‚ˆ…·√Î¯ÜËìÈ˝∂∞ø≤˜±∞˝í∫≥˛¥µ˙∞õ±º∑∫∞≥∞ÀµûöÖ©—É˛â‘ÓÏ‡ÄÙ‰¯¯Ù‰‰ÄÙÙÏÏÏ¸ˆ
¿Ó˛Âı„˚˜è‡ËÏÍ¨ˇ˚∂Ÿ¨æãà˜ô—˚””Û¶æ·Êù∆†Ãî¡òèÇ∫îÓØä¬˜Ä—£££‡íÅ≥“òÖÈ±Ö≠¬Øî„Í©õÿΩÈÉ“¡õ
…¸é∫Í˜ƒ˝ÜÆè√äåÚ£–Ùü†ùÃÜùˆï”⁄Ò≠ì–‹∏âµÕ®∞ÈíÂÑ∆ÃÈ§¶˜£ÃÇ´Ïî⁄˙Â“˙¨Ü†ìö»˙≠π®Û¢Ÿ„ü…˙˙£Ó
ü∏ﬂ·±—¶Ã¬è∂ÓÒØ‡ä‘àòˆ∂£≤·¿˜ÃçèÏÔ¨ﬂÂÛ◊≈€Úπ⁄“øÁˆˇÔ¢˝Åëÿüã•⁄∆ïÏßÙ±∑ªïî…ıª≠éíâ∏º⁄çŒ©É
‰ˇÇãïÑ §¯∆òçÚõ—ÉÓ¬ï˛ä∏ëìä¥Õ±Ë‘ÃÄáçÀâÍ≥Á¢øöÕﬁÏœñÌŒè´’⁄ıæ˙—ΩÔ¢ç˝ÈîÌÕ–ﬂ¬Ñ”∆¥Ù¡ı√øÆ
˜ÔøºæØœÁúâÜ‹°†ªúÙÚáÉÓøﬂÂ◊ı∫¸˛∑ø‡ÒÒÖºûø◊ÌÛÛπ¸æß◊ÔìÇ˜√˛–§êòé«‚ëƒ≤Êûâ≈ÄùÄÇ¢ë‡Õ§¢Ò‘
æ¥ëäƒ”Äë¡ôÏˆ‡ÄäÄÆ¢ô ƒ”ôƒˆ∫ì»‰„È˛‹¿ºÎãÅóπà‰Ê∞íöÃèÌœåœÆœÆÏ≈œÔü¡∫úﬁÇ¨Ôﬂ≠Óüœ∫Œéå∆Ã”©
‹ÍÌƒ∂øòõÅ Ö£˛œÜ∏˛©—∞«äß‹áÜô¥˝„ı·õ˜‰Ú˚Ïñò·®€ßÖÙæùöﬂﬁΩüﬂºÅÜ∂™ï¶Ç∂ÏŒ¥·Ú€®¬ßë’™‰Íôƒ•
·ïÄè¬£óÎÿîÚ‹≠¥ö‘†æ“ìÑûÑ—¬í¬§ë‹ö√®‰â‹ñ¢•È∆⁄ûµ®Û÷’§Ä∞®ËÈŒ¨ïœŸ‰Ÿ≥ú¿∞§Ì÷è∏Ù…ÇØÊÚ·Œèé
„–‡·Ñ»·•ıÄæÚ¶ÚÄË∂‡ƒ◊Ä¡Á∞àã…Ò¿ñ≈÷¿¯∆¸õê«˘ÉÍﬁ˘âê·ç§‡˝˜‚È˚Ï∂·ÛÑë¯üø·Ω˙Äãßß÷üì£ãÒ°˘Ä
˘Ö–ˆ⁄üöà§∆›—·Ôåˇ›˜Ï‡–‡˝–†£°âÒâˇ˘π˘’’µ›˛Ω¿àÀÛÎ“ÇÕıÅ…àà∂¢«öÁÑ˚˙‘®ìûΩΩÉàÔ ∞¢Ùˇ¯”Æ
î›ñÓŒÆäÀ“´´√√É¡ü—€¬ô®£Ü”ã ıäë¡•∂”™Åü°µ”ÜˇÄ€öä˙”Òæ–Î≤õÕ∆ı‰æ–Üù≈†Úı‡ö∑©Ò“ööø…Ì°‘Œ
ƒ“·«ÂŒ Â†∂Î˝ˆ„»Õ÷∆˝Ê‰»æêÛéèƒÊ«“˛õÛ√ãìπâ„¡Ù¿™öÚ∫ÃÌ¶“†—…˙∞˙ÙÚ±©ïπò’∫‰ûùü«⁄€ŸÆîÀ•
ŸÚΩ¨‰·ëàÜç¸ÅèìóÉ∏´æ‹ó™Á§ó∑€ª«„„€”—åÜı† ÍΩ °”»Åô˚‡Ñûë ¬¢Í§§ãà‡•ã…≠∆πÉ”¯åâ≈ÿ§»⁄ï
åÉé≈Ñﬂ∑ü€√÷≥Ù¡‰ËÏÏ˝¸Ê‰ÁÓ•Æ‹©‘´’˙ªı˛Ì≈Ë√ﬂü÷˝ˇÀﬁ˚∑ñ∑é•˚öüÏíˆÊˆ◊•ƒí¸Ä·ËΩÖ˜ﬁπ—€˘ÔΩî©
∫©Ω©”’≥˜Ó‚ˆ≥Ìπ†“˛‘›ûâ⁄›àì‹˚«°û¿¿˙Ã •≈Û‡áÁÉÛæœ¸œ˜›Àﬂ€üﬂÑÅ»ó≈ﬁñ∏÷≠§©º°â™¢å»¢ÿãÄ—í
÷ÑëÁ·îÄé¿êô¡í‰≈—¥¥’◊ºﬂôÕß˚ùéÔÿ¶Í∫ò≤Ó ∫•ö˘∂Í∫≤Ó™¶∏Î¡≈¿ªò¢Õ†©¸¿˜ºﬂ¨˝≈ÚÂ”◊ëù¿Û¬≈âµ
àˇ‡ˆ¶ŒˇµÏòÔˆÊª˝’ÔΩ¡ÍÓæ÷˚Îπ›´µ‚◊ÏˆÜÌ›ï≥Ô•“ª‚»¥©ıµï∫ò˘ôÎ›⁄ÿôÓÓù˚¨Óﬁ¨Óú≠ ÓüÇ¶õßÉ
ª’£‡º≠⁄·±´⁄‘⁄πé“∂Á™πúâß∞£îƒ–Ç√©§®Ü»•±åéñﬁ·˜ñ§êœ Ä˘‘´¶Å“∂„°ÊêòñÚàïñÀÿ•‘¨°⁄æÓ◊ê∞Å
¨èË™€¢ÉÜêπëÛΩπ®®˘ñàÇ´Òº˜ÑÉ˚≤ˇˇüö†æıƒˇ∆ê˝ø˜ƒ∑´ÊøÚ∫‚ˇ•à≈Ïø√≈√á£õ◊˘ªÁØ÷ˆØÊ˛ªÁ”≠”·⁄
∞ë‡ÄπÙ«ª¬⁄∏äÙ–ìÉ¢äÔïôßﬂ∂…Û≠ìÏãÌ≥’ŒŸÇäŒ¢¸‰ÓßÏÑΩåáÔ∫ΩŒÁ∫ö˜ÆØÃèˆˆŒäØÆÓÔﬂ˝ñÜÑ‰õï¥ﬂù
„Ò˙õ¡¨¯◊˜ö¯ÂˇÁà∑Òü˛˚¸˚ÈÓÕÒçà¢Îõõπ‡Ô’‚€ı“À¶ú£Ú©¥ùœå•ìÀêÏπÚ—Œ™Ç›Ò¨í˛«≥‘ôÈµµÛœÒ°’ —
“Ïëí´î“ß•Ùñ•∂©•¡à ı“•ÛÜﬂì≤ï∆‡±˝ﬂÕûÙçı…∞∫µπ∏⁄ºøπ›æ∏ﬁ›Ÿ‹ùßåÜñ·∞Ì÷⁄°õ¡ñüÂí¡ÒÍö„À‰®
˝ÛõÛÒ’≈≈”˚èøˇ¯ÑœΩÿˇ˙ıﬂˇ„ﬁÿ¯∫ˆÛõﬁ˛˝¸ı˛ÒÌÂﬂ’ã››£Œ–Äáß˘ô≤≈† àÖ¶“ÿÇÿÜÅƒáË˙öä¶Çí∆™ô¨é
Ë√ƒíÑàπúÙ¥Í™”àßû÷îÙéïªù˝≤∑€›«À›·˜Ôﬂ˛¯ˇÓÙÉƒœˇ¯ˇÌ¢Ü‡ªÔ˘ûÔÔóø∂øøÒ≠º†äÇ¢ã≤”êÇ¨⁄∫Ï®
Ìí¨âö¢©äÏ§ ¢¢»öÆ∏Ä≠àíÃÅÁû©ä¢´Ä†Ù ∑÷ß”ÈøÏÀ¯∫ÖÏÄù°∫òñÏÿ¢Ê»É∫´Ÿ†∏Ä≈¿Ñ·òÙª¡ØËõÙ∫Åó¯ñ
ÌÎõéØπæÓîÇÇÕçÄ£‰ıª£€›’Ìõˇ˛¿ÍåùÓ»ﬂÎˆÇ¿Ó˙‚Ü ßΩ±‡Á√˘À÷È∂öí⁄ŒÖÉØÿ¬Â†ùñ◊‹ˆ ‰∞∆ŸŸÓŒ˜ª„
’Õ∏ñˆ◊ùΩŒÃı∆Ç’„⁄§ùÈî§∆Œ’†ãçÇï•íì©»ãîé≠ÑàÅ‰ÅﬁØŸ¨Ú—£–æü‘ ÅÑÜÚƒÅ„“¯Ò°ù◊ã®îáéâ©•Ωóá
≤ˆêÚ±≠òÀÎ…°óô∞Èﬁæù⁄‘ã≥£©¨àÖÅáîƒ·æ§â•ë¿ó≤≈Óªã‚˝óÆŒø·Ï€Óø√ÛØùë•à≠«Ë’¸ã∏‰æûƒ±É´ó˚
ø∞î √ŒŸ˝˝ƒã¿÷ÈÛÕ©ìª¯ÌËÔÚ°≤ı∫ÿªËÃÆ˜¸—ñ≥ÿ∞ŸªÎ∆ÑﬁŸìëºô„åÍ“ò≥©¡⁄¨±œµï·˝—•⁄Ëä€ñ®∫¶ÌıÇ
¬Äû∏¯–ê›‰∞ò˜˚´ì—ˇˆ∆⁄··—ŸøÂÕÕ„ªÁÔ‰ææ¸Òç˛ÔÃñ⁄˛Å∑ˇ‡’õï˙ﬂß§æ¥ê≠ä˘ı˛óéËáõÁáßÁßìŸ‘Û≠ÅÌ
î∂ ôÍÅ™Á…Ò¬ä…ëâêò£®Ù©öœã≤Ù√±’ﬂ±îò—Ë∑ÍÖÙî‘üÚ©ˆûí‡˘›éÈ´ùÒ˘Œ¬®‘∫∆˚‡ßä„é±ù∏´è”Ó∆⁄ËÁÛ
¶ˆ∞ªˆ˙¥Û¯˝˘¸ˇ¸ÍÌˇÎáõ´ãÉΩÉ›˘ÏÍ€ñÀ≈∑ãπåí˚ñÍßëÊû–ÍŸü°£ÓÆœÔÔåÓæ¸Ù´ò—∆·…Øﬂ·ÖÄ≤Ô´Å¸·Î¢
´–à∑Ôˇæ˝è˛Í‰ÙÍÌÌõ€±˝∫≈–∏Øëå«âƒå¬ÿ‚È¥ñ¡†ŸÑ‰åöÇËÇä¥¢Ÿœ•õ˘§¶”ƒ•äÓ‘âÅØÕåÖÍ≈Õ¨Ô€Ÿ„˛Î‚
˚Õæπòﬁﬂú˚º∏Ωøªªæ¯˝˛˜˚‚≈›Ø˝√Ûá«∏÷üÙ’Î«◊Ôﬂ˝æ˘˜ÍÌ˚®õ‘¿”µÅøÄºû‹ÍÌ«ØØ«ÄÔ˘˛˘Îª˚ÛõÕäº†
ÏÅÅÄ¥†ãíï©©¿Üíà∂Ë†é—í§À‘¢√ﬂÒúÀ’ı¿ﬁ÷Ö≈ï’∞‘◊Ã«öé˙ÛìïÒ‰∂≤ú«©÷Ù¯ï…∑§√›ñÙ®àŸ˜¬‘”≥Ï¯˝…
∏Ö£¿∞∏‚ÑßóœÅú·√´ä–°€èÃØœ‘‹é‚Ùµ∑‚Á˙˝´òÿùÛ°€ùÇØÙ∫ì÷ºØÇ¿ì•«æ—»„úòçÖÍ®’Áˆ„Ô´À≠ÕÜ£˘
Ì¶∑ﬁ∂˚´∫¥ø“ÏÆ´∫çÄ¡Ô Ü≠»‹◊ª ’ÚÌÚ∫—í•Ûîë°âñ¥Åßòù®êåﬁÓ¢ë¯Ü©ãÉ¡à≈¬Èö‰îí¡§ìÕå≤∆ìÀ¬¶¶
ˇ¶ó‰Ùíëâ«ô˘ÑﬁÃ»πÈ∏⁄ƒÉˆ∫œ≤Êß‚˛ﬂ©Ïò∏ï›ÃÀ¬¡‘ëÀö‘“•∞“ØÀ¿ñﬂ˚íÔ˙›Ç”Ô÷≈âˇøñƒ£ÑúﬁüﬂæŸø˙‹
ùäÆ”≠ÏﬂŒˆ˜Œ«€ß„ÕÔì—ÊÒ„®˝∑˙–ôÔ˚ìÙ›˜∏È˜◊ÃùŒ‹Ûß≤ªñ—ÌëÁåŸà∆÷ò∂¥æ∞√ôÕ˜ä¥ı§à∏£†“®≥Êç≥
∆È…ñı®à¯ùÁê≠∏˜Ω®•à—Ø≥õı∑¿÷Êªªªá«Øßß◊˜◊˜Ôˇûø˛¯¯·◊µˇ†àˇ·àø˜⁄·€ï˙ﬂ¨ª”ˇÚ¸Ê”ÔÙÔæ˛Éƒá´
≥Áè¿»õª˚Â˝¡ﬂ∂µûÆÜ∑ ô∆±’ØÉê’¨ﬁÃ≈≥„Œ»“©À—çù™¶ã¶¶ä¨É”¨÷ãÈ∏∑è›»ßò¢•’È”∆≥Î±˘æ˙“õ˙¬⁄¿ﬁ
‹ŸõÌÔ¨ûÓˆÔùúúúùüîﬁﬁùﬂﬂ‹æ¸ˇ˛˚¯¯˙„ÕÎèüø‹›±›ùûûŒÁÛı€≤øà¢ò¬ÌË“ §À”–‚ºÿ⁄ˇ˛˜¸åÉºÂ˛ß¨ã
Å˘˚ã¸€ñ‡º—ô‡ÀÒÎ˜ƒø››ºúúû»ÕßÎÜÓ∑Îú≠≤â£±ÿ≤Ãû£‚À‰Ç∞¿Ñ˛£äŸÇÓ»ÉÜÑø„•ª¨êÑŒîà© îòç“Îì√°
”µ¢Ê¿Ç∏Íÿ∂ŒÁÍ˘÷ÁÌÚËÌÏË„¯®ËÎüﬁ˙∂ˇÄ¯˙Ù¸ˆÚˆÄ˘ÒÒÛ˝ç˛œü˚˚»óØÔÊﬂ¬ÚÇèØﬁàÄÎºæ˛óÅÜéﬁÇ«ñÂá
˛Ä‡‡˛’õá˛◊‡¡∑ﬂÛ˝ÔÚ◊ˆÁ∑ñá§≥çÇ®ã¢†∫Àíì•ÉΩÉ´»Ì—Í Ú⁄”◊È«ÊÌá‡∑≠Œ≥ò›‚–·ƒ∞π√ñ¸˙¿üÃª”Ÿ¿
íÒÅæ¥’∞°è¶•ö∂‚¿«“®Üãú°€û±Â¯¿äö”–ÄÚƒò¿Ò¡∞ãªÄßËÿ¡ã◊Ò˚ûì˙ˆ€‡€·ËÑ›∑˙≥˙£∞◊ÎöˆΩ·œÍ˜Ì
∫∑ÙÂâ°´©ΩÆ’–¯äØ‘’÷√ÊËÆÔ≠Åı˚£∞∫ıŸ´›ßïª≠õı§¡≠≥¬√êËèÅ„⁄ï÷π”ï ê≈åôœ”ïŸîô¶°†®ú∏ôÅ¿…àÿ
•ñøŒ‰±£¬ñÖÕå⁄¿€‰ªºö¸÷¿üâŸÈÂ©≥¨‰ñóıÏ≤ç≤√ïììÛâ°áËï“º§≥Û•¯‡å’∑˝≤„‰ÕÙ£Îôπ€Ã⁄ﬁü◊îå§¶
ÔøÁã¯üÏõƒüóƒ¬é–íê˜üéÆﬂÇ÷ªÃ˜Æáõ«ë›ïÿÎÀûÏÍÿ˝ı´á“¡„äÏœÖ˚ƒËá˝∂⁄È´·€¨â⁄íﬂêΩöõÔïŸ∑ƒ˙•
®é“‰›Ç»éÌ¥ç‘∞†€ÇõÀÌÎú’í≠‹ﬁı¿ﬂÜÖœÚíûÑÅËÜ˛π†õÃ˙· ‰ˇ∏æ∂∂µ≥∑‡˚¸ÅúÒ˝∑ˆ·˝ØˇÔÖÉÒ”Áµ¸À‘
ØŒÂı«ˇ¿∫ÎÆØæú››Ì‰ûûÕ÷◊É–Â”µâÌ◊õï÷å¯ÀÅßñ≠ò¢ÖŒ≠¡êäÀÜ”¸äË‘ä‘ÇÀÀÇœ¥õÂıÚπàÖëäÌä∆¶î˙∆¶
ıÛùæ¥≥ËÌµë˚≤ΩøªæπªˇÒ˜Úëıú◊ø˛ˇÒÕÒáœÔø˛ÙÓÒ”Øü˛˛ı„˛Øø˝Ùèˇ˛ˇ‚·’—…È ≥  ó¡†±Á≠ÂåŒ◊Úûÿ
»ﬂõ¯«˚€˜‚ØÓóµ·˛Ö¥†˛˘›—òÔô¯øﬂæ˘Ù˛˘˘¸˚Á®ﬂüö®≤ﬁØ÷Ò¨˘â¨˙€≈ñí°±Áëø‚±ü∏©ÙôËØò¬ï††Ú¯≥¿∞
‰Â“™—∆ìÚ”Ë‘–ÃØËŸòùª˜ÿ©ùç÷πéª‹à∑˘◊∫Î´ù…º»ª›ùÆÏÃ◊˝∑÷∂˜∂ˆèøˆèŒéœØÆŒØÆØØÓÔØáÔûﬂºˇÅ´
Éœ‹øæˇ¯âÏÄáÄ€ó˜‡ûÒØêÁØÓˆﬁ‹ﬂ∆ùèæøÁ˚ÂØÌœÔ◊ÇÃÛíœãúÚ†“¿á™“∞´Ù∫Î’¥µ§ò´πŸÀ Í÷Ù—ÛÜ’∞ú¢
õÂîúØÊÜÕ◊€´öÓÕÙö€ΩÅ∂öÄ¸†Ûò∂ÑÁßÌ≈ñ¥µ°€Ü‚òπ™ÈÍñ•ôñŸºÅñﬁÙÍÅ˜Ë–Îπ›û›ÉÈò·«ãªÚŸß´åÜÚ¨
¯ˆá –”ºûÔ·˜ú†óÑ˝∆ŒßÁ˜˙ˆÿãıÇÓËˆ¬ÕÑóÖ¶¨∞’ÕÅ®Ü Â˚†‹Ó˚á€˝ﬁ–àÔÀ¬ùÁÎºø€î¯¶ƒ’Ö¡Ü—∏¡ÓÛÌ
é“ΩòŒ“¯°¨Àï≥©‹öÇÉÇÄˆÄ‡»™’âÊí»ÖƒîÃíû‘©ﬁ≥•¨óÒöå◊–üÜõ¯è‚¶¶Á‡„⁄ÆÊîñ›ÇËˆ¡´‚‹ñË·≥âª¡Õî
Û“Æ”Ÿ˜ãû»ÀπÆÂÇº“Öı®œœôñ°¶Îÿ˜¸ë·úÎ˝Ôππ˚˜ƒªÛœ¿â¸ÒöÔ‘È˝˚ÔÁ≤˝¡±¯‹Õ”ÁÃ†µÑ≥ΩÎ∞ÎÆÖÛ…ùÛ÷
˝Ä∑˚ºŸ„å‹ÓÀˆ´º”†∏Õêáä—ı¢ ¿°Ì‰⁄∆¶é·ÄòêÖÄ£◊¥£ÈÜ⁄¨“©–ã≈∆Ç‘¯Ì´•Ÿ¨Ç„ÉÇÓπ†⁄˛ñÑºÑà¢ﬂÅÖŸ
á¶√Ó¸∫ŸŸ‹›€ﬂﬁ€øπÚ∏ª˛Ò˚¯ˆè˛Ìßá˜øΩüº˚˘Ò˝Ø†Í–º‹‘Ê˘Ùëï◊ü°Øøæ»ù¿ñìóﬂ¯˘¯èËÑœÄØﬁÛˇˆßÁ—ï
ˇÔ√”ãçÌùù—‰Ïπñ¶ÛÆ‹´‹ÆÊÿ⁄ÎÜØÁ˘⁄¶ë†ßäîÇÑë°´ìÂ¸∂’Ø±•≠ñàè—À—§ÿˇ£›»ÒÂ“∞¢ÊÇÆ∞îÑ´õæåª∏
˚Ïø˝˚Ê˝ß˝üæ¸Ú€ßﬂ◊˛˝”øˇß‡˝ßÔ˘Ôˇ◊Û—óÔèœŒ¿ÇƒÄπùÈ∑ãŸê¥ñˇ÷ÕëÚÔ´ÿ’⁄€ìŒˇÈ¡Ó˝˝„˚ˇüˇ˚ˇíü˛
ºâ¯˜È∑∑üüˇÒÊ√ØÔøﬁÇ˛Òˆ”Ô˜¡¿˙ó˜–êÉ´©™òıÍÉ¯¢íƒíÜ˘ÜÆ´˝àÆ¡‚≥ƒäÇÜàã÷£Ê¡¿ÍÅûùæ“Îî’§∫Ï
¸Í¨•◊µÃ∂ä”‹âÍªΩˆºÎ§œ˚Ú†´◊˚°Ï˜ıŒﬂüÍçº—¥ª€äù≠Ó¿≤Â¿‹ñª˚€ªáªƒ¿ô˚«áá«ä«áß«‡üÒÈ∏˜¯∞˚∞ø
„≥øáÄá‡√€˝„√¿˜¸øœ¸µ˝˘Õ∂Àùä—“Ù¢§Á¶¯û‡ÿ¬‰à¿≈§‰ô◊Ù¬Ñ»›¶‘ì◊î≠‹≥àÄÍíÒäÅé»”•¨ú†ìÙÀΩ—Ω
µÏ’√ÓΩ”Îùá›⁄Ô–Œ¿°€æÌ˘Ì«ÔË€ﬁÀ≥ö¶”Í˝∆ÿÌˆ˙õ≠¡ê–ÏèÍÜ·Ø‰˜ã˛∑›ﬂùî˙£ ßÃﬂé˚Â‡–Ìïéõ˝Òªø‰
Ñ˚√∂ª‚˙£π±ª‘Éû™ô∂¨À‘§°ÈõΩÙÍá€ü»Á∆õ÷’¡ˇÁ”∞ÒªÕ£®Í˛¡Ÿà´Ê–∂ÉÛíﬂÔ˙‡‹ñåØµ´πÚæ”é≠Ê Öå»ê
¿©†àúŒ√‰º“‰µò•ÀŸ£πöˆõ‘øÕ¢Õò‘–£‘úó¬êÀåπ§ÊØñ’Ï≤’åá¨µåÿ¡ÔêÊ¿ÖóÛàÕ≠È‡ïÏ®‰◊µ¸äÑƒáù–ïÃ
…ë–Õ≈ÃÉ∑åÏ‚¡˜¸ëèﬁÓﬂ˛Å√ßﬁ∞ΩÒáŸ‘⁄∫ﬁ˙˛Ö˜«ã‡Øç¡Ô¡£á˜¥Ô˜ØﬁŒˆÆÄª´á⁄Á≈Ê∆¨ŸÔÎùñﬁÇ“ôœ®æ´
ç˙Õ—©Úñ∞÷¬æ≠ó⁄˙â•÷óÇÿ‘ÂÇœö»√Í´Êâñ±À¨ŸÂÃ†ÎÖö‡’Å¢’…ñÌ÷µÖùﬁÌ…‡¬Â±ﬁ¢∂‡◊ÇŒË˜∫Î¡∞õåá›È
¨¯¥õÕ÷†¢¯ééŒŒœÆü˝É·¸˙·ÕÕﬂ„ªª◊∞®«Àã®®«ü¯¸√ÜØü˛‹ú¸€˙¯ûñ˘ÇäÛπèÄﬂÁÊÒáÌÂã◊ß◊˜÷ªá«Îõ€√
√…»ÛÕ›·˘ﬁ÷â≠¶Ö∫¨ûÄ¶¥≤ÌíŸÜ £‡ÃªÕ‡Ö≈Ï∫÷ÆµÎı’⁄©‘…ÁÄ˙†Íπäâ¶ßÖÌˆ¶´îÊ⁄ ı∑£û≠˙¿ﬂΩˇÓßﬂﬂ
Îˇ¸œ∏úÔ∑ˇˇÒÖœï∆ˇˇÁÌ√Î˝√ì°…‰ÍË™›ÍÊî≤©‰‚©ûÏß’∫‘∑òÌˆŸ˜ÙÔ˘ã˝«∑ü˛˘Ø¯úÌÅ¥ﬁªﬂû˚ﬂ¸Ñ§˙
¯£∏¯˘ÓÁÁßèÔœÆﬂÏûÒúÕÁõ¶È∂ëöÌäß∞ÿ¨ƒô€∆„À∏àÜèÀøìƒ±ííƒ„…ê§–°Â§ä°÷Òßâà˜Ë‚ƒëÛ•πΩÍî◊›˙∆
Ÿ»ÃÇ¶Î±é´√⁄ÆÔ¯¢õ˘»˛«ÎÙÃﬁù–ôåÉ·§úêÕ∫¿ô„ÈÔ∂ºÎèÁÉÒ¸≤∏ûç«≥¡Ë¯∫òŒ˙Ä¡∏òáÅáì¡∫‹Ñ¸œ˜Àﬂ€
õüﬂ¯ùîå“¿µ…¡∆ÈÆ÷◊¢ó€Ùå·‚¶åü‡’ƒ±∞±ŸƒÖ≠¡™âÚ÷âÛÍÇë‚¯Ì±ÜˆºÜ»–ˆ“Ûâ√ßÌ∂∞ÂÜôØì˜Û√Çìîù†‰
ÈÜ≈∞¯á›Ç¢óÊçß„πÈŒØ»ÑΩÃÔò™ùå†˝—∫ª±†∫É≤ÇÍé•¡±’ô—›â≤µòÂ∆≥‹ò“úÃãÉŸÈÚ¥ØçÁÕÓ®†Â˚ÕŸÈÇ˜
Ú“ﬂóèÊ∆Ÿù¶˝π„ﬁú¯ßá€ÚË÷ÓÕ“ÚùÒö–ù•Ê˚√Ù‰ﬁÈè¢ö™‘·⁄Â∆π≠åó∆ñÚŸëê†®¢õ¶ÀµÖÃ≥⁄‡õ•Ω•Ÿ∞öŸêŒ
ÓœÖÃ¶⁄Ë”û©£àäÈ∏¿ÒÀ£˙æÓîêé˛ìËµ“ﬁ£–ÂÉ˝îÄ†û™È•ƒüÕ≠®øŸ±ö÷•§§∞§ƒæÆâà¢Á£ääÍ∫¶õ◊…Ô˘¢Óﬁø
◊˘ñΩßﬁÆÿœø˘ÃÌìá∑«Ôæ∞á ù˛ç¯‹◊õÁÁ˜˝çìÛ∞…÷ò˝Üú‘ƒ∑À¬€‡úÔàÈ“ê£€ç¡¨≥∞¿à⁄æ•Å†¡îÈ¿§à®
ÜƒÄÉ¢„¿µ≈¶ê¬ì ¡∏Í´ñ≠ÿåÍ“Ûö≠ö∂Ï¬º˙¨ñÎπ∆ÀË‡èÇòÙ√“—øªûˆß„Ò—Ù∫›ﬂÿÿ√ﬁ›ª∏æΩæ≈∏πΩ˛Òˆ˜
øÚÚ˛›ıÀè€∑–ëøæ¯ˇ∂˚ﬁ˝áﬁˇ˙ûè˘Â˛›ØØßﬁ˛¸ÇƒÜ∑√üÄöÓﬂ˝ºê˙¯˚˛˜¯¸çı¸Ô‰¸„˜¶ˇæææúè¸¡◊¥î÷Ïï˘
Ä⁄ ÓÊÙ∫Á†∞˘∆›Õ‡¿ê©úÕß©¿Ú°◊≠ï´Ö∞‹©Íí—»—ûÏâî´§«ÁŸÜ“ÒÕ¨ûˆˇ•ãÌ…Û´≥∞∑ü˛˛˚”Ô’Ô˛˛ßºÔÍ
’◊ˇˇÇ¨±˚Œè»ıã◊ª˚Ü«√—ÿ’‰∏ªÂù…·â•∆øë“Ÿ »Ê˜ôÊ˝†à€óèºØø¸ˆØ¯‹’Ëí¸ÍØø›ˆΩæø˛˛˙Êﬂ˛Õ›„ªóØ
Óæ‹‹øΩª∫¥ø∏æ⁄€ÿÛœ›ê°Ÿ¥¡‚πÒ∏ëèëÒÂí≤˛åﬂ˛ë€Ÿ˙ëàÇ¸¨àƒ⁄§àˆâäÇÇ´„¸©©î—«≠å⁄µ¥Ë†ºÉÜüàÈπ÷
“ÿµÎ√´ûòÇı√ËË˙√≥øöö—“Õ∂·Ü±¶Õ⁄ÆÔπàÄ°¬¥úâ‡÷∫≤·»ÜùÕÄœ⁄êç‡∆˚îÀﬁΩﬂØˇ¯Ìˇ˛´ÛâñëÑ∞™∆À»”
∂»‘πÿùç∑≈ôƒù…¥∏§‡≠£ÌâÈÚâú™ì´–±öõ◊Ø∞âíì®¥¯‚Â∫ﬁ‡±¶˜ê§•§Ø·Çã˛Ñ‚í–á°±–Å–ÄÿßÏõü¥º“Ò…∑
ä§Çû—õÇùï§Ä¿ë√ÒÊÑ¥…˜∆‘∏†Éª£‘ˇ ç÷”»·ºµöÁÜÉÛÃË•≥ﬁ…æœ◊≤”’‹Ë√ûÔé´†îÿÙ∂∑ÍËßõÓ≠›ÒÔ≈ŸÔÏ
˙€Ÿﬁ≠œÊ˘Ω—¨›üê˝â…—üìÉ°µôæó˙£ö¶÷˘´∂…∞≠ˆªﬁ„´Ä∑Ù™õ°ﬂ •¨¿‘ı°õ°—•≠û∂·è±÷‘™÷∏äàõ√Æ´ñ–±
Õõ–Û®€°¨Ó¢Êî–˛ãÏ≥µ”˛´Î÷…èΩàôÁ˙πÂﬁÁ…≠µª¢π•ıÅïƒÑ§¿ß§©°àÒÜ˚ç±¬úï‚ﬂ≠Û≈‹∫¸˝ü¥¯Éÿﬁµò
è„õùÎÖÔ˜√Æüä≥ÆûŸµæ¸µÒ˛∞ˆ’˙‚≤˜ÈÃ∂ò˘˜‹‚Ï‹ÀÖÀµ’Ï”Æ¥≠Âä≠‘Ÿµ≠»ËıŒ´∞Í°∑≠Ê€ÍÅÜÖÑõÀÃ¬ì∞–
ŸîŸÿπ±Æ¿∑‚ç¥‚±™Ö´ÚÇ‡ÿ¢ºÑÏÈ´ñ£∆•∫‹Ãß™’ı§à‹ß«Ñ∞ˆÔ–ÁÎÑÉﬁÁÿÔ˛∆öËÕ¶ì’ˇ’’çÕ≠ÌΩˇ√›£≥˝”Î
ø√´á”ª◊ó√˜ÔÓﬁˇÇö›∏˝ÛÛÌ€¿˙Øê∑ﬂÚÛÇ˚ª∑øﬂÅßÅˆßø˙¯ÛÚ˝ø’„ªÀá∑óÅ˜ÔŒÓüèÆ°ÔˆœÆ∂ˆèÃ◊∂∑¶´´√
„Ò–˜ç”°ÂÑˆﬁéñ¬˙¬Ä°êÀÚ£ç∆πŒÙ≤û¬ë¨¨≈î≤Èí»éÖ¿éô‘±≈∂àÚ§◊Ç•ˇ√¿∂≥’ €Œ˝˙Ù¨ﬁÌÕüﬂÊ‹æ˙“≠‰Á
øêïõ¸ÂﬂäØÓû∑ˆè˙”˝°¢…≠˙≠ìê°à‰©’ ¯ô‹ı‚âª´øÉì£˝ÎÁè˚˜Ô˛Ωˇ¨ºÒÁÔˇÅøŒ›Ö´œ∑Ø¡Ç˙˛ˇ˝ÓÍ≈õ
ﬂ≥õáìãª†ˇÉ£≥õ”ãªﬂÛÎ˚ÛÎó«˘Á∑á«óõ€ƒá¡–ïÿæüÊ •–ƒÜãÁ≤†ÀÈ‰âäø∫£ñˇÃ†ÀÖÿºﬁ…°ç‹≤ñ›¿ô≤¡’»
À∂ò‰ÚËªìì‡≤±¯ˇﬁ·˜Ÿ©ƒª≠‰ê©£¶ÚÆ´Áù∆©ØàÖëÄã”íÂ˘°œ≈∞Ê¬û·Ûåó£ΩŸ‹õÅ¥ò‹Í‘¶¨◊ËÁÍ™œ‰õÌ¸Ω›
€®∑äµœ¸œ˚˜Àﬂ€üﬂ⁄æ¬åï„ÇÜí ú’‚åä‚Ñ¬‹‘ê—√ÂÍÉ”ùÃí∫√ïîç‘≤–≈Ç ™∂Ä◊Ì¬•µ…Õ°Ùó¥∂º“ÚâÄÈë
…∫ÉÅ«Ó»˙¶› Ô–ﬁáÑ≥ÿ‡ﬂÌ‹Ú¡Ø√Äµ¯Ω¬ïÍ§Ωæ—ô“ —î»¡êÅ‚ˇ√òÏåÒﬁÑçÍœ“›Òä®É≤Ã˜áŸﬁî∏üˆÀ∂ﬂ—
É¥™Ì¥Á˝ˆ®ÔÕª⁄∂÷Á…ÙËÿÍçÛÉà…ÊºÀÃ◊ÄÉŸ…¨∑›ÈÙÌ˚Ìë´∂™•⁄ πÿ≠î •∞¸¶ÕñÛÈ÷ıΩ§≥µù´®œÕå—§Ç∂Ω
∂Í’ˆ˚Ï…ÿÛÓÚ≈∂ì†©„òñ«áÕ Ø‚§Å†∫çº€¸Œ˜Ëúﬂ†ÏòäÍîóΩ⁄‚°Ãëª⁄˙√»›πä™´àä˛Á«§∏àÕÅ öö™òË˜¸ë
á£˙äˇ‰ùÇÒãÆÓ‹¸ûÀÁ◊õóÔñ¨üÅø‹ª˝ÿΩ˛¶±›øûÓüıΩ◊éùÒñ‰œë⁄⁄ÿ‰Ù¸À«…∑‰¸À™∂È§ä≠◊ÿ•…Ÿ´ıŒ≠≤Íô
ïâ‡µä´’πÑù–ãËà∆ÀÏ≤òŸ‰‘ì◊Ñ¨—∑¿ﬁ–Äèñ≠⁄∂Êª⁄ƒ§à√ÒÏÿß∫ãŒ¡¯∞¢£†§ÚÑ‹µ≥§Ñêƒ‰àãÄâœÊ‡ﬁŸ›
Ìﬂ‹€‹Ÿ€ﬁ´ﬂÿøŸΩæºã∏∏ªΩ˝ºˇˇÒÊÚ·˝’´»œﬂäíøÅÎ‘«≠¯Ñ∏˙˘óÒÚ·ùêÇà∆†‡≥ŒÓüéÓûéÆüÔùÄ›Ô√¡úÅ¡
˛ÃÊÛ˛∞ÁÉ‡ü–ÈÎÃﬁ†Û”ÅÿòÍ’âá›∞˘Ç◊Õï†®§œ•ÀŸ‘◊•òØñã•ÿîòΩï£Ò˙ñÜÁã∏⁄•¬ÆÜ∑’™€æÛ∫Ì≥Ø˜◊ÓØØ
∏¿Ú˘¯€ü˘˘à˘¸ÙœóÛü˛˘ˆ˚∏πÇ˜‘üÕ‘ïﬁú‚™Ê“‰Á¡ëå›˛Á•ˇ¸ë∏˜˝˜ˇÇñ¯∫Üﬁ¯˘ˆıòœóØﬁû‹ø¶úúﬁÓüúÌ
µÏú≠ÌÓÕ÷ú∑ß”’—¯∂‡ÄﬂË¥ŸôÕÙ∑÷◊∂◊÷◊∞◊÷◊ÁÛÈËï‰ô∫Ã∑‡ç≠êî÷ §ô‹ÎíÀßÈ¨⁄ ⁄†ç‘¢ù≈ôíº∆óÒ±åØ
í´…’¬Æê¸¿ë„Û»´ï££”À‹˙Ÿ»¨ß©˘ò’¡ΩÜ®û≤©á…ÈÙ“⁄¢≥¢ìïπáú»Á£Õî¯´°¿Å¯Ç–ë¥ìï±Ñ¥ô¸€ã…«é»∑æ
ú“Ë¥ü®«¸˜œ˜Àﬂ€üﬂ†Ñ•¶ÈÒ≈ƒ‰£é–ÏÑÊÊ†¶á⁄ûÊ¯¿…Ø»Ü°ûÑ”‡¸üåÔ…€««¡Åóê˛ò¬ˆ√î∞Çò˘£ùÄÑÄ£∏Å
ÊÈª§ÈìÜùÊ¥≠§Âë¶Ö¯€é¸ÇÈ‡™üë‘¨§·ëŒñ∑•˘…—…ËåæÍÜ∏çæ±È§º‡ìæÂ˜¬»∑ÀÑ˝™Ë≥ßúéÖ˛…éÊ®√°Ã∑ÌÑ
Ê˘√À…∆¶ÕÈßôÆ£Î˘–≈Øç˙Õ∑®ôŒﬁ÷Ù“˛∑›òüÂ«”‹Ë⁄ÃòŒä˝±Ìª Ã≥éﬁÛ©ä•¿≥È‘∂õŒ„ß»Í•è‚ÉÂÃ‘‘÷ﬁ©ë
≤éùöÎ≈É°Ïˇ∫◊˜ß∫≈§î™¯Úµπ§≈ÁíÉ¶ô‘ìËÓÖãÍ¯ßïøÓ“˘†ôÚÍä…øöÔµì≥éÆπ£êõÜë∫ö®Ì¢∫Å»ÙÚ‹íë›¿°
Ùä›¸˚æà≥ü”«àóÛÔÿü˘ÃÍŸÇ◊ã√÷Œﬂˇ·˛¸î∞π∏õÌﬁıø÷éú…éﬁ€¬ïú¡€ãÍÇú’ËÀÖñà®≥∂ƒ“€¨–“óçëÎ¨»ë 
¶ç÷Ú’Íúäœ®√äæ⁄”–é€¢ﬁñÙ∆∂¢∏’ê¥”“Õ´Ÿ∞î”“≠≈Âçﬂ€Ä∞êê£îƒ‘Ô∑Ú¸ƒ‘êÂ©Ô†‚ Ç∏¢Ï˜∫·àÄÇòåÜ
√ìÒË≤ôœ¶≥”…Ïæû≠Œ¯◊∑◊∑ˆ∑ˆ¥œ¿ö∏∫∫áíÈ‚¸—™±‚ﬁ—¥¸‚Âß¿°Á≈ˇü¿‹∏˛˝˛˙¯Ê·˝Úé§Ü˝ã€ßΩÛ€ùËá´
ËÿúÚˇ¯æ±≤µûÕ˚˝–˜‡î«ÌôÏﬁÂ›≤É≠ºÍµ∑©∫Ã◊≠™≠ÏÊ•Éï≥ƒµêËÌ‘ ı πú£œÂ”‘¶â≈ªµº•äá‹Ë˛Í¥∂ª¬…
àÍÒ∏˘˝˝§ıáÛãú€·≤©˚àÿ‚˛À´Õ·èˇ∏π˘„ÿÿªûÃÊ∫‰·≤Ìñ∏Ö˘Ó¨à ÿÅÇË¥ùüÓﬁ›Ω¯Êë•Ü€†ª«ùè«ó˜€áó
œÛıΩ¡Ë•ÏÄå›∑∞Ù”◊§µÕñ’—∞îçŸ∑¥û„éΩ®˚êÅéùâ’î€ï∂»Ò≈¬ì©êÊΩÂ¥”“÷ù≤Í§⁄∆Öı™¨‡Ïµ†“Êù¥Î§÷¡
Ö•Ç ÊìÕÍŸπ≈Æµ…•öû˙∆ßóÂ¸¬“´°÷ç◊ÍÑ§‹ßÑ∫©¥“Ê¢ùëπ¯ß∑®§î≈±®à≈õ˛…ëÖ»ëÖ™¿õ©¥¢æ‘˛º˛Ü‚ÇÔ˘
˙˛˘Î˚Ûõ·º†˙Ñ·¢¿ãƒÜ–Éè≈‡˚¬…”¡µ≥·ÿ£®È‡†¡ ˘Çïíåê†˘“ƒõ≈öÌƒ•ÜëØµíçÜÌ®Û…÷ƒ≈Ö‘Ùî“ï»â√˘
≤∂∆µ—â≈ïÛâêï‹“íÇìû‰Å∆í≥âî∆œ“ö∆ÍòÒ≠‚ÕâÂÙ§Ô‚ê∫ü≤É¥≈ÂÁ¨†Ì¯î∆‰ìí… ∫…π…Ñ£íúöÈ¢â©äÕîõ¯
≈©≈‘ŸÇ¸ÿÿñ”≠ë§•Ç‡”à¡£‰É‘ºÏ†îåÈ‰ï…Ω∆≥µ¬∂ÖÄ„ËºûÀ†âÚú«≤ô™’¨â™í´µÀ“≠ﬂ•’âéê›Û‚ö«≥˘ˇ¢å
≤ÌÜ©—©ƒèˆ˘Ù«ñπ¨Â÷ëßúåÍ‰ß«ïÍ¥≥·´„Äµ®∑íƒüàÉÇ¡÷πÙ™„”Åâ€ñ™ÅΩ¯™Äƒ’ì∫ôˆ¥¨Ô™à—ÒºÉ‡¸ƒ„‰õ
´¶Äì»åÚ≤Â‚ï‰î≤⁄øÒÎÌﬂÜ´«∆Ñ‡”Ù¶≠≠¨’≥˙â÷äÌ©Û–‰ã≠æÿÊ∞·°ﬂ⁄®–‚◊Öë™å»ë∞µﬁ˚É◊öÇäíËâÍ€“òŸ
©„î›à ˚∆ﬁ˜Ø¡Íæê€‰≤Ï€ÙÏÀÅ≈ÖõÅîÃõﬁ—ÂÜÄá÷”…òﬂß™˘ÎÇ«àƒ¿«ÄêÅÑº±ùˆ∆√ﬁË˝–õÙªñÁ‰Ó∑ÏÙªΩ˛
∏≤áßÕÎª„áªß◊áó˜ºßØﬁùø¸Ùø˛¯ÈÏâœµˆÏ·≠ŸŒﬁõ”ª∑á◊è˚Çó∑†ØÄ£Ï¬ûüÔú¡∂èòÆÄ†íÃ∂∂˙«”…ßÎ˚∂
ª‰™Õõœá“ÓÓÓ‘ˆÜÏ…⁄Ãö∫‡ë‚≥™¿‘ã©Ê•¿‘∑Í≠˙≠“»–É¡–Ë¢Ã°†Ùé”™¯¿óπ¶÷˚ªØûÏÊåŒéˆÆÆÆπØﬁ˛∫˙öÂ
∑¢“Ö‡ˇ˙ßùÿ˙ÈÕœ∑Ô§ˇπ∫˛Äﬂ˝»∏€Éˇõ¨À·îãŸäÖÇ†∫å∂º≤ŸÌ˘˚˜Ë‡ÁÍˆ≈äÛêù†ùæ‹Öº¯∑˝¯π›ø›Ò√ë¢ò¨
âÕ∑™µ“¶ÉéªŸ•…“äÔ≠Ê®Æî—Î˘˝´›≤òäÕ÷ÍÏÆÓ“•£ëÕ”™î›»ıòΩÃÿÃœï¬‘ø èÖ‹ÄÀƒ±§ÅΩÜ¸ù”û˛É¥´∏“¿
Â∏Ãº¡º´„«œÍ…Áµ¯„⁄∑–ƒåÀ§ƒ∆º Á—éò§£àÒ’¬Í•ï∆∂ßä≈≈¨…–‚…îÀÇÿ‚∂Ö†â‚‰àß¿¡¨Ë˝±Ö‹÷óøÂ˚æﬂ˛
«⁄˛¸ÊÅ©∏©Ë”Í±∞∑îΩåÑÖ√¡’ƒ≥°™Ò…œ»†¶¢ËÒ÷Ü◊…◊Èæ÷‰„µ“ˆ¨‘ﬂŒ◊üÿÂ´ÀŸÍÚæ»ñØ∂ê“ì≠Ûµ∞∆ã´’±
∏ Ö©ï‚ıö¡“Í‚≠öÎÚÄÿ”∆õí⁄óÁ±™Ä’¯¨ƒ¿·Âé≠≤»∂•ù®≤à¯∞°úìÌÖ¢ï‘€Ú°õ≤—ìöò»¥Ñ≤ íÂóÜ¨∞‘°Õ‰
ìÍ…Ã£ôÈ¶€≥Ì‰∂ÖÂõ÷ÿÆÖï⁄ÿâ∂Œñ⁄ÿπç’›ô‰ï¡´Ì¢ö“œñäŸî¨ƒ˛üˇÅÏø˛ÎÄ†»°‘†€ñıë≠≥ïî◊ã¬ÙÜﬁ◊ãõ
ßùˆŒ–òŸ≠˜æñŒì±‰Ú≠ø—Òø’íø∑ÚÈòŸ¬¬ÜªúÄÄŸ¡™íàÊ•ÈñíÊÒ≥…ØÆè⁄Ò°ÜõÔàÒ”√‰‚µŒÀÀå∑Ã„Ú‡†°©®Ú
∞˝ÔÛùÏﬂ∆Ì‹ÅÜﬂ∞Ç˚ø€àõ·ﬁı‡ı˜˙∞Û’ﬂæÌÏÌù˜∂œ∫ﬁõß˛ æ≥ŸŒ≤˚´≤∑‰ç≈†°öÂ⁄™∑‡¡‰êñÖﬁÀ∆¥¨Òﬁ·‡ô
â˜Í§à∫ß∑ÿ˘Ö¯Åñ‘‡ºÖ÷—¯®Íªá∞É®Ωâ¬ÉÖÇ∞°†åãä∂¢™ÒãÀ˙Ç»ù¨ú±®àøˆêƒ¢¨¸Á ∑∫ì¨˝õÍÜΩË∂˙–Î
ıÙ˚ùﬁ∑”ÅÕ”ï°Ááﬁ–üÈÙÜÉ—Ï≤’ﬂÿ›æ‹ºº·ÿΩˇ˛ÛÍõˆçÏáà˚¥†˛©∫Ú˛Ñ°ˆ∆¸˚Ù˚Í€ˇÍ˛˜¸ˆÎç‰ÚÛ¯ÚÉ
¶Ñ˜õ†¡ú‹ÃÔûûÉÑ±öÂå∫ÅïÿºØ±÷Ω∞Ú™˘ıÈø˛¥ÊÆ÷îèµÄˆ•ä”ŒÈµ´π¶—·ô∫∂€Ó◊À•Ç àßËºÃ°Ì§ú™ÉÛâ°¥
Óı˘ùâ˚∫∑¿˙˛˙ÙÒÛ˜¥‘‰¯Î”ìÔˆ´“ˇ¸˝ÊÁõ£õ«›ßÎØ€éﬂ·˘æ“ãÃÉÑ—¨“∫ÿ”û⁄ÃÊ¥‹˚ﬂ›∫ª∫Ωºòˇ˜¸ı∞˜Ú
£ΩæÛ≤ò≠∫®Ó◊íµÊΩ’ä©îÚ©Ùä√ÿ”Ë¢ç∆·õÄ°¢—œ·Ùé‘ó î‘…®µ∑ú÷ÃÎı¨÷òÕÈï¬©”ùå∫Ω‚≥Û©èøòâπàÏÇâ
Èóû≤©ﬁãÚºÎÑ¯Àó≤â“öƒá‚©Ö¿õüÅú¡¶‚¬‰¶°Âê°á≤πÑÉáÂ§‹…ß¥∫´ÛÖïÇØ»¸—î ¬Ï‰áéˇ⁄Ëò§Êø√Ω’≈Ô
¸ø’≈áÖÒ€ÔﬂΩ˙Òﬂ€üâﬂÃó∆áÖ¶äÖƒÚ–®–§¿Ùª°õÒ—ãµÛ úá£üé§ÿñçò¯öÑ”éïÿ€åµ¯›Â™˚¨–¸ñÒ©ΩÀ◊ó» 
ˆ“≤ºƒîó©÷Úå™§®Íúë™ÆÁ™±¸µ∆—¨£•ö≠Ò¬»ë√äå“†¡—ÏõÕ∑ë™∑ÅêçÑÄ®ßô»ö¬÷—‘É¨À∑ì©¶ñÍ™·©∂ñÓ„π
£öÃ≤‰ñÏ€Æ‰¶âßõ…Ù‹ãáèß⁄…™πÖ—çî™£‘ƒç£´®—∆à¬“Ç¨°¯°Å÷‰ñâ¨ö „ƒ©≤â§‚ˇˇø˚ﬂ˛˛√ﬂ–Óú≠∑õÙª÷
ÓÆßÎô∏ìèØô≠‰–®≠ıèË±”î€ô¢ô˛˜Ï˘ø·ÀÕˇµπÙﬂãËœﬂ∏Ù≤ﬁƒÇË∫±Ëßºê©—Ûâ•üìÛœüÑÏﬂ‘“èŒ∑Ûô«≠ô‚í
ÏØ¨µôÁı÷ı≥ „Ÿ≠ı¨ë≈Æ≥¸úììõﬂÛ≈ÏÌ‹Ç´˙øÁ∞çÈÔ∂ﬂÙÅé∂ÄéŒ∫Î«¡ Å¢ø€ıß€ŒËµ√ö¨ö›âñƒåï∑´òû£
ùò‡ÿØ±‚çÌÍ¿®“¸ïéÜéûÏÄÖ—„©‰Üß∑ÑÖñ§’Œ÷‡‰–Ù»Ÿ∑§ÕÄˆ¯óÚ°™õö·Í•¿ë¶Âÿ∂ô™π¬ÒûÑÒ‘ÅÔ∑ÇÍ£¢ä
‚íÓó”ƒÎÓ˘°£‡±¶à®Ù¶«Ë˙ÅÎáﬁ˛ßﬂÈç˙£—È¯ÂÛ∂˘∞•∂ˇæıÚ∑ÛÍ˛ø˜ıäﬁ‹ÊãÔÊ˝ˇ¸¸ˇ√—Û˜∂ﬁ°€∑¿ü˚óè
ÓªÁœ∑ŒØ°‰ë¢∂ŒÔ∂œçÓˆŒûÔùﬂÈ≠ÏÓè¶√–’”√ìâ’öà®è„Ωøô÷œˇ¶¸’∫∫ﬁÅƒèÚµ¨”ØÚ‘Ù√‚πÅ„õÅ’ä∏Å…¡¡
¬§⁄πƒ™„Åæÿû˙“Œº˚Ç˛¥Û˛Ò˛Ò∆˜ˇøïÑÙƒˇÒÌÀÌ„´˘∑÷æﬂùà¢‘Ñ≠ïÄ†öÖ“à„ ˝ãŸüáõª€õ«Á◊ßÛ◊è€áÁ≥µ
°›ŒÔÏòÆ† ®¿é•¸ëÿÆ°ç„—˙œ∏ÉÖƒ“à¨ﬂﬂ¢¢ë⁄öÎÁà›∂¿ÒÂ¥‹Õß»ïî“¶’íËéŒÚ¸¶’£∞”£Æ≥·≥”à°∑óÛ´‚Ó
»ï†ÂÁ¬Ê¬ô¶«Ù™€†¡æˆ÷ÛàêéÒ•©Âé⁄Ê©•íæµ¨Ê‚ Æ±°ï–πèà–Ãêâ±à©’“≤Ë¶Õ¶ß±˘Òô´à¸ëÁ°ÿù˛ßàœº’
≥√Á ÌÆÙ≈Ëçˇâ‚øÛ¶‹πÒ’Ü≥è™ºì∑º∏·«ı∂ØÆ∫â…éã–∆‰á≠∆õÀ∫Ä¡ÿñ≠∏‚é£™É Æ¢∫Íâ¡ ∆Ç’Êì¥ñË©Ò∫
ÍòØÒ≤õ®¡ÒË›¿õ¢¬ô»…ﬁ¡·µ¢¶ôßÂ‰À∆õòÄ©Ÿ·ìÖÜÁÕµëÃã…¥›—úçÂË‰ñë∆Åô¶⁄Øéÿòºá˜…úõ®¨ÒƒâÖ‡ï
¶ûØä¯ï‰ãÜâØÉÉæŸ‰±∞Ïã≠¥–‚ƒõ´∂—»ã©µ“¯ìŒ—Õ‘€•ÑÁ≤â“ï¯Ñß∞™÷ê≈˙Ç◊ “’∏ÿà¿ú©’µÀùÕ≥™”ÈÚ¶é
õ≈˛õ≤Ùˇ†ªœ˛ã˙Ïø‰÷íˇ≠…˛‡ïÈ—°ÕçÈ ´Ç••¸Ã©Ø…˝’üÈÍﬂ⁄’êˇ„Íˇ„⁄ˇàÁ⁄ø‘öø‘¸õø÷öœöÃí≠¨É•Çê
˝’‚ñøÁãÿ››º˝ñﬁÜ‡ Ñ≤˛“›ªÍù¨ùÜ´¿ãª’Ó‰”ûÆô›˘È¬äíÂãÜê£Ë¶∏ˇÁÂçÖëÂ∂†∂πﬁû∂ˇ¯‹Å§¥é˝±¿Ç∞†
∂æÄß©å§äÅ™¶Î∫êÅàÑã ‰àê•ƒ‘ı—˚ùˆá—ûÄÜ•õ∂ÈΩìÖà≈ﬁ¯¢Ö≠˚À”êØ≠√ø‡ƒƒ¢œ‘‚Ñ ‚±≈ˆà¬Ö†Ü«ãÌ∑Ñ
ªÁ€Ó–ıÄ¶ÁÜ≥¡Ï„º∂À∑≤˚∂˛¯Ω˙˝ˆ‚Ô˙ı˛˚Õ€≈áë≈Ìπªá∑Ø˜ÆﬂÒÓÇ¡‹ºÄ›å∆ñ¿û£ØˆÇÆÓ˜Øü˜Œ§Ô◊∑ˆ∆„¡
ﬂË»°—ÓËï∑ûúùô˘ùÓÌ„Ï∆ﬂÏ™õ÷Ω∂´∑ƒÊÅÿÔ’§Æ≠©¢¥¨âÌ∆≠úáÜÁÔ±≤ïÔÂ∏ì“âı≈ ¥Ì‘ˆ◊«óÁ«íó◊∑óœü·
≥’Ùˇ•à≈£˘ë†ﬁÌû›¨ÅÌû˜ÜìŸ—äÎÂíë¯ÜÑÔ°–≈û…Øéˇ¸≠Õ’É„ÛﬂΩ”€È⁄Ó–÷ôäÇäŒÒ™©•ÍæùÕ·Ñïâ·¿é…¯å
óè«àÑÏÔÇàÚƒ∂ô®ê»™µÖµ≤∏ùß·¯í•ñâ©àßÀ≈§ﬁŒ⁄’â‹ÔêΩ∂µ‰È“”æª™Ê◊ø§¸∫î€êÛŸÎ‚ˆÖÀÃô£Ù†…Ö’‹Æ
Å¢ ∂ÆÁ‚Úπ‘å≈…∆¥¸Å¬®††‹¿«çÄêëçÀÖ‘˚≠‡Êù≤⁄›ñ‡ô…ÛÏÅ•„äãæ©ﬂä⁄‚—≠Â©û˛§ÁÕ–ˇâ‚øï˝¶ŸÀ¢Öá
⁄ˇ—‹ñÌÂÕ·è©ﬁÃÙ‚ºÊô„ÙòáÏ†ﬁ≠É¯¿ÍÀúπ±§ÿÀ≤ì–ÄΩ‘ã—≠¿‘“è—¸‘Û±≈Ë√§õëÃî’Äê⁄…ö™òΩò‰¬˚ù‡∫
Äªã◊Ã‹∂à……¢ÖãêÃúÀ’Ä¨íçë≠ŸãËÕ¬Äµ¢†Î ƒ’¢Í≤éŸì»öÃ∂‘á≤•ë¥¿ßŸ≠ìí¨î‰•Åª≤ì“‹îÍ—˚∫ü≤¸¥Ó¶
ñ‰ì‚‘í”àíŒ∆ŸôÁî‹Ä–Ò…ßµ£Â—ÈôÕ…ã“ ˜∞äÆ◊ıΩ≤¢ÌÅ«∞’¬Ãïïà‹ÃËóúæ≥œ∂ìÂ‚“≤æ˚√Æı˜¨˚ı∑‹Íﬂ È
ıø∑≥ø⁄≠‰¸» èƒîœ‡§⁄≥°≥±Öﬂ≥ÊÑÁÌÓ«ñ›Û√õ˘°≈ˇ∞ÿÔÆµ€À¥æó„§ò†≈§Œ≥¡àÀç˙ÈŒ˚æàﬁÙõ‡“œ∑ñÑ”Õ»
˛Ñ‚è◊Ω—öåª⁄±˙”£úòÍﬁœ∂ÅóÏ†®Ö…ÊƒëëˆÑÜÁñ–¸¢›˜«‘˛Éóö§—áêÜ´÷Ÿç©Åëõ‹‰á“—·‰’“¥’Ñ∞åÀ¥≠ãË
Ç¬éÑ°¿«òÉÚ¢øàÏëÇÅ˛âˆâ±øõ±‚˜ˆºà®¿æƒºô§ÚÑûÉõâ¿ôâêåßºˇ´ù¨∑Ã∞ùà·ÑŒÿÕ˜˚ìÓ¯ı∏•≤≥Ÿº⁄ŸΩ
åﬂø∫€ºÅ¢àÄ∫¯µÛâ¯∂ôÒ∏ÛÒ∞˝ÍÒø˝˛ÖÑ¥á‡áΩØ¿ êõ€£—∑¥ˇ˘é—ËÎÂ°Ìö€µã˚˝Êõ˘ä≥•ÌçˇÑæŸñÈäƒ
§∂µâ∑ç‡≈èÖ£õµ“Æã¬ª˚†¨òß´Ïÿ«©œ˚÷˛ˇˆÍ≈’Â≈ÌÎããóØ‡ıí˙Ë∫ºÍ÷˜ÒÁ¸ÒõÜú—á¸—π‹ªçÉÛŸ˛‰’’å˙µ
¡≤»ê¿ê›ÉÔûˆ¨˘Ù∏∂ç±Ö≤Éüå§í…´ñ´ÖÃ∂Í√í§ä¶„ÒÉàÿ¬ê¯Ç∫’Ç¬„—±÷§ˆƒ¢µÙ≤ï√°πÇ°î±ÄôŒ–µàö•
œÃéäÿ˜€‰ùêŒœ∏ÏúËóÇœÆâô’æÑ≥Á””∫ΩÏâ•∫µ¬´ßÕ≤ÿ˙æ·‰„˙àæÓî‚÷ëÒ€ ®€¡Ãí™àïê©ó–ä®˝îŒç¥õ∞˘
Öë Á§∂Øƒıé‡πØç¥æó⁄Åã˙¶ƒΩÁ‹£∂©—Áî˛ÖﬂÛ˝Ú◊ˆ˝Á∑Œ˙ë≥û‚ÒÊ≤£Ø±Ú»≤àé¢ùÁç†Â⁄˘∆Àœ∏ Ââ◊óπÌô
˛—ü—⁄èå˘Öåµñ¯£¶⁄∆Ò…äã˙Ã≤ƒíÄËë∏€Åã¬€Â’Ñàù£íû–åÃ‘—–—«§£°⁄®ç·¶ÕÄÄòﬁ∏“˜ÒÕ¡¡ﬁûıìíÉ…Æ†
â…€…—¶Ù∞À˛€©ÕÕπ›ÏÃ∞»ˆ«˘…Ÿ¨º›©åÊ•ÀÒ¨∏ôï«≥Ò‚÷òÃÚ£Ù˘˛≤“òØî∆Á≥ ¯ÿé˝©¨Ì“ñ…ŸñÁÂ€îÎëÆÉ
˝á¡™€…ˆªŸ˘ﬁ∑◊È‰√‡†ìÑÈø§¬∂Œ™”ÀáÉÙ¢ÁòçáŸ«À§ôÜ°È…∏¶ûö†¢íàà¥ÿ‚´‰◊ŒÙÌ®Ù‡—é≈⁄∂ÑÓ∫òÅßË∞
Å›¬‘É—Ù‘Ö“–’Äü‚†ÈÖ…¶¯èâ±Ω Úõ˚ÆÇ∞§ª£ÄÖô⁄œ´Ωü‡˚ß∑ªê∫˝ªÏŸ·⁄◊˜¬’Òî”¯ëıìú¸Û≤¡¬…¢ßä—ø
îèÓ—ôäë¯î†àöó⁄ë≠‰ûÑäÖ“ãìÜóΩ¢ÿ¯ÒâÖ÷ÏîµÕ≥Ù—√¥Ãã∂ÖÅµΩÖÅù¢Ó—“Ú‘üƒÅÿ™ÊÓÎ∆”ß©≈˚≈•Ö∞Á†Î
πﬂÔ„Ç›•ª‹Ëíì˝Ñá«ü‡˘—ïØÿÆÍ é∫ÕôÆÚ‘ÉÊá™Âõˆﬁ◊ÈåÉ˛¥ú∑ﬁÎœ∂Ü´ˇª£ç£ÈˆŸ⁄¸ÔÚıÊ˝Â¯Ó„Ñ∂¢⁄
Ü∫ã§Ä˚—Ωˇµ£˙Ã√Ó≈ûÉºƒ¥ææö˛Ü°≈Ùå˙–‹®œå¸∫◊⁄∂ü¥Œ◊≈´≠Ä„Î†çÏ∆Â±Îäœõä‰ÿ•¶ÂË¢«∑ÎÅ•“¶…óÒâ
ΩúÔ∑à°—‹⁄õ∫áªÎó◊Ω◊ó∑˜ÁœﬂîØ‹‘˘Í—˚œ‰Ûëì®˛◊…€É´êﬂÄË†Æá”ıøØ‘¨◊“ÿ¨’Â¢¥¶Ñíƒ»¸◊¡™Åı»¸
ëÆ¢ÎÙãÆÊ‘ÛŸä ·∏Å∂¢ãæ◊üªﬂ∞•»§ëïÚ∆ë≤∏⁄†∞ ™ßŸ»¿Ñ°î»¡ÿå´íÑúœ”ã®Ÿ∆ùÍ“«Ø·ùÜπË•¡Äé∆ÌÍÇí
“ÚõÇáÉñ’îÎ“ù∫·◊Ò•∑å◊Å≠∫ÂÒ¡Ú‰Âƒ´†à˛ïÉöÿ∏¢â˙Í†ïêŒ£ˆÂµ“¶úå»ëÀúÙÑÀñËìÆ≈êãˆ⁄ÇœÇ¯Úƒô∏
◊Ç·˛íƒˇÍèç”ó£´»‡˚ÑèŒÄÌò‰Å∆•áëÃê´◊ñ‡àÁ∆¿∏êêÃêüñƒÁ™Ÿ∞‚Çëó≠Äƒ∂ë—á√‚ä¬±êÕƒıìÊ≥¨§Ó„öç
à®ÊúÏ≠ËÅ≤å®Åå§‡âìÜÖÈñ¶ª…°€œ˙ù¢Ï”€·ÄäÜîø§≠ºÅÂˆ®≥√ÑÿÅÈÜ©ß»í˘ùÙÁêôŒ¬Ú”Ÿ·≤©åñ∆π˛®€üÊ
˙˙ÉÏ∑üÎÙ±≥¡ØÉû–õÆÂá£¸ËòüµˆÛΩ∞€Ò≤·ÅÉ»áˆ⁄∑Ê»œ¬ΩßŸÒÈ…·∑€è≤°ˇóÒºîÎßΩæ†Ì˙î◊°∫…£Ù∏ÃÑΩ
öàÂ˙∏„•Ω ü˜É¬çâÛ√Î∫Ó€§ÔÎß¬∞Âáî„É¬µ˛ˆ¿⁄Å¨·áÑ„ì∂á…˜Ó¿˘·™Ë´§‹ê∑ÉÃå∫–›≈¿ØˇΩ»ﬂƒ∞ÏÉ‘”
˘÷¨€∑¨Ëﬁ“°ÀÙ†‡¿∑ÇÕÕ≤‘√á√Î∫§™Ç¨√Üƒüêûóó”™∫ÄÃ¡≥ëƒ†ñ¥˘ë∏¢ÇÖ€à ¥Ø≥ºÇä∫®Ë´Á©™ÚŸêÄÖ
¢†ƒ®æ¸”¿∆äˆê“ÙõÑÃàù÷÷ÃÀ¯±¨˜¡óûÉ‰˛‘©Øå‡Ç‚à∞¡¡ƒ˘¬ÃÚ‡ç«»ì∞≈ˆØ¥√§à√ôÖˆ–‡ı±œ≠µ√ÀÔ˘–ë
√ì†øãÜ´¡™ËΩª€ô¨ùìå∑ŒÊªó´”˚◊Î«∑—¶€Ä©·˜ÚÂƒ´´›≈∏Äæ∏òˇÅß©◊÷∆öÉØ„∂˚Êı∆ÏïÁˆ·´®ÆúåË≥’ÈÚ
„À‹ú™Éá◊≥˘ñ¶ÚñÆ˙⁄∞Íÿ™»Û≠¢ÍΩ¿â’ Ë·•¬∂µıËÕçâˇ∏∑˘Ò˝˚˛˚ˇˆ¸ı›Ÿ”Ô◊Öî¢˘äÿ˜ÔéÔﬁñû›æπº˘˝
‚˝ÍóËÎ‰¶Ó„˙∏ﬂÇ≈ˇßÌÊ£«ƒ—ÈÚ ÁÀ˝ì∑Ó€ÇÔà¶¢∆âú–Ó¥´≈Í‚ñ§”¯íÇÊ◊ñÄ†‡Î´ÇàÑƒëä≈“Óòö≈±ºÅï
ô®‰≠ç∑ÚÑÑ◊†•≤•ïâ£©Ä…˘ƒ…£‘™ú’ë‡àªìåπïº®ê·ŒïÔ‘ÓΩŸÜÖ±¢ŒÙöÄÇ≤¨ì·ùèÍä∏–ÅêÇ‘√‚ôÒ´Ë–≈¬
¬öÊÅ«‘–´âÑ⁄Âê«¢¡Ì‘©ßïˆô¨ß√ÁÅ£¿é‡ÛŸËû◊‡àœãÇ‡æ≥êÑã∂ÑÒŸëÃ˜≠üèˇ◊ˆÁ∑—¿≈‰ç—å¬Êè∆¸ÑêÑ‚
Í¶Ù†©˙àÍ±∆µ∑Åˆ≤õ”ìùò∏–˝Ñ·¬±ë†ÇÄ®–É¿∑—œ£ñ¸üº‹ã∆ßÄ∑¬≠¨‡âÑÒúÓ¶µ∞ù’‘ƒî·©œ∑Î„›Åµò
ÆÊ˙”‘‡öûóŒ≤Ω…∂≥Çä˘Åä∫‘ÄÜ±€∞ïˆ“›˛º˙∏ å«ŸŒÍ»Ç¨‡¯∏ÂÅ‡¶‹€¡Ååïã¥úËçûòı∑ù“Û…Æü˛û¯πÆõÛ≠
˜å£ßúç¥îÖ§Æ≈Ìïç≠ﬁ™∞“ÆôÜüàÂ∫©ÀÕÎ¶ÍÓÄ¸—œ˘ùüî„Å¡üÄˆåÇƒÔ‡·óÒ˜˚∏ÑÌÇëºÄ◊¥∏°â·éıÉµÌ¬Ú
ı¿∑≈ÌÀÜÿî†ÄñÈÂ∫ΩΩÏØüÈˆ”ôùæôÙ…ê∞ﬁ†øÁã»ã¸’¢≥†®Çÿ¿äæıáÑ≠Çπû’Å«≤Ã íâ˚ÙΩâá˚Ãåû·ƒà¡‚
µÄ¶¢®Ò‘Ωßê˘‹√—∆ÁêÊ¡ß´º§ìää¶≈˝ÑÅ∑™ÆÈÜúÖÜ¯â¬∑·ƒ¥ÙÕŸéíˆ’ê≤öÊÉê°ô¿ç≈—çÆ¶Ï∏®¯€∞ƒÿÓ‚¶
¯÷ÿáæ€Å•™∂¿Ò‰àËÖ˘ÎÀó¬≈áûôøË«®ªãª∑¯¨Ê…¶Øπì°ÌıúøÎ˘ˆ›√Öï„≠Œ˝ƒõ¨Ü”Ì˛´⁄¡ËÛ‰∂Û˘±˙Ä”ƒÊÈ
Œ˝‘ØÂÙÈÔÈ’ˆ˘„‘€©‚ÊÌÔË∂≤∆ôÇœ≥õ√´ƒ≤ı ÎﬁÈß‹€ªò∞ßÎ «Ÿ¶µ±‘áÆˆËàå°ãé°˛áûÎªé©ä‚í€‡ÍŸ≠ù
∑ı∏Ï–Œ≠Ö≥‹ˆ¨º‹ﬂæ€ªæΩΩπøπ®˝˘úÈÄˆ∫Èº˝ç€ùﬁ¿˛Ê¨∞À∆≈„∆Ó‘ÜÔì‰úˇ£õµÉÎ¡ ◊éÈ¯¨›™¶∆ê∂ö∂π®à
∞–ÌÎÏç©ñ„ÀŸÙæÖõ¡¥—ò≤Ëâ∏Ä‰¯·—õ±Ö¡÷äˆÈ§ñ£íÿ≈äØßà∆ñ≥§Û§ﬂ§î û é∫ú»π‰π—≥—Ö≠’ëñΩÍúËÊ–´
ãè⁄Öƒ©¢Ó–âﬁæÚ´…Ø¡ÜáıºÏê›≤¶êÖéå‡◊Ë¡¿√ö⁄©≤·›∫ñ÷±Ä®æàÃëÑœ˜Æ¿÷®ëæ∞§≥ìÈä˘õœ´à¿êÃæùπÇ
Úà‚À¸¯ñ»îëœÌóøÛá∑∆ø∂øøô∫âÊò…ﬂ§ıÖ∂ì≈Ñ†«·ïÿ°ó—ÏÑÏ√§ıÛíïóÕò˚ú°∂„∫ÉòúÆÍ∫òÈ§Õ‹∑È˙®·¢Ü
´ÉÿÆÍ˙òó¬§ΩêÛÅ¡“°ÒÒª∫ß∑úÜÃ◊¡◊∆ÿÆéÆ™®·¿ÒÙ¶Ü©Œ∞ç∆“˘ø«ÜΩî€•£Ï»¶¿êΩ˛º∂å“ÅüÍˆ⁄”Ωﬁ∫€…˘
ˆ‡Ù∂â◊•¿€˘ºÌ‚Æãœ—Í˝ø„π©Õƒ¢åã¨íåÅˇΩ´ª¥ÛÉ˙ΩÁàî–¿Ñ÷¶ µê¶ô„ÉÈÑ¨ÄΩ•ïÚÉóÙ‘≥¿ò—ˇ§óª§åá
ÍÄ≤Ä±Ç§ÄßË«∆¨è”Ωà‘˜–√ƒ¯æÉŒ–â…»Ú¨π“ÏΩ£πòêè’ß≤Ä†¿‰§à»«˚™ÕÁÄª¿±†ÕÄÒ§ê˛áÑ¸œóãÒ≥†¯»êú
·Ïè®√‘Ä¬ÈêûúÉ«≈«Ë™íÑ˚àÚ∏œ·¬‚øâí‹ºß¿ÓÅœöôàÑÅÏ¿à≤⁄ßÅÁÉ‡ÄÏØ†ÅÏ†Ë™·˙ôÆÎÖ°€ÈÈ¿˛ø∂ÉíÇ
åâ¥°Ë∞Õ¸¬ïÑÓ©ÜêÑÊùı¢Ö±«â€ä¿«éÄ≈‡çüú±Ëºòıæã˘∫∏⁄˝ÜÎÁã†ú¿±˝’®Î≠á˚í⁄‹†”Ñ¶◊±‹”éÊÙ‘ßÎ¯
‡√ã∂®‹Ò«∏õ¿ìΩ˘ﬁ‡•Ì¥˘ºŸæﬂùÔûœ˜ÆÊ˛˚óÛ˝´˘ﬁï˘¸˜‰æ˝∏Í›ﬂòç˚˝éì—±⁄√ª∂Û”´Î˛˘ß◊ﬂÙõÛá´Í¡ö
´Ω“”∫ñßÒ®çÕËõäÔö∂ä¿êòÖ≠âèçú…‰ÇæáûÑ±‚±õ„ˇ‹˜Î˝‡‰‚ˇçÙÍ˜Ùñ®‡ÎÁÙπ»àê„˛€»—Ì÷·Õˇ∆¡ı⁄˛’ 
´ÓÂÙ˚Ïº˘»≤ÿªÚá´‡¬øîÈ∑´ôå˘ØÂå∂‘’öôû‘◊⁄Âˆπ◊Ã”πí»‡»ã¡¿àà≤Í©êàÑ±ËèòÏı»Ñé®ê… äÔÊÒ®Ÿ
–∫©ñê†‰Äºö•à‘´‚ê˙ëŒ¥´πƒÖÕ§Ùú‚ñê«ªà⁄•òäÃÙ¢§ ó…†ñ˙‹ÑÖªÑÿ•©Ñ€¡†¶ÍÿØßëÑÂÖöÑ—ê»–ßºö·
Î◊…ÆùûŸ˘±‹àƒÇ¡Á¥˘∂œÄ¢´ÃÇÏ‘Èë∂€î‘†úˇ≠ÂÖﬂøÛ˝Ú◊ˆÁ∑µ◊ ∫‡Ñ‘…ÇÕ≈ƒπ°©â¡Ÿ¿îá∆É∏ãº⁄ Äÿ°õ
ÉÓ÷ŸœË¶Ü∏ã∆Ï∑ÈÅíºÃé–”«å‘∑·∫â€«Ä‚öúêıÇ‘ÒÖÅóíöàé¶Ì¶öò§‚¨Äø¥˘Ñ®√ôàéõ§åêÇÇ†˛ÖóÛΩ¬
‡ò—çƒ¥ìµûœÅ∏‡π∞ãí¯‡»ı¿‰ò»öñ¯Ç¿ê›¿î∏ÔëÅº‚¢î≤ùíåˆÜ†ûÿñâ“√œ«ŒˆÃÏœ…•ö¥Ÿ™ß⁄ıÈÑÓ°Ìˆú¶
Õ„¨óêµÃãÜäÜ®ñÜ˛Ü¡’…™ã¨Ãµ¡Äó“ﬁà»ö“ûºƒÜ±íöñëÁÿÿ≥ñÎÓ∑‚µ “±¯…˜⁄„âñííÍÅÙÅÙ«Çü‡÷≥–…¡£Ö
éè®¢√´È—ÕË»⁄ﬁ˜≠÷åßºŸÅá£øï»ÖãÜ≤Ä’ÅÜÌ‰ı’∞¥≤ÅÏ’ô∂¸Äâœ—§ÿ“÷áÉ„Ω¢Ç«¨öΩÀÆ‡·¨ƒÁ∫¡¿¶≥∞œ¨
æ´ º§àäöÈ©Å∂úÖÿé¬âÏ∞öˆ¶„ÇÓøá∆ˆêä°‡ùÕ¿ì–ëã˙¢ã≠ôâÇªˆ¢ÖéàÏÄ¶ª“ ã«ÿ∞‘ùƒùÈ·Õ≠√ùØºı≥è£
ÜÃ±®Üéú·Ï√¡¯∫É§‡ÈÀñÑáˆ‡»ÿÛ¿¨à¡î¯–·á€éÍı®ÙˇËÑ„´ª©≥˚´Ó‡ÕüﬁÌÑìÕŒÏª©øíµ¥›€ü˘¨ùå◊ˆÜ´
ﬂ€√ïı·Ù“ÖÔá›¿ã≠˙¡Ë¥ÊﬁÌ£®ÉÏÙöª£Ó˜ç¶ÓœÌ…«é–å⁄·™¿à∫ã–≤î÷÷ÿ√ÄÏÍÏ’ÈóÏ¯∂üä˘ëúüüªÃ⁄–›‹€
õŸﬁøﬁ‹ª›˘ﬂª€›ªÑÕ§˜œ·ø‹Öº—◊‹ŸÊ…‰ı„¯¥ˆ‹õÔ”á”›Ô¥Â¯Ï£…—ô÷£ûÎ«Ö˙∆‡ä°ú“ÛÑä‘¨¥Éãôº≈‰âº
°ç„–êêÿ∂¶∑≤√”ÔƒØ§≠Ù”ã«‡ËÀâƒÍ©‰´õ´‰¿óÃß≈¸í§àÇÜÌ–  ÇÄ©‰¨¢¶ô„ ∂°Áë¯ßΩ®àöÖÿ„∞óæ“¿¿á†
∫–ÿÂ‡ãÑ÷èàÎÖ∏’ Ä‡ŸÑ¿êÕº◊»¶Ü¥’Î”ù†¶âúë∂»æÔìûàπ’≠‘ßìã∂ÿêü⁄∑Ò¬ŸÍñ˚¨≤à˛ÎÁﬁœü˚ñøˇãﬁø∂
ÓøøÅ¡‹∑Ä∆ö‡úÇàéö·ãÇ‘áéß™¨âÀπ¬¡œ¬ÑçÉ’ÙÿÂà£å˘†æÿ∞ñúáÍ˘®È•Äóñó∞Éƒé—ê≥¿åü§åƒâ’——¬∏â„
∂§åñ°¨ä…Ö‡€¿ôäƒÖÿòâÕèõÜ‚—⁄…›œà“å·„‚úüó‰Ñà·¡Òòñ‚∆Ù≥ó–”‡˘–ﬁë∏Á·¿é∂∫À ∫™òòéäæ◊Œö¿úÓ
ﬂ Û”Åõé¥Î¥ íı∫¬õò†‡ºíÄàˆö‚∏¯‡ìñõçÎé¶∫®Ôâ∆ÿ˝ô≥ìä¿àƒæÜåÂ˘£ò€Àöû¯∆‚â”∆çª°⁄¿±≈≈¯∂—Ø
âˇÿÆ˝–¨º©´∑„åÄ´‘¯©˙“’ìäÇåà“úé÷Ωàä“Çïó¯˘ôîËé˛ˆÊ˜›ñáÒ≠†‰»çÜœÇê¯›ëÃö’¥ı…‘§›¯ï·ÔŸ®ü¨
è° ñÆÄ«®óãŒ„—éê°îÇÙƒ”éêŸò‡á¡Ç≠ÊÂÄÂÒŸ…µ¥≠è¬ÛÌ·†‹›ƒôÓÆ«Â™˙ÎÅÎŸñÉ◊«¨≈Ω üÑ°ƒØÇÄéÄÌ§‡
ë∑¡—Û≈ß«êƒçá›∏¸˙∫—Öâ∂¶ÌÑ√˝ÓÁ˙¡∑ÍÃ‘ãÙ·˝ñÑ∑Ê¯öçΩ†ÿ≈û‚˘†ÕÄ‰ÓßÇàßé∂äù””úæÍé¥¿ÔËÑ£≥ú€
ø›â”õπÉï∂Ô∏ÍèÁ¡‡ÑíéÜ·Øå√Æ´Ùî–ÊáÓ‚ÎÓ∑∂ªµÌ»˛ÛÎ»ÔŒ¥èµâ›˜≈«Ì¢…≠Ç¿· ∫ú◊µÑö¡◊çïÆÏ∞≈èØÙ
ì≈¬»©ÃúÊÛﬁµ◊Áì’çè›˘˙ﬁÙÌˇ—¥˙∞⁄›ü≠˚Ïè◊ˆ∆Û›ºØå¶ª›…Œä∏⁄Ò˙õ∆ÁŒ’ÒßÇëÇ‰Ã∑Î≠¬ﬂ⁄ÄÇÄÁ·Âì
Ü≠ÚµÏ≠«Ââ…‰Ü∂¡ƒ£¿¡ƒÍ»‚ã®âﬂÉóÕ·»ö†√Ú∏“¶—˙∂ä•≥òó√Ñ™†àÇ¶ñì⁄Ïâ∑ Ñﬁ∆ï†º¶Êì¨µ¨¬Â‚™îÑÜÇ
õÉöÀàÈòœ˛≠ÀôòñÉ∑æ≠ÈÖÑöèÀõãí˝≈ÑŸ∆≈ºö÷ÈëÉÜ∑öôëõÓ«˝éÄåπÃ™úıﬂ∂À„≠‰…ò≈Ÿÿ‹ÔäÔ÷˘ÂÄ†∏íœ
‰˝ïƒóÑæºı‡˚æﬂ˛⁄˛≠¸¶™àÄÜ–ÿôÜá’˘ä≠ÇÜœ∑Å‡ÙÜöéÁ†¡»öôàíâ…Ñ˜Æ¯…ˆ‹≤·‹Ñ‹Ç·Ï∂‡ƒ»Ö£°ËâŒâ
À∑Ö¯ùÕ¥¯†ƒ”ƒËïÂ¥Ñ‘¥†Î‘Ω°à®Ò—â…Ä©á¶™àÖ«¯Ãë—íÌ¶÷‰‚ÄÌë‰ñÿ§ê‹¯¡‘‡é´Êà¬Âò†‘ÄƒÄÇãáÇµ
†ÍÇ†ÍààÜóää…í¯ÈÒŸê©ÃÉîÂúÇñ–∂¿ƒÿÖËñ≠á…≈∑í«¶Ö´∞«‡Ü∑á¡Â˛⁄≤‡—¬…ˆπ˙∆‡¯∑ı∫¯‚¢ô®ì‡ñ∞§»Õ
Õ‡´–â∆„πË“€Ç¥π’·‡˝Ÿ ¥€Æ™˝ûˆ∑‹¬ˆ·é≤ú‘Ç◊˝ﬂê√˘ÆÇ‡ë£ÎÃ‡ì‡€¸¿—ë‡ç˙≤ÊàÅ˝±’…÷¡Åé‰òô‰ÿ
ƒúÏµ≈Üé∆Ä¶ÄçÊ•ÿ‡Ó÷÷§ïﬁö‰§´™¨√ëÄÂ‘Ä·‡¸ÄΩﬂ»·ƒ¥˛ï«˚≥ÅîÊ≥¡∆Ò˙¯æüå«ΩîﬁøÇ◊Ú¡¯ÿÿà‚èéê
Åøà¬ÛóçáñµÎ…ëÆóƒîµ∂û∫òØÃﬂ—Î¬ÉÖ†æ§çëÜÉÛÑÊ˛«ùÕüöõ˚Ëö§–ù¿Å◊˚†¬ØöÄ¶∞Ó˚™€◊ΩÿÒÅ‹å«÷ß≤≈
™˜Ïˆáﬁ∑èØÔÖŒ‡Î°†´ç˝Óû“õÀ›ÊÊüﬁüàò´√˝‹∑˝À√–≈∫◊œÛı«åÿ ´Ã…Á∏™Üﬂ±Ñä£ÒñªÚ˘†ñßƒ˙È≈„◊˙ˆ
Ò Ù¥ﬂ€û¨ƒÏèÁª›®ÇÎÌÓ¯´≥⁄Ë⁄å◊ªÉµ∞∫öÙ◊‹Ó‹ä≤Á∫ø’‹ëËÃè¯≥À™∂√¨âÃ´Œ‘“‚éΩ´∂Ú¶”–ùöπ¶úÆÊ…
¯®à¢ô∆øä´Äå˙ö–ﬂºÄ¬Ω›Ó¬®¥”ëä÷√îà§Ü∆°Ìåêƒ“ä¨‡ÍâÿÙ›‹á®≈§ò√πíÙºÕÆÛÈ∞«˛√ÂÔ⁄ûë∏≥Ò°µÃìÀ
®“˙ŸÕ∆ùˆëÛÒµóóÛ±ÖÄ⁄à√«‘–ÉøÅ˜ÈÍÑÑöëıÁÔ∂”˝–∫µìÛõ˙ÒØ¿≠ÃçÚ®ñ¸’Ò—´Ω¿Û‰≈Åä†Í⁄˛—•ÔªÏ›û¡
πïó¸œ˜Àﬂß€üﬂ⁄‹åñ¿‚ã¡®∂¢€åâÿØœ∆¥É±ÄÍ∂öùÛß»§£¶ÍêŒã…±ÄΩ∆òÀåèÒå≥ñÑÁôß¿ËÀÕÈπç°ª≈§ö‹¨Ã
°«‡…¶ﬁêñìåñê¿óá¿ÒÄ»«Õ¸‡≤Ø¨ã‚åÁµÅ¶¿ÿö¥Ω˛ú§òˇõΩ˜Ã‡Ú‹ íµÁÇ˜çÈ…’…É·Ô¡ãÇÑ·Ωª∑ÙÈ …µ⁄
ã›ÍÓΩ˘≥Ôè˜áÓûãÊ…‡ΩΩØ∂î±±í’≥µııÖÑÆ≤ôô¨¬Ò˛Ü∏◊úÉˆ–ˆñ„ÕÖóì·…∏˙ôÓÜòÓŒÊêﬁƒà¶†ûÕ∞†¸å‚Ã
÷åß¶ø±ºàº∏à«≈Õ›ò≥Ç†àîê—˛Ë ªâπı±ç∏ΩµÇ‘•˚âòïÄëÈ˙‡∫•à˝®ê‚ñË±‚Â”∑∆ÏÄΩ˝®ËˆÀñ©»öí¬°Êœï
»µï≈ÑÎ„Ü™æ›Í¯∆õÓ¿ˆ„´¿˚Ωº¬°Ó»ª™Ì®’¢„∫ìø«Àﬂ´ƒ˛»µ˚¶˜ÈË™° ÊÉÖÆ‚≤Ïè–ïÒ–ÉÍÛá‡ﬁü≈Ü—‡•
Ä»ê·ûá…àûùøÔ£◊¿®≈Å‰Åﬁô¯⁄îÿ°é√¨ãâãú•…»“¥˘îÑ∆Ç»ÒÊ√¿Ö»É˘„ÏôøÜÄ†Î˙˛ÜºÍ’ÂÈ∑È¸¯∫øø
”ˆáé∫˚∏À≠ÜöÂ√‰ƒ∞É ÑÄ¡§∑¡Åî∑¡¿êëÊ≥ıÕë˘Ÿî»íìÕÌ ‚∂Ûõ›ˇ¬ﬂ¥êƒ˝¥Ì’¥‡àº¢˙÷Ñú—ë¬≤¯≤¡Îú≤ö
È≠Ò¬√Õ’Û®”ÿÓÔ°ƒÙ‘ùûÖ—ûÛÕÓ‹ÙóÚèúÔ´˝©Ü®•É˛ê¶Ëõ¯“˘µè˚˚≠ÿ·ËÏΩù˝Î±›˜û˘ëÿÅ≠òÎÈ÷‹Ûé¶˘ª
ƒ≈ÏØ≥‰á• âî„¨‹”ÂñıÆºÈù∑ßÅà†¿êﬂﬂﬂ¯˝Û¸¸∂ªˇªÆΩ¸”úﬂµøß◊’·©ÏüÉãÓæÔÆÒı‰ÇÁπŸÔßùìï¥Ê‹õ¡Í
˜⁄éµ”≠ç‰ü™≤ß…¡÷ˆﬁ∂¢æ‡áÚÍØ¥ƒ“‡àû∆Æœ¸ç†êö™–ëƒñ¿ã›¿πêÑπ›åÇ˝Äà¬ô—¡íä…◊‹≤ÊáªÔ–ﬁå¥≈‹‰
Åß€ÊÏÚì◊ùúÜ√»íØŒ§‹˛‰Ú˜Å∞√é≈…¶œÔõ‰††ààÔÃß±—Ç´î™›Ú∞«ÀºŸ“—îÀèÜÄ¶¯ùŒÁæÀñˇÍ‡Øﬁ‚µèóâ∑˜
âúõ®ˇÜ¯°Œó°˜ÃÄ∏ ´ﬂŸ˘∂™‡À˛ÅÎ∏ˇÓøªΩ˝ﬂü€ˇ∑±Ì√ﬁƒ”ÚÄÒ€¡√≈–Âáã¡‡á◊Ûù∑µÙ˝¬ûÑŸ»ú¢¡¯»êø
Ù∂üŸÅûÉ‡‹†À„™Ã∫∏æ…ØÚ¢»”¨ããí§à‚óú¢ëÄ†¢êÑó™è«à˘Ç¬Ñ˚Ë—è«˛˛·ÛÙ¯Ù˘ıÍ˙œÊÌ√˘ÚÏ∫Ü¶…ÛﬂÖëø
µœƒÄ†ÜÇ±°Ö∑∏ÇÑÅú”ë‘·⁄ﬂäóéèö∏Ì˝ò∂î«åÂ»€·¶úÇÑ—ıÌÌé∏õú—÷œ°ƒ‘±Ï£√ï§∫≠ÆÍª≤ÌÛÜ∂¥—•ıáˆ˝
Ñ‘ËŒ∫ƒú‚Í§à√÷ú≥ˆµîÉ»ëÁ≤øêøáë®£«ç˛Æ∏Ï„«ÉˇÂˆ‰øæπ˜áØ∏œ™∫è√èÀŸ≠∂ã ﬂí≤÷∆ı˘Ñ∑Çî™ÿÏ„
˜ıí©œÖ˙Æ∆÷øﬁ˚ÔØÂﬂõ«À„õ˜ó˜ßØ˜èﬂ‘◊æÔÀ”ª‚¯∫ÈﬂøÂÌµ©œò—˘å≤Œ…ö¿≥™ıø√ÑôòûÊ®±ïÌ€„ÜÊË≥€ïÌ
πç‰©¢æ‰∑Ë≥Ó©≤≥ûë∞∏∆‰ôÅ°Å√˛Ÿî§Çâ‚ò◊ª†Ü¥õ¢†íÑ∂àÇ¬áà¶⁄Òπ™‰äîáÒÑ∑ß¨ëÄ∑¯ÃÁßöΩ—Ü†‚»≤¥ı
ÀƒâßåΩ˛‰µÒ˜°∞ä≈Ò†¶Õá«‡Èú∞¬ååÑ·À”“œû¶´≤‘ŸÖﬁêè†àÀÇÿÕº¯Úª¯Ò∂√π˛ÀØæ´˚◊ß‰ô˙Û¿Ä≈ŸﬁÉ˜ëå
÷“àºÂ‚Ø„åÁ÷ë∑äøã˚‚ø¸˚ÿ˝Ëäß€æ‰≈¥¨ñü∞ê†ûÉàΩÔ∂ÒÏ∏∑ôÿáàü≤¯∂ƒŸÙºœÌÅœ–∏§É⁄ÊÍ˚é“Ù¸˘ñ÷’“
Â∑Ÿ€‰ıÖ“íÚß˘î•°¸Öèó˘˛ºåÀò•êÈ†áûÿ‰¢çÛ™Ì∫ÚÙÙ∆é¯ı˙˙≠ı∑ØÆèÁÓØﬂ¶œÇ»˛Œ¿ê¡⁄†í‘ìë⁄œ˛Òƒê 
Ò§ê˜åö»ÎŸ¨¿ª¨À€ÇÈ◊ﬂµ—éÈ˙ß⁄˜°Ñ⁄◊‘¸èﬂÍ˚∂ﬂ∑¥ÌèöË™∂©É€¶Ë€†Î∫ÿ¯ˇ…›«’üØ‘˚¥ŸÁÕæËÔè¿—ùãÓ
‰¥à‚“üŒÌø·–Ìª˝ææ∂üÚ˚„ÚÍîÓº¢≤À…€°‚‘¥¡‡€ªïì∂†¡êÊ∆·Ù˘Ë°¡‘·∑Êê™èÂˆÕÉ≥˚˛Ô¸ˇâùø˘®ﬁæ∫ø
°æ˛˝˙¯›ﬁÆﬁßá◊Ò˜òˆµ◊∞∫˘≈ﬁ˘Õ⁄ß©Õ∞–π˝‘ı¢µÂ´ø∂Ø∫¡÷ãˆÌûÔ⁄¶Ì®Óÿˆ·Ó‰› ‘£Êº¢Û´â›—Ä†Ñ÷ÊŸâ
îÁ≈éïûµ¡Ÿ˝Áª‹í≤±·¯ÑÑÅëƒíç¡¥û∞Âàü•ÿ‘·´ÕÃ∑‹ºà¬ï…ùè“Ì¡éå Ïø¥Â∆É£∆é∞÷ˆÖëßåÇ·†Æí«âíƒ§
’üÕ—Æπ•ûÍÆÒÜ˘“‘¨ñß¡ú»◊Ã√µÂª√∫ÿ“ﬂŸëüÇ˘∑ŸªÍ«⁄˘˘†˛ä”ØØŸ≤èÁ…ëØ«¡¨éÊ …∏»¬˘öŒ†≥†⁄å¨ìÄ
áîÅ¶™¸˚Ü˛¸»œÀÁˆ€Á€«Ó«øŸˆ®°Ø¢®ÅÄàÜÇÑ‡˙‡áÄåÖêÆÎÑË°˜ìõ≈ﬁû¿—ò…„ªâ¨®¥≤ÉﬂΩ›Ûëû˛†áëë§÷
Ÿûª’Èïà∑ˆ®›¨íÃâÚ–Öáªæ’ˇ≤©¸∏˜£Ãâ‹”´…›ºå™∫¶ÓèÂ˘⁄Ω÷æùﬁ˝˝Ω¥øˆ˚¶À”·ô•”í°ƒ˝ŸÖîÇ˘≥Ë≠”
Æë¶∑ÂÃÂÕ“ê·†àà£‚ôˇê ‹ˆ–∑∫ˇ£â‰àÓ˘æπÏÌ…∫ÌÍá∏˘Ô˚∏÷Ì‡ﬂ∂®—”ŸÏ˘´Êﬂ◊˚¶Ñ£à®ÌÚƒõ√÷˜ﬂı≈∑á 
ÕÓ–ˆß°¸∏é¥œÇË¡ê«õ¯≥úÚÎ±ΩÆü≤˝æË∫∫çØ´àô‹Àµ¥îÅ∞¥ŸŸ≠ºÇÎù∫Î“◊Ÿ˙ÿ˘®Ù˜Ωﬂ˘áã˚„„˙ˇ˝´ÊÎ◊ß«
Ó◊è˚À˝±øﬂ¯Â…Õ˜Œ÷§Ÿ√»‡∆ÖëŸÊªø’º¯„ÑÑ´À”Ïœıì˝≠óòûã˙ˇ∞çç£Õçµ›§≠ÑµÃ¬ÏäÄÄ†ÊÇ˜ÛÑÑà‚ñ«å¡
Èƒ‡áÍÊáø¬óÑÃŒïñﬂïâ‰∞‡íï€‚Ï¯∆ìãÖè óå´ëªé€Ç≤ìçΩ⁄ëØπ’°”´ÕÉùô∆ê¡¯‚∏êö«ë∏é•Ò™◊çÛ˘îÏ–≠
Èâ‚Õƒã‹ìà±ã‚çôûÆâ©‘·¡ëê«óúúŸ”∏Ä¡€¸°÷˛∫◊øÑÈ÷◊‚ﬁ¶“‡íÀÈ¢Ââú˚•≥ºïÅ‚ô™ôê•≥∏í√ÚﬂÚ›Å¯êü
ˇóœÌ∑∑è›çè˛≤Ì√ﬁƒ◊¥’ÌΩŒÄ≤ô√ê¶ÑÅù«ê∂¯°œ¨¢Ö©Ã¢µ“ÿè¬ªÕ˜∑ûøÒÉ•≈èï†“¢ÿ–Õí≠Àı¨◊…Û¬Ø ›ﬁÏ
·≤¬œÛ‡óï¥ÊﬁŸ≈Ì¬ëãŒì‚óñÊﬁÖ⁄≈Âµ›¸º˚ï˜è˝õ∑◊ÅÀıèùıﬁ§∏Òíçãùí€Ê⁄◊Åú»–ÜÇà¥ÔÆÇ«åˆÒƒﬂâ‚Êà
Õ¨ËÚ≤¡˚®∞´¥∑ÒõûËÄ⁄‰á‘îÇÕ¿€è”ïùà¢”Í˙íı¸˝ﬁ’ıŸ∑œ˚√æ®øñáîÂ§Ï¸è†ã≤ÍÅ∫öƒ¥Âﬂï˝ﬂ˜ˆá™ø÷˚”
¢‡áÛ˜∏Ù»‚êÕ·ﬂüˆ’¥Òﬂ¡…◊£çÂﬁ≈”¿§‰ªÁ∫ΩùŸ€Ÿ÷”ÁŒÓÖÇ»úŒı·≠∫ËçÇﬂ¯ÊË¸Ùî˝ÔÙ¶¸·Ì˜ÌÃ€êƒµªù”
Ö∫≥„Íóî€ñø”ºÑ§ÄÏ¿ëëƒçêª¨ƒ¨◊±›ï˘⁄Ã≥º€¥ÙÎß≥ÿ⁄Ãﬂâõπ≠øêôπÄÄ†€∏πàÇ‡‰Å¢õÇà∆‡ÈﬂõÏÎ¯üÖ
∂±£¶™á€≤∑ï¨ò‹·™çÈﬂÍ¬©ÒÇä∆Ç‡ßöΩ÷À»ªÖ˛‚√Ωô‘ô§à˜ˆòõá“∏íê†íö˘îØ¶ÂäÔ¸∂‰•ê…í≤Á∏•‚éå˚
ÿÛÇúë´Ïª™ìè†àË˘Õ…æÜ¡¸ÖÑ—øÆÆ’¨Œ≈º¢óä¬¨ä©¨¢Ôà’©‘•ÿí¢§ºóìƒàñ£Œ˜Ÿ‡√˛úﬁæ∑ﬂﬁæˆéø˙…∂è˘ë
Ú›ﬂ«õﬁ–¶⁄ËœÚ–‡ë˙˝œ√Ÿ¯¬–àô¢›íÔÚ‘Ø™≥ΩÆÄÌëÓ¡í¯ùÑãøò«˚·ÃèÂøíï—±úÖ·¨ŒÙñÈ∂¨ ’¿îö®´Õ’ËÕ
âªÎ˚≥Ó˜’Ò≥≠ÍµØ‘¨Ω‹Ê≈Æ®ÏÓ˛üüØ’´◊«ﬂÛ•Åæ∫®‚ò¥ç—‹ê•È—ûé‰∑≤¢œÅä¢≠ä∞≈¥‰å∑Å¶†öà´≤Íó¿ëœÉ
–Õ’ÛÀ∏±úâ◊êµÄöéË°ÌÎÉå—√ÕÃçŸ◊ß›ü˜Ò˚Ëˇàèß¯¸¸…Æ˜Ÿ˘öﬁùœŸ˛îˆáΩ¥Ω‰Õ°®è˙’ç¢ã∂ﬁ˜˜ı˛–úéÌ·
·‘°¿ê›øØˇá…éÆÖˇ°ûá˚™ﬂóÌã˚äã¬€≥÷Æ≠πÙÕïÎÆ˝ÿÎùª´‹›∂œ¡Ú±ﬁ˝ˇÒ‹ˇ˛óˇ¸Ì˚ùßÔ˙Ôøææ≠Ω∫∂˚∫Ã≥
ﬂ’ˆ◊¶ªö≤∑ó¢ öñªÒ∂–»†∏Ó⁄ˆ∂÷ñ≠∏∂ÊπÍªﬂ¡ì»ˆ÷Û⁄≠≠’÷ñ¬ö Ê‰ûñ∏öÏÇá∞√¬‚ÁË¢øìˇ∑Ç∏ÌÖèÃﬂàÇ√
±ßÇæŸ–òÆ∆ê˛úÆ∑‹≠éÚ≠∆˜öﬂ©ÜÏ≤†Ωëâ¬´•∆´‚ÓÜ˝À•ê†àª§àÄ°ÒÎ®Õ¢˘ÚíÀ„†ôçä´Â¥÷Œ£öÉïŒ÷‰∞ß¨
ÃëúÅ€íÜƒ§´öÇà§à¯ùııæ˛°÷˛±Í∑ﬂ˜À˚∆‹ÓÎî£†Í¯«∫·õ÷Ë⁄æ÷È∏∫·—∆ßàñ¿ﬂÕä¶ –©ø¬€•æ‰ªÁÂÛ˚ÌÌ„
∑˜£üÏ˚êóíëç·É·¿ÄËÏΩﬂı¸≈ÛÜ÷ÄÔØÉˇôÇÖã§‹‰±∞öœ¢ŸÑå†Üïëò∆‚òä—¶™Ñ…¿åÁ‚„íâÈ°ÂÖ¸åÃπÀÀ˘ﬁ≠
ÅÛÍµúóŸµÎœ–Òô’Î¢∆—€É˘∏˚˜◊è…ÒíÙáØæÓ¨≤≤ ⁄ÆûÎ∑¨˝˜ÑÖ––Å·ﬁÖŸâ≈Ê¢–ŒÎ–ƒñÌÃ©¿Î·Ô¬ª∞¡ù˘ôñ
¡ı≥’ì∂›ÿóı–ˇ««”ÙÑº∆á„¯∏Ñü˚É◊Ô›ÓÔˇÉπ⁄ΩŸÌçˇÓ∞€üÃ„Ÿ‹∫ﬂù”≈πúüÌ˝…Óéˆ˚ÓÙ⁄£€úΩˆÓò¥ß∞ª≈›
Ç©ÌœŸˇ™†€‹ÿü≥˝±›òü„∏ˆ«Ï»ºÿµá™Ÿ◊‰Â◊§˘êÜ¶ºÎ©éπÜ£Á†Ô¢◊≠¬µÀµ¥À±˝ª∑˛Ïˇç˛Ú¶˝Î˛∑ıüøÌ˛Ù
˜˝ÒªÔé◊˚Û™Ó‚∑±¨»∂∞£ùëÍÄôõÔÏ‰Á§Üú—≠òûÓùÙÁÅÁ∆‘⁄—ÊÑßÅÊŸÎÀõÌÉ◊í≤óñíä¯úñ≤øúâ´ﬂô∏êÑøóƒ
È‡àˆ–Ñœê‰¢À…ú≥ÁÈÃ•œØ—ñπ⁄¡ù≥êæ¥™◊ÍÈ‹û∫ÒùÔ≤‘Æ‹±ÒúÙ≤Ñ‹¿≥‘£µÕÒ¬£ÔåñïÇ˚¡ê‡áâôÏ∞…Áì°ˆ
º¿ˆÄ°”Ë‹êƒæ¶¢ÖÈÀ Ñ£°˙Û¶∞âñƒ°≤ΩË©æÑÛ∑…‚ïõ¸˘√Ω˙›„ÕŒê¨î‹êæòá…≥Åú©£ÄÇ©ÀÜ•µá´üÀìßôù
≈ßî°øØü—€ÔÔüªü˝ﬁ‰€áºà˛¢úÄ¡¢Ñàıò˚≠ÇÓ˘≤Î∫äÆÉÆÕ˚¥éü∏¸ËÒ—≥¿áÅóÑúü•≤·¬∆©óîôà∏Á£åç”
öÜé—Œ∆πó“óœﬂ ‡ºÁ«„åè≤ÅŒíïÇœÅ »Í±ß´Â¢€¥áı˙ˇ÷Œ˜∆·¨À˜«Ì€ÎÕßØµΩﬁˆ®ËË“˚´Å∫´ÓÌè™µÀ∆ÆÍª
øØ≠º≤≤ ¬‚ßµ’åè”“»ÃÚ£Œ˜È±ÉÀ´›ŸõıÎ“¥ùÅø§øò≥˚£˘ºôß≥√Ùˇ‚úœª’√Ò€ü‘ÓøñÈ˜Îˆø¬úÇ÷˝Ò˝∏ÔŒ
¬óı¯—èÁµ‹øÓ∫„¶Ω™¡Ì—Îœ⁄áÔπ‚úÆÊ·‚–üÆÆ¯ÌÚ˚·∏øª˚≥≥ÔááÁ‡é˜”Á§ª•Ì°©â˙∏≠‹†¥ÏíÎ„ÿõœ÷É
»◊ÀÉΩÕ˜Áı“¯›€ﬂûºéüÓ√üﬁßˇø˝∫¸˘ªÓˇÔ¸·¯”è«∑ÜÔ ”•ÃõΩØÆ¥®‹Ñ•ÇõÑïí’õ¯†ÀÇÛÍù∂Í—èÈıﬁ®⁄£Õ
˚’¿€€€ôÆÉ»Í Ç©†µ∑Ã•ıÇ±ê¡¶ê¸≥ùòä„âﬁØÛ•…¯®≈Òåÿ‰Ωì‘◊π∞ùãªÖÔ Æ˘ËÕïõÆÈ€ÚáùªÌ¯õã¡ü∂®⁄‹
Îæ§¬ôÖ©Î…éµÍÓ«‹∂Ÿ‹»ù∆óÓ≈ÉÀÈÏ“¨¶°’äÔ∑ƒß“Ø›êó©÷ô¬≥°ìïïÃ¡ë‡êƒÃçÓ‰ÕÕ∞Ã°€ùˇı´‰ßˆ∂ôˇ
∞€¨ûÚ˘êâÇ≈ §âù«‘íúí¿íë…Ã≠•©ï°íΩ®»µÕﬁ¢úµ‡Õà¨ß≥è€¢›˜Û˚ÌÌ„˜õ£üÏ˚êó±ŸÃÂ† Â§ó‰”∏¶‚ò±æ
†œªéÓ€∏Â∆‚¶ÅŸÜÊõùò„·ûç˚ûÖÓÖÑüë· Ã•Öù’Ã÷”éÈÖíÒé€£Åâê¡é˘»ê·∆Ñâ·≈∏ÿõ·˛Çá©Å≈ˆõñî¸ó 
äÌ€Ûı¡ËœÑ“ˇû°ŒÛ‡¥ˇ⁄Óœ´√…µÅÆ˙“ˆßŒçâ„∞Ì˙≠∑Í¥≤Ö‘ÅÕ€’©¿›©Â£√‰’∫©÷…æ£Ã Â—≠´ÊèŸˆÀ˛ÄøÁ
î˝ºØœ¿ÖÿéÅÁÀÚ¸›ûóœ≥˝…Íé¬ª≥Áá∏Ê¿˜™«îä¯ñÅô¯Íˇî∫£ ˜ÍéÄÁÆÛ„œ˝‚Ù∏ºø¨˜œ˜»£ßÛˆ¬¯—é◊Ìˇ’
ﬁé˜ª„Ω˘∫Ú˜é◊–Ü◊ùﬂ¸∫˜“¬ä¢›ùÂ©∂≠˙ÓÊ∂Ùı≤±èïè˝‘ªﬂﬂ—µâÛøºÜøæ≈¸ˇ˘óˇÎÔ€øˇıß„øˇÙª˙√˜≈Î◊ﬁ
äˇﬁ¥«’˝‹”ñá˘˘ò’«åŸ˝úøÁ÷ü”‡ØÎı∏ÓŒüè˙Ò≤´Îµ‰é◊∂æ≥‘Ÿ˙÷ôÌñ≤≤Ñ›„Ï≥ö¡â»ã‚ÔÖÒû¯ÊñÆÒ ¬“∑ã
ÏÀ›∏⁄º–Ê©…÷Æ∂‚≥≈®•ƒ——´ìˇ´∞¿ë‹·À∂¯õ∫€ê°åÉ‡£åÊ‰¡¯¿•°ôÍ¿ú¶û…‰Ò∂ü‘´¨ß€ê«≠Ûÿ≤û®ôÜ∆÷™€
∑†™ªÃº§º˙êÑ∞—Ÿóéê¥…÷ƒΩÂœÂé¯Ô…œ°¸∫¶ëØ±–∆‹¬ñ—·¿Üû·ÇªòÁœÉóèà˝¢äˇ∆ìÖﬁ›‹íòï®ﬂœÊá¸º¸Óø
µΩ˝Ï˛ÙìÌÇüÚ¢˙©áÑ∞ôóÊ´œ≈¯©ìƒîÎ„ñ‹≥±≠…ŒúÎäªÑ·åÕ„‚úŸìÀô⁄ÆÊ¬áòóì¡∆∆•ÖƒÖ⁄ΩÓßßòó‡Óåπ
äÉÓ˚ÏÍÜÓì√õ¿ëŒ›ñËÍß¯êèŸŸ¿Æç™Ùë‰˜ÏŸÛ’œ◊í˚°π»Ì¡ÓÓèÛÓ∞Ë˜À∆Ê†Ó◊’∑–—¬ëûÇãÊèúﬁÄ‘›ºËÊ
∏Ÿ©•Öòï‚¬—ã’£∂ΩÙÁˇ“˛§üœÚùÒ¢üûëŒ◊€ƒÒ¢Ïœ®â®Ó˜‰Äˆ†ªˇ±˝âÉÁ—ó¸Œú–¡ÒÃıÿóÓˇÂéè¬ÂÈ—ºº…ó«
”ŸÂ·˛æﬂÄ‹ËŒ˜À„É‚˜Ù®úü’À£‡˛˛Ä”ò˚ã•—ûçÇÇÅÏ≠ÎÖöÔÔ√‡Ëó«Êùπó÷ÿˇ˚éˇÙ™˝˝Èı˙”€ÏœÔÔ≤üø≠ˇÁ
ˇÔÎˇı«Óﬂˇ˛ÿ¸ÒªË›Ó€Î¸∞∫ﬁœ≠˜◊πø ›≈èÏØœÏØÉÁ©ÒÙøøæ≠üÌﬁËóáÌ€™¬ÈºˆˆÀÛªÒ‹ÈÀÌ≥‘îõÅÕÚ≥û√®
ÄÅñê¿êàöÄê˘∆ñπ¯Ú¸µó’ïº€ÃˇÌÌ“”ó°æªàıŸÆÀıŒïÏˆ¬âÏ‡ãûœÆ¯∆óüùÑëû∂™Ñë‰¢º√˝ÒÍ±„Îóü¯¬¥ê–
´éÈÿû¶Ú§ıüœÍµﬁ´‘“˘«û˜‹ﬁÁ©⁄≠ﬂ´‹π¡˘é»°àò·é÷∏ﬁÅ†ƒÒºïƒ´çæ⁄êƒçàœÅÙñÑëÏâÌ»¶ÅÖÏ˚‡¬ç…‘µ¶
ÔûÉ˘É£¬ê…à¢ÇÂ¥»»ﬁ§•“åŒäƒÁá¸º¸ÓøΩö˝Ï˛ÙìÌüÅÚ¢⁄ëç∏±çÌ¶¥ü∞˛¬πª°È˚∏ÈŒä˜ˆƒ∑¶€ÛÌºµ∆∫â≤ò
∆öÄçôéÊ∏µòï‡é Ç¬≠¢ù‚ÈÛΩß÷´ƒå‡ä∂≈≈˚”√áØ‡¶üèÌº©ÓÕµ˙˜∫≈†ÌÌÜæèØƒùÑÙ≤˜ œË†ß√Ç˘âÀ∂©ÿ¨Ê
«ÚÜœõ°Ôƒ˚ºëÜ‰¨ñÛ–ÊÜä®õ˘’≥Œ´Óﬁ˜≥ÊËøÎè≥éÓ•ﬁè≥¶ó´÷ ´°≈©Ÿ˘¢åñ–ÒéŒË˚ÃÅ‚Ñ˝…∏ú√ƒ„Ÿºﬁ˛Â
‡ñé†Ç–¡ªçÛˇ†ˇÖ¡∫ç˚˚Ó¿ùéü˘”£˛í‚Øè≤˛òùÿ¡ìèÛ”–ÖÔÍ¸¥∏º≠ÆˇØîÎ”Ê¸†˛ûÔ’˛¨÷Ω ÷‘⁄íÍÒ¢
∆ßâë¶÷”∏˝◊æ¸˙ö¸˘ˆÌ˛◊Ô üøˇ´˛ı˜Õˇ¸◊°˛◊ü™ˇ˝ü°¯Î˜…èﬂ˛ªÔﬁË◊«ÂπÈ·ˆºóˆ◊î·ˇÂˇ·Ä‹Ï¯¯ÁÁ˚˘˙
ô¥¸¯≥πºË∫ÌßÊâ‚öÎ¢Î∑∂Ù≈ﬂ™€çÿÔ’µ™¿¨◊ã˘Æã»ÿéº£Ú¨®∞‰≥–√¬“◊≥€ù∫¥µ•≥ÿÆ°–Á—÷éµä©ﬂäµÆÙÜ
Üˇ≤¯áœ˙ùÛŒæ…ñØÉŸÒŸí¿êÈÇå—ê†∞õÏ†¢‹¸˝‡ãÙ˝˝Ÿ∏¸æ≠◊ƒØ≥“Áù˜‹Ò˚˘ùÔı°÷¯ÍÏñƒ¨®éπó“ÿ‡ﬁ¬±Ÿ
ùÅ†®£≈Ω¯Î¬õ–˙∂ù›ô«¬áÇ°¸”¿ë††Ç¶ßæã˘ûñ¯∏µ‡áà†ƒÉàÊÂπåíí¯êˇüóœÌ∑∑èõ›è˛≤Ì√ﬁ ƒ›£Ä›¿ö
Ø‡Öà≤£“ä¶â”«˜û¶ª≥”≠Ω—≠âÍËŒ¥Û∫µß∫…âÌ≠ÑÊ·∫°ãõêËƒ∏ïœ„–€õÅºÑˆßßö≥›¿∑ç¿∑⁄Ë‹Û∞Û’¡∂∂∂∂ß
ÑÎÎ∫∂ßÎ„Â¢ÿºéø◊–à®≥ƒ‹§ÂÊ¡¸í≥·ŒæØá˚Ö∏ê“ ıÃÎ©ÏÂ∫ì¿†∫Ëè®¿Ëáè“˛®ı˚ë†Ù–¥‹÷±˘¡≈ïµ◊∂¸’
ì¢∫õÕœˆáŸÆøÚ–©¬‘ÃÄŒ¬ê£†—ì£¡úÆ¸ŒâıΩÉù†éŒòÏ∞øå˝Ú∫Ωƒé‹ÒâΩÑΩÒÁßûâÇàπ¬∫ﬂ¯ö¯∏•›ﬂ…˜ØÁ
ﬁèØÁ˜Øñ–∫ıüüó«˚’§ˇ“ö˛”÷öé⁄˙õõ˘Ê∑€•ˇàø˝»˙˛˛Ü‘–˛˚ˇè˝ˇÒ”·ﬂ√ˇÿ°ª¸óˇˇÒùÛ›€›€ø◊õ˚ß˘‡†
˝Ò¯èﬁ¿·û–ù‡é◊ˆ¯·≈éÁÓÚØ›üΩÊèØ◊óáÕ’€Ô≤Ù„Ÿ™é„™ñ°öªÌﬂß´Äæ›Ëõô’˙πõÀ¢à⁄Ç‘†âúºÿ¢¡€¨¨˝Ì€
òÍ‡™Å±é˜…ÀËªã¥±ﬁ«ô ’∫áÇƒ∏ª¬”∏˚ó‡Æˇﬂ™ÔìÂª≈ëöççÁ®£ı∑Ø·éäù¡¯∆‰ãä¬¡ä¸¡‰≥ºü÷„ıöƒÇ∞∏
Á£¡Ô˜¬ßãÂı÷®–ê¡‰‘Î≤ö„æ¥∂∑¯óæåÇ¿ÓÌÇ¯ÙÑ˜ëÙò√∂ˇäÖΩà±¬£Éó˜¡ë¨û“ÉÔøÉÍéä∏ò„Œëæ≠ ÇÁÜ∏°
åøØü€ÔÔüıªü˝‰€áº·à√üé∑ˆÿÉŒ«ç–ÉÇ∑¯Ç¸áΩΩáÓÈ˘µ˜¢πì≠í≥’ùÃ∑Ò‡§ãÿ¯Õê‚ÜÑ˘¨à¯íâªâ‚Û¨àÏ
ﬂÎ‚öã´Ææ‹ÒÒ≈∆◊ñ∂÷∂ßäÖ÷†∂ñÆπÛ©„∆ÄÚÓ¢Ü«–˛êéîÀ˚ÖÑëÊÑÃ“∞Ÿµîç∑Ï–¨¡ã¶åè¯˛àÍ√ıáÔæÈπ≤Âã°
Ë⁄ﬂ—≥¿Ÿ†”Ã’«◊Ω›∆û–ÿë∏Ç∫Ñ˝‘éêïÊ¬Ï√…‡àìÿ·∆™úÅ˚∞Öà¨Éﬁ¿ﬁ∆ñ¿ÏêáËü—Ï¿ÚûüòêƒÈëÄø√ê±ö∏
Êø›‡Øœ¬ıïïˇ ºáŸô†·®˘Ù∞øÆ‹ñ‡àˆØ÷ïçﬁî∆ñŸï¸ƒê˚Î˚‰˘œﬂóˇ„˜ÕØø˝±ˇøˇÚ´˙∑øµˇ˝æˇ˝ÒùÛÌìˆ
πˆ…˘ı∏ºˇπú›ü•ÀÉû∆µ±∏”Éªø¿úÔŸÛïªÓﬂ¯ÎΩÙˇ¥‰òÑ°¬Ñë¶˝™Î©∂©ôÜ»∂à¬”á¡®ùÍ˙µúœ¿ê∏ºÉˆò…¸‚
ñÆ™ úÏê∏ƒ⁄ñÏ“◊Õ¨Œıºõ«€©—≠¯‰√Êö€öÛ¸—ëü¢˘€⁄Ï˝›≠˛Òø◊˚∫ÿ‹ºŸ•Ô±¡å‰‹Ö¨à¬òÜ≤ëÆ≠®ÍÕ‘Î∫‹
æ“˘ú»Á¯Õ∆êß›Õê‚∑ÅìöÑÑëõç¶ÿìÈíãÁºìœ∫ãÊ„¸±çµö˘≤ÿ◊“æˇÌ¨˝±üïø…Ñ„»‰û†ë˙¯à°√êé¨¯¡Õé”ó
√º‡ÃÌÁ∫å¿ÛŒÌˇÍáàöè˘˘˘‹˛˚Ø˚ÿ˝Ëß€æ≠‰≈º•¥‹˘ÃË∏»éƒà∆∆äÃ‡ÍåÏôÇâú·°ÖØÜà˜êÆ Œ‰ÀÑŸ∞•ùë
· ÜâÈ≈Ñ™ì°ƒ∏ˆà˛°îììÓ¬ﬁ˙ê†Ä é©€áà÷‡ÇÜàÜ¨¢¥ ƒ§’˚† ‘¸º™Ë®œß√úéã¶≠Ë–’æ∞¿’°„™Ø≠∏‚œ›
‰Äª‘Ω€ıÏëêµë•»≈ã–›™‘˘™ËËΩ™Áö√Û‰ÇÛ®°æ∞ÂûÆÑéÙÉ¶ùÜÕ–‹–Åö∏‹îÊüéËˆ·êÂƒ∞æÙÂ˜˜ÄÛ¯‡è®Ø
˛ñÄ„ΩÄ‹Òû∑¿≠˛Ò™å∑à«´¸ºüùóØÛ˝˘Ÿù◊ÍÕØî’¶Õ’÷®÷“¯◊’÷Ô˝¡˘ˆ°¯√∑õ‰ßÔãüˇˆË˛¸˝˛√ª˚‡õ◊Ê´´Ú
∏››Øã”’úæ›•ÛΩ¯˛¥êÆèÇ≤œì‰èÇ∫æÇ‚ı™ﬁºˇúøº⁄˛øæﬁûÔı∫˜Ÿ∆ëÍÍ ÓÜ∂¨¥ŸÌ∂´ÄıÉßÎ≈ŸÆ‚ó≤¨ãº«»
˛í∑òâÎ•¨â´À˚∑éÌµøı∂â≠ƒ∆¢’⁄ ±¶§öüÈÏçˆ‰áÍ¯˘ı•˛◊ÍÜˇÿõˇ∫ªﬂñ◊˙…ﬂ‰™∑Ö¨”≈¨˛ÍÚ≠ì–∆Û•’Ì
•ò˛√¥Î™‹Ùæã¢”ÈÉŒêûˆ»êΩ°£ƒ«ÏÖ⁄ÂÀÖŸ ◊¯∫ò∆≥Ê—∂ú‘õ¨€ıíóõ˘ÌÛﬂˇ≈‚üÚŸ€î¬ø«Ãôê…Ä¡«°û«¯òÅ
Ç£ê†‹êÑÇåŒ∞œ«Ωê«ãÑ˚±‚√˛ﬁéæ∑ﬂﬁæˆøá˙…∂è˘ëç®Ôåÿ˛Ì˙ò˚’ø¿È–√©¯ùŒê—ƒéßÄŒ≤ıì¥Åù‰§¯êÇƒò›
‰ñ£ëô™ˇÑÚÚÍñïù£º◊ÂÔó≥°ä„Ä‘““È≈£Íî∫√∞Ÿ√Âç∏øÄßêÕ—µ©Ó∂Ë—˝ˇãŸ°Œæ¬•ÇîëÖ™å≈‰»íñ»éË·”√
∞è¡Å”£“˚˘Éó∫ˆôû≠ª˜É£ö°‰ÎÀé¶Ø¡ë˘ç°äÃµ˚ñÖõ–ÁûÃ—ˆÙ⁄®í˘œñ˚™æÀ“Ì∞†¥Ë‚∏∂´ÅÇ‡üÖ¸±∏¢ø∞˝Ö
›ÈÆÙ˛¬ûÆ©‹ÕÑ˚´ﬂﬂÉ¯ÓÔÁ´Œ¸‰˜ö÷¡ù≈˛ﬁ§Ôœ≥√Â—Ûù–Ã˙’°µ‹’—≤ÚÇ’‡˚Øã‘Ì´Ì„©¡¯˚Ô˛˜™ˇ¸ÒõÙ˜Ô¬
˛˜ØÏß˚Ì˘ı¥∫ùñá”¨Ïø á≥Ù∫äçóáÒ˛…¸¯Ï•Ωæñûﬂ»ÔèØÂá◊Ú√ˇ”Ï·’Í·µÁˆ¯⁄º›≠≤Ÿ“Éˇ≥›¨’πı¬ŸŒóãô
÷ÄÅ√˙Ω‹Ø∆ñÛπ§ÀºœÛ—œ”è ‹‹—Æ‹√âÌÕÍØ®±πäˆ≥–‹ï‚çÀ∂‹°˚≥Ω£ﬁ«À∑ˇïˆá£ı„Ÿü˝”’ˇÈ°˙ì◊ò«ˇïÏ∏
≤”¿å˛∫„¶Ëªˆú»¯±¢ÿéö˚‘ÿŒ°ë´•—¨Í≈Õ∂õ™ﬂ≤˚ùÙÉ¡Ïçπﬂ…ÒÌ÷Ë¥Æ⁄–˜®∆‹äÀñ„‡ÚΩÈ’ÏØì˝˜∂˝÷Áæè
˚ÖüÚ˘Îåø†¶Ãìë˘ÑÁ≤±û¶ƒúÄ£¨∞øÄ·‚˘—ÿ∞Å¡Ñ±ö⁄˙à“Úñ∂ˇ√˛ﬁæ∑ﬂﬁ∏æˆø˙…∂è¿˘ëÌˇå∏¡Áƒã¶˛¸´‹
Æ±ı¢âõç—ªíˆÙ‚Á®˙∞Ï‘‚Òâ•ï˝ûçıûäÜ¨Œª™ÈÈÓœÔﬁèÃ˛¿Óè‹Ú˛œı˚∂Ì°µ˜∆£∫ÕûÈ˙è–≠£Òˇ˝îâçõèÉπ
ò◊ºø›œé◊ÿÈˇë†è≈√ñ∏ø¬ç≥Ä¨õ–ü–ÖÓÜ¸Ø—≤√êëƒÇ∆‘—íÖ™æà¥äˇÉî™ñ¡ÿ∫ÄÖ¥õô†Ê®ƒãœÊ•Éù¢’Æ‘⁄–
∞√°¬Üƒ—û∏—áﬁÓô¥•·ÉÍöµÿ“·ﬂ˘û¨éÔ√âŒìöŒﬁüÔü∑ôÎ˛Ãˆß’¶≈ØÅï∞ç∞ºÁûÖ£ƒ‹Û¯ïàÍ™ıéüÊæƒ¶ó
Áª√ºø≠⁄√ßºÓÁ≈ªãÛâŸã•”ëÕÀ≤ä¸Ÿöœ€‚˛ıÓ¥«ìÒÓ’—˝ÛË›ﬂıÈ√—È˙˘”œıÍﬁÆ˚¥¬ˆ∂¯ëÆè“„´ˇ˘ÈıÏı€˘
Ìõ˜À◊ÔóØÕﬁÆüﬁ¨üﬂ€®èÔå«∑ﬁøÒ‚ÂÖ·⁄õ™Öº∞ìÊí†ãÒú§Ú≥Ÿ⁄ÕÃÁã≈Ï¶ã◊¢»Å†Ê≤Øœ¨‰ù∂Ùå≈¶∑ï»“ÓÉÀ
°°¶Üäì´ÃˇÆ≥µ…ùÇŸÌõ“˝·Ë˝¸˛∫˛ÎÎÙØØ˜≥øºÆﬁıﬁñæÿÑäØÒ”ºÌ»÷ß—£¿Û¬™ïı—πÎïéª®Ù≥¶áî¡êµ¬∂™∏
ÙÇâ‚‡Ãˆ∆ó¨◊≈÷„õàê¥–¡Íº—¶±ù∫ΩÏÒì¡ºº∫ÙªÄ˝˝ÛÃˇπóøÕπÔ«åﬁßƒï‚∂ÂäÁôë§ƒÄÑ·¬∞ı¿êËæñ∆†å∏≤
∞ËìÑÑâ√‚¢Û°øØü€®ÔÔüªü˝‰è€áºà˛º¡⁄§”†≈¬åãÄ≤Ùå≥¯∏çØü’Ä«•û‘∞ƒ⁄ëŸçÍÓÖ£˙ØçŸé‘Æ‡äÜ™€∫È
√ô¶„™‘œú± –Ì¶¥ŸÅÅÌ˜ò¿ıÊ´ùà≤‡„Ãïµ—ü’‹⁄≤Ò¡åƒËÏàŒä∫’Öò°ƒ¡¬Üß∆ëûÅØ‰•πõóÏñ≥–ßç§ôù§î¢Å
úË†Ωë¢¯Åπç©çﬂïñÙëú”·¬Ö©ïÑÂ‘î”Ò…ßñêâ¥Ï÷ÒÒ∏Õá•ÈÊò◊Å‡¡…ƒµô∑Ù‘“£ó§öÈÊ˝∏”Ò’ûÖ´èœ‘√Ï“
Û£Ωíúà¶πîï√Ô∏Ù≥ÔºÒ–√¶’∞Áöê≠ñÊìíÌ¶ÉÃöŸ“ ·ÊÆ˘©Ëá¬ŒÌÂµì◊åﬁ¥ÓÖ◊ó‚ÿŒÛÂ›©˝ßµ›°¶Í÷ŸµãÀ
‰±©ÖÅæØ˘ı≤ïöﬁËé‚…˛§ù–Ç›˘Ç¯ùùØÛ‡ˇé÷˙”∂ª⁄≈ûÂ˘æ∂€Æóôí†ãú¿≤¨™Õ±åÕÒ¨†é¢»√—êÿÖÅÁñËÓ˙È
≤ô™ÁÈæπâïÃı∏€Ñ€©´‘Ñ√¿¯–˝ô¸Ôä˜ÈÚ˛›´ˇ·‚¸¸∂¶˘È©˛Òæ√˛˛ú›Ú£∑∂ñÊúﬁ“„ïµ⁄”£Ö∫≥˘Ö…ºáﬁûÍ
Ù∑Ù´–ıöÇÎ°÷®÷•ª÷≠€Î¸ˇæÇ›ËÙ±—¯˙√ˇ‘œÇòˆÍÙ˜∏≈œª‚—¶Øﬁ∏Ù∑ÅÛ√˚ÃæÕò˚îÍŒ”≤ à¢£≤åœ“ºÇ˚ñ
äêçØ˘ÅíƒÿÑ◊ø£”€Ñ∏ÿ¢æ‰ÁÂÛ˜˚ÌÌ„˜£ü∆Ï˚êó±ö§“êÂò™ˇ◊¸Ëç†Ö‰Ö§Ü≈§¶“ö’÷ÕÖ·û˛∆çùæœ™ùóï¯∂‘
˚¬ÉÁ öŒÜ¿ùÛËÛ‹ ¯Æâ·«Ñó°›öıàòé§ó—¿ﬁ»ªÅÂ˘§˝ÈíÜÉ√—Ú à€ß·…∑§«‹àˆ„∫»òú∞·¢îç”∂ÿ…ÄÆéÿ®§
Ç√üäº™©±ﬂ°˝õùÑû¿ùÖÙú±ÒÃ·ƒ©ìÁÙî“¥¡ ˘âÂßÉÆôÂÁ¥ﬂÄó∆ò¿ÀÈß£Õ„î∞”ÇŒ‹úçÙÚ Õ®ß≈†¯ôùî¥öä
ø©Ÿ¨ÂÚéØÊ˙æËÜ⁄æÏòêï˙ÜÀöÑçõñÏÑî†ƒª∂Ãÿø·›Üà‡ºÿÄ©ÑèôÏ∆¢ïäª£è”Ïˆ„≤ÎŒù°öúÓÛñŒß∏°‰«Úò
´Œ√ÙÊßíõ¨ãŒÃπ±Á•˜úü¬è‰¢ú–À é¸ö∑Á´Ö™ó°≤®¶ªπÓÊÂ≥™‰– ´›êÈ¶Áπﬁ¨ÁíœÚ¢åÕ–§…‡Ä◊¸ü¢©∞Ö«
§±ºœ¬ê±ﬁﬂÕÙ≠‚⁄˙‡èÍÅµÒ˜ÀﬂùìÉ∞É ≈õÀ¶ÿÒ≠£ûÉïŸ”°¸ª∑˛Ûº˜˛˛õ˛¯êê˝˜äﬁÙÓÜó™†í’§•È‚Ó∆‹≠»ó
Œ´ÚÂéõ˚“∏¥›êÂäÑ–ÙÇÑ—®Ù≠ÃäÙØÛ˝ŒÄ¡ŸÙªâÏ–ÓÖòöç±˝Ω†Æ‰†àã¸ÌìÔä‹Í€ü˙ï“˜óÑÍ≤™ …§¢ß≤úœ‡ü
ÄÁºÖ¡¯æàÃÇ«Çà—˙ç§ﬂúÒ˘üƒˇﬂŸˆ±˚—œ∂é˝»ãò¡ßøÑƒá¿ÕáÈÕ›†Ò§ÜÃ˛Ö»¬ÍË—»Œ—â–∑¡Äóí‘¥œˆä ÑÁ¯
¿⁄êêµƒπ®ÏÅÌö∂ÉÃÙæ–—ÌùÖ–á√∂Ê‘≥∞’«îîÌ∫“ßãÂÿ≤◊¶ÀÕÑŒ©◊€Ã—Ò’†ñ¥ì‘Ì⁄ËÖÀÂ‹°◊ÁòÃÒ¿Öûõÿ
≠åÁ“éŒŸöÆÈ©Ñπ°Ï≠°Èöîºü∏Ã≈‡±æœ˚ÅÌ¬°À–Ë¬º°ìÏ˜ÿà⁄ÃË“£¶ˆ±•«‰§Æ¢çõ°©á˚ÑÊë™á√âÇôÉ©¿®Ãÿ
≠ÅÇ¯¥ï“∆€ÃÄöé¬Äü°‘‹ÿÚﬁŒ∫ﬁ©Î¶¨ó†úÎêµ¸∆òù±√òûÁ¯Çë≠à€óﬂÿÏÚÃ«Ø∂Ù¸Àœ ¥ƒ¬ß◊Ü≠ˆÿÙ∆‚‡´™
äΩ‡∞™√ÎÆ˝•˚ÙŒßÕÄÂ∂Ç⁄éô¿çˆô¯˘¡Ü÷˝Í∆πêÁ‚íÀ∂·≤£≈ıµâ√‘∑ ‚ÆãƒºÕ—Ñ¡‡ÿ¨Ù≤¥È‰äÕ¥ÒÇßÈí„ô
òŸñ∂õ•ÈË…ÅÔ≈æôÿ™ëßØ‹≈∑‹ô∏·„ï©Ã·˜Ô…◊‰˘∂Ÿ‹˛ˇ≤ˇÙò¸Õ¸Æ˚·æ¸€±ªêæ‰÷“‹êÒãñ„ßØÑœÏÖÑøÄ«®
ﬂÃ»‡∆Æ∏ç‘Á¯∂§´µ›≠©Ï∑Â–Ï‡˚‹Ï†ﬁÏ–†©≠êË•ÎµÅ˜â‡≠≤›´”Õ”ñøÇ†Ã‚ë…§ﬁŸ¯õõáª˜â”ƒˆôƒ›ï‘†à¨Œ
¶—≤ç£©Ô „¡†à·Ç‚ëÊàäúë˛Œ´µÒﬂµ˝Ï˛”ÙìÌüÚ¢ÊÉ√úÇò·Ñ‚æå£°ª†—‡˛î≤†¡ºèäùêùÚ·ÑâÅÏÄ“à”¥·
¥Åå√”¯≥ùºÓ€ß˝©·Œ•ﬁÔµ√ËÊ‰€≥õ®€…⁄õÃ¨∂„˘˙§Ø‚∆“Ú‡µñ◊Îìô≤ùÔ∞ÖˇÜØ∂Ñ¢ìõ⁄ùπµ©ù…õÏñÌ€åÂ–¶
ÚŒÔ¥ ﬂê⁄©ö–◊¯ÊâÌúï¸ßíñŒŸÃÜÂôîÈ—¶‡ÕôûÂÑ®Á‘áÖÏ¬Ñûêß€Ä‘Ç ıëÜõü‹¯ÉÆ¬éÒàïçØ¨Ï∏Ó‚¢∏Ññ
∑¿˜“ÊÃÿ©çÁ‰ºõâﬁ¶ß¯Ë»è‡Ö”¯ÍÆ‚∂°£‚áÃÆ‡çü’›÷Ûò‘Áõˇ®€ﬂÿ‹∆ÊÃ◊Üª–ôŸ•‚§ïíÔËî¥ù•‰ùöÌ»ô
—–…˘ÀŒÙƒ¿¸«ÆçÚÂíÅä≈ÌÏÍ„—„ÍÄÊ”€á“ÿ›êá®áÖ˚ΩòÆø≥•≠πÁ◊Ù’⁄ŸŒÁ·í§¨…êÉ†ù∆˜˜Ø·øŒË£ª…ÙÃ—
ÿ§ØãÂ‹”ñî”‘¸œåúü›€≠ÏçË≠»È˜Õáõ∫—ªË∆ñé·¸°⁄◊ﬁıªÔŒˆ∂üÓ˝üü¢∑˝Ω˝ ∂Ö≥≤êïﬁë…Åú—›„ØŸ…óºˆ
õïÑˇµ¢ﬁÍøÃ»‰«æÑ•®ê«åÇò¶û√ÑÉÅˆê⁄˝ê◊µîäù´‘”±ÜÉ´÷„Ú÷ô†«Ëÿ’√¿Ø¶ˆÔŒÔˇ≠……√∫ê®‡“≤Àâ¥∏î
 ÃÉËÏÖì◊Ωü∏ÓÙ˝˙•î¢¯÷Ñ˘πŸ˝ﬂˇÂˆ±˚—∫œ∂˝»ã¯¨´à¡é…∞¨ÙîÛÑ≤ÉöÓ¿çÁÉœ≥èê¶∆§≈≥ùÇ‘ûä˚äÊ∑ìåú
åë‰∏ó‚Óºú≈Îü¬âùÃË≠ˇÍπ””ùÍòŒ‰ÁÃ∂÷‰ƒµê¶÷€Ò¥“õÕ∑£ôÚ†ß´£Èçå•ÿÂ¢ÉõÈÃ≈ïÊöÄØ∂∑ŸÑê™¯‚ÀÓ¥
÷“ŸêÎÖÿ≠„ÒÂä€¨ß´⁄µ¶®∏¶í⁄≥ñ–ÙÚãú¬úÍ∂©π†É¬ôÛâïàçÄ¨¢…µ√ÆÜ·ñ∂¿¨≠ƒ¬ê§ÊÜÆÔ∞ÈãáÕå–Ñéﬂ
—Ç◊™Æ•˙åõÊ≥€Ë¸ÉÓîÄŒÿÌ¿ËÅﬂ≠˙¥‚—ÎóË÷‹Ê„≤™À¯Óú÷±Ÿï≈’¨ÃÄ›ôú∞«ÿ±≥≠≠ÈœÄÍ¿Ï©—«»îª“‘á∂àÈ
˘˚üÃ±∏°ì˝ã≠±–âÂéî‹ô‘öÿ—´ËÄ‚êç›©¬¶“ëÁÀŸñÑÈÅÁòäƒ·√ú¯¸˜ÚÂãËúØë£…à‚®º—ñïç≤›Ó◊ÃùÍ⁄öª‰
åÜ∑ñÏÓ∆∏®À≤–®‘£€ÿ«º≈ãßÚÛ≥Æﬂ˝˝∞æΩÆπﬂú‹Ûæ≠è›•≥·∑íπò„¶ú˛íöˇÖ¡Ãæ‡ßﬂàÈ”Øó‰ãïæ¢≥π¡êÛ¢ÿ
Ê—≠ †¡Ë¸¸ëÑ°ÛçÜí¿Ï‰Î∫€—Ÿ—äÃ—ıπÈ∫éòßÛª‚µÍıîÒØÕéªÈˇ…úû¨…Ÿô◊Ùﬁ¥ä∞¨¬Â£àÕª¯¡ÿÂˆ∆ñ≥∂ÌÒ
ˇŒúòˆêÈ≈µ√ú–ƒÄ˚ëäëƒ§°øØ£ü€ÔÔüªü˝˝‰€áºàÔñÖÅ‘õ˝»ÓÀ‰ã‚∂≈¯ú÷å÷ÄÛíÑ»äàî∞îª∞§”Ø∆âïê
∏±⁄ÑãÏÄñµƒŒú¯Ïƒìæó≠Üö∫·î“≠öç¶ñøŸµπì›∆°Ñ‚Øå◊ª— ø€ËË˜≥ıŒ⁄‹√âÎª·ΩíîãÒ¿Ñ‡áƒÂ¨™©”ŸÜõ©
â∏¸„çÀÎÃå‹Õ≈’ƒﬁÕáÊÎÈ‚Ω›ÌœïäØµ‚≠≈ìÎì–Ï¬Òßµãÿπ¯ä¬‹ÄÙ¿ÇÛìßúôÑÏ†§ùÀÄã∫Äâ€åØﬂ¢£†„å
à¸âÓÔÁÜµﬁ∑È≈¿“‚ñÚñΩ¿–ÛÅŸÅ¿úΩÉﬂ‚ó®ÉÛÛ¿¿Ãˆõ¨¨≥“ñÜØúà∑∂¥†Éî£¿Ô»ﬁß˘≠Ö…ÎƒçÅ–…Ñ—˚è àõ¬
»–¿‹êíÇ∞îï¬ÿ·¸íµÁó§∏ß•πñß…º†∞úÀ÷èÇòŒ∆£ê£√óﬂ˝˘„Ó®¸«±¥ºüÈÙª›ﬂÈÜæ±ıÙµ™Ã∫‰∂í≤±ïßˆúã
ç÷ƒ∂•∫◊º∫ßÀ◊µˆÆ∑üﬁˆÊõﬁ¸Ï˚å⁄ﬂ˘ö†àï∏ƒç¯¸•Ω€˝íúˇéöˇ≠éôˇéùˇÓçéˇµ°ﬂö»⁄ê”¯éÁ´≤œŸ–ù∆ûÙ˛
ËÏà‡ä˙É“ÇÎƒã‘—ÆﬁÚ‡Ò¸öÕ∆—ÂÏÆîﬂ§ãóü≈ÍÆﬁå∫˝Ø‹õ„ΩΩŸ˚óì⁄ü‰˛§ä˙¶ûø∂›±Èùåˆ÷›÷òÈÔªë™è¥îÚ
¯Âå·©È˛™¬â‡√˛ﬁæ∑áœÌø’ÉµÄïÈ‡á·™öÀçê£¡é¯Ü¥Å≈â•ÔâãÀÙÇ†õ˙†Çù¡éŒ–ˇª‘≤Ïó¿é…‘ò¿Î„”Èê
ƒ·√†Å°Üº‚ûÖïÊ∞Ÿ˘˚ì≠ª÷ú±Í•åîÎÓÂ‹≠èÙë¨ç§¿ù˚âÍ»ÿå∏ÕÈà’∆Ã˙ƒÆΩ«Ä∑ûÎ©¡ØÃÖÖì∂¿€îﬂÃÑÕÿ‹
ã††„¶Ãóõ÷Ï°‚ÀçÿÈäßÕ¨¨‚µç®·È‡ã¥¸Ñƒ¨Ü»·Ó¿˜üèº†ÍÑ†Î„¸ñÁµú”’∞õÏçıÜŒ…©Ñ¬Ééú∫÷ÒË€ë®Àˇ
˘Öù¬ÿ—¢¡–»Ü´õ¨‰æ‰áûÓÿâÏ†Ä≥¢à‚˚ãúÑËëΩò°òîª‚Êàù‡∑∑÷«êº Úõíï€êú¬∞ÎÄ∏Á◊òªƒÿï±Œç∞ãåöÕ
à…˘¬Àò´ìÂºå—Å£ä¥√¿”ºÀ—ËåÙÈº∫˚ÒÚÓŒ´ƒË´©æÊ§≈Ó©¨Õ◊≤ß≠≠π€õÍÏá∑‚∂Û“ïß·öÁ≠¶ÅÇ›Áª∫Û•Û∫
º˙Í∂Øª›√ª≠üã≠ãî¿äó‘ô¡ìØ†òÄµê£ˇ°‚∆ˇÇ«ÍÙœ€ÄÑ˝±¶ﬁÏô∏ªœöƒÛ¨»ﬂë†àﬁ„è¿∫˜Å¡ËÏ©Ö–‡á‡∂çÊ
Åì∂éÁ£ˇÁˆ“ìﬂ∏“óó˛¸´Ë˘¢’úﬁñ€ª“ò’üˆ®Ú∆©ªäÔ›ë„‹ô÷ù¢ÓﬁÌ˜˜™˛˜∑Ÿﬁ©€ªµŒˆ∑—Ô¥„¥£µê£†¶ΩÈ
˛Ç¯ÿü°œÌ›ø≈˚á°˝ÏÜ˜Ò°€⁄·¿ÿ‹‚—Å°ı≠ôŸî¨ê†≠èÄÉµ∞ÕÄ¯ÕÈÖˆ√ÑÖÊÁ§”ØÖ»·ê¬·Ù˜ˆ¯ñÑõ√éΩÏÇ™âë
¢–û¡°∆äπ‡©Ê•”´ûö·—‰óŒÙÖå∑—›…Ü¶πä≥ŸŸì•µÓŸòìπ±ôÌ†¶≥˝¢Ô·∏óùÎÿ‚áØçƒ‚È¿ë€ö”„µâÌÃ‹≥à
°≠≈¢Ïä·“Ü€è‘¸“˝ø´◊†å„Àä√˘≠ùäÇàÖı—¨ââ‚á€‘ÜÄ¶–ëÇíáÔ“á®ìÇÇ∫¡Å∫ˆÑı√ÿä—œΩº¡¡Ô§Øô¥Ï≤Ä‰
Ö“∑‡¸Ê≤ÛΩ¿ÄËŸéÜ£¯ä∆†‹∞üôñª‘¬˛è≈≠ÙÃí˘ÎòïêÕ∂®öÄàÇêà˛¡√àõ‹ÑöÀÆ°∞¬Áä´πÿ∞´çªﬁ´≥Îïªﬁ∞≥
ªâ‚πú¶«îàΩ·∏åìà˘≠Œ®™º≥◊¡‹Ë¯Ó∆µñˆ»Œ∞’⁄ÿì˙æ‚™œ«ÆÇ≈‡∫’∫Ïó…«ÏÏ˘≠‘À•ı◊…∫Ùó°°Óé÷‘íü∞∏Ñ
¯é»çƒËüâ®ªÑ˘˜œÄ∏œ˝›í¯“„ÜÎ‘¨ã¨‘»˝ÓñÏ¿û˜‹’Ö‡˚ç©ïàüÃ‰á˘∏‹åØ£Âùø¸È¨•ø≤óﬂ⁄´ØﬂùıóûÚ’†
µΩàıóëÒ≤©≤ﬂ˙÷À€¸Òπ≥ﬁÍªóöÕ˛“›æÑªÄ†ó‘∞Ö“√Ë¬Áèç˜‚˙ìÁ€∏•·œÅèÌ˝ô˙‹æ˝ˆéèÔ€«˛£ü◊∫≈Âîàä¥
ÂÎÅ˙ÛÄŒ·äôêÇ®°Ìêä…Éû∆µñ÷¿ÿ–‚˛±ı≥√©õé“¯ñƒ”†ÜÙ¸√ŒüîÖÒ€‰á∂”¿¿¯“í°–·ñ¡Ö˚¿â˘ÖœÊÊòõ·ŒÜ
∏ª©Óß∏ï‡ûÊÇÌ‚©ûŒõ∑¡‘ı¶õ˜Á∫∂ßÀãéò∑‚‡™À†˘ÖËè≠∫⁄˙¯Œöß≠Ä≠åı£ë∆âëˆ¿ÿÅ–⁄Ç‰Ü‘Æ§ßıêﬁÇÅ≠Å
∫æ≠˙î‚Ç¶˝ê£Ê√‘ÉÑ˛Üî–òí∏√÷≈∞Ü¡ ® ˘†Äô∫˝°Û˛«ò¥¯æÛøúΩ£öœº∑ÇÄ¿…àíŒÊ»ÊÑÃÍÜöÄÇºì˙Ì–äãÊ
˛ÆÄçˇåÍﬂ•ﬁòΩÒ©§∞Âã‹Ç©ˆÑ¨¸ì≥ùÄì≈é÷‹¢…Î–¢±ÄˆÉ∫æ⁄ë’ä¡´¶ªµπÀù¡õ∫‡ÍÇ·∫Ê◊ã ñÒãŸú‹Æô≈ì
ÿÙÛÊª´†‹£§È∂ä◊Åß˘ªñÎÎ§•‚âÜ≤µ◊ìﬂßñíõ©¬°ÀÊë˚Ï˘¨ïÆ€ÔÁﬁ‹ﬂâ˙íä⁄â”ë˘…·‘ﬂê„ˇ∆«ˇèåﬂ˝”ç‚ê
ÏÑø˘óπ˛ÖÄ¬˝Âã£êƒÒ£“ÁÉïê∏òò∑¡–†å¯∞æÄª¨«—Í∏î©˜ûÚ¬÷≥æ⁄©ﬂË óÙ¥îØÕÕóœñˆ•≠ˇÂÔıæ≤Ùó€Ìã
√çˆ¬—ﬂ¢¥ÈóêªÄù∂Ê »∑«ÜªÏ˘öüÚØô∑•ê¯¶öÙ˙ÿü°≥œÌ”Óˇü£€«˛ªü∫≈ÒÂÑàã∏€üø¯Èƒá€˛‚ûíñì∏ô§¿
‘ª…ª…“Õ‚¯ˆöµ”†∆¸ﬁäÛ ©õœùˆÏÍ§√ÏüπÏÄ∏ŸÿÕß¢æ˙—ÿ‘·… Õ¶çïˆ‘⁄”ìÈÅ†íΩë≈Ùƒ“Å§˙æª£‡§Ó»®£
â¬Ü…ƒ´êØáô‡ªà”¸‰Ïõ¿â·∆‰òØ´◊ùîäÚÙ∫ÆŸåÌº„çìíï±ã≥∂¢»—˛ÑèîëÖ≥ˆÃ⁄âÎ¶ûË—ëÅ÷ù°˙Ÿ‹¥±¥âÖ
Àâœˆòåò∏–Ïê≥ìÕ¨¿ÏÕæÙÍÀõÅ‡û˛Ó±≥ãë¢ÌÑ¥úÂÎô…Èôöê≠ÍÄ°îÃ¬¥ÒŸ¢ıÊ∂πÚÈçáÛÊØëé£∫åÕÊ≥€ÄÅÁ¡
ç€Äåò∂ìÜÔﬁûù≤÷Ãòµ‰Ä‚üç√≥Æ‡ßòƒ §¬îã‘Ì ıÄ…Áµ◊™ªÃ¡˛˚‰ŒßçÄµÜ¢ŒçÑøê√öﬂä›Ë¸Ö„Î«©ö≠õºÌ
ìŒÇÿ…ãÌﬁÙÍÂ©µïößëÛ§îÇóÚÕ´ÃŸ·ˆˆÍÏü±–¶≤ÄÎ≤◊©ÕÁÌæÏÛ®ıûÆ‹ùØ≠æ…ëπ„®ØËÈ¸ã‚ÚØÿ¯◊ü∞ªˇ¬ﬁ˛
ç„ç¸Âøí˜Îˇ»Õ˛Ÿ¬˛¿Â…¸·ä˜…ﬁíÎ∂ÙÉ≠‹¶€¸ΩâΩ∆±ïäëØ¶˛¸¯îéˆà¥óÓÔˆÖπ˝“”øπÿÏˇß®ø€§Ô˛∑÷˛ßÏ
í˛∑—ˇßÍﬂí®õØ◊ÎØóÎ´Øñ´óÀıÛÀ’ÜúÅÜïÂ¿éñøˆ¬âòõÕ—ÕëËä ¡ÑÓç’ùÕ¸Õ¸Ïœ–ÁˆÔ…∂ˇÊˆÄÊ†ó£ƒé∑¸É
øÈÛâüçÉ‚Ñú’»î—≥âÅÎÈ–ÕáµÃ¡úÏ‚•„ªôôÿ± ÷ÑÑëë£·•‰á‰ÑÄÿÃ«Œ∂Úõ·«ñ†âÈÿ¡‚—≈ƒµë∑≤ƒ‹Å¯‹‚—
¡É«µû∑ƒ“âê…À¬âºå‚™Ã«¡äÓÁ≥∏óëŒÇõ±±æã±£ƒÃàâ∑„¯Ü”ÚÖ®√ÅÑëïÙ““¢‰Õ∑˝Ÿƒ·∂êÇ®Å†Àü≥ﬁÅÛ∆
πÕËß•ÌÑ∂£àÆ®¥ˆ’ñßáÌè√ ßùüÑ‘ÄÇÿÉÈ≈ùÅ£ÜÃ∆¿Áîüù∑Å´ÄéÏ¸Ñ∫èŸˆ≠‚∂±ß«ÏúÆÊçõÜ“éÕ∞≤Œ…˘∑Á
â˝¬¿‰¬Äî§¬–Ú°–Ò—Õ˝Ã‹…Û€úÍ°§µê‘¸ä‹Ú¡é ∏πÄëÎÉ√Ã‰áŸ¥ΩÏã∞¬ ¯éÈßÑàâˆﬁ‚∆èô‡÷¯Ñµï≤ƒ¨í©
£çƒ®ê¸üã‡√±…Á˘π™™ö•Ó÷ÕΩÓÈÎ’’»˘∆ß‰±ùá‰îêë⁄œ∆æô≈Ãô±ÛÌ≠ıÕ¨Î’ÊÌ≥®ÛŸË„Ûñ¡Ì÷”ıæ‚ºú›Õ•ó
“≥Ò´Ÿ¯ Ê≈ÅØÅô†¸πÍîæ“ƒó™¯ë“ŸŒ“çË≠Âˆíàóﬂ∏¯∑¸Ÿ„˙õ©Õñ™”âÿ«‚„ÌöÿìƒôáŒ…ÿÛ«ñ˜Á∏‹Øç˚ÂŒ˙°
˘õØˆàóÜıê“∑ﬁéÛé£úÕü›™êòËƒ ö≠¿ˆà»ßﬁ≤ı”¥ˇÉÖ÷º∏„á≥±ø√ü€ß˜ÿ˛·øµè˝ÒéæŒÛÛªÅãëõ¡óæƒÅ
´–’√áüç¿úñ®ö®êù¸é‰áâìœ†∫∑∞‚±ôéÃËê‰≈„ªÇ◊éëØüú·«£´æûôÒùï‹ŸøÈ»Üüô£–˜¬Àß˛éÜßÉ†ˇèÎºúƒ
ôÄûñÿ‡éº©Æâ∏ÒãÃõñÜØºîÛ”ú°’° ¿ä∏ôÃ‚÷Ñ∞„åûïÒí¬œâÄÑ–±êæ√˛¬î†”ëÚáìïÃö‘≥—ÕÑïì·ñÙ––˛é∂
íÀª©˝€…∑öÏ¶£≠Ñ¥ìâ“¿«  ©ª«•≠î∂í⁄èêëê˙¿ÁÔßŸúµ‡∆úı€‰ü—¡•ÑïÑê¡ﬂﬁãÈ´ÎÊù‘ÙÚπüÖï˚˘∆µ–è
¥‘˚©∫¸Çÿ∆ù≈˙®∂ß©–≈•ÕÂƒë’îøöäü…≈≠¶ÂÖò‰¸î⁄·ÃÜ‡ÇÅ¥Ÿ¡º·ìÙÜìÏ‰Ãñ±˘Œï•Ö–ó‚’àÂ°ñπú•™ö
á’ÕºÈó≠‚¬éùˇ”À’…ÏóÖºû¡≈ù¿≥–É”’‹”·›Øˆ˚©€Øœ∫˝ºÓ‰¨¶ì¬êı¨“≤úÒÛáÌ∂ì›≈õÏ’±‚É«¬ë†Î£˝≥⁄
ê©„„Ωˆ¥¨æµ©∆Áé±¯€åÖ„Ãü‚æ‚èŸ¶‡äèŒÏº≤Ûâ¨¿”Úàã£¨åß∑à•®Â◊∏ˆáÉ±Îø„˚ì«Àùòé⁄ù˝·ºŒâ’¸¿Ü
ùïÚìÁ‰‡ô†⁄©ÁÖ„√ùúÌÕƒ«˛å˝™Óü⁄˚Ïáﬂ∑öùøú∏≥≥Œ‡Ñ˙ˇË‰˙˙£”ùŸ·Öëœ„πÖº‚äÓˆ∞“±ôèå≠ÓÁΩ¥¸ª
˛≥∏≥√ëÛﬁ‡ëˆº∆ˆàÓ◊åË•ëø¥ìúª≥Ω≥≤ë≤˛ÖÚŒƒÕ¶»‘îËöõâÄÕíÙÉã≤‰é‰çáå≠í≈˚®ß†ﬂà±Ø∆˝é‘Ã˙π÷ù
ªõÏ„€∂ãàÇ•òá∏¢¡ñØêµ¿©√é∫©úàãí•óíΩ’≥ÌêÈÇãÁÙ“”…≈ï°ƒ‚®üÜùà◊Ù“°„ÿ—‰¡…˙•â«†Æ¢√öÚ´ ≠
¿®ªáËØÁ¬±ÇÕ æÇ˚ˇ≈ê¬¡≈ïè˜Ò¸≈èΩº‘¢Û¢’®˜ÎÈ∫≈∏º¨ûﬁ©¿Î˝”Í®ô¥∏¸ºÆÆèã˚·¿¯ô∏øæ◊†§√≈ËûÁóß
ˇƒ˝´˘√´Âˇ”´’””˙Ò“·ç≤˛ü◊«®û˙ı©™¯¢è„ÚåÀÜäñ¡Œ’Ö¿÷π”ÙÙŸ–ı≈Ω¥ç◊˜¬æó›˜˚È∞óÔ¿≤˘ﬂÀ˝áË⁄
ù±ÌÖ¶Âëù±◊ˆ†Öæﬂãç˝¡Ëé¬˜íø˙≥ÿü•˛ÇáûíÍﬁ¨ÎûÉ•¿Çá•ÙúÄ”—ƒ∆°ôÜúÉ°™˛âíÒæ˚∏Á‡ÆâáÃ∂®Ä©¡äπ
©∏áä·–¿π£ﬂ¬€≈Ôò≠ﬁÚ∫»©º†≥ò°ÉäÄÎƒœ‹ˆ‘ƒôû°ÿÌÒ¯ò‡¡åÛ≤≤ÑâÅíµÿÊ‘ÇÆœà∏≈¨Ë¸ÛòÇıœÛâõ¢ªì¯
¶◊∫≥ëü˚≥”Ùπ˝∫Ìø´◊·ø≥ù†Ÿ–ìá£Ó¸∞√∂¯ûÑ±∂¸±ÈÉéë†í¥¿ÙñìÑõœÌ·†â©†¯ÈÖ˜àˆ¥Ú„±¢¡µ†à‚∂ƒÙú
Ã¡å“…«∑Ô·ÿËåÍÚ”Å°è÷¥ÕÔõ¶°‡ÑûÜπˆ‹Ω“öÌº√¸òı˙ä‘≈ÑÈê±÷©≈ÖØ—ÌäÁÇ÷œ¬˜ë÷Áƒ–ıÄ£à∏Áªåõı¬î
⁄•ï Üºü–»ãÂµïµÙ÷’“πÇ™⁄€¬†êÊ®™±§ñ•ﬂ£ Ω”ÙÙ˜÷”˘˚“ééË€∫ÏË»„âóç•Íä‘ã˚ïœÍîö©ﬁÚ√ß™ûÆ“È
ﬁﬁæ‹œÓüÁîœœÀ◊Ø÷Ô‹ﬁ´Øﬂ¨ﬂΩ‘ﬁΩæ⁄‹øÕèÅá˜¿ÎÜÕ˚Ãá¬•≥”ÉÙ¸ìƒïÂÓû™øæ⁄¬˜Ω˛Ìµ˛ı∏˛ı∞∞˙Ç«úÁßŒ
ıÓÎ±≠˘¶‰ÖÎÇ—î¸€äÚùÇÎã¶ŒÈœ∫†öäÓõ¶«Ôÿ√ﬁ∏ùƒµÀ…æúÂÛ¡Ê∫Ó≈˝ßÙùõ–µ¸€≠úıá≤¨≠º˘Åá˛¿˜»ç¬ˇ
âπ°Ç›ŸÏè∂¬”Ûı√’çÇ€’å–÷ÙŸä±ø“£⁄õŸ¡éíß∆ÅïáæÙñÄ∞¯∂ñí∫†ë˙à öùÙÀÎö´ê√äöˆÜ•«Ù£∑ì°ÛÀç≤
ó∆Á’≠Ø£˝ê£«˘ù·€Ó≠π¸È¨¢„°ΩüÑ¶¥¶°Û¸õƒÎ≠—∏Êì±ø√ü€öß“æ€·ø∑æõıÍ√¥Ú≥£√ú¬¥ÒÇ·π»óÓÿ âﬂ
∫Õá∏‚∂ÓËö∫Ãµ‹πâöΩÉ˙æ©À–´•ΩÏÏ¢Ó≥èí£∑ò˘Âò®Û¯ÚÚÏáåòãÛú∏≠˙ùœÜî±õ†–≤≈éåè”¢Œ…º§√§–ˇ‰‰
—Ç Àê—ÌÉ≥ﬁ∞ÓˆÿÕáÇ∑¢Ù⁄“±™Ñø¡Üπ™—ë√¶Ë∆Ùáëöñ∑Ô¸•¨ÖˇÍÖ ñ∫’íêË—“÷ÆΩçü’ÂŒË§πÒ˘ıØúπÚ¿Ê
»˚∂≈°áÛÀ°Ô⁄µº”ƒŸ’âçÙÆ†à≤°√™à‚·Ò”ˆ∞ˇ∏«‹¨ﬂÁó´¸æ»ß®¯ıŸ˙Ê§îù†–ÄéÚˇÜƒ©ºª‘∂Ç∂Ó⁄≈ΩºÏ∫
Ø¸úÎÏ˙ôﬂ«ŒÛÀÈÜΩ¸‡ÈØüªÈﬂàÄ˝•ˆïË¬Ï÷ °˝Öê≈Œó÷˘≥£Íí¥Ú¡ıç˚Ë¯Û‰ﬁº˜í¸Ô°Îï˙∆Ñûª©«˘™ÜéüêÍ
Í÷¨€°Í·èù√¡˜€Ï—˛√⁄õ†¿ç‡ΩåÁª¥®◊«Íﬂ®È≤¶™Ü†ËÎÑ”∑Ã’“Œ’áäÄ‰Öù•§®°Óé·Ã≈â∆´¿√›«Ì˜Ωú¿√ÆÜ
ö˛˛∆É∏â¢“ÔüÕ—ﬁí∂¬ø◊ÅÑŸ∞∑„ó¡ÔÔ∫æÌÙë®Øª¸ÆñâÃ€Ç»⁄¢Ìû–ﬁ∞∏Ñˇí˜àè˝ô˙‹˛Æˇﬂ˛∑Í¯¨‘àË√≤§·Ë
ˆ¶Å»ùçÒ¬º≠ΩÍ∑·ûú£˙ÛÇâ¢Ä˚√∂˙∂˜¿åì—»ê–å”ì≈¥§™Ü•≤≈¥Œ¿ì—„ﬂà ÇÒÄ¶Øê¡πèÒÜï±Ë®âìÂå≈π˝¿á
ÚÅöËòÏÄ¨´Å≈√ØﬁîÙ÷Ÿ“≈˚Üº·´‘êÄäòˆ˙¬„∑‰†√Ôœûåπ°Ë¥ÜÖîå∫»»‰óêÙƒ∆§ôàÜÄÃ∞§à—‡îà‡Ü˛éÙÎÕû
ÉÈéú™»˝ë∏é◊ûÅÍ¡âÎ¿˛ﬁ‹ﬂ≈˛»™ˆó±Ωãı”ëØ–ı°°–œË∏∫ÂˇÅ•Ì‡æ†¬¿áãä¯æ ◊˚ôÑ∏ÒÖ©ú•”¡÷∏Ù¸˛œ˜∑
‡Í˜√’£ËÎÆöÙç®ÛÆö∏¿òƒ√ï–â√á¯¿˘√á≠ª˙±ÔÔ¿Ìÿøè∑ˇá÷ÑÜŒ˛–»˝≠ıãï‘éö¿î¬ÀìÉ†∏∫„Î´åÆ≤™Ãâ≤
Ô≈õÇÍ´ˆÿÛ«∆º‘‚æÉí∫“Í ëÕíΩî‹—ÒÂ≠Ö¿õÄ„ﬁÒŸ◊¡¿Ω∫¸µÇ™£ê†ŒË¨â›ﬁÜº€õù‡“¬îï’÷‰Ÿ†ê’À∂ù’ˆ
Ó‘◊”Ì–Ï£◊ﬁ£Óù∫Ùî≤ËΩ¿’¸¿Çâ»∏ù˙”√ÏÊÇ«∫ùîÉæ≈ƒå·Ä◊†€ò≤π‹∏Ê¶à’Ù≠ø∞‘êø°˝’¿Ù£Óá—‡«ı†ÙÉº
æœ∫ﬁ”’û™«˚ÅÓñí˛Œﬂ≈Ùπ˝øÍüüÌˇ–ºà«◊ƒÿ≠⁄–Ìî∏Ü˙Ô‘ÅÁÿ«Ä∆ó–◊ü’¿ò⁄ÓÙ€Î‘›Ä¨¶—∞≥‚–˘Éùò§Çè
∫†¬ÅÙ’€ÜÌäÜÉÔçá˘ÕËà•Ú†”∏Ω•Ø®¯˘∂ü·øÖîüòï¥‰Ÿ—ÕÕ∂çÛ€¿Ω‰Õ‘ıéÀ⁄“ıá∫∆†ãÊπ±î™ê∆±‘àÇÄ
ÖÄÛ–ƒÜ·¬îŒ¡Ùë£≤‰‘ƒú£Íï˛ÄÈªæ ⁄ÉÓ»ù·ûª£ªÉ˚∆∏ÛÁáëòä„áÚ√ÈÈÒ¸æ§ùü‰”£¸⁄¸êè˜Ú∞“˛¢†»‚¿ﬁ˙
Ò˘ñÓËﬂ”ıÇËË˛ˇîéÁÒÒÙëØ˜“˝©É¸æÅö∏ñÅﬁˆ…◊ï˝õùÑ¡Öˆ °´≤ıŒ‰πõÂº€êÊ‹ö≥…Œ‡§√ù’‘á¢ÊÊÛö≠™Ëì
ªŸ≤‚´ä©´á¶´π˝£ú€ÔÈ‘ á∆ﬁ◊∆≤ÙˆÌ°∂π⁄ÿ•¬ïÚ’¬»’â€∆Ùûë˙ÈÄ•˛§àﬁ∫•∫ÇÌãæìÀÖ∂óõÃ™∏Û°«øîÕä
ÏÆØÖ¢í·ò≠‰ÏÑ÷˙°¿œ¨Íÿ˜ˆ¿˙µ´ÍÆƒÔÑ©Æ ™®êù ñùØ∫¢È…Óœµıá¯ª§€É‹›üòÁ⁄ˇ√π˝ø‹ÇÁ˙˝†√˘¶‘¡©√
©—«÷êôÿôìπûÖß¬ü≥¢√•∑–Ò¿Ç”ÇÖñﬁˆãˇÿ˜˚Á¢‘ÿé˙Ù·˛µø´ÒÆ›Ω≤ƒÙ±˙üø˚æÿ∆îïïÛ‰ÛËì«≤È¸µÌ†Ã«Ù
¬–’æﬂ‰êÄÔçπæñÙî©ŸîÚ†˛Ç»√æ÷î˜ÀπÿóºÂè‚©…Ω˝¿‘ı‡’ô©îòî«’Òù˝Ù‡ü¢ø±‘á∑†…“«¨Õù¨ﬁÛÚ‹Õ’Äõ
›§ˆÑÇø™Ï«Øä ·¯ÔúÒ¥≈¶Í‹»Ä¬Ôä•∫¶‹„à«Òì‡Ó‘ÿ®√îìü¢¡ÓÄª´Ô—‘¢Ì¬ÙÅâ¨çî™˝êêÊ¸µì™§ÍƒœÁ¿…
ª‘¥›ËÇñÜ˘®√◊‘°›∏ìÕ∆’ö·÷®à◊§˙—√ππàıÇ˘Ω“ëÏá—ù¬Õ“∑ŒΩú®≤É–¬¡ÜΩ±»üºà—¥•ôÆ◊˜⁄˝ºò˝˘ÁŸ√ø
Õ·¡†Í¥Ï¥£≈è∫Çˇ∫˙êº›˝ô©“¡≥É’ñÇ±Ÿ‚Ω◊Ù·≥˘ˇ˙ê ÷úÖ«”ÍØèÁèﬂœæ¯˛Ω˚êÁ»¸ÛπÓ∑’Ø©ıô¥˘–©ŸıŸ
¢Ç”’Ú„∞È∆‰ﬁõå◊√È¢ô‰ÕÄ≈íıÙººö‘’†∑©Ü´˙∏©œ«∑ÕŸ„Û°ˆ„•æ¯®ØÓÛ·Û’˙⁄≈£‹êü‰˛øı˚‚œâÔ∆ûù≠≠
∏È§æ»Ñ”Í¶›™Ì◊Ÿ†á á»Î≠ÜŸ˝—œ∞‡€œ°¨ë£±ºö§Âêø≠˚≈ÉÈºÃ †›èÍô¿ËõîÈÇ‘µ√–ˆµÏ÷à¶…ÕùÔ÷é¸ä 
€∫Õ∆Õﬁ∑›“ÚΩ€æ»ÀÒ”›‹®Ò¬òı¶ö≤º◊÷É¨ÖÔ ”ä£ÀæΩã∞∂£‘ïÎî»≈ƒ´§äî€Ì≥¿å¥∫¥√Îæ¯úàÆÈïïÙΩ‚≤ã¥
˘âª∂–∂⁄Í¥èÂ∞˛ÿŸ«Ö˜°ÚÛ√ı¯˘Éºıƒ Ã≠πÛÓ”ÙÍº◊ì§›§ˆ‘ÄÎÈ˘ëŒ€√ƒ©„∞‚ä˚ô‚⁄ ÷ÓÄ¬ ˙óŸ¥òúç“åÙ
¥ ÑÇ‘ù®¿˝Ù…áÉ¥©¬ÑìÓÒ‘„é’’Á–á»ù¢´Å‰√üÄ’∑Œã”äÃË¬ıª˙ı–Ç—¨±àà†Ú–ØÙŸäﬂëÜû˙±€’ªò¶Êπõ»ò
‰•ﬁÉÔ≠ñ¶àóÿ˛å‘®ôÂî‰û¨≥€ıÇ÷ï¿˝î›∏Àªˇ∫¬ÇÑ¶ï£˝ıı¥´ï≠˜∞¨∂É±⁄¥ºî‘Œïøöèäø˛ˇ⁄ˇ∂ŸüÆ€¨
…Ω¢Ñ¥ùΩ‡Ω˜ªªﬂà˚†à„˚èô≈¢ì†ÑƒéÜïàŒƒ—´ æ∆˛∂Êÿ∂£ÿÍ‘ç¨÷≈ı≤ôÀˇ≈ïÄãÅÏ”àö´ã£ı˘˙ÚﬂùΩ‘Áè’
ˇ≈◊Ú‚±∏¯ôíﬂÏìÛ∂òíïÀ¡¥ÌÜìæ∑ò¬¸©É›‡Ô”Û±µºπ¨ô¨ÏÿÛí–˙æƒﬁöˆìºÇüÔ™®π⁄£¶–§Øôñı†ÂÉ©ÁÂΩ¨Ô
≈á®ä◊√≤‘ÄÛƒö‰æ√≠∏…‹¥ı∑Ÿ¡ÔÍàæùÇØ≠ »©Ûõãäœï£√–⁄ÆÕÔŸÃÙñˆ‡ªí…‘èÉ◊€⁄°≥ŸÁ∫ª¡¡Ø¨æÈâﬁßŒ
¥§∫é“∞‚ƒÆåÍÕì¿¨’¯ÅŸïè™‰¶ÑÈÎ‡œ°ÚÖÔùÍ∑⁄‘µŸÓïäíÔ§„£›˚ê˘π°∫ºˆ¡ìπ‚´º∑Ù©Ü€Ú¥…áí’“ÀÉ◊ﬂÿ
€“ªΩœƒπ¸à≤âÃ’ ¡Âπã´ë©Û˙‡´• æÇ¥òÍóˇÇ⁄“çΩîÈù∞≥‚ÇÁ˛ÊΩ’õÛÎô˘±ÎÖÖ›Å¥ıõı≤„Å¿ë¥¸¢Ô¨¢∂ ⁄
•™öªÓëÙ˛ãö≤∫ ∞˜Ìˇµ„Ä‘óû™“·™‚†Ú“ïÿ€îÓñπÒ‰Û¯æΩﬂò„©“±˜¨Ø∆îÈÀ‰ıΩ€õﬁœ® ‘˜ﬂœï†úåƒÇÙÖÎ
–§µ°¬ﬁØ÷≠˝Ï’Õ◊›¬á≈∑õÕœÑ¬ˇúÖ««ãáÌÉŸΩàƒ£¬∞ª‹¨«€˚ë¯™çÚ’§·≥ö£Æö¡€£ÔÙÕßÖı®ÈŒÎà •˘Ê–ç˘
®‘î‰≥ûﬁ£ø≥ËÏ˛˛¢›áùïÕ®≠áÈ—Åùç˘—´ãò˛›å€‚Ù”≈úÔï®¸ïÀÊÆæ¶óﬂ„ãÔá¨ûèìª◊ﬁÚ≥Ω˚ÏÕÓÆ≠Èïµπ∂
ˇ∆◊Ê¯⁄∏ªÃ±¶∑∆ÙÏŒ‚ß÷Ú·˚Å„æßéüπæ Ñıˆ“‰Üß•´¶ÛÑƒÛ©⁄·ıﬁéÂÎ±ÇâÇÿƒ≈Øîó¨‘ìÖóë˚¸˚ÌÂüõÌπñâ
ìïõ¶ü¢™¶ÇãøØ—≠°¡ò¡ü´À≠®≤¸óÛ„Óˆ‰ÍÔÛ…›µπª≥ÒΩŸ˜∂∂«∑◊ùÎ´œó◊ˇå˜˝˚ÈˆÚ≤úŸöÛ–Úìª•ñèÎŸ†Ù≤
”¥˛ÿÂÊ–¢ï™Ïö©€’ÖèÍÍÍ¢ïÇ˜˙êëÜ´ÿ∞’ˆÜˆ—˝à¸ƒ¨–ùﬁ¶©ª‚∫Ì—Îã˛™ò‘¥Ÿø»∫…‰ÑÑ®ŒÇß¬¨∫ÃÏ∞∑£“ñ
„Å–ÀÜÿ°ÄÜÈÅ¿¬≥ÃÂ◊¢Ü©ô§∆î˘ôÒÆ£áùóä≤√ÙÀöµ…°øëà˜◊ìË´Ìî´ªÿŸ∏†ï–Ñ¬Ÿ’ÿÔı Æ◊éºÜçç”√£Çù
¿´î∂˝ä®©¶úß“≈¸£¿≥˘ò®ÒÉ»ŸÚÜˆÏÈçÁÊ∫º∞ÂÑåÂπÅÚ¸’ëè¬Äµø‡–‚›πﬂ‘£¬‚æ◊ÀïÀëß‘ô—…ÌÊ§»œ˜ˆÎÊ
≤ÕØÈáÎıË≠µö≠⁄·€å‘ƒãÙ⁄…Ê≠Ä÷Ã⁄¨ô‡Ì£ΩÍï≈ó¶¶ôÍÿóÆ‡¡‘ÆÂØŸÌ÷Ä¿ìËóíè¢“≥Í˘Ñ˙äﬁ¶¨Ùêﬂ£–˘¢Â
ªŸ¶ºﬂñÁèüÂ˘ó‚¸À˙È˛•æøèŒ ù˘†∫ÎŒÆÌÔëÜÈçµΩ≥Åß„ÀÏ≤∂ßàì„∫≈ú˛æÒ∑ºﬂï÷…∞†Á∞˛ ÜéãòàÑÍÖÎç†
ÿıØ˘∂¶ß≥í˙π◊—é¿–åﬁèÍ’Ë˝ˇ”æ∫ø≈Á£Ö‚¯≠“áïèÑƒ“ôñàËÑ“ Ç˙À—¶„ÎÓ∫≥ÜŒô≥≠±•ﬂ‹‹ù¸ﬂ˛˝◊œüˇΩ
É∫˘Ù≥æôØ—å˘‡˘â≠ˇ†•ïßÇÂƒáËïñﬂ˙ÿ§®áê∫ñ‰Œî´ÉìœÆÒ’·†Æáñ€»Ω¿Œ‘ä›ﬂúìÑı∑©±¢ú˘ì–ÇîæÀ›˜·Â
˜∑íÈÎ÷À”¿Ó™Û»Ï÷≤»¢¨€€‚ÒÆ¨ƒ¡‘°ˆ‚ …Ü‹«Ì›Ú¢ÖùÃÿ˙—ª©ò–≈®Œ¨∏’àﬂ≥ª—π≈≤∆Î≠‹›æí ÜÃ®⁄√´áÜ
Çè¬ñè¬Ñ¬Ò´√©Ç—⁄ï‡íà•çŒµ±£Àµ≠““≠úèôƒä÷º´¬µ¨ÃÜ¡ô˜ÜèâÎ«Ó≤Úóù°≥ÛùâÁØ®¢€¶œÚµÈ‹≠ö·Êç€Ë
Îî˚›‚ΩÍ‚ˆï‚„‘éº¶ô∫èÍå“ô÷çô˙˚âïßˆ˜ö…¿•í‘ØÈƒ¡¿Ï»¨ßˇ¨´¨◊®Ã´¡≤¬®ﬁÜ±πü∂¨ÏÊ»ëöÔîµ˙´ û‹
 ◊ö¢Í∞Ï€ÿ·•ì¯™Øœì…™ﬁ⁄ˆ‘‡ñáõ¬ä¨÷å¶ÍÏÉª›◊Áüõ˙ÏæÜÃÏìÛ≥ÌÚ∂éŒÎã≈†úÙß◊Øˆ‰÷ñ©˙¸âÊ¨ñŒ¸°Œ
ôìµ€òÛ•ÏÒ·Ÿ˛‰Àù… À¯ŸπŒâ»ªù§ƒﬁØ»Â´⁄∏¢Ç—ÂÑÂ•ùñŒÿâπ˛—Â∆ïõ®ƒ™‡ƒˇ⁄¡Ωû™°πà™™Ú™∆˜œ…‘•¡≠
¿ëôÃâƒÈ∆ò≤´àÃÂ¬ÙÖ¡¸ﬁˆ˝Û˝¨Úı˘õíàœå◊∑ø›Œ∫ì©π¬ëàÿÊõò–Æ˘◊∏Àà´ØıÚπÈπì±ßˆò£Ø¿«Ö≠á¨Ë‡›√
·Êßâ ”…á∫Çô®±¢ÑçÏ˜œˆ—˝à¸Òî–ùé°∆‰ôπ€æ≠ ˇâ‹©¬•ñù∞Ãî∏ÿÜ–øôı⁄˜êñÿ§¶Ó¥¬ŸÌﬂê…ˆˆâ¸“ÈÊ©
æ»Éöı≈ÍÎ±™ˆ¥îıì¿—®í≈ªÅ–ÍË–‡≠€™îïï≠•Í–â¢ÏÎƒú“©√±∂Áµé†Ù≈•Úñ◊±˙©˛Çƒ˜¡”∫êπÛ»âùΩ€Ë¨ìß
·êÁæçË˙çÇËø†´Â∫™ÀÜ»û€Â·˛‘•€»∞É‰Ãÿªí·Æ∂˝™ê·µœ”ïÆ¥£€ÓÃöª≈πŸ’€∆∞Í—øâ≤î⁄‹≤‘ˆãµÍÓ®Í´
êÓ™É˛ÖΩﬂ¢»‹Ì˙Ì€ﬂ—≠Ü˙ÎÄÃòƒÂ»å±◊◊„›§ ÎÇ·≈∫≤ÿ™∆Æ”®ÌÃŒ◊ÌÈ˙ÖôπÿîÁõÙ¥çÓ∆Ÿ¢Ôœª„à€˚≤±ÁÛÁ
Î·ªﬁËÆÂÕ”Ü÷≤¥ÒµÇ“≈©¸ú›∆˛·Èöπ—Â’€Ñπ‹µãıËƒîë¨ø∞’ñòÚÉÁÆñ∂»Ñ®ÃÄ“¨çπÍ—ß≥ñÓ ’ØµﬁÙõÌ∑⁄‡
—›¡ÌΩ–•»˛°¨π÷à à§Û”ƒ¥îóÕë§ÍÍÃ∆ß∑“∑øﬁ›˝˙˘ˇ˘◊ﬂˇÒÈÛÁÁﬂÆÔ∫∑‚≈‡¨≠≈‡˚±ÑìÊ»ú»–∞Ò¥ß¶⁄ü§‚
 Ø¿›ı¬Î·ˇÀ”‰Ìÿæ™ á‰«¶óöã◊ƒèçÒœ•‚ÒÍèë“•Ïª£˚ê˘˘†∫∞úàˆúÀ†——‚É˝Óç∂‚§ó∏ü©—Ÿé«ç£ÌŸ†“
„§¨Ùìµæ≈É ï–√ÆÑ¢àØˆöÒÙÏÇîåÛ◊¬Õ›´¯Ã¡ıâ∫¨±Åæ±”õéòÔıëä´ÉŸ©Ï≤‹å™ÿÅΩÄÚô¬ﬁﬁËØÙ¸ÁÓª≥¿ô
«äÖ ƒó‚ÿ†ºÒóâ‚Ô·ß·ËΩ‘ÒÅåÍﬁºµ∏ÜòÒô¬íÿöò˘Êïã∆‘ÿ»¿©¶‰Ãá£„Èô±Û˘Á∫ÎÃñïÍÙ†Äˇ◊‹Ìß¬…úåè¢
¯≤ÈÓâ†®Õ¥˝–√–ÌÈ÷Ìá˛ÿêŒÏÚÂÉ≤Íê÷£ÇÎÁ‚ªà»ìËÀ¯®∑®«—‰á —¿≠¬Ñ∞ﬁÙƒæˆ¡â™∆Ì–Î∆ÕÑπÍ≤—ùçì
ªøÔ≠∫ì©≥©ôª≥œŸà∫≠àæ∆éÃÀΩµú∆ÇÄéá˚˝éÈ∂¸«ıÍÃ…†Úù¿‰≠‘õ∑‚∏òﬁ÷ƒŸ§ø≠“ıÓÄ‚ÓÜ∏¨ãüÍ˜ÜÌÀÚﬂ‡
ƒßÊç¯˘Ó˝Ωç¿§ïï◊ÿæ⁄™·Ç°û‘Ê⁄…†≠˜“ä¥¢ßêÀöô¢™ê◊∑øﬁä¿¸‹˛˙¸˚ßÀÀÀìõÒ©ÊÊõ‹∑Âÿ˜íàöƒÑÌÚ–à±
Ê¯Ç©ì…€ãÑ≠ÈÅﬁÑØ£ªêπ ¢úŸƒ®˛È¯—…ê†¬Ù©Á£ ö¥óˇ¬Ç¡‰˝–∞ùé«áë¬î›ª°¸»˙èÅª®˜ÕÕ‹∞ü“çÜ¿”©∆í
πßÈ˘ä≠–ˆ°¢ÇÕ≤îáÍÃ˚≈®Ü¡Ñ£ı∞˛°ìüÙﬁ†∫”ü≠Æñ¥ØÊ∑ÜºŸïô ïÍµé“Ôª‘÷ÖØ£Ñ©¶ŸûµﬁäŸ„œˆû˚ˆ§∞ßë
ñ≥ãù˘‚¨“‹«ù˙ÍÜπƒ ´√Ö€ ÷‡éóàÖ±Ã˜ëÄ∂è£ÿú€ÈÇÍã¥‘ïéãÏ“˛ê¯ãñ∏‚è≥˘Áº≈ôÁ¢”•πã∞∫ﬁ™√æ⁄óç±
È”°µı∂’›¶”„Û≤±ó∏î≥äáäÍãê¸Âƒ˙ñöêã∞¡ùä◊Óâ∫Ç› ˙∫ı æ≤Öëò◊Õ•›Ûƒﬁ„∑Õƒ°ÿ–…«ı∞¨á»Â∂¨í¡úı
À¸†´“∞ïÛÆ·≥–û«ˆ¢€“«’…Î˚÷©ÜﬂåŒàÚ´© íÍêß ëÄ™∏à¢Ç—∞©◊Ë¨ìì•ÑŒŸÑìëîÚóüÌß˝Óÿıˇíñêàõ–∫
èª§÷ïÆ‹Ó©âü◊©¨ÇäÿŸÃò®„‘‘ó·ŸÉêÅ∞ÓÃÓŒÆÆˇìªÁºÇ˚∑Ö˚ã∞åƒ»ˆÁÁÄÈÅÕ™∏–≤ˇÂê∂Ñ„òÚäß≠‘˛◊ﬁ·
¥ã…éπâ¸û˛⁄êÈí∏»óèÜˇíã⁄’î™àÚé®éÔ¬˘≈•êöîÌºÑ¬Üæ˙…Ùë¨…ãƒ•¶ÔˇÖ⁄âÜæ¨’ÿÅ”ÙÌÏß∫©∆ËÓÍ≤£∏
È±≥’ı…µÂ´™“êÃ–÷—ÎÁÅ˜¬äª≠ùü≠®Ë≤∑Üñ©±î≈»≠»ôÔÂÙ€ƒÊ»¨¥ßæ±ﬁöÖ∑ãÛ˙ÊﬁØóÃõ•ıÎŸ˜çÅΩâÏŸ‚Ø
î“˚ôπ‚ªÑ∞˙÷€™·î†¸·˙Å·áô∞Ä’èïÖ∆ÄòŸ…ÍÁÍß˝ù≤Ñµ≈˝ÒòÇÛ≤Ô‹Œ∫»˜≥„º∞¶æË‹ÉéòÕÎÆ∑òåúÚóîÊ
µ…Ù◊⁄Ñ÷ºÍ¥ÁÚÖë ™∏Æ‚Ûë‡≤Éé¨ﬂ»ü‡ù‰®˜™»·Üô£öè·π±π‘¶≠∑›‹∑∞¡∫≤˚àåÅ€˜¨Ì˘Ü£∫ä‘ãû“‡‹º¥ü•
ÖΩ†ÓÆ‘åÌô‚´˝Ù¨…‡©Õ¢ı⁄≠‹á§áÇë·Úéîï†ÇÈ¿Ú∫·†åüáŸÃ≠à©ú¸Í¢Î¶óÃ˜¸ΩâﬁâÑÓ÷·›õ“•†›‰Ωä”°é∞
’Í¢ªïÒ¿ªΩ“õ´Ô±…⁄ï∆ÍÒô±ß±ëƒµØÁ÷Ï¢‹Ç·ﬁﬁö∑∑˚∆ıÌÁÍÊ‰¢Ê¨◊üƒπ‹Ωπ∂úùµÀ˘ˆê©Íæù†à‘É’Úö˚£ 
€ïﬁ·Ω€¿∑ËÀ⁄Ìˆ¡‘ˆ≠Ú©ƒ° ∆ÆÆ’¢’«˜°√Ú”¿ı¯Øìâ•°∂’ï˛√Èå‘ÿûÆ‘íÒ¥ŒÍ’¶™¬È·ê§Ì£§ôΩ≠èµÿùí‘
¢ΩÖÒä”‘°üÃ’…Ã ¸‘Ωã¿ÂàÖ∂å⁄∂œªÛ¸‰ûø„∏◊›ë‡’À«Ø˜∂¨∫Ûµ¨≥çª„∞≥íâçÈ®∫‡ÕÃÉÎÏô˜À„∏˙Ó‹Œåœ”
π„üì„˜ôÒ¸π«…π€¿ö¢ƒ÷ÃÙ§∑˝€ô˙ñˆ≥∫î§¢ê©≠ÓÇü‹òãØ≥Ùªû≥ﬂÒÉ±√Ûçùœ≥¸¿äí˘Úﬂÿ”‹™Ìœ‹≤ú…ﬂä‹é
ˇÁù´Ò…ı¥äÛ≥∞Ó¸Îñ◊€ûËŒ›ïã†™∑®ÏØìÒµﬁ“≤ÖÂ⁄Ÿ◊Ä∫€–äòÆÓƒ†”ÉÀƒªŸûé∞ó¡‹‘øƒòá ‰¬ÇÕ”√»›â¬◊
˜ÛÏ•åß”Œ˝“≥±ˆÅı‹ºûÃ’πàÚﬁ≠ê˜˝ºªè—Ú”ÔÊâïºÛâÃÈ®Ø©ﬁâÒ‘°êπ»∫ëáË¬ì¥ÑÍ«“˘î¬†êÉ÷çà∑°ñ⁄ƒ®
∞ìîŸ–áÎÍä›ê˘æ©⁄¨ó∂ìÅÕ∞˘èΩ¬à›”ª†—∞Ï‘ÔúˆÎÕÆü∫Õö˙ö„Â»Æé¸€õÔõÛâ©“™¨Ωªª˚ˆŒ∫ôˇõ◊„ŒÌÃò
«äº∆Úá‹‹ÄÑë÷È™Í€˙Õ∂÷Â¢÷»Í´Œ€ïÈ∂≈üûî˚Øß∏éˇÂÎŸ√„ŸˆœÒÙ˛Àêõ®Á∑˝’˝Ôµ¡’ˆ…µØ√¡∂”€äÎê‰†®
¿éˇÇäÑÖÇåÇ¡˛ˆ–ùà∆°ÑÏ√ı†Ñöê≤è’ÅúõÊÏÑúÇ’ÅòúõÏÛÑúà†˚äπû¯™ò¶°á√Ó¬éáπ™É°á√û¬ªéÑﬂˆ¯Ë´
Æ¢‰áÜΩÖúêâ‘á¬ﬁÖ˚∏äπÜˆ•ÅÌ«ûê∑·œ°«˛Ä’Åê˜¬ûé¬˛ˆº®çÏ◊ˆÑºçªâ˘—€√ı†‰œ¿åÓ¬˛ˆ–ùà›˘ì∞õê£Å
çÍ¿»ª‡∑°Ω«É’Åê√·Ù∑°«É’Åêë√·∑°«É€‹ª°á√˛¬é‚á∂ˆ¬éá˝±ÖúèÏÌÑú‚é˚ãπûÿ⁄àâπúˆóÚº£∞µìÚ∏ÏØ
°‰¯‡Îß‰Ωÿﬂ»Ò¿÷Œˇ»·∞øê„Åè≠ùê√·ˇ°·«É€ª°á√ï˛¬éá∂ˆ¬ééá˝ÖúèÏìÌÑúé˚ãπ∆™ÿ⁄âπê™òÉπ™ÿ⁄âπ
‚ê™Éπ™ÿ‡ÄâπúˆñÚºÄ–ùàπúˆñÄÚº–ùàπú±ˆóÚº∞µìîÚ∏ÏØ‰¯‡‘Îß‰ÿﬂ»˜Ò¿÷Œ»·∞ˇøê„Å≠ùêë√·ˇ°«É€
‹ª°á√˛¬é‚á∂ˆ¬éá˝±ÖúèÏÌÑú‚é˚ãπûÿ⁄àâπúˆóÚº£∞µìÚ∏ÏØ°‰¯‡Îß‰Ωÿﬂ»Ò¿÷Œêà°‰ü™É°´Ñê˝®éÑê
Ü¬ˆ°∫ê¬à∑ŸáÍ¿éçﬂæﬁ·£ØãêüãˆçÚÑ∞Õ™ìÚ›®é‰∏™·≥ß‰Ì®éô‰Ë˘£Âøø”¥ﬂ¡ﬁå«Ωãˆ¸ó˚ó£⁄ó‰ßÄÕîú≥
ıﬂïàóÖ‡≈‡◊¯€óﬁ˚˛©ÌÎ†‰É°∫°ê£ÁﬂÖ˛∞µ‡ü¯ä˚œˇèÌ«˝óÂü§‡ø†‘á¬ﬁËÚÇØΩÙ††ºà≠êØæÇ°øúéÏñÑº◊†
œﬁòˇˇÈèÇ…êüç∂“¬˛¬≈Å†‰è˙µÌÑË˛˜ÇÒ⁄Ôãºˇ⁄â˘Ô∏√ı†‰ô˜ä£ƒøﬁŸƒÿˇ©˛°ø≤≥íü¿ä∂»¬à°˚∞–ùà°ÑÏ√
‘ı†Ñê≤è’âÅê¬»æ‘áà¬à°˚–ùà∆°ÑÏ√ı†Ñöê≤è’Åê¬Å»æ‘á¬à°°Ñê¬à°Ñê»¬à°Ñ¸óÆË¶ê¬»±Û∆áÈéﬂïà™à
÷°Ñ¸≠ŸóÖŒø¸ÊØñàÍºã°Ñ¸òº∏≤æøÀ†˛äã∫†ÑêÚ±º±ò∏øøÙáÔﬁ˘Ùµﬁ˚Âã—ı™˝ı˘Ò…Ñÿê¬æê√Ö‚Æçá‚ç«ﬂ˛
«Ëª◊¿à°‰µ£¯óáÒ⁄ﬂñùÚÃ√˛ë°õÑêüáø¬†Öˆπüë¬à≠˘Ò¯´Ωà˜Å˝£¬à°øîæØéœﬂ˛íÇ¯¡à°«…∑ÅŸ†óˇÛöúº
£¯õóü˝ÌıÇﬁ˚ì¬à°ø∆éø¸ßÔºÙÎ∂ªÒ¯ÀΩ¯ß˚ˇﬁõ¢Ñê£ÚÒÏßÑêÅÚ¢îà¬à°õﬂÚ∆–âê¬Ñà°Ñê¬à°°Ñê¬à°Ñê
à¬à°Ñê¬à¬°Ñê¬à°Ñêê¬à°Ñê¬Ñà°Ñê¬à°°Ñê¬à°Ñêà¬à°Ñê¬à¬°Ñê¬à°Ñê¬à°Ñêˇö•ˇÜ§Ó∑‰
&]
[s3; &]
[s4;%- &]
[s2;l0;%- [_^Color^ Color]_[* RandomColor]()&]
[s2; Returns a random color.&]
[s3; &]
[s4;%- &]
[s5;:GetPixel`(Image`&`,int`,int`):%- [@(0.0.255) const]_[_^RGBA^ RGBA]_`*[* GetPixel](Imag
e_`&[*@3 img], [@(0.0.255) int]_[*@3 x], [@(0.0.255) int]_[*@3 y])&]
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
[s4;%- &]
[s5;:Rotate180`(const Image`&`):%- Image_[* Rotate180]([@(0.0.255) const]_Image[@(0.0.255) `&
]_[*@3 img])&]
[s2; Returns image [%-*@3 img].rotated 180¬∫.&]
[s3; &]
[s4; &]
[s5;:GetRect`(const Image`&`,const Rect`&`):%- [_^Image^ Image]_[* GetRect]([@(0.0.255) con
st]_[_^Image^ Image][@(0.0.255) `&]_[*@3 orig], [@(0.0.255) const]_[_^Rect^ Rect]_`&[*@3 r])&]
[s2; Returns a subimage inside [%-*@3 orig] defined by rectangle [%-*@3 r].&]
[s3; &]
[ {{10000@1 [s0; [* Other functions]]}}&]
[s4; &]
[s5;:DayOfYear`(Date`):%- [@(0.0.255) int]_[* DayOfYear]([_^Date^ Date]_[*@3 d])&]
[s2; Returns the number of days from 1st of January to [%-*@3 d].&]
[s3; &]
[s4; &]
[s5;:GetExtExecutable`(String`):%- [_^String^ String]_[* GetExtExecutable]([_^String^ Strin
g]_[*@3 ext])&]
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
[s3;%- &]
[s4;%- &]
[s5;:Chdir`(const String`&`):%- [@(0.0.255) bool]_[* Chdir]_([@(0.0.255) const]_[_^String^ St
ring]_`&[*@3 folder])&]
[s2; Sets the process`'s working directory to [%-*@3 folder].&]
[s0;l288; Returns true if successful.&]
[s3; &]
[s4;%- &]
[s5;:GetDesktopManagerNew`(`):%- [_^String^ String]_[* GetDesktopManagerNew]()&]
[s2; A more complete version of GetDesktopManager() based on GetOsInfo().&]
[s0; ]