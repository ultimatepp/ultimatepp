topic "Draw Tutorial";
[l288;i1120;a17;O9;~~~.1408;2 $$1,0#10431211400427159095818037425705:param]
[a83;*R6 $$2,5#31310162474203024125188417583966:caption]
[H4;b83;*4 $$3,5#07864147445237544204411237157677:title]
[i288;O9;C2 $$4,6#40027414424643823182269349404212:item]
[b42;a42;ph2 $$5,5#45413000475342174754091244180557:text]
[l288;b17;a17;2 $$6,6#27521748481378242620020725143825:desc]
[l321;t246;C@5+75 $$7,7#20902679421464641399138805415013:code]
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
[i448;a25;kKO9;*@(64)2 $$22,0#37138531426314131252341829483370:item]
[2 $$0,0#00000000000000000000000000000000:Default]
[{_} 
[s2;%% Draw tutorial&]
[s3;%% Table of contents&]
[s0;%% &]
[s0;%% [^topic`:`/`/Draw`/srcdoc`/DrawTutorial`$en`-us`#1^ 1. Basic 
drawing operations]&]
[s0;%% [^topic`:`/`/Draw`/srcdoc`/DrawTutorial`$en`-us`#2^ 2. Offsets 
and clipping]&]
[s0;%% [^topic`:`/`/Draw`/srcdoc`/DrawTutorial`$en`-us`#3^ 3. Fonts 
and font metrics]&]
[s0;%% [^topic`:`/`/Draw`/srcdoc`/DrawTutorial`$en`-us`#4^ 4. DrawingDraw]&]
[s0;%% [^topic`:`/`/Draw`/srcdoc`/DrawTutorial`$en`-us`#5^ 5. ImageDraw]&]
[s0;%% [^topic`:`/`/Draw`/srcdoc`/DrawTutorial`$en`-us`#6^ 6. Printing]&]
[s0;%% &]
[s3;:1:%% 1. Basic drawing operations&]
[s5;%% Draw class is base class representing graphical output. It 
is intentionally designed with quite limited set of easy to use 
drawing primitives. Unlike most of other similar classes in competing 
toolkits, U`+`+ drawing operations are [*/ stateless] `- there 
is no separate setup of e.g. line width, pen color etc, all necessary 
painting attributes are parameters of respective methods.&]
[s5;%% &]
[s0;l320; [C@(128.0.255)+75 #include][C+75  ][C@(0.0.255)+75 <][C+75 CtrlLib][C@(0.0.255)+75 /][C+75 C
trlLib][C@(0.0.255)+75 .][C+75 h][C@(0.0.255)+75 >]&]
[s0;l320;C+75 &]
[s0;l320; [*C@(0.0.255)+75 using][C+75  ][*C@(0.0.255)+75 namespace][C+75  
Upp;]&]
[s0;l320;C+75 &]
[s0;l320; [*C@(0.0.255)+75 struct][C+75  MyApp ][C@(0.0.255)+75 :][C+75  TopWindow 
`{]&]
[s0;l320; [C+75     ][*C@(0.0.255)+75 void][C+75  Paint(Draw][C@(0.0.255)+75 `&][C+75  
w) ][*C@(0.0.255)+75 override][C+75  `{]&]
[s0;l320; [C+75         w][C@(0.0.255)+75 .][*_C+75 DrawRect][C+75 (GetSize(), 
White());]&]
[s0;l320;C+75 &]
[s0;l320; [C+75         w][C@(0.0.255)+75 .][*_C+75 DrawRect][C+75 (][C@3+75 10][C+75 , 
][C@3+75 10][C+75 , ][C@3+75 60][C+75 , ][C@3+75 80][C+75 , Green());]&]
[s0;l320;C+75 &]
[s0;l320; [C+75         w][C@(0.0.255)+75 .][*_C+75 DrawLine][C+75 (][C@3+75 100][C+75 , 
][C@3+75 10][C+75 , ][C@3+75 160][C+75 , ][C@3+75 80][C+75 , ][C@3+75 0][C+75 , 
Black());]&]
[s0;l320; [C+75         w][C@(0.0.255)+75 .][*_C+75 DrawLine][C+75 (][C@3+75 160][C+75 , 
][C@3+75 10][C+75 , ][C@3+75 100][C+75 , ][C@3+75 80][C+75 , ][C@3+75 4][C+75 , 
Red());]&]
[s0;l320; [C+75         w][C@(0.0.255)+75 .][*_C+75 DrawLine][C+75 (][C@3+75 160][C+75 , 
][C@3+75 40][C+75 , ][C@3+75 100][C+75 , ][C@3+75 50][C+75 , PEN`_DOT, Red());]&]
[s0;l320;C+75 &]
[s0;l320; [C+75         w][C@(0.0.255)+75 .][*_C+75 DrawEllipse][C+75 (][C@3+75 210][C+75 , 
][C@3+75 20][C+75 , ][C@3+75 80][C+75 , ][C@3+75 60][C+75 , Blue());]&]
[s0;l320;C+75 &]
[s0;l320; [C+75         w][C@(0.0.255)+75 .][*_C+75 DrawEllipse][C+75 (][C@3+75 310][C+75 , 
][C@3+75 20][C+75 , ][C@3+75 80][C+75 , ][C@3+75 60][C+75 , LtBlue(), ][C@3+75 5][C+75 , 
Red());]&]
[s0;l320;C+75 &]
[s0;l320; [C+75         w][C@(0.0.255)+75 .][*_C+75 DrawArc][C+75 (RectC(][C@3+75 410][C+75 , 
][C@3+75 20][C+75 , ][C@3+75 80][C+75 , ][C@3+75 60][C+75 ), Point(][C@3+75 10][C+75 , 
][C@3+75 10][C+75 ), Point(][C@3+75 450][C+75 , ][C@3+75 80][C+75 ), ][C@3+75 3][C+75 , 
Cyan);]&]
[s0;l320;C+75 &]
[s0;l320; [C+75         Vector][C@(0.0.255)+75 <][C+75 Point][C@(0.0.255)+75 >][C+75  
p;]&]
[s0;l320; [C+75         p ][C@(0.0.255)+75 <<][C+75  Point(][C@3+75 30][C+75 , 
][C@3+75 110][C+75 ) ][C@(0.0.255)+75 <<][C+75  Point(][C@3+75 60][C+75 , 
][C@3+75 180][C+75 ) ][C@(0.0.255)+75 <<][C+75  Point(][C@3+75 10][C+75 , 
][C@3+75 150][C+75 ) ][C@(0.0.255)+75 <<][C+75  Point(][C@3+75 70][C+75 , 
][C@3+75 150][C+75 );]&]
[s0;l320; [C+75         w][C@(0.0.255)+75 .][*_C+75 DrawPolyline][C+75 (p, 
][C@3+75 4][C+75 , Black);]&]
[s0;l320;C+75 &]
[s0;l320; [C+75         p][C@(0.0.255)+75 .][C+75 Clear();]&]
[s0;l320; [C+75         p ][C@(0.0.255)+75 <<][C+75  Point(][C@3+75 130][C+75 , 
][C@3+75 110][C+75 ) ][C@(0.0.255)+75 <<][C+75  Point(][C@3+75 160][C+75 , 
][C@3+75 180][C+75 ) ][C@(0.0.255)+75 <<][C+75  Point(][C@3+75 110][C+75 , 
][C@3+75 150][C+75 ) ][C@(0.0.255)+75 <<][C+75  Point(][C@3+75 170][C+75 , 
][C@3+75 120][C+75 )]&]
[s0;l320; [C+75           ][C@(0.0.255)+75 <<][C+75  Point(][C@3+75 130][C+75 , 
][C@3+75 110][C+75 );]&]
[s0;l320; [C+75         w][C@(0.0.255)+75 .][*_C+75 DrawPolygon][C+75 (p, 
Blue);]&]
[s0;l320;C+75 &]
[s0;l320; [C+75         p][C@(0.0.255)+75 .][C+75 Clear();]&]
[s0;l320; [C+75         p ][C@(0.0.255)+75 <<][C+75  Point(][C@3+75 230][C+75 , 
][C@3+75 110][C+75 ) ][C@(0.0.255)+75 <<][C+75  Point(][C@3+75 260][C+75 , 
][C@3+75 180][C+75 ) ][C@(0.0.255)+75 <<][C+75  Point(][C@3+75 210][C+75 , 
][C@3+75 150][C+75 ) ][C@(0.0.255)+75 <<][C+75  Point(][C@3+75 270][C+75 , 
][C@3+75 120][C+75 )]&]
[s0;l320; [C+75           ][C@(0.0.255)+75 <<][C+75  Point(][C@3+75 230][C+75 , 
][C@3+75 110][C+75 );]&]
[s0;l320; [C+75         w][C@(0.0.255)+75 .][*_C+75 DrawPolygon][C+75 (p, 
Cyan, ][C@3+75 5][C+75 , Magenta);]&]
[s0;l320;C+75 &]
[s0;l320; [C+75         p][C@(0.0.255)+75 .][C+75 Clear();]&]
[s0;l320; [C+75         p ][C@(0.0.255)+75 <<][C+75  Point(][C@3+75 330][C+75 , 
][C@3+75 110][C+75 ) ][C@(0.0.255)+75 <<][C+75  Point(][C@3+75 360][C+75 , 
][C@3+75 180][C+75 ) ][C@(0.0.255)+75 <<][C+75  Point(][C@3+75 310][C+75 , 
][C@3+75 150][C+75 ) ][C@(0.0.255)+75 <<][C+75  Point(][C@3+75 370][C+75 , 
][C@3+75 120][C+75 )]&]
[s0;l320; [C+75           ][C@(0.0.255)+75 <<][C+75  Point(][C@3+75 330][C+75 , 
][C@3+75 110][C+75 );]&]
[s0;l320; [C+75         w][C@(0.0.255)+75 .][*_C+75 DrawPolygon][C+75 (p, 
Cyan, ][C@3+75 5][C+75 , Magenta, I64(][C@5+75 0xaa55aa55aa55aa55][C+75 ));]&]
[s0;l320;C+75 &]
[s0;l320; [C+75         w][C@(0.0.255)+75 .][*_C+75 DrawImage][C+75 (][C@3+75 40][C+75 , 
][C@3+75 240][C+75 , CtrlImg][C@(0.0.255)+75 `::][C+75 save());]&]
[s0;l320; [C+75         w][C@(0.0.255)+75 .][*_C+75 DrawImage][C+75 (][C@3+75 110][C+75 , 
][C@3+75 210][C+75 , ][C@3+75 80][C+75 , ][C@3+75 80][C+75 , CtrlImg][C@(0.0.255)+75 `::][C+75 sav
e());]&]
[s0;l320; [C+75         w][C@(0.0.255)+75 .][*_C+75 DrawImage][C+75 (][C@3+75 240][C+75 , 
][C@3+75 240][C+75 , CtrlImg][C@(0.0.255)+75 `::][C+75 save(), Blue);]&]
[s0;l320; [C+75         w][C@(0.0.255)+75 .][*_C+75 DrawImage][C+75 (][C@3+75 310][C+75 , 
][C@3+75 210][C+75 , ][C@3+75 80][C+75 , ][C@3+75 80][C+75 , CtrlImg][C@(0.0.255)+75 `::][C+75 sav
e(), Blue);]&]
[s0;l320;C+75 &]
[s0;l320; [C+75         w][C@(0.0.255)+75 .][*_C+75 DrawText][C+75 (][C@3+75 20][C+75 , 
][C@3+75 330][C+75 , ][C@3+75 `"Hello world!`"][C+75 );]&]
[s0;l320; [C+75         w][C@(0.0.255)+75 .][*_C+75 DrawText][C+75 (][C@3+75 120][C+75 , 
][C@3+75 330][C+75 , ][C@3+75 `"Hello world!`"][C+75 , Arial(][C@3+75 15][C+75 )][C@(0.0.255)+75 .
][C+75 Bold());]&]
[s0;l320; [C+75         w][C@(0.0.255)+75 .][*_C+75 DrawText][C+75 (][C@3+75 220][C+75 , 
][C@3+75 330][C+75 , ][C@3+75 `"Hello world!`"][C+75 , Roman(][C@3+75 15][C+75 )][C@(0.0.255)+75 .
][C+75 Italic(), Red);]&]
[s0;l320; [C+75         w][C@(0.0.255)+75 .][*_C+75 DrawText][C+75 (][C@3+75 320][C+75 , 
][C@3+75 380][C+75 , ][C@3+75 400][C+75 , ][C@3+75 `"Hello world!`"][C+75 , 
Courier(][C@3+75 15][C+75 )][C@(0.0.255)+75 .][C+75 Underline());]&]
[s0;l320; [C+75     `}]&]
[s0;l320; [C+75 `};]&]
[s0;l320;C+75 &]
[s0;l320; [C+75 GUI`_APP`_MAIN]&]
[s0;l320; [C+75 `{]&]
[s0;l320; [C+75     MyApp()][C@(0.0.255)+75 .][C+75 Sizeable()][C@(0.0.255)+75 .][C+75 Run();]&]
[s7;l320; `}&]
[s5;%% &]
[s0;=%% 
@@image:2420&2314
(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)
&]
[s5;%% &]
[s5;%% The examples shows the full range of drawing operations in 
action.&]
[s5;%% &]
[s3;:2:%% 2. Offsets and clipping&]
[s5;%% You can offset the drawing position and clip the drawing operations 
to specified rectangle. Offsetting and clipping in U`+`+ is stack 
based operation `- [*/ End] method restores the previous state. 
Particularly important is combined offset/clip operation [*/ Clipoff] 
that in fact creates a new Draw for subregion of current Draw 
area.&]
[s5;%% &]
[s0;l320; [C@(128.0.255)+75 #include][C+75  ][C@(0.0.255)+75 <][C+75 CtrlLib][C@(0.0.255)+75 /][C+75 C
trlLib][C@(0.0.255)+75 .][C+75 h][C@(0.0.255)+75 >]&]
[s0;l320;C+75 &]
[s0;l320; [*C@(0.0.255)+75 using][C+75  ][*C@(0.0.255)+75 namespace][C+75  
Upp;]&]
[s0;l320;C+75 &]
[s0;l320; [*C@(0.0.255)+75 struct][C+75  MyApp ][C@(0.0.255)+75 :][C+75  TopWindow 
`{]&]
[s0;l320; [C+75     ][*C@(0.0.255)+75 void][C+75  DoPainting(Draw][C@(0.0.255)+75 `&][C+75  
w) `{]&]
[s0;l320; [C+75         w][C@(0.0.255)+75 .][C+75 DrawEllipse(][C@3+75 0][C+75 , 
][C@3+75 0][C+75 , ][C@3+75 100][C+75 , ][C@3+75 30][C+75 , WhiteGray(), 
][C@3+75 1][C+75 , Cyan);]&]
[s0;l320; [C+75         w][C@(0.0.255)+75 .][C+75 DrawText(][C@3+75 0][C+75 , 
][C@3+75 0][C+75 , ][C@3+75 `"Hello world`"][C+75 , Roman(][C@3+75 30][C+75 )][C@(0.0.255)+75 .
][C+75 Bold());]&]
[s0;l320; [C+75     `}]&]
[s0;l320; [C+75     ]&]
[s0;l320; [C+75     ][*C@(0.0.255)+75 void][C+75  Paint(Draw][C@(0.0.255)+75 `&][C+75  
w) ][*C@(0.0.255)+75 override][C+75  `{]&]
[s0;l320; [C+75         w][C@(0.0.255)+75 .][C+75 DrawRect(GetSize(), White());]&]
[s0;l320; [C+75         DoPainting(w);]&]
[s0;l320; [C+75         w][C@(0.0.255)+75 .][*_C+75 Offset][C+75 (][C@3+75 30][C+75 , 
][C@3+75 50][C+75 );]&]
[s0;l320; [C+75         DoPainting(w);]&]
[s0;l320; [C+75         w][C@(0.0.255)+75 .][*_C+75 End][C+75 ();]&]
[s0;l320; [C+75         w][C@(0.0.255)+75 .][*_C+75 Offset][C+75 (][C@3+75 20][C+75 , 
][C@3+75 100][C+75 );]&]
[s0;l320; [C+75         w][C@(0.0.255)+75 .][*_C+75 Clip][C+75 (][C@3+75 5][C+75 , 
][C@3+75 5][C+75 , ][C@3+75 40][C+75 , ][C@3+75 20][C+75 );]&]
[s0;l320; [C+75         DoPainting(w);]&]
[s0;l320; [C+75         w][C@(0.0.255)+75 .][*_C+75 End][C+75 ();]&]
[s0;l320; [C+75         w][C@(0.0.255)+75 .][*_C+75 End][C+75 ();]&]
[s0;l320; [C+75         w][C@(0.0.255)+75 .][*_C+75 Clipoff][C+75 (][C@3+75 10][C+75 , 
][C@3+75 150][C+75 , ][C@3+75 60][C+75 , ][C@3+75 20][C+75 );]&]
[s0;l320; [C+75         DoPainting(w);]&]
[s0;l320; [C+75         w][C@(0.0.255)+75 .][*_C+75 End][C+75 ();]&]
[s0;l320; [C+75     `}]&]
[s0;l320; [C+75 `};]&]
[s0;l320;C+75 &]
[s0;l320; [C+75 GUI`_APP`_MAIN]&]
[s0;l320; [C+75 `{]&]
[s0;l320; [C+75     MyApp()][C@(0.0.255)+75 .][C+75 Sizeable()][C@(0.0.255)+75 .][C+75 Run();]&]
[s0;l320; [C+75 `}]&]
[s5;%% &]
[s0;=%% 
@@image:1287&1425
(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)
&]
[s5;%% &]
[s3;:3:%% 3. Fonts and font metrics&]
[s5;%% [* Font] is a simple font description value type `- it contains 
an index of typeface, height of font and additional attributes 
(e.g. italic flag). Font also provides metrics information about 
the font and individual characters.&]
[s5;%% The most important information of Font is the [* GetAscent] 
value `- distance from the baseline to the top of character, 
the [* GetDescent] value `- distance from the baseline to the bottom 
of character cell (height of character cell `- [* GetHeight] `- 
is simply the sum of both values) and the individual character 
width that can be obtained by Font`'s [* operator`[`]] (where index 
is in UNICODE).&]
[s5;%% To get the list all available typefaces and respective use 
[* GetFaceCount] and [* GetFaceName] static methods of Font.&]
[s5;%% Position given in [* DrawText] specifies the top`-left corner 
of the first letter of text.&]
[s5;%% Different than default character spacing can be specified 
by the C array with integer widths of characters.&]
[s5;%% &]
[s0;l320; [C@(128.0.255)+75 #include][C+75  ][C@(0.0.255)+75 <][C+75 CtrlLib][C@(0.0.255)+75 /][C+75 C
trlLib][C@(0.0.255)+75 .][C+75 h][C@(0.0.255)+75 >]&]
[s0;l320;C+75 &]
[s0;l320; [*C@(0.0.255)+75 using][C+75  ][*C@(0.0.255)+75 namespace][C+75  
Upp;]&]
[s0;l320;C+75 &]
[s0;l320; [*C@(0.0.255)+75 struct][C+75  MyApp ][C@(0.0.255)+75 :][C+75  TopWindow 
`{]&]
[s0;l320; [C+75     DropList font`_list;]&]
[s0;l320;C+75 &]
[s0;l320; [C+75     ][*C@(0.0.255)+75 void][C+75  Paint(Draw][C@(0.0.255)+75 `&][C+75  
w) ][*C@(0.0.255)+75 override][C+75  `{]&]
[s0;l320; [C+75         ][*C@(0.0.255)+75 const][C+75  String text ][C@(0.0.255)+75 `=][C+75  
][C@3+75 `"Programming is fun`"][C+75 ;]&]
[s0;l320; [C+75         Font fnt(][C@(0.0.255)+75 `~][C+75 font`_list, 
][C@3+75 60][C+75 );]&]
[s0;l320; [C+75         ]&]
[s0;l320; [C+75         w][C@(0.0.255)+75 .][C+75 DrawRect(GetSize(), White);]&]
[s0;l320; [C+75         w][C@(0.0.255)+75 .][C+75 Offset(][C@3+75 50][C+75 , 
][C@3+75 50][C+75 );]&]
[s0;l320; [C+75         ][*C@(0.0.255)+75 int][C+75  x ][C@(0.0.255)+75 `=][C+75  
][C@3+75 0][C+75 ;]&]
[s0;l320; [C+75         Vector][C@(0.0.255)+75 <][*C@(0.0.255)+75 int][C@(0.0.255)+75 >][C+75  
dx;]&]
[s0;l320; [C+75         ][*C@(0.0.255)+75 for][C+75 (][*C@(0.0.255)+75 char][C+75  
letter ][C@(0.0.255)+75 :][C+75  text) `{]&]
[s0;l320; [C+75             ][*C@(0.0.255)+75 int][C+75  width ][C@(0.0.255)+75 `=][C+75  
fnt][C@(0.0.255)+75 `[][C+75 letter][C@(0.0.255)+75 `]][C+75 ;]&]
[s0;l320; [C+75             w][C@(0.0.255)+75 .][C+75 DrawRect(x, ][C@3+75 0][C+75 , 
width ][C@(0.0.255)+75 `-][C+75  ][C@3+75 1][C+75 , fnt][C@(0.0.255)+75 .][C+75 GetAscent(), 
Color(][C@3+75 255][C+75 , ][C@3+75 255][C+75 , ][C@3+75 200][C+75 ));]&]
[s0;l320; [C+75             w][C@(0.0.255)+75 .][C+75 DrawRect(x, fnt][C@(0.0.255)+75 .][C+75 G
etAscent(), width ][C@(0.0.255)+75 `-][C+75  ][C@3+75 1][C+75 , fnt][C@(0.0.255)+75 .][C+75 G
etDescent(), Color(][C@3+75 255][C+75 , ][C@3+75 200][C+75 , ][C@3+75 255][C+75 ));]&]
[s0;l320; [C+75             w][C@(0.0.255)+75 .][C+75 DrawRect(x ][C@(0.0.255)+75 `+][C+75  
width ][C@(0.0.255)+75 `-][C+75  ][C@3+75 1][C+75 , ][C@3+75 0][C+75 , ][C@3+75 1][C+75 , 
fnt][C@(0.0.255)+75 .][C+75 GetHeight(), Black());]&]
[s0;l320; [C+75             dx][C@(0.0.255)+75 .][C+75 Add(width ][C@(0.0.255)+75 `+][C+75  
][C@3+75 4][C+75 );]&]
[s0;l320; [C+75             x ][C@(0.0.255)+75 `+`=][C+75  width;]&]
[s0;l320; [C+75         `}]&]
[s0;l320; [C+75         w][C@(0.0.255)+75 .][C+75 DrawRect(][C@3+75 0][C+75 , 
][C@3+75 0][C+75 , ][C@3+75 4][C+75 , ][C@3+75 4][C+75 , Black());]&]
[s0;l320; [C+75         w][C@(0.0.255)+75 .][C+75 DrawText(][C@3+75 0][C+75 , 
][C@3+75 0][C+75 , text, fnt);]&]
[s0;l320; [C+75         w][C@(0.0.255)+75 .][C+75 DrawText(][C@3+75 0][C+75 , 
][C@3+75 70][C+75 , text, fnt, Blue(), dx][C@(0.0.255)+75 .][C+75 GetCount(), 
dx][C@(0.0.255)+75 .][C+75 Begin());]&]
[s0;l320; [C+75         w][C@(0.0.255)+75 .][C+75 End();]&]
[s0;l320; [C+75     `}]&]
[s0;l320;C+75 &]
[s0;l320; [C+75     ][*C@(0.0.255)+75 void][C+75  NewFont() `{]&]
[s0;l320; [C+75         Refresh();]&]
[s0;l320; [C+75     `}]&]
[s0;l320;C+75 &]
[s0;l320; [C+75     MyApp() `{]&]
[s0;l320; [C+75         ][*C@(0.0.255)+75 for][C+75 (][*C@(0.0.255)+75 int][C+75  
i ][C@(0.0.255)+75 `=][C+75  ][C@3+75 0][C+75 ; i ][C@(0.0.255)+75 <][C+75  
][*_C+75 Font][*_C@(0.0.255)+75 `::][*_C+75 GetFaceCount()][C+75 ; i][C@(0.0.255)+75 `+`+][C+75 )
]&]
[s0;l320; [C+75             font`_list][C@(0.0.255)+75 .][C+75 Add(i, ][*_C+75 Font][*_C@(0.0.255)+75 `:
:][*_C+75 GetFaceName(i)][C+75 );]&]
[s0;l320; [C+75         Add(font`_list][C@(0.0.255)+75 .][C+75 TopPos(][C@3+75 0][C+75 , 
MINSIZE)][C@(0.0.255)+75 .][C+75 LeftPosZ(][C@3+75 0][C+75 , ][C@3+75 200][C+75 ));]&]
[s0;l320; [C+75         font`_list ][C@(0.0.255)+75 <<`=][C+75  ][C@3+75 0][C+75 ;]&]
[s0;l320; [C+75         font`_list ][C@(0.0.255)+75 <<][C+75  ][C@(0.0.255)+75 `[`=`]][C+75  
`{ NewFont(); `};]&]
[s0;l320; [C+75     `}]&]
[s0;l320; [C+75 `};]&]
[s0;l320;C+75 &]
[s0;l320; [C+75 GUI`_APP`_MAIN]&]
[s0;l320; [C+75 `{]&]
[s0;l320; [C+75     MyApp()][C@(0.0.255)+75 .][C+75 Sizeable()][C@(0.0.255)+75 .][C+75 Run();]&]
[s0;l320; [C+75 `}]&]
[s0;%% &]
[s5;%% In this lesson we used [* operator`~] to obtain current value 
stored by [* font`_list]. Alternatively, the GetData() function 
can be used for more verbosity. More information about this topic 
can be found in [*^topic`:`/`/CtrlLib`/srcdoc`/Tutorial`_en`-us`#17^ Value 
of widget] section of GUI Tutorial.&]
[s5;%% The final results of program operation is shown below:&]
[s0;%% &]
[s0;=%% 
@@image:2103&725
(A6EC6AAAAAAAAAAAAHic7X0LlBbFmXaDK5jsIT8bEo9GUYyKxqg7g8YbQfCGlwEdbgqKOCgoS1QG4l6SjRuy/8nJrtkczL+7bnb37EGP5+Qk62XUxKjZmFmzq+IFByEiGsg3wFyBSFCQyMX53576pqenqy9V1dXV1f09z3nOnJme7n7fqnrferq6q6v7+obAaVg7bNo6onPl2y6v2VRlY7sza6tzY4ezoJv9HLZwh7Og17mx25nf6/4yf4Bzd4JgxnwPBLPlvB4QzJDBDm1oFzeDsdP92dDd/0tn/y/0Z/+Whu3O1A6X9Iv7Z7e7feoOZ/L6vggwZR9xx96RKw6P+Xrf5/7W5cnf7jvte331/9BX/099F33f5aR/djn5XyLJdgDB7BgTfiCohbkHOVh68iHHFNb7GUVXkYeSlJpIwj3izkOk8k79S7y+D7+5l/R9+J17Rt7zMe3pqfxJf++ShP6L9w+SFJ/RvxEEQRAEwUSSpDL6/2RKzUbWTHk90kb/n0ydPTLJJo5cdpjozP3AmTg4nHcuedVp3ObMenfY4p1E547fMZLcH3H3viOWHSDRd756+IgVfUT2O/30NrJf2MYRyz8euaK6JwiCYEE57M6PQTA7shhjGsoElHHY0gPDF390xJK9RyzeM6xpn3PL+86N/T9vep/+rG5peq/K+bvcn7Rlzo7qHX7aQpy93blsgzNp86DEN6x1bqwMckG3c2svk3sSekZX7iPo/pdc8rh4DwiCIAiCorxtl8uFO4ct3DH85l6iO9VtbpfLOZ1VknYzztrqsrGdOGz6Zueaze6UOW8GHen7+a85xzzi6vuEF9z/ksTPerdKn8p7Ql/ln70/SG/jHb9z9/FfV4BgtuwFQRAsNknBA2Rqzin4gIgPSLlHpukk6MTJ651Jr1dJ+k7KTvzsg9VfLnnVnUUfIfEeA7LubqEd2NT6BT6f2YUHCIIgCII8vfG4f3jeuO3Ime23f7+7c9fBqInxUbj9O5UjJ/aLe/1Lrqaf8Ywz/lFX4k/5YZjEd4RKvCfrPmXv7n+NrnuIuHt3FUAwC3rZAYIZkQ2jQDBrevHWuI3G7KTvB7Zu3vndr/b+dVP38jldS6d33n5Vx8JLO267vOOWi4nb508kbrv+HJez6tqnn775ihPXzTp396b1pPKuvjOJ//xzrrh/6l7nmEfcPye9PvyKoMS7Is6k3BP6hTsGNy4YequBvzgBQRAsKHPv+cHS070Pv63/l21M34k0Ht/1vb/Y/fD9v//hP733b9/e+b0/3/Gdu3d8a0nvN2/v/catPV+7pedrC3r+fF73iuu77m7sXHJ1R9Pk9jkT1k09+WcNdZ07DlRH8Weu4SXevaXvH8Iv6GWCzsjmALBpAIN35kNvzuc+xANLz9z7f7D0zL3/B8vNqqz72e40uBLf+83Fv//xv+x59N/e+4+/3/X9r+/87ood376r91tLev5mMal8718v7PnLm3vumde1bGbn0mkdt122dd756685/T/P+aw7uY4knkgSP27NoMST6E9eX13CjmLbW7DOp/Ik8f6Zfu5kv9BRfO6dPwiCIAhazhmdQ/R9+naX/RLfc89cEvfdD33vd//8zZ33Ld/xf5f23ruo568WdH91Xvey2V13zehc0tC55Br31n3T5O3zL2q//tz108549LxjXIk/b71LJvHjH4XEgyAIgqBpRkt897KZu77/tV3/+A3S9/2v/8rdsmJe912zOpdM71g0teOWS7bfdNH2eRfQ9g/+67Ftc+ors+o2XHvmYxeNDZf4z/3CHddPXO/Oxp+11TXdL/FMxP367tFT+eqLe5B4EARLxhkgaIRM3H0S37V0+s7v3L3zvmYav3sT5vv1/cqOW6aQvm+7/lxv+7aZZ1Vmnr1hepjEsxv1JPG0ZUr/C3ez+yfwz+tho3he3HmJd1V+cF19qDwIgiAIJjFa4jsXT+391hL3/vzfLO75ywWemncsvKzj5knb5543qO/Xnrat8Yz2GWdumH7GYxce70r8BZtcQf/CuiGjeE/iZ/SbdiV+l7eUDVty58jb3mOsCv3Nu/x0Lwmg8iAIgiAoTr/ENwxKfM/Xm3q+cav7856buu+e7Wn69vkXDer79FO2NXx+2/RT2xu/8OtpX3j8wuOqEv+lTeES37jNVWc2hB+Q+COa9njiPkTlgxK/y10d1xvOz9kBgiAIgmAcvbF8QzcTelfHb/5yzz3z3Gnzf3Fz94p5XctmdX7l2sBCNx0zTuu47uTt08Ztn/75rded+tY1p7acf+ygxLOB/BnPuBJPWk8bp3b0D+F3VNex7xd3lwPL5464Yy8j/e6qPPsvMaDyIGiAuScmCIJges7YOZSdrsTPO7/7rhk9K27o+eq87uU3dN09010A544rB5/L33Bm56zxnY2f75x+Quf0E7dde9LGq09q+dLRrsRP2uxc9E51IE+j+DHfHSLxc6v67txS/aiNq+/9X5bxJN4T+kGVb9rj3OR+B6f/u/YgCIIgCCZxzp5BDqi8ewd+9oTOP2vobp7dvfz6nuY53ctmdt05PTCK754zvnvmSV3XHt81/fjt047fOPX4J875tCvxE7c5E9tdiT95ozuKJ4k/cb2r+yTxcwYkvl/fj1j44bBFHw1f/FH/h+0OjFj60SCZyi/80KP7hbsb+8m0HgRBsNCc+wEIGuKg0P+eZHrrdV/suO3yrq9cS+LevWxW190zBpV98ST2y8f79vTMPqmn8bju6cd0XHPM25ce/VT9/3El/pIeZ8pWV+XP3uxK/Gf/nyvx9Kcr8TuYvrsS7xN3xiES30/aWN3HJ/QgaIDV60kQBMES0Cf0JNPtDaduXzCp846ruu68tuuuxkF9X3Jp9+Iv99w6OKO+t/HYnmlHd1w5ZtPk0U+e9cmqxBNpLO9JPA3nXYnvfyjA9J160UUD+v6Vg8OXfkw/R9x5iCdTfyb0dAjRaQJBEARBMJbzffSEfs5ekunKFSdsm3tex62Xdt5xtafmXUuu6L7jku5FE3tu/dKOBWcNqvw1f9J1+ai3J/7xk18c4Ur8lbtdNWcqP+GFQYlv6Hbv0g/covf03S/uI5cdDvCIu/voAoA4bOkBorP4MJFpPQiCYHHpNB0CwSzJtP7QEMWf9weS6d9OOWbrjLO3z7+IVH7fC0+7+r74sq7Fl3QtmtR96wU9TRN655+xY94ptH3/sw/1XvWpzimfePu8I584bXhV4okk8RfscBfAqd6o3+Y07HSf/vdLfEDfScpHLP+YOHLF4ZH3fOxyRXULkVSeOOzOj4lM7kEQBItONmABQRP0dH/+RyTTW778mfaGU7fOrt9+04UdCyZ13DKpY8HEzgUXdi44v+vmCd03ndlzw6m9s0/sbTyWhvA9l/9xx6QjN54zvGX8MFfip31QHciTxNe/5Er859o8iQ8M4UcsOxCi7wOsSvyKvirvHmTu6QmCIAiClpMXepLpTed/6jeXHb9l2um/nfmnlTnntN9wbvuc+vbr67def/a2OWd0zDq1o/HEzmnHdl41pvOyUR0Xj2y/YPibf+r858lOVeKZypPEsxv1QyWeqXx1ip13c37FYV7i41UeBLNj7okJgiCohQGV7/zd4bVXn/76BWNev/i4tstParvy1LYrx7dNPWXdlSe/OfWk9Vec8NYVx7116dEbp4x5a9KnNl74ibe+9Edv1g371RnOI18+0ZX4GXuHSPyY7/ol3rlp8Fn84LtyAaH33aUP1ffcawwEQRAE7Sd/x/72f9333sZ1T19++g+/OOrRuj95pP7TP57wmR/XffrRCWMemzDm8frRT9aNeursT/70rKOePGPEk6cf8dPThj1+ivPol0/c8eu22/9xT4TEt3sSz35W17Rxh/N7h7wx582l739SHyDz2XvPDgQzYv7PzkAQBNNzyNQ7d8bdkQv23/7AB32S6Nx1iPT9yOveH/IsPiDx07e7X5pj35HxLXPnrWHLlrkb4AE2i95PZ2A6ff7vI4AlZ+6zYcHSM/cgB0vN+R+5YdY/i776xtwNHzpz97ukX+bsdcfj7Cf7xeOM3zvTdrs/2X+9h+8BBiTeVfl2Z9LrzmVtwy5fN/wKl841mxiHTd/M6DS2u1cC3lfsZ20d8lF777v2IAiChebUdhDMkCS4DRFhdtlm98NwxEkDvPjd6p9TBsi20M+L3nF5wabq0vTsMzQk6KESf1mbK/H9Ku8JfSjZDi69/X0HgmCmZIEKgtlxGAhmzAwDmC1gy2bUH9s28tx3j1763lFztnzzP54DwYwo+3QJAAAAUIBz1R5n4rvOST92xvxDQOLzdg0oJxBaAAAAZhAv8fVLXwTBKLIQ6nrv0Jvth1/d9NH/btjfuu4PxOff2P/z1/c++9qHz7y6j/j0mv0/eXlfy4t7N3e56zVB4gEAAMwAEg8qk4UQ6fvWHR+/2/Xxr7e6Wk9ct+XQG785/Oo7h9dsOkh8ceOhX2049Is3Djz3ivsCCCQeAADADCDxoDJZCNH4nfR90/ZDb/72MCk7E/dXNh168a1DL25wSfr+y3WuxNPQvg8SDwAAYAqZSnz6M4A2k4XQ/27Yz8bva39ziJSdcc1bB0nW/+uNwx5J4n/xOkbxAAAA5hCQ+E9ESzzbP6qrl9oOloMsJFrX/cG9M7+l/878xoMeSdZ/vvYQ8bm1B9kvkHgAAACTcCV+0mZn7INpJD5GAnKXITA7spB4/o39NH5nQ/jpC7/NRvGk76TsRNryTL/KEyHxAAAAJiEo8d7vgV/8P70+n98ZLCVZWz/72oek6a9uOkxqzvjChoPEn71x0NvCVB7P4gEAAEwijcTzGwP789vBMpHFgCfxAZX3fieth8QDAACYR8pRPL/RQz0kvuxkDf3Mq/u85+9+Zff0vcq1B597DTfqAQAAzEFE4vmj6sMkPn4LWD6yYPjpmg/d9+N89PT9+Q0Hn99Qlfin1x54Gu/FAwAAGISgxAc69npIPDgg8U++/OGvNhx6ceMg/+ct9479z3996Km2gx6ffO3AT9bs64PEAwAAmEJA4o+qf0dZ4utxo77GyBr6sf/58Jfr3PVtXL41SJL4Zza4/Mmbh1rWHmx57cBTa/AsHgAAwBxEJB4EQ8lCiCT+52vd9eteWO8ud8P4/IaDJO4/e/MgkYbwJPGPvfLRky/hRj0AAIA5QOJBZbIQeuSFvc+89tFzbxx0168b4HNt7vy6p/r55GsHSN8fX7P/iRff74PEAwAAmAIkHlQmC6HfbP/DMy/tefaV95975QOP9OdP1uxjfGrNXhq/k76/s/3DPkg8AACAKcRLPABoB0ILAADADGIkHgQzYt5RDwAAUBMISPzI+k1M4vP2CwAAAACAVKhK/AmQeAAAAAAoFSDxAAAAAFBKQOIBAAAAoJSAxAMAAABAKQGJBwAAAIBSAhIPAAAAAKUEJB4AAAAASglIPAAAAACUEpB4AAAAACglIPEAAAAAUEpA4gEAAACglIDEAwAAAEApEZD4oyDxAAAAAFAKuBJ/8WZn7EOQeAAAAAAoEyDxAAAAAFBKQOIBAAAAoJSAxAMAAABAKQGJBwAAAIBSIiDxeGkOAAAAAMqB/pfmtjgnQOIBAAAAoFSAxAMAAABAKQGJBwAAAIBSAhIPAAAAAKUEJB4AAAAASglIPAAAAACUEgGJx3vxAAAAAFAOVL80NxZfmgMAAACAUgESDwAAAAClBCQeAAAAAEoJSDwAAAAAlBLFknjHh76+NdqZ/rQZOVY4E5k6aaAGatCrQsRVdiZyrCgDPheoISRNAAngJP4dSHwRAtt2E5B4SwiJt7+iIPEpTAAJqEr8CZB4PWFZotyx18mii6mdXhUirrIzAYm3pFCSJoAEQOL1hmWJcsdeJ4supnZ6VYi4ys4EJN6SQkmaABJQLIm3PyxLlDv2Oll0MbXTq0LEVXYmIPGWFErSBJAASLzesCxR7tjrZNHF1E6vChFX2ZmAxFtSKEkTQAIg8XrDskS5Y6+TRRdTO70qRFxlZwISb0mhJE0ACQhI/MgJkPhCBLbtJiDxlhASb39FQeJTmAASMPAZGki8nrAsUe7Y62TRxdROrwoRV9mZgMRbUihJE0ACIPF6w7JEuWOvk0UXUzu9KkRcZWcCEm9JoSRNAAmAxOsNyxLljr1OFl1M7fSqEHGVnQlIvCWFkjQBJAASrzcsS5Q79jpZdDG106tCxFV2JiDxlhRK0gSQAEi83rAsUe7Y62TRxdROrwoRV9mZgMRbUihJE0ACIPF6w7JEuWOvk0UXUzu9KkRcZWcCEm9JoSRNAAkISLzlX5qzPyxLlDv2Oll0MbXTq0LEVXYmyirxra0PNDfPnTJlwujRoxwfaEtj4+SVKxfZ1hCSJoAEcEvfQOILEdi2m4DEW0JIvP0VlZHPJO7jxh3rJMG2hpA0ASSguBKfGLoe6urG0yVrU1PDqlXL29oezjQsbe7ETJqAxFvCgFcxPXwh4io7EzlWVBYSTyN3we7RtoaQNAEkoBYkPgC6siWtzygsbe7ETJrI1EkDNVBWr0wqVy5VpEsuTVaULp89rly5SLw/tK0hJE0ACahBiWcYPXpUS8t92sPS5k7MpIlMnTRQA2X1KqaHL0RcZWcix4rS5TNja+sDUj2hbQ0haQJIQM1KPENTU4PesDQV2LabyNRJAzVQVq9ievhCxFV2JnKsKF0+M06ZMoF3njbSiKZSafEOpD+bm+eOG3esbQ0haQJIQHE/Q5NW3QfgV/n0YWlzJ2bSRKZOGqiBsnplUrlyqSJdcmmyonT5TGxre5j3nH8uaXNDSJoAElAmiY+KhNbWB1avvpekPPDaiAe6mtUVliXKHXudLISYWkJIvP0VpVHi+afwNH4vVkNImgASUAsSH58CDHQZoCUsS5Q79jpZCDG1hOJeFSKusjORY0Vp9Jm/S0+jm2I1hKQJIAG1JvF9/YN6fjjPrnUtl7YCmYDEW0JIvP0VpdFn/kX40NeEbW4ISRNAAmpQ4omrVi3nD2ePsQoS2LabgMRbQki8/RWl0WfBXtHmhpA0ASSgNiWeWFc3PnA4u4dfkMC23QQk3hJC4u2vKEh8ChNAAmpW4vmBPHuMZSCw29oepssJMue/qzZ69CjaQtvj198TNLF69b2NjZP956dLGtoS+mAui/SMOkOl0tLSch8Vk5zh181mlUD/otbxXvBJ6R6baemvCjLR3Dw35vxRdRh/YNG9km301tYHqAj+S+XEItgcuroqir2MFrMmPJv2k5HPgr1i+gTX3hCq4QQkoEzvxUtFFP92CUtJWROsR/Knc3wYh761GgD1b/yyPIK5Q31I1IsDXjHZtyf4+xhRJrTUQ+C/IqC64vvDgHsxviWu0R1YFcEjVb7CgfZ7Fe+Y+J6UO/Fh7L2iknvoilO5osT998N/0ZLSZ0GL8SWK+leiA4K1FLNnunACEuBK/MUk8Q/VmsRHpYa4CRpphmZ0lDlK6pgw5kH7i/RFHulyl+/6ohDvuZNBPUiUfCj4BHcEeifBNbqpxgInJ6FUO9B+r7T0yYKLo/Ke5BK6Ur2BWkX19V99yV6+emVJ77OCXb5EUf9KdCDmnEbCCUgAJD7mJFH/pcvOmB4p1JZ4DxYIbP6+fagJ2k2tkwn13MmgHtI4FrMIYahdqdr2vzisfGAhvErfJ4vcg/LA31VwjIeuVG+gVlE0GE/peUqf05hOLGyiAzHnNBJOQAIg8TEnCf0vZXR8j8QbUtN3Bn6FyVATIl+NTIRUesrWQ0rf/E8unNjeSaG22S1T5QOL4lXKPlnBk8BzFsd46Er1BgoVJbsgfOhpU/qcxnRiYRMdiDmnkXACElCzEl+ptAQOZ11NvAmRG60BQ6FXqiSOdCp/xNLvUc/y4sdlfRGPANiDS/9NADKxatXymOcF4umpUA9+x7x5R4G5NMzD0D7ff6njxPZOflDV+fWOTekJPXmgwxE/sEBepeyT4z0JjavcQ1ecahUVFav8d6u9IgRyPL3Pce0UC5HCJjogUktZhhOQgJqVeBoVBg5nQSUellHwnyH0Jl7UQ3bG0E7bv8p0wARfECdi1UqPJKyhj8DU0lOkHojsqibxfYGYSvAG8o6AmJK5qDnYoasi+A+MmusYtZxCUbyKz5rEPRlIv0LnQEa1mv8qzjEeuuJUqKjQ7I76ULWfdKB35a/R53i31Qqb6IBgQ/B7MqQOJyABnMTXyktzfPywrlIkLL3g5F+BCZyBH5XH63uUb2xaTqgJfhwR30lqTE/xelAgf/fDexjnJIkpDX7j3z6LujkcOqsn/kA2x7sQXqVv9MTo5QM+5urUQOiKU6Gi+JGmV+0iZO9WaPQ53m21wupqCH5PR084AQmoTYnn79I7/deHImHpxF55+s/AX+SLf7qR7/282I434b8YyDQ9xetBjfwQz+v/nVgxjXrpzM/QkaDagQXyKmWji3jCP8Hxd+OO8dAVp0JF8QKUuKyBiOk0B8a4rVZYXQ3B76kpnIAE1KbE8zM9Qteo50040S9p8oHNWxFcvqMvrAP0hnJ+E2nGETEVqLcelBnloRMrpiJnDp0lpXagd9lmv1cpG13EEz5u/QNzx3joSgWbrAm1Kko0neZAQZe0tLVUQ6iZEAgnIAE1KPGhj3hCvzTH75ao0d4Z+BsF4oMUxqgxgpNaStKnp3g9pGGUh+lrQKOYFsirlI2evgjpq0i2FFLnlDWRpggxptMcKOiSlraWagg1EwLhBCSgpiSeAib0TQ3v5k/6sPT24a8/RZ7C+8kPc5iweiZCZwxK9Q+xuaOnHtIwyq6jQym0H2i/V1k3el/EIzDehLHQlTqnrAn+aZr4bboY02kOFGw1LW0t1RBqJgTCCUhA6SWeZJ0tih71GqZ/LlP6sPT24Z8iiUy19ZOfKc3ujceYkJrtk5Q7euohkW1tD7MXBr2F62MWxOBPruBbRgfa71XWjS5uwljoSp1T1gR/EU6ir/A4XpfP4q1msq0zNgEkoEwSr4DRo0f5X+NKH5bePrxORc1MiyJ/kyowYSCliZjSaayHUHof4hFrpaDdgKuyvmV0oP1eZd3o4iaMha7UOWVNhL705y2kr2w6zYGCrWayrTM2ASSgliU+zYLeiUnH3zQQfCXcI/+hHOawLhMxpdNYDwGuXn1vmrX++JMr+JbRgfZ7lXWji5swFrpS51QwERPM7MORIpcuunwWbzWTbZ2xCSABtSnxUVfa6cPS8XWtaiGd6IAuEzGHa6wHj5VKi8KwPdRuwFW1StB+oP1eZd3o4iaUTy5bCqlzKpgQXGB/3Lhjm5oa6Po29Da+Lp/FK1a8sLoaIjMTQAJqTeLjb6OlD0tHUycWcxJdJmIO11gPjFo+NeIveKZdh9qB9nuVdaOLm1A+uWwppM6pZoJiW2qZff4Tybp8Fq9Y8cLqaojMTAAJqAWJp5zyFkXXleaJZ1AO6UQHdJlIyh099cCY+B1waqPm5rnURi0t91Ezsdu2UXYDrqpVgvYD7fcq60YXN6F8ctlSSJ0zjQmpj8U70csFpPFZvGLFC6urITIzASSgTBKvliZ6I9/R1InFnESXiaTc0VMPfdHfOmdf64iZgRxlN+CqWiVoP9B+r7JudHETyieXLYXUOdObWL36Xv5DM1EIXcwqjc/iFSteWF0NkZkJIAGQeL2R72jqxGJOostEUu7oqYfQN1sdsVcIo+wGXFWrBO0H2u9V1o0ubkL55LKlkDqnRhNtbQ+T3Dc3z42ffxK6qKayz+IVa7KtMzYBJAASrzfyvX0MzKjnnwBaOKM+9DtogiuERNkNuCrrW0YH2u9V1o0ubsJY6EqdMzsT/k/LBcDejdXis3irmWzrjE0ACYDE6418bx+8Fx/lpMjnJ+LtBlyV9S2jA+33KutGFzdRjvfiZRn68Vn2GEuLz+KtZrKtMzYBJAASrzfyvX1Srt/VJ7C6Xej3cKUKm116evvwzyXFO/MouwFXZX3L6ED7vcq60cVNGAtdqXNmbaIv7JuA7HafFp/FW81kW2dsAkgAJF5v5Hv7GFijnr8GsHCN+jTNFHVswFW182s/0H6vsm50cRPGQlfqnFmb6IuemqLFZ/FWM9nWGZsAEgCJ1xv53j7xH1AQYeKX5viH9VIfs8s0PR2fxChXQtSxAVfVzq/9QPu9yrrRxU0YC12pc2ZtIurMunwWb7XEwgreahOvJeUiJ5kAEgCJV0tzkTPwM+7E79XzNwFCvwDOXwaIf+sq0/R0fBKj3ExRxwZcVTu/9gPt9yrrRpcyYSZ0pYItaxNRZ9bls3irBbbzUyNaWu7T2xDKRU4yASQAEq+W5iJn4GVafKjCd4Dew0q/Cf6Jv4iJSqUldG3tLOqBNyTYe/BPLZ0w2VLwLaMD7fcq/vzieyoXwTEeuuJUqCjyRPY1Gf7mng3P4vlngmzaTzxD17swHk5AAiDxamkueAZeqf2rWkWRzzh/7+c3Efo4gP+8jp8xy3BlUQ98PyDy1DVqtRzePQXfMjrQfq/izy++p3IRHOOhK06FimLbpabR8hc2bIsWn8VbLbCdnxqReLkV9f0d4+EEJAASr5bmgmcIfU0mXuV5fXeGdiMBE6FqSBlKaesfYtDYmfaMX3cri3oI/eBmzC3Z1tYHYlb85t1T8C2jA+33Kv784nsqF8ExHrriVKgov8+UoYlfig9dI4JNS9Dis3irBbaHXm5FDeSpOWLawng4AQmAxKuluXjShS55QTlCGeSf00K/05bQ3AkMbXgTWr7w4mhNT/8+oZJN1eK/Y0+dTMzyIAG7AVdlfcvoQPu9ij+/+J7KReBPknXoilOhovh9KFX5b2HQn7QxKgs0+izeavz20NSj4Ya/LOIZajCcgARA4tXSXDzp+mI/Kp0I/o4Zb0LhO25ZP0fz7xM6kBcpeJTdgKuyvmV0oP1exZ9ffE/lIvAnyTp0xalQUVJu86CCB96RSemzeKvx2/mFthKR9XwesYYAEgCJV0tz8aTr6+/H1FServz5+TxRJsS7SjZzL6Z02ush6tl6FLylekPtBlyV9S2jA+33KutGlzVhIHTFqVBRgj6HgorspbYun8VbLXS7VJJ69x+izDmGwglIACReLc3Fk85j4j2u0CQSN0EjgkQTtIO/Y/EjakafrnoQ70C8h4BRdp2haS7rW0YH2u9V/PnF91QuQtRJsgtdcSpUlOwHZD0EpuLr8lm81aK2C45EEjPUYDgBCYDEq6W5eNL52dJyX8xcMg+0T8ybZfEmqOsIfN/K+xp74IZAwGjMB6x11QMNweK7RPLB/+wvyq5jvZja6VX8+cX3VC5C/EmyCF1xKlcU+4Asv38oKLX5uaa6fBZvtRiL/Jx/P+gaQCRDDYYTkICCSbxKHkjQjZnsz9D2cNvKRSunTJgy7thxXtyOHjWattB2+m/WTnrn8aNxcqOZeli1fBWV1G+a/mye25xYcO01oJdF98qA/zaHbvqKarmvhcI4kNcE+pNFOO2gt1oyOrDSUmEF8YpQN76uqaEpyn+9vsmZAJIAidcblkXpJ1sfaA30k3R1UZR6KLqY2ulVjYdujhVlm8TnWyg5E0ASIPF6w7IouUND6UA/ST1nUeqh6GJqp1c1HrqQeEsKJWcCSAIkXm9YFiV3AvcSR48aXaB6KLqY2ulVjYcuJN6SQsmZAJIAidcbloXInZWLVgbGQU0NTQWqh6KLqZ1e1XjoQuItKZScCSAJkHi9YZlL7tC4Rny6Gu3pcKi0VApUD0UXUzu9qvHQhcRbUig5E0AS+iV+izMWEq8nLHPJHf9Ny8bJjTTSaX2gNdD10RbqIWkHvpMMjIPsr4eii6mdXtV46ELiLSmUnAkgCZB4vWGZbz+pABpGFa4eii6mdnpV46GbY0UZ8DnrIuRmAkgCJF5vWBarn6SRUeg9UsvrwUAl16BXNR66kHhLCiVnAkgCJF5vWBaon5wyYUpB66HoYmqnVzUeupB4SwolZwJIAiReb1jmkjuBxeISUTe+Ln6hKsvroehiaqdXNR66kHhLCiVnAkgCJF5vWOaYO60PtK5avqqpoYm6TeoJA30jbWyc3Eg7BOYyFbEeii6mdnpV46ELibekUHImgCS4Ej+pOC/NAQAAAAAgBkg8AAAAAJQSkHgAAAAAKCUg8QAAAABQSkDiAQAAAKCUgMQDAAAAQCkRkPiRkHgAAAAAKAUg8QAAAABQSkDiAQAAAKCUgMQDAAAAQCkBiQcAAACAUgISDwAAAAClBCQeAAAAsAetrZXm5menTHlw9Oi/c5yVjPQ7bWls/NHKlf+dt4NFQr/Eb3HGPgiJBwAAAHIEifu4cfd7sh7FvN0sEoor8Ylh4LGu7gd0+dfU9MSqVS+3tXXn7TgAlBbojTNCLVQsjdwFu/S8PS0SakHiA6SrRNL6vN0HgBICvXFGKH3Frlz53+J9eN7OFgk1KPHek52WlrfzLgQAlArojTNCuSu2tbUi1Xvn7W+RULMSz9jU9ETe5QCA8gC9cUYod8VOmfIgX0DaSKOwSmW3txv92dz87Lhx9+foauFQ4xIPlQcAjSi3EuWIEldsW1s3Xzo8S9WFMkl81J6trZXVq9tIyv2vYPhJV4YmPQeAsqLESpQvSlyx/FN4Gr/n7VR5UJX4E8os8X5ETeqgy4CM/QUAAACC4O/S04gsb6fKg1qT+L7+QT0/nMd1IwAAgHnwL8Lj1WaNqEGJJ6xa9TJ/OOIKAADAMEr8DMIG1KbEE+rqfhA4HOsiAgAAGAYkPlPUrMTzA/nc79W3tXXTZQa54b9zxVZmpu3abzKsXt3W2Pgjvy267KEt1j4Iq1R2t7S8TVVBTgbWr3Z8S1hTy/pftNEFNmPTX11krrn52URbfD0LHlg+D5XR2lqhovkvy7V7WLh0UAB76YzPHW/t91ymJJVP4g2EqzhqVuL5NzUo7FOaY72Elz7xJ/SDQiL0zdAAqf/RslwP5XLUywWe5+yeBn+vQ+T82uvKv6cgqT6l+qsYnxPXzY5675IaS+3AgnoY76TaUZSn8amR/nWYrNNBC9Qq1kNiGf00cGEjlcuJpVaunCKGqyxcib94cw1KvOwZ4nemkWNoBom4QQklFeS0v2xJPdBlJN9NxfRs2lOgT6mulDsE8YSK8kdw3Wyq1cAJSRzVDiyuhzFOqh0luKiplId+mEkHLVA2TVdxspfHXnlNFkeQgmdTdiPNUVmHqxog8YJniNqZLttieolEH8R7mECQKNy3p0PUkl1XCijXVRqHBcehof5ItY7/QY/ygYX2MMpJtaNE7mvJtrIfxtJBC9RM02DcztLpdUnZ8wKFqzIg8YJnCN2ZMii+l4h3QE3fGRVWcRT5SqOWrNdeVyl9Fnm6wR+l0Drs9qbygUX3MKrpzRRN9jmysXTQAgXTsgu/myydXpeUPS9QuCqjZiW+UtkdODxeNHlzIjdIY04YetVHIkin9bc+/R71HE1qfmDo4wD2kNF/Q4DMrVr1csyzAxFb2uvK77A3Lygwd4V5Htpvi1wOxftGVe3XODadJtRQINnFDyyBh6FOqh0V72FofFqbDlqgYDoqF/hvanvFDPQzmZWmbBKfdbimQc1KPI3sAofHP+PWGJB9ETfQ4h0I7XgFV3LmC5sYYySgoY+WRMzprSsCu/IRfDYRWlGJA/kor8h01AA2dHUF/4FRRtWWZbDfw1AnEw+JKRrpUdRgJ7SVBWcsG04HLZA1HdrDiHQXdKA3+tDkewLS16ryGQoRrilRsxLP13l8/Md0laxxpV454UflIpPoeJ8Fp8Tw1/Npnv8qHJWmrhTA3yFJfPgV6ioNeOPTMOpmb+KMGv7AxGUZ7Pcw1MnEQ6KKlpgRfBIJXvEaTgctkDXNjxyl1v1g72ikc1kU6WtV+QyFCNeU6Jf4Lc4JD9WUxPN36Z2ka6rQZnVir9yiwF9gi2cT3zslxglvTnyurMYUUKsrNfDDtMQ+nPdWZEpM6MhO7cASeBjqZOIhykXjn/6IXCebTwctkDXNC4o9qxwEkL5Wlc9gf7imR21KPD87QqGDdVRfcuSti7+FyndQicOxNNfzulJAua6UIeu5WklDZzSpHZh4mWe/h8pOqh3F54LIRYj5dNACM/GcC9K7qnwG+8M1PWpQ4kMfiySOLvlD1FaH4G8gyL5/Knt9niaDdKWA+SXCzHSJGgW0BB4qO2m/h+mPTQkz0ZIL0ruqfAb7wzU9akriqZJD324QuWGiq3X4aznZ2zX8MCRGQGVnFQZgMgX0wliXaOxA+z1UtqV2VOjjtvhDckkHLZA1zT/Rs3Yl3vS1ajg1jIWrFpRe4knW2cLmUa8uCq41pKt1+CcyspMu+NnOMffAeXNSs25MpoAC2tq62UuF3sL1MQtQZOSzsQPt91DZljEPc0kHLZA1zQ8ESPTtfByfvlYNp4b5TEyDMkm8AkeP/jvBV7F0tQ6vQbIz0PgbPjHPdFKaM5wCIvA+1iPb1hn5bOxA+z1UtmXMw1zSQQtkTYe+GOgttm8V0teq4dQwn4lpEJD4o2pJ4g0svs2Dv5kguxQt/wGdmIKkNGc4BeKxenVbmvUAM/LZ2IH2e6hsy5iHuaSDFiiYjkkW9vHKXL4rxyN9rRpODfOZmAZViR/7YE1JvMLVrK7W0XIe8ZOkNGc4BaJQqexWGLZL+WA+bcvnobIt++tQy+FpoGBacBH+cePub2p6gq6f87qNn75WDaeG+UxMg1qTeOVbVbpaR8t5xE+SS5+mN5K1fC4k0QfzaVs+D5Vt2V+HWg5PAzXTlDtSS/HLfoJZC9LXquHUMJ+JaVALEk9x6y1srtFcjucRP0kufZreSE78lje1b3Pzs9S+LS1vUxOzW6/GOn9jB9rvobIt++tQy+FpkMa01MfiHYPLpzOkr1XDqWE+E9OgTBJfCHNaziN+klz6NI1NE/VNc/Y1jZhbi8Y6f2MH2u+hsi3761DL4WmQ3vTq1W38h2aiaPKb5umLZjg1zGdiGkDiDZvTch7xk+TSp+mqq9A3SR2x1wyNdf7GDrTfQ2Vb9tehlsPTQKPptrZukvvm5mfj57eY+xRa6qIZTg3zmZgGAYkfWb8JEp+pOcMz6vkncQWaUR/6vbOMPlxuPm3L56GyLWMe5pIOWpCdaf+n5QI081w+fdEMp4b5TEwDV+Inbfa9Fw+Jz9Yc3osXh8IH45R9MJ+25fNQ2ZYxD2vnvXhZhH58Vjzd0iB90QynhvlMTANIvGFzKdfX6pNc3U72m7kBGE6BAPjnhuIdsqwP5tO2fB4q2zLmYS7poAUGTPPfFjTzRD590QynhvlMTANIvGFzhteo568HCrRGfZrzyB5rPm3L56GyLWMe5pIOWmDAdG6LqKc2ajg1zGdiGkDiDZtLn0dSX5rjH9xLfdjOcApoPI/ssebTtnweKtsy5mEu6aAFZkznUsD0RvkzCN7uszxctQASb94cP+NO/F49fxMg8Sve/CWB+DenDKeAxvPIHms+bcvnobItkx6aTwctMGM6Hw1KbZSfYtHS8nZ2ps1nYhoEJB4vzRkwx8u0+FCC76ASHybyT/9FzFUqu0PXuBZxUldd8Q4IZi7/VDHRB/NpWz4PlW2Z9NB8OmiBrGnyVvZVHf4GY1GexfPPLmOmJ3kIXXMjU4dziZ8Bicd78UbN8Uot8hYqH8mCvRPvfHzyxiyHJVI6XXXF56DIk9Oo1XIy8tnYgfZ7qGzLpIfm00EL1KJFaiovf/EjIpTpkb5W+SkWiR1j1Dd6MnU4l/iBxOdiLvQVlXiV5/VdPIVDVY+ygFLDf6lPY2TaM379KxFzuuoq9IOYMbdVW1srMStyZ+SzsQPt91DZluGiGU4HLVCOFvZVjsRPzISuQSF7H0AN6Ws19LIt6vqEmjWmTTN1OJf4gcTnZS50uQmKPYpM/1wR+p22hMak1G00LV9yyToFeIRKNlWd/449JXjM8h2yXaKxtC2fh8q2zBfNZDpoQfpooe6C/04H/Ukbo7Isw/LEuqpwktD0p2GRv7zpe4mUDucSP5D4HM2l+fq51EzgPqXvtZl/VsUjdCAvUjnpu0RBD40daL+HyrbMF81kOmhB+miRzSBj35bVUqv8gmCJND/XKJf4gcTnaI76GTWVp6tuhXtoUt0am8VnOAVCEfVsPSZzFXwwn7bl81DZlvmi9RlMBy1IHy3ipGoxc4s+ylW180h1FOweRYHCVRmQ+NzNJd47Cg1ONdCVeaI52sFLcD73Raxoryvx5PUewKXvEgV9M3ag/R4q28qrzzSTDlogW1LZD8h6VJiKnxIauwvBEZNyL5HSYY0lFUdA4kdOgMTnYK6l5e2YeWIeaR/Bt8biQSkc+M6U99X1QHbz3Z3I+bOoKxpGxXdZ5Jv/uZusD+bTtnweKtvKt8/MOh20QK2k7AOyiR2L172ILxGgEXq7C/69AD/pGiBNL5HSYb0lFTXqfmluszP2ocJJfPlA/Qlde1O/4Zd71tvQdsOX1gyBgJRda1c7SOgDw67QrhgAsoBt6SAOGhqwy5jAUIL+ZBmkZexgCSqV3YFrNlL2pqYnylRGcUDigSjwM1hkv5gDAKUB0gEoIiDxQBT4V2XNfD8aACwE0gEoIiDxQBQC9/RMTi4CANuAdACKCEg8EAp+mfempifydgoA8gHSASgoAhJfoJfmACnQGER8WlrorFRjS2EAQNZAOgA1guK+Fw9IwX+DsbHxR2wpy0A3FbNYLsYsQJmAdABqBJD4GkHMu6KJTPwkPQAUC0gHoEYAia8RKHdohlezBAADQDoANQJIfI1ArUMzuX4XABgD0gGoEUDiawSyK+HX1f2gNheDAmoBSAegRgCJrym0tlZWrXq5qekJ6uL4TzbQxsbGH9EOmC0M1AKQDkDpAYkHAAAAgFKiKvFjIfEAAAAAUCpA4gEAAACglIDEAwAAAEApAYkHAAAAgFICEg8AAAAApQQv8Z+e2clUfuR1O2Tpng0EQRAEQRtI+n7WL/0SP+rKd0fPrnzy6nf+6JJNsnQu3iJNcsBOXizPSVvkaaY48o4pFN/a1jfjmKHiK8RYicJSpV1qO5GtdczedrHWipJj569zzvyFc8LDrsQf/bj7grzLhwZ+McCH5GnEsbFGaMYxhUo2Y0Wl+A9J01BYKlSyERqqZDM0kvtwzE6WKffN5cvD7i/H/bsr8Z/5V/cnCIIgCILl4v8HRrhJVw==)
&]
[s5;%% &]
[s3;:4:%% 4. DrawingDraw&]
[s5;%% [* Drawing] object contains a set of drawing operations and 
in fact represents a vector image with smooth rescaling. [* DrawingDraw] 
serves as the target Draw when creating Drawing, Drawing can 
be painted to any Draw target using the [* DrawDrawing] method:&]
[s5;%% &]
[s0;l320; [C@(128.0.255)+75 #include][C+75  ][C@(0.0.255)+75 <][C+75 CtrlLib][C@(0.0.255)+75 /][C+75 C
trlLib][C@(0.0.255)+75 .][C+75 h][C@(0.0.255)+75 >]&]
[s0;l320;C+75 &]
[s0;l320; [*C@(0.0.255)+75 using][C+75  ][*C@(0.0.255)+75 namespace][C+75  
Upp;]&]
[s0;l320;C+75 &]
[s0;l320; [*C@(0.0.255)+75 struct][C+75  MyApp ][C@(0.0.255)+75 :][C+75  TopWindow 
`{]&]
[s0;l320; [C+75     Drawing drawing;]&]
[s0;l320;C+75 &]
[s0;l320; [C+75     ][*C@(0.0.255)+75 void][C+75  Paint(Draw][C@(0.0.255)+75 `&][C+75  
w) ][*C@(0.0.255)+75 override][C+75  `{]&]
[s0;l320; [C+75         w][C@(0.0.255)+75 .][C+75 DrawRect(GetSize(), White());]&]
[s0;l320; [C+75         w][C@(0.0.255)+75 .][*_C+75 DrawDrawing][C+75 (][C@3+75 10][C+75 , 
][C@3+75 10][C+75 , ][C@3+75 50][C+75 , ][C@3+75 60][C+75 , drawing);]&]
[s0;l320; [C+75         w][C@(0.0.255)+75 .][*_C+75 DrawDrawing][C+75 (][C@3+75 100][C+75 , 
][C@3+75 10][C+75 , ][C@3+75 150][C+75 , ][C@3+75 100][C+75 , drawing);]&]
[s0;l320; [C+75         w][C@(0.0.255)+75 .][*_C+75 DrawDrawing][C+75 (][C@3+75 10][C+75 , 
][C@3+75 110][C+75 , ][C@3+75 300][C+75 , ][C@3+75 300][C+75 , drawing);]&]
[s0;l320; [C+75     `}]&]
[s0;l320;C+75 &]
[s0;l320; [C+75     MyApp() `{]&]
[s0;l320; [C+75         ][*_C+75 DrawingDraw][C+75  iw(][C@3+75 200][C+75 , 
][C@3+75 200][C+75 );]&]
[s0;l320; [C+75         iw][C@(0.0.255)+75 .][C+75 DrawEllipse(][C@3+75 10][C+75 , 
][C@3+75 10][C+75 , ][C@3+75 180][C+75 , ][C@3+75 100][C+75 , Cyan());]&]
[s0;l320; [C+75         iw][C@(0.0.255)+75 .][C+75 DrawImage(][C@3+75 100][C+75 , 
][C@3+75 100][C+75 , CtrlImg][C@(0.0.255)+75 `::][C+75 exclamation());]&]
[s0;l320; [C+75         iw][C@(0.0.255)+75 .][C+75 DrawRect(][C@3+75 20][C+75 , 
][C@3+75 100][C+75 , ][C@3+75 30][C+75 , ][C@3+75 30][C+75 , Blue);]&]
[s0;l320; [C+75         ][*_C+75 drawing ][*_C@(0.0.255)+75 `=][*_C+75  iw][C+75 ;]&]
[s0;l320; [C+75     `}]&]
[s0;l320; [C+75 `};]&]
[s0;l320;C+75 &]
[s0;l320; [C+75 GUI`_APP`_MAIN]&]
[s0;l320; [C+75 `{]&]
[s0;l320; [C+75     MyApp()][C@(0.0.255)+75 .][C+75 Sizeable()][C@(0.0.255)+75 .][C+75 Run();]&]
[s0;l320; [C+75 `}]&]
[s5;%% &]
[s0;=%% 
@@image:1620&1869
(A0QBdgEAAAAAAAAAAHic7ZwHfBPl4/+fpHtSKDJkyRJEEAEZggVliqWyERDKEGQPARUcwFf9uuXv/LoVfn4FkaEoiqJflSHKbCmlezez6d4jyfO/y6VputBics/l8nm/3q+80iNJL/c8b+5yTUJpPUj4RcWUaE4yKZ73vgSr0zLIzEwyX0UitcKlYkkOidST+VqyQM9fWVDrXAO0mA/JPB1sNCvqz5Ppgmr+MlxruaK2XOF+tCwJzyYTVbzcFf5HLb98Yg4ZE0ObQUjYe0WpzyZT6BP0xmd4e/6b9nmNDnqVDnqbjnyDN+wd3jHvNqtwA3iNTeQ+Mh8Fidh4swgp2S6bk0+vvlySnFyh3muNXM5k0JnGISsX6rmQlWuLfLaYuVvacu7+Ei9X9K2v18mlLWi/EELYWK4dQfsfhSSFfaWQmE1uof2PQoY2hTY5fTaYOMncEjKqbgdN7jlHpmWRmUmK5QZOsiJPkOvaY32Zx4Zqrm6y2eSxiXIK17lL20LhirDQ+xEzt1sXbgmhYq0ZCttBiEUoRVCxulq5vMpjZanH8iLF4jKyqJjMt1w+WMz9aF2yON/qglz+klsyO8d6cM4t4ZyVTcZdIWEpdS2HXyTz0+vkXhQv1Qtdc0UL8l03I/+v3CrZXF4EIWzWh3J5lxgUS3K442FO/rzTXA3vbLVVLlLBmZm80zI4FREp5L4U/vyV7XQWF/Lw86TDAT7kwSf4f+Vanplk1S5nW9FWVxXXaVu4Io+/jf1/IJBXD6FVLtUGCtk2SrW21tpmbQrxcuVyci+Twy5Y5ULmEua8Ybf1yj3n+HPXzbRss0G//BLuBsIJ7Ui7dRb+h4EQctr2sPY73GlZXjMyHn5Dq86tae50dHM8/EK61yhLxYPO8PH2O0ZuPsi33GtvUy2rmmzZ1q9dwlrLX6m09Sq2HSe4s7YRdGeFnQ4UtG2TaVncXpgLuTozxfDKZv2Ti7WPzNasjlA/fK9qyVjVQ+NVi0ZzZi8YxZk1ZwjvzNszIvqmTOgWPfOOgoQYLmc+ZKHlHj/yFQc/zR1p8z+GXVBOaNgyX6vQrK3oJTl1CyPrHzw0/l8IQub5SET+EDqrtuIs4Yia28PmvvZYwWevF+59O/+DfxteezTnhfU5/1qp3/Gw/qmlum2LdNsidY/O026ao1k/Tb1ysmrxmIzZg6Mn9vw+/HZ1TrV1v9z/z8Yt80fj9jvlSL1QrqDwOl14qV53UN3kcTXzHaJEZN6RFGQekRS09mtvBgnnW9bvWF64/92igx/kf/xS7htPGF7ZlPPvdfp/rdRtX87lrH9yie7xhbot8zQbZqhXT1E9NC5z3vCY+/p+OeQG/kwX1zIn1/JNf9a1zNXNvZoW3tzFbX/bW7nscuZatj/txp95a3K/zLwgCKXmdHW9kCOyeS0t67bM5Sou2PNa3js7DC8/kvPsav3Ty3RbI7Wb52k3zNKsm65eGa5eeR9/1L14TPaCkRlz7oiZ0u/gsA58y8NieIWWudfLaBlCZ9t8y9oNM3Lf2Jb71lNcyBUXTvJLNs3TrpupXhmhWjZRteie7AdHZs8bwS0v+elQ1uxB6TNvv3J//0MjuzTd8o0/83vqUTH8OXDueID71ZaWhVrtQ7Zpy9n6dzG0DJtzOrRTqNiuZc3qCMML6w0vb+T2yLbT1JaQJ6kW3c2FnDXnDtvyrBkD0mfcdiWiqZaFY2yuZW7J3Za/Z82ynDafpxP2y40rbtwyn3Pde8iRM4SNbL5l9fKJ3Ktj/tB6+3Ld45G2bFVLxqkWhmXPHVYX8v19sqb1y5je/0pEv0N3duZbHpHAl3tLdL39sq3l6ZZfzbeca3vvh/AeFa+H8gWtRS/MtZdvHzlDeA3tWw6va1n3xGLdU0v5yy0PatfPssXLvTquCzmiV1Z4j6yI3hnTbomdcsvhOztZWx6a0HTL3JE8l6GwU65t2WNxka3iejk3bDmXf4OobQc9OwdCWE/b3jlcKxTNB7vwLt2WefzJ6scWcq+UNRtmqtfc3+CdIarpfVRTe2ZPuSk7okfm1N5X7+v91fCOdS0Lu+Z+x/iWuai5hRNVlp1yjvU925aKeWvfQeq9olSQu87nLPwrZ4OcoU3mkwdKyumG+qr5lucN166brtv0gG7zPO0jD2jWz+DfMbJiUt1r5wf6q2ferJ7WQx3RVR3RLev+7nGTu381tB3fclgKGZlo3TVz++XQV+q1XBsyWWT9pAYfsuXjEraWbUXX5by4iDzIf7iD/SffIZSms4vqrM2ZP3ieNVi9Kly7cZb2kTm6jbO1G2Zo1kY02C9rZ9+sndFdc39nTUTn7Cmd4yZ2/npIG77lUVlkVAbfcs84fr/Mtdwthg+ca3l2bcuWkD2WlCuWVSmXV1k+llXtvbqqTiHnJeU2+c9nzbcoRA2h4NwSWM+6ogu5HjOn3qp6aLxmzf1cxdoNMzXrp9clvDxMuGIuK9LN6q6b1kkb0UF1X4f4se2+GdSKb/keHbk7k8/5thS+5Rve5FvmfuRbzhFC5lu2q1iwXssWuYXW29gVDW1a/3OD0F67orkeM8J7Z0eGqVfcq1l7v2bdtLqQV47VLr9Lt7TuPLZ+WkfdlHaqSaEJY0KODPC3tszJ7Z1tLXM7aL5ly/G8EDI3FZfVhrymRrnazF16rzU2VshcKJq7CydZDCGs7wI7bUXPLuV6TJ/QNWvuMNXSseoVk23ZalZO0K64R7tslG7p0JzIAXU539daMz4oflTAkVu9+ZYnFfDZCjkPPlHXcriWP8CuPbq2hWxfsfCdJPZ6rKdc6ZyK1dWcZLmJU4gaQgX/f7sR1kZtrJf2vEqux7S7O2ROvy17wUgu57IT3/EhLx+nWX6PZlmYdukI3eLB+gX9cub14pZX/LBHf2+w+m6/+GFeX/dRWlvm5FoekcO/Y8R6jJ3Ff7PfXOsprwYhc816P2L98h/hq4G4K8ISTi5nTuG7UISuIbQp/PcO67QFvqCK6zH1rrbcYXbmrEHZD96pigxTLQpTRY5SR96pjhyuWThY+2B/3QO99bO6cQfY3E5ZNz5AFeYVN0T51c0KvuUpJdZdM9fyoDN8yzdG2VpusFP23lDdRMi1Wlu2fbnT+jqZTyEIpWbjorkeE4YHJ4/rnDqlb9qMgemzh2Q8cEfG7EEZcwZlzrkta3Y/1czeqmnd1FM6qu8NVY8LUo32yRihvDyQfNmTWFsWcuZaFo6x67cs5Gw932U7rt5katzytXOGzCcPlJoNclbnmS5O7nthROiF0Z2ixnePmtQ7atLNURN7RU/qeXli95gJXa9O6HR1bLu4u0OvhgXH3el3dajn5dsVJ/uRA3d141ueXlqv5dBX7FsmD9a9Xq77U1SDou0OsJsMmfkWg1CCNj7Yfvj9svy46O/G9917a9DB21sfGNRm/+C2+29vc3Bw6KHBoYcHhRy5Peib2/yPDvA90s/7SF+Po30Uh3uRg3d1y4mNevitomZazrC1LFxa3wTC76BL6/1BynYG2/JquoHCOtv+jOXmsn9pBiVlvfNg/Okvr8iKh/9TQluIOtfIhew1tbje6+UGLUdYvgjC+vUgdW8Aq/siUPtvzV1ZLZy7tpfUnsRm/1cAScj89KkUZD4KEnBBlXCyi8yrtP5B6oFyMreCl7syu5TfwwqXwhWb0wvJlAL+UvhX2wvkBjZomc85g/+moHFRivHRygm8tu/zFL7qk/+2z2kZfPK2T1U3/v4T+091QTgxA/JlhTezKcal8B9r4gyrdXSS9ce7axWWcJcjE3lHJFjfhi18toIrt8mWx0XxLVtythXdpMINeG23t7sj5BQ2ppurgBaduJGF93AK57E7RvnckdRudb7v7NSWHroDANhC7i0io5JI9/0k9FW0DIDrgpYBkAdoGQB5gJYBkAdoGQB5gJYBkAdoGQB5gJYBkAdoGQB5gJYBkAcNWvZDywC4JnzLYSmky260DIBLw6plsnOnTWf/LgDcARFats/27+vAFQDAHXBqy9dXMYpmBcbFpXFSy/+8YswcB+LY4cDASROHt4yJwRBWzWIopYBjW8YcEBnmVWJYpYMDW8a4iwDz7jDEksUhLWO4nQ3zvhC19PnnLWOgnQfzmiQi63FwDRq07DsoES0zh3k7kpX1yEiaf9gyRtaBMC/FhWQ9VlIELUsB5mm4qKzHTVqgZbYwz0EGsh5DqYCWWcE8AZnJejzZ809aZj58LjqCzDeajGU9tizBfllkmM922ct6hJmBlkWD+SR3K1mPNgPQsjgwn9tuKOsxFxu0LALMZ7XbynrkRQXv+3I2zOezm8t6/MWjQcs+gxLQsmNhPpndXNbjLx7Wlrtef8sUn5NqHuYzGRLXmS3/EIe0bH0ojEt9mM9haJP1XBADB7ZM8b0i9WE+gaFN1nNBDBzbMsX3fdnBfAJDm6znghg4vGXrw2Ig0LKUZD0XxMBJLVsf3I2HgPnshQ1kPSOcjmPOY5Odja3717+xnRVPPfn+b/18V6+U05ZnPnuhTdZzQQxEaLnpu9TfzhcvHqypbvfBB3PMZvP1PxmJwXwCQ5us54IYsGq5AbrM259/Ligrrc3Ro/9r0R2lDPMJDG2yngtiIIWWk5MuV+R7B3n3SY4JfeuNbS18BtKF+QSGNlnPBTGQQsvnf51tLvBaOW+kKd879veOsbGJLXwS0oX5HIbEPUKmEmg5L89QnBRMsxT/93oXqiYmfcC777zb8uchXZjPZDeX9fiLB/OWf//xMVOKN00mrz7tXx6rpFkBxz+7zWDIa/lTkSjMJ7Oby3r8xYN5y9knbzHGKk1xZPywEbrfA41XlMUXfT///JuWPxXpwnw+u62sR15UmLf8876ZR9/q8N3bHfa93PP7dzpw1795s/Ozz37c8qciaZjPajeU9ZiLjVPf9wXsYT633UrWo80AtCwyzCe57GU9wsxAy+LDfLbLWNZjyxK0zArm015msh5P9qBltjBPQAayHkOpgJalAPMcXFTW4yYtGrTsi5bZwTwNF5L1WEkRvuXRKaTLHrQsHZiXIllZj4ykQctShnk7EpH1OLgGaNklYF4TEpY+aNm1YN4XEpYsaFlkCGnivestst6jse6u03Or3jo7avyrk8n27YiXLWhZZBzbcsMHFzfkW19bWlREjDUBlHp9duSWgBkrybZtiJcVaFlknNpys7/UCSG32b72RFz74gIvs3kwNfcqL1U+89yIwPBH0Swr0LLIMGn5767b3w65oCD/4tkFBhUxVvtT6kvpjeYa34Ic7zlzpp46ddV5awiuAVoWGSm3/PepLttlyFBUl3r/8nPwxjWKrY/5lJZ0Nld5ZaX6h0c8Hx+fxnoF3RG0LDIyaDk97ZIumZQbPKkpiBBvX9LLk4zcubkPNbahpcoDn/V9ZNN/8/MLWK+m29GgZbwf29m4est6fXb8ueGFaUpTeSA1e/bpHhKoHBqiGL/37baUtqblIcUqz21bJn740S9s19MNsbwfO5V0Rcsi4dItFxcXRf++XH9FYczzp8W+tIq88ax/9w4z7hkwv1RDaJEHrbmR5vqoY/wj7ou8GpfOcFXdELQsMi7dcnz0J9oo75IkD1oYQDUKmkOiTvqOHBC5adE9tIRQNaEGb3Nl+xqVMu5E66kz3sjO1jFcW3cDLYuM67as06boL/kUXlGYcoKoyoNmEJpF9GlecyYP3/taR1pAaCah6dzeuY0pt7U5k3z46sAnnz5UUlLKaoXdDbQsMi7acq5Bk/z7mPyLiprsAKrypSmEN5VwzR79oF3Sr/5c19aFaR60tKNJFVgY4/3YmvAv9p9lssJuCFoWGVdsubS05PKJVfrTipokP6oKoIkKmkBoIr8jPved546t3rt2BprSLCFzC+O5nH1oRTdzoofqlP/U8EUJCVnir7MbgpZFxhVbTov7SnfKr/iskmYE0DgFjSX0qqXZTBLk5x/o3bO1T/hHzw7kd83c8liLaX4m7Q3GWMX5g22nz/qPTmcQf7XdDbQsMq7Ycv6lkaofSemfiuqoWi8pqqMV5kTy6paQ8UPu+ujF17J/7VITS6qjCP9P/A1IdYxX2Z+Kmmjy4IxpL758oKqqSvw1dyvQssi4YsuXT7+d+n3rq4dI3AESd9ByKVw5SFKOEPUPJPMoiT9U75+uHiAJh0jmd+Tsf9v27bsscsk7Gg3OaTsXtAz+DjkGw8efHF+1+r01a99f2xIfWv7hk0/tuxyTYjabWT8JmYOWAZAHDVrG+7EBcFGs36nbBd+pC4Brg5YBkAdoGQB5gJYBkAdoGQB5gJYBkAdoGQB5gJYBkAdoGQB5gJYBkAeNWk5EywC4ItaWu6JlAFwbtAyAPEDLAMgDtAyAPEDLAMgDtAyAPEDLAMgDtAyAPEDLAMgDtAyAPGjQss9gtAyAS1L7nbpoGQDXBi0DIA/QMgDyAC0DIA/QMgDyAC0DIA/QMgDyAC0DIA/QMgDyAC0DIA8atIzv1AXARWn02Qq0DIBLgpYBkAdoGQB5gJYBkAdoGQB5gJYBkAdoGQB5gJYBkAdoGQB5gJYBkAf4Hk4A5AFaBkAeoGUA5AFaBkAeoGU3gezc2ZysVw04BrTsclyjSjFlvRlAQ9CyNGGeKjJ3OdCyFGBeH+qWAWhZfJiXJQVZD4IMQcsiwDwc6ct6iOQAWnYGzNNwdVkPoEuC92M7CubzX66yHliXgW95NNfyHrR8fTCf6u4j66GWOmj5+mA+sd1Z1oMvUdByi2A+jaG9rKeDtEDLfwnzGQv/jqynCXvQcnMwn5zw+mQ9cZiBlhvAfCpCR8l6KokNWrbBfO5BZ8h6WokHWqao2A1kPcXEwM1bZj7HoJiynm7OxT1bZj6pIFtZT0Cn0Khlmb8fm/ksgtKR9WR0MO7TMvOZA6Up64npMNyhZeazBUpf1pPUAci7ZeYzBLqWrCfsP0KuLTOfFdB1ZT15rxP5tcx8JkB5yHoitxg5tcx89KH8ZD2pW4BsWmY+6FCusp7afxcZtMx8rKE7yHqa/zWu3jLzIYbuI+vJ/he4bsvMRxa6p6wnfrO4aMvMBxS6s6ynf9O4YsvMhxJC1hE0gWu1zHwEIbSXdRD1cKGWmQ8chI1lnUUdrtIy8yGDsDlZx2HFJVpmPlgQXlvWifBIvGXmYwTh35dxLBJumfnQQNhSWfYi1ZaZDwqE1yezZCTZMvPhgPCfyKYa6bXMfCAg/OcyCEdiLTMfAggdpdjtSKll5hsfQscqaj5oGUKnKWo+kmmZ+WaH0BmKV5A0Wma+wSF0niJFJIGWmW9qCJ2tGB2xbpn5RoZQHJ2eElqGUBSdnhLTlplvXgjF1Lk1sWuZ+YaFUHydGBRahlBEnRgUo5aZb1IIWemspli0zHxjQshWp2SFliEUXadkJXrLzDcjhFLQ8WWhZQhZ6PiyxG2Z+QaEUDo6OC60DCEjHRyXiC0z33QQSk1H9oWWIWSnI/tCyxCy05F9idUy840GoTR1WGJoGUKmOiwxtAwhUx2WGFqGkKkOS0yUlplvLgilrGMqQ8sQstYxlaFlCFnrmMrQMoSsdUxlaBlC1jqmMrQMIWsdUxlahpC1jqnM+S0z31AQSl8HhMa3nEq6oGUIWeqA0NAyhBLQAaGhZQgloANCQ8sQSkAHhOb8lilyhvCaOqYytAwhax1TGVqGkLWOqQwtQ8hax1SGliFkrWMqQ8sQstYxlaFlCFnrmMq4lsPwfV8QMtNhiaFlCJnqsMTQMoRMdVhiaBlCpjosMbFapsgZwkY6si+0DCE7HdkXWoaQnY7sS8SWKXKG0E4Hx4WWIWSkg+MSt2WKnCG06Piy6rfsg5YhFEXHlyV6yxQ5Q7fXKVmhZQhF1ylZsWiZImfoxjqrKUYtU+QM3VInBoWWIRRRJwbFrmWKnKGb6dyamLZMkTN0G52eElqGUBSdnhLrlilyhm6gGB1JoGWKnKGsFSkiabRMkTOUqeIVxLecSrrsZt4yRc5QdoqaD1qG0GmKmo+UWqbIGcpIsduRWMsUOUNZyCAc6bVMkTN0cdlUI8mWKXKGLiuzZKTaMkXO0AVl2YuEW6bIGbqUjGORdssCzMcIwmvLOhEel2iZImcoYVnHYcVVWqbIGUpS1lnUYW25qwu0TJEzlJisg6iHa7UswHwEIWQdQRO4YssUOUOmsp7+TeOiLVPkDBnJeuI3i+u2LMB8ZKH7yHqy/wWu3jJFzlAUWU/zv0YGLQswH2soV1lP7b+LbFqmyBk6QdaTugXIqWUB5qMP5SHridxi5NeyAPOZAF1X1pP3OpFrywLMZwV0LVlP2H8E3/LoFLm2LMB8hkDpy3qSOgB3aFmA+WyB0pT1xHQY7tOyAPOZA6Uj68noYNytZQHmswiylfUEdAru2bIN5pMKiinr6eZc3LxlAeZzDDpb1lNMDNCyDebzDTpD1tNKPKTWMiE7JSHrGShZlTue7vjqlrG7l4buWMd8Za4tqznMCrTcpBS76cZu3+61fduwNx584dTdn0YNX7J7bLdtCzy2biU7drBfNztZTV3moOUmrbdKrCenRPR6euuQtxfvjr69tMq3xuSdVRTy5v+G9ty22mvzo2TrNq50tqvHasZKB7TcpM2uHuugmLljR69dqz85e2t6TmBFpdJoUlQaPeOz27x1eNDANZGKpZv4nEVfKzFnpvSxtJxKuu5By/b+9Xoyj0tcQ3esXXd0UlRmm9ISUlPlZTa1pjSAizpLE/jIi2Pbha/yXLCWbHoUCTMELTdpC1aYdWViuGPHoH/P+vZK92SVX0mxh6kmxGweQM1dqFlJzeTbo90j5s9oPXazInIDefpp5718dt6skwdouUmvZ82ZF+ckd+zw2vnkyv1jk7P8tFplabGf2djdbI6g5iHU7MvlHBcX8tKrQ3oOWeE9eZPi0Sf4nB26Ag6fY3IFLTfpP30WzAN0nMontg58fclHZwbo1CRPT8pLA8xmruV7qHmQ2dTGWO1ZkO/188+dw++f2Xb0Rs/IzeTxreiXCWi5SR38pFj3+E/0enTjsk9Hn4htr1ORQgOpKg+g9EZKe3OaTZ1qKryN1SQjPXjnjjtvHbfUc+x6sn7TdRxmO3aDuydoWYSWGz5H1nm2SJ+Nq176omdaurdBQ0oLlDWV/pS2obQ1pZ3Mpm6mSh9zDcnL8/nxu87T5t7vdetmz4e3kieeQrzig5bFb7mJZ8062Ob0fWz9qOfu+/a30JxsUqBXVJX5mIzcfpnTm9JQs7mL2djKXO1VWaJUZ/rt/NeIzoNW+0/bpli7BeWKD1qWQstNbAfWFQve9twDB871jk/yz1ORsnwPY3Wg2RxgrPEuK1FUVQZS2oG3KpCWE2okhw/1mjRtdusRmw4fPllTU2M2m1lvRfcCLUuz5WsgWsiKnTuWfT4hIdlfleZRrFcYS73MxmBzTUBpvkdCtFKT5WM50u5Ma0JoJaEmculC6HPPhIWN2bZr13f5+UVczqw3lXuBll2u5evjGs02efvS0uKSonNlBYv0aYq8LEV5vmd5sX9yUsj+ff7bHlWsWKrYuM7rnXeCkxK7VJa3M1f7mioVBo3Pd9/cFD5l/roNn548eaWgoFDk5+jmNGjZFy3LtOWWotFkZqY8V6Adok8lJWpSXeKj1QZ/8nHwuLG+hHgoCHcZ2Ofmtrs/vik7k3vV3NpU4mkqI2lJrR5/NGzevB3Pv/B1Wno2DrPFxNpyl91oGS3bk5R05dKfkwxpQXnppDJHYSr3v3IlJCLCPyTER0GCvMlNnqRP+7a3LJp96/GjPc3GdmbuCLycGDS+X+3r8eD8hTNnf3ThYnJlZRXr5+FGoGW03BiNJj3m0vvxF2/OSSGF2YqaYm9aE3DxYuDgwV6E+HmQdkFkkC8ZGRI0fPywgV/u6UlN7Wh1EC33qjB4ZMQGbt82ZvyEZz/59GRqajbrp+JGoGW03Ji42G/LDDNzUm7ITSGlWg9jVSClgRcv+gy9gzu6DvAkXduQOwPI2JDAsPFDbvvy0y6Wvzi3p2WBNI/QYrL///rMnrls5ao9x344azKZWD8bdwEto+XGnP7tzfy0LrpEn4J0RZXBy1QdTKl/1CXPsDs9vT1DPEmP1mSEP7m7batRM+6+5dt9N1j+4tyJloXQfEJLyZ+/dnhiy4Rx4555972fSkvLjEYj6yfkFqBltGxPWWlpdmZM9J8rdHHehgRlqcrTWOxnrgmmZt/4q8qFc3xubNfek9za1mN0qHJ873ajNj7Y4/QPXOk+/Bs7q24wF/ua8pXaJL9De2+eOPGhLY/tvXgxqbi4hPXTcgvQMlq2R6dTqVJfL8ocqYslBUmkUu9DK4JpdQA1eWWlK17c7jN0YGdv5dBO/pO7B0wJ6x329tOdEs77cP9KTTfQmo7m8lCj3tNkIElRIasenjBv/otv/+d4VrYGJ7RFAC2jZXuSk2Mv/x6RE986J46UpCur8/1pZSta6kMrlbl6cnSfcvrkDn5ew/u2e+COLg/MHj3m+09Dc1KUtNKDVgbSmg7mqk5mg7fZQLTJ/rv/c8sDc1YsWrInNjYd7xsRAbSMlm3o9dlXo3cn/NFPd0WZl6CoVHubigJpeRDN96RlpCSfXPqVLHmgbZDfiGE3rxl72/KlU8dc/CWkMofQIgUt9KQVobSmMy0KpgbPsgzPuFMhj62fOHHSi/u//D0rS8P6yckftIyWbSQn/FSmXaSP7aC/TIqSlDU5/pRruciP5ihpEakoJBlRZOOy1u1a3zlhxDNTRz+ybsHIpJhgWkyogVAdocVB3GE2rWxnzvU3ZZLqbPLpW7dGTFmzact/f/3tEg6znQ1aRss2zpx8Nz+xtzba33CFlKZ6GPMDaYE/NXhRrYLmElMBqcgmL29t3b/bmDn3vrN46pan1/RPuxJACywhqwnN87Xsmjsa84JqMolJT3490mnjqvB7w5/9+NOTlZXVyNmpNGjZZ1ACWnbDlsvLy7WaxJg/1mmj/LSXPArilJXZ3qaCQD5PjZLvVE+Evx1/+npI2IARwwY8s3Dqok9e7qxN8eFb1hCq4or2pIX8rtlU1Lom26NGRbIuBux7r9+48cu3PXEwNjajtLSU9ROVM3zLYSl2n61Ay+7YsiFHq0v/qCh1rPo80V8iJcmeJp2/OT+A5njTbEunasuBdDX5em/w/WMGhrZ6eG74hN++bFWg8eBb5v6Vv5mC5vjSyvbm0rYmrV9VqrImnSScbL1kwb2RkS/v3vM/jUbP+onKGbSMljnSUuOv/jHXcKW97gLJjyEVGT7m3CCzwZ9qPGkWsebM7ZqryG/HglfO79+x3cKHZo26/INPiV7Bvz+Eu0GmRY0XLQ01V7Q3FbSpSvXkWtZc9n/v1f7z565aufrzhET8ccqJoGW0nJerS4rdn/zHIP1Fr9xLpDRBWaP2p3nBVMvtlJU0g/BmWg6kS0js7/4vPtajV5cpGyMHZp/xqNRZ9tdZltukc1eU1BBAy9qbyzoas3xN6YqSBM/oH9tsXjNp8pRXD391SaMxsH66sgUto+X0lFNV2nWGy53050lhjKIq3cekD6S5gTTbg88zzWK6ZedrINrLXvvfaDf45rueXtm7OFZRoyVUawmZu00qd6mgWdyr5lBa2YUaWpkzvIxJpCKRfPBy//D71j+5/cszZ66wfrqyBS2j5XOnPyqKH6C/EJB7kZTGcTvlQDPXssaXZij5PFNrc87gXxeXJylPf9F63JDhL23uUZNKzBpL40LIVhXUEEwrbqRlHYzqwJoEYswkP+/vsm7Z1OkzX9q77w982sJJoGV3brm8vFyjio89s95wITDnrEfRZY+qFF+TvhXVBtAMTz7MlFqForOIKYOkn/J7eVOP7z+6gT+0VlkaT7EziavbhxZwu+Zupty2xhRPU5oi60zg3rf6T560YvvOr5KS1NzvZf3UZQhadueWc3I0muT3ihPH6P8geedIeZy3OTvYzLWs8qXJlio5ky2mWFrO5OMtTVZePR6gPudjPX2dXnsbwUTuxkqqDqDl3WjxjWZ1oDHRw5RIEn9pvXzBxKVLd32+95RWm8P6qcsQtOzOLackX435ZZrhfBuu5eILiqoUf7MuxKwJpOlefJKCSbUtZ1heGucQo56UqpWVGgV/ZltrCdwWfpLlLglc+N60oB0t72ou6mhK9jYlEe0F/09e6bdw3qpV6/YlJqlxQtvhoGW3bTnXoE2I3pv4S3/daWXun4qKGE9jeiDVtqIZPjRZyfcoKOScTqrSiD5KeWS3147HfXZs83xpu9++twN1FzzMwovlpNr2ubvEc1eUNDuAFnU0l3U1ZwSYk5Sllz1jj4Y8unrS5IhXvzx4VqXG35odDFp225bTk05WZK7R/9lJd5IUnlNWJ/iaM4P4AJOUfIxxFuNrc84k+fGKHz/xnjXRn5AgQgJa+befMLTLTx8GFkZ78LvmpNqKuXtdtVwmcjvuNrSkC9W0Nid6Gy+Tysvkg+cH3Dtp7aPbPj9xMor1BpAbDVrG+7Hdp+Vzpz4uvDxQdyow5zQpvqisSQngdqA01ZvGK/gYY2uTjLe2nPS7ctW8gB43+hHiTUhrD2X3Hjf2ezyy69mvg/j3YyfX5n+1Vu56ZgDNbU8N7Y0pAdXRxJhEju3p9vDCGVOmPv/Zf0/h+0YcS23L+GyFe7VcmJ8d/8cTOaeDtSc8C84oKi97mdICaZovTVBaQ46tzfmq8PqXRP+ovGe4r5+3v6cyNNR/cKjfxF7tI5ZHjP3fvm78C+dkUu+OgkneVNWK5nYwZrSqiVEa40jK8aAPnxs4JmzN9h2Hk5NVOKHtQNCye7ZsLPiuJPFB9c9E9yspPuthivUxp/rTZC9rj1fq5xzPn/uKPqYYPdTbxyvI17NHz9bT77jpsfAR/9q2+KFTBwfwZ8CS7Fq+Umuckqb6Ut0NpsxWxljPmkuKivPk/P62U8bPXBD55sGDJ/R6nNB2GGjZPVuuUm0vjB6jOk70v5DSs0pzrKc50YsmeNQdIdsfKifw574ST3gsmxHStf0NXore7XxmLpzyf7tfP7P/jfVXf+rL/2Uqsf4BtvUoXUETPWlGoDklwBjjVXFeUXWJJB4LWb94XETE1q1P7klJSWO9JeQDWnbPlkviInL+6JN5jGh+IoWnFZUXlNVRnIrqSwr+UtB2/bLCFEfyziuPve+3eVHbsXf0nTBk6o41T//6xfOx344x/BFaE0eqL5PqKFLvXtyVS5YlVzyroj3LzylLzpDKCyT7l8BdW4dMGr8kfNquk6eiSkrwQUjHgJbds+WCqLs0J7qmHiVczrqfSf4JUniSFJ0ihZyna7X8WHSaFP1Oyv4kVRdI1WVybl/Qe0/0ePOxu754edS5L/rknfIrP0u4SIt/529pu5fwCNwDchaf5h+c+xX5v5GKs0TzS8D7/xowfvSsQcP/vXffmawsLeuNIRPQsnu2XJ60QnfmjvivSPIRknaUZB0j2ZzfWy6bUv0j0f5E9L9y+3GPrO99M79vlX0sWP2jn/a4UnOcqH5o9o6cqtq75/1KKv4g6d8HP7XqrqGDF942ZPsrr34TfTme9caQCWjZPVtOij0e9fP69OOdk7/xiT9MYg+SqwdI3EGLBxppWR5/iCQeJunf8uWqfyBZ35O0I/wSbrn1jgfr3Z67cvWA9WG5m6V9w0d95UDwZy/0DR87s0evFUOGPfXSK19HRcWx3hgyAS27Z8scl84ejflpwZkvRx37oNehXe0PvxZ6eBdnm8OvNXJXG2H5V5y72nz9/3i5KzYP76q7jf3tBbl7ffN6m2NvtTn+bts3t906d0p49x4rOt+0ceLkZz/d/UtycgbrLSETpNYyEI38/LzYKxeOHT3y3I5n5kx5YPqkqdMmTpk26VpOb8pr30Vwxr3hsyaHz7g3YtSw+T16rujYefUdw7at3fDh2XPJJSVlrLeETEDL7kxFRUVGRtbXR04+89zhDRs/WbX6vTVr31/7T1xn0W7JGouW6/yDL1/xwdLlH61e/dErr33z4/GLOTn5+Dizo2jQss9gtOx2FBaWxMXrvjsWc+Dg+YOHLxz+6qLVry02+LHxksY3+Lq5G1w49NW5r74++8MPUUlJmpoaIz4t5UAs36mbQrrsQcsAuDRoGQB5gJYBkAdoGQB5gJYBkAdoGQB5gJYBkAdoGQB5gJYBkAdoGQB50KBlvB8bABcFn60AQB6gZQDkAVoGQB6gZQDkAVoGQB6gZQDkAVoGQB6gZQDkAVoGQB6gZQDkAVoGQB5YW+6ClgFwbdAyAPIALQMgD9AyAPIALQMgD9AyAPIALQMgD9AyAPIALQMgD9AyAPKgccttZqiFnH2m5jQpfxcIodTkQh7wi33LQZOSQmal+09O9LwnoUnJ6NSm5R7KIY5uxrDUZmzpr2jucVKb/dWOesotfZwW/97mn5pzN53UhvI6Jl4Lp0SLf7Wjbt/84wyPJv1/Jl0/41tud5j/kAXvntorLXJPM7bwcbq00JY+TrPruafld2nuV+9p2hZvuubWs4W2eD1bKqOp4rDHabnOHuLrGYLP+CudPuRbbvs+fwkhdHH/P5Qt4tU=)
&]
[s5;%% &]
[s3;:5:%% 5. ImageDraw&]
[s5;%% [* ImageDraw] provides a Draw target that creates an Image object. 
To define the alpha component, use Draw target returned by [* Alpha] 
method and [* GrayColor] to define the value (if you never use 
Alpha, it is considered to be 255 for the whole Image, otherwise 
it is set to 0 when Alpha is called for the first time).&]
[s5;%% &]
[s0;l320; [C@(128.0.255)+75 #include][C+75  ][C@(0.0.255)+75 <][C+75 CtrlLib][C@(0.0.255)+75 /][C+75 C
trlLib][C@(0.0.255)+75 .][C+75 h][C@(0.0.255)+75 >]&]
[s0;l320;C+75 &]
[s0;l320; [*C@(0.0.255)+75 using][C+75  ][*C@(0.0.255)+75 namespace][C+75  
Upp;]&]
[s0;l320;C+75 &]
[s0;l320; [*C@(0.0.255)+75 struct][C+75  MyApp ][C@(0.0.255)+75 :][C+75  TopWindow 
`{]&]
[s0;l320; [C+75     Image image;]&]
[s0;l320;C+75 &]
[s0;l320; [C+75     ][*C@(0.0.255)+75 void][C+75  Paint(Draw][C@(0.0.255)+75 `&][C+75  
w) ][*C@(0.0.255)+75 override][C+75  `{]&]
[s0;l320; [C+75         w][C@(0.0.255)+75 .][C+75 DrawRect(GetSize(), Cyan());]&]
[s0;l320; [C+75         w][C@(0.0.255)+75 .][C+75 DrawImage(][C@3+75 10][C+75 , 
][C@3+75 10][C+75 , image);]&]
[s0;l320; [C+75     `}]&]
[s0;l320;C+75 &]
[s0;l320; [C+75     MyApp() `{]&]
[s0;l320; [C+75         ][*_C+75 ImageDraw][C+75  iw(][C@3+75 100][C+75 , ][C@3+75 40][C+75 );]&]
[s0;l320; [C+75         iw][C@(0.0.255)+75 .][*_C+75 Alpha][C+75 ()][C@(0.0.255)+75 .][C+75 DrawR
ect(][C@3+75 0][C+75 , ][C@3+75 0][C+75 , ][C@3+75 100][C+75 , ][C@3+75 40][C+75 , 
GrayColor(][C@3+75 0][C+75 ));]&]
[s0;l320; [C+75         iw][C@(0.0.255)+75 .][*_C+75 Alpha][C+75 ()][C@(0.0.255)+75 .][C+75 DrawE
llipse(][C@3+75 0][C+75 , ][C@3+75 0][C+75 , ][C@3+75 100][C+75 , ][C@3+75 40][C+75 , 
GrayColor(][C@3+75 255][C+75 ));]&]
[s0;l320; [C+75         iw][C@(0.0.255)+75 .][C+75 DrawEllipse(][C@3+75 0][C+75 , 
][C@3+75 0][C+75 , ][C@3+75 100][C+75 , ][C@3+75 40][C+75 , Yellow());]&]
[s0;l320; [C+75         iw][C@(0.0.255)+75 .][C+75 DrawText(][C@3+75 26][C+75 , 
][C@3+75 10][C+75 , ][C@3+75 `"Image`"][C+75 , Arial(][C@3+75 16][C+75 )][C@(0.0.255)+75 .][C+75 B
old());]&]
[s0;l320; [C+75         ][*_C+75 image ][*_C@(0.0.255)+75 `=][*_C+75  iw][C+75 ;]&]
[s0;l320; [C+75     `}]&]
[s0;l320; [C+75 `};]&]
[s0;l320;C+75 &]
[s0;l320; [C+75 GUI`_APP`_MAIN]&]
[s0;l320; [C+75 `{]&]
[s0;l320; [C+75     MyApp()][C@(0.0.255)+75 .][C+75 Sizeable()][C@(0.0.255)+75 .][C+75 Run();]&]
[s0;l320; [C+75 `}]&]
[s5;%% &]
[s0;=%% 
@@image:806&618
(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)
&]
[s5;%% &]
[s3;:6:%% 6. Printing&]
[s5;%% Printing is quite similar to painting on the screen. To acquire 
Draw target for the printer, you need to use [* PrinterJob] (CtrlLib 
feature). Each page printed should be started by calling [* StartPage] 
method and ended with [* EndPage]. Drawing coordinates when printing 
are in [*/ dots], defined as 1/600 of inch (in fact, a pixel on 
600dpi printer).&]
[s5;%% &]
[s7; [@(128.0.255) #include][@0  ][@(0.0.255) <][@0 CtrlLib][@(0.0.255) /][@0 CtrlLib][@(0.0.255) .
][@0 h][@(0.0.255) >]&]
[s0;l320;C+75 &]
[s0;l320; [*C@(0.0.255)+75 using][C+75  ][*C@(0.0.255)+75 namespace][C+75  
Upp;]&]
[s0;l320;C+75 &]
[s0;l320; [C+75 GUI`_APP`_MAIN]&]
[s0;l320; [C+75 `{]&]
[s0;l320; [C+75     ][*_C+75 PrinterJob][C+75  pd(][C@3+75 `"My printer job`"][C+75 );]&]
[s0;l320; [C+75     pd][C@(0.0.255)+75 .][C+75 CurrentPage(][C@3+75 0][C+75 );]&]
[s0;l320; [C+75     pd][C@(0.0.255)+75 .][C+75 MinMaxPage(][C@3+75 0][C+75 , 
][C@3+75 1][C+75 );]&]
[s0;l320; [C+75     ][*C@(0.0.255)+75 if][C+75 (pd][C@(0.0.255)+75 .][*_C+75 Execute][C+75 ()) 
`{]&]
[s0;l320; [C+75         Draw][C@(0.0.255)+75 `&][C+75  w ][C@(0.0.255)+75 `=][C+75  
pd][C@(0.0.255)+75 .][*_C+75 GetDraw][C+75 ();]&]
[s0;l320; [C+75         w][C@(0.0.255)+75 .][*_C+75 StartPage][C+75 ();]&]
[s0;l320; [C+75         w][C@(0.0.255)+75 .][C+75 DrawText(][C@3+75 200][C+75 , 
][C@3+75 1200][C+75 , ][C@3+75 `"Helo world!`"][C+75 , Arial(][C@3+75 600][C+75 ));]&]
[s0;l320; [C+75         w][C@(0.0.255)+75 .][*_C+75 EndPage][C+75 ();]&]
[s0;l320; [C+75         w][C@(0.0.255)+75 .][*_C+75 StartPage][C+75 ();]&]
[s0;l320; [C+75         w][C@(0.0.255)+75 .][C+75 DrawText(][C@3+75 200][C+75 , 
][C@3+75 1200][C+75 , ][C@3+75 `"Second page`"][C+75 , Roman(][C@3+75 600][C+75 ));]&]
[s0;l320; [C+75         w][C@(0.0.255)+75 .][*_C+75 EndPage][C+75 ();]&]
[s0;l320; [C+75     `}]&]
[s0;l320; [C+75 `}]&]
[s5;%% &]
[s3;%% Recommended tutorials:&]
[s5;%% If you want to learn more, we have several tutorials that 
you can find useful:&]
[s5;l160;i150;O0;%% [^topic`:`/`/Draw`/srcdoc`/ImgTutorial`$en`-us^ Image 
tutorial] `- here we move things related to images. We show how 
to create, use and embed icons directly in application.]]