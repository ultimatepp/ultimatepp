topic "Morphing functions";
[ $$0,0#00000000000000000000000000000000:Default]
[H6;0 $$1,0#05600065144404261032431302351956:begin]
[i448;a25;kKO9;2 $$2,0#37138531426314131252341829483370:codeitem]
[l288;2 $$3,0#27521748481378242620020725143825:desc]
[0 $$4,0#96390100711032703541132217272105:end]
[{_}%EN-US 
[s0;%- &]
[ {{10000F(128)G(128)@1 [s0; [* Morphing functions]]}}&]
[s1;%- &]
[s2;:Pix`:`:ErodeGray`(int`,int`):%- [@(0.0.255) Pix]_[* ErodeGray]([@(0.0.255) int]_[*@3 hsi
ze], [@(0.0.255) int]_[*@3 vsize])&]
[s3; Apply the morphological [* erosion] operation on grayscale image.&]
[s3; [%-*@3 hsize] and [%-*@3 vsize] are [* Sel] (structuring elements) 
sizes, and must be odd numbers (they`'re changed to biggest nearest 
odd number if not) If both [%-*@3 hsize] and [%-*@3 vsize] are equal 
to 1, [* ErodeGray] just returns a cloned image.&]
[s3; Erosion remove pixels on objects boundaries; the amount of eroded 
pixels depends on size of [* Sel]s&]
[s3; Eroded image is appended at the end of PixRaster&]
[s3; &]
[s3; Returns eroded [* Pix ]on success,an empty [* Pix ]otherwise.&]
[s3; &]
[s4; &]
[s1;%- &]
[s2;:Pix`:`:DilateGray`(int`,int`):%- [@(0.0.255) Pix]_[* DilateGray]([@(0.0.255) int]_[*@3 h
size], [@(0.0.255) int]_[*@3 vsize])&]
[s3; Apply the morphological [* dilation] operation on grayscale image.&]
[s3; [%-*@3 hsize] and [%-*@3 vsize] are [* Sel] (structuring elements) 
sizes, and must be odd numbers (they`'re changed to biggest nearest 
odd number if not) If both [%-*@3 hsize] and [%-*@3 vsize] are equal 
to 1, [* DilateGray] just returns a cloned image.&]
[s3; Dilation adds pixels on objects boundaries; the amount of added 
pixels depends on size of [* Sel]s&]
[s3; Dilated image is appended at the end of PixRaster&]
[s3; &]
[s3; Returns dilated [* Pix ]on success,an empty [* Pix ]otherwise.&]
[s3; &]
[s4; &]
[s1;%- &]
[s2;:Pix`:`:OpenGray`(int`,int`):%- [@(0.0.255) Pix]_[* OpenGray]([@(0.0.255) int]_[*@3 hsize
], [@(0.0.255) int]_[*@3 vsize])&]
[s3; Apply the morphological [* opening] operation on grayscale image.&]
[s3; [%-*@3 hsize] and [%-*@3 vsize] are [* Sel] (structuring elements) 
sizes, and must be odd numbers (they`'re changed to biggest nearest 
odd number if not) If both [%-*@3 hsize] and [%-*@3 vsize] are equal 
to 1, [* OpenGray] just returns a cloned image.&]
[s3; Opening combines an [* erosion] operation followed by a [* dilation] 
one; the amount of added/eroded pixels depends on size of [* Sel]s&]
[s3; Opened image is appended at the end of PixRaster&]
[s3; &]
[s3; Returns opened [* Pix ]on success,an empty [* Pix ]otherwise.&]
[s3; &]
[s4; &]
[s1;%- &]
[s2;:Pix`:`:CloseGray`(int`,int`):%- [@(0.0.255) Pix]_[* CloseGray]([@(0.0.255) int]_[*@3 hsi
ze], [@(0.0.255) int]_[*@3 vsize])&]
[s3; Apply the morphological [* closing] operation on grayscale image.&]
[s3; [%-*@3 hsize] and [%-*@3 vsize] are [* Sel] (structuring elements) 
sizes, and must be odd numbers (they`'re changed to biggest nearest 
odd number if not) If both [%-*@3 hsize] and [%-*@3 vsize] are equal 
to 1, [* CloseGray] just returns a cloned image.&]
[s3; Closing combines a [* dilation] operation followed by an [* erosion 
]one; the amount of added/eroded pixels depends on size of [* Sel]s&]
[s3; Closed image is appended at the end of PixRaster&]
[s3; &]
[s3; Returns closed [* Pix ]on success,an empty [* Pix ]otherwise.&]
[s3; &]
[s4; &]
[s0; ]