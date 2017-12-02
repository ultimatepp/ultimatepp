topic "Drawing";
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
[ {{10000@(113.42.0) [s0;%% [*@7;4 Drawing]]}}&]
[s3; &]
[s1;:Drawing`:`:class: [@(0.0.255)3 class][3 _][*3 Drawing][3 _:_][@(0.0.255)3 private][3 _][*@3;3 A
ssignValueTypeNo][3 <][*3 Drawing][3 , ][@3;3 49][3 , ][_^Moveable^3 Moveable][3 <][*3 Drawing][3 >
_>_]&]
[s9;%% This concrete class, with full value semantics (with fast 
deep copy) represents a record of Draw drawing operations that 
can be replayed at any time. In other words, Drawing is a sort 
of vector format. To create Drawing, use [^DrawingDraw^ DrawingDraw] 
class.&]
[s9;%% Drawing is Value compatible with `"rich`" feature set `- as 
Value it supports equality comparisons, serialization, Null compatibility 
and hash`-value support.&]
[s3; &]
[s0; &]
[ {{10000F(128)G(128)@1 [s0;%% [* Public Method List]]}}&]
[s3; &]
[s5;:Drawing`:`:operator bool`(`)const: [* operator_bool]()_[@(0.0.255) const]&]
[s2;%% True if Drawing is non`-empty.&]
[s3; &]
[s4; &]
[s5;:Drawing`:`:GetSize`(`)const: [_^Size^ Size]_[* GetSize]()_[@(0.0.255) const]&]
[s2;%% Returns the size of Drawing `- the value explicitly defined 
in DrawingDraw or set by SetSize.&]
[s3; &]
[s4; &]
[s5;:Drawing`:`:SetSize`(Size`): [@(0.0.255) void]_[* SetSize]([_^Size^ Size]_[*@3 sz])&]
[s5;:Drawing`:`:SetSize`(int`,int`): [@(0.0.255) void]_[* SetSize]([@(0.0.255) int]_[*@3 cx],
 [@(0.0.255) int]_[*@3 cy])&]
[s2;%% Sets the size of Drawing.&]
[s3;%% &]
[s4;%% &]
[s5;:Drawing`:`:RatioSize`(int`,int`)const: [_^Size^ Size]_[* RatioSize]([@(0.0.255) int]_[*@3 c
x], [@(0.0.255) int]_[*@3 cy])_[@(0.0.255) const]&]
[s5;:Drawing`:`:RatioSize`(Size`)const: [_^Size^ Size]_[* RatioSize]([_^Size^ Size]_[*@3 sz])
_[@(0.0.255) const]&]
[s2;%% If either width or height specified as parameter is zero, 
it gets computed based on current Drawing size and nonzero height 
or width keeping the aspect ratio. If both width and height are 
zero, returns Drawing size.&]
[s3;%% &]
[s4;%% &]
[s5;:Drawing`:`:Clear`(`): [@(0.0.255) void]_[* Clear]()&]
[s2;%% Sets Drawing empty.&]
[s3;%% &]
[s4;%% &]
[s5;:Drawing`:`:Append`(Drawing`&`): [@(0.0.255) void]_[* Append]([_^Drawing^ Drawing][@(0.0.255) `&
]_[*@3 dw])&]
[s2;%% Appends another Drawing at the end.&]
[s3;%% &]
[s4;%% &]
[s5;:Drawing`:`:Serialize`(Stream`&`): [@(0.0.255) void]_[* Serialize]([_^Stream^ Stream][@(0.0.255) `&
]_[*@3 s])&]
[s2;%% Serializes the content of Drawing with the stream [%-*@3 s].&]
[s3;%% &]
[s4;%% &]
[s5;:Drawing`:`:IsNullInstance`(`)const: [@(0.0.255) bool]_[* IsNullInstance]()_[@(0.0.255) c
onst]&]
[s2;%% True if Drawing is empty.&]
[s3;%% &]
[s4;%% &]
[s5;:Drawing`:`:operator`=`=`(const Drawing`&`)const: [@(0.0.255) bool]_[* operator`=`=](
[@(0.0.255) const]_[_^Drawing^ Drawing][@(0.0.255) `&]_[*@3 b])_[@(0.0.255) const]&]
[s2;%% True if both Drawings are equal.&]
[s3;%% &]
[s4;%% &]
[s5;:Drawing`:`:ToString`(`)const: [_^String^ String]_[* ToString]()_[@(0.0.255) const]&]
[s2;%% Text representation for debug purposes.&]
[s3;%% &]
[s4;%% &]
[s5;:Drawing`:`:GetHashValue`(`)const: [@(0.0.255) unsigned]_[* GetHashValue]()_[@(0.0.255) c
onst]&]
[s2;%% Hash value.&]
[s3;%% &]
[s4;%% &]
[s5;:Drawing`:`:operator Value`(`)const: [* operator_Value]()_[@(0.0.255) const]&]
[s2;%% Returns Drawing as Value `- Drawing is `"rich`" Value, supporting 
operator`=`=, hashing, Null and serialization.&]
[s3; &]
[s0; &]
[ {{10000F(128)G(128)@1 [s0;%% [* Constructor detail]]}}&]
[s3; &]
[s5;:Drawing`:`:Drawing`(const Value`&`): [* Drawing]([@(0.0.255) const]_[_^Value^ Value][@(0.0.255) `&
]_[*@3 src])&]
[s2;%% Deep copy constructor.&]
[s3;%% &]
[s4;%% &]
[s5;:Drawing`:`:Drawing`(`): [* Drawing]()&]
[s2;%% Default constructor.&]
[s3;%% &]
[s4;%% &]
[s5;:Drawing`:`:Drawing`(const Nuller`&`): [* Drawing]([@(0.0.255) const]_[_^Nuller^ Nuller
][@(0.0.255) `&])&]
[s2; Null constructor (provides Drawing Null).&]
[s3; &]
[s0; ]