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
[s2;:Pix`:`:DitherToBinary`(`):%- [@(0.0.255) Pix]_[* DitherToBinary]()&]
[s3;  &]
[s4; &]
[s1;%- &]
[s2;:Pix`:`:DitherToBinarySpec`(int`,int`):%- [@(0.0.255) Pix]_[* DitherToBinarySpec]([@(0.0.255) i
nt]_[*@3 lowerclip], [@(0.0.255) int]_[*@3 upperclip])&]
[s3;  [%-*@3 lowerclip] [%-*@3 upperclip] &]
[s4; &]
[s1;%- &]
[s2;:Pix`:`:ThresholdToBinary`(int`):%- [@(0.0.255) Pix]_[* ThresholdToBinary]([@(0.0.255) i
nt]_[*@3 threshold])&]
[s3; Creates a new 1bpp black/white image from source`'s grayscale 
using [%-*@3 threshold] as limiting value.&]
[s3; [* WARNING], it operates [* ONLY ]on grayscale source images.&]
[s3; &]
[s3; Returns thresholded [* Pix ]on success, an empty [* Pix ]on failure 
.&]
[s4; &]
[s1;%- &]
[s2;:Pix`:`:VarThresholdToBinary`(Pix`&`):%- [@(0.0.255) Pix]_[* VarThresholdToBinary]([@(0.0.255) P
ix`&]_[*@3 thresholdPix])&]
[s3;  [%-*@3 thresholdPix].&]
[s4; &]
[s1;%- &]
[s2;:Pix`:`:DitherToBinaryLUT`(int`,int`):%- [@(0.0.255) Pix]_[* DitherToBinaryLUT]([@(0.0.255) i
nt]_[*@3 lowerclip], [@(0.0.255) int]_[*@3 upperclip])&]
[s3;  [%-*@3 lowerclip] [%-*@3 upperclip].&]
[s4; &]
[s1;%- &]
[s2;:Pix`:`:GenerateMaskByValue`(int`):%- [@(0.0.255) Pix]_[* GenerateMaskByValue]([@(0.0.255) i
nt]_[*@3 val])&]
[s3;  [%-*@3 val].&]
[s4; &]
[s1;%- &]
[s2;:Pix`:`:GenerateMaskByBand`(int`,int`,int`):%- [@(0.0.255) Pix]_[* GenerateMaskByBand
]([@(0.0.255) int]_[*@3 lower], [@(0.0.255) int]_[*@3 upper], [@(0.0.255) int]_[*@3 inband])&]
[s3;  [%-*@3 lower] [%-*@3 upper] [%-*@3 inband].&]
[s4; &]
[s1;%- &]
[s2;:Pix`:`:DitherTo2bpp`(int`):%- [@(0.0.255) Pix]_[* DitherTo2bpp]([@(0.0.255) int]_[*@3 cm
apflag])&]
[s3;  [%-*@3 cmapflag].&]
[s4; &]
[s1;%- &]
[s2;:Pix`:`:DitherTo2bppSpec`(int`,int`,int`):%- [@(0.0.255) Pix]_[* DitherTo2bppSpec]([@(0.0.255) i
nt]_[*@3 lowerclip], [@(0.0.255) int]_[*@3 upperclip], [@(0.0.255) int]_[*@3 cmapflag])&]
[s3;  [%-*@3 lowerclip] [%-*@3 upperclip] [%-*@3 cmapflag].&]
[s4; &]
[s1;%- &]
[s2;:Pix`:`:ThresholdTo2bpp`(int`,int`):%- [@(0.0.255) Pix]_[* ThresholdTo2bpp]([@(0.0.255) i
nt]_[*@3 nlevels], [@(0.0.255) int]_[*@3 cmapflag])&]
[s3;  [%-*@3 nlevels] [%-*@3 cmapflag].&]
[s4; &]
[s1;%- &]
[s2;:Pix`:`:ThresholdTo4bpp`(int`,int`):%- [@(0.0.255) Pix]_[* ThresholdTo4bpp]([@(0.0.255) i
nt]_[*@3 nlevels], [@(0.0.255) int]_[*@3 cmapflag])&]
[s3;  [%-*@3 nlevels] [%-*@3 cmapflag].&]
[s4; &]
[s1;%- &]
[s2;:Pix`:`:ThresholdOn8bpp`(int`,int`):%- [@(0.0.255) Pix]_[* ThresholdOn8bpp]([@(0.0.255) i
nt]_[*@3 nlevels], [@(0.0.255) int]_[*@3 cmapflag])&]
[s3;  [%-*@3 nlevels] [%-*@3 cmapflag].&]
[s4; &]
[s1;%- &]
[s2;:Pix`:`:ThresholdGrayArb`(const char`*`,int`,int`,int`,int`):%- [@(0.0.255) Pix]_[* T
hresholdGrayArb]([@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 edgevals], 
[@(0.0.255) int]_[*@3 outdepth], [@(0.0.255) int]_[*@3 use`_average], 
[@(0.0.255) int]_[*@3 setblack], [@(0.0.255) int]_[*@3 setwhite])&]
[s3;  [%-*@3 edgevals] [%-*@3 outdepth] [%-*@3 use`_average] [%-*@3 setblack] 
[%-*@3 setwhite].&]
[s4; &]
[s1;%- &]
[s2;:Pix`:`:MakeGrayQuantIndexTable`(int`):%- [_^Buffer^ Buffer]<[@(0.0.255) int]>_[* MakeG
rayQuantIndexTable]([@(0.0.255) int]_[*@3 nlevels])&]
[s3;  [%-*@3 nlevels].&]
[s4; &]
[s1;%- &]
[s2;:Pix`:`:MakeGrayQuantTargetTable`(int`,int`):%- [_^Buffer^ Buffer]<[@(0.0.255) int]>_
[* MakeGrayQuantTargetTable]([@(0.0.255) int]_[*@3 nlevels], [@(0.0.255) int]_[*@3 depth])&]
[s3;  [%-*@3 nlevels] [%-*@3 depth].&]
[s4; &]
[s1;%- &]
[s2;:Pix`:`:GenerateMaskByBand32`(unsigned`,int`,int`):%- [@(0.0.255) Pix]_[* GenerateMas
kByBand32]([@(0.0.255) unsigned]_[*@3 refval], [@(0.0.255) int]_[*@3 delm], 
[@(0.0.255) int]_[*@3 delp])&]
[s3;  [%-*@3 refval] [%-*@3 delm] [%-*@3 delp].&]
[s4; &]
[s1;%- &]
[s2;:Pix`:`:GenerateMaskByDiscr32`(unsigned`,unsigned`,int`):%- [@(0.0.255) Pix]_[* Gener
ateMaskByDiscr32]([@(0.0.255) unsigned]_[*@3 refval1], [@(0.0.255) unsigned]_[*@3 refval2
], [@(0.0.255) int]_[*@3 distflag])&]
[s3;  [%-*@3 refval1] [%-*@3 refval2] [%-*@3 distflag].&]
[s4; &]
[s1;%- &]
[s2;:Pix`:`:GrayQuantFromHisto`(Pix`&`,double`,int`):%- [@(0.0.255) Pix]_[* GrayQuantFrom
Histo]([@(0.0.255) Pix `&][*@3 mPaix], [@(0.0.255) double]_[*@3 minfract], 
[@(0.0.255) int]_[*@3 maxsize])&]
[s3;  [%-*@3 mPix] [%-*@3 minfract] [%-*@3 maxsize].&]
[s4; &]
[s1;%- &]
[s2;:Pix`:`:ThresholdToValue`(int`,int`):%- [@(0.0.255) Pix]_[* ThresholdToValue]([@(0.0.255) i
nt]_[*@3 threshval], [@(0.0.255) int]_[*@3 setval])&]
[s3;  [%-*@3 threshval] [%-*@3 setval].&]
[s4; &]
[s0; ]