topic "LabelBase - basic label routines";
[i448;a25;kKO9;2 $$1,0#37138531426314131252341829483380:class]
[l288;2 $$2,0#27521748481378242620020725143825:desc]
[0 $$3,0#96390100711032703541132217272105:end]
[H6;0 $$4,0#05600065144404261032431302351956:begin]
[i448;a25;kKO9;2 $$5,0#37138531426314131252341829483370:item]
[l288;a4;*@5;1 $$6,6#70004532496200323422659154056402:requirement]
[l288;i1121;b17;O9;~~~.1408;2 $$7,0#10431211400427159095818037425705:param]
[i448;b42;O9;2 $$8,8#61672508125594000341940100500538:tparam]
[b42;2 $$9,9#13035079074754324216151401829390:normal]
[2 $$0,0#00000000000000000000000000000000:Default]
[{_}%EN-US 
[s0; [*+150 Label routines]&]
[s3; &]
[s0; LabelBase.h contains functions and simple helper classes used 
to implement Ctrls with text, images and accelerator keys.&]
[s0; Generally, format of text used by LabelBase facilities is either 
simple multi`-line or [^topic`:`/`/RichText`/srcdoc`/QTF`$en`-us^ QTF]:&]
[s0;i150;O2; If first character of text is `'`\1`', text is QTF (starting 
with next character).&]
[s0;i150;O2; If first character is NOT `'`\1`', text is simple multi`-line 
text with `'`\n`' used as line separator.&]
[s0; In both cases, `'`&`' or `'`\b`' are used as accelerator key 
flags. If `'`&`' should be the part of text, it must be escaped 
as `"`&`&`".&]
[s0; This format is formally named `"SmartText`".&]
[s0; Note also that U`+`+ normally distributes accelerator keys automatically. 
Providing accelerator keys is thus usually unnecessary.&]
[s3; &]
[s0; &]
[s5;:DeAmp`(const char`*`):%- [_^String^ String]_[* DeAmp]([@(0.0.255) const]_[@(0.0.255) cha
r]_`*[*@3 s])&]
[s2; This routine escapes all occurrences of `'`&`' character in 
source string with `"`&`&`" string to avoid using it as access`-key 
flag.&]
[s3; &]
[s4;%- &]
[s5;:GetSmartTextSize`(const char`*`,Font`,int`):%- [_^Size^ Size]_[* GetSmartTextSize]([@(0.0.255) c
onst]_[@(0.0.255) char]_`*[*@3 text], [_^Font^ Font]_[*@3 font]_`=_StdFont(), 
[@(0.0.255) int]_[*@3 cx]_`=_INT`_MAX)&]
[s2; Returns the minimal size of SmartText for given width [%-*@3 cx]. 
If text is multi`-paragraph QTF text, width of text is the width 
of widest paragraph.&]
[s3; &]
[s4;%- &]
[s5;:GetSmartTextHeight`(const char`*`,int`,Font`):%- [@(0.0.255) int]_[* GetSmartTextHei
ght]([@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 s], [@(0.0.255) int]_[*@3 cx], 
[_^Font^ Font]_[*@3 font]_`=_StdFont())&]
[s2; Returns the height of given SmartText for given width [%-*@3 cx]. 
QTF paragraphs are formatted for this width.&]
[s3; &]
[s4;%- &]
[s5;:Upp`:`:DrawSmartText`(Draw`&`,int`,int`,int`,const char`*`,Font`,Color`,int`,Color`,int`):%- [@(0.0.255) v
oid] [* DrawSmartText](Draw[@(0.0.255) `&] [*@3 w], [@(0.0.255) int] 
[*@3 x], [@(0.0.255) int] [*@3 y], [@(0.0.255) int] [*@3 cx], [@(0.0.255) const] 
[@(0.0.255) char] [@(0.0.255) `*][*@3 text], Font [*@3 font ][@(0.0.255) `=] 
StdFont(), Color [*@3 ink ][@(0.0.255) `=] SBlack(), [@(0.0.255) int] 
[*@3 accesskey] [@(0.0.255) `=] [@3 0], Color [*@3 qtf`_ink ][@(0.0.255) `=] 
Null, [@(0.0.255) int] [*@3 dark`_theme ][@(0.0.255) `=] Null)&]
[s5;:Upp`:`:DrawSmartText`(Draw`&`,int`,int`,int`,const char`*`,int`,Font`,Color`,int`,Color`,int`):%- [@(0.0.255) v
oid] [* DrawSmartText](Draw[@(0.0.255) `&] [*@3 w], [@(0.0.255) int] 
[*@3 x], [@(0.0.255) int] [*@3 y], [@(0.0.255) int] [*@3 cx], [@(0.0.255) const] 
[@(0.0.255) char] [@(0.0.255) `*][*@3 text], [@(0.0.255) int] [*@3 orientation], 
Font font [@(0.0.255) `=] [*@3 StdFont](), Color ink [@(0.0.255) `=] 
[*@3 SBlack](), [@(0.0.255) int] [*@3 accesskey] [@(0.0.255) `=] [@3 0], 
Color qtf`_ink [@(0.0.255) `=] [*@3 Null], [@(0.0.255) int] dark`_theme 
[@(0.0.255) `=] [*@3 Null])&]
[s2; Draws SmartText [%-*@3 text] at position [%-*@3 x] [%-*@3 y] with 
width [%-*@3 cx]. [%-*@3 ink] is text color for non`-QTF text. If 
[%-  ][%-*@3 qtf`_ink ][%- is not null, it overrides the text color 
of qtf. First character equal to ][%-*@3 accesskey][%- , regardless 
case, is underlined (this is to support key accelerators). If 
][%-*@3 dark`_theme][%-  is Null, qtf text converts colors to dark 
theme is dark theme is active. 1 forces the conversion to dark 
theme to happen always (even in light theme), 0 disables it. 
][%-*@3 orientation] parameter can rotate text clockwise (ORIENTATION`_CLOCKWISE) 
or anticlockwise (ORIENTATION`_ANTICLOCKWISE).&]
[s3; &]
[s4;%- &]
[s5;:ExtractAccessKey`(const char`*`,String`&`):%- [_^byte^ byte]_[* ExtractAccessKey]([@(0.0.255) c
onst]_[@(0.0.255) char]_`*[*@3 s], [_^String^ String][@(0.0.255) `&]_[*@3 label])&]
[s7; Scans input string for access`-key.&]
[s3; &]
[s4;%- &]
[s5;:CompareAccessKey`(byte`,dword`):%- [@(0.0.255) bool]_[* CompareAccessKey]([_^byte^ byt
e]_[*@3 accesskey], [_^dword^ dword]_[*@3 key])&]
[s2; Compares access key to U`+`+ event key value as received by 
Key virtual method.&]
[s3; &]
[s4;%- &]
[s5;:ChooseAccessKey`(const char`*`,dword`):%- [_^byte^ byte]_[* ChooseAccessKey]([@(0.0.255) c
onst]_[@(0.0.255) char]_`*[*@3 s], [_^dword^ dword]_[*@3 used])&]
[s7; Scans input string for suitable access`-key. [%-*C@3 used] is 
a bit`-set of already used access`-keys. Bit`-mask of specific 
access`-key is defined by [^topic`:`/`/CtrlCore`/src`/Ctrl`$en`-us`#`:`:Ctrl`:`:AccessKeyBit`(byte`)^ C
trl`::AccessKeyBit] class method. Returns access`-key or 0 if 
no suitable access`-key found.&]
[s3; &]
[s0;%- &]
[ {{10000@3 [s0; [*@(229)4 DrawLabel]]}}&]
[s3; &]
[s1;:DrawLabel`:`:struct:%- [@(0.0.255)3 struct][3 _][*3 DrawLabel]&]
[s9; This structure in fact just encapsulates generic label painting 
routine that otherwise would have way too much parameters.&]
[s3; &]
[s0;%- &]
[ {{10000F(128)G(128)@1 [s0; [* Constructor Detail]]}}&]
[s3;%- &]
[s5;:DrawLabel`:`:DrawLabel`(`):%- [* DrawLabel]()&]
[s2; Default constructor. Default values are documented in attributes 
descriptions.&]
[s3; &]
[s0;%- &]
[ {{10000F(128)G(128)@1 [s0; [* Public Member List]]}}&]
[s3;%- &]
[s5;:DrawLabel`:`:GetSize`(int`)const:%- [_^Size^ Size]_[* GetSize]([@(0.0.255) int]_[*@3 txt
cx]_`=_INT`_MAX)_[@(0.0.255) const]&]
[s2; Returns the size of label based on current set of attributes. 
[%-*@3 txtcx] specifies the maximum width of text.&]
[s3; &]
[s4;%- &]
[s5;:DrawLabel`:`:Paint`(Draw`&`,const Rect`&`,bool`)const:%- [_^Size^ Size]_[* Paint]([_^Draw^ D
raw][@(0.0.255) `&]_[*@3 w], [@(0.0.255) const]_[_^Rect^ Rect][@(0.0.255) `&]_[*@3 r], 
[@(0.0.255) bool]_[*@3 visibleaccesskey]_`=_[@(0.0.255) true])_[@(0.0.255) const]&]
[s2; Paints the label in given rectangle.&]
[s7; [%-*C@3 w]-|Draw.&]
[s7; [%-*C@3 r]-|Rectangle.&]
[s7; [%-*C@3 visibleacckey]-|Access`-keys should be graphically highlighted 
(underlined).&]
[s7; [*/ Return value]-|Size of label.&]
[s3; &]
[s4;%- &]
[s5;:DrawLabel`:`:Paint`(Draw`&`,int`,int`,int`,int`,bool`)const:%- [_^Size^ Size]_[* Pai
nt]([_^Draw^ Draw][@(0.0.255) `&]_[*@3 w], [@(0.0.255) int]_[*@3 x], [@(0.0.255) int]_[*@3 y], 
[@(0.0.255) int]_[*@3 cx], [@(0.0.255) int]_[*@3 cy], [@(0.0.255) bool]_[*@3 visibleaccesskey
]_`=_[@(0.0.255) true])_[@(0.0.255) const]&]
[s2; Paints the label in given rectangle.&]
[s7; [%-*C@3 w]-|Draw.&]
[s7; [%-*C@3 x, y, cx, cy]-|Position and size of rectangle.&]
[s7; [%-*C@3 visibleacckey]-|Access`-keys should be graphically highlighted 
(underlined).&]
[s7; [*/ Return value]-|Size of label.&]
[s3; &]
[s4;%- &]
[s5;:DrawLabel`:`:push:%- [@(0.0.255) bool]_[* push]&]
[s2; Label should be painted as `"pushed`" (means paint offset one 
pixel right and down). Default is false.&]
[s3; &]
[s4;%- &]
[s5;:DrawLabel`:`:focus:%- [@(0.0.255) bool]_[* focus]&]
[s2; Label should be painted as `"with input focus`" (light blue 
rectangle is drawn around label). Default is false.&]
[s3; &]
[s4;%- &]
[s5;:DrawLabel`:`:disabled:%- [@(0.0.255) bool]_[* disabled]&]
[s2; Label should be painted as `"disable`" (affects the way how 
images and text are painted, disabled mean they are gray).&]
[s3; &]
[s4;%- &]
[s5;:DrawLabel`:`:paintrect:%- [_^PaintRect^ PaintRect]_[* paintrect]&]
[s2; This paintrect is painted behind the label text. If label text 
is empty, size of text area is determined as paintrect.GetSize().&]
[s3; &]
[s4;%- &]
[s5;:DrawLabel`:`:limg:%- [_^Image^ Image]_[* limg]&]
[s2; Left image.&]
[s3; &]
[s4;%- &]
[s5;:DrawLabel`:`:lcolor:%- [_^Color^ Color]_[* lcolor]&]
[s2; Color of left monochromatic image. Null means that the image 
is color.&]
[s3; &]
[s4;%- &]
[s5;:DrawLabel`:`:lspc:%- [@(0.0.255) int]_[* lspc]&]
[s2; Space between left image and text. If Null, image is placed 
at left edge of Label.&]
[s3; &]
[s4;%- &]
[s5;:DrawLabel`:`:text:%- [_^String^ String]_[* text]&]
[s2; Text of label.&]
[s3; &]
[s4;%- &]
[s5;:DrawLabel`:`:font:%- [_^Font^ Font]_[* font]&]
[s2; Font used to paint non`-QTF text.&]
[s3; &]
[s4;%- &]
[s5;:DrawLabel`:`:ink:%- [_^Color^ Color]_[* ink]&]
[s2; Color used to paint non`-QTF text.&]
[s3; &]
[s4;%- &]
[s5;:DrawLabel`:`:rimg:%- [_^Image^ Image]_[* rimg]&]
[s2; Right image.&]
[s3; &]
[s4;%- &]
[s5;:DrawLabel`:`:rcolor:%- [_^Color^ Color]_[* rcolor]&]
[s2; Color of monochromatic right image. Null means that the image 
is color.&]
[s3; &]
[s4;%- &]
[s5;:DrawLabel`:`:rspc:%- [@(0.0.255) int]_[* rspc]&]
[s2; Space between the right image and text. If Null, image is placed 
at the right size of Label.&]
[s3; &]
[s4;%- &]
[s5;:DrawLabel`:`:align:%- [@(0.0.255) int]_[* align]&]
[s2; Horizontal alignment. Can be ALIGN`_LEFT, ALIGN`_RIGHT or ALIGN`_CENTER.&]
[s3; &]
[s4;%- &]
[s5;:DrawLabel`:`:valign:%- [@(0.0.255) int]_[* valign]&]
[s2; Vertical alignment. Can be ALIGN`_TOP, ALIGN`_BOTTOM or ALIGN`_CENTER.&]
[s3; &]
[s4;%- &]
[s5;:DrawLabel`:`:nowrap:%- [@(0.0.255) bool]_[* nowrap]&]
[s2; When true (default false), no text wrapping is performed.&]
[s3;%- &]
[s4;%- &]
[s5;:DrawLabel`:`:accesskey:%- [@(0.0.255) int]_[* accesskey]&]
[s2; Access`-key.&]
[s3; &]
[s0; &]
[s0;3 &]
[s0;%- &]
[ {{10000@3 [s0; [*@(229)4 LabelBase]]}}&]
[s3; &]
[s1;:LabelBase`:`:class:%- [@(0.0.255)3 class][3 _][*3 LabelBase]&]
[s9; This class encapsulates basic DrawLabel struct into form suitable 
to play a role of base class of GUI elements.&]
[s3; &]
[s0;%- &]
[ {{10000F(128)G(128)@1 [s0; [* Public Method List]]}}&]
[s3;%- &]
[s5;:LabelBase`:`:LabelUpdate`(`):%- [@(0.0.255) virtual] [@(0.0.255) void]_[* LabelUpdate](
)&]
[s2; This virtual method is called each time when any of attributes 
changes.&]
[s3; &]
[s4;%- &]
[s5;:LabelBase`:`:SetLeftImage`(const Image`&`,int`):%- [_^LabelBase^ LabelBase][@(0.0.255) `&
]_[* SetLeftImage]([@(0.0.255) const]_[_^Image^ Image][@(0.0.255) `&]_[*@3 bmp1], 
[@(0.0.255) int]_[*@3 spc]_`=_[@3 0])&]
[s2; Sets the left image.&]
[s7; [%-*C@3 bmp1]-|Image.&]
[s7; [%-*C@3 spc]-|Space between left image and text. If Null, image 
is placed at left size of Label.&]
[s7; [*/ Return value]-|`*this for chaining.&]
[s3; &]
[s4;%- &]
[s5;:LabelBase`:`:SetPaintRect`(const PaintRect`&`):%- [_^LabelBase^ LabelBase][@(0.0.255) `&
]_[* SetPaintRect]([@(0.0.255) const]_[_^PaintRect^ PaintRect][@(0.0.255) `&]_[*@3 pr])&]
[s2; Sets PaintRect to be drawn behind or instead of label text.&]
[s7; [%-*C@3 pr]-|PaintRect&]
[s7; [*/ Return value]-|`*this for chaining.&]
[s3; &]
[s4;%- &]
[s5;:LabelBase`:`:SetText`(const char`*`):%- [_^LabelBase^ LabelBase][@(0.0.255) `&]_[* Set
Text]([@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 text])&]
[s2; Sets text of the label.&]
[s7; [%-*C@3 text]-|Text.&]
[s7; [*/ Return value]-|`*this for chaining.&]
[s3; &]
[s4;%- &]
[s5;:LabelBase`:`:SetFont`(Font`):%- [_^LabelBase^ LabelBase][@(0.0.255) `&]_[* SetFont]([_^Font^ F
ont]_[*@3 font])&]
[s2; Sets font of non`-QTF text.&]
[s7; [%-*C@3 font]-|Font.&]
[s7; [*/ Return value]-|`*this for chaining.&]
[s3; &]
[s4;%- &]
[s5;:LabelBase`:`:SetInk`(Color`):%- [_^LabelBase^ LabelBase][@(0.0.255) `&]_[* SetInk]([_^Color^ C
olor]_[*@3 color])&]
[s2; Sets the color of non`-QTF text .&]
[s7; [%-*C@3 color]-|Color.&]
[s7; [*/ Return value]-|`*this for chaining.&]
[s3; &]
[s4;%- &]
[s5;:LabelBase`:`:SetRightImage`(const Image`&`,int`):%- [_^LabelBase^ LabelBase][@(0.0.255) `&
]_[* SetRightImage]([@(0.0.255) const]_[_^Image^ Image][@(0.0.255) `&]_[*@3 bmp2], 
[@(0.0.255) int]_[*@3 spc]_`=_[@3 0])&]
[s2; Sets the right image.&]
[s7; [%-*C@3 bmp2]-|Image.&]
[s7; [%-*C@3 spc]-|Space between left image and text. If Null, image 
is placed at left size of Label.&]
[s7; [*/ Return value]-|`*this for chaining.&]
[s3; &]
[s4;%- &]
[s5;:LabelBase`:`:SetAlign`(int`):%- [_^LabelBase^ LabelBase][@(0.0.255) `&]_[* SetAlign]([@(0.0.255) i
nt]_[*@3 align])&]
[s2; Sets horizontal alignment.&]
[s7; [%-*C@3 align]-|One of ALIGN`_LEFT, ALIGN`_RIGHT or ALIGN`_CENTER.&]
[s7; [*/ Return value]-|`*this for chaining.&]
[s3; &]
[s4;%- &]
[s5;:LabelBase`:`:AlignLeft`(`):%- [_^LabelBase^ LabelBase][@(0.0.255) `&]_[* AlignLeft]()&]
[s2; Same as SetAlign(ALIGN`_LEFT).&]
[s3;%- &]
[s4;%- &]
[s5;:LabelBase`:`:AlignCenter`(`):%- [_^LabelBase^ LabelBase][@(0.0.255) `&]_[* AlignCenter
]()&]
[s2; Same as SetAlign(ALIGN`_CENTER).&]
[s3;%- &]
[s4;%- &]
[s5;:LabelBase`:`:AlignRight`(`):%- [_^LabelBase^ LabelBase][@(0.0.255) `&]_[* AlignRight](
)&]
[s2; Same as SetAlign(ALIGN`_RIGHT).&]
[s3;%- &]
[s4;%- &]
[s5;:LabelBase`:`:SetVAlign`(int`):%- [_^LabelBase^ LabelBase][@(0.0.255) `&]_[* SetVAlign](
[@(0.0.255) int]_[*@3 align])&]
[s2; Sets vertical alignment.&]
[s7; [%-*C@3 align]-|One of ALIGN`_TOP, ALIGN`_BOTTOM or ALIGN`_CENTER.&]
[s7; [*/ Return value]-|`*this for chaining.&]
[s5; &]
[s4;%- &]
[s5;:LabelBase`:`:AlignTop`(`):%- [_^LabelBase^ LabelBase][@(0.0.255) `&]_[* AlignTop]()&]
[s2; Same as SetVAlign(ALIGN`_TOP).&]
[s3;%- &]
[s4;%- &]
[s5;:LabelBase`:`:AlignVCenter`(`):%- [_^LabelBase^ LabelBase][@(0.0.255) `&]_[* AlignVCent
er]()&]
[s2; Same as SetVAlign(ALIGN`_CENTER).&]
[s3;%- &]
[s4;%- &]
[s5;:LabelBase`:`:AlignBottom`(`):%- [_^LabelBase^ LabelBase][@(0.0.255) `&]_[* AlignBottom
]()&]
[s2; Same as SetVAlign(ALIGN`_BOTTOM).&]
[s3;%- &]
[s4;%- &]
[s5;:LabelBase`:`:NoWrap`(bool`):%- [_^LabelBase^ LabelBase][@(0.0.255) `&]_[* NoWrap]([@(0.0.255) b
ool]_[*@3 b]_`=_[@(0.0.255) true])&]
[s2; When active (default false), now text wrapping is performed.&]
[s3; &]
[s4;%- &]
[s5;:LabelBase`:`:SetImage`(const Image`&`,int`):%- [_^LabelBase^ LabelBase][@(0.0.255) `&
]_[* SetImage]([@(0.0.255) const]_[_^Image^ Image][@(0.0.255) `&]_[*@3 bmp], 
[@(0.0.255) int]_[*@3 spc]_`=_[@3 0])&]
[s2; Same as SetLeftImage(bmp, spc).&]
[s3; &]
[s4;%- &]
[s5;:LabelBase`:`:GetAlign`(`)const:%- [@(0.0.255) int]_[* GetAlign]()_[@(0.0.255) const]&]
[s7; [*/ Return value]-|Current horizontal alignment.&]
[s3; &]
[s4;%- &]
[s5;:LabelBase`:`:GetVAlign`(`)const:%- [@(0.0.255) int]_[* GetVAlign]()_[@(0.0.255) const]&]
[s7; [*/ Return value]-|Current vertical alignment.&]
[s3; &]
[s4;%- &]
[s5;:LabelBase`:`:GetPaintRect`(`)const:%- [_^PaintRect^ PaintRect]_[* GetPaintRect]()_[@(0.0.255) c
onst]&]
[s7; [*/ Return value]-|Current PaintRect.&]
[s3; &]
[s4;%- &]
[s5;:LabelBase`:`:GetText`(`)const:%- [_^String^ String]_[* GetText]()_[@(0.0.255) const]&]
[s7; [*/ Return value]-|Current label text.&]
[s3; &]
[s4;%- &]
[s5;:LabelBase`:`:GetFont`(`)const:%- [_^Font^ Font]_[* GetFont]()_[@(0.0.255) const]&]
[s7; [*/ Return value]-|Current font.&]
[s3; &]
[s4;%- &]
[s5;:LabelBase`:`:GetInk`(`)const:%- [_^Color^ Color]_[* GetInk]()_[@(0.0.255) const]&]
[s7; [*/ Return value]-|Current text color.&]
[s3; &]
[s4;%- &]
[s5;:LabelBase`:`:PaintLabel`(Draw`&`,const Rect`&`,bool`,bool`,bool`,bool`):%- [_^Size^ S
ize]_[* PaintLabel]([_^Draw^ Draw][@(0.0.255) `&]_[*@3 w], [@(0.0.255) const]_[_^Rect^ Rect][@(0.0.255) `&
]_[*@3 r], [@(0.0.255) bool]_[*@3 disabled]_`=_[@(0.0.255) false], [@(0.0.255) bool]_[*@3 pus
h]_`=_[@(0.0.255) false], [@(0.0.255) bool]_[*@3 focus]_`=_[@(0.0.255) false], 
[@(0.0.255) bool]_[*@3 vak]_`=_[@(0.0.255) true])&]
[s2; Paints label in the given rectangle.&]
[s7; [%-*C@3 w]-|Draw.&]
[s7; [%-*C@3 r]-|Rectangle.&]
[s7; [%-*C@3 disabled]-|Disable flag.&]
[s7; [%-*C@3 push]-|Push flag.&]
[s7; [%-*C@3 focus]-|Focus flag.&]
[s7; [%-*C@3 vak]-|If true, accelerator should be highlighted.&]
[s7; [*/ Return value]-|Size of label.&]
[s3; &]
[s4;%- &]
[s5;:LabelBase`:`:PaintLabel`(Draw`&`,int`,int`,int`,int`,bool`,bool`,bool`,bool`):%- [_^Size^ S
ize]_[* PaintLabel]([_^Draw^ Draw][@(0.0.255) `&]_[*@3 w], [@(0.0.255) int]_[*@3 x], 
[@(0.0.255) int]_[*@3 y], [@(0.0.255) int]_[*@3 cx], [@(0.0.255) int]_[*@3 cy], 
[@(0.0.255) bool]_[*@3 disabled]_`=_[@(0.0.255) false], [@(0.0.255) bool]_[*@3 push]_`=_[@(0.0.255) f
alse], [@(0.0.255) bool]_[*@3 focus]_`=_[@(0.0.255) false], [@(0.0.255) bool]_[*@3 vak]_`=_
[@(0.0.255) true])&]
[s2; Paints label in the given rectangle.&]
[s7; [%-*C@3 w]-|Draw.&]
[s7; [%-*C@3 x, y, cx, cy]-|Rectangle.&]
[s7; [%-*C@3 disabled]-|Disable flag.&]
[s7; [%-*C@3 push]-|Push flag.&]
[s7; [%-*C@3 focus]-|Focus flag.&]
[s7; [%-*C@3 vak]-|If true, accelerator should be highlighted.&]
[s7; [*/ Return value]-|Size of label.&]
[s3; &]
[s4;%- &]
[s5;:LabelBase`:`:GetLabelSize`(`)const:%- [_^Size^ Size]_[* GetLabelSize]()_[@(0.0.255) co
nst]&]
[s7; [*/ Return value]-|Size of label.&]
[s3; &]
[s0; ]]