topic "Index";
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
[ {{10000@(113.42.0) [s0; [*@7;4 Index]]}}&]
[s3; &]
[s1;:noref:%- [@(0.0.255)3 template][3 _<][@(0.0.255)3 class][3 _][*@4;3 T][3 , 
][@(0.0.255)3 class][3 _][*@4;3 HashFn][3 _`=_StdHash][@(0.0.255)3 <][*@4;3 T][@(0.0.255)3 >][3 _
>]&]
[s1;:Index`:`:class:%- [@(0.0.255) class]_[* Index]_:_[@(0.0.255) private]_[*@3 MoveableAndDee
pCopyOption][@(0.0.255) <]_[* Index][@(0.0.255) <][*@4 T], [*@4 HashFn]_>_>, 
[@(0.0.255) public]_[*@3 AIndex][@(0.0.255) <][*@4 T], [_^Vector^ Vector][@(0.0.255) <][*@4 T][@(0.0.255) >
], [*@4 HashFn][@(0.0.255) >]_&]
[s8; [*@4 T]-|Type of elements stored in Index. T is required to be 
[/^topic`:`/`/Core`/srcdoc`/Moveable`$en`-us^ moveable][/  ]and must 
have [/^topic`:`/`/Core`/srcdoc`/pick`_`$en`-us^ deep copy constructor] 
and deep copy assignment.&]
[s8; [*@4 HashFn]-|Hashing class. Must have unsigned operator()(const 
T`& x) method defined, returning hash value for elements. Defaults 
to StdHash<T> which requires unsigned GetHashValue(const T`&) 
function returning hash value of elements to be defined.&]
[s9; Vector flavor of index. Inherits most of its functionality from 
[^topic`:`/`/Core`/src`/AIndex`$en`-us^ AIndex] and adds only members 
specific for its flavor.&]
[s9; Like any other NTL container, Index is a [*/^topic`:`/`/Core`/srcdoc`/Moveable`$en`-us^ m
oveable][*/  ]type with [*/^topic`:`/`/Core`/srcdoc`/pick`_`$en`-us^ pick 
and optional deep copy] transfer semantics. Calling methods of 
picked Index is logic error with exception of&]
[s0; [C+75 void ][*C+75 operator`=][C+75 (pick`_ Index`& ][*C@3+75 v][C+75 ) 
][/+75 (defined by composition)]&]
[s0; [C+75 void ][*C+75 operator<<`=][C+75 (const AIndex`& ][*C@3+75 v][C+75 )][+75  
][/+75 (defined in AIndex)]&]
[s0; [C+75 void ][*C+75 Clear][C+75 ()]&]
[s0; [C+75 bool ][*C+75 IsPicked][C+75 () const]&]
[s9; Optional deep copy operator is inherited from AIndex class. 
Pick operator is implicitly defined by composition.&]
[s0; &]
[s0; [* Base class]&]
[s0; [^topic`:`/`/Core`/src`/AIndex`$en`-us`#AIndex`:`:class^ AIndex<T, 
Vector<T>, HashFn>]&]
[s3; &]
[s0;%- &]
[ {{10000F(128)G(128)@1 [s0; [* Constructor Detail]]}}&]
[s3;%- &]
[s5;:Index`:`:Index`(`):%- [* Index]()&]
[s2; Constructs empty Index.&]
[s3; &]
[s4;%- &]
[s5;:Index`:`:Index`(pick`_ Index`&`):%- [* Index]([@(0.128.128) pick`_]_[* Index][@(0.0.255) `&
]_[*@3 s])&]
[s2; Pick constructor. Transfers source Index in low constant time, 
but destroys it by picking.&]
[s7; [*C@3 s]-|Source Index.&]
[s3; &]
[s4;%- &]
[s5;:Index`:`:Index`(pick`_ Vector`<T`>`&`):%- [* Index]([@(0.128.128) pick`_]_[_^Vector^ V
ector][@(0.0.255) <][*@4 T][@(0.0.255) >`&]_[*@3 s])&]
[s2; Pick operator. Transfers source Vector to Index in low constant 
time, but destroys it by picking.&]
[s7; [*C@3 x]-|Source Vector.&]
[s3; &]
[s4;%- &]
[s5;:Index`:`:Index`(const Index`&`,int`):%- [* Index]([@(0.0.255) const]_[* Index][@(0.0.255) `&
]_[*@3 s], [@(0.0.255) int])&]
[s2; Optional deep copy constructor.&]
[s6; Requires T to have deep copy constructor or optional deep copy 
constructor.&]
[s7; [*C@3 s]-|Source Index.&]
[s3; &]
[s4;%- &]
[s5;:Index`:`:Index`(const Vector`<T`>`&`,int`):%- [* Index]([@(0.0.255) const]_[_^Vector^ V
ector][@(0.0.255) <][*@4 T][@(0.0.255) >`&]_[*@3 s], [@(0.0.255) int])&]
[s2; Deep`-copy constructs Index from Vector.&]
[s6; Requires T to have deep copy constructor or optional deep copy 
constructor.&]
[s7; [*@3 s]-|Source Vector.&]
[s3; &]
[s0; &]
[ {{10000F(128)G(128)@1 [s0; [* Public Member List]]}}&]
[s3;%- &]
[s5;:Index`:`:Pop`(`):%- [*@4 T]_[* Pop]()&]
[s2; Drops last element of the Index and returns its value.&]
[s6; Requires T to have deep copy constructor.&]
[s7; [*/ Return value]-|Value of the dropped element.&]
[s3; &]
[s4;%- &]
[s5;:Index`:`:operator`=`(pick`_ Vector`<T`>`&`):%- [_^Index^ Index][@(0.0.255) `&]_[* oper
ator`=]([@(0.128.128) pick`_]_[_^Vector^ Vector][@(0.0.255) <][*@4 T][@(0.0.255) >`&]_[*@3 x])
&]
[s2; Pick operator. Transfers source Vector to Index in low constant 
time, but destroys it by picking.&]
[s7; [*@3 x]-|Source Vector.&]
[s3; &]
[s0; ]