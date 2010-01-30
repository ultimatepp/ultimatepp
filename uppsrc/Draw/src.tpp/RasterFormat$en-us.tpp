topic "RasterFormat";
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
[ {{10000@(113.42.0) [s0;%% [*@7;4 RasterFormat]]}}&]
[s3; &]
[s1;:RasterFormat`:`:struct: [@(0.0.255)3 class][3 _][*3 RasterFormat]&]
[s9;%% This class serves as description and converter of various 
pixel formats. U`+`+ normally uses fixed 32`-bit RGBA format 
for all processing, but special formats are required e.g. to 
deal with image files or to reduce memory requirements.&]
[s3; &]
[s0;i448;a25;kKO9;@(0.0.255) &]
[ {{10000F(128)G(128)@1 [s0;%% [* Raster Types]]}}&]
[s3; &]
[ {{3523:6477^ [s0; RASTER`_1]
:: [s0; 1 bit per pixel (2 colors, with palette)]
:: [s0; RASTER`_2]
:: [s0; 2 bits per pixel (4 colors, with palette)]
:: [s0; RASTER`_4]
:: [s0; 4 bits per pixel (16 colors, with palette)]
:: [s0; RASTER`_8]
:: [s0; 8 bits per pixel (256 colors, with palette)]
:: [s0; RASTER`_8ALPHA]
:: [s0; 16 bits per pixel; 8 bits for color (256 colors, with palette) 
and 8 bits for alpha]
:: [s0; RASTER`_16]
:: [s0; 16 bits per pixel (65536 colors without palette), number of 
bits and placements for individual channels depends on other 
variables]
:: [s0; RASTER`_24]
:: [s0; 24 bits per pixel RGB (truecolor), number of bits and placements 
for individual channels depends on other variables]
:: [s0; RASTER`_32]
:: [s0; 32 bits per pixel RGB (truecolor), number of bits and placements 
for individual channels depends on other variables]
:: [s0; RASTER`_32ALPHA]
:: [s0; 32 bits per pixel RGBA (truecolor), alpha is not premultiplied, 
number of bits and placements for individual channels depends 
on other variables]
:: [s0; RASTER`_32PREMULTIPLIED]
:: [s0; 32 bits per pixel RGBA (truecolor), alpha is premultiplied, 
number of bits and placements for individual channels depends 
on other variables]}}&]
[s0;i448;a25;kKO9;@(0.0.255) &]
[s0;i448;a25;kKO9; All these values can be combined (using operator`|) 
with&]
[s0;i448;a25;kKO9;@(0.0.255) &]
[s0;i448;a25;kKO9; RASTER`_MSBFIRST-|-|Indicates that bytes or nibbles 
are in reversed order (in big endian).&]
[s3; &]
[s0; &]
[ {{10000F(128)G(128)@1 [s0;%% [* Public Method List]]}}&]
[s3; &]
[s5;:RasterFormat`:`:Set1lf`(`): [@(0.0.255) void]_[* Set1lf]()&]
[s2; Sets 1 bit per pixel format (RASTER`_1).&]
[s3; &]
[s4; &]
[s5;:RasterFormat`:`:Set1mf`(`): [@(0.0.255) void]_[* Set1mf]()&]
[s2;%% Sets 1 bit per pixel format in reversed MSB mode (RASTER`_1`|RASTER`_MSBFIRST)
.&]
[s3; &]
[s4; &]
[s5;:RasterFormat`:`:Set2lf`(`): [@(0.0.255) void]_[* Set2lf]()&]
[s2;%% Sets 2 bits per pixel format (RASTER`_2).&]
[s3; &]
[s4; &]
[s5;:RasterFormat`:`:Set2mf`(`): [@(0.0.255) void]_[* Set2mf]()&]
[s2;%% Sets 2 bits per pixel format in reversed MSB mode (RASTER`_2`|RASTER`_MSBFIRST
).&]
[s3; &]
[s4; &]
[s5;:RasterFormat`:`:Set4lf`(`): [@(0.0.255) void]_[* Set4lf]()&]
[s2;%% Sets 4 bits per pixel format (RASTER`_4).&]
[s3; &]
[s4; &]
[s5;:RasterFormat`:`:Set4mf`(`): [@(0.0.255) void]_[* Set4mf]()&]
[s2;%% Sets 4 bits per pixel format in reversed MSB mode (RASTER`_4`|RASTER`_MSBFIRST
).&]
[s3; &]
[s4; &]
[s5;:RasterFormat`:`:Set8`(`): [@(0.0.255) void]_[* Set8]()&]
[s2;%% Sets 8 bits per pixel format (RASTER`_8).&]
[s3; &]
[s4; &]
[s5;:RasterFormat`:`:Set8A`(`): [@(0.0.255) void]_[* Set8A]()&]
[s2;%% Sets 8`+8 bits per pixel format, with 8 bits for color with 
palette and 8 bits for alpha (RASTER`_8ALPHA).&]
[s3; &]
[s4; &]
[s5;:RasterFormat`:`:Set16le`(dword`,dword`,dword`): [@(0.0.255) void]_[* Set16le]([_^dword^ d
word]_[*@3 rmask], [_^dword^ dword]_[*@3 gmask], [_^dword^ dword]_[*@3 bmask])&]
[s2;%% Sets 16 bits per pixel format with channel placement defined 
by channel masks in little`-endian mode (RASTER`_16).&]
[s3;%% &]
[s4;%% &]
[s5;:RasterFormat`:`:Set16be`(dword`,dword`,dword`): [@(0.0.255) void]_[* Set16be]([_^dword^ d
word]_[*@3 rmask], [_^dword^ dword]_[*@3 gmask], [_^dword^ dword]_[*@3 bmask])&]
[s2;%% Sets 16 bits per pixel format with channel placement defined 
by channel masks in big`-endian mode (RASTER`_16`|RASTER`_MSBFIRST).&]
[s3;%% &]
[s4;%% &]
[s5;:RasterFormat`:`:Set24le`(dword`,dword`,dword`): [@(0.0.255) void]_[* Set24le]([_^dword^ d
word]_[*@3 rmask], [_^dword^ dword]_[*@3 gmask], [_^dword^ dword]_[*@3 bmask])&]
[s2;%% Sets 24 bits per pixel format with channel placement defined 
by channel masks in little`-endian mode (RASTER`_24).&]
[s3;%% &]
[s4;%% &]
[s5;:RasterFormat`:`:Set24be`(dword`,dword`,dword`): [@(0.0.255) void]_[* Set24be]([_^dword^ d
word]_[*@3 rmask], [_^dword^ dword]_[*@3 gmask], [_^dword^ dword]_[*@3 bmask])&]
[s2;%% Sets 24 bits per pixel format with channel placement defined 
by channel masks in big`-endian mode (RASTER`_24`|RASTER`_MSBFIRST).&]
[s3;%% &]
[s4;%% &]
[s5;:RasterFormat`:`:Set32le`(dword`,dword`,dword`,dword`): [@(0.0.255) void]_[* Set32le](
[_^dword^ dword]_[*@3 rmask], [_^dword^ dword]_[*@3 gmask], [_^dword^ dword]_[*@3 bmask], 
[_^dword^ dword]_[*@3 amask]_`=_[@3 0])&]
[s2;%% Sets 32 bits per pixel format with channel placement defined 
by channel masks in little`-endian mode. If [%-*@3 amask ]is not 
zero, format is with premultiplied alpha (RASTER`_32PREMULTIPLIED), 
otherwise it is without alpha (RASTER`_32).&]
[s3;%% &]
[s4;%% &]
[s5;:RasterFormat`:`:Set32be`(dword`,dword`,dword`,dword`): [@(0.0.255) void]_[* Set32be](
[_^dword^ dword]_[*@3 rmask], [_^dword^ dword]_[*@3 gmask], [_^dword^ dword]_[*@3 bmask], 
[_^dword^ dword]_[*@3 amask]_`=_[@3 0])&]
[s2;%% Sets 32 bits per pixel format with channel placement defined 
by channel masks in big`-endian mode. If [%-*@3 amask ]is not zero, 
format is with premultiplied alpha (RASTER`_32PREMULTIPLIED`|RASTER`_MSBFIRST), 
otherwise it is without alpha (RASTER`_32`|RASTER`_MSBFIRST).&]
[s3;%% &]
[s4;%% &]
[s5;:RasterFormat`:`:Set32leStraight`(dword`,dword`,dword`,dword`): [@(0.0.255) void]_[* S
et32leStraight]([_^dword^ dword]_[*@3 rmask], [_^dword^ dword]_[*@3 gmask], 
[_^dword^ dword]_[*@3 bmask], [_^dword^ dword]_[*@3 amask])&]
[s2;%% Sets 32 bits per pixel format with channel placement defined 
by channel masks in little`-endian mode with non`-premultiplied 
alpha (RASTER`_32ALPHA).&]
[s3;%% &]
[s4;%% &]
[s5;:RasterFormat`:`:Set32beStraight`(dword`,dword`,dword`,dword`): [@(0.0.255) void]_[* S
et32beStraight]([_^dword^ dword]_[*@3 rmask], [_^dword^ dword]_[*@3 gmask], 
[_^dword^ dword]_[*@3 bmask], [_^dword^ dword]_[*@3 amask])&]
[s2;%% Sets 32 bits per pixel format with channel placement defined 
by channel masks in little`-endian mode with non`-premultiplied 
alpha (RASTER`_32ALPHA`|RASTER`_MSBFIRST).&]
[s3;%% &]
[s4;%% &]
[s5;:RasterFormat`:`:SetRGBA`(`): [@(0.0.255) void]_[* SetRGBA]()&]
[s2;%% Sets the mode that is bitwise equivalent of standard RGBA 
quads used in Image.&]
[s3;%% &]
[s4;%% &]
[s5;:RasterFormat`:`:SetRGBAStraight`(`): [@(0.0.255) void]_[* SetRGBAStraight]()&]
[s2;%% Similar to SetRGBA, but with non`-premultiplied alpha.&]
[s3;%% &]
[s4;%% &]
[s5;:RasterFormat`:`:GetType`(`)const: [@(0.0.255) int]_[* GetType]()_[@(0.0.255) const]&]
[s2;%% Returns the current format.&]
[s3;%% &]
[s4;%% &]
[s5;:RasterFormat`:`:IsRGBA`(`)const: [@(0.0.255) int]_[* IsRGBA]()_[@(0.0.255) const]&]
[s2;%% True if format is bitwise equivalent of standard RGBA quads 
used in Image.&]
[s3;%% &]
[s4;%% &]
[s5;:RasterFormat`:`:GetByteCount`(int`)const: [@(0.0.255) int]_[* GetByteCount]([@(0.0.255) i
nt]_[*@3 cx])_[@(0.0.255) const]&]
[s2;%% Returns a number of bytes required for [%-*@3 cx] pixels in 
current format.&]
[s3;%% &]
[s4;%% &]
[s5;:RasterFormat`:`:GetBpp`(`)const: [@(0.0.255) int]_[* GetBpp]()_[@(0.0.255) const]&]
[s2;%% Returns the number of bits per pixel of current format.&]
[s3;%% &]
[s4;%% &]
[s5;:RasterFormat`:`:HasAlpha`(`)const: [@(0.0.255) bool]_[* HasAlpha]()_[@(0.0.255) const]&]
[s2;%% True if current format has alpha.&]
[s3;%% &]
[s4;%% &]
[s5;:RasterFormat`:`:GetColorCount`(`)const: [@(0.0.255) int]_[* GetColorCount]()_[@(0.0.255) c
onst]&]
[s2;%% Returns the number of colors the current format supports.&]
[s3;%% &]
[s4;%% &]
[s5;:RasterFormat`:`:GetPaletteCount`(`)const: [@(0.0.255) int]_[* GetPaletteCount]()_[@(0.0.255) c
onst]&]
[s2;%% Returns the palette size of current format. If format does 
not have palette, 0 is returned.&]
[s3;%% &]
[s4;%% &]
[s5;:RasterFormat`:`:Read`(RGBA`*`,const byte`*`,int`,const RGBA`*`)const: [@(0.0.255) v
oid]_[* Read]([_^RGBA^ RGBA]_`*[*@3 t], [@(0.0.255) const]_[_^byte^ byte]_`*[*@3 s], 
[@(0.0.255) int]_[*@3 cx], [@(0.0.255) const]_[_^RGBA^ RGBA]_`*[*@3 palette])_[@(0.0.255) con
st]&]
[s2;%% Converts scanline [%-*@3 s] of [%-*@3 cx] pixels in current format 
to regular RGBA scanline [%-*@3 t] using [%-*@3 palette] (ignored 
for 16 bit and truecolor formats). If format does not have alpha, 
it is assigned 255 (opaque) to all target pixels.&]
[s3;%% &]
[s4;%% &]
[s5;:RasterFormat`:`:Write`(byte`*`,const RGBA`*`,int`,const PaletteCv`*`)const: [@(0.0.255) v
oid]_[* Write]([_^byte^ byte]_`*[*@3 t], [@(0.0.255) const]_[_^RGBA^ RGBA]_`*[*@3 s], 
[@(0.0.255) int]_[*@3 cx], [@(0.0.255) const]_[_^PaletteCv^ PaletteCv]_`*[*@3 palcv])_[@(0.0.255) c
onst]&]
[s2;%% Converts regular RGBA scanline [%-*@3 s] of [%-*@3 cx] pixels 
to scanline [%-*@3 t] in current format using [%-*@3 palcv] to convert 
truecolor to palette formats (ignored for 16 bit and truecolor 
formats).&]
[s3; &]
[s0; ]