topic "Display";
[2 $$0,0#00000000000000000000000000000000:Default]
[i448;a25;kKO9;2 $$1,0#37138531426314131252341829483380:class]
[l288;2 $$2,2#27521748481378242620020725143825:desc]
[0 $$3,0#96390100711032703541132217272105:end]
[H6;0 $$4,0#05600065144404261032431302351956:begin]
[i448;a25;kKO9;2 $$5,0#37138531426314131252341829483370:item]
[l288;a4;*@5;1 $$6,6#70004532496200323422659154056402:requirement]
[l288;i1121;b17;O9;~~~.1408;2 $$7,0#10431211400427159095818037425705:param]
[i448;b42;O9;2 $$8,8#61672508125594000341940100500538:tparam]
[b42;2 $$9,9#13035079074754324216151401829390:normal]
[{_}%EN-US 
[ {{10000@(113.42.0) [s0; [*@7;4 Display]]}}&]
[s3;%- &]
[s1;:Display`:`:class:%- [@(0.0.255) class]_Display&]
[s9; Display and Display`-derived classes render Value into the specified 
rectangular area. References to Displays are used in many widgets 
as attributes affecting the rendering of widget Values. Default 
implementation uses StdDisplay to perform all actions (see below 
for StdDisplay description).&]
[s0; &]
[s0; [* Visual style constants ]are used as `"style`" parameter bit 
flags of rendering methods and provide additional information 
about required visual appearance:&]
[s0;l288;:Display`:`:CURSOR:~~~.1408;%- [* CURSOR-|]Gui element is current 
(`"has cursor`").&]
[s0;l288;:Display`:`:FOCUS:~~~.1408; [%-* FOCUS][* -|]Gui element has 
focus.&]
[s0;l288;:Display`:`:SELECT:~~~.1408;%- [* SELECT]-|Gui element is selected.&]
[s0;l288;:Display`:`:READONLY:~~~.1408;%- [* READONLY]-|Gui element 
is read`-only.&]
[s3;%- &]
[s0;%- &]
[ {{10000F(128)G(128)@1 [s0; [* Public Member List]]}}&]
[s3;%- &]
[s5;:Display`:`:Paint`(Draw`&`,const Rect`&`,const Value`&`,Color`,Color`,dword`)const:%- [@(0.0.255) v
irtual] [@(0.0.255) void]_Paint([_^Draw^ Draw][@(0.0.255) `&]_[@3 w], 
[@(0.0.255) const]_[_^Rect^ Rect][@(0.0.255) `&]_[@3 r], [@(0.0.255) const]_[_^Value^ Value][@(0.0.255) `&
]_[@3 q], [_^Color^ Color]_[@3 ink], [_^Color^ Color]_[@3 paper], [_^dword^ dword]_[@3 style])_
[@(0.0.255) const]&]
[s2; This virtual method is used to paint rectangle content according 
to specified Value. Note that it is OK for derived class to understand 
just Value types it was designed for (and crash otherwise) `- 
it is client code responsibility to use the correct Display.&]
[s7; [%-*C@3 w]-|Draw.&]
[s7; [%-*C@3 r]-|Target rectangle.&]
[s7; [%-*C@3 q]-|Value to be painted.&]
[s7; [%-*C@3 ink]-|Suggested foreground color.&]
[s7; [%-*C@3 paper]-|Suggested background color.&]
[s7; [%-*C@3 style]-|Visual style.&]
[s3;%- &]
[s4;%- &]
[s5;:Display`:`:PaintBackground`(Draw`&`,const Rect`&`,const Value`&`,Color`,Color`,dword`)const:%- [@(0.0.255) v
irtual] [@(0.0.255) void]_PaintBackground([_^Draw^ Draw][@(0.0.255) `&]_[@3 w], 
[@(0.0.255) const]_[_^Rect^ Rect][@(0.0.255) `&]_[@3 r], [@(0.0.255) const]_[_^Value^ Value][@(0.0.255) `&
]_[@3 q], [_^Color^ Color]_[@3 ink], [_^Color^ Color]_[@3 paper], [_^dword^ dword]_[@3 style])_
[@(0.0.255) const]&]
[s2; This virtual method is used to paint areas of GUI element that 
are outside of rectangle specified in Paint method, but should 
have color related somewhat to current Display class `- usually 
this color is the same as background painted in Paint. (Note 
that Paint method must clear the background separately, although 
base Display class is defined to use PaintBackground for this 
task).&]
[s7; [%-*C@3 w]-|Draw.&]
[s7; [%-*C@3 r]-|Target rectangle.&]
[s7; [%-*C@3 q]-|Value to be painted.&]
[s7; [%-*C@3 ink]-|Suggested foreground color.&]
[s7; [%-*C@3 paper]-|Suggested background color.&]
[s7; [%-*C@3 style]-|Visual style.&]
[s3;%- &]
[s4;%- &]
[s5;:Display`:`:GetStdSize`(const Value`&`)const:%- [@(0.0.255) virtual] 
[_^Size^ Size]_GetStdSize([@(0.0.255) const]_[_^Value^ Value][@(0.0.255) `&]_[@3 q])_[@(0.0.255) c
onst]&]
[s2; Should return standard size for given value and Display. E.g. 
if Display is rendering Images, it should return the Size of 
the Image in pixels. Base Display returns the size of textual 
representation of the Value.&]
[s7; [%-*C@3 q]-|Value.&]
[s7; [*/ Return value]-|Size of Value for Display.&]
[s3;%- &]
[s4;%- &]
[s5;:Display`:`:RatioSize`(const Value`&`,int`,int`)const:%- [@(0.0.255) virtual] 
[_^Size^ Size]_RatioSize([@(0.0.255) const]_[_^Value^ Value][@(0.0.255) `&]_[@3 q], 
[@(0.0.255) int]_[@3 cx], [@(0.0.255) int]_[@3 cy])_[@(0.0.255) const]&]
[s2; Returns size for defined value keeping the aspect ratio.&]
[s7; [%-*C@3 q]-|Value.&]
[s7; [%-*C@3 cx]-|Required width. If zero, it should be computed to 
keep aspect ratio with cy.&]
[s7; [%-*C@3 cy]-|Required height. If zero, it should be computed to 
keep aspect ratio with cx.&]
[s7; [*/ Return value]-|Size of Value for Display.&]
[s3; &]
[s0; &]
[s0; &]
[s0;%- &]
[ {{10000t/25b/25@(113.42.0) [s0;%- [*@(229)4 AttrText]]}}&]
[s3; &]
[s1;:AttrText`:`:struct:%- [@(0.0.255) struct]_AttrText&]
[s9; Helper class convertible to the Value. StdDisplay, StdRightDisplay 
and StdCenterDisplay detect whether Value passed in is of AttrText 
type and handle it differently by adopting non`-null attributes 
for the text painted. AttrText is `'rich`' Value type, supporting 
comparison and serialization. It can be compred to other Value 
types. It is also possible to assign text that is different from 
AttrText value.&]
[s3;%- &]
[s0;%- &]
[ {{10000F(128)G(128)@1 [s0; [* Public Member List]]}}&]
[s3;%- &]
[s5;:AttrText`:`:text:%- [_^WString^ WString]_text&]
[s2; Text to be displayed.&]
[s3;%- &]
[s4;%- &]
[s5;:AttrText`:`:value:%- [_^Value^ Value]_[* value]&]
[s2; Represents Value of AttrText, usually for comparison/sorting.&]
[s3;%- &]
[s4;%- &]
[s5;:AttrText`:`:font:%- [_^Font^ Font]_[* font]&]
[s2; Font of text. It is default initialized by constructor to StdFont.&]
[s3;%- &]
[s4;%- &]
[s5;:AttrText`:`:ink:%- [_^Color^ Color]_[* ink]&]
[s2; Text color.&]
[s3;%- &]
[s4;%- &]
[s5;:AttrText`:`:normalink:%- [_^Color^ Color]_[* normalink]&]
[s2; Text color to be used if the item is not in selected nor focused 
nor read`-only state.&]
[s3;%- &]
[s4;%- &]
[s5;:AttrText`:`:paper:%- [_^Color^ Color]_[* paper]&]
[s2; Background color&]
[s3;%- &]
[s4;%- &]
[s5;:AttrText`:`:normalpaper:%- [_^Color^ Color]_[* normalpaper]&]
[s2; Background color to be used if the item is not in selected nor 
focused nor read`-only state.&]
[s3;%- &]
[s4;%- &]
[s5;:AttrText`:`:align:%- [@(0.0.255) int]_[* align]&]
[s2; Current alignment. Can be one of ALIGN`_LEFT, ALIGN`_RIGHT, 
ALIGN`_CENTER.&]
[s3;%- &]
[s4;%- &]
[s5;:AttrText`:`:img:%- [_^Image^ Image]_[* img]&]
[s2; Icon aligned to the left side.&]
[s3;%- &]
[s4;%- &]
[s5;:AttrText`:`:imgspc:%- [@(0.0.255) int]_[* imgspc]&]
[s2; Space between icon and text.&]
[s3;%- &]
[s4;%- &]
[s5;:AttrText`:`:Set`(const Value`&`):%- [_^AttrText^ AttrText][@(0.0.255) `&]_[* Set]([@(0.0.255) c
onst]_[_^Value^ Value][@(0.0.255) `&]_[*@3 v])&]
[s5;:AttrText`:`:operator`=`(const Value`&`):%- [_^AttrText^ AttrText][@(0.0.255) `&]_[* op
erator`=]([@(0.0.255) const]_[_^Value^ Value][@(0.0.255) `&]_[*@3 v])&]
[s2; Sets the value of AttrText and also text of it (using AsString).&]
[s3; &]
[s4;%- &]
[s5;:AttrText`:`:Text`(const String`&`):%- [_^AttrText^ AttrText][@(0.0.255) `&]_[* Text]([@(0.0.255) c
onst]_[_^String^ String][@(0.0.255) `&]_[*@3 txt])&]
[s5;:AttrText`:`:Text`(const WString`&`):%- [_^AttrText^ AttrText][@(0.0.255) `&]_[* Text](
[@(0.0.255) const]_[_^WString^ WString][@(0.0.255) `&]_[*@3 txt])&]
[s5;:AttrText`:`:Text`(const char`*`):%- [_^AttrText^ AttrText][@(0.0.255) `&]_[* Text]([@(0.0.255) c
onst]_[@(0.0.255) char]_`*[*@3 txt])&]
[s2; Sets the text independent of Value.&]
[s3; &]
[s4;%- &]
[s5;:AttrText`:`:Ink`(Color`):%- [_^AttrText^ AttrText][@(0.0.255) `&]_[* Ink]([_^Color^ Colo
r]_[@3 c])&]
[s2; Sets the text color.&]
[s7; [%-*C@3 c]-|The color.&]
[s7; [*/ Return value]-|`*this.&]
[s3;%- &]
[s4;%- &]
[s5;:AttrText`:`:NormalInk`(Color`):%- [_^AttrText^ AttrText][@(0.0.255) `&]_[* NormalInk](
[_^Color^ Color]_[*@3 c])&]
[s2; Sets the text color to be used if the item is not in selected 
nor focused nor read`-only state.&]
[s3; &]
[s4;%- &]
[s5;:AttrText`:`:Paper`(Color`):%- [_^AttrText^ AttrText][@(0.0.255) `&]_[* Paper]([_^Color^ C
olor]_[@3 c])&]
[s2; Sets the paper color.&]
[s7; [%-*C@3 c]-|The color.&]
[s7; [*/ Return value]-|`*this.&]
[s3;%- &]
[s4;%- &]
[s5;:AttrText`:`:NormalPaper`(Color`):%- [_^AttrText^ AttrText][@(0.0.255) `&]_[* NormalPap
er]([_^Color^ Color]_[*@3 c])&]
[s2; Sets the background color to be used if the item is not in selected 
nor focused nor read`-only state.&]
[s3; &]
[s4;%- &]
[s5;:AttrText`:`:SetFont`(Font`):%- [_^AttrText^ AttrText][@(0.0.255) `&]_[* SetFont]([_^Font^ F
ont]_[@3 f])&]
[s2; Sets the font.&]
[s7; [%-*C@3 f]-|The font.&]
[s7; [*/ Return value]-|`*this.&]
[s3;%- &]
[s4;%- &]
[s5;:AttrText`:`:Bold`(bool`):%- [_^AttrText^ AttrText][@(0.0.255) `&]_[* Bold]([@(0.0.255) b
ool]_[*@3 b]_`=_[@(0.0.255) true])&]
[s2; Calls font.Bold([%-*@3 b]). Returns `*this.&]
[s3; &]
[s4;%- &]
[s5;:AttrText`:`:Italic`(bool`):%- [_^AttrText^ AttrText][@(0.0.255) `&]_[* Italic]([@(0.0.255) b
ool]_[*@3 b]_`=_[@(0.0.255) true])&]
[s2; Calls font.Italic([%-*@3 b]). Returns `*this.&]
[s3; &]
[s4;%- &]
[s5;:AttrText`:`:Underline`(bool`):%- [_^AttrText^ AttrText][@(0.0.255) `&]_[* Underline]([@(0.0.255) b
ool]_[*@3 b]_`=_[@(0.0.255) true])&]
[s2; Calls font.Underline([%-*@3 b]). Returns `*this.&]
[s3; &]
[s4;%- &]
[s5;:AttrText`:`:Strikeout`(bool`):%- [_^AttrText^ AttrText][@(0.0.255) `&]_[* Strikeout]([@(0.0.255) b
ool]_[*@3 b]_`=_[@(0.0.255) true])&]
[s2; Calls font.Strikeout([%-*@3 b]). Returns `*this.&]
[s3; &]
[s4;%- &]
[s5;:AttrText`:`:Align`(int`):%- [_^AttrText^ AttrText][@(0.0.255) `&]_[* Align]([@(0.0.255) i
nt]_[@3 a])&]
[s2; Sets the text horizontal alignment. Approved values are ALIGN`_LEFT, 
ALIGN`_CENTER and ALIGN`_RIGHT.&]
[s3;%- &]
[s4;%- &]
[s5;:AttrText`:`:Left`(`):%- [_^AttrText^ AttrText][@(0.0.255) `&]_[* Left]()&]
[s2; Aligns the text left.&]
[s3;%- &]
[s4;%- &]
[s5;:AttrText`:`:Center`(`):%- [_^AttrText^ AttrText][@(0.0.255) `&]_[* Center]()&]
[s2; Aligns the text to the center.&]
[s3;%- &]
[s4;%- &]
[s5;:AttrText`:`:Right`(`):%- [_^AttrText^ AttrText][@(0.0.255) `&]_[* Right]()&]
[s2; Aligns the text right.&]
[s3;%- &]
[s4;%- &]
[s5;:AttrText`:`:SetImage`(const Image`&`,int`):%- [_^AttrText^ AttrText][@(0.0.255) `&]_
[* SetImage]([@(0.0.255) const]_[_^Image^ Image][@(0.0.255) `&]_[@3 m], 
[@(0.0.255) int]_[@3 spc]_`=_[@3 4])&]
[s2; Sets the icon and space between the icon and text.&]
[s3; &]
[s4;%- &]
[s5;:AttrText`:`:operator Value`(`)const:%- operator_[* Value]()_[@(0.0.255) const]&]
[s7; Converts AttrText to Value.&]
[s3;%- &]
[s4;%- &]
[s5;:AttrText`:`:AttrText`(const Value`&`):%- [* AttrText]([@(0.0.255) const]_[_^Value^ Val
ue][@(0.0.255) `&]_[*@3 v])&]
[s2; Converts Value to AttrText. If [%-*@3 v] is not AttrText, it gets 
Set as Value of default constructed AttrText.&]
[s3; &]
[s4;%- &]
[s5;:AttrText`:`:AttrText`(`):%- [* AttrText]()&]
[s2; Default constructor. Assigns Null to all members except font, 
which is initialized to StdFont.&]
[s3; &]
[s0; &]
[s0; &]
[s0;%- &]
[ {{10000t/25b/25@(85.42.0) [s0;%- [*@(229)4 Standard displays]]}}&]
[s9; Standard Displays are implemented as `"functional globals`" 
`- functions returning constant reference to single global Display 
instance.&]
[s3; &]
[s0; &]
[ {{3530:6470h1;@(204) [s0; Display name]
:: [s0; Description]
::^@2 [s0;:StdDisplay: [* StdDisplay]]
::= [s0; Standard Display. Displays Value as text, unless it is AttrText 
(see above).]
::^ [s0;:StdRightDisplay: [* StdRightDisplay]]
::= [s0; Standard Display. Displays Value as right`-aligned text, unless 
it is AttrText (see above).]
::^ [s0;:StdCenterDisplay: [* StdCenterDisplay]]
::= [s0; Standard Display. Displays Value as centered text, unless it 
is AttrText (see above).]
::^ [s0;:ColorDisplay: [* ColorDisplay]]
::= [s0; Displays Color (required) `- simply paints background using 
the Value passed in.]
::^ [s0;:SizeTextDisplay: [* SizeTextDisplay]]
::= [s0; Similar to StdDisplay, but stretches the text size to fill whole 
display area.]
::^ [s0;:ImageDisplay: [* ImageDisplay]]
::= [s0; Displays Image passed in as Value, aligns it to the top`-left 
corner.]
::^ [s0;:FittedImageDisplay: [* FittedImageDisplay]]
::= [s0; Displays Image scaled to fit the rectangle.]
::^ [s0;:CenteredImageDisplay: [* CenteredImageDisplay]]
::= [s0; Displays Image centered in the rectangle.]
::^ [s0;:CenteredHighlightImageDisplay: [* CenteredHighlightImageDisplay]]
::= [s0; Displays Image centered in the rectangle with 1 pixel wide white 
border.]
::^ [s0;:DrawingDisplay: [* DrawingDisplay]]
::= [s0; Displays Drawing scaled to fit the rectangle.]}}&]
[s3; &]
[s0; ]]