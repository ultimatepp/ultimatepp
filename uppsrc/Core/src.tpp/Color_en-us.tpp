topic "Color";
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
[ {{10000@(113.42.0) [s0; [*@(229)4 Color]]}}&]
[s3;%- &]
[s1;:Color`:`:class:%- [@(0.0.255)3 class][3 _][*3 Color][3 _:_][@(0.0.255)3 private][3 _][*@3;3 Ass
ignValueTypeNo][@(0.0.255)3 <][*3 Color][3 , COLOR`_V, ][_^Moveable^3 Moveable][@(0.0.255)3 <
][*3 Color][@(0.0.255)3 >][3 _>_]&]
[s9; [* Color] type representing RGB color. All components are in the 
range 0 through 255. Null value is defined for Color. Color is 
Value compatible as a Rich`-Value type (supports Value comparison, 
serialization etc.).&]
[s3; &]
[s0;%- &]
[ {{10000F(128)G(128)@1 [s0; [* Constructor Detail]]}}&]
[s3; &]
[s5;:Color`:`:Color`(`):%- [* Color]()&]
[s2; Default constructor. Leaves component values uninitialized.&]
[s3; &]
[s4;%- &]
[s5;:Color`:`:Color`(int`,int`,int`):%- [* Color]([@(0.0.255) int]_[*@3 r], 
[@(0.0.255) int]_[*@3 g], [@(0.0.255) int]_[*@3 b])&]
[s2; Constructor.&]
[s7; [%-*C@3 r]-|Red channel.&]
[s7; [%-*C@3 g]-|Green channel.&]
[s7; [%-*C@3 b]-|Blue channel.&]
[s3; &]
[s4;%- &]
[s5;:Color`:`:Color`(const Nuller`&`):%- [* Color]([@(0.0.255) const]_[_^Nuller^ Nuller][@(0.0.255) `&
])&]
[s2; Null constructor.&]
[s3; &]
[s4;%- &]
[s5;:Color`:`:Color`(const Value`&`):%- [* Color]([@(0.0.255) const]_[_^Value^ Value][@(0.0.255) `&
]_[*@3 q])&]
[s2; Conversion from Value.&]
[s7; [%-*@3 q]-|Value.&]
[s3; &]
[s4;%- &]
[s5;:Color`:`:Color`(Color`(`*`)`(`)`):%- [* Color]([* Color]_(`*[*@3 fn])())&]
[s6;%- `[deprecated`]&]
[s2; This constructor allows using a pointer to a function returning 
Colors instead of Colors in many places. Global functions returning 
Color are needed instead of global Color constants on many platforms, 
because often global variables are not allowed when using dynamic 
libraries.&]
[s7; [%-*@3 fn]-|Pointer to a function returning Color.&]
[s3;%- &]
[s4;%- &]
[s5;:Color`:`:Color`(RGBA`):%- [* Color]([_^RGBA^ RGBA]_[*@3 rgba])&]
[s2; Conversion from a RGBA structure [%-*@3 rgba].&]
[s3; &]
[ {{10000F(128)G(128)@1 [s0; [* Public Member List]]}}&]
[s3; &]
[s5;:Color`:`:operator Value`(`)const:%- [* operator_Value]()_[@(0.0.255) const]&]
[s2; Conversion to Value.&]
[s7; [*/ Return value]-|Value.&]
[s3; &]
[s4;%- &]
[s5;:Color`:`:GetRaw`(`)const:%- [_^dword^ dword]_[* GetRaw]()_[@(0.0.255) const]&]
[s7; [*/ Return value]-|Platform specific dword representing the color.&]
[s3; &]
[s4;%- &]
[s5;:Color`:`:GetR`(`)const:%- [@(0.0.255) int]_[* GetR]()_[@(0.0.255) const]&]
[s7; [*/ Return value]-|The red component.&]
[s3; &]
[s4;%- &]
[s5;:Color`:`:GetG`(`)const:%- [@(0.0.255) int]_[* GetG]()_[@(0.0.255) const]&]
[s7; [*/ Return value]-|The green component.&]
[s3; &]
[s4;%- &]
[s5;:Color`:`:GetB`(`)const:%- [@(0.0.255) int]_[* GetB]()_[@(0.0.255) const]&]
[s7; [*/ Return value]-|The blue component.&]
[s3; &]
[s4;%- &]
[s5;:Color`:`:SetNull`(`):%- [@(0.0.255) void]_[* SetNull]()&]
[s2; Sets the Color to Null.&]
[s3;%- &]
[s4;%- &]
[s5;:Color`:`:IsNullInstance`(`)const:%- [@(0.0.255) bool]_[* IsNullInstance]()_[@(0.0.255) c
onst]&]
[s7; [*/ Return value]-|True if Color is Null. Used to implement IsNull 
for Color.&]
[s3; &]
[s4;%- &]
[s5;:Color`:`:GetHashValue`(`)const:%- [@(0.0.255) unsigned]_[* GetHashValue]()_[@(0.0.255) c
onst]&]
[s7; [*/ Return value]-|Hash`-value for Color.&]
[s3; &]
[s4;%- &]
[s5;:Color`:`:operator`=`=`(Color`)const:%- [@(0.0.255) bool]_[* operator`=`=]([_^Color^ Co
lor]_[*@3 c])_[@(0.0.255) const]&]
[s7; [%-*C@3 c]-|Color to compare with.&]
[s7; [*/ Return value]-|True if Color is equal to [@3 c].&]
[s3; &]
[s4;%- &]
[s5;:Color`:`:operator`!`=`(Color`)const:%- [@(0.0.255) bool]_[* operator!`=]([_^Color^ Col
or]_[*@3 c])_[@(0.0.255) const]&]
[s7; [%-*C@3 c]-|Color to compare with.&]
[s7; [*/ Return value]-|True if Color is not equal to [@3 c].&]
[s3; &]
[s4;%- &]
[s5;:Color`:`:Serialize`(Stream`&`):%- [@(0.0.255) void]_[* Serialize]([_^Stream^ Stream][@(0.0.255) `&
]_[*@3 s])&]
[s2; Serializes the color to the [^topic`:`/`/Core`/src`/Stream`$en`-us`#Stream`:`:class^ S
tream] [%-*@3 s].&]
[s3; &]
[s4;%- &]
[s5;:Color`:`:FromRaw`(dword`):%- [@(0.0.255) static] [_^Color^ Color]_[* FromRaw]([_^dword^ d
word]_[*@3 co])&]
[s2; Creates a Color based on a platform specific raw dword value.&]
[s7; [%-*C@3 co]-|Raw dword value.&]
[s7; [*/ Return value]-|Color.&]
[s3; &]
[s4;%- &]
[s5;:Upp`:`:Color`:`:Special`(int`):%- [@(0.0.255) static] [_^Upp`:`:Color^ Color]_[* Speci
al]([@(0.0.255) int]_[*@3 n])&]
[s2; Creates special color [%-*@3 n]. Special colors are application 
defined, sort of `"escape`" color in situation where something 
else has to be stored in Color, e.g. index of some color palette.&]
[s3;%- &]
[s4;%- &]
[s5;:Upp`:`:Color`:`:GetSpecial`(`)const:%- [@(0.0.255) int]_[* GetSpecial]()_[@(0.0.255) c
onst]&]
[s2; Returns special color value or `-1 if Color is not special.&]
[s3;%- &]
[s4;%- &]
[s5;:Color`:`:operator COLORREF`(`)const:%- [* operator_COLORREF]()_[@(0.0.255) const]&]
[s6;%- Win32 specific.&]
[s2; Return COLORREF for Color.&]
[s7; [*/ Return value]-|COLORREF.&]
[s3; &]
[s4;%- &]
[s5;:Color`:`:FromCR`(COLORREF`):%- [@(0.0.255) static] [_^Color^ Color]_[* FromCR](COLORRE
F_[*@3 cr])&]
[s6; Win32 specific.&]
[s2; Creates Color from COLORREF.&]
[s7; [%-*C@3 cr]-|COLORREF.&]
[s7; [*/ Return value]-|Color.&]
[s3;%- &]
[s4;%- &]
[s5;:Color`:`:operator dword`(`)const:%- [* operator_dword]()_[@(0.0.255) const]&]
[s6;%- Not available on Win32&]
[s7; [*/ Return value]-|[^topic`:`/`/Core`/src`/Color`$en`-us`#Color`:`:GetRaw`(`)const^ G
etRaw]().&]
[s3; &]
[s4;%- &]
[s5;:Color`:`:operator RGBA`(`)const:%- [* operator_RGBA]()_[@(0.0.255) const]&]
[s2; Returns a RGBA structure containing the color information.&]
[s3;%- &]
[s4;%- &]
[s5;:Upp`:`:Color`:`:ToString`(`)const:%- String [* ToString]() [@(0.0.255) const]&]
[s2; Converts Color to textual form.&]
[s3; &]
[ {{10000@(113.42.0) [s0; [*@(229)4 SColor]]}}&]
[s3;%- &]
[s1;:Upp`:`:SColor:%- [@(0.0.255) struct ][*3 SColor][3  ][@(0.0.255)3 :][3  
Color]&]
[s2; This creates special kind of `"logical`" color constant that 
is defined by function which is reevaluated on GUI theme change, 
thus allowing color to react to the new theme. Instances of this 
class must have static storage duration (cannot be stack nor 
thread variables). Number of SColor definitions is limited for 
performance reasons to about 1000. The SColor instances can be 
copied to normal Color while maintaining its dynamic feature.&]
[s0; &]
[ {{10000F(128)G(128)@1 [s0; [* Constructor Detail]]}}&]
[s3; &]
[s5;:Upp`:`:SColor`:`:SColor`(Color`(`*`)`(`)`):%- [* SColor](Color 
([@(0.0.255) `*]fn)() [@(0.0.255) `=] [@3 0])&]
[s2; Creates SColor with color definition function.&]
[s3; &]
[ {{10000@(113.42.0) [s0; [*@(229)4 AColor]]}}&]
[s3;%- &]
[s1;:Upp`:`:AColor:%- [@(0.0.255) struct ][*3 AColor][3  ][@(0.0.255)3 :][3  
Color]&]
[s2; This special kind of Color logical constant is created from 
normal color. In light theme, color is used unchanged. In dark 
theme, color is adjusted using [%-* DarkThemeCached] function. 
The AColor instances can be copied to normal Color while maintaining 
its dynamic feature. [/ (Unlike SColor there are no limitations 
with respect to storage duration).]&]
[s3; &]
[ {{10000F(128)G(128)@1 [s0; [* Constructor Detail]]}}&]
[s3;%- &]
[s5;:Upp`:`:AColor`:`:AColor`(Color`):%- [* AColor](Color [*@3 c])&]
[s5;:Upp`:`:AColor`:`:AColor`(int`,int`,int`):%- [* AColor]([@(0.0.255) int] 
[*@3 r], [@(0.0.255) int] [*@3 g], [@(0.0.255) int] [*@3 b])&]
[s2; Creates AColor.&]
[s3; &]
[ {{10000@(113.42.0) [s0; [*@(229)4 Utility functions]]}}&]
[s3;%- &]
[s5;:GetRValue`(dword`):%- [@(0.0.255) int]_[* GetRValue]([_^dword^ dword]_[*@3 c])&]
[s2; Returns red component from a platform specific value.&]
[s7; [%-*C@3 c]-|Platform specific value.&]
[s7; [*/ Return value]-|Red component.&]
[s3;%- &]
[s4;%- &]
[s5;:Upp`:`:operator`*`(int`,Color`):%- RGBA operator[@(0.0.255) `*]([@(0.0.255) int] 
alpha, Color c)&]
[s2; Returns RGBA value (premultiplied color with alpha).&]
[s3;%- &]
[s4;%- &]
[s5;:GetGValue`(dword`):%- [@(0.0.255) int]_[* GetGValue]([_^dword^ dword]_[*@3 c])&]
[s2; Returns green component from a platform specific value.&]
[s7; [%-*C@3 c]-|Platform specific value.&]
[s7; [*/ Return value]-|Green component.&]
[s3; &]
[s4;%- &]
[s5;:GetBValue`(dword`):%- [@(0.0.255) int]_[* GetBValue]([_^dword^ dword]_[*@3 c])&]
[s2; Returns blue component from a platform specific value.&]
[s7; [%-*C@3 c]-|Platform specific value.&]
[s7; [*/ Return value]-|Blue component.&]
[s3; &]
[s4;%- &]
[s5;:RGB`(byte`,byte`,byte`):%- [_^dword^ dword]_[* RGB]([_^byte^ byte]_[*@3 r], 
[_^byte^ byte]_[*@3 g], [_^byte^ byte]_[*@3 b])&]
[s2; Calculates platform specific value from components.&]
[s7; [%-*C@3 r]-|Red component.&]
[s7; [%-*C@3 g]-|Green component.&]
[s7; [%-*C@3 b]-|Blue component.&]
[s7; [*/ Return value]-|Platform specific value.&]
[s3; &]
[s4;%- &]
[s5;:GetHashValue`(Color`):%- [@(0.0.255) unsigned]_[* GetHashValue]([_^Color^ Color]_[*@3 c])
&]
[s2; Returns a hash value for the color.&]
[s7; [%-*C@3 c]-|Color.&]
[s7; [*/ Return value]-|Hash value.&]
[s3; &]
[s4;%- &]
[s5;:Nvl`(Color`,Color`):%- [_^Color^ Color]_[* Nvl]([_^Color^ Color]_[*@3 a], 
[_^Color^ Color]_[*@3 b])&]
[s2; Returns first parameter if it is not Null, second otherwise.&]
[s7; [%-*C@3 a]-|Color.&]
[s7; [%-*C@3 b]-|Color.&]
[s7; [*/ Return value]-|a if not Null, b otherwise.&]
[s3; &]
[s4;%- &]
[s5;:RGBtoHSV`(double`,double`,double`,double`&`,double`&`,double`&`):%- [@(0.0.255) vo
id]_[* RGBtoHSV]([@(0.0.255) double]_[*@3 r], [@(0.0.255) double]_[*@3 g], 
[@(0.0.255) double]_[*@3 b], [@(0.0.255) double`&]_[*@3 h], [@(0.0.255) double`&]_[*@3 s], 
[@(0.0.255) double`&]_[*@3 v])&]
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
[s3; &]
[s4;%- &]
[s5;:HSVtoRGB`(double`,double`,double`,double`&`,double`&`,double`&`):%- [@(0.0.255) vo
id]_[* HSVtoRGB]([@(0.0.255) double]_[*@3 h], [@(0.0.255) double]_[*@3 s], 
[@(0.0.255) double]_[*@3 v], [@(0.0.255) double`&]_[*@3 r], [@(0.0.255) double`&]_[*@3 g], 
[@(0.0.255) double`&]_[*@3 b])&]
[s2; Transformes an HSV triplet (each of the components is supposed 
to lie in the interval `[0..1`]) into an RGB triplet (again with 
component ranges `[0..1`]). This projects the HSV cone (as explained 
in the preceding section) back onto the RGB cube.&]
[s3; &]
[s4;%- &]
[s5;:HsvColorf`(double`,double`,double`):%- [_^Color^ Color]_[* HsvColorf]([@(0.0.255) doub
le]_[*@3 h], [@(0.0.255) double]_[*@3 s], [@(0.0.255) double]_[*@3 v])&]
[s2; Converts an HSV color triplet into a [* Color] object. The H, 
S, and V components are supposed to be in the range `[0..1`].&]
[s3; &]
[s4;%- &]
[s5;:Upp`:`:CMYKtoRGB`(double`,double`,double`,double`,double`&`,double`&`,double`&`):%- [@(0.0.255) v
oid]_[* CMYKtoRGB]([@(0.0.255) double]_[*@3 c], [@(0.0.255) double]_[*@3 m], 
[@(0.0.255) double]_[*@3 y], [@(0.0.255) double]_[*@3 k], [@(0.0.255) double`&]_[*@3 r], 
[@(0.0.255) double`&]_[*@3 g], [@(0.0.255) double`&]_[*@3 b])&]
[s2; Converts CMYK color definition to RGB (all components are in 
`[0..1`] range).&]
[s3; &]
[s4;%- &]
[s5;:Upp`:`:RGBtoCMYK`(double`,double`,double`,double`&`,double`&`,double`&`,double`&`):%- [@(0.0.255) v
oid]_[* RGBtoCMYK]([@(0.0.255) double]_[*@3 r], [@(0.0.255) double]_[*@3 g], 
[@(0.0.255) double]_[*@3 b], [@(0.0.255) double`&]_[*@3 c], [@(0.0.255) double`&]_[*@3 m], 
[@(0.0.255) double`&]_[*@3 y], [@(0.0.255) double`&]_[*@3 k])&]
[s2; Converts RGB color definition to CMYK (all components are in 
`[0..1`] range).&]
[s3; &]
[s4;%- &]
[s5;:Upp`:`:CmykColorf`(double`,double`,double`,double`):%- [_^Upp`:`:Color^ Color]_[* Cm
ykColorf]([@(0.0.255) double]_[*@3 c], [@(0.0.255) double]_[*@3 m], [@(0.0.255) double]_[*@3 y
], [@(0.0.255) double]_[*@3 k])&]
[s2; Converts CMYK color definition to RGB (all components are in 
`[0..1`] range).&]
[s3; &]
[s4;%- &]
[s5;:Upp`:`:RelativeLuminance`(Color`):%- [@(0.0.255) double] [* RelativeLuminance](Color
 [*@3 color])&]
[s2; Computes [^https`:`/`/www`.w3`.org`/TR`/2008`/REC`-WCAG20`-20081211`/`#relativeluminancedef^ r
elative luminance].&]
[s3;%- &]
[s4;%- &]
[s5;:Upp`:`:ContrastRatio`(Color`,Color`):%- [@(0.0.255) double] [* ContrastRatio](Color 
[*@3 c1], Color [*@3 c2])&]
[s2; Computes [^https`:`/`/www`.w3`.org`/TR`/2008`/REC`-WCAG20`-20081211`/`#contrast`-ratiodef^ c
ontrast ration].&]
[s3;%- &]
[s4;%- &]
[s5;:Upp`:`:CmyColorf`(double`,double`,double`):%- [_^Upp`:`:Color^ Color]_[* CmyColorf](
[@(0.0.255) double]_[*@3 c], [@(0.0.255) double]_[*@3 m], [@(0.0.255) double]_[*@3 y])&]
[s2; Converts CMY color definition to RGB (all components are in 
`[0..1`] range).&]
[s3; &]
[s4;%- &]
[s5;:Blend`(Color`,Color`,int`):%- [_^Color^ Color]_[* Blend]([_^Color^ Color]_[*@3 c1], 
[_^Color^ Color]_[*@3 c2], [@(0.0.255) int]_[*@3 alpha]_`=_[@3 128])&]
[s2; Blends two colors `- computes  (255 `- [%-*@3 alpha]) / 255.0 
`* [%-*@3 c1] `+ [%-*@3 alpha ]/ 255.0 `* [%-*@3 c2].&]
[s3; &]
[s4;%- &]
[s5;:Upp`:`:Lerp`(Color`,Color`,double`):%- Color [* Lerp](Color [*@3 a], 
Color [*@3 b], [@(0.0.255) double] [*@3 t])&]
[s2;%- Computes the linear interpolation (basically blend) between 
[*@3 a] and [*@3 b], if the parameter [*@3 t] is inside `[0,1`], the 
linear extrapolation otherwise. Extrapolation is clamped at valid 
channel values 0..255. This is similar to blend except [*@3 t] 
range is `[0,1`] and extrapolation works.&]
[s3;%- &]
[s4;%- &]
[s5;:ColorToHtml`(Color`):%- [_^String^ String]_[* ColorToHtml]([_^Color^ Color]_[*@3 color])
&]
[s7; Converts Color to the textual format used in HTML (into hexadecimal 
form like #ffffff for white).&]
[s3; &]
[s4;%- &]
[s5;:Upp`:`:ColorFromText`(const char`*`):%- [_^Upp`:`:Color^ Color]_[* ColorFromText]([@(0.0.255) c
onst]_[@(0.0.255) char]_`*[*@3 s])&]
[s2; Converts text to Color. Allowed formats are 3 decimal numbers 
in range 0`-255, representing r, g, b colors and separated by 
`',`' or `';`' or `'.`' or `' `', or the text must exactly contain 
6 hexadecimal digits representing RRGGBB values.&]
[s3; &]
[s4;%- &]
[s5;:GrayColor`(int`):%- [_^Color^ Color]_[* GrayColor]([@(0.0.255) int]_[*@3 a]_`=_[@3 128])&]
[s2; Returns a gray color of intensity [%-*@3 a].&]
[s3; &]
[s4;%- &]
[s5;:Grayscale`(const Color`&`):%- [@(0.0.255) int]_[* Grayscale]([@(0.0.255) const]_[_^Color^ C
olor][@(0.0.255) `&]_[*@3 c])&]
[s2; Converts a color [%-*@3 c][%-  to grayscale].&]
[s3; &]
[s4;%- &]
[s5;:IsDark`(Color`):%- [@(0.0.255) bool]_[* IsDark]([_^Color^ Color]_[*@3 c])&]
[s2; True if [%-*@3 c] grayscale level is lower than 80.&]
[s3; &]
[s4;%- &]
[s5;:IsLight`(Color`):%- [@(0.0.255) bool]_[* IsLight]([_^Color^ Color]_[*@3 c])&]
[s2; True if [%-*@3 c] grayscale is higher than 175 (255 `- 80).&]
[s3; &]
[s4;%- &]
[s5;:Upp`:`:DarkTheme`(Upp`:`:Color`):%- [_^Upp`:`:Color^ Color]_[* DarkTheme]([_^Upp`:`:Color^ C
olor]_[*@3 c])&]
[s2; `'Dark Theme conversion`'. Converts dark colors to light ones 
and vice versa, sustaining the color hue.&]
[s3; &]
[s4;%- &]
[s5;:Upp`:`:DarkThemeCached`(Upp`:`:Color`):%- [_^Upp`:`:Color^ Color]_[* DarkThemeCached
]([_^Upp`:`:Color^ Color]_[*@3 c])&]
[s2; Cached variant of DarkTheme, caches up to 4 repeating colors.&]
[s3; &]
[s4;%- &]
[s5;:StraightColor`(RGBA`):%- [_^Color^ Color]_[* StraightColor]([_^RGBA^ RGBA]_[*@3 rgba])&]
[s2; Converts a RGBA structure [%-*@3 rgba] to a [* Color].&]
[s3; &]
[s0;3 &]
[ {{10000@(113.42.0) [s0; [*@(229)4 Color constants]]}}&]
[s3; &]
[s9; Predefined colors are represented by functions that return the 
predefined color value.&]
[s3; &]
[s3; &]
[s5;:Black`(`):%- [_^Color^ Color]_[* Black]()&]
[s2; Black.&]
[s3;%- &]
[s4;%- &]
[s5;:Gray`(`):%- [_^Color^ Color]_[* Gray]()&]
[s2; Gray.&]
[s3;%- &]
[s4;%- &]
[s5;:LtGray`(`):%- [_^Color^ Color]_[* LtGray]()&]
[s2; Light gray.&]
[s3;%- &]
[s4;%- &]
[s5;:WhiteGray`(`):%- [_^Color^ Color]_[* WhiteGray]()&]
[s2; White gray.&]
[s3;%- &]
[s4;%- &]
[s5;:White`(`):%- [_^Color^ Color]_[* White]()&]
[s2; White.&]
[s3;%- &]
[s4;%- &]
[s5;:Red`(`):%- [_^Color^ Color]_[* Red]()&]
[s2; Red.&]
[s3;%- &]
[s4;%- &]
[s5;:Green`(`):%- [_^Color^ Color]_[* Green]()&]
[s2; Green.&]
[s3;%- &]
[s4;%- &]
[s5;:Blue`(`):%- [_^Color^ Color]_[* Blue]()&]
[s2; Blue.&]
[s3;%- &]
[s4;%- &]
[s5;:Brown`(`):%- [_^Color^ Color]_[* Brown]()&]
[s2; Brown.&]
[s3;%- &]
[s4;%- &]
[s5;:Orange`(`):%- [_^Color^ Color]_[* Orange]()&]
[s2; Orange.&]
[s3; &]
[s3;%- &]
[s4;%- &]
[s5;:Pink`(`):%- [_^Color^ Color]_[* Pink]()&]
[s2; Pink.&]
[s3; &]
[s3;%- &]
[s4;%- &]
[s5;:Magenta`(`):%- [_^Color^ Color]_[* Magenta]()&]
[s2; Magenta.&]
[s3;%- &]
[s4;%- &]
[s5;:Cyan`(`):%- [_^Color^ Color]_[* Cyan]()&]
[s2; Cyan.&]
[s3;%- &]
[s4;%- &]
[s5;:Yellow`(`):%- [_^Color^ Color]_[* Yellow]()&]
[s2; Yellow.&]
[s3; &]
[s4;%- &]
[s5;:DkRed`(`):%- [_^Color^ Color]_[* DkRed]()&]
[s2; Dark Red.&]
[s3;%- &]
[s4;%- &]
[s5;:DkGreen`(`):%- [_^Color^ Color]_[* DkGreen]()&]
[s2; Dark Green.&]
[s3;%- &]
[s4;%- &]
[s5;:DkBlue`(`):%- [_^Color^ Color]_[* DkBlue]()&]
[s2; Dark Blue.&]
[s3;%- &]
[s4;%- &]
[s5;:DkYellow`(`):%- [_^Color^ Color]_[* DkYellow]()&]
[s2; Dark Yellow.&]
[s3;%- &]
[s4;%- &]
[s5;:DkMagenta`(`):%- [_^Color^ Color]_[* DkMagenta]()&]
[s2; Dark Magenta.&]
[s3; &]
[s3;%- &]
[s4;%- &]
[s5;:DkCyan`(`):%- [_^Color^ Color]_[* DkCyan]()&]
[s2; Dark Cyan.&]
[s3; &]
[s3;%- &]
[s4;%- &]
[s5;:LtRed`(`):%- [_^Color^ Color]_[* LtRed]()&]
[s2; Light Red.&]
[s3;%- &]
[s4;%- &]
[s5;:LtGreen`(`):%- [_^Color^ Color]_[* LtGreen]()&]
[s2; Light Green.&]
[s3;%- &]
[s4;%- &]
[s5;:LtYellow`(`):%- [_^Color^ Color]_[* LtYellow]()&]
[s2; Light Yellow.&]
[s3;%- &]
[s4;%- &]
[s5;:LtBlue`(`):%- [_^Color^ Color]_[* LtBlue]()&]
[s2; Light Blue.&]
[s3;%- &]
[s4;%- &]
[s5;:LtMagenta`(`):%- [_^Color^ Color]_[* LtMagenta]()&]
[s2; Light Magenta.&]
[s3;%- &]
[s4;%- &]
[s5;:LtCyan`(`):%- [_^Color^ Color]_[* LtCyan]()&]
[s2; Light Cyan.&]
[s0; ]]
