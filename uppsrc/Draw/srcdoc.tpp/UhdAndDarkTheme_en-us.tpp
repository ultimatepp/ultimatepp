topic "Supporting UHD displays and Dark theme";
[l288;i1120;a17;O9;~~~.1408;2 $$1,0#10431211400427159095818037425705:param]
[a83;*R6 $$2,5#31310162474203024125188417583966:caption]
[H4;b83;*4 $$3,5#07864147445237544204411237157677:title]
[i288;O9;C2 $$4,6#40027414424643823182269349404212:item]
[b42;a42;2 $$5,5#45413000475342174754091244180557:text]
[l288;b17;a17;2 $$6,6#27521748481378242620020725143825:desc]
[l321;C@5;1 $$7,7#20902679421464641399138805415013:code]
[b2503;2 $$8,0#65142375456100023862071332075487:separator]
[*@(0.0.255)2 $$9,0#83433469410354161042741608181528:base]
[C2 $$10,0#37138531426314131251341829483380:class]
[l288;a17;*1 $$11,11#70004532496200323422659154056402:requirement]
[i417;b42;a42;O9;~~~.416;2 $$12,12#10566046415157235020018451313112:tparam]
[b167;C2 $$13,13#92430459443460461911108080531343:item1]
[i288;a42;O9;C2 $$14,14#77422149456609303542238260500223:item2]
[*@2$(0.128.128)2 $$15,15#34511555403152284025741354420178:NewsDate]
[l321;*C$7;2 $$16,16#03451589433145915344929335295360:result]
[l321;b83;a83;*C$7;2 $$17,17#07531550463529505371228428965313:result`-line]
[l160;*C+117 $$18,5#88603949442205825958800053222425:package`-title]
[2 $$19,0#53580023442335529039900623488521:gap]
[C2 $$20,20#70211524482531209251820423858195:class`-nested]
[b50;2 $$21,21#03324558446220344731010354752573:Par]
[2 $$0,0#00000000000000000000000000000000:Default]
[{_}%EN-US 
[s2; Supporting UHD displays and Dark theme&]
[s3; GUI mode detection&]
[s5; UHD mode is activated when GUI font is larger than 24 pixels. 
Dark theme mode is activated if [* IsDark]([* SColorPaper]()), which 
means that grayscale value of default background is less than 
80. Note that both modes create 4 combinations in total `- standard 
resolution with light theme, standard resolution with dark theme, 
UHD resolution with light theme, UHD resolution with dark theme.&]
[s5; [* IsUHDMode() ]and [* IsDarkTheme() ]functions return respective 
current GUI status.&]
[s3; Scaling GUI for actual GUI font and UHD resolution&]
[s5; U`+`+ coordinates in drawing operations are always in real pixels 
for screen targets. U`+`+ provides various functions to adjust 
GUI elements metrics to host platform font size and UHD mode. 
Some of these function use `'font`-ratio`', which is the ratio 
of metrics of current default GUI font to `'design`-font`' (which 
is more or less defined as font where text `"OK Cancel Exit Retry`" 
has Size(99, 13), font ratio is then current GUI font Size vs 
this predefined size).&]
[ {{4916:5084 [s0;b42;a42; int [* Zx](int cx);&]
[s0;b42;a42; double [* Zxf](double cx);]
:: [s5; Scales the value horizontally based on font ratio.]
:: [s0;b42;a42; int [* Zy](int cy);]
:: [s5; Scales the value vertically based on font ratio.]
:: [s5; Size [* Zsz](int cx, int cy);&]
[s0;b42;a42; Size [* Zsz](Size sz);]
:: [s5; Scales size based on font ratio.]
:: [s5; Font [* FontZ](int face, int height `= 0);&]
[s5; Font [* StdFontZ](int height `= 0);&]
[s5; Font [* SansSerifZ](int height `= 0);&]
[s5; Font [* SerifZ](int height `= 0);&]
[s5; Font [* MonospaceZ](int height `= 0);&]
[s5; Font [* RomanZ](int height `= 0);&]
[s5; Font [* ArialZ](int height `= 0);&]
[s0;b42;a42; Font [* CourierZ](int height `= 0);]
:: [s5; Creates the font while scaling its height based on current font 
ratio. If height is 0, the height is set to current GUI font 
height.]
:: [s0;b42;a42; int [* DPI](int a);&]
[s0;b42;a42; double [* DPI](double a);&]
[s0;b42;a42; Size [* DPI](Size sz);&]
[s0;b42;a42; Size [* DPI](int cx, int cy);]
:: [s5; If UHD mode is active, returns the input argument multiplied 
by 2, otherwise returns it unchanged.]
:: [s0;b42;a42; Image [* DPI](const Image`& a, const Image`& b);]
:: [s5; Returns [* b] if UHD is active, [* a] otherwise.]}}&]
[s5; Usually [* DPI ]functions are used if the value is Image related, 
`'Z`' functions if it is text size related.&]
[s3; Color adjustment&]
[s5; If application is specifying any colors, these colors need to 
be adjusted for dark theme. This can be often done by using [^topic`:`/`/Draw`/src`/Colors`_en`-us^ p
redefined colors]. Sometimes only the light theme color is available 
that needs to be converted to dark theme `- this can be done 
using [* DarkTheme ]function. Alternatively [* AdjustIfDark] converts 
the color with [* DarkTheme] only if dark theme mode is currently 
active.&]
[s3; .iml files&]
[s5; Iml files most often contain images that are used in GUI interface. 
Obviously, these images must be usually different for any of 
4 GUI modes.&]
[s5; .iml should always contain images for standard resolution / 
light theme. These images are used to define the set of icons.&]
[s5; U`+`+ then uses smart algorithms to convert such images for 
the current GUI mode. These work acceptably well in most cases.&]
[s5; Developer might decide to provide dedicated variants for any 
image for any target mode which will be used instead of converted 
basic icon. Such variant can be either placed into the same .iml 
file or into separate .iml file.&]
[s0; The complete control of the process is available in image details 
in the icon designer:&]
[s0; &]
[s0;= 
@@image:924&1250
(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)
&]
[s0; &]
[ {{1666:8334^ [s0; Fixed]
:: [s0; Image is never scaled or darkened to match current mode.]
:: [s0; Fixed colors]
:: [s0; Image is never darkened to match current mode.]
:: [s0; Fixed size]
:: [s0; Image is never scaled to match current mode.]
:: [s0; UHD variant]
:: [s0; Image is variant for UHD mode.]
:: [s0; Dark variant]
:: [s0; Image is variant for Dark theme.]
:: [s0; [/ Export...]]
:: [s0; [/ This is unrelated to UHD / Dark theme mode, but if this is 
checked, the Image is exported as .ico and .png files. This is 
intended for application icon (e.g. shown in host shell).]]}}&]
[s0; &]
[s0; Alternatively, .iml dedicated for overrides for particular mode 
is used. This override is then used as parameter to `"iml`_source`" 
part of .iml inclusion process:&]
[s0; &]
[s7; #define IMAGECLASS MyImg&]
[s7; #define IMAGEFILE <MyApp/MyImg.iml>&]
[s7; #define [* IMAGEFILE`_UHD ]<MyApp/MyImgHD.iml>&]
[s7; &]
[s7; #include <Draw/iml`_source.h>&]
[s0; &]
[s0; The macro names for corresponding GUI modes are [* IMAGEFILE`_DARK], 
[* IMAGEFILE`_UHD], [* IMAGEFILE`_DARK`_UHD]. Note that theide tool 
that creates .iml import code will automatically add reasonably 
named .iml files (like MyAppHD.iml, MyAppDK.iml, MyAppHDDK.iml) 
as variant .imls automatically.&]
[s0; &]
[s0; Finally, it is also possible to disable automatic conversion 
for the while .iml file by #defining FIXED`_COLORS and/or FIXED`_SIZE 
macros:&]
[s0; &]
[s7; #define IMAGECLASS MyImg&]
[s7; #define IMAGEFILE <MyApp/MyImg.iml>&]
[s7; #define [* FIXED`_COLORS]&]
[s7; #define [* FIXED`_SIZE]&]
[s7; &]
[s7; #include <Draw/iml`_source.h>&]
[s0; ]]