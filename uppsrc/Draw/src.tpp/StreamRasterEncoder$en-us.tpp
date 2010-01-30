topic "StreamRasterEncoder";
[2 $$0,0#00000000000000000000000000000000:Default]
[i448;a25;kKO9;2 $$1,0#37138531426314131252341829483380:class]
[l288;2 $$2,2#27521748481378242620020725143825:desc]
[0 $$3,0#96390100711032703541132217272105:end]
[H6;0 $$4,0#05600065144404261032431302351956:begin]
[i448;a25;kKO9;2 $$5,0#37138531426314131252341829483370:item]
[l288;a4;*@5;1 $$6,6#70004532496200323422659154056402:requirement]
[l288;i1121;b17;O9;~~~.1408;2 $$7,0#10431211400427159095818037425705:param]
[i448;b42;O9;2 $$8,8#61672508125594000341940100500538:tparam]
[b42;2 $$9,9#13035079074754324216151401829390:normal]
[{_}%EN-US 
[ {{10000@(113.42.0) [s0; [*@7;4 StreamRasterEncoder]]}}&]
[s3;%- &]
[s1;:StreamRasterEncoder`:`:class:%- [@(0.0.255)3 class][3 _][*3 StreamRasterEncoder][3 _:_][@(0.0.255)3 p
ublic][3 _][*@3;3 RasterEncoder]&]
[s9; This class adds methods useful for working with image streams 
to RasterEncoder.&]
[s3; &]
[s0; &]
[ {{10000F(128)G(128)@1 [s0; [* Public Method List]]}}&]
[s3;%- &]
[s5;:StreamRasterEncoder`:`:SetStream`(Stream`&`):%- [@(0.0.255) void]_[* SetStream]([_^Stream^ S
tream][@(0.0.255) `&]_[*@3 `_s])&]
[s2; Sets the Stream to be used as source of image data. Derived 
class can query this stream using GetStream method.&]
[s3; &]
[s4;%- &]
[s5;:StreamRasterEncoder`:`:GetStream`(`):%- [_^Stream^ Stream][@(0.0.255) `&]_[* GetStream
]()&]
[s2; Returns current stream.&]
[s3; &]
[s4; &]
[s5;:StreamRasterEncoder`:`:Save`(Stream`&`,Raster`&`):%- [@(0.0.255) void]_[* Save]([_^Stream^ S
tream][@(0.0.255) `&]_[*@3 s], [_^Raster^ Raster][@(0.0.255) `&]_[*@3 raster])&]
[s2; Stores [%-*@3 raster] to the stream.&]
[s3; &]
[s4; &]
[s5;:StreamRasterEncoder`:`:SaveFile`(const char`*`,Raster`&`):%- [@(0.0.255) bool]_[* Sa
veFile]([@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 fn], [_^Raster^ Raster][@(0.0.255) `&]_
[*@3 raster])&]
[s2; Saves [%-*@3 raster ]to the file [%-*@3 fn].&]
[s3; &]
[s4; &]
[s5;:StreamRasterEncoder`:`:SaveString`(Raster`&`):%- [_^String^ String]_[* SaveString]([_^Raster^ R
aster][@(0.0.255) `&]_[*@3 raster])&]
[s2; Stores [%-*@3 raster] to memory and returns resulting binary data 
as String.&]
[s3; &]
[s4; &]
[s5;:StreamRasterEncoder`:`:Save`(Stream`&`,const Image`&`):%- [@(0.0.255) void]_[* Save](
[_^Stream^ Stream][@(0.0.255) `&]_[*@3 s], [@(0.0.255) const]_[_^Image^ Image][@(0.0.255) `&]_
[*@3 img])&]
[s2; Stores [%-*@3 img] to the stream.&]
[s3; &]
[s4; &]
[s5;:StreamRasterEncoder`:`:SaveFile`(const char`*`,const Image`&`):%- [@(0.0.255) bool
]_[* SaveFile]([@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 fn], [@(0.0.255) const]_[_^Image^ I
mage][@(0.0.255) `&]_[*@3 img])&]
[s2; Saves [%-*@3 img ]to the file [%-*@3 fn].&]
[s3; &]
[s4; &]
[s5;:StreamRasterEncoder`:`:SaveString`(const Image`&`):%- [_^String^ String]_[* SaveStri
ng]([@(0.0.255) const]_[_^Image^ Image][@(0.0.255) `&]_[*@3 img])&]
[s2; Stores [%-*@3 img] to memory and returns resulting binary data 
as String.&]
[s3; &]
[s0; ]