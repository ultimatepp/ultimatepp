topic "Image arithmetic operations";
[ $$0,0#00000000000000000000000000000000:Default]
[H6;0 $$1,0#05600065144404261032431302351956:begin]
[i448;a25;kKO9;2 $$2,0#37138531426314131252341829483370:codeitem]
[l288;2 $$3,0#27521748481378242620020725143825:desc]
[0 $$4,0#96390100711032703541132217272105:end]
[{_} 
[s0; &]
[ {{10000F(128)G(128)@1 [s0;%% [* Arithmetic operations]]}}&]
[s1; &]
[s2;:PixRaster`:`:Invert`(int`): [@(0.0.255) bool]_[* Invert]([@(0.0.255) int]_[*@3 page]_`=_
PIXRASTER`_CURPAGE)&]
[s3;%%  [%-*@3 page].&]
[s4;%% &]
[s1; &]
[s2;:PixRaster`:`:AddConstantGray`(int`,int`): [@(0.0.255) bool]_[* AddConstantGray]([@(0.0.255) i
nt]_[*@3 val], [@(0.0.255) int]_[*@3 page]_`=_PIXRASTER`_CURPAGE)&]
[s3;%%  [%-*@3 val] [%-*@3 page].&]
[s4;%% &]
[s1; &]
[s2;:PixRaster`:`:MultConstantGray`(int`,int`): [@(0.0.255) bool]_[* MultConstantGray]([@(0.0.255) i
nt]_[*@3 val], [@(0.0.255) int]_[*@3 page]_`=_PIXRASTER`_CURPAGE)&]
[s3;%%  [%-*@3 val] [%-*@3 page].&]
[s4;%% &]
[s1; &]
[s2;:PixRaster`:`:AddGray`(int`,int`): [@(0.0.255) bool]_[* AddGray]([@(0.0.255) int]_[*@3 pa
ge1], [@(0.0.255) int]_[*@3 page2])&]
[s3;%%  [%-*@3 page1] [%-*@3 page2].&]
[s4;%% &]
[s1; &]
[s2;:PixRaster`:`:SubtractGray`(int`,int`): [@(0.0.255) bool]_[* SubtractGray]([@(0.0.255) i
nt]_[*@3 page1], [@(0.0.255) int]_[*@3 page2])&]
[s3;%%  [%-*@3 page1] [%-*@3 page2].&]
[s4;%% &]
[s0; ]