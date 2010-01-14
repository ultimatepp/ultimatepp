topic "Display";
[2 $$0,0#00000000000000000000000000000000:Default]
[i448;a25;kKO9;*@(64)2 $$1,0#37138531426314131252341829483380:class]
[l288;2 $$2,0#27521748481378242620020725143825:desc]
[a83;*R6 $$3,0#31310162474203024125188417583966:caption]
[l288;i1121;b17;O9;~~~.1408;2 $$4,0#10431211400427159095818037425705:param]
[i448;a25;kKO9;*@(64)2 $$5,0#37138531426314131252341829483370:item]
[*+117 $$6,6#14700283458701402223321329925657:header]
[0 $$7,0#96390100711032703541132217272105:end]
[H6;0 $$8,0#05600065144404261032431302351956:begin]
[{_}%EN-US 
[ {{10000@(113.42.0) [s0; [*@7;4 Display]]}}&]
[s8;H0;%- &]
[s1;:Display`:`:class:%- [@(0.0.255) class][@0 _Display]&]
[s2; Display and Display`-derived classes render Value into the specified 
rectangular area. References to Displays are used in many widgets 
as attributes affecting the rendering of widget Values. Default 
implementation uses StdDisplay to perform all actions (see below 
for StdDisplay description).&]
[s2; &]
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
[s7;%- &]
[s8;%- &]
[s5;:Display`:`:Paint`(Draw`&`,const Rect`&`,const Value`&`,Color`,Color`,dword`)const:%- [@(0.0.255) v
irtual][@0  ][@(0.0.255) void][@0 _Paint(][_^Draw^@0 Draw][@(0.0.255) `&][@0 _][@3 w][@0 , 
][@(0.0.255) const][@0 _][_^Rect^@0 Rect][@(0.0.255) `&][@0 _][@3 r][@0 , ][@(0.0.255) const][@0 _][_^Value^@0 V
alue][@(0.0.255) `&][@0 _][@3 q][@0 , ][_^Color^@0 Color][@0 _][@3 ink][@0 , 
][_^Color^@0 Color][@0 _][@3 paper][@0 , ][_^dword^@0 dword][@0 _][@3 style][@0 )_][@(0.0.255) cons
t]&]
[s2; This virtual method is used to paint rectangle content according 
to specified Value. Note that it is OK for derived class to understand 
just Value types it was designed for (and crash otherwise) `- 
it is client code responsibility to use the correct Display.&]
[s4; [%-*C@3 w]-|Draw.&]
[s4; [%-*C@3 r]-|Target rectangle.&]
[s4; [%-*C@3 q]-|Value to be painted.&]
[s4; [%-*C@3 ink]-|Suggested foreground color.&]
[s4; [%-*C@3 paper]-|Suggested background color.&]
[s4; [%-*C@3 style]-|Visual style.&]
[s7;%- &]
[s8;%- &]
[s5;:Display`:`:PaintBackground`(Draw`&`,const Rect`&`,const Value`&`,Color`,Color`,dword`)const:%- [@(0.0.255) v
irtual][@0  ][@(0.0.255) void][@0 _PaintBackground(][_^Draw^@0 Draw][@(0.0.255) `&][@0 _][@3 w][@0 ,
 ][@(0.0.255) const][@0 _][_^Rect^@0 Rect][@(0.0.255) `&][@0 _][@3 r][@0 , 
][@(0.0.255) const][@0 _][_^Value^@0 Value][@(0.0.255) `&][@0 _][@3 q][@0 , 
][_^Color^@0 Color][@0 _][@3 ink][@0 , ][_^Color^@0 Color][@0 _][@3 paper][@0 , 
][_^dword^@0 dword][@0 _][@3 style][@0 )_][@(0.0.255) const]&]
[s2; This virtual method is used to paint areas of GUI element that 
are outside of rectangle specified in Paint method, but should 
have color related somewhat to current Display class `- usually 
this color is the same as background painted in Paint. (Note 
that Paint method must clear the background separately, although 
base Display class is defined to use PaintBackground for this 
task).&]
[s4; [%-*C@3 w]-|Draw.&]
[s4; [%-*C@3 r]-|Target rectangle.&]
[s4; [%-*C@3 q]-|Value to be painted.&]
[s4; [%-*C@3 ink]-|Suggested foreground color.&]
[s4; [%-*C@3 paper]-|Suggested background color.&]
[s4; [%-*C@3 style]-|Visual style.&]
[s7;%- &]
[s8;%- &]
[s5;:Display`:`:GetStdSize`(const Value`&`)const:%- [@(0.0.255) virtual][@0  
][_^Size^@0 Size][@0 _GetStdSize(][@(0.0.255) const][@0 _][_^Value^@0 Value][@(0.0.255) `&][@0 _
][@3 q][@0 )_][@(0.0.255) const]&]
[s2; Should return standard size for given value and Display. E.g. 
if Display is rendering Images, it should return the Size of 
the Image in pixels. Base Display returns the size of textual 
representation of the Value.&]
[s4; [%-*C@3 q]-|Value.&]
[s4; [*/ Return value]-|Size of Value for Display.&]
[s7;%- &]
[s8;%- &]
[s5;:Display`:`:RatioSize`(const Value`&`,int`,int`)const:%- [@(0.0.255) virtual][@0  
][_^Size^@0 Size][@0 _RatioSize(][@(0.0.255) const][@0 _][_^Value^@0 Value][@(0.0.255) `&][@0 _
][@3 q][@0 , ][@(0.0.255) int][@0 _][@3 cx][@0 , ][@(0.0.255) int][@0 _][@3 cy][@0 )_][@(0.0.255) const
]&]
[s2; Returns size for defined value keeping the aspect ratio.&]
[s4; [%-*C@3 q]-|Value.&]
[s4; [%-*C@3 cx]-|Required width. If zero, it should be computed to 
keep aspect ratio with cy.&]
[s4; [%-*C@3 cy]-|Required height. If zero, it should be computed to 
keep aspect ratio with cx.&]
[s4; [*/ Return value]-|Size of Value for Display.&]
[s0; &]
[s0;%- &]
[ {{10000t/25b/25@3 [s0;%- [*@(229)4 AttrText]]}}&]
[s0; &]
[s8;H0;%- &]
[s1;:AttrText`:`:struct:%- [@(0.0.255) struct][@0 _AttrText]&]
[s2; Simple helper class convertible to the Value. StdDisplay, StdRightDisplay 
and StdCenterDisplay detect whether Value passed in is of AttrText 
type and handle it differently by adopting non`-null attributes 
for the text painted.&]
[s7;%- &]
[s8;%- &]
[s5;:AttrText`:`:text:%- [_^WString^@0 WString][@0 _text]&]
[s2; Text to be displayed.&]
[s7;%- &]
[s8;%- &]
[s5;:AttrText`:`:font:%- [_^Font^@0 Font][@0 _font]&]
[s2; Font of text.&]
[s7;%- &]
[s8;%- &]
[s5;:AttrText`:`:ink:%- [_^Color^@0 Color][@0 _ink]&]
[s2; Text color.&]
[s7;%- &]
[s8;%- &]
[s5;:AttrText`:`:paper:%- [_^Color^@0 Color][@0 _paper]&]
[s2; Background color&]
[s7;%- &]
[s8;%- &]
[s5;:AttrText`:`:align:%- [@(0.0.255) int][@0 _align]&]
[s2; Current alignment. Can be one of ALIGN`_LEFT, ALIGN`_RIGHT, 
ALIGN`_CENTER.&]
[s7;%- &]
[s8;%- &]
[s5;:AttrText`:`:img:%- [_^Image^@0 Image][@0 _img]&]
[s2; Icon aligned to the left side.&]
[s7;%- &]
[s8;%- &]
[s5;:AttrText`:`:imgspc:%- [@(0.0.255) int][@0 _imgspc]&]
[s2; Space between icon and text.&]
[s7;%- &]
[s8;%- &]
[s5;:AttrText`:`:Set`(const char`*`):%- [_^AttrText^@0 AttrText][@(0.0.255) `&][@0 _Set(][@(0.0.255) c
onst][@0 _][@(0.0.255) char][@0 _`*][@3 s][@0 )]&]
[s5;:AttrText`:`:Set`(const wchar`*`):%- [_^AttrText^@0 AttrText][@(0.0.255) `&][@0 _Set(][@(0.0.255) c
onst][@0 _][_^wchar^@0 wchar][@0 _`*][@3 s][@0 )]&]
[s5;:AttrText`:`:Set`(const WString`&`):%- [_^AttrText^@0 AttrText][@(0.0.255) `&][@0 _Set(
][@(0.0.255) const][@0 _][_^WString^@0 WString][@(0.0.255) `&][@0 _][@3 s][@0 )]&]
[s5;:AttrText`:`:operator`=`(const char`*`):%- [_^AttrText^@0 AttrText][@(0.0.255) `&][@0 _
operator`=(][@(0.0.255) const][@0 _][@(0.0.255) char][@0 _`*][@3 s][@0 )]&]
[s5;:AttrText`:`:operator`=`(const wchar`*`):%- [_^AttrText^@0 AttrText][@(0.0.255) `&][@0 _
operator`=(][@(0.0.255) const][@0 _][_^wchar^@0 wchar][@0 _`*][@3 s][@0 )]&]
[s5;:AttrText`:`:operator`=`(const WString`&`):%- [_^AttrText^@0 AttrText][@(0.0.255) `&][@0 _
operator`=(][@(0.0.255) const][@0 _][_^WString^@0 WString][@(0.0.255) `&][@0 _][@3 s][@0 )]&]
[s2; Sets the text to [%-*@3 s].&]
[s7; &]
[s8;%- &]
[s5;:AttrText`:`:Ink`(Color`):%- [_^AttrText^@0 AttrText][@(0.0.255) `&][@0 _Ink(][_^Color^@0 C
olor][@0 _][@3 c][@0 )]&]
[s2; Sets the text color.&]
[s4; [%-*C@3 c]-|The color.&]
[s4; [*/ Return value]-|`*this.&]
[s7;%- &]
[s8;%- &]
[s5;:AttrText`:`:Paper`(Color`):%- [_^AttrText^@0 AttrText][@(0.0.255) `&][@0 _Paper(][_^Color^@0 C
olor][@0 _][@3 c][@0 )]&]
[s2; Sets the paper color.&]
[s4; [%-*C@3 c]-|The color.&]
[s4; [*/ Return value]-|`*this.&]
[s7;%- &]
[s8;%- &]
[s5;:AttrText`:`:SetFont`(Font`):%- [_^AttrText^@0 AttrText][@(0.0.255) `&][@0 _SetFont(][_^Font^@0 F
ont][@0 _][@3 f][@0 )]&]
[s2; Sets the font.&]
[s4; [%-*C@3 f]-|The font.&]
[s4; [*/ Return value]-|`*this.&]
[s7;%- &]
[s8;%- &]
[s5;:AttrText`:`:Align`(int`):%- [_^AttrText^@0 AttrText][@(0.0.255) `&][@0 _Align(][@(0.0.255) i
nt][@0 _][@3 a][@0 )]&]
[s2; Sets the text horizontal alignment. Approved values are ALIGN`_LEFT, 
ALIGN`_CENTER and ALIGN`_RIGHT.&]
[s7;%- &]
[s8;%- &]
[s5;:AttrText`:`:Left`(`):%- [_^AttrText^@0 AttrText][@(0.0.255) `&][@0 _Left()]&]
[s2; Aligns the text left.&]
[s7;%- &]
[s8;%- &]
[s5;:AttrText`:`:Center`(`):%- [_^AttrText^@0 AttrText][@(0.0.255) `&][@0 _Center()]&]
[s2; Aligns the text to the center.&]
[s7;%- &]
[s8;%- &]
[s5;:AttrText`:`:Right`(`):%- [_^AttrText^@0 AttrText][@(0.0.255) `&][@0 _Right()]&]
[s2; Aligns the text right.&]
[s7;%- &]
[s8;%- &]
[s5;:AttrText`:`:SetImage`(const Image`&`,int`):%- [_^AttrText^@0 AttrText][@(0.0.255) `&
][@0 _SetImage(][@(0.0.255) const][@0 _][_^Image^@0 Image][@(0.0.255) `&][@0 _][@3 m][@0 , 
][@(0.0.255) int][@0 _][@3 spc][@0 _`=_][@3 4][@0 )]&]
[s2; Sets the icon and space between the icon and text.&]
[s7; &]
[s8;%- &]
[s5;:AttrText`:`:operator Value`(`)const:%- [@0 operator_Value()_][@(0.0.255) const]&]
[s4; [*/ Return value]-|AttrText as raw Value.&]
[s7;%- &]
[s8;%- &]
[s5;:AttrText`:`:AttrText`(const char`*`):%- [@0 AttrText(][@(0.0.255) const][@0 _][@(0.0.255) c
har][@0 _`*][@3 text][@0 )]&]
[s2; Constructs AttrText, assigning the text attribute and all other 
attributes to zero.&]
[s7;%- &]
[s8;%- &]
[s5;:AttrText`:`:AttrText`(const wchar`*`):%- [@0 AttrText(][@(0.0.255) const][@0 _][_^wchar^@0 w
char][@0 _`*][@3 text][@0 )]&]
[s2; Constructs AttrText, assigning the text attribute and all other 
attributes to zero.&]
[s7;%- &]
[s8;%- &]
[s5;:AttrText`:`:AttrText`(const WString`&`):%- [@0 AttrText(][@(0.0.255) const][@0 _][_^WString^@0 W
String][@(0.0.255) `&][@0 _][@3 text][@0 )]&]
[s2; Constructs AttrText, assigning the text attribute and all other 
attributes to zero.&]
[s7; &]
[s8;%- &]
[s5;:AttrText`:`:AttrText`(`):%- [@0 AttrText()]&]
[s2; Default constructor.&]
[s0; &]
[s0;%- &]
[ {{10000t/25b/25@3 [s0;%- [*@(229)4 Standard displays]]}}&]
[s0; &]
[s0; Standard Displays are implemented as `"functional globals`" 
`- functions returning constant reference to single global Display 
instance.&]
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
[s0; &]
[s0; &]
[s0; &]
[s0; &]
[s0; ]