topic "FileSel utilities";
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
[ {{10000@(113.42.0) [s0; [*@7;4 FileSel based utility functions]]}}&]
[s9; These functions simplify opening files using single global file`-selector. 
The configuration of file`-selector is stored into [^topic`:`/`/Core`/src`/SerializationUtils`$en`-us^ g
lobal configuration].&]
[s3; &]
[s0; &]
[ {{10000F(128)G(128)@1 [s0; [* Function List]]}}&]
[s3;%- &]
[s5;:SelectFileOpen`(const char`*`):%- [_^String^ String]_[* SelectFileOpen]([@(0.0.255) co
nst]_[@(0.0.255) char]_`*[*@3 types])&]
[s2; Selects file for opening, [%-*@3 types] follow rules of [^topic`:`/`/CtrlLib`/src`/FileSel`$en`-us`#FileSel`:`:Types`(const char`*`)^ T
ypes] method of FileSel. If selections is canceled, returns empty 
String.&]
[s3; &]
[s4; &]
[s5;:SelectFileSaveAs`(const char`*`):%- [_^String^ String]_[* SelectFileSaveAs]([@(0.0.255) c
onst]_[@(0.0.255) char]_`*[*@3 types])&]
[s2; Selects file for saving, [%-*@3 types] follow rules of [^topic`:`/`/CtrlLib`/src`/FileSel`$en`-us`#FileSel`:`:Types`(const char`*`)^ T
ypes] method of FileSel. If selections is canceled, returns empty 
String.&]
[s3; &]
[s4;%- &]
[s5;:SelectDirectory`(`):%- [_^String^ String]_[* SelectDirectory]()&]
[s2; Selects directory.&]
[s3;%- &]
[s4; &]
[s5;:SelectLoadFile`(const char`*`):%- [_^String^ String]_[* SelectLoadFile]([@(0.0.255) co
nst]_[@(0.0.255) char]_`*[*@3 types])&]
[s2; Selects file and loads it, [%-*@3 types] follow rules of [^topic`:`/`/CtrlLib`/src`/FileSel`$en`-us`#FileSel`:`:Types`(const char`*`)^ T
ypes] method of FileSel. If selections is canceled or file cannot 
be opened, returns empty String`::GetVoid().&]
[s3; &]
[s4; &]
[s5;:SelectSaveFile`(const char`*`,const String`&`):%- [@(0.0.255) bool]_[* SelectSaveFil
e]([@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 types], [@(0.0.255) const]_[_^String^ String
][@(0.0.255) `&]_[*@3 data])&]
[s2; Selects file and saves [%-*@3 data] to it, [%-*@3 types] follow 
rules of [^topic`:`/`/CtrlLib`/src`/FileSel`$en`-us`#FileSel`:`:Types`(const char`*`)^ T
ypes] method of FileSel. Returns true on success.&]
[s3;%- &]
[s0;%- &]
[s0;%- &]
[s0;%- &]
[ {{10000@(113.42.0) [s0; [*@2 SelectFileIn class]]}}&]
[s9;%- This class inherits FileIn. Its constructor invokes global 
FileSel and opens selected file for reading:&]
[s0;%- &]
[s5;:SelectFileIn`:`:SelectFileIn`(const char`*`):%- [* SelectFileIn]([@(0.0.255) const]_
[@(0.0.255) char]_`*[*@3 types])&]
[s2; [%-*@3 types] follow rules of [^topic`:`/`/CtrlLib`/src`/FileSel`$en`-us`#FileSel`:`:Types`(const char`*`)^ T
ypes] method of FileSel. In case of any failure (selection is 
canceled, open fails), the resulting object is not in opened 
state (can be tested using operator bool).&]
[s3;%- &]
[s0;%- &]
[s0;%- &]
[s0;%- &]
[ {{10000@(113.42.0) [s0; [*@2 SelectFileOut class]]}}&]
[s9;%- This class inherits FileOut. Its constructor invokes global 
FileSel and opens selected file for writing:&]
[s0;%- &]
[s5;:SelectFileOut`:`:SelectFileOut`(const char`*`):%- [* SelectFileOut]([@(0.0.255) cons
t]_[@(0.0.255) char]_`*[*@3 types])&]
[s2; [%-*@3 types] follow rules of [^topic`:`/`/CtrlLib`/src`/FileSel`$en`-us`#FileSel`:`:Types`(const char`*`)^ T
ypes] method of FileSel. In case of any failure (selection is 
canceled, open fails), the resulting object is not in opened 
state (can be tested using operator bool).&]
[s3;%- &]
[s0;%- ]]