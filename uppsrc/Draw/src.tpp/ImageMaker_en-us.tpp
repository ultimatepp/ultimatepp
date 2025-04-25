topic "ImageMaker - image cache system";
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
[ {{10000@(113.42.0) [s0;%% [*@7;4 ImageMaker `- image cache system]]}}&]
[s1;@(0.0.255)3 &]
[s0;%% Image cache system provides means to create and cache Images. 
For example, in GUI there might be requirement for monochromatic 
version of icons to express disabled status. In this case, ImageMaker 
derived class can be defined to make such Image from color version 
and by using MakeImage, caching mechanism is introduced. Image 
cache is using global Value cache as underlying mechanism.&]
[s0; &]
[ {{10000F(128)G(128)@1 [s0;%% [* ImageMaker]]}}&]
[s0; &]
[s5; [@(0.0.255) struct]_[* ImageMaker]&]
[s2;%% This class serves as base class for all image makers. Client 
class usually adds parameters to class derived from ImageMaker. 
For example, for caching rescaled images, parameters would be 
the source Image and the final size.&]
[s3; &]
[s4; &]
[s5;:ImageMaker`:`:Key`(`)const: [@(0.0.255) virtual] [_^String^ String]_[* Key]()_[@(0.0.255) c
onst]_`=_[@3 0]&]
[s2;%% Returns the key for provided set of parameter. Note that the 
type identification of ImageMaker is automatically added by MakeImage.&]
[s3; &]
[s4; &]
[s5;:ImageMaker`:`:Make`(`)const: [@(0.0.255) virtual] [_^Image^ Image]_[* Make]()_[@(0.0.255) c
onst]_`=_[@3 0]&]
[s2;%% Creates Image based on parameters.&]
[s3; &]
[s4; &]
[s5;:ImageMaker`:`:`~ImageMaker`(`): [@(0.0.255) `~][* ImageMaker]()&]
[s2;%% Virtual destructor.&]
[s0;i448;a25;kKO9;@(0.0.255) &]
[ {{10000F(128)G(128)@1 [s0;%% [* Function List]]}}&]
[s0; &]
[s5;:MakeImage`(const ImageMaker`&`): [_^Image^ Image]_[* MakeImage]([@(0.0.255) const]_[_^ImageMaker^ I
mageMaker][@(0.0.255) `&]_[*@3 m])&]
[s2;%% Returns the Image based on ImageMaker (either retrieves from 
the cache or creates and stores to cache). Images are stored 
in the U`+`+ global cache.&]
[s3;%% &]
[s4; &]
[s5;:MakeImage`(const Image`&`,Image`(`*`)`(const Image`&`)`): [_^Image^ Image]_[* MakeIm
age]([@(0.0.255) const]_[_^Image^ Image][@(0.0.255) `&]_[*@3 image], 
[_^Image^ Image]_(`*[*@3 make])([@(0.0.255) const]_[_^Image^ Image][@(0.0.255) `&]_[*@3 image
]))&]
[s2;%% Simplified version for Image making process that does not 
require other parameters than single input image.&]
[s3;%% &]
[s4; &]
[s5;:MakeImagePaintOnly`(const ImageMaker`&`): [_^Image^ Image]_[* MakeImagePaintOnly]([@(0.0.255) c
onst]_[_^ImageMaker^ ImageMaker][@(0.0.255) `&]_[*@3 m])&]
[s2;%% Similar to MakeImage, but creates and Image that can only 
be used in Draw`::DrawImage (this is optimization hint that can 
save some memory in certain situations).&]
[s3;%% &]
[s4; &]
[s5;:CachedRescale`(const Image`&`,Size`,const Rect`&`,int`): [_^Image^ Image]_[* CachedR
escale]([@(0.0.255) const]_[_^Image^ Image][@(0.0.255) `&]_[*@3 m], [_^Size^ Size]_[*@3 sz], 
[@(0.0.255) const]_[_^Rect^ Rect][@(0.0.255) `&]_[*@3 src], [@(0.0.255) int]_[*@3 filter]_`=_
Null)&]
[s5;:CachedRescale`(const Image`&`,Size`,int`): [_^Image^ Image]_[* CachedRescale]([@(0.0.255) c
onst]_[_^Image^ Image][@(0.0.255) `&]_[*@3 m], [_^Size^ Size]_[*@3 sz], 
[@(0.0.255) int]_[*@3 filter]_`=_Null)&]
[s2;%% Returns rescaled Image, with results being cached. [%-*@3 filter] 
can be one of predefined filters for RescaleFilter function (e.g. 
FILTER`_BILINEAR), if Null, standard speed optimized custom algorithm 
is used.&]
[s3;%% &]
[s4; &]
[s5;:CachedRescalePaintOnly`(const Image`&`,Size`,const Rect`&`,int`): [_^Image^ Image]_
[* CachedRescalePaintOnly]([@(0.0.255) const]_[_^Image^ Image][@(0.0.255) `&]_[*@3 m], 
[_^Size^ Size]_[*@3 sz], [@(0.0.255) const]_[_^Rect^ Rect][@(0.0.255) `&]_[*@3 src], 
[@(0.0.255) int]_[*@3 filter]_`=_Null)&]
[s5;:CachedRescalePaintOnly`(const Image`&`,Size`,int`): [_^Image^ Image]_[* CachedRescal
ePaintOnly]([@(0.0.255) const]_[_^Image^ Image][@(0.0.255) `&]_[*@3 m], 
[_^Size^ Size]_[*@3 sz], [@(0.0.255) int]_[*@3 filter]_`=_Null)&]
[s2;%% Returns rescaled Image, with results being cached, that can 
only be used with Draw`::DrawImage(this is optimization hint that 
can save some memory in certain situations). [%-*@3 filter] can 
be one of predefined filters for RescaleFilter function (e.g. 
FILTER`_BILINEAR), if Null, standard speed optimized custom algorithm 
is used.&]
[s3;%% &]
[s4; &]
[s5;:CachedSetColorKeepAlpha`(const Image`&`,Color`): [_^Image^ Image]_[* CachedSetColorK
eepAlpha]([@(0.0.255) const]_[_^Image^ Image][@(0.0.255) `&]_[*@3 img], 
[_^Color^ Color]_[*@3 color])&]
[s2;%% Cached variant of SetColorKeepAlpha. Replaces all pixel colors 
in Image with [%-*@3 color] while not changing the alpha value.&]
[s3;%% &]
[s4; &]
[s5;:CachedSetColorKeepAlphaPaintOnly`(const Image`&`,Color`): [_^Image^ Image]_[* Cached
SetColorKeepAlphaPaintOnly]([@(0.0.255) const]_[_^Image^ Image][@(0.0.255) `&]_[*@3 img],
 [_^Color^ Color]_[*@3 color])&]
[s2;%% Cached variant of SetColorKeepAlpha. Replaces all pixel colors 
in Image with [%-*@3 color] while not changing the alpha value, 
with results being cached, that can only be used with Draw`::DrawImage(this 
is optimization hint that can save some memory in certain situations).&]
[s3;%% &]
[s4; &]
[s5;:Upp`:`:Magnify`(const Image`&`,const Rect`&`,int`,int`,bool`): Image 
[* Magnify]([@(0.0.255) const] Image[@(0.0.255) `&] [*@3 img], [@(0.0.255) const] 
Rect[@(0.0.255) `&] [*@3 src], [@(0.0.255) int] [*@3 nx], [@(0.0.255) int] 
[*@3 ny], [@(0.0.255) bool] [*@3 co])&]
[s5;:Upp`:`:Magnify`(const Image`&`,int`,int`,bool`): Image [* Magnify]([@(0.0.255) const
] Image[@(0.0.255) `&] [*@3 img], [@(0.0.255) int] [*@3 nx], [@(0.0.255) int] 
[*@3 ny], [@(0.0.255) bool] [*@3 co] [@(0.0.255) `=] [@(0.0.255) false])&]
[s2;%% Multiplies the resolution of [%-*@3 img] by factors [%-*@3 nx] 
and [%-*@3 ny] by simply repeating the pixels (each pixel becomes 
[%-*@3 nx] x [%-*@3 ny] same color block). If present in overload, 
[%-*@3 src`_] represents the source are of picture, [%-*@3 co] activates 
parallel processing.&]
[s3;%% &]
[s4; &]
[s5;:Upp`:`:Minify`(const Upp`:`:Image`&`,int`,int`,bool`): [_^Upp`:`:Image^ Image]_[* Mi
nify]([@(0.0.255) const]_[_^Upp`:`:Image^ Image][@(0.0.255) `&]_[*@3 img], 
[@(0.0.255) int]_[*@3 nx], [@(0.0.255) int]_[*@3 ny], [@(0.0.255) bool]_[*@3 co]_`=_[@(0.0.255) f
alse])&]
[s2;%% Reduces the resolution of [%-*@3 img] by factors [%-*@3 nx] and 
[%-*@3 ny] by averaging pixel value (each [%-*@3 nx] x [%-*@3 ny] block 
of pixels is averaged and produces single pixel in the resulting 
image). If [%-*@3 co] is true, parallel processing is enabled.&]
[s3;%% &]
[s4; &]
[s5;:Upp`:`:MinifyCached`(const Upp`:`:Image`&`,int`,int`,bool`): [_^Upp`:`:Image^ Imag
e]_[* MinifyCached]([@(0.0.255) const]_[_^Upp`:`:Image^ Image][@(0.0.255) `&]_[*@3 img], 
[@(0.0.255) int]_[*@3 nx], [@(0.0.255) int]_[*@3 ny], [@(0.0.255) bool]_[*@3 co]_`=_[@(0.0.255) f
alse])&]
[s2;%% Same as Minify, but cached.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:Upscale2x`(const Upp`:`:Image`&`): [_^Upp`:`:Image^ Image]_[* Upscale2x]([@(0.0.255) c
onst]_[_^Upp`:`:Image^ Image][@(0.0.255) `&]_[*@3 src])&]
[s2;%% Doubles the resolution of image, using smart heuristics algorithm 
designed for upscaling synthetic images (like icons).&]
[s3;%% &]
[s4; &]
[s5;:Upp`:`:Downscale2x`(const Upp`:`:Image`&`): [_^Upp`:`:Image^ Image]_[* Downscale2x](
[@(0.0.255) const]_[_^Upp`:`:Image^ Image][@(0.0.255) `&]_[*@3 src])&]
[s2;%% Halves the resolution of image.&]
[s3;%% ]]