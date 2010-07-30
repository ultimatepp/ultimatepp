topic "";
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
[{_} 
[ {{10000@(113.42.0) [s0;%% [*@7;4 Functions4U Reference. Dll]]}}&]
[s0;i448;a25;kKO9; &]
[s0;i448;a25;kKO9; Dll class is an easy way to dinamically load DLL 
files and functions in Windows.&]
[s0;i448;a25;kKO9;@(0.0.255) &]
[ {{10000F(128)G(128)@1 [s0;%% [* Public Method List]]}}&]
[s4; &]
[s5;:Dll`:`:Load`(const String`&`): [@(0.0.255) bool]_[* Load]([@(0.0.255) const]_[_^String^ S
tring]_`&[*@3 fileDll])&]
[s2;%% Loads .dll file [%-*@3 fileDll] before loading its functions. 
It returns false if error or [%-*@3 fileDll] does not exist.&]
[s3; &]
[s4; &]
[s5;:Dll`:`:GetFunction`(const String`&`): [@(0.0.255) void]_`*[* GetFunction]([@(0.0.255) c
onst]_[_^String^ String]_`&[*@3 functionName])&]
[s2;%% It returns a pointer to [%-*@3 functionName] in DLL or NULL 
if error or [%-*@3 functionName] is not in DLL.&]
[s3; &]
[s0; &]
[s0; For example:&]
[s0; &]
[s0; [C     Dll adsapi32;-|-|// Dll class]&]
[s0; [C     long (`*DRV`_DeviceGetNumOfList)(int);-|// Pointer to function]&]
[s0; [C    ]&]
[s0; [C     if (!adsapi32.Load(AppendFileName(myDllFolder, `"adsapi32.dll`")))]&]
[s0; [C         throw Exc(t`_(`"Adsapi32 dll not found`"));]&]
[s0; [C    ]&]
[s0; [C     DRV`_DeviceGetNumOfList `= (long (`*)(int))adsapi32.GetFunction(`"DRV`_Devi
ceGetNumOfList`");]&]
[s0; [C     if (!DRV`_DeviceGetNumOfList)]&]
[s0; [C         throw Exc(Format(t`_(`"Function %s does not found in 
dll`"), `"DRV`_DeviceGetNumOfList`"));]&]
[s0; [C     long num `= DRV`_DeviceGetNumOfList(3);-|// Just use the 
function as any other!]&]
[s0;C ]