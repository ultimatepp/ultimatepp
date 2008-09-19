topic "ProgressIndicator";
[2 $$0,0#00000000000000000000000000000000:Default]
[i448;a25;kKO9;*@(64)2 $$1,0#37138531426314131252341829483380:class]
[l288;2 $$2,2#27521748481378242620020725143825:desc]
[a83;*R6 $$3,0#31310162474203024125188417583966:caption]
[l288;i1121;b17;O9;~~~.1408;2 $$4,0#10431211400427159095818037425705:param]
[i448;a25;kKO9;*@(64)2 $$5,0#37138531426314131252341829483370:item]
[*+117 $$6,6#14700283458701402223321329925657:header]
[{_}%EN-US 
[s3; ProgressIndicator&]
[s0;3 &]
[s1;K:`:`:ProgressIndicator`:`:class:%- [@(0.0.255) class]_[@0 ProgressIndicator]_:_[@(0.0.255) p
ublic]_[^`:`:Ctrl^ Ctrl]&]
[s2; ProgressIndicator is a static widget used to display the progress 
of lengthy operation.&]
[s2; &]
[s2; 
@@image:2006&125
€ƒÁ”€€€€€€€€€€ø§œíšÁÊÃÀš”†ï¤è½”ß÷¥®½Á¬´Ö­¹€úŠô‘÷ì²÷’İˆĞ°Õ…­Ô²ĞÀÎ¤Âø˜‡±òÔƒ§ƒßÇ
öÇ˜ğç‘Åû¸ÉÈæ·½ß¡À¢¢¢¢¢¢şÖË­µé²ÜÌ ç—€°îò¼Àï×¸ÍâãÌÔ€Œ›©ğ–šåíŠËó×¯ÉÈàÄÈÉ£Ğ¨ê×Øªİ÷
Ğ·ùÉ»ığÂ†šÚˆŸí¨—´õ›ŠËå×¥óñò³ÓÅ÷í»ƒ´ĞÔ÷ûôÁĞªª²®ùÅ¿Öøäáì™´áıúæ ê´õ½€°”ş€•¾Øµ½Ü
ëâÕÁšèêû€á¨åÙùÚş•¬§““õ¹»Î›ëıĞÉ£´õ½€¦°”¢¨ì¿Šß¼ßŠ×Ïßƒê©şª¡´Åõ½€°”ù¿ë²û«¬¿•ô
¦û—ï»…€î¿´¡ÒÖ®­´ËãÌßıÚ‚¡€ß¢½Í“ø­Ï†Ë˜–€€ãÆšªÏÛéû«÷­Ùˆˆˆ¿ˆˆˆˆ¾ü„‰ç†‘
&]
[s2; &]
[s0; &]
[s5;K:`:`:ProgressIndicator`:`:Set`(int`,int`):%- [@(0.0.255) void]_[@0 Set]([@(0.0.255) in
t]_[@3 actual], [@(0.0.255) int]_[@3 total])&]
[s2;%- [%% Sets the progress to ][*@3 actual][%%  steps of ][*@3 total][%%  
steps. If ][*@3 total][@2  ]is zero, ProgressIndicator displays a 
bar starting at pixel position [*@3 actual] with reasonable size.&]
[s0; &]
[s5;K:`:`:ProgressIndicator`:`:Set`(int`):%- [@(0.0.255) void]_[@0 Set]([@(0.0.255) int]_[@3 a
ctual])&]
[s2;%- [%% Sets the progress to ][*@3 actual][%%  steps. Total number of 
steps is defined by previous call to ][%%* Set][%%  or ][%%* SetTotal][%% . 
If ][*@3 total][@2  ]is zero, ProgressIndicator displays a bar starting 
at pixel position [*@3 actual] with reasonable size.&]
[s0; &]
[s5;K:`:`:ProgressIndicator`:`:operator`=`(int`):%- [@(0.0.255) void]_[@0 operator`=]([@(0.0.255) i
nt]_[@3 i])&]
[s2; Same as Set([%-*@3 i]).&]
[s0; &]
[s5;K:`:`:ProgressIndicator`:`:operator`+`+`(`):%- [@(0.0.255) int]_[@0 operator`+`+]()&]
[s2; Increments actual number of steps by one.&]
[s0; &]
[s5;K:`:`:ProgressIndicator`:`:operator`+`+`(int`):%- [@(0.0.255) int]_[@0 operator`+`+](
[@(0.0.255) int])&]
[s2; Post`-increment version.&]
[s0; &]
[s5;K:`:`:ProgressIndicator`:`:operator`+`=`(int`):%- [@(0.0.255) int]_[@0 operator`+`=](
[@(0.0.255) int]_[@3 i])&]
[s2; Adds [%-*@3 i] steps to actual position.&]
[s0; &]
[s5;K:`:`:ProgressIndicator`:`:operator int`(`):%- operator_[@(0.0.255) int]()&]
[s2; Returns the current actual position.  &]
[s0; &]
[s5;K:`:`:ProgressIndicator`:`:SetTotal`(int`):%- [^`:`:ProgressIndicator^ ProgressIndi
cator]`&_[@0 SetTotal]([@(0.0.255) int]_[@3 total])&]
[s2; Sets the total number of steps.&]
[s0; &]
[s5;K:`:`:ProgressIndicator`:`:Percent`(bool`):%- [^`:`:ProgressIndicator^ ProgressIndi
cator]`&_[@0 Percent]([@(0.0.255) bool]_[@3 b]_`=_true)&]
[s5;%- [^`:`:ProgressIndicator^ ProgressIndicator]`&_[@0 NoPercent]()&]
[s2; In [* Percent] mode, number of percent is displayed in ProgressIndicator.&]
[s0; ]