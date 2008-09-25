topic "Ptr and Pte";
[2 $$0,0#00000000000000000000000000000000:Default]
[i448;a25;kKO9;2 $$1,0#37138531426314131252341829483380:class]
[l288;2 $$2,0#27521748481378242620020725143825:desc]
[0 $$3,0#96390100711032703541132217272105:end]
[H6;0 $$4,0#05600065144404261032431302351956:begin]
[i448;a25;kKO9;2 $$5,0#37138531426314131252341829483370:item]
[l288;a17;*@5;1 $$6,6#70004532496200323422659154056402:requirement]
[l288;i1121;b17;O9;~~~.1408;2 $$7,0#10431211400427159095818037425705:param]
[{_}%EN-US 
[s0; Ptr and Pte&]
[s0; Ptr and Pte class templates provide a smart pointer system that 
cleared (assigned NULL) when pointed object is destructed. That 
makes it useful in situations where life`-time of object cannot 
be precisely determined.&]
[s0; While Ptr class template provides smart pointers, Pte is the 
class which adds necessary functionality to pointed objects. 
Ptr could only point to objects of Pte`-derived type. So to make 
a class `"Ptr`-able`" you should add Pte as one of class bases, 
with the derived class as its parameter, e.g.:&]
[s0; struct Foo : Pte<Foo> `{....&]
[s0; &]
[s5;K%- template_<[@(0.0.255) class][@(64) _][@4 T][@(64) >__][@(0.0.255) class][@(64) _]Pte[@(64) _
:_][@(0.0.255) public][@(64) _][^`:`:PteBase^@(64) PteBase]&]
[s0; class_[* Pte]&]
[s0;3 &]
[s0;* &]
[s2; This template class implements the functionality needed in the 
pointed object.  &]
[s0; [*C@4 T]-|Type of pointed object.&]
[s0; [/ Derived from] [^topic`:`/`/Core`/src`/PtePtr`$en`-us`#`:`:PteBase`:`:class PteBase^ P
teBase]&]
[s0; &]
[s0; &]
[s5;K%- template_<[@(0.0.255) class][@(64) _][@4 T][@(64) >__][@(0.0.255) class][@(64) _]Ptr[@(64) _
:_][@(0.0.255) public][@(64) _][^`:`:PtrBase^@(64) PtrBase][@(64) , ][@(0.0.255) private][@(64) _
][^`:`:Moveable^@(64) Moveable][@(64) <_]Ptr[@(64) <][@4 T][@(64) >_>_]&]
[s0; class_[* Ptr]&]
[s0;3 &]
[s0;* &]
[s2; Type specific pointer.&]
[s0; [*C@4 T]-|Type of pointed object. T class must be derived from 
Pte<T>.&]
[s0; [/ Derived from] [^topic`:`/`/Core`/src`/PtePtr`$en`-us`#`:`:PtrBase`:`:PtrBase`(`)^ P
trBase]&]
[s0;3 &]
[s0;:`:`:Ptr`:`:Ptr`(`): [* Ptr]()&]
[s2; Default constructor.&]
[s0;3 &]
[s0;:`:`:Ptr`:`:Ptr`(T`*`): [* Ptr](T_`*[*@3 ptr])&]
[s2; Constructs Ptr pointing to specified object.&]
[s7; [*C@3 ptr]-|C`+`+ pointer to pointed object.&]
[s0;3 &]
[s0;:`:`:Ptr`:`:Ptr`(const`:`:Ptr`&`): [* Ptr](const_[* Ptr]`&_[*@3 ptr])&]
[s2; Constructs Ptr pointing to the same object as other Ptr.&]
[s7; [*C@3 ptr]-|Other Ptr.&]
[s0;3 &]
[s5;K%- T_`*operator`->[@(64) ()_][@(0.0.255) const]&]
[s7; [*/ Return value]-|C`+`+ pointer to pointed object or NULL if Ptr 
does not point to any object.&]
[s0;3 &]
[s5;K%- T_`*operator`~[@(64) ()_][@(0.0.255) const]&]
[s7; [*/ Return value]-|C`+`+ pointer to pointed object or NULL if Ptr 
does not point to any object.&]
[s0;3 &]
[s5;K%- operator_T`*()_[@(0.0.255) const]&]
[s7; [*/ Return value]-|C`+`+ pointer to pointed object or NULL if Ptr 
does not point to any object.&]
[s0;3 &]
[s5;K%- [^`:`:Ptr^ Ptr]`&_operator`=[@(64) (T_`*][@3 ptr][@(64) )]&]
[s2; Assigns new pointer.&]
[s7; [*C@3 ptr]-|Pointer.&]
[s7; [*/ Return value]-|`*this.&]
[s0;3 &]
[s5;K%- [^`:`:Ptr^ Ptr]`&_operator`=[@(64) (][@(0.0.255) const][@(64) _][^`:`:Ptr^@(64) Ptr][@(64) `&
_][@3 ptr][@(64) )]&]
[s2; Assigns other Ptr.&]
[s7; [*C@3 ptr]-|&]
[s7; [*/ Return value]-|&]
[s0;3 &]
[s5;K%- friend_[@(0.0.255) bool][@(64) _]operator`=`=[@(64) (][@(0.0.255) const][@(64) _][^`:`:Ptr^@(64) P
tr][@(64) `&_][@3 a][@(64) , ][@(0.0.255) const][@(64) _T_`*][@3 b][@(64) )]&]
[s5;K%- friend_[@(0.0.255) bool][@(64) _]operator`=`=[@(64) (][@(0.0.255) const][@(64) _T_`*][@3 a
][@(64) , ][@(0.0.255) const][@(64) _][^`:`:Ptr^@(64) Ptr][@(64) `&_][@3 b][@(64) )]&]
[s5;K%- friend_[@(0.0.255) bool][@(64) _]operator`=`=[@(64) (][@(0.0.255) const][@(64) _][^`:`:Ptr^@(64) P
tr][@(64) `&_][@3 a][@(64) , ][@(0.0.255) const][@(64) _][^`:`:Ptr^@(64) Ptr][@(64) `&_][@3 b][@(64) )
]&]
[s5;K%- friend_[@(0.0.255) bool][@(64) _]operator`=`=[@(64) (][@(0.0.255) const][@(64) _][^`:`:Ptr^@(64) P
tr][@(64) `&_][@3 a][@(64) , T_`*][@3 b][@(64) )]&]
[s5;K%- friend_[@(0.0.255) bool][@(64) _]operator`=`=[@(64) (T_`*][@3 a][@(64) , 
][@(0.0.255) const][@(64) _][^`:`:Ptr^@(64) Ptr][@(64) `&_][@3 b][@(64) )]&]
[s5;K%- friend_[@(0.0.255) bool][@(64) _]operator!`=[@(64) (][@(0.0.255) const][@(64) _][^`:`:Ptr^@(64) P
tr][@(64) `&_][@3 a][@(64) , ][@(0.0.255) const][@(64) _T_`*][@3 b][@(64) )]&]
[s5;K%- friend_[@(0.0.255) bool][@(64) _]operator!`=[@(64) (][@(0.0.255) const][@(64) _T_`*][@3 a
][@(64) , ][@(0.0.255) const][@(64) _][^`:`:Ptr^@(64) Ptr][@(64) `&_][@3 b][@(64) )]&]
[s5;K%- friend_[@(0.0.255) bool][@(64) _]operator!`=[@(64) (][@(0.0.255) const][@(64) _][^`:`:Ptr^@(64) P
tr][@(64) `&_][@3 a][@(64) , ][@(0.0.255) const][@(64) _][^`:`:Ptr^@(64) Ptr][@(64) `&_][@3 b][@(64) )
]&]
[s2; Comparison operators.&]
[s0;3 &]
[s5;K%- [^`:`:String^ String]_ToString[@(64) ()_][@(0.0.255) const]&]
[s2; Converts all information to string for diagnostic purposes&]
[s7; [*/ Return value]-|Text.&]
[s0; ]