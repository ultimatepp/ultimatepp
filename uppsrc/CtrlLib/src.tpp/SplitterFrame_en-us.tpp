topic "SplitterFrame";
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
[ {{10000@3 [s0; [*@(229)4 SplitterFrame]]}}&]
[s3; &]
[s1;:SplitterFrame`:`:class:%- [@(0.0.255)3 class][3 _][*3 SplitterFrame][3 _:_][@(0.0.255)3 pub
lic][3 _][*@3;3 CtrlFrame][3 , ][@(0.0.255)3 private][3 _][*@3;3 Ctrl]&]
[s9; This class acts as an resizable frame area.&]
[s3; &]
[s0; &]
[ {{10000F(128)G(128)@1 [s0; [* Public Method List]]}}&]
[s3;%- &]
[s5;:SplitterFrame`:`:Serialize`(Stream`&`):%- [@(0.0.255) virtual] 
[@(0.0.255) void]_[* Serialize]([_^Stream^ Stream][@(0.0.255) `&]_[*@3 s])&]
[s2; Serializes actual SplitterFrame size.&]
[s3; &]
[s4;%- &]
[s5;:SplitterFrame`:`:Set`(Ctrl`&`,int`,int`):%- [_^SplitterFrame^ SplitterFrame][@(0.0.255) `&
]_[* Set]([_^Ctrl^ Ctrl][@(0.0.255) `&]_[*@3 c], [@(0.0.255) int]_[*@3 size], 
[@(0.0.255) int]_[*@3 type])&]
[s2; Sets SplitterFrame to contain [%-*@3 c] widget, with initial [%-*@3 size 
](in pixels).[%-*@3  type] determines the position `- it can be 
SplitterFrame`::[* LEFT], SplitterFrame`::[* RIGHT], SplitterFrame`::[* TOP 
]or SplitterFrame`::[* BOTTOM].&]
[s3; &]
[s4;%- &]
[s5;:SplitterFrame`:`:Left`(Ctrl`&`,int`):%- [_^SplitterFrame^ SplitterFrame][@(0.0.255) `&
]_[* Left]([_^Ctrl^ Ctrl][@(0.0.255) `&]_[*@3 c], [@(0.0.255) int]_[*@3 size])&]
[s2; Same as Set([%-*@3 c] , [%-*@3 size], SplitterFrame`::LEFT).&]
[s3; &]
[s4;%- &]
[s5;:SplitterFrame`:`:Top`(Ctrl`&`,int`):%- [_^SplitterFrame^ SplitterFrame][@(0.0.255) `&
]_[* Top]([_^Ctrl^ Ctrl][@(0.0.255) `&]_[*@3 c], [@(0.0.255) int]_[*@3 size])&]
[s2; Same as Set([%-*@3 c] , [%-*@3 size], SplitterFrame`::TOP).&]
[s3; &]
[s4;%- &]
[s5;:SplitterFrame`:`:Right`(Ctrl`&`,int`):%- [_^SplitterFrame^ SplitterFrame][@(0.0.255) `&
]_[* Right]([_^Ctrl^ Ctrl][@(0.0.255) `&]_[*@3 c], [@(0.0.255) int]_[*@3 size])&]
[s2; Same as Set([%-*@3 c] , [%-*@3 size], SplitterFrame`::RIGHT).&]
[s3; &]
[s4;%- &]
[s5;:SplitterFrame`:`:Bottom`(Ctrl`&`,int`):%- [_^SplitterFrame^ SplitterFrame][@(0.0.255) `&
]_[* Bottom]([_^Ctrl^ Ctrl][@(0.0.255) `&]_[*@3 c], [@(0.0.255) int]_[*@3 size])&]
[s2; Same as Set([%-*@3 c] , [%-*@3 size], SplitterFrame`::BOTTOM).&]
[s3; &]
[s4;%- &]
[s5;:SplitterFrame`:`:MinSize`(int`):%- [_^SplitterFrame^ SplitterFrame][@(0.0.255) `&]_[* M
inSize]([@(0.0.255) int]_[*@3 sz])&]
[s2; Sets the minimal size.&]
[s3; &]
[s4;%- &]
[s5;:SplitterFrame`:`:SizeMin`(int`):%- [_^SplitterFrame^ SplitterFrame][@(0.0.255) `&]_[* S
izeMin]([@(0.0.255) int]_[*@3 sz])&]
[s2; Sets the minimal size of next frame in the list or final view 
if there is none `- this in fact determines the maximum size 
of SplitterFrame.&]
[s3; &]
[s4;%- &]
[s5;:SplitterFrame`:`:GetType`(`)const:%- [@(0.0.255) int]_[* GetType]()_[@(0.0.255) const]&]
[s2; Returns the type (in pixels).&]
[s3; &]
[s4;%- &]
[s5;:SplitterFrame`:`:GetSize`(`)const:%- [@(0.0.255) virtual] [@(0.0.255) int]_[* GetSize](
)_[@(0.0.255) const]&]
[s2; Returns the current size.&]
[s3; &]
[s4;%- &]
[s5;:SplitterFrame`:`:SetSize`(int`):%- [@(0.0.255) void]_[* SetSize]([@(0.0.255) int]_[*@3 s
z])&]
[s2; Sets the size (in pixels).&]
[s3; &]
[s0; ]]