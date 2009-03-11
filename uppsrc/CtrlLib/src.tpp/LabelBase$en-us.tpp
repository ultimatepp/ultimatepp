topic "LabelBase - basic label routines";
[2 $$0,0#00000000000000000000000000000000:Default]
[i448;a25;kKO9;*@(64)2 $$1,0#37138531426314131252341829483380:class]
[l288;2 $$2,0#27521748481378242620020725143825:desc]
[a83;*R6 $$3,0#31310162474203024125188417583966:caption]
[l288;i1121;b17;O9;~~~.1408;2 $$4,0#10431211400427159095818037425705:param]
[i448;a25;kKO9;*@(64)2 $$5,0#37138531426314131252341829483370:item]
[*+117 $$6,6#14700283458701402223321329925657:header]
[{_}%EN-US 
[s3; Label routines&]
[s0; LabelBase.h contains functions and simple helper classes used 
to implement Ctrls with text, images and accelerator keys.&]
[s0; Generally, format of text used by LabelBase facilities is either 
simple multi`-line or [^topic`:`/`/RichText`/srcdoc`/QTF`$en`-us^ QTF]:&]
[s0;i150;O2; If first character of text is `'`\1`', text is QTF (starting 
with next character).&]
[s0;i150;O2; If first character is NOT `'`\1`', text is simple multi`-line 
text with `'`\n`' used as line separator.&]
[s0; In both cases, `'`&`' or `'`\b`' are used as access`-key flags. 
If `'`&`' should be the part of text, it must be escaped as `"`&`&`". 
This format is formally named `"SmartText`".&]
[s0; Note also that U`+`+ normally distributes access`-keys automatically. 
Providing access`-key is thus usually unnecessary.&]
[s0;3 &]
[s5;K%- [^`:`:String^ String]_[@0 DeAmp]([@(0.0.255) const]_[@(0.0.255) char]_`*[@3 s])&]
[s2; This routine escapes all occurrences of `'`&`' character in 
source string with `"`&`&`" string to avoid using it as access`-key 
flag.&]
[s4; [%-*C@3 s]-|Input string.&]
[s4; [*/ Return value]-|Escaped string.&]
[s0;3 &]
[s0;:`:`:GetSmartTextSize`(`:`:Draw`&`,const char`*`,`:`:Font:%- Size_[* GetSmartTextSi
ze](Draw`&_[*@3 w], const_char_`*[*@3 text], Font_[*@3 font]_`=_StdFont())&]
[s2; Returns the minimal size of SmartText. If text is multi`-paragraph 
QTF text, width of text is the width of widest paragraph without 
doing any line breaks (paragraphs are formatted for infinite 
width).&]
[s4; [%-*C@3 w]-|Draw.&]
[s4; [%-*C@3 text]-|SmartText string.&]
[s4; [%-*C@3 font]-|Font for non`-QTF text.&]
[s4; [*/ Return value]-|Size of SmartText.&]
[s0;3 &]
[s0;:`:`:GetSmartTextHeight`(`:`:Draw`&`,const char`*`,int`,`:`:Font:%- int_[* GetSmart
TextHeight](Draw`&_[*@3 w], const_char_`*[*@3 s], int_[*@3 cx], Font_[*@3 font]_`=_StdFon
t())&]
[s2; Returns the height of given SmartText for given width. QTF paragraphs 
are formatted for this width.&]
[s4; [%-*C@3 w]-|Draw.&]
[s4; [%-*C@3 s]-|SmartText string.&]
[s4; [%-*C@3 cx]-|Required width.&]
[s4; [%-*C@3 font]-|Font for non`-QTF text.&]
[s4; [*/ Return value]-|Height of SmartText.&]
[s0;3 &]
[s0;:`:`:DrawSmartText`(`:`:Draw`&`,int`,int`,int`,const char`*`,`:`:Font:%- void_[* Dr
awSmartText](Draw`&_[@3 w], int_[@3 x], int_[@3 y], int_[@3 cx], const_char_`*[@3 text], 
Font_[@3 font]_`=_StdFont(), Color_[@3 ink]_`=_SBlack, int_[@3 accesskey]_`=_[@3 0])&]
[s2; Paints SmartText to target Draw.&]
[s4; [%-*C@3 w]-|Draw.&]
[s4; [%-*C@3 x, y]-|Position.&]
[s4; [%-*C@3 cx]-|Required width.&]
[s4; [%-*C@3 text]-|SmartText.&]
[s4; [%-*C@3 font]-|Font for non`-QTF SmartText.&]
[s4; [%-*C@3 ink]-|Text color for non`-QTF SmartText.&]
[s4; [%-*C@3 accesskey]-|Access`-key `- first corresponding character 
will be painted with underline.&]
[s0;3 &]
[s5;K%- [^`:`:byte^ byte]_[@0 ExtractAccessKey]([@(0.0.255) const]_[@(0.0.255) char]_`*[@3 s], 
[^`:`:String^ String]`&_[@3 label])&]
[s2; Scans input string for access`-key.&]
[s4; [%-*C@3 s]-|Input string.&]
[s4; [%-*C@3 label]-|Output string `- with access`-key removed.&]
[s4; [*/ Return value]-|Access`-key.&]
[s0;3 &]
[s5;K%- [@(0.0.255) bool]_[@0 CompareAccessKey]([^`:`:byte^ byte]_[@3 accesskey], 
[^`:`:dword^ dword]_[@3 key])&]
[s2; Compares access key to U`+`+ event key value as received by 
Key virtual method.&]
[s4; [%-*C@3 accesskey]-|Access`-key.&]
[s4; [%-*C@3 key]-|U`+`+ event key value.&]
[s4; [*/ Return value]-|True if values match.&]
[s0;3 &]
[s5;K%- [^`:`:byte^ byte]_[@0 ChooseAccessKey]([@(0.0.255) const]_[@(0.0.255) char]_`*s, 
[^`:`:dword^ dword]_[@3 used])&]
[s2; Scans input string for suitable access`-key.&]
[s4; [%-*C@3 s]-|Input string.&]
[s4; [%-*C@3 used]-|Bit`-set of already used access`-keys. Bit`-mask 
of specific access`-key is defined by [^topic`:`/`/CtrlCore`/src`/Ctrl`$en`-us`#`:`:Ctrl`:`:AccessKeyBit`(byte`)^ C
trl`::AccessKeyBit] class method.&]
[s4; [*/ Return value]-|Access`-key or 0 if no suitable access`-key 
found.&]
[s0; &]
[s0; &]
[s0;3 &]
[s5;K%- [@(0.0.255) struct]_[@0 DrawLabel]&]
[s2; &]
[s2; This structure in fact just encapsulates generic label painting 
routine that otherwise would have way too much parameters.&]
[s0;3 &]
[s0;:`:`:DrawLabel`:`:DrawLabel`(`):%- [* DrawLabel]()&]
[s2; Default constructor. Default values are documented in attributes 
descriptions.&]
[s0;3 &]
[s5;K:`:`:DrawLabel`:`:GetSize`(int`)const:%- [^`:`:Size^ Size]_[@0 GetSize]([@(0.0.255) in
t]_[@3 txtcx]_`=_INT`_MAX)_[@(0.0.255) const]&]
[s2; Returns the size of label based on current set of attributes. 
[%-*@3 txtcx] specifies the maximum width of text.&]
[s0; &]
[s5;K%- [^`:`:Size^ Size]_[@0 Paint]([^`:`:Draw^ Draw]`&_[@3 w], [@(0.0.255) const]_[^`:`:Rect^ R
ect]`&_[@3 r], [@(0.0.255) bool]_visibleacckey_`=_true)_[@(0.0.255) const]&]
[s2; Paints the label in given rectangle.&]
[s4; [%-*C@3 w]-|Draw.&]
[s4; [%-*C@3 r]-|Rectangle.&]
[s4; [%-*C@3 visibleacckey]-|Access`-keys should be graphically highlighted 
(underlined).&]
[s4; [*/ Return value]-|Size of label.&]
[s0;3 &]
[s5;K%- [^`:`:Size^ Size]_[@0 Paint]([^`:`:Draw^ Draw]`&_[@3 w], [@(0.0.255) int]_[@3 x], 
[@(0.0.255) int]_[@3 y], [@(0.0.255) int]_[@3 cx], [@(0.0.255) int]_[@3 cy], 
[@(0.0.255) bool]_visibleacckey_`=_true)_[@(0.0.255) const]&]
[s2; Paints the label in given rectangle.&]
[s4; [%-*C@3 w]-|Draw.&]
[s4; [%-*C@3 x, y, cx, cy]-|Position and size of rectangle.&]
[s4; [%-*C@3 visibleacckey]-|Access`-keys should be graphically highlighted 
(underlined).&]
[s4; [*/ Return value]-|Size of label.&]
[s0;3 &]
[s5;K%- [@(0.0.255) bool]_[@0 push]&]
[s2; Label should be painted as `"pushed`" (means paint offset one 
pixel right and down). Default is false.&]
[s0;3 &]
[s5;K%- [@(0.0.255) bool]_[@0 focus]&]
[s2; Label should be painted as `"with input focus`" (light blue 
rectangle is drawn around label). Default is false.&]
[s0;3 &]
[s5;K%- [@(0.0.255) bool]_[@0 disabled]&]
[s2; Label should be painted as `"disable`" (affects the way how 
images and text are painted, disabled mean they are gray).&]
[s0;3 &]
[s5;K%- [^`:`:PaintRect^ PaintRect]_[@0 paintrect]&]
[s2; This paintrect is painted behind the label text. If label text 
is empty, size of text area is determined as paintrect.GetSize().&]
[s0;3 &]
[s5;K%- [^`:`:Image^ Image]_[@0 limg]&]
[s2; Left image.&]
[s0;3 &]
[s5;K%- [^`:`:Color^ Color]_[@0 lcolor]&]
[s2; Color of left monochromatic image. Null means that the image 
is color.&]
[s0;3 &]
[s5;K%- [@(0.0.255) int]_[@0 lspc]&]
[s2; Space between left image and text. If Null, image is placed 
at left edge of Label.&]
[s0;3 &]
[s5;K%- [^`:`:String^ String]_[@0 text]&]
[s2; Text of label.&]
[s0;3 &]
[s5;K%- [^`:`:Font^ Font]_[@0 font]&]
[s2; Font used to paint non`-QTF text.&]
[s0;3 &]
[s5;K%- [^`:`:Color^ Color]_[@0 ink]&]
[s2; Color used to paint non`-QTF text.&]
[s0;3 &]
[s5;K%- [^`:`:Image^ Image]_[@0 rimg]&]
[s2; Right image.&]
[s0;3 &]
[s5;K%- [^`:`:Color^ Color]_[@0 rcolor]&]
[s2; Color of monochromatic right image. Null means that the image 
is color.&]
[s0;3 &]
[s5;K%- [@(0.0.255) int]_[@0 rspc]&]
[s2; Space between the right image and text. If Null, image is placed 
at the right size of Label.&]
[s0;3 &]
[s5;K%- [@(0.0.255) int]_[@0 align]&]
[s2; Horizontal alignment. Can be ALIGN`_LEFT, ALIGN`_RIGHT or ALIGN`_CENTER.&]
[s0;3 &]
[s5;K%- [@(0.0.255) int]_[@0 valign]&]
[s2; Vertical alignment. Can be ALIGN`_TOP, ALIGN`_BOTTOM or ALIGN`_CENTER.&]
[s0;3 &]
[s5;K%- [@(0.0.255) int]_[@0 accesskey]&]
[s2; Access`-key.&]
[s0; &]
[s0; &]
[s0;3 &]
[s5;K%- [@(0.0.255) class]_[@0 LabelBase]&]
[s2; &]
[s2; This class encapsulates basic DrawLabel struct into form suitable 
to play a role of base class of GUI elements.&]
[s0;3 &]
[s5;K%- virtual virtual_[@(0.0.255) void]_[@0 LabelUpdate]()&]
[s2; This virtual method is called each time when any of attributes 
changes.&]
[s0;3 &]
[s5;K%- [^`:`:LabelBase^ LabelBase]`&_[@0 SetLeftImage]([@(0.0.255) const]_[^`:`:Image^ Image
]`&_bmp1, [@(0.0.255) int]_[@3 spc]_`=_[@3 0])&]
[s2; Sets the left image.&]
[s4; [%-*C@3 bmp1]-|Image.&]
[s4; [%-*C@3 spc]-|Space between left image and text. If Null, image 
is placed at left size of Label.&]
[s4; [*/ Return value]-|`*this for chaining.&]
[s0;3 &]
[s5;K%- [^`:`:LabelBase^ LabelBase]`&_[@0 SetPaintRect]([@(0.0.255) const]_[^`:`:PaintRect^ P
aintRect]`&_pr)&]
[s2; Sets PaintRect to be drawn behind or instead of label text.&]
[s4; [%-*C@3 pr]-|PaintRect&]
[s4; [*/ Return value]-|`*this for chaining.&]
[s0;3 &]
[s5;K%- [^`:`:LabelBase^ LabelBase]`&_[@0 SetText]([@(0.0.255) const]_[@(0.0.255) char]_`*[@3 t
ext])&]
[s2; Sets text of the label.&]
[s4; [%-*C@3 text]-|Text.&]
[s4; [*/ Return value]-|`*this for chaining.&]
[s0;3 &]
[s5;K%- [^`:`:LabelBase^ LabelBase]`&_[@0 SetFont]([^`:`:Font^ Font]_[@3 font])&]
[s2; Sets font of non`-QTF text.&]
[s4; [%-*C@3 font]-|Font.&]
[s4; [*/ Return value]-|`*this for chaining.&]
[s0;3 &]
[s5;K%- [^`:`:LabelBase^ LabelBase]`&_[@0 SetInk]([^`:`:Color^ Color]_color)&]
[s2; Sets the color of non`-QTF text .&]
[s4; [%-*C@3 color]-|Color.&]
[s4; [*/ Return value]-|`*this for chaining.&]
[s0;3 &]
[s5;K%- [^`:`:LabelBase^ LabelBase]`&_[@0 SetRightImage]([@(0.0.255) const]_[^`:`:Image^ Imag
e]`&_bmp2, [@(0.0.255) int]_[@3 spc]_`=_[@3 0])&]
[s2; Sets the right image.&]
[s4; [%-*C@3 bmp2]-|Image.&]
[s4; [%-*C@3 spc]-|Space between left image and text. If Null, image 
is placed at left size of Label.&]
[s4; [*/ Return value]-|`*this for chaining.&]
[s0;3 &]
[s5;K%- [^`:`:LabelBase^ LabelBase]`&_[@0 SetAlign]([@(0.0.255) int]_[@3 align])&]
[s2; Sets horizontal alignment.&]
[s4; [%-*C@3 align]-|One of ALIGN`_TOP, ALIGN`_BOTTOM or ALIGN`_CENTER.&]
[s4; [*/ Return value]-|`*this for chaining.&]
[s0;3 &]
[s5;K%- [^`:`:LabelBase^ LabelBase]`&_[@0 SetVAlign]([@(0.0.255) int]_[@3 align])&]
[s2; Sets vertical alignment.&]
[s4; [%-*C@3 align]-|One of ALIGN`_TOP, ALIGN`_BOTTOM or ALIGN`_CENTER.&]
[s4; [*/ Return value]-|`*this for chaining.&]
[s0;3 &]
[s5;K%- [^`:`:LabelBase^ LabelBase]`&_[@0 SetImage]([@(0.0.255) const]_[^`:`:Image^ Image]`&_
[@3 bmp], [@(0.0.255) int]_[@3 spc]_`=_[@3 0])&]
[s2; Same as SetLeftImage(bmp, spc).&]
[s0;3 &]
[s5;K%- [@(0.0.255) int]_[@0 GetAlign]()_[@(0.0.255) const]&]
[s4; [*/ Return value]-|Current horizontal alignment.&]
[s0;3 &]
[s5;K%- [@(0.0.255) int]_[@0 GetVAlign]()_[@(0.0.255) const]&]
[s4; [*/ Return value]-|Current vertical alignment.&]
[s0;3 &]
[s5;K%- [^`:`:PaintRect^ PaintRect]_[@0 GetPaintRect]()_[@(0.0.255) const]&]
[s4; [*/ Return value]-|Current PaintRect.&]
[s0;3 &]
[s5;K%- [^`:`:String^ String]_[@0 GetText]()_[@(0.0.255) const]&]
[s4; [*/ Return value]-|Current label text.&]
[s0;3 &]
[s5;K%- [^`:`:Font^ Font]_[@0 GetFont]()_[@(0.0.255) const]&]
[s4; [*/ Return value]-|Current font.&]
[s0;3 &]
[s5;K%- [^`:`:Color^ Color]_[@0 GetInk]()_[@(0.0.255) const]&]
[s4; [*/ Return value]-|Current text color.&]
[s0;3 &]
[s5;K%- [^`:`:Size^ Size]_[@0 PaintLabel]([^`:`:Draw^ Draw]`&_[@3 w], [@(0.0.255) const]_[^`:`:Rect^ R
ect]`&_[@3 r], [@(0.0.255) bool]_[@3 disabled]_`=_false, [@(0.0.255) bool]_[@3 push]_`=_fal
se, [@(0.0.255) bool]_[@3 focus]_`=_false, [@(0.0.255) bool]_[@3 vak]_`=_true)&]
[s2; Paints label in the given rectangle.&]
[s4; [%-*C@3 w]-|Draw.&]
[s4; [%-*C@3 r]-|Rectangle.&]
[s4; [%-*C@3 disabled]-|Disable flag.&]
[s4; [%-*C@3 push]-|Push flag.&]
[s4; [%-*C@3 focus]-|Focus flag.&]
[s4; [%-*C@3 vak]-|If true, accelerator should be highlighted.&]
[s4; [*/ Return value]-|Size of label.&]
[s0;3 &]
[s5;K%- [^`:`:Size^ Size]_[@0 PaintLabel]([^`:`:Draw^ Draw]`&_[@3 w], [@(0.0.255) int]_[@3 x], 
[@(0.0.255) int]_[@3 y], [@(0.0.255) int]_[@3 cx], [@(0.0.255) int]_[@3 cy], 
[@(0.0.255) bool]_[@3 disabled]_`=_false, [@(0.0.255) bool]_[@3 push]_`=_false, 
[@(0.0.255) bool]_[@3 focus]_`=_false, [@(0.0.255) bool]_[@3 vak]_`=_true)&]
[s2; Paints label in the given rectangle.&]
[s4; [%-*C@3 w]-|Draw.&]
[s4; [%-*C@3 x, y, cx, cy]-|Rectangle.&]
[s4; [%-*C@3 disabled]-|Disable flag.&]
[s4; [%-*C@3 push]-|Push flag.&]
[s4; [%-*C@3 focus]-|Focus flag.&]
[s4; [%-*C@3 vak]-|If true, accelerator should be highlighted.&]
[s4; [*/ Return value]-|Size of label.&]
[s0;3 &]
[s5;K%- [^`:`:Size^ Size]_[@0 GetLabelSize]()_[@(0.0.255) const]&]
[s4; [*/ Return value]-|Size of label.&]
[s0; ]