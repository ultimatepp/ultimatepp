topic "KeyInfo";
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
[{_}%EN-US 
[ {{10000@(113.42.0) [s0; [*@7;4 KeyInfo]]}}&]
[s1;@(0.0.255)3%- &]
[s1;:Upp`:`:KeyInfo:%- [@(0.0.255)3 struct][3  ][*3 KeyInfo]&]
[s2; The structure represents the underlying data associated with 
a single entry in .key files. It contains two fields: a name and 
an array of four elements used to store information about physical 
key combinations. The structure can hold up to four such combinations. 
However, only two are currently utilized.&]
[s2; &]
[s2; The following example shows how the structure is used, assuming 
the presence of the following entries in the [* MyApp.key] file:&]
[s2;l480; [1 KEY(SAVEFILE, `"Save`", K`_CTRL`_S)]&]
[s2;l480; [1 KEY2(CUTLINE, `"Cut line`", K`_CTRL`_Y, K`_CTRL`_L)]&]
[s2;~~~384;@(89) &]
[s2;~~~384; In this case, the file should be imported as follows 
in the [* .cpp] file:&]
[s0;l480;%- [C@(128.0.255)1 #define][C@(35.38.41)1  KEYGROUPNAME ][C@3;1 `"MyApp`"]&]
[s0;l480;%- [C@(128.0.255)1 #define][C@(35.38.41)1  KEYNAMESPACE MyAppKeys]&]
[s0;l480;%- [C@(128.0.255)1 #define][C@(35.38.41)1  KEYFILE      ][C@(0.0.255)1 <][C@(35.38.41)1 M
yApp][C@(0.0.255)1 /][C@(35.38.41)1 MyApp][C@(0.0.255)1 .][C@(35.38.41)1 key][C@(0.0.255)1 >
]&]
[s0;l480;%- [C@(128.0.255)1 #include][C@(35.38.41)1              ][C@(0.0.255)1 <][C@(35.38.41)1 C
trlLib][C@(0.0.255)1 /][C@(35.38.41)1 key`_source][C@(0.0.255)1 .][C@(35.38.41)1 h][C@(0.0.255)1 >
]&]
[s2;~~~384;@(89) &]
[s2;~~~384; The structure can now be used in various contexts, such 
as when creating a menu bar entry, as shown below:&]
[s2;l480;~~~384;%- [C@(35.38.41)1 menu][C@(0.0.255)1 .][C@(35.38.41)1 AddMenu(MyAppKeys`::][C1 A
K`_SAVE][C@(35.38.41)1 , ][C@(0.0.255)1 `[`=`]][C@(35.38.41)1  `{ Save(); 
`});]&]
[s2;~~~384; &]
[s2;~~~384; Typically, at the beginning of your file, you can use 
[* using namespace MyAppKeys] to simplify the line above to:&]
[s2;l480;~~~384~288;%- [*C@(0.0.255)1 using][C@(35.38.41)1  ][*C@(0.0.255)1 namespace][C@(35.38.41)1  
MyAppsKeys;]&]
[s2;l480;~~~384~288;%- [C@(35.38.41)1 ...]&]
[s2;l480;~~~384~288;%- [C@(35.38.41)1 menu][C@(0.0.255)1 .][C@(35.38.41)1 AddMenu(][C1 AK`_SA
VE][C@(35.38.41)1 , ][C@(0.0.255)1 `[`=`]][C@(35.38.41)1  `{ Save(); 
`});]&]
[s2;~~~384; &]
[s2;~~~384; In this case,[*  AK`_SAVE] is a [* KeyInfo ]structure.&]
[s2;~~~384; &]
[s2;~~~384; Please note that [* KEY] is used when only one key combination 
should be associated with a given operation, whereas [* KEY2] is 
used when two separate combinations can activate the operation.&]
[s3;%- &]
[s0;%- &]
[ {{10000F(128)G(128)@1 [s0; [* Public Member List]]}}&]
[s3; &]
[s4;%- &]
[s5;:Upp`:`:KeyInfo`:`:name:%- [@(0.0.255) const] [@(0.0.255) char] [@(0.0.255) `*][* name]&]
[s2; The name of the operation (for example, “Print”, “Open”, 
etc.).&]
[s3;%- &]
[s4;%- &]
[s5;:Upp`:`:KeyInfo`:`:key:%- [_^topic`:`/`/Core`/src`/PrimitiveDataTypes`_en`-us`#Upp`:`:dword^ d
word] [* key][@(0.0.255) `[][@3 4][@(0.0.255) `]]&]
[s2; The structure can accommodate up to four physical key shortcuts 
per operation, though only two are currently utilized. The remaining 
capacity is reserved for future use.&]
[s3;%- &]
[s0;*@7;4 ]]