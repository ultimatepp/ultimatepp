topic "One";[2 $$0,0#00000000000000000000000000000000:Default]
[i448;a25;kKO9;*@(64)2 $$1,0#37138531426314131252341829483380:class]
[l288;2 $$2,0#27521748481378242620020725143825:desc]
[a83;*R6 $$3,0#31310162474203024125188417583966:caption]
[l288;i1121;b17;O9;~~~.1408;2 $$4,0#10431211400427159095818037425705:param]
[i448;a25;kKO9;*@(64)2 $$5,0#37138531426314131252341829483370:item]
[*+117 $$6,6#14700283458701402223321329925657:header]
[l288;a17;*1 $$7,7#70004532496200323422659154056402:requirement]
[{_}%EN-US 
[s3; [%00-00 One]&]
[s5;K [%00-00 template_<][%00-00@(0.0.255) class][%00-00 _][%00-00@4 T][%00-00 >__][%00-00@(0.0.255) c
lass][%00-00 _][%00-00@0 One][%00-00 _:_][%00-00@(0.0.255) private][%00-00 _][%00-00^`:`:MoveableAndDeepCopyOption^ M
oveableAndDeepCopyOption][%00-00 <_][%00-00@0 One][%00-00 <][%00-00@4 T][%00-00 >_>_]&]
[s2; &]
[s0; [%00-00*C@4 T]-|Type or base class of element stored in One.&]
[s0; One is a container capable of containing none or single element 
of type specified as template argument or derived from it.&]
[s0; One is [*/^topic`:`/`/Core`/srcdoc`/Moveable`$en`-us^ moveable][*/  
]type with [*/^topic`:`/`/Core`/srcdoc`/pick`_`$en`-us^ pick and 
optional deep copy] transfer semantics. Calling methods of picked 
One is logic error with exception of&]
[s0;C+75 void [* operator`=](pick`_ One`& [*@3 v])&]
[s0;/+75 [/C void ][*/C operator<<`=][/C (const One`& ][*/C@3 v][/C )][/  ](defined 
using DeepCopyOption template base class)&]
[s0;C+75 void [* Clear]()&]
[s0;C+75 bool [* IsPicked]() const&]
[s0; Members&]
[s0;3 &]
[s0;:`:`:One`:`:One`(`): [%00-00* One][%00-00 ()]&]
[s2; Constructor. Constructs empty One.&]
[s0;3 &]
[s0;:`:`:One`:`:One`(T`*`): [%00-00* One][%00-00 (T_`*][%00-00*@3 newt][%00-00 )]&]
[s2; Constructs One with content. Content is specified by pointer 
to object created using operator new. One takes over ownership 
of this this object. &]
[s4; [*C@3 newt]-|Object to be added.&]
[s0;3 &]
[s0;:`:`:One`:`:One`(pick`_`:`:One`<T`>`&`): [%00-00* One][%00-00 (pick`__][%00-00* One][%00-00 <
T>`&_][%00-00*@3 p][%00-00 )]&]
[s2; Pick constructor. Transfers source One in low constant time, 
but destroys it by picking. &]
[s4; [*C@3 p]-|Source One.&]
[s0;3 &]
[s0;:`:`:One`:`:One`(const`:`:One`<T`>`&`,int`): [%00-00* One][%00-00 (const_][%00-00* One][%00-00 <
T>`&_][%00-00*@3 p][%00-00 , int)]&]
[s2; Optional deep copy constructor.&]
[s7; Requires T to have deep copy constructor or optional deep copy 
constructor.&]
[s4; [*C@3 p]-|Source One.&]
[s0;3 &]
[s0;:`:`:One`:`:`~One`(`): [%00-00 `~][%00-00* One][%00-00 ()]&]
[s2; Destructor.&]
[s0;3 &]
[s5;K:`:`:One`:`:Attach`(T`*`): [%00-00@(0.0.255) void][%00-00 _][%00-00@0 Attach][%00-00 (T_
`*][%00-00@3 data][%00-00 )]&]
[s2; Assigns element to One. (Note: if there is content in One while 
calling this function, it is destroyed).&]
[s4; [%00-00*C@3 data]-|Pointer to the new element created using operator 
new.&]
[s0;3 &]
[s5;K [%00-00 T_`*][%00-00@0 Detach][%00-00 ()_pick`_]&]
[s2; Removes content giving up ownership. Client is responsible for 
deletion of content.&]
[s4; [*/ Return value]-|Pointer to the content allocated on the heap.&]
[s0;3 &]
[s5;K [%00-00 T_`*][%00-00@0 operator`-][%00-00 ()_pick`_]&]
[s2; Same as Detach() (operator version).&]
[s4; [*/ Return value]-|Pointer to the content allocated on the heap.&]
[s0;3 &]
[s5;K [%00-00@(0.0.255) void][%00-00 _][%00-00@0 Clear][%00-00 ()]&]
[s2; Removes and destroys content.&]
[s0;3 &]
[s5;K [%00-00@(0.0.255) void][%00-00 _][%00-00@0 operator`=][%00-00 (T_`*][%00-00@3 data][%00-00 )
]&]
[s2; Same as Attach(data) (operator version).&]
[s4; [%00-00*C@3 data]-|Pointer to the new element created using operator 
new.&]
[s0;3 &]
[s5;K [%00-00@(0.0.255) void][%00-00 _][%00-00@0 operator`=][%00-00 (pick`__][%00-00^`:`:One^ O
ne][%00-00 <][%00-00^T^ T][%00-00 >`&_][%00-00@3 d][%00-00 )]&]
[s2; Pick operator. Transfers source One in low constant time, but 
destroys it by picking.&]
[s4; [%00-00*C@3 d]-|Source One.&]
[s0;3 &]
[s5;K@(0.0.255) [%00-00 const][%00-00@(64) _T_`*][%00-00@0 operator`->][%00-00@(64) ()_][%00-00 c
onst]&]
[s2; Constant content access operator. Illegal if there is no content.&]
[s4; [*/ Return value]-|Returns constant pointer to content.&]
[s0;3 &]
[s5;K [%00-00 T_`*][%00-00@0 operator`->][%00-00 ()]&]
[s2; Content access operator. Illegal if there is no content.&]
[s4; [*/ Return value]-|Returns pointer to content.&]
[s0;3 &]
[s5;K@(0.0.255) [%00-00 const][%00-00@(64) _T_`*][%00-00@0 operator`~][%00-00@(64) ()_][%00-00 c
onst]&]
[s2; Constant content pointer access.&]
[s4; [*/ Return value]-|Returns constant pointer to content or NULL 
when there is no content.&]
[s0;3 &]
[s5;K [%00-00 T_`*][%00-00@0 operator`~][%00-00 ()]&]
[s2; Content pointer access.&]
[s4; [*/ Return value]-|Returns pointer to content or NULL when there 
is no content.&]
[s0;3 &]
[s5;K@(0.0.255) [%00-00 const][%00-00@(64) _T`&_][%00-00@0 operator`*][%00-00@(64) ()_][%00-00 c
onst]&]
[s2; Content constant reference access. Illegal if there is no content.&]
[s4; [*/ Return value]-|Constant reference to content.&]
[s0;3 &]
[s5;K [%00-00 T`&_][%00-00@0 operator`*][%00-00 ()]&]
[s2; Content reference access. Illegal if there is no content.&]
[s4; [*/ Return value]-|Reference to content.&]
[s0; &]
[s0; [%00-00* template <class ][%00-00*@4 TT][%00-00* >][%00-00*@4  TT][%00-00 `&_][%00-00* Creat
e][%00-00 ()]&]
[s2; Creates content in One. (Note: if there is content in One while 
calling this function, it is destroyed).&]
[s4; [*C@4 TT]-|Type of content. Must be equal to T or derived from 
it.&]
[s4; [*/ Return value]-|Reference to actual content (of type TT).&]
[s0;3 &]
[s5;K@(0.0.255) [%00-00 bool][%00-00@(64) _][%00-00@0 IsPicked][%00-00@(64) ()_][%00-00 const]&]
[s4; [*/ Return value]-|true if One is picked.&]
[s0;3 &]
[s5;K@(0.0.255) [%00-00 bool][%00-00@(64) _][%00-00@0 IsEmpty][%00-00@(64) ()_][%00-00 const]&]
[s4; [*/ Return value]-|true if there is no content.&]
[s0;3 &]
[s5;K@(0.0.255) [%00-00@(64) operator_][%00-00 bool][%00-00@(64) ()_][%00-00 const]&]
[s4; [*/ Return value]-|true if there is content.&]
[s0; ]