topic "";
[ $$0,0#00000000000000000000000000000000:Default]
[H6;0 $$1,0#05600065144404261032431302351956:begin]
[i448;a25;kKO9;2 $$2,0#37138531426314131252341829483370:codeitem]
[l288;2 $$3,0#27521748481378242620020725143825:desc]
[0 $$4,0#96390100711032703541132217272105:end]
[{_}%EN-US 
[s1;%- &]
[s2;:SubpixelFiller`:`:Write`(int`):%- [@(0.0.255) void]_[* Write]([@(0.0.255) int]_[*@3 len])
&]
[s3; This function writes the filtered subpixel buffer to RGBA output, 
performing per`-channel (subpixel) alpha blending. Note that 
it is only possible to do correct per`-channel blending when 
target is opaque (a `=`= 255). If it is not, normal anti`-aliasing 
is used, using average.&]
[s3; Important note: due to rounding errors during filtering, sometimes 
the subpixel value can be 257. This is remedied by using 257 
instead of 256 in the pixel blending command.&]
[s0; &]
[s4; &]
[s1;%- &]
[s2;:SubpixelFiller`:`:RenderN`(int`,int`,int`):%- [@(0.0.255) void]_[* RenderN]([@(0.0.255) i
nt]_[*@3 val], [@(0.0.255) int]_[*@3 h], [@(0.0.255) int]_[*@3 n])&]
[s3; This function adds 1 `- 6 val values to filtered subpixel buffer 
in single pass (6 is just enough needed for Render optimizations). 
Case variants are created by adding shifted Render filtering 
procedures.&]
[s4; &]
[s0; ]]