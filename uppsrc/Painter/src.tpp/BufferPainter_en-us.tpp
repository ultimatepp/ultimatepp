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
[ {{10000@(113.42.0) [s0;%% [*@7;4 Title]]}}&]
[s1;*3 &]
[s1;:Upp`:`:BufferPainter: [*3 BufferPainter][3  ][@(0.0.255)3 :][3  ][@(0.0.255)3 public][3  
Painter]&]
[s2;%% BufferPainter is software implementation of Painter interface 
which uses ImageBuffer as the painting target.&]
[s0;i448;a25;kKO9;:noref:@(0.0.255) &]
[ {{10000F(128)G(128)@1 [s0;%% [* Public Method List]]}}&]
[s3; &]
[s5;:Upp`:`:BufferPainter`:`:GetBuffer`(`): ImageBuffer[@(0.0.255) `&] 
[* GetBuffer]()&]
[s5;:Upp`:`:BufferPainter`:`:GetBuffer`(`)const: [@(0.0.255) const] 
ImageBuffer[@(0.0.255) `&] [* GetBuffer]() [@(0.0.255) const]&]
[s2;%% Returns current output ImageBuffer.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:BufferPainter`:`:Co`(bool`): BufferPainter[@(0.0.255) `&] 
[* Co]([@(0.0.255) bool] [*@3 b] [@(0.0.255) `=] [@(0.0.255) true])&]
[s2;%% Activates multithreaded optimizations. Default is off. Activating 
makes sense for larger outputs&]
[s3; &]
[s4; &]
[s5;:Upp`:`:BufferPainter`:`:PreClip`(bool`): BufferPainter[@(0.0.255) `&] 
[* PreClip]([@(0.0.255) bool] [*@3 b] [@(0.0.255) `=] [@(0.0.255) true])&]
[s2;%% If active, Painter tries to test whether actual path can intersect 
current output buffer and skips processing it completely if not. 
This can improve performance if rendering small part of very 
large drawing and actually sending everything to Painter anyway 
with transformations scaling and transforming content drawing 
to current ImageBuffer target. However, prechecking path has 
performance costs too so whether this option should be active 
or not depends on case by case basis. Default is off.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:BufferPainter`:`:ImageCache`(bool`): BufferPainter[@(0.0.255) `&] 
[* ImageCache]([@(0.0.255) bool] [*@3 b] [@(0.0.255) `=] [@(0.0.255) true])&]
[s5;:Upp`:`:BufferPainter`:`:NoImageCache`(`): BufferPainter[@(0.0.255) `&] 
[* NoImageCache]()&]
[s2;%% Can be used to deactivate caching of image scaling data. Default 
of on. Probably best left on.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:BufferPainter`:`:Create`(Upp`:`:ImageBuffer`&`,int`): [@(0.0.255) void] 
[* Create](ImageBuffer[@(0.0.255) `&] [*@3 ib], [@(0.0.255) int] [*@3 mode 
][@(0.0.255) `=] MODE`_ANTIALIASED)&]
[s0;l288;%% Initializes Painter to target [%-*@3 ib]. [%-*@3 mode ]can 
be MODE`_ANTIALIASED (normal antialising, usually the best), 
MODE`_NOAA (not recommened, obsolete), MODE`_SUBPIXEL (improved 
horizontal resolution by subpixel rendering, slower). Note that 
it for performance reasons, it makes sense to reuse BufferPainter 
instance, especially if using multithreaded mode (Co) `- it saves 
time for internal buffer allocations.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:BufferPainter`:`:Finish`(`): [@(0.0.255) void] [* Finish]()&]
[s2;%% Makes sure that all scheduled painting operations are finished. 
Called by destructor&]
[s3;%% &]
[s0;%% &]
[ {{10000F(128)G(128)@1 [s0;%% [* Constructor detail]]}}&]
[s3; &]
[s5;:Upp`:`:BufferPainter`:`:BufferPainter`(Upp`:`:ImageBuffer`&`,int`): [* BufferPaint
er](ImageBuffer[@(0.0.255) `&] [*@3 ib], [@(0.0.255) int] [*@3 mode ][@(0.0.255) `=] 
MODE`_ANTIALIASED)&]
[s2; [%% Initializes BufferPainter and calls Create(][*@3 ib], [*@3 mode]).&]
[s3; &]
[s4; &]
[s5;:Upp`:`:BufferPainter`:`:BufferPainter`(Upp`:`:PainterTarget`&`,double`): [* Buffer
Painter](PainterTarget[@(0.0.255) `&] [*@3 t], [@(0.0.255) double] 
tolerance [@(0.0.255) `=] [*@3 Null])&]
[s2;%% Special mode where rendering is targeted to PainterTarget. 
Experimental.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:BufferPainter`:`:BufferPainter`(`): [* BufferPainter]()&]
[s2;%% Default constructor. You are supposed to call Create to assign 
an ImageBuffer.&]
[s3; &]
[s0;%% ]]