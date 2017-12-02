topic "Slider";
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
[ {{10000@3 [s0; [*@(229)4 SliderCtrl]]}}&]
[s3; &]
[s1;:SliderCtrl`:`:class:%- [@(0.0.255)3 class][3 _][*3 SliderCtrl][3 _:_][@(0.0.255)3 public][3 _
][*@3;3 Ctrl]&]
[s9; SliderCtrl is a widget that allows entering an integer value 
by dragging a thumb.&]
[s0; &]
[s2;%- 
@@image:681&143
€ƒí€—€€€€€€€€€€ø¯œí˜½‰À ¨Æİµ²Á‡Šìàš©Ü úà•…Ì„üà –š¸ïãò•–ŸÏ¯ğÂÁ¶†€€˜‡ûÅŒúŸ‘‘ïŞ
ùûøÆÎ˜¬ÓÎ¨öœÛ“¼È¦ÆÆš´¢‘›Õ¬óÔ‰Ùïê­îÈ½‚ªÆÓÇ²šµË°İ‘×åê“¢±ÊĞ—´Æî×’Õ¨İêõ‡«üÁëœ‚´
Ë¢±ŸøŒş¥ò±­£Øò´›‰Ä¡€¨ğ€§ƒÌÂœ
&]
[s0;%- &]
[s0; [/ Derived from] [^topic`:`/`/CtrlCore`/src`/Ctrl`$en`-us`#`:`:Ctrl`:`:class Ctrl^ C
trl]&]
[s3; &]
[s0;%- &]
[ {{10000F(128)G(128)@1 [s0; [* Public Member List]]}}&]
[s3;%- &]
[s5;:SliderCtrl`:`:Inc`(`):%- [@(0.0.255) void]_[* Inc]()&]
[s2; Increments slider value by [* Step].&]
[s3; &]
[s4;%- &]
[s5;:SliderCtrl`:`:Dec`(`):%- [@(0.0.255) void]_[* Dec]()&]
[s2; Decrements slider value by [* Step].&]
[s3; &]
[s4;%- &]
[s5;:SliderCtrl`:`:MinMax`(int`,int`):%- [_^SliderCtrl^ SliderCtrl][@(0.0.255) `&]_[* MinMa
x]([@(0.0.255) int]_[*@3 `_min], [@(0.0.255) int]_[*@3 `_max])&]
[s2; Sets the minimum (thumb in leftmost position) and maximum (thumb 
in rightmost position) values. Returns `*this.&]
[s3; &]
[s4;%- &]
[s5;:SliderCtrl`:`:Range`(int`):%- [_^SliderCtrl^ SliderCtrl][@(0.0.255) `&]_[* Range]([@(0.0.255) i
nt]_[*@3 max])&]
[s2; Same as [^topic`:`/`/CtrlLib`/src`/Slider`$en`-us`#SliderCtrl`:`:MinMax`(int`,int`)^ M
inMax](0, [%-*@3 max]).&]
[s3; &]
[s4;%- &]
[s5;:SliderCtrl`:`:GetMin`(`)const:%- [@(0.0.255) int]_[* GetMin]()_[@(0.0.255) const]&]
[s2; Returns minimum value.&]
[s3; &]
[s4;%- &]
[s5;:SliderCtrl`:`:GetMax`(`)const:%- [@(0.0.255) int]_[* GetMax]()_[@(0.0.255) const]&]
[s2; Returns maximum value.&]
[s3; &]
[s4;%- &]
[s5;:SliderCtrl`:`:IsVert`(`)const:%- [@(0.0.255) bool]_[* IsVert]()_[@(0.0.255) const]&]
[s2; Sets the step used in Inc and Dec methods. If [%-*@3 r] is true, 
SliderCtrl rounds the value to step multiplies even when dragging 
the thumb by mouse.&]
[s3; &]
[s4;%- &]
[s5;:SliderCtrl`:`:GetStep`(`)const:%- [@(0.0.255) int]_[* GetStep]()_[@(0.0.255) const]&]
[s2; Returns current step.&]
[s3; &]
[s4;%- &]
[s5;:SliderCtrl`:`:IsRoundStep`(`)const:%- [@(0.0.255) bool]_[* IsRoundStep]()_[@(0.0.255) c
onst]&]
[s2; Tests whether step rounding is active.&]
[s3; &]
[s0; ]