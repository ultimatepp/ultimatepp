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
[s3; This function flushes the content of filtered subpixel buffer.&]
[s0; Important note: due to rounding errors during filtering, sometimes 
the subpixel value can be 257. This is remedied by using 257 
instead of 256 in the pixel blending command.&]
[s0; &]
[s4; &]
[s0; ]