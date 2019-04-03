topic "Common Raster classes";
[i448;a25;kKO9;2 $$1,0#37138531426314131252341829483380:class]
[l288;2 $$2,2#27521748481378242620020725143825:desc]
[0 $$3,0#96390100711032703541132217272105:end]
[H6;0 $$4,0#05600065144404261032431302351956:begin]
[i448;a25;kKO9;2 $$5,0#37138531426314131252341829483370:item]
[l288;a4;*@5;1 $$6,6#70004532496200323422659154056402:requirement]
[l288;i1121;b17;O9;~~~.1408;2 $$7,0#10431211400427159095818037425705:param]
[i448;b42;O9;2 $$8,8#61672508125594000341940100500538:tparam]
[b42;2 $$9,9#13035079074754324216151401829390:normal]
[2 $$0,0#00000000000000000000000000000000:Default]
[{_} 
[ {{10000@(113.42.0) [s0;%% [*@7;4 ImageRaster]]}}&]
[s3; &]
[s1;:ImageRaster`:`:class: [@(0.0.255)3 class][3 _][*3 ImageRaster][3 _:_][@(0.0.255)3 public][3 _
][*@3;3 Raster]&]
[s9;%% This class provides the Raster interface to existing Image.&]
[s3;%% &]
[s0;%% &]
[ {{10000F(128)G(128)@1 [s0;%% [* Constructor detail]]}}&]
[s3; &]
[s5;:ImageRaster`:`:ImageRaster`(const Image`&`): [* ImageRaster]([@(0.0.255) const]_[_^Image^ I
mage][@(0.0.255) `&]_[*@3 img])&]
[s2;%% Creates Raster of [%-*@3 img].&]
[s3; &]
[s0; &]
[s0; &]
[s0; &]
[ {{10000@(113.42.0) [s0;%% [*@7;4 MemoryRaster]]}}&]
[s3;%% &]
[s1;:MemoryRaster`:`:class: [@(0.0.255)3 class][3 _][*3 MemoryRaster][3 _:_][@(0.0.255)3 public][3 _
][*@3;3 Raster]&]
[s9;%% This class represents an image in specific format stored in 
memory. It has implicit pick transfer.&]
[s3; &]
[s0; &]
[ {{10000F(128)G(128)@1 [s0;%% [* Public Method List]]}}&]
[s3; &]
[s5;:MemoryRaster`:`:Load`(Raster`&`): [@(0.0.255) void]_[* Load]([_^Raster^ Raster][@(0.0.255) `&
]_[*@3 raster])&]
[s2;%% Loads [%-*@3 raster] into MemoryRaster. Keeps the format of 
source.&]
[s3;%% &]
[s4; &]
[s5;:MemoryRaster`:`:GetLength`(`)const: [@(0.0.255) int]_[* GetLength]()_[@(0.0.255) const
]&]
[s2;%% Returns the number of bytes used to store scanlines of MemoryRaster.&]
[s3; &]
[s0; &]
[ {{10000F(128)G(128)@1 [s0;%% [* Constructor detail]]}}&]
[s3; &]
[s5;:MemoryRaster`:`:MemoryRaster`(`): [* MemoryRaster]()&]
[s2;%% Constructs empty MemoryRaster.&]
[s3; &]
[s4; &]
[s5;:MemoryRaster`:`:MemoryRaster`(Raster`&`): [* MemoryRaster]([_^Raster^ Raster][@(0.0.255) `&
]_[*@3 raster])&]
[s2;%% Loads [%-*@3 raster] into MemoryRaster. Keeps the format of 
source.&]
[s3;%% &]
[s0;%% &]
[s0;%% &]
[s0;%% &]
[ {{10000@(113.42.0) [s0;%% [*@7;4 StreamRaster]]}}&]
[s3;%% &]
[s1;:StreamRaster`:`:class: [@(0.0.255)3 class][3 _][*3 StreamRaster][3 _:_][@(0.0.255)3 public][3 _
][*@3;3 Raster]&]
[s9;%% Represents an image stored in Stream, usually used as base`-class 
for image decoders, e.g. PNGRaster. StreamRaster derived classes 
can be registered, in that case a class factory of such class 
is stored in an array and attempted to load the file of unknown 
format type.&]
[s3;%% &]
[s0;%% &]
[ {{10000F(128)G(128)@1 [s0;%% [* Public Method List]]}}&]
[s3; &]
[s5;:StreamRaster`:`:GetStream`(`): [_^Stream^ Stream][@(0.0.255) `&]_[* GetStream]()&]
[s2;%% Returns the associated Stream.&]
[s3; &]
[s4; &]
[s5;:StreamRaster`:`:Open`(Stream`&`): [@(0.0.255) bool]_[* Open]([_^Stream^ Stream][@(0.0.255) `&
]_[*@3 s])&]
[s2;%% Associates stream [%-*@3 s].with StreamRaster (reference is 
stored) and calls Create virtual method so that derived class 
can read the stream header and prepare everything for reading 
scanlines. If Create signals failure by returning false, put 
StreamRaster in error state and returns false.&]
[s3;%% &]
[s4; &]
[s5;:StreamRaster`:`:IsError`(`): [@(0.0.255) bool]_[* IsError]()&]
[s2;%% Returns true if there was any error encountered. Gets reset 
in Open.&]
[s3; &]
[s4; &]
[s5;:StreamRaster`:`:SetError`(`): [@(0.0.255) void]_[* SetError]()&]
[s2;%% Puts StreamRaster into error state.&]
[s3; &]
[s4; &]
[s5;:StreamRaster`:`:Load`(Stream`&`,const Gate2`<int`,int`>`): [_^Image^ Image]_[* Load](
[_^Stream^ Stream][@(0.0.255) `&]_[*@3 s], [@(0.0.255) const]_[_^Gate2^ Gate2]<[@(0.0.255) in
t], [@(0.0.255) int]>_[*@3 progress]_`=_[@(0.0.255) false])&]
[s5;:StreamRaster`:`:LoadFile`(const char`*`,const Gate2`<int`,int`>`): [_^Image^ Image
]_[* LoadFile]([@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 fn], [@(0.0.255) const]_[_^Gate2^ G
ate2]<[@(0.0.255) int], [@(0.0.255) int]>_[*@3 progress]_`=_[@(0.0.255) false])&]
[s5;:StreamRaster`:`:LoadString`(const String`&`,const Gate2`<int`,int`>`): [_^Image^ I
mage]_[* LoadString]([@(0.0.255) const]_[_^String^ String][@(0.0.255) `&]_[*@3 s], 
[@(0.0.255) const]_[_^Gate2^ Gate2]<[@(0.0.255) int], [@(0.0.255) int]>_[*@3 progress]_`=_[@(0.0.255) f
alse])&]
[s2;%% Using Raster virtual methods, loads Image from Stream, file, 
or string. [%-*@3 progress] can be used to track the progress and 
to cancel loading (by returning true).&]
[s3;%% &]
[s0;%% &]
[ {{10000F(128)G(128)@1 [s0;%% [* Registered Decoders and multi`-format support]]}}&]
[s4;H0; &]
[s5;:StreamRaster`:`:Register`(`): [@(0.0.255) static] [@(0.0.255) template]_<[@(0.0.255) c
lass]_[*@4 T]>_[@(0.0.255) void]_[* Register]()&]
[s2;%% Registers StreamRaster derived class [%-*@4 T].&]
[s3; &]
[s4; &]
[s5;:StreamRaster`:`:OpenAny`(Stream`&`): [@(0.0.255) static] [_^One^ One]<[_^StreamRaster^ S
treamRaster]>_[* OpenAny]([_^Stream^ Stream][@(0.0.255) `&]_[*@3 s])&]
[s2;%% Attempts to open the image raster stored in [%-*@3 s] using 
all StreamRaster derived classes (decoders) registered by Register 
(usually, established image file formats like png, gif, bmp are 
registered). If any decoder is able to open the stream, returns 
One container with opened decoder, otherwise returned One is 
empty.&]
[s3;%% &]
[s4; &]
[s5;:StreamRaster`:`:LoadAny`(Stream`&`,const Gate2`<int`,int`>`): [@(0.0.255) static] 
[_^Image^ Image]_[* LoadAny]([_^Stream^ Stream][@(0.0.255) `&]_[*@3 s], 
[@(0.0.255) const]_[_^Gate2^ Gate2]<[@(0.0.255) int], [@(0.0.255) int]>_[*@3 progress]_`=_[@(0.0.255) f
alse])&]
[s5;:StreamRaster`:`:LoadFileAny`(const char`*`,const Gate2`<int`,int`>`): [@(0.0.255) s
tatic] [_^Image^ Image]_[* LoadFileAny]([@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 fn], 
[@(0.0.255) const]_[_^Gate2^ Gate2]<[@(0.0.255) int], [@(0.0.255) int]>_[*@3 progress]_`=_[@(0.0.255) f
alse])&]
[s5;:StreamRaster`:`:LoadStringAny`(const String`&`,const Gate2`<int`,int`>`): [@(0.0.255) s
tatic] [_^Image^ Image]_[* LoadStringAny]([@(0.0.255) const]_[_^String^ String][@(0.0.255) `&
]_[*@3 s], [@(0.0.255) const]_[_^Gate2^ Gate2]<[@(0.0.255) int], [@(0.0.255) int]>__`=_[@(0.0.255) f
alse])&]
[s2;%% Attempts to read Image from the stream, file or string, using 
all registered decoders. [%-*@3 progress] can be used to track 
progress of operation or to cancel it by returning true.&]
[s3;%% &]
[s0;%% &]
[ {{10000F(128)G(128)@1 [s0;%% [* Constructor detail]]}}&]
[s4;H0; &]
[s5;:StreamRaster`:`:StreamRaster`(`): [* StreamRaster]()&]
[s2;%% Puts StreamRaster into error state (error state is reset by 
Open).&]
[s3; &]
[s0; ]]