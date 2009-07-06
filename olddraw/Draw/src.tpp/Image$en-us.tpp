topic "Image and ImageBuffer";[2 $$0,0#00000000000000000000000000000000:Default]
[i448;a25;kKO9;*@(64)2 $$1,0#37138531426314131252341829483380:class]
[l288;2 $$2,2#27521748481378242620020725143825:desc]
[a83;*R6 $$3,0#31310162474203024125188417583966:caption]
[l288;i1121;b17;O9;~~~.1408;2 $$4,0#10431211400427159095818037425705:param]
[i448;a25;kKO9;*@(64)2 $$5,0#37138531426314131252341829483370:item]
[*+117 $$6,6#14700283458701402223321329925657:header]
[{_}%EN-US 
[s3; Image and ImageBuffer&]
[s1;K:`:`:Image`:`:class:%- [@(0.0.255) class]_[@0 Image]_:_[@(0.0.255) public]_[^`:`:AssignValueTypeNo^ A
ssignValueTypeNo]<_[@0 Image], [@3 150], [^`:`:Moveable^ Moveable]<[@0 Image]>_>_&]
[s2; Image represents an immutable image value. Image can be directly 
painted to Draw. To create or change Image, use [^`:`:ImageBuffer^ ImageBuffer].&]
[s2; Image has low`-cost constant time deep copy. It is moveable 
and Rich`-Value compatible type.&]
[s2; U`+`+ expects pixels to be in premultiplied alpha format.&]
[s0; &]
[s5;K:`:`:Image`:`:operator`~`(`)const:%- [@(0.0.255) const]_[^`:`:RGBA^ RGBA]`*_[@0 operat
or`~]()_[@(0.0.255) const]&]
[s5;%- [@0 operator]_[@(0.0.255) const]_[^`:`:RGBA^ RGBA][@0 `*]()_[@(0.0.255) const]&]
[s2; Returns a pointer to Image pixels.&]
[s0; &]
[s5;K:`:`:Image`:`:operator`[`]`(int`)const:%- [@(0.0.255) const]_[^`:`:RGBA^ RGBA]`*_[@0 o
perator`[`]]([@(0.0.255) int]_[@3 i])_[@(0.0.255) const]&]
[s2; Returns a pointer to the first pixel in the line [%-*@3 i].&]
[s0; &]
[s5;K:`:`:Image`:`:GetSize`(`)const:%- [^`:`:Size^ Size]_[@0 GetSize]()_[@(0.0.255) const]&]
[s2; Returns the dimension of Image.&]
[s0; &]
[s5;K:`:`:Image`:`:GetWidth`(`)const:%- [@(0.0.255) int]_[@0 GetWidth]()_[@(0.0.255) const]&]
[s2; Same as GetSize().cx.&]
[s0; &]
[s5;K:`:`:Image`:`:GetHeight`(`)const:%- [@(0.0.255) int]_[@0 GetHeight]()_[@(0.0.255) cons
t]&]
[s2; Same as GetSize().cy.&]
[s0; &]
[s5;K:`:`:Image`:`:GetLength`(`)const:%- [@(0.0.255) int]_[@0 GetLength]()_[@(0.0.255) cons
t]&]
[s2; Number of pixels in Image `- GetWidth() `* GetHeight().&]
[s0; &]
[s5;K:`:`:Image`:`:GetHotSpot`(`)const:%- [^`:`:Point^ Point]_[@0 GetHotSpot]()_[@(0.0.255) c
onst]&]
[s2; Returns the reference point.&]
[s0; &]
[s5;K:`:`:Image`:`:GetDots`(`)const:%- [^`:`:Size^ Size]_[@0 GetDots]()_[@(0.0.255) const]&]
[s2; Gets the physical size of Image. If physical size is not set, 
returns Size(0, 0).&]
[s0; &]
[s5;K:`:`:Image`:`:GetKind`(`)const:%- [@(0.0.255) int]_[@0 GetKind]()_[@(0.0.255) const]&]
[s2; Returns the kind of image. See [^`:`:ImageBuffer^ ImageBuffer] 
for detail.&]
[s0; &]
[s5;K:`:`:Image`:`:GetSerialId`(`)const:%- [^`:`:int64^ int64]_[@0 GetSerialId]()_[@(0.0.255) c
onst]&]
[s2; Returns the unique, per application run, identifier of Image. 
All Images with the same serial id can be considered equal (this 
is useful for caching images).&]
[s0; &]
[s5;K:`:`:Image`:`:IsSame`(const`:`:Image`&`)const:%- [@(0.0.255) bool]_[@0 IsSame]([@(0.0.255) c
onst]_[^`:`:Image^ Image]`&_[@3 img])_[@(0.0.255) const]&]
[s2; Same as GetSerialId() `=`= img.GetSerialId().&]
[s0; &]
[s5;K:`:`:Image`:`:operator`=`=`(const`:`:Image`&`)const:%- [@(0.0.255) bool]_[@0 operato
r`=`=]([@(0.0.255) const]_[^`:`:Image^ Image]`&_[@3 img])_[@(0.0.255) const]&]
[s5;K:`:`:Image`:`:operator`=`=`(const`:`:Image`&`)const:%- [@(0.0.255) bool]_[@0 operato
r!`=]([@(0.0.255) const]_[^`:`:Image^ Image]`&_[@3 img])_[@(0.0.255) const]&]
[s2; Tests whether two Images are equal (or not equal). Dimensions, 
hot`-spot, dots and all pixels are compared. Note that the comparison 
can be slow.&]
[s0; &]
[s5;K:`:`:Image`:`:GetHashValue`(`)const:%- [^`:`:dword^ dword]_[@0 GetHashValue]()_[@(0.0.255) c
onst]&]
[s2; Returns the hash`-value for image. All pixels combined into 
hash value (potentially slow).&]
[s0; &]
[s5;K:`:`:Image`:`:ToString`(`)const:%- [^`:`:String^ String]_[@0 ToString]()_[@(0.0.255) c
onst]&]
[s2; Returns the basic Image informations as String.&]
[s0; &]
[s5;K:`:`:Image`:`:Serialize`(`:`:Stream`&`):%- [@(0.0.255) void]_[@0 Serialize]([^`:`:Stream^ S
tream]`&_[@3 s])&]
[s2; Serializes Image.&]
[s0; &]
[s5;K:`:`:Image`:`:Clear`(`):%- [@(0.0.255) void]_[@0 Clear]()&]
[s2; Assigns an empty Image.&]
[s0; &]
[s5;K:`:`:Image`:`:operator`=`(const`:`:Image`&`):%- [^`:`:Image^ Image]`&_[@0 operator`=
]([@(0.0.255) const]_[^`:`:Image^ Image]`&_[@3 img])&]
[s2; Assigns another Image.&]
[s0; &]
[s5;K:`:`:Image`:`:operator`=`(`:`:ImageBuffer`&`):%- [^`:`:Image^ Image]`&_[@0 operator`=
]([^`:`:ImageBuffer^ ImageBuffer]`&_[@3 img])&]
[s2; Assigns Image created in ImageBuffer. ImageBuffer is emptied 
by this operation.&]
[s0; &]
[s5;K:`:`:Image`:`:IsNullInstance`(`)const:%- [@(0.0.255) bool]_[@0 IsNullInstance]()_[@(0.0.255) c
onst]&]
[s2; Test whether Image has nonzero dimension.&]
[s0; &]
[s5;K:`:`:Image`:`:IsEmpty`(`)const:%- [@(0.0.255) bool]_[@0 IsEmpty]()_[@(0.0.255) const]&]
[s2; Same as IsNullInstance.&]
[s0; &]
[s5;K:`:`:Image`:`:operator`:`:Value`(`)const:%- [@0 operator_][^`:`:Value^@0 Value]()_[@(0.0.255) c
onst]&]
[s2; Converts Image to Value.&]
[s0; &]
[s5;K:`:`:Image`:`:`:`:Image`(`):%- [@0 Image]()&]
[s5;%- [@0 Image]([@(0.0.255) const]_[^`:`:Nuller^ Nuller]`&)&]
[s2; Constructs empty Image.&]
[s0; &]
[s5;K:`:`:Image`:`:`:`:Image`(const`:`:Value`&`):%- [@0 Image]([@(0.0.255) const]_[^`:`:Value^ V
alue]`&_[@3 src])&]
[s2; Converts Image from Value.&]
[s0; &]
[s5;K:`:`:Image`:`:`:`:Image`(const`:`:Image`&`):%- [@0 Image]([@(0.0.255) const]_[@0 Image
]`&_[@3 img])&]
[s2; Copy constructor.&]
[s0; &]
[s5;K:`:`:Image`:`:`:`:Image`(`:`:Image`(`*`)`(`)`):%- [@0 Image]([@0 Image]_(`*[@3 fn])())
&]
[s2; This function allow Image to be directly constructed from pointer 
to function returning the Image. This allows omitting parenthesis 
when passing Iml image constants as arguments.&]
[s0; &]
[s5;K:`:`:Image`:`:`:`:Image`(`:`:ImageBuffer`&`):%- [@0 Image]([^`:`:ImageBuffer^ ImageB
uffer]`&_[@3 b])&]
[s2; Uses Image created in ImageBuffer. ImageBuffer is emptied by 
this operation.&]
[s0; &]
[s5;K:`:`:Image`:`:`~`:`:Image`(`):%- `~[@0 Image]()&]
[s2; Destructor.&]
[s0; &]
[s0; &]
[s0; [3 Standard cursors]&]
[s0; &]
[s0; Image class contains several standard mouse cursor Images as 
static member methods:&]
[s0;^`:`:Image^ &]
[ {{3333:3333:3334f0;g0; [ {{6850:3150>274; [s0;%- Arrow]
:: [s0; ]
:: [s0;%- Wait]
:: [s0; ]
:: [s0;%- IBeam]
:: [s0; ]
:: [s0;%- No]
:: [s0; ]
:: [s0;%- SizeAll]
:: [s0; ]}}]
:: [ {{7129:2871>274; [s0;%- SizeHorz]
:: [s0; ]
:: [s0;%- SizeVert]
:: [s0; ]
:: [s0;%- SizeTopLeft]
:: [s0; ]
:: [s0;%- SizeTop]
:: [s0; ]
:: [s0;%- SizeTopRight]
:: [s0; ]}}]
:: [ {{6956:3044>274; [s0;%- SizeLeft]
:: [s0; ]
:: [s0;%- SizeRight]
:: [s0; ]
:: [s0;%- SizeBottomLeft]
:: [s0; ]
:: [s0;%- SizeBottom]
:: [s0; ]
:: [s0;%- SizeBottomRight]
:: [s0; ]}}]}}&]
[s0;^`:`:Image^ &]
[s0;^`:`:Image^ &]
[s0; &]
[s1;K:`:`:ImageBuffer`:`:class:%- [@(0.0.255) class]_[@0 ImageBuffer]_:_[@(0.0.255) private]_
[^`:`:NoCopy^ NoCopy]&]
[s2; ImageBuffer represents a writable Image `- an array of RGBA 
pixels. ImageBuffer can be converted to Image in low`-const constant 
time while loosing its content and vice versa, Image can be converted 
to ImageBuffer again loosing its content.&]
[s2; &]
[s2; Content of Image / ImageBuffer can be classified to optimize 
drawing. Possible classifications are&]
[s2; &]
[ {{2913:7087<288;>640; [s0; [* IMAGE`_UNKNOWN]]
:: [s0; The image kind is unknown.]
:: [s0; [* IMAGE`_EMPTY]]
:: [s0; The image is empty (all alpha values are 0).]
:: [s0; [* IMAGE`_ALPHA]]
:: [s0; The image has alpha values different from 255 and 0.]
:: [s0; [* IMAGE`_MASK]]
:: [s0; The image has alpha values 0 or 255 only.]
:: [s0; [* IMAGE`_OPAQUE]]
:: [s0; The image has all alpha values 255.]}}&]
[s2; &]
[s2; Note that is the kind of image is unknown, painting routine 
determines it automatically by scanning pixels and stores the 
result.&]
[s2; &]
[s2; Pixels of image are organized in simple continual POD array, 
first pixel being top`-left.&]
[s2; &]
[s2; U`+`+ expects pixels to be in premultiplied alpha format.&]
[s0; &]
[s5;K:`:`:ImageBuffer`:`:SetKind`(int`):%- [@(0.0.255) void]_[@0 SetKind]([@(0.0.255) int]_
[@3 k])&]
[s2; Sets the kind of image. You can use this to avoid automatic 
detection.&]
[s0; &]
[s5;K:`:`:ImageBuffer`:`:GetKind`(`)const:%- [@(0.0.255) int]_[@0 GetKind]()_[@(0.0.255) co
nst]&]
[s2; Returns the kind of image set by SetKind.&]
[s0; &]
[s5;K:`:`:ImageBuffer`:`:ScanKind`(`)const:%- [@(0.0.255) int]_[@0 ScanKind]()_[@(0.0.255) c
onst]&]
[s2; Scans all RGBA pixels to determine Image kind.&]
[s0; &]
[s5;K:`:`:ImageBuffer`:`:GetScanKind`(`)const:%- [@(0.0.255) int]_[@0 GetScanKind]()_[@(0.0.255) c
onst]&]
[s2; If kind set by SetKind is other than IMAGE`_UNKNOWN, returns 
it, otherwise calls ScanKind().&]
[s0; &]
[s5;K:`:`:ImageBuffer`:`:SetHotSpot`(`:`:Point`):%- [@(0.0.255) void]_[@0 SetHotSpot]([^`:`:Point^ P
oint]_[@3 p])&]
[s2; Sets the reference point. This point is e.g. used as hot`-spot 
when Image is used as mouse pointer.&]
[s0; &]
[s5;K:`:`:ImageBuffer`:`:GetHotSpot`(`)const:%- [^`:`:Point^ Point]_[@0 GetHotSpot]()_[@(0.0.255) c
onst]&]
[s2; Returns the reference point.&]
[s0; &]
[s5;K:`:`:ImageBuffer`:`:SetDots`(`:`:Size`):%- [@(0.0.255) void]_[@0 SetDots]([^`:`:Size^ S
ize]_[@3 sz])&]
[s2; Sets the optional physical size in dots.&]
[s0; &]
[s5;K:`:`:ImageBuffer`:`:GetDots`(`)const:%- [^`:`:Size^ Size]_[@0 GetDots]()_[@(0.0.255) c
onst]&]
[s2; Returns the optional physical size. Default value is Size(0, 
0).&]
[s0; &]
[s5;K:`:`:ImageBuffer`:`:GetSize`(`)const:%- [^`:`:Size^ Size]_[@0 GetSize]()_[@(0.0.255) c
onst]&]
[s2; Returns the dimensions of image.&]
[s0; &]
[s5;K:`:`:ImageBuffer`:`:GetWidth`(`)const:%- [@(0.0.255) int]_[@0 GetWidth]()_[@(0.0.255) c
onst]&]
[s2; Same as GetSize().cx.&]
[s0; &]
[s5;K:`:`:ImageBuffer`:`:GetHeight`(`)const:%- [@(0.0.255) int]_[@0 GetHeight]()_[@(0.0.255) c
onst]&]
[s2; Same as GetSize().cy.&]
[s0; &]
[s5;K:`:`:ImageBuffer`:`:GetLength`(`)const:%- [@(0.0.255) int]_[@0 GetLength]()_[@(0.0.255) c
onst]&]
[s2; Returns the number of pixels in the image. Same as GetSize().cx 
`* GetSize().cy.&]
[s0; &]
[s5;K:`:`:ImageBuffer`:`:operator`[`]`(int`):%- [^`:`:RGBA^ RGBA]_`*[@0 operator`[`]]([@(0.0.255) i
nt]_[@3 i])&]
[s5;K:`:`:ImageBuffer`:`:operator`[`]`(int`)const:%- [@(0.0.255) const]_[^`:`:RGBA^ RGBA]_
`*[@0 operator`[`]]([@(0.0.255) int]_[@3 i])_[@(0.0.255) const]&]
[s2; Returns a pointer to the first pixel in the line [%-*@3 i].&]
[s0; &]
[s5;K:`:`:ImageBuffer`:`:operator`~`(`):%- [^`:`:RGBA^ RGBA]_`*[@0 operator`~]()&]
[s5;K:`:`:ImageBuffer`:`:operator`:`:RGBA`*`(`):%- [@0 operator_RGBA]`*()&]
[s5;K:`:`:ImageBuffer`:`:operator`~`(`)const:%- [@(0.0.255) const]_[^`:`:RGBA^ RGBA]_[@0 `*
operator`~]()_[@(0.0.255) const]&]
[s5;K:`:`:ImageBuffer`:`:operator const`:`:RGBA`*`(`)const:%- [@0 operator]_[@(0.0.255) c
onst]_[@0 RGBA`*]()_[@(0.0.255) const]&]
[s2; Returns a pointer to the first pixel of the image.  &]
[s0; &]
[s5;K:`:`:ImageBuffer`:`:Create`(int`,int`):%- [@(0.0.255) void]_[@0 Create]([@(0.0.255) in
t]_[@3 cx], [@(0.0.255) int]_[@3 cy])&]
[s5;K:`:`:ImageBuffer`:`:Create`(`:`:Size`):%- [@(0.0.255) void]_[@0 Create]([^`:`:Size^ Si
ze]_[@3 sz])&]
[s2; Creates a new image of specified size. Value of pixels is undefined. 
Previous content of ImageBuffer is lost.&]
[s0; &]
[s5;K:`:`:ImageBuffer`:`:IsEmpty`(`)const:%- [@(0.0.255) bool]_[@0 IsEmpty]()_[@(0.0.255) c
onst]&]
[s2; Same as GetLength() `=`= 0.&]
[s0; &]
[s5;K:`:`:ImageBuffer`:`:Clear`(`):%- [@(0.0.255) void]_[@0 Clear]()&]
[s2; Clears the content, removing all pixels and setting the size 
of Image to Size(0, 0).&]
[s0; &]
[s5;K:`:`:ImageBuffer`:`:operator`=`(`:`:Image`&`):%- [@(0.0.255) void]_[@0 operator`=]([^`:`:Image^ I
mage]`&_[@3 img])&]
[s5;K:`:`:ImageBuffer`:`:operator`=`(`:`:ImageBuffer`&`):%- [@(0.0.255) void]_[@0 operato
r`=]([^`:`:ImageBuffer^ ImageBuffer]`&_[@3 img])&]
[s2; Assigns pixels of [%-*@3 img] to ImageBuffer. [%-*@3 img] is cleared 
and empty after this operation, price paid for low`-cost constant 
time operation.&]
[s0; &]
[s5;K:`:`:ImageBuffer`:`:`:`:ImageBuffer`(`):%- [@0 ImageBuffer]()&]
[s2; Constructs empty ImageBuffer.&]
[s0; &]
[s5;K:`:`:ImageBuffer`:`:`:`:ImageBuffer`(int`,int`):%- [@0 ImageBuffer]([@(0.0.255) int]_
[@3 cx], [@(0.0.255) int]_[@3 cy])&]
[s5;K:`:`:ImageBuffer`:`:`:`:ImageBuffer`(`:`:Size`):%- [@0 ImageBuffer]([^`:`:Size^ Size
]_[@3 sz])&]
[s2; Constructs ImageBuffer of specified size. Value of pixels is 
undefined.&]
[s0; &]
[s5;K:`:`:ImageBuffer`:`:`:`:ImageBuffer`(`:`:Image`&`):%- [@0 ImageBuffer]([^`:`:Image^ I
mage]`&_[@3 img])&]
[s5;K:`:`:ImageBuffer`:`:`:`:ImageBuffer`(`:`:ImageBuffer`&`):%- [@0 ImageBuffer]([@0 Ima
geBuffer]`&_[@3 img])&]
[s2; Assigns pixels of [%-*@3 img] to ImageBuffer. [%-*@3 img] is cleared 
and empty after this operation, price paid for low`-cost constant 
time operation.&]
[s0; &]
[s5;K:`:`:ImageBuffer`:`:`:`:ImageBuffer`(`:`:ImageDraw`&`):%- [@0 ImageBuffer]([^`:`:ImageDraw^ I
mageDraw]`&_[@3 w])&]
[s2; Creates ImageBuffer from ImageDraw. ImageDraw is cleared and 
empty after this operation, price paid for low`-cost constant 
time operation.&]
[s2; &]
[s0; &]
[s0; ]