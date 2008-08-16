topic "Color";[2 $$0,0#00000000000000000000000000000000:Default]
[i448;a25;kKO9;*@(64)2 $$1,0#37138531426314131252341829483380:class]
[l288;2 $$2,0#27521748481378242620020725143825:desc]
[a83;*R6 $$3,0#31310162474203024125188417583966:caption]
[l288;i1121;b17;O9;~~~.1408;2 $$4,0#10431211400427159095818037425705:param]
[i448;a25;kKO9;*@(64)2 $$5,0#37138531426314131252341829483370:item]
[*+117 $$6,6#14700283458701402223321329925657:header]
[l288;a17;*1 $$7,7#70004532496200323422659154056402:requirement]
[{_}%EN-US 
[s5;K [%00-00@(0.0.255) class][%00-00 _][%00-00@0 Color][%00-00 _:_][%00-00@(0.0.255) private][%00-00 _
][%00-00^`:`:AssignValueTypeNo^ AssignValueTypeNo][%00-00 <][%00-00@0 Color][%00-00 , 
][%00-00^COLOR`_V^ COLOR`_V][%00-00 , ][%00-00^`:`:Moveable^ Moveable][%00-00 <][%00-00@0 C
olor][%00-00 >_>_]&]
[s5;K [%00-00@(0.0.255) class][%00-00 _][%00-00@0 Color][%00-00 _:_][%00-00@(0.0.255) private][%00-00 _
][%00-00^`:`:AssignValueTypeNo^ AssignValueTypeNo][%00-00 <][%00-00@0 Color][%00-00 , 
][%00-00^COLOR`_V^ COLOR`_V][%00-00 , ][%00-00^`:`:Moveable^ Moveable][%00-00 <][%00-00@0 C
olor][%00-00 >_>_]&]
[s2; Type representing RGB color. All components are in the range 
0 through 255. Null value is defined for Color. Color is Value 
compatible as a Rich`-Value type (supports Value comparison, 
serialization etc.).&]
[s0;/ &]
[s5;K@(0.0.255) [%00-00^`:`:dword^@(64) dword][%00-00@(64) _][%00-00@0 GetRaw][%00-00@(64) ()
_][%00-00 const]&]
[s4; [*/ Return value]-|Platform specific dword representing the color.&]
[s0; &]
[s5;K@(0.0.255) [%00-00 int][%00-00@(64) _][%00-00@0 GetR][%00-00@(64) ()_][%00-00 const]&]
[s4; [*/ Return value]-|The red component.&]
[s0; &]
[s5;K@(0.0.255) [%00-00 int][%00-00@(64) _][%00-00@0 GetG][%00-00@(64) ()_][%00-00 const]&]
[s4; [*/ Return value]-|The green component.&]
[s0; &]
[s5;K@(0.0.255) [%00-00 int][%00-00@(64) _][%00-00@0 GetB][%00-00@(64) ()_][%00-00 const]&]
[s4; [*/ Return value]-|The blue component.&]
[s0; &]
[s5;K@(0.0.255) [%00-00 bool][%00-00@(64) _][%00-00@0 IsNullInstance][%00-00@(64) ()_][%00-00 c
onst]&]
[s4; [*/ Return value]-|True if Color is Null. Used to implement IsNull 
for Color.&]
[s0; &]
[s5;K@(0.0.255) [%00-00 unsigned][%00-00@(64) _][%00-00@0 GetHashValue][%00-00@(64) ()_][%00-00 c
onst]&]
[s4; [*/ Return value]-|Hash`-value for Color.&]
[s0; &]
[s5;K@(0.0.255) [%00-00 bool][%00-00@(64) _][%00-00@0 operator`=`=][%00-00@(64) (][%00-00^`:`:Color^@(64) C
olor][%00-00@(64) _][%00-00@3 c][%00-00@(64) )_][%00-00 const]&]
[s4; [%00-00*C@3 c]-|Color to compare with.&]
[s4; [*/ Return value]-|True if Color is equal to [@3 c].&]
[s0; &]
[s5;K@(0.0.255) [%00-00 bool][%00-00@(64) _][%00-00@0 operator!`=][%00-00@(64) (][%00-00^`:`:Color^@(64) C
olor][%00-00@(64) _][%00-00@3 c][%00-00@(64) )_][%00-00 const]&]
[s4; [%00-00*C@3 c]-|Color to compare with.&]
[s4; [*/ Return value]-|True if Color is equal to [@3 c].&]
[s0; &]
[s5;K [%00-00@(0.0.255) void][%00-00 _][%00-00@0 Serialize][%00-00 (][%00-00^`:`:Stream^ Stream
][%00-00 `&_][%00-00@3 s][%00-00 )]&]
[s2; Serialization.&]
[s4; [%00-00*C@3 s]-|Stream.&]
[s0; &]
[s0;:`:`:Color`:`:Color`(`): [%00-00* Color][%00-00 ()]&]
[s2; Default constructor. Leaves component values uninitialized.&]
[s0; &]
[s0;:`:`:Color`:`:Color`(int`,int`,int`): [%00-00* Color][%00-00 (int_][%00-00*@3 r][%00-00 ,
 int_][%00-00*@3 g][%00-00 , int_][%00-00*@3 b][%00-00 )]&]
[s2; Constructor.&]
[s4; [%00-00*C@3 r]-|Red channel.&]
[s4; [%00-00*C@3 g]-|Green channel.&]
[s4; [%00-00*C@3 b]-|Blue channel.&]
[s0; &]
[s0;:`:`:Color`:`:Color`(const`:`:Nuller`&`): [%00-00* Color][%00-00 (const_Nuller`&)]&]
[s2; Null constructor.&]
[s0; &]
[s5;K@(0.0.255) [%00-00@(64) operator_Value()_][%00-00 const]&]
[s2; Conversion to Value.&]
[s4; [*/ Return value]-|Value.&]
[s0; &]
[s0;:`:`:Color`:`:Color`(const`:`:Value`&`): [%00-00* Color][%00-00 (const_Value`&_][%00-00*@3 q
][%00-00 )]&]
[s2; Conversion from Value.&]
[s4; [%00-00*C@3 q]-|Value.&]
[s0; &]
[s0;:`:`:Color`:`:Color`(`:`:Color`(`*`)`(`)`): [%00-00* Color][%00-00 (][%00-00* Color][%00-00 _
(`*][%00-00*@3 fn][%00-00 )())]&]
[s2; This constructor allows using a pointer to a function returning 
Colors instead of Colors in many places. Global functions returning 
Color are needed instead of global Color constants on many platforms, 
because often global variables are not allowed when using dynamic 
libraries.&]
[s4; [%00-00*C@3 fn]-|Pointer to a function returning Color.&]
[s0; &]
[s5;K [%00-00 static ][%00-00^`:`:Color^ Color][%00-00 _][%00-00@0 FromRaw][%00-00 (][%00-00^`:`:dword^ d
word][%00-00 _][%00-00@3 co][%00-00 )]&]
[s2; Creates a Color based on a platform specific raw dword value.&]
[s4; [%00-00*C@3 co]-|Raw dword value.&]
[s4; [*/ Return value]-|Color.&]
[s0; &]
[s5;K@(0.0.255) [%00-00@(64) operator_COLORREF()_][%00-00 const]&]
[s0; [%00-00 Win32 specific.]&]
[s2; Return COLORREF for Color.&]
[s4; [*/ Return value]-|COLORREF.&]
[s0; &]
[s5;K [%00-00 static ][%00-00^`:`:Color^ Color][%00-00 _][%00-00@0 FromCR][%00-00 (][%00-00^COLORREF^ C
OLORREF][%00-00 _][%00-00@3 cr][%00-00 )]&]
[s0; Win32 specific.&]
[s2; Creates Color from COLORREF.&]
[s4; [%00-00*C@3 cr]-|COLORREF.&]
[s4; [*/ Return value]-|Color.&]
[s0; &]
[s5;K@(0.0.255) [%00-00@(64) operator_dword()_][%00-00 const]&]
[s4; [*/ Return value]-|GetRaw().&]
[s0;3 &]
[s0;3 &]
[s0;:`:`:GrayColor`:`:struct:* [%00-00* struct_][%00-00 GrayColor]&]
[s2; &]
[s2; This simple utility class provides constructor that sets all 
components to the same value producing the gray color.&]
[s2; &]
[s0; [/ Derived from] [%00-00 Color]&]
[s0; &]
[s0;:`:`:GrayColor`:`:GrayColor`(int`): [%00-00* GrayColor][%00-00 (int_][%00-00*@3 level][%00-00 )
]&]
[s2; Constructor.&]
[s4; [%00-00*C@3 level]-|Gray intensity.&]
[s0; &]
[s0;3 Utility functions&]
[s5;K [%00-00@(0.0.255) int][%00-00 _][%00-00@0 GetRValue][%00-00 (][%00-00^`:`:dword^ dword][%00-00 _
][%00-00@3 c][%00-00 )]&]
[s0; [%00-00 int_][%00-00* GetRValue][%00-00 (dword_][%00-00*@3 c][%00-00 )]&]
[s2; Returns red component from a platform specific value.&]
[s4; [%00-00*C@3 c]-|Platform specific value.&]
[s4; [*/ Return value]-|Red component.&]
[s0; &]
[s5;K [%00-00@(0.0.255) int][%00-00 _][%00-00@0 GetGValue][%00-00 (][%00-00^`:`:dword^ dword][%00-00 _
][%00-00@3 c][%00-00 )]&]
[s2; Returns green component from a platform specific value.&]
[s4; [%00-00*C@3 c]-|Platform specific value.&]
[s4; [*/ Return value]-|Green component.&]
[s0; &]
[s5;K [%00-00@(0.0.255) int][%00-00 _][%00-00@0 GetBValue][%00-00 (][%00-00^`:`:dword^ dword][%00-00 _
][%00-00@3 c][%00-00 )]&]
[s2; Returns blue component from a platform specific value.&]
[s4; [%00-00*C@3 c]-|Platform specific value.&]
[s4; [*/ Return value]-|Blue component.&]
[s0; &]
[s5;K [%00-00^`:`:dword^ dword][%00-00 _][%00-00@0 RGB][%00-00 (][%00-00^`:`:byte^ byte][%00-00 _
][%00-00@3 r][%00-00 , ][%00-00^`:`:byte^ byte][%00-00 _][%00-00@3 g][%00-00 , 
][%00-00^`:`:byte^ byte][%00-00 _][%00-00@3 b][%00-00 )]&]
[s2; Calculates platform specific value from components.&]
[s4; [%00-00*C@3 r]-|Red component.&]
[s4; [%00-00*C@3 g]-|Green component.&]
[s4; [%00-00*C@3 b]-|Blue component.&]
[s4; [*/ Return value]-|Platform specific value.&]
[s0; &]
[s5;K [%00-00@(0.0.255) unsigned][%00-00 _][%00-00@0 GetHashValue][%00-00 (][%00-00^`:`:Color^ C
olor][%00-00 _][%00-00@3 c][%00-00 )]&]
[s2; Returns a hash value for the color.&]
[s4; [%00-00*C@3 c]-|Color.&]
[s4; [*/ Return value]-|Hash value.&]
[s0; &]
[s5;K [%00-00^`:`:Color^ Color][%00-00 _][%00-00@0 Nvl][%00-00 (][%00-00^`:`:Color^ Color][%00-00 _
][%00-00@3 a][%00-00 , ][%00-00^`:`:Color^ Color][%00-00 _][%00-00@3 b][%00-00 )]&]
[s2; Returns first parameter if it is not Null, second otherwise.&]
[s4; [%00-00*C@3 a]-|Color.&]
[s4; [%00-00*C@3 b]-|Color.&]
[s4; [*/ Return value]-|a if not Null, b otherwise.&]
[s0; &]
[s5;K [%00-00 template_<>__][%00-00^`:`:String^ String][%00-00 _][%00-00@0 AsString][%00-00 (][%00-00@(0.0.255) c
onst][%00-00 _][%00-00^`:`:Color^ Color][%00-00 `&_][%00-00@3 c][%00-00 )]&]
[s5;K [%00-00 template_<>__][%00-00^`:`:String^ String][%00-00 _][%00-00@0 AsString][%00-00 (][%00-00@(0.0.255) c
onst][%00-00 _][%00-00^`:`:Color^ Color][%00-00 `&_][%00-00@3 c][%00-00 )]&]
[s2; Converts Color to textual form.&]
[s4; [%00-00*C@3 c]-|Color.&]
[s4; [*/ Return value]-|Textual form.&]
[s0; &]
[s5;K [%00-00@(0.0.255) void][%00-00 _][%00-00@0 RGBtoHSV][%00-00 (][%00-00@(0.0.255) double][%00-00 _
][%00-00@3 r][%00-00 , ][%00-00@(0.0.255) double][%00-00 _][%00-00@3 g][%00-00 , 
][%00-00@(0.0.255) double][%00-00 _][%00-00@3 b][%00-00 , ][%00-00@(0.0.255) double][%00-00 `&
_][%00-00@3 h][%00-00 , ][%00-00@(0.0.255) double][%00-00 `&_][%00-00@3 s][%00-00 , 
][%00-00@(0.0.255) double][%00-00 `&_][%00-00@3 v][%00-00 )]&]
[s2; Transforms an `[R,G,B`] color triplet (each of the components 
is supposed to be in the range `[0..1`]) into an `[H,S,V`] triplet 
(again all components are in the range `[0..1`]). This effectively 
transforms the RGB cube to a cone in which color selection more 
intuitively matches our our visual perception ([/ hue] corresponds 
to the tint of the color, [/ saturation] to the richness of the 
color hue and [/ value] corresponds to luminance or brightness). 
&]
[s2; &]
[s2; The mathematical formula used to convert between the two color 
spaces are:&]
[s2; &]
[s2; v `= max(r, g, b)&]
[s2; s `= 1 `- min(r, g, b) / v&]
[s2; h `= (x `- min(r, g, b)) / (max(r, g, b) `- min(r, g, b))&]
[s2; &]
[s2; where [/ x] is the color component which is neither minimum nor 
maximum. There are six combinations for this, in every even h 
is taken, in every odd 1`-h. The six combinations correspond 
to six points on the color wheel forming the base of the HSV 
cone `- pure red, yellow, green, cyan, blue, and magenta. The 
color component ranges along the circumference are the following 
(capital letters denote maximum component value, minuscule letters 
minimum): Rgb `-> RGb `-> rGb `-> rGB `-> rgB `-> RgB.&]
[s2; &]
[s2; An easy way to visualize the HSV transform based on the RGB 
cube is: imagine a slight deformation of the cube where the tip 
of the cone corresponds to the black RBG cube vertex, the opposite 
(white) RGB cube vertex corresponds to the center of the base 
circle of the HSV cone and the remaining six RGB cube vertices 
get projected on the circumference of the cone base as explained 
in the preceding paragraph.&]
[s2; &]
[s4; [%00-00*C@3 r]-|input red component `[0..1`]&]
[s4; [%00-00*C@3 g]-|input green component `[0..1`]&]
[s4; [%00-00*C@3 b]-|input blue component `[0..1`]&]
[s4; [%00-00*C@3 h]-|output hue `[0..1`], i.e. color tint&]
[s4; [%00-00*C@3 s]-|output saturation `[0..1`]. Maximum saturation 
(1) corresponds to the `"purest`" basic color (one of the six), 
whereas minimum saturation (0) is gray or white.&]
[s4; [%00-00*C@3 v]-|output brightness `[0..1`].&]
[s0; &]
[s0; &]
[s5;K [%00-00@(0.0.255) void][%00-00 _][%00-00@0 HSVtoRGB][%00-00 (][%00-00@(0.0.255) double][%00-00 _
][%00-00@3 h][%00-00 , ][%00-00@(0.0.255) double][%00-00 _][%00-00@3 s][%00-00 , 
][%00-00@(0.0.255) double][%00-00 _][%00-00@3 v][%00-00 , ][%00-00@(0.0.255) double][%00-00 `&
_][%00-00@3 r][%00-00 , ][%00-00@(0.0.255) double][%00-00 `&_][%00-00@3 g][%00-00 , 
][%00-00@(0.0.255) double][%00-00 `&_][%00-00@3 b][%00-00 )]&]
[s2; Transformes an HSV triplet (each of the components is supposed 
to lie in the interval `[0..1`]) into an RGB triplet (again with 
component ranges `[0..1`]). This projects the HSV cone (as explained 
in the preceding section) back onto the RGB cube.&]
[s4; [%00-00*C@3 h]-|input hue, i.e. color tint `[0..1`]&]
[s4; [%00-00*C@3 s]-|input saturation, i.e. color `"purity`" `[0..1`]&]
[s4; [%00-00*C@3 v]-|value `- luminance or brightness `[0..1`]&]
[s4; [%00-00*C@3 r]-|output red component `[0..1`]&]
[s4; [%00-00*C@3 g]-|output green component `[0..1`]&]
[s4; [%00-00*C@3 b]-|output blue component `[0..1`]&]
[s0; &]
[s5;K [%00-00^`:`:Color^ Color][%00-00 _][%00-00@0 HsvColorf][%00-00 (][%00-00@(0.0.255) double
][%00-00 _][%00-00@3 h][%00-00 , ][%00-00@(0.0.255) double][%00-00 _][%00-00@3 s][%00-00 , 
][%00-00@(0.0.255) double][%00-00 _][%00-00@3 v][%00-00 )]&]
[s2; Converts an HSV color triplet into a [* Color] object. The H, 
S, and V components are supposed to be in the range `[0..1`].&]
[s4; [%00-00*C@3 h]-|input hue `[0..1`]&]
[s4; [%00-00*C@3 s]-|input saturation `[0..1`]&]
[s4; [%00-00*C@3 v]-|input value aka brightness `[0..1`]&]
[s4; [*/ Return value]-|the [* Color] object with the given HSV components&]
[s0; &]
[s5;K [%00-00^`:`:Color^ Color][%00-00 _][%00-00@0 Blend][%00-00 (][%00-00^`:`:Color^ Color][%00-00 _
][%00-00@3 c1][%00-00 , ][%00-00^`:`:Color^ Color][%00-00 _][%00-00@3 c2][%00-00 , 
][%00-00@(0.0.255) int][%00-00 _][%00-00@3 alpha][%00-00 _`=_][%00-00@3 128][%00-00 )]&]
[s2; Blends two colors.&]
[s4; [%00-00*C@3 c1]-|First color.&]
[s4; [%00-00*C@3 c2]-|Second color.&]
[s4; [%00-00*C@3 alpha]-|Blending factor in the range 0..255.&]
[s4; [*/ Return value]-|Blended color `- (255 `- alpha) / 255.0 `* c1 
`+ alpha / 255.0 `* c2.&]
[s0; &]
[s5;K [%00-00^`:`:String^ String][%00-00 _][%00-00@0 ColorToHtml][%00-00 (][%00-00^`:`:Color^ C
olor][%00-00 _][%00-00@3 color][%00-00 )]&]
[s2; Converts Color to the textual format used in HTML (into hexadecimal 
form like #ffffff for white).&]
[s4; [%00-00*C@3 color]-|Color.&]
[s4; [*/ Return value]-|HTML text.&]
[s0; &]
[s0;3 &]
[s0;3 Color constants&]
[s0; Predefined colors are represented by functions that return the 
predefined color value.&]
[s5;K [%00-00^`:`:Color^ Color][%00-00 _][%00-00@0 Black][%00-00 ()]&]
[s5;K [%00-00^`:`:Color^ Color][%00-00 _][%00-00@0 Gray][%00-00 ()]&]
[s0; [%00-00 Color_][%00-00* LtGray][%00-00 ()]&]
[s0; [%00-00 Color_][%00-00* WhiteGray][%00-00 ()]&]
[s0; [%00-00 Color_][%00-00* White][%00-00 ()]&]
[s0; [%00-00 Color_][%00-00* Red][%00-00 ()]&]
[s0; [%00-00 Color_][%00-00* Green][%00-00 ()]&]
[s0; [%00-00 Color_][%00-00* Brown][%00-00 ()]&]
[s0; [%00-00 Color_][%00-00* Blue][%00-00 ()]&]
[s0; [%00-00 Color_][%00-00* Magenta][%00-00 ()]&]
[s0; [%00-00 Color_][%00-00* Cyan][%00-00 ()]&]
[s5;K [%00-00^`:`:Color^ Color][%00-00 _][%00-00@0 Yellow][%00-00 ()]&]
[s5;K [%00-00^`:`:Color^ Color][%00-00 _][%00-00@0 LtRed][%00-00 ()]&]
[s5;K [%00-00^`:`:Color^ Color][%00-00 _][%00-00@0 LtGreen][%00-00 ()]&]
[s5;K [%00-00^`:`:Color^ Color][%00-00 _][%00-00@0 LtYellow][%00-00 ()]&]
[s5;K [%00-00^`:`:Color^ Color][%00-00 _][%00-00@0 LtBlue][%00-00 ()]&]
[s5;K [%00-00^`:`:Color^ Color][%00-00 _][%00-00@0 LtMagenta][%00-00 ()]&]
[s5;K [%00-00^`:`:Color^ Color][%00-00 _][%00-00@0 LtCyan][%00-00 ()]&]
[s0; ]