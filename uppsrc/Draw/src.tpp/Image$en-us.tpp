topic "Image and ImageBuffer";
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
[{_} 
[ {{10000@(113.42.0) [s0;%% [*@7;4 Image]]}}&]
[s3; &]
[s1;:Image`:`:class: [*@(0.0.255) class][* _Image_:_][*@(0.0.255) public][* _][*@3 AssignValueTy
peNo][* <_Image, ][*@3 150][* , ][*_^Moveable^ Moveable][* <Image>_>_]&]
[s9;%% Image represents an immutable image value. Image can be directly 
painted to Draw. To create or change Image, use [^`:`:ImageBuffer^ ImageBuffer].&]
[s9;%% Image has low`-cost constant time deep copy. It is moveable 
and Rich`-Value compatible type.&]
[s9;%% U`+`+ expects pixels to be in premultiplied alpha format.&]
[s3; &]
[s0; &]
[ {{10000F(128)G(128)@1 [s0;%% [* Public Member List]]}}&]
[s3; &]
[s5;:Image`:`:operator`~`(`)const: [@(0.0.255) const]_[_^RGBA^ RGBA][@(0.0.255) `*]_operato
r`~()_[@(0.0.255) const]&]
[s5;:Image`:`:operator const RGBA`*`(`)const: operator_const_RGBA`*()_[@(0.0.255) const
]&]
[s2;%% Returns a pointer to Image pixels.&]
[s3; &]
[s4; &]
[s5;:Image`:`:operator`[`]`(int`)const: [@(0.0.255) const]_[_^RGBA^ RGBA][@(0.0.255) `*]_op
erator`[`]([@(0.0.255) int]_[@3 i])_[@(0.0.255) const]&]
[s2;%% Returns a pointer to the first pixel in the line [%-*@3 i].&]
[s3; &]
[s4; &]
[s5;:Image`:`:GetSize`(`)const: [_^Size^ Size]_GetSize()_[@(0.0.255) const]&]
[s2;%% Returns the dimension of Image.&]
[s3; &]
[s4; &]
[s5;:Image`:`:GetWidth`(`)const: [@(0.0.255) int]_GetWidth()_[@(0.0.255) const]&]
[s2;%% Same as GetSize().cx.&]
[s3; &]
[s4; &]
[s5;:Image`:`:GetHeight`(`)const: [@(0.0.255) int]_GetHeight()_[@(0.0.255) const]&]
[s2;%% Same as GetSize().cy.&]
[s3; &]
[s4; &]
[s5;:Image`:`:GetLength`(`)const: [@(0.0.255) int]_GetLength()_[@(0.0.255) const]&]
[s2;%% Number of pixels in Image `- GetWidth() `* GetHeight().&]
[s3; &]
[s4; &]
[s5;:Image`:`:GetHotSpot`(`)const: [_^Point^ Point]_GetHotSpot()[@(64) _][@(0.0.255) const]&]
[s2;%% Returns the reference point.&]
[s3; &]
[s4; &]
[s5;:Image`:`:Get2ndSpot`(`)const: [_^Point^ Point]_Get2ndSpot()_[@(0.0.255) const]&]
[s2;%% Returns secondary reference point.&]
[s3; &]
[s4; &]
[s5;:Image`:`:GetDots`(`)const: [_^Size^ Size]_GetDots()[@(64) _][@(0.0.255) const]&]
[s2;%% Gets the physical size of Image. If physical size is not set, 
returns Size(0, 0).&]
[s3; &]
[s4; &]
[s5;:Image`:`:GetDPI`(`): [_^Size^ Size]_GetDPI()&]
[s2;%% Returns the image resolution in dpi (dots per inch).&]
[s3; &]
[s4; &]
[s5;:Image`:`:GetKindNoScan`(`)const: [@(0.0.255) int]_GetKindNoScan()_[@(0.0.255) const]&]
[s2;%% Returns the kind of image. Unlike GetKind(), it does not perform 
the scan of image if the kind it not know, therefore it can return 
IMAGE`_UNKNOWN.&]
[s3; &]
[s4; &]
[s5;:Image`:`:GetKind`(`)const: [@(0.0.255) int]_GetKind()_[@(0.0.255) const]&]
[s2;%% Returns the kind of image, either set by ImageBuffer`::SetKind 
or (if it is IMAGE`_UNKNOWN) resolved by scanning pixels. If 
the kind is IMAGE`_UNKNOWN, the pixels are scanned and the type 
is determined, it is then stored (in private Image instance variable) 
for future use and returned.&]
[s3; &]
[s4; &]
[s5;:Image`:`:Begin`(`)const: [@(0.0.255) const]_[_^RGBA^ RGBA]_`*[* Begin]()_[@(0.0.255) con
st]&]
[s2;%% Returns a pointer to the first pixel.&]
[s3; &]
[s4; &]
[s5;:Image`:`:End`(`)const: [@(0.0.255) const]_[_^RGBA^ RGBA]_`*[* End]()_[@(0.0.255) const]&]
[s2;%% Returns Begin() `+ GetLength().&]
[s3; &]
[s4; &]
[s5;:Image`:`:GetSerialId`(`)const: [_^int64^ int64]_GetSerialId()[@(64) _][@(0.0.255) cons
t]&]
[s2;%% Returns the unique, per application run, identifier of Image. 
All Images with the same serial id can be considered equal (this 
is useful for caching images).&]
[s3; &]
[s4; &]
[s5;:Image`:`:IsSame`(const Image`&`)const: [@(0.0.255) bool]_IsSame([@(0.0.255) const]_[_^Image^ I
mage][@(0.0.255) `&]_[@3 img])_[@(0.0.255) const]&]
[s2;%% Same as GetSerialId() `=`= img.GetSerialId().&]
[s3; &]
[s4; &]
[s5;:Image`:`:operator`=`=`(const Image`&`)const: [@(0.0.255) bool]_operator`=`=([@(0.0.255) c
onst]_[_^Image^ Image][@(0.0.255) `&]_[@3 img])_[@(0.0.255) const]&]
[s3; &]
[s4; &]
[s5;:Image`:`:operator`!`=`(const Image`&`)const: [@(0.0.255) bool]_operator!`=([@(0.0.255) c
onst]_[_^Image^ Image][@(0.0.255) `&]_[@3 img])_[@(0.0.255) const]&]
[s2;%% Tests whether two Images are equal (or not equal). Dimensions, 
hot`-spot, dots and all pixels are compared. Note that the comparison 
can be slow.&]
[s3; &]
[s4; &]
[s5;:Image`:`:GetHashValue`(`)const: [_^dword^ dword]_GetHashValue()[@(64) _][@(0.0.255) co
nst]&]
[s2;%% Returns the hash`-value for image. All pixels combined into 
hash value (potentially slow).&]
[s3; &]
[s4; &]
[s5;:Image`:`:ToString`(`)const: [_^String^ String]_ToString()[@(64) _][@(0.0.255) const]&]
[s2;%% Returns the basic Image informations as String.&]
[s3; &]
[s4; &]
[s5;:Image`:`:Serialize`(Stream`&`): [@(0.0.255) void]_Serialize([_^Stream^ Stream][@(0.0.255) `&
]_[@3 s])&]
[s2;%% Serializes Image.&]
[s3; &]
[s4; &]
[s5;:Image`:`:Clear`(`): [@(0.0.255) void]_Clear()&]
[s2;%% Assigns an empty Image.&]
[s3; &]
[s4; &]
[s5;:Image`:`:operator`=`(const Image`&`): [_^Image^ Image][@(0.0.255) `&]_operator`=([@(0.0.255) c
onst]_[_^Image^ Image][@(0.0.255) `&]_[@3 img])&]
[s2;%% Assigns another Image.&]
[s3; &]
[s4; &]
[s5;:Image`:`:operator`=`(ImageBuffer`&`): [_^Image^ Image][@(0.0.255) `&]_operator`=([_^ImageBuffer^ I
mageBuffer][@(0.0.255) `&]_[@3 img])&]
[s2;%% Assigns Image created in ImageBuffer. ImageBuffer is emptied 
by this operation.&]
[s3; &]
[s4; &]
[s5;:Image`:`:IsNullInstance`(`)const: [@(0.0.255) bool]_IsNullInstance()_[@(0.0.255) con
st]&]
[s2;%% Test whether Image has nonzero dimension.&]
[s3; &]
[s4; &]
[s5;:Image`:`:IsEmpty`(`)const: [@(0.0.255) bool]_IsEmpty()_[@(0.0.255) const]&]
[s2;%% Same as IsNullInstance.&]
[s3; &]
[s4; &]
[s5;:Image`:`:operator Value`(`)const: operator_Value()[@(64) _][@(0.0.255) const]&]
[s2;%% Converts Image to Value.&]
[s3; &]
[s0; &]
[ {{10000t/25b/25@1 [s0; [* Constructor Detail]]}}&]
[s3; &]
[s5;:Image`:`:Image`(`): Image()&]
[s3; &]
[s4; &]
[s5;:Image`:`:Image`(const Nuller`&`): Image([@(0.0.255) const]_[_^Nuller^ Nuller][@(0.0.255) `&
])&]
[s2;%% Constructs empty Image.&]
[s3; &]
[s4; &]
[s5;:Image`:`:Image`(const Value`&`): Image([@(0.0.255) const]_[_^Value^ Value][@(0.0.255) `&
]_[@3 src])&]
[s2;%% Converts Image from Value.&]
[s3; &]
[s4; &]
[s5;:Image`:`:Image`(const Image`&`): Image([@(0.0.255) const]_Image[@(0.0.255) `&]_[@3 img
])&]
[s2;%% Copy constructor.&]
[s3; &]
[s4; &]
[s5;:Image`:`:Image`(Image`(`*`)`(`)`): Image(Image_(`*fn)())&]
[s2;%% This function allow Image to be directly constructed from 
pointer to function returning the Image. This allows omitting 
parenthesis when passing Iml image constants as arguments.&]
[s3; &]
[s4; &]
[s5;:Image`:`:Image`(ImageBuffer`&`): Image([_^ImageBuffer^ ImageBuffer][@(0.0.255) `&]_[@3 b
])&]
[s2;%% Uses Image created in ImageBuffer. ImageBuffer is emptied 
by this operation.&]
[s3;%% &]
[s0;%% &]
[ {{10000t/25b/25@1 [s0;%% [* Standard cursors]]}}&]
[s9;%% Image class contains several standard mouse cursor Images 
as static member methods:&]
[s0;%% &]
[ {{3333:3333:3334f0;g0; [ {{10000>274; [s0; Arrow]
:: [s0; Wait]
:: [s0; IBeam]
:: [s0; No]
:: [s0; SizeAll]}}]
:: [ {{10000>274; [s0; SizeHorz]
:: [s0; SizeVert]
:: [s0; SizeTopLeft]
:: [s0; SizeTop]
:: [s0; SizeTopRight]}}]
:: [ {{10000>274; [s0; SizeLeft]
:: [s0; SizeRight]
:: [s0; SizeBottomLeft]
:: [s0; SizeBottom]
:: [s0; SizeBottomRight]}}]}}&]
[s3; &]
[s0;%% &]
[s0;%% &]
[s0; &]
[ {{10000@(113.42.0) [s0;%% [*@(229)4 Utility functions]]}}&]
[s4; &]
[s5;:WithHotSpots`(const Image`&`,int`,int`,int`,int`): [_^Image^ Image]_[* WithHotSpots](
[@(0.0.255) const]_[_^Image^ Image][@(0.0.255) `&]_[*@3 m], [@(0.0.255) int]_[*@3 x1], 
[@(0.0.255) int]_[*@3 y1], [@(0.0.255) int]_[*@3 x2], [@(0.0.255) int]_[*@3 y2])&]
[s2;%% Returns a new Image based in Image [%-*@3 m] with hotspots [%-*@3 x1,] 
[%-*@3 y1] and [%-*@3 x2,] [%-*@3 y2].&]
[s3;%% &]
[s4;%% &]
[s5;:WithHotSpot`(const Image`&`,int`,int`): [_^Image^ Image]_[* WithHotSpot]([@(0.0.255) c
onst]_[_^Image^ Image][@(0.0.255) `&]_[*@3 m], [@(0.0.255) int]_[*@3 x1], 
[@(0.0.255) int]_[*@3 y1])&]
[s2; [%% Returns a new Image based in Image ][*@3 m][%%  with hotspots 
][*@3 x1,][%%  ][*@3 y1.]&]
[s3;%% &]
[s4; &]
[s5;:CreateImage`(Size`,const RGBA`&`): [_^Image^ Image]_[* CreateImage]([_^Size^ Size]_[*@3 s
z], [@(0.0.255) const]_[_^RGBA^ RGBA][@(0.0.255) `&]_[*@3 rgba])&]
[s2;%% Creates a new Image with Size [%-*@3 sz] filled with color [%-*@3 rgba].&]
[s3;%% &]
[s4; &]
[s5;:CreateImage`(Size`,Color`): [_^Image^ Image]_[* CreateImage]([_^Size^ Size]_[*@3 sz], 
[_^Color^ Color]_[*@3 color])&]
[s2;%% Creates a new Image with Size [%-*@3 sz] filled with color [%-*@3 color].&]
[s3;%% &]
[s4; &]
[s5;:DstSrc`(ImageBuffer`&`,Point`&`,const Image`&`,Rect`&`): [_^Size^ Size]_[* DstSrc]([_^ImageBuffer^ I
mageBuffer][@(0.0.255) `&]_[*@3 dest], [_^Point^ Point][@(0.0.255) `&]_[*@3 p], 
[@(0.0.255) const]_[_^Image^ Image][@(0.0.255) `&]_[*@3 src], [_^Rect^ Rect][@(0.0.255) `&]_[*@3 s
r])&]
[s2;%% Calculates the minimum Size of Rect [%-*@3 sr] of Image [%-*@3 src] 
when trying to copy it into Point [%-*@3 p] of ImageBuffer [%-*@3 dest].&]
[s3;%% &]
[s4; &]
[s5;:DstSrcOp`(ImageBuffer`&`,Point`,const Image`&`,const Rect`&`,void`(`*`)`(RGBA`*t`,const RGBA`*s`,int n`)`): [@(0.0.255) v
oid]_[* DstSrcOp]([_^ImageBuffer^ ImageBuffer][@(0.0.255) `&]_[*@3 dest], 
[_^Point^ Point]_[*@3 p], [@(0.0.255) const]_[_^Image^ Image][@(0.0.255) `&]_[*@3 src], 
[@(0.0.255) const]_[_^Rect^ Rect][@(0.0.255) `&]_[*@3 srect], [@(0.0.255) void]_(`*[*@3 op])(
RGBA_`*t, [@(0.0.255) const]_RGBA_`*s, [@(0.0.255) int]_n))&]
[s2;%% Copies Rect [%-*@3 srect] of Image [%-*@3 src] into Point [%-*@3 p] 
of ImageBuffer [%-*@3 dest] using copying function [%-*@3 op].&]
[s3;%% &]
[s4; &]
[s5;:Copy`(ImageBuffer`&`,Point`,const Image`&`,const Rect`&`): [@(0.0.255) void]_[* Copy
]([_^ImageBuffer^ ImageBuffer][@(0.0.255) `&]_[*@3 dest], [_^Point^ Point]_[*@3 p], 
[@(0.0.255) const]_[_^Image^ Image][@(0.0.255) `&]_[*@3 src], [@(0.0.255) const]_[_^Rect^ Rec
t][@(0.0.255) `&]_[*@3 srect])&]
[s2;%% Copies Rect [%-*@3 srect] of Image [%-*@3 src] into Point [%-*@3 p] 
of ImageBuffer [%-*@3 dest].&]
[s3;%% &]
[s4; &]
[s5;:Fill`(ImageBuffer`&`,const Rect`&`,RGBA`): [@(0.0.255) void]_[* Fill]([_^ImageBuffer^ I
mageBuffer][@(0.0.255) `&]_[*@3 dest], [@(0.0.255) const]_[_^Rect^ Rect][@(0.0.255) `&]_[*@3 r
ect], [_^RGBA^ RGBA]_[*@3 color])&]
[s2;%% Fills rectangular are [%-*@3 rect] with [%-*@3 color] .&]
[s3;%% &]
[s4; &]
[s5;:Copy`(Image`&`,Point`,const Image`&`,const Rect`&`): [@(0.0.255) void]_[* Copy]([_^Image^ I
mage][@(0.0.255) `&]_[*@3 dest], [_^Point^ Point]_[*@3 p], [@(0.0.255) const]_[_^Image^ Image
][@(0.0.255) `&]_[*@3 src], [@(0.0.255) const]_[_^Rect^ Rect][@(0.0.255) `&]_[*@3 srect])&]
[s2;%% Copies Rect [%-*@3 srect] of Image [%-*@3 src] into Point [%-*@3 p] 
of Image [%-*@3 dest].&]
[s3;%% &]
[s4; &]
[s5;:Over`(ImageBuffer`&`,Point`,const Image`&`,const Rect`&`): [@(0.0.255) void]_[* Over
]([_^ImageBuffer^ ImageBuffer][@(0.0.255) `&]_[*@3 dest], [_^Point^ Point]_[*@3 p], 
[@(0.0.255) const]_[_^Image^ Image][@(0.0.255) `&]_[*@3 src], [@(0.0.255) const]_[_^Rect^ Rec
t][@(0.0.255) `&]_[*@3 srect])&]
[s2;%% Mixes using alpha information Rect [%-*@3 srect] of Image [%-*@3 src] 
into Point [%-*@3 p] of ImageBuffer [%-*@3 dest].&]
[s3;%% &]
[s4; &]
[s5;:Over`(Image`&`,Point`,const Image`&`,const Rect`&`): [@(0.0.255) void]_[* Over]([_^Image^ I
mage][@(0.0.255) `&]_[*@3 dest], [_^Point^ Point]_[*@3 p], [@(0.0.255) const]_[_^Image^ Image
][@(0.0.255) `&]_[*@3 src], [@(0.0.255) const]_[_^Rect^ Rect][@(0.0.255) `&]_[*@3 srect])&]
[s2;%% Mixes using alpha information Rect [%-*@3 srect] of Image [%-*@3 src] 
into Point [%-*@3 p] of Image [%-*@3 dest].&]
[s3;%% &]
[s4; &]
[s5;:Fill`(Image`&`,const Rect`&`,RGBA`): [@(0.0.255) void]_[* Fill]([_^Image^ Image][@(0.0.255) `&
]_[*@3 dest], [@(0.0.255) const]_[_^Rect^ Rect][@(0.0.255) `&]_[*@3 rect], 
[_^RGBA^ RGBA]_[*@3 color])&]
[s2;%% Fills rectangular are [%-*@3 rect] with [%-*@3 color].&]
[s3;%% &]
[s4; &]
[s5;:OverStraightOpaque`(ImageBuffer`&`,Point`,const Image`&`,const Rect`&`): [@(0.0.255) v
oid]_[* OverStraightOpaque]([_^ImageBuffer^ ImageBuffer][@(0.0.255) `&]_[*@3 dest], 
[_^Point^ Point]_[*@3 p], [@(0.0.255) const]_[_^Image^ Image][@(0.0.255) `&]_[*@3 src], 
[@(0.0.255) const]_[_^Rect^ Rect][@(0.0.255) `&]_[*@3 srect])&]
[s2;%% Mixes using alpha information Rect [%-*@3 srect] of Image [%-*@3 src] 
into Point [%-*@3 p] of ImageBuffer [%-*@3 dest].&]
[s2;%% The alpha channel of [%-*@3 dest] is 255 (opaque).&]
[s3;%% &]
[s4; &]
[s5;:OverStraightOpaque`(Image`&`,Point`,const Image`&`,const Rect`&`): [@(0.0.255) voi
d]_[* OverStraightOpaque]([_^Image^ Image][@(0.0.255) `&]_[*@3 dest], 
[_^Point^ Point]_[*@3 p], [@(0.0.255) const]_[_^Image^ Image][@(0.0.255) `&]_[*@3 `_src], 
[@(0.0.255) const]_[_^Rect^ Rect][@(0.0.255) `&]_[*@3 srect])&]
[s2;%% Mixes using alpha information Rect [%-*@3 srect] of Image [%-*@3 src] 
into Point [%-*@3 p] of Image [%-*@3 dest].&]
[s2;%% The alpha channel of [%-*@3 dest] is 255 (opaque).&]
[s3;%% &]
[s4; &]
[s5;:Crop`(const Image`&`,const Rect`&`): [_^Image^ Image]_[* Crop]([@(0.0.255) const]_[_^Image^ I
mage][@(0.0.255) `&]_[*@3 img], [@(0.0.255) const]_[_^Rect^ Rect][@(0.0.255) `&]_[*@3 rc])&]
[s2;%% Returns a subimage from [%-*@3 img] defined by Rect [%-*@3 rc].&]
[s3;%% &]
[s4; &]
[s5;:Crop`(const Image`&`,int`,int`,int`,int`): [_^Image^ Image]_[* Crop]([@(0.0.255) const
]_[_^Image^ Image][@(0.0.255) `&]_[*@3 img], [@(0.0.255) int]_[*@3 x], 
[@(0.0.255) int]_[*@3 y], [@(0.0.255) int]_[*@3 cx], [@(0.0.255) int]_[*@3 cy])&]
[s2;%% Returns a subimage from [%-*@3 img] defined by origin [%-*@3 x,] 
[%-*@3 y], width [%-*@3 cx] and height [%-*@3 cy].&]
[s3;%% &]
[s4; &]
[s5;:AutoCrop`(const Image`&`,RGBA`): [_^Image^ Image]_[* AutoCrop]([@(0.0.255) const]_[_^Image^ I
mage][@(0.0.255) `&]_[*@3 m], [_^RGBA^ RGBA]_[*@3 c])&]
[s2;%% Detects rectangular margin with uniform color [%-*@3 c] and 
then crops this margin out.&]
[s3;%% &]
[s4; &]
[s5;:ColorMask`(const Image`&`,Color`): [_^Image^ Image]_[* ColorMask]([@(0.0.255) const]_[_^Image^ I
mage][@(0.0.255) `&]_[*@3 src], [_^Color^ Color]_[*@3 transparent])&]
[s2;%% Returns a new Image based on [%-*@3 src] replaced Color [%-*@3 transparent] 
with RGBAZero().&]
[s3;%% &]
[s4; &]
[s5;:CanvasSize`(const Image`&`,int`,int`): [_^Image^ Image]_[* CanvasSize]([@(0.0.255) con
st]_[_^Image^ Image][@(0.0.255) `&]_[*@3 img], [@(0.0.255) int]_[*@3 cx], 
[@(0.0.255) int]_[*@3 cy])&]
[s2;%%  [%-*@3 img] [%-*@3 cx] [%-*@3 cy].&]
[s3;%% &]
[s4; &]
[s5;:AssignAlpha`(const Image`&`,const Image`&`): [_^Image^ Image]_[* AssignAlpha]([@(0.0.255) c
onst]_[_^Image^ Image][@(0.0.255) `&]_[*@3 img], [@(0.0.255) const]_[_^Image^ Image][@(0.0.255) `&
]_[*@3 new`_alpha])&]
[s2;%% Returns a new Image based on [%-*@3 img] colors and [%-*@3 new`_alpha] 
alpha.&]
[s2;%% Image size is the minimum of [%-*@3 img] and [%-*@3 new`_alpha].&]
[s3;%% &]
[s4; &]
[s5;:Equalight`(const Image`&`,int`): [_^Image^ Image]_[* Equalight]([@(0.0.255) const]_[_^Image^ I
mage][@(0.0.255) `&]_[*@3 img], [@(0.0.255) int]_[*@3 thold]_`=_[@3 10])&]
[s2;%%  [%-*@3 img] [%-*@3 thold].&]
[s3;%% &]
[s4; &]
[s5;:Grayscale`(const Image`&`): [_^Image^ Image]_[* Grayscale]([@(0.0.255) const]_[_^Image^ I
mage][@(0.0.255) `&]_[*@3 img])&]
[s2;%% Returns a new Image including [%-*@3 img] converted to grayscale.&]
[s3;%% &]
[s4; &]
[s5;:Grayscale`(const Image`&`,int`): [_^Image^ Image]_[* Grayscale]([@(0.0.255) const]_[_^Image^ I
mage][@(0.0.255) `&]_[*@3 img], [@(0.0.255) int]_[*@3 amount])&]
[s2;%%  [%-*@3 img] [%-*@3 amount].&]
[s3;%% &]
[s4; &]
[s5;:Colorize`(const Image`&`,Color`,int`): [_^Image^ Image]_[* Colorize]([@(0.0.255) const
]_[_^Image^ Image][@(0.0.255) `&]_[*@3 img], [_^Color^ Color]_[*@3 color], 
[@(0.0.255) int]_[*@3 alpha]_`=_[@3 100])&]
[s2;%%  [%-*@3 img] [%-*@3 color] [%-*@3 alpha].&]
[s3;%% &]
[s4; &]
[s5;:Contrast`(const Image`&`,int`): [_^Image^ Image]_[* Contrast]([@(0.0.255) const]_[_^Image^ I
mage][@(0.0.255) `&]_[*@3 img], [@(0.0.255) int]_[*@3 amount]_`=_[@3 256])&]
[s2;%% Returns a new Image with its contrast increased in [%-*@3 amount] 
based on [%-*@3 img].&]
[s3;%% &]
[s4; &]
[s5;:Rescale`(const Image`&`,Size`,const Rect`&`,Gate2`<int`,int`>`): [_^Image^ Image]_
[* Rescale]([@(0.0.255) const]_[_^Image^ Image][@(0.0.255) `&]_[*@3 src], 
[_^Size^ Size]_[*@3 sz], [@(0.0.255) const]_[_^Rect^ Rect][@(0.0.255) `&]_[*@3 src`_rc], 
[_^Gate2^ Gate2]<[@(0.0.255) int], [@(0.0.255) int]>_[*@3 progress]_`=_[@(0.0.255) false])&]
[s2; [%% Returns a new Image rescaled to Size ][*@3 sz][%%  based on Rect 
][*@3 src`_rc ][%% of Image ][*@3 src]&]
[s3;%% &]
[s4; &]
[s5;:Sharpen`(const Image`&`,int`): [_^Image^ Image]_[* Sharpen]([@(0.0.255) const]_[_^Image^ I
mage][@(0.0.255) `&]_[*@3 img], [@(0.0.255) int]_[*@3 amount]_`=_[@3 100])&]
[s2;%% Sharpens image using primitive convolution filter.&]
[s3;%% &]
[s4; &]
[s5;:Dither`(const Image`&`,int`): [_^Image^ Image]_[* Dither]([@(0.0.255) const]_[_^Image^ I
mage][@(0.0.255) `&]_[*@3 m], [@(0.0.255) int]_[*@3 dival]_`=_[@3 394])&]
[s2;%% Provides primitive dithering with 8x8 matrix into black/white 
picture. Dival is sort of gamma control of output, lowering it 
from default value (to e.g. 350) can produce better results when 
dithering is using for printing on purely monochromatic (without 
half`-toning support) printers.&]
[s3;%% &]
[s4; &]
[s5;:Etched`(const Image`&`): [_^Image^ Image]_[* Etched]([@(0.0.255) const]_[_^Image^ Image][@(0.0.255) `&
]_[*@3 img])&]
[s2;%% `"Etching`" effect.&]
[s3;%% &]
[s4; &]
[s5;:SetColorKeepAlpha`(const Image`&`,Color`): [_^Image^ Image]_[* SetColorKeepAlpha]([@(0.0.255) c
onst]_[_^Image^ Image][@(0.0.255) `&]_[*@3 img], [_^Color^ Color]_[*@3 c])&]
[s2;%% Returns a new Image based on [%-*@3 img] alpha and [%-*@3 c] Color.&]
[s3;%% &]
[s4; &]
[s5;:RotateClockwise`(const Image`&`): [_^Image^ Image]_[* RotateClockwise]([@(0.0.255) con
st]_[_^Image^ Image][@(0.0.255) `&]_[*@3 img])&]
[s2;%% Returns a new[%-*@3  ]Image rotated 90º (clockwise) that is 
based on [%-*@3 img].&]
[s3;%% &]
[s4; &]
[s5;:RotateAntiClockwise`(const Image`&`): [_^Image^ Image]_[* RotateAntiClockwise]([@(0.0.255) c
onst]_[_^Image^ Image][@(0.0.255) `&]_[*@3 img])&]
[s2;%% Returns a new Image[%-*@3  ]rotated `-90º (anticlockwise) that 
is based on [%-*@3 img].&]
[s3;%% &]
[s4; &]
[s5;:Rotate180`(const Image`&`): [_^Image^ Image]_[* Rotate180]([@(0.0.255) const]_[_^Image^ I
mage][@(0.0.255) `&]_[*@3 orig])&]
[s2;%% Returns a new Image[%-*@3  ]rotated 180º that is based on [%-*@3 img].&]
[s3;%% &]
[s4; &]
[s5;:MirrorHorz`(const Image`&`): [_^Image^ Image]_[* MirrorHorz]([@(0.0.255) const]_[_^Image^ I
mage][@(0.0.255) `&]_[*@3 img])&]
[s2;%% Returns a new Image that is a mirror of [%-*@3 img ]following 
horizontal axis.&]
[s3;%% &]
[s4; &]
[s5;:MirrorVert`(const Image`&`): [_^Image^ Image]_[* MirrorVert]([@(0.0.255) const]_[_^Image^ I
mage][@(0.0.255) `&]_[*@3 img])&]
[s2;%% Returns a new Image that is a mirror of [%-*@3 img ]following 
vertical axis.&]
[s3;%% &]
[s4;%% &]
[s5;:Rotate`(const Image`&`,int`): [_^Image^ Image]_[* Rotate]([@(0.0.255) const]_[_^Image^ I
mage][@(0.0.255) `&]_[*@3 m], [@(0.0.255) int]_[*@3 angle])&]
[s2;%% Returns a new Image based on Image [%-*@3 m] rotated [%-*@3 angle] 
by its center where the full rotation (2[%- π) ]is 3600.&]
[s3;%% &]
[s3; ]]