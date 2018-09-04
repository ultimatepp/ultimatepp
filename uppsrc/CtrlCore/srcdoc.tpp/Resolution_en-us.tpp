topic "Reacting to resolution";
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
`- Standard, UHD, None. This info can be adjusted in Icon designed 
(right click Image name and choose Image..., or double`-click 
Image name)&]
[s0; &]
[s0;= 
@@image:565&775
(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)
&]
[s0; &]
[s0; U`+`+ can be switched to [^topic`:`/`/Draw`/src`/UHD`$en`-us`#Upp`:`:SetUHDMode`(bool`)^ `"
UHD`" mode]. In this mode, if icon is retrieved from .iml and 
it is in Standard resolution, it is upscaled 2x (relatively smart 
algorithm is used so that it looks quite fine on UHD display). 
Correspondingly, in standard mode, UHD resolution icons are downscaled. 
Icons with `"None`" resolution are left intact.&]
[s0; &]
[s0; It is also possible and preferable to have both standard and 
UHD variants in the same .iml file. If this the case, UHD variant 
[/ name] is changed to [/ name]`_`_UHD in C`+`+ code (to avoid name 
clash). Also, in UHD mode, UHD variant is used to actually overwrite 
standard resolution variant.&]
[s0; &]
[s0; UHD is normally activated when standard icon height is greater 
than 22.&]
[s0; &]
[s0; In addition to Zx/Zy/Zsz functions, U`+`+ also provides set 
of [^topic`:`/`/Draw`/src`/UHD`$en`-us^ DPI functions] which simply 
double arguments when UHD mode is active `- this is sometimes 
better approach for scaling things that are related to images.&]
[s0; &]
[s0; Last but not least, some host platforms (Windows) need to be 
specifically informed that the application is able to handle 
UHD, otherwise they enter into mode when application is rendered 
in standard resolution and upscaled by windows (which looks quite 
terrible). Normally, U`+`+ is in this `"HiDPI`" mode by default. 
You can revert to ugly upscaling mode by calling Ctrl`::SetUHDEnabled(false) 
in INITIBLOCK.]]