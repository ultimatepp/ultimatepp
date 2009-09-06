topic "Image thresholding functions";
[ $$0,0#00000000000000000000000000000000:Default]
[H6;0 $$1,0#05600065144404261032431302351956:begin]
[i448;a25;kKO9;2 $$2,0#37138531426314131252341829483370:codeitem]
[l288;2 $$3,0#27521748481378242620020725143825:desc]
[0 $$4,0#96390100711032703541132217272105:end]
[{_}%EN-US 
[s0;%- &]
[ {{10000F(128)G(128)@1 [s0; [* Image thresholding functions]]}}&]
[s1;%- &]
[s1;%- &]
[s2;:PixRaster`:`:DitherToBinary`(int`):%- [@(0.0.255) bool]_[* DitherToBinary]([@(0.0.255) i
nt]_[*@3 page]_`=_PIXRASTER`_CURPAGE)&]
[s3;  [%-*@3 page].&]
[s4; &]
[s1;%- &]
[s2;:PixRaster`:`:DitherToBinarySpec`(int`,int`,int`):%- [@(0.0.255) bool]_[* DitherToBin
arySpec]([@(0.0.255) int]_[*@3 lowerclip], [@(0.0.255) int]_[*@3 upperclip], 
[@(0.0.255) int]_[*@3 page]_`=_PIXRASTER`_CURPAGE)&]
[s3;  [%-*@3 lowerclip] [%-*@3 upperclip] [%-*@3 page].&]
[s4; &]
[s1;%- &]
[s2;:PixRaster`:`:ThresholdToBinary`(int`,int`):%- [@(0.0.255) bool]_[* ThresholdToBinary
]([@(0.0.255) int]_[*@3 threshold], [@(0.0.255) int]_[*@3 page]_`=_PIXRASTER`_CURPAGE)&]
[s3; Creates a new 1bpp black/white image from source`'s grayscale 
[%-*@3 page] using [%-*@3 threshold] as limiting value. Newly created 
image is appended at the end of PixRaster, and currently active 
page is set to it.&]
[s3; [* WARNING], it operates [* ONLY ]on grayscale source images.&]
[s3; Thresholded image is appended at the end of PixRaster&]
[s3; &]
[s3; Returns [%-@(0.0.255) true] on success, [%-@(0.0.255) false] otherwise.&]
[s4; &]
[s1;%- &]
[s2;:PixRaster`:`:VarThresholdToBinary`(int`,int`):%- [@(0.0.255) bool]_[* VarThresholdTo
Binary]([@(0.0.255) int]_[*@3 thresholdPage], [@(0.0.255) int]_[*@3 page]_`=_PIXRASTER`_C
URPAGE)&]
[s3;  [%-*@3 thresholdPage] [%-*@3 page].&]
[s4; &]
[s1;%- &]
[s2;:PixRaster`:`:DitherToBinaryLUT`(int`,int`,int`):%- [@(0.0.255) bool]_[* DitherToBina
ryLUT]([@(0.0.255) int]_[*@3 lowerclip], [@(0.0.255) int]_[*@3 upperclip], 
[@(0.0.255) int]_[*@3 page]_`=_PIXRASTER`_CURPAGE)&]
[s3;  [%-*@3 lowerclip] [%-*@3 upperclip] [%-*@3 page].&]
[s4; &]
[s1;%- &]
[s2;:PixRaster`:`:GenerateMaskByValue`(int`,int`):%- [@(0.0.255) bool]_[* GenerateMaskByV
alue]([@(0.0.255) int]_[*@3 val], [@(0.0.255) int]_[*@3 page]_`=_PIXRASTER`_CURPAGE)&]
[s3;  [%-*@3 val] [%-*@3 page].&]
[s4; &]
[s1;%- &]
[s2;:PixRaster`:`:GenerateMaskByBand`(int`,int`,int`,int`):%- [@(0.0.255) bool]_[* Genera
teMaskByBand]([@(0.0.255) int]_[*@3 lower], [@(0.0.255) int]_[*@3 upper], 
[@(0.0.255) int]_[*@3 inband], [@(0.0.255) int]_[*@3 page]_`=_PIXRASTER`_CURPAGE)&]
[s3;  [%-*@3 lower] [%-*@3 upper] [%-*@3 inband] [%-*@3 page].&]
[s4; &]
[s1;%- &]
[s2;:PixRaster`:`:DitherTo2bpp`(int`,int`):%- [@(0.0.255) bool]_[* DitherTo2bpp]([@(0.0.255) i
nt]_[*@3 cmapflag], [@(0.0.255) int]_[*@3 page]_`=_PIXRASTER`_CURPAGE)&]
[s3;  [%-*@3 cmapflag] [%-*@3 page].&]
[s4; &]
[s1;%- &]
[s2;:PixRaster`:`:DitherTo2bppSpec`(int`,int`,int`,int`):%- [@(0.0.255) bool]_[* DitherTo
2bppSpec]([@(0.0.255) int]_[*@3 lowerclip], [@(0.0.255) int]_[*@3 upperclip], 
[@(0.0.255) int]_[*@3 cmapflag], [@(0.0.255) int]_[*@3 page]_`=_PIXRASTER`_CURPAGE)&]
[s3;  [%-*@3 lowerclip] [%-*@3 upperclip] [%-*@3 cmapflag] [%-*@3 page].&]
[s4; &]
[s1;%- &]
[s2;:PixRaster`:`:ThresholdTo2bpp`(int`,int`,int`):%- [@(0.0.255) bool]_[* ThresholdTo2bp
p]([@(0.0.255) int]_[*@3 nlevels], [@(0.0.255) int]_[*@3 cmapflag], [@(0.0.255) int]_[*@3 pag
e]_`=_PIXRASTER`_CURPAGE)&]
[s3;  [%-*@3 nlevels] [%-*@3 cmapflag] [%-*@3 page].&]
[s4; &]
[s1;%- &]
[s2;:PixRaster`:`:ThresholdTo4bpp`(int`,int`,int`):%- [@(0.0.255) bool]_[* ThresholdTo4bp
p]([@(0.0.255) int]_[*@3 nlevels], [@(0.0.255) int]_[*@3 cmapflag], [@(0.0.255) int]_[*@3 pag
e]_`=_PIXRASTER`_CURPAGE)&]
[s3;  [%-*@3 nlevels] [%-*@3 cmapflag] [%-*@3 page].&]
[s4; &]
[s1;%- &]
[s2;:PixRaster`:`:ThresholdOn8bpp`(int`,int`,int`):%- [@(0.0.255) bool]_[* ThresholdOn8bp
p]([@(0.0.255) int]_[*@3 nlevels], [@(0.0.255) int]_[*@3 cmapflag], [@(0.0.255) int]_[*@3 pag
e]_`=_PIXRASTER`_CURPAGE)&]
[s3;  [%-*@3 nlevels] [%-*@3 cmapflag] [%-*@3 page].&]
[s4; &]
[s1;%- &]
[s2;:PixRaster`:`:ThresholdGrayArb`(const char`*`,int`,int`,int`,int`,int`):%- [@(0.0.255) b
ool]_[* ThresholdGrayArb]([@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 edgevals], 
[@(0.0.255) int]_[*@3 outdepth], [@(0.0.255) int]_[*@3 use`_average], 
[@(0.0.255) int]_[*@3 setblack], [@(0.0.255) int]_[*@3 setwhite], [@(0.0.255) int]_[*@3 page]_
`=_PIXRASTER`_CURPAGE)&]
[s3;  [%-*@3 edgevals] [%-*@3 outdepth] [%-*@3 use`_average] [%-*@3 setblack] 
[%-*@3 setwhite] [%-*@3 page].&]
[s4; &]
[s1;%- &]
[s2;:PixRaster`:`:MakeGrayQuantIndexTable`(int`):%- [_^Buffer^ Buffer]<[@(0.0.255) int]>_
[* MakeGrayQuantIndexTable]([@(0.0.255) int]_[*@3 nlevels])&]
[s3;  [%-*@3 nlevels].&]
[s4; &]
[s1;%- &]
[s2;:PixRaster`:`:MakeGrayQuantTargetTable`(int`,int`):%- [_^Buffer^ Buffer]<[@(0.0.255) i
nt]>_[* MakeGrayQuantTargetTable]([@(0.0.255) int]_[*@3 nlevels], [@(0.0.255) int]_[*@3 dep
th])&]
[s3;  [%-*@3 nlevels] [%-*@3 depth].&]
[s4; &]
[s1;%- &]
[s2;:PixRaster`:`:GenerateMaskByBand32`(unsigned`,int`,int`,int`):%- [@(0.0.255) bool]_
[* GenerateMaskByBand32]([@(0.0.255) unsigned]_[*@3 refval], [@(0.0.255) int]_[*@3 delm], 
[@(0.0.255) int]_[*@3 delp], [@(0.0.255) int]_[*@3 page]_`=_PIXRASTER`_CURPAGE)&]
[s3;  [%-*@3 refval] [%-*@3 delm] [%-*@3 delp] [%-*@3 page].&]
[s4; &]
[s1;%- &]
[s2;:PixRaster`:`:GenerateMaskByDiscr32`(unsigned`,unsigned`,int`,int`):%- [@(0.0.255) b
ool]_[* GenerateMaskByDiscr32]([@(0.0.255) unsigned]_[*@3 refval1], 
[@(0.0.255) unsigned]_[*@3 refval2], [@(0.0.255) int]_[*@3 distflag], 
[@(0.0.255) int]_[*@3 page]_`=_PIXRASTER`_CURPAGE)&]
[s3;  [%-*@3 refval1] [%-*@3 refval2] [%-*@3 distflag] [%-*@3 page].&]
[s4; &]
[s1;%- &]
[s2;:PixRaster`:`:GrayQuantFromHisto`(int`,double`,int`,int`):%- [@(0.0.255) bool]_[* Gra
yQuantFromHisto]([@(0.0.255) int]_[*@3 mPage], [@(0.0.255) double]_[*@3 minfract], 
[@(0.0.255) int]_[*@3 maxsize], [@(0.0.255) int]_[*@3 page]_`=_PIXRASTER`_CURPAGE)&]
[s3;  [%-*@3 mPage] [%-*@3 minfract] [%-*@3 maxsize] [%-*@3 page].&]
[s4; &]
[s1;%- &]
[s2;:PixRaster`:`:ThresholdToValue`(int`,int`,int`):%- [@(0.0.255) bool]_[* ThresholdToVa
lue]([@(0.0.255) int]_[*@3 threshval], [@(0.0.255) int]_[*@3 setval], 
[@(0.0.255) int]_[*@3 page]_`=_PIXRASTER`_CURPAGE)&]
[s3;  [%-*@3 threshval] [%-*@3 setval] [%-*@3 page].&]
[s4; &]
[s0;%- ]