topic "Color";
[ $$0,0#00000000000000000000000000000000:Default]
[i448;a25;kKO9; $$1,0#37138531426314131252341829483380:class]
[l288;2 $$2,0#27521748481378242620020725143825:desc]
[0 $$3,0#96390100711032703541132217272105:end]
[H6;0 $$4,0#05600065144404261032431302351956:begin]
[i448;a25;kKO9;2 $$5,0#37138531426314131252341829483370:item]
[l288;a17;*@5;1 $$6,6#70004532496200323422659154056402:requirement]
[l288;i1121;b17;O9;~~~.1408;2 $$7,0#10431211400427159095818037425705:param]
[{_} 
[s5; [@(0.0.255) class][@(64) _]Color[@(64) _:_][@(0.0.255) private][@(64) _][^`:`:AssignValueTypeNo^@(64) A
ssignValueTypeNo][@(64) <]Color[@(64) , ][^COLOR`_V^@(64) COLOR`_V][@(64) , 
][^`:`:Moveable^@(64) Moveable][@(64) <]Color[@(64) >_>_]&]
[s5; [@(0.0.255) class][@(64) _]Color[@(64) _:_][@(0.0.255) private][@(64) _][^`:`:AssignValueTypeNo^@(64) A
ssignValueTypeNo][@(64) <]Color[@(64) , ][^COLOR`_V^@(64) COLOR`_V][@(64) , 
][^`:`:Moveable^@(64) Moveable][@(64) <]Color[@(64) >_>_]&]
[s2;%% Type representing RGB color. All components are in the range 
0 through 255. Null value is defined for Color. Color is Value 
compatible as a Rich`-Value type (supports Value comparison, 
serialization etc.).&]
[s0;/%% &]
[s3; &]
[s4; &]
[s5;:`:`:Color`:`:GetRaw`(`)const: [_^`:`:dword^ dword]_[* GetRaw]()_[@(0.0.255) const]&]
[s7;%% [*/ Return value]-|Platform specific dword representing the color.&]
[s3; &]
[s4; &]
[s5;:`:`:Color`:`:GetR`(`)const: [@(0.0.255) int]_[* GetR]()_[@(0.0.255) const]&]
[s7;%% [*/ Return value]-|The red component.&]
[s3; &]
[s4; &]
[s5;:`:`:Color`:`:GetG`(`)const: [@(0.0.255) int]_[* GetG]()_[@(0.0.255) const]&]
[s7;%% [*/ Return value]-|The green component.&]
[s3; &]
[s4; &]
[s5;:`:`:Color`:`:GetB`(`)const: [@(0.0.255) int]_[* GetB]()_[@(0.0.255) const]&]
[s7;%% [*/ Return value]-|The blue component.&]
[s3; &]
[s4; &]
[s5;:`:`:Color`:`:IsNullInstance`(`)const: [@(0.0.255) bool]_[* IsNullInstance]()_[@(0.0.255) c
onst]&]
[s7;%% [*/ Return value]-|True if Color is Null. Used to implement IsNull 
for Color.&]
[s3; &]
[s4; &]
[s5;:`:`:Color`:`:GetHashValue`(`)const: [@(0.0.255) unsigned]_[* GetHashValue]()_[@(0.0.255) c
onst]&]
[s7;%% [*/ Return value]-|Hash`-value for Color.&]
[s3; &]
[s4; &]
[s5;:`:`:Color`:`:operator`=`=`(`:`:Color`)const: [@(0.0.255) bool]_[* operator`=`=]([_^`:`:Color^ C
olor]_[*@3 c])_[@(0.0.255) const]&]
[s7;%% [%-*C@3 c]-|Color to compare with.&]
[s7;%% [*/ Return value]-|True if Color is equal to [@3 c].&]
[s3; &]
[s4; &]
[s5;:`:`:Color`:`:operator`!`=`(`:`:Color`)const: [@(0.0.255) bool]_[* operator!`=]([_^`:`:Color^ C
olor]_[*@3 c])_[@(0.0.255) const]&]
[s7;%% [%-*C@3 c]-|Color to compare with.&]
[s7;%% [*/ Return value]-|True if Color is equal to [@3 c].&]
[s3; &]
[s4; &]
[s5;:`:`:Color`:`:Serialize`(`:`:Stream`&`): [@(0.0.255) void]_[* Serialize]([_^`:`:Stream^ S
tream]`&_[*@3 s])&]
[s2;%% Serialization.&]
[s7;%% [%-*C@3 s]-|Stream.&]
[s3; &]
[s4; &]
[s5;:`:`:Color`:`:Color`(`): [* Color]()&]
[s2;%% Default constructor. Leaves component values uninitialized.&]
[s3; &]
[s4; &]
[s5;:`:`:Color`:`:Color`(int`,int`,int`): [* Color]([@(0.0.255) int]_[*@3 r], 
[@(0.0.255) int]_[*@3 g], [@(0.0.255) int]_[*@3 b])&]
[s2;%% Constructor.&]
[s7;%% [%-*C@3 r]-|Red channel.&]
[s7;%% [%-*C@3 g]-|Green channel.&]
[s7;%% [%-*C@3 b]-|Blue channel.&]
[s3; &]
[s4; &]
[s5;:`:`:Color`:`:Color`(const`:`:Nuller`&`): [* Color]([@(0.0.255) const]_[_^`:`:Nuller^ N
uller]`&)&]
[s2;%% Null constructor.&]
[s3; &]
[s4; &]
[s5;:`:`:Color`:`:operator Value`(`)const: [@(0.0.255) operator]_Value()_[@(0.0.255) cons
t]&]
[s2;%% Conversion to Value.&]
[s7;%% [*/ Return value]-|Value.&]
[s3; &]
[s4; &]
[s5;:`:`:Color`:`:Color`(const`:`:Value`&`): [* Color]([@(0.0.255) const]_[_^`:`:Value^ Val
ue]`&_[*@3 q])&]
[s2;%% Conversion from Value.&]
[s7;%% [%-*C@3 q]-|Value.&]
[s3; &]
[s4; &]
[s5;:`:`:Color`:`:Color`(`:`:Color`(`*`)`(`)`): [* Color]([* Color]_(`*[*@3 fn])())&]
[s2;%% This constructor allows using a pointer to a function returning 
Colors instead of Colors in many places. Global functions returning 
Color are needed instead of global Color constants on many platforms, 
because often global variables are not allowed when using dynamic 
libraries.&]
[s7;%% [%-*C@3 fn]-|Pointer to a function returning Color.&]
[s5;K static [^`:`:Color^ Color]_FromRaw[@(64) (][^`:`:dword^@(64) dword][@(64) _][@3 co][@(64) )
]&]
[s2;%% Creates a Color based on a platform specific raw dword value.&]
[s7;%% [%-*C@3 co]-|Raw dword value.&]
[s7;%% [*/ Return value]-|Color.&]
[s3; &]
[s4; &]
[s5;:`:`:Color`:`:operator COLORREF`(`)const: [@(0.0.255) operator]_COLORREF()_[@(0.0.255) c
onst]&]
[s0; Win32 specific.&]
[s2;%% Return COLORREF for Color.&]
[s7;%% [*/ Return value]-|COLORREF.&]
[s5;K static [^`:`:Color^ Color]_FromCR[@(64) (][^COLORREF^@(64) COLORREF][@(64) _][@3 cr][@(64) )
]&]
[s0;%% Win32 specific.&]
[s2;%% Creates Color from COLORREF.&]
[s7;%% [%-*C@3 cr]-|COLORREF.&]
[s7;%% [*/ Return value]-|Color.&]
[s3; &]
[s4; &]
[s5;:`:`:Color`:`:operator dword`(`)const: [@(0.0.255) operator]_dword()_[@(0.0.255) cons
t]&]
[s7;%% [*/ Return value]-|GetRaw().&]
[s0;%% &]
[s0;%% &]
[s0;:`:`:GrayColor`:`:struct: struct_[* GrayColor]&]
[s2;%% &]
[s2;%% This simple utility class provides constructor that sets all 
components to the same value producing the gray color.&]
[s2;%% &]
[s0; [%%/ Derived from][%%  ]Color&]
[s0;%% &]
[s0;:`:`:GrayColor`:`:GrayColor`(int`): [* GrayColor](int_[*@3 level])&]
[s2;%% Constructor.&]
[s7;%% [%-*C@3 level]-|Gray intensity.&]
[s0;%% &]
[s0;%% Utility functions&]
[s5;K [@(0.0.255) int][@(64) _]GetRValue[@(64) (][^`:`:dword^@(64) dword][@(64) _][@3 c][@(64) )]&]
[s0; int_[* GetRValue](dword_[*@3 c])&]
[s2;%% Returns red component from a platform specific value.&]
[s7;%% [%-*C@3 c]-|Platform specific value.&]
[s7;%% [*/ Return value]-|Red component.&]
[s0;%% &]
[s5;K [@(0.0.255) int][@(64) _]GetGValue[@(64) (][^`:`:dword^@(64) dword][@(64) _][@3 c][@(64) )]&]
[s2;%% Returns green component from a platform specific value.&]
[s7;%% [%-*C@3 c]-|Platform specific value.&]
[s7;%% [*/ Return value]-|Green component.&]
[s0;%% &]
[s5;K [@(0.0.255) int][@(64) _]GetBValue[@(64) (][^`:`:dword^@(64) dword][@(64) _][@3 c][@(64) )]&]
[s2;%% Returns blue component from a platform specific value.&]
[s7;%% [%-*C@3 c]-|Platform specific value.&]
[s7;%% [*/ Return value]-|Blue component.&]
[s0;%% &]
[s5;K [^`:`:dword^ dword]_RGB[@(64) (][^`:`:byte^@(64) byte][@(64) _][@3 r][@(64) , 
][^`:`:byte^@(64) byte][@(64) _][@3 g][@(64) , ][^`:`:byte^@(64) byte][@(64) _][@3 b][@(64) )]&]
[s2;%% Calculates platform specific value from components.&]
[s7;%% [%-*C@3 r]-|Red component.&]
[s7;%% [%-*C@3 g]-|Green component.&]
[s7;%% [%-*C@3 b]-|Blue component.&]
[s7;%% [*/ Return value]-|Platform specific value.&]
[s0;%% &]
[s5;K [@(0.0.255) unsigned][@(64) _]GetHashValue[@(64) (][^`:`:Color^@(64) Color][@(64) _][@3 c][@(64) )
]&]
[s2;%% Returns a hash value for the color.&]
[s7;%% [%-*C@3 c]-|Color.&]
[s7;%% [*/ Return value]-|Hash value.&]
[s0;%% &]
[s5;K [^`:`:Color^ Color]_Nvl[@(64) (][^`:`:Color^@(64) Color][@(64) _][@3 a][@(64) , 
][^`:`:Color^@(64) Color][@(64) _][@3 b][@(64) )]&]
[s2;%% Returns first parameter if it is not Null, second otherwise.&]
[s7;%% [%-*C@3 a]-|Color.&]
[s7;%% [%-*C@3 b]-|Color.&]
[s7;%% [*/ Return value]-|a if not Null, b otherwise.&]
[s0;%% &]
[s5;K template_<>__[^`:`:String^ String]_AsString[@(64) (][@(0.0.255) const][@(64) _][^`:`:Color^@(64) C
olor][@(64) `&_][@3 c][@(64) )]&]
[s5;K template_<>__[^`:`:String^ String]_AsString[@(64) (][@(0.0.255) const][@(64) _][^`:`:Color^@(64) C
olor][@(64) `&_][@3 c][@(64) )]&]
[s2;%% Converts Color to textual form.&]
[s7;%% [%-*C@3 c]-|Color.&]
[s7;%% [*/ Return value]-|Textual form.&]
[s0;%% &]
[s5;K [@(0.0.255) void][@(64) _]RGBtoHSV[@(64) (][@(0.0.255) double][@(64) _][@3 r][@(64) , 
][@(0.0.255) double][@(64) _][@3 g][@(64) , ][@(0.0.255) double][@(64) _][@3 b][@(64) , 
][@(0.0.255) double][@(64) `&_][@3 h][@(64) , ][@(0.0.255) double][@(64) `&_][@3 s][@(64) , 
][@(0.0.255) double][@(64) `&_][@3 v][@(64) )]&]
[s2;%% Transforms an `[R,G,B`] color triplet (each of the components 
is supposed to be in the range `[0..1`]) into an `[H,S,V`] triplet 
(again all components are in the range `[0..1`]). This effectively 
transforms the RGB cube to a cone in which color selection more 
intuitively matches our our visual perception ([/ hue] corresponds 
to the tint of the color, [/ saturation] to the richness of the 
color hue and [/ value] corresponds to luminance or brightness). 
&]
[s2;%% &]
[s2;%% The mathematical formula used to convert between the two color 
spaces are:&]
[s2;%% &]
[s2;%% v `= max(r, g, b)&]
[s2;%% s `= 1 `- min(r, g, b) / v&]
[s2;%% h `= (x `- min(r, g, b)) / (max(r, g, b) `- min(r, g, b))&]
[s2;%% &]
[s2;%% where [/ x] is the color component which is neither minimum 
nor maximum. There are six combinations for this, in every even 
h is taken, in every odd 1`-h. The six combinations correspond 
to six points on the color wheel forming the base of the HSV 
cone `- pure red, yellow, green, cyan, blue, and magenta. The 
color component ranges along the circumference are the following 
(capital letters denote maximum component value, minuscule letters 
minimum): Rgb `-> RGb `-> rGb `-> rGB `-> rgB `-> RgB.&]
[s2;%% &]
[s2;%% An easy way to visualize the HSV transform based on the RGB 
cube is: imagine a slight deformation of the cube where the tip 
of the cone corresponds to the black RBG cube vertex, the opposite 
(white) RGB cube vertex corresponds to the center of the base 
circle of the HSV cone and the remaining six RGB cube vertices 
get projected on the circumference of the cone base as explained 
in the preceding paragraph.&]
[s2;%% &]
[s7;%% [%-*C@3 r]-|input red component `[0..1`]&]
[s7;%% [%-*C@3 g]-|input green component `[0..1`]&]
[s7;%% [%-*C@3 b]-|input blue component `[0..1`]&]
[s7;%% [%-*C@3 h]-|output hue `[0..1`], i.e. color tint&]
[s7;%% [%-*C@3 s]-|output saturation `[0..1`]. Maximum saturation (1) 
corresponds to the `"purest`" basic color (one of the six), whereas 
minimum saturation (0) is gray or white.&]
[s7;%% [%-*C@3 v]-|output brightness `[0..1`].&]
[s0;%% &]
[s0;%% &]
[s5;K [@(0.0.255) void][@(64) _]HSVtoRGB[@(64) (][@(0.0.255) double][@(64) _][@3 h][@(64) , 
][@(0.0.255) double][@(64) _][@3 s][@(64) , ][@(0.0.255) double][@(64) _][@3 v][@(64) , 
][@(0.0.255) double][@(64) `&_][@3 r][@(64) , ][@(0.0.255) double][@(64) `&_][@3 g][@(64) , 
][@(0.0.255) double][@(64) `&_][@3 b][@(64) )]&]
[s2;%% Transformes an HSV triplet (each of the components is supposed 
to lie in the interval `[0..1`]) into an RGB triplet (again with 
component ranges `[0..1`]). This projects the HSV cone (as explained 
in the preceding section) back onto the RGB cube.&]
[s7;%% [%-*C@3 h]-|input hue, i.e. color tint `[0..1`]&]
[s7;%% [%-*C@3 s]-|input saturation, i.e. color `"purity`" `[0..1`]&]
[s7;%% [%-*C@3 v]-|value `- luminance or brightness `[0..1`]&]
[s7;%% [%-*C@3 r]-|output red component `[0..1`]&]
[s7;%% [%-*C@3 g]-|output green component `[0..1`]&]
[s7;%% [%-*C@3 b]-|output blue component `[0..1`]&]
[s0;%% &]
[s5;K [^`:`:Color^ Color]_HsvColorf[@(64) (][@(0.0.255) double][@(64) _][@3 h][@(64) , 
][@(0.0.255) double][@(64) _][@3 s][@(64) , ][@(0.0.255) double][@(64) _][@3 v][@(64) )]&]
[s2;%% Converts an HSV color triplet into a [* Color] object. The H, 
S, and V components are supposed to be in the range `[0..1`].&]
[s7;%% [%-*C@3 h]-|input hue `[0..1`]&]
[s7;%% [%-*C@3 s]-|input saturation `[0..1`]&]
[s7;%% [%-*C@3 v]-|input value aka brightness `[0..1`]&]
[s7;%% [*/ Return value]-|the [* Color] object with the given HSV components&]
[s0;%% &]
[s5;K [^`:`:Color^ Color]_Blend[@(64) (][^`:`:Color^@(64) Color][@(64) _][@3 c1][@(64) , 
][^`:`:Color^@(64) Color][@(64) _][@3 c2][@(64) , ][@(0.0.255) int][@(64) _][@3 alpha][@(64) _`=_
][@3 128][@(64) )]&]
[s2;%% Blends two colors.&]
[s7;%% [%-*C@3 c1]-|First color.&]
[s7;%% [%-*C@3 c2]-|Second color.&]
[s7;%% [%-*C@3 alpha]-|Blending factor in the range 0..255.&]
[s7;%% [*/ Return value]-|Blended color `- (255 `- alpha) / 255.0 `* 
c1 `+ alpha / 255.0 `* c2.&]
[s0;%% &]
[s5;K [^`:`:String^ String]_ColorToHtml[@(64) (][^`:`:Color^@(64) Color][@(64) _][@3 color][@(64) )
]&]
[s2;%% Converts Color to the textual format used in HTML (into hexadecimal 
form like #ffffff for white).&]
[s7;%% [%-*C@3 color]-|Color.&]
[s7;%% [*/ Return value]-|HTML text.&]
[s0;%% &]
[s0;%% &]
[s0;%% Color constants&]
[s0;%% Predefined colors are represented by functions that return 
the predefined color value.&]
[s5;K [^`:`:Color^ Color]_Black[@(64) ()]&]
[s5;K [^`:`:Color^ Color]_Gray[@(64) ()]&]
[s0; Color_[* LtGray]()&]
[s0; Color_[* WhiteGray]()&]
[s0; Color_[* White]()&]
[s0; Color_[* Red]()&]
[s0; Color_[* Green]()&]
[s0; Color_[* Brown]()&]
[s0; Color_[* Blue]()&]
[s0; Color_[* Magenta]()&]
[s0; Color_[* Cyan]()&]
[s5;K [^`:`:Color^ Color]_Yellow[@(64) ()]&]
[s5;K [^`:`:Color^ Color]_LtRed[@(64) ()]&]
[s5;K [^`:`:Color^ Color]_LtGreen[@(64) ()]&]
[s5;K [^`:`:Color^ Color]_LtYellow[@(64) ()]&]
[s5;K [^`:`:Color^ Color]_LtBlue[@(64) ()]&]
[s5;K [^`:`:Color^ Color]_LtMagenta[@(64) ()]&]
[s5;K [^`:`:Color^ Color]_LtCyan[@(64) ()]&]
[s0;%% &]
[s0; ]