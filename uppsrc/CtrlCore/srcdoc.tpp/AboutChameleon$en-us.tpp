topic "Cameleon Rendering & Creating own chameleonized Ctrl's";
[ $$0,0#00000000000000000000000000000000:Default]
[{_}%EN-US 
[s0; [*R6 Cameleon Rendering `& Creating own chameleonized Ctrl`'s]&]
[s0; &]
[s0; [2 Ultimate`+`+ uses a key feature to yield almost identical look 
on target platforms. It is called `'Chameleon`' and was meant 
to be only a slim layer to resemble target platform look, but 
passing time has evolved to a rich featured part of Ultimate`+`+. 
Own custom Controls can be created quite fast, but often lack 
the proper support for Chameleon technology, rendering an ugly 
spot in your overall application, when it uses the other Ultimate`+`+ 
standard controls, which are `'Chameleon aware`'. So here is 
how you can extend / properly create a Ctrl derived Ultimate`+`+ 
control with chameleon support.]&]
[s0;2 &]
[s0; [*2 1) Define what you consider Style for your control]&]
[s0;2 &]
[s0; [2 In .h file]&]
[s0;C1 &]
[s0; [C1 struct Style : ChStyle<Style> `{]&]
[s0; [C1 -|Color paper;]&]
[s0; [C1 -|Color disabled;]&]
[s0; [C1 -|Color focus;]&]
[s0; [C1 -|Color invalid;]&]
[s0; [C1 -|Color text, textdisabled;]&]
[s0; [C1 -|Color selected, selectedtext;]&]
[s0; [C1 -|Value edge`[4`];]&]
[s0; [C1 -|bool  activeedge;]&]
[s0; [C1 -|int   vfm;]&]
[s0; [C1 `};]&]
[s0;2 &]
[s0; [2 This is maybe the most important task to do. When you go ahead 
implementing your Ctrl, you should already use the respective 
properties, where possible, thats why this is important to do 
as soon as possible in development. Later porting can be pain. 
Parts considered to be Style of your Control are Colors of parts, 
think of states, your Ctrl can be in (active, inactive..), general 
sizes of  bars, margins, etc. Font should `*not`* be part of 
it. It should be specified via API (thus later including of your 
Ctrl in TheIDE can be done easier, provide SetFont() for it)]&]
[s0;2 &]
[s0; [2 The ChStyle base enriches your own style with some handling 
with respect to static Style instances for all Ctrl instances 
of that type. so do not forget it.]&]
[s0;2 &]
[s0; [2 Stick to the naming convention, Style...]&]
[s0;2 &]
[s0; [*2 2) Setup a default style]&]
[s0;2 &]
[s0; [2 It is registered globally in Chameleon database for this special 
control (thats why the macro needs Ctrl class name, Style class 
name. It generates the function with name `'StyleDefault`'`'). 
Use default System color descriptions, where possible, like SColorFace, 
SColorPaper, to remain consistent to global GUI design. They 
are derived from current target Platform settings.]&]
[s0;2 &]
[s0; [2 In .cpp file]&]
[s0; [C1 CH`_STYLE(EditField, Style, StyleDefault)]&]
[s0; [C1 `{]&]
[s0; [C1 -|paper `= SColorPaper();]&]
[s0; [C1 -|disabled `= SColorFace();]&]
[s0; [C1 -|focus `= paper;]&]
[s0; [C1 -|invalid `= Blend(paper, Color(255, 0, 0), 32);]&]
[s0; [C1 -|text `= SColorText();]&]
[s0; [C1 -|textdisabled `= SColorDisabled();]&]
[s0; [C1 -|selected `= SColorHighlight();]&]
[s0; [C1 -|selectedtext `= SColorHighlightText();]&]
[s0; [C1 -|for(int i `= 0; i < 4; i`+`+)]&]
[s0; [C1 -|-|edge`[i`] `= CtrlsImg`::EFE();]&]
[s0; [C1 -|activeedge `= false;]&]
[s0; [C1 -|vfm `= 2;]&]
[s0; [C1 `}]&]
[s0;2 &]
[s0; [*2 3) Use a const Style `*style to reference]&]
[s0;2 &]
[s0; [2  It is the currently used style in your control code. Don`'t 
forget to initialize the pointer to your default style. Now, 
that you have a StyleDefault() function, you can use this one. 
provide means to override the Style  settings used in your Ctrl 
(SetStyle). Of course, the referenced Style needs to exist as 
long as the Ctrl uses it.]&]
[s0;2 &]
[s0; In .h file&]
[s0; [C1 -|const Style `*style;]&]
[s0; [C1 -|static const Style`& StyleDefault();]&]
[s0; [C1 -|EditField`&  SetStyle(const Style`& s);]&]
[s0; &]
[s0; In .cpp file&]
[s0; [C1 -|style `= `&StyleDefault(); //ctor]&]
[s0; &]
[s0; [C1 EditField`& EditField`::SetStyle(const Style`& s)]&]
[s0; [C1 `{]&]
[s0; [C1 -|style `= `&s;]&]
[s0; [C1 -|RefreshLayout();]&]
[s0; [C1 -|RefreshFrame();]&]
[s0; [C1 -|return `*this;]&]
[s0; [C1 `}]&]
[s0;2 &]
[s0; [*2 4) Use your style information to paint your control]&]
[s0;2 &]
[s0; [2 Either use it directly, or provide Chameleon helper functions 
with some of your style info]&]
[s0;2 &]
[s0; [C1 void EditField`::Paint(Draw`& w)]&]
[s0; [C1 `{]&]
[s0; [C1 -|Size sz `= GetSize();]&]
[s0;C1 &]
[s0; [C1 -|bool enabled `= IsShowEnabled();]&]
[s0; [C1 -|Color paper `= enabled `&`& !IsReadOnly() ? (HasFocus() ? 
style`->focus : style`->paper) : style`->disabled;]&]
[s0; [C1 -|if(nobg)]&]
[s0; [C1 -|-|paper `= Null;]&]
[s0; [C1 -|Color ink `= enabled ? style`->text : style`->textdisabled;]&]
[s0; [C1 ....]&]
[s0; [C1 `}]&]
[s0; &]
[s0; [2 other controls use it like that (ScrollBar)]&]
[s0; &]
[s0; [C1 -|-|if(i !`= 2 `|`| thumbsize >`= style`->thumbmin)]&]
[s0; [C1 -|-|-|ChPaint(w, pr, l`[i`]`[p `=`= i ? CTRL`_PRESSED : light 
`=`= i ? CTRL`_HOT : CTRL`_NORMAL`]);]&]
[s0; [C1 -|-|if(i !`= 2)]&]
[s0; [C1 -|-|-|w.End();]&]
[s0; [C1 -|`}]&]
[s0; [C1 `}]&]
[s0; [C1 else]&]
[s0; [C1 -|if(style`->through) `{]&]
[s0; [C1 -|-|ChPaint(w, sz, l`[0`]`[CTRL`_DISABLED`]);]&]
[s0; [C1 -|`}]&]
[s0; [C1 -|else]&]
[s0; [C1 -|if(IsHorz()) `{]&]
[s0; [C1 -|-|-|ChPaint(w, style`->arrowsize, 0, sz.cx / 2, sz.cy, l`[0`]`[CTRL`_DISABLED`]
);]&]
[s0; [C1 -|-|-|ChPaint(w, style`->arrowsize `+ sz.cx / 2, 0, sz.cx `- 
sz.cx / 2, sz.cy, l`[1`]`[CTRL`_DISABLED`]);]&]
[s0; [C1 -|-|`}]&]
[s0; [C1 -|-|else `{]&]
[s0; [C1 -|-|-|ChPaint(w, 0, style`->arrowsize, sz.cx, sz.cy / 2, l`[0`]`[CTRL`_DISABLED`]
);]&]
[s0; [C1 -|-|-|ChPaint(w, 0, style`->arrowsize `+ sz.cy / 2, sz.cx, sz.cy 
`- sz.cy / 2, l`[1`]`[CTRL`_DISABLED`]);]&]
[s0; [C1 ...-|-|`}]&]
[s0;2 &]
[s0; [*2 Usage Hints:]&]
[s0;2 &]
[s0; [2 Normally, the used Style struct is not alterable (thats why 
`'const Style `*`'), you can only replace ist as an entity at 
once (SetStyle) by a reference to another style. The referenced 
struct needs to exist as long as the control that`'s using it, 
exists as well (logical since it`'s no copy).]&]
[s0;2 &]
[s0; [2 if you want to permanently alter the default Style for `*all`* 
controls of that type, you can disable the const lock, to edit 
the static global instance of your custom control`'s default 
style]&]
[s0; [2 (or even others if your control supports multiple global styles, 
see below). You can always make a preinitialisation to a Standard() 
style, which was defined one time as copy from the first global 
registered style (i.e. StyleDefault()) for your control. The 
Standard() preinit saves a lot of code when to alter only few 
properties. this is also how to restore an altered StyleDefault 
to its previous state.]&]
[s0;2 &]
[s0; [C1 -|EditField`::Style`& es `= EditField`::StyleDefault().Write();]&]
[s0;C1 &]
[s0; [C1 -|es `= es.Standard();]&]
[s0; [C1 -|es.paper `= SColorPaper();]&]
[s0; [C1 -|es.disabled `= SColorFace();]&]
[s0; [C1 -|es.focus `= Blend(Green(), Black(), 192);]&]
[s0;2 &]
[s0; [2 your main application window should update all instantiated 
controls after finishing updating all desired styles]&]
[s0; [C1  -|RefreshLayoutDeep();]&]
[s0;2 &]
[s0; [2 Some Ctrl`'s may have more than one global style (i.e. Button). 
if your Ctrl`'s also needs more, just use another CH`_STYLE macro 
to provide it. Do not forget to define it in .h just in the same 
way as StyleDefault.]&]
[s0; [2 Thus, your Ctrl can be constructed parametrized or use another 
default Style (Button`::StyleNormal, Button`::StyleOk..)]&]
[s0;2 &]
[s0; [2 As of chameleonized helper functions, ][C2 ChPaint][2  is the only 
one you will likely need. The others are for internal use in 
the OS theming engine. The beauty and cornerstone of ChPaint 
and Chameleon is that ChPaint expects Value and can be extended 
w.r.t. Value types it is able to render. There are two basic 
types supported directly in Draw: Color and Image. Image has `"hotspots`" 
logic to define intelligent scaling, and color is self explaining. 
You can extend recognized types using ChLookFn. This way, e.g. 
XP chameleon registers its internal Value type `"XpElement`" 
and is able to use XP style rendering system to render Values 
from Styles. (thanks Mirek)]&]
[s0;2 &]
[s0; [2 Use SColorFace for drawing background color, SColorText for 
text, SColorPaper for controls that really draw some data (graphs, 
text background in Editfields..) Here is a list of common SColor...derivates:]&]
[s0; [2 (i.e. the values used are for a dark look, BLENDIT `= 180, 
should be setup at the beginning in aplication, but it is not 
used in all Ctrl`'s in upp.)]&]
[s0;2 &]
[s0; [C2 -|SColorPaper`_Write(Blend(White(), Black(), BLENDIT`-20));]&]
[s0; [C2 -|SColorText`_Write(LtGray());]&]
[s0; [C2 -|SColorHighlight`_Write(Gray());]&]
[s0; [C2 -|SColorHighlightText`_Write(White());]&]
[s0; [C2 -|SColorMenu`_Write(SColorPaper());]&]
[s0; [C2 -|SColorMenuText`_Write(SColorText());]&]
[s0; [C2 -|SColorInfo`_Write(SColorPaper());]&]
[s0; [C2 -|SColorInfoText`_Write(SColorText());]&]
[s0; [C2 -|SColorMark`_Write(SColorText());]&]
[s0; [C2 -|SColorDisabled`_Write(Blend(White(), Black(), BLENDIT/2));]&]
[s0; [C2 -|SColorLight`_Write(SColorText());]&]
[s0; [C2 -|SColorFace`_Write(Blend(White(), Black(), BLENDIT));]&]
[s0; [C2 -|SColorLabel`_Write(SColorText());]&]
[s0; [C2 -|SColorShadow`_Write(Black());]&]
[s0;C2 &]
[s0; [C2 -|SColorLtFace`_Write(LtGray());]&]
[s0; [C2 -|SColorDkShadow`_Write(Gray());]&]
[s0;C2 &]
[s0; [C2 -|LabelBoxTextColor`_Write(SColorText());]&]
[s0; [C2 -|LabelBoxColor`_Write(SColorHighlight());]&]
[s0;2 &]
[s0; [2 Play around with the Colors to see where they show up or browse 
the uppsrc :)]&]
[s0;2 &]
[s0; [2 Chameleon is not that complicated, the problem is that the 
underlying code ][/2 is][2  complicated so it`'s difficult to work 
out documentation.]&]
[s0;2 &]
[s0; ]