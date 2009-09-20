topic "class PixBase : public Raster";
[ $$0,0#00000000000000000000000000000000:Default]
[i448;a25;kKO9; $$1,0#37138531426314131252341829483380:structitem]
[l288;2 $$2,0#27521748481378242620020725143825:desc]
[0 $$3,0#96390100711032703541132217272105:end]
[H6;0 $$4,0#05600065144404261032431302351956:begin]
[i448;a25;kKO9;2 $$5,0#37138531426314131252341829483370:codeitem]
[{_} 
[s1;:PixBase`:`:class: [@(0.0.255) class]_[* PixBase]_:_[@(0.0.255) public]_[*@3 Raster]&]
[s2;%% This is the base class of Leptonica raster objects, Pix and 
PixRaster.&]
[s2;%% It has the sole purpose to provide a common base class for 
Leptonica raster pointers,&]
[s2;%% with some common function prototypes for pointer access.&]
[s2;%% Used mainly inside PixRasterCtrl&]
[s2;%% PixBase is an abstract class and cannot be instantiated.&]
[s3; &]
[s0; &]
[ {{10000F(128)G(128)@1 [s0;%% [* Internal handling of Leptonica objects]]}}&]
[s0; &]
[s4; &]
[s5;:PixBase`:`:GetPIX`(int`): [@(0.0.255) virtual] [_^PIX^ PIX]_`*[* GetPIX]([@(0.0.255) int
]_[*@3 page]_`=_PIXRASTER`_CURPAGE)_`=_[@3 0]&]
[s2;%% Gets underlying Leptonica PIX object for [%-*@3 page].&]
[s3;%% &]
[s0; &]
[ {{10000F(128)G(128)@1 [s0;%% [* Page handling functions]]}}&]
[s0; &]
[s4; &]
[s5;:PixBase`:`:SeekPage`(int`): [@(0.0.255) virtual] [@(0.0.255) void]_[* SeekPage]([@(0.0.255) i
nt]_[@3 page])&]
[s2;%% Sets active [%-*@3 page].&]
[s3;%% &]
[s4; &]
[s5;:PixBase`:`:GetPageCount`(`): [@(0.0.255) virtual] [@(0.0.255) int]_[* GetPageCount]()&]
[s2;%% Gets number of images on PixRaster&]
[s3; &]
[s4; &]
[s5;:PixBase`:`:GetActivePage`(`): [@(0.0.255) virtual] [@(0.0.255) int]_[* GetActivePage](
)&]
[s2;%% Gets number of currently active page&]
[s3; &]
[s0; &]
[ {{10000F(128)G(128)@1 [s0;%% [* Miscellaneous Raster functions]]}}&]
[s0; &]
[s4; &]
[s5;:PixBase`:`:GetSize`(`): [@(0.0.255) virtual] [_^Size^ Size]_[* GetSize]()&]
[s2;%% Returns the size of Raster`'s active page in pixels.&]
[s3; &]
[s4; &]
[s5;:PixBase`:`:GetSizeEx`(int`): [@(0.0.255) virtual] [_^Size^ Size]_[* GetSizeEx]([@(0.0.255) i
nt]_[@3 page])&]
[s2;%% Returns the size of Raster in pixels for [%-*@3 page].&]
[s3;%% &]
[s4; &]
[s5;:PixBase`:`:GetWidth`(`): [@(0.0.255) virtual] [@(0.0.255) int]_[* GetWidth]()&]
[s2;%% Returns the width of Raster in pixels for active page.&]
[s3; &]
[s4; &]
[s5;:PixBase`:`:GetWidthEx`(int`): [@(0.0.255) virtual] [@(0.0.255) int]_[* GetWidthEx]([@(0.0.255) i
nt]_[@3 page])&]
[s2;%% Returns the width of Raster in pixels for [%-*@3 page].&]
[s3;%% &]
[s4; &]
[s5;:PixBase`:`:GetHeight`(`): [@(0.0.255) virtual] [@(0.0.255) int]_[* GetHeight]()&]
[s2;%% Returns the height of Raster in pixels for active page.&]
[s3; &]
[s4; &]
[s5;:PixBase`:`:GetHeightEx`(int`): [@(0.0.255) virtual] [@(0.0.255) int]_[* GetHeightEx]([@(0.0.255) i
nt]_[@3 page])&]
[s2;%% Returns the height of Raster in pixels for [%-*@3 page].&]
[s3;%% &]
[s4; &]
[s5;:PixBase`:`:GetInfo`(`): [@(0.0.255) virtual] [_^Raster`:`:Info^ Info]_[* GetInfo]()&]
[s2;%% Returns the information about Raster`'s active page.&]
[s3; &]
[s4; &]
[s5;:PixBase`:`:GetInfoEx`(int`): [@(0.0.255) virtual] [_^Raster`:`:Info^ Info]_[* GetInfoE
x(][@(0.0.255) int]_[@3 page])&]
[s2;%% Returns the information about Raster for [%-*@3 page].&]
[s3;%% &]
[s4; &]
[s5;:PixBase`:`:GetLine`(int`): [@(0.0.255) virtual] [_^Raster`:`:Line^ Line]_[* GetLine]([@(0.0.255) i
nt]_[@3 line])&]
[s2;%% Reads a single scanline [%-*@3 line] from the raster`'s active 
page. If possible, Raster should be optimized for reading scanlines 
in ascending order `- this what most processing functions (should) 
require.&]
[s3;%% &]
[s4; &]
[s5;:PixBase`:`:GetLineEx`(int`,int`): [@(0.0.255) virtual] [_^Raster`:`:Line^ Line]_[* Get
LineEx]([@(0.0.255) int]_[@3 line], [@(0.0.255) int]_[@3 page])&]
[s2;%% Reads a single scanline [%-*@3 line] from [%-*@3 page] ot the 
raster. If possible, Raster should be optimized for reading scanlines 
in ascending order `- this what most processing functions (should) 
require.&]
[s3;%% &]
[s4; &]
[s5;:PixBase`:`:GetPaletteCount`(`): [@(0.0.255) virtual] [@(0.0.255) int]_[* GetPaletteCou
nt]()&]
[s2;%% Returns the size of palette for raster`'s active page. If 
there is no palette, returns 0&]
[s3; &]
[s4; &]
[s5;:PixBase`:`:GetPaletteCountEx`(int`): [@(0.0.255) virtual] [@(0.0.255) int]_[* GetPalet
teCountEx]([@(0.0.255) int]_[@3 page])&]
[s2;%% Returns the size of palette for raster`'s [%-*@3 page]. If there 
is no palette, returns 0..&]
[s3;%% &]
[s4; &]
[s5;:PixBase`:`:GetPalette`(`): [@(0.0.255) virtual] [@(0.0.255) const]_[_^RGBA^ RGBA]_`*[* G
etPalette]()&]
[s2;%% Returns active pages`'current palette, NULL if there is no 
palette.&]
[s3; &]
[s4; &]
[s5;:PixBase`:`:GetPaletteEx`(int`): [@(0.0.255) virtual] [@(0.0.255) const]_[_^RGBA^ RGBA]_
`*[* GetPaletteEx]([@(0.0.255) int]_[@3 page])&]
[s2;%% Returns [%-*@3 page][%- `'s ]current palette, NULL if there is 
no palette &]
[s3;%% &]
[s4; &]
[s5;:PixBase`:`:GetFormat`(`): [@(0.0.255) virtual] [@(0.0.255) const]_[_^RasterFormat^ Ras
terFormat]_`*[* GetFormat]()&]
[s2;%% Returns the format of Raster`'s active page, can return NULL 
if format is RGBA.&]
[s3; &]
[s4; &]
[s5;:PixBase`:`:GetFormatEx`(int`): [@(0.0.255) virtual] [@(0.0.255) const]_[_^RasterFormat^ R
asterFormat]_`*[* GetFormatEx]([@(0.0.255) int]_[@3 page])&]
[s2;%% Returns the format of Raster`'s [%-*@3 page], can return NULL 
if format is RGBA.&]
[s3;%% &]
[s4; &]
[s5;:PixBase`:`:IsEmpty`(`): [@(0.0.255) bool]_[* IsEmpty]()&]
[s2;%% Returns [%-@(0.0.255) true] if raster contains no images.&]
[s3; &]
[s4; &]
[s5;:PixBase`:`:operator bool`(`): [* operator_bool]()&]
[s2;%% Returns true if raster has some content, false otherwise&]
[s3; &]
[s4; &]
[s5;:PixBase`:`:Clear`(`): [@(0.0.255) virtual] [@(0.0.255) void]_[* Clear]()_`=_[@3 0]&]
[s2;%% Clears raster content&]
[s3; &]
[s0; &]
[ {{10000F(128)G(128)@1 [s0;%% [* Polygon markers access]]}}&]
[s0; &]
[s4; &]
[s5;:PixBase`:`:GetPolyMarkers`(`): [@(0.0.255) virtual] [_^PolyMarkers^ PolyMarkers]_`*[* G
etPolyMarkers]()&]
[s2;%% Returns array of polygon markers for current raster page&]
[s3; &]
[s4; &]
[s5;:PixBase`:`:GetPolyMarkersEx`(int`): [@(0.0.255) virtual] [_^PolyMarkers^ PolyMarkers
]_`*[* GetPolyMarkersEx]([@(0.0.255) int]_[@3 page])&]
[s2; [%% Returns array of polygon markers for raster ][@3 page]&]
[s3; &]
[s0; ]