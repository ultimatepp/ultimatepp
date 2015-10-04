topic "Reacting to resolution";
[2 $$0,0#00000000000000000000000000000000:Default]
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
[{_}%EN-US 
[s2; Reacting to actual resolution, UHD support&]
[s0; While all widgets coordinates are always in pixels, applications 
need to react actual settings of target GUI to scale things accordingly.&]
[s0; &]
[s0; In U`+`+, the basic scaling factor is always determined by standard 
font size, which is read from host platform (scaling factor can 
also be adjusted by application, but that is less relevant here).&]
[s0; &]
[s0; All layouts are designed in some artificial defined, relatively 
small, `'basic font`' font size, then scaled based on ratios 
of standard system font to this basic font. Also, it is possible 
to scale individual dimensions by Ctrl`::`[Horz`|Vert`]LayoutZoom 
or [^topic`:`/`/CtrlCore`/src`/Zooming`$en`-us^ Zx/Zy/Zsz] functions.&]
[s0; &]
[s0; This, when used appropriately, works well for host GUI font 
size between 12`-22. When GUI font gets bigger, there starts 
to be a problem with Images (e.g. button icons) being too small. 
This is typical for UHD display.&]
[s0; &]
[s0; To resolve this problem, upscaled Images need to provided in 
this case. This is not a problem for Images that are created 
by code (because they can easily incorporate scaling factor into 
the algorithm), but would be a problem for images from .iml resources.&]
[s0; &]
[s0; To this means, Image has Resolution info. This can have 3 values 
`- None, Standard, UHD. This info can be adjusted in Icon designed.&]
[s0; &]
[s0; U`+`+ can be switched to [^topic`:`/`/Draw`/src`/UHD`$en`-us`#Upp`:`:SetUHDMode`(bool`)^ `"
UHD`" mode]. In this mode, if icon is retrieved from .iml and 
it is in Standard resolution, it is upscaled 2x (relatively smart 
algorithm is used so that it looks quite fine on UHD display). 
Correspondingly, in standard mode, UHD resolution icons are downscaled.&]
[s0; &]
[s0; UHD is normally activated when standard icon height is greater 
than 22.&]
[s0; &]
[s0; In addition to Zx/Zy/Zsz functions, U`+`+ also provides set 
of [^topic`:`/`/Draw`/src`/UHD`$en`-us^ DPI functions] which simply 
double arguments when UHD mode is active `- this is sometimes 
better approach for scaling things that are related to icons.&]
[s0; &]
[s0; Last but not least, some host platforms (Windows) need to be 
specifically informed that the application is able to handle 
UHD. You can do this using Ctrl`::SetUHDEnabled.]]