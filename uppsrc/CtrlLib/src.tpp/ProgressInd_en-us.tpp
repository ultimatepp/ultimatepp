topic "ProgressIndicator";
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
[ {{10000@3 [s0; [*@(229)4 ProgressIndicator]]}}&]
[s3; &]
[s1;:ProgressIndicator`:`:class:%- [@(0.0.255)3 class][3 _][*3 ProgressIndicator][3 _:_][@(0.0.255)3 p
ublic][3 _][*@3;3 Ctrl]&]
[s9; ProgressIndicator is a static widget used to display the progress 
of lengthy operation.&]
[s0; &]
[s2; 
@@image:2006&125
€ƒÁ”€€€€€€€€€€ø§œíšÁÊÃÀš”†ï¤è½”ß÷¥®½Á¬´Ö­¹€úŠô‘÷ì²÷’İˆĞ°Õ…­Ô²ĞÀÎ¤Âø˜‡±òÔƒ§ƒßÇ
öÇ˜ğç‘Åû¸ÉÈæ·½ß¡À¢¢¢¢¢¢şÖË­µé²ÜÌ ç—€°îò¼Àï×¸ÍâãÌÔ€Œ›©ğ–šåíŠËó×¯ÉÈàÄÈÉ£Ğ¨ê×Øªİ÷
Ğ·ùÉ»ığÂ†šÚˆŸí¨—´õ›ŠËå×¥óñò³ÓÅ÷í»ƒ´ĞÔ÷ûôÁĞªª²®ùÅ¿Öøäáì™´áıúæ ê´õ½€°”ş€•¾Øµ½Ü
ëâÕÁšèêû€á¨åÙùÚş•¬§““õ¹»Î›ëıĞÉ£´õ½€¦°”¢¨ì¿Šß¼ßŠ×Ïßƒê©şª¡´Åõ½€°”ù¿ë²û«¬¿•ô
¦û—ï»…€î¿´¡ÒÖ®­´ËãÌßıÚ‚¡€ß¢½Í“ø­Ï†Ë˜–€€ãÆšªÏÛéû«÷­Ùˆˆˆ¿ˆˆˆˆ¾ü„‰ç†‘
&]
[s0; &]
[s0; [/ Derived from] [^topic`:`/`/CtrlCore`/src`/Ctrl`$en`-us`#`:`:Ctrl`:`:class Ctrl^ C
trl]&]
[s3; &]
[s0;%- &]
[ {{10000F(128)G(128)@1 [s0; [* Public Member List]]}}&]
[s3;%- &]
[s5;:ProgressIndicator`:`:Set`(int`,int`):%- [@(0.0.255) void]_[* Set]([@(0.0.255) int]_[*@3 a
ctual], [@(0.0.255) int]_[*@3 total])&]
[s2;%- [%% Sets the progress to ][*@3 actual][%%  steps of ][*@3 total][%%  
steps. If ][*@3 total][@2  ]is zero, ProgressIndicator displays a 
bar starting at pixel position [*@3 actual] with reasonable size.&]
[s3; &]
[s4;%- &]
[s5;:ProgressIndicator`:`:Set`(int`):%- [@(0.0.255) void]_[* Set]([@(0.0.255) int]_[*@3 `_act
ual])&]
[s2;%- [%% Sets the progress to ][%%*@3 `_][*@3 actual][%%  steps. Total 
number of steps is defined by previous call to ][%%* Set][%%  or 
][%%* SetTotal][%% . If ][*@3 total][@2  ]is zero, ProgressIndicator 
displays a bar starting at pixel position [*@3 actual] with reasonable 
size.&]
[s3; &]
[s4;%- &]
[s5;:ProgressIndicator`:`:operator`=`(int`):%- [@(0.0.255) void]_[* operator`=]([@(0.0.255) i
nt]_[*@3 i])&]
[s2; Same as [^topic`:`/`/CtrlLib`/src`/ProgressInd`$en`-us`#ProgressIndicator`:`:Set`(int`)^ S
et]([%-*@3 i]).&]
[s3; &]
[s4;%- &]
[s5;:ProgressIndicator`:`:operator`+`+`(`):%- [@(0.0.255) int]_[* operator`+`+]()&]
[s2; Increments actual number of steps by one.&]
[s3; &]
[s4;%- &]
[s5;:ProgressIndicator`:`:operator`+`+`(int`):%- [@(0.0.255) int]_[* operator`+`+]([@(0.0.255) i
nt])&]
[s2; Post`-increment version.&]
[s3; &]
[s4;%- &]
[s5;:ProgressIndicator`:`:operator`+`=`(int`):%- [@(0.0.255) int]_[* operator`+`=]([@(0.0.255) i
nt]_[*@3 i])&]
[s2; Adds [%-*@3 i] steps to actual position.&]
[s3; &]
[s4;%- &]
[s5;:ProgressIndicator`:`:Get`(`)const:%- [@(0.0.255) int]_[* Get]()_[@(0.0.255) const]&]
[s2; Returns the current actual position.  &]
[s3;%- &]
[s4;%- &]
[s5;:ProgressIndicator`:`:GetTotal`(`)const:%- [@(0.0.255) int]_[* GetTotal]()_[@(0.0.255) c
onst]&]
[s2; Returns the total number of steps (set by SetTotal).&]
[s3;%- &]
[s4;%- &]
[s5;:ProgressIndicator`:`:operator int`(`):%- [* operator_int]()&]
[s2; Returns the current actual position.  &]
[s3; &]
[s4;%- &]
[s5;:ProgressIndicator`:`:SetTotal`(int`):%- [_^ProgressIndicator^ ProgressIndicator][@(0.0.255) `&
]_[* SetTotal]([@(0.0.255) int]_[*@3 `_total])&]
[s2; Sets the total number of steps to [%-*@3 `_total].&]
[s3; &]
[s4;%- &]
[s5;:ProgressIndicator`:`:Percent`(bool`):%- [_^ProgressIndicator^ ProgressIndicator][@(0.0.255) `&
]_[* Percent]([@(0.0.255) bool]_[*@3 b]_`=_[@(0.0.255) true])&]
[s5;:ProgressIndicator`:`:NoPercent`(`):%- [_^ProgressIndicator^ ProgressIndicator][@(0.0.255) `&
]_[* NoPercent]()&]
[s2; In [* Percent] mode, number of percent is displayed in ProgressIndicator.&]
[s3; &]
[s0; ]