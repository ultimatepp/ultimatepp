topic "Ptr and Pte";[2 $$0,0#00000000000000000000000000000000:Default]
[i448;a25;kKO9;*@(64)2 $$1,0#37138531426314131252341829483380:class]
[l288;2 $$2,0#27521748481378242620020725143825:desc]
[a83;*R6 $$3,0#31310162474203024125188417583966:caption]
[l288;i1121;b17;O9;~~~.1408;2 $$4,0#10431211400427159095818037425705:param]
[i448;a25;kKO9;*@(64)2 $$5,0#37138531426314131252341829483370:item]
[*+117 $$6,6#14700283458701402223321329925657:header]
[l288;a17;*1 $$7,7#70004532496200323422659154056402:requirement]
[{_}%EN-US 
[s3; Ptr and Pte&]
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
[s5;K^`:`:PteBase^ [%00-00^^ template_<][%00-00^^@(0.0.255) class][%00-00^^ _][%00-00^^@4 T][%00-00^^ >
__][%00-00^^@(0.0.255) class][%00-00^^ _][%00-00^^@0 Pte][%00-00^^ _:_][%00-00^^@(0.0.255) p
ublic][%00-00^^ _][%00-00 PteBase]&]
[s0;* [* class_]Pte&]
[s0;3 &]
[s0;* &]
[s2; This template class implements the functionality needed in the 
pointed object.  &]
[s0; [*C@4 T]-|Type of pointed object.&]
[s0;^topic`:`/`/Core`/src`/PtePtr`$en`-us`#`:`:PteBase`:`:class PteBase^ [/^^ Derived 
from][^^  ]PteBase&]
[s0; &]
[s0; &]
[s5;K [%00-00 template_<][%00-00@(0.0.255) class][%00-00 _][%00-00@4 T][%00-00 >__][%00-00@(0.0.255) c
lass][%00-00 _][%00-00@0 Ptr][%00-00 _:_][%00-00@(0.0.255) public][%00-00 _][%00-00^`:`:PtrBase^ P
trBase][%00-00 , ][%00-00@(0.0.255) private][%00-00 _][%00-00^`:`:Moveable^ Moveable][%00-00 <
_][%00-00@0 Ptr][%00-00 <][%00-00@4 T][%00-00 >_>_]&]
[s0;* [* class_]Ptr&]
[s0;3 &]
[s0;* &]
[s2; Type specific pointer.&]
[s0; [*C@4 T]-|Type of pointed object. T class must be derived from 
Pte<T>.&]
[s0;^topic`:`/`/Core`/src`/PtePtr`$en`-us`#`:`:PtrBase`:`:PtrBase`(`)^ [/^^ Derived 
from][^^  ]PtrBase&]
[s0;3 &]
[s0;:`:`:Ptr`:`:Ptr`(`): [* Ptr]()&]
[s2; Default constructor.&]
[s0;3 &]
[s0;:`:`:Ptr`:`:Ptr`(T`*`): [* Ptr](T_`*[*@3 ptr])&]
[s2; Constructs Ptr pointing to specified object.&]
[s4; [*C@3 ptr]-|C`+`+ pointer to pointed object.&]
[s0;3 &]
[s0;:`:`:Ptr`:`:Ptr`(const`:`:Ptr`&`): [* Ptr](const_[* Ptr]`&_[*@3 ptr])&]
[s2; Constructs Ptr pointing to the same object as other Ptr.&]
[s4; [*C@3 ptr]-|Other Ptr.&]
[s0;3 &]
[s5;K@(0.0.255) [%00-00@(64) T_`*][%00-00@0 operator`->][%00-00@(64) ()_][%00-00 const]&]
[s4; [*/ Return value]-|C`+`+ pointer to pointed object or NULL if Ptr 
does not point to any object.&]
[s0;3 &]
[s5;K@(0.0.255) [%00-00@(64) T_`*][%00-00@0 operator`~][%00-00@(64) ()_][%00-00 const]&]
[s4; [*/ Return value]-|C`+`+ pointer to pointed object or NULL if Ptr 
does not point to any object.&]
[s0;3 &]
[s5;K@(0.0.255) [%00-00@(64) operator_T`*()_][%00-00 const]&]
[s4; [*/ Return value]-|C`+`+ pointer to pointed object or NULL if Ptr 
does not point to any object.&]
[s0;3 &]
[s5;K [%00-00^`:`:Ptr^ Ptr][%00-00 `&_][%00-00@0 operator`=][%00-00 (T_`*][%00-00@3 ptr][%00-00 )
]&]
[s2; Assigns new pointer.&]
[s4; [*C@3 ptr]-|Pointer.&]
[s4; [*/ Return value]-|`*this.&]
[s0;3 &]
[s5;K [%00-00^`:`:Ptr^ Ptr][%00-00 `&_][%00-00@0 operator`=][%00-00 (][%00-00@(0.0.255) const][%00-00 _
][%00-00^`:`:Ptr^ Ptr][%00-00 `&_][%00-00@3 ptr][%00-00 )]&]
[s2; Assigns other Ptr.&]
[s4; [*C@3 ptr]-|&]
[s4; [*/ Return value]-|&]
[s0;3 &]
[s5;K [%00-00 friend_][%00-00@(0.0.255) bool][%00-00 _][%00-00@0 operator`=`=][%00-00 (][%00-00@(0.0.255) c
onst][%00-00 _][%00-00^`:`:Ptr^ Ptr][%00-00 `&_][%00-00@3 a][%00-00 , ][%00-00@(0.0.255) cons
t][%00-00 _T_`*][%00-00@3 b][%00-00 )]&]
[s5;K [%00-00 friend_][%00-00@(0.0.255) bool][%00-00 _][%00-00@0 operator`=`=][%00-00 (][%00-00@(0.0.255) c
onst][%00-00 _T_`*][%00-00@3 a][%00-00 , ][%00-00@(0.0.255) const][%00-00 _][%00-00^`:`:Ptr^ P
tr][%00-00 `&_][%00-00@3 b][%00-00 )]&]
[s5;K [%00-00 friend_][%00-00@(0.0.255) bool][%00-00 _][%00-00@0 operator`=`=][%00-00 (][%00-00@(0.0.255) c
onst][%00-00 _][%00-00^`:`:Ptr^ Ptr][%00-00 `&_][%00-00@3 a][%00-00 , ][%00-00@(0.0.255) cons
t][%00-00 _][%00-00^`:`:Ptr^ Ptr][%00-00 `&_][%00-00@3 b][%00-00 )]&]
[s5;K [%00-00 friend_][%00-00@(0.0.255) bool][%00-00 _][%00-00@0 operator`=`=][%00-00 (][%00-00@(0.0.255) c
onst][%00-00 _][%00-00^`:`:Ptr^ Ptr][%00-00 `&_][%00-00@3 a][%00-00 , T_`*][%00-00@3 b][%00-00 )
]&]
[s5;K [%00-00 friend_][%00-00@(0.0.255) bool][%00-00 _][%00-00@0 operator`=`=][%00-00 (T_`*][%00-00@3 a
][%00-00 , ][%00-00@(0.0.255) const][%00-00 _][%00-00^`:`:Ptr^ Ptr][%00-00 `&_][%00-00@3 b][%00-00 )
]&]
[s5;K [%00-00 friend_][%00-00@(0.0.255) bool][%00-00 _][%00-00@0 operator!`=][%00-00 (][%00-00@(0.0.255) c
onst][%00-00 _][%00-00^`:`:Ptr^ Ptr][%00-00 `&_][%00-00@3 a][%00-00 , ][%00-00@(0.0.255) cons
t][%00-00 _T_`*][%00-00@3 b][%00-00 )]&]
[s5;K [%00-00 friend_][%00-00@(0.0.255) bool][%00-00 _][%00-00@0 operator!`=][%00-00 (][%00-00@(0.0.255) c
onst][%00-00 _T_`*][%00-00@3 a][%00-00 , ][%00-00@(0.0.255) const][%00-00 _][%00-00^`:`:Ptr^ P
tr][%00-00 `&_][%00-00@3 b][%00-00 )]&]
[s5;K [%00-00 friend_][%00-00@(0.0.255) bool][%00-00 _][%00-00@0 operator!`=][%00-00 (][%00-00@(0.0.255) c
onst][%00-00 _][%00-00^`:`:Ptr^ Ptr][%00-00 `&_][%00-00@3 a][%00-00 , ][%00-00@(0.0.255) cons
t][%00-00 _][%00-00^`:`:Ptr^ Ptr][%00-00 `&_][%00-00@3 b][%00-00 )]&]
[s2; Comparison operators.&]
[s0;3 &]
[s5;K@(0.0.255) [%00-00^`:`:String^@(64) String][%00-00@(64) _][%00-00@0 ToString][%00-00@(64) (
)_][%00-00 const]&]
[s2; Converts all information to string for diagnostic purposes&]
[s4; [*/ Return value]-|Text.&]
[s0; ]