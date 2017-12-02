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
[s2;:Pix`:`:CombineMasked`(Pix`&`,Pix`&`):%- [@(0.0.255) bool]_[* CombineMasked]([@(0.0.255) P
ix`&]_[*@3 aPix], [@(0.0.255) Pix`&]_[*@3 maskPix])&]
[s3; Replaces current Pix pixels from [%-*@3 aPix]`'s ones if corresponding 
bits of [%-*@3 maskPix ]are set.&]
[s3; &]
[s3; Returns the combined Pix on success, an empty Pix otherwise.&]
[s4; &]
[s0; ]