topic "Supporting UHD displays and Dark theme";
[l288;i1120;a17;O9;~~~.1408;2 $$1,0#10431211400427159095818037425705:param]
[a83;*R6 $$2,5#31310162474203024125188417583966:caption]
[H4;b83;m`.;N1;*4 $$3,5#07864147445237544204411237157677:title]
[i288;O9;C2 $$4,6#40027414424643823182269349404212:item]
[b42;a42;ph2 $$5,5#45413000475342174754091244180557:text]
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
[s3; Table of contents&]
[s0; &]
[s0; [^topic`:`/`/Draw`/srcdoc`/UhdAndDarkTheme`_en`-us`#1^ 1. GUI 
mode detection]&]
[s0; [^topic`:`/`/Draw`/srcdoc`/UhdAndDarkTheme`_en`-us`#2^ 2. Scaling 
GUI for actual GUI font and UHD resolution]&]
[s0; [^topic`:`/`/Draw`/srcdoc`/UhdAndDarkTheme`_en`-us`#3^ 3. Color 
adjustment]&]
[s0; [^topic`:`/`/Draw`/srcdoc`/UhdAndDarkTheme`_en`-us`#4^ 4. Iml 
files]&]
[s0; &]
[s3;:1: GUI mode detection&]
[s5; UHD mode is activated when standard GUI font is larger than 
24 pixels. Dark theme mode is activated if [* IsDark]([* SColorPaper]()), 
which means that grayscale value of default background is less 
than 80. Note that both modes create 4 combinations in total 
`- standard resolution with light theme, standard resolution 
with dark theme, UHD resolution with light theme, UHD resolution 
with dark theme.&]
[s5; [* IsUHDMode() ]and [* IsDarkTheme() ]functions return respective 
current GUI status.&]
[s3;:2: Scaling GUI for actual GUI font and UHD resolution&]
[s5; U`+`+ coordinates in drawing operations are always in real pixels 
for screen targets. U`+`+ provides various functions to adjust 
GUI elements metrics to host platform font size and UHD mode. 
Some of these function use `'font`-ratio`', which is the ratio 
of metrics of current default GUI font to `'design`-font`' (which 
is more or less defined as font where text `"OK Cancel Exit Retry`" 
has Size(99, 13), font ratio is then current GUI font Size vs 
this predefined size).&]
[ {{4916:5084^ [s0;b42;a42; int [* Zx](int cx);&]
[s0;b42;a42; double [* Zxf](double cx);]
::= [s5; Scales the value horizontally based on font ratio.]
::^ [s5; int [* Zy](int cy);]
::= [s5; Scales the value vertically based on font ratio.]
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
[s3;:3: Color adjustment&]
[s5; If application is specifying any colors, these colors need to 
be adjusted for dark theme. This can be often done by using [^topic`:`/`/Draw`/src`/Colors`_en`-us^ p
redefined colors]. Sometimes only the light theme color is available 
that needs to be converted to the dark theme `- this can be done 
using [* DarkTheme ]function. Alternatively [* AdjustIfDark] converts 
the color with [* DarkTheme] only if dark theme mode is currently 
active.&]
[s3;:4: Iml files&]
[s5; Iml files most often contain images that are used in GUI interface. 
Obviously, these images must be usually different for any of 
4 GUI modes.&]
[s5; .iml should always contain images for either standard or UHD 
resolution and the light theme. These images are used to define 
the set of icons.&]
[s5; U`+`+ then uses smart algorithms to convert such images for 
the current GUI mode. These work acceptably well in most cases.&]
[s5; Developer might decide to provide dedicated variants for any 
image for any target mode which will be used instead of converted 
basic icon. Such variant can be either placed into the same .iml 
file or into separate .iml file.&]
[s0; The complete control of the process is available in image details 
in the icon designer:&]
[s0; &]
[s0;= &]
[s0;= 
@@image:924&1482
(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)
&]
[s0; &]
[ {{2001:7999^ [s5; Fixed]
:: [s5; Image is never scaled or darkened to match current mode.]
:: [s5; Fixed colors]
:: [s5; Image is never darkened to match current mode. Set this if the 
image looks better in the dark mode without actually converting 
it.]
:: [s5; Fixed size]
:: [s5; Image is never scaled to match current mode.]
:: [s5; UHD variant]
:: [s5; Image is variant for UHD mode.]
:: [s5; Dark variant]
:: [s5; Image is variant for Dark theme.]
:: [s5; Supersampled 3x]
:: [s5; Image is downsampled 3x before use `- this simplifies design 
of antialiased images as images are drawn and stored as unaliased.]
:: [s5; Export...]
:: [s5; [/ This is unrelated to UHD / Dark theme mode, but if this is 
checked, the Image is exported as .ico and .png files. This is 
intended for application icon (e.g. shown in host shell).]]}}&]
[s0; &]
[s5; Finally, it is also possible to disable automatic conversion 
for the whole .iml file by #defining FIXED`_COLORS and/or FIXED`_SIZE 
macros:&]
[s0; &]
[s7; #define IMAGECLASS MyImg&]
[s7; #define IMAGEFILE <MyApp/MyImg.iml>&]
[s7; #define [* FIXED`_COLORS]&]
[s7; #define [* FIXED`_SIZE]&]
[s7; &]
[s7; #include <Draw/iml`_source.h>&]
[s0; &]
[s3;:4: Reacting to theme changes&]
[s5; Before 2025.1 U`+`+ release, the application skin was loaded 
from host platform just once at the start of execution and any 
if user changed host platform theme, this was not reflected in 
the application. Also changing to user defined or some predefined 
skin required restart of application to work properly.&]
[s5; Since 2025.1, application can be made to react to host platform 
changes and can change skins without restart with a call to Ctrl`::SkinChangeSensi
tive() (e.g. in GUI`_APP`_MAIN). However, this poses development 
challenges with switching between light theme and dark theme.&]
[s5; While application can easily react in Paint to current mode, 
situation is much more complicated when colors or images are 
used as attributes of widgets or widget contents. For example 
ArrayCtrl`::EvenRowColor can be called by developer to define 
some color that is appropriate for light theme, theme but when 
the theme is switched can be no longer viable.&]
[s5; U`+`+ provides several tools to handle this problem:&]
[s5;i150;O0; All .iml images are now considered `"special logical 
constants`" whose appearance changes according to current theme 
(dark / light) `- this applies to all copies as well.&]
[s5;i150;O0; [^topic`:`/`/Draw`/src`/Colors`_en`-us^ Predefined colors] 
constants are also `"special logical constants`" that are interpreted 
differently after theme switch.&]
[s5;i150;O0; Special logical type of Color `- SColor `- that is defined 
by function that is reevaluated after theme switch, e.g.&]
[s7; static SColor light`_highlight(`[`] `{ return Blend(SColorHighlight(), 
SColorPaper()) `});&]
[s0; &]
[s5;i150;O0; Another special logical type of Color `- AColor `- which 
should be defined as light theme color value, but when in dark 
theme, adjusts its value to the dark theme (with DarkThemeCached 
function)&]
[s5;i150;O0; RichText / QTF can use DarkThemeCached to adjust colors, 
which is automatically done e.g. in Labels or RichTextCtrl when 
rendered on dark backgrounds&]
[s5;i150;O0; If everything else fails, Ctrl`::Skin virtual function 
is always called on widget opening and on theme changes giving 
developer chance to alter colors are required&]
[s5; To simplify testing, in debug mode Ctrl `+ Num`[`*`] toggles 
quickly between light and dark theme `- that way developer can 
check whether colors adjust correctly to the situation.&]
[s5; Demonstration of these features is in reference/ThemeChangeSensitive 
example.]]