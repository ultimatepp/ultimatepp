topic "Image Utility Functions";
[i448;a25;kKO9;2 $$1,0#37138531426314131252341829483380:class]
[l288;2 $$2,2#27521748481378242620020725143825:desc]
[0 $$3,0#96390100711032703541132217272105:end]
[H6;0 $$4,0#05600065144404261032431302351956:begin]
[i448;a25;kKO9;2 $$5,0#37138531426314131252341829483370:item]
[l288;a4;*@5;1 $$6,6#70004532496200323422659154056402:requirement]
[l288;i1121;b17;O9;~~~.1408;2 $$7,0#10431211400427159095818037425705:param]
[i448;b42;O9;2 $$8,8#61672508125594000341940100500538:tparam]
[b42;2 $$9,9#13035079074754324216151401829390:normal]
[2 $$0,0#00000000000000000000000000000000:Default]
[{_} 
[ {{10000@(113.42.0) [s0;%% [*@(229)4 Image Utility Functions]]}}&]
[s3; &]
[s5;:Upp`:`:SetHotSpots`(Upp`:`:Image`&`,Upp`:`:Point`,Upp`:`:Point`): [@(0.0.255) void
]_[* SetHotSpots]([_^Upp`:`:Image^ Image][@(0.0.255) `&]_[*@3 m], [_^Upp`:`:Point^ Point]_[*@3 h
otspot], [_^Upp`:`:Point^ Point]_[*@3 hotspot2])&]
[s2;%% Sets hotspots in [%-*@3 m] to [%-*@3 hotspot], [%-*@3 hotspot2].&]
[s3;%% &]
[s4; &]
[s5;:Upp`:`:WithHotSpots`(const Upp`:`:Image`&`,Upp`:`:Point`,Upp`:`:Point`): [_^Upp`:`:Image^ I
mage]_[* WithHotSpots]([@(0.0.255) const]_[_^Upp`:`:Image^ Image][@(0.0.255) `&]_[*@3 m], 
[_^Upp`:`:Point^ Point]_[*@3 hotspot], [_^Upp`:`:Point^ Point]_[*@3 hotspot2])&]
[s5;:WithHotSpots`(const Image`&`,int`,int`,int`,int`): [_^Image^ Image]_[* WithHotSpots](
[@(0.0.255) const]_[_^Image^ Image][@(0.0.255) `&]_[*@3 m], [@(0.0.255) int]_[*@3 x1], 
[@(0.0.255) int]_[*@3 y1], [@(0.0.255) int]_[*@3 x2], [@(0.0.255) int]_[*@3 y2])&]
[s2;%% Returns [%-*@3 m] with new hotspots.&]
[s3;%% &]
[s4;%% &]
[s5;:WithHotSpot`(const Image`&`,int`,int`): [_^Image^ Image]_[* WithHotSpot]([@(0.0.255) c
onst]_[_^Image^ Image][@(0.0.255) `&]_[*@3 m], [@(0.0.255) int]_[*@3 x1], 
[@(0.0.255) int]_[*@3 y1])&]
[s2; [%% Returns ][*@3 m][%%  with hotspot ][*@3 x1,][%%  ][*@3 y1.]&]
[s3; &]
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
[s3; &]
[s4; &]
[s5;:Upp`:`:DstSrcOp`(ImageBuffer`&`,Point`,const Image`&`,const Rect`&`,void`(`*`)`(RGBA`*`,const RGBA`*`,int`)`,bool`): [@(0.0.255) v
oid] [* DstSrcOp](ImageBuffer[@(0.0.255) `&] [*@3 dest], Point [*@3 p], 
[@(0.0.255) const] Image[@(0.0.255) `&] [*@3 src], [@(0.0.255) const] 
Rect[@(0.0.255) `&] [*@3 srect], [@(0.0.255) void] ([@(0.0.255) `*]op)(RGBA 
[@(0.0.255) `*], [@(0.0.255) const] [*@3 RGBA] [@(0.0.255) `*], [@(0.0.255) int]), 
[@(0.0.255) bool] [*@3 co] [@(0.0.255) `=] [@(0.0.255) false])&]
[s2;%% Copies Rect [%-*@3 srect] of Image [%-*@3 src] into Point [%-*@3 p] 
of ImageBuffer [%-*@3 dest] using copying function [%-*@3 op]. If 
[%-*@3 co] is true, uses multithreading.&]
[s0;%% &]
[s4; &]
[s5;:Upp`:`:Copy`(ImageBuffer`&`,Point`,const Image`&`,const Rect`&`,bool`): [@(0.0.255) v
oid] [* Copy](ImageBuffer[@(0.0.255) `&] [*@3 dest], Point [*@3 p], [@(0.0.255) const] 
Image[@(0.0.255) `&] [*@3 src], [@(0.0.255) const] Rect[@(0.0.255) `&] 
[*@3 srect], [@(0.0.255) bool] [*@3 co] [@(0.0.255) `=] [@(0.0.255) false])&]
[s2;%% Copies Rect [%-*@3 srect] of Image [%-*@3 src] into Point [%-*@3 p] 
of ImageBuffer [%-*@3 dest]. [%-*@3 co] activates multithreading.&]
[s3; &]
[s4; &]
[s5;:Fill`(ImageBuffer`&`,const Rect`&`,RGBA`): [@(0.0.255) void]_[* Fill]([_^ImageBuffer^ I
mageBuffer][@(0.0.255) `&]_[*@3 dest], [@(0.0.255) const]_[_^Rect^ Rect][@(0.0.255) `&]_[*@3 r
ect], [_^RGBA^ RGBA]_[*@3 color])&]
[s2;%% Fills rectangular are [%-*@3 rect] with [%-*@3 color] .&]
[s3;%% &]
[s4; &]
[s5;:Upp`:`:Copy`(Image`&`,Point`,const Image`&`,const Rect`&`,bool`): [@(0.0.255) void
] [* Copy](Image[@(0.0.255) `&] [*@3 dest], Point [*@3 p], [@(0.0.255) const] 
Image[@(0.0.255) `&] [*@3 src], [@(0.0.255) const] Rect[@(0.0.255) `&] 
[*@3 srect], [@(0.0.255) bool] [*@3 co] [@(0.0.255) `=] [@(0.0.255) false])&]
[s2;%% Copies Rect [%-*@3 srect] of Image [%-*@3 src] into Point [%-*@3 p] 
of Image [%-*@3 dest]. If [%-*@3 co] is true, uses multithreading.&]
[s3;%% &]
[s4; &]
[s5;:Upp`:`:Over`(ImageBuffer`&`,Point`,const Image`&`,const Rect`&`,bool`): [@(0.0.255) v
oid] [* Over](ImageBuffer[@(0.0.255) `&] [*@3 dest], Point [*@3 p], [@(0.0.255) const] 
Image[@(0.0.255) `&] [*@3 src], [@(0.0.255) const] Rect[@(0.0.255) `&] 
[*@3 srect], [@(0.0.255) bool] [*@3 co] [@(0.0.255) `=] [@(0.0.255) false])&]
[s2;%% Paints over (using alpha channel) Rect [%-*@3 srect] of Image 
[%-*@3 src] into Point [%-*@3 p] of ImageBuffer [%-*@3 dest]. If [%-*@3 co] 
is true, uses multithreading.&]
[s3;%% &]
[s4; &]
[s5;:Upp`:`:Over`(Image`&`,Point`,const Image`&`,const Rect`&`,bool`): [@(0.0.255) void
] [* Over](Image[@(0.0.255) `&] [*@3 dest], Point [*@3 p], [@(0.0.255) const] 
Image[@(0.0.255) `&] [*@3 `_src], [@(0.0.255) const] Rect[@(0.0.255) `&] 
[*@3 srect], [@(0.0.255) bool] [*@3 co] [@(0.0.255) `=] [@(0.0.255) false])&]
[s2;%% Paints over (using alpha channel) Rect [%-*@3 srect] of Image 
[%-*@3 src] into Point [%-*@3 p] of Image [%-*@3 dest]. If [%-*@3 co] 
is true, uses multithreading.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:Over`(Image`&`,const Image`&`,bool`): [@(0.0.255) void] 
[* Over](Image[@(0.0.255) `&] [*@3 dest], [@(0.0.255) const] Image[@(0.0.255) `&] 
[*@3 `_src], [@(0.0.255) bool] [*@3 co] [@(0.0.255) `=] [@(0.0.255) false])&]
[s2;%% Same as Over([%-*@3 dest], Point(0, 0), [%-*@3 src], [%-*@3 src].GetSize(), 
[%-*@3 co]).&]
[s3;%% &]
[s4; &]
[s5;:Fill`(Image`&`,const Rect`&`,RGBA`): [@(0.0.255) void]_[* Fill]([_^Image^ Image][@(0.0.255) `&
]_[*@3 dest], [@(0.0.255) const]_[_^Rect^ Rect][@(0.0.255) `&]_[*@3 rect], 
[_^RGBA^ RGBA]_[*@3 color])&]
[s2;%% Fills rectangular are [%-*@3 rect] with [%-*@3 color].&]
[s3;%% &]
[s4; &]
[s5;:Upp`:`:Copy`(const Upp`:`:Image`&`,const Upp`:`:Rect`&`): [_^Upp`:`:Image^ Image]_
[* Copy]([@(0.0.255) const]_[_^Upp`:`:Image^ Image][@(0.0.255) `&]_[*@3 src], 
[@(0.0.255) const]_[_^Upp`:`:Rect^ Rect][@(0.0.255) `&]_[*@3 srect])&]
[s2;%% Returns [%-*@3 srect] area of [%-*@3 src].&]
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
[s5;:Upp`:`:FindBounds`(const Upp`:`:Image`&`,Upp`:`:RGBA`): [_^Upp`:`:Rect^ Rect]_[* Fin
dBounds]([@(0.0.255) const]_[_^Upp`:`:Image^ Image][@(0.0.255) `&]_[*@3 m], 
[_^Upp`:`:RGBA^ RGBA]_[*@3 bg]_`=_RGBAZero())&]
[s2;%% Detects rectangular margin with uniform color [%-*@3 bg] and 
returns the rectangle with the image with pixels inside this 
margin.&]
[s3;%% &]
[s4; &]
[s5;:Upp`:`:AutoCrop`(const Upp`:`:Image`&`,Upp`:`:RGBA`): [_^Upp`:`:Image^ Image]_[* Aut
oCrop]([@(0.0.255) const]_[_^Upp`:`:Image^ Image][@(0.0.255) `&]_[*@3 m], 
[_^Upp`:`:RGBA^ RGBA]_[*@3 bg]_`=_RGBAZero())&]
[s2;%% Detects rectangular margin with uniform color [%-*@3 bg] and 
then crops this margin out.&]
[s3;%% &]
[s4; &]
[s5;:Upp`:`:AutoCrop`(Upp`:`:Image`*`,int`,Upp`:`:RGBA`): [@(0.0.255) void]_[* AutoCrop](
[_^Upp`:`:Image^ Image]_`*[*@3 m], [@(0.0.255) int]_[*@3 count], [_^Upp`:`:RGBA^ RGBA]_[*@3 b
g])&]
[s2;%% Finds the maximal rectangular margin with uniform color [%-*@3 bg] 
of all images and then crops this margin out. All images must 
have the same size and will have the same size after the operation.&]
[s3;%% &]
[s4; &]
[s5;:Upp`:`:ClampHotSpots`(Upp`:`:Image`&`): [@(0.0.255) void]_[* ClampHotSpots]([_^Upp`:`:Image^ I
mage][@(0.0.255) `&]_[*@3 m])&]
[s2;%% Clamps the positions of hotspots so that they are inside [%-*@3 m].&]
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
[s2;%% Changes the size of [%-*@3 img]. If any areas have to be added, 
they are filled with RGBAZero().&]
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
[s3; &]
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
[s5;:Rescale`(const Image`&`,Size`,const Rect`&`,Gate`<int`,int`>`): [_^Image^ Image]_[* R
escale]([@(0.0.255) const]_[_^Image^ Image][@(0.0.255) `&]_[*@3 src], 
[_^Size^ Size]_[*@3 sz], [@(0.0.255) const]_[_^Rect^ Rect][@(0.0.255) `&]_[*@3 src`_rc], 
[_^Gate2^ Gate2]<[@(0.0.255) int], [@(0.0.255) int]>_[*@3 progress]_`=_[@(0.0.255) false])&]
[s2; [%% Returns a new Image rescaled to Size ][*@3 sz][%%  based on Rect 
][*@3 src`_rc ][%% of Image ][*@3 src]&]
[s3;%% &]
[s4; &]
[s5;:Upp`:`:Rescale`(const Image`&`,Size`,Gate`): Image [* Rescale]([@(0.0.255) const] 
Image[@(0.0.255) `&] [*@3 src], Size [*@3 sz], Gate<[@(0.0.255) int], 
[@(0.0.255) int]> progress [@(0.0.255) `=] [*@3 Null])&]
[s5;:Upp`:`:Rescale`(const Image`&`,int`,int`,Gate`): Image [* Rescale]([@(0.0.255) const
] Image[@(0.0.255) `&] [*@3 src], [@(0.0.255) int] [*@3 cx], [@(0.0.255) int] 
[*@3 cy], Gate<[@(0.0.255) int], [@(0.0.255) int]> progress [@(0.0.255) `=] 
[*@3 Null])&]
[s2;%% Returns [%-*@3 src] rescaled to requested size.&]
[s3; &]
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
[s5;:Upp`:`:GaussianBlur`(const Upp`:`:Image`&`,int`,bool`): [_^Upp`:`:Image^ Image]_[* G
aussianBlur]([@(0.0.255) const]_[_^Upp`:`:Image^ Image][@(0.0.255) `&]_[*@3 img], 
[@(0.0.255) int]_[*@3 radius], [@(0.0.255) bool]_[*@3 co]_`=_[@(0.0.255) false])&]
[s2;%% [%-@N An implementation of well known fast box and gaussian 
blur approximation algorithms by Wojciech Jarosz and Peter Kovesi.] 
[%-*@3 co] activates parallel processing.&]
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
[s3; &]
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
[s4; &]
[s5;:Upp`:`:Transpose`(const Upp`:`:Image`&`): [_^Upp`:`:Image^ Image]_[* Transpose]([@(0.0.255) c
onst]_[_^Upp`:`:Image^ Image][@(0.0.255) `&]_[*@3 img])&]
[s2;%% Result is the same as MirrorHorz(RotateClockwise([%-*@3 img])), 
but implemented effectively in single step.&]
[s3;%% &]
[s4; &]
[s5;:Upp`:`:Transverse`(const Upp`:`:Image`&`): [_^Upp`:`:Image^ Image]_[* Transverse]([@(0.0.255) c
onst]_[_^Upp`:`:Image^ Image][@(0.0.255) `&]_[*@3 img])&]
[s2;%% Result is the same as MirrorHorz(RotateAntiClockwise([%-*@3 img])), 
but implemented effectively in single step.&]
[s3;%% &]
[s4; &]
[s5;:Upp`:`:FlipImage`(const Upp`:`:Image`&`,int`): [_^Upp`:`:Image^ Image]_[* FlipImage](
[@(0.0.255) const]_[_^Upp`:`:Image^ Image][@(0.0.255) `&]_[*@3 m], [@(0.0.255) int]_[*@3 mode
])&]
[s2;%% Transforms Image based on flip [%-*@3 mode], which can be one 
of FLIP`_NONE, FLIP`_MIRROR`_HORZ, FLIP`_ROTATE`_180, FLIP`_MIRROR`_VERT, 
FLIP`_TRANSPOSE, FLIP`_ROTATE`_CLOCKWISE, FLIP`_TRANSVERSE, FLIP`_ROTATE`_ANTICLO
CKWISE. Flip mode values are compatible with Raster`::GetOrientation 
and are equal to EXIF orientation `- 1. This function is intended 
to flip Image to correct orientation (usually JPEG from digital 
camera).&]
[s2;%% &]
[s0;%% ]]