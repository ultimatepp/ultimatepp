topic "PlotCtrl";
[2 $$0,0#00000000000000000000000000000000:Default]
[l288;i1120;a17;O9;~~~.1408;2 $$1,0#10431211400427159095818037425705:param]
[a83;*R6 $$2,5#31310162474203024125188417583966:caption]
[H4;b83;*4 $$3,5#07864147445237544204411237157677:title]
[i288;O9;C2 $$4,6#40027414424643823182269349404212:item]
[b42;a42;2 $$5,5#45413000475342174754091244180557:text]
[l288;b17;a17;2 $$6,6#27521748481378242620020725143825:desc]
[l321;C@5;1 $$7,7#20902679421464641399138805415013:code]
[b2503;2 $$8,0#65142375456100023862071332075487:separator]
[*@(0.0.255)2 $$9,0#83433469410354161042741608181528:base]
[C2 $$10,0#37138531426314131251341829483380:class]
[l288;a17;*1 $$11,11#70004532496200323422659154056402:requirement]
[i417;b42;a42;O9;~~~.416;2 $$12,12#10566046415157235020018451313112:tparam]
[b167;C2 $$13,13#92430459443460461911108080531343:item1]
[i288;a42;O9;C2 $$14,14#77422149456609303542238260500223:item2]
[*@2$(0.128.128)2 $$15,15#34511555403152284025741354420178:NewsDate]
[l321;*C$7;2 $$16,16#03451589433145915344929335295360:result]
[l321;b83;a83;*C$7;2 $$17,17#07531550463529505371228428965313:result`-line]
[l160;*C+117 $$18,5#88603949442205825958800053222425:package`-title]
[2 $$19,0#53580023442335529039900623488521:gap]
[C2 $$20,20#70211524482531209251820423858195:class`-nested]
[b50;2 $$21,21#03324558446220344731010354752573:Par]
[{_} 
[s2; PlotCtrl&]
[s0; &]
[s0; PlotCtrl package aims to provide simple to use widget for displaying 
graphs. It is based on [^topic`:`/`/PlotLib`/srcdoc`/PlotLib`$en`-us^ PlotLib] 
and additionally provides means for user interaction (mouse, 
keyboard shortcuts) and GUI dialogs for common tasks, such as 
setting the visual appearance of the graph.&]
[s0;# &]
[s0;# The main target for this widget are static graphs, mainly for 
visualization of scientific XY data and similar. It is not very 
useful for animated graphs, as the repainting (even though it 
is optimized) is not fast enough.&]
[s0;# &]
[s0; [@6 WARNING:] If you try this package and the result feels slow 
or you experience lags in painting, please make sure you try 
the same in Optimal mode. The speed difference between Debug 
and Optimal is very significant.&]
[s0; &]
[s0; The documentation to the classes provided by this package can 
be found here:&]
[s0;i150;O0; [^topic`:`/`/PlotCtrl`/src`/PlotCtrl`$en`-us^ PlotCtrl]&]
[s0;i150;O0; [^topic`:`/`/PlotCtrl`/src`/LegendCtrl`$en`-us^ LegendCtrl]&]
[s0;i150;O0; [^topic`:`/`/PlotCtrl`/src`/PlotStyleDlg`$en`-us^ PlotStyleDlg]&]
[s0; ]