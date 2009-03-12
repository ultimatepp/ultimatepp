topic "StatusBar and related widgets";
[2 $$0,0#00000000000000000000000000000000:Default]
[i448;a25;kKO9;2 $$1,0#37138531426314131252341829483380:class]
[l288;2 $$2,0#27521748481378242620020725143825:desc]
[0 $$3,0#96390100711032703541132217272105:end]
[H6;0 $$4,0#05600065144404261032431302351956:begin]
[i448;a25;kKO9;2 $$5,0#37138531426314131252341829483370:item]
[l288;a4;*@5;1 $$6,6#70004532496200323422659154056402:requirement]
[l288;i1121;b17;O9;~~~.1408;2 $$7,0#10431211400427159095818037425705:param]
[i448;b42;O9;2 $$8,8#61672508125594000341940100500538:tparam]
[b42;2 $$9,9#13035079074754324216151401829390:normal]
[{_}%EN-US 
[s0; [*+150 StatusBar related classes and routines]&]
[s3;%- &]
[ {{10000@3 [s0; [*@(229)4 InfoCtrl]]}}&]
[s3; &]
[s1;:InfoCtrl`:`:class:%- [@(0.0.255)3 class][3 _][*3 InfoCtrl][3 _:_][@(0.0.255)3 public][3 _][*@3;3 F
rameLR][@(0.0.255)3 <][_^Ctrl^3 Ctrl][@(0.0.255)3 >][3 _]&]
[s0; &]
[s2; 
@@image:643&143
€ƒç€—€€€ƒéÿ€€€€ø¯œíÕÍ‰€°€Œ…à§öÇ¨ğƒâ¼® âô‡Š„“¶±¦¶í£¼ğ°’Êÿ¾©ºã”÷ˆÒªë²£¥éøï¹Ú÷—
àç¼ÀƒšĞÌ¸†èµĞ†«È¹‡ ÍÂğ¹€¥£èüËüÿç‰ÁŸÅÈ³ñ¤ã„­¶™Ú¾è±µä¾èÅÁ«Ğ†­øõ¬ˆ½¢ÉğâÙ¶ÚôÖÄÔĞ
ıªáÓ˜ö¢šâ‘ÒŸıñîâé´—óëƒ×İ•ÙåøÖÁ°ã‡šĞ§€ö¯ğ´’’”¸Êêœ¶¤Œ·§¶©Ø­
&]
[s0; &]
[s9; InfoCtrl is a simple static Ctrl that represents information 
field. It is supposed to be used as Frame of StatusBar `- multiple 
InfoCtrls can be added as Frames to the StatusBar, filling it 
from the left or right.&]
[s9; InfoCtrl is capable of displaying several sections with defined 
width, defined either as Value or as PaintRect. It is also able 
to display temporary content for the defined amount of time.&]
[s0; &]
[s0;%- [%%/ Derived from][%%  ]FrameLR<Ctrl>&]
[s3; &]
[s0;%- &]
[ {{10000F(128)G(128)@1 [s0; [* Constructor Detail]]}}&]
[s3;%- &]
[s5;:InfoCtrl`:`:InfoCtrl`(`):%- [* InfoCtrl]()&]
[s2; Constructor. Sets ThinInsetFrame as Frame 0 and `"Ready`" as 
the default text.&]
[s3; &]
[s0; &]
[ {{10000F(128)G(128)@1 [s0; [* Public Method List]]}}&]
[s3;%- &]
[s5;:InfoCtrl`:`:Set`(int`,const PaintRect`&`,int`):%- [@(0.0.255) void]_[* Set]([@(0.0.255) i
nt]_[*@3 tab], [@(0.0.255) const]_[_^PaintRect^ PaintRect][@(0.0.255) `&]_[*@3 info], 
[@(0.0.255) int]_[*@3 width])&]
[s5;:InfoCtrl`:`:Set`(int`,const Value`&`,int`):%- [@(0.0.255) void]_[* Set]([@(0.0.255) in
t]_[*@3 tab], [@(0.0.255) const]_[_^Value^ Value][@(0.0.255) `&]_[*@3 info], 
[@(0.0.255) int]_[*@3 width])&]
[s2; Sets (creates if needed) the section of the InfoCtrl.&]
[s7; [%-*C@3 tab]-|Index of section.&]
[s7; [%-*C@3 info]-|Content of section. Value content is converted to 
the text using AsString.&]
[s7; [%-*C@3 width]-|Width of section in pixels. Negative value indicates 
that section should be placed given number of pixels from the 
right border of the InfoCtrl.&]
[s3; &]
[s4;%- &]
[s5;:InfoCtrl`:`:Set`(const PaintRect`&`):%- [@(0.0.255) void]_[* Set]([@(0.0.255) const]_[_^PaintRect^ P
aintRect][@(0.0.255) `&]_[*@3 info])&]
[s5;:InfoCtrl`:`:Set`(const Value`&`):%- [@(0.0.255) void]_[* Set]([@(0.0.255) const]_[_^Value^ V
alue][@(0.0.255) `&]_[*@3 info])&]
[s2; Sets the single section that covers the whole InfoCtrl.&]
[s7; [%-*C@3 info]-|Content of section.&]
[s3; &]
[s4;%- &]
[s5;:InfoCtrl`:`:Temporary`(const PaintRect`&`,int`):%- [@(0.0.255) void]_[* Temporary]([@(0.0.255) c
onst]_[_^PaintRect^ PaintRect][@(0.0.255) `&]_[*@3 info], [@(0.0.255) int]_[*@3 timeoout]_`=
_[@3 2000])&]
[s5;:InfoCtrl`:`:Temporary`(const Value`&`,int`):%- [@(0.0.255) void]_[* Temporary]([@(0.0.255) c
onst]_[_^Value^ Value][@(0.0.255) `&]_[*@3 info], [@(0.0.255) int]_[*@3 timeout]_`=_[@3 2000])
&]
[s2; Sets the temporary content, displayed instead of defined sections.&]
[s7; [%-*C@3 info]-|Content.&]
[s7; [%-*C@3 timeoout]-|Timespan of temporary content.&]
[s3; &]
[s4;%- &]
[s5;:InfoCtrl`:`:EndTemporary`(`):%- [@(0.0.255) void]_[* EndTemporary]()&]
[s2; Immediately stops displaying temporary content.&]
[s3; &]
[s4;%- &]
[s5;:InfoCtrl`:`:GetTabCount`(`)const:%- [@(0.0.255) int]_[* GetTabCount]()_[@(0.0.255) con
st]&]
[s7; [*/ Return value]-|The number of sections.&]
[s3; &]
[s4;%- &]
[s5;:InfoCtrl`:`:operator`=`(const String`&`):%- [@(0.0.255) void]_[* operator`=]([@(0.0.255) c
onst]_[_^String^ String][@(0.0.255) `&]_[*@3 s])&]
[s2; Same as Set(s).&]
[s7; [%-*C@3 s]-|Content of single section covering the whole InfoCtrl.&]
[s3; &]
[s4;%- &]
[s5;:InfoCtrl`:`:SetDefault`(const String`&`):%- [_^InfoCtrl^ InfoCtrl][@(0.0.255) `&]_[* S
etDefault]([@(0.0.255) const]_[_^String^ String][@(0.0.255) `&]_[*@3 d])&]
[s2; Sets the default text `- this value is used instead of Null 
values passed to the Set method.&]
[s7; [%-*C@3 d]-|Default text.&]
[s7; [*/ Return value]-|`*this for chaining.&]
[s3; &]
[s4;%- &]
[s5;:InfoCtrl`:`:Left`(int`):%- [_^InfoCtrl^ InfoCtrl][@(0.0.255) `&]_[* Left]([@(0.0.255) in
t]_[*@3 w])&]
[s2; Sets up InfoCtrl to be placed [* left] when added as Frame with 
defined width.&]
[s7; [%-*C@3 w]-|Width of InfoCtrl.&]
[s7; [*/ Return value]-|`*this for chaining.&]
[s3; &]
[s4;%- &]
[s5;:InfoCtrl`:`:Right`(int`):%- [_^InfoCtrl^ InfoCtrl][@(0.0.255) `&]_[* Right]([@(0.0.255) i
nt]_[*@3 w])&]
[s2; Sets up InfoCtrl to be placed [* right] when added as Frame with 
defined width.&]
[s7; [%-*C@3 w]-|Width of InfoCtrl.&]
[s7; [*/ Return value]-|`*this for chaining.&]
[s3; &]
[s0; &]
[s0; &]
[s0;%- &]
[ {{10000@3 [s0; [*@(229)4 StatusBar]]}}&]
[s3; &]
[s1;:StatusBar`:`:class:%- [@(0.0.255)3 class][3 _][*3 StatusBar][3 _:_][@(0.0.255)3 public][3 _][*@3;3 I
nfoCtrl]&]
[s0; &]
[s2; 
@@image:1268&131
‚ƒË€•€€€ƒëÿ€€€€ø·œí™½Ã ˆÆıÿ›ì™«¥ÙâËÄÊÑ«¶Ø¦éÖâ¤Î‘É¢Œó“€°ŒØŸúÅääÛ†¾¼à”„£ÿß€À›Öõ
”õŸÃĞüüÿ™œàÔÂ°÷±Œ„«–‚©À°š é˜Š„«‚©•¸‹¶¬‹ûˆéÅÀ°•šÁ–©¨•à°‚åİ£È®ÔÇåÉÏöÒ¨ß‹•
‚ñó¬ç®ö¶Á²ÑĞï½·Å¬©ËÉçÆ‘ıšé¡˜å…¦¥»ŸïˆÅä¡Èº£¹®ú·¸·ÉãèƒÅ÷°Ûåü»œèÏÃê²å¦¸Úäª«†Š–ç
¾¬«Î‚É‡õö·¬‚İó‚É¬Û“—¶àæÄ˜ö…¢˜×Ñåÿß©ÆÜÁ¶Ç­¸Ì²ë©Ñ…Ï®˜Úñõ“œÌøù°‚åŸü¶‘ö¨ç²è«°Å¾½‡½
øô÷„“ìÏ«»‚­†‹ï¥‰ç¦÷ÅåÏú•Ä‚‰ó¤‚¹ì¸†¡Øá¶¤İ„»Şâ÷¹šìß¦³¦«à‡Öƒ‚³“ñ­ì¶¯ì¸ÿÕ´µ
€Ì¥ö¦Û÷‚…¾†ƒÌ¥Ş•ì½îêÙƒ§ÍÈŠæèÆ°ºŸ÷¿ï‘¢¹¿½ÍôÇœ
&]
[s0; &]
[s9; StatusBar is a Frame Ctrl intended to be used as informational 
bar at the bottom of the main application window.&]
[s9; StatusBar is inherited from InfoCtrl and retains all the functionality 
of the base class. It adds `"size`-grip`" for resizing of the 
window (if supported by the platform) and changes its Frame behaviour 
to be placed at the bottom of its parent window. To add more 
fields to the StatusBar, use InfoCtrl widgets.&]
[s0; &]
[s0;%- [%%/ Derived from][%%  ][^topic`:`/`/CtrlLib`/src`/StatusBar`$en`-us`#InfoCtrl`:`:class^ I
nfoCtrl]&]
[s3; &]
[s0;%- &]
[ {{10000F(128)G(128)@1 [s0; [* Constructor Detail]]}}&]
[s3;%- &]
[s5;:StatusBar`:`:StatusBar`(`):%- [* StatusBar]()&]
[s2; Constructor. Sets the height based on the current system default 
font. If there is no global WhenHelpDefault() callback defined 
for the Bar, defines it so that Bar will use StatusBar to display 
Menu and ToolBar informational texts.&]
[s3; &]
[s4;%- &]
[s5;:StatusBar`:`:`~StatusBar`(`):%- [@(0.0.255) `~][* StatusBar]()&]
[s2; Default destructor.&]
[s3;%- &]
[s0; &]
[s0;%- &]
[ {{10000F(128)G(128)@1 [s0; [* Public Method List]]}}&]
[s3;%- &]
[s5;:StatusBar`:`:operator`=`(const String`&`):%- [@(0.0.255) virtual] 
[@(0.0.255) void]_[* operator`=]([@(0.0.255) const]_[_^String^ String][@(0.0.255) `&]_[*@3 s])
&]
[s2; Same as Set(s).&]
[s7; [%-*C@3 s]-|Content of StatusBar.&]
[s3; &]
[s4;%- &]
[s5;:StatusBar`:`:operator Callback1`<const String`&`>`(`):%- [* operator_Callback1<con
st_String`&>]()&]
[s7; [*/ Return value]-|A callback that, when invoked, calls Set for 
the StatusBar.&]
[s3; &]
[s4;%- &]
[s5;:StatusBar`:`:Height`(int`):%- [_^StatusBar^ StatusBar][@(0.0.255) `&]_[* Height]([@(0.0.255) i
nt]_[*@3 `_cy])&]
[s2; Sets the height of the StatusBar.&]
[s7; [%-*C@3 `_cy]-|The height.&]
[s7; [*/ Return value]-|`*this for chaining.&]
[s3; &]
[s4;%- &]
[s5;:StatusBar`:`:NoSizeGrip`(`):%- [_^StatusBar^ StatusBar][@(0.0.255) `&]_[* NoSizeGrip](
)&]
[s2; Do not display `"size grip`".&]
[s7; [*/ Return value]-|`*this for chaining.&]
[s3; &]
[s0; &]
[s0; &]
[s0;%- &]
[ {{10000@3 [s0; [*@(229)4 ProgressInfo]]}}&]
[s3; &]
[s1;:ProgressInfo`:`:class:%- [@(0.0.255)3 class][3 _][*3 ProgressInfo]&]
[s9; This class provides simple tool for displaying progress information 
in the StatusBar (or generally, in InfoCtrl). It connects to 
InfoCtrl object and replaces its content with progress indicator.&]
[s3; &]
[s0;%- &]
[ {{10000F(128)G(128)@1 [s0; [* Constructor Detail]]}}&]
[s3;%- &]
[s5;:ProgressInfo`:`:ProgressInfo`(`):%- [* ProgressInfo]()&]
[s2; Constructor.&]
[s3; &]
[s4;%- &]
[s5;:ProgressInfo`:`:ProgressInfo`(InfoCtrl`&`):%- [* ProgressInfo]([_^InfoCtrl^ InfoCtrl
][@(0.0.255) `&]_[*@3 f])&]
[s2; Constructor.&]
[s7; [%-*C@3 f]-|Target InfoCtrl.&]
[s3; &]
[s4;%- &]
[s5;:ProgressInfo`:`:`~ProgressInfo`(`):%- [@(0.0.255) `~][* ProgressInfo]()&]
[s2; Destructor. Sets the Null to target InfoCtrl.&]
[s3; &]
[s0;%- &]
[ {{10000F(128)G(128)@1 [s0; [* Public Method List]]}}&]
[s3;%- &]
[s5;:ProgressInfo`:`:Text`(const String`&`):%- [_^ProgressInfo^ ProgressInfo][@(0.0.255) `&
]_[* Text]([@(0.0.255) const]_[_^String^ String][@(0.0.255) `&]_[*@3 s])&]
[s2; Sets the text to be displayed as the label of progress indicator.&]
[s7; [%-*C@3 s]-|Text.&]
[s7; [*/ Return value]-|`*this for chaining.&]
[s3; &]
[s4;%- &]
[s5;:ProgressInfo`:`:TextWidth`(int`):%- [_^ProgressInfo^ ProgressInfo][@(0.0.255) `&]_[* T
extWidth]([@(0.0.255) int]_[*@3 cx])&]
[s2; Sets the width of text. Zero means that the width has to be 
determined based on text.&]
[s7; [%-*C@3 cx]-|Width in pixels.&]
[s7; [*/ Return value]-|`*this for chaining.&]
[s3; &]
[s4;%- &]
[s5;:ProgressInfo`:`:Width`(int`):%- [_^ProgressInfo^ ProgressInfo][@(0.0.255) `&]_[* Width
]([@(0.0.255) int]_[*@3 `_cx])&]
[s2; Sets the width of progress indicator. Default is 200.&]
[s7; [%-*C@3 `_cx]-|Width in pixels.&]
[s7; [*/ Return value]-|`*this for chaining.&]
[s3; &]
[s4;%- &]
[s5;:ProgressInfo`:`:Placement`(int`):%- [_^ProgressInfo^ ProgressInfo][@(0.0.255) `&]_[* P
lacement]([@(0.0.255) int]_[*@3 `_tabi])&]
[s2; Can be used to place progress indicator to specific section 
of InfoCtrl.&]
[s7; [%-*C@3 `_tabi]-|Section index. Progress label is placed at [@3 `_tabi], 
progress indicator at [@3 `_tabi] `+ 1.&]
[s7; [*/ Return value]-|`*this for chaining.&]
[s3; &]
[s4;%- &]
[s5;:ProgressInfo`:`:Info`(InfoCtrl`&`):%- [_^ProgressInfo^ ProgressInfo][@(0.0.255) `&]_
[* Info]([_^InfoCtrl^ InfoCtrl][@(0.0.255) `&]_[*@3 `_info])&]
[s2; Specifies the target InfoCtrl.&]
[s7; [%-*C@3 `_info]-|InfoCtrl. Must exists during the lifetime of ProgressInfo.&]
[s7; [*/ Return value]-|`*this for chaining.&]
[s3; &]
[s4;%- &]
[s5;:ProgressInfo`:`:Total`(int`):%- [_^ProgressInfo^ ProgressInfo][@(0.0.255) `&]_[* Total
]([@(0.0.255) int]_[*@3 `_total])&]
[s2; Sets the total number of progress steps.&]
[s7; [%-*C@3 `_total]-|Total number.&]
[s7; [*/ Return value]-|`*this for chaining.&]
[s3; &]
[s4;%- &]
[s5;:ProgressInfo`:`:Set`(int`,int`):%- [_^ProgressInfo^ ProgressInfo][@(0.0.255) `&]_[* Se
t]([@(0.0.255) int]_[*@3 `_pos], [@(0.0.255) int]_[*@3 `_total])&]
[s2; Sets the new progress information.&]
[s7; [%-*C@3 `_pos]-|Current progress.&]
[s7; [%-*C@3 `_total]-|Total number of progress steps.&]
[s7; [*/ Return value]-|`*this for chaining.&]
[s3; &]
[s4;%- &]
[s5;:ProgressInfo`:`:Set`(int`):%- [@(0.0.255) void]_[* Set]([@(0.0.255) int]_[*@3 `_pos])&]
[s2; Sets the new progress information.&]
[s7; [%-*C@3 `_pos]-|Current progress. Total number of steps is set 
by Total method.&]
[s3; &]
[s4;%- &]
[s5;:ProgressInfo`:`:Get`(`)const:%- [@(0.0.255) int]_[* Get]()_[@(0.0.255) const]&]
[s7; [*/ Return value]-|Current progress.&]
[s3; &]
[s4;%- &]
[s5;:ProgressInfo`:`:operator`=`(int`):%- [@(0.0.255) void]_[* operator`=]([@(0.0.255) int]_
[*@3 p])&]
[s2; Same as Set(p).&]
[s3; &]
[s4;%- &]
[s5;:ProgressInfo`:`:operator`+`+`(`):%- [@(0.0.255) void]_[* operator`+`+]()&]
[s2; Same as Set(Get() `+ 1)&]
[s3; &]
[s4;%- &]
[s5;:ProgressInfo`:`:operator int`(`):%- [* operator_int]()&]
[s7; [*/ Return value]-|Get().&]
[s3; &]
[s0; ]