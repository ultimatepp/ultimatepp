topic "";
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
[ {{10000@(113.42.0) [s0;%% [*@7;4 Painter]]}}&]
[s0; &]
[s0; Painter is abstract class that represents `"SVG level`" rendering 
engine, supporting cubic and quadratic Bezier paths, affine transformations, 
filling and stroking with raster images, linear or radial gradients, 
clipping and alpha masking.&]
[s0; &]
[s0; &]
[s0; &]
[ {{10000F(128)G(128)@1 [s0;%% [* Other operations]]}}&]
[s4;H0; &]
[s5;:Upp`:`:Painter`:`:Clear`(const Upp`:`:RGBA`&`): [@(0.0.255) void] 
[* Clear]([@(0.0.255) const] RGBA[@(0.0.255) `&] [*@3 color ][@(0.0.255) `=] 
RGBAZero())&]
[s2;%% Sets all values in the target canvas to color. This is usually 
the first operation performed. Default RGBAZero() values sets 
canvas to be initially completely transparent.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:Painter`:`:Paint`(const Upp`:`:Painting`&`): [@(0.0.255) void] 
[* Paint]([@(0.0.255) const] Painting[@(0.0.255) `&] [*@3 p])&]
[s2;%% Replays all painting commands contained Painting object (which 
can be created using PaintingPainter class)&]
[s0; &]
[ {{10000F(128)G(128)@1 [s0;%% [* Painter context and attributes]]}}&]
[s0; &]
[s0; &]
[s0; &]
[ {{10000F(128)G(128)@1 [s0;*%% ]}}&]
[s0; &]
[s0; &]
[s0; &]
[ {{10000F(128)G(128)@1 [s0;%% [* Path stroking and filling]]}}&]
[s0; &]
[ {{10000F(128)G(128)@1 [s0;%% [* Path definition methods]]}}&]
[s3; &]
[s5;:Upp`:`:Painter`:`:Move`(const Upp`:`:Pointf`&`,bool`): Painter[@(0.0.255) `&] 
[* Move]([@(0.0.255) const] Pointf[@(0.0.255) `&] [*@3 p], [@(0.0.255) bool] 
[*@3 rel])&]
[s5;:Upp`:`:Painter`:`:Move`(const Upp`:`:Pointf`&`): Painter[@(0.0.255) `&] 
[* Move]([@(0.0.255) const] Pointf[@(0.0.255) `&] [*@3 p])&]
[s5;:Upp`:`:Painter`:`:Move`(double`,double`,bool`): Painter[@(0.0.255) `&] 
[* Move]([@(0.0.255) double] [*@3 x], [@(0.0.255) double] [*@3 y], [@(0.0.255) bool] 
[*@3 rel])&]
[s5;:Upp`:`:Painter`:`:Move`(double`,double`): Painter[@(0.0.255) `&] 
[* Move]([@(0.0.255) double] [*@3 x], [@(0.0.255) double] [*@3 y])&]
[s5;:Upp`:`:Painter`:`:RelMove`(const Upp`:`:Pointf`&`): Painter[@(0.0.255) `&] 
[* RelMove]([@(0.0.255) const] Pointf[@(0.0.255) `&] [*@3 p])&]
[s5;:Upp`:`:Painter`:`:RelMove`(double`,double`): Painter[@(0.0.255) `&] 
[* RelMove]([@(0.0.255) double] [*@3 x], [@(0.0.255) double] [*@3 y])&]
[s2;%% Move to a new path point, without any visual connection to 
the previous path. Path definition usually starts with Move. 
[%-*@3 p] or [%-*@3 x], [%-*@3 y] specify the point, if [%-*@3 rel] is 
true or with [* RelMove] the position is relative to the last point 
of path.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:Painter`:`:Line`(const Upp`:`:Pointf`&`,bool`): Painter[@(0.0.255) `&] 
[* Line]([@(0.0.255) const] Pointf[@(0.0.255) `&] [*@3 p], [@(0.0.255) bool] 
[*@3 rel])&]
[s5;:Upp`:`:Painter`:`:Line`(const Upp`:`:Pointf`&`): Painter[@(0.0.255) `&] 
[* Line]([@(0.0.255) const] Pointf[@(0.0.255) `&] [*@3 p])&]
[s5;:Upp`:`:Painter`:`:Line`(double`,double`,bool`): Painter[@(0.0.255) `&] 
[* Line]([@(0.0.255) double] [*@3 x], [@(0.0.255) double] [*@3 y], [@(0.0.255) bool] 
[*@3 rel])&]
[s5;:Upp`:`:Painter`:`:Line`(double`,double`): Painter[@(0.0.255) `&] 
[* Line]([@(0.0.255) double] [*@3 x], [@(0.0.255) double] [*@3 y])&]
[s5;:Upp`:`:Painter`:`:RelLine`(const Upp`:`:Pointf`&`): Painter[@(0.0.255) `&] 
[* RelLine]([@(0.0.255) const] Pointf[@(0.0.255) `&] [*@3 p])&]
[s5;:Upp`:`:Painter`:`:RelLine`(double`,double`): Painter[@(0.0.255) `&] 
[* RelLine]([@(0.0.255) double] [*@3 x], [@(0.0.255) double] [*@3 y])&]
[s2;%% Add a line to the current path from the last point of path 
to [%-*@3 p] / [%-*@3 x] [%-*@3 y]. If [%-*@3 rel] is true or with [* RelLine] 
the position is relative to the last point of path.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:Painter`:`:Quadratic`(const Upp`:`:Pointf`&`,const Upp`:`:Pointf`&`,bool`): P
ainter[@(0.0.255) `&] [* Quadratic]([@(0.0.255) const] Pointf[@(0.0.255) `&] 
[*@3 p1], [@(0.0.255) const] Pointf[@(0.0.255) `&] [*@3 p], [@(0.0.255) bool] 
[*@3 rel])&]
[s5;:Upp`:`:Painter`:`:Quadratic`(const Upp`:`:Pointf`&`,const Upp`:`:Pointf`&`): Pai
nter[@(0.0.255) `&] [* Quadratic]([@(0.0.255) const] Pointf[@(0.0.255) `&] 
[*@3 p1], [@(0.0.255) const] Pointf[@(0.0.255) `&] [*@3 p])&]
[s5;:Upp`:`:Painter`:`:Quadratic`(const Upp`:`:Pointf`&`): Painter[@(0.0.255) `&] 
[* Quadratic]([@(0.0.255) const] Pointf[@(0.0.255) `&] [*@3 p])&]
[s5;:Upp`:`:Painter`:`:Quadratic`(double`,double`,bool`): Painter[@(0.0.255) `&] 
[* Quadratic]([@(0.0.255) double] [*@3 x], [@(0.0.255) double] [*@3 y], 
[@(0.0.255) bool] [*@3 rel])&]
[s5;:Upp`:`:Painter`:`:Quadratic`(double`,double`,double`,double`,bool`): Painter[@(0.0.255) `&
] [* Quadratic]([@(0.0.255) double] [*@3 x1], [@(0.0.255) double] [*@3 y1], 
[@(0.0.255) double] [*@3 x], [@(0.0.255) double] [*@3 y], [@(0.0.255) bool] 
[*@3 rel])&]
[s5;:Upp`:`:Painter`:`:Quadratic`(const Upp`:`:Pointf`&`,bool`): Painter[@(0.0.255) `&] 
[* Quadratic]([@(0.0.255) const] Pointf[@(0.0.255) `&] [*@3 p], [@(0.0.255) bool] 
[*@3 rel])&]
[s5;:Upp`:`:Painter`:`:Quadratic`(double`,double`,double`,double`): Painter[@(0.0.255) `&
] [* Quadratic]([@(0.0.255) double] [*@3 x1], [@(0.0.255) double] [*@3 y1], 
[@(0.0.255) double] [*@3 x], [@(0.0.255) double] [*@3 y])&]
[s5;:Upp`:`:Painter`:`:Quadratic`(double`,double`): Painter[@(0.0.255) `&] 
[* Quadratic]([@(0.0.255) double] [*@3 x], [@(0.0.255) double] [*@3 y])&]
[s5;:Upp`:`:Painter`:`:RelQuadratic`(const Upp`:`:Pointf`&`,const Upp`:`:Pointf`&`): P
ainter[@(0.0.255) `&] [* RelQuadratic]([@(0.0.255) const] Pointf[@(0.0.255) `&] 
[*@3 p1], [@(0.0.255) const] Pointf[@(0.0.255) `&] [*@3 p])&]
[s5;:Upp`:`:Painter`:`:RelQuadratic`(double`,double`,double`,double`): Painter[@(0.0.255) `&
] [* RelQuadratic]([@(0.0.255) double] [*@3 x1], [@(0.0.255) double] 
[*@3 y1], [@(0.0.255) double] [*@3 x], [@(0.0.255) double] [*@3 y])&]
[s5;:Upp`:`:Painter`:`:RelQuadratic`(double`,double`): Painter[@(0.0.255) `&] 
[* RelQuadratic]([@(0.0.255) double] [*@3 x], [@(0.0.255) double] [*@3 y])&]
[s5;:Upp`:`:Painter`:`:RelQuadratic`(const Upp`:`:Pointf`&`): Painter[@(0.0.255) `&] 
[* RelQuadratic]([@(0.0.255) const] Pointf[@(0.0.255) `&] [*@3 p])&]
[s2;%% Adds quadratic Bezier curve to final point [%-*@3 p ][%- /][%-*@3  
x] [%-*@3 y ][%- with control point][%-*@3  p1] / [%-*@3 x1] [%-*@3 y1]. 
If [%-*@3 rel] is true or with [* RelQuadratic] the points are relative 
to the last point of path.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:Painter`:`:Cubic`(const Upp`:`:Pointf`&`,const Upp`:`:Pointf`&`,const Upp`:`:Pointf`&`,bool`): P
ainter[@(0.0.255) `&] [* Cubic]([@(0.0.255) const] Pointf[@(0.0.255) `&] 
[*@3 p1], [@(0.0.255) const] Pointf[@(0.0.255) `&] [*@3 p2], [@(0.0.255) const] 
Pointf[@(0.0.255) `&] [*@3 p], [@(0.0.255) bool] [*@3 rel])&]
[s5;:Upp`:`:Painter`:`:Cubic`(const Upp`:`:Pointf`&`,const Upp`:`:Pointf`&`,bool`): P
ainter[@(0.0.255) `&] [* Cubic]([@(0.0.255) const] Pointf[@(0.0.255) `&] 
[*@3 p2], [@(0.0.255) const] Pointf[@(0.0.255) `&] [*@3 p], [@(0.0.255) bool] 
[*@3 rel])&]
[s5;:Upp`:`:Painter`:`:Cubic`(const Upp`:`:Pointf`&`,const Upp`:`:Pointf`&`,const Upp`:`:Pointf`&`): P
ainter[@(0.0.255) `&] [* Cubic]([@(0.0.255) const] Pointf[@(0.0.255) `&] 
[*@3 p1], [@(0.0.255) const] Pointf[@(0.0.255) `&] [*@3 p2], [@(0.0.255) const] 
Pointf[@(0.0.255) `&] [*@3 p])&]
[s5;:Upp`:`:Painter`:`:Cubic`(const Upp`:`:Pointf`&`,const Upp`:`:Pointf`&`): Painter
[@(0.0.255) `&] [* Cubic]([@(0.0.255) const] Pointf[@(0.0.255) `&] [*@3 p2], 
[@(0.0.255) const] Pointf[@(0.0.255) `&] [*@3 p])&]
[s5;:Upp`:`:Painter`:`:Cubic`(double`,double`,double`,double`,double`,double`,bool`): P
ainter[@(0.0.255) `&] [* Cubic]([@(0.0.255) double] [*@3 x1], [@(0.0.255) double] 
[*@3 y1], [@(0.0.255) double] [*@3 x2], [@(0.0.255) double] [*@3 y2], [@(0.0.255) double] 
[*@3 x], [@(0.0.255) double] [*@3 y], [@(0.0.255) bool] [*@3 rel])&]
[s5;:Upp`:`:Painter`:`:Cubic`(double`,double`,double`,double`,bool`): Painter[@(0.0.255) `&
] [* Cubic]([@(0.0.255) double] [*@3 x2], [@(0.0.255) double] [*@3 y2], 
[@(0.0.255) double] [*@3 x], [@(0.0.255) double] [*@3 y], [@(0.0.255) bool] 
[*@3 rel])&]
[s5;:Upp`:`:Painter`:`:Cubic`(double`,double`,double`,double`,double`,double`): Paint
er[@(0.0.255) `&] [* Cubic]([@(0.0.255) double] [*@3 x1], [@(0.0.255) double] 
[*@3 y1], [@(0.0.255) double] [*@3 x2], [@(0.0.255) double] [*@3 y2], [@(0.0.255) double] 
[*@3 x], [@(0.0.255) double] [*@3 y])&]
[s5;:Upp`:`:Painter`:`:Cubic`(double`,double`,double`,double`): Painter[@(0.0.255) `&] 
[* Cubic]([@(0.0.255) double] [*@3 x2], [@(0.0.255) double] [*@3 y2], [@(0.0.255) double] 
[*@3 x], [@(0.0.255) double] [*@3 y])&]
[s5;:Upp`:`:Painter`:`:RelCubic`(const Upp`:`:Pointf`&`,const Upp`:`:Pointf`&`,const Upp`:`:Pointf`&`): P
ainter[@(0.0.255) `&] [* RelCubic]([@(0.0.255) const] Pointf[@(0.0.255) `&] 
[*@3 p1], [@(0.0.255) const] Pointf[@(0.0.255) `&] [*@3 p2], [@(0.0.255) const] 
Pointf[@(0.0.255) `&] [*@3 p])&]
[s5;:Upp`:`:Painter`:`:RelCubic`(const Upp`:`:Pointf`&`,const Upp`:`:Pointf`&`): Pain
ter[@(0.0.255) `&] [* RelCubic]([@(0.0.255) const] Pointf[@(0.0.255) `&] 
[*@3 p2], [@(0.0.255) const] Pointf[@(0.0.255) `&] [*@3 p])&]
[s5;:Upp`:`:Painter`:`:RelCubic`(double`,double`,double`,double`,double`,double`): Pa
inter[@(0.0.255) `&] [* RelCubic]([@(0.0.255) double] [*@3 x1], [@(0.0.255) double] 
[*@3 y1], [@(0.0.255) double] [*@3 x2], [@(0.0.255) double] [*@3 y2], [@(0.0.255) double] 
[*@3 x], [@(0.0.255) double] [*@3 y])&]
[s5;:Upp`:`:Painter`:`:RelCubic`(double`,double`,double`,double`): Painter[@(0.0.255) `&
] [* RelCubic]([@(0.0.255) double] [*@3 x2], [@(0.0.255) double] [*@3 y2], 
[@(0.0.255) double] [*@3 x], [@(0.0.255) double] [*@3 y])&]
[s2;%% Adds cubic Bezier curve to final point [%-*@3 p ][%- /][%-*@3  x] 
[%-*@3 y ][%- with control points][%-*@3  p1] / [%-*@3 x1] [%-*@3 y1].and 
[%-*@3 p2] / [%-*@3 x2] [%-*@3 y2]. If [%-*@3 rel] is true or with [* RelCubic] 
the points are relative to the last point of path.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:Painter`:`:Arc`(const Upp`:`:Pointf`&`,const Upp`:`:Pointf`&`,double`,double`,bool`): P
ainter[@(0.0.255) `&] [* Arc]([@(0.0.255) const] Pointf[@(0.0.255) `&] 
[*@3 c], [@(0.0.255) const] Pointf[@(0.0.255) `&] [*@3 r], [@(0.0.255) double] 
[*@3 angle], [@(0.0.255) double] [*@3 sweep], [@(0.0.255) bool] [*@3 rel])&]
[s5;:Upp`:`:Painter`:`:Arc`(const Upp`:`:Pointf`&`,double`,double`,double`,double`,bool`): P
ainter[@(0.0.255) `&] [* Arc]([@(0.0.255) const] Pointf[@(0.0.255) `&] 
[*@3 c], [@(0.0.255) double] [*@3 rx], [@(0.0.255) double] [*@3 ry], [@(0.0.255) double] 
[*@3 angle], [@(0.0.255) double] [*@3 sweep], [@(0.0.255) bool] [*@3 rel])&]
[s5;:Upp`:`:Painter`:`:Arc`(const Upp`:`:Pointf`&`,double`,double`,double`,bool`): Pa
inter[@(0.0.255) `&] [* Arc]([@(0.0.255) const] Pointf[@(0.0.255) `&] 
[*@3 c], [@(0.0.255) double] [*@3 r], [@(0.0.255) double] [*@3 angle], 
[@(0.0.255) double] [*@3 sweep], [@(0.0.255) bool] [*@3 rel])&]
[s5;:Upp`:`:Painter`:`:Arc`(double`,double`,double`,double`,double`,double`,bool`): P
ainter[@(0.0.255) `&] [* Arc]([@(0.0.255) double] [*@3 x], [@(0.0.255) double] 
[*@3 y], [@(0.0.255) double] [*@3 rx], [@(0.0.255) double] [*@3 ry], [@(0.0.255) double] 
[*@3 angle], [@(0.0.255) double] [*@3 sweep], [@(0.0.255) bool] [*@3 rel])&]
[s5;:Upp`:`:Painter`:`:Arc`(double`,double`,double`,double`,double`,bool`): Painter[@(0.0.255) `&
] [* Arc]([@(0.0.255) double] [*@3 x], [@(0.0.255) double] [*@3 y], [@(0.0.255) double] 
[*@3 r], [@(0.0.255) double] [*@3 angle], [@(0.0.255) double] [*@3 sweep], 
[@(0.0.255) bool] [*@3 rel])&]
[s5;:Upp`:`:Painter`:`:Arc`(const Upp`:`:Pointf`&`,const Upp`:`:Pointf`&`,double`,double`): P
ainter[@(0.0.255) `&] [* Arc]([@(0.0.255) const] Pointf[@(0.0.255) `&] 
[*@3 c], [@(0.0.255) const] Pointf[@(0.0.255) `&] [*@3 r], [@(0.0.255) double] 
[*@3 angle], [@(0.0.255) double] [*@3 sweep])&]
[s5;:Upp`:`:Painter`:`:Arc`(const Upp`:`:Pointf`&`,double`,double`,double`,double`): P
ainter[@(0.0.255) `&] [* Arc]([@(0.0.255) const] Pointf[@(0.0.255) `&] 
[*@3 c], [@(0.0.255) double] [*@3 rx], [@(0.0.255) double] [*@3 ry], [@(0.0.255) double] 
[*@3 angle], [@(0.0.255) double] [*@3 sweep])&]
[s5;:Upp`:`:Painter`:`:Arc`(const Upp`:`:Pointf`&`,double`,double`,double`): Painter[@(0.0.255) `&
] [* Arc]([@(0.0.255) const] Pointf[@(0.0.255) `&] [*@3 c], [@(0.0.255) double] 
[*@3 r], [@(0.0.255) double] [*@3 angle], [@(0.0.255) double] [*@3 sweep])&]
[s5;:Upp`:`:Painter`:`:Arc`(double`,double`,double`,double`,double`,double`): Painter
[@(0.0.255) `&] [* Arc]([@(0.0.255) double] [*@3 x], [@(0.0.255) double] 
[*@3 y], [@(0.0.255) double] [*@3 rx], [@(0.0.255) double] [*@3 ry], [@(0.0.255) double] 
[*@3 angle], [@(0.0.255) double] [*@3 sweep])&]
[s5;:Upp`:`:Painter`:`:Arc`(double`,double`,double`,double`,double`): Painter[@(0.0.255) `&
] [* Arc]([@(0.0.255) double] [*@3 x], [@(0.0.255) double] [*@3 y], [@(0.0.255) double] 
[*@3 r], [@(0.0.255) double] [*@3 angle], [@(0.0.255) double] [*@3 sweep])&]
[s5;:Upp`:`:Painter`:`:RelArc`(const Upp`:`:Pointf`&`,const Upp`:`:Pointf`&`,double`,double`): P
ainter[@(0.0.255) `&] [* RelArc]([@(0.0.255) const] Pointf[@(0.0.255) `&] 
[*@3 c], [@(0.0.255) const] Pointf[@(0.0.255) `&] [*@3 r], [@(0.0.255) double] 
[*@3 angle], [@(0.0.255) double] [*@3 sweep])&]
[s5;:Upp`:`:Painter`:`:RelArc`(const Upp`:`:Pointf`&`,double`,double`,double`,double`): P
ainter[@(0.0.255) `&] [* RelArc]([@(0.0.255) const] Pointf[@(0.0.255) `&] 
[*@3 c], [@(0.0.255) double] [*@3 rx], [@(0.0.255) double] [*@3 ry], [@(0.0.255) double] 
[*@3 angle], [@(0.0.255) double] [*@3 sweep])&]
[s5;:Upp`:`:Painter`:`:RelArc`(const Upp`:`:Pointf`&`,double`,double`,double`): Paint
er[@(0.0.255) `&] [* RelArc]([@(0.0.255) const] Pointf[@(0.0.255) `&] 
[*@3 c], [@(0.0.255) double] [*@3 r], [@(0.0.255) double] [*@3 angle], 
[@(0.0.255) double] [*@3 sweep])&]
[s5;:Upp`:`:Painter`:`:RelArc`(double`,double`,double`,double`,double`,double`): Pain
ter[@(0.0.255) `&] [* RelArc]([@(0.0.255) double] [*@3 x], [@(0.0.255) double] 
[*@3 y], [@(0.0.255) double] [*@3 rx], [@(0.0.255) double] [*@3 ry], [@(0.0.255) double] 
[*@3 angle], [@(0.0.255) double] [*@3 sweep])&]
[s5;:Upp`:`:Painter`:`:RelArc`(double`,double`,double`,double`,double`): Painter[@(0.0.255) `&
] [* RelArc]([@(0.0.255) double] [*@3 x], [@(0.0.255) double] [*@3 y], 
[@(0.0.255) double] [*@3 r], [@(0.0.255) double] [*@3 angle], [@(0.0.255) double] 
[*@3 sweep])&]
[s2;%% Adds circular or elliptical arc. [%-*@3 c] / [%-*@3 x] [%-*@3 y] 
is the center of circle or ellipse, [%-*@3 r] is the radius of 
circle if it is double or horizontal and vertical radius specified 
as Pointf, [%-*@3 rx] [%-*@3 ry] are ellipse radii, [%-*@3 angle] is 
the starting angle of arc in radians where 0 is the angle of 
positive x axis, [%-*@3 sweep] is the ending angle of arc, positive 
sweep is in clockwise direction. The line is first added from 
the current position to the starting point of arc (if you want 
arc without it, just Move the position to the starting point). 
If [%-*@3 rel] is true or with [%-* RelArc ]the center is relative 
to the last point of path.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:Painter`:`:SvgArc`(const Upp`:`:Pointf`&`,double`,bool`,bool`,const Upp`:`:Pointf`&`,bool`): P
ainter[@(0.0.255) `&] [* SvgArc]([@(0.0.255) const] Pointf[@(0.0.255) `&] 
[*@3 r], [@(0.0.255) double] [*@3 xangle], [@(0.0.255) bool] [*@3 large], 
[@(0.0.255) bool] [*@3 sweep], [@(0.0.255) const] Pointf[@(0.0.255) `&] 
[*@3 p], [@(0.0.255) bool] [*@3 rel])&]
[s5;:Upp`:`:Painter`:`:SvgArc`(double`,double`,double`,bool`,bool`,const Upp`:`:Pointf`&`,bool`): P
ainter[@(0.0.255) `&] [* SvgArc]([@(0.0.255) double] [*@3 rx], [@(0.0.255) double] 
[*@3 ry], [@(0.0.255) double] [*@3 xangle], [@(0.0.255) bool] [*@3 large], 
[@(0.0.255) bool] [*@3 sweep], [@(0.0.255) const] Pointf[@(0.0.255) `&] 
[*@3 p], [@(0.0.255) bool] [*@3 rel])&]
[s5;:Upp`:`:Painter`:`:SvgArc`(double`,double`,double`,bool`,bool`,double`,double`,bool`): P
ainter[@(0.0.255) `&] [* SvgArc]([@(0.0.255) double] [*@3 rx], [@(0.0.255) double] 
[*@3 ry], [@(0.0.255) double] [*@3 xangle], [@(0.0.255) bool] [*@3 large], 
[@(0.0.255) bool] [*@3 sweep], [@(0.0.255) double] [*@3 x], [@(0.0.255) double] 
[*@3 y], [@(0.0.255) bool] [*@3 rel])&]
[s5;:Upp`:`:Painter`:`:SvgArc`(const Upp`:`:Pointf`&`,double`,bool`,bool`,const Upp`:`:Pointf`&`): P
ainter[@(0.0.255) `&] [* SvgArc]([@(0.0.255) const] Pointf[@(0.0.255) `&] 
[*@3 r], [@(0.0.255) double] [*@3 xangle], [@(0.0.255) bool] [*@3 large], 
[@(0.0.255) bool] [*@3 sweep], [@(0.0.255) const] Pointf[@(0.0.255) `&] 
[*@3 p])&]
[s5;:Upp`:`:Painter`:`:SvgArc`(double`,double`,double`,bool`,bool`,const Upp`:`:Pointf`&`): P
ainter[@(0.0.255) `&] [* SvgArc]([@(0.0.255) double] [*@3 rx], [@(0.0.255) double] 
[*@3 ry], [@(0.0.255) double] [*@3 xangle], [@(0.0.255) bool] [*@3 large], 
[@(0.0.255) bool] [*@3 sweep], [@(0.0.255) const] Pointf[@(0.0.255) `&] 
[*@3 p])&]
[s5;:Upp`:`:Painter`:`:SvgArc`(double`,double`,double`,bool`,bool`,double`,double`): P
ainter[@(0.0.255) `&] [* SvgArc]([@(0.0.255) double] [*@3 rx], [@(0.0.255) double] 
[*@3 ry], [@(0.0.255) double] [*@3 xangle], [@(0.0.255) bool] [*@3 large], 
[@(0.0.255) bool] [*@3 sweep], [@(0.0.255) double] [*@3 x], [@(0.0.255) double] 
[*@3 y])&]
[s5;:Upp`:`:Painter`:`:RelSvgArc`(const Upp`:`:Pointf`&`,double`,bool`,bool`,const Upp`:`:Pointf`&`): P
ainter[@(0.0.255) `&] [* RelSvgArc]([@(0.0.255) const] Pointf[@(0.0.255) `&] 
[*@3 r], [@(0.0.255) double] [*@3 xangle], [@(0.0.255) bool] [*@3 large], 
[@(0.0.255) bool] [*@3 sweep], [@(0.0.255) const] Pointf[@(0.0.255) `&] 
[*@3 p])&]
[s5;:Upp`:`:Painter`:`:RelSvgArc`(double`,double`,double`,bool`,bool`,const Upp`:`:Pointf`&`): P
ainter[@(0.0.255) `&] [* RelSvgArc]([@(0.0.255) double] [*@3 rx], [@(0.0.255) double] 
[*@3 ry], [@(0.0.255) double] [*@3 xangle], [@(0.0.255) bool] [*@3 large], 
[@(0.0.255) bool] [*@3 sweep], [@(0.0.255) const] Pointf[@(0.0.255) `&] 
[*@3 p])&]
[s5;:Upp`:`:Painter`:`:RelSvgArc`(double`,double`,double`,bool`,bool`,double`,double`): P
ainter[@(0.0.255) `&] [* RelSvgArc]([@(0.0.255) double] [*@3 rx], [@(0.0.255) double] 
[*@3 ry], [@(0.0.255) double] [*@3 xangle], [@(0.0.255) bool] [*@3 large], 
[@(0.0.255) bool] [*@3 sweep], [@(0.0.255) double] [*@3 x], [@(0.0.255) double] 
[*@3 y])&]
[s2;%% Adds arc specified by SVG compatible definition. If [%-*@3 rel] 
is true or with [%-* RelSvgArc ]the center is relative to the last 
point of path.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:Painter`:`:Path`(Upp`:`:CParser`&`): Painter[@(0.0.255) `&] 
[* Path](CParser[@(0.0.255) `&] [*@3 p])&]
[s5;:Upp`:`:Painter`:`:Path`(const char`*`): Painter[@(0.0.255) `&] 
[* Path]([@(0.0.255) const] [@(0.0.255) char] [@(0.0.255) `*][*@3 path])&]
[s2;%% Adds path by parsing input text in SVG path definition format.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:Painter`:`:Character`(const Upp`:`:Pointf`&`,int`,Upp`:`:Font`): Painter[@(0.0.255) `&
] [* Character]([@(0.0.255) const] Pointf[@(0.0.255) `&] [*@3 p], [@(0.0.255) int] 
[*@3 ch], Font [*@3 fnt])&]
[s5;:Upp`:`:Painter`:`:Character`(double`,double`,int`,Upp`:`:Font`): Painter[@(0.0.255) `&
] [* Character]([@(0.0.255) double] [*@3 x], [@(0.0.255) double] [*@3 y], 
[@(0.0.255) int] [*@3 ch], Font [*@3 fnt])&]
[s2;%% [%- Adds character with UNICODE codepoint ][%-*@3 ch] from Font 
[%-*@3 fnt ](font can be replaced if given character is missing 
in it) at the position[%-*@3  p] / [%-*@3 x] [%-*@3 y].&]
[s3; &]
[s4; &]
[s5;:Upp`:`:Painter`:`:Text`(const Upp`:`:Pointf`&`,const Upp`:`:wchar`*`,Upp`:`:Font`,int`,const double`*`): P
ainter[@(0.0.255) `&] [* Text]([@(0.0.255) const] Pointf[@(0.0.255) `&] 
[*@3 p], [@(0.0.255) const] wchar [@(0.0.255) `*][*@3 text], Font [*@3 fnt], 
[@(0.0.255) int] [*@3 n] [@(0.0.255) `=] [@(0.0.255) `-][@3 1], [@(0.0.255) const] 
[@(0.0.255) double] [@(0.0.255) `*][*@3 dx] [@(0.0.255) `=] [@3 0])&]
[s5;:Upp`:`:Painter`:`:Text`(double`,double`,const Upp`:`:wchar`*`,Upp`:`:Font`,int`,const double`*`): P
ainter[@(0.0.255) `&] [* Text]([@(0.0.255) double] [*@3 x], [@(0.0.255) double] 
[*@3 y], [@(0.0.255) const] wchar [@(0.0.255) `*][*@3 text], Font [*@3 fnt], 
[@(0.0.255) int] [*@3 n] [@(0.0.255) `=] [@(0.0.255) `-][@3 1], [@(0.0.255) const] 
[@(0.0.255) double] [@(0.0.255) `*][*@3 dx] [@(0.0.255) `=] [@3 0])&]
[s5;:Upp`:`:Painter`:`:Text`(const Upp`:`:Pointf`&`,const Upp`:`:WString`&`,Upp`:`:Font`,const double`*`): P
ainter[@(0.0.255) `&] [* Text]([@(0.0.255) const] Pointf[@(0.0.255) `&] 
[*@3 p], [@(0.0.255) const] WString[@(0.0.255) `&] [*@3 s], Font [*@3 fnt], 
[@(0.0.255) const] [@(0.0.255) double] [@(0.0.255) `*][*@3 dx] [@(0.0.255) `=] 
[@3 0])&]
[s5;:Upp`:`:Painter`:`:Text`(double`,double`,const Upp`:`:WString`&`,Upp`:`:Font`,const double`*`): P
ainter[@(0.0.255) `&] [* Text]([@(0.0.255) double] [*@3 x], [@(0.0.255) double] 
[*@3 y], [@(0.0.255) const] WString[@(0.0.255) `&] [*@3 s], Font [*@3 fnt], 
[@(0.0.255) const] [@(0.0.255) double] [@(0.0.255) `*][*@3 dx] [@(0.0.255) `=] 
[@3 0])&]
[s5;:Upp`:`:Painter`:`:Text`(const Upp`:`:Pointf`&`,const Upp`:`:String`&`,Upp`:`:Font`,const double`*`): P
ainter[@(0.0.255) `&] [* Text]([@(0.0.255) const] Pointf[@(0.0.255) `&] 
[*@3 p], [@(0.0.255) const] String[@(0.0.255) `&] [*@3 s], Font [*@3 fnt], 
[@(0.0.255) const] [@(0.0.255) double] [@(0.0.255) `*][*@3 dx] [@(0.0.255) `=] 
[@3 0])&]
[s5;:Upp`:`:Painter`:`:Text`(double`,double`,const Upp`:`:String`&`,Upp`:`:Font`,const double`*`): P
ainter[@(0.0.255) `&] [* Text]([@(0.0.255) double] [*@3 x], [@(0.0.255) double] 
[*@3 y], [@(0.0.255) const] String[@(0.0.255) `&] [*@3 s], Font [*@3 fnt], 
[@(0.0.255) const] [@(0.0.255) double] [@(0.0.255) `*][*@3 dx] [@(0.0.255) `=] 
[@3 0])&]
[s5;:Upp`:`:Painter`:`:Text`(const Upp`:`:Pointf`&`,const char`*`,Upp`:`:Font`,int`,const double`*`): P
ainter[@(0.0.255) `&] [* Text]([@(0.0.255) const] Pointf[@(0.0.255) `&] 
[*@3 p], [@(0.0.255) const] [@(0.0.255) char] [@(0.0.255) `*][*@3 text], 
Font [*@3 fnt], [@(0.0.255) int] [*@3 n] [@(0.0.255) `=] [@(0.0.255) `-][@3 1], 
[@(0.0.255) const] [@(0.0.255) double] [@(0.0.255) `*][*@3 dx] [@(0.0.255) `=] 
[@3 0])&]
[s5;:Upp`:`:Painter`:`:Text`(double`,double`,const char`*`,Upp`:`:Font`,int`,const double`*`): P
ainter[@(0.0.255) `&] [* Text]([@(0.0.255) double] [*@3 x], [@(0.0.255) double] 
[*@3 y], [@(0.0.255) const] [@(0.0.255) char] [@(0.0.255) `*][*@3 text], 
Font [*@3 fnt], [@(0.0.255) int] [*@3 n] [@(0.0.255) `=] [@(0.0.255) `-][@3 1], 
[@(0.0.255) const] [@(0.0.255) double] [@(0.0.255) `*][*@3 dx] [@(0.0.255) `=] 
[@3 0])&]
[s2;%% Convenience method that renders [%-*@3 text] (using Character 
method) at [%-*@3 p] / [%-*@3 x] [%-*@3 y] using [%-*@3 fnt]. Number 
of characters can be limited to [%-*@3 n] (`-1 means render all 
of them, advance between characters can be overriden with [%-*@3 dx].&]
[s3; &]
[s4; &]
[s5;:Upp`:`:Painter`:`:Rectangle`(double`,double`,double`,double`): Painter[@(0.0.255) `&
] [* Rectangle]([@(0.0.255) double] [*@3 x], [@(0.0.255) double] [*@3 y], 
[@(0.0.255) double] [*@3 cx], [@(0.0.255) double] [*@3 cy])&]
[s2;%% Convenience method that adds a rectangle to the path.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:Painter`:`:RoundedRectangle`(double`,double`,double`,double`,double`): Pa
inter[@(0.0.255) `&] [* RoundedRectangle]([@(0.0.255) double] [*@3 x], 
[@(0.0.255) double] [*@3 y], [@(0.0.255) double] [*@3 cx], [@(0.0.255) double] 
[*@3 cy], [@(0.0.255) double] [*@3 r])&]
[s5;:Upp`:`:Painter`:`:RoundedRectangle`(double`,double`,double`,double`,double`,double`): P
ainter[@(0.0.255) `&] [* RoundedRectangle]([@(0.0.255) double] [*@3 x], 
[@(0.0.255) double] [*@3 y], [@(0.0.255) double] [*@3 cx], [@(0.0.255) double] 
[*@3 cy], [@(0.0.255) double] [*@3 rx], [@(0.0.255) double] [*@3 ry])&]
[s2;%% Convenience method that adds rounded rectangle to the path. 
[%-*@3 r] specifies the radius of circular corner, [%-*@3 rx] [%-*@3 ry] 
radii of elliptical corners.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:Painter`:`:Ellipse`(double`,double`,double`,double`): Painter[@(0.0.255) `&
] [* Ellipse]([@(0.0.255) double] [*@3 x], [@(0.0.255) double] [*@3 y], 
[@(0.0.255) double] [*@3 rx], [@(0.0.255) double] [*@3 ry])&]
[s2;%% Convenience method that adds ellipse to the path.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:Painter`:`:Circle`(double`,double`,double`): Painter[@(0.0.255) `&] 
[* Circle]([@(0.0.255) double] [*@3 x], [@(0.0.255) double] [*@3 y], [@(0.0.255) double] 
[*@3 r])&]
[s2;%% Convenience method that adds circle to the path.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:Painter`:`:Div`(`): Painter[@(0.0.255) `&] [* Div]()&]
[s2;%% Divides path into groups that are treated separately with 
respect to current `"insideness`" rule (even`-odd or non`-zero 
winding rule). For example:&]
[s2;%% [C1 sw.Circle(200, 200, 100).Circle(270, 200, 100).EvenOdd().Fill(Black());]&]
[s2;%% 
@@image:457&339
(A3UC0QEAAAAAAAAAAHic7d13VFT3vv//38DQUSkaCxAUFRWjGGNECXZjrERNYokiYkuxRVGDSXRZotiNLZZgPWrUGA0x3oRY0CiaiDXFFitgoUsRBIY5333WPet37rk3R+Q9wGs+w+vxf9aKn8/zvdl7Zs/ef/87EREREREREREREREREREREREREREREREREREREREREREREREREaktPz8/KSnp4sWLR44c2b1799q1a5ctWxYZGTl79uzp06eHh4ePGzduzJgxoaGhgwcP7t+/f58+ffr16zdo0KBhw4aNGjVq7NixkyZNioiImDlz5vz585csWbJ69eodO3bExMScPXv2zp07OTk56H+iJTMYDCkpKZcvXz5x4sT+/fujoqI+++yzhQsXzp079+OPP546deqECRPefffdsLCwIUOGvPnmm8HBwa+//vqAAQOGDh06YsSI9957b+LEidOmTfvkk08+/fTTRYsWrVixYvPmzQcOHDh16tT169fT09ONRiP6X0lEZI6ys7MvXboUHR2tHXi1Y+nAgQO7du3aokULLy8vR0fH/6/82dra1q5du1mzZh07dtSO8NohXTuM79mzJz4+PjU1Fb085q6goODPP/88dOjQhg0btBMe7aymZ8+eAQEB9evXd3FxqYDts7Kycnd3b9SoUWBgoPbXWfujPGvWrC1bthw/fvzu3bvFxcXoFSIiKl/aVcbNmzcPHjyoXT9qV53alWbLli1dXV0r4AhsCicnp6ZNm/bu3Vu7TNYunPft2/f7778XFRWhlxNAO9k4duzYpk2bZsyYoV1dBgUFeXh46HQ69BY9jV6vr1evXufOnbU/u9r17/bt20+fPq2d0aHXkohILjMz86efftL+mI4ZM6ZNmzba3yn0sbbMaBe8/v7+ISEhixcvjomJefDgAXqxy552WXrx4sVt27ZNmTKlW7dutWrVQq96WdL+5mqXup988slXX3119epVXuQSkTlLTEzcvXt3REREz549PT090UfQCuXu7t6pU6eJEydu3rz5ypUr6K2Q0K7pfvzxx8jIyEGDBvn5+VlbW6MXteLY29u/9NJLw4cPX758eVxc3JMnT9C7QUSVmnbOf+HCBe36VDsge3l5oY+RZsTNza13794LFy48efKkOR+rExISdu7c+f7772sX41ZWVuhlMxe2traBgYFTp06Njo7m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166ddo2zY8eOCntVelnJz88/ceLEggUL3nzzTe1kqRLeQK6dYDRv3nzIkCGrVq06f/68eZ4RERFpf22PHj362Wefade2L730kr29PfrwWcasra2bNm06aNCgyMjIgwcPWtjNLXl5eWfPnt20adPEiRM7d+5skV8EeHp69uzZMyIiYufOnb///ntRURF61YmISq24uPjatWtfffXVnDlztD+4nTp1qlu3rkLPsqhTp05gYKB2afPxxx9v27bt4sWLpr+YVS0PHjz48ccfly1bpl2e9+rVy8/Pz9HREb0tz8rFxaVFixb9+vWbNGnSmjVrfvrpp8zMTPSKEhGVF4PBcOfOnWPHjm3evHnmzJkhISFBQUENGzZ0c3ODfDhZtWpVHx+fgICAgQMHfvjhh+vXr4+Jibl+/fqTJ0/QS2WmUlJSzpw5s3v37oULF77zzjuvvfaadkVfq1YtyMM27e3ttQtSf3//Pn36jB8/fvny5fv379dOhB49eoReJyIic6Fd7aalpWkXvHFxcd9+++2mTZsWL148bdq0kSNHapeQb775ZnBwsHYw166CtYvKVq1aNWvWrFGjRtoVsXaZ6e3t7evr+8ILL7Rs2bJt27YdOnTo1q1b796933jjjcGDB2vXzpMnT46MjNywYcO+ffuOHz/+xx9/PHz4kD+yKFvZ2dm3b9+Oj4//4Ycftm/fvmLFik8++eTdd9/VTqIGDBjQt2/fHj16dOnSRTuhat26tfY3sUmTJtq5jYeHh5eXV4MGDbSrY+16UzvVadeuXdeuXXv27Klde2qnPdp/Pm7cuNmzZ69evXrXrl2HDh26cOFCQkJCXl4e+l9MREREREREREREREREREREREREREREREREREREREREREREREREREREREREREREREREREREREREREREREREREREREREREREREREREREREREREREREREREREREREREREREREREREREREZMn+Hwv4lKA=)
&]
[s2;%% produces shape with `"hole`" where circles overlap, by adding 
Div into path&]
[s2;%% [C1 sw.Circle(200, 200, 100).][*C1 Div()][C1 .Circle(270, 200, 100).EvenOdd().Fill(B
lack());]&]
[s2;%% 
@@image:449&333
(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BAAAtpkzZ466P4ulp6erkwAAwDZLlixR92cxi8ViMpnUYQAAYIP169er+/N3QUFB6jAAALDBjh071OX5u5YtW6rDAADABsnJyery/N2dd96pDgMAABukp6ery/N3999/vzoMAADKysPDw2KxqMvzd88884w6DwAAyqpZs2bq5rxi/vz56jwAACirXr16qZvzii+++EKdBwAAZfXII4+om/OK/fv3q/MAAKCsXn31VXVzXpGVlaXOAwCAslqxYoW6Of+gZs2a6kgAACiT7du3q2vzDzp06KCOBACAMjl9+rS6Nv/ggQceUEcCAEDpvLy8rFarujb/4Nlnn1WnAgBA6UJDQ9WdebW3335bnQoAAKXr06ePujOvtmXLFnUqAACU7qmnnlJ35tXOnTunTgUAgNItWrRI3ZklqFOnjjoYAABKkZSUpC7MEvTu3VsdDAAApcjJyVEXZgkmTZqkDgYAgOtp3ry5ui1LtmTJEnU2AABcT2RkpLotS/bjjz+qswEA4HpeeukldVuWLD8/v0qVKup4AAC4ptWrV6vb8ppat26tjgcAgGs6evSouiqvaejQoep4AAAoma+vr8ViUVflNcXFxakTAgCgZJ06dVL35PV888036oQAACjZhAkT1D15Pbm5uR4eHuqQAAAowcqVK9U9WYrw8HB1SAAAlMDRPnr7/zJeKatDAgDgak2bNlU3ZOmMV8rqnAAAuNrIkSPVDVk645WyOicAAK62cOFCdUOWifF6WR0VAAB/cODAAXU9lsmIESPUUQEAcEVgYKAj313hv7377rvqtAAAuKJv377qbiyr/fv3q9MCAOCKV199Vd2NZWW1WuvUqaMODACA3+3YsUPdjTYYNWqUOjAAAIrVrFnTWf4y9D8++eQTdWYAABQbNmyYuhVt89tvv5lMJnVsAADc9NFHH6lb0Wa33367OjYAAG46d+6cuhJtNmPGDHVsAAB3FxYWpu7D8ti1a5c6OQCAu4uJiVH3YXlYLJagoCB1eAAAt7Zt2zZ1H5ZTdHS0OjwAgPuqXr262WxWl2E5LV++XJ0fAMB9RUVFqZuw/NLT0z09PdURAgDc1IoVK9RNeEP69eunjhAA4I68vb2zs7PVNXhDlixZok4RAOCOIiMj1R14ozIyMqpWraoOEgDgdj799FN1B9rBwIED1UECANyLr69vTk6OugDtgHfnAgAq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)
&]
[s2;%% circles are treated as separate subshapes and simply overwrite 
each other.&]
[s2;%% This feature is especially important when drawing text with 
effects as it allows individual characters to be grouped to use 
single fill or stroke (glyphs and advance widths are often designed 
with possible overlaps).&]
[s3; &]
[s4; &]
[s5;:Upp`:`:Painter`:`:Close`(`): Painter[@(0.0.255) `&] [* Close]()&]
[s2;%% Adds line from the last point of path back to the first point 
(if they are not equal). Note that this is done automatically 
in any Fill call.&]
[s3; &]
[s4; &]
[s0; &]
[ {{10000F(128)G(128)@1 [s0;%% [* Painter context and attributes]]}}&]
[s3; &]
[s5;:Upp`:`:Painter`:`:ColorStop`(double`,const Upp`:`:RGBA`&`): Painter[@(0.0.255) `&] 
[* ColorStop]([@(0.0.255) double] [*@3 pos], [@(0.0.255) const] RGBA[@(0.0.255) `&] 
[*@3 color])&]
[s2;%% Adds additional gradient [%-*@3 color] at relative position 
[%-*@3 pos]. Position in fraction of distance between gradient 
starting and ending point as defined in Fill or Stroke.&]
[s3;%% &]
[s4; &]
[s5;:Upp`:`:Painter`:`:ClearStops`(`): Painter[@(0.0.255) `&] [* ClearStops]()&]
[s2;%% Removes all positions defined by ColorStop. As ColorStop is 
usually used as path specific attribute, this is really needed 
nor used.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:Painter`:`:Opacity`(double`): Painter[@(0.0.255) `&] [* Opacity]([@(0.0.255) do
uble] [*@3 o])&]
[s2;%% Sets painting opacity to [%-*@3 o], where 1 is fully opaque 
and 0 is fully transparent.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:Painter`:`:LineCap`(int`): Painter[@(0.0.255) `&] [* LineCap]([@(0.0.255) int] 
[*@3 linecap])&]
[s2;%% Sets line caps. [%-*@3 linecap] can be one of LINECAP`_BUTT, 
LINECAP`_SQUARE or LINECAP`_ROUND (LINECAP`_BUTT is default value):&]
[s2;%% 
@@image:2721&406
(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)
&]
[s3; &]
[s4; &]
[s5;:Upp`:`:Painter`:`:LineJoin`(int`): Painter[@(0.0.255) `&] [* LineJoin]([@(0.0.255) int
] [*@3 linejoin])&]
[s2;%% Sets the line joining mode. [%-*@3 linejoin] can be one of LINEJOIN`_MITER, 
LINEJOIN`_ROUND, LINEJOIN`_BEVEL (LINEJOIN`_MITER is default 
value):&]
[s2;%% 
@@image:2684&809
(A1sDAwEAAAAAAAAAAHic7d15lFtl/cfxWzLFUgFZMkChrNIMltWydGbAVkFEyBx22TRjWUVQEqjsyMHOWAVUDtgMwqmleNSMBxABEWemgAgCSkZkaQGRzAEsu5CAQIHS/L7Oc3h+lyw3d54sT5b366/2zr03T26eT+43yTc32SwAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAACAlvbfce+++67P9d955x1Z/+2333YvfO+999R+PvzwQ49tV69erVaTf+iFssl/fXNvWJAeifdqmUzmrrvuSiQSv/jFL+65557nnnvOe/0PPvig4AB8Hj110Pwf5Hz60Pncj7pFIf9wL3///fdzHqlVq1b5P/7uDf1vIg9KwTuSQybVmjVrjA9R4/L/sCpk0L3c59ErP4PeR8lg6j755JO///3vr7vuuptvvvnhhx9+6623/Ny6RNhjNRlGfuiy5R0l/WTiJg9HqfvXSMhgQ2RQDyCHdyiyrhOfH2oTeXD9HENFP6YqFzqGfrijpE/HpkeoXJ/85CcdxwmHwz7X33nnnWX9z3zmM+6F3/3ud51xp59+use2w8PDajX5h174t7/9zfHNvWFBMgC15muvvVZwhXvvvXf//fdva2vL2fOsWbPkmVkejoJb/fKXv1SryRO4XihP4GrhBhts8MILL3iMapdddpHVvvjFL3oP3sNvf/tbPdTtttvOe2WZ/BtuuKFa+bOf/az7Tz/84Q/V8n/84x9qyXHHHef/+At53lAb5h/DYi6++GI9ADn3eaw5adKkT33qU52dnRdccMF//vMf48PVWMigUucZ/Pvf/+59ZNZbbz2JWzQa9T6xymlRYrjNNtvkbD5lypR58+bJrXjf+mWXXeax81deeUWt9u1vf9t9i2rh+uuv/+9//9tjcxm/rPaFL3whf2E+OeYzZsyYM2eO3B3vg1//yKBS5xn89a9/XeyYyCMoD8rZZ5/94osv5m94ySWX+Dy2gUBAbXLQQQepJfLCreTAvve976mVR0dH5b//+te/fN6cGBwc1PuJRCJqYckSt0oqGwRx9913F9vWbhDkCJ9wwgnuva211lpSfriXyFPfs88+m79b7yCUPICVLQhFsbOGcvvtt7vvkftPdV4Qum288caPP/648RFrIGTQvaRuM1iyINSmTp161113FdzJI488stVWW7lXXnfddSdPnuxeIieX/PcbK1IQii9/+csem0+oIHSTYnjp0qUljmAdI4PuJXWbQY+CUJNXPfkH36Ag1Ofciy66qOTAtttuO1lzt912U/+lINT3Tl75FnvD0zsIvb29N5by8ssvew+vWBBWr179pS99Sf1Jnq4XLlz40EMPqY9T0+m0VCl77723+mswGHz00UdzdlsyCOL6668vNqqKF4TnnXeex8pf+9rX9JolC8K//vWvOQf5V7/6lVpn3333zX8IXn31VbWhKgj32muvko/a8uXL9QB0QXj55Zc//nFy2EdGRhYtWjRjxgy1jqSsyT6ZKogMNkQGdUl25pln5kzdxx57TM5BixcvnjVrllpngw02yH+L+8EHH5TyT62w++67S6jHxsay4x8XPvPMM/K6aaONNlJ/Peqoo4rdepkFoViyZEmxzT0Kws0228w9DWQnUrgef/zx+i57H/86RwYbIoO6IDznnHP00ZCFV1555fnnn9/T06PTl3OIdEEoa3of25tuukltIiXZJptsIptsu+223qO699571c4HBgbUEl0QHn744SUfzeeff17vqvkKQnHaaacV3NY7CD/5yU+M74VWLAhSQanlc+fOTaVSBbfV7/rKvM15OPwEQV5krVy5suCeK1gQ7rrrro7np8bvvvuuOulsvvnmjo+CsOAe1DonnXSSx5BUQSgTfkJ3RBeEv/nNb4qtI0WgPExqNferp2ZFBrV6zqDPkuyrX/2qWu3SSy91L5f6cMstt5TlkydPlldDBdvA5NSgHlxx9dVXG9y6n4Jw/fXXd5+D3DwKQnmZVuxGpQZQb3IGAoEVK1Z4DK9ukUGtnjOoC8Jbb7214Apy9NQK7o+lsq6C0M/nv9p3vvMdtdX999/vsdqJJ57ojLd8SFGtluiC8MILL/R/c9mmKwhVuSIKfmJiKwjy6EyaNMkZf0fXu6NVXvurzSUU7uXeQdD3+qCDDiq42woWhD/4wQ/UP4p9aiwvcNSQ5FndacyCUEgA1Wre3TjNgQy61W0GfZZkcjZUq/X09LiXx2IxtTyn0sshdWN7e7sz3hPlLtsqUhDqo3TAAQcU3NysIBTXXnut2vMRRxzhsVrdIoNudZvBkgVhJpORVyWywn777edeblYQyspqq29+85vF1pFwrbfeerKO1HJ6IQWhuhfyrLX2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)
&]
[s3; &]
[s4; &]
[s5;:Upp`:`:Painter`:`:MiterLimit`(double`): Painter[@(0.0.255) `&] 
[* MiterLimit]([@(0.0.255) double] [*@3 l])&]
[s2; Defines a limit on the ratio of the miter length to the stroke 
width. Default value is 4. Past this maximum value, join reverts 
to bevel:&]
[s2; 
@@image:2537&887
(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)
&]
[s3; &]
[s4; &]
[s5;:Upp`:`:Painter`:`:Dash`(const Vector`&`,double`): Painter[@(0.0.255) `&] 
[* Dash]([@(0.0.255) const] Vector<[@(0.0.255) double]>[@(0.0.255) `&] 
[*@3 dash], [@(0.0.255) double] [*@3 start])&]
[s5;:Upp`:`:Painter`:`:Dash`(const char`*`,double`): Painter[@(0.0.255) `&] 
[* Dash]([@(0.0.255) const] [@(0.0.255) char] [@(0.0.255) `*][*@3 dash], 
[@(0.0.255) double] [*@3 start] [@(0.0.255) `=] [@3 0])&]
[s2; [%% Defines ]the pattern of dashes and gaps used to stroke the 
path[%% . First value is the length of dash. If total number of 
values is odd, meaning of individual values alternates between 
dash and gap. For ][@(0.0.255) const] [@(0.0.255) char] [@(0.0.255) `*][*@3 dash][%% , 
values are defined by space separate list of numbers. ][*@3 start] 
can offset the starting position.&]
[s2;%% 
@@image:2984&421
(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)
&]
[s3; &]
[s4; &]
[s5;:Upp`:`:Painter`:`:EvenOdd`(bool`): Painter[@(0.0.255) `&] [* EvenOdd]([@(0.0.255) bool
] [*@3 evenodd] [@(0.0.255) `=] [@(0.0.255) true])&]
[s2;%% Sets the filling mode to either even`-odd rule or zero`-winding 
rule.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:Painter`:`:Invert`(bool`): Painter[@(0.0.255) `&] [* Invert]([@(0.0.255) bool] 
[*@3 b] [@(0.0.255) `=] [@(0.0.255) true])&]
[s2;%% Special mode where instead of filling with color or source 
spans, affected pixels have the color channels inverted (with 
binary NOT operation). This is occasionally useful for displaying 
cursors.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:Painter`:`:ImageFilter`(int`): Painter[@(0.0.255) `&] [* ImageFilter]([@(0.0.255) i
nt] [*@3 filter])&]
[s2;%% Sets the convolution [%-*@3 filter] used to rescale images. 
Can be one of FILTER`_NEAREST, FILTER`_BILINEAR, FILTER`_BSPLINE, 
FILTER`_COSTELLA, FILTER`_BICUBIC`_MITCHELL,&]
[s2;%% FILTER`_BICUBIC`_CATMULLROM, FILTER`_LANCZOS3.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:Painter`:`:Transform`(const Upp`:`:Xform2D`&`): Painter[@(0.0.255) `&] 
[* Transform]([@(0.0.255) const] Xform2D[@(0.0.255) `&] [*@3 m])&]
[s2;%% Multiplies transformation matrix by [%-*@3 m]. Cannot be used 
as path specific attribute change.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:Painter`:`:Translate`(double`,double`): Painter[@(0.0.255) `&] 
[* Translate]([@(0.0.255) double] [*@3 x], [@(0.0.255) double] [*@3 y])&]
[s5;:Upp`:`:Painter`:`:Translate`(const Upp`:`:Pointf`&`): Painter[@(0.0.255) `&] 
[* Translate]([@(0.0.255) const] Pointf[@(0.0.255) `&] [*@3 p])&]
[s2;%% Translates transformation matrix by [%-*@3 p] / [%-*@3 x] [%-*@3 y]. 
Cannot be used as path specific attribute change.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:Painter`:`:Rotate`(double`): Painter[@(0.0.255) `&] [* Rotate]([@(0.0.255) doub
le] [*@3 a])&]
[s2;%% Rotates transformation matrix by [%-*@3 a] radians. Cannot be 
used as path specific attribute change.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:Painter`:`:Scale`(double`,double`): Painter[@(0.0.255) `&] 
[* Scale]([@(0.0.255) double] [*@3 scalex], [@(0.0.255) double] [*@3 scaley])&]
[s2;%% Scales the transformation by [%-*@3 scalex], [%-*@3 scaley] factors. 
Cannot be used as path specific attribute change.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:Painter`:`:Scale`(double`): Painter[@(0.0.255) `&] [* Scale]([@(0.0.255) double
] [*@3 scale])&]
[s2;%% Same as Scale( [%-*@3 scale], [%-*@3 scale]).&]
[s3; &]
[s3;%% ]]