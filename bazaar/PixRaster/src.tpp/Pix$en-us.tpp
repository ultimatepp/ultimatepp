topic "class Pix : public PixBase";
[ $$0,0#00000000000000000000000000000000:Default]
[i448;a25;kKO9; $$1,0#37138531426314131252341829483380:structitem]
[l288;2 $$2,0#27521748481378242620020725143825:desc]
[0 $$3,0#96390100711032703541132217272105:end]
[H6;0 $$4,0#05600065144404261032431302351956:begin]
[i448;a25;kKO9;2 $$5,0#37138531426314131252341829483370:codeitem]
[{_} 
[s1;:Pix`:`:class: [@(0.0.255) class]_[* Pix]_:_[@(0.0.255) public]_[*@3 PixBase]&]
[s2;%% Single page Leptonica raster object.&]
[s2;%% It encapsulates [* Pix] single`-page Leptonica raster objects&]
[s2;%% &]
[s3; &]
[s0; &]
[ {{10000F(128)G(128)@1 [s0;%% [* Public enums]]}}&]
[s0; &]
[s4; &]
[s5;:Pix`:`:PIXRASTER`_BRING`_IN`_WHITE: [@(0.0.255) enum]_[* BringInModes]&]
[s2;%% &]
[s2; Color filling mode in operations that adds parts to images, 
for example rotations :&]
[s0; &]
[s5; -|[* PIXRASTER`_BRING`_IN`_WHITE]-|Fills new parts with white color&]
[s5; -|[* PIXRASTER`_BRING`_IN`_BLACK]-|Fills new parts with black color&]
[s2;%% &]
[s3; &]
[s0; &]
[ {{10000F(128)G(128)@1 [s0;%% [* Constructors, destructor and assignment operator]]}}&]
[s0; &]
[s4; &]
[s5;:Pix`:`:Pix`(`): [* Pix]()&]
[s2;%% Constructs an empty [* Pix].&]
[s3; &]
[s4; &]
[s5;:Pix`:`:Pix`(int`,int`,int`,RGBA`*`): [* Pix]([@(0.0.255) int]_[*@3 width], 
[@(0.0.255) int]_[*@3 height], [@(0.0.255) int]_[*@3 depth], [_^RGBA^ RGBA]_`*[*@3 colorTable
]_`=_NULL)&]
[s2;%% Constructs and initializes Pix of [%-*@3 width] , [%-*@3 height] 
and [%-*@3 depth] .&]
[s2;%% Optionally sets its [%-*@3 colorTable] if available.&]
[s3;%% &]
[s4; &]
[s5;:Pix`:`:Pix`(PIX`*`*`): [* Pix]([_^PIX^ PIX]_`*`*[*@3 `_pix])&]
[s2;%% Constructs [* Pix] from the Leptonica [* PIX] [%-*@3 `_pix] object.&]
[s2;%% To avoid mis`-use of owned [* PIX], it clears source pointer; 
beware to double indirection when using.&]
[s3;%% &]
[s4; &]
[s5;:Pix`:`:Pix`(Pix const`&`): [* Pix]([* Pix]_[@(0.0.255) const]_`&[*@3 `_pix])&]
[s2;%% Copy constructor. Initializes [* Pix] object from a source one 
[%-*@3 `_pix].&]
[s2;%% Source is [* reference copied], so any change on copy will reflect 
on original object.&]
[s3;%% &]
[s4; &]
[s5;:Pix`:`:Pix`(Pix const`&`,int`): [* Pix]([* Pix]_[@(0.0.255) const]_`&[*@3 `_pix], 
[@(0.0.255) int]_[*@3 i])&]
[s2;%% Deep copy constructor. Initializes [* Pix] object from a source 
one [%-*@3 `_pix].&]
[s2;%% Source is [* deep copied], so any change on copy will [* NOT] 
reflect on original object.&]
[s2;%% &]
[s3;%% &]
[s4; &]
[s5;:Pix`:`:Pix`(Raster`&`,bool`,int`): [* Pix]([_^Raster^ Raster]_`&[*@3 raster], 
[@(0.0.255) bool]_[*@3 deepCopy]_`=_[@(0.0.255) false], [@(0.0.255) int]_[*@3 page]_`=_PIXR
ASTER`_CURPAGE)&]
[s2;%% Constructs [* Pix] from [%-*@3 page] in source [%-*@3 raster] with 
optional [%-*@3 deepCopy] &]
[s3;%% &]
[s4; &]
[s5;:Pix`:`:Pix`(FileIn`&`,int`): [* Pix]([_^FileIn^ FileIn]_`&[*@3 fs], 
[@(0.0.255) int]_[*@3 page]_`=_[@3 0])&]
[s2;%% Constructs [* Pix] reading its content from opened stream [%-*@3 fs] 
; if fs supports multipaged images, an optional argument [%-*@3 page] 
can select one of them.&]
[s3;%% &]
[s4; &]
[s5;:Pix`:`:Pix`(String const`&`,int`): [* Pix]([_^String^ String]_[@(0.0.255) const]_`&[*@3 f
ileName], [@(0.0.255) int]_[*@3 page]_`=_[@3 0])&]
[s2;%% Constructs [* Pix] reading its content from [%-*@3 fileName] file; 
if file format supports multipaged images, an optional argument 
[%-*@3 page] can select one of them&]
[s3;%% &]
[s4; &]
[s5;:Pix`:`:`~Pix`(`): [@(0.0.255) `~][* Pix]()&]
[s2;%% Frees underlying PIX decrementing its reference counter and 
deleting it if needed.&]
[s3; &]
[s4; &]
[s5;:Pix`:`:operator`=`(Pix`&`): [_^Pix^ Pix]_`&[* operator`=]([_^Pix^ Pix]_`&[*@3 pix])&]
[s2;%% Assignment operator. [%-*@3 pix] is [* referenced], NOT copied, 
so any change on destination [* Pix] will reflect on original one.&]
[s3;%% &]
[s4; &]
[s5;:Pix`:`:operator`<`<`=`(Pix`&`): [_^Pix^ Pix]_`&[@(0.0.255) operator]_<<`=([_^Pix^ Pix]_
`&[*@3 pix])&]
[s2;%% Assignment operator. [%-*@3 pix] is [* deep copied], NOT referenced, 
so any change on destination [* Pix] will [* NOT ]reflect on original 
one.&]
[s3;%% &]
[s4; &]
[s5;:Pix`:`:DeepCopy`(Pix`&`): [_^Pix^ Pix]_`&[* DeepCopy]([_^Pix^ Pix]_`&[*@3 pix])&]
[s2;%% Return a [* deep copy] of [%-*@3 pix].&]
[s3;%% &]
[s0; &]
[ {{10000F(128)G(128)@1 [s0;%% [* Internal handling of Leptonica objects]]}}&]
[s0; &]
[s4; &]
[s5;:Pix`:`:GetPIX`(int`): [@(0.0.255) virtual] [_^PIX^ PIX]_`*[* GetPIX]([@(0.0.255) int]_[*@3 p
age]_`=_PIXRASTER`_CURPAGE)&]
[s2;%% Gets underlying Leptonica [* PIX ]object for [%-*@3 page]. As 
[* Pix] is single`-paged, just returns the underlying [* PIX] object.&]
[s2;%% [* WARNING], [* Pix] owns [* PIX ]object, so don`'t free it.&]
[s3;%% &]
[s4; &]
[s5;:Pix`:`:operator PIX`*`(`): [* operator_PIX`*]()&]
[s2;%% Gets underlying Leptonica [* PIX ]object.&]
[s2;%% [* WARNING], [* Pix] owns [* PIX ]object, so don`'t free it.&]
[s3; &]
[s0; &]
[ {{10000F(128)G(128)@1 [s0;%% [* Page handling functions]]}}&]
[s0; &]
[s4; &]
[s5;:Pix`:`:SeekPage`(int`): [@(0.0.255) virtual] [@(0.0.255) void]_[* SeekPage]([@(0.0.255) i
nt]_[@3 page])&]
[s2;%% Sets active [%-*@3 page]. As Pix object is single`-paged, does 
nothing.&]
[s3;%% &]
[s4; &]
[s5;:Pix`:`:GetPageCount`(`): [@(0.0.255) virtual] [@(0.0.255) int]_[* GetPageCount]()&]
[s2;%% Gets number of images on PixRaster. As Pix object is single`-paged, 
returns 1.&]
[s3; &]
[s4; &]
[s5;:Pix`:`:GetActivePage`(`): [@(0.0.255) virtual] [@(0.0.255) int]_[* GetActivePage]()&]
[s2;%% Gets number of currently active page. As Pix object is single`-paged, 
returns 0.&]
[s3; &]
[s0; &]
[ {{10000F(128)G(128)@1 [s0;%% [* Miscellaneous Raster functions]]}}&]
[s0; &]
[s4; &]
[s5;:Pix`:`:GetSize`(`): [@(0.0.255) virtual] [_^Size^ Size]_[* GetSize]()&]
[s2;%% Returns the size of Raster`'s active page in pixels.&]
[s3; &]
[s4; &]
[s5;:Pix`:`:GetSizeEx`(int`): [@(0.0.255) virtual] [_^Size^ Size]_[* GetSizeEx]([@(0.0.255) i
nt]_[@3 page])&]
[s2;%% Returns the size of Raster in pixels for [%-*@3 page].&]
[s3;%% &]
[s4; &]
[s5;:Pix`:`:GetWidth`(`): [@(0.0.255) virtual] [@(0.0.255) int]_[* GetWidth]()&]
[s2;%% Returns the width of Raster in pixels for active page.&]
[s3; &]
[s4; &]
[s5;:Pix`:`:GetWidthEx`(int`): [@(0.0.255) virtual] [@(0.0.255) int]_[* GetWidthEx]([@(0.0.255) i
nt]_[@3 page])&]
[s2;%% Returns the width of Raster in pixels for [%-*@3 page].&]
[s3;%% &]
[s4; &]
[s5;:Pix`:`:GetHeight`(`): [@(0.0.255) virtual] [@(0.0.255) int]_[* GetHeight]()&]
[s2;%% Returns the height of Raster in pixels for active page.&]
[s3; &]
[s4; &]
[s5;:Pix`:`:GetHeightEx`(int`): [@(0.0.255) virtual] [@(0.0.255) int]_[* GetHeightEx]([@(0.0.255) i
nt]_[@3 page])&]
[s2;%% Returns the height of Raster in pixels for [%-*@3 page].&]
[s3;%% &]
[s4; &]
[s5;:Pix`:`:GetInfo`(`): [@(0.0.255) virtual] [_^Raster`:`:Info^ Info]_[* GetInfo]()&]
[s2;%% Returns the information about Raster`'s active page.&]
[s3; &]
[s4; &]
[s5;:Pix`:`:GetInfoEx`(int`): [@(0.0.255) virtual] [_^Raster`:`:Info^ Info]_[* GetInfoEx(][@(0.0.255) i
nt]_[@3 page])&]
[s2;%% Returns the information about Raster for [%-*@3 page].&]
[s3;%% &]
[s4; &]
[s5;:Pix`:`:GetLine`(int`): [@(0.0.255) virtual] [_^Raster`:`:Line^ Line]_[* GetLine]([@(0.0.255) i
nt]_[@3 line])&]
[s2;%% Reads a single scanline [%-*@3 line] from the raster`'s active 
page. If possible, Raster should be optimized for reading scanlines 
in ascending order `- this what most processing functions (should) 
require.&]
[s3;%% &]
[s4; &]
[s5;:Pix`:`:GetLineEx`(int`,int`): [@(0.0.255) virtual] [_^Raster`:`:Line^ Line]_[* GetLine
Ex]([@(0.0.255) int]_[@3 line], [@(0.0.255) int]_[@3 page])&]
[s2;%% Reads a single scanline [%-*@3 line] from [%-*@3 page] ot the 
raster. If possible, Raster should be optimized for reading scanlines 
in ascending order `- this what most processing functions (should) 
require.&]
[s3;%% &]
[s4; &]
[s5;:Pix`:`:GetPaletteCount`(`): [@(0.0.255) virtual] [@(0.0.255) int]_[* GetPaletteCount](
)&]
[s2;%% Returns the size of palette for raster`'s active page. If 
there is no palette, returns 0&]
[s3; &]
[s4; &]
[s5;:Pix`:`:GetPaletteCountEx`(int`): [@(0.0.255) virtual] [@(0.0.255) int]_[* GetPaletteCo
untEx]([@(0.0.255) int]_[@3 page])&]
[s2;%% Returns the size of palette for raster`'s [%-*@3 page]. If there 
is no palette, returns 0..&]
[s3;%% &]
[s4; &]
[s5;:Pix`:`:GetPalette`(`): [@(0.0.255) virtual] [@(0.0.255) const]_[_^RGBA^ RGBA]_`*[* GetPa
lette]()&]
[s2;%% Returns active pages`'current palette, NULL if there is no 
palette.&]
[s3; &]
[s4; &]
[s5;:Pix`:`:GetPaletteEx`(int`): [@(0.0.255) virtual] [@(0.0.255) const]_[_^RGBA^ RGBA]_`*[* G
etPaletteEx]([@(0.0.255) int]_[@3 page])&]
[s2;%% Returns [%-*@3 page][%- `'s ]current palette, NULL if there is 
no palette &]
[s3;%% &]
[s4; &]
[s5;:Pix`:`:GetFormat`(`): [@(0.0.255) virtual] [@(0.0.255) const]_[_^RasterFormat^ RasterF
ormat]_`*[* GetFormat]()&]
[s2;%% Returns the format of Raster`'s active page, can return NULL 
if format is RGBA.&]
[s3; &]
[s4; &]
[s5;:Pix`:`:GetFormatEx`(int`): [@(0.0.255) virtual] [@(0.0.255) const]_[_^RasterFormat^ Ra
sterFormat]_`*[* GetFormatEx]([@(0.0.255) int]_[@3 page])&]
[s2;%% Returns the format of Raster`'s [%-*@3 page], can return NULL 
if format is RGBA.&]
[s3;%% &]
[s4; &]
[s5;:Pix`:`:IsEmpty`(`): [@(0.0.255) bool]_[* IsEmpty]()&]
[s2;%% Returns [%-@(0.0.255) true] if raster contains no images.&]
[s3; &]
[s4; &]
[s5;:Pix`:`:operator bool`(`): [* operator_bool]()&]
[s2;%% Returns true if raster has some content, false otherwise&]
[s3; &]
[s4; &]
[s5;:Pix`:`:Clear`(`): [@(0.0.255) virtual] [@(0.0.255) void]_[* Clear]()_`=_[@3 0]&]
[s2;%% Clears raster content&]
[s3; &]
[s0; &]
[ {{10000F(128)G(128)@1 [s0;%% [* Polygon markers access]]}}&]
[s0; &]
[s4; &]
[s5;:Pix`:`:GetPolyMarkers`(`): [@(0.0.255) virtual] [_^PolyMarkers^ PolyMarkers]_`*[* GetP
olyMarkers]()&]
[s2;%% Returns array of polygon markers for current raster page&]
[s3; &]
[s4; &]
[s5;:Pix`:`:GetPolyMarkersEx`(int`): [@(0.0.255) virtual] [_^PolyMarkers^ PolyMarkers]_`*
[* GetPolyMarkersEx]([@(0.0.255) int]_[@3 page])&]
[s2; [%% Returns array of polygon markers for raster ][@3 page]&]
[s3; &]
[s0; &]
[ {{10000F(128)G(128)@1 [s0;%% [* Conversion and file I/O functions]]}}&]
[s0; &]
[s4; &]
[s5;:Pix`:`:Load`(Raster`&`,bool`,int`): [@(0.0.255) void]_[* Load]([_^Raster^ Raster][@(0.0.255) `&
]_[*@3 raster], [@(0.0.255) bool]_[*@3 deepCopy]_`=_[@(0.0.255) false], 
[@(0.0.255) int]_[*@3 page]_`=_PIXRASTER`_CURPAGE)&]
[s2;%% Initializes [* Pix] from [%-*@3 page] in source [%-*@3 raster] with 
optional [%-*@3 deepCopy] &]
[s3;%% &]
[s4; &]
[s5;:Pix`:`:Load`(FileIn`&`,int`): [@(0.0.255) bool]_[* Load]([_^FileIn^ FileIn]_`&[*@3 fs], 
[@(0.0.255) int]_[*@3 page]_`=_[@3 0])&]
[s2;%% Initializes [* Pix] reading its content from opened stream [%-*@3 fs] 
; if fs supports multipaged images, an optional argument [%-*@3 page] 
can select one of them.&]
[s3;%% &]
[s4; &]
[s5;:Pix`:`:Load`(String`,int`): [@(0.0.255) bool]_[* Load]([_^String^ String]_[*@3 fileName],
 [@(0.0.255) int]_[*@3 page]_`=_[@3 0])&]
[s2;%% Initializes [* Pix] reading its content from [%-*@3 fileName] 
file; if file format supports multipaged images, an optional 
argument [%-*@3 page] can select one of them&]
[s3;%% &]
[s4; &]
[s5;:Pix`:`:Save`(String`): [@(0.0.255) bool]_[* Save]([_^String^ String]_[*@3 fileName])&]
[s2;%% Stores Pix into [%-*@3 fileName] named file.&]
[s3;%% &]
[s0; &]
[ {{10000F(128)G(128)@1 [s0;%% [* Leptonica functions wrappers]]}}&]
[s0; &]
[s2; [*^topic`:`/`/Leptonica`/src`/PixRaster`$en`-us`#PixRaster`:`:class^ Pix] 
provides wrappers to many Leptonica image handling functions; 
as the list is huge it has been divided by cathegories; see Leptonica 
wrappers index.&]
[s0; ]