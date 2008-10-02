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
[s5;%- template_<[@(0.0.255) class][@(64) _][@4 T][@(64) >__][@(0.0.255) class][@(64) _]Pte[@(64) _
:_][@(0.0.255) public][@(64) _][^`:`:PteBase^@(64) PteBase]&]
[s0; class_[* Pte]&]
[s0; &]
[s0;* &]
[s2; This template class implements the functionality needed in the 
pointed object.  &]
[s0; [*C@4 T]-|Type of pointed object.&]
[s0; [/ Derived from] [^topic`:`/`/Core`/src`/PtePtr`$en`-us`#`:`:PteBase`:`:class PteBase^ P
teBase]&]
[s0; &]
[s0; &]
[s3;%- &]
[s1;:noref:%- [@(0.0.255) template]_<[@(0.0.255) class]_[*@4 T]>&]
[s1;:`:`:Ptr`:`:class:%- [@(0.0.255) class]_[* Ptr]_:_[@(0.0.255) public]_[*@3 PtrBase], 
[@(0.0.255) private]_[*@3 Moveable]<_[* Ptr]<[*@4 T]>_>_&]
[s0; class_[* Ptr]&]
[s2; Type specific pointer.&]
[s0; [*C@4 T]-|Type of pointed object. T class must be derived from 
Pte<T>.&]
[s0; [/ Derived from] [^topic`:`/`/Core`/src`/PtePtr`$en`-us`#`:`:PtrBase`:`:PtrBase`(`)^ P
trBase]&]
[s3;%- &]
[s4;%- &]
[s5;:`:`:Ptr`:`:Ptr`(`):%- [* Ptr]()&]
[s2; Default constructor.&]
[s3;%- &]
[s4;%- &]
[s5;:`:`:Ptr`:`:Ptr`(T`*`):%- [* Ptr]([*@4 T]_`*[*@3 ptr])&]
[s2; Constructs Ptr pointing to specified object.&]
[s7; [*C@3 ptr]-|C`+`+ pointer to pointed object.&]
[s3;%- &]
[s4;%- &]
[s5;:`:`:Ptr`:`:Ptr`(const`:`:Ptr`&`):%- [* Ptr]([@(0.0.255) const]_[* Ptr]`&_[*@3 ptr])&]
[s2; Constructs Ptr pointing to the same object as other Ptr.&]
[s7; [*C@3 ptr]-|Other Ptr.&]
[s3;%- &]
[s4;%- &]
[s5;:`:`:Ptr`:`:operator`-`>`(`)const:%- [*@4 T]_`*[* operator`->]()_[@(0.0.255) const]&]
[s7; [*/ Return value]-|C`+`+ pointer to pointed object or NULL if Ptr 
does not point to any object.&]
[s3;%- &]
[s4;%- &]
[s5;:`:`:Ptr`:`:operator`~`(`)const:%- [*@4 T]_`*[* operator`~]()_[@(0.0.255) const]&]
[s7; [*/ Return value]-|C`+`+ pointer to pointed object or NULL if Ptr 
does not point to any object.&]
[s3;%- &]
[s4;%- &]
[s5;:`:`:Ptr`:`:operator T`*`(`)const:%- [@(0.0.255) operator]_[*@4 T]`*()_[@(0.0.255) cons
t]&]
[s7; [*/ Return value]-|C`+`+ pointer to pointed object or NULL if Ptr 
does not point to any object.&]
[s3;%- &]
[s4;%- &]
[s5;:`:`:Ptr`:`:operator`=`(T`*`):%- [_^`:`:Ptr^ Ptr]`&_[* operator`=]([*@4 T]_`*[*@3 ptr])&]
[s2; Assigns new pointer.&]
[s7; [*C@3 ptr]-|Pointer.&]
[s7; [*/ Return value]-|`*this.&]
[s3;%- &]
[s4;%- &]
[s5;:`:`:Ptr`:`:operator`=`(const`:`:Ptr`&`):%- [_^`:`:Ptr^ Ptr]`&_[* operator`=]([@(0.0.255) c
onst]_[_^`:`:Ptr^ Ptr]`&_[*@3 ptr])&]
[s2; Assigns other Ptr.&]
[s7; [*C@3 ptr]-|Pointer.&]
[s7; [*/ Return value]-|`*this.&]
[s3;%- &]
[s4;%- &]
[s5;:`:`:Ptr`:`:operator`=`=`(const`:`:Ptr`&`,const T`*`):%- [@(0.0.255) friend]_[@(0.0.255) b
ool]_[* operator`=`=]([@(0.0.255) const]_[_^`:`:Ptr^ Ptr]`&_[*@3 a], 
[@(0.0.255) const]_[*@4 T]_`*[*@3 b])&]
[s5;:`:`:Ptr`:`:operator`=`=`(const T`*`,const`:`:Ptr`&`):%- [@(0.0.255) friend]_[@(0.0.255) b
ool]_[* operator`=`=]([@(0.0.255) const]_[*@4 T]_`*[*@3 a], [@(0.0.255) const]_[_^`:`:Ptr^ Pt
r]`&_[*@3 b])&]
[s5;:`:`:Ptr`:`:operator`=`=`(const`:`:Ptr`&`,const`:`:Ptr`&`):%- [@(0.0.255) friend]_[@(0.0.255) b
ool]_[* operator`=`=]([@(0.0.255) const]_[_^`:`:Ptr^ Ptr]`&_[*@3 a], 
[@(0.0.255) const]_[_^`:`:Ptr^ Ptr]`&_[*@3 b])&]
[s5;:`:`:Ptr`:`:operator`=`=`(const`:`:Ptr`&`,T`*`):%- [@(0.0.255) friend]_[@(0.0.255) bo
ol]_[* operator`=`=]([@(0.0.255) const]_[_^`:`:Ptr^ Ptr]`&_[*@3 a], [*@4 T]_`*[*@3 b])&]
[s5;:`:`:Ptr`:`:operator`=`=`(T`*`,const`:`:Ptr`&`):%- [@(0.0.255) friend]_[@(0.0.255) bo
ol]_[* operator`=`=]([*@4 T]_`*[*@3 a], [@(0.0.255) const]_[_^`:`:Ptr^ Ptr]`&_[*@3 b])&]
[s5;:`:`:Ptr`:`:operator`!`=`(const`:`:Ptr`&`,const T`*`):%- [@(0.0.255) friend]_[@(0.0.255) b
ool]_[* operator!`=]([@(0.0.255) const]_[_^`:`:Ptr^ Ptr]`&_[*@3 a], [@(0.0.255) const]_[*@4 T
]_`*[*@3 b])&]
[s5;:`:`:Ptr`:`:operator`!`=`(const T`*`,const`:`:Ptr`&`):%- [@(0.0.255) friend]_[@(0.0.255) b
ool]_[* operator!`=]([@(0.0.255) const]_[*@4 T]_`*[*@3 a], [@(0.0.255) const]_[_^`:`:Ptr^ Ptr
]`&_[*@3 b])&]
[s5;:`:`:Ptr`:`:operator`!`=`(const`:`:Ptr`&`,const`:`:Ptr`&`):%- [@(0.0.255) friend]_[@(0.0.255) b
ool]_[* operator!`=]([@(0.0.255) const]_[_^`:`:Ptr^ Ptr]`&_[*@3 a], [@(0.0.255) const]_[_^`:`:Ptr^ P
tr]`&_[*@3 b])&]
[s2; Comparison operators.&]
[s3;%- &]
[s4;%- &]
[s5;:`:`:Ptr`:`:ToString`(`)const:%- [_^`:`:String^ String]_[* ToString]()_[@(0.0.255) cons
t]&]
[s2; Converts all information to string for diagnostic purposes&]
[s7; [*/ Return value]-|Text.&]
[s0; ]