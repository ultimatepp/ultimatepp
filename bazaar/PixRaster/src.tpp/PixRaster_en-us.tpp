topic "class PixRaster : public PixBase";
[ $$0,0#00000000000000000000000000000000:Default]
[i448;a25;kKO9; $$1,0#37138531426314131252341829483380:structitem]
[l288;2 $$2,0#27521748481378242620020725143825:desc]
[0 $$3,0#96390100711032703541132217272105:end]
[H6;0 $$4,0#05600065144404261032431302351956:begin]
[*2 $$5,0#37138531426314131252341829483370:codeitem]
[{_} 
[s1;:PixRaster`:`:class: [@(0.0.255) class]_[* PixRaster]_:_[@(0.0.255) public]_[*@3 PixBase]&]
[s2;%% Multipaged Leptonica raster object.&]
[s2;%% It contains an array of [* Pix] single`-page Leptonica raster 
objects&]
[s2;%% &]
[s3; &]
[s0; &]
[ {{10000F(128)G(128)@1 [s0;%% [* Constructors and destructor]]}}&]
[s4; &]
[s5;:PixRaster`:`:PixRaster`(`): PixRaster()&]
[s2;%% Constructs an empty PixRaster object.&]
[s3; &]
[s4; &]
[s5;:PixRaster`:`:PixRaster`(PIX`*`*`): PixRaster([_^PIX^ PIX]_`*`*[@3 pix])&]
[s2;%% [%- Constructs a single page PixRaster from a Leptonica][%-*@3  
pix] image.&]
[s2;%% PixRaster takes ownership of PIX object; to avoid misuse of 
it, [%-*@3 pix] is cleared.&]
[s3;%% &]
[s4; &]
[s5;:PixRaster`:`:PixRaster`(PIXA`*`*`): PixRaster([_^PIXA^ PIXA]_`*`*[@3 pixa])&]
[s2;%% Constructs a multipage PixRaster from a Leptonica [%-*@3 pixa] 
array of images.&]
[s2;%% PixRaster takes ownership of array; to avoid misuse of it, 
[%-*@3 pixa] is cleared.&]
[s3;%% &]
[s4; &]
[s5;:PixRaster`:`:PixRaster`(Raster`&`,bool`): PixRaster([_^Raster^ Raster]_`&[@3 raster],
 [@(0.0.255) bool]_[@3 deepCopy]_`=_[@(0.0.255) false], [@(0.0.255) int]_[@3 page]_`=_PIXRA
STER`_CURPAGE)&]
[s2;%% Constructs PixRaster from a source [%-*@3 raster] with optional 
[%-*@3 deepCopy].&]
[s3;%% &]
[s4; &]
[s5;:PixRaster`:`:`~PixRaster`(`): [@(0.0.255) `~]PixRaster()&]
[s2;%% Destructs PixRaster object and frees internal resources&]
[s3; &]
[ {{10000F(128)G(128)@1 [s0;%% [* Internal handling of Leptonica objects]]}}&]
[s4; &]
[s5;:PixRaster`:`:GetPIX`(int`): [_^PIX^ PIX]_`*GetPIX([@(0.0.255) int]_[@3 page]_`=_PIXRAS
TER`_CURPAGE)&]
[s2;%% Gets underlying Leptonica [* PIX ]object for [%-*@3 page].&]
[s2;%% [* WARNING], [* Pix] owns [* PIX ]object, so don`'t free it.&]
[s3;%% &]
[s4; &]
[s5;:PixRaster`:`:operator PIX`*`(`): operator_PIX`*()&]
[s2;%% same as [^topic`:`/`/Leptonica`/src`/PixRaster`$en`-us`#PixRaster`:`:GetPIX`(int`,CopyModes`)^ G
etPIX](PIXRASTER`_CURPAGE)&]
[s3; &]
[s0; &]
[ {{10000F(128)G(128)@1 [s0;%% [* Member images access]]}}&]
[s0; &]
[s4; &]
[s5;:PixRaster`:`:Set`(Pix`&`,int`,bool`):%% [%-@(0.0.255) void][%- _Set(][%-_^PIX^ Pix][%- _
`&][%-@3 pix][%- , ][%-@(0.0.255) int][%- _][%-@3 page][%- _`=_PIXRASTER`_CURPAGE, 
][%-@(0.0.255) bool] [%-@3 DeepCopy][%- `=_][%-@(0.0.255) false])&]
[s2;%% sets [%-*@3 pix] at [%-*@3 page] with optional [%-*@3 DeepCopy].&]
[s0;%% &]
[s4; &]
[s5;:PixRaster`:`:Add`(Pix`&`,bool`): [@(0.0.255) void]_Add([_^PIX^ Pix]_`&[@3 pix], 
[@(0.0.255) bool][%%  ][@3 DeepCopy]`=_[@(0.0.255) false])&]
[s2;%% Appends a [%-*@3 pix] to the end of PixRaster with optional 
[%-*@3 DeepCopy] .&]
[s3;%% &]
[s4; &]
[s5;:PixRaster`:`:Add`(PixRaster`&`,bool`): [@(0.0.255) void]_Add([_^PIXA^ PixRaster]_`&[@3 p
ixr], [@(0.0.255) bool][%%  ][@3 DeepCopy]`=_[@(0.0.255) false])&]
[s2;%% Appends [%-*@3 pixr] PixRaster at end of current one with optional 
[%-*@3 DeepCopy] &]
[s3;%% &]
[s4; &]
[s5;:PixRaster`:`:Insert`(Pix`&`,int`,bool`): [@(0.0.255) void]_Insert([_^PIX^ Pix]_`&[@3 p
ix], [@(0.0.255) int]_[@3 where]_`=_PIXRASTER`_CURPAGE, [@(0.0.255) bool][%%  
][@3 DeepCopy]`=_[@(0.0.255) false])&]
[s2;%% Inserts Pix [%-*@3 pix] at [%-*@3 where] position with optional 
[%-*@3 DeepCopy] &]
[s3;%% &]
[s4; &]
[s5;:PixRaster`:`:Insert`(PixRaster`&`,int`,bool`): [@(0.0.255) void]_Insert([_^PIXA^ Pix
Raster]_`&[@3 pixr], [@(0.0.255) int]_[@3 where]_`=_PIXRASTER`_CURPAGE, 
[@(0.0.255) bool][%%  ][@3 DeepCopy]`=_[@(0.0.255) false])&]
[s2;%% Inserts [%-*@3 pixr] PixRaster at [%-*@3 where] position with 
optional [%-*@3 DeepCopy] &]
[s3;%% &]
[s4; &]
[s5;:PixRaster`:`:Remove`(int`,int`): [@(0.0.255) void]_Remove([@(0.0.255) int]_[@3 startPa
ge]_`=_PIXRASTER`_CURPAGE, [@(0.0.255) int]_[@3 count]_`=_[@3 1])&]
[s2;%% Removes [%-*@3 count ]Pix images starting at [%-*@3 startPage 
]position.&]
[s3;%% &]
[s4; &]
[s5;:PixRaster`:`:operator`[`]`(int`): [_^Pix^ Pix]_`&operator`[`]([@(0.0.255) int]_[@3 pag
e])&]
[s2;%% Access [* Pix] at [%-*@3 page].&]
[s3;%% &]
[s4; &]
[s5;:PixRaster`:`:operator Pix`&`(`): operator_Pix`&()&]
[s2;%% Access [* Pix ]at current page.&]
[s3; &]
[s4; &]
[s5;:PixRaster`:`:At`(int`): [_^Pix^ Pix]_`&At([@(0.0.255) int]_[@3 page])&]
[s2;%% Access [* Pix ]at [%-*@3 page].&]
[s3;%% &]
[s4; &]
[s5;:PixRaster`:`:Clear`(`): [@(0.0.255) virtual] [@(0.0.255) void]_Clear()&]
[s2;%% Clears PixRaster content.&]
[s3; &]
[s0; &]
[ {{10000F(128)G(128)@1 [s0;%% [* Stack`-like operations]]}}&]
[s4; &]
[s5;:PixRaster`:`:Dup`(int`): [@(0.0.255) void]_Dup([@(0.0.255) int]_[@3 page]_`=_PIXRASTER
`_CURPAGE)&]
[s2;%% Duplicates image at [%-*@3 page] putting the copy at the end 
of Pixraster`'s array. Active page is set to the newly created 
one.&]
[s3;%% &]
[s4; &]
[s5;:PixRaster`:`:Drop`(int`): [@(0.0.255) void]_Drop([@(0.0.255) int]_[@3 count]_`=_[@3 1])&]
[s2;%% Removes [%-*@3 count] images starting at the end of PixRaster`'s 
array and going backwards. Active page is set to last one.&]
[s3;%% &]
[s0; &]
[ {{10000F(128)G(128)@1 [s0;%% [* Page handling functions]]}}&]
[s4; &]
[s5;:PixRaster`:`:SeekPage`(int`): [@(0.0.255) virtual] [@(0.0.255) void]_SeekPage([@(0.0.255) i
nt]_[@3 page])&]
[s2;%% Sets active [%-*@3 page].&]
[s3;%% &]
[s4; &]
[s5;:PixRaster`:`:GetPageCount`(`): [@(0.0.255) virtual] [@(0.0.255) int]_GetPageCount()&]
[s2;%% Gets number of images on PixRaster&]
[s3; &]
[s4; &]
[s5;:PixRaster`:`:GetActivePage`(`): [@(0.0.255) virtual] [@(0.0.255) int]_GetActivePage(
)&]
[s2;%% Gets number of currently active page&]
[s3; &]
[ {{10000F(128)G(128)@1 [s0;%% [* Miscellaneous Raster functions]]}}&]
[s4; &]
[s5;:PixRaster`:`:GetSize`(void`): [@(0.0.255) virtual] [_^Size^ Size]_GetSize([@(0.0.255) v
oid])&]
[s2;%% Returns the size of Raster`'s active page in pixels.&]
[s3; &]
[s4; &]
[s5;:PixRaster`:`:GetSizeEx`(int`): [@(0.0.255) virtual] [_^Size^ Size]_GetSizeEx([@(0.0.255) i
nt]_[@3 page])&]
[s2;%% Returns the size of Raster in pixels for [%-*@3 page].&]
[s3;%% &]
[s4; &]
[s5;:PixRaster`:`:GetWidth`(`): [@(0.0.255) virtual] [@(0.0.255) int]_GetWidth()&]
[s2;%% Returns the width of Raster in pixels for active page.&]
[s3; &]
[s4; &]
[s5;:PixRaster`:`:GetWidthEx`(int`): [@(0.0.255) virtual] [@(0.0.255) int]_GetWidthEx([@(0.0.255) i
nt]_[@3 page])&]
[s2;%% Returns the width of Raster in pixels for [%-*@3 page].&]
[s3;%% &]
[s4; &]
[s5;:PixRaster`:`:GetHeight`(`): [@(0.0.255) virtual] [@(0.0.255) int]_GetHeight()&]
[s2;%% Returns the height of Raster in pixels for active page.&]
[s3; &]
[s4; &]
[s5;:PixRaster`:`:GetHeightEx`(int`): [@(0.0.255) virtual] [@(0.0.255) int]_GetHeightEx([@(0.0.255) i
nt]_[@3 page])&]
[s2;%% Returns the height of Raster in pixels for [%-*@3 page].&]
[s3;%% &]
[s4; &]
[s5;:PixRaster`:`:GetInfo`(void`): [@(0.0.255) virtual] [_^Raster`:`:Info^ Info]_GetInfo(
[@(0.0.255) void])&]
[s2;%% Returns the information about Raster`'s active page.&]
[s3; &]
[s4; &]
[s5;:PixRaster`:`:GetInfoEx`(int`): [@(0.0.255) virtual] [_^Raster`:`:Info^ Info]_GetInfo
Ex([@(0.0.255) int]_[@3 page])&]
[s2;%% Returns the information about Raster for [%-*@3 page].&]
[s3;%% &]
[s4; &]
[s5;:PixRaster`:`:GetLine`(int`): [@(0.0.255) virtual] [_^Raster`:`:Line^ Line]_GetLine([@(0.0.255) i
nt]_[@3 line])&]
[s2;%% Reads a single scanline [%-*@3 line] from the raster`'s active 
page. If possible, Raster should be optimized for reading scanlines 
in ascending order `- this what most processing functions (should) 
require.&]
[s3;%% &]
[s4; &]
[s5;:PixRaster`:`:GetLineEx`(int`,int`): [@(0.0.255) virtual] [_^Raster`:`:Line^ Line]_Ge
tLineEx([@(0.0.255) int]_[@3 line], [@(0.0.255) int]_[@3 page])&]
[s2;%% Reads a single scanline [%-*@3 line] from [%-*@3 page] ot the 
raster. If possible, Raster should be optimized for reading scanlines 
in ascending order `- this what most processing functions (should) 
require.&]
[s3;%% &]
[s4; &]
[s5;:PixRaster`:`:GetPaletteCount`(`): [@(0.0.255) virtual] [@(0.0.255) int]_GetPaletteCo
unt()&]
[s2;%% Returns the size of palette for raster`'s active page. If 
there is no palette, returns 0&]
[s3; &]
[s4; &]
[s5;:PixRaster`:`:GetPaletteCountEx`(int`): [@(0.0.255) virtual] [@(0.0.255) int]_GetPale
tteCountEx([@(0.0.255) int]_[@3 page])&]
[s2;%% Returns the size of palette for raster`'s [%-*@3 page]. If there 
is no palette, returns 0..&]
[s3;%% &]
[s4; &]
[s5;:PixRaster`:`:GetPalette`(`): [@(0.0.255) virtual] [@(0.0.255) const]_[_^RGBA^ RGBA]_`*
GetPalette()&]
[s2;%% Returns active pages`'current palette, NULL if there is no 
palette.&]
[s3; &]
[s4; &]
[s5;:PixRaster`:`:GetPaletteEx`(int`): [@(0.0.255) virtual] [@(0.0.255) const]_[_^RGBA^ RGB
A]_`*GetPaletteEx([@(0.0.255) int]_[@3 page])&]
[s2;%% Returns [%-*@3 page][%- `'s ]current palette, NULL if there is 
no palette &]
[s3;%% &]
[s4; &]
[s5;:PixRaster`:`:GetFormat`(`): [@(0.0.255) virtual] [@(0.0.255) const]_[_^RasterFormat^ R
asterFormat]_`*GetFormat()&]
[s2;%% Returns the format of Raster`'s active page, can return NULL 
if format is RGBA.&]
[s3; &]
[s4; &]
[s5;:PixRaster`:`:GetFormatEx`(int`): [@(0.0.255) virtual] [@(0.0.255) const]_[_^RasterFormat^ R
asterFormat]_`*GetFormatEx([@(0.0.255) int]_[@3 page])&]
[s2;%% Returns the format of Raster`'s [%-*@3 page], can return NULL 
if format is RGBA.&]
[s3;%% &]
[s4; &]
[s5;:PixRaster`:`:IsEmpty`(void`): [@(0.0.255) bool]_IsEmpty([@(0.0.255) void])&]
[s2;%% Returns true if PixRaster has no images.&]
[s3; &]
[s0; &]
[ {{10000F(128)G(128)@1 [s0;%% [* Polygon markers access]]}}&]
[s0; &]
[s4; &]
[s5;:PixRaster`:`:GetPolyMarkers`(`): [@(0.0.255) virtual] [_^PolyMarkers^ PolyMarkers]_`*
GetPolyMarkers()&]
[s2;%% Returns array of polygon markers for current raster page&]
[s3; &]
[s4; &]
[s5;:PixRaster`:`:GetPolyMarkersEx`(int`): [@(0.0.255) virtual] [_^PolyMarkers^ PolyMarke
rs]_`*GetPolyMarkersEx([@(0.0.255) int]_[@3 page])&]
[s2;%% Returns array of polygon markers for raster [%-*@3 page].&]
[s3;%% &]
[s0; &]
[ {{10000F(128)G(128)@1 [s0;%% [* Conversion and file I/O functions]]}}&]
[s0; &]
[s4; &]
[s5;:PixRaster`:`:Load`(Raster`&`,bool`,bool`): [@(0.0.255) void]_Load([_^Raster^ Raster][@(0.0.255) `&
]_[@3 raster], [@(0.0.255) bool]_[@3 Append]_`=_[@(0.0.255) false], [@(0.0.255) bool]_[@3 Dee
pCopy]_`=_[@(0.0.255) false])&]
[s2;%% Loads image array from [%-*@3 raster] object or [%-*@3 Append] 
them to current one. Source images are referenced or deep copied 
depending on [%-*@3 DeepCopy] parameter.&]
[s3;%% &]
[s4; &]
[s5;:PixRaster`:`:operator`=`(Raster`&`): [@(0.0.255) void]_operator`=([_^Raster^ Raster]_
`&[@3 raster])&]
[s2;%% Same as [*^topic`:`/`/Leptonica`/src`/PixRaster`$en`-us`#PixRaster`:`:Load`(Raster`&`,bool`,CopyModes`)^ L
oad][* (][%-*@3 raster][* , ][%-*@(0.0.255) false][* , ][%-*@(0.0.255) false][* )] 
 Sets PixRaster`'s content equal to [%-*@3 raster]`'s one.&]
[s3;%% &]
[s4; &]
[s5;:PixRaster`:`:operator`+`=`(Raster`&`): [@(0.0.255) void]_operator`+`=([_^Raster^ Ras
ter]_`&[@3 raster])&]
[s2;%% Same as [*^topic`:`/`/Leptonica`/src`/PixRaster`$en`-us`#PixRaster`:`:Load`(Raster`&`,bool`,CopyModes`)^ L
oad][* (][%-*@3 raster], [%-*@(0.0.255) true][* , ][%-*@(0.0.255) false][* )] 
 Appends [%-*@3 raster]`'s content at the end of PixRaster`'s one&]
[s3; &]
[s4; &]
[s5;:PixRaster`:`:operator`<`<`=`(Raster`&`): [_^PixRaster^ PixRaster]_`&[@(0.0.255) oper
ator]_<<`=(Raster_`&[@3 raster])&]
[s2;%% Same as [*^topic`:`/`/Leptonica`/src`/PixRaster`$en`-us`#PixRaster`:`:Load`(Raster`&`,bool`,CopyModes`)^ L
oad][* (][%-*@3 raster][* , ][%-*@(0.0.255) false][* , ][%-*@(0.0.255) true][* )] 
 Sets PixRaster`'s content equal to [%-*@3 raster]`'s one deep 
copying it.&]
[s3;%% &]
[s4; &]
[s5;:PixRaster`:`:Load`(FileIn`&`,bool`): [@(0.0.255) bool]_Load([_^FileIn^ FileIn]_`&[@3 f
s], [@(0.0.255) bool]_[@3 Append]_`=_[@(0.0.255) false])&]
[s2;%% Loads images from stream [%-*@3 fs][%- ,] [%-*@3 Append][%- ing them 
to current PixRaster`'s content if required].&]
[s2;%% [* WARNING], it seems that don`'t support 8 bit alpha channel 
images.&]
[s0;%% &]
[s2;%% Returns [%-@(0.0.255) true] on success, [%-@(0.0.255) false] otherwise.&]
[s3;%% &]
[s4; &]
[s5;:PixRaster`:`:Load`(String`,bool`): [@(0.0.255) bool]_Load([_^String^ String]_[@3 fileN
ame], [@(0.0.255) bool]_[@3 Append]_`=_[@(0.0.255) false])&]
[s2;%% Loads images from [%-*@3 fileName ][%- named file,] [%-*@3 Append][%- ing 
them to current PixRaster`'s content if required].&]
[s2;%% [* WARNING], it seems that don`'t support 8 bit alpha channel 
images.&]
[s0;%% &]
[s2;%% Returns [%-@(0.0.255) true] on success, [%-@(0.0.255) false] otherwise.&]
[s3;%% &]
[s4; &]
[s5;:PixRaster`:`:Save`(String`,int`): [@(0.0.255) bool]_Save([_^String^ String]_[@3 fileNa
me], [@(0.0.255) int]_[@3 page]_`=_PIXRASTER`_CURPAGE)&]
[s2;%% Stores a single [%-*@3 page ][%- of PixRaster to] [%-*@3 fileName] 
named file.&]
[s0;%% &]
[s2;%% Returns [%-@(0.0.255) true] on success, [%-@(0.0.255) false] otherwise.&]
[s3;%% &]
[s4; &]
[s5;:PixRaster`:`:SaveAll`(String`): [@(0.0.255) bool]_SaveAll([_^String^ String]_[@3 fileN
ame])&]
[s2;%% Stores all pages of PixRaster to a [%-*@3 fileName] named file; 
choosen file format must support multiple pages.&]
[s0;%% &]
[s2;%% Returns [%-@(0.0.255) true] on success, [%-@(0.0.255) false] otherwise.&]
[s3;%% &]
[s0; ]