topic "Size";[ $$0,0#00000000000000000000000000000000:Default]
[i448;a25;kKO9;*@(64) $$1,0#37138531426314131252341829483380:class]
[l288;2 $$2,0#27521748481378242620020725143825:desc]
[i448;H4;b50;a25;kKO9;*@(64)2 $$3,0#37138531426314131252341829483370:item]
[~~~=_8333; $$4,4#05600065144404261032431302351956:break]
[l288;a25;kK~~~.1408;@3;2 $$5,0#61217621437177404841962893300719:param]
[i448;a25;kO9;*@(64)2 $$6,6#46142A79FADFFD11679EC28E167BF415:item`_next]
[{_}%EN-US 
[s0; Upp&]
[s0; Size`_&]
[s0;*@(154.0.0)+109 &]
[s1;K:`:`:Size`_`:`:struct: template_<[@(0.0.255) class]_[@4 T]>&]
[s1;K:`:`:Size`_`:`:struct: [@(0.0.255) struct]_[@0 Size`_]_:_[@(0.0.255) public]_[^topic`:`/`/Core`/srcdoc`/Moveable`$en`-us^ M
oveable]<_[@0 Size`_]<[@4 T]>_>_&]
[s2; &]
[s0; [* Size`_] is a generic structure that describes a two dimensional 
size object. The horizontal dimension is represented by [*@(154.0.0) cx][@(154.0.0)  
]and the vertical one by [*@(154.0.0) cy]. Both dimension are of 
type [@4 T]. To get a specialized version of [* Size`_], use one 
of the following:&]
[s0; &]
[s3; Size&]
[s6; [@(64) typedef_][^`:`:Size`_^@(64) Size`_][@(64) <][@(0.0.255) int][@(64) >_]Size&]
[s2; Size`_ with with [* int] dimensions.&]
[s0;*@(154.0.0)2 &]
[s3; Size16&]
[s6;:`:`:Size16`:`:typedef: [@(64) typedef_][^`:`:Size`_^@(64) Size`_][@(64) <][@(0.0.255) in
t16][@(64) >_]Size16&]
[s2; Size`_ with with [* int16][@(0.0.255)  ]dimensions.&]
[s0;*@(154.0.0)2 &]
[s3; Size64&]
[s6;:`:`:Size64`:`:typedef: [@(64) typedef_][^`:`:Size`_^@(64) Size`_][@(64) <][@(0.0.255) in
t64][@(64) >_]Size64&]
[s2; Size`_ with with [* int64][@(0.0.255)  ]dimensions.&]
[s0;*@(154.0.0)2 &]
[s3; Sizef&]
[s6;:`:`:Sizef`:`:typedef: [@(64) typedef_][^`:`:Size`_^@(64) Size`_][@(64) <][@(0.0.255) dou
ble][@(64) >_]Sizef&]
[s2; Size`_ with with [* double][@(0.0.255)  ]dimensions.&]
[s0; &]
[s0; &]
[ {{10000F(128)G(128)@1 [s0; Constructor Detail]}}&]
[s0; &]
[s3;K:`:`:Size`_`:`:`:`:Size`_`(`): [@0 Size`_]()&]
[s2; Creates an uninitialized [* Size`_].&]
[s4; &]
[s3;K:`:`:Size`_`:`:`:`:Size`_`(const`:`:Value`&`): [@0 Size`_]([@(0.0.255) const]_[^`:`:Value^ V
alue]`&_[@3 src])&]
[s2; Default copy constructor.&]
[s4; &]
[s3;K:`:`:Size`_`:`:`:`:Size`_`(T`,T`): [@0 Size`_]([@4 T]_[@3 cx], [@4 T]_[@3 cy])&]
[s5; Creates a [* Size`_] object an initializes it`'s horizontal dimension 
with [%-@3;3 cx][%-  and it`'s vertical dimension with ][*@3 cy].&]
[s4; &]
[s3;K:`:`:Size`_`:`:`:`:Size`_`(const`:`:Size`_`<int`>`&`): [@0 Size`_]([@(0.0.255) const
]_[@0 Size`_]<[@(0.0.255) int]>`&_[@3 sz])&]
[s2; Creates a [* Size`_] object and initializes it with a [* Size] [*@3 sz].&]
[s4; &]
[s3;K:`:`:Size`_`:`:`:`:Size`_`(const`:`:Size`_`<short`>`&`): [@0 Size`_]([@(0.0.255) con
st]_[@0 Size`_]<[@(0.0.255) short]>`&_[@3 sz])&]
[s2; Creates a [* Size`_] object and initializes it with a [* Size16] 
[*@3 sz].&]
[s4; &]
[s3;K:`:`:Size`_`:`:`:`:Size`_`(const`:`:Size`_`<double`>`&`): [@0 Size`_]([@(0.0.255) co
nst]_[@0 Size`_]<[@(0.0.255) double]>`&_[@3 sz])&]
[s2; Creates a [* Size`_] object and initializes it with a [* Sizef] 
[*@3 sz].&]
[s4; &]
[s3;K:`:`:Size`_`:`:`:`:Size`_`(const`:`:Size`_`<`:`:int64`>`&`): [@0 Size`_]([@(0.0.255) c
onst]_[@0 Size`_]<[@(0.0.255) int64]>`&_[@3 sz])&]
[s2; Creates a [* Size`_ ]object and initializes it with a [* Size64] 
[%-*@3 sz].&]
[s4; &]
[s3;K:`:`:Size`_`:`:`:`:Size`_`(const`:`:Point`_`<T`>`&`): [@0 Size`_]([@(0.0.255) const]_
[^`:`:Point`_^ Point`_]<[@4 T]>`&_[@3 pt])&]
[s2; Creates a Size`_ object and initializes it with a [* Point] [%-*@3 sz].&]
[s4; &]
[s3;K:`:`:Size`_`:`:`:`:Size`_`(const`:`:Nuller`&`): [@0 Size`_]([@(0.0.255) const]_[^`:`:Nuller^ N
uller]`&)&]
[s2; Creates a [* Size`_ ]object and initializes with [* Null].&]
[s0; &]
[s0; &]
[ {{10000F(128)G(128)@1 [s0; Public Field Detail]}}&]
[s0; &]
[s3;K:`:`:Size`_`:`:cx:%- [@4 T][@(64) _]cx&]
[s2; the horizontal dimension&]
[s0; &]
[s3;K:`:`:Size`_`:`:cy:%- [@4 T][@(64) _]cy&]
[s2; the vertical dimension&]
[s0; &]
[ {{10000F(128)G(128)@1 [s0; Method Detail]}}&]
[s0; &]
[s3;K:`:`:Size`_`:`:Clear`(`): [@(0.0.255) void]_[@0 Clear]()&]
[s2; Sets both dimension to 0.&]
[s4; &]
[s3;K:`:`:Size`_`:`:IsEmpty`(`)const: bool[@(64) _][@0 IsEmpty][@(64) ()_]const&]
[s2; Returns [* true] if at least one dimension is 0, [* false] otherwise.&]
[s4; &]
[s3;K:`:`:Size`_`:`:SetNull`(`): [@(0.0.255) void]_[@0 SetNull]()&]
[s2; Reinitializes the object to [* Null].&]
[s4; &]
[s3;K:`:`:Size`_`:`:IsNullInstance`(`)const: bool[@(64) _][@0 IsNullInstance][@(64) ()_]con
st&]
[s2; Verifies if size is [* Null].&]
[s4; &]
[s3;K:`:`:Size`_`:`:GetHashValue`(`)const: unsigned[@(64) _][@0 GetHashValue][@(64) ()_]con
st&]
[s2; Returns the hash value of the size.&]
[s4; &]
[s3;K:`:`:Size`_`:`:ToString`(`)const: [^topic`:`/`/Core`/src`/String`$en`-us^@(64) Str
ing][@(64) _][@0 ToString][@(64) ()_]const&]
[s2; Returns the [* String] representation of the size.&]
[s4; &]
[s3;K:`:`:Size`_`:`:operator`+`=`(`:`:Size`_`): [^`:`:Size`_^ Size`_]`&_[@0 operator`+`=](
[^`:`:Size`_^ Size`_]_[@3 p])&]
[s2; Increases cx with [*@3 p]`'s horizontal dimension and cy with 
[*@3 p]`'s vertical dimension.&]
[s4; &]
[s3;K:`:`:Size`_`:`:operator`+`=`(T`): [^`:`:Size`_^ Size`_]`&_[@0 operator`+`=](T_[@3 t])&]
[s2; Increases both dimension with the same value [*@3 t].&]
[s4; &]
[s3;K:`:`:Size`_`:`:operator`-`=`(`:`:Size`_`): [^`:`:Size`_^ Size`_]`&_[@0 operator`-`=](
[^`:`:Size`_^ Size`_]_[@3 p])&]
[s2; Decreases cx with [*@3 p]`'s horizontal dimension and cy with 
[*@3 p]`'s vertical dimension.&]
[s4; &]
[s3;K:`:`:Size`_`:`:operator`-`=`(T`): [^`:`:Size`_^ Size`_]`&_[@0 operator`-`=](T_[@3 t])&]
[s2; Decreases both dimension with the same value [*@3 t].&]
[s4; &]
[s3;K:`:`:Size`_`:`:operator`*`=`(`:`:Size`_`): [^`:`:Size`_^ Size`_]`&_[@0 operator`*`=](
[^`:`:Size`_^ Size`_]_[@3 p])&]
[s2; Multiplies cx with [*@3 p]`'s horizontal dimension and cy with 
[*@3 p]`'s vertical dimension.&]
[s4; &]
[s3;K:`:`:Size`_`:`:operator`*`=`(T`): [^`:`:Size`_^ Size`_]`&_[@0 operator`*`=](T_[@3 t])&]
[s2; Multiplies both dimension with the same value [*@3 t].&]
[s4; &]
[s3;K:`:`:Size`_`:`:operator`/`=`(`:`:Size`_`): [^`:`:Size`_^ Size`_]`&_[@0 operator/`=](
[^`:`:Size`_^ Size`_]_[@3 p])&]
[s2; Divides cx with [*@3 p]`'s horizontal dimension and cy with [*@3 p]`'s 
vertical dimension.&]
[s4; &]
[s3;K:`:`:Size`_`:`:operator`/`=`(T`): [^`:`:Size`_^ Size`_]`&_[@0 operator/`=](T_[@3 t])&]
[s2; Divides both dimension with the same value [*@3 t].&]
[s4; &]
[s3;K:`:`:Size`_`:`:operator`<`<`=`(int`): [^`:`:Size`_^ Size`_]`&_[@0 operator<<`=]([@(0.0.255) i
nt]_[@3 sh])&]
[s2; Shift both dimensions left by [*@3 sh].&]
[s4; &]
[s3;K:`:`:Size`_`:`:operator`>`>`=`(int`): [^`:`:Size`_^ Size`_]`&_[@0 operator>>`=]([@(0.0.255) i
nt]_[@3 sh])&]
[s2; Shift both dimensions right by [*@3 sh].&]
[s4; &]
[s3;K:`:`:Size`_`:`:operator`+`+`(`): [^`:`:Size`_^ Size`_]`&_[@0 operator`+`+]()&]
[s2; Increments both dimensions.&]
[s4; &]
[s3;K:`:`:Size`_`:`:operator`-`-`(`): [^`:`:Size`_^ Size`_]`&_[@0 operator`-`-]()&]
[s2; Decrements both dimensions.&]
[s4; &]
[s3;K:`:`:Size`_`:`:operator`:`:Value`(`)const: operator[@(64) _][@0 Value][@(64) ()_]const
&]
[s2; Returns a conversion of the size to [* Value].&]
[s4; &]
[s3;K:`:`:Size`_`:`:Serialize`(`:`:Stream`&`): [@(0.0.255) void]_[@0 Serialize]([^`:`:Stream^ S
tream]`&_[@3 s])&]
[s2; Serializes the size to a stream [*@(141.42.0) s].&]
[s0; &]
[s0; &]
[ {{10000F(128)G(128)@1 [s0; Friend Detail]}}&]
[s0; &]
[s3;K:`:`:Size`_`:`:operator`+`(`:`:Size`_`):%- [@(0.0.255) friend]_[^`:`:Size`_^ Size`_]_
[@0 operator`+]([^`:`:Size`_^ Size`_]_[@3 s])&]
[s2; Returns an unmodified size [%-*@3 s].&]
[s4; &]
[s3;K:`:`:Size`_`:`:operator`-`(`:`:Size`_`):%- [@(0.0.255) friend]_[^`:`:Size`_^ Size`_]_
[@0 operator`-]([^`:`:Size`_^ Size`_]_[@3 s])&]
[s2; Returns a negated size [%-*@3 s].&]
[s4; &]
[s3;K:`:`:Size`_`:`:operator`+`(`:`:Size`_`,`:`:Size`_`):%- [@(0.0.255) friend]_[^`:`:Size`_^ S
ize`_]_[@0 operator`+]([^`:`:Size`_^ Size`_]_[@3 a], [^`:`:Size`_^ Size`_]_[@3 b])&]
[s6;:`:`:Size`_`:`:operator`+`(`:`:Size`_`,T`):%- [@(0.0.255) friend]_[^`:`:Size`_^ Size`_
]_[@0 operator`+]([^`:`:Size`_^ Size`_]_[@3 a], T_[@3 t])&]
[s6;:`:`:Size`_`:`:operator`+`(T`,`:`:Size`_`):%- [@(0.0.255) friend]_[^`:`:Size`_^ Size`_
]_[@0 operator`+](T_[@3 t], [^`:`:Size`_^ Size`_]_[@3 b])&]
[s2; Returns the result of the addition of two Size`_ values or a 
Size`_ with single value.&]
[s4; &]
[s3;K:`:`:Size`_`:`:operator`-`(`:`:Size`_`,`:`:Size`_`):%- [@(0.0.255) friend]_[^`:`:Size`_^ S
ize`_]_[@0 operator`-]([^`:`:Size`_^ Size`_]_[@3 a], [^`:`:Size`_^ Size`_]_[@3 b])&]
[s6;:`:`:Size`_`:`:operator`-`(`:`:Size`_`,T`):%- [@(0.0.255) friend]_[^`:`:Size`_^ Size`_
]_[@0 operator`-]([^`:`:Size`_^ Size`_]_[@3 a], T_[@3 t])&]
[s6;:`:`:Size`_`:`:operator`-`(T`,`:`:Size`_`):%- [@(0.0.255) friend]_[^`:`:Size`_^ Size`_
]_[@0 operator`-](T_[@3 t], [^`:`:Size`_^ Size`_]_[@3 b])&]
[s2; Returns the result of the subtraction of two Size`_ values or 
a Size`_ with single value.&]
[s4; &]
[s3;K:`:`:Size`_`:`:operator`*`(`:`:Size`_`,`:`:Size`_`):%- [@(0.0.255) friend]_[^`:`:Size`_^ S
ize`_]_[@0 operator`*]([^`:`:Size`_^ Size`_]_[@3 a], [^`:`:Size`_^ Size`_]_[@3 b])&]
[s6;:`:`:Size`_`:`:operator`*`(`:`:Size`_`,T`):%- [@(0.0.255) friend]_[^`:`:Size`_^ Size`_
]_[@0 operator`*]([^`:`:Size`_^ Size`_]_[@3 a], T_[@3 b])&]
[s6;:`:`:Size`_`:`:operator`*`(T`,`:`:Size`_`):%- [@(0.0.255) friend]_[^`:`:Size`_^ Size`_
]_[@0 operator`*](T_[@3 a], [^`:`:Size`_^ Size`_]_[@3 b])&]
[s2; Returns the result of the multiplication of two Size`_ values 
or a Size`_ with single value.&]
[s4; &]
[s3;K:`:`:Size`_`:`:operator`/`(`:`:Size`_`,`:`:Size`_`):%- [@(0.0.255) friend]_[^`:`:Size`_^ S
ize`_]_[@0 operator/]([^`:`:Size`_^ Size`_]_[@3 a], [^`:`:Size`_^ Size`_]_[@3 b])&]
[s6;:`:`:Size`_`:`:operator`/`(`:`:Size`_`,T`):%- [@(0.0.255) friend]_[^`:`:Size`_^ Size`_
]_[@0 operator/]([^`:`:Size`_^ Size`_]_[@3 a], T_[@3 b])&]
[s2; Returns the result of the division of two Size`_ values or a 
Size`_ with single value.&]
[s4; &]
[s3;K:`:`:Size`_`:`:operator`<`<`(`:`:Size`_`,int`):%- [@(0.0.255) friend]_[^`:`:Size`_^ S
ize`_]_[@0 operator<<]([^`:`:Size`_^ Size`_]_[@3 a], [@(0.0.255) int]_[@3 sh])&]
[s2; Returns the result of left shifting [*@3 a] by [*@3 sh].&]
[s4; &]
[s3;K:`:`:Size`_`:`:operator`>`>`(`:`:Size`_`,int`):%- [@(0.0.255) friend]_[^`:`:Size`_^ S
ize`_]_[@0 operator>>]([^`:`:Size`_^ Size`_]_[@3 a], [@(0.0.255) int]_[@3 sh])&]
[s2; Returns the result of right shifting [*@3 a] by [*@3 sh].&]
[s4; &]
[s3;K:`:`:Size`_`:`:operator`=`=`(`:`:Size`_`,`:`:Size`_`):%- [@(0.0.255) friend]_[@(0.0.255) b
ool]_[@0 operator`=`=]([^`:`:Size`_^ Size`_]_[@3 a], [^`:`:Size`_^ Size`_]_[@3 b])&]
[s2; Returns [* true ]if [*@3 a ]and [*@3 b ]have their respective dimension 
equal, [* false ]otherwise.&]
[s4; &]
[s3;K:`:`:Size`_`:`:operator`!`=`(`:`:Size`_`,`:`:Size`_`):%- [@(0.0.255) friend]_[@(0.0.255) b
ool]_[@0 operator!`=]([^`:`:Size`_^ Size`_]_[@3 a], [^`:`:Size`_^ Size`_]_[@3 b])&]
[s2; Returns [* true ]if [*@3 a ]and [*@3 b ]have at least an unequal dimension, 
[* false ]otherwise.&]
[s4; &]
[s3;K:`:`:Size`_`:`:min`(`:`:Size`_`,`:`:Size`_`):%- [@(0.0.255) friend]_[^`:`:Size`_^ Si
ze`_]_[@0 min]([^`:`:Size`_^ Size`_]_[@3 a], [^`:`:Size`_^ Size`_]_[@3 b])&]
[s2;%- [%%* Returns a ][%% Size`_ ][%%* with it`'s dimension equaling the 
minimum of the respective dimension between ][@3 a ][* and][%%*  ][@3 b].&]
[s0;%- &]
[s3;K:`:`:Size`_`:`:max`(`:`:Size`_`,`:`:Size`_`):%- [@(0.0.255) friend]_[^`:`:Size`_^ Si
ze`_]_[@0 max]([^`:`:Size`_^ Size`_]_[@3 a], [^`:`:Size`_^ Size`_]_[@3 b])&]
[s2;%- [%%* Returns a ][%% Size`_ ][%%* with it`'s dimension equaling the 
maximum of the respective dimension between ][@3 a ][* and][%%*  ][@3 b].&]
[s4; &]
[s3;K:`:`:Size`_`:`:Nvl`(`:`:Size`_`,`:`:Size`_`):%- [@(0.0.255) friend]_[^`:`:Size`_^ Si
ze`_]_[@0 Nvl]([^`:`:Size`_^ Size`_]_[@3 a], [^`:`:Size`_^ Size`_]_[@3 b])&]
[s2; Returns [%-*@3 b] if [%-*@3 a] is Null, [%-*@3 a] otherwise.&]
[s4; &]
[s3;K:`:`:Size`_`:`:ScalarProduct`(`:`:Size`_`,`:`:Size`_`):%- [@(0.0.255) friend]_[@4 T]_
[@0 ScalarProduct]([^`:`:Size`_^ Size`_]_[@3 a], [^`:`:Size`_^ Size`_]_[@3 b])&]
[s2; Returns the scalar product between [%-*@3 a] and [%-*@3 b].&]
[s4; &]
[s3;K:`:`:Size`_`:`:VectorProduct`(`:`:Size`_`,`:`:Size`_`):%- [@(0.0.255) friend]_[@4 T]_
[@0 VectorProduct]([^`:`:Size`_^ Size`_]_[@3 a], [^`:`:Size`_^ Size`_]_[@3 b])&]
[s2; Returns the vectorial product between [%-*@3 a] and [%-*@3 b].&]
[s4; &]
[s3;K:`:`:Size`_`:`:Squared`(`:`:Size`_`):%- [@(0.0.255) friend]_[@4 T]_[@0 Squared]([^`:`:Size`_^ S
ize`_]_[@3 a])&]
[s2; Returns a Size`_ with it`'s dimension equal to the square of 
the dimension of [%-*@3 a].&]
[s4; &]
[s3;K:`:`:Size`_`:`:Length`(`:`:Size`_`):%- [@(0.0.255) friend]_[@(0.0.255) double]_[@0 Len
gth]([^`:`:Size`_^ Size`_]_[@3 a])&]
[s2; Returns the hypotenuse of the triangle described by [%-*@3 a].&]
[s0; ]