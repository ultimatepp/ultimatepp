topic "DrawingDraw";
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
[{_}%EN-US 
[ {{10000@(113.42.0) [s0;%- [*@7;4 DrawingDraw]]}}&]
[s3;%- &]
[s1;:DrawingDraw`:`:class:%- [@(0.0.255)3 class][3 _][*3 DrawingDraw][3 _:_][@(0.0.255)3 public][3 _
][*@3;3 Draw]&]
[s9; DrawingDraw is a Draw able to store drawing operations of Draw 
and to create Drawing value that can be replayed using Draw`::DrawDrawing.&]
[s3;%- &]
[s0;%- &]
[ {{10000F(128)G(128)@1 [s0; [* Public Method List]]}}&]
[s3;%- &]
[s5;:DrawingDraw`:`:Create`(int`,int`,bool`):%- [@(0.0.255) void]_[* Create]([@(0.0.255) in
t]_[*@3 cx], [@(0.0.255) int]_[*@3 cy], [@(0.0.255) bool]_[*@3 dots]_`=_[@(0.0.255) true])&]
[s5;:DrawingDraw`:`:Create`(Size`,bool`):%- [@(0.0.255) void]_[* Create]([_^Size^ Size]_[*@3 s
z], [@(0.0.255) bool]_[*@3 dots]_`=_[@(0.0.255) true])&]
[s2; Starts a new recording session. The size of resulting Drawing 
is in fact only important to compute correct rescaling in Draw`::DrawDrawing. 
[%-*@3 dots ]determine DOTS flag in Draw:GetInfo(). If there are 
already any records stored in DrawingDraw, they are cleaned.&]
[s3; &]
[s4; &]
[s5;:DrawingDraw`:`:GetSize`(`)const:%- [_^Size^ Size]_[* GetSize]()_[@(0.0.255) const]&]
[s2; Returns the size set by constructor or Create.&]
[s3; &]
[s4; &]
[s5;:DrawingDraw`:`:GetResult`(`):%- [_^Drawing^ Drawing]_[* GetResult]()&]
[s2; Returns actual result.&]
[s3; &]
[s4; &]
[s5;:DrawingDraw`:`:operator Drawing`(`):%- [* operator_Drawing]()&]
[s2; Returns GetResult().&]
[s3;%- &]
[s0;%- &]
[ {{10000F(128)G(128)@1 [s0; [* Constructor detail]]}}&]
[s3; &]
[s5;:DrawingDraw`:`:DrawingDraw`(`):%- [* DrawingDraw]()&]
[s2; Default constructor. Create has to be explicitly called on default 
constructed DrawingDraw.&]
[s3; &]
[s4; &]
[s5;:DrawingDraw`:`:DrawingDraw`(int`,int`,bool`):%- [* DrawingDraw]([@(0.0.255) int]_[*@3 c
x], [@(0.0.255) int]_[*@3 cy], [@(0.0.255) bool]_[*@3 dots]_`=_[@(0.0.255) true])&]
[s5;:DrawingDraw`:`:DrawingDraw`(Size`,bool`):%- [* DrawingDraw]([_^Size^ Size]_[*@3 sz], 
[@(0.0.255) bool]_[*@3 dots]_`=_[@(0.0.255) true])&]
[s2; Calls Create to start a recording session.&]
[s3; &]
[s0; ]