topic "RasterEncoder";
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
[{_}%EN-US 
[ {{10000@(113.42.0) [s0; [*@7;4 RasterEncoder]]}}&]
[s3;%- &]
[s1;:RasterEncoder`:`:class:%- [@(0.0.255)3 class][3 _][*3 RasterEncoder]&]
[s9; RasterEncoder serves as consumer of RGBA scanline data. Typical 
use is as encoder into one of common image file formats, like 
PNG. RasterEncoder provides an output buffer for client code 
(producer) to store the output RGBA scanline, when scanline is 
finished, producer calls WriteLine and RasterEncoder converts 
the RGBA scanline into specific format, according to [* format] 
protected member variable, then calls WriteLineRaw virtual method 
which performs the final processing (e.g. PNGEncoder stores it 
into PNG output stream).&]
[s9; RGBA optimization: There is a specific need to avoid redundant 
conversions and copies when the target is normal Image (with 
ImageEncoder). SetLine method can replace the default scanline 
buffer provided by RasterEncoder; that way it can use a pointer 
to ImageBuffer data and data are then can be stored directly 
into ImageBuffer (ImageEncoder calls SetLine in Start and WriteLineRaw 
methods). WriteLineRaw can then check whether [%-@3 data] points 
to suggested buffer (it is not guaranteed, because producer can 
provide yet another scanline buffer in WriteLine) and in that 
case, copying into target scanline can be completely skipped.&]
[s3; &]
[s0;%- &]
[ {{10000F(128)G(128)@1 [s0; [* Protected Members List]]}}&]
[s3;%- &]
[s5;:RasterEncoder`:`:format:%- [_^RasterFormat^ RasterFormat]_[* format]&]
[s2; Derived encoder should store the required conversion format 
here in Start method.&]
[s3;%- &]
[s4;%- &]
[s5;:RasterEncoder`:`:SetLine`(RGBA`*`):%- [@(0.0.255) void]_[* SetLine]([_^RGBA^ RGBA]_`*[*@3 `_
line])&]
[s2; Replaces the default scanline buffer to optimize storing RGBA 
data to direct targets..&]
[s3; &]
[s0; &]
[ {{10000F(128)G(128)@1 [s0; [* Public Method List]]}}&]
[s3; &]
[s5;:RasterEncoder`:`:GetPaletteCount`(`):%- [@(0.0.255) virtual] [@(0.0.255) int]_[* GetPa
letteCount]()&]
[s2; Returns the number of colors in the palette (or zero).&]
[s3; &]
[s4; &]
[s5;:RasterEncoder`:`:Start`(Size`):%- [@(0.0.255) virtual] [@(0.0.255) void]_[* Start]([_^Size^ S
ize]_[*@3 sz])_`=_[@3 0]&]
[s2; Called at the start of processing [%-*@3 sz].&]
[s3; &]
[s4; &]
[s5;:RasterEncoder`:`:WriteLineRaw`(const byte`*`):%- [@(0.0.255) virtual] 
[@(0.0.255) void]_[* WriteLineRaw]([@(0.0.255) const]_[_^byte^ byte]_`*[*@3 data])_`=_[@3 0]&]
[s2; Called to write a single scanline [%-*@3 data] to output.&]
[s3; &]
[s4; &]
[s5;:RasterEncoder`:`:WriteLine`(`):%- [@(0.0.255) void]_[* WriteLine]()&]
[s2; Encodes the internal RasterEncoder scanline buffer `- converts 
it to required format and calls WriteLineRaw.&]
[s3; &]
[s4; &]
[s5;:RasterEncoder`:`:WriteLine`(const RGBA`*`):%- [@(0.0.255) void]_[* WriteLine]([@(0.0.255) c
onst]_[_^RGBA^ RGBA]_`*[*@3 s])&]
[s2; Encodes the scanline [%-*@3 s] `- converts it to required format 
and calls WriteLineRaw.&]
[s3; &]
[s4; &]
[s5;:RasterEncoder`:`:GetFormat`(`)const:%- [@(0.0.255) const]_[_^RasterFormat^ RasterFor
mat][@(0.0.255) `&]_[* GetFormat]()_[@(0.0.255) const]&]
[s2; Returns current [* format].&]
[s3; &]
[s4; &]
[s5;:RasterEncoder`:`:GetPalette`(`):%- [@(0.0.255) const]_[_^RGBA^ RGBA]_`*[* GetPalette](
)&]
[s2; Returns current palette.&]
[s3; &]
[s4; &]
[s5;:RasterEncoder`:`:GetPaletteCv`(`):%- [@(0.0.255) const]_[_^PaletteCv^ PaletteCv]_`*[* G
etPaletteCv]()&]
[s2; Returns palette convertor.&]
[s3; &]
[s4; &]
[s5;:RasterEncoder`:`:operator RGBA`*`(`):%- [@(0.0.255) operator]_[_^RGBA^ RGBA]_`*()&]
[s5;:RasterEncoder`:`:operator`~`(`):%- [_^RGBA^ RGBA]_`*[* operator`~]()&]
[s2; Returns the pointer to scanline buffer that is used in parameterless 
WriteLine.&]
[s3; &]
[s4; &]
[s5;:RasterEncoder`:`:GetSize`(`)const:%- [_^Size^ Size]_[* GetSize]()_[@(0.0.255) const]&]
[s2; Returns the size.&]
[s3; &]
[s4; &]
[s5;:RasterEncoder`:`:GetWidth`(`)const:%- [@(0.0.255) int]_[* GetWidth]()_[@(0.0.255) cons
t]&]
[s2; Same as GetSize().cx.&]
[s3; &]
[s4; &]
[s5;:RasterEncoder`:`:GetHeight`(`)const:%- [@(0.0.255) int]_[* GetHeight]()_[@(0.0.255) co
nst]&]
[s2; Same as GetSize().cy.&]
[s3; &]
[s4; &]
[s5;:RasterEncoder`:`:SetHotSpot`(Point`):%- [@(0.0.255) void]_[* SetHotSpot]([_^Point^ Poi
nt]_[*@3 p])&]
[s2; Sets hotspot. This is useful for defining screen cursors (.ico 
or .cur files in Win32). The derived encoder might need the hotspot 
information (it reads it by calling GetHotSpot in to store it 
into the output file in Start).&]
[s3; &]
[s4; &]
[s5;:RasterEncoder`:`:GetHotSpot`(`)const:%- [_^Point^ Point]_[* GetHotSpot]()_[@(0.0.255) c
onst]&]
[s2; Reads hotspot set be SetHotSpot.&]
[s3; &]
[s4; &]
[s5;:RasterEncoder`:`:SetDots`(Size`):%- [@(0.0.255) void]_[* SetDots]([_^Size^ Size]_[*@3 `_
dots])&]
[s2; Sets the physical size of output image, in 1/600 of inch. The 
derived encoder might read this information in Start and store 
it into output file, if given format supports any related information.&]
[s3; &]
[s4; &]
[s5;:RasterEncoder`:`:GetDots`(`)const:%- [_^Size^ Size]_[* GetDots]()_[@(0.0.255) const]&]
[s2; Returns the size set by SetDots.&]
[s3; &]
[s4; &]
[s5;:RasterEncoder`:`:SetPalette`(const RGBA`*`):%- [@(0.0.255) void]_[* SetPalette]([@(0.0.255) c
onst]_[_^RGBA^ RGBA]_`*[*@3 palette])&]
[s2; Sets the [%-*@3 palette] to be used with raster.&]
[s3; &]
[s4; &]
[s5;:RasterEncoder`:`:SetPalette`(Raster`&`):%- [@(0.0.255) void]_[* SetPalette]([_^Raster^ R
aster][@(0.0.255) `&]_[*@3 pal`_raster])&]
[s2; Generates the optimal palette based on colors in [%-*@3 pal`_raster] 
and sets it for further processing.&]
[s3; &]
[s4; &]
[s5;:RasterEncoder`:`:Create`(Size`):%- [@(0.0.255) void]_[* Create]([_^Size^ Size]_[*@3 sz])
&]
[s5;:RasterEncoder`:`:Create`(int`,int`):%- [@(0.0.255) void]_[* Create]([@(0.0.255) int]_[*@3 c
x], [@(0.0.255) int]_[*@3 cy])&]
[s5;:RasterEncoder`:`:Create`(Size`,const RGBA`*`):%- [@(0.0.255) void]_[* Create]([_^Size^ S
ize]_[*@3 sz], [@(0.0.255) const]_[_^RGBA^ RGBA]_`*[*@3 palette])&]
[s5;:RasterEncoder`:`:Create`(int`,int`,const RGBA`*`):%- [@(0.0.255) void]_[* Create]([@(0.0.255) i
nt]_[*@3 cx], [@(0.0.255) int]_[*@3 cy], [@(0.0.255) const]_[_^RGBA^ RGBA]_`*[*@3 palette])&]
[s5;:RasterEncoder`:`:Create`(Size`,Raster`&`):%- [@(0.0.255) void]_[* Create]([_^Size^ Siz
e]_[*@3 sz], [_^Raster^ Raster][@(0.0.255) `&]_[*@3 pal`_raster])&]
[s5;:RasterEncoder`:`:Create`(int`,int`,Raster`&`):%- [@(0.0.255) void]_[* Create]([@(0.0.255) i
nt]_[*@3 cx], [@(0.0.255) int]_[*@3 cy], [_^Raster^ Raster][@(0.0.255) `&]_[*@3 pal`_raster])
&]
[s2; Initializes encoder for output of image of specified size, optionally 
setting the palette.&]
[s3; &]
[s0; ]