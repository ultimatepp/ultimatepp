topic "ImageBuffer";
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
[ {{10000@(113.42.0) [s0;%% [*@7;4 ImageBuffer]]}}&]
[s3; &]
[s1;:ImageBuffer`:`:class: [*@(0.0.255) class][* _ImageBuffer_:_][*@(0.0.255) private][* _][*@3 N
oCopy]&]
[s9;%% ImageBuffer represents a writable Image `- an array of RGBA 
pixels. ImageBuffer can be converted to Image in low`-const constant 
time while loosing its content and vice versa, Image can be converted 
to ImageBuffer again loosing its content.&]
[s9;%% Content of Image / ImageBuffer can be classified to optimize 
drawing. Possible classifications are&]
[s2;%% &]
[ {{2913:7087<288;>416; [s0;%% [* IMAGE`_UNKNOWN]]
:: [s0;%% The image kind is unknown.]
:: [s0;%% [* IMAGE`_EMPTY]]
:: [s0;%% The image is empty (all alpha values are 0).]
:: [s0;%% [* IMAGE`_ALPHA]]
:: [s0;%% The image has alpha values different from 255 and 0.]
:: [s0;%% [* IMAGE`_MASK]]
:: [s0;%% The image has alpha values 0 or 255 only.]
:: [s0;%% [* IMAGE`_OPAQUE]]
:: [s0;%% The image has all alpha values 255.]}}&]
[s2;%% &]
[s9;%% Note that is the kind of image is unknown, painting routine 
determines it automatically by scanning pixels and stores the 
result.&]
[s9;%% Pixels of image are organized in simple continual POD array, 
first pixel being top`-left.&]
[s9;%% U`+`+ expects pixels to be in premultiplied alpha format.&]
[s3; &]
[s0; &]
[ {{10000F(128)G(128)@1 [s0;%% [* Public Member List]]}}&]
[s3; &]
[s5;:ImageBuffer`:`:SetKind`(int`): [*@(0.0.255) void][* _SetKind(][*@(0.0.255) int][* _][*@3 k
][* )]&]
[s2;%% Sets the kind of image. You can use this to avoid automatic 
detection.&]
[s3; &]
[s4; &]
[s5;:ImageBuffer`:`:GetKind`(`)const: [*@(0.0.255) int][* _GetKind()_][*@(0.0.255) const]&]
[s2;%% Returns the kind of image set by SetKind.&]
[s3; &]
[s4; &]
[s5;:ImageBuffer`:`:ScanKind`(`)const: [*@(0.0.255) int][* _ScanKind()_][*@(0.0.255) const]&]
[s2;%% Scans all RGBA pixels to determine Image kind.&]
[s3; &]
[s4; &]
[s5;:ImageBuffer`:`:GetScanKind`(`)const: [*@(0.0.255) int][* _GetScanKind()_][*@(0.0.255) c
onst]&]
[s2;%% If kind set by SetKind is other than IMAGE`_UNKNOWN, returns 
it, otherwise calls ScanKind().&]
[s3; &]
[s4; &]
[s5;:ImageBuffer`:`:SetHotSpot`(Point`): [*@(0.0.255) void][* _SetHotSpot(][*_^Point^ Point
][* _][*@3 p][* )]&]
[s2;%% Sets the reference point. This point is e.g. used as hot`-spot 
when Image is used as mouse pointer. Iml designer has the ability 
to specify this point.&]
[s3; &]
[s4; &]
[s5;:ImageBuffer`:`:GetHotSpot`(`)const: [*_^Point^ Point][* _GetHotSpot()_][*@(0.0.255) co
nst]&]
[s2;%% Returns the reference point.&]
[s3; &]
[s4; &]
[s5;:ImageBuffer`:`:Set2ndSpot`(Point`): [*@(0.0.255) void][* _Set2ndSpot]([_^Point^ Point]_
[*@3 p])&]
[s2;%% Sets secondary reference point. Iml designer has the ability 
to specify this point.&]
[s3;%% &]
[s4; &]
[s5;:ImageBuffer`:`:Get2ndSpot`(`)const: [*_^Point^ Point][* _Get2ndSpot]()_[@(0.0.255) con
st]&]
[s2;%% Reads secondary reference point.&]
[s3; &]
[s4; &]
[s5;:ImageBuffer`:`:SetDots`(Size`): [*@(0.0.255) void][* _SetDots(][*_^Size^ Size][* _][*@3 sz
][* )]&]
[s2;%% Sets the optional physical size in dots.&]
[s3; &]
[s4; &]
[s5;:ImageBuffer`:`:GetDots`(`)const: [*_^Size^ Size][* _GetDots()_][*@(0.0.255) const]&]
[s2;%% Returns the optional physical size. Default value is Size(0, 
0).&]
[s3; &]
[s4; &]
[s5;:ImageBuffer`:`:SetDPI`(Size`): [@(0.0.255) void]_[* SetDPI]([_^Size^ Size]_[*@3 sz])&]
[s2;%% Sets the image resolution in dpi (dots per inch).&]
[s2;%% As the dpi are not directly stored, if SetDots() is called 
or image is resized, the dpi could change.&]
[s3;%% &]
[s4; &]
[s5;:ImageBuffer`:`:GetDPI`(`): [_^Size^ Size]_[* GetDPI]()&]
[s2;%% Returns the image resolution in dpi (dots per inch).&]
[s2;%% As the dpi are not directly stored, if SetDots() is called 
or image is resized, the dpi could change.&]
[s3; &]
[s4; &]
[s5;:ImageBuffer`:`:GetSize`(`)const: [*_^Size^ Size][* _GetSize()_][*@(0.0.255) const]&]
[s2;%% Returns the dimensions of image.&]
[s3; &]
[s4; &]
[s5;:ImageBuffer`:`:GetWidth`(`)const: [*@(0.0.255) int][* _GetWidth()_][*@(0.0.255) const]&]
[s2;%% Same as GetSize().cx.&]
[s3; &]
[s4; &]
[s5;:ImageBuffer`:`:GetHeight`(`)const: [*@(0.0.255) int][* _GetHeight()_][*@(0.0.255) cons
t]&]
[s2;%% Same as GetSize().cy.&]
[s3; &]
[s4; &]
[s5;:ImageBuffer`:`:GetLength`(`)const: [*@(0.0.255) int][* _GetLength()_][*@(0.0.255) cons
t]&]
[s2;%% Returns the number of pixels in the image. Same as GetSize().cx 
`* GetSize().cy.&]
[s3; &]
[s4; &]
[s5;:ImageBuffer`:`:operator`[`]`(int`): [*_^RGBA^ RGBA][* _`*operator`[`](][*@(0.0.255) in
t][* _][*@3 i][* )]&]
[s5;:ImageBuffer`:`:operator`[`]`(int`)const: [*@(0.0.255) const][* _][*_^RGBA^ RGBA][* _`*op
erator`[`](][*@(0.0.255) int][* _][*@3 i][* )_][*@(0.0.255) const]&]
[s2;%% Returns a pointer to the first pixel in the line [%-*@3 i].&]
[s3; &]
[s4; &]
[s5;:ImageBuffer`:`:operator`~`(`): [*_^RGBA^ RGBA][* _`*operator`~()]&]
[s5;:ImageBuffer`:`:operator RGBA`*`(`): [* operator_RGBA`*()]&]
[s5;:ImageBuffer`:`:operator`~`(`)const: [*@(0.0.255) const][* _][*_^RGBA^ RGBA][* _`*operato
r`~()_][*@(0.0.255) const]&]
[s5;:ImageBuffer`:`:operator const RGBA`*`(`)const: [* operator_const_RGBA`*()_][*@(0.0.255) c
onst]&]
[s2;%% Returns a pointer to the first pixel of the image.  &]
[s3; &]
[s4; &]
[s5;:ImageBuffer`:`:Create`(int`,int`): [*@(0.0.255) void][* _Create(][*@(0.0.255) int][* _][*@3 c
x][* , ][*@(0.0.255) int][* _][*@3 cy][* )]&]
[s5;:ImageBuffer`:`:Create`(Size`): [*@(0.0.255) void][* _Create(][*_^Size^ Size][* _][*@3 sz][* )
]&]
[s2;%% Creates a new image of specified size. Value of pixels is 
undefined. Previous content of ImageBuffer is lost.&]
[s3; &]
[s4; &]
[s5;:ImageBuffer`:`:IsEmpty`(`)const: [*@(0.0.255) bool][* _IsEmpty()_][*@(0.0.255) const]&]
[s2;%% Same as GetLength() `=`= 0.&]
[s3; &]
[s4; &]
[s5;:ImageBuffer`:`:Clear`(`): [*@(0.0.255) void][* _Clear()]&]
[s2;%% Clears the content, removing all pixels and setting the size 
of Image to Size(0, 0).&]
[s3; &]
[s4; &]
[s5;:ImageBuffer`:`:operator`=`(Image`&`): [*@(0.0.255) void][* _operator`=(][*_^Image^ Ima
ge][*@(0.0.255) `&][* _][*@3 img][* )]&]
[s5;:ImageBuffer`:`:operator`=`(ImageBuffer`&`): [*@(0.0.255) void][* _operator`=(][*_^ImageBuffer^ I
mageBuffer][*@(0.0.255) `&][* _][*@3 img][* )]&]
[s2;%% Assigns pixels of [%-*@3 img] to ImageBuffer. [%-*@3 img] is cleared 
and empty after this operation, price paid for low`-cost constant 
time operation.&]
[s3; &]
[s0; &]
[ {{10000t/25b/25@1 [s0; [* Constructor Detail]]}}&]
[s3; &]
[s5;:ImageBuffer`:`:ImageBuffer`(`): [* ImageBuffer]()&]
[s2;%% Constructs empty ImageBuffer.&]
[s3; &]
[s4; &]
[s5;:ImageBuffer`:`:ImageBuffer`(int`,int`): [* ImageBuffer(][*@(0.0.255) int][* _][*@3 cx][* ,
 ][*@(0.0.255) int][* _][*@3 cy][* )]&]
[s5;:ImageBuffer`:`:ImageBuffer`(Size`): [* ImageBuffer(][*_^Size^ Size][* _][*@3 sz][* )]&]
[s2;%% Constructs ImageBuffer of specified size. Value of pixels 
is undefined.&]
[s3; &]
[s4; &]
[s5;:ImageBuffer`:`:ImageBuffer`(Image`&`): [* ImageBuffer]([_^Image^ Image][@(0.0.255) `&]_
[*@3 img])&]
[s5;:ImageBuffer`:`:ImageBuffer`(ImageBuffer`&`): [* ImageBuffer]([* ImageBuffer][@(0.0.255) `&
]_[*@3 img])&]
[s2;%% Assigns pixels of [%-*@3 img] to ImageBuffer. [%-*@3 img] is cleared 
and empty after this operation, price paid for low`-cost constant 
time operation.&]
[s3; &]
[s0; ]