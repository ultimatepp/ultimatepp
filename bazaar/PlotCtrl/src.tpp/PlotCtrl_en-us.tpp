topic "PlotCtrl";
[2 $$0,0#00000000000000000000000000000000:Default]
[i448;a25;kKO9;2 $$1,0#37138531426314131252341829483380:class]
[l288;2 $$2,2#27521748481378242620020725143825:desc]
[0 $$3,0#96390100711032703541132217272105:end]
[H6;0 $$4,0#05600065144404261032431302351956:begin]
[i448;a25;kKO9;2 $$5,0#37138531426314131252341829483370:item]
[l288;a4;*@5;1 $$6,6#70004532496200323422659154056402:requirement]
[l288;i1121;b17;O9;~~~.1408;2 $$7,0#10431211400427159095818037425705:param]
[i448;b42;O9;2 $$8,8#61672508125594000341940100500538:tparam]
[b42;2 $$9,9#13035079074754324216151401829390:normal]
[{_} 
[ {{10000@(113.42.0) [s0;%% [*@7;4 PlotCtrl]]}}&]
[s1;@(0.0.255)3 &]
[s1;:PlotCtrl`:`:class: [@(0.0.255)3 class][3 _][*3 PlotCtrl][3 _:_][@(0.0.255)3 public][3 _][*@3;3 C
trl][3 , ][@(0.0.255)3 public][3 _][*@3;3 Plot]&]
[s2;%% Widget for interactive displaying of plots.&]
[s0;i448;a25;kKO9;:noref:@(0.0.255) &]
[ {{10000F(128)G(128)@1 [s0;%% [* Public Method List]]}}&]
[s5;_^PlotCtrl^ &]
[s5;:PlotCtrl`:`:Zoom`(double`,double`,Pointf`): [_^PlotCtrl^ PlotCtrl][@(0.0.255) `&]_[* Z
oom]([@(0.0.255) double]_[*@3 rx],[@(0.0.255) double]_[*@3 ry],[_^Pointf^ Pointf]_[*@3 C][@(0.0.255) `=
]Null)&]
[s2;%% Anisotropic zoom of the displayed area, with center of zooming 
in C. If C is Null, then the zooming occurs around the center 
of currently displayed area.&]
[s3;%% &]
[s4; &]
[s5;:PlotCtrl`:`:ZoomIn`(double`,const Pointf`&`): [_^PlotCtrl^ PlotCtrl][@(0.0.255) `&]_
[* ZoomIn]([@(0.0.255) double]_[*@3 ratio][@(0.0.255) `=][@3 2],[@(0.0.255) const]_[_^Pointf^ P
ointf][@(0.0.255) `&]_[*@3 C][@(0.0.255) `=]Null)&]
[s2;%% Shows [%-*@3 ratio] times smaller area. Same as Zoom(1/ratio,1/ratio,C)&]
[s3;%% &]
[s4; &]
[s5;:PlotCtrl`:`:ZoomOut`(double`,const Pointf`&`): [_^PlotCtrl^ PlotCtrl][@(0.0.255) `&]_
[* ZoomOut]([@(0.0.255) double]_[*@3 ratio][@(0.0.255) `=][@3 2],[@(0.0.255) const]_[_^Pointf^ P
ointf][@(0.0.255) `&]_[*@3 C][@(0.0.255) `=]Null)&]
[s2;%% Shows [%-*@3 ratio] times bigger area. Same as Zoom(ratio,ratio,C).&]
[s3;%% &]
[s4; &]
[s5;:PlotCtrl`:`:ZoomAll`(bool`): [_^PlotCtrl^ PlotCtrl][@(0.0.255) `&]_[* ZoomAll]([@(0.0.255) b
ool]_[*@3 visibleonly][@(0.0.255) `=false])&]
[s2;%% Sets the visible area to display all data points in Plot. 
If [%-*@3 visibleonly], then invisible data sets are ignored.&]
[s3;%% &]
[s4; &]
[s5;:PlotCtrl`:`:Layout`(`): [@(0.0.255) virtual] [@(0.0.255) void]_[* Layout]()&]
[s2;%% Called when the widget changes size.&]
[s3; &]
[s4; &]
[s5;:PlotCtrl`:`:Paint`(Draw`&`): [@(0.0.255) virtual] [@(0.0.255) void]_[* Paint]([_^Draw^ D
raw][@(0.0.255) `&]_[*@3 w])&]
[s2;%% Painting routine.&]
[s3;%% &]
[s4; &]
[s5;:PlotCtrl`:`:LeftDown`(Point`,dword`): [@(0.0.255) virtual] [@(0.0.255) void]_[* LeftDo
wn]([_^Point^ Point]_[*@3 pt],[_^dword^ dword]_[*@3 keyflags])&]
[s5;:PlotCtrl`:`:LeftUp`(Point`,dword`): [@(0.0.255) virtual] [@(0.0.255) void]_[* LeftUp](
[_^Point^ Point]_[*@3 pt],[_^dword^ dword]_[*@3 keyflags])&]
[s5;:PlotCtrl`:`:RightDown`(Point`,dword`): [@(0.0.255) virtual] [@(0.0.255) void]_[* Right
Down]([_^Point^ Point]_[*@3 pt],[_^dword^ dword]_[*@3 keyflags])&]
[s5;:PlotCtrl`:`:MouseMove`(Point`,dword`): [@(0.0.255) virtual] [@(0.0.255) void]_[* Mouse
Move]([_^Point^ Point]_[*@3 pt], [_^dword^ dword]_[*@3 keyflags])&]
[s5;:PlotCtrl`:`:MiddleDown`(Point`,dword`): [@(0.0.255) virtual] [@(0.0.255) void]_[* Midd
leDown]([_^Point^ Point]_[*@3 pt],[_^dword^ dword]_[*@3 keyflags])&]
[s5;:PlotCtrl`:`:MiddleUp`(Point`,dword`): [@(0.0.255) virtual] [@(0.0.255) void]_[* Middle
Up]([_^Point^ Point]_[*@3 pt],[_^dword^ dword]_[*@3 keyflags])&]
[s5;:PlotCtrl`:`:MouseLeave`(`): [@(0.0.255) virtual] [@(0.0.255) void]_[* MouseLeave]()&]
[s5;:PlotCtrl`:`:MouseWheel`(Point`,int`,dword`): [@(0.0.255) virtual] 
[@(0.0.255) void]_[* MouseWheel]([_^Point^ Point]_[*@3 pt], [@(0.0.255) int]_[*@3 zdelta], 
[_^dword^ dword]_[*@3 keyflags])&]
[s2;%% Mouse handling routines. Default implementation provides:&]
[ {{5000:5000<288;>736; [s2;l64;%% Zoom in]
:: [s2;l64;%% Left click OR Mouse wheel]
:: [s2;l64;%% Zoom out]
:: [s2;l64;%% Right click OR Mouse wheel]
:: [s2;l64;%% Zoom to selection]
:: [s2;l64;%% Left button drag]
:: [s2;l64;%% Zoom all]
:: [s2;l64;%% Ctrl `+ Left click]
:: [s2;l64;%% Panning]
:: [s2;l64;%% Shift `+ Left button drag]}}&]
[s3;%% &]
[s4; &]
[s5;:PlotCtrl`:`:CursorImage`(Point`,dword`): [@(0.0.255) virtual] 
[_^Image^ Image]_[* CursorImage]([_^Point^ Point]_[*@3 p], [_^dword^ dword]_[*@3 keyflags])&]
[s2;%% Returns cursor image to display in current situation.&]
[s3;%% &]
[s4; &]
[s5;:PlotCtrl`:`:WhenSync: [_^Callback^ Callback]_[* WhenSync]&]
[s2;%% Callback executed whenever the visual representation of Plot 
changes.&]
[s3;%% &]
[ {{10000F(128)G(128)@1 [s0;%% [* Constructor detail]]}}&]
[s5;* &]
[s5;:PlotCtrl`:`:PlotCtrl`(`): [* PlotCtrl]()&]
[s2;%% Constructs empty PlotCtrl.&]
[s0; ]