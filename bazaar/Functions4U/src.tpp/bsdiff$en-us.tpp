topic "BSDiff";
[ $$0,0#00000000000000000000000000000000:Default]
[H6;0 $$1,0#05600065144404261032431302351956:begin]
[i448;a25;kKO9;2 $$2,0#37138531426314131252341829483370:codeitem]
[l288;2 $$3,0#27521748481378242620020725143825:desc]
[0 $$4,0#96390100711032703541132217272105:end]
[{_}%EN-US 
[ {{10000@(113.42.0) [s0; [*@7;4 Functions4U Reference. BSDiff]]}}&]
[s0;*@3;4 &]
[s0; [2 BSDiff  functions permit to:]&]
[s0; [2 `- Compare two binary or text files to get a new differences 
file (patch)]&]
[s0; [2 `- Create a new file based in an old version and a file with 
the differences (patch)]&]
[s0;2 &]
[s0; [2 Patch file size is related with the size of the change in original 
file compared with the file full size. Small changes let to get 
small patch files too.]&]
[s0;2 &]
[s3;l0; In arithmetical notation it would be: &]
[s0;i150;O0;%- [%%2 BSDiff does:-|][*@3;2 patchfile `= newfile `- oldfile]&]
[s0;i150;O0;%- [%%2 BSPatch does:-|][*@3;2 newfile `= oldfile `+ patchfile]&]
[s0;2 &]
[s0; [2 BSDiff is a BSD licensed binary diff/patch utility written 
by Colin Percival (Copyright 2003`-2005 Colin Percival <cperciva`@freebsd.org>), 
ported to Win32 by Andreas John <dynacore`@tesla.inka.de>.]&]
[s0;2 &]
[s0; [2 BSDiff/BSPatch functions are a light wrapper to BSDiff utilities.]&]
[s0;2 &]
[s0;2 &]
[ {{10000@1 [s0; [*2 BSDiff functions]]}}&]
[s1;2%- &]
[s2;:BSDiff`(String`,String`,String`):%- [@(0.0.255) bool]_[* BSDiff]([_^String^ String]_[*@3 o
ldfile], [_^String^ String]_[*@3 newfile], [_^String^ String]_[*@3 patchfile])&]
[s3; Takes [%-*@3 oldfile] and [%-*@3 newfile], compares them and put 
the differences in [%-*@3 patchfile].so that it is possible to 
rebuild [%-*@3 newfile].&]
[s3; In arithmetical notation it would be: &]
[s0;l448;%- [*@3;2 patchfile `= newfile `- oldfile]&]
[s0;l288; [2 Returns true if correct. Error String can be got with 
BSGetLastError()]&]
[s4;%- &]
[s1;%- &]
[s2;:BSPatch`(String`,String`,String`):%- [@(0.0.255) bool]_[* BSPatch]([_^String^ String]_
[*@3 oldfile], [_^String^ String]_[*@3 newfile], [_^String^ String]_[*@3 patchfile])&]
[s3; Takes [%-*@3 oldfile] and [%-*@3 patchfile], to rebuild [%-*@3 newfile].&]
[s3; In arithmetical notation it would be: &]
[s0;l448;%- [*@3;2 newfile `= oldfile `+ patchfile]&]
[s0;l288; [2 Returns true if correct. Error String can be got with 
BSGetLastError()]&]
[s4; &]
[s1;%- &]
[s2;:BsGetLastError`(`):%- [_^String^ String]_[* BsGetLastError]()&]
[s3; Returns the last BSDiff/BSPatch error message.&]
[s0; ]