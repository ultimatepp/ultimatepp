topic "Raster";
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
[ {{10000@(113.42.0) [s0;%% [*@7;4 Raster]]}}&]
[s3; &]
[s1;:Raster`:`:class: [@(0.0.255)3 class][3 _][*3 Raster]&]
[s9;%% Raster is an abstract class representing raster image. Example 
of Raster derived classes is PngRaster, that serves as .png image 
file decoder or MemoryRaster, that represent image in memory 
that can be in specific format (not RGBA).&]
[s9;%% Raster provides basic information about image (GetSize and 
GetInfo returning Raster`::Info) and is a source of image scanlines. 
Scanlines are returned as Raster`::Line objects. If possible, 
Raster derived classes should attempt to minimize memory requirements. 
For example, PngRaster only fetches individual scanline from 
the file at time, if possible.&]
[s3; &]
[s0; &]
[ {{10000F(128)G(128)@1 [s0;%% [* Raster`::Line]]}}&]
[s3; &]
[s1;:Raster`:`:Line`:`:class: [@(0.0.255)3 class][3 _][*3 Raster`::Line]&]
[s2;%% This class represents a single scanline in the raster. For 
performance reasons, it uses [^topic`:`/`/Core`/srcdoc`/pick`_`$en`-us^ pick 
transfer semantics]. As is can be used both is situations where 
referenced data are stored in Raster or in situations where they 
need custom storage, constructors have `'free`' parameter. Scanline 
can be represented in format of Raster or as RGBA array. Conversion 
to RGBA array is lazy `- does not happen until it is needed.&]
[s2;%% &]
[s2;%% Note that Line is only valid as long as its source Raster 
exists.&]
[s2;%% &]
[s2;%% For convenience, this class is also typedefed as RasterLine.&]
[s3; &]
[s0; &]
[ {{10000F(128)G(128)@1 [s0;%% [* Raster`::Line Public Method List]]}}&]
[s3; &]
[s5;:Raster`:`:Line`:`:GetRGBA`(`)const: [@(0.0.255) const]_[_^RGBA^ RGBA]_`*[* GetRGBA]()_
[@(0.0.255) const]&]
[s2;%% Returns a pointer to array with scanline pixels. Conversion 
of Raster format to RGBA buffer is lazy, does not happen before 
this function is called for the first time. Also, if Raster format 
is RGBA, no conversion is performed and pointer to raw data is 
returned.&]
[s3; &]
[s4; &]
[s5;:Raster`:`:Line`:`:GetRawData`(`)const: [@(0.0.255) const]_[_^byte^ byte]_`*[* GetRawDa
ta]()_[@(0.0.255) const]&]
[s2;%% Returns the scanline in Raster format.&]
[s3; &]
[s4; &]
[s5;:Raster`:`:Line`:`:operator const RGBA`*`(`)const: [@(0.0.255) operator]_[@(0.0.255) c
onst]_[_^RGBA^ RGBA]_`*()_[@(0.0.255) const]&]
[s5;:Raster`:`:Line`:`:operator`~`(`)const: [@(0.0.255) const]_[_^RGBA^ RGBA]_`*[* operator
`~]()_[@(0.0.255) const]&]
[s2;%% Returns GetRGBA().&]
[s3; &]
[s0; &]
[ {{10000F(128)G(128)@1 [s0;%% [* Raster`::Line Constructor detail]]}}&]
[s3; &]
[s5;:Raster`:`:Line`:`:Line`(const RGBA`*`,bool`): [* Line]([@(0.0.255) const]_[_^RGBA^ RGB
A]_`*[*@3 data], [@(0.0.255) bool]_[*@3 free])&]
[s2;%% Constructs Line from RGBA data. If [%-*@3 free] is true, Line 
destructor deletes the [%-*@3 data] pointer.&]
[s3;%% &]
[s4; &]
[s5;:Raster`:`:Line`:`:Line`(const byte`*`,Raster`*`,bool`): [* Line]([@(0.0.255) const]_
[_^byte^ byte]_`*[*@3 fmtdata], [_^Raster^ Raster]_`*[*@3 raster], [@(0.0.255) bool]_[*@3 fmt
free])&]
[s2;%% Constructs Line from data [%-*@3 fmtdata] in format of [%-*@3 raster]. 
If [%-*@3 fmtfree].is true, Line destructor deletes the [%-*@3 fmtdata] 
pointer.&]
[s3;%% &]
[s4; &]
[s5;:Raster`:`:Line`:`:Line`(pick`_ Raster`:`:Line`&`): [* Line]([@(0.128.128) pick`_]_[* L
ine][@(0.0.255) `&]_[*@3 b])&]
[s2;%% Pick constructor.&]
[s3;%% &]
[s4; &]
[s5;:Raster`:`:Line`:`:Line`(`): [* Line]()&]
[s2;%% Default constructor. Default constructed Line can be used 
as target of pick transfer from another Line.&]
[s3; &]
[s4; &]
[s5;:Raster`:`:Line`:`:operator`=`(pick`_ Raster`:`:Line`&`): [@(0.0.255) void]_[* operat
or`=]([@(0.128.128) pick`_]_[_^Raster`:`:Line^ Line][@(0.0.255) `&]_[*@3 b])&]
[s2;%% Pick assignment.&]
[s3;%% &]
[s1;@(0.0.255)3 &]
[ {{10000F(128)G(128)@1 [s0;%% [* Raster`::Info]]}}&]
[s3; &]
[s1;:Raster`:`:Info`:`:struct: [@(0.0.255)3 struct][3 _][*3 Raster`::Info]&]
[s2;%% This structure is used to provide the information about Raster. 
For convenience, this class is also typedefed as RasterInfo.&]
[s3; &]
[s0; &]
[ {{10000F(128)G(128)@1 [s0;%% [* Raster`::Line Member Variables]]}}&]
[s3; &]
[s5;:Raster`:`:Info`:`:bpp: [@(0.0.255) int]_[* bpp]&]
[s2;%% Number of bits per pixel. Default value is 24.&]
[s3; &]
[s4; &]
[s5;:Raster`:`:Info`:`:colors: [@(0.0.255) int]_[* colors]&]
[s2;%% Number of colors. Default value is 16777216.&]
[s3; &]
[s4; &]
[s5;:Raster`:`:Info`:`:dots: [_^Size^ Size]_[* dots]&]
[s2;%% Physical size of source image, e.g. for scanned images, in 
1/600 inches. Size(0, 0) if not known (default value).&]
[s3; &]
[s4; &]
[s5;:Raster`:`:Info`:`:hotspot: [_^Point^ Point]_[* hotspot]&]
[s2;%% Hotspot location. If not know or there is none, Point(0, 0) 
(default value).&]
[s3; &]
[s4; &]
[s5;:Raster`:`:Info`:`:kind: [@(0.0.255) int]_[* kind]&]
[s2;%% The kind of image, one of [* IMAGE`_EMPTY], [* IMAGE`_ALPHA], 
[* IMAGE`_MASK], [* IMAGE`_OPAQUE]. Default value is [* IMAGE`_OPAQUE].&]
[s3;%% &]
[s4; &]
[s5;:Upp`:`:Raster`:`:Info`:`:orientation: [@(0.0.255) int]_[* orientation]&]
[s2;%% Orientation of image. This is one of FLIP`_NONE, FLIP`_MIRROR`_HORZ, 
FLIP`_ROTATE`_180, FLIP`_MIRROR`_VERT, FLIP`_TRANSPOSE, FLIP`_ROTATE`_CLOCKWISE, 
FLIP`_TRANSVERSE, FLIP`_ROTATE`_ANTICLOCKWISE. This value is 
intended to flip Image to correct orientation (usually JPEG from 
digital camera).&]
[s3; &]
[s0; &]
[ {{10000F(128)G(128)@1 [s0;%% [* Raster public methods]]}}&]
[s3; &]
[s5;:Raster`:`:SeekPage`(int`): [@(0.0.255) virtual] [@(0.0.255) void]_[* SeekPage]([@(0.0.255) i
nt]_[*@3 page])&]
[s2;%% Provides Raster with ability to handle sources that contain 
more than single image (e.g. animated GIFs). Sets Raster to image 
no [%-*@3 page], first image is 0. Default implementation is empty.&]
[s3; &]
[s4;%% &]
[s5;:Raster`:`:GetPageCount`(`): [@(0.0.255) virtual] [@(0.0.255) int]_[* GetPageCount]()&]
[s2;%% Returns a number of images in Raster. Default implementation 
returns 1.&]
[s3;%% &]
[s4;%% &]
[s5;:Raster`:`:Create`(`): [@(0.0.255) virtual] [@(0.0.255) bool]_[* Create]()&]
[s2;%% Called at the beginning of processing of a new raster, e.g. 
used to read file header and get internal variables ready for 
processing. false return value indicates failure.&]
[s3;%% &]
[s4;%% &]
[s5;:Raster`:`:GetSize`(`): [@(0.0.255) virtual] [_^Size^ Size]_[* GetSize]()_`=_[@3 0]&]
[s2;%% Returns the size of Raster in pixels.&]
[s3;%% &]
[s4;%% &]
[s5;:Raster`:`:GetInfo`(`): [@(0.0.255) virtual] [_^Raster`:`:Info^ Info]_[* GetInfo]()&]
[s2;%% Returns the information about Raster.&]
[s3;%% &]
[s4;%% &]
[s5;:Raster`:`:GetLine`(int`): [@(0.0.255) virtual] [_^Raster`:`:Line^ Line]_[* GetLine]([@(0.0.255) i
nt]_[*@3 line])_`=_[@3 0]&]
[s2;%% Reads a single scanline [%-*@3 line] from the raster. If possible, 
Raster should be optimized for reading scanlines in ascending 
order `- this what most processing functions (should) require.&]
[s3;%% &]
[s4;%% &]
[s5;:Raster`:`:IsError`(`): [@(0.0.255) virtual] [@(0.0.255) bool]_[* IsError]()&]
[s2; Signals that during the Raster processing, there was an error. 
For example, it can signal that .png file is corrupt.&]
[s3;%% &]
[s4;%% &]
[s5;:Raster`:`:GetPaletteCount`(`): [@(0.0.255) virtual] [@(0.0.255) int]_[* GetPaletteCoun
t]()&]
[s2;%% Returns the size of palette for raster. If there is no palette, 
returns 0.&]
[s3;%% &]
[s4;%% &]
[s5;:Raster`:`:GetPalette`(`): [@(0.0.255) virtual] [@(0.0.255) const]_[_^RGBA^ RGBA]_`*[* Ge
tPalette]()&]
[s2;%% Returns current palette, NULL if there is no palette.&]
[s3;%% &]
[s4;%% &]
[s5;:Raster`:`:GetFormat`(`): [@(0.0.255) virtual] [@(0.0.255) const]_[_^RasterFormat^ Rast
erFormat]_`*[* GetFormat]()&]
[s2;%% Returns the format of Raster, can return NULL if format is 
RGBA.&]
[s3;%% &]
[s4;%% &]
[s5;:Raster`:`:GetWidth`(`): [@(0.0.255) int]_[* GetWidth]()&]
[s2;%% Same as GetSize().cx.&]
[s3;%% &]
[s4;%% &]
[s5;:Raster`:`:GetHeight`(`): [@(0.0.255) int]_[* GetHeight]()&]
[s2;%% Same as GetSize().cy.&]
[s3;%% &]
[s4;%% &]
[s5;:Raster`:`:operator`[`]`(int`): [_^Raster`:`:Line^ Line]_[* operator`[`]]([@(0.0.255) i
nt]_[*@3 i])&]
[s2;%% Same as GetLine([%-*@3 i]).&]
[s3;%% &]
[s4;%% &]
[s5;:Raster`:`:GetImage`(int`,int`,int`,int`,const Gate2`<int`,int`>`): [_^Image^ Image
]_[* GetImage]([@(0.0.255) int]_[*@3 x], [@(0.0.255) int]_[*@3 y], [@(0.0.255) int]_[*@3 cx], 
[@(0.0.255) int]_[*@3 cy], [@(0.0.255) const]_[_^Gate2^ Gate2]<[@(0.0.255) int], 
[@(0.0.255) int]>_[*@3 progress]_`=_[@(0.0.255) false])&]
[s5;:Raster`:`:GetImage`(const Gate2`<int`,int`>`): [_^Image^ Image]_[* GetImage]([@(0.0.255) c
onst]_[_^Gate2^ Gate2]<[@(0.0.255) int], [@(0.0.255) int]>_[*@3 progress]_`=_[@(0.0.255) fa
lse])&]
[s2;%% Converts area [%-*@3 x],[%-*@3 y],[%-*@3 cx],[%-*@3 cy], or the whole 
Rater, to Image. Parameter [%-*@3 progress].can be used to trace 
the progress of operation (first int is current step, second 
is number of step to do) and also to cancel the operation (if 
it returns true; empty Image is returned in that case). Default 
value for the parameter means no progress feedback is provided.&]
[s3; &]
[s0; ]]