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
[s2;:Pix`:`:RotateAM`(double`,BringInModes`):%- [@(0.0.255) Pix]_[* RotateAM]([@(0.0.255) d
ouble]_[*@3 angle], [@(0.0.255) BringInModes]_[*@3 incolor])&]
[s3; Rotates image around its center by [%-*@3 angle] in radians, positive 
clockwise; created image pixels are filled in black or white 
depending on [%-*@3 incolor] parameter.&]
[s3; Image must be in 2, 4, 8 or 32 bpp format, either paletted or 
grayscale. The rotated image is appended to PixRaster and active 
page is set to it.&]
[s3; &]
[s3; Returns rotated [* Pix ]on success,an empty [* Pix ]otherwise.&]
[s4; &]
[s1;%- &]
[s2;:Pix`:`:RotateAMColor`(double`,int`):%- [@(0.0.255) Pix]_[* RotateAMColor]([@(0.0.255) d
ouble]_[*@3 angle], [@(0.0.255) int]_[*@3 colorval])&]
[s3; Rotates image around its center by [%-*@3 angle] in radians, positive 
clockwise; created image pixels are color filled with [%-*@3 colorval].&]
[s3; Image must be in 2, 4, 8 or 32 bpp format, either paletted or 
grayscale. The rotated image is appended to PixRaster and active 
page is set to it.&]
[s3; &]
[s3; Returns rotated [* Pix ]on success,an empty [* Pix ]otherwise.&]
[s4; &]
[s1;%- &]
[s2;:Pix`:`:RotateAMGray`(double`,int`):%- [@(0.0.255) Pix]_[* RotateAMGray]([@(0.0.255) do
uble]_[*@3 angle], [@(0.0.255) int]_[*@3 grayval])&]
[s3; Rotates image around its center by [%-*@3 angle] in radians, positive 
clockwise; created image pixels are gray filled with [%-*@3 grayval].&]
[s3; Image must be in 2, 4, 8 or 32 bpp format, either paletted or 
grayscale. The rotated image is appended to PixRaster and active 
page is set to it.&]
[s3; &]
[s3; Returns rotated [* Pix ]on success,an empty [* Pix ]otherwise.&]
[s4; &]
[s0; ]