topic "Image rotation functions";
[ $$0,0#00000000000000000000000000000000:Default]
[H6;0 $$1,0#05600065144404261032431302351956:begin]
[i448;a25;kKO9;2 $$2,0#37138531426314131252341829483370:codeitem]
[l288;2 $$3,0#27521748481378242620020725143825:desc]
[0 $$4,0#96390100711032703541132217272105:end]
[{_}%EN-US 
[s0;%- &]
[ {{10000F(128)G(128)@1 [s0; [* Image rotation functions]]}}&]
[s1;%- &]
[s2;:PixRaster`:`:RotateAM`(double`,BringInModes`,int`):%- [@(0.0.255) bool]_[* RotateAM](
[@(0.0.255) double]_[*@3 angle], [@(0.0.255) BringInModes]_[*@3 incolor], 
[@(0.0.255) int]_[*@3 page])&]
[s3; Rotates image at [%-*@3 page] around its center by [%-*@3 angle] 
in radians, positive clockwise; created image pixels are filled 
in black or white depending on [%-*@3 incolor] parameter.&]
[s3; Image must be in 2, 4, 8 or 32 bpp format, either paletted or 
grayscale. The rotated image is appended to PixRaster and active 
page is set to it.&]
[s3; &]
[s3; Returns [%-@(0.0.255) true] on success, [%-@(0.0.255) false] otherwise.&]
[s4; &]
[s1;%- &]
[s2;:PixRaster`:`:RotateAMColor`(double`,int`,int`):%- [@(0.0.255) bool]_[* RotateAMColor
]([@(0.0.255) double]_[*@3 angle], [@(0.0.255) int]_[*@3 colorval], [@(0.0.255) int]_[*@3 pag
e]_`=_PIXRASTER`_CURPAGE)&]
[s3; Rotates image at [%-*@3 page] around its center by [%-*@3 angle] 
in radians, positive clockwise; created image pixels are color 
filled with [%-*@3 colorval].&]
[s3; Image must be in 2, 4, 8 or 32 bpp format, either paletted or 
grayscale. The rotated image is appended to PixRaster and active 
page is set to it.&]
[s3; &]
[s3; Returns [%-@(0.0.255) true] on success, [%-@(0.0.255) false] otherwise.&]
[s4; &]
[s1;%- &]
[s2;:PixRaster`:`:RotateAMGray`(double`,int`,int`):%- [@(0.0.255) bool]_[* RotateAMGray](
[@(0.0.255) double]_[*@3 angle], [@(0.0.255) int]_[*@3 grayval], [@(0.0.255) int]_[*@3 page]_
`=_PIXRASTER`_CURPAGE)&]
[s3; Rotates image at [%-*@3 page] around its center by [%-*@3 angle] 
in radians, positive clockwise; created image pixels are gray 
filled with [%-*@3 grayval].&]
[s3; Image must be in 2, 4, 8 or 32 bpp format, either paletted or 
grayscale. The rotated image is appended to PixRaster and active 
page is set to it.&]
[s3; &]
[s3; Returns [%-@(0.0.255) true] on success, [%-@(0.0.255) false] otherwise.&]
[s4; &]
[s0; ]