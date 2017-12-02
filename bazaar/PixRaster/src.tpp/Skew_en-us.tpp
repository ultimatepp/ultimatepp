topic "Skew finding/removal functions";
[ $$0,0#00000000000000000000000000000000:Default]
[H6;0 $$1,0#05600065144404261032431302351956:begin]
[i448;a25;kKO9;2 $$2,0#37138531426314131252341829483370:codeitem]
[l288;2 $$3,0#27521748481378242620020725143825:desc]
[0 $$4,0#96390100711032703541132217272105:end]
[{_}%EN-US 
[s0;%- &]
[ {{10000F(128)G(128)@1 [s0; [* Skew finding/removal functions]]}}&]
[s1;%- &]
[s2;:Pix`:`:Deskew`(int`):%- [@(0.0.255) Pix]_[* Deskew]([@(0.0.255) int]_[*@3 ReductionFacto
r]_`=_1)&]
[s3; Try to find skew of image and remove it. If succeed, it returns 
a new de`-skewed image  It uses [%-*@3 ReductionFactor] value while 
binary`-searching for the skew factor.&]
[s3; [* WARNINGS ]: it operates [* ONLY ]on [* 1bpp monochromatic] images; 
if the skew factor is too low or not found, it just returns a 
clone of source image.&]
[s3; &]
[s3; Returns deskewed [* Pix ]on success, empty [* Pix ]otherwise.&]
[s4; &]
[s1;%- &]
[s2;:Pix`:`:FindSkewAndDeskew`(int`,double`*`,double`*`):%- [@(0.0.255) Pix]_[* FindSkewA
ndDeskew]([@(0.0.255) int]_[*@3 ReductionFactor], [@(0.0.255) double]_`*[*@3 skewAngle]_`=
_NULL, [@(0.0.255) double]_`*[*@3 confidenceFactor]_`=_NULL)&]
[s3; Try to find skew of image and remove it. If succeed, it returns 
a new de`-skewed image.&]
[s3; It uses [%-*@3 ReductionFactor] value while binary`-searching 
for the skew factor. If [%-*@3 skewAngle] and [%-*@3 confidenceFactor] 
pointers are non`-NULL, it sets them to the skew angle and the 
confidence value found when de`-skewing&]
[s3; &]
[s3; Returns deskewed [* Pix ]on success, empty [* Pix ]otherwise.&]
[s4; &]
[s1;%- &]
[s2;:PixRaster`:`:FindSkew`(double`*`,double`*`):%- [@(0.0.255) bool]_[* FindSkew]([@(0.0.255) d
ouble]_`*[*@3 pangle], [@(0.0.255) double]_`*[*@3 pconf]_`=_NULL)&]
[s3; Try to find skew of image and returns it`'s value in [%-*@3 pangle] 
; confidence factor is returned in [%-*@3 pconf].&]
[s3; &]
[s3; Returns [%-@(0.0.255) true] on success, [%-@(0.0.255) false] otherwise.&]
[s4; &]
[s1;%- &]
[s2;:Pix`:`:FindSkewSweep`(double`*`,int`,double`,double`,int`):%- [@(0.0.255) bool]_[* F
indSkewSweep]([@(0.0.255) double]_`*[*@3 pangle], [@(0.0.255) int]_[*@3 reduction], 
[@(0.0.255) double]_[*@3 sweeprange], [@(0.0.255) double]_[*@3 sweepdelta])&]
[s3; Basic full`-parameters skew finding function.Try to find skew 
of image and returns it`'s value in [%-*@3 pangle] ; Uses [%-*@3 reduction] 
factor in binary searching, a semi`-range of [%-*@3 sweeprange] 
skew angle and steps by [%-*@3 sweepdelta] value.&]
[s3; &]
[s3; Returns [%-@(0.0.255) true] on success, [%-@(0.0.255) false] otherwise.&]
[s4; &]
[s1;%- &]
[s2;:Pix`:`:FindSkewSweepAndSearch`(double`*`,double`*`,int`,int`,double`,double`,double`):%- [@(0.0.255) b
ool]_[* FindSkewSweepAndSearch](_[@(0.0.255) double]_`*[*@3 pangle], 
[@(0.0.255) double]_`*[*@3 pconf], [@(0.0.255) int]_[*@3 redsweep], [@(0.0.255) int]_[*@3 red
search], [@(0.0.255) double]_[*@3 sweeprange], [@(0.0.255) double]_[*@3 sweepdelta], 
[@(0.0.255) double]_[*@3 minbsdelta])&]
[s3; Full parameters skew finding function.Try to find skew of image 
and returns it`'s value in [%-*@3 pangle] and confidence value 
in [%-*@3 pconf].[%-*@3 redsweep] is the sweep reduction factor (1, 
2, 4 or 8), [%-*@3 redsearch] is the binary search reduction factor 
(1, 2, 4 or 8, must be <`= [%-*@3 redsweep]), [%-*@3 sweeprange] 
is the half the full range, assumed about 0; in degrees , [%-*@3 sweepdelta] 
the angle increment of sweep; in degrees and [%-*@3 minbsdelta] 
the min binary search increment angle; in degrees.&]
[s3; &]
[s3; Returns [%-@(0.0.255) true] on success, [%-@(0.0.255) false] otherwise.&]
[s3; &]
[s3; [* Notes ]:&]
[s3; &]
[s3;i150;O1; This finds the skew angle, doing first a sweep through 
a set of equal angles, and then doing a binary search until convergence.&]
[s3;i150;O1; Caller must check the return value for validity of the 
result.&]
[s3; &]
[s3;i150;O1; In computing the differential line sum variance score, 
we sum the result over scanlines, but we always skip:&]
[s3;l448;i150;O0; at least one scanline&]
[s3;l448;i150;O0; not more than 10% of the image height&]
[s3;l448;i150;O0; not more than 5% of the image width&]
[s3;l448; &]
[s4; &]
[s0; ]