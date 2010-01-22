topic "RepGen - Reference";
[ $$0,0#00000000000000000000000000000000:Default]
[H6;0 $$1,0#05600065144404261032431302351956:begin]
[i448;a25;kKO9;2 $$2,0#37138531426314131252341829483370:codeitem]
[l288;2 $$3,0#27521748481378242620020725143825:desc]
[0 $$4,0#96390100711032703541132217272105:end]
[i448;a25;kKO9;2 $$5,0#37138531426314131252341829483380:class]
[b42;2 $$6,6#13035079074754324216151401829390:normal]
[l288;C@(0.0.255)1 $$7,7#50527422365114151273124311222011:code]
[{_} 
[ {{10000f0;t/25b/25@(113.42.0) [s0; [*@2;4 RepGen `- Very simple Report Generator]]}}&]
[s0;%% &]
[s5;:RepGen`:`:class: [@(0.0.255)3 class][3 _][*3 RepGen][3 _:_][@(0.0.255)3 public][3 _][*@3;3 Ctrl
]&]
[s3;%% &]
[s3;l0;%% RepGen `- Simple Report Generation Class&]
[s0;l416; [2 Supports the construction of simple printed forms. Based 
on the format QTF.]&]
[s0; &]
[ {{10000F(128)G(128)@1 [s0; [* Principle of operation].]}}&]
[s0;2 &]
[s0;l416; [2 RepGen looks QTF`-pattern and looks for the occurrence 
of the special variable substitution. In addition, you can loop 
through the records document, inside the loop is also visible 
in the substitution of only those variables that are listed inside 
the loop. (See ][^topic`:`/`/RepGen`/srcdoc`/RepGen`$en`-us^2 Tutorial][2 )]&]
[s0;2 &]
[ {{10000F(128)G(128)@1 [s0;%% [* Public Method List]]}}&]
[s0;%% &]
[s0;%% &]
[s1; &]
[s2;:RepGen`:`:SetTemplate`(String`): [@(0.0.255) void]_[* SetTemplate]([_^String^ String]_
[*@3 tpl])&]
[s3;%% Set QTF template [%-*@3 tpl].to Report. &]
[s3;%% This must be done at the beginning of this report.&]
[s4;%% &]
[s1; &]
[s2;:RepGen`:`:RepGenReportStart: [_^Callback^ Callback]_[* RepGenReportStart]&]
[s3;%% This callback is invoked at the [* beginning of report processing]. 
Create a function for this purpose and set callback using the 
operator &]
[s3;%% [C@(0.0.255)1 rep.RepGenReportStart  `= callback(ClentCallbackRepStart);]&]
[s4; &]
[s1; &]
[s2;:RepGen`:`:RepGenReportVar: [_^Callback^ Callback]_[* RepGenReportVar]&]
[s3;%% This callback is invoked during the processing [* static] variables 
of the report ([* in the middle between the start and finish, but 
not in the cycle of processing lines in the document]).&]
[s3;%% Create a function for this purpose and follow the callback 
using the operator&]
[s0;l288;%% [C@(0.0.255)1 rep.RepGenReportVar  `= callback(ClentCallbackRepVar);]&]
[s4; &]
[s1; &]
[s2;:RepGen`:`:RepGenReportFinish: [_^Callback^ Callback]_[* RepGenReportFinish]&]
[s3;%% This callback is invoked at the [* end of report processing]. 
Create a function for this purpose and set callback using the 
operator &]
[s3;%% [C@(0.0.255)1 rep.RepGenReportFinish  `= callback(ClentCallbackRepFinish);]&]
[s4; &]
[s1; &]
[s2;:RepGen`:`:RepGenCalculateStart: [_^Callback^ Callback]_[* RepGenCalculateStart]&]
[s3;%% This callback is invoked at the [* beginning of the ][*@(0.0.255) document 
processing][@(0.0.255)  ][*@(0.0.255) cycle] of report.&]
[s3;%%  &]
[s3;%% [*@3 Here are reset counters before the new cycle.]&]
[s3;%%  &]
[s3;%% Create a function for this purpose and follow the callback 
using the operator&]
[s7;%% rep.RepGenCalculateStart  `= callback(ClentCallbackCalcStart);&]
[s4; &]
[s1; &]
[s2;:RepGen`:`:RepGenCalculateBody: [_^Callback^ Callback]_[* RepGenCalculateBody]&]
[s3;%% This callback is invoked at the [* middle of the ][*@(0.0.255) document 
processing cycle] of report.&]
[s3;%%  &]
[s3;%% [*@3 Here is the substitution of variables, insert images, the 
calculation results and counters.]&]
[s3;%% [*@3  ]&]
[s3;%% [*@3 Also, there need to test at the end of a cycle of processing 
of the document and in this case to call ][*^topic`:`/`/RepGen`/src`/RepGen`$en`-us`#RepGen`:`:LoopDone`(bool`)^@(0.0.255) L
oopDone();][*@3  ]&]
[s3;%%  &]
[s3;%% Create a function for this purpose and follow the callback 
using the operator&]
[s7;%% rep.RepGenCalculateBody  `= callback(ClentCallbackCalcBody);&]
[s4; &]
[s1; &]
[s2;:RepGen`:`:RepGenCalculateFinish: [_^Callback^ Callback]_[* RepGenCalculateFinish]&]
[s3;%% This callback is invoked at the [* end of the ][*@(0.0.255) document 
processing cycle] of report.&]
[s3;%%  &]
[s3;%% [*@3 Here is the substitution of variables totals and counters.]&]
[s3;%%  &]
[s3;%% Create a function for this purpose and follow the callback 
using the operator&]
[s7;%% rep.RepGenCalculateFinish  `= callback(ClentCallbackCalcFinish);&]
[s4; &]
[s1; &]
[s2;:RepGen`:`:SubstVar`(String`,String`): [@(0.0.255) void]_[* SubstVar]([_^String^ String
]_[*@3 s`_from],[_^String^ String]_[*@3 s`_to])&]
[s3;%% Provides a one`-time substitution variable [%-*@3 s`_from] to 
the expression [%-*@3 s`_to].&]
[s0;l288;%% &]
[s0;l288;%% [*2 Examples:]&]
[s0;l288;%% [C@(0.0.255)1 rep.SubstVar(][C@3;1 `"##BANKCORRFROM`"][C@(0.0.255)1 ,][C@3;1 `"Na
tional `&Program Bank`"][C@(0.0.255)1 );-|][/C@4;1 // ][/@4;1 Substitution 
of static text]&]
[s0;l288;C1%% &]
[s0;l288;%% [C@(0.0.255)1 rep.SubstVar(][C@3;1 `"##TOTAL`"][C@(0.0.255)1 ,Format(][C@3;1 `"%``
`"][C@(0.0.255)1 ,TOTAL));-|-|-|][/C@4;1 // ][/@4;1 Substitution  of the 
estimated variable]&]
[s0;l288;C1%% &]
[s0;l288;%% [*2 Note][2  that the substitution occurs one time each cycle.]&]
[s4;%% &]
[s1; &]
[s2;:RepGen`:`:PlaceImage`(String`,Image`,Size`): [@(0.0.255) void]_[* PlaceImage]_([_^String^ S
tring]_[*@3 s`_from], [_^Image^ Image]_[*@3 im`_to], [_^Size^ Size]_[*@3 rep`_place]_`=_[_^Size^ S
ize]([@3 0],[@3 0]))&]
[s3;%% Provides a one`-time change of variable [%-*@3 s`_from] to qtf`-image 
[%-*@3 im`_to] size [%-*@3 rep`_place] (in the QTF`-conditioned units).&]
[s0;l288;*2%% &]
[s0;l288;%% [*2 Examples:]&]
[s0;l288;%% [C@(0.0.255)1 pictaddr `= ][C@3;1 `"/MyApps/RepGenTest/`"][C@(0.0.255)1 `+pict;
 -|-|][/@4;1 //Define real name of Image file]&]
[s0;l288;%% [C@(0.0.255)1 Image im `= StreamRaster`::LoadFileAny(pictaddr);][/@4;1 //Read 
image to Image var]&]
[s0;l288;%% [C@(0.0.255)1 rep.PlaceImage( ][C@3;1 `"##IMAGE`"][C@(0.0.255)1 , 
im, Size(1100,700) );][/@4;1 //Substitution ##IMAGE by image im 
with size 1100x700 units]&]
[s0;l288;*2%% &]
[s0;l288;*2%% &]
[s4;%% &]
[s1; &]
[s2;:RepGen`:`:LoopDone`(bool`): [@(0.0.255) void]_[* LoopDone]([@(0.0.255) bool]_[*@3 indica
tor][@(0.0.255) `=true])&]
[s3;%% Indicates the end of the processing cycle of the document.&]
[s0;%% &]
[s0;l288;%% [2 Must be called in the client Callback at the time it 
reaches the end of the document.]&]
[s4;%% &]
[s1; &]
[s2;:RepGen`:`:Perform`(`): [@(0.0.255) void]_[* Perform]()&]
[s3;%% Performance report and [* call the preview window and print 
the report].&]
[s4; &]
[s0; ]