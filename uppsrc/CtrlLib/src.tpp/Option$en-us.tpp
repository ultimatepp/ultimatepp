topic "Option";
[2 $$0,0#00000000000000000000000000000000:Default]
[i448;a25;kKO9;*@(64)2 $$1,0#37138531426314131252341829483380:class]
[l288;2 $$2,0#27521748481378242620020725143825:desc]
[a83;*R6 $$3,0#31310162474203024125188417583966:caption]
[l288;i1121;b17;O9;~~~.1408;2 $$4,0#10431211400427159095818037425705:param]
[i448;a25;kKO9;*@(64)2 $$5,0#37138531426314131252341829483370:item]
[*+117 $$6,6#14700283458701402223321329925657:header]
[{_}%EN-US 
[s3; Option&]
[s5;K%- [@(0.0.255) class]_[@0 Option]&]
[s2; &]
[s0; 
@@image:712&131
€ƒò€•€€€ƒëÿ€€€€ø·œí˜íŠƒÀöŒ†½Óï´ç©ØÓ®Ã¢‚¬ğ¢â‘ÙÊÒŸØ‰•›Ìäâ‚Ø»”’Şä¾¹Ôä“üğ¼¯„³Ëåò¹
÷ĞéôáÑ¿ÇùŞ‚âßè¡í„ï´¦éÚ¶Çı¹Û°ŒÎõ¥‰Ô§Ì£ÔÁ«Ç»®¼úùƒúú‚ÉÇÛİæƒ±€†€ëºèŞ§êõÙ¥¨·–ùÚù–—Ñ
İÿÒ Æ©Ê–È—Â‡¤Äõ„œÇ§ÚÀ•ø¸‚½àÈ‚ ŠÑçù¶Óå½ÖÁ£«òæ¶­Âº€Òùä»ºå´©¿Ì¥Šæ…ª¤¨Ñ¥µ°ë¤Û©²­
½åôúŠä ¼ú˜Ú¼¤‚´ê¿«Ğ¥°ÀÔ¤ÁáƒÏß´å‚ÜµĞÍİœÑ¥ªÓ¨å¥Š¤®›¤ÂµåÚÖÒ•…¿¢ÏÎ‹š‘ª¥¢„±«ß¨Êµõ
˜–£Õİ‹ÎÕä¡©©öİ×÷²½Ô»Ãú‡öÎ´Õ­¹êÀ¼²®£šÍÕ÷ºèÒø„ô÷Š¶¿ƒÈ­õºÊ–ÒªŠ£¡ÁÉ­¹åá¨ƒÚìÌÙª€ôš
ô¦»Õ²ÙëèäıõÜÿ€–´—Æ¯ìò¹Ü®ƒ×¢›«¤¬Ü
&]
[s2; &]
[s0; Widget providing the selection of 2 or alternatively 3 states 
(true, false, Null). Value of Option is either 0, 1, or Null. 
When setting Value to Option, string `"1`" is interpreted as 
true state, other non`-Null strings as false. Depending on whether 
Option is in NotNull mode, Null is interpreted as false or Null 
state. 0 is always interpreted as false, Null number as either 
Null (NotNull mode) or false (NoNotNull mode), other numbers 
as true.&]
[s0; &]
[s0;%- [%%/ Derived from][%%  ]Pusher&]
[s0;3 &]
[s0;:`:`:Option`:`:Option`(`):%- [* Option]()&]
[s2; Initializes Option into NotNull, 2`-state mode, false value 
and standard appearance.&]
[s0;3 &]
[s5;K%- [@(0.0.255) void]_[@0 Set]([@(0.0.255) int]_[@3 b])&]
[s2; Sets Option to the specified state (0, 1 or Null).&]
[s4; [%-*C@3 b]-|State.&]
[s0;3 &]
[s5;K%- [@(0.0.255) int]_[@0 Get]()_[@(0.0.255) const]&]
[s4; [*/ Return value]-|Current state of Option.&]
[s0;3 &]
[s5;K%- operator_[@(0.0.255) int]()_[@(0.0.255) const]&]
[s4; [*/ Return value]-|Get()&]
[s0;3 &]
[s5;K%- [@(0.0.255) void]_[@0 operator`=]([@(0.0.255) int]_[@3 b])&]
[s2; Same as Set(b).&]
[s4; [%-*C@3 b]-|State.&]
[s0;3 &]
[s5;K%- [^`:`:Option^ Option]`&_[@0 BlackEdge]([@(0.0.255) bool]_[@3 b]_`=_true)&]
[s2; Activates visual appearance suitable for placing Option on white 
background (SColorPaper), like in list, as opposed to default 
representation suitable for placing it on dialog (SColorFace).&]
[s4; [%-*C@3 b]-|true to activate.&]
[s4; [*/ Return value]-|`*this for chaining.&]
[s0;3 &]
[s5;K%- [^`:`:Option^ Option]`&_[@0 SwitchImage]([@(0.0.255) bool]_[@3 b]_`=_true)&]
[s2; Activates visual representation that paints the Option with 
the Switch appearance.&]
[s4; [%-*C@3 b]-|true to activate.&]
[s4; [*/ Return value]-|`*this for chaining.&]
[s0;3 &]
[s5;K%- [^`:`:Option^ Option]`&_[@0 ThreeState]([@(0.0.255) bool]_[@3 b]_`=_true)&]
[s2; Activates three`-state mode. Also activates NoNotNull mode.&]
[s4; [%-*C@3 b]-|true to activate.&]
[s4; [*/ Return value]-|`*this for chaining.&]
[s0;3 &]
[s5;K%- [^`:`:Option^ Option]`&_[@0 NotNull]([@(0.0.255) bool]_[@3 nn]_`=_true)&]
[s2; Activates NotNull mode `- Null Value assigned to Option (via 
SetData) is interpreted as false.&]
[s4; [%-*C@3 nn]-|true to activate.&]
[s4; [*/ Return value]-|`*this for chaining.&]
[s0;3 &]
[s5;K%- [^`:`:Option^ Option]`&_[@0 NoNotNull]()&]
[s2; Activates NoNotNull mode `- Null Value assigned to Option is 
interpreted as Null.&]
[s4; [*/ Return value]-|`*this for chaining.&]
[s0; ]