topic "Tutorial";
[2 $$0,0#00000000000000000000000000000000:Default]
[l288;i704;a17;O9;~~~.992;2 $$1,0#10431211400427159095818037425705:param]
[a83;*R6 $$2,5#31310162474203024125188417583966:caption]
[b83;*2 $$3,5#07864147445237544204411237157677:title]
[b167;a42;C2 $$4,6#40027414424643823182269349404212:item]
[b42;a42;2 $$5,5#45413000475342174754091244180557:text]
[l288;a17;2 $$6,6#27521748481378242620020725143825:desc]
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
[{_}%EN-US 
[s0; [* 1. Basics]&]
[s0;* &]
[s0; -|To use a grid control in your application add the GridCtrl 
package to your project and then just add grid as any other control 
in layout editor (Complex`-> GridCtrl) or manually put some code 
somewhere in your app constructor eg:&]
[s0; &]
[s7; #include <CtrlLib/CtrlLib.h>&]
[s7; #include <GridCtrl/GridCtrl.h>&]
[s7; &]
[s7; struct App : TopWindow&]
[s7; `{&]
[s7; -|typedef App CLASSNAME;&]
[s7; &]
[s7; -|GridCtrl grid;&]
[s7; &]
[s7; -|App()&]
[s7; -|`{&]
[s7; -|-|Add(grid.SizePos());&]
[s7; -|`}&]
[s7; `};&]
[s7; &]
[s7; GUI`_APP`_MAIN&]
[s7; `{&]
[s7; -|App().Run();&]
[s7; `}&]
[s0; &]
[s0; Now we have grid control spanned onto the main window. However 
grid without columns is useless. To add some columns write:&]
[s0; &]
[s7; grid.AddColumn(`"Name`");&]
[s7; grid.AddColumn(`"Age`");&]
[s7; &]
[s7; &]
[s0; Let`'s add some data into it:&]
[s0; &]
[s7; grid.Add(`"Ann`", 21)&]
[s7;     .Add(`"Jack`", 34)&]
[s7;     .Add(`"David`", 15);&]
[s0; &]
[s0; As you can see the first row of grid containing column names 
is painted differently. It is often called header (as in the 
array control), but here I call it fixed row (because there can 
me more than one fixed row). Next `"lines`" are just ordinary 
rows.&]
[s0; &]
[s0; Once you`'ve added data into the grid you can change it:&]
[s0; &]
[s7; grid.Set(0, 0, `"Daniel`");&]
[s0; &]
[s0; First argument of Set() is a row number, second is a column 
and the last `- a new value to be set. Remember that the row 
0 is the first row after fixed rows. To change the value of fixed 
item use SetFixed:&]
[s0; &]
[s7; grid.SetFixed(0, 1, `"Age of person`");&]
[s9; &]
[s0; If number of row/column in Set is greater than the number of 
total rows/columns `- grid is automatically `"stretched`" to 
fit the new item[` 1].&]
[s0; &]
[s0; If you want to change value of current row you can omit the 
first argument of Set():&]
[s0; &]
[s7; grid.Set(0, `"Daniel 1`");&]
[s7; &]
[s0; In both cases you can use the short form:&]
[s0; &]
[s7; grid(0, 0) `= `"Daniel`";&]
[s7; grid(0) `= `"Daniel 1`";&]
[s0; &]
[s0; However there are two differences:&]
[s0;i150;O0; short form always updates internal data. That means 
if there is an edit control active above your item it`'s value 
won`'t change `- only underlaying value will change.&]
[s0;i150;O0; short form never refreshes grid.&]
[s0; &]
[s0; Short form are mainly dedicated to extremely fast updates and 
to use in callbacks (see next chapter)&]
[s0; &]
[s0; Now let`'s do opposite. Let`'s get the data from grid. To do 
it simply use Get() method:&]
[s0; &]
[s7; Value v0 `= grid.Get(0, 0); // get value from row 0 and column 
0&]
[s7; Value v1 `= grid.Get(0);    // get value from cursor row and 
column 0&]
[s7; Value v0 `= grid(0, 0);     // short form of case 1&]
[s7; Value v1 `= grid(0);        // short form of case 2&]
[s0; &]
[s0; Get always returns a copy of internal data (short form returns 
reference to the internal value).&]
[s0; &]
[s0; 1) Not fully implemented in current version, only resizing of 
rows. Column resizing will be available in full 1.0 version.&]
[s0; &]
[s0; [* 2. Callbacks]&]
[s0;* &]
[s0; The easiest method to add some interaction to the grid control 
is to use callbacks. There are many. The most basic are:&]
[s0; &]
[s0; [* WhenLeftClick] `- called when left mouse button is pushed.&]
[s0; [* WhenLeftDouble] `- called when left mouse button is pushed 
twice.&]
[s0; [* WhenRowChange] `- called when cursor in grid changes its position.&]
[s0; [* WhenEnter] `- called when enter key is pressed.&]
[s0; &]
[s0; Add to the code:&]
[s0; &]
[s7; void ShowInfo()&]
[s7; `{&]
[s7; -|PromptOK(Format(`"%s is %d years old`", grid(0), grid(1)));&]
[s7; `}&]
[s7; &]
[s7; grid.WhenLeftDouble `= THISBACK(ShowInfo);&]
[s0; &]
[s0; After running the application and double clicking at first row 
you should see:&]
[s0; &]
[s0;= 
@@image:1449&1034
ˆƒ¢Ï€€€€€€€€€€øÃœíý‰ÐœÙí™æÅÌîÌ®‰Ç»»³»±»æî·Ç½“ŽžÆÙ÷ìôû¶¼¯Þ°Ãíž·ç™íñ÷ÛíîÏåÛòßºÚ­û
ÀÂ‡ˆ‰Ýˆˆ¨îÑÜÈœªÉª€âÐñßé—„Òë„¸ÀïÂ‡Âœ’‡Þþ™¯³««Ý³ª©¢ªÔ¥õâÿñÅÆŸÿ¯ûÏ—ï£³Ê¤ „‡Žœ¸
ŽðàÀƒ‡ŽØœ¸ð¸ãÐáÏÀÃÆá«¿ƒœÇßœ¸¼õŠØÉÁæêâÚ„ÀØ„Û×´Ú€ÊÄ£³®´¯•ø®¡ÌÜ´‰Ë¾îýý¯ŽÜú¯û»
Ÿ¿æÓùŸýº¯ÿÌ÷ÖÿÝ÷õëïŽôþÕ¿Ž¿ôëøº÷ÝûÿÛßñõŸÓö¯œù²µÇÝå­¡¼’×–¯·ÖÖ–Ù‰à°ö‡ß…–É„ïäª—Ô›
²®«¶ÿ¥Úô÷ù·ßùÛ®À¾¬Úô‰¹Îî¹ô†æÈµçþÒÏÒæž¬éÿ§¸½òñ‚ÛË¯ˆÂÃÀ·¯ºõöâïªðßŠËÄ“³ìÎâÏ­â«·‹
Ñ¯ö”ßéàŽ Œî²š¸½ò»³èÒû¹Ë·Ü‹®µë¯ŒüäÀýíÒË“Ë¬¯ŠÂÃàÓÖ¿—¸¸Ê—Âú‡–±êù«ÀÖË¦ö£ËŽÇÅ²áÙœ‹‰­
ò„Í“Âž‚Ñãš”žä™ÂáÈð Ó²ôÚ³Ã°Þ×Ùã·ËÌž“² ü¯û’²©“Ù†ð¸»ÜŠå‚±²¯‹…í¬›·÷»Ž³Áú„¶ä™³Âáð“–™õ
À×®õÉ¬ÓÆÂ†øë—¯´Çºü¢ ¼ŒŽ®å™ÜØú–Íé™’íÑÒÁ¥²Í±ßµÖÖÇ¹ü¢ ¼ŒË¹†å‹ìç²ÅÊØ¶­º…Ù†˜ð¸œûÙÊž
¥ßíž˜—¥˜àñŽ¨¦ä™ÓÂáð“–¹ÿùÛºæÙ›šû¥ñä‡¦¹¾§¦Âå—…áá¾ðÆ÷„îöÍåþ÷ßŠûéÍ“À—þšÈœÿÎæÄ·ß¼¦Á
Ì«¿ÛÕÐÓéÏò‹‚ð°¬‡úÿ•žèéìºÜùÃŽ›×¯Ë˜ñçüØå—…ááØŽ¬Ã ¾Ü‹åðÛÂˆ·„Ëñ¼¾ù²‘…¬Ã œä¤™‚áì Ë
”ˆçÃš–Ýî¡ªÄ Ë€Íº¬“›àÀÃäù¬åï¿ÿŒ‘‚°’ËæÏÙüÂ‚œ´ ÕÙ©„µÀ…ô¾¼ØÃ²Ù¹¹ˆ¹èÀ³Óˆëµ€‹èù°‡å
Ê³žÉÁƒšÁÂØƒÜÀÏ‰ƒ½¬›þ¸£€‡èöŠáðð½ö°ìÃþƒé¹èÀ³ÈÓˆëðì–©þå„—·»Ì¼àËîÞ»¯¸‡¨œÒ«©
ü¾µë…°´€–æ—ÑÎàÕø—Æ°‡å“Ó÷«å …Óº½«Ä§æ½––«Ðì²Ú‰¼ŠÏ˜ö°ìâò€ÊŽšÐ¸…Çùá¨‘èŸŠ™õòË‡ä
‡Í®žøÙ¬­ý©¤Â¢–ò€¼æ­¼ˆ–‚ü®ÏØÃ²±‰É¹èÀá”œÓ…£ØØÔÀ¢Þ·ëë‘¬ ëÂâØ¹ÐÊÆâÖš‰šË—Ò×¦’–›ìáÙ
§ÈØ„œ´ ð‚ŠŽâÁüªŒç¹™ØÞâéñÝ¬ ÂâØ¹ìÐæåù­‰î©±‘»ØÊ²ŽÌ®òµÝš»Ë†ÆÆä á…Óð”‹â£ÄÜïÉþÙÁ¾
Ÿ€’¡¼€òØ¬éì™Ùµ¬¥­Ãë¨†½°¬›˜š•ƒ†¤”ÎÁÑ¬ÓÓ‰Ò¬‘ˆòËÇ‡Â‚ª¬‹ÁÂÂ©›Ûé¶Ã¨£Í¥­·ØÃ²Þá¹è
²Àá”œÅ²ð†ªþá¬¶³«ˆ½žŠ‹èäöÙ¬Ù¨¹¥ß‚ü’™õ´·ÕØå‹ëòëÕº´–õ÷ÊŒÁƒšÂØƒÐÛ—®Ãò€ºªŽÂŽûØ
Æö«§ßŽšÐåì”ÂšØ¹€‡ÂÃÑù˜äŠ¡½¬ëèÇê“ƒ†´»…°†¸€ž‡ûŽØÖÞÙ£‡€èöŠáðü½ö°ìÆÍ®¹èÀ³Óˆ
êë€‹èù°‡Èåˆ„ÂÊ¬™˜åò³ª¯íêÈÁ¬™×¯¸¥¼†„ã×ô½É¬››Ÿ—ƒ„Ùæÿ•ñùÖˆÇÂž–½š£•ƒÙæÿ•°ñùˆÇÂž
é–Í¿œ½ü¥åë§–Œ©ÝÍ¾èÎ¾¢®¯ÚáØØÃ²³‚¯¶ËØ¦ŽâÜÓá–ì—¡Ë½Ì® Ç¯ƒ§µÁž–ÍÝ½§Éü†‚È”ò–·™•—ŒßÝ
Ö’–™ÝÀ¥üõÔÂ¸ö°ì­ö“÷áá”× À²ÚÙ¾‘ËŸŠµ–¬Ý¾¶èÎ¾¢–¦œ‰þ£‹ñìà«ËÆïßÑ™ŽÕÐØÉ²‰Æ¾†ÿ”õžŒË¬
íÃ§Æç—ö°ÿÌìë´¶ÞËÜ–å™‚¥ÇÂŠ¬Ë¬ýôÂ¼¦—°‡åÃ¦ïýòŠ¯…Šã›öå™òÊÒ½¹ÌìÀÏƒÙïÒ¿äæž¿Žž€„ûØ¶°´¢
àÐŒäŠë¾ýüØšË–ç½ÌªÒ¿ž¿Žž„ŒûØÖÓ¿Ä©ò†‚€«¿É·ãò‰Ñì©žõÂàÈÙ¦ßÃ³òËŠðÛØÃ²›®ÞÐÃÑŒûüÝû¯ˆË
ðëÀ¸–ö°ÌáÒ‹ë®ïê¹Î†²ÌóàËþ¬½°æò¦žäë Ë¼ö°ÃÌÒ‹ë®ïê¹†²ÌóàË¬â¢ˆö±–Ì²¹™‚Žž‡•ƒÞ‘œ
õž´ìá™¥·Ò¸Ÿí‡ž¶êåž·ìá™¥¥·¸Ÿí‡ž¶Õåž·ìá™×ÂŸÐÝò°›°ýÙä™çŒûØ¦ð§ô¥¡¾ôØÓ²ÌÑÎ²Ë‹ˆëõ
á¼ŒûØ¦¼¹æ©†¥û™²Ì‰Ê¡á™øÁ¤®²®Õó²ìá™ýíÓ‚ÅÙö¦ÿÉ¬ÖˆÍÞœØ¦¥’ðàêðÈÐÞéö°æìüö¹¬¿³—©Ìé¥ ô
ãø «Óóµ§ÁêÊ·œÑÀÁ´ÎÁ™ëö°Ì“áûÚŠÀÌµï ÄœËžéÅúÆ§€ÉçÅšñ…¨ öà¿°¹ø¤¥é­ëË”þ¸ó§¥ˆªÙ§Œ
€çü¯ã¨¶¶Á•äèÎ²´ÅÊ–Ô§ë‚Ä…æÓ¡Ù¬¤ò Ó¦¶†ÁØì¢´‹§âŒÆ®æŒÍŠÍæÉŒ‹¤…­š°‘ÙÜ»¶•¡ñ¶ïæÜ²Þ
›–Ü›Ì×ãñ¥é”²¿ç×â«õþÐ³Û×ž±–‰ïîŠá¯–ÉõŽš”åÿš¼Ú¤¢¦¤áÂ¦Î¨êŠæ…„æÐ¤–µ§–ñÓ¢Î±±ÆÔ€ëÕ¬
­¸óÆ…Ô¨ãÒ´…Âä“Ü‰Ø¢Íâ‘¥²¸Â©±à©Î¢§ç©“ØðåðÆïò¾’˜Šë’òÌêé¶¦È•Á§àÚ–š´×ÆÅ³¤å®û¡èÄÓÇ—Î¨Ð
æý®Å©Ã³¶Ïì‚¾¢Õ×ÑˆÊ…ýÅÅÞè´•ÏÁ‘Œ©ûœÂÁãšýáŽ®ç™øåÍñÛéœ’ÇÉÃ ü¦³£¥‡È‡“Ô‚–Ýº…«•‘ÐØøâÌ
°œÁ¾ñ¦š—¸ØÌåÔ°¶ÊŒÐÈ‹…µ‰ôŠêÎÔ¤™²Þáúòðí¨‰ ¨µÐ“Ó¦šèœöÊ˜ã‡¬’’…©ø‚¡ ãÅ•‘´Óí¾°ˆÌˆ
àÆãÑ½Ñ’Ä¸ì… ¤ò¤¶˜ë©àÚ‘’×˜¨ªƒ•šÏ¸¤â…ª”—‚ÚÍ†ìùÕê›Ûá…èÉ”±°ˆ¦š¤àÆËµëÚ°ý‰ÛÙÒµÈ¬ê
éÈ¨ïÙ—–ìŠ“’ˆÇÅ…°ì¾ù®‰ë¹ èñëÈŠüßÚ½Ã²Éû›ÂÛœŽ¥†ƒ‡ôìÑû­×äÁù±ýŒ„‰Ó–œÓ‰Ï¹œ›…Øˆÿ¢£ðäþØ
ôÆ±È¸¸šæ³Õá™Œ›Âà²Ê´¹Õôâñ‚—˜Ÿ‰ÍÚ¶—‘ÆøÊ‡€÷¿Öƒ²Ã½Ööçˆ¯®±ŽÁ¶„ä¦²äÏì¨¹’Àä‡¼¤‚áÃôå ‡
›³ ”žÂžÍš´ûÿèÙÏ´©¸Ãù«“€¾€€«…íÙ‹ÝÖ±Èåð–©üÕÙÁ÷œÐÅÝ­õÞø¸š™Ýˆ•ßÐŸÐÐŸ™™Õ…Çè³Õô‡ã³Çê
ž©»ÔÉ‡ã³¨Á¾”›¦µÂèÎÔÆäÕ‚¬±ø©µ¥¡Ú¼Ãè†áØÉüÐÍÎ¥›§²ðäÙÇ“éü—ßˆ´ý‘Ú¸À…ì½š‹ñÀÜ¾¤£„ºÇ°
´ÉàŽñ™ôÓ‘ÚÐí±†²óÇãßˆÔå­±ÐƒÈ ÷ìÀ¸¨Ç—³ÑèƒÎåÓ¸ðÐ³œÁŽÈ¢ÎÝÐÈ¬¦›ÚÎé …Å—¶°ÀÉ«“¸ã¸
‰–é™¬±ß˜•³Éê£ùŠ™Ì÷ºÅôŽÓí´Ò””“Ø€Ââ²É¹Í–ÃÌÕ¨˜³‘‡³„ÈÕ´²Ÿ…ó§ÉéœÖ¼à‹ã‹È’—ÖŸßÈÎ•
ßÖ›¸óÅ‹í–ÄðÅ›û’ºøµÙ–¦žÓ¥»ç»»ƒã¨×‰ŒÂñ‹Âª«¡‰¯¬þ­ÿ™œãÙ¦ðç‡µÝóÐ§Æ¸üøæµÂõåñùõ ÃÝªÄë
›ðÈÙìŸ£è÷¹öò¬ÌúÍÅä†ž†òÕõŠæˆÅè´µ†ãÉç¿ÜŸ§þáæƒÔ¹³²¨Ñì„ÿä¼ç†ž§ °¹ŽÒÁ‹ÎúÏ¬¼œ£ß™” 
“É¾·¢Õ¯†—˜®ßüä„—ª§ù¬ÔÌ‹áËä„Ú¼«©¶ã¦àªçóÓ†…Ü”àƒˆ•òùÅ‹¶ìÞàƒâÐ©¶ŽãÜÀØ€À­Ôæ†™—‚õ·ÙÃ
©ÖÆðçÓÔ±¢Éõüœªˆì’Â© ©ƒÉÑ¹Ç„”·ÃÖ£ä˜óÅÃ½ùÆÛ€ñ´—¸÷¾¸øãáìÒ‰›í¤ÞÔ†Š§ÎÖµú¨“àŽµð«¨–„ºøô
•ž¥Öø‡¤€üÀƒÇô’óñ†…ÜòîÍõº¬Ë”þì ûªŽöÞÖ—œÑ´šû¶– Î¨ÿ‹É«Í«óƒÖïºœÅíØÀ¸˜­Û“šæ–”÷Ð£Û
Öž”à“ž‚Ê¥íÁ‰ÞÁñßáìÕï¼œ½éÄÐ”ÈûÂÓ÷ë†§üî•ðÖœ§Û©ïÚýáË ÿð³Ÿ½õÖŠŒßšŠî€ð÷èšÄƒèÀäˆò‡ž˜
Øà©ÖîÕÓùÓ¿ìöûÌëž¨•™ì‰Â¹ñ‡µú˜¼º¨µÛ¼”ÅÃáå Ã»±œ†åâúâ´± ·Ñ–‚ËÌÙºöà”‘À†ÑÄ˜í‚ÙŸ‡
ˆ±Žæ–…´õ©ÁÚš°®˜ÏáÊš›Ó°‚–Ñ®¡·•ËÍñ²šêÌåÒêë…ÒËÌ¼ò«˜ìôŠ©ƒöµÁèÄ•ÖŠìÓø¤ìÀÆÝËºÎåñÅÓÈÌ
©Ò¼î¡Ø¡Ì•¼˜†¡‹¾¸ÅÚñ³×±˜·¸…í‰ŽÛñÔÌ£¬¿Õˆ§ªÞšÐ€À˜žÜ¸€Ú²áªéŽåÙó÷–§ôÑÿ£–Ô¶°Ý“×£
ãÉ«Ê¯ÖäÝÕœˆÖýáË¤¡îŸï¹´£ü°þã¡¨õ‡Ó¢€Íßøðû¶Ÿ””…ôôÓÃ¯£Ô£žÀÆžµ¾¡¿úì»ÄÿúÂ»¬åõÙÞúºáà”
÷àòó•ëÅŽí¿Ý·—Š¿üŠøËï×úÓ™¸ü÷„Å¿»ê„§½Ýú¬žª­‡êÙâÁ¨ôÌ³À±†é‚¤“‚Ùö´¹µíéÅ¬†Œ‹öé§¬Ð«¡Æ
å¹µâ¸ÁÆ© â¾Ž³•“Û•Ã†¤É‹ãòÐëµüŠÎ¦—ŠÉÁæç‚²Ó¯¹µ¬Àù‰Š¥«•¯¼¯Ð²œêâÄ‘ÓèÒÖÚ˜ÑÆ“«®™Ð¸åÌ•ËÛ
Éæ¢‡ÑŠó‹½³Ý°ÓéüµÍéì¢äÖðÞ¶¹µðø‡›‚Ûƒ¢áÈ…„¹„©ª¤•­Ô…ò—àõ¡œì³åü–§ÈôÑò±³º—‡ù®ãèÊ‘ü
§„×äÖ…¤”—ú„œÜ±é¿ƒµÙñ¬öÏøµº÷èÉÀÇ»ïŽ®Øä§Žðãþ—œþµÒõ‹Š¿Þ¹ó«Ï¶ÑÙ§ô”×àýµ¾ôô³Öÿ¬ß‰íšŸ
ààúŠ‚„¤òŸëï®¢ÂªŸÛõÆÃ‘¿Ý»Ó‡žÂ™ïÿÉ“¹¯»¹£°îÎ˜‚ÈÌÍ¹—Õ€£©…Ì—Û¹Õ¦šœª®†¯Ð˜ˆàÜ•¥Í›ƒ×€çÞ
€…ÓšÎàÖ£‘šŠÜÀº¾Â°ŠºÕåŒÌ¦ñÕˆÉó¸¤ ×á’æ˜×ÙÈ¼ŽÅÒâÅÆ‚üÀ€¶ ³õÉŠ³Ž ÉóÅÞ©˜š©ÆÔ¨ž’÷®¡ëóÊ
ó”Ý¨˜»Ð˜Ã´Ì¬Ôâþí«Î ˜À–ÿ¨”ÁïÉ”œÊÓÎµ²ý±íÖø…§»öÞöÇÜ½Å‰Ý´Ü­è»œœÿâÌà•À¶žÖ¬÷ØÈ±ìû±Ø
¹ýÑÚÁåÍÝÖÒá£íÇê¦ïüÊ‘êá÷ÖÓ´¿ø”è³•¡Îž €¬ŸŠäñäæ„ù‡ïýÌ¹æœŒüüÏðûîßì‘ÛÈÜáÀÔÜ³Š¶ˆÙ
¤¶˜Ÿ¬êØü ¤‚žˆ‚ ¥ÞÅ’©ÀÅ²ðß™ñÚ±¬ÎûîÞâäØÖ…ÖŸ™Ýù ¡®—Ž¬‡Çý›‘§¿ØÀíæÀÖûŸÿïÍŒîúúÆï¸¶
¸í¸ùéî ÝŽ‚þíÏ·î‰ÔÛ­¾¹þÿÐ‰ßó¼€ÓÔýÑÚÿç£ÿƒâÃÍï‹¤•³…Ý–ûîšÖ®ìÏ¿Ü¤¤£Ø£¾‰ÓïüŸ¾ƒžÂ³èè
ÐÆ‰­¾Š­Þ÷é¢ªÊ¨©‚’õå¢ÄÉµ•Ô«Ïšã¨óª¶‘Ò›ª³ÞÖþÆ€ùâ…—ˆß‚‹Æ¦µ‹¼¶Ôˆ¢Æ‰¦àÞÑ¢›áŠÓ—œ©òÛ¤‹Æ
˜È®ì€ã›î…Ã‘ŠòÁÄÏÅÐ¡¶·½ŠßºªÏýÎª¸–åÎø‹ÓÞáØÿÔÁËÐÒ‘™Õ‘žÊÕ¾¯öŒˆ‘±Ù• œŒÍÜ›çÇ­æª½Á
¢«¶œúïÍ®ºúú¶ÿÍ†ïüäšà÷üÇ·å·¾å¾ü½»öæ÷ï·î£òë¾Èô°ÙÁÛ€ì¸’û¨¶óˆ½Ÿ™Ì„¶‰¡î«ýÂö©“à×ù‡‰¶ï¯
•ó•Ü÷ª“¿ýÞþ˜ž‚™÷€‰½Ó³ÓüöÄÖù„Û…œ Á‰‚´•žÍØµàÓáË´È–ÎÅá«üËã²£éÔœëýžÝîËÄ©ˆÈ®ðå‚²‚
Ñ¬‰Í‘ÎÅÎî–’†ƒ±›”¶ÎùÒµäêáÝ¦üã˜¤‘¬Æ—­‡ÏÁ—¼†¼’ºç³‚ÓÌ«ì‹³é‡­ƒìè®ì¡¤à¤—½›¢‰·¹„Þ£ÄË’
Œ§˜³ËâÜÔáýÊýù®èÊÎ ©ÀÅó¦À’¾ðí¸±ÍáÀ˜øÃ™ÈÍ ‘Ÿò¶§àäó™·ôÙå»—£´Žå™µïºñÊÿÓÃÊ¡é·÷ˆËšÛú
ºý¢³Ãôåáú²¢²Á¨‹ß®¾éõ¼ñ‹‹¿Æî¢üíÛ”à—“¹ë™·Ù¹§ð³’½Á’ã°¹ ­²ûí¡§öØ†Ãäé¿‘¬Üô¹ÈÜ
°¤¶Ð¸¯¿°äÒÒðºÁ§ôÚŸ®¯ƒš™Æ®ØâÁ€Í¸‹¾ÜÛ£çþ‘ÔÅßæ­ç¦º²Ëé§¬†•²À£ãÆ‹·Ã¦š²æµÀÈì¬‹Ë„åÑÓ
©Èà“Îø±š·š‚â¼˜²¡ÊÅå°¶áôÖÊ£çµÌ”¸êûÓŠÙúÂ®µ³õðÞÞüØáì®êÅÕÈ¼–ßÆ·ÃôÎ­§è©´æ¡Á€ïß¼×á™
ŠÝŸ°Ú©×¼õÛ©­Ùè‚³¹ÑƒåÖù…ìŒ‹¸±Õ¿Š„—ºòëŽÈ´þ ¡ðŠÊÀ¥™œÝçˆ³§ª›Ã±¬³æ­ÎÎ‡þÆ”ãØÖÕÞŸª¯Ý••
ª‘ªãÖ™„°ÐýéðúÑúÿ¶¸Ÿ°Ô¯ã‰¸«ýÃá¸Ù¿‚ú•ï¨œß¦žˆò‰ÏõÏ«ÊÝ’”Œ‚ÏÊøÚªÆ¦‘ìÊ™›£ŒøÁ‰©ÿâÎÖê“
§–ºöÀ”¸œÌÏ¯ÚÀÄÎƒ—˜ŸìŠÚüÄ¾ƒÂª½¼ûÞëÆËÏ£ßË¦ì€¥Ø€Èë·Æù§åÀ”äÍÊÇã …Î°¶ø¸²ÂÔä†ô’¦«Ý
“Í´ÓŠòîÖ‚“ª¯ƒ¡Ì×ñÆ÷ÚŠÍîŠÓÒ±‚ëÌúí§ŠÔ…±ÞÌ¢Á°ÞÌÀØ“êºžÊˆÔ¸ŒÆúŽ£ñ¸¤Ô†µ“Åúé¸úÝ¨×˜±°²Þ
Ë†ÍžÉ™³îÀº¡©ëœéìÌ‹ÎÅòµë„¹›¦æÞàóÃ•ÑÜá´ Û¨²˜Ê¦Ý¥¥ê¢”®‹¿¾ñ´Ó®»ë•˜¦ß»Õ¿òí¾Æµˆ—¢å‚
ÑÉÔÔ¦¿¬ú¡ÒÏü£œË²–ëÞâÔøåàãÉ¯²Êìä–ØŽáÙÑ…öº…Ùö„ëÃõ¥š¡ºÒ¯ðôãŽ§•îº–´Æß£÷‰ËÙ³¢¿Ê»¬…äõ
å‡ãý´ºÿÄ¤³›¥…¥–¡±¶‰Ç’Ïœ¢ÉÌ¸µß¹£ƒãÊ”¾‰ÇÓŠ‚Ø¸°à€ÞÛÕ§ ÈþÐÚÑÆ¦²÷Î›Œ–à¸Ë‘ƒÊ¯Ç
É€¥ÍÁæÏ…±¤à€ÞÛÔÀÑÛ³þÌŠˆ˜Ü‚ûÐ„èÊØëâÓÃØ®¼Â¡µ ðÝäÀÔ½‰×˜–·¡¹–ÐãÖÆØòœˆ•Â´¬—“ê°”‹ÈÆÎà
Ã–…‰ÈÓä”™°šÆ Æ¡ÜÊŠ®ŒÞ¸Ý¬ò‰á«ŒåÜ…ù€Ìá¼¦”‹ÿ¬“Ø”ŠŒà© éÈ†âÏ»àÝ…úö„Ô‰¹¸Â¢ìÛà³›¹¸´
Ó¯’Èýé–ÿ™È­¬‰¬”’§òª…Òý¢õ…Žå™µïñÚÒâ™Äã‰åŽ»ýåáˆÐÔ¾‘°­¤•¶ŠÀ¾¶†¤ª©¦·”Ñ¨õÆ©í¡éâãµï£
èöç„È£³®¬ÈÜ™Ü¡´†û©±ŒšÐ×øêÌê’«”‹‘Ãî¤ãŠÐÎØêš”ëÇçÈâƒý‰Ú®å•¦ô¥¦¹ø›‘¦°ï¬³­ÃžïýëÈÚˆ
ÿØÆ˜«Ô´°„ÒÑœÒ®²ëå£öë÷£ÌˆÝÉæ£‘€¥Õûƒ–•À¸óŒÍÈ£ïÕè²Ë²ëÞâÜ™ôÞ¦¹ÙºÛ•‡ÚÂ€ˆÍø·‡Â´Å›ƒ
´ÓÂ´†µ¶„Ö„µ¶Ø­µ°ÁšâþŠ´ œÖÒ³š­ŒÍ‰µÚäŠúÁÔ©–ºÅÚÐ÷ÓÌµœŒÎ¦ÄÄ¸Ë…Ù¿¼ŸÇž ØˆÅÂˆÿ“¢Äè¥æÊ
’à³‹¶àîà“½¸”êÓ®Ì‡Êº›»‘Æ…äËÂÌŽÃØšù¸¥–„ê¶É¥ÞÞñÈÒñ»îó­€¼µáÚâø¼êÓÀ¢‚˜«Å…Šýäíæ•³ç
Ë¿Åé–åæ¹±óìØÖÐÜÆÞ¯åÆ “ôÁ ·†¥óÂàÉÃãÔéºõ²„Í³‘ŒÓêÓÁ©ÍÁ§Í¢Ÿ°–´’ãƒ©¦÷™Õ˜¤•¤°˜Ç¬ˆÜƒ•
¼ÊÍ¦…Éê±ù˜–æ°˜ÂÄ•Ýà²âÒÕ½”é®—Ä“¬ëÕç¹¼–¦‚ÄïÌ¦ãÙæÍšÛœŠ„²ÆöÛÆ½ìý±¤ºâ¬ö‰†‡Å±ôÏ¸½ì’ºÿ
•µà…ý±Žæ‘™ÍïñÊïêâò½¥ÊºðÛËßíðÆ œÎŠÇ²ìùü‹ã“ðÝ Î€˜Ù†­··¸‰¥²ŒáÌ¸œåËÏãÛœ„Ù†øð¦œÎ²çñ
„­Î‚¬Ã¸“œŽçÙóø–§…²ŒáÔÐ†¹Íê­Îçñ Ë†Î†¥†‘ää™‚á  Ë åˆçƒÙ†¬ˆÃ¸›È²ä™«ÂÙÀ–¡Ë¥Î†²ŒÙ†ð
ê¶åÈ²„³ñ¬ó”ËÆ†ø‡¦¦ÆÛúÆÞû†ÆÇÇïÃƒ˜œäñûòüøüòàüî­¡Å©‹­×ïÜ»¯áò‹ŽøÆÀÅ–¹„åÍ­×ï¿ºÑýðúþ
‘Ã‡÷ŸÍ¼ü—òø±¸ÅÓÂÇææææçŸŒ¼½ÿäÌÑÝËûö—ßúÄì³ƒ²ÌÎ–õÖö¯Ìþ½œŸ™™üÝñ«´æ¼š¾§üúÑŒ½
ðÝøÂææžÌ¹Í³†÷æžæ™ëðùÕÇ¼¼› Ë–Ì²»ãÃ×»ÆÆÇ®ÆîÞ½³ýð¿æÉÓ§â°¿ÔþïÊõ£•»êÇÿô­ß½»šŸ—ôÀ±û
ŸžÜŸ™Û ÿ¤¤±½¦î‚ÛË÷€ÂÙÀ–Â­™å·úÆ†Ï‡úÇÆ§žÌ¡ÎËâ¶ßîÑÝ©þ¡úæ‘Õô‹ê•êÕÖ ü‚ÐŠ¦çž­ŽÌÎÌ®‹
ÀÝÓ¹…Õ®ß¤†ˆ§‚Ù¶´Å–Ýíšž˜èŒŸž¸·½ÇããÌ¿¡çÉ¦ŒûÀ¸°þÀËõŸÖªÞÍøûÕÑžÕ«©±êç®‘æ©‘Ü•ž¡÷ø²Í½
úúÒ·ñðÎÌßãìš›ù…å‘·Ç‡Ý¾™ÄÎ‚²ì‰¬»¹Ù³ô«»»ÿÌèìòþ¬òîðÜŸ£Óó«äÁœ™ùÂèÑü“›ßø·ý‚ïü‡Ö…´ƒ
›±¦Ÿ“ÉÇä¡ü¡™ûÀÆžÍ’Ùöéòìúžñ×ØÃ‰Ùòü º¯®®Éå¥›á¤ Ëì¾åÙäØð…ëÇÝ·ºû‡§îñ±ü¢Àµ»…˜ÛÕÝ–ž
ð›ŸŽŸñÇñÈ™²ºÍàøš²ŒÞ£ý÷É´»õëâ®ûüï¦¾óöŽ¿ØŸ›íøÅ£ç‚ˆÙæ«Ëºû†ó®µõôöÞ²þ°€¬›š¥¯“³äÎ£æÖ
”…‚°Œž çüˆó„ûÕÃ§žäèÞÂØ£‚°˜úÀ†ï“‹ ØÝÒ»Åú¡î²æãóäÊÏßƒ¾ûÏ¿ó‰ŠÎÎÍ™ŸžÆðù–À¸œÈà²ÛÙÖõéÖ
ß•ö®ž¡»÷†—È½…²±Í¬¦æÈøƒæÑÊðâ‘™Ú ëŒŽêÿæ‡ÛÞûù·£ßûµÿÎÿÿß›ÔÿñÉëã‰å‰Ã¯¯Þ»¤ëŠô¢Ï‘Û÷ÈÇ„
ñéûÀ¦Ÿ’«˜Ãó×úïçµ×ö­Ùúüºþƒ›µ§‚Ùæ«ÙËŒõ—Îßíáèžýðçž¡£Õðý²‚˜æ îÕ£™–…Ì¤·ý‡ÿ¼¨ÙúÓßÕ‹Áª
¯—Ôª¯íò¹øÑ…¬ë¾Ë¥«ó’ôÌ’öéÀ¸é› ƒ³§äúèüãûÄ—é•‘ŸþòÓÜÞþ¹Ë·„Â½áÀ–ÙÊ²óÍÕ­µ­íÃòÆÅ€þ
Š©ÒÿŸŒÎ“¬ ”àÛ¿ÔÅ½œ¡ú­Ùõ±£þ‰Í÷‰¿é¼Ã«í¶é¹¾Á®ÆÈ•Õ±ÒñÜœ…œ«¿ºÜþíøÛÿüáäÌó†«ö‚¬ƒË
ê÷£³ºð‡¾ùå¶³¬¶µ¿¢ùÆÁûÃ»Óì§¬»¤ç¡Š¼‡ö±˜˜æ¾Ùøçã×ŠêÅ€Õë‰™ª—ŽùÆµÍº’–ßãäÚ¨¹¼ÆàŠœÇ
ÓÈÉò¾ïÎüðñ°üêÈ·ÐÞÛ‘”‘êýüö’†ûÐ¬¶þÑ°æ³»üìË£ËÜóØÊ¿²üê–²¦›¿—ûîÁ§ª®áö¡ÐøŽ¼ÉŒ¾ŠÞºË¦–
ïÈ›«âÕßò¦×½–€¬Û›Ù×´†Ÿ¿¦Èå¥Ñò•à˜´×Ž’‹£¤õ„šÜž ½óÅä—‡ÏÝšþæï«¿·‰åŽÝÈ¶…±›ý‰©”÷¸åî¹
øìåÙ†¡ÙÐŽðµ¥óŽÐŒðž‚»î“öÛÍÌýªýŽ„Ç§Á›Óäøž·üôóøÕ‹Ç¶©Ë¼ãš†Ÿ‡“¯Æ˜æÝ˜Ò¦ç‡ÉË°¹Ä°ÄÎ±ô«
©îøØØ² ëÝèÿáÕß­†åò­®¿•ë ¬ÙÚ“Ð¹’Ù³æŠ””ÌÉ­ëÙ´ŒÅ»ÃŸ–¹Ã°•åé—¥—ÏÖ¶Öü©ÕÿåìñÛ‰ö¡ð‚Ü™§
®×ÇÉ•‘æÑÛðú¤é›¥ÿ¥²’îåÇ™§™ÆïžÚ­éš”ØàîÜç‡ÈÁó éœ¤Íý¥¤‘ä¡ÒßÏˆŠ®Íä¶ôÇþ–µ¿ðêŠãø‘éÖ–±è£
åšä—Ê´–ô›ÊÊ¥”±è®‹…Ê­ý„Ø´‹²Ãïôö‚Ùôæ«ËòËÎëŸËšóê®·ö‹Ü»ö›¡—ý¢Ž¼¸Ê®±‘×¦È÷ŒùíãóÊ²„Õ‹
±‡öÆ·õÌ²Áœê˜ õý¤ÞžŽ½Ì±á¶”·ÏäÖ÷æ´¶ôÿæÓ‰ï¾ü÷ß£âôÖÏ–±è­çÑ–ü©ØšÝê‹Ö‰°¨Ì–éå¥å¶ñ—Ùæî
•ÃÖ–Ý»þÃàÛõÃ×ò¾ ‡¹§¹÷îÞ÷ðÔ×ŠÏôÄ¡ðÜœ£·ÿ¦ÉëŸóŸË¬Þ¼è¥¹ß±ýÃÐ¬¶ŸÔôÓ‘ê¾ÒÕËªùúÉé×“¬æ
¿‘ÔÊ¶ˆÂëº¯ýèÝÿýÿÂëÏ–ÖØÙÆ›¢·žëïåŠ¸•¶ù¢ÛÞ…¡¼åèŠ•ª®—©Ðµë¬Ë³•å€¨ÝÛ¶äüº¡¥¿®å ¡ïÉë˜
¢óŸêšäîÍÄðì ½‹äß§«™ïðÕß—Õ¨þúÿºµ¶‹Ô’ª¾šÒÖÍÊ»Éå¬Ÿ©¾õÄß¸ñšÔü½©¼ýìƒßíŸþë—ïë½‚­ÿ“Ú
ÑìÆÚšË”î´åÖžÚ³¥­©íü«ëÙ´Œêä™»¥°¬‡·«ëðÊåÂüîÍÊ¹–¼óŒƒµÝóÍ£Œ‹Åà¡°‘îçß£¤äæôß¿þž¨ú§¸¯½
Öþúß‹ýñ¸ÍõÑû½Ú¡‚ÒÙÃÊûÉÙ”©êøªíœ¡½÷­º¿õìÇÜÕŸýç½¯ï½±úÔúÙµëÿÃÊþÄå–Ö¯°ûåìÑòì¶…¤‹
Ç¥€‹’Äâ¬þÜ¹¥È–º°µ´¾È²—Œ¥…°ŒÛÜÿ¬Å£¾öÌÍâÁ¿ÅÌ†¾â¶™¼ø”„»ÕÕ‡ÝÄ·ù’êÏ¾êÐýýÜ·ß­îøåÓ–ÿø¸
»å…ß•ÿÑëÁ¡‡²ºêÇˆ­á–É¹º›Òì»ü¢ðêÔÙ±Ö½§›þî§é›Þøå½¯ý¬Ñí×ÛÜ¾Ÿ™œ„÷Øê®„á²ìé¬ƒÄ¤È›ü’Êãòë
Îãòš’ÊîèÞš‡ó¯ÍÔôû‘Ü«³‰Õ÷ÿòÚ‰î«ù´íó³©ççÊ‡Ëˆö«¢íý””÷ýöÔýèÌøÉÇøÞ¥ˆÜá‹ÛÎÔüçÍÜÿþ·¿Û
þð´ªºÑå›éÁùÀ–¡ËÈÚÒÿÇ‚¸Ó¹Ã«æ†ç·œÇÈ®ºó¾¦ÁðêüÉûúýÎÐÞõ‡Å·ç¦Œó•ƒ¤êÿ„”ö“Üë³ÞÉÕƒ‘…í¡
ÙÙ­¡Ùçë¼®üà—õçÿïÿÛÿñ¿ßýÆðíá¢Ã•þ¢¶¾‡ƒÙ¶äÍ–Æ††ôÙÝ†Ï¶†®ÎõóÏ©ÔëëŽÇëëüÏ·žÕ…ŸÌì‰¿û
òµ¹æ¤ûÚã«Ó­¢áŠ¹—‚³®œÈ»¯ÿ¬¨àå·å·ýýïÿýòŒ¾Þâ“ÞÖÊÑ”óì Ëáìá™ÅÃã…”ƒ±ï­Œøßýã£ŸîÏõ
åŽ«Û›ßæ—„ÜùðäÕÐöüëòÃ„ÚÛÄÁ¹×¶Æ”ÿôäåÈ¿¼µ÷îÏ¾ñ¯ÿûÏ´ÿøïÓß¨Ø«Ý¥Ë—Ý¾‡ÚÎ†²Ìþ–ÊúûºŒ¥Õ
ÿûÆ®øâÈ´Ï¿ðùùÇßöüï—ÞÛÂòºƒõ­±µ·Óð®ÌŸÈ¿øÿï¯úËï¼óñý·×ÿôåá¯ÐèÓµõÍÃ«ý½®ß€ˆç„€ä™ÃØÆ
þÜëëîîëî»ª¬êø×ßì¿ýþ»ûƒÏÞËª˜ÖÞÙðýÍèû»ÿóÉß†ê¼‚ò¸øÿô¹üÒŠ£ž™åŽäÍ™˜­­ßýïîçëÖ„×„—
Ûô¦ßšÛöêåú¿þäÍÒ©—œÉ±¡—ßô™Ä³†²ÌÉ¬úƒä—–¿ò“‘µŸ…•Ÿêùß°ùÔ—ÿ¸˜²®¹Ëå×œÇñì¬óžËŽ€ÕÕïÿò
üàëÊÕíúžÝ—þðù‰¸ý½Äˆþ¿‹ËŽíÈ²§²ŒÐÕ³Ù±åÿôÖ†Æý¯ÿÙ‹¬­Ã¯í—¡åÑÎæ™€îßúóì¢ÃÁ‰çŠ««ñÞ¶Œ¬
€û†¬Ã¬ó ƒËåˆçƒÙ¡†¬Ã¸›È²¬ä™ÂÙÀ–¡•ËÎ†²ŒÙ©†ð¶åÈ²§„³¬Ã–¡Ñœä™²ŒáŒì Ëåˆç„ƒÙ†¬Ã¸›
±È²ä™ÂÙÀÕ–¡ËÎ†²¤ŒÙ†ð¶åÝÈ²„³¬óŠ¤–éÓÓÔžª´ —¯©Ã¡€¬ÓùŠË¨ÅÜ½œŽœžøÃ´ä™Ç°Ì˜ãš‘ÐßùÍ
íå‘ÿÁ¾²Ììã¶§¥ÆîŒè‹ÐŽ ¶´Ä‰Ë®Ù¹ˆ²íŒ ËÜŒ´‘Â»ãÃ²–Œ‰¹°ÄžÙ¡¥’—€¬£È²·°ƒíäÒº«í“ÙäãÃÖÂ’
ëÖ²¥Ô£†Ä™ÑäÊæ‹¬£ñÈ²·ƒÇ²±†ÁÉÆæ´‚”õ¦–ÕÀÒ¼¦–ËŽ¬£È²Ø·ƒíäöÈÿ‡äè€˜“ÖÃ¥âˆ¡„ÌÊæª­¥ÐÎ®Ž‰
Ð¨†æ‘å„ÚÙææ Ì˜ìîãé®¶…õ—³Â¿á‰µæååž©Ô¦¯ˆ†ÙÆåî†‰å“Ãý–¡££’’ø¯ˆ¬Ÿ—å‚Ã¬£È²·ƒíäü
Ÿ¨×±€°ð”¤ïþª‘Ÿ„˜ä™Á–¹™è´£ãƒ½ã–±¨æ» ¬™ÍÀôý€„Õ§Ñ¬¯‡æ‘åä„ÙææÀ–¹ú­åžÆ²Øî‰Æû»„È
€ç—…Ë¥³š“ñÑÉÀ–‘äç™›¶¢æ”œœ£¦°Ò˜¢‚ËäÑÖ˜µ••–€åâå„úÙææ Œô¥ÝÒÀèß§‚´”œ…ïòÅš×³ò’³Êç‘
èå„Ùææ ’ŒõöÈöªèÔ„Œ÷öŠ¡ž›’Ïªâ¦©¬´Úª§ÐÎŽ¬ò£È²·ƒÀ²Ñ¡ž“Œ‹ Ù…óÃ½·ÍáñÊöØ´Ž”âÆ€É²Ê Íá‘
“à‹¬£È²·ŽƒÇ²îö› Šå™åØ”Ö¡­…­¾È²‚¬äó³ÐÆ†»Û®†»ÚÅïÛñÉ”ã¬†‚£ÄÒÁÑôÊÂòœåÙä™òþæÄ–¹œœËº
ÄäÔêê“° ÑˆÁ§à±”æ¯ë¤ŽÈ²‚¬¤ó³ð¬Û„Ê·Ö³Ì¡–Ö“¤ÙîŒ¬£È²Œ·ƒýßõàç¸úŒÔø¿ë ëËÜø¿Å¹‚§åªâ™
¥Â½¬Ã–¡Ñœä™²ŒáŒì Ëåˆç„ƒÙ†¬Ã¸›±È²ä™ÂÙÀÕ–¡ËÎ†²¤ŒÙ†ð¶åÈ²„³¬ÃÅ–¡œä™²ÒŒáì ËÔì
èïýÃˆ„ó€¬Ãã™Œ›³Ûï™°Œ‡€Ž—Žä™ŽœÅÎžÈ²œ¸œ‚½å¸ð¸ûª ËðàðöÀ¿–áÀáì¬ßÃÃÙÃÖ–Ô¡ˆ¥Àú–
¡ˆû€¬ÃÒ œ…–Íá ï€¢Ž‚¥ÿ–áÀÃÃóÆïÎðàÀ€ƒ‡Žœ¸ð¬·ßñÿ€àá€
&]
[s0; Now some funky stuff:&]
[s0; &]
[s7; void RandomColor()&]
[s7; `{&]
[s7; -|grid.GetRow().Bg(Color(rand() % 255, rand() % 255, rand() % 
255));&]
[s7; `}&]
[s7; &]
[s7; grid.WhenRowChange `= THISBACK(RandomColor);&]
[s0;* &]
[s0; Try to change cursor position (using cursor keys or mouse). 
After each position change the background color of the previous 
active row is changed.&]
[s0;* &]
[s0; [* 3. Editing, indexes and integration with databases]&]
[s0;* &]
[s0; Displaying static data is very useful, but in most cases some 
of it must be changed. One way is to show another window (eg 
as a reaction on double click) and put entered data from it into 
the grid. Second way is to edit data directly in grid. GridCtrl 
support two edit modes:&]
[s0; &]
[s0; 1. Row editing.&]
[s0; -|In this mode all edit controls binded to the columns are shown. 
You can move from one edit to another by pressing tab key (or 
enter if special switch is on). &]
[s0; &]
[s0; 2. Cell editing&]
[s0; -|In this mode only one edit control is displayed. Tab moves 
the cursor to the next edit control (if available).&]
[s0;* &]
[s0; Binding edits with columns is very easy. Let`'s make our example 
to allow edit name and age. First we need to declare edit controls:&]
[s7; -|&]
[s7; EditString name;&]
[s7; EditInt age;&]
[s7; &]
[s0; Then we simply call Edit method for each column:&]
[s0; &]
[s7; grid.AddColumn(`"Name`").Edit(name);&]
[s7; grid.AddColumn(`"Age`").Edit(age);&]
[s7; &]
[s0; Now you can press Enter or click double LMB to start editing. 
By default Tab skips from one cell to another (with binded edit). 
If it is the last editing cell pressing Tab adds a new row. There 
are several ways to change editing behaviour e.g:&]
[s0; &]
[s7; grid.TabAddsRow(bool b) -|enables/disables adding new row after 
pressing tab key &]
[s7; grid.TabChangesRow(bool b)-|enables/disables changing row after 
pressing tab key &]
[s7; grid.EnterLikeTab(bool b)-|enables/disables emulation of tab 
by enter key&]
[s7; grid.OneClickEdit(bool b)-|enables/disables immediate editing 
after LMB click&]
[s0; &]
[s0; [* 4. Bulit`-in toolbar, popup menu]&]
[s0;* &]
[s0; [* 5. Properties]&]
[s0;* &]
[s0; [* 6. Others]]]