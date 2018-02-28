topic "StaticPlugin";
[ $$0,0#00000000000000000000000000000000:Default]
[l288;2 $$1,0#27521748481378242620020725143825:desc]
[H6;0 $$2,0#05600065144404261032431302351956:begin]
[i448;a25;kKO9;2 $$3,0#37138531426314131252341829483370:codeitem]
[0 $$4,0#96390100711032703541132217272105:end]
[{_}%EN-US 
[ {{10000@(113.42.0) [s0; [*@7;4 Functions4U Reference. StaticPlugin]]}}&]
[s0;*@3;4 &]
[s0;# [2 Sometimes it is necessary to access files, devices or other 
services through an API that hides a lower level layer from different 
sources.]&]
[s0;#2 &]
[s0;# [2 If this lower level source is known at compile time, these 
API could be implemented using virtual functions.]&]
[s0;#2 &]
[s0;# [2 However if the plugin has to be defined at run time, StaticPlugin 
can help. This will be explained with an example:]&]
[s0;#2 &]
[s0;# [2 There is a StaticPlugin demo in Functions4U`_Demo package. 
It is a dummy API to access spreadsheet files using OpenOffice 
Calc or Microsoft Excel. This is the class diagram:]&]
[s0;#2 &]
[s0;= [2 
@@image:2093&1525
ˆƒÏô€€€€€€€€€€ø×œí±ˆÇ›š…§ó‹¬¥½ûÖ‰…Ì€‘Ô¬ïñËÌˆŒ¨™œ©°Á¡”ÀÈ¨Ôâ£Þ¢ç—Œ•ˆ¦ØÛÀ°ëìÀ¯³
Øƒ…Š„¤ðŒ“‡Æ‚åƒé¢¥ÂŠ›—©«ôž·‡ûøÕûÕÝÓÓ÷Þîï¾Ýçð³ô­ÿ®®ªî®¯êï¯îªž¿þ°ŸÞÑÁÁÁíëÿóõàÁƒ¡«
¹’µöíîîî¶Ù›¯óçÎîÝ•¬±‹°ŸÚ½ûÁõÏ×¶ÓÍµÏ¿½àØÉ­ãåØ§à†µÝª‘áŸ€™Šö«Ä†ý‚æÓØ­’™ö‰˜¢á·Ê´ÛØÿ
¿úùÇ¯¾¾±°ø±Öâ†Ýª½Ñ °ÿöû¯ ßüùñûûÃ¤˜¶–ÌŠ‰üÇÿÃ·Í‘ÀôÓÎ…ÏèÈ‹ñ®™¾÷Í­„ô‘Ë”¹Þ½öåÚðÆå†ÝªÑ
ê °“–—‡Ïñ€ž¶ˆàÁÓÙÇˆ¿öØ¼Ú‘Ýû÷Úâ“óÀœ…ÃÞ¸–ùˆ®‰»Ú–µš´ã°Ãî•¨ÿÝØÝÖÖ–þ‹ÆÀ×¯Á„È¤•±²Ìä
™ÕÆË’â®’Ö­Øµ—Ç‚óœ„ö¤”ãŠ²ù‚´Æ‹ñÙê¶üÍŽ²ìÖÉØ´àÏžÃÐˆ»£€ýûû„€Ö¹ßòö‘óèÌÍ ±²ÿâÔÞˆ»¶
²˜˜¤˜Ë€Ì›ñ—Þ„â£šë¶é°Ð†éñŠ»Õ¢ÁàçÉïŒÌâØ»÷ïÏ…ðÃÇ÷ÁÆÚ“·¯¶‘ž€ ²ŽÊàÐçºŒ¯ã¨ý¥˜·£ìÈ
èÐ—Á·ìÖì¸µÔÓøŽÖÙ‰ü“ìµÄ·ÀÞû¤Îý½Ê·ô„¾„öÐÑÝž¿©Ì²ÏÌŒ»µîû¯ê°°·Ì©Ô†èä”Ðé˜ÁßŽû¾ð¦ã
íÀí€†ò…«°ÓƒÁßŒˆƒäÿ›öëÃµ™øûö¹Ù‰¸ð„Íø®Ó³×€‚Ø‚»Ú¡úßïÉˆÃ°Û›û¨þáÇ™£ïæêèéúÒ³Ã‹îˆŽ÷ÅØ
ÈãŸ½î«¨‘¡Ÿ³·ÁžˆÄáäÅðŒóž¼‘°ìÖ†íªÿØé’ë™æùò¨Ü¡ •ŽáÙ±Æ—½ÖÆ‹°ó‚ÍûƒÓøÚ°Ù¤ž¹Å§‰Ñ×ø
ŒŸÿÙÈ˜öë”“µ”›î—²èäÃìð¥À¿…Ÿì “×ÈÚ±Ú’ˆöÅ‰á¹Ž¬ý†µ§¢†£þÝûÖ÷ôÀßÍÈô¹ÞµìïÙãÝîÉš†Ýš•
Ü†Ÿ³ÙÀ·ç¦ç±“ï™—Ò» ™ö«Ä†¿½ÅèÆËâÃ•¼Ñ™á·ÊäÙØ§à†Ýª‘¦áŸ€™ö«Ä±†ý‚æØ­’ª™ö‰˜á·ÊÍÔƒì¯ÞŸî
˜ýó‹¦¹´¥ÿëÖ«£×Öƒ²ó´‘æØ­’½õ€ûÁÓÇßüýýƒßŽàºý’ÂØÇùžëŒÄ§¾ä÷øŠ“Í°Û¥ê‡öõÑÄœÿ¾ÿ§¾üôóœ
í†¿ ƒí‚ãóš­‚Á½Ï¶“Õ£±œÎ´ÃÜ‚ú÷Þ“¦Ã©®ØÀÌ•„°—ê±Þµ…®èãÏ¶¯ª¾†÷ÆÃáœà²Ã­³ÒÙ¥à™ñ¡Ïþ‰
êá·ÊÔ§ì„†È°óëÑä©Æ¯Å Æçê´ÎÁåŒ·ÈÏ»Âù•¹¤¥îˆ„‘ÜÕ‹¹€±è˜Ÿ›œÊÐàÀ„£š©¡ç´øþŸ¯Š™ý²çôÝ¶
Ð¬ªŒ…ŽØ™¡å£á·¨†×¿°ë–»»ò“Âž¿‹€“á×ø’Á¤“ò§ÆÊ‘ÄÝ³·çÒ×ˆ»õÂáÑ©ŽÖ‹ËÄôð—‡¢÷šœŠ¤£¤À
ã†ÝšÖ½»ñí´¦ØÙ ¨ûÁî†»›ì³ÚãÍ™„ƒÁ®É‰ªÿÊ›§¼ÑñÃö­°óŠòþœËïØ­ÑìêÀØÁ”Œ£°àÄÀïËÕä³ÜîŒ»Óá¨
‘¾–ööúìÛôð ÿ¥¦¶“‚ÍÂ‹ìüâÃÀ˜œ—¤™±ü¤öã­‘¨Ÿˆ×ïúé¦Î££°üÞœÌì¬ÒÆÓú÷¤“Ù¿šÃ“ˆÀ…ïú¸ø×ö
˜œÐ×‹‘‚¡Š¥ïÄ›¸³ÐÁïŠõøü®Çúô˜‘Ý’–Œ’Œ»µ†çÒÄûØˆÝ¿†Õ¼ˆ»ž¸íœô™ö«Äã½ÜÚô©ý¿Þ—Ô§°ø•èæ†
­ÝªÑ¨à·õº³Ãî•È°Ï¥ÀŒ»Õ¢Ã¾Ô³ìÖ‰€ûÐûÿúµåÂ¿ö×¿þçàå¶Ø«ºõ¾áÿ·ŽÕþþþîÔµµµµô‘‡Ö®›·ïŽÝ
á•¬ëøö¡Á‹àÙÖŸ²ìá–µ“™öËšÕ‰Œ»åÍÄ†“Ý²æ¢ÃîÙú³‘á·¬™È¥°ÛÖÌäØ­˜ë¦²ì–µíø¬„ØµýöÏíÛ¡ËåÙÖ
ÿŠµ³³³õ®’ÎŸ¾íØ­ëÎúÚßßÏà¿¾þýúÐ…²¬šëõúùòå‚Óû“§Ï†®”ðåÙëÑôäíÃÛÖ„•½ù‡ûð–µá½ÿ«þÜ¾¼åÍÄ
çÏŸ·ïÙþóÐíÛ·íÃ¢ÛÖœÄÏÞ¾…¼åÍÁ»»»ûöá­ëòåËæÛÖæ‹ÚÐŠÕÉš»ö÷÷úÏúÿÖßÿ¢æÛÜ»ûöƒäÿ·ÞºÖ»þ
µ°›ïŸú¿‘ÂØ…²ì“°ÝÃî•È°ÏÀ’Œ»Õ¢Ã¾Ú³ìÖ‰æƒû¾ï¿ÿòêõá—ÓÞŸþþ‘‘ †¯ä·³ìÖÝ‰ú†ýûŸ¾÷Ý¹ð‰ì«¯
âïôÌ„²ÝûÍæÖ·½£·ÃÖî•¨ÏØø·öö‡÷ïÝÑòßüù±ç¸ËúîÎ­×ÏóªŒ»Õ¦è¾á‡éƒöž›ýÀþðô±ÎÓ³çÓ³ìÖ‰
Þú†ýêµ«µÉáà„¸áƒ†ŽþŸáè¢Ÿ¾ÃºÏðÄ ÊŠæ–“Â¶ƒ“ö¢Äáðª¢ÎŠŒ×¯Ì‘…þ¦‚å¨–Ž‰Œñâ°´ï‰Çúø¯¹µµÅðÃî
©ÊýÂŽ‚€Ð‚‘Îà‡±Ø˜ŠÀºý¤’±ñ“›¯ÞŸ â¯ùÿñøÌÈ”ã¯â ÄèõñØÄÄÖŒ†Ôâ œë„µÑòÒ¢ŒÇ¡Ø©Å„ù ’¯
Ñç¶²Õ™»¼µ’õü€Ž½žµ‘°ÇžÓéà‹çµ•±»ŒÌþÆÃ‰š»Öô…«çìÆ¡½±òÌ¤ºŠ¿êô›ÂíÆÛ»£Õ ¯ÞÀòêô¡Å–þ’£
¤äœíùÈ²ÙîÂÛÔèˆô£Î£‹‘»ñ‡ˆ¬×íÉÞ‘¯öÄØû·…›”öë´šê½»Úýì¦ØÙËæ›°×òÄ¦ÃŽòùä´Ž½“Ô£Ð³“ùöñ­
‹°ÇÄòÂ¦æïØ­ÑªÏØ£ÇçÌ§Û‚Ç ™ÜôÍìð“È€‰õâþ×‹Åá³·±’—§ðñ”À†óçí˜±ò­ìÉÐ¢¼¶µá†Ýšµ­úƒüÒó
 †ÓüÜ–ûâè¾Ýçß­ˆòÙ×á¸œÌ„¢±Ù¾ „Žô¨üôßžáè¾ÕôØœ©ñØÅþ½¶çç‹ÃÉøìÕØµ…¦×‹—ŽýÎšú²ìÖ‰úìÙ
…ƒ¹€ˆñ´£ÍÁµÐ…ËœïùÒ°¼·¯¥Žž¯‘óìÕŒ¯®É—ÂÅ®é±É¸ÓãÓû–Ã©¤ù‚¡‚’áü¶Ó°±ÎÓ¸¶Ùˆà˜öëÔšÃÜ
¥øäÌî»©™Šö«Ä†ý‚æËØ­’ö““¾¤™ö«ÄãÏ€Ý¶¤™ö«âÄ†ý‚æØ­Ô’™ö‰˜á·øÊ´½Øã«·«™˜á·ÊÔ‚î»çÀìÜø¤
ÄöÞèè…îÛöÄ¯¾âŸªåäñÊŸþòúÙóôÚ¾‰áØý­’µÀÎÚÅÓÅ¤¯ŽŸñþÝ‰¸ÂÏÈÔò•³ºšÂ£íÖ³‘¸ËÓÚ’ßô“Ô’¼Ôž
ì¼Â„½¥‘¼¼ÁÈ¦¹Ö¶˜¹ñÜØ’ø³¥»ãóÇÒÄÛ˜‘¸­‡Ô›ö ëù‹»ñÁ©•ÖÇƒíÎá¹ã—¡ªÅ¢³ÙÎÊýøè¾ç£§Ñ¥à´üå
°êÌ„¿Ù‹±Ð…âËÇ‚±™øš“çÖœ«†°íÎ¾çÌØ§ÛòÓ“ŒŒ¸üè¼”Ë¢…³øµÉÙ¬ñ›Æ™õ¼ÌŸÊ‹Ç€ªÎ¼—ÆãÈÐÒ†ý÷‹±‘
àˆÿ¢¼îÇ«™ã‘Â¿‘öµÂ†ÝÚÒ­‡°³ÂÇÎÙƒÁ¦º ÔÎÎöó®¦ªíâ·õÑ—žÈÈ¿¬Õú „ºæÀî ©žÂÌòÁ•¦ä¤–»ö
›ÌŸ€úç‚íç¥äŠ¬‰¬óìŠ©èµåÅà®ž¦Ìûãœöòµ«ð‰»Çœ‚š‹ÞŸÃ´î­©öØã—ä×„‘œËöé¢Ü’»úñËÂŠØÓ‹¦‚
ªˆÚìÕæÑ´°¯úƒ¬ö±£÷ˆóŽ»›©±­ÿÁÑµÚ¥žîç°ÇÆ¤¢û–œËŒ–”­¾‡¾ý„šý½ªùÂ•×¸‰±‘®ÍÁ©•àúÒÅÉËâ
£Ø­Žªñã“¯ˆ’žˆÄæÞ¡Öü„ Ž°ý«ç§å°×öÁìÊ¶ïŠê·Ñ«÷ûöŽ¬¸çŽ¦©µÐ÷á¿Í½û´„è‹Ôâ¶Û¥þý£‹Œó»ßñç
£Ø­åÔŽ»üœõ²®Þ–õµãŽè£ßš†ëø©Ó¹ìŒÌ¶µ…ù©ú·É‚Ñ•»Ÿó¡Žý¿ÿÅ¬åÆóâçÜÎÎ°ŽÙÕ•àŒÏÅÑ™ôœÀ‡ÐóÐ
å‚œé°æÜÎÓ›‘ê±Ê°ìÿÖÊµ’÷ìñóÐ›û¯–¶õ×Œ»Õ¢•ÀÎ§ÐÉ“òÛŒïæØ­’Íïš†ÝŒÍ°Û¥Ò²ì“°Ãî•«È°ÏÀŒ»Õ
î¢Àþçÿÿ·ï‡îÛ¶×þ÷ò—ÿ°ìÖ±ÕºøñßÿÕÁÅÚô¿ÿþèåªäÝïÞ¼ö¹˜º¾ÿüó×¡‹°ö½üù†òèªäÙƒ‹‘­™º¾à°ô
ôÁ¬ËÚ¯¾ûõî»ÀþãÆÑ¡‹âÙÖúµ»»»‹ØÏœ¹Ð³ôÁ¬ËÚ£›èÃÓöä­ëóÂ¢ÏÙ“·…¬‰‹¾¼åÏÿÞ²¦ªèÃÛç“·¬‰«úð
³öä­ëÂŠ¾¹¼½ùËš°î¬¯„Ìµßþ™èòÙ–µ®‡ö¡‹àÙÖŸ†²ì–µ“™ö×Ëš‰Œ»åÍÎÄ†Ý²æ¢ÃèîÙ³‘á·¬—™È°ÛÖÌä
áØ­ë¦²ìóÏÔóçÏ¯ß¾Ù¼ëí¸®ß¿ìÞ¾ñõÄ°ßù¼ùóè“³–ëÕ¥Ë—þýõî×Î”¶¦ ò»õªÛÅ°¿üùòèÓ²Ö£¾­‹øßÿûõ
×ðÿâáëìÿÝý¿ÿµµÎØöïýýïƒŸ£Øí™³ì“±Ãˆîë·Ã¾™³³ì¶ö³ì“±‘Ãîë·Ã¾™Æ³ì¶öÛ•ìè‡ÏŸÃþûý¿—µ––¹ôÿ
ë•®Þ»ò÷ÞÇ¦½ßüù±àåïï¶·Ø“ÿËŽº‡ºñâðÙÏÅÀ™Ð¼½Úö¢œƒßî«Úò°ÿÿÃ—·ÛÛŸÊö®’ü²Žž‘¦ÒØÖ®¿ýüãŸ
È×‰Ùïýó«¯ÛÞ“žæçã±Ç‰»˜ŒÕƒ‡¶ª†±Þö€Åº‡¿ßýýÏ£Ïº² ß¨îûû«›´¥àá‡ô¸Ð·ïëÕëÃ´ò¸à°«ïàñÅÔþ
ûò¿ï¯ïöÕûæÞƒì¸›¸¢ôŸ§ì¸å©ÿ•êí¿Ì®»èÆõÃÝ¡Ã¶ªžúŸž×ª„¬·ØñŽ¤ËÞÆ¥áŒÆÒ‘ÿÜÿî•à¯Îº¹‹€
Æ–°È®ââæ¨îŠÝ€Ò¬æ…ŠÈ¹‰Ï²ôÐ–¡œÕÀ•Ñà±­Â¶ƒ¹“žÇáØˆ¸ü«ÜÅæÞ¼Ìˆ±òòîù¸ŽÒç¥åø‡Û¹ÞÁ°ãŽÄóÄ
ÚíÒõ¨½âµ ÕÅ·…›µÀƒÅº”ïº»‹Ù¸œ±ÁÚ¬š÷ê†ª´Š¹Ä§ÕûãÝÊð“éÊ¼ö¡øúÉ§¥Öç“ÚÇ’°£€ù§«Í‹Ïß›‚Ê
ù‰¬ô‹éü³ÇðÑõÂäìë™¯®£ý´¼¯ˆ„¢ñìåºšÈŸ±…ºöü’¥€è¬žŒçÀ©€Î¦‹‘³ÃüÆÌ™‘ÕÔà®æÅøñüó²ˆ³ñ¤’¤Þ›
“Žòàƒ—ˆÿ§»ò¶ˆöªüô“¼¤¼³ÉÑ•áíï©ª¼º©ˆ–ÝË˜…ùïˆÓ“×„Ÿ­ô¿Û³èª›±¬õÊ¶îâÉ¸¡œ÷§Ì„í™Žë°øç
øÖáÚÂÙÝ°’î¨¾÷°åà¾ïº¼ÂŠ€Û¹Ýðì¨œ…õÜá‰”¸Ü»§†«€Ÿø›õ‘çÝ¡ŠŸÅÜÞ¸ÝÏ•À°šëÏ‡¹Ùˆ¸ú×¬×µ
øÞÙÓ™‘‚Êƒ†Ö¸¦Þ›¾€á·ÞÐ¤·‹¾ÿãÕˆÛ¼¸¡íÕÅý‡»’‘„Äÿ Þ—Þ¦ç¢Íú˜ëí¤Ó–µõƒ‰è½ÔÐ½Ûµå‡„¦ù©ŒÌ¤–
ÀÕ×íøµ´À›ß‚û¬¹ã¯ó©©ÚÌšÎä¤™ÿ¦§Èº¼‘›ü®ñâ¢¦é°‹åÇ¶ðÓûÙ¤–Š©Ž¥Ï‘”šïºæŽ­ëÓ™Õ’Ééœ™­©ü
·Â°ãþåïë™äµ‰œÜù«¯ªÈ™×•ù“©¹È¢ÈåÊ¦Úì–¬êëÆ¡âú¥¾ÞÒ™à¥ù¥Ãåˆå£˜­©¼¯ê˜ì™Ø£ƒ–„Í×£–öÝº
¹Ñœ¥¶ÃÎß‹¨æ°ëâ¶¡Àž‡Äú„›åïò’ìÌ•¸ªú”…°ÃØ‡Õ›Â§Œûíº­Õûœ–ÛÝðÞÕèôéåÝê¥°“œÚØ›ç²ÈëÆÞµ¢
·ÿ Á£ÝÈ–¯Øé«Ë¸®ìèŠïâáÄ¶ò°‡ôìØó¦ëµ—ƒáêáë—Ö–¼„öä¸Íøõµ÷¤ñ—ŽšÂ£§É½ÚÉêÕÒÔ·Áÿž×Æ–ð¹Á
âÎ÷©ºÑÛÙß‹¹òª’çÇ ±ûž¿°©ç‹¦º‘ûÛª„îÈÈ¡ÃíÞÜªÃ’ˆ¬©¼´šìüõ¦ë–ŽîâÀ‰×žá­ÎŸ–ýŸ‹¤•ÐóÕ‹Ž
È»Ó¶×üØ¡Òô’ÇðµƒºªêÑ„©¢á§ë›ˆ¸þâ§À§ëÉ§ùŒ§ó£ÈÔð”Ûì¸ˆë‚Õ ¾Æ„ŽÕ£ç­óëüÖñÌ°ËÉ×õåƒ«Ý
À‚ÞÞöîãèö™ÇÖ•è ¹§õàûñ¤œ–ÉéÚæÞŠê³èª›ôÈ¿âø©ñÉÿšÒ¶¦Øùä ýÞúÖá¦ðªÛ¹¨·•Ïª¥‰—Ý•¯ÿ
ôíø–ü¸É–“¼ôá”Âì¾ÎóÈŒ ¡ß±¼ÊÉºÕâñÝ·âÆ‡ºŠÙÔ‰³‚â¸ÒãáÍÍÒèª³êÔÞþúÍáºÍøŽ·âœ°ç“Äêð­Ø±Ô
ä£ëº‚êÃÅ­ª±‚¤÷ç­‡òå¸ÇÂ¨É¡ë³èª›Íºýˆ©Ö«ž™©ÒÈòŠÕå¶ïñ þûÝùôâÍáºÍÎýÿûª¦Ë¢ºæÔï„¢ßÚ´Í
õæÄï‘òÓÔ¹—¦Ã—™ùìàÓäÂâœ´ÝØ­áç£â“öŽÖûŽ¯½ÅØ›œ¥î´¼Žå±ÛîÀÇÓ¡Ïô‡›ŸÊ¶Ž™çëÝöˆ³ä ´Ä–
ÊŸÈÌÆ¦ƒûƒª¬öÐ¿çÛž«ìôŽÖÚþ¯¤¿îÕ·›â†½±öåË³˜´¯ñŒ™öºÛ»™öÉ˜á··µ›áŸŒ™ôöÛ»™öÉ˜þá·µÛ±°×
–®ýëàùê¾³ÎùñøŒ¯Ê±¹œõÄé¢¦ÈÉ²ÖæÁ÷”Ÿñ’ÃÕé•£üºß¥´ÔÑ·Ë½†Øµõš·Þ”†Ô®öÖíÚ»áÊÍ¯ƒ§óç¿É
¦ýØË–‘Ç¬à¦ßŸòë‡Áóÿðá¹µ®Ãó÷á¸ûéÐèÂ‘×¿¦××ð¤ŸàÄˆ…ø“É¢‘ÎÓ„áöÚ›§£ÌõóóÚž‹¯¬¹¤ÝÞ¯Œ
¨öÍ§Óåìš–ÖŸÞøöß„±Ã×ÆâýÊéªÌÑÕŽÉ“ù¶¥¾¨¯ëÎõ¡·öØ·ûÃŠ¤äÈ‹†ÝŽ˜ÔÇ¹Àü“Ð–þÞ½ö…È÷¡šö˜§
è¨óò£¾ÔÀè¹éìØµŠƒóŽø¸×¶¦ÿÅ¦ïÊèò™ÅéË¼³‘ö¾”†å‰Åã²˜º¶²ø‰ÃâîÞºàñ„éŠ¯°„ˆÆÝÉ¬œÐæ”ÄÄ‰ò
¹å¬‹Ì˜¼²äÜµ€íì¨œ€ºñ•›ˆé§úâÙ˜àç‰õÆÕÉÜò“Ô•ØÛõéâ…ÛÐŠò¦À„ãÒÝÈáï©¢ž¾ÂÎëÜò·¬™¿®ºÝ²¾
Œ¤÷°¥¾Ä¨˜ÔŸˆæ†Ÿ€ÛŸÎ„ûþ½–ù²šäÎüõÂŸéÅ«‚§Ãíó–Ä‹¬çŠÝó¡“…¥ÕøÇ‚Ÿ¦Æ£‘¦´–‹ƒèôØ•Èž§Ò¢à‰ì
îÚÍÄâé±ªÏÊÉû…ÞÕÊØžø‚¬°©ÙÜ¾Úò£‹¥£ÛŠäÙ€ÆÔ²„¤ƒ£ø¼å»ø©ë…¤øÍŒ’­ŒÒÔáýô‘…ÅëúÂáÀÈàÇÑÙ¼¢
øšÏ‡áÉì²›½£ï‰ˆÐÞÊö¦º‚ÜÈÿß…Âú „„öõˆÍ÷Ó¹ëèØÂåëÃ¬þ‡›êÃ„âÚï±®’‹µâ¯é´×‡ÕÁž”¯—¯‰ïÚä
”Ä·«–öÚ·Áž›±þÜþ¸±­»åÍ‰›‹±©ì½éœãÔò«þ¼Óä¬„ãÉãúœê­±ÔÆí¹…Æ¬øÙ¤õžºëüúèëÚîÍÚ¨àç¹
•Äš®Ó¦×Ÿš‡ƒòããòÛ­Î–´Ý¿…¦úÀ¤Ô•†¶ÁÞô·õÇü”çÉúßÁÇ–úÐ–ãâÂÆêÚÔŸò®»Ö×¶‡Ý”ù±Ø¥Åá×
â‚»šÆÝØ½ÙÕÇÁÎŸ‡ïþ÷€ý²ï½¥Úàä‰ÍŠÊá§Øþóã«Ú®–ØÑáùËôñÚàçí²Á°³ûÊëÖóµðÅãÍ‰þ€²ÛÊŸê´
ßß…Âú°ÂØ÷»Õ‡–ØóúñÐ‚»Û˜•ÃûŽ¬Ù†ø€Ž•Þ¯³€€œ¸¯ðŒØ„¦°ë·…û±è¢›ô¯Ô™¯ŸíÄ¿‚ÅõªòšâÍá»ÀÎþÇß
úæÌä„ÛîÂ®çç²š¼®Åüœéòä‡â”Ü­¹ÇæóÑçá˜ˆ¾ÚØ¸‘°Ç…Åòû‹¯°ðÒñ…É¼©‘œ¾Øâ­§‡Üò¤—ŠëÃ‡ØÙ
ô±´íöÉúïßÁ–úÐëÆæ£„‹·ë›Žì¥á¯‚©çÿ¤÷¯üÃ£Õùê«ãè´‹ªûÊ§¾¼›ëãÇƒ•þíçÚÆ•òü™¼Ì´ïúõ–½Êø
»øÞâü˜—§±žà¬ùü¼Þ´¬ùòñÝäœ©ã—÷ÕÂêãÉ§ªçò™Û…åÇäÇ†û¼¤ƒñ²·ýå·ù£õ–Ü®Þ“ãÑïÂáý¨ýõ¹Öò·ÙƒÇÊ
¹»äý¨½ƒÍÑõÁŸÆÖÑÉ¬çò­¹õ¹˜Ô³ÈÛÆž°¬ñ¸·ÎÆ€ãû”ìç½ÿî”äÀë€ûÆ“ýÑâýÿï‰Õéšáß­¥ÞÂÆ„ÌÓå§ã†
‹ÝÖî†ý²æ•ØííæØ§ã‡†ÝÖî‘öë˜‚ÚŸìÖ„„ãÒŸ¿ÿþñâ»ÅÝÉëã¿×ÞÙÙ™º”ë»Õ¥Ë—Þ¼ùý³òØ¡ë×¯ØýòëÕçŸ
†ýöÑÇŸÝ¯Šµ«¼¯ØèƒÝ¸ñãÿÿÿ¥èÒŒÈ§‚ýíòúòä‰ú‚É¡Ëá­ÆçÎ¯œñÝŒ²ìÑ€ðÝðÁ°à™ýº Ö²‚÷Âî±‹ï¦äØ¥
·ïßž¾ýšŽ—Ä¾ß„„Ÿ§ž°ûîÊ†Ý­‚Ïêáßïú‚‚ï»™È½°ËôáíûÍ¦Àòáý·£Œ¸»¥ŸÞ¾ß„ä¤ŸÞ÷³Ê°žÓòáíûÍÀ
“òáý·£Œ»È•ýøûþ›­ßÄ‡÷Ý”ŒûÒŸ™ïßï£¿•øð¾›’á½ÿ£óáíûÍ¾ÃðÖþñÿ´ŒÝŠ‘É°ÇÝÁƒÜÝÝº’”ÖêäØ“™
Åö¨Ã¾¥™öšÄ†½Ê°ÏÉÕ†½‘á²ìâÓ’áÏäØ£–Œû”äØ“™Åö¨Ã¾¥™öšÄ†½Ê°ÏÉÕ†½‘á²ìâÓ’áÏäØ£–Œû”äØ“™
Åö¨Ã¾¥™öšÄ†½Ê°ÏÉþÜ¶Õ®ô‡èÃ×ÒËÚ‹¶ËÜ³ÛÖ¼äØ°­ë¦²ì–µ¸“™öËš‰Œõ»åÍÄ†Ý²Äæ¢ÃîÙ³‘¾á·¬™È°Û
‰ÖÌäØ­ë¦†²ì–µ“™ö×Ëš‰Œ»åÍÎÄ†Ý²æ¢ÃèîÙ³‘á·¬—™È°ÛÖÌäáØ­ë¦²ì–ðµ“™öËš‰ªŒ»åýñôáÃáÛÓ—àŸº
Ìˆë×ƒ‡†ý®ÊÖØµ»»ÛÛã·Œ­õéƒÜ¹óÃ×¥ë•ì‚ìçÏþÐçýó•íóíÓÏ¯˜öëØ™•ö‰˜á·ÊäÙØ§à†Ýª‘¦áŸ€™ö«Ä
±†ý‚æØ­’í­œößþÿå·àéã—‡Ïú»®ð÷ïÝ¹úííÊËÞ¯¾¼›¸Ö‚à¬Ïô­•Ö˜µá·Êõ´ÚØÔööñ‡»—¾Áß¯×¾¾±Ö¸’Û
ûûæ–òí¶÷ðÄÖ‹µÚ™­×­­ïÿø¶¿äš²íÅáðÓñ°Û½ë…°ƒÖîÔá×¯±ûž½©ù“žç˜óÎ¦®É¢†ÀµÀë—ððÑýÃî
Ý¥•Âž×öÕúó´€¨ç¨äô°ùøˆÁ¡ Å ©à £©á¤ºêº˜ˆ¤óòÄ™¹Á™áõ±³ÅÞ¥“Œä²˜¹©ñ…ãÊ¬È©šµì°ÒÙƒ¡—¶€ø
à›ÓÎ®ˆöâÒ¨Ãî¥•ÂƒÎÎ«Ç†•žÊôà¯‰Ó½ÇŒÏø¿ì“ù”¡©¡Êšˆ µüÐàŽ’»Ç°”‘ŽÇšá½°“äºô® Å¥¢£Ä…é
ò‰‡¡çÈ¼‚´îÆ´–Õ¸è›öë„Úí˜„õž¬ÄŸ¾½Öùô†¨çÄ™›õ¾ºÄñà—ûÓÏ¿æÄ€Ä·‚Ä´êˆÇ¹°‡ÍŽÌò„Ò™˜È¬ªù
¤ìŠ¯²·žÅº¢øœÈÔž¸°“äËâØ­ÁÏ´Ž§ñüÚ¥®çã‘”êÑãºÖÙ¸š²‡½§Å¬øŸñê˜¨ô„óìÙ±±«Ž„’¼ÔÆ¤ò¨œñ”£
Þµ…ç°ƒâíøÂ¡ñâêñéî†Ð£À¶š¯ÓØò˜ö«ç­Áé½»ÝúÖûäœöÒÑ‹»øß¢éèÞ‹ö‘˜ìÌÝ½»îÙŽÓÛá§¯Ô®£’Íá¹ì
†¬›Æ‘É«‡ËßÄšöë‰­‰öÔðÔû¨Ñ™ßÃ‰´„üÀö‹ùƒîÄù£–ë±·­Ž®Üèäñ ‡ŒÅµù©ŒÉ¢²“¦ð„ö–™Ã¸ú°èÉ·
š½±´ìÖ Ú–¡ìÑóæÃ­‰Õùì³ç„—™ñÔþ®ªÎÃŒá¡ñìÎ‡·¬é™¸â°„µ¾ùïïØ”ÉÎ§ƒŠøéÕ´…×ï¾…ÌË†éÕâˆÏ
äƒ»µª­üþ½»†¨êþˆŽ°‘¤ð¬œàÓ€Ðñ¢Šö©êØößÊ™Í¤Šãò±ûµøžúŸ¤äÅ—¢½¢§ËÆÖù©¸Î“ÅÈ†’æÉ“…Üê§í
°™öë­ã Ý²Ä•‘åó–Äó˜­óïëæ÷åÅ¸éŽíëá£²Ãî•¨»‡…°µÝŸí­…æØ­’õ€´»üÚü¦íÍ…æØ­’ù‰ë„Ì°Û¥²
éì“°Ãî•È•°ÏÀŒ»Õ¢ÑÃ¾³ìÖ‰ÿ–ýá£ûñÉ›§•ê¥‹Ýöê•ÊôÌëýéäø¹îáª´™á·Ê´Œûìœ€£Ÿœî›¢¹êØˆ¸Œ
óÈÂº¬ÎíÑ®‰žôû˜¸ ´«ùïØ­’þõ†‹»ã·†â„·ÒŸñ»àÄ³Í®¹úåíŠâ§ß³á‡—â°Ëˆ°š®ç¯á¯ÖŽ³ÂÁíÂá˜“¾
ÊÄÜÖÊð³˜ºòµè…ô³Ú¹¼œäù™‹ö„¥Åâœ¿ÎˆéÔÊùÎ“Ö„Óºˆ°×´ƒ™˜¸ŽáœÀÍˆžÎÖê™öëÓµêŒ»ª±ŒµÈÇØ•’ÀŽ
µ®…ˆ¶’êÍ—öñ—É©µ€¢£À‡ÇñÞ»©þŠó€ÃµéµîË¸»žöóê´°‡Ëè§ªÅòö”‰¬ò×ÁÉŠÄ’É“Óìà¨¦…³Ä¹êÏÔó’
Ôñ¯¿ø“¤Ž†Ã¸ðŸ›±ÕÃî­Éá×áêÑíð®ºøÃÍÝøýÜÂíªƒªúœÜ‡ôÒØí™¡Ž‘âëŠ×ê±š…½°¡Ÿ”¨ß¸­’Ã›€ÓÙ°
•¤þ¼Ç«Ûä™˜‹”ËÎŸ¶ì‚Ø Ä½¼Û–­Š’¢“’Î§®’ÄôŒ»Ùµ¦õƒë»‹ˆ¶ºÊäÈÞô‚»»¼¥ÉÊôÏŸàØØµªêÔ‡ëªÞÜ–
ÿþËªö­œ‹“Õ÷¨Ôí¤œöØÄÔÂ—®Õ¯ùâÕ¶ƒðÏ×†™öë£…ê†»ª­Ðç’ßÞŽû¤Œû‹ÈÈ‡–«È¬Ñ×ÏŽ„éÝ¡Ó‡–÷œòó
Ú—½ú¤Æ®˜îß™Ð¶ò§·×Œ£˜©“Òƒ¢ ØÅ·¬Ž÷¢³ð­®å˜‡³¹ŽÈáÇî´æÈ˜ÉâÕ¾âœÂ±‰Ò±ìÖšÔöþì°¶ù™ø¡ù
¹«·¨¯¼¦åÎ¶½ú„Æ¯¯Â’ôêèäžµŸ‡ÆÏ¿ÆÝúÉ´˜™É²ÃÙñâ³üÞ¸¶©’¿…‹ƒìùöü¹÷ô¹ÌÚŽ½“„ùøëÞâû¶Éâú
™²îÌÇ°ÛëÙÒŒ§ÕäïÍª¶ÝŒ»Õ¢™ÇÂž¸¥“°ÃÚî•è†°ÏÏ’Œ»Õ¢Ã¾ª³ìÖ‰Œû„ãÌ°Û¥‚ìïëýé½Ô–ÛæËÚ©Óï›öë
ÞØíïïïÚ›Ã¯›·ïŽÝ•ÖÆ§ÿ‡ß»Í¨
]&]
[s0;#2 &]
[s0;# [2 SpreadsheetAPI class includes just the API list of functions]&]
[s0;#2 &]
[s0;# [2 Spreadsheet class implements the by default API that usually 
it is just a stub. However this is the class really used in the 
programs. Once called PluginInit(), this class will call the 
right API low level function.]&]
[s0;#2 &]
[s0;# [2 OpenSpreasheet and ExcelSpreadsheet include the low level 
API and related private methods and variables.]&]
[s0;#2 &]
[s0;#2 &]
[s0;# [2 In addition to this sample, StaticPlugin is used in ][^topic`:`/`/OfficeAutomation`/srcdoc`/OfficeAutomation`$en`-us^2 O
fficeAutomation][2  to choose between Open/LibreOffice and Microsoft 
Office to access spreadsheets and wordprocessing files.]&]
[s0;2 &]
[s0;2 &]
[ {{10000@1 [s0; [*2 StaticPlugin functions]]}}&]
[s2;2%- &]
[s3;:PluginRegister`(a`, b`, c`):%- [* PluginRegister]([*@3 User`_class], 
[*@3 Low`_level`_class, name])&]
[s1;%- Registers [*@3 Low`_level`_class] with [*@3 name] as a class that 
implements API in [*@3 User`_class. ]&]
[s4;%- &]
[s2;%- &]
[s3;:PluginInit`(a`, b`):%- [* PluginInit]([*@3 API`_var], [*@3 name])&]
[s1; Initializes [%-*@3 API`_var] (that has been declared as [%-*@3 User`_class]) 
with [%-*@3 Low`_level`_class] of name [%-*@3 name].&]
[s4; &]
[s0; ]