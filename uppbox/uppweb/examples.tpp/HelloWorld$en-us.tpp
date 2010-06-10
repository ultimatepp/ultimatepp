topic "Slicing HelloWorld U++ example";
[2 $$0,0#00000000000000000000000000000000:Default]
[l288;i1120;a17;O9;~~~.1408;2 $$1,0#10431211400427159095818037425705:param]
[a83;*R6 $$2,5#31310162474203024125188417583966:caption]
[b83;*4 $$3,5#07864147445237544204411237157677:title]
[i288;O9;C2 $$4,6#40027414424643823182269349404212:item]
[b42;a42;2 $$5,5#45413000475342174754091244180557:text]
[l288;b17;a17;2 $$6,6#27521748481378242620020725143825:desc]
[l321;t246;C@5;1 $$7,7#20902679421464641399138805415013:code]
[b2503;2 $$8,0#65142375456100023862071332075487:separator]
[*@(0.0.255)2 $$9,0#83433469410354161042741608181528:base]
[t4167;C2 $$10,0#37138531426314131251341829483380:class]
[l288;a17;*1 $$11,11#70004532496200323422659154056402:requirement]
[i417;b42;a42;O9;~~~.416;2 $$12,12#10566046415157235020018451313112:tparam]
[b167;C2 $$13,13#92430459443460461911108080531343:item1]
[i288;a42;O9;C2 $$14,14#77422149456609303542238260500223:item2]
[*@2$(0.128.128)2 $$15,15#34511555403152284025741354420178:NewsDate]
[l321;*C$7;2 $$16,16#03451589433145915344929335295360:result]
[l321;b83;a83;*C$7;2 $$17,17#07531550463529505371228428965313:result`-line]
[l160;t4167;*C+117 $$18,5#88603949442205825958800053222425:package`-title]
[2 $$19,0#53580023442335529039900623488521:gap]
[t4167;C2 $$20,20#70211524482531209251820423858195:class`-nested]
[b50;2 $$21,21#03324558446220344731010354752573:Par]
[{_}%EN-US 
[s0; &]
[s0; Historically almost every program language or framework have 
some kind of `"Hello world!`" example and Ultimate`+`+ framework 
is not exception:). So in this short tutorial I will try to explain 
you the basics of U`+`+ framework with the help of HelloWorld 
U`+`+ example.&]
[s0; So when you start HelloWorld application, and go to `"File`->About`" 
you will see somethings like this:&]
[s0; &]
[s0;= 
@@image:2162&1572
€ƒáƒõ‚€€ƒ‹ý€€€€ø—œíÝ÷˜ìçîÝÀñùÛ±Ä®Øã—”´ÖÂÐ¼Ê¬ ˆ¨µ‘æÁ–•ÞïÂ¨Ë‚¤¡¡Œ„Ò¸¡¤¤¡óÐÃ§”€
ŠòÑ†ÀÐ”öû¿ò¼þáŸÍë»ûî½¹ùù³çÞ™Ý÷æóû¾ÏÿžÙÙÙ™³óåÎóÏ·çÊŽ®ïúð–±Æ˜ÂãŒ±ÛÛÇŽøœ¹ôèÐø¦
¦·“±Ÿ€¤€à“õ“ñÈÕñ¼«ìÎª›ï³ËæÙåóœ™²ùîÙœÓ¶çç–Íùåó¼Åâ¹¿ì®¨›÷ç—Í‹Êæ…¾åó¢²¹°ìàÞÜ¶¯©›‹
¿Êæââ¹¤ìü®­›—–ÍÑ‡²¹¬ìÞÖ¶´¯¯›×”Í«¿Ëæòâ¹¢ì°ÞÕ¶×–ÍÕ¸åóõÙ¼ºì½®©›×”Íë¿ËæÚ²¹®ø¨Þ×¶¯¯›·
›”Í›ËæÍå§óæ²ùËÙÜðß¶ï­›·•ÏÍÛ‹çåóóÎ²ù×Ù¼»âìÞÓ¶·”ÍæûËæýåóþû²ùÀÙÜ¸‹ƒ‘ˆ’Ñ¢ÊÄÑ‰¨‘¥¢Ä”
Áˆ’Ñ¢ÊÄ‰è¨‘¥¢Ä”ˆÀ’Ñ¢îÎ¢ÎŸõúÃÔ„¡‘éÓÿ¥¢Ä”ˆ ’Ñ¢ÊÄ‰¨¬‘¥âú¥âÜØ¯¾Œ‰øÿÿƒûï’Ñ¢ÊÄÑ‰¨‘¥¢Ä”
Áˆ’Ñ¢ÊÄ‰è¨‘¥¢Ä”ˆ°’Ñ¢ÊÄÍÉ¿ÄÑö´³°ýŽÄ”ˆ’Ñ¢ÄÊÄ‰¨‘¥¢†ÄÜÀÄŒÍ˜¡ŠÑ¢ÊÄ‰¨´‘¥¢Ä”ˆ’€Ñ¢îæ¢¶å
ÅØ½¢¨‘¥¢†Ä”ˆ’Ñ¢Ê‚Ä‰¨‘·¶‘šûŸñ–ˆ’Ñˆ¢ÊÄ‰¨‘¥ý¢Ä”ˆ›œˆ‘Ž¢ÊÄ‰¨‘š¥¢Ä”ˆ’Ñè¢ÊÄ¯Å”ˆ ’Ñ¢ÊÄ‰¨
ô‘¥¢Ä”ˆÓª’Ñ»š¥¢Äƒ”ˆ’Ñ¢ÊÄ±‰¨‘¥¢ÏÅƒ”ˆ’Ñ¢ÊÄÑ‰¨‘¥¢Ä”Áˆ’Ñ¢ÊÄ‰è¨‘¥¢Ä”ˆˆ’Ñ¢–¤âù‰ÚÉÄ‰¨‘¥
¢Ä”ˆ’Ñ¢’ÊÄÍÈÄÃóŽÄ”ˆ’Ñ¢ÄÊÄ‰¨‘¥¢„ÄÜûÄÜì¤¢Ä”ˆ’Ñ¢ÄÊÄ‰¨‘¥¢€Ä¬ÉÄ€€¶áœÄ„€ ˆÀ€€„’‘€€
€À¢‚€ÈÄœ€€‚œˆ¹Žð‡’ñôûÉçâ´ÿ…€àäÎÄÌ§‡“ñ®™ó€¨—Ä„ € Ø¸‘Ã“ÏÍíç €¨èÚù¢†ˆœöÅŠÄ•ˆ€Ð»
Ÿœˆé¿ãÝœÎÅì¾™…€€ŠúåÄÌ‡’÷›“ˆ‡þ‹€€¥°¬åŸúÓûåŽ§š€ÖÏ˜Ò’ñìíª€„€ë¤°‘€‰Øœ¥‰¸šíÌ…‡¾Ù€€ì
€Ç‘€ˆ¤ˆ¢€€Ä„€† ˆ€€„’„‘€€À¢‚€ÀÈÄ€€‚‰€ˆ€À ‘€ ˆ¤¢€€Ä˜„€ ˆ€€„’‘€€À¢è‚€äÄÌ§
€¥¢€€ãÇ‘„€èð”‘€€À¢‚€ÈøÄ€€‚œˆéå¿ã‰ˆ€À¶§œˆ×ßÿýØˆ¢‚€°Í¢‚€€ÈÄ€€‚‰ˆ€À ‘€€ˆ¤¢€€Á
žÎÄÑ‘ÑØ¢Œ‚€°ÍÑÄ€„€‚‰ˆ€À €‘€ˆ¤¢€Ž€·«€€‚¸Šˆ€À ‘€€ˆ¤¢€€áÄ„€ ˆÀ€€„’‘€€€À¢‚€ÈÄ
ˆ€€‚‰ˆ€À€ ‘€ˆ¤¢„€€Ä„€ ƒˆ€€„’‘‚€€À¢‚€ ÈÄ€€‚‰ˆ€€À ‘€ˆ¤¢€€Ä„Œ€ ˆ€€„ˆ’‘€€À¢‚
€€ÈÄ€€‚‰ˆ€À ‘À€ˆ¤¢€€°Ä„€ ˆ€ €„’‘€€À€¢‚€ÈÄ€„€‚‰ˆ€À €‘€ˆ¤¢€Â€Ä„€ ˆ€€„’‘€
€À¢‚€ÈÄ€€‚‰ˆ€€À ‘€ˆ¤ˆ¢€€Ä„€† ˆ€€„’„‘€€À¢‚€ÀÈÄ€€‚‰€ˆ€À ‘€ ˆ¤¢€€Ä˜„€ ˆ€€
„’‘€€À¢€‚€ÈÄ€€‚‚‰ˆ€À ‘€€ˆ¤¢€€áÄ„€ ˆÀ€€„’‘€€€À¢‚€ÈÄˆ€€‚‰ˆ€À€ ‘€ˆ¤¢„€€Ä„€ 
ƒˆ€€„’‘‚€€À¢‚€ ÈÄ€€‚‰ˆ€€À ‘€ˆ¤¢€€Ä„Œ€ ˆ€€„ˆ’‘€€À¢‚€€ÈÄ€€‚‰ˆ€À ‘À€ˆ¤¢€€
°Ä„€ ˆ€ €„’‘€€À€¢‚€ÈÄ€„€‚‰ˆ€À €‘€ˆ¤¢€Â€Ä„€ ˆ€€„’‘€€À¢‚€ÈÄ€€‚‰ˆ€€À ‘€ˆ¤
ˆ¢€€Ä„€† ˆ€€„’„‘€€À¢‚€ÀÈÄ€€‚‰€ˆ€À ‘€ ˆ¤¢€€Ä˜„€ ˆ€€„’‘€€À¢€‚€ÈÄ€€‚‚‰ˆ€À ‘
€€ˆ¤¢€€áÄ„€ ˆÀ€€„’‘€€€À¢‚€ÈÄˆ€€‚‰ˆ€À€ ‘€ˆ¤¢„€€Ä„€ ƒˆ€€„’‘‚€€À¢‚€ ÈÄ€€‚‰ˆ
€€À ‘€ˆ¤¢€€Ä„Œ€ ˆ€€„ˆ’‘€€À¢‚€€ÈÄ€€‚‰ˆ€À ‘À€ˆ¤¢€€°Ä„€ ˆ€ €„’‘€€À€¢‚€ÈÄ€
„€‚‰ˆ€À €‘€ˆ¤¢€Â€Ä„€ ˆ€€„’‘€€À¢‚€ÈÄ€€‚‰ˆ€€À ‘€ˆ¤ˆ¢€€Ä„€† ˆ€€„’„‘€€À¢‚€
ÀÈÄ€€‚‰€ˆ€À ‘€ ˆ¤¢€€Ä˜„€ ˆ€€„’‘€€À¢€‚€ÈÄ€€‚‚‰ˆ€À ‘€€ˆ¤¢€€áÄ„€ ˆÀ€€„’‘€€
€À¢‚€ÈÄˆ€€‚‰ˆ€À€ ‘€ˆ¤¢„€€Ä„€ Ëˆ€ÀÞË±ŽñàÛÀÜ¤â÷Õ¶Œ±Æ˜•±Ÿœ›‡¾ÓäÔ®’±ê–Œ€ÁÕ§‘ë¤‘«
ÿ–—ì‹¾ãá¯þÎä‘“Çßõˆ»Ì¼òîŽßýÈ¯Ùñ¨¯ùžÇýéÿö½þ²ìûã’Çüùö§Ç­œô§ÇÎÜùýâ«îü¸¯øçþ‰¯Ìþàñ
ÁÙ×¥÷ùÂò‹ÕÉÝ“§¦ßßóƒÙ“¾¶ùç¡ìÉß÷ÊŽ¯¯ÿáäðö¾¿’œ‘ÜñÇÿ³§ÜñÇžò·Ù¿õ›šö¿ñ´ïÎîöÝ´ï¹ÛÓ§î
Æ¾ñ­÷¿óâž§Ïü¶‰çüùÛÏÍœœÜã§™É·§¿œÒÉ¾£¹”¼ëþ»“ŸÉÎú®ßŸÝñÝÉ½ÏÿÎ¾çÔäÙÉû÷þÜöœäûØþ¾ûîþá
ù„ß¼çÎÙ½÷Îùþû»ãÿÞçþÀ½Ï›ïø¥óî²ñ¼Û»Üçâ®“çßßõ¾çÿàö±Ë—¤¿”Ü¯ºùþ©Éý³—üðò€‰ŸÉ¾þå…Ùþê
Ÿò¢äÇ’ßËø®Ìþü´ìÅþ¿ñë—Üí½»îþ ‹öî¼ø¢ŸüðÅœ“¹ø§¦®õ™øè¥÷˜¸•ôž¿ñéÏç¯¿ùÒäÐöÛøÇÿ¦ûíä²
áŸÍž¶ñÏ‰‡—Ýó÷¦ÎÍÞþ÷åÉÏ¥¿û—¼¢ùùßÏÊÞù‹ÉŸä—÷ÿâé»îõ‡ì×´îýÇ¯ÊŽþé“«§žòþåý¦®ºÏþ£®ºïŽ«ï
ÇûÈ«ÿ¹ûÔç«ï—½º¹æëþÙã®ùÀÄœëžðØ‰ßÉîž÷Úä×³Çöß›üÚò„äçºÓ’§æ¯ûµõ§íøà“ÞëÿÀ§ïøÐáŠ·ì¸ã¾
÷ãÙÃž’¼æéâ©ïúèÄ­›Ÿú´·ÿÆ÷öô·¤¿™ÒùýïåÏÈÞ‡úÛÉ³§ž¶÷ñ¶‡™øäÙïÏþ·¹è«ùÇòûÉÙÁÉ»ÿ¿ùöö
ù®¿øÎŽÓŸ÷ûîÆŸžóž§ìÎÍî˜¸Ëï†¿žøïòëðçåïûøù±ïûÄöÁûŸù™½¿ùÀ£²Œ—üàÑ“·¾Ëú…“ÉÞôŸÁä±Ù…ŸÊ
öž—¼¸ù“ÇÜ§¯Éþô‰ÉøÅ“Ï¼èÏžæøñŽ§Ýòç“Ù“¯Íþ¢»ŸüÒäÃÉ™Ç×³œñÙÇž§²ã£ÏýÙ‰ž¯ÿØÓ²×ü‹ìé“Ç¦Þù
ßìÌ‰Ÿùù÷ÇŸ‘Ýñ—É‚³“×¥ÿõ¤­»²¹éÙÉÕø“çÝõ‰³®íÞñöÕ·Ÿý“ê‰ç§×ü²êûÎë’ñòÜµ×æŸÊÎ¹¶Ï»åÜëÎøÝ
«§Ï»áù¯Ï¼¾³ñ†Ïœ¿öñëùï¼õ‚®Ÿ½àÍŸýÎþöæ¿þÁö“–äïÞ˜Ü¿çñ¢ë¿·ñÖÝ‰‹ßú·—¾¾íâÅÉÛÿ®œûÉòŽÏ§—
ê¥ïÌþ¾¹øòÝÙ¿ÜòîÆÌ¿Þúž¯½þ¡ù§‰÷þÓƒÑ‰ÿÎ®ûßö¯—½ÿßÞå–ýàßßÞ¸ëñßÞ‘ýðßöÞùÁÿŸøÐûÄåŸúËÿ
þäâŠ‰ÿ¼â¸Ï§Þ•üÙÿ°¥×¦ÿžü÷ôòÕò—Éñ«ù“çÿóêÏì¸æ£ßØØ„Ø¢ÖÉ¢ÖÍˆ¢ÊÄ‰¨‘¥Ý¢Ä”ˆì…‰ÄØµ‰¨‘¥¢
¦Ä”ˆ’ñß’öñ®×Ÿ¦ºíˆÕ“ˆÕ“ˆ ’Ñ¢ÊÄ‰¨‚‘÷³‘‹÷Ï‘¹®¤âÕ¤â¹Õ–ÎÄü¿øÌ©‘ó‰½ÍÄ°ü“¡‘ó™’Ñ¢ÊÄ‰¨‘
Ä¥¢«É¢Ö­Ûˆ¹ºšÄÌ’ßî²‘ó÷ó¦¢æÓ¹‘óéÑ‰¨‘¥¢Ä”½ˆ››ˆ£íé¯ï’ÚçÈÄµÄ ‘«¦‘¥¢†Ä”ˆ’Ñ¢ž¥ÔÑÄö“†â
Ã”ˆëÀ¢Ö­Œ°‘Û¯ŸžÈïÄöÅÊ“±ýØÓ¹‘óéœ‡„¡‘·î¿’ÑÈ¢ÊÄ‰¨‘ë®ÍÄñ‰²ìž†Ñ”ˆëÀ¢ÖÇ­›ˆáÒ¢æâ“ù¨âÓ‰Ý
úÄÜ©Ãí£ˆ‘ùôþ­âÎ¥©¶’ñ§·ß»‹˜Ïç£ˆ»ñ•ø¢‘Û¡˜„“±Ÿð”Ñ¢¤ÊÄ‰¨‘×¦ª‘ÇýÏ¸¥­âÚˆÕë¦’â÷¨â·›—
îÈÄîÌÌÄî‚“Í’Ñ¢ÊÄ‰‰¨‘×¬‘ÐÅÜã’±êå‰¸óµˆ³“†±¿£Ù¢ÊÄÑ‰¨‘¥¢ÄÌé¼–ñ½ÉÄªÙíÃ¢¶Ÿë¾‡íéæ“Ÿú£
´‘¥¢Ä”ˆ’èñ³“Ñ»š×ÅžÄ¬ÚÉ¢†ã·«ìÍù¢¹Ã÷íª³“ñø¥íªÝ÷´‡­ƒ‘ˆ’Ñ¢ÊÄá‰Øé¢Ž“ˆ ëÀ¢ÖíËß†×‘ˆ’Ñ¢Ê
’Ä‰¨‘Ùƒ’±ê’Ñ¢ÊÄ‘‰¨‘¥âþ¦ÿâ‘â‘ˆµ“ˆÕ“ˆ’Ñ¢¤ÊÄ‰¨‘÷­¤‘Í¹’±ê‚’±ê’Ñ¢ÊâÄ‰¨‘¥â¾ª¥ââÆ’±Î
‚’±ê’Ñ¢Ê¢Ä‰¨‘¥¢Ä˜ä¯ÈÄªÙ²´€ÖŸÄ¬‘Ä£¬š¥ƒàõÉ™ÄšÉÄªÙ²´€ÖŸÄ¬‘Ä£¬š¥ƒàõÉ™ÄšÉÄªÙ²´€ÖŸÄ¬‘Ä
£¬š¥ƒàõÉ™ÄšÉÄªÙ²´€ÖŸÄ¬‘ÄÅ¬Ú½Ë¶²Æú˜³²³×ÿæòËÄšÉÄªÕµ»äéß¤ö§ÀþƒðÐ¤¢½Ì¤âÕêÝ²‰¢ˆ°¢¤¢½¤
¦âÕêÝ²‰ˆ‘°¢¤¢½¤âÓÕêÝ²‰ˆ°È¢¤¢½¤âÕ©êÝ²‰ˆ°¢ä¤¢½¤âÕêìÝ²‰ÈþÚÚŒšõåóû¯Üöþµ‹íÌß’¡Ð¦‘é¥‘
«Öï’ÉÄŽöÄÈ³ý«µöÐÉÄÖÄ¤•—Ä¬Ú½Ë¦ ‘¹’‘º¤²¢½¤âÕöâûÉŽÌ™åÞŠÊÄäÀô¦âóÕ¼ó÷²íç¥»‡¯íà’áÛ
Ù½·ºó¶àï°ÈÄúÉÄª­öñÉš”¼ñÆš›·º“ÎÜîÀ«ÊÄŽÄï•…ïõÏôÿðÝáËÎ¼Ñé×€ûÇ¢ÒË¢ÀÖí©Ë¶Ð†îƒ­¸Ë›•ˆ
öœˆ§š‡³ÎíøÚÊ¯¹ó”ÆË®–Ë¢¡ÒË¢Öí÷ËìÖž‡ÝÐÜÍÑíÊÄŽÄÜ‰ÑøÅËú¢ùÚò…›ï¶†Èý£‘é¥‘«Ó¶Ë¥Ë«¿­
ü€Ç£­Æ´†§ÜÍ¿ª’‘ƒð±ï¢ÎûÁÝ™œÙþ£°¿¤Ò¢½¤âÕö³öäÓúð»ý¢Ëé—ÜðŸ–‰ûÈØø‰æåé¾–ñë€½ä£‘é¥‘«¶
Ûð’ôáÜ‰Á¸ð¥ÊÄŽÄ”¾Ñ¼³†½ûËÌÁ‘ˆô’ÑˆÕÛìÉæ¾ÿþ°ð‰æÖ…ÇçþçÅ¢‚¬Òˆ‰È¯‰Øµù…“ñè°àü»“þù™÷Ý«
€’‘àÅÈÄúÖÉÄª­°ä‹ó½…ùøòÄ¡¹ªÑ¢‚¬ˆé‰È¯‰ØµÅ´’ñÖ¡ÁùÞ‹øâ§¤¢ÀÉ•¤¢½¤âÕ¡ö¢‘ûÃ±°Â‘Ë«Ñ¢‚¬
Òˆ‰È¯‰Øµåª“Ñ˜ãÌŠÍÌÞï­¤âì¤âÕªÎÄ’µÍ°Æ˜³×£ò‘é¥‘«¶ÀÑ’ÉÄãŒ±íŸ‘ˆô’ˆÕÛšàÉŠßÑ¼×ô¢¶ïè–ˆÆ
„˜ÓÝÈÄúÉ‹Äª­¶ä¥‡š’çÊÄ¦¥Å¢±ÆÔ·’‘”Þ’±ê‹­ÙÉÄòÄìŸÎÂ”ˆÆ˜ÓÝ°ÈÄúÉÄª­¼äƒ‡’û¿à—ñ ”—èÃ
¹±Ä´Æ˜•™é‰È¯‰ØµÝò¬ÙÒ¢­Ý‡»ÌÄüÁ‹ƒö·ÔþîûËã×ÇŸöÚéåþ×µÄéÞôŠÙÌÜ÷æ¼×¸”ï÷ïÒ¦äëïëúÂÔ½ñ
ÐæÅÆ¢Ò«¯àÚþøìÉÄÚ•ìòÉö¹çÙ¡¸ûïúáÄŸìþà¾í´×òöï¬‘›‘ˆ‹ÆÌœ‰È¯ÇÎ‘«¶û¥Ëÿô²¬ð§›ÃëžçíÃñž§
¿â¾ÍÊî¥¸â³’Ñ˜™£ò‘é¥‘«¶”Þ¥Ë»˜ÂÐÌÆñ¸Ë¾œËÄê¹¯áÞ›’ˆ¡ÍÈÄãæŽÄû¤—Ä¬Ú²–à¬½ŒÆ§¦·¢•›²Œñ¸Ø
þŸŽ——ˆ£Û‡Ïð…Ÿ¾üÙÒÉ§¥Ó§Ÿêüòõ×Ý—¿Îûî¹ç¶ç¦ãÓáöÏ¿ýôŒôþ¾†ÓÎ¹ƒå†›îˆ×›¨îºÏºØºðõóÓÍíÍü½
¼öù±ÂË÷þì÷ø›‡“ñØÍ±ã§öÚ¾Ú›ÓÿÓéôÎ‘´ÛÏ·”®–îÞÃ™Ïìß½êéÒåÚçÎ¼Û•¼áÇŸÏ¼Ëöê©§öîîíúÕòïš®
Ð¤½¤æÞÝÍÞÍç‹å›Ž›ÕºµøÐÙÚôæÁØüÔóáý¥éîÌÂ‡Ÿ±ùÆ’‘¾’Ñ±êËÜ²¦ù»šæé·â¨ÓŸ†»éÃñÁ¤ªâÚµ‡‹§½
öé³ßï¦½ƒÕÎ»å°ÍŽ·Ç÷¦Óœ½ú´ÅûÓ©ŸÊþ¯¹Âßÿ——ïþò÷æ¶Ü½éöŽéþìûŽêúÎ¾ãÛç¤Û›êÞË£íž«ã¿•üÎªÃö
ïö‡­Í·ÔžÁæ±°íëó¾ŽŽßôðÃåëýÑ‡á´Ÿš¶ð´Ûó¾üŒÉ¾³’ˆô‘ˆÕžÛú’µóïôÃûÊÚÖœæÝÆ½òðîºü·Åýå÷‡û¯œ
ÆöâÍ®»÷ÂêÉÛ“Î¼ùä÷“ó—ù¿œ¶²µÏ¹å”ÓÞ†¶ùØ±ã£±ÛŸÄÊçäÃÖº½É´üÛÏþû±ÂË‡ï–¦ìó´»¼ÿìç¾£ŽŸ¿¾µ
Õý®ï¦Ã¾÷§¹ÏõîÀ¾§÷»‰Õ„áÖ‰ö¦Ì÷ÿá­äž¯Ó–çËŒìîíê¨æÖšºé›ö÷åãƒÍìÈ°…žòªó¾ˆ›Ÿê°îðó„µû‹
–¿üŒÉ£‘™é¥‘«¶÷Ëè¶­‘—õýûÇŸˆíÝäÊˆÇî™á‡ó˜´°ããø³òäÝ¦üø©üóùùïÏæ§¦Í¸òÙ™—›¸»Š¯Ý¡ü›»§û
ÕþíøŽøÚ³Á«”õ£Öó×ÍùËü‡·½ƒ—¯¡ÿÙø«¶ùÃ¦½ÃÛÜÐÏø¹ÁúÍ»äÇµ§ÝÃï”ä¿ÔæýæÓÃÝÒÎ›œþë¹ö¯¸¼”Æô
Èç¤‘é£‘«Û¶×ËÖÞÕÕ·žˆÃûð³é»–öâ‡ŸîÞßµð‡ùæÕèƒÕ¶þðî•Ï¼ØãÀÝÕøåÛùÜ½º®™¦Óú¦íá¾³ó
¥éüÌ­Ÿšë§ŽÅÚÇáŠžï¥÷žéîíœø¤¤ÉÏÍÏßÛÔüÝü’Ä—´‘ÓŽî•¤¼ÔæýŽîìÿ¨²½É½¿òõÉÒšÓ‘¶Ä¤Ä¬Ú¾
®¬ÙèÄËþêŽÏÄ’›‹ç¶½ï‘˜¶«ößÓåûèÂý÷÷¼»ø…îÌáÐí®¹½×¶äê Ì‡ÐòØäžŸÉå“Ÿåî¹žÝ­¼•™ëúºíúÚ½Ú
—Ô„ááÐýçÙÓ‚•Âî·Íìæžëý…¿‡—Ò˜îŒ¤°¢ý¤âÕöç—ÉòþåÓ’±Ÿíôó—©ŸÎÖ¿ýôüøèææµÏ»¯®¼üÞò¯¶äþ™ø
›‘á»¿üêÇÛÛÏµ·Ÿ†ÍßÇëòß¬™µ»Ž¹•ÞÊÌµÑö½¥÷ÀªÿÖð¾’æ€ç¬â•ÂîÍ¥ÄöÓ–®‡±üƒ³’ˆô‘ˆæÕÛ·¥ÛÖ»
äÔšÝÙÞëÑõþØÊó¢üñö¿èƒ»Å¥¿ìóš—Ã¶õÞÉø«ÁýÌ©Ü§Ï»ëªŸ¾üøîÉ¶¦Ï»Ã—þ¢ùÿ«õÊÀÑ‰×´‡¶§Ç·ÿÔ
Ùœå½ÑÜûõæµƒ›Úž©æƒðš·×¬ùöÐ©ÜàÞ×èÎû¢ùÞ‡¡Ÿ±ù¤¢½¤¶âÕöóÉöºø‘ó°ôþÉãËî»•ö¡‘» —è¾÷åàÊ
üò¼„ß¢›¸ŸÃ›±ðÅŠ¯úŸ¾Û²ÍÓîÕ™é«¶üö•¶¼ùÃäÚÿë¡ï´—×ûÍÆœ…·²óÍóÒÑÊÐÐÍ±ÀÍíÎîŸªü¥çæÓÝ·Ÿ
œÌû»Ê¼¿ÂãòÈÄúÉÄ—ªÕ»äÝ½ãøó°§íàšãÚÅéÄóÙ¦ÓŠ¾É¤ŸîŸâÖÚÊÞ¹‡Êóç’³ÞÏÇéÞ–ôó±ªÍž¼ƒ“ÿð¿¿õ
²ä®Ä¬ÙæéÚÅÔ¼£šþ¶ûñ´³çµúï¯ˆÛÖþôû›òÛ™¹¦¹¶†ÛŸ¡Óþ”§ÁáìÿèÍ¯¾éæøÞùÃÔåàÛúÏìî©æ¡š«ïý
à–þÅžþ¦ºè™ÉÄúÈÄª«Õ»ä½ûäé¾ŸÜÝÁµ÷ã‹í÷ñö¿Û¿ý²ðÌæàÒ–÷š§”»·½ßí·ØçÏ®îËýí†¾»ó›Æ‡îÞ…¾º»
—™îç‡æƒýÿÝ»«ü£³‡¾ÿÙæ½ÆÍ´€ŸÌ½í£û¿°º»õ×ó¿†ì¥•’íëžÁ™Ï·Ÿ†ÞòžÛ¯ÂÃÏÔ·Ò£‰ÈŸ‰Øµêú—ìÚžùØ
¥±ãí¿ä–ÎÅ¤¯»¯ÂëïÞ–ÓäË·‡óžÂÎó±½óºÖëíªÍßÜßËûßæðÑ÷Ÿ¦óäƒŸƒÿ•×ï¯ç¸ùú¿»¼ÍÝîéôÿ÷®¿À×ž
¾üè¸ý¡Ó¹±·ú¿Ø¦ð—ðÍüåž½ªºïŸàëÿ…”üÓá‹ç¾¬Ôö“ˆÛ­¿äŠÓòÛŸøÃ‘ÏÔ·’‘Þ’µ±êõ®Ùðž­Ù˜³åÊæ
‰¿Æ¢ÒË¢Ö›­Þ¥³ã³æ‰æÈÄ³¿£‘éé¥‘«Öï’ÃÙñ™³ó¤¢ÿÙŸ‘ˆô’ˆúÕ«÷ÉìøŒ¹™¹’ÑìÏÈøÄúÉÄªÕ»¨äöüÆ˜³¢
¤£‘é¥‘«Ö£ï’ÉÄãŒÙ¿‘‘ˆô’ˆÕÕ«÷É¤¢±ÆÃ¬ÈÈÄúÉÄ§ªÕ»ä’Ñ˜ ãÖä¤¢½¤¶âÕêÝ²‘±¿Æ˜•™½þ·Äß¢ÖÈ¢ÖÍ
Í’°ú¤âÄ¤âÕ¬™€«ÄÏ¢ÖÈ¢ÖÍÍ’°ú¤âÄ¤âÕ¬™€«ÄÏ¢ÖÈ¢ÖÍÍ’°ú¤âÄ¤âÕ¬™€«ÄÏ¢ÖÈ¢ÖÍÍ’°ú¤â
Ä¤âÕ¬™€«ÄÏ¢ÖÈ¢ÖÍÍ’°ú¤âÄ¤âÕ¬™€«ÄÏ¢ÖÈ¢ÖÍÍ’°ú¤âÄ¤âÕ¬™€«ÄÏ¢ÖÈ¢ÖÍÍ’°ú¤âÄ¤âÕ¬™€«
ÄÏ¢ÖÈ¢ÖÍ‡’±ÞÆ›³‡¼þ×Ãž’ˆµë’ˆÕ³ä¬·ÉÔÎÛë¾’‘ÀÍ ‘ë¤‘«ÂæÉØï¹‘ÿˆîý§ý‹¬‘ÉÙ„’±Æ’±Þê–ŒõÖ¤â
êßßúËÚ’ˆ¤ìˆ‰Ø£‰ØôµËÆú“ˆ°Èž¤â¤âÕì¬™ë­ÉÄÏÏÞú©µ¤‘Ù¤’±Æ’±ê‡–ŒõöÛ¢þ†æ¼–¤¢›Ââ¢ÖÈ¢ÖÍ’
Ó±Þ¤¢¬‡‰ÄØ£‰ØµËÆâú»­‘¿ýÉÂ½Òü²á¸³îû±½¸õ‰È·†ˆµ’ˆÕ³ä¬·¦‘ïÇýôÍû¡Ýÿ—ÌÓÝ¯—È¢‘²¡¤â¤â
¹Õ¬™ëí¶Äÿ¼åæ¥›„ß”óÚßêÞìÁ‰¤¢›Â¢ÖøÈ¢ÖÍ’±Þ¾Ú‰ø‰¥Ûîë¿þóº§öˆÄäÃÈÄšÉÓÄªÙ²ÖÛ“¿ˆŸ¹å¦¥Ë
¬×¾íœü¢ûÏ¥“ˆìˆ‰ÄØ£‰ØµËÆÂú»­‘¿õÓ‘ÒÍŠðÊ¹¹ÉÛú¯³Ü’‘À¡‘ë¤‘«ÒæÉØï­Äü¿«¥ÛÎ¿þó§ÚßêÞì‰¤
¢›Â¢ÖÈ¢¾ÖÍ’±ÞšÄ×üôø¯÷ÂºÏþ™§»ß®‘ÄäÃÈÄšÉ³ÄªÙ²ÖÛÞ¬§âÎ¿¼ÃÍ¥¸Ç•¨‘ÙÒ’±Æ’±ê–ÃŒõöÛ¢þò
‘ãëÉ¢²¡¤’â¤âÕ¬™‹ë­ÉÄÛ¾ù ì­ÉÄ¶„Ä›¬‘Ä¬š¥ãÁ½ÉÄØ’±ôÆ’±ê–ŒõøöÛ¢Þü±µ¤¤‘Ù’±Æú’±ê–ŒõÖ
¶¤â§îþèÚ“’ˆìˆ‰Ø£Ñ‰ØµËÆú»¸­‘¿ñÑµ¤’‘Ù’±Æ’½±ê–Œõ–“‹ñÈÚÄä“†œ‘ˆ•º¢‘äëæÉØï£™ƒ¿«áÏÉ
™ÄšÉÄªÙ²ûÖÛÁ‡ÛþÍŸßÕ°§¤â‰¤âÕ¬™€‰«Ï¢ÖÈ¢Ö›Í’°ú¤â‰¤âÕ¬™€‰«Ï¢ÖÈ¢Ö›Í’°ú¤âÏòªå—Ëë
ÃêØ²€ÖŸÞÄ¬‘£ˆÕ³”ä€¬¾‰Ø£á‰ØµË†ÀêÛ“ˆµ’ˆÕÛÓß¿‚áŒ±Æˆ¬¸’±ºÇ¤¢âµŽ™ãŒ±ïõÌï·™‹°Ä„€ ˆ€
 €„’‘€€À€¢‚€ÈÄ€„€‚‰ˆ€À €‘€ˆ¤¢€Â€Ä„€ ˆ€€„’‘€€À¢‚€ÈÄ€€‚‰ˆ€€À ‘€ˆ¤ˆ¢€€Ä„€
† ˆ€€„’„‘€€À¢‚€ÀÈÄ€€‚‰€ˆ€À ‘€ ˆ¤¢€€Ä˜„€ ˆ€€„’‘€€À¢€‚€ÈÄ€€‚‚‰ˆ€À ‘€€ˆ¤¢€€
áÄ„€ ˆÀ€€„’‘€€€À¢‚€ÈÄˆ€€‚‰ˆ€À€ ‘€ˆ¤¢„€€Ä„€ ƒˆ€€„’‘‚€€À¢‚€ ÈÄ€€‚‰ˆ€€À ‘€ˆ
¤¢€€Ä„Œ€ ˆ€€„ˆ’‘€€À¢‚€€ÈÄ€€‚‰ˆ€À ‘À€ˆ¤¢€€°Ä„€ ˆ€ €„’‘€€ÀÐ¢‚€äÄÌ¡§¥¢€€ã“
‡‰˜ãÐ¢‚€ °ö”‘€€ŽÑ‰ˆ€À“ñ…Úë¯ËÄ€‚€¶ÉÄ€€‚‰ˆ€À ‘À€ˆ¤¢€€°Ä„€ ˆ€À€„¾—‘€€€À¢‚€ÈÄ
ˆ€€‚‰ˆ€Àà ‘€ˆ¼£„™€€À¢‚€ÀÈÄ€€‚‰€ˆ€À ‘€ ˆ¤¢€€±½€ ˆ€ €„’‘€€À€¢‚€ÈÄ€„€‚‰ˆ€À 
€‘€ˆ¤¢€Â€Ä„€ ˆ€€„’‘€€À¢‚€ÈÄ€€‚‰ˆ€€À ‘€ˆ¤ˆ¢€€Ä„€† ˆ€€„’„‘€€À¢‚€àÈÄ€€‚œ
—ˆé¿ã‰ˆ€À¶œˆ¹Ž¥€¢€€ãÏ´ƒ‚€Ð¡‘€ˆð¤¢€€·«È€€äÄœ¤Æã‰ˆ€À¶áïè†€ ˆÀ€€„’‘€€€À¢‚€ÈÄ
ˆ€€‚‰ˆ€À€ ‘€ˆ¤¢„€€Ä„€ ƒˆ€€„’‘Ž€€Àßè†€  ð”‘€€À€¢‚€ÈÄ€¾€‚œˆù´Äˆ„€àì¨¢€„€Ž¢‚€
 ÈÄ€€‚‰ˆ€€À ‘€ˆœò¢¦ÿŽ¥¢œ€€Û¼£™€‹€À“Í€€„ˆ’‘€€À¢‚€€ÈÄ€€‚‰ˆ€À ‘À€ˆ¤¢€€°Ä„€ ˆ€
 €„’‘€€À€¢‚€ÈÄ€„€‚‰ˆ€À €‘€ˆ¤¢€Â€Ä„€ ˆ€€„’‘€€À¢‚€ÈÄ€€‚‰ˆ€€À ‘€ˆ¤ˆ¢€€Ä„€
† ˆ€€„’„‘€€À¢‚€ÀÈÄ€€‚‰€ˆ€À ‘€ ˆ¤¢€€Ä˜„€ ˆ€€„’‘€€À¢€‚€ÈÄ€€‚‚‰ˆ€À ‘€€ˆ¤¢€€
áÄ„€ ˆÀ€€„’‘€€€À¢‚€ÈÄˆ€€‚‰ˆ€À€ ‘€ˆ¤¢„€€Ä„€ ƒˆ€€„’‘‚€€À¢‚€‰äÄÌÿËÄÌ€€¶åÄœ
äÆã‰ˆ€À¶›œˆßøÂ¤Ž ¥¢€€ã‰ˆ„€À‡Ä„€ ƒˆ€€„’‘‚€€À¢‚€ ÈÄ€€‚‰ˆ€€À ‘€ˆ¤¢€€Ä„Œ€ ˆ€€„
ˆ’‘€€À¢‚€€ÈÄ€€‚‰ˆ€À ‘À€ˆ¤¢€€°Ä„€ ˆ€ €„’‘€€À€¢‚€ÈÄ€„€‚‰ˆ€À €‘€ˆ¤¢€Â€Ä„€ 
ˆ€€„’‘€€À¢‚€ÈÄ€€‚‰ˆ€€À ‘€ˆ¤ˆ¢€€Ä„€† ˆ€€„’„‘€€À¢‚€ÂäÄ¼²š¥¢€€Û¤¢„€€Ä„€ 
ãˆ€€„Þ®€‚€À ‘€ ˆ¤¢€€Ä˜„€ ˆ€€´„ƒ‰¸ºý¬Œü“»ùÙ€€ƒößð¢ŽüÙÎ¢‚€ÔÅ¢ð‚€¬–ˆÝšçŸ‡ÎÉç¶
…ßÒŒ€€«®¶§âç¿ÿùñÙñë‘§îŸÇÓ½ŒÄ„€¨éÅð¢öþÙ›—ŽÓŽ™Ž¥¢ €Àí–ËÄðˆ¥ƒÁØ½Ç¢Ì‚€¬¸Ý¦âÌ œ÷Ž¿ž
³ø¯Œ€À°…¸’ñÜðæ”éÍçœøï‹€ÀŒÌƒ‰¸ô’‘ú€ Šû“ˆž˜â†€¨ÈþžˆÅ„€ Š’‘‚€€À¢‚€ÈÄ€€‚ö¢¤ž±Æ˜ãŒ±
Àæûò¢ž²Æ”˜ãŒ±¦µ©ê‘ÿŸ÷Žß
&]
[s0; &]
[s0; Let as see actual code for HelloWorld U`+`+ application:&]
[s0; &]
[s7; #include <CtrlLib/CtrlLib.h>&]
[s7; &]
[s7; class HelloWorld : public TopWindow `{&]
[s7; -|MenuBar menu;&]
[s7; -|StatusBar status;&]
[s7; -|&]
[s7; -|void FileMenu(Bar`& bar);&]
[s7; -|void About();&]
[s7; &]
[s7; public:&]
[s7; -|typedef HelloWorld CLASSNAME;&]
[s7; &]
[s7; -|HelloWorld();&]
[s7; `};&]
[s7; &]
[s7; void HelloWorld`::About()&]
[s7; `{&]
[s7; -|PromptOK(`"`{`{1`@5 `[`@9`= This is the`]`::`@2 `[A5`@0 Ultimate```+```+ 
Hello world sample`}`}`");&]
[s7; `}&]
[s7; &]
[s7; void HelloWorld`::FileMenu(Bar`& bar)&]
[s7; `{&]
[s7; -|bar.Add(`"About..`", THISBACK(About));&]
[s7; -|bar.Separator();&]
[s7; -|bar.Add(`"Exit`", THISBACK(Close));&]
[s7; `}&]
[s7; &]
[s7; HelloWorld`::HelloWorld()&]
[s7; `{&]
[s7; -|AddFrame(menu);&]
[s7; -|AddFrame(status);&]
[s7; -|menu.Add(`"File`", THISBACK(FileMenu));&]
[s7; -|status `= `"Welcome to the Ultimate`+`+ !`";&]
[s7; `}&]
[s7; &]
[s7; GUI`_APP`_MAIN&]
[s7; `{&]
[s7; -|SetLanguage(LNG`_ENGLISH);&]
[s7; -|HelloWorld().Run();&]
[s7; `}&]
[s0;3 &]
[s0; OK, lots of unknown stuff here, but lets go slowly line by line.&]
[s0; &]
[s0; First of all we include `"CtrlLib/CtrlLib.h`"; this header includes 
most of the U`+`+ widgets. By the way all U`+`+ widgets have 
the base class [^topic`:`/`/CtrlCore`/src`/Ctrl`$en`-us`#`:`:Ctrl`:`:class^ Ctrl] 
and that`'s way U`+`+ widgets are also named `"Ctrls`".&]
[s0; &]
[s0; Next we derived HelloWorld class from [^topic`:`/`/CtrlCore`/src`/TopWindow`$en`-us`#`:`:TopWindow`:`:class^ T
opWindow]  which represent basic independent OS window with stuff 
like caption, close/zoom/restore buttons etc. In plain English 
it`'s our main window;)&]
[s0; &]
[s0; Our HelloWorld window will have menu bar and status bar and 
in class they are saved like two private members.&]
[s0; &]
[s7; MenuBar menu;&]
[s7; StatusBar status; &]
[s7; &]
[s0; &]
[s0; In constructor of HelloWorld with a help of [^topic`:`/`/CtrlCore`/src`/Ctrl`$en`-us`#`:`:Ctrl`:`:AddFrame`(`:`:CtrlFrame`&`)^ A
ddFrame] function we added menu bar and status bar. AddFrame 
will add those elements as [^topic`:`/`/CtrlCore`/srcdoc`/AboutFrames`$en`-us^ Fram
es], placing them at border of TopWindow and reducing the view`-area 
of window. Both MenuBar and StatusBar know where they should 
place self when used as Frames (MenuBar at the top, StatusBar 
at the bottom).&]
[s0; &]
[s0; Next line in constructor is very interesting&]
[s0; &]
[s7; menu.Add(`"File`", THISBACK(FileMenu));&]
[s0; &]
[s0; and deserves more explanation. U`+`+ is using [^topic`:`/`/Core`/src`/Callbacks`$en`-us^ c
allbacks] `- callbacks can be described as a very generalized 
form of function pointers. Each Callback represents some kind 
of action (usually calling a certain function or a certain object 
method) that can be invoked at any time. Now, &]
[s0; &]
[s7; THISBACK(x)&]
[s0; &]
[s0;  is a macro that expands to &]
[s0; &]
[s7; callback(this, `&CLASSNAME`::[@3 x][@0 )]&]
[s0;*C@3 &]
[s0; [%-C where x] is name of method we want to call with this action. 
Function [^topic`:`/`/Core`/src`/Callbacks`$en`-us`#`:`:callback`(OBJECT`*`,void`(METHOD`:`:`*`)`(P1 p1`)`)^ c
allback] is a template function and in order to work you must 
define CLASSNAME variable and thats way every U`+`+ class that 
use [^topic`:`/`/Core`/src`/Callbacks`$en`-us`#`:`:THISBACK`(x`)^ THISBACK] 
macros has next line of code&]
[s0; &]
[s7; typedef HelloWorld CLASSNAME;&]
[s0;3 &]
[s0;3 &]
[s0; So basicaly[3  ]&]
[s0;3 &]
[s7; menu.Add(`"File`", THISBACK(FileMenu));&]
[s0; &]
[s0; adds `"File`" menu in menu bar of our application and when a 
user click on `"File`" U`+`+ will call corresponding FileMenu 
function from our HelloWorld class&]
[s0;3 &]
[s7; void HelloWorld`::FileMenu(Bar`& bar)&]
[s7; `{&]
[s7; -|bar.Add(`"About..`", THISBACK(About));&]
[s7; -|bar.Separator();&]
[s7; -|bar.Add(`"Exit`", THISBACK(Close));&]
[s7; `}&]
[s0;3 &]
[s0; What happens here is that when a user clicks on `"File`" menu, 
U`+`+ creates a pop up menu bar and than calls FileMenu function 
which parameter is address of that created menu bar. Then FileMenu 
adds `"About`" and `"Exit`" choices, adds separator between them 
and binds actions to that choices to corresponding function calls 
from our HelloWorld object (see more detailed dicussion of callbacks 
involved [^topic`:`/`/CtrlLib`/srcdoc`/MenuCb`$en`-us^ here]).&]
[s0;@5;3 &]
[s0; Last line of constructor is &]
[s0;3 &]
[s7; status `= `"Welcome to the Ultimate`+`+ !`";&]
[s0; &]
[s0; Class StatusBar has overloaded operator `= which sets text of 
our status bar. Nice and clean.&]
[s0; &]
[s0; The last function from HelloWorld class is &]
[s0;3 &]
[s7; void HelloWorld`::About()&]
[s7; `{&]
[s7; -|PromptOK(`"`{`{1`@5 `[`@9`= This is the`]`::`@2 `[A5`@0 Ultimate```+```+ 
Hello world sample`}`}`");&]
[s7; `}&]
[s0;3 &]
[s0; Obviously when user click on `"File`"`->`"About`" in menu this 
function is called. PromtOK is handy global function declared 
in `"CtrlLib`\RichText.h`", and she pops up prompt dialog with 
nice information icon and OK button. Also this dialog has some 
information text displayed to user and this text is parameter 
of PromtOK function. Notice some strange characters in our info 
text, this are [^topic`:`/`/RichText`/srcdoc`/QTF`$en`-us^ QTF 
codes] and QTF is the native format for U`+`+ rich texts (formatted 
texts).&]
[s0; &]
[s0; And on the end we have &]
[s0; &]
[s7; GUI`_APP`_MAIN&]
[s7; `{&]
[s7; -|SetLanguage(LNG`_ENGLISH);&]
[s7; -|HelloWorld().Run();&]
[s7; `}&]
[s0; &]
[s0; As you may guess every program needs to have entry point where 
it execution begins, and in U`+`+ this code begins with GUI`_APP`_MAIN 
macro. This macro is tacking care of initialisation of U`+`+ 
GUI application for appropriate OS platform, and on the end closing 
and exiting from framework.&]
[s0; Like every good framework U`+`+ also have [^topic`:`/`/Core`/srcdoc`/i18n`$en`-us^ s
upport] for multi lingual programs, so first we set language 
of our application to English.&]
[s0; Then we make one object from HelloWorld class and call appropriate 
[^topic`:`/`/CtrlCore`/src`/TopWindow`$en`-us`#`:`:TopWindow`:`:Run`(bool`)^ Run] 
function defined in TopWindow class which starts our application.[@5 .]&]
[s0;3 &]
[s0; ]