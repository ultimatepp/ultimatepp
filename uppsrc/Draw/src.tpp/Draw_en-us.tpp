topic "Draw";
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
[ {{10000t/25b/25@3 [s0; [*@(229)4 Draw]]}}&]
[s3; &]
[s1;:Draw`:`:class: [@(0.0.255)3 class][3 _][*3 Draw][3 _:_][@(0.0.255)3 private][3 _][*@3;3 NoCopy]&]
[s9;%% The class provides an abstract interface to basic graphical 
output, possibly HW accelerated. Whenever you need to paint something 
in a window, you need a Draw object for it. The standard Paint 
method of the Ctrl class takes a Draw`& parameter as well. In 
that case the underlying mechanism passes a derived SystemDraw 
which is already initialized for output into the respective control, 
knows about the current clipping, resolution etc. In a similar 
manner you can create your own Draw objects to draw onto Images, 
Drawings (vector metafiles used for serialization of graphical 
operations) or to perform printer output. You can also use a 
Draw to query some graphics`-related properties, like the size 
of output area or native resolution.&]
[s9;%% When Draw represents screen output, all metrics are always 
in pixels. If Draw represents any physical device (like printer), 
it uses artificial unit, `"dot`", by default, which is equivalent 
to 1/600 of inch. In other words, it is the same as pixels size 
of 600dpi printer (standard printer resolution).&]
[s9;%% However, it is also possible to switch to `"Native`" mode 
in some cases. In that case, Draw works in `"native`" pixels, 
for example 1/720 inch for some printers. Draw provides necessary 
metrics information to adjust graphic output.&]
[s9;%% Some methods have several overloaded variants, for example 
DrawRect has variants that specify the rectangle using Rect type 
or x, y, cx, cy coordinates. In that case, implementation is 
provided by single abstract virtual backend method ending with 
Op (DrawRectOp) and non`-virtual frontend methods are then used 
to provide overloading (via parameter translation to backend 
method parameters).&]
[s9;%% Painting outside the output size is always legal `- necessary 
clipping is performed in that case.&]
[s3; &]
[s0; &]
[ {{10000F(128)G(128)@1 [s0;%% [* Public Member List]]}}&]
[s3; &]
[s5;:Draw`:`:GetInfo`(`)const: [@(0.0.255) virtual] [_^dword^ dword]_[* GetInfo]()_[@(0.0.255) c
onst]_`=_[@3 0]&]
[s2;%% Returns some basic information about Draw as combination of 
flags:&]
[s2;%% &]
[ {{3735:6265<256;^ [s2;l64;%% [* Draw`::DOTS_`=_0x001]]
:: [s2;l96;%% Draw possibly represent physical device (printer, PDF 
output etc...).]
:: [s2;l64;%% [* Draw`::GUI_`=_0x002]]
:: [s2;l96;%% Draw encapsulates main host platform GUI graphical output 
(e.g. GDI in Win32).]
:: [s2;l64;%% [* Draw`::PRINTER_`=_0x004]]
:: [s2;l96;%% Draw represents printer.]
:: [s2;l64;%% [* Draw`::NATIVE_`=_0x008]]
:: [s2;l96;%% Draw is in native mode.]
:: [s2;l64; [* Draw`::DATABANDS_`=_0x010]]
:: [s2;l96;%% Draw can use banding to rescale images or for DrawData 
operation to reduce memory consumption.]}}&]
[s3; &]
[s4; &]
[s5;:Draw`:`:GetPageSize`(`)const: [@(0.0.255) virtual] [_^Size^ Size]_[* GetPageSize]()_[@(0.0.255) c
onst]&]
[s2;%% Returns the size of actual output (size of window in pixels, 
size of printable area). If native mode is active and has different 
dpi, value is adjusted accordingly.&]
[s3; &]
[s4; &]
[s5;:Draw`:`:StartPage`(`): [@(0.0.255) virtual] [@(0.0.255) void]_[* StartPage]()&]
[s2;%% For printer output, starts a new page.&]
[s3; &]
[s4; &]
[s5;:Draw`:`:EndPage`(`): [@(0.0.255) virtual] [@(0.0.255) void]_[* EndPage]()&]
[s2;%% For printer output, ends page.&]
[s3; &]
[s4; &]
[s5;:Draw`:`:BeginOp`(`): [@(0.0.255) virtual] [@(0.0.255) void]_[* BeginOp]()_`=_[@3 0]&]
[s2;%% Implements Begin operation: Pushes current offset and clipping 
settings on Draw`'s internal stack.&]
[s3; &]
[s4; &]
[s5;:Draw`:`:EndOp`(`): [@(0.0.255) virtual] [@(0.0.255) void]_[* EndOp]()_`=_[@3 0]&]
[s2;%% Implements End operation: Pops offset and clipping settings, 
restoring the status before pairing BeginOp, OffsetOp, ClipOp, 
ExcludeClipOp or IntersectClipOp.&]
[s3; &]
[s4; &]
[s5;:Draw`:`:OffsetOp`(Point`): [@(0.0.255) virtual] [@(0.0.255) void]_[* OffsetOp]([_^Point^ P
oint]_[*@3 p])_`=_[@3 0]&]
[s2;%% Implements Offset operation: Calls Begin and then offsets coordinates 
so that [%-*@3 p] becomes Point(0, 0).&]
[s3;%% &]
[s4;%% &]
[s5;:Draw`:`:ClipOp`(const Rect`&`): [@(0.0.255) virtual] [@(0.0.255) bool]_[* ClipOp]([@(0.0.255) c
onst]_[_^Rect^ Rect][@(0.0.255) `&]_[*@3 r])_`=_[@3 0]&]
[s2;%% Implements Clip operation: Calls Begin and then restricts all 
painting operations to [%-*@3 r] `- nothing is painted outside 
this rectangle.&]
[s3;%% &]
[s4;%% &]
[s5;:Draw`:`:ClipoffOp`(const Rect`&`): [@(0.0.255) virtual] [@(0.0.255) bool]_[* ClipoffOp
]([@(0.0.255) const]_[_^Rect^ Rect][@(0.0.255) `&]_[*@3 r])_`=_[@3 0]&]
[s2;%% Implements ClipOff operation: Calls begin, then restricts all 
painting operations to[%-  ][%-*@3 r] and offsets coordinates so 
that to left point of [%-*@3 r] becomes Point(0, 0).&]
[s3;%% &]
[s4;%% &]
[s5;:Draw`:`:ExcludeClipOp`(const Rect`&`): [@(0.0.255) virtual] [@(0.0.255) bool]_[* Exclu
deClipOp]([@(0.0.255) const]_[_^Rect^ Rect][@(0.0.255) `&]_[*@3 r])_`=_[@3 0]&]
[s2;%% Implements ExcludeClip operation: Excludes [%-*@3 r] from the 
output `- nothing is painted in this area. Does not affect the 
offset/clipping stack. This operation is only guaranteed to work 
in SystemDraw.&]
[s3;%% &]
[s4;%% &]
[s5;:Draw`:`:IntersectClipOp`(const Rect`&`): [@(0.0.255) virtual] 
[@(0.0.255) bool]_[* IntersectClipOp]([@(0.0.255) const]_[_^Rect^ Rect][@(0.0.255) `&]_[*@3 r
])_`=_[@3 0]&]
[s2;%% Implements IntersectClip operation: Intersects current clipping 
area with [%-*@3 r]. Does not affect the offset/clipping stack. 
This operation is only guaranteed to work in SystemDraw.&]
[s3;%% &]
[s4;%% &]
[s5;:Draw`:`:IsPaintingOp`(const Rect`&`)const: [@(0.0.255) virtual] 
[@(0.0.255) bool]_[* IsPaintingOp]([@(0.0.255) const]_[_^Rect^ Rect][@(0.0.255) `&]_[*@3 r])_
[@(0.0.255) const]_`=_[@3 0]&]
[s2;%% Implements IsPainting operation: Tests whether that should 
any painting happen in [%-*@3 r]. For example, if Draw represents 
graphics output to window in Ctrl`::Paint, not all areas are always 
required to be repainted.&]
[s3;%% &]
[s4;%% &]
[s5;:Draw`:`:GetPaintRect`(`)const: [@(0.0.255) virtual] [_^Rect^ Rect]_[* GetPaintRect]()_
[@(0.0.255) const]&]
[s2;%% Provides bounding rectangle for painted areas (areas where 
IsPaintingOp returns true). Default implementation returns very 
rectangle.&]
[s3;%% &]
[s4;%% &]
[s5;:Draw`:`:DrawRectOp`(int`,int`,int`,int`,Color`): [@(0.0.255) virtual] 
[@(0.0.255) void]_[* DrawRectOp]([@(0.0.255) int]_[*@3 x], [@(0.0.255) int]_[*@3 y], 
[@(0.0.255) int]_[*@3 cx], [@(0.0.255) int]_[*@3 cy], [_^Color^ Color]_[*@3 color])_`=_[@3 0]&]
[s2;%% Implements DrawRect operation: Fills rectangle [%-*@3 x],[%-*@3 y],[%-*@3 cx],[%-*@3 cy
] using [%-*@3 color]. As special addition, if Draw represents 
screen output in Ctrl`::Paint, [%-*@3 color] can be assigned special 
value InvertColor() causing invertion of all pixels in target 
area. Note: On some platform, using InvertColor might results 
in suboptimal performance.&]
[s3;%% &]
[s4; &]
[s5;:Draw`:`:SysDrawImageOp`(int`,int`,const Image`&`,Color`): [@(0.0.255) virtual] 
[@(0.0.255) void]_[* SysDrawImageOp]([@(0.0.255) int]_[*@3 x], [@(0.0.255) int]_[*@3 y], 
[@(0.0.255) const]_[_^Image^ Image][@(0.0.255) `&]_[*@3 img], [_^Color^ Color]_[*@3 color])&]
[s2;%% Low`-level draw of Image. It depends on second variant of 
SysDrawImageOp to support [%-*@3 src] parameter using Offset and 
Clip.&]
[s3;%% &]
[s4; &]
[s5;:Draw`:`:SysDrawImageOp`(int`,int`,const Image`&`,const Rect`&`,Color`): [@(0.0.255) v
irtual] [@(0.0.255) void]_[* SysDrawImageOp]([@(0.0.255) int]_[*@3 x], 
[@(0.0.255) int]_[*@3 y], [@(0.0.255) const]_[_^Image^ Image][@(0.0.255) `&]_[*@3 img], 
[@(0.0.255) const]_[_^Rect^ Rect][@(0.0.255) `&]_[*@3 src], [_^Color^ Color]_[*@3 color])&]
[s2;%% Low(er)`-level draw of Image, called by DrawImageOp. Draw 
derived class can either override this method and depend on DrawImageOp 
to do rescaling and printer banding or override DrawImageOp directly. 
Default implementation uses Offset and Clip to provide support 
for [%-*@3 src] parameter.&]
[s3;%% &]
[s4;%% &]
[s5;:Draw`:`:DrawImageOp`(int`,int`,int`,int`,const Image`&`,const Rect`&`,Color`): [@(0.0.255) v
irtual] [@(0.0.255) void]_[* DrawImageOp]([@(0.0.255) int]_[*@3 x], [@(0.0.255) int]_[*@3 y],
 [@(0.0.255) int]_[*@3 cx], [@(0.0.255) int]_[*@3 cy], [@(0.0.255) const]_[_^Image^ Image][@(0.0.255) `&
]_[*@3 img], [@(0.0.255) const]_[_^Rect^ Rect][@(0.0.255) `&]_[*@3 src], 
[_^Color^ Color]_[*@3 color])_`=_[@3 0]&]
[s2;%% Implements DrawImageOp: Draws [%-*@3 src] part of Image [%-*@3 img] 
at [%-*@3 x], [%-*@3 y] rescaling it to [%-*@3 cx], [%-*@3 cy] (if they 
are equal to original Image size, no rescaling is performed). 
If [%-*@3 color] is not null, only alpha information of Image is 
used and color pixels are painted using [%-*@3 color]. Default 
implementation performs rescaling operation in software, including 
banding support for printer, then calls SysDrawImageOp.&]
[s3;%% &]
[s4;%% &]
[s5;:Draw`:`:DrawDataOp`(int`,int`,int`,int`,const String`&`,const char`*`): [@(0.0.255) v
irtual] [@(0.0.255) void]_[* DrawDataOp]([@(0.0.255) int]_[*@3 x], [@(0.0.255) int]_[*@3 y], 
[@(0.0.255) int]_[*@3 cx], [@(0.0.255) int]_[*@3 cy], [@(0.0.255) const]_[_^String^ String][@(0.0.255) `&
]_[*@3 data], [@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 id])&]
[s2;%% Implements DrawData operation. Draws arbitrary [%-*@3 data] 
of type [%-*@3 id] to rectangular area [%-*@3 x],[%-*@3 y],[%-*@3 cx],[%-*@3 cy]. 
This can be used for example to print compressed bitmaps (e.g. 
in png format) directly, without the need of loading them to 
Image, this possibly saving memory requirements, as banding can 
be used to send the image to printer part by part. Actual painting 
process is implemented in [^DataDrawer^ DataDrawer] and [%-*@3 id] 
has to be registered using DataDrawer`::Register.&]
[s3;%% &]
[s4;%% &]
[s5;:Draw`:`:DrawLineOp`(int`,int`,int`,int`,int`,Color`): [@(0.0.255) virtual] 
[@(0.0.255) void]_[* DrawLineOp]([@(0.0.255) int]_[*@3 x1], [@(0.0.255) int]_[*@3 y1], 
[@(0.0.255) int]_[*@3 x2], [@(0.0.255) int]_[*@3 y2], [@(0.0.255) int]_[*@3 width], 
[_^Color^ Color]_[*@3 color])_`=_[@3 0]&]
[s2;%% Implements DrawLine operation. Draws line from [%-*@3 x1],[%-*@3 y1] 
to [%-*@3 x2],[%-*@3 y2] (included) [%-*@3 width] pixels wide, with 
[%-*@3 color]. Width can contain special values, in that case line 
with width 1 is drawn and special dash pattern is applied:&]
[s2;%% &]
[ {{3035:6965<288; [s2;l64;%% [* PEN`_NULL]]
:: [s2;l64;%% empty pen (nothing is drawn)]
:: [s2;l64;%% [* PEN`_SOLID]]
:: [s2;l64;%% solid pen]
:: [s2;l64;%% [* PEN`_DASH]]
:: [s2;l64;%% dashed pen  `_ `_ `_]
:: [s2;l64;%% [* PEN`_DOT]]
:: [s2;l64;%% dotted pen (dashes are shorter) [* . . .]]
:: [s2;l64;%% [* PEN`_DASHDOT]]
:: [s2;l64;%% dash `- dot `- dash `- dot pattern `_.`_.`_.]
:: [s2;l64;%% [* PEN`_DASHDOTDOT]]
:: [s2;l64;%% dash `- dot `- dot `- dash `- dot `- dot pattern `_..`_..`_..]}}&]
[s3;%% &]
[s4;%% &]
[s5;:Draw`:`:DrawPolyPolylineOp`(const Point`*`,int`,const int`*`,int`,int`,Color`,Color`): [@(0.0.255) v
irtual] [@(0.0.255) void]_[* DrawPolyPolylineOp]([@(0.0.255) const]_[_^Point^ Point]_`*[*@3 v
ertices], [@(0.0.255) int]_[*@3 vertex`_count], [@(0.0.255) const]_[@(0.0.255) int]_`*[*@3 c
ounts], [@(0.0.255) int]_[*@3 count`_count], [@(0.0.255) int]_[*@3 width], 
[_^Color^ Color]_[*@3 color], [_^Color^ Color]_[*@3 doxor])_`=_[@3 0]&]
[s2;%% Implements DrawPolyPolyline operation: Draws a series of polylines. 
Polyline vertices are kept in the array [%-*@3 vertices]. The parameter 
[%-*@3 vertex`_count] gives the total number of vertices of all 
polylines in the array. The array [%-*@3 counts ]gives numbers 
of points defining the individual polylines and [%-*@3 count`_count 
]gives number of entries in this array (i.e. the number of connected 
polylines). The first polyline comprises vertices [%-*@3 vertices]`[0`], 
[%-*@3 vertices]`[1`] ... [%-*@3 vertices]`[[%-*@3 counts]`[0`] `- 
1`], the second polyline [%-*@3 vertices]`[[%-*@3 counts]`[0`]`], 
[%-*@3 vertices]`[[%-*@3 counts]`[0`] `+ 1`] ... [%-*@3 vertices]`[[%-*@3 counts]`[0`] 
`+ [%-*@3 counts]`[1`] `- 1`], etc. The width parameter can be 
equal to the same [* PEN`_]`* special values as width in DrawLineOp. 
Line is drawn with [%-*@3 color], if [%-*@3 doxor] is not null, such 
color in the background gets inverted.&]
[s3;%% &]
[s4;%% &]
[s5;:Draw`:`:DrawPolyPolyPolygonOp`(const Point`*`,int`,const int`*`,int`,const int`*`,int`,Color`,int`,Color`,uint64`,Color`): [@(0.0.255) v
irtual] [@(0.0.255) void]_[* DrawPolyPolyPolygonOp]([@(0.0.255) const]_[_^Point^ Point]_`*
[*@3 vertices], [@(0.0.255) int]_[*@3 vertex`_count], [@(0.0.255) const]_[@(0.0.255) int]_`*
[*@3 subpolygon`_counts], [@(0.0.255) int]_[*@3 scc], [@(0.0.255) const]_[@(0.0.255) int]_`*
[*@3 disjunct`_polygon`_counts], [@(0.0.255) int]_[*@3 dpcc], [_^Color^ Color]_[*@3 color],
 [@(0.0.255) int]_[*@3 width], [_^Color^ Color]_[*@3 outline], [_^uint64^ uint64]_[*@3 patter
n], [_^Color^ Color]_[*@3 doxor])_`=_[@3 0]&]
[s2;%% Implements DrawPolyPolyPolygon operation: Draws a series of 
complex polygons. The [%-*@3 vertices] array holds all polygon 
defining vertices. The array is divided into sections corresponding 
to the whole complex polygons (parameters [%-*@3 disjunct`_polygon`_counts]) 
and these sections are further divided into the individual polygons 
defining one complex polygon (i.e. outer boundary and holes). 
Numbers of vertices in the individual polygons are held in the 
array [%-*@3 subpolygon`_counts ](total number of simple polygons 
`= subpolygon`_count`_count).&]
[s3;%% &]
[s4;%% &]
[s5;:Draw`:`:DrawArcOp`(const Rect`&`,Point`,Point`,int`,Color`): [@(0.0.255) virtual] 
[@(0.0.255) void]_[* DrawArcOp]([@(0.0.255) const]_[_^Rect^ Rect][@(0.0.255) `&]_[*@3 rc], 
[_^Point^ Point]_[*@3 start], [_^Point^ Point]_[*@3 end], [@(0.0.255) int]_[*@3 width], 
[_^Color^ Color]_[*@3 color])_`=_[@3 0]&]
[s2;%% Implements DrawArc operation: Draws elliptic arc corresponding 
to the largest ellipse fully within the rectangle [%-*@3 rc] and 
running counterclockwise from the direction corresponding to 
the line connecting the centre of the ellipse ([%-*@3 rc].CenterPoint()) 
with the point [%-*@3 start ]and ending at direction of the point 
[%-*@3 end ]from the ellipse centre. When start `=`= end, the full 
ellipse is drawn. [%-*@3 width] is the width of line, painted in 
[%-*@3 color].&]
[s3;%% &]
[s4;%% &]
[s5;:Draw`:`:DrawEllipseOp`(const Rect`&`,Color`,int`,Color`): [@(0.0.255) virtual] 
[@(0.0.255) void]_[* DrawEllipseOp]([@(0.0.255) const]_[_^Rect^ Rect][@(0.0.255) `&]_[*@3 r],
 [_^Color^ Color]_[*@3 color], [@(0.0.255) int]_[*@3 pen], [_^Color^ Color]_[*@3 pencolor])_`=
_[@3 0]&]
[s2;%% Implements DrawEllipse operation: Draws the largest ellipse 
with both axes parallel to coordinate axes fully within rectangle 
[%-*@3 r], i.e. with center point at r.CenterPoint(), semi major 
axis and semi minor axis equal to r.Width() / 2 and r.Height() 
/ 2. If [%-*@3 pen] is not 0, Ellipse will be have border line 
of width [%-*@3 pen] and color [%-*@3 pencolor].&]
[s3;%% &]
[s4;%% &]
[s5;:Draw`:`:DrawTextOp`(int`,int`,int`,const wchar`*`,Font`,Color`,int`,const int`*`): [@(0.0.255) v
irtual] [@(0.0.255) void]_[* DrawTextOp]([@(0.0.255) int]_[*@3 x], [@(0.0.255) int]_[*@3 y], 
[@(0.0.255) int]_[*@3 angle], [@(0.0.255) const]_[_^wchar^ wchar]_`*[*@3 text], 
[_^Font^ Font]_[*@3 font], [_^Color^ Color]_[*@3 ink], [@(0.0.255) int]_[*@3 n], 
[@(0.0.255) const]_[@(0.0.255) int]_`*[*@3 dx])_`=_[@3 0]&]
[s2;%% Implements DrawText operation:Draws a [%-*@3 text ]at position 
starting at [%-*@3 x], [%-*@3 y] (it represents top`-left corner 
of text cell: for horizontal text, x identifies reference point 
and y is ascent above the baseline) going at [%-*@3 angle] using 
[%-*@3 font] and color [%-*@3 ink]. [%-*@3 n] is a number of characters 
to paint. If [%-*@3 dx ]is not NULL, it specifies advancements 
of individual characters (it must be [%-*@3 n] elements in this 
array), otherwise advancements from font metrics are used.&]
[s3;%% &]
[s4;%% &]
[s5;:Draw`:`:DrawDrawingOp`(const Rect`&`,const Drawing`&`): [@(0.0.255) virtual] 
[@(0.0.255) void]_[* DrawDrawingOp]([@(0.0.255) const]_[_^Rect^ Rect][@(0.0.255) `&]_[*@3 tar
get], [@(0.0.255) const]_[_^Drawing^ Drawing][@(0.0.255) `&]_[*@3 w])&]
[s2;%% Implements DrawDrawing operation: Draws [^Drawing^ Drawing] [%-*@3 w] 
scaled into [%-*@3 target]. Base Draw provides default implementation 
of this function (in fact, it could most likely be non`-virtual).&]
[s3;%% &]
[s4;%% &]
[s5;:Draw`:`:DrawPaintingOp`(const Rect`&`,const Painting`&`): [@(0.0.255) virtual] 
[@(0.0.255) void]_[* DrawPaintingOp]([@(0.0.255) const]_[_^Rect^ Rect][@(0.0.255) `&]_[*@3 ta
rget], [@(0.0.255) const]_[_^Painting^ Painting][@(0.0.255) `&]_[*@3 w])&]
[s2;%% Implements DrawPainting operation: Draws [^Drawing^ Drawing] 
[%-*@3 w] scaled into [%-*@3 target]. Base Draw provides default 
implementation, which paints Painting using DrawImage, possibly 
using banding technique to reduce the memory requirements. Painter 
package has to be included in the project for this to work.&]
[s3;%% &]
[s4;%% &]
[s5;:Draw`:`:GetNativeDpi`(`)const: [@(0.0.255) virtual] [_^Size^ Size]_[* GetNativeDpi]()_
[@(0.0.255) const]&]
[s2;%% Returns native resolution of target device. Default implementation 
returns Size(600, 600) for Dots device and Size(96, 96) for screen 
output.&]
[s3;%% &]
[s4;%% &]
[s5;:Draw`:`:BeginNative`(`): [@(0.0.255) virtual] [@(0.0.255) void]_[* BeginNative]()&]
[s2;%% Starts native resolution mode, if available. Can be called 
several times (only first call is effective). If device does 
not have native mode, it has no effect.&]
[s3;%% &]
[s4;%% &]
[s5;:Draw`:`:EndNative`(`): [@(0.0.255) virtual] [@(0.0.255) void]_[* EndNative]()&]
[s2;%% Ends native resolution mode. If BeginNative was called several 
times, has to be called the same number of times to end the native 
mode.&]
[s3;%% &]
[s4;%% &]
[s5;:Draw`:`:GetCloffLevel`(`)const: [@(0.0.255) virtual] [@(0.0.255) int]_[* GetCloffLevel
]()_[@(0.0.255) const]&]
[s2;%% Returns the number of elements in clip`&offset stack. Mostly 
used for diagnostic purposes.&]
[s3;%% &]
[s4; &]
[s5;:Upp`:`:Draw`:`:Escape`(const Upp`:`:String`&`): [@(0.0.255) virtual 
void]_[* Escape]([@(0.0.255) const]_[_^Upp`:`:String^ String][@(0.0.255) `&]_[*@3 data])&]
[s2;%% Passes additional information to Draw target instance. For 
example, PdfDraw understands `"url:http://link.html`" escapes to 
define text hyperlinks. Frontend to EscapeOp.&]
[s3;%% &]
[s4;%% &]
[s5;:Draw`:`:GetPixelsPerInch`(`)const: [_^Size^ Size]_[* GetPixelsPerInch]()_[@(0.0.255) c
onst]&]
[s2;%% Returns active resolution. Returns native resolution if native 
mode is active, or Size(600, 600) for physical output or Size(96, 
96) for screen output.&]
[s3;%% &]
[s4;%% &]
[s5;:Draw`:`:GetPageMMs`(`)const: [_^Size^ Size]_[* GetPageMMs]()_[@(0.0.255) const]&]
[s2;%% Recomputes GetPageSize to millimeters.&]
[s3;%% &]
[s4;%% &]
[s5;:Draw`:`:Dots`(`)const: [@(0.0.255) bool]_[* Dots]()_[@(0.0.255) const]&]
[s2;%% Same as GetInfo() `& DOTS. True for physical device.&]
[s3;%% &]
[s4;%% &]
[s5;:Draw`:`:Pixels`(`)const: [@(0.0.255) bool]_[* Pixels]()_[@(0.0.255) const]&]
[s2;%% Same as !Dots(). True for screen output.&]
[s3;%% &]
[s4;%% &]
[s5;:Draw`:`:IsPrinter`(`)const: [@(0.0.255) bool]_[* IsPrinter]()_[@(0.0.255) const]&]
[s2;%% Same as GetInfo() `& PRINTER. True if output is printer.&]
[s3;%% &]
[s4;%% &]
[s5;:Draw`:`:IsNative`(`)const: [@(0.0.255) bool]_[* IsNative]()_[@(0.0.255) const]&]
[s2;%% Same as GetInfo() `& NATIVE. True if native resolution mode 
is active.&]
[s3;%% &]
[s4;%% &]
[s5;:Draw`:`:GetNativeX`(int`)const: [@(0.0.255) int]_[* GetNativeX]([@(0.0.255) int]_[*@3 x])
_[@(0.0.255) const]&]
[s2;%% Recomputes [%-*@3 x] from 600 dpi dots to native horizontal 
resolution.&]
[s3;%% &]
[s4;%% &]
[s5;:Draw`:`:GetNativeY`(int`)const: [@(0.0.255) int]_[* GetNativeY]([@(0.0.255) int]_[*@3 y])
_[@(0.0.255) const]&]
[s2;%% Recomputes [%-*@3 y] from 600 dpi dots to native vertical resolution.&]
[s3;%% &]
[s4;%% &]
[s5;:Draw`:`:Native`(int`&`,int`&`)const: [@(0.0.255) void]_[* Native]([@(0.0.255) int`&]_[*@3 x
], [@(0.0.255) int`&]_[*@3 y])_[@(0.0.255) const]&]
[s2;%% Recomputes point coordinates from 600 dpi dots to native resolution.&]
[s3;%% &]
[s4;%% &]
[s5;:Draw`:`:Native`(Point`&`)const: [@(0.0.255) void]_[* Native]([_^Point^ Point][@(0.0.255) `&
]_[*@3 p])_[@(0.0.255) const]&]
[s2;%% Recomputes point from 600 dpi dots to native resolution.&]
[s3;%% &]
[s4;%% &]
[s5;:Draw`:`:Native`(Size`&`)const: [@(0.0.255) void]_[* Native]([_^Size^ Size][@(0.0.255) `&
]_[*@3 sz])_[@(0.0.255) const]&]
[s2;%% Recomputes Size from 600 dpi dots to native resolution.&]
[s3;%% &]
[s4;%% &]
[s5;:Draw`:`:Native`(Rect`&`)const: [@(0.0.255) void]_[* Native]([_^Rect^ Rect][@(0.0.255) `&
]_[*@3 r])_[@(0.0.255) const]&]
[s2;%% Recomputes Rect from 600 dpi dots to native resolution..&]
[s3;%% &]
[s4;%% &]
[s5;:Draw`:`:Begin`(`): [@(0.0.255) void]_[* Begin]()&]
[s2;%% Pushes current offset and clipping settings on Draw`'s internal 
stack. Frontend to BeginOp.&]
[s3;%% &]
[s4;%% &]
[s5;:Draw`:`:End`(`): [@(0.0.255) void]_[* End]()&]
[s2;%% Pops offset and clipping settings, restoring the status before 
pairing BeginOp, OffsetOp, ClipOp, ExcludeClipOp or IntersectClipOp. 
Frontend to EndOp.&]
[s3;%% &]
[s4;%% &]
[s5;:Draw`:`:Offset`(Point`): [@(0.0.255) void]_[* Offset]([_^Point^ Point]_[*@3 p])&]
[s5;:Draw`:`:Offset`(int`,int`): [@(0.0.255) void]_[* Offset]([@(0.0.255) int]_[*@3 x], 
[@(0.0.255) int]_[*@3 y])&]
[s2;%% Calls Begin and then offsets coordinates so that [%-*@3 p] or 
[%-*@3 x][%- ,][%-*@3 y][%-  ]becomes Point(0, 0). Uses OffsetOp for 
implementation. Frontend to OffsetOp.&]
[s3;%% &]
[s4;%% &]
[s5;:Draw`:`:Clip`(const Rect`&`): [@(0.0.255) bool]_[* Clip]([@(0.0.255) const]_[_^Rect^ Rec
t][@(0.0.255) `&]_[*@3 r])&]
[s5;:Draw`:`:Clip`(int`,int`,int`,int`): [@(0.0.255) bool]_[* Clip]([@(0.0.255) int]_[*@3 x],
 [@(0.0.255) int]_[*@3 y], [@(0.0.255) int]_[*@3 cx], [@(0.0.255) int]_[*@3 cy])&]
[s2;%% Calls Begin and then restricts all painting operations to 
[%-*@3 r] or [%-*@3 x][%- ,][%-*@3 y][%- ,][%-*@3 cx][%- ,][%-*@3 cy] rectangle 
`- nothing is painted outside this rectangle. Frontend to ClipOp.&]
[s3;%% &]
[s4;%% &]
[s5;:Draw`:`:Clipoff`(const Rect`&`): [@(0.0.255) bool]_[* Clipoff]([@(0.0.255) const]_[_^Rect^ R
ect][@(0.0.255) `&]_[*@3 r])&]
[s5;:Draw`:`:Clipoff`(int`,int`,int`,int`): [@(0.0.255) bool]_[* Clipoff]([@(0.0.255) int]_
[*@3 x], [@(0.0.255) int]_[*@3 y], [@(0.0.255) int]_[*@3 cx], [@(0.0.255) int]_[*@3 cy])&]
[s2;%% Implements ClipOff operation: Calls begin, then restricts all 
painting operations to[%-  ][%-*@3 r] or [%-*@3 x][%- ,][%-*@3 y][%- ,][%-*@3 cx][%- ,][%-*@3 cy] 
rectangle and offsets coordinates so that to left point of [%-*@3 r] 
or point [%-*@3 x][%- ,][%-*@3 y] becomes Point(0, 0). Frontend to 
ClipoffOp.&]
[s3;%% &]
[s4;%% &]
[s5;:Draw`:`:ExcludeClip`(const Rect`&`): [@(0.0.255) bool]_[* ExcludeClip]([@(0.0.255) con
st]_[_^Rect^ Rect][@(0.0.255) `&]_[*@3 r])&]
[s5;:Draw`:`:ExcludeClip`(int`,int`,int`,int`): [@(0.0.255) bool]_[* ExcludeClip]([@(0.0.255) i
nt]_[*@3 x], [@(0.0.255) int]_[*@3 y], [@(0.0.255) int]_[*@3 cx], [@(0.0.255) int]_[*@3 cy])&]
[s2;%% Excludes [%-*@3 r] or rectangle [%-*@3 x],[%-*@3 y],[%-*@3 cx],[%-*@3 cy] 
from the output `- nothing is painted in this area. Does not 
affect the offset/clipping stack. This operation is only guaranteed 
to work in SystemDraw. Frontend to ExcludeClipOp.&]
[s3;%% &]
[s4;%% &]
[s5;:Draw`:`:IntersectClip`(const Rect`&`): [@(0.0.255) bool]_[* IntersectClip]([@(0.0.255) c
onst]_[_^Rect^ Rect][@(0.0.255) `&]_[*@3 r])&]
[s5;:Draw`:`:IntersectClip`(int`,int`,int`,int`): [@(0.0.255) bool]_[* IntersectClip]([@(0.0.255) i
nt]_[*@3 x], [@(0.0.255) int]_[*@3 y], [@(0.0.255) int]_[*@3 cx], [@(0.0.255) int]_[*@3 cy])&]
[s2;%% Intersects current clipping area with [%-*@3 r] or rectangle 
[%-*@3 x],[%-*@3 y],[%-*@3 cx],[%-*@3 cy]. Does not affect the offset/clipping 
stack. This operation is only guaranteed to work in SystemDraw. 
Frontend to IntersectClipOp.&]
[s3;%% &]
[s4;%% &]
[s5;:Draw`:`:IsPainting`(const Rect`&`)const: [@(0.0.255) bool]_[* IsPainting]([@(0.0.255) c
onst]_[_^Rect^ Rect][@(0.0.255) `&]_[*@3 r])_[@(0.0.255) const]&]
[s5;:Draw`:`:IsPainting`(int`,int`,int`,int`)const:%% [%-@(0.0.255) bool][%- _][%-* IsPaint
ing][%- (][%-@(0.0.255) int][%- _][%-*@3 x][%- , ][%-@(0.0.255) int][%- _][%-*@3 y][%- , 
][%-@(0.0.255) int][%- _][%-*@3 cx][%- , ][%-@(0.0.255) int][%- _][%-*@3 cy][%- )_][%-@(0.0.255) co
nst] .&]
[s2;%% Tests whether that should any painting happen in [%-*@3 r] or 
rectangle [%-*@3 x],[%-*@3 y],[%-*@3 cx],[%-*@3 cy]. For example, if 
Draw represents graphics output to window in Ctrl`::Paint, not 
all areas are always required to be repainted. Frontend to IsPaintingOp.&]
[s3;%% &]
[s4;%% &]
[s5;:Draw`:`:DrawRect`(int`,int`,int`,int`,Color`): [@(0.0.255) void]_[* DrawRect]([@(0.0.255) i
nt]_[*@3 x], [@(0.0.255) int]_[*@3 y], [@(0.0.255) int]_[*@3 cx], [@(0.0.255) int]_[*@3 cy], 
[_^Color^ Color]_[*@3 color])&]
[s5;:Draw`:`:DrawRect`(const Rect`&`,Color`): [@(0.0.255) void]_[* DrawRect]([@(0.0.255) co
nst]_[_^Rect^ Rect][@(0.0.255) `&]_[*@3 rect], [_^Color^ Color]_[*@3 color])&]
[s2;%% Fills [%-*@3 rect] or rectangle [%-*@3 x],[%-*@3 y],[%-*@3 cx],[%-*@3 cy] 
using [%-*@3 color]. As special addition, if Draw represents screen 
output in Ctrl`::Paint, [%-*@3 color] can be assigned special value 
InvertColor() causing invertion of all pixels in target area. 
Frontend to DrawRectOp. Note: On some platform, using InvertColor 
might results in suboptimal performance.&]
[s3;%% &]
[s4;%% &]
[s5;:Draw`:`:DrawImage`(int`,int`,int`,int`,const Image`&`,const Rect`&`): [@(0.0.255) v
oid]_[* DrawImage]([@(0.0.255) int]_[*@3 x], [@(0.0.255) int]_[*@3 y], 
[@(0.0.255) int]_[*@3 cx], [@(0.0.255) int]_[*@3 cy], [@(0.0.255) const]_[_^Image^ Image][@(0.0.255) `&
]_[*@3 img], [@(0.0.255) const]_[_^Rect^ Rect][@(0.0.255) `&]_[*@3 src])&]
[s5;:Draw`:`:DrawImage`(int`,int`,int`,int`,const Image`&`): [@(0.0.255) void]_[* DrawIma
ge]([@(0.0.255) int]_[*@3 x], [@(0.0.255) int]_[*@3 y], [@(0.0.255) int]_[*@3 cx], 
[@(0.0.255) int]_[*@3 cy], [@(0.0.255) const]_[_^Image^ Image][@(0.0.255) `&]_[*@3 img])&]
[s5;:Draw`:`:DrawImage`(int`,int`,int`,int`,const Image`&`,const Rect`&`,Color`): [@(0.0.255) v
oid]_[* DrawImage]([@(0.0.255) int]_[*@3 x], [@(0.0.255) int]_[*@3 y], 
[@(0.0.255) int]_[*@3 cx], [@(0.0.255) int]_[*@3 cy], [@(0.0.255) const]_[_^Image^ Image][@(0.0.255) `&
]_[*@3 img], [@(0.0.255) const]_[_^Rect^ Rect][@(0.0.255) `&]_[*@3 src], 
[_^Color^ Color]_[*@3 color])&]
[s5;:Draw`:`:DrawImage`(int`,int`,int`,int`,const Image`&`,Color`): [@(0.0.255) void]_[* D
rawImage]([@(0.0.255) int]_[*@3 x], [@(0.0.255) int]_[*@3 y], [@(0.0.255) int]_[*@3 cx], 
[@(0.0.255) int]_[*@3 cy], [@(0.0.255) const]_[_^Image^ Image][@(0.0.255) `&]_[*@3 img], 
[_^Color^ Color]_[*@3 color])&]
[s5;:Draw`:`:DrawImage`(const Rect`&`,const Image`&`,const Rect`&`): [@(0.0.255) void]_
[* DrawImage]([@(0.0.255) const]_[_^Rect^ Rect][@(0.0.255) `&]_[*@3 r], 
[@(0.0.255) const]_[_^Image^ Image][@(0.0.255) `&]_[*@3 img], [@(0.0.255) const]_[_^Rect^ Rec
t][@(0.0.255) `&]_[*@3 src])&]
[s5;:Draw`:`:DrawImage`(const Rect`&`,const Image`&`): [@(0.0.255) void]_[* DrawImage]([@(0.0.255) c
onst]_[_^Rect^ Rect][@(0.0.255) `&]_[*@3 r], [@(0.0.255) const]_[_^Image^ Image][@(0.0.255) `&
]_[*@3 img])&]
[s5;:Draw`:`:DrawImage`(const Rect`&`,const Image`&`,const Rect`&`,Color`): [@(0.0.255) v
oid]_[* DrawImage]([@(0.0.255) const]_[_^Rect^ Rect][@(0.0.255) `&]_[*@3 r], 
[@(0.0.255) const]_[_^Image^ Image][@(0.0.255) `&]_[*@3 img], [@(0.0.255) const]_[_^Rect^ Rec
t][@(0.0.255) `&]_[*@3 src], [_^Color^ Color]_[*@3 color])&]
[s5;:Draw`:`:DrawImage`(const Rect`&`,const Image`&`,Color`): [@(0.0.255) void]_[* DrawIm
age]([@(0.0.255) const]_[_^Rect^ Rect][@(0.0.255) `&]_[*@3 r], [@(0.0.255) const]_[_^Image^ I
mage][@(0.0.255) `&]_[*@3 img], [_^Color^ Color]_[*@3 color])&]
[s5;:Draw`:`:DrawImage`(int`,int`,const Image`&`,const Rect`&`): [@(0.0.255) void]_[* Dra
wImage]([@(0.0.255) int]_[*@3 x], [@(0.0.255) int]_[*@3 y], [@(0.0.255) const]_[_^Image^ Imag
e][@(0.0.255) `&]_[*@3 img], [@(0.0.255) const]_[_^Rect^ Rect][@(0.0.255) `&]_[*@3 src])&]
[s5;:Draw`:`:DrawImage`(int`,int`,const Image`&`): [@(0.0.255) void]_[* DrawImage]([@(0.0.255) i
nt]_[*@3 x], [@(0.0.255) int]_[*@3 y], [@(0.0.255) const]_[_^Image^ Image][@(0.0.255) `&]_[*@3 i
mg])&]
[s5;:Draw`:`:DrawImage`(int`,int`,const Image`&`,const Rect`&`,Color`): [@(0.0.255) voi
d]_[* DrawImage]([@(0.0.255) int]_[*@3 x], [@(0.0.255) int]_[*@3 y], [@(0.0.255) const]_[_^Image^ I
mage][@(0.0.255) `&]_[*@3 img], [@(0.0.255) const]_[_^Rect^ Rect][@(0.0.255) `&]_[*@3 src], 
[_^Color^ Color]_[*@3 color])&]
[s5;:Draw`:`:DrawImage`(int`,int`,const Image`&`,Color`): [@(0.0.255) void]_[* DrawImage](
[@(0.0.255) int]_[*@3 x], [@(0.0.255) int]_[*@3 y], [@(0.0.255) const]_[_^Image^ Image][@(0.0.255) `&
]_[*@3 img], [_^Color^ Color]_[*@3 color])&]
[s2;%% Paints [^Image^ Image] [%-*@3 img]. If target rectangle is present 
([%-*@3 r] or [%-*@3 x,y,cx,cy]), Image is rescaled (using high quality 
filter) to fit the area (banding can be used in the process to 
reduce memory requirements). If only target point [%-*@3 x],[%-*@3 y] 
is provided, Image is not rescaled. If [%-*@3 src] rectangle is 
provided, only that portion of Image is used. If [%-*@3 color] 
is provided, only alpha information is used and the color information 
is replaced with it. Frontend to DrawImageOp.&]
[s3;%% &]
[s4;%% &]
[s5;:Draw`:`:DrawData`(int`,int`,int`,int`,const String`&`,const char`*`): [@(0.0.255) v
oid]_[* DrawData]([@(0.0.255) int]_[*@3 x], [@(0.0.255) int]_[*@3 y], [@(0.0.255) int]_[*@3 cx],
 [@(0.0.255) int]_[*@3 cy], [@(0.0.255) const]_[_^String^ String][@(0.0.255) `&]_[*@3 data], 
[@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 type])&]
[s5;:Draw`:`:DrawData`(const Rect`&`,const String`&`,const char`*`): [@(0.0.255) void]_
[* DrawData]([@(0.0.255) const]_[_^Rect^ Rect][@(0.0.255) `&]_[*@3 r], 
[@(0.0.255) const]_[_^String^ String][@(0.0.255) `&]_[*@3 data], [@(0.0.255) const]_[@(0.0.255) c
har]_`*[*@3 type])&]
[s2;%% Draws arbitrary [%-*@3 data] of type [%-*@3 id] to rectangular 
area [%-*@3 r] or [%-*@3 x],[%-*@3 y],[%-*@3 cx],[%-*@3 cy]. This can be 
used for example to print compressed bitmaps (e.g. in png format) 
directly, without the need of loading them to Image, this possibly 
saving memory requirements, as banding can be used to send the 
image to printer part by part. Actual painting process is implemented 
in [^DataDrawer^ DataDrawer] and [%-*@3 id] has to be registered 
using DataDrawer`::Register. Frontend to DrawDataOp.&]
[s3;%% &]
[s4;%% &]
[s5;:Draw`:`:DrawLine`(int`,int`,int`,int`,int`,Color`): [@(0.0.255) void]_[* DrawLine]([@(0.0.255) i
nt]_[*@3 x1], [@(0.0.255) int]_[*@3 y1], [@(0.0.255) int]_[*@3 x2], [@(0.0.255) int]_[*@3 y2], 
[@(0.0.255) int]_[*@3 width]_`=_[@3 0], [_^Color^ Color]_[*@3 color]_`=_DefaultInk)&]
[s5;:Draw`:`:DrawLine`(Point`,Point`,int`,Color`): [@(0.0.255) void]_[* DrawLine]([_^Point^ P
oint]_[*@3 p1], [_^Point^ Point]_[*@3 p2], [@(0.0.255) int]_[*@3 width]_`=_[@3 0], 
[_^Color^ Color]_[*@3 color]_`=_DefaultInk)&]
[s2;%% Draws line from [%-*@3 x1],[%-*@3 y1] or [%-*@3 p1] to [%-*@3 x2],[%-*@3 y2] 
or [%-*@3 p2] (included) [%-*@3 width] pixels wide, with [%-*@3 color]. 
Width can contain special values, in that case line with width 
1 is drawn and special dash pattern is applied `- see DrawLineOp 
for details. Frontend to DrawLineOp.&]
[s3;%% &]
[s4;%% &]
[s5;:Draw`:`:DrawEllipse`(const Rect`&`,Color`,int`,Color`): [@(0.0.255) void]_[* DrawEll
ipse]([@(0.0.255) const]_[_^Rect^ Rect][@(0.0.255) `&]_[*@3 r], [_^Color^ Color]_[*@3 color]_
`=_DefaultInk, [@(0.0.255) int]_[*@3 pen]_`=_Null, [_^Color^ Color]_[*@3 pencolor]_`=_Def
aultInk)&]
[s5;:Draw`:`:DrawEllipse`(int`,int`,int`,int`,Color`,int`,Color`): [@(0.0.255) void]_[* D
rawEllipse]([@(0.0.255) int]_[*@3 x], [@(0.0.255) int]_[*@3 y], [@(0.0.255) int]_[*@3 cx], 
[@(0.0.255) int]_[*@3 cy], [_^Color^ Color]_[*@3 color]_`=_DefaultInk, 
[@(0.0.255) int]_[*@3 pen]_`=_Null, [_^Color^ Color]_[*@3 pencolor]_`=_DefaultInk)&]
[s2;%% Draws the largest ellipse with both axes parallel to coordinate 
axes fully within rectangle [%-*@3 r] ([%-*@3 x],[%-*@3 y],[%-*@3 cx],[%-*@3 cy]), 
i.e. with center point at r.CenterPoint(), semi major axis and 
semi minor axis equal to r.Width() / 2 and r.Height() / 2. If 
[%-*@3 pen] is not 0, Ellipse will be have border line of width 
[%-*@3 pen] and color [%-*@3 pencolor]. Frontend to DrawEllipseOp.&]
[s3;%% &]
[s4;%% &]
[s5;:Draw`:`:DrawArc`(const Rect`&`,Point`,Point`,int`,Color`): [@(0.0.255) void]_[* Draw
Arc]([@(0.0.255) const]_[_^Rect^ Rect][@(0.0.255) `&]_[*@3 rc], [_^Point^ Point]_[*@3 start],
 [_^Point^ Point]_[*@3 end], [@(0.0.255) int]_[*@3 width]_`=_[@3 0], [_^Color^ Color]_[*@3 colo
r]_`=_DefaultInk)&]
[s2;%% Draws elliptic arc corresponding to the largest ellipse fully 
within the rectangle [%-*@3 rc] and running counterclockwise from 
the direction corresponding to the line connecting the centre 
of the ellipse ([%-*@3 rc].CenterPoint()) with the point [%-*@3 start 
]and ending at direction of the point [%-*@3 end ]from the ellipse 
centre. When start `=`= end, the full ellipse is drawn. [%-*@3 width] 
is the width of line, painted in [%-*@3 color]. Frontend to DrawArcOp.&]
[s3;%% &]
[s4;%% &]
[s5;:Draw`:`:DrawPolyPolyline`(const Point`*`,int`,const int`*`,int`,int`,Color`,Color`): [@(0.0.255) v
oid]_[* DrawPolyPolyline]([@(0.0.255) const]_[_^Point^ Point]_`*[*@3 vertices], 
[@(0.0.255) int]_[*@3 vertex`_count], [@(0.0.255) const]_[@(0.0.255) int]_`*[*@3 counts], 
[@(0.0.255) int]_[*@3 count`_count], [@(0.0.255) int]_[*@3 width]_`=_[@3 0], 
[_^Color^ Color]_[*@3 color]_`=_DefaultInk, [_^Color^ Color]_[*@3 doxor]_`=_Null)&]
[s5;:Draw`:`:DrawPolyPolyline`(const Vector`<Point`>`&`,const Vector`<int`>`&`,int`,Color`,Color`): [@(0.0.255) v
oid]_[* DrawPolyPolyline]([@(0.0.255) const]_[_^Vector^ Vector]<[_^Point^ Point]>`&_[*@3 ve
rtices], [@(0.0.255) const]_[_^Vector^ Vector]<[@(0.0.255) int]>`&_[*@3 counts], 
[@(0.0.255) int]_[*@3 width]_`=_[@3 0], [_^Color^ Color]_[*@3 color]_`=_DefaultInk, 
[_^Color^ Color]_[*@3 doxor]_`=_Null)&]
[s2;%% Draws multiple polylines (polygon without filling).&]
[s3;%% &]
[s4;%% &]
[s5;:Draw`:`:DrawPolyline`(const Point`*`,int`,int`,Color`,Color`): [@(0.0.255) void]_[* D
rawPolyline]([@(0.0.255) const]_[_^Point^ Point]_`*[*@3 vertices], 
[@(0.0.255) int]_[*@3 count], [@(0.0.255) int]_[*@3 width]_`=_[@3 0], [_^Color^ Color]_[*@3 col
or]_`=_DefaultInk, [_^Color^ Color]_[*@3 doxor]_`=_Null)&]
[s5;:Draw`:`:DrawPolyline`(const Vector`<Point`>`&`,int`,Color`,Color`): [@(0.0.255) vo
id]_[* DrawPolyline]([@(0.0.255) const]_[_^Vector^ Vector]<[_^Point^ Point]>`&_[*@3 vertice
s], [@(0.0.255) int]_[*@3 width]_`=_[@3 0], [_^Color^ Color]_[*@3 color]_`=_DefaultInk, 
[_^Color^ Color]_[*@3 doxor]_`=_Null)&]
[s2;%% Draws a polyline (polygon without filling).&]
[s3;%% &]
[s4;%% &]
[s5;:Draw`:`:DrawPolyPolyPolygon`(const Point`*`,int`,const int`*`,int`,const int`*`,int`,Color`,int`,Color`,uint64`,Color`): [@(0.0.255) v
oid]_[* DrawPolyPolyPolygon]([@(0.0.255) const]_[_^Point^ Point]_`*[*@3 vertices], 
[@(0.0.255) int]_[*@3 vertex`_count], [@(0.0.255) const]_[@(0.0.255) int]_`*[*@3 subpolygon
`_counts], [@(0.0.255) int]_[*@3 subpolygon`_count`_count], [@(0.0.255) const]_[@(0.0.255) i
nt]_`*[*@3 disjunct`_polygon`_counts], [@(0.0.255) int]_[*@3 disjunct`_polygon`_count`_
count], [_^Color^ Color]_[*@3 color]_`=_DefaultInk, [@(0.0.255) int]_[*@3 width]_`=_[@3 0],
 [_^Color^ Color]_[*@3 outline]_`=_Null, [_^uint64^ uint64]_[*@3 pattern]_`=_[@3 0], 
[_^Color^ Color]_[*@3 doxor]_`=_Null)&]
[s5;:Draw`:`:DrawPolyPolyPolygon`(const Vector`<Point`>`&`,const Vector`<int`>`&`,const Vector`<int`>`&`,Color`,int`,Color`,uint64`,Color`): [@(0.0.255) v
oid]_[* DrawPolyPolyPolygon]([@(0.0.255) const]_[_^Vector^ Vector]<[_^Point^ Point]>`&_[*@3 v
ertices], [@(0.0.255) const]_[_^Vector^ Vector]<[@(0.0.255) int]>`&_[*@3 subpolygon`_coun
ts], [@(0.0.255) const]_[_^Vector^ Vector]<[@(0.0.255) int]>`&_[*@3 disjunct`_polygon`_co
unts], [_^Color^ Color]_[*@3 color]_`=_DefaultInk, [@(0.0.255) int]_[*@3 width]_`=_[@3 0], 
[_^Color^ Color]_[*@3 outline]_`=_Null, [_^uint64^ uint64]_[*@3 pattern]_`=_[@3 0], 
[_^Color^ Color]_[*@3 doxor]_`=_Null)&]
[s2; Draws poly`-poly`-polygon.&]
[s3; &]
[s4; &]
[s5;:Draw`:`:DrawPolyPolygon`(const Point`*`,int`,const int`*`,int`,Color`,int`,Color`,uint64`,Color`): [@(0.0.255) v
oid]_[* DrawPolyPolygon]([@(0.0.255) const]_[_^Point^ Point]_`*[*@3 vertices], 
[@(0.0.255) int]_[*@3 vertex`_count], [@(0.0.255) const]_[@(0.0.255) int]_`*[*@3 subpolygon
`_counts], [@(0.0.255) int]_[*@3 subpolygon`_count`_count], [_^Color^ Color]_[*@3 color]_
`=_DefaultInk, [@(0.0.255) int]_[*@3 width]_`=_[@3 0], [_^Color^ Color]_[*@3 outline]_`=_Nu
ll, [_^uint64^ uint64]_[*@3 pattern]_`=_[@3 0], [_^Color^ Color]_[*@3 doxor]_`=_Null)&]
[s5;:Draw`:`:DrawPolyPolygon`(const Vector`<Point`>`&`,const Vector`<int`>`&`,Color`,int`,Color`,uint64`,Color`): [@(0.0.255) v
oid]_[* DrawPolyPolygon]([@(0.0.255) const]_[_^Vector^ Vector]<[_^Point^ Point]>`&_[*@3 ver
tices], [@(0.0.255) const]_[_^Vector^ Vector]<[@(0.0.255) int]>`&_[*@3 subpolygon`_counts
], [_^Color^ Color]_[*@3 color]_`=_DefaultInk, [@(0.0.255) int]_[*@3 width]_`=_[@3 0], 
[_^Color^ Color]_[*@3 outline]_`=_Null, [_^uint64^ uint64]_[*@3 pattern]_`=_[@3 0], 
[_^Color^ Color]_[*@3 doxor]_`=_Null)&]
[s2;%% Draws poly`-polygon.&]
[s3;%% &]
[s4;%% &]
[s5;:Draw`:`:DrawPolygons`(const Point`*`,int`,const int`*`,int`,Color`,int`,Color`,uint64`,Color`): [@(0.0.255) v
oid]_[* DrawPolygons]([@(0.0.255) const]_[_^Point^ Point]_`*[*@3 vertices], 
[@(0.0.255) int]_[*@3 vertex`_count], [@(0.0.255) const]_[@(0.0.255) int]_`*[*@3 polygon`_c
ounts], [@(0.0.255) int]_[*@3 polygon`_count`_count], [_^Color^ Color]_[*@3 color]_`=_Def
aultInk, [@(0.0.255) int]_[*@3 width]_`=_[@3 0], [_^Color^ Color]_[*@3 outline]_`=_Null, 
[_^uint64^ uint64]_[*@3 pattern]_`=_[@3 0], [_^Color^ Color]_[*@3 doxor]_`=_Null)&]
[s5;:Draw`:`:DrawPolygons`(const Vector`<Point`>`&`,const Vector`<int`>`&`,Color`,int`,Color`,uint64`,Color`): [@(0.0.255) v
oid]_[* DrawPolygons]([@(0.0.255) const]_[_^Vector^ Vector]<[_^Point^ Point]>`&_[*@3 vertic
es], [@(0.0.255) const]_[_^Vector^ Vector]<[@(0.0.255) int]>`&_[*@3 polygon`_counts], 
[_^Color^ Color]_[*@3 color]_`=_DefaultInk, [@(0.0.255) int]_[*@3 width]_`=_[@3 0], 
[_^Color^ Color]_[*@3 outline]_`=_Null, [_^uint64^ uint64]_[*@3 pattern]_`=_[@3 0], 
[_^Color^ Color]_[*@3 doxor]_`=_Null)&]
[s2;%% Draws multiple polygons.&]
[s3;%% &]
[s4;%% &]
[s5;:Draw`:`:DrawPolygon`(const Point`*`,int`,Color`,int`,Color`,uint64`,Color`): [@(0.0.255) v
oid]_[* DrawPolygon]([@(0.0.255) const]_[_^Point^ Point]_`*[*@3 vertices], 
[@(0.0.255) int]_[*@3 vertex`_count], [_^Color^ Color]_[*@3 color]_`=_DefaultInk, 
[@(0.0.255) int]_[*@3 width]_`=_[@3 0], [_^Color^ Color]_[*@3 outline]_`=_Null, 
[_^uint64^ uint64]_[*@3 pattern]_`=_[@3 0], [_^Color^ Color]_[*@3 doxor]_`=_Null)&]
[s5;:Draw`:`:DrawPolygon`(const Vector`<Point`>`&`,Color`,int`,Color`,uint64`,Color`): [@(0.0.255) v
oid]_[* DrawPolygon]([@(0.0.255) const]_[_^Vector^ Vector]<[_^Point^ Point]>`&_[*@3 vertice
s], [_^Color^ Color]_[*@3 color]_`=_DefaultInk, [@(0.0.255) int]_[*@3 width]_`=_[@3 0], 
[_^Color^ Color]_[*@3 outline]_`=_Null, [_^uint64^ uint64]_[*@3 pattern]_`=_[@3 0], 
[_^Color^ Color]_[*@3 doxor]_`=_Null)&]
[s2;%% Draws a polygon.&]
[s3;%% &]
[s4;%% &]
[s5;:Draw`:`:DrawDrawing`(const Rect`&`,const Drawing`&`): [@(0.0.255) void]_[* DrawDrawi
ng]([@(0.0.255) const]_[_^Rect^ Rect][@(0.0.255) `&]_[*@3 r], [@(0.0.255) const]_[_^Drawing^ D
rawing][@(0.0.255) `&]_[*@3 iw])&]
[s5;:Draw`:`:DrawDrawing`(int`,int`,int`,int`,const Drawing`&`): [@(0.0.255) void]_[* Dra
wDrawing]([@(0.0.255) int]_[*@3 x], [@(0.0.255) int]_[*@3 y], [@(0.0.255) int]_[*@3 cx], 
[@(0.0.255) int]_[*@3 cy], [@(0.0.255) const]_[_^Drawing^ Drawing][@(0.0.255) `&]_[*@3 iw])&]
[s2;%% Draws [^Drawing^ Drawing] [%-*@3 iw] scaled to rectangle [%-*@3 r] 
respectively [%-*@3 x],[%-*@3 y],[%-*@3 cx],[%-*@3 cy]. Frontend to DrawDrawing 
op.&]
[s3;%% &]
[s4; &]
[s5;:Draw`:`:DrawDrawing`(int`,int`,const Drawing`&`): [@(0.0.255) void]_[* DrawDrawing](
[@(0.0.255) int]_[*@3 x], [@(0.0.255) int]_[*@3 y], [@(0.0.255) const]_[_^Drawing^ Drawing][@(0.0.255) `&
]_[*@3 iw])&]
[s2;%% Draws [^Drawing^ Drawing] [%-*@3 iw] at [%-*@3 x],[%-*@3 y], using 
its original size. Frontend to DrawDrawing op.&]
[s3;%% &]
[s4;%% &]
[s5;:Draw`:`:DrawPainting`(int`,int`,const Painting`&`): [@(0.0.255) void]_[* DrawPaintin
g]([@(0.0.255) const]_[_^Rect^ Rect][@(0.0.255) `&]_[*@3 r], [@(0.0.255) const]_[_^Painting^ P
ainting][@(0.0.255) `&]_[*@3 iw])&]
[s5;:Draw`:`:DrawPainting`(int`,int`,int`,int`,const Painting`&`): [@(0.0.255) void]_[* D
rawPainting]([@(0.0.255) int]_[*@3 x], [@(0.0.255) int]_[*@3 y], [@(0.0.255) int]_[*@3 cx], 
[@(0.0.255) int]_[*@3 cy], [@(0.0.255) const]_[_^Painting^ Painting][@(0.0.255) `&]_[*@3 iw])
&]
[s5;:Draw`:`:DrawPainting`(int`,int`,const Painting`&`): [@(0.0.255) void]_[* DrawPaintin
g]([@(0.0.255) int]_[*@3 x], [@(0.0.255) int]_[*@3 y], [@(0.0.255) const]_[_^Painting^ Painti
ng][@(0.0.255) `&]_[*@3 iw])&]
[s2;%% Draws [^Painting^ Painting] [%-*@3 iw] scaled to rectangle [%-*@3 r] 
respectively [%-*@3 x],[%-*@3 y],[%-*@3 cx],[%-*@3 cy]. Painter package 
has to be included in the project for this to work. Variant without 
[%-*@3 cx],[%-*@3 cy] paints Painting in its original size. Frontend 
to DrawPaintingOp.&]
[s3;%% &]
[s4;%% &]
[s5;:Draw`:`:DrawText`(int`,int`,int`,const wchar`*`,Font`,Color`,int`,const int`*`): [@(0.0.255) v
oid]_[* DrawText]([@(0.0.255) int]_[*@3 x], [@(0.0.255) int]_[*@3 y], [@(0.0.255) int]_[*@3 ang
le], [@(0.0.255) const]_[_^wchar^ wchar]_`*[*@3 text], [_^Font^ Font]_[*@3 font]_`=_StdFont
(), [_^Color^ Color]_[*@3 ink]_`=_DefaultInk, [@(0.0.255) int]_[*@3 n]_`=_`-[@3 1], 
[@(0.0.255) const]_[@(0.0.255) int]_`*[*@3 dx]_`=_NULL)&]
[s5;:Draw`:`:DrawText`(int`,int`,const wchar`*`,Font`,Color`,int`,const int`*`): [@(0.0.255) v
oid]_[* DrawText]([@(0.0.255) int]_[*@3 x], [@(0.0.255) int]_[*@3 y], [@(0.0.255) const]_[_^wchar^ w
char]_`*[*@3 text], [_^Font^ Font]_[*@3 font]_`=_StdFont(), [_^Color^ Color]_[*@3 ink]_`=_D
efaultInk, [@(0.0.255) int]_[*@3 n]_`=_`-[@3 1], [@(0.0.255) const]_[@(0.0.255) int]_`*[*@3 d
x]_`=_NULL)&]
[s5;:Draw`:`:DrawText`(int`,int`,const WString`&`,Font`,Color`,const int`*`): [@(0.0.255) v
oid]_[* DrawText]([@(0.0.255) int]_[*@3 x], [@(0.0.255) int]_[*@3 y], [@(0.0.255) const]_[_^WString^ W
String][@(0.0.255) `&]_[*@3 text], [_^Font^ Font]_[*@3 font]_`=_StdFont(), 
[_^Color^ Color]_[*@3 ink]_`=_DefaultInk, [@(0.0.255) const]_[@(0.0.255) int]_`*[*@3 dx]_`=
_NULL)&]
[s5;:Draw`:`:DrawText`(int`,int`,int`,const WString`&`,Font`,Color`,const int`*`): [@(0.0.255) v
oid]_[* DrawText]([@(0.0.255) int]_[*@3 x], [@(0.0.255) int]_[*@3 y], [@(0.0.255) int]_[*@3 ang
le], [@(0.0.255) const]_[_^WString^ WString][@(0.0.255) `&]_[*@3 text], 
[_^Font^ Font]_[*@3 font]_`=_StdFont(), [_^Color^ Color]_[*@3 ink]_`=_DefaultInk, 
[@(0.0.255) const]_[@(0.0.255) int]_`*[*@3 dx]_`=_NULL)&]
[s5;:Draw`:`:DrawText`(int`,int`,int`,const char`*`,byte`,Font`,Color`,int`,const int`*`): [@(0.0.255) v
oid]_[* DrawText]([@(0.0.255) int]_[*@3 x], [@(0.0.255) int]_[*@3 y], [@(0.0.255) int]_[*@3 ang
le], [@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 text], [_^byte^ byte]_[*@3 charset], 
[_^Font^ Font]_[*@3 font]_`=_StdFont(), [_^Color^ Color]_[*@3 ink]_`=_DefaultInk, 
[@(0.0.255) int]_[*@3 n]_`=_`-[@3 1], [@(0.0.255) const]_[@(0.0.255) int]_`*[*@3 dx]_`=_NULL)
&]
[s5;:Draw`:`:DrawText`(int`,int`,const char`*`,byte`,Font`,Color`,int`,const int`*`): [@(0.0.255) v
oid]_[* DrawText]([@(0.0.255) int]_[*@3 x], [@(0.0.255) int]_[*@3 y], [@(0.0.255) const]_[@(0.0.255) c
har]_`*[*@3 text], [_^byte^ byte]_[*@3 charset], [_^Font^ Font]_[*@3 font]_`=_StdFont(), 
[_^Color^ Color]_[*@3 ink]_`=_DefaultInk, [@(0.0.255) int]_[*@3 n]_`=_`-[@3 1], 
[@(0.0.255) const]_[@(0.0.255) int]_`*[*@3 dx]_`=_NULL)&]
[s5;:Draw`:`:DrawText`(int`,int`,int`,const char`*`,Font`,Color`,int`,const int`*`): [@(0.0.255) v
oid]_[* DrawText]([@(0.0.255) int]_[*@3 x], [@(0.0.255) int]_[*@3 y], [@(0.0.255) int]_[*@3 ang
le], [@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 text], [_^Font^ Font]_[*@3 font]_`=_StdFon
t(), [_^Color^ Color]_[*@3 ink]_`=_DefaultInk, [@(0.0.255) int]_[*@3 n]_`=_`-[@3 1], 
[@(0.0.255) const]_[@(0.0.255) int]_`*[*@3 dx]_`=_NULL)&]
[s5;:Draw`:`:DrawText`(int`,int`,const char`*`,Font`,Color`,int`,const int`*`): [@(0.0.255) v
oid]_[* DrawText]([@(0.0.255) int]_[*@3 x], [@(0.0.255) int]_[*@3 y], [@(0.0.255) const]_[@(0.0.255) c
har]_`*[*@3 text], [_^Font^ Font]_[*@3 font]_`=_StdFont(), [_^Color^ Color]_[*@3 ink]_`=_De
faultInk, [@(0.0.255) int]_[*@3 n]_`=_`-[@3 1], [@(0.0.255) const]_[@(0.0.255) int]_`*[*@3 dx
]_`=_NULL)&]
[s5;:Draw`:`:DrawText`(int`,int`,const String`&`,Font`,Color`,const int`*`): [@(0.0.255) v
oid]_[* DrawText]([@(0.0.255) int]_[*@3 x], [@(0.0.255) int]_[*@3 y], [@(0.0.255) const]_[_^String^ S
tring][@(0.0.255) `&]_[*@3 text], [_^Font^ Font]_[*@3 font]_`=_StdFont(), 
[_^Color^ Color]_[*@3 ink]_`=_DefaultInk, [@(0.0.255) const]_[@(0.0.255) int]_`*[*@3 dx]_`=
_NULL)&]
[s5;:Draw`:`:DrawText`(int`,int`,int`,const String`&`,Font`,Color`,const int`*`): [@(0.0.255) v
oid]_[* DrawText]([@(0.0.255) int]_[*@3 x], [@(0.0.255) int]_[*@3 y], [@(0.0.255) int]_[*@3 ang
le], [@(0.0.255) const]_[_^String^ String][@(0.0.255) `&]_[*@3 text], 
[_^Font^ Font]_[*@3 font]_`=_StdFont(), [_^Color^ Color]_[*@3 ink]_`=_DefaultInk, 
[@(0.0.255) const]_[@(0.0.255) int]_`*[*@3 dx]_`=_NULL)&]
[s2;%% Draws a [%-*@3 text ]at position starting at [%-*@3 x],[%-*@3 y] 
(it represents top`-left corner of text cell: for horizontal text, 
x identifies reference point and y is ascent above the baseline) 
going at [%-*@3 angle] using [%-*@3 font] and color [%-*@3 ink]. [%-*@3 n] 
is a number of characters to paint, if negative, it is retrieved 
using `[w`]strlen. If [%-*@3 dx ]is not NULL, it specifies advancements 
of individual characters (it must be [%-*@3 n] elements in this 
array), otherwise advancements from font metrics are used. For 
8`-bit texts ([%-*@3 text] is [^String^ String] or [@(0.0.255) const 
char `*]), if [%-*@3 charset] is present, text is converted from 
this charset before printing, if no [%-*@3 charset] is specified, 
default charset is used. DrawText also performs missing glyph 
replacement and composition. If Draw`::GetInfo() returns DRAWTEXTLINES, 
DrawText uses DrawLine to provide support for Undeline and Strikeout 
Font styles. DrawText calls DrawTextOp for low`-level glyph rendering.&]
[s3;%% &]
[s4;%% &]
[s5;:Draw`:`:SinCos`(int`,double`&`,double`&`): [@(0.0.255) static] 
[@(0.0.255) void]_[* SinCos]([@(0.0.255) int]_[*@3 angle], [@(0.0.255) double`&]_[*@3 sina], 
[@(0.0.255) double`&]_[*@3 cosa])&]
[s2;%% This simple utility function computes sin(angle) and cos(angle), 
where the full angle (2[%- π) ]is 3600.&]
[s3; &]
[s0; ]]