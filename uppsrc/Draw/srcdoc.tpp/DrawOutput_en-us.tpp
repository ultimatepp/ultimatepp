topic "Draw";
[2 $$0,0#00000000000000000000000000000000:Default]
[l288;i704;a17;O9;~~~.992;2 $$1,0#10431211400427159095818037425705:param]
[a83;*R6 $$2,5#31310162474203024125188417583966:caption]
[b83;*2 $$3,5#07864147445237544204411237157677:title]
[b167;a42;C2 $$4,6#40027414424643823182269349404212:item]
[b42;a42;2 $$5,5#45413000475342174754091244180557:text]
[l288;a17;2 $$6,6#27521748481378242620020725143825:desc]
[l321;t246;C@5;1 $$7,7#20902679421464641399138805415013:code]
[b2503;2 $$8,0#65142375456100023862071332075487:separator]
[*@(0.0.255)2 $$9,0#83433469410354161042741608181528:base]
[t4167;C2 $$10,0#37138531426314131251341829483380:class]
[l288;a17;*1 $$11,11#70004532496200323422659154056402:requirement]
[i417;b42;a42;O9;~~~.416;2 $$12,12#10566046415157235020018451313112:tparam]
[b167;C2 $$13,13#92430459443460461911108080531343:item1]
[i288;a42;O9;C2 $$14,14#77422149456609303542238260500223:item2]
[*@2$(0.128.128)2 $$15,15#34511555403152284025741354420178:NewsDate]
[l321;*C$7;2 $$16,16#03451589433145915344929335295360:result]
[l321;b83;a83;*C$7;2 $$17,17#07531550463529505371228428965313:result`-line]
[l160;t4167;*C+117 $$18,5#88603949442205825958800053222425:package`-title]
[{_}%EN-US 
[s2; Draw&]
[s0; This article tries to describe view of the Draw object`'s role 
in the painting process.&]
[s0; &]
[s0; Please remember: Draw is not the raster image itself, much the 
less encoded in a particular image format. I believe you would 
do best to think about Draw as a channel or a toolbox interfacing 
the painting routine (a part of the program which wants to paint 
something; for the time being let`'s ignore the distinction between 
drawing vectors and rasters) to the actual `"canvas`" being painted 
onto. The main role of Draw is exactly to turn your logical drawing 
commands (e.g. `"draw a line`", `"draw a bit of text`", `"draw 
a raster image`" etc.) into something the output `"device`" can 
understand.&]
[s0; &]
[s0; Now, this is the general story. In reality there are basically 
two families of Draw`-related objects sharing many common traits. 
These are:&]
[s0; &]
[s0;i150;O2; raster`-oriented Draw objects: these normally include 
the Draw object in the Ctrl`::Paint override, the ViewDraw, the 
BackDraw, the ImageDraw `& ImageMaskDraw and sometimes PrintDraw.&]
[s0; &]
[s0;i150;O2; vector`-oriented Draw objects: DrawingDraw, WinMetaFileDraw 
and sometimes PrintDraw, and also (normally) PageDraw, PdfDraw 
and Report.&]
[s0; &]
[s0; Although some might prefer to explain the characteristics of 
the above objects in abstract terms (with multiple references 
to `"you don`'t need to know this and that`" :`-) ), I choose 
to be honest with you as to how they really work, because I believe 
that`'ll move you the farthest along the way.&]
[s0; &]
[s0; Actually the Draw is something like a semi`-intelligent painter 
with a plentiful palette of logical drawing objects; some of 
these are by nature vector`-oriented, like lines, rectangles, 
polygons etc., some are naturally rasterized, typically Images, 
some are a bit of both, especially text objects.&]
[s0; &]
[s0; Now, when the Draw receives a command to draw something (in 
U`+`+ terminology, a drawing operation, see the host of xxxOp 
methods in the declaration of Draw), the painter for the desired 
output device has to decide what to do with it. Typically, when 
the object is of the same type as the output medium (both are 
vector or raster), not much work has to be done. When the source 
(the drawing operation) is vector`-oriented and the output device 
is raster`-based, the vector object has to be rasterized.&]
[s0; &]
[s0; Typical raster`-based Draw objects are related to painting into 
windows and images; the Draw passed to Ctrl`::Paint by the U`+`+ 
windowing mechanism is such a case, as well as ViewDraw, BackDraw 
or ImageDraw and ImageMaskDraw. All these drawing objects use 
the MS Windows or X Windows mechanism called GDI to channel the 
drawing objects to the built`-in rasterizer which (perhaps using 
some graphic card accelerator in certain cases) ends up by modifying 
the desired pixels on the target device (the Ctrl area or the 
Image).&]
[s0; &]
[s0; By calling the other Draw classes `"vector`-based`" I don`'t 
mean they cannot cope with raster data. I am perhaps a little 
abusing the standard notion of the word to emphasize the fact 
that the latter group can manipulate vector objects directly, 
without rasterizing them first. So, for instance, DrawingDraw 
and WinMetaFileDraw are used to generate a serialized representation 
of the executed sequence of drawing operations, which can be 
later `"played back`" thus reproducing the original drawing.&]
[s0; &]
[s0; Notice that in the above cases there is no actual `"canvas`" 
to be painted onto. Both DrawingDraw and WinMetaFileDraw merely 
maintain a data stream which is used to record the individual 
drawing operations (coming into Draw via the xxxOp member functions) 
without actually caring about what is being drawn very much. 
This is also the reason why you cannot read rasterized data from 
such types of Draw: if you call DrawToImage, for instance, for 
a DrawingDraw, it is sure to fail. But then, even if you call 
DrawToImage for a ViewDraw, you cannot always count on getting 
the correct image, because in situations when a portion of the 
window is obscured by another application, you`'ll get a snapshot 
of this other application and not your window.&]
[s0; &]
[s0; The main advantage of DrawingDraw compared with ImageDraw (if 
we choose to see these two classes as two different means for 
creating a drawing which can be stored afterwards, perhaps to 
a file or a program cache) is that it can later (upon play`-back) 
reproduce the original vector operations (like lines or polygons) 
exactly even when size of `"painting`" is rescaled, without blocky 
artifacts inevitable in rasters.&]
[s0; &]
[s0; To sum the above into a few practical guidelines, please try 
to remember the following:&]
[s0; &]
[s0;i150;O2; To create a recording of an image, use DrawingDraw or 
ImageDraw (see the above distinction between these two). E.g.&]
[s0; &]
[s7; DrawingDraw ddraw(100, 100);&]
[s7; myobject.Paint(ddraw);&]
[s7; Drawing dwg `= ddraw.GetResult(); // dwg is the output recording&]
[s7; &]
[s7; Image img(100, 100);&]
[s7; ImageDraw idraw(img);&]
[s7; myobject.Paint(idraw);&]
[s7; idraw.Close();&]
[s7; // now the Image has been modified by the myobject`'s drawing&]
[s7; operations and can be e.g. saved to disk&]
[s0; &]
[s0;i150;O2; When using DrawToImage, you should take care of the 
above limitations. In particular, it is not always wise to use 
DrawToImage on a Draw which is not entirely under your control. 
To make the long story short, you`'re all right with ImageDraw 
and perhaps with BackDraw, but scarcely with anything else, becauses 
in most cases you cannot be sure what you`'ll get (e.g. with 
ViewDraw or PrintDraw). In certain cases you can be sure you`'ll 
get nothing at all (e.g. with DrawingDraw).&]
[s0; &]
[s0;i150;O2; As concerns the difference between an Image and an AlphaArray, 
logically there`'s none (both represent masked raster images). 
Physically there is a lot of difference (you can view this difference 
as more or less optimization`-related), because the AlphaArray 
is a physical matrix of pixels maintained directly by the application, 
whereas an Image is (at least sometimes) a logical bitmap object 
maintained by the Windows system (this is especially important 
in X Windows, because the AlphaArray is stored on the client, 
whereas Images are stored in the X Server). Another difference 
is that you can create an AlphaArray in any pixel format you 
want, but the supported pixel formats of Images are (or can be) 
limited by the properties of your windowing system.&]
[s0; &]
[s0;i150;O2; As concerns image storage in the various standard formats 
(like jpg, png or gif), again these formats belong neither to 
the Draw object, nor to the Image object. It`'s best to see the 
ImageEncoder`'s as mere data processors which receive a raster 
input (an AlphaArray or an Image) and produce an encoded linearized 
image as their output (this is obviously the encoding phase corresponding 
to the Save`-routines; with the Load`-routines, the situation 
is vice versa). Therefore, as soon as you have an Image (or an 
AlphaArray), you can Save it in any format you like. Similarly, 
you can take an image file in one of the standard formats and 
read it into an AlphaArray or an Image.&]
[s0; ]]