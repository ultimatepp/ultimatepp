topic "Static widgets";
[i448;a25;kKO9;2 $$1,0#37138531426314131252341829483380:class]
[l288;2 $$2,0#27521748481378242620020725143825:desc]
[0 $$3,0#96390100711032703541132217272105:end]
[H6;0 $$4,0#05600065144404261032431302351956:begin]
[i448;a25;kKO9;2 $$5,0#37138531426314131252341829483370:item]
[l288;a4;*@5;1 $$6,6#70004532496200323422659154056402:requirement]
[l288;i1121;b17;O9;~~~.1408;2 $$7,0#10431211400427159095818037425705:param]
[i448;b42;O9;2 $$8,8#61672508125594000341940100500538:tparam]
[b42;2 $$9,9#13035079074754324216151401829390:normal]
[2 $$0,0#00000000000000000000000000000000:Default]
[{_} 
[s0;%% [*+150 Static widgets]&]
[s3;%% &]
[ {{10000@3 [s0;%% [*@(229)4 StaticText]]}}&]
[s3;%% &]
[s1;:StaticText`:`:class: [@(0.0.255)3 class][3 _][*3 StaticText][3 _:_][@(0.0.255)3 public][3 _][*@3;3 C
trl][3 , ][@(0.0.255)3 public][3 _][*@3;3 LabelBase]&]
[s9;%% Static text.&]
[s3; &]
[s0; &]
[ {{10000F(128)G(128)@1 [s0;%% [* Public Method List]]}}&]
[s3; &]
[s5;:StaticText`:`:SetFont`(Font`): [_^StaticText^ StaticText][@(0.0.255) `&]_[* SetFont]([_^Font^ F
ont]_[*@3 font])&]
[s2;%% Sets the [%-*@3 font] of text.&]
[s3;%% &]
[s4; &]
[s5;:StaticText`:`:SetInk`(Color`): [_^StaticText^ StaticText][@(0.0.255) `&]_[* SetInk]([_^Color^ C
olor]_[*@3 color])&]
[s2;%% Sets the [%-*@3 color] of text.&]
[s3;%% &]
[s4; &]
[s5;:StaticText`:`:SetAlign`(int`): [_^StaticText^ StaticText][@(0.0.255) `&]_[* SetAlign](
[@(0.0.255) int]_[*@3 align])&]
[s2;%% Sets alignment [%-*@3 align]. Allowed values are ALIGN`_LEFT, 
ALIGN`_CENTER, ALIGN`_RIGHT.&]
[s3;%% &]
[s4; &]
[s5;:Upp`:`:StaticText`:`:AlignLeft`(`): [_^Upp`:`:StaticText^ StaticText][@(0.0.255) `&]_
[* AlignLeft]()&]
[s2;%% Same as SetAlign(ALIGN`_LEFT).&]
[s3; &]
[s4; &]
[s5;:Upp`:`:StaticText`:`:AlignCenter`(`): [_^Upp`:`:StaticText^ StaticText][@(0.0.255) `&
]_[* AlignCenter]()&]
[s2;%% Same as SetAlign(ALIGN`_CENTER).&]
[s3; &]
[s4; &]
[s5;:Upp`:`:StaticText`:`:AlignRight`(`): [_^Upp`:`:StaticText^ StaticText][@(0.0.255) `&
]_[* AlignRight]()&]
[s2;%% Same as SetAlign(ALIGN`_RIGHT).&]
[s3; &]
[s4; &]
[s5;:StaticText`:`:SetImage`(const Image`&`,int`): [_^StaticText^ StaticText][@(0.0.255) `&
]_[* SetImage]([@(0.0.255) const]_[_^Image^ Image][@(0.0.255) `&]_[*@3 img], 
[@(0.0.255) int]_[*@3 spc]_`=_[@3 0])&]
[s2;%% Sets the Image to be displayed before the text. [%-*@3 spc] 
is space between the text and Image.&]
[s3;%% &]
[s4; &]
[s5;:StaticText`:`:SetText`(const char`*`): [_^StaticText^ StaticText][@(0.0.255) `&]_[* Se
tText]([@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 text])&]
[s5;:StaticText`:`:operator`=`(const char`*`): [_^StaticText^ StaticText][@(0.0.255) `&]_
[* operator`=]([@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 s])&]
[s2;%% Sets [%-*@3 text]. `"`\1`" at the beginning of text activates 
QTF.&]
[s3;%% &]
[s3;%% &]
[s4; &]
[s5;:Upp`:`:StaticText`:`:GetText`(`)const: [_^Upp`:`:String^ String]_[* GetText]()_[@(0.0.255) c
onst]&]
[s2;%% Returns the text.&]
[s0;%% &]
[s0; &]
[ {{10000@3 [s0;%% [*@(229)4 Label]]}}&]
[s3;%% &]
[s1;:Label`:`:class: [@(0.0.255)3 class][3 _][*3 Label][3 _:_][@(0.0.255)3 public][3 _][*@3;3 Static
Text]&]
[s9;%% Generally used to label other widgets. Differs from StaticText 
by implementing hotkey processing. Hotkeys are assigned automatically 
by U`+`+ library, however they can also by preassigned using 
`'`&`' character.&]
[s3; &]
[s0; &]
[ {{10000F(128)G(128)@1 [s0;%% [* Public Method List]]}}&]
[s3; &]
[s5;:Label`:`:SetText`(const char`*`): [_^Label^ Label][@(0.0.255) `&]_[* SetText]([@(0.0.255) c
onst]_[@(0.0.255) char]_`*[*@3 text])&]
[s5;:Label`:`:operator`=`(const char`*`): [_^Label^ Label][@(0.0.255) `&]_[* operator`=]([@(0.0.255) c
onst]_[@(0.0.255) char]_`*[*@3 s])&]
[s2;%% Sets the text. Ignores `'`&`' used to mark hotkeys.&]
[s3; &]
[s4; &]
[s5;:Label`:`:SetLabel`(const char`*`): [_^Label^ Label][@(0.0.255) `&]_[* SetLabel]([@(0.0.255) c
onst]_[@(0.0.255) char]_`*[*@3 lbl])&]
[s2;%% Sets the text, `'`&`' marks hotkeys.&]
[s3;%% &]
[s0;%% &]
[s0;%% &]
[s0; &]
[ {{10000@3 [s0;%% [*@(229)4 LabelBox]]}}&]
[s3;%% &]
[s1;:LabelBox`:`:class: [@(0.0.255)3 class][3 _][*3 LabelBox][3 _:_][@(0.0.255)3 public][3 _][*@3;3 L
abel]&]
[s9;%% Rectangular static widget used to visually group other widgets.&]
[s3; &]
[s0; &]
[ {{10000F(128)G(128)@1 [s0;%% [* Public Method List]]}}&]
[s3; &]
[s5;:LabelBox`:`:operator`=`(const char`*`): [@(0.0.255) virtual] [_^LabelBox^ LabelBox][@(0.0.255) `&
]_[* operator`=]([@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 s])&]
[s2;%% Assigns the text of LabelBox.&]
[s3;%% &]
[s4;%% &]
[s5;:LabelBox`:`:SetColor`(Color`): [_^LabelBox^ LabelBox][@(0.0.255) `&]_[* SetColor]([_^Color^ C
olor]_[*@3 c])&]
[s2;%% Sets the color of LabelBox.&]
[s3;%% &]
[s0;%% &]
[s0; &]
[ {{10000@3 [s0;%% [*@(229)4 ParentCtrl]]}}&]
[s3;%% &]
[s1;:ParentCtrl`:`:class: [@(0.0.255)3 class][3 _][*3 ParentCtrl][3 _:_][@(0.0.255)3 public][3 _][*@3;3 C
trl]&]
[s9;%% This class is supposed to be used as logical parent in situation 
when more widgets are to be grouped together. The only difference 
from Ctrl is that ParentCtrl activates Transparent in constructor.&]
[s3;%% &]
[s0;%% &]
[s0;%% &]
[s0; &]
[ {{10000@3 [s0;%% [*@(229)4 StaticRect]]}}&]
[s3;%% &]
[s1;:StaticRect`:`:class: [@(0.0.255)3 class][3 _][*3 StaticRect][3 _:_][@(0.0.255)3 public][3 _][*@3;3 C
trl]&]
[s9;%% Widget completely filled with single color.&]
[s3; &]
[s0; &]
[ {{10000F(128)G(128)@1 [s0;%% [* Public Method List]]}}&]
[s3; &]
[s5;:StaticRect`:`:Color`(class Color`): [_^StaticRect^ StaticRect][@(0.0.255) `&]_[* Color
]([@(0.0.255) class]_[* Color]_[*@3 c])&]
[s2;%% The color. Default is SColorFace.&]
[s3;%% &]
[s0;%% &]
[s0;%% &]
[s0; &]
[ {{10000@3 [s0;%% [*@(229)4 ImageCtrl]]}}&]
[s3;%% &]
[s1;:ImageCtrl`:`:class: [@(0.0.255)3 class][3 _][*3 ImageCtrl][3 _:_][@(0.0.255)3 public][3 _][*@3;3 C
trl]&]
[s9;%% Display raster Image. Image is centered in the widget rectangle.&]
[s3; &]
[s0; &]
[ {{10000F(128)G(128)@1 [s0;%% [* Public Method List]]}}&]
[s3; &]
[s5;:ImageCtrl`:`:SetImage`(const Image`&`): [_^ImageCtrl^ ImageCtrl][@(0.0.255) `&]_[* Set
Image]([@(0.0.255) const]_[_^Image^ Image][@(0.0.255) `&]_[*@3 `_img])&]
[s2;%% Sets the image.&]
[s3;%% &]
[s0;%% &]
[s0;%% &]
[s0; &]
[ {{10000@3 [s0;%% [*@(229)4 DrawingCtrl]]}}&]
[s3;%% &]
[s1;:DrawingCtrl`:`:class: [@(0.0.255)3 class][3 _][*3 DrawingCtrl][3 _:_][@(0.0.255)3 public][3 _
][*@3;3 Ctrl]&]
[s9;%% Displays Drawing.&]
[s3; &]
[s0; &]
[ {{10000F(128)G(128)@1 [s0;%% [* Public Method List]]}}&]
[s3; &]
[s5;:DrawingCtrl`:`:Background`(Color`): [_^Picture^ DrawingCtrl][@(0.0.255) `&]_[* Backgro
und]([_^Color^ Color]_[*@3 color])&]
[s2;%% Sets background [%-*@3 color]. Default is white.&]
[s3;%% &]
[s4; &]
[s5;:DrawingCtrl`:`:Get`(`)const: [_^Drawing^ Drawing]_[* Get]()_[@(0.0.255) const]&]
[s2;%% Returns Drawing assigned to Picture.&]
[s3; &]
[s4; &]
[s5;:DrawingCtrl`:`:KeepRatio`(bool`): [_^Picture^ DrawingCtrl][@(0.0.255) `&]_[* KeepRatio
]([@(0.0.255) bool]_[*@3 keep]_`=_[@(0.0.255) true])&]
[s2;%% If active, Picture keeps the aspect ratio of Drawing. Default 
is on.&]
[s3;%% &]
[s4; &]
[s5;:DrawingCtrl`:`:NoKeepRatio`(`): [_^Picture^ DrawingCtrl][@(0.0.255) `&]_[* NoKeepRatio
]()&]
[s2;%% Do not keep aspect ratio.&]
[s3;%% &]
[s4; &]
[s5;:DrawingCtrl`:`:Set`(const Drawing`&`): [_^Picture^ DrawingCtrl][@(0.0.255) `&]_[* Set](
[@(0.0.255) const]_[_^Drawing^ Drawing][@(0.0.255) `&]_[*@3 `_picture])&]
[s5;:DrawingCtrl`:`:operator`=`(const Drawing`&`): [_^Picture^ DrawingCtrl][@(0.0.255) `&
]_[* operator`=]([@(0.0.255) const]_[_^Drawing^ Drawing][@(0.0.255) `&]_[*@3 `_picture])&]
[s2;%% Assigns the Drawing.&]
[s3;%% &]
[s4; &]
[s5;:DrawingCtrl`:`:operator`=`(const Painting`&`): [_^Picture^ DrawingCtrl][@(0.0.255) `&
]_[* operator`=]([@(0.0.255) const]_[_^Painting^ Painting][@(0.0.255) `&]_[*@3 `_picture])&]
[s2;%% Assigns [%-*@3 `_picture], converted to Drawing.&]
[s0;%% &]
[s0;%% &]
[s0; &]
[ {{10000@3 [s0;%% [*@(229)4 SeparatorCtrl]]}}&]
[s3;%% &]
[s1;:SeparatorCtrl`:`:class: [@(0.0.255)3 class][3 _][*3 SeparatorCtrl][3 _:_][@(0.0.255)3 publi
c][3 _][*@3;3 Ctrl]&]
[s9;%% Horizontal or vertical separator line. Direction depends on 
aspect ratio.&]
[s3; &]
[s0; &]
[ {{10000F(128)G(128)@1 [s0;%% [* Public Method List]]}}&]
[s3; &]
[s5;:SeparatorCtrl`:`:Margin`(int`): [_^SeparatorCtrl^ SeparatorCtrl][@(0.0.255) `&]_[* Mar
gin]([@(0.0.255) int]_[*@3 w])&]
[s2;%% Sets the left`-right (for horizontal separator) or top`-bottom 
margin. Default value is 2.&]
[s3;%% &]
[s4; &]
[s5;:SeparatorCtrl`:`:Margin`(int`,int`): [_^SeparatorCtrl^ SeparatorCtrl][@(0.0.255) `&]_
[* Margin]([@(0.0.255) int]_[*@3 l], [@(0.0.255) int]_[*@3 r])&]
[s2;%% Sets the top`-right (for horizontal separator) or top`-bottom 
to [%-*@3 l] and [%-*@3 r], in that order. Default value is 2.&]
[s3;%% &]
[s4; &]
[s5;:SeparatorCtrl`:`:SetSize`(int`): [_^SeparatorCtrl^ SeparatorCtrl][@(0.0.255) `&]_[* Se
tSize]([@(0.0.255) int]_[*@3 w])&]
[s2;%% Sets the size returned by GetMinSize `- included to affect 
the way how widget is treated in BarCtrl. Default value is 7.&]
[s3;%% &]
[s4; &]
[s5;:SeparatorCtrl`:`:SetStyle`(const SeparatorCtrl`:`:Style`&`): [_^SeparatorCtrl^ Sep
aratorCtrl][@(0.0.255) `&]_[* SetStyle]([@(0.0.255) const]_[_^SeparatorCtrl`:`:Style^ Sty
le][@(0.0.255) `&]_[*@3 s])&]
[s2;%% Sets the visual style to SeparatorCtrl`::Style [%-*@3 s]. The 
style is a simple structure with two fields:&]
[s7;%% [*@5 l1]-|the first margin&]
[s7; [*@5 l2]-|the second margin&]
[s3;%% &]
[s0;%% ]]