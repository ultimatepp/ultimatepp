topic "class PixRaster";
[ $$0,0#00000000000000000000000000000000:Default]
[i448;a25;kKO9; $$1,0#37138531426314131252341829483380:structitem]
[l288;2 $$2,0#27521748481378242620020725143825:desc]
[0 $$3,0#96390100711032703541132217272105:end]
[H6;0 $$4,0#05600065144404261032431302351956:begin]
[*2 $$5,0#37138531426314131252341829483370:codeitem]
[{_} 
[s1;:PixRaster`:`:class: [@(0.0.255) class]_[* PixRaster]_:_[@(0.0.255) public]_[*@3 Raster]&]
[s2;%% &]
[s3; &]
[s0; &]
[ {{10000F(128)G(128)@1 [s0;%% [* Public enums]]}}&]
[s0;2 &]
[s4; &]
[s5;:PixRaster`:`:PIXRASTER`_REF: [@(0.0.255) enum]_CopyModes&]
[s2;%% &]
[s2; Gives ownership of internal Leptonica PIX images and PIXA arrays 
when creating or copying PixRaster objects :&]
[s0; &]
[s5; -|PIXRASTER`_CLONE -|-|[* PIX and PIXA`'s reference count gets increased]&]
[s5; -|PIXRASTER`_COPY-|-|-|[* PIX and PIXAs are deep cloned]&]
[s5; -|PIXRASTER`_COPY`_CLONE-|-|[* PIX`'s reference count increased and 
PIXA deep cloned]&]
[s5; -|PIXRASTER`_REF-|-|-|[* PIX and PIXAs are just referenced by pointer]&]
[s0; &]
[s0; [2 This enum is used mostly when dealing directly with Leptonica 
PIX and PIXA objects. PixRaster provides wrapping for most Leptonica 
functions, so user usually don`'t care of internal objects ownership.]&]
[s0; [2 If you need to access internal objects keep in mind that cloned 
or referenced objects may change by means of other calls; in 
doubt use PIXRASTER`_COPY to get a full image copy.]&]
[s3; &]
[s4; &]
[s5;:PixRaster`:`:PIXRASTER`_BRING`_IN`_WHITE: [@(0.0.255) enum]_BringInModes&]
[s2;%% &]
[s2; Color filling mode in operations that adds parts to images, 
for example rotations :&]
[s0; &]
[s5; -|PIXRASTER`_BRING`_IN`_WHITE-|[* Fills new parts with white color]&]
[s5; -|PIXRASTER`_BRING`_IN`_BLACK-|[* Fills new parts with black color]&]
[s2;%% &]
[s3; &]
[s0;*%% &]
[ {{10000F(128)G(128)@1 [s0;%% [* Constructors and destructor]]}}&]
[s4; &]
[s5;:PixRaster`:`:PixRaster`(`): PixRaster()&]
[s2;%% Constructs an empty PixRaster object.&]
[s3; &]
[s4; &]
[s5;:PixRaster`:`:PixRaster`(PIX`*`,CopyModes`): PixRaster([_^PIX^ PIX]_`*[@3 pix], 
CopyModes_[@3 copyMode]_`=_PIXRASTER`_CLONE)&]
[s2;%% [%- Constructs a single page PixRaster from a Leptonica][%-*@3  
pix] image; ownership of image is assigned by [%-*@3 copyMode] 
parameter&]
[s3;%% &]
[s4; &]
[s5;:PixRaster`:`:PixRaster`(PIXA`*`,CopyModes`): PixRaster([_^PIXA^ PIXA]_`*[@3 pixa], 
CopyModes_[@3 copyMode]_`=_PIXRASTER`_COPY`_CLONE)&]
[s2;%% Constructs a multipage PixRaster from a Leptonica [%-*@3 pixa] 
array of images; ownership of array and images is assigned by 
[%-*@3 copyMode] parameter&]
[s3;%% &]
[s4; &]
[s5;:PixRaster`:`:`~PixRaster`(`): [@(0.0.255) `~]PixRaster()&]
[s2;%% Destructs PixRaster object and frees internal resources&]
[s3; &]
[ {{10000F(128)G(128)@1 [s0;%% [* Internal handling of Leptonica objects]]}}&]
[s4; &]
[s5;:PixRaster`:`:GetPIX`(int`,CopyModes`): [_^PIX^ PIX]_`*GetPIX([@(0.0.255) int]_[@3 page
]_`=_PIXRASTER`_CURPAGE, CopyModes_[@3 copyMode]_`=_PIXRASTER`_REF)&]
[s2;%% gets internal [* PIX] object from given [%-*@3 page] , defaulting 
to current one.&]
[s2;%% [%-*@3 copyMode] sets the ownership of retrieved object, defaulting 
to simple reference.&]
[s3;%% &]
[s4; &]
[s5;:PixRaster`:`:GetPIXA`(CopyModes`): [_^PIXA^ PIXA]_`*GetPIXA(CopyModes_[@3 copyMode]_
`=_PIXRASTER`_REF)&]
[s2;%% gets internal PIXA image array; [%-*@3 copyMode] sets the ownership 
of retrieved array, defaulting to simple reference&]
[s3;%% &]
[s4; &]
[s5;:PixRaster`:`:operator PIX`*`(void`): operator_PIX`*([@(0.0.255) void])&]
[s2;%% same as [^topic`:`/`/Leptonica`/src`/PixRaster`$en`-us`#PixRaster`:`:GetPIX`(int`,CopyModes`)^ G
etPIX](PIXRASTER`_CURPAGE, PIXRASTER`_REF)&]
[s3; &]
[s4; &]
[s5;:PixRaster`:`:SetPIX`(PIX`*`,int`,CopyModes`): [@(0.0.255) void]_SetPIX([_^PIX^ PIX]_
`*[@3 pix], [@(0.0.255) int]_[@3 page]_`=_PIXRASTER`_CURPAGE, CopyModes_[@3 copyMode]_`=_
PIXRASTER`_CLONE)&]
[s2;%% sets [%-*@3 pix] at [%-*@3 page] with ownership given by [%-*@3 copyMode]. 
Previous PIX gets it`'s reference decreased and is freed if necessary.&]
[s0;%% &]
[s3;%% &]
[s4; &]
[s5;:PixRaster`:`:SetPIXA`(PIXA`*`,CopyModes`): [@(0.0.255) void]_SetPIXA([_^PIXA^ PIXA]_
`*[@3 pixa], CopyModes_[@3 copyMode]_`=_PIXRASTER`_COPY`_CLONE)&]
[s2;%% replaces image array with [%-*@3 pixa] using ownership given 
by [%-*@3 copyMode]. Previous image array gets it`'s reference 
decreased and is freed if necessary.&]
[s3;%% &]
[s4; &]
[s5;:PixRaster`:`:AddPIX`(PIX`*`,CopyModes`): [@(0.0.255) void]_AddPIX([_^PIX^ PIX]_`*[@3 p
ix], CopyModes_[@3 copyMode]_`=_PIXRASTER`_CLONE)&]
[s2;%% Appends a [%-*@3 pix] to the end of PixRaster using ownership 
given by [%-*@3 copyMode].&]
[s3;%% &]
[s4; &]
[s5;:PixRaster`:`:AddPIXA`(PIXA`*`,CopyModes`): [@(0.0.255) void]_AddPIXA([_^PIXA^ PIXA]_
`*[@3 pixa], CopyModes_[@3 copyMode]_`=_PIXRASTER`_COPY`_CLONE)&]
[s2;%% Appends a [%-*@3 pixa] image array to the end of PixRaster using 
ownership given by [%-*@3 copyMode].Valid modes are only PIXRASTER`_COPY 
and PIXRASTER`_CLONE.&]
[s3;%% &]
[s4; &]
[s5;:PixRaster`:`:InsertPIX`(PIX`*`,int`,CopyModes`): [@(0.0.255) void]_InsertPIX([_^PIX^ P
IX]_`*[@3 pix], [@(0.0.255) int]_[@3 where]_`=_PIXRASTER`_CURPAGE, 
CopyModes_[@3 copyMode]_`=_PIXRASTER`_COPY`_CLONE)&]
[s2;%% Inserts a [%-*@3 pix] at [%-*@3 where] position using ownership 
given by [%-*@3 copyMode].&]
[s3;%% &]
[s4; &]
[s5;:PixRaster`:`:InsertPIXA`(PIXA`*`,int`,CopyModes`): [@(0.0.255) void]_InsertPIXA([_^PIXA^ P
IXA]_`*[@3 pixa], [@(0.0.255) int]_[@3 where]_`=_PIXRASTER`_CURPAGE, 
CopyModes_[@3 copyMode]_`=_PIXRASTER`_COPY`_CLONE)&]
[s2;%% Inserts a [%-*@3 pixa] image array at [%-*@3 where] position using 
ownership given by [%-*@3 copyMode].Valid modes are only PIXRASTER`_COPY 
and PIXRASTER`_CLONE.&]
[s3;%% &]
[s4; &]
[s5;:PixRaster`:`:RemovePIX`(int`,int`): [@(0.0.255) void]_RemovePIX([@(0.0.255) int]_[@3 s
tartPage]_`=_PIXRASTER`_CURPAGE, [@(0.0.255) int]_[@3 count]_`=_[@3 1])&]
[s2;%% Removes [%-*@3 count ]PIX images starting at [%-*@3 startPage 
]position.&]
[s3;%% &]
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
[ {{10000F(128)G(128)@1 [s0;%% [* Conversions support from other Raster derived objects]]}}&]
[s4; &]
[s5;:PixRaster`:`:Load`(Raster`&`,bool`,CopyModes`): [@(0.0.255) void]_Load([_^Raster^ Ra
ster][@(0.0.255) `&]_[@3 raster], [@(0.0.255) bool]_[@3 Append]_`=_[@(0.0.255) false], 
CopyModes_[@3 copyMode]_`=_PIXRASTER`_CLONE)&]
[s2;%% Loads image array from [%-*@3 raster] object or [%-*@3 Append] 
them to current one. Ownership of added images is given by [%-*@3 copyMode].&]
[s3;%% &]
[s4; &]
[s5;:PixRaster`:`:operator`=`(Raster`&`): [@(0.0.255) void]_operator`=([_^Raster^ Raster]_
`&[@3 raster])&]
[s2;%% Same as [*^topic`:`/`/Leptonica`/src`/PixRaster`$en`-us`#PixRaster`:`:Load`(Raster`&`,bool`,CopyModes`)^ L
oad][* (][%-*@3 raster][* , ][%-*@(0.0.255) false][* , PIXRASTER`_COPY)] 
 Sets PixRaster`'s content equal to [%-*@3 raster]`'s one.&]
[s3;%% &]
[s4; &]
[s5;:PixRaster`:`:operator`+`=`(Raster`&`): [@(0.0.255) void]_operator`+`=([_^Raster^ Ras
ter]_`&[@3 raster])&]
[s2;%% Same as [*^topic`:`/`/Leptonica`/src`/PixRaster`$en`-us`#PixRaster`:`:Load`(Raster`&`,bool`,CopyModes`)^ L
oad][* (][%-*@3 raster], [%-*@(0.0.255) true][* , PIXRASTER`_COPY)]  
Appends [%-*@3 raster]`'s content at the end of Pixraster`'s one&]
[s0; &]
[ {{10000F(128)G(128)@1 [s0;%% [* File I/O]]}}&]
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
[s5;:PixRaster`:`:GetSize`(int`): [@(0.0.255) virtual] [_^Size^ Size]_GetSize([@(0.0.255) i
nt]_[@3 page])&]
[s2;%% Returns the size of Raster in pixels for [%-*@3 page].&]
[s3;%% &]
[s4; &]
[s5;:PixRaster`:`:GetInfo`(void`): [@(0.0.255) virtual] [_^Raster`:`:Info^ Info]_GetInfo(
[@(0.0.255) void])&]
[s2;%% Returns the information about Raster`'s active page.&]
[s3; &]
[s4; &]
[s5;:PixRaster`:`:GetInfo`(int`): [@(0.0.255) virtual] [_^Raster`:`:Info^ Info]_GetInfo([@(0.0.255) i
nt]_[@3 page])&]
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
[s5;:PixRaster`:`:GetLine`(int`,int`): [@(0.0.255) virtual] [_^Raster`:`:Line^ Line]_GetL
ine([@(0.0.255) int]_[@3 line], [@(0.0.255) int]_[@3 page])&]
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
[s5;:PixRaster`:`:GetPaletteCount`(int`): [@(0.0.255) virtual] [@(0.0.255) int]_GetPalett
eCount([@(0.0.255) int]_[@3 page])&]
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
[s5;:PixRaster`:`:GetPalette`(int`): [@(0.0.255) virtual] [@(0.0.255) const]_[_^RGBA^ RGBA]_
`*GetPalette([@(0.0.255) int]_[@3 page])&]
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
[s5;:PixRaster`:`:GetFormat`(int`): [@(0.0.255) virtual] [@(0.0.255) const]_[_^RasterFormat^ R
asterFormat]_`*GetFormat([@(0.0.255) int]_[@3 page])&]
[s2;%% Returns the format of Raster`'s [%-*@3 page], can return NULL 
if format is RGBA.&]
[s3;%% &]
[s4; &]
[s5;:PixRaster`:`:IsEmpty`(void`): [@(0.0.255) bool]_IsEmpty([@(0.0.255) void])&]
[s2;%% Returns true if PixRaster has no images.&]
[s3; &]
[s0; &]
[ {{10000F(128)G(128)@1 [s0;%% [* Leptonica functions wrappers]]}}&]
[s0; &]
[s2; [*^topic`:`/`/Leptonica`/src`/PixRaster`$en`-us`#PixRaster`:`:class^ Pixraster] 
provides wrappers to many Leptonica image handling functions; 
as the list is huge it has been divided by cathegories; see Leptonica 
wrappers index.&]
[s0; ]