topic "Scatter Reference";
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
[ {{10000@(113.42.0) [s0;%% [*@7;4 Scatter]]}}&]
[s3; &]
[s1;:Scatter`:`:class: [@(0.0.255)3 class][3 _][*3 Scatter][3 _:_][@(0.0.255)3 public][3 _][*@3;3 St
aticRect]&]
[s9; A class to plot 2D graphs.&]
[s0;i448;a25;kKO9;@(0.0.255) &]
[ {{10000F(128)G(128)@1 [s0;%% [* Function List]]}}&]
[s4; &]
[s5;:Scatter`:`:class: [@(0.0.255) void]_[* FitToData]([@(0.0.255) bool]_[*@3 Y]_`=_[@(0.0.255) f
alse])&]
[s0;l288;%% Changes X axis zoom to fit visible all data in Ctrl.&]
[s0;l288;i448;a25;kKO9;%% If [%-*@3 Y ]is true, Y axis zoom is also 
changed to fit the data.&]
[s3;%% &]
[s4; &]
[s5;:Scatter`:`:SaveToClipboard`(bool`): [@(0.0.255) void]_[* SaveToClipboard]([@(0.0.255) b
ool]_[*@3 saveAsMetafile]_`=_[@(0.0.255) false])&]
[s2;%% Saves graph to clipboard as a bitmap.&]
[s2;%% In windows if [%-*@3 saveAsMetafile ]is true, a metafile is 
sent to clipboard.&]
[s3;%% &]
[s4; &]
[s5;:Scatter`:`:SaveToImage`(String`): [@(0.0.255) void]_[* SaveToImage]([_^String^ String]_
[*@3 fileName]_`=_Null)&]
[s2;%% Saves graph to file [%-*@3 fileName.] Supported formats are 
.PNG and .JPG.&]
[s2;%% If [%-*@3 fileName] is null, a window is opened asking the user 
to enter a file name.&]
[s3;%% &]
[s4; &]
[s5;:Scatter`:`:SetFastViewX`(bool`): [_^Scatter^ Scatter][@(0.0.255) `&]_[* SetFastViewX](
[@(0.0.255) bool]_[*@3 set]_`=_[@(0.0.255) true])&]
[s2;%% If [%-*@3 set ]is true, it will be viewed a point per horizontal 
pixel. This point will be the average of all data that is in 
that pixel.&]
[s2;%% This speeds up the graph painting.&]
[s3;%% &]
[s4; &]
[s5;:Scatter`:`:GetTitleFont`(`): [_^Font^ Font][@(0.0.255) `&]_[* GetTitleFont]()&]
[s2;%% Returns title font.&]
[s3; &]
[s4; &]
[s5;:Scatter`:`:GetXByPoint`(const int`): [@(0.0.255) double]_[* GetXByPoint]([@(0.0.255) c
onst]_[@(0.0.255) int]_[*@3 x])&]
[s2;%% Gets the X coordinate in data units from value [%-*@3 x ]in 
screen pixels inside Ctrl.&]
[s3;%% &]
[s4; &]
[s5;:Scatter`:`:GetYByPoint`(const int`): [@(0.0.255) double]_[* GetYByPoint]([@(0.0.255) c
onst]_[@(0.0.255) int]_[*@3 y])&]
[s2;%% Gets the Y coordinate in data units from value [%-*@3 y ]in 
screen pixels inside Ctrl.&]
[s3;%% &]
[s4; &]
[s5;:Scatter`:`:GetY2ByPoint`(const int`): [@(0.0.255) double]_[* GetY2ByPoint]([@(0.0.255) c
onst]_[@(0.0.255) int]_[*@3 y])&]
[s2;%% Gets the Y2 coordinate in data units from value [%-*@3 y ]in 
screen pixels inside Ctrl.&]
[s3;%% &]
[s4; &]
[s5;:Scatter`:`:GetXPointByValue`(const double`): [@(0.0.255) double]_[* GetXPointByValue
]([@(0.0.255) const]_[@(0.0.255) double]_[*@3 x])&]
[s2;%% Gets the horizontal position in pixels  inside Ctrl for a 
value [%-*@3 x ]in data units.&]
[s3;%% &]
[s4; &]
[s5;:Scatter`:`:GetYPointByValue`(const double`): [@(0.0.255) double]_[* GetYPointByValue
]([@(0.0.255) const]_[@(0.0.255) double]_[*@3 y])&]
[s2;%% Gets the vertical position in pixels inside Ctrl for a value 
[%-*@3 y ]in data units.&]
[s3;%% &]
[s4; &]
[s5;:Scatter`:`:ShowInfo`(bool`): [_^Scatter^ Scatter][@(0.0.255) `&]_[* ShowInfo]([@(0.0.255) b
ool]_[*@3 show][@(0.0.255) `=true])&]
[s2;%% Defines to show or not with [%-*@3 show ]the pop`-up window 
that indicates in data units the cursor location and, if dragged, 
the size of the window.&]
[s3;%% &]
[s4; &]
[s5;:Scatter`:`:ShowContextMenu`(const bool`&`): [_^Scatter^ Scatter][@(0.0.255) `&]_[* Sho
wContextMenu]([@(0.0.255) const]_[@(0.0.255) bool`&]_[*@3 show][@(0.0.255) `=true])&]
[s2;%% Defines to show or not with [%-*@3 show ]the context menu with 
options to save the plot to clipboard or file. Formats supported 
are .PNG and .JPG &]
[s2;%% &]
[s2;%% 
@@image:875&375
‚ƒŒ€¼€€€€€€€€€€øœíœùÓ“çŞ˜Çó÷øÃ­Ğœíí»ÓéíùŒ³ºã‰­Ó±éØĞÀä¨ÊÜÀ¥¨­£Ôªã‚€àÁ””„ê³ ¨—®Š
º¢Èá¢¢„£ÂÎ€Â’²ÀÉöƒıÂÆšœÉ¶›°»ä·¥Îşç»»›½ıŞıŞ÷Ãó¾ôïö—¬ëç–ş™ò¡ƒ»™ºä´—†ïÅÅ…Áş¨¸°¸ó
ÏæÌ™Í®¯ÉëáìÍ“ã´ÊÉÛ‚Ö¦±çÁà˜ùÓœ£öÅ‹ãÄşÑäÔ¸®›—¿˜Êî·˜È®ŸË®Ó€æÕ«îÕå¸¼È¯”§½ëüÚŞ÷¯ú
„­œÃìıİëĞÚ³ª‘õÜñ¸æñÊ§Ñ¹ëæ·Ä¨ õ²ÒÜ¹Ïìô²Š©³õôõÁë¢Ë¥ƒò™ê”Ú˜¶Ê¨Ç¦²áÖÿ—ö²ÛÇß—´öÅ
ÀùÑËóàšÕ›ƒÔÚ£ıìú÷ıØÕ‚íîÍ®„×ÅãŞùûŠĞÚÙ™€·“ä“ôú„›æôÔ‰Ë“©†½éíÏ¯ÒöˆŠ†„¢„¯‚şø¼÷üû
ìïâï¼¾•ÓÂûòö×ôæçÄŸùíµíƒ®ÏÂàÙÉËµÇÛ‰áÿ¯ºæáÂÉ€ÅÑ”´ËöçÛ·”–æÙ÷ô¿‡îÎ»‹¤Ú˜£º«£î”Ãşª˜Î¨
´×‘ûÔÚ¿®ÒêÄ˜—»˜ÜşïÁİîù‰¤…­½õ³­¢€ÑŞ÷×›‰Æ‚ÙµŠØâ–Ë–âñÛ‡ßïÓÅ«²ş•œÒ­Ê¹ÑìÏ¤Èú™÷ìçœÂÍ
Ş¯·È¨‹¢£œÛÕƒ°ÄÖÖš„¢Á·×ºö£µÉù£îóº£·¿§ÛÈË•Ş£ûõõ¾Ç†¨íÑ‡ØÚÖÄ×“„òØ±¢¨˜âÂõˆªÙŞ¶ÍëÙÅÃ
ŸôÜòÄ¬Î±Ï­…½ÁÉÎúæ„»‹œÌ¼áä°î¿â¤ø²ŠÑ‘´öÚòòåÒŒÒÅáïì«°Ï€¥‚™ÃÌ¨æ°Û¨±­™Ğ…½ñæÎ‹Ìê•Ú
´€ûÜªÃëšŠŸääáÇÚòÄ‘ÑÑíÛëŞ¥²«Â³÷³ûà£ê±˜­†–˜æ­Ã¶™´Ñ‚ê—ÑÅµÙ­µÂŞßş¯ÖàÆàÜÈÓ÷ìôÍ·ˆ
Õ‡şÏ™áî¡¾¨ŠŒÖÅ¿«˜Ûİ›˜À¶ÒŞÆ€…¢ˆ™µ»¦•‘…½‡…Ÿ•®ÂÀ’½Ï™Ö·ÓÕ”éÎ“æÜÌ“ËÄ¡ıîÍ‹£›“ÿØ¿¨Ú¾
Ôâ²¯á²™Á€°ƒêå””ß¨ÎºÎ·İâ§Š¬Šó†õÅç¾ô¸½ÖşØÒÙÒñòŒ¨³º“ş ½¢µ­´ñ¹¹¤ÉòÛååúßÎ¿¹œøïøç„‡¿©
òˆŠ—Ÿä—îˆ¿İŠÇ°ÒÇÈß—İŒ¸í„¸Æì—İó†êÎ›§‘ûÅ„±â¿¨²ªì˜šÍ‘Ë¤÷ëîˆëİÓôîÅ”ÄêÈ«·èÇ·şóûÓ×
ª±’ŸãÚÂÒøº›û¦˜­©ßïŠâœáÊá¦ûÙ¨Á¶Š˜¡Ñ•„İ±˜¬ášåìÖã×¢°˜­óËïèÈŒ±ÙªÖÌï©ü®ä¶¹²¾ªŠŒ‚®
ãµËÆŒÿ–ÍÒ˜Ì«†³¥ô–ÆÇ‚İÂÉµ†Œ¨­™°²±µéğÚØ ¬İè’ª÷ğ›ß™­¬Š¢£ëïù­Ò‘ÃÅä’•à€ƒ£™Ö¥ó‚…Œ‚
Ø¼ÒéÌÃªãáà”¢ªÛú¸–ƒÏàÜ÷æÈĞ÷ÿØû×ñ‚¢£˜ƒº‡…—¯–Ìƒğ¼øÊçŒÆ¬³ÚŒæ•Å½¶Ø²ƒÉ„È§‘¼ó…£¥Ï¦ƒ
°£êñã ÷åğ†‹¡¾µÎÔªÑœ³ÀŒ¡ÈóÈÿ¡øÚ©¶ÇçÍ‹úÊŒ’È‡Ø¦çùÍ¶Àı“†³ı÷•¢æ‰€ã›ÕÎĞŞ²¤ ˜íÒ¤ô‰º
»£—òı…¢©Ëèåàòö¼µä³º£«¦£ëÎÄÚ«°›¡ Ÿ÷÷ ¿Áè™Şåœ€«Ã›¥îßİğ•†úÄÏ¸ğó™È£š“š–Ñ°å”©ùÑ¯Í
Ú”‹„ã‚Ñ‚ø¢è¬ÑˆÖ˜¦”¨ÏŞŸ¹ï“¯ˆË›½Ğ»˜÷ïèÎÍ”ğÀØãÜ˜ˆ§ÚÄÕƒ¾‚ñ¿ÄÃ§®Êä¹áˆÅÑ³¾å­•ğüÎ
Ğ˜à‚ÀçŒšÂÿ–Ÿ§€ºÓüÜş®¼«ŠòÀ¿ô‘ñ°´„ÏôÜı©“ú¿õ£ÇˆŞñ¯‘ºˆçŒ‡ñîöøãß¶Å•Ñó›À¸õ¶æ¥­‰¬€
™ò•çò‡Ğæì®üôÎÿ©¯œ÷©â¾ÿã–•ÄÁü¥»ˆ³¢®„ø÷‡—óÕÙ¢ûÆçûöìŒº°ËÁ™‹áµÈ¤—‹‚‚±ø¶Ä×à˜Ûï¯•¿
áñ»ª¦†¸Í·¡İøÆÿæ¾ÊÜû«î¨ÉÓŞÅèÜ’óÊòëÑÜíÃ—¥ÈÛ¯¯´ˆûıÂÊœƒ£ÀíØÜíÄ½İÑÕ™Ä‡‘Œ®ñ¬êèÖ©³
ÇŠ–Ôö¯ˆÖ˜½›êÁµ°ıŠÈë¹”×í„—ÒÂìåıÔ‚”çŒßÆµÔÓ½š—”Œ’çüÑØÊ«ó¹ÿÛ‚‹™­¬­Š¢£½—ÚªÓÚçªŞÌ’
ÆÇçïô¤ä¿½ü®ÊÍœ“®¨¿§Úÿ²ƒ³ç‡şãàÄğİàÄÍÀñêë€ğ§şá•ş½¡ı•û’­ó‹©İ–Ü„‚™ôíÄÙğ¸’÷Ï•Œ¸ç
Úüì—÷Î‰ûÉÅ±â¿¸Æì˜—éÆœ½é’Øó†‡Ï¨Æ¤ÿÒŠò¢¬Ìç›×®¥ÁŸ€Óù¼û³›¦Œ‰ÿ«÷°úÀîüÀ®ëé®ñƒòŠ‹óø
ˆ§à±çïæÍƒ¡ñûŒÖ€¬ÎÁÂ‰€÷º§û¹¤¤¯åûçÛÚÌğø¼¤şçÄÁôÀ–Î…Ä‰˜Ğ­Ş·ç§ˆİÅíÔË¼¾›Â°÷ş½»
â·úûÁ†³Àğº˜Æ´Ú€œôà—İÅù†îŒÇƒ³†ÜîëØèƒ´îÉöş¯¥¯òèøìˆƒ¥­Á³Â¨ğ±§ñ°Ø’†Ó¶ıëÚ±ãáöÿâ
‚¾Éú
&]
[s2;%% &]
[s2;%% If SetMouseHandling() is true in X and/or Y axis, scroll and 
zoom are also included.&]
[s2;%% &]
[s2;%% 
@@image:881&1237
Šƒ€Æ€€€€€€€€€€øœíùÛ”ÇÂšÇ§ÏşÊş‡ü°¿¦ÏÌúÄİ•Üì‘ìÅš¼›ıö·¯ºú¸›Ä’‹ã±±â‘¡±™ú ˆ¨ˆ¨„
ºÑÀğÉé…ÂŒ„„£ÆÈ¼‚ÈÑ’À®á¸†ÔÃÁ™˜™èâı‡‰ãçº«÷»§»úº«©ïÓÏ÷×õŞÍÕû™ªú»‘ªú˜›í·±§¦±Ñş©Á„
¼ÉÍŒÄÄ¢ÙêáÉçö„‰˜µğš–âòÛâ¢â˜°¶¢ŸìÂµ¯»ÁÁ˜“°òã®Ûò¶¦¹™ã‰ªæï×­æ‚˜°üá©ì¦¬™óâÂØñÑ‰—Æá§Ì
¨æÌìÓ•å…§¡Áº¦ö©€›™ªÀ‰â€¢Â”“‘Âá£šö”Ì‚¤¨¡¦ÀÊ”“Š”ÙÅ”Èµû­üíƒı–¡ÁîŸ‘áŞÑ‘¾®ş¦ŞÀÆ”œœ
øœ“““ššŠÿú”„„„ĞĞÿĞˆˆˆ€ÔÉù€‚ë¿õæŸ÷şş··şõâ·”¯¦ÑÖğ”››ûÿşıæıûö¬éÓ»Ğ•û¡«ú¼ì×ô™®¨¢
¤Ëä¡åÏ¾å£Íšµ·îÜØ±â…†µ¥Úşàíâ¢Üêª¿²¦Æºş¾øèĞ²Æ¦´´ÿ´ÈÈÈı‡’äóîôçßëÏ·ºÖŸùµïÔÀõßÆÕßú¥
õõä…õÃÖİÚË–¬Ù¹ó§€¦õ´Á½ˆ—ŒØ»éØá Õ€Ûû‹ú¿¯¦ÃÑà³¶Á‹Ê›¹îÔÿÕõÑß©€ô¥·­íÂú¢Üúüëù¯Ë»·§µ
»¢Êàúµ˜™·Ãòš×†ÍÑ•·„¬—î—•““ñ÷ÎØëÏëğ“«Œ¼ÌĞËåş³‹İˆ¼€ªÿí”ÒÚü©ûœØÉ×–ÔæÔº™ü²ã¬Å”
Ü““™şçÉöõ¥˜à˜ò©ôò€èä¹µÛõ²µ¥¬»¹¿´ûØÅ®£¥…ÇÎ÷îÊî†Â•ë®ïë£ò¶¯ö”û¢ÄïšŸ¨Ôâé°Ùé®Ü˜ö–
‚ØÒ«ûá˜‚’ÀŞ®é¢´Ÿ¢ÎÄó‰ç†­‡ÏØ¶¤¶›¢Ê »ÓÃˆ‹Ç‰ç¯ğİğÃ‰à’§õ›ÁôÅ®Ù»Œ ùú¹€ôäì€§€ºÔì¹Ğ
ÙØ‘šß€ªƒøßæÔ‚‰÷Ë­ÀÎ¦Û†…‹¼ÓÄ¦ÙİËİêò÷ò‰—Ü½ÇÜá³îÀ‡‹úºâŠÌ±ùæµ‡ÿÂ•¡°“˜¤ÜÁ•Çó—áÂ´³£µé
ìú˜òôòñĞÃˆŠºÌƒÚòÿº½º·íıØúÔ™¨¦Ñ³áÂ”ö¢åé˜¯×„½œ›ĞŠÌ¹úôûÔŞÎÛ¾¬¶÷£şŸ Ê€ÂÄÌ”ôÄÜØ…®„
¦ïş„öúÒİÿˆåË—¿öÚÖë‹ßŞâ™†Ó —ƒÀû½€Ë²Û¢³Úßš÷úóÏ¿¿çáÎœëÊ´°€àŠ‰‰©««ûõÕ¹¡‰‚ëã›à˜Ú
üÛ½Ü»Ñõëÿ‰ú°²êÁúóÏ½—››ûÇì³ÏêØÓ¢……ÓÊ•«ß¢şùåèÓ¨ÏªÙ¼øñ³Ï¼ı³èÑ¢€ÔÉï€ÂõÜÄ¥ª²ğ¹ÔªŠâ
à¢Â”“‘¢˜Ñˆ®—Ââ±À„‡‡ûŸá¢˜®èÕ©ã‰Î‹¦‚‚‚ÀÓãØøôœáˆÊáá¡–õ²« ”ã‚ÖãåøäÚŠ¼ö˜–è•°³¤è›Â
î¢å˜ ÿó¶Ğ£¶¦æš”Ä êÓ±ãÉ¹‹£ñËì³€ò„©À´óÌÈ›íº¶ıÚªÖ—ĞâáÃ„ô¸ñ€ÿÑÀÌ†ÀÍÓ²‚÷ğ—¶¦éÓÆÏ“
Íù¬úø´®Ôõª„ƒØïàÍ‰–Ğ©Ç¤¼¶ç…„„È¬‰ğ¶¢À•îÀ“Î„Ê¿öÈÌ¦¾ùçİÂˆËó“¼ŸñÜ¾·Åıæ·‡¯çÎí”ÀŠë
úáŸãÚšÔ˜·…€¾®€›¢˜ÄÉñÍÈº±ùÈÕõ¦ÎŠé¤øû¥©®„ê€›¿„Ë‘èÚêÏ½Ê¤´Å¥¬€ÌÓúÒÊ•˜…Œ§³¤ŞúË½º˜
€ØÛï‚Òû¸ßø¡ê¥—Ô·ªˆ±Å·¬†¡Š‰ˆÑÌÄ‘ˆâ¢Â¸Üˆ¡º—ÂÕáÁÄõ÷Â¨­å˜Øš³äìÑ½˜…÷½ÉÇô¯€„®©÷¡Ì¾
Øû¡Ø‘¶í¢éŒ¥¡Õ…Ç“÷Â˜ÀØ¨°­«à†ÆÏá³‹¬Ûğ¯°ôÏßŒñîÓ‹ó‘ÎöÂ˜ÀÏúñô°ã²Òöà‡Â„ŒØ˜î¼§ì‹„í…
¾°€ÀÚ¯ÌùØ±øáĞ›ÿıâƒÆÙˆİí…â°€ÔİïÒÉ°ŞˆƒÈí©×§ºÂÅ˜¡‡§¤”«°‹™Äô¯„Â…ì½ç‰î…Ğ¢úŠÜ´‘¢˜ˆÅ
ÂÄ„¨¦¢Ä·‰¬¡•°™ø¸ÂÀ“©É°‡óÒú‘öÌ€öèÏ½ÄÄ‡›•ş£©ßš§¼®„‡ÑÖ¯¥Óßêó§Ş‡ıº—‘ÜŠ‘±¸ËââÍ…‘
Ç“—‘‡“×¨”²¨´’Å¤’“›‘ïğ—“ø›®÷‰»¢’¡ŠÉà¡ÄöŒÃ˜°à’åú¤Ú¢Ö’Ø³±Ü¨Şˆ¨œÖøïâÍ¬Ãá¬ò¡”´¦Ÿü
È˜‰Ææ¦–ÅÏäÓıÎÎÌŒ‡‚±É§“ÆúÌ¼Ÿ„ŠÕ›Ğ˜ è²œòÙ“ÜÏƒ£È²¦£µ¦¨ºş·¨ıÜÄÛâäĞ”Ï”ƒÆõÄÍ†šã¼±
ãÒËœàáÇÄ‡è¾Ê¤ÊÌ„Œ‰İˆÙÒ‰€“•ŞÑÌÄÈ¹Ô¡Ç‘¢˜‚’©º¬È„¨¦à¢Ä±‘¡•¶õ¬Ğ‡œ¯ÔÃøÄÉá”¿˜¼ÚŞŸ¨¦€­µ
ÙóÎ–ÉÈÁÏ—’Ñš¼¤Ä±¸åÕº¥“ò÷æ¢İçŞÆÂœì·Àõ½Ã“çÂÀµü–…¹ò¬èÈ¡«¥Ğ¦¢³ä²àŠàŒÌãœÕÂÛ…ÁÑÒ¼
³±Ü™òá¹îæ©˜¸¹øïåÏæ©«áµ€˜Ö¤’âÚŞ§–²›Å‹‘•Œ·“¥”«úÚè“ÛşõàÂ˜ı¸­Â˜üÚ§²Â•ŒÆŒŠ««·”“À¶¦
˜¿½½öŒÖğÌŒ‡™Åä•ÇÉŸç¦Ô‡åÿ½‚êĞ¼§¼Îé¸§ìÛĞù—û¤‘Òş€èƒ–ÏÂÑÈæ«¶¾¸Ãí¼º‚—¡§Â”“‘¢˜ˆıÅ
ÁšˆÑÌÄˆËéÏÒ©­°¶É”¯üçıÖÄ¯Ë„¼˜÷°¿Éà·¤ˆ¯Ï§ç²•õâı“İŞ„â¼éó–¯ğœâ²ıúÁíò—Ù…Î‰“×
……°™àüÛÁĞ¯ÒŠí¸«ö°©÷¦»æ‹»ìì´ÄÍæù·¤Æ² ûµèŸ‰–áµ¼Ï¬Â·Š—¢ª‡Ş˜ûÉ‘öÇÛŸ¶Ìş–ÇÔæ”ö“Å¤Û©ñÓ
Ò”“öé“ÇïøÅõ¡ø£ˆ˜„¦Şì¥…”º”°…Ì´ú«ô©ÉÙ·ßÚ½•ÇºñèÂ†‘¢˜ˆŠÅÄ„¨¦¢æÄ·¬¡Š‰ÚˆÍª‡œÜÍ
óªÇÛò¥·©€×™ÂÍ–™ÉŞºÁ¤¤Ÿ”÷³†¾½àÒÔèÉ¿¦Õ‰•°î±ñçÈ¹“çİìÛ…°‰ŞåË½ĞÛãÄõ¡Š´Û„­²‹ØàÂå¥‰‚ğ
ÚóñÚ½É®œÂ­÷ŞÔîÆÂ‘Ä Èéş¦»úªÛÙ‹‰áŸÒç›ì²¯ê€¨µÜˆÿ·§«óßì«¼Ã»•Àè‚Ê“ûÀ¯³Û±±³¤¹ôĞ»Ãˆşº
Ì¥‘¢ëÍÄˆ°®ä¡Š‰ˆˆÑÌÄˆâ¢ÂÓÔ… Â¸°Áè©øƒš˜¨´Ô•ÅÄ„¨‹‚Á„¨¦¢Ä±Œ‘¡Š‰ˆ©÷ä¡îÖÙŠ¿íôÎ©ïÌ¹Ü‘
ØÒæÎºØú´µ¤¹ñüÓ‘óÑ‰çş¯ª’Ï»øŸÒèÛæÍÒ¥œÓæÕ·®ÂšÇÂáÈÁ­¶Õ¤İ¨’ÌÒÿÏïøÅ½§èˆÍ¹§í°µÉ—Øû•¹
ô¯ªù½½”ËâÚşÌ»ªõ×£ Šüš§Èò—¢åë³ü…ğ¹®Üí¸ÚÒÒšÛœËİ´«³áûú÷ƒ°´Ÿ·¦ş¿°åø½“ïèïºÖóÅúÍ
ÆË×›Ğ¥Áí¤Şˆ¤ÌÏÿ§­ó¯§¨äşæ±¥Ğ˜°•¹±ü¹İ®èâ–Ä­ËÖá°¥á›¶Ä¥´ß”ç–èÁ¢¦é·Ê–øòÌÉ¥­ğ×µ¹™ñºÙ´
Í„¸Ê×ëş°Úñ÷ñî©ê’ÎÄ¢øïæ…¶¬ŞÖ¦ÊÍ£¹”ãºÚò¿şº¦¦Â³ŸµŠşÜ‡û¯ğ’¯òÚ˜µŸÏëøŠì¡°±ï½§ºÂ¤Ñ…
´È¸û³üäÉ˜İÙğ°îÚôÙ§œ£öçÏĞØÄ£‘ûß· ­§ÿ¬³ïƒ¤µç˜â×Å…•óË ›“ïÙ”¼ÉÁäóÌÌı¤ŒèÊ¤¨œÓüáƒÏ
¸ğ×áòÍ¸¸İ¶‡›Ô“ï›óñèøÿ°¥½ïÈî°Á¹ìûûËø¬Á¬¤¦‰§Ø¤œŒÓîéıƒùõû³îî¸Õ·§ÎÉÉÇä¤ø¸şãĞ¦Æ
œ£öçğ˜óğáÄ±°¼±ÿİı‰·˜Ü»ÖËïâğ¹ŸÂ“€ôÌ¼×”²ŠãúÓØÉº¼æú‚àµö¼œÎ»Ãé”îÜãÎˆİ‘€şÕ’ŞÁ
—Ø…ğŞÇå®—ÍÛ•Ééø¾úóÈ½úøšË÷³ìäŒ©±Ç´©Ï°»Üº¶§›ÓÒíÅ‹²‹ªÚ·šØĞ˜±íÊ¼¥úšñïìÍ—İÔÉŠ‰‹¦ƒßİ
ÀÜŸÿıíÌÕÏê¨Š÷£¯¯ÿüìÂŠˆóËş÷ù÷â¥ıÛ‹—„å¿óüÉ¾¤Ñ¤š¤£‹®·‘¡ºå¡…ˆÑÌÄˆâÀ¢Â”“‘ÒîÂÂäÜé¿ÔØ
éøŞÌÍ›Û®¼†ï¾òÛ•—î°Á‚·Õ¾®úß•Í•šÿÙÖ½îĞÍÃåİîí«Ë®”ãÚ—óÓİï»­®İ¾îœ¥²»§ì““Ã¶ã®¾¡Ç£á
´§œÛ“ıÏ­œí½ãÕµƒæ›Ü½÷Ã¬ˆíı‰•òÌË·÷Ş¬ë³í©ëèµ¹¦ìÉÁµ‡ï½ñöÈ½ñÛÜ€îç‰ÍÕ±°ñ°şç¼¼öñ•Ú
µ·Âªµ­¯İ¡ÒéÏáÕµí¶İùÖ ˆ‚°õ¾ñ¼°õÂ»‚Öä”ò´ØÇŸö¿ÛÃ³Ì×ë‡ä—üÄ³Ã®¡À„şüÀ×Òî’ÂÀøô§åì
÷Şú‹¨¦÷È±²¾Æ¹î˜šœõµÉõÂÓ²·öÙú¿ÍìÏ¿Ö³ àı©ï±ğîß„˜‰ğ¤¼ƒ®¤œ“¸ŒµŸÍ£ŸŸî±”ÌÀ°¼šø¼
çê‚¦´âŸìæúäğ°êè²­í¼ÕšİÕ¹ÿÃ‹Ü²à·ÑŒÌª•„—í°àú®ñôóıÜîèîÀûò°¾æÓâíÍÓ‰·¿ÊøşİñÁÛëçÂ¶“
¼Í™åæù¡¥ÍÜ² æÔÅ·º½ š ğ‚ˆ¼ï²ŸÖ„±éÀã‘†Ì›ª¾¼í˜È¶„ç¦ÙÂ³­‘ÙØ‘ÙÌ»²ÍÜ»ÏµíÈèİ¿–Ş²õäÃí
îéÍ©—Úæ¯â»ÏÛŒ³ ©Ö„…Æ”‰‹ƒˆ¨¼õ¸Ú©¸÷¡€Óµúáª·£¬Ë´ ´ …Œ¾ıªººíé­€¨ùìÄÓØ‰Æ¤œóÍóÖ•®
“À¢ ê‹¯á¼Ğ¨“Ş±¶Ò¹°àªúñ¤¨¼«ãº˜½–ÍÉÍ¨£õÇèÆ‹°Úú¢ñ³“ãÛÿ”ØßØ¿ïƒâáùòˆüŸ¹˜„¸³‘¾æâş
…ºÎ„ƒÓùÅíàøæ‡¼¥¾‰„ÍÉ÷ãÎ­ÿ˜´¦Ğ¸¼—ÂİìûËÌòæàÎïáÆŠƒ¯ïÂ¯ç÷ÒÌÅÜİÃ›à¬êÃÑŸöå‘İ‹ÚİÀ­ƒ
œïƒºÈû£äªÏáä‚ãÑ®ú¼†‡‹±øğÓÅéÇá§Ó˜®µ×ó†ûÀ´¾áîŞô¼»Á«ó´­¯Í¡Òé÷›Ë²Ë¾ÿ”Ù®÷ÜšÖúş³
ğ“µó×Ö¶“¾„Ê¹¦ÿşæş½øæÿñ·êıÂãÓç©·ºö¥ü÷Ç¢¨®„•¯ğìúşíøøØİ×Ãäª—¬¯Û°şÓÒü¯Î½Ì°ïİ¹ùëÏ
°„ıĞ”´¦ ÌèõşÜÕùĞş¿¾àÕ”Õú•£ºå…ˆÑÌÄˆâ¢ Â”“‘ÒâÂ¥ĞÈ “Ï¿èµ¶ÕÖŞª‚®ÊÍºö¸®ä®Šò”¯Ì
ÀùÑÊ²ùÁ½ƒÌãÍ¦öàß†ÔµğàØ°¯ ˜™Ì½²Ü«úÓÀ¿¥©±÷®ºªŒ²ÊÉÉ€î¢¡øáû”äò‚ª™ ëÒø°¬˜—ÿŒÜß×áÅô
ÿ›Å»Õ
&]
[s2;%% &]
[s3;%% &]
[s4; &]
[s5;:Scatter`:`:FitToData`(bool`): [@(0.0.255) void]_[* FitToData]([@(0.0.255) bool]_[*@3 Y]_
`=_[@(0.0.255) false])&]
[s2;%% Scales and scrolls the plots so that all data is included 
in X axis. If [%-*@3 Y] is true, Y axis is also fitted so that 
all plot points are visible.&]
[s3;%% &]
[s4; &]
[s5;:Scatter`:`:Zoom`(double`,bool`,bool`): [@(0.0.255) void]_[* Zoom]([@(0.0.255) double]_
[*@3 scale], [@(0.0.255) bool]_[*@3 hor]_`=_[@(0.0.255) true], [@(0.0.255) bool]_[*@3 ver]_`=
_[@(0.0.255) true])&]
[s2;%% Zooms graph by [%-*@3 scale] factor. The axis scaled are X if 
[%-*@3 hor] is true and Y if [%-*@3 ver ]is true.&]
[s3;%% &]
[s4; &]
[s5;:Scatter`:`:Scroll`(double`,double`): [@(0.0.255) void]_[* Scroll]([@(0.0.255) double]_
[*@3 factorX], [@(0.0.255) double]_[*@3 factorY])&]
[s2;%% Scrolls graph by [%-*@3 factorX] in X axis and [%-*@3 factorY 
]in Y axis.&]
[s3;%% &]
[s4; &]
[s5;:Scatter`:`:SetMouseHandling`(bool`,bool`): [_^Scatter^ Scatter][@(0.0.255) `&]_[* SetM
ouseHandling]([@(0.0.255) bool]_[*@3 valx]_`=_[@(0.0.255) true], [@(0.0.255) bool]_[*@3 val
y]_`=_[@(0.0.255) false])&]
[s2;%% Activates plot scrolling and zoom in X axis if [%-*@3 valx] 
is true and in Y axis if [%-*@3 valy ]is true.&]
[s3;%% &]
[s4; &]
[s5;:Scatter`:`:SetRange`(double`,double`,double`): [_^Scatter^ Scatter][@(0.0.255) `&]_[* S
etRange]([@(0.0.255) double]_[*@3 rx], [@(0.0.255) double]_[*@3 ry], 
[@(0.0.255) double]_[*@3 ry2]_`=_`-[@3 1])&]
[s2;%% Sets visible data range for X axis in [%-*@3 rx], Y axis in 
[%-*@3 ry] and secondary Y axis in [%-*@3 ry2].&]
[s3;%% &]
[s4; &]
[s5;:Scatter`:`:GetXRange`(`)const: [@(0.0.255) double]_[* GetXRange]()[@(0.0.255) const]&]
[s2;%% Gets X axis visible data range.&]
[s3; &]
[s4; &]
[s5;:Scatter`:`:GetYRange`(`)const: [@(0.0.255) double]_[* GetYRange]()[@(0.0.255) const]&]
[s2;%% Gets Y axis visible data range.&]
[s3; &]
[s4; &]
[s5;:Scatter`:`:GetY2Range`(`)const: [@(0.0.255) double]_[* GetY2Range]()[@(0.0.255) const]&]
[s2;%% Gets secondary Y axis visible data range.&]
[s3; &]
[s4; &]
[s5;:Scatter`:`:SetMajorUnits`(double`,double`): [_^Scatter^ Scatter][@(0.0.255) `&]_[* Set
MajorUnits]([@(0.0.255) double]_[*@3 ux], [@(0.0.255) double]_[*@3 uy])&]
[s2;%% Sets the space between grid lines, horizontal grid in [%-*@3 ux 
]and vertical grid in [%-*@3 uy].&]
[s3;%% &]
[s4; &]
[s5;:Scatter`:`:GetMajorUnitsX`(`): [@(0.0.255) double]_[* GetMajorUnitsX]()&]
[s2;%% Gets the space between horizontal grid lines.&]
[s3; &]
[s4; &]
[s5;:Scatter`:`:GetMajorUnitsY`(`): [@(0.0.255) double]_[* GetMajorUnitsY]()&]
[s2;%% Gets the space between vertical grid lines.&]
[s3; &]
[s4; &]
[s5;:Scatter`:`:SetMinUnits`(double`,double`): [_^Scatter^ Scatter][@(0.0.255) `&]_[* SetMi
nUnits]([@(0.0.255) double]_[*@3 ux], [@(0.0.255) double]_[*@3 uy])&]
[s2;%% Sets the distance from the leftmost vertical grid to origin 
([%-*@3 ux]) and from bottommost horizontal grid to origin ([%-*@3 uy]).&]
[s3;%% &]
[s4; &]
[s5;:Scatter`:`:SetXYMin`(double`,double`,double`): [_^Scatter^ Scatter][@(0.0.255) `&]_[* S
etXYMin]([@(0.0.255) double]_[*@3 xmin],[@(0.0.255) double]_[*@3 ymin],[@(0.0.255) double]_
[*@3 ymin2]_`=_[@3 0])&]
[s2;%% Sets the coordinate of the first visible point in the leftmost, 
bottommost corner of control in X axis coordinates as [%-*@3 xmin], 
Y axis coordinates as [%-*@3 ymin] and secondary Y axis coordinates 
as [%-*@3 ymin2].&]
[s3;%% &]
[s4; &]
[s5;:Scatter`:`:GetXMin`(`)const: [@(0.0.255) double]_[* GetXMin]_()_[@(0.0.255) const]&]
[s2;%% Gets X axis coordinate of the first visible point in the leftmost 
side of control.&]
[s3; &]
[s4; &]
[s5;:Scatter`:`:GetYMin`(`)const: [@(0.0.255) double]_[* GetYMin]_()_[@(0.0.255) const]&]
[s2;%% Gets Y axis coordinate of the first visible point in the bottommost 
side of control.&]
[s3; &]
[s4; &]
[s5;:Scatter`:`:GetYMin2`(`)const: [@(0.0.255) double]_[* GetYMin2]_()_[@(0.0.255) const]&]
[s2;%% Gets secondary Y axis coordinate of the first visible point 
in the bottommost side of control.&]
[s3; &]
[s4; &]
[s5;:Scatter`:`:SetPopText`(const String`,const String`,const String`): [@(0.0.255) voi
d]_[* SetPopText]([@(0.0.255) const]_[_^String^ String]_[*@3 x], [@(0.0.255) const]_[_^String^ S
tring]_[*@3 y], [@(0.0.255) const]_[_^String^ String]_[*@3 y2])&]
[s2;%% Sets the text that will define the values in axis [%-*@3 x], 
[%-*@3 y] and [%-*@3 y2].&]
[s2;%% It is used in the pop text that appears when selecting a point 
or a window.&]
[s3;%% &]
[s4; &]
[s5;:Scatter`:`:AddSeries`(Vector`<XY`>`&`,const String`&`,const bool`&`,const Color`&`,const int`&`,const int`&`): [_^Scatter^ S
catter]_`&[* AddSeries]([_^Vector^ Vector]<[_^XY^ XY]>_`&_[*@3 points],[@(0.0.255) const]_[_^String^ S
tring][@(0.0.255) `&]_[*@3 legend][@(0.0.255) `=]`"`", [@(0.0.255) const]_[@(0.0.255) bool`&
]_[*@3 join][@(0.0.255) `=false],[@(0.0.255) const]_class`::Color[@(0.0.255) `&]_[*@3 pcolor
][@(0.0.255) `=]LtBlue,[@(0.0.255) const]_[@(0.0.255) int`&]_[*@3 width][@(0.0.255) `=][@3 30
],[@(0.0.255) const]_[@(0.0.255) int`&]_[*@3 thickness][@(0.0.255) `=][@3 6])&]
[s2;%% Adds a new data series located in [%-*@3 points], with [%-*@3 legend].&]
[s3;%% &]
[s4; &]
[s5;:Scatter`:`:Stroke`(int`,Color`,const String`): [_^Scatter^ Scatter]_`&[* Stroke]([@(0.0.255) i
nt]_[*@3 width]_`=_[@3 30], class`::Color_[*@3 color]_`=_Null, [@(0.0.255) const]_[_^String^ S
tring]_[*@3 pattern]_`=_LINE`_SOLID)&]
[s2;%% Sets the line [%-*@3 width], [%-*@3 color] and line [%-*@3 pattern] 
of the latest added series.&]
[s2;%% If [%-*@3 color] is Null, it is selected automatically a different 
color depending of the series id.&]
[s2;%% Available [%-*@3 pattern] values are:&]
[s2;i150;O0;%% LINE`_DOTTED -|-|[C `"o...`"]&]
[s2;i150;O0;%% LINE`_DOTTED`_SEP-|[C `"o...........`"]&]
[s2;i150;O0;%% LINE`_DASHED -|-|[C `"oooooo......`"]&]
[s2;i150;O0;%% LINE`_DASH`_DOT -|-|[C `"ooooo...o...`"]&]
[s2;i150;O0;%% LINE`_SOLID -|-|[C `"oooooooooooo`"]&]
[s2;%% However this value is user configurable just entering a String 
with the pattern defining line elements as `"o`" and space elements 
as `".`".&]
[s3;%% &]
[s4; &]
[s5;:Scatter`:`:Mark`(int`,Color`,MarkStyle`): [_^Scatter^ Scatter]_`&[* Mark]([@(0.0.255) i
nt]_[*@3 thickness]_`=_[@3 6], class`::Color_[*@3 color]_`=_Null, MarkStyle_[*@3 style]_`=
_CIRCLE)&]
[s2;%% Sets the mark [%-*@3 thickness], [%-*@3 color] and [%-*@3 style] 
of the latest added series.&]
[s2;%% If [%-*@3 color] is Null, it is selected automatically a different 
color depending of the series id.&]
[s2;%% Available mark [%-*@3 style] values are CIRCLE, RECTANGLE, SQUARE, 
TRIANGLE, CROSS, X, RHOMB.&]
[s3;%% &]
[s4; &]
[s5;:Scatter`:`:HideMark`(`): [_^Scatter^ Scatter]_`&[* HideMark]()&]
[s2;%% Hides the of the latest added series.marks.&]
[s3; &]
[s4; &]
[s5;:Scatter`:`:SetSequentialX`(const int`&`,const bool`&`): [@(0.0.255) void]_[* SetSequ
entialX]([@(0.0.255) const]_[@(0.0.255) int`&]_[*@3 j], [@(0.0.255) const]_[@(0.0.255) bool
`&]_[*@3 sequential]_`=_[@(0.0.255) true])&]
[s2;%% If [%-*@3 sequential] is true, it Indicates that the [%-*@3 j] 
graph data has been sequentially inserted following X axis.&]
[s2;%% This speeds up the graph painting.&]
[s3;%% &]
[s4; &]
[s5;:Scatter`:`:SetSequentialX`(const bool`&`): [_^Scatter^ Scatter]_`&[* SetSequentialX](
[@(0.0.255) const]_[@(0.0.255) bool`&]_[*@3 sequential]_`=_[@(0.0.255) true])&]
[s2;%% If [%-*@3 sequential] is true, it Indicates that the last inserted 
graph data has been sequentially inserted following X axis.&]
[s2;%% This speeds up the graph painting.&]
[s3;%% &]
[s4; &]
[s5;:Scatter`:`:SetSequentialXAll`(const bool`&`): [_^Scatter^ Scatter]_`&[* SetSequentia
lXAll]([@(0.0.255) const]_[@(0.0.255) bool`&]_[*@3 sequential]_`=_[@(0.0.255) true])&]
[s2;%% If [%-*@3 sequential] is true, it Indicates that all graph data 
has been sequentially inserted following X axis.&]
[s2;%% This speeds up the graph painting.&]
[s3;%% &]
[s4; &]
[s5;:Scatter`:`:RemoveSeries`(const int`&`): [@(0.0.255) void]_[* RemoveSeries]([@(0.0.255) c
onst]_[@(0.0.255) int`&]_[*@3 j])&]
[s2;%% Remove [%-*@3 j] data series.&]
[s3;%% &]
[s4; &]
[s5;:Scatter`:`:RemoveAllSeries`(`): [@(0.0.255) void]_[* RemoveAllSeries]()&]
[s2;%% Remove all data series, cleaning the graph.&]
[s3; &]
[s4; &]
[s5;:Scatter`:`:SetMouseBehavior`(Scatter`:`:MouseBehaviour`*`): [@(0.0.255) bool]_[* Set
MouseBehavior]([_^Scatter`:`:MouseBehaviour^ MouseBehaviour]_`*[*@3 mouseBehavior])&]
[s2;%% Sets with [%-*@3 mouseBehavior] the array of MouseBehavior conditions 
and actions to be considered when handling the mouse over the 
control.&]
[s2;%% This array has to be ended with an item with action `=`= NO`_ACTION.&]
[s2;%% Returns false if [%-*@3 mouseBehavior] array is not well defined.&]
[s3; &]
[s0; &]
[ {{10000@3 [s0;%% [*@(229)4 Scatter`::MouseBehavior]]}}&]
[s3;%% &]
[s1;:Scatter`:`:MouseBehaviour`:`:struct: [@(0.0.255)3 struct][3 _][*3 MouseBehavior]&]
[s9;%% This structure is used to describe the behavior of the mouse 
when used in Scatter. It includes the keyboard and mouse conditions 
that, when complied, will launch the indicated action.&]
[s9;%% It is used by SetMouseBehavior(MouseBehaviour `*`_mouseBehavior) 
function to set an array of MouseBehavior items that will be 
used to launch actions as zooming or scrolling when mouse is 
used.&]
[s9;%% The default array is in Scatter`::MouseBehaviour defaultMouse.&]
[s3;%% &]
[s0; &]
[ {{10000F(128)G(128)@1 [s0;%% [* Public Member List]]}}&]
[s4;%% &]
[s5;:Scatter`:`:MouseBehaviour`:`:ctrl: [@(0.0.255) bool]_[* ctrl]&]
[s2;%% Set to true if Ctrl has to be pressed.&]
[s3; &]
[s4; &]
[s5;:Scatter`:`:MouseBehaviour`:`:alt: [@(0.0.255) bool]_[* alt]&]
[s2;%% Set to true if Alt has to be pressed.&]
[s3; &]
[s4; &]
[s5;:Scatter`:`:MouseBehaviour`:`:shift: [@(0.0.255) bool]_[* shift]&]
[s2;%% Set to true if Shift has to be pressed.&]
[s3; &]
[s4; &]
[s5;:Scatter`:`:MouseBehaviour`:`:left: [@(0.0.255) bool]_[* left]&]
[s2;%% Set to true if mouse left button has to be pressed.&]
[s3; &]
[s4; &]
[s5;:Scatter`:`:MouseBehaviour`:`:middle: [@(0.0.255) bool]_[* middle]&]
[s2;%% Set to true if mouse middle button has to be pressed.&]
[s3; &]
[s4; &]
[s5;:Scatter`:`:MouseBehaviour`:`:middleWheel: [@(0.0.255) int]_[* middleWheel]&]
[s2;%% Set to true if mouse middle wheel has to be rolled.&]
[s3; &]
[s4; &]
[s5;:Scatter`:`:MouseBehaviour`:`:right: [@(0.0.255) bool]_[* right]&]
[s2;%% Set to true if mouse right button has to be pressed.&]
[s3; &]
[s4; &]
[s5;:Scatter`:`:MouseBehaviour`:`:action: MouseAction_[* action]&]
[s2;%% Is the action to be launched if the previous conditions are 
complied. It can be:&]
[s2;i150;O0;~~~1248;%% NO`_ACTION-|No action. It serves to mark the 
end of MouseBehavior array.&]
[s2;i150;O0;~~~1248;%% SCROLL-|Scrolls the graphs.&]
[s2;i150;O0;~~~1248;%% ZOOM`_H`_ENL-|Zooms horizontally enlarging 
the graphs. &]
[s2;i150;O0;~~~1248;%% ZOOM`_H`_RED-|Zooms horizontally reducing the 
graphs.&]
[s2;i150;O0;~~~1248;%% ZOOM`_V`_ENL-|Zooms vertically enlarging the 
graphs.&]
[s2;i150;O0;~~~1248;%% ZOOM`_V`_RED-|Zooms horizontally reducing the 
graphs.&]
[s2;i150;O0;~~~1248;%% SHOW`_INFO-|Shows an info label including mouse 
real X and Y coordinates.&]
[s3;%% &]
[s0; ]