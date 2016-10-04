topic "Size";
[2 $$0,0#00000000000000000000000000000000:Default]
[i448;a25;kKO9;2 $$1,0#37138531426314131252341829483380:class]
[l288;2 $$2,0#27521748481378242620020725143825:desc]
[0 $$3,0#96390100711032703541132217272105:end]
[H6;0 $$4,0#05600065144404261032431302351956:begin]
[i448;a25;kKO9;2 $$5,0#37138531426314131252341829483370:item]
[l288;a17;*@5;1 $$6,6#70004532496200323422659154056402:requirement]
[l288;i1121;b17;O9;~~~.1408;2 $$7,0#10431211400427159095818037425705:param]
[{_}%EN-US 
[s0; [*@(128) Upp]&]
[s0; [*@3;4 Size`_]&]
[s0;*@3;4 &]
[s1;:noref:%- [@(0.0.255) template][@(64) _<][@(0.0.255) class][@(64) _][@4 T][@(64) >]&]
[s1;%- [@(0.0.255) struct][@(64) _]Size`_[@(64) _:_][@(0.0.255) public][@(64) _][^`:`:Moveable^@(64) M
oveable][@(64) <_]Size`_[@(64) <][@4 T][@(64) >_>_]&]
[s0; &]
[s0; [* Size`_] is a generic structure that describes a two dimensional 
size object. The horizontal dimension is represented by [*@(154.0.0) cx][@(154.0.0)  
]and the vertical one by [*@(154.0.0) cy]. Both dimension are of 
type [*@4 T]. To get a specialized version of [* Size`_], use one 
of the following:&]
[s0; &]
[s0;%- Size&]
[s5;:`:`:Size`:`:typedef:%- [@(0.0.255) typedef][@(64) _][^`:`:Size`_^@(64) Size`_][@(64) <][@(0.0.255) i
nt][@(64) >_]Size&]
[s2; Size`_ with with [* int] dimensions.&]
[s3; &]
[s4; &]
[s0;%- Size16&]
[s5;:`:`:Size16`:`:typedef:%- [@(0.0.255) typedef][@(64) _][^`:`:Size`_^@(64) Size`_][@(64) <
][@(0.0.255) int16][@(64) >_]Size16&]
[s2; Size`_ with with [* int16][@(0.0.255)  ]dimensions.&]
[s3; &]
[s4;*@(154.0.0) &]
[s0;%- Size64&]
[s5;:`:`:Size64`:`:typedef:%- [@(0.0.255) typedef][@(64) _][^`:`:Size`_^@(64) Size`_][@(64) <
][@(0.0.255) int64][@(64) >_]Size64&]
[s2; Size`_ with with [* int64][@(0.0.255)  ]dimensions.&]
[s3; &]
[s4;*@(154.0.0) &]
[s0; Sizef&]
[s5;:`:`:Sizef`:`:typedef:%- [@(0.0.255) typedef][@(64) _][^`:`:Size`_^@(64) Size`_][@(64) <][@(0.0.255) d
ouble][@(64) >_]Sizef&]
[s2; Size`_ with with [* double][@(0.0.255)  ]dimensions.&]
[s0; &]
[s0; &]
[ {{10000F(128)G(128)@1 [s0; [* Constructor Detail]]}}&]
[s0; &]
[s5;K:`:`:Size`_`:`:`:`:Size`_`(`):%- Size`_[@(64) ()]&]
[s2; Creates an uninitialized [* Size`_].&]
[s3; &]
[s4; &]
[s5;K:`:`:Size`_`:`:`:`:Size`_`(const`:`:Value`&`):%- Size`_[@(64) (][@(0.0.255) const][@(64) _
][^`:`:Value^@(64) Value][@(64) `&_][@3 src][@(64) )]&]
[s2; Default copy constructor.&]
[s3; &]
[s4; &]
[s5;K:`:`:Size`_`:`:`:`:Size`_`(T`,T`):%- Size`_[@(64) (][@4 T][@(64) _][@3 cx][@(64) , 
][@4 T][@(64) _][@3 cy][@(64) )]&]
[s2; Creates a [* Size`_] object an initializes it`'s horizontal dimension 
with [%-*@3 cx ][%- and it`'s vertical dimension with ][*@3 cy].&]
[s3; &]
[s4; &]
[s5;K:`:`:Size`_`:`:`:`:Size`_`(const`:`:Size`_`<int`>`&`):%- Size`_[@(64) (][@(0.0.255) c
onst][@(64) _]Size`_[@(64) <][@(0.0.255) int][@(64) >`&_][@3 sz][@(64) )]&]
[s2; Creates a [* Size`_] object and initializes it with a [* Size] [*@3 sz].&]
[s3; &]
[s4; &]
[s5;K:`:`:Size`_`:`:`:`:Size`_`(const`:`:Size`_`<short`>`&`):%- Size`_[@(64) (][@(0.0.255) c
onst][@(64) _]Size`_[@(64) <][@(0.0.255) short][@(64) >`&_][@3 sz][@(64) )]&]
[s2; Creates a [* Size`_] object and initializes it with a [* Size16] 
[*@3 sz].&]
[s3; &]
[s4; &]
[s5;K:`:`:Size`_`:`:`:`:Size`_`(const`:`:Size`_`<double`>`&`):%- Size`_[@(64) (][@(0.0.255) c
onst][@(64) _]Size`_[@(64) <][@(0.0.255) double][@(64) >`&_][@3 sz][@(64) )]&]
[s2; Creates a [* Size`_] object and initializes it with a [* Sizef] 
[*@3 sz].&]
[s3; &]
[s4; &]
[s5;K:`:`:Size`_`:`:`:`:Size`_`(const`:`:Size`_`<`:`:int64`>`&`):%- Size`_[@(64) (][@(0.0.255) c
onst][@(64) _]Size`_[@(64) <][@(0.0.255) int64][@(64) >`&_][@3 sz][@(64) )]&]
[s2; Creates a [* Size`_ ]object and initializes it with a [* Size64] 
[%-*@3 sz].&]
[s3; &]
[s4; &]
[s5;K:`:`:Size`_`:`:`:`:Size`_`(const`:`:Point`_`<T`>`&`):%- Size`_[@(64) (][@(0.0.255) c
onst][@(64) _][^`:`:Point`_^@(64) Point`_][@(64) <][@4 T][@(64) >`&_][@3 pt][@(64) )]&]
[s2; Creates a [* Size`_ ]object and initializes it with a [* Point] 
[%-*@3 sz].&]
[s3; &]
[s4; &]
[s5;K:`:`:Size`_`:`:`:`:Size`_`(const`:`:Nuller`&`):%- Size`_[@(64) (][@(0.0.255) const][@(64) _
][^`:`:Nuller^@(64) Nuller][@(64) `&)]&]
[s2; Creates a [* Size`_ ]object and initializes with [* Null].&]
[s0; &]
[s0; &]
[ {{10000F(128)G(128)@1 [s0; [* Public Field Detail]]}}&]
[s0; &]
[s5;K:`:`:Size`_`:`:cx:%- [@4 T][@(64) _]cx&]
[s2; the horizontal dimension&]
[s3; &]
[s4; &]
[s5;K:`:`:Size`_`:`:cy:%- [@4 T][@(64) _]cy&]
[s2; the vertical dimension&]
[s0; &]
[ {{10000F(128)G(128)@1 [s0; [* Method Detail]]}}&]
[s0; &]
[s5;K:`:`:Size`_`:`:Clear`(`):%- [@(0.0.255) void][@(64) _]Clear[@(64) ()]&]
[s2; Sets both dimension to 0.&]
[s3; &]
[s4; &]
[s5;K:`:`:Size`_`:`:IsEmpty`(`)const:%- [@(0.0.255) bool][@(64) _]IsEmpty[@(64) ()_][@(0.0.255) c
onst]&]
[s2; Returns [* true] if at least one dimension is 0, [* false] otherwise.&]
[s3; &]
[s4; &]
[s5;K:`:`:Size`_`:`:SetNull`(`):%- [@(0.0.255) void][@(64) _]SetNull[@(64) ()]&]
[s2; Reinitializes the object to [* Null].&]
[s3; &]
[s4; &]
[s5;K:`:`:Size`_`:`:IsNullInstance`(`)const:%- [@(0.0.255) bool][@(64) _]IsNullInstance[@(64) (
)_][@(0.0.255) const]&]
[s2; Verifies if size is [* Null].&]
[s3; &]
[s4; &]
[s5;K:`:`:Size`_`:`:GetHashValue`(`)const:%- [@(0.0.255) unsigned][@(64) _]GetHashValue[@(64) (
)_][@(0.0.255) const]&]
[s2; Returns the hash value of the size.&]
[s3; &]
[s4; &]
[s5;K:`:`:Size`_`:`:ToString`(`)const:%- [^topic`:`/`/Core`/src`/String`$en`-us^ String
]_ToString[@(64) ()_][@(0.0.255) const]&]
[s2; Returns the [* String] representation of the size.&]
[s3; &]
[s4; &]
[s5;K:`:`:Size`_`:`:operator`+`=`(`:`:Size`_`):%- [^`:`:Size`_^ Size`_]`&_operator`+`=[@(64) (
][^`:`:Size`_^@(64) Size`_][@(64) _][@3 p][@(64) )]&]
[s2; Increases [* cx] with [*@3 p]`'s horizontal dimension and [* cy] with 
[*@3 p]`'s vertical dimension.&]
[s3; &]
[s4; &]
[s5;K:`:`:Size`_`:`:operator`+`=`(T`):%- [^`:`:Size`_^ Size`_]`&_operator`+`=[@(64) (T_][@3 t
][@(64) )]&]
[s2; Increases both dimension with the same value [*@3 t].&]
[s3; &]
[s4; &]
[s5;K:`:`:Size`_`:`:operator`-`=`(`:`:Size`_`):%- [^`:`:Size`_^ Size`_]`&_operator`-`=[@(64) (
][^`:`:Size`_^@(64) Size`_][@(64) _][@3 p][@(64) )]&]
[s2; Decreases [* cx ]with [*@3 p]`'s horizontal dimension and [* cy ]with 
[*@3 p]`'s vertical dimension.&]
[s3; &]
[s4; &]
[s5;K:`:`:Size`_`:`:operator`-`=`(T`):%- [^`:`:Size`_^ Size`_]`&_operator`-`=[@(64) (][@4 T
][@(64) _][@3 t][@(64) )]&]
[s2; Decreases both dimension with the same value [*@3 t].&]
[s3; &]
[s4; &]
[s5;K:`:`:Size`_`:`:operator`*`=`(`:`:Size`_`):%- [^`:`:Size`_^ Size`_]`&_operator`*`=[@(64) (
][^`:`:Size`_^@(64) Size`_][@(64) _][@3 p][@(64) )]&]
[s2; Multiplies [* cx ]with [*@3 p]`'s horizontal dimension and [* cy ]with 
[*@3 p]`'s vertical dimension.&]
[s3; &]
[s4; &]
[s5;K:`:`:Size`_`:`:operator`*`=`(T`):%- [^`:`:Size`_^ Size`_]`&_operator`*`=[@(64) (T_][@3 t
][@(64) )]&]
[s2; Multiplies both dimension with the same value [*@3 t].&]
[s3; &]
[s4; &]
[s5;K:`:`:Size`_`:`:operator`/`=`(`:`:Size`_`):%- [^`:`:Size`_^ Size`_]`&_operator/`=[@(64) (
][^`:`:Size`_^@(64) Size`_][@(64) _][@3 p][@(64) )]&]
[s2; Divides [* cx ]with [*@3 p]`'s horizontal dimension and [* cy ]with 
[*@3 p]`'s vertical dimension.&]
[s3; &]
[s4; &]
[s5;K:`:`:Size`_`:`:operator`/`=`(T`):%- [^`:`:Size`_^ Size`_]`&_operator/`=[@(64) (T_][@3 t
][@(64) )]&]
[s2; Divides both dimension with the same value [*@3 t].&]
[s3; &]
[s4; &]
[s5;K:`:`:Size`_`:`:operator`<`<`=`(int`):%- [^`:`:Size`_^ Size`_]`&_operator<<`=[@(64) (
][@(0.0.255) int][@(64) _][@3 sh][@(64) )]&]
[s2; Shift both dimensions left by [*@3 sh].&]
[s3; &]
[s4; &]
[s5;K:`:`:Size`_`:`:operator`>`>`=`(int`):%- [^`:`:Size`_^ Size`_]`&_operator>>`=[@(64) (
][@(0.0.255) int][@(64) _][@3 sh][@(64) )]&]
[s2; Shift both dimensions right by [*@3 sh].&]
[s3; &]
[s4; &]
[s5;K:`:`:Size`_`:`:operator`+`+`(`):%- [^`:`:Size`_^ Size`_]`&_operator`+`+[@(64) ()]&]
[s2; Increments both dimensions.&]
[s3; &]
[s4; &]
[s5;K:`:`:Size`_`:`:operator`-`-`(`):%- [^`:`:Size`_^ Size`_]`&_operator`-`-[@(64) ()]&]
[s2; Decrements both dimensions.&]
[s3; &]
[s4; &]
[s5;K:`:`:Size`_`:`:operator`:`:Value`(`)const:%- [@(0.0.255) operator][@(64) _]Value[@(64) (
)_][@(0.0.255) const]&]
[s2; Returns a conversion of the size to [* Value].&]
[s3; &]
[s4; &]
[s5;K:`:`:Size`_`:`:Serialize`(`:`:Stream`&`):%- [@(0.0.255) void][@(64) _]Serialize[@(64) (
][^`:`:Stream^@(64) Stream][@(64) `&_][@3 s][@(64) )]&]
[s2; Serializes the size to a stream [*@(141.42.0) s].&]
[s0; &]
[s0; &]
[ {{10000F(128)G(128)@1 [s0; [* Friend Detail]]}}&]
[s0;* &]
[s5;K:`:`:Size`_`:`:operator`+`(`:`:Size`_`):%- [@(0.0.255) friend][@(64) _][^`:`:Size`_^@(64) S
ize`_][@(64) _]operator`+[@(64) (][^`:`:Size`_^@(64) Size`_][@(64) _][@3 s][@(64) )]&]
[s2; Returns an unmodified size [%-*@3 s].&]
[s3; &]
[s4; &]
[s5;K:`:`:Size`_`:`:operator`-`(`:`:Size`_`):%- [@(0.0.255) friend][@(64) _][^`:`:Size`_^@(64) S
ize`_][@(64) _]operator`-[@(64) (][^`:`:Size`_^@(64) Size`_][@(64) _][@3 s][@(64) )]&]
[s2; Returns a negated size [%-*@3 s].&]
[s3; &]
[s4; &]
[s5;K:`:`:Size`_`:`:operator`+`(`:`:Size`_`,`:`:Size`_`):%- [@(0.0.255) friend][@(64) _][^`:`:Size`_^@(64) S
ize`_][@(64) _]operator`+[@(64) (][^`:`:Size`_^@(64) Size`_][@(64) _][@3 a][@(64) , 
][^`:`:Size`_^@(64) Size`_][@(64) _][@3 b][@(64) )]&]
[s5;:`:`:Size`_`:`:operator`+`(`:`:Size`_`,T`):%- [@(0.0.255) friend][@(64) _][^`:`:Size`_^@(64) S
ize`_][@(64) _]operator`+[@(64) (][^`:`:Size`_^@(64) Size`_][@(64) _][@3 a][@(64) , 
T_][@3 t][@(64) )]&]
[s5;:`:`:Size`_`:`:operator`+`(T`,`:`:Size`_`):%- [@(0.0.255) friend][@(64) _][^`:`:Size`_^@(64) S
ize`_][@(64) _]operator`+[@(64) (T_][@3 t][@(64) , ][^`:`:Size`_^@(64) Size`_][@(64) _][@3 b][@(64) )
]&]
[s2; Returns the result of the addition of two Size`_ values or a 
Size`_ with single value.&]
[s3; &]
[s4; &]
[s5;K:`:`:Size`_`:`:operator`-`(`:`:Size`_`,`:`:Size`_`):%- [@(0.0.255) friend][@(64) _][^`:`:Size`_^@(64) S
ize`_][@(64) _]operator`-[@(64) (][^`:`:Size`_^@(64) Size`_][@(64) _][@3 a][@(64) , 
][^`:`:Size`_^@(64) Size`_][@(64) _][@3 b][@(64) )]&]
[s5;:`:`:Size`_`:`:operator`-`(`:`:Size`_`,T`):%- [@(0.0.255) friend][@(64) _][^`:`:Size`_^@(64) S
ize`_][@(64) _]operator`-[@(64) (][^`:`:Size`_^@(64) Size`_][@(64) _][@3 a][@(64) , 
T_][@3 t][@(64) )]&]
[s5;:`:`:Size`_`:`:operator`-`(T`,`:`:Size`_`):%- [@(0.0.255) friend][@(64) _][^`:`:Size`_^@(64) S
ize`_][@(64) _]operator`-[@(64) (T_][@3 t][@(64) , ][^`:`:Size`_^@(64) Size`_][@(64) _][@3 b][@(64) )
]&]
[s2; Returns the result of the subtraction of two Size`_ values or 
a Size`_ with single value.&]
[s3; &]
[s4; &]
[s5;K:`:`:Size`_`:`:operator`*`(`:`:Size`_`,`:`:Size`_`):%- [@(0.0.255) friend][@(64) _][^`:`:Size`_^@(64) S
ize`_][@(64) _]operator`*[@(64) (][^`:`:Size`_^@(64) Size`_][@(64) _][@3 a][@(64) , 
][^`:`:Size`_^@(64) Size`_][@(64) _][@3 b][@(64) )]&]
[s5;:`:`:Size`_`:`:operator`*`(`:`:Size`_`,T`):%- [@(0.0.255) friend][@(64) _][^`:`:Size`_^@(64) S
ize`_][@(64) _]operator`*[@(64) (][^`:`:Size`_^@(64) Size`_][@(64) _][@3 a][@(64) , 
T_][@3 b][@(64) )]&]
[s5;:`:`:Size`_`:`:operator`*`(T`,`:`:Size`_`):%- [@(0.0.255) friend][@(64) _][^`:`:Size`_^@(64) S
ize`_][@(64) _]operator`*[@(64) (T_][@3 a][@(64) , ][^`:`:Size`_^@(64) Size`_][@(64) _][@3 b][@(64) )
]&]
[s2; Returns the result of the multiplication of two Size`_ values 
or a Size`_ with single value.&]
[s3; &]
[s4; &]
[s5;K:`:`:Size`_`:`:operator`/`(`:`:Size`_`,`:`:Size`_`):%- [@(0.0.255) friend][@(64) _][^`:`:Size`_^@(64) S
ize`_][@(64) _]operator/[@(64) (][^`:`:Size`_^@(64) Size`_][@(64) _][@3 a][@(64) , 
][^`:`:Size`_^@(64) Size`_][@(64) _][@3 b][@(64) )]&]
[s5;:`:`:Size`_`:`:operator`/`(`:`:Size`_`,T`):%- [@(0.0.255) friend][@(64) _][^`:`:Size`_^@(64) S
ize`_][@(64) _]operator/[@(64) (][^`:`:Size`_^@(64) Size`_][@(64) _][@3 a][@(64) , 
T_][@3 b][@(64) )]&]
[s2; Returns the result of the division of two Size`_ values or a 
Size`_ with single value.&]
[s3; &]
[s4; &]
[s5;K:`:`:Size`_`:`:operator`<`<`(`:`:Size`_`,int`):%- [@(0.0.255) friend][@(64) _][^`:`:Size`_^@(64) S
ize`_][@(64) _]operator<<[@(64) (][^`:`:Size`_^@(64) Size`_][@(64) _][@3 a][@(64) , 
][@(0.0.255) int][@(64) _][@3 sh][@(64) )]&]
[s2; Returns the result of left shifting [*@3 a] by [*@3 sh].&]
[s3; &]
[s4; &]
[s5;K:`:`:Size`_`:`:operator`>`>`(`:`:Size`_`,int`):%- [@(0.0.255) friend][@(64) _][^`:`:Size`_^@(64) S
ize`_][@(64) _]operator>>[@(64) (][^`:`:Size`_^@(64) Size`_][@(64) _][@3 a][@(64) , 
][@(0.0.255) int][@(64) _][@3 sh][@(64) )]&]
[s2; Returns the result of right shifting [*@3 a] by [*@3 sh].&]
[s3; &]
[s4; &]
[s5;K:`:`:Size`_`:`:operator`=`=`(`:`:Size`_`,`:`:Size`_`):%- [@(0.0.255) friend][@(64) _
][@(0.0.255) bool][@(64) _]operator`=`=[@(64) (][^`:`:Size`_^@(64) Size`_][@(64) _][@3 a][@(64) ,
 ][^`:`:Size`_^@(64) Size`_][@(64) _][@3 b][@(64) )]&]
[s2; Returns [* true ]if [*@3 a ]and [*@3 b ]have their respective dimension 
equal, [* false ]otherwise.&]
[s3; &]
[s4; &]
[s5;K:`:`:Size`_`:`:operator`!`=`(`:`:Size`_`,`:`:Size`_`):%- [@(0.0.255) friend][@(64) _
][@(0.0.255) bool][@(64) _]operator!`=[@(64) (][^`:`:Size`_^@(64) Size`_][@(64) _][@3 a][@(64) ,
 ][^`:`:Size`_^@(64) Size`_][@(64) _][@3 b][@(64) )]&]
[s2; Returns [* true ]if [*@3 a ]and [*@3 b ]have at least an unequal dimension, 
[* false ]otherwise.&]
[s3; &]
[s4; &]
[s5;K:`:`:Size`_`:`:min`(`:`:Size`_`,`:`:Size`_`):%- [@(0.0.255) friend][@(64) _][^`:`:Size`_^@(64) S
ize`_][@(64) _]min[@(64) (][^`:`:Size`_^@(64) Size`_][@(64) _][@3 a][@(64) , 
][^`:`:Size`_^@(64) Size`_][@(64) _][@3 b][@(64) )]&]
[s2;%- [%% Returns a ][%%* Size`_][%%  with it`'s dimension equaling the 
minimum of the respective dimension between ][*@3 a][@3  ]and[%%  ][*@3 b].&]
[s3;%- &]
[s4;%- &]
[s5;K:`:`:Size`_`:`:max`(`:`:Size`_`,`:`:Size`_`):%- [@(0.0.255) friend][@(64) _][^`:`:Size`_^@(64) S
ize`_][@(64) _]max[@(64) (][^`:`:Size`_^@(64) Size`_][@(64) _][@3 a][@(64) , 
][^`:`:Size`_^@(64) Size`_][@(64) _][@3 b][@(64) )]&]
[s2;%- [%% Returns a ][%%* Size`_][%%  with it`'s dimension equaling the 
maximum of the respective dimension between ][*@3 a][@3  ]and[%%  ][*@3 b].&]
[s3;%- &]
[s4; &]
[s5;K:`:`:Size`_`:`:Nvl`(`:`:Size`_`,`:`:Size`_`):%- [@(0.0.255) friend][@(64) _][^`:`:Size`_^@(64) S
ize`_][@(64) _]Nvl[@(64) (][^`:`:Size`_^@(64) Size`_][@(64) _][@3 a][@(64) , 
][^`:`:Size`_^@(64) Size`_][@(64) _][@3 b][@(64) )]&]
[s2; Returns [%-*@3 b] if [%-*@3 a] is Null, [%-*@3 a] otherwise.&]
[s3; &]
[s4; &]
[s5;K:`:`:Size`_`:`:ScalarProduct`(`:`:Size`_`,`:`:Size`_`):%- [@(0.0.255) friend][@(64) _
][@4 T][@(64) _]ScalarProduct[@(64) (][^`:`:Size`_^@(64) Size`_][@(64) _][@3 a][@(64) , 
][^`:`:Size`_^@(64) Size`_][@(64) _][@3 b][@(64) )]&]
[s2; Returns the scalar product between [%-*@3 a] and [%-*@3 b].&]
[s3; &]
[s4; &]
[s5;K:`:`:Size`_`:`:VectorProduct`(`:`:Size`_`,`:`:Size`_`):%- [@(0.0.255) friend][@(64) _
][@4 T][@(64) _]VectorProduct[@(64) (][^`:`:Size`_^@(64) Size`_][@(64) _][@3 a][@(64) , 
][^`:`:Size`_^@(64) Size`_][@(64) _][@3 b][@(64) )]&]
[s2; Returns the vectorial product between [%-*@3 a] and [%-*@3 b].&]
[s3; &]
[s4; &]
[s5;K:`:`:Size`_`:`:Squared`(`:`:Size`_`):%- [@(0.0.255) friend][@(64) _][@4 T][@(64) _]Squar
ed[@(64) (][^`:`:Size`_^@(64) Size`_][@(64) _][@3 a][@(64) )]&]
[s2; Returns [%-*@3 a].cx[` 2] `+ [%-*@3 a].cy[` 2].&]
[s3; &]
[s4; &]
[s5;K:`:`:Size`_`:`:Length`(`:`:Size`_`):%- [@(0.0.255) friend][@(64) _][@(0.0.255) double][@(64) _
]Length[@(64) (][^`:`:Size`_^@(64) Size`_][@(64) _][@3 a][@(64) )]&]
[s2; Returns the hypotenuse of the triangle described by [%-*@3 a].&]
[s0; &]
[s0; ]