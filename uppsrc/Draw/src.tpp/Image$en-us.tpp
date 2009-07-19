topic "Image and ImageBuffer";
[2 $$0,0#00000000000000000000000000000000:Default]
[i448;a25;kKO9;*@(64)2 $$1,0#37138531426314131252341829483380:class]
[l288;2 $$2,2#27521748481378242620020725143825:desc]
[a83;*R6 $$3,0#31310162474203024125188417583966:caption]
[l288;i1121;b17;O9;~~~.1408;2 $$4,0#10431211400427159095818037425705:param]
[i448;a25;kKO9;*@(64)2 $$5,0#37138531426314131252341829483370:item]
[*+117 $$6,6#14700283458701402223321329925657:header]
[0 $$7,0#96390100711032703541132217272105:end]
[H6;0 $$8,0#05600065144404261032431302351956:begin]
[{_} 
[ {{10000@(113.42.0) [s0;%% [*@7;4 Image]]}}&]
[s1;@(0.0.255) &]
[s1;:Image`:`:class: [@(0.0.255) class][@0 _Image_:_][@(0.0.255) public][@0 _][@3 AssignValueTyp
eNo][@0 <_Image, ][@3 150][@0 , ][_^Moveable^@0 Moveable][@0 <Image>_>_]&]
[s2;%% Image represents an immutable image value. Image can be directly 
painted to Draw. To create or change Image, use [^`:`:ImageBuffer^ ImageBuffer].&]
[s2;%% Image has low`-cost constant time deep copy. It is moveable 
and Rich`-Value compatible type.&]
[s2;%% U`+`+ expects pixels to be in premultiplied alpha format.&]
[s0; &]
[ {{10000F(128)G(128)@1 [s0;%% [* Public Member List]]}}&]
[s0; &]
[s5;:Image`:`:operator`~`(`)const: [@(0.0.255) const][@0 _][_^RGBA^@0 RGBA][@(0.0.255) `*][@0 _
operator`~()_][@(0.0.255) const]&]
[s7; &]
[s8; &]
[s5;:Image`:`:operator const RGBA`*`(`)const: operator_const_RGBA`*()_[@(0.0.255) const
]&]
[s2;%% Returns a pointer to Image pixels.&]
[s7; &]
[s8; &]
[s5;:Image`:`:operator`[`]`(int`)const: [@(0.0.255) const][@0 _][_^RGBA^@0 RGBA][@(0.0.255) `*
][@0 _operator`[`](][@(0.0.255) int][@0 _][@3 i][@0 )_][@(0.0.255) const]&]
[s2;%% Returns a pointer to the first pixel in the line [%-*@3 i].&]
[s7; &]
[s8; &]
[s5;:Image`:`:GetSize`(`)const: [_^Size^ Size]_GetSize()_[@(0.0.255) const]&]
[s2;%% Returns the dimension of Image.&]
[s7; &]
[s8; &]
[s5;:Image`:`:GetWidth`(`)const: [@(0.0.255) int][@0 _GetWidth()_][@(0.0.255) const]&]
[s2;%% Same as GetSize().cx.&]
[s7; &]
[s8; &]
[s5;:Image`:`:GetHeight`(`)const: [@(0.0.255) int][@0 _GetHeight()_][@(0.0.255) const]&]
[s2;%% Same as GetSize().cy.&]
[s7; &]
[s8; &]
[s5;:Image`:`:GetLength`(`)const: [@(0.0.255) int][@0 _GetLength()_][@(0.0.255) const]&]
[s2;%% Number of pixels in Image `- GetWidth() `* GetHeight().&]
[s7; &]
[s8; &]
[s5;:Image`:`:GetHotSpot`(`)const: [_^Point^@0 Point][@0 _GetHotSpot()]_[@(0.0.255) const]&]
[s2;%% Returns the reference point.&]
[s7; &]
[s8; &]
[s5;:Image`:`:Get2ndSpot`(`)const: [_^Point^ Point]_Get2ndSpot()_[@(0.0.255) const]&]
[s2;%% Returns secondary reference point.&]
[s7; &]
[s8; &]
[s5;:Image`:`:GetDots`(`)const: [_^Size^@0 Size][@0 _GetDots()]_[@(0.0.255) const]&]
[s2;%% Gets the physical size of Image. If physical size is not set, 
returns Size(0, 0).&]
[s7; &]
[s8; &]
[s5;:Image`:`:GetKindNoScan`(`)const: [@(0.0.255) int][@0 _GetKindNoScan()_][@(0.0.255) con
st]&]
[s2;%% Returns the kind of image. Unlike GetKind(), it does not perform 
the scan of image if the kind it not know, therefore it can return 
IMAGE`_UNKNOWN.&]
[s7; &]
[s8; &]
[s5;:Image`:`:GetKind`(`)const: [@(0.0.255) int][@0 _GetKind()_][@(0.0.255) const]&]
[s2;%% Returns the kind of image, either set by ImageBuffer`::SetKind 
or (if it is IMAGE`_UNKNOWN) resolved by scanning pixels. If 
the kind is IMAGE`_UNKNOWN, the pixels are scanned and the type 
is determined, it is then stored (in private Image instance variable) 
for future use and returned.&]
[s7; &]
[s8; &]
[s5;:Image`:`:GetSerialId`(`)const: [_^int64^@0 int64][@0 _GetSerialId()]_[@(0.0.255) const
]&]
[s2;%% Returns the unique, per application run, identifier of Image. 
All Images with the same serial id can be considered equal (this 
is useful for caching images).&]
[s7; &]
[s8; &]
[s5;:Image`:`:IsSame`(const Image`&`)const: [@(0.0.255) bool][@0 _IsSame(][@(0.0.255) const
][@0 _][_^Image^@0 Image][@(0.0.255) `&][@0 _][@3 img][@0 )_][@(0.0.255) const]&]
[s2;%% Same as GetSerialId() `=`= img.GetSerialId().&]
[s7; &]
[s8; &]
[s5;:Image`:`:operator`=`=`(const Image`&`)const: [@(0.0.255) bool][@0 _operator`=`=(][@(0.0.255) c
onst][@0 _][_^Image^@0 Image][@(0.0.255) `&][@0 _][@3 img][@0 )_][@(0.0.255) const]&]
[s7; &]
[s8; &]
[s5;:Image`:`:operator`!`=`(const Image`&`)const: [@(0.0.255) bool][@0 _operator!`=(][@(0.0.255) c
onst][@0 _][_^Image^@0 Image][@(0.0.255) `&][@0 _][@3 img][@0 )_][@(0.0.255) const]&]
[s2;%% Tests whether two Images are equal (or not equal). Dimensions, 
hot`-spot, dots and all pixels are compared. Note that the comparison 
can be slow.&]
[s7; &]
[s8; &]
[s5;:Image`:`:GetHashValue`(`)const: [_^dword^@0 dword][@0 _GetHashValue()]_[@(0.0.255) con
st]&]
[s2;%% Returns the hash`-value for image. All pixels combined into 
hash value (potentially slow).&]
[s7; &]
[s8; &]
[s5;:Image`:`:ToString`(`)const: [_^String^@0 String][@0 _ToString()]_[@(0.0.255) const]&]
[s2;%% Returns the basic Image informations as String.&]
[s7; &]
[s8; &]
[s5;:Image`:`:Serialize`(Stream`&`): [@(0.0.255) void][@0 _Serialize(][_^Stream^@0 Stream][@(0.0.255) `&
][@0 _][@3 s][@0 )]&]
[s2;%% Serializes Image.&]
[s7; &]
[s8; &]
[s5;:Image`:`:Clear`(`): [@(0.0.255) void][@0 _Clear()]&]
[s2;%% Assigns an empty Image.&]
[s7; &]
[s8; &]
[s5;:Image`:`:operator`=`(const Image`&`): [_^Image^@0 Image][@(0.0.255) `&][@0 _operator`=
(][@(0.0.255) const][@0 _][_^Image^@0 Image][@(0.0.255) `&][@0 _][@3 img][@0 )]&]
[s2;%% Assigns another Image.&]
[s7; &]
[s8; &]
[s5;:Image`:`:operator`=`(ImageBuffer`&`): [_^Image^@0 Image][@(0.0.255) `&][@0 _operator`=
(][_^ImageBuffer^@0 ImageBuffer][@(0.0.255) `&][@0 _][@3 img][@0 )]&]
[s2;%% Assigns Image created in ImageBuffer. ImageBuffer is emptied 
by this operation.&]
[s7; &]
[s8; &]
[s5;:Image`:`:IsNullInstance`(`)const: [@(0.0.255) bool][@0 _IsNullInstance()_][@(0.0.255) c
onst]&]
[s2;%% Test whether Image has nonzero dimension.&]
[s7; &]
[s8; &]
[s5;:Image`:`:IsEmpty`(`)const: [@(0.0.255) bool][@0 _IsEmpty()_][@(0.0.255) const]&]
[s2;%% Same as IsNullInstance.&]
[s7; &]
[s8; &]
[s5;:Image`:`:operator Value`(`)const: [@0 operator_Value()]_[@(0.0.255) const]&]
[s2;%% Converts Image to Value.&]
[s7; &]
[ {{10000t/25b/25@1 [s0; [* Constructor Detail]]}}&]
[s7; &]
[s5;:Image`:`:Image`(`): [@0 Image()]&]
[s7; &]
[s8; &]
[s5;:Image`:`:Image`(const Nuller`&`): [@0 Image(][@(0.0.255) const][@0 _][_^Nuller^@0 Nuller
][@(0.0.255) `&][@0 )]&]
[s2;%% Constructs empty Image.&]
[s7; &]
[s8; &]
[s5;:Image`:`:Image`(const Value`&`): [@0 Image(][@(0.0.255) const][@0 _][_^Value^@0 Value][@(0.0.255) `&
][@0 _][@3 src][@0 )]&]
[s2;%% Converts Image from Value.&]
[s7; &]
[s8; &]
[s5;:Image`:`:Image`(const Image`&`): [@0 Image(][@(0.0.255) const][@0 _Image][@(0.0.255) `&][@0 _
][@3 img][@0 )]&]
[s2;%% Copy constructor.&]
[s7; &]
[s8; &]
[s5;:Image`:`:Image`(Image`(`*`)`(`)`): [@0 Image(Image_(`*fn)())]&]
[s2;%% This function allow Image to be directly constructed from 
pointer to function returning the Image. This allows omitting 
parenthesis when passing Iml image constants as arguments.&]
[s7; &]
[s8; &]
[s5;:Image`:`:Image`(ImageBuffer`&`): [@0 Image(][_^ImageBuffer^@0 ImageBuffer][@(0.0.255) `&
][@0 _][@3 b][@0 )]&]
[s2;%% Uses Image created in ImageBuffer. ImageBuffer is emptied 
by this operation.&]
[s2;%% &]
[s7; &]
[ {{10000t/25b/25@1 [s0;%% [* Standard cursors]]}}&]
[s0;%% &]
[s0;%% Image class contains several standard mouse cursor Images 
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
[s0; ]