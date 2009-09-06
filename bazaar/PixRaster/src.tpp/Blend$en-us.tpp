topic "Combining/blending functions";
[ $$0,0#00000000000000000000000000000000:Default]
[H6;0 $$1,0#05600065144404261032431302351956:begin]
[i448;a25;kKO9;2 $$2,0#37138531426314131252341829483370:codeitem]
[l288;2 $$3,0#27521748481378242620020725143825:desc]
[0 $$4,0#96390100711032703541132217272105:end]
[{_}%EN-US 
[s0;%- &]
[ {{10000F(128)G(128)@1 [s0; [* Combining/blending functions]]}}&]
[s1;%- &]
[s2;:PixRaster`:`:CombineMasked`(int`,int`,int`):%- [@(0.0.255) bool]_[* CombineMasked]([@(0.0.255) i
nt]_[*@3 destPage], [@(0.0.255) int]_[*@3 sourcePage], [@(0.0.255) int]_[*@3 maskPage])&]
[s3; Creates an image from [%-*@3 destPage] with pixels replaced by 
[%-*@3 sourcePage]`'s ones if corresponding bits of [%-*@3 maskPage 
]are set.&]
[s3; The combined image is appended at the end of PixRaster.&]
[s3; &]
[s3; Returns [%-@(0.0.255) true] on success, [%-@(0.0.255) false] otherwise.&]
[s4; &]
[s0; ]