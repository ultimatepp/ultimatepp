topic "Callbacks";[2 $$0,0#00000000000000000000000000000000:Default]
[i448;a25;kKO9;*@(64)2 $$1,0#37138531426314131252341829483380:class]
[l288;2 $$2,0#27521748481378242620020725143825:desc]
[a83;*R6 $$3,0#31310162474203024125188417583966:caption]
[l288;i1121;b17;O9;~~~.1408;2 $$4,0#10431211400427159095818037425705:param]
[i448;a25;kKO9;*@(64)2 $$5,0#37138531426314131252341829483370:item]
[*+117 $$6,6#14700283458701402223321329925657:header]
[l288;a17;*1 $$7,7#70004532496200323422659154056402:requirement]
[{_}%EN-US 
[s3; Callbacks&]
[s0; Callbacks can be described as a very generalized form of function 
pointers. Each Callback represents some kind of action (usually 
calling a certain function or a certain object method) that can 
be invoked at any time.&]
[s0; There are several basic callback types, depending on number 
of parameters passed and return value. In order to keep description 
of callbacks short, all these types are described in a single 
`"parametrized`" description, with parameters [*/@(128.0.255) highlighted].&]
[s0; Callback types&]
[s2; &]
[ {{1514:5185:1933:1368h1;t/17b/17@(204) [s0;*/@(128.0.255)1 [%00-00 CallbackType]]
:: [s0;*/@(128.0.255)1 [%00-00 parameters]]
:: [s0;*/@(128.0.255)1 [%00-00 arguments]]
:: [s0;*/@(128.0.255)1 [%00-00 return`_type]]
::@2 [s0;*C+75 [%00-00 Callback]]
:: [s0;*/C@(0.0.255)+75 [%00-00 none]]
:: [s0;*/C@(0.0.255)+75 [%00-00 none]]
:: [s0;*/C+75 [%00-00 void]]
:: [s0;*C+75 [%00-00 Callback1]]
:: [s0;*/C+75 [%00-00 class P1]]
:: [s0;*/C+75 [%00-00 P1]]
:: [s0;*/C+75 [%00-00 void]]
:: [s0;*C+75 Callback2]
:: [s0;*/C+75 class P1, class P2]
:: [s0;*/C+75 P1, P2]
:: [s0;*/C+75 void]
:: [s0;*C+75 Callback3]
:: [s0;*/C+75 class P1, class P2, class P3]
:: [s0;*/C+75 P1, P2, P3]
:: [s0;*/C+75 void]
:: [s0;*C+75 Callback4]
:: [s0;*/C+75 class P1, class P2, class P3, class P4]
:: [s0;*/C+75 P1, P2, P3, P4]
:: [s0;*/C+75 void]
:: [s0;*C+75 Gate]
:: [s0;*/C@(0.0.255)+75 none]
:: [s0;*/C@(0.0.255)+75 none]
:: [s0;*/C+75 bool]
:: [s0;*C+75 Gate1]
:: [s0;*/C+75 class P1]
:: [s0;*/C+75 P1]
:: [s0;*/C+75 bool]
:: [s0;*C+75 Gate2]
:: [s0;*/C+75 class P1, class P2]
:: [s0;*/C+75 P1, P2]
:: [s0;*/C+75 bool]
:: [s0;*C+75 Gate3]
:: [s0;*/C+75 class P1, class P2, class P3]
:: [s0;*/C+75 P1, P2, P3]
:: [s0;*/C+75 bool]}}&]
[s0; &]
[s0; &]
[s0; `[ template_<[*/@(128.0.255) parameters]> `]&]
[s0;*/@(128.0.255) [%00-00*/@0 class_]CallbackType&]
[s2; &]
[s2; Callback type. Callbacks are moveable types with fast deep copy 
(using reference counting).&]
[s2; &]
[s0; &]
[s0; [%00-00 explicit_][*/@(128.0.255) CallbackType][%00-00 (][*/@(128.0.255) CallbackType][%00-00 A
ction<][*/@(128.0.255) arguments>][%00-00 _`*][%00-00*@3 newaction][%00-00 )]&]
[s2; Constructs callback based from new action.&]
[s4; [%00-00*C@3 newaction]-|Action. Must be created using [* new] operator. 
Callback takes over its ownership.&]
[s0; &]
[s0;:`:`:Callback1`:`:Callback1`(`): [*/@(128.0.255) CallbackType][%00-00 ()]&]
[s2; Creates an empty callback.&]
[s0; &]
[s0; [*/@(128.0.255) CallbackType][%00-00 (const_][*/@(128.0.255) CallbackType][%00-00 `&_][%00-00*@3 c
][%00-00 )]&]
[s2; Copy constructor. &]
[s4; [%00-00*C@3 c]-|Source callback.&]
[s0; &]
[s0;:`:`:Callback1`:`:`~Callback1`(`): [%00-00 `~][*/@(128.0.255) CallbackType][%00-00 ()]&]
[s2; Destructor.&]
[s0; &]
[s0;:`:`:Callback1`:`:Clear`(`): [%00-00 void_][%00-00* Clear][%00-00 ()]&]
[s2; Empties the callback.&]
[s0; &]
[s0;:`:`:Gate`:`:ClearTrue`(`): [%00-00 void_][%00-00* ClearTrue][%00-00 ()]&]
[s0; Only in Gate callbacks.&]
[s2; Clears Gate. After this method is invoked, the Gate performs 
no actions but returns true.&]
[s0; &]
[s0;:`:`:Gate`:`:ClearFalse`(`): [%00-00 void_][%00-00* ClearFalse][%00-00 ()]&]
[s0; Only in Gate callbacks.&]
[s2; Clears Gate. After this method is invoked, the Gate performs 
no actions and returns false. This is the same as Clear.&]
[s0; &]
[s0;:`:`:Callback1`:`:operator bool`(`)const: [%00-00* operator_bool][%00-00 ()_const]&]
[s4; [*/ Return value]-|True if the callback is valid. Invoking a valid 
callback invokes an action.&]
[s0; &]
[s0;:`:`:Callback1`:`:Execute`(P1`)const: [%00-00*/@(128.0.255) return`_type][%00-00 _][%00-00* E
xecute][%00-00 (][*/@(128.0.255) arguments][%00-00 )_const]&]
[s2; Executes the callback with given set of arguments.&]
[s0; &]
[s0;:`:`:Callback1`:`:operator`(`)`(P1`)const: [*/@(128.0.255) return`_type][%00-00 _][%00-00* o
perator()][%00-00 (][*/@(128.0.255) arguments][%00-00 )_const]&]
[s2; Same as the Execute method.&]
[s0; &]
[s0;:`:`:Callback1`:`:Empty`(`):*/@(128.0.255) &]
[s0; [%00-00 `[ template_<][%00-00*/@(128.0.255) parameters][%00-00 > `]]&]
[s0;* [%00-00* struct_][%00-00/@(128.0.255) CallbackType][%00-00 Action]&]
[s2; This is the abstract base class for callback action implementations. 
Callback holds reference counted pointer to instance of class 
derived from this class.&]
[s0; &]
[s0;:`:`:Callback4Action`:`:Callback4Action`(`): [%00-00*/@(128.0.255) CallbackType][%00-00* A
ction][%00-00 ()]&]
[s2; Constructor. Assigns 1 to the reference count.&]
[s0; &]
[s0;:`:`:Callback4Action`:`:`~Callback4Action`(`): [%00-00 `~][%00-00*/@(128.0.255) Callb
ackType][%00-00* Action][%00-00 ()]&]
[s2; Virtual destructor.&]
[s0; &]
[s0;:`:`:Callback4Action`:`:Execute`(P1`,P2`,P3`,P4`):@3 [%00-00@0 virtual 
][*/@(128.0.255) return`_type][%00-00@0 _][%00-00*@0 Execute][%00-00@0 (][%00-00*/@(128.0.255) a
rguments][%00-00@0 )_`=_][%00-00 0]&]
[s2; Performs the action with specified set of arguments.&]
[s0; &]
[s0; [%00-00 virtual bool_][%00-00* IsValid][%00-00 ()_const]&]
[s4; [*/ Return value]-|true if the Action is valid. Default implementation 
returns true.&]
[s0; &]
[s0;* [%00-00* Atomic_][%00-00 count]&]
[s2; Member variable used as reference count.&]
[s0; &]
[s0; Common functions creating callbacks&]
[s0;:`:`:callback`(OBJECT`*`,void`(METHOD`:`:`*`)`(P1 p1`)`): template_<class 
[*@4 OBJECT], class [*@4 METHOD][*@(128.0.255) ,][*@4  ][*/@(128.0.255) parameters]>&]
[s0; [%00-00*/@(128.0.255) CallbackType][%00-00 <][%00-00*/@(128.0.255) arguments][%00-00 > 
][%00-00* callback][%00-00 (][%00-00*@4 OBJECT][%00-00 _`*][%00-00*@3 object][%00-00 , 
void_(][%00-00*@4 METHOD][%00-00 `::`*][%00-00*@3 method][%00-00 )(][*/@(128.0.255) argument
s][%00-00 ))]&]
[s2; Creates a callback that invokes the specified method of the 
specified object.&]
[s4; [%00-00*C@3 object]-|Pointer to the object.&]
[s4; [%00-00*C@3 method]-|Pointer to the method.&]
[s4; [*/ Return value]-|Created callback.&]
[s0; &]
[s0;:`:`:callback`(OBJECT`*`,void`(METHOD`:`:`*`)`(P1 p1`)`): template_<class 
[*@4 OBJECT], class [*@4 METHOD][*@(128.0.255) ,][*@4  ][*/@(128.0.255) parameters]>&]
[s0; [%00-00*/@(128.0.255) CallbackType][%00-00 <][%00-00*/@(128.0.255) arguments][%00-00 > 
][%00-00* callback][%00-00 (const ][%00-00*@4 OBJECT][%00-00 _`*][%00-00*@3 object][%00-00 , 
void_(][%00-00*@4 METHOD][%00-00 `::`*][%00-00*@3 method][%00-00 )(][*/@(128.0.255) argument
s][%00-00 ) const)]&]
[s2; Constant variant of callback. The created Callback object invokes 
a constant method of the object.&]
[s4; [%00-00*C@3 object]-|Pointer to the object.&]
[s4; [%00-00*C@3 method]-|Pointer to the constant method to invoke.&]
[s4; [*/ Return value]-|Created callback.&]
[s0; &]
[s0;:`:`:pteback`(OBJECT`*`,void`(METHOD`:`:`*`)`(P1 p1`)`): `[ template_<[*/@(128.0.255) p
arameters]> `]&]
[s0; [%00-00*/@(128.0.255) CallbackType]<[*/@(128.0.255) arguments]> 
[%00-00* pteback][%00-00 (][%00-00*@4 OBJECT][%00-00 _`*][%00-00*@3 object][%00-00 , 
void_(][%00-00*@4 METHOD][%00-00 `::`*][%00-00*@3 method][%00-00 )(][*/@(128.0.255) argument
s][%00-00 ))]&]
[s2; Creates a Callback object which is similar to the one created 
by callback, but which can be safely invoked even after destruction 
of [@3 object] (results in no operation).&]
[s4; [%00-00*C@3 object]-|Pointer to the object. Must be an ancestor 
of the [^topic`:`/`/Core`/src`/PtePtr`$en`-us`#`:`:Pte`:`:template `<class T`> class Pte^ P
te] class.&]
[s4; [%00-00*C@3 method]-|Pointer to the method.&]
[s4; [*/ Return value]-|Created callback.&]
[s0; &]
[s0;:`:`:callback`(void`(`*`)`(P1 p1`)`): `[ template_<[*/@(128.0.255) parameters]> 
`]&]
[s0; [%00-00*/@(128.0.255) CallbackType]<[*/@(128.0.255) arguments]> 
[%00-00* callback][%00-00 (void_(`*][%00-00*@3 fn][%00-00 )(][*/@(128.0.255) arguments][%00-00 )
)]&]
[s2; Creates a Callback object that invokes a non`-member function.&]
[s4; [%00-00*C@3 fn]-|Pointer to the function.&]
[s4; [*/ Return value]-|Created callback.&]
[s0; &]
[s0; `[ template_<[*/@(128.0.255) parameters]> `]&]
[s0; [%00-00*/@(128.0.255) CallbackType]<[*/@(128.0.255) arguments]>[%00-00  
][%00-00* Proxy][%00-00 (][%00-00*/@(128.0.255) CallbackType]<[*/@(128.0.255) arguments]>
[%00-00 `&_][%00-00*@3 cb][%00-00 )]&]
[s2; Creates a Callback object that invokes another Callback object. 
Useful for callback routing (e.g. when some parent Ctrl wants 
to reroute some of its child callbacks via its interface). Note 
that this function stores a reference to the target callback 
in the created callback `- storing callbacks that are used as 
Proxy targets in Vector flavor of containers is not a good idea.&]
[s4; [%00-00*C@3 cb]-|Target callback.&]
[s4; [*/ Return value]-|Created callback.&]
[s0; &]
[s0; `[ template_<[*/@(128.0.255) parameters]> `]&]
[s0; [*/@(128.0.255) CallbackType]<[*/@(128.0.255) arguments]> [%00-00* callback][%00-00 (][*/@(128.0.255) C
allbackType]<[*/@(128.0.255) arguments]> [%00-00 _][%00-00*@3 cb1][%00-00 , 
][*/@(128.0.255) CallbackType]<[*/@(128.0.255) arguments]> [%00-00 _][%00-00*@3 cb2][%00-00 )
]&]
[s2; Creates a Callback object that invokes two other callbacks. 
Note that this function stores a reference to the target callbacks 
in the created callback `- storing callbacks that are used as 
targets here in Vector flavor of containers is not a good idea. 
For Gate callbacks, the result of this operation is the boolean 
value returned by [*@3 cb2].&]
[s4; [%00-00*C@3 cb1]-|First target callback.&]
[s4; [%00-00*C@3 cb2]-|Second target callback.&]
[s4; [*/ Return value]-|Created callback.&]
[s0; &]
[s0; `[ template_<[*/@(128.0.255) parameters]> `]&]
[s0; [*/@(128.0.255) CallbackType]<[*/@(128.0.255) arguments]>[%00-00 `&_][%00-00* operator<<
][%00-00 (][*/@(128.0.255) CallbackType]<[*/@(128.0.255) arguments]> 
[%00-00 `&_][%00-00*@3 a][%00-00 , ][*/@(128.0.255) CallbackType]<[*/@(128.0.255) arguments
]>[%00-00 _][%00-00*@3 b][%00-00 )]&]
[s2; Operator variant of the previous function. Allows chaining thus 
imitating callback insertion.&]
[s4; [%00-00*C@3 a]-|First callback. Its value is replaced by a callback 
that calls both the first and the second callback.&]
[s4; [%00-00*C@3 b]-|Second callback.&]
[s4; [*/ Return value]-|Reference to the first callback.&]
[s0;3 &]
[s0; Special functions creating callbacks&]
[s0; &]
[s0;:`:`:callback1`(Object`*`,void`(M`:`:`*`)`(P`)`,T`): [%00-00 template_<class_][%00-00*@4 O
bject][%00-00 , class_][%00-00*@4 M][%00-00 , class_][%00-00*@4 P][%00-00 , 
class_][%00-00*@4 T][%00-00 >]&]
[s0;:`:`:callback1`(Object`*`,void`(M`:`:`*`)`(P`)`,T`): [%00-00 Callback_][%00-00* callb
ack1][%00-00 (][%00-00*@4 Object][%00-00 _`*][%00-00*@3 object][%00-00 , 
void_(][%00-00*@4 M][%00-00 `::`*][%00-00*@3 method][%00-00 )(][%00-00*@4 P][%00-00 ), 
][%00-00*@4 T][%00-00 _][%00-00*@3 arg][%00-00 )]&]
[s2; Returns a no`-parameter callback for a method expecting a single 
parameter. The parameter is supplied as [@3 arg] and stored in 
the Callback.&]
[s4; [%00-00*C@3 object]-|Object.&]
[s4; [%00-00*C@3 method]-|Method pointer.&]
[s4; [%00-00*C@3 arg]-|Argument&]
[s4; [*/ Return value]-|Created callback.&]
[s0; &]
[s0;:`:`:callback1`(const Object`*`,void`(M`:`:`*`)`(P`)const`,T`): [%00-00 template_<c
lass_][%00-00*@4 Object][%00-00 , class_][%00-00*@4 M][%00-00 , class_][%00-00*@4 P][%00-00 ,
 class_][%00-00*@4 T][%00-00 >]&]
[s0;:`:`:callback1`(const Object`*`,void`(M`:`:`*`)`(P`)const`,T`): [%00-00 Callback_][%00-00* c
allback1][%00-00 (const_][%00-00*@4 Object][%00-00 _`*][%00-00*@3 object][%00-00 , 
void_(][%00-00*@4 M][%00-00 `::`*][%00-00*@3 method][%00-00 )(][%00-00*@4 P][%00-00 )_const, 
][%00-00*@4 T][%00-00 _][%00-00*@3 arg][%00-00 )]&]
[s2; Constant variant of the previous callback1 routine.&]
[s4; [%00-00*C@3 object]-|Object.&]
[s4; [%00-00*C@3 method]-|Method pointer.&]
[s4; [%00-00*C@3 arg]-|Argument&]
[s4; [*/ Return value]-|Created callback.&]
[s0;*C@4 &]
[s0;:`:`:pteback1`(Object`*`,void`(M`:`:`*`)`(P`)`,T`): [%00-00 template_<class_][%00-00*@4 O
bject][%00-00 , class_][%00-00*@4 M][%00-00 , class_][%00-00*@4 P][%00-00 , 
class_][%00-00*@4 T][%00-00 >]&]
[s0;:`:`:pteback1`(Object`*`,void`(M`:`:`*`)`(P`)`,T`): [%00-00 Callback_][%00-00* ptebac
k1][%00-00 (][%00-00*@4 Object][%00-00 _`*][%00-00*@3 object][%00-00 , 
void_(][%00-00*@4 M][%00-00 `::`*][%00-00*@3 method][%00-00 )(][%00-00*@4 P][%00-00 ), 
][%00-00*@4 T][%00-00 _][%00-00*@3 arg][%00-00 )]&]
[s2; Returns a no`-parameter callback for a method expecting a single 
parameter. Parameter is supplied as [@3 arg] and stored in the 
Callback. The created Callback object can be safely invoked even 
after the object instance is destructed.&]
[s4; [%00-00*C@3 object]-|Object. Must be an ancestor of [^topic`:`/`/Core`/src`/PtePtr`$en`-us`#`:`:Pte`:`:template `<class T`> class Pte^ P
te].&]
[s4; [%00-00*C@3 method]-|Method pointer.&]
[s4; [%00-00*C@3 arg]-|Argument&]
[s4; [*/ Return value]-|Created callback.&]
[s0; &]
[s0;:`:`:callback1`(void`(`*`)`(P`)`,T`): [%00-00 template_<class_][%00-00*@4 T][%00-00 , 
class_][%00-00*@4 P][%00-00 >]&]
[s0;:`:`:callback1`(void`(`*`)`(P`)`,T`): [%00-00 Callback_][%00-00* callback1][%00-00 (voi
d_(`*][%00-00*@3 fn][%00-00 )(][%00-00*@4 P][%00-00 ), ][%00-00*@4 T][%00-00 _][%00-00*@3 arg][%00-00 )
]&]
[s2; Returns a no`-parameter callback for a non`-member function 
expecting a single parameter. Parameter is supplied as [@3 arg] 
and stored in the Callback.&]
[s4; [%00-00*C@3 fn]-|Function.&]
[s4; [%00-00*C@3 arg]-|Argument.&]
[s4; [*/ Return value]-|Created callback.&]
[s0; &]
[s0;:`:`:callback1`(Object`*`,void`(M`:`:`*`)`(P1`,P`)`,T`): [%00-00 template_<class_][%00-00*@4 O
bject][%00-00 , class_][%00-00*@4 M][%00-00 , class_][%00-00*@4 P1][%00-00 , 
class_][%00-00*@4 P][%00-00 , class_][%00-00*@4 T][%00-00 >]&]
[s0;:`:`:callback1`(Object`*`,void`(M`:`:`*`)`(P1`,P`)`,T`): [%00-00 Callback1<][%00-00*@4 P
1][%00-00 >_][%00-00* callback1][%00-00 (][%00-00*@4 Object][%00-00 _`*][%00-00*@3 object][%00-00 ,
 void_(][%00-00*@4 M][%00-00 `::`*][%00-00*@3 method][%00-00 )(][%00-00*@4 P1][%00-00 , 
][%00-00*@4 P][%00-00 ), ][%00-00*@4 T][%00-00 _][%00-00*@3 arg][%00-00 )]&]
[s2; Returns a single`-parameter callback for a method expecting 
two parameters. Additional parameter is supplied as [@3 arg] and 
stored in the Callback.&]
[s4; [%00-00*C@3 object]-|Object.&]
[s4; [%00-00*C@3 method]-|Method pointer.&]
[s4; [%00-00*C@3 arg]-|Second argument to the method.&]
[s4; [*/ Return value]-|Created callback.&]
[s0; &]
[s0;:`:`:callback1`(const Object`*`,void`(M`:`:`*`)`(P1`,P`)const`,T`): [%00-00 templat
e_<class_][%00-00*@4 Object][%00-00 , class_][%00-00*@4 M][%00-00 , class_][%00-00*@4 P1][%00-00 ,
 class_][%00-00*@4 P][%00-00 , class_][%00-00*@4 T][%00-00 >]&]
[s0;:`:`:callback1`(const Object`*`,void`(M`:`:`*`)`(P1`,P`)const`,T`): [%00-00 Callbac
k1<][%00-00*@4 P1][%00-00 >_][%00-00* callback1][%00-00 (const_][%00-00*@4 Object][%00-00 _`*
][%00-00*@3 object][%00-00 , void_(][%00-00*@4 M][%00-00 `::`*][%00-00*@3 method][%00-00 )(][%00-00*@4 P
1][%00-00 , ][%00-00*@4 P][%00-00 )_const, ][%00-00*@4 T][%00-00 _][%00-00*@3 arg][%00-00 )]&]
[s2; Constant variant of the previous callback.&]
[s4; [%00-00*C@3 object]-|Object.&]
[s4; [%00-00*C@3 method]-|Method pointer.&]
[s4; [%00-00*C@3 arg]-|Second argument to the method.&]
[s4; [*/ Return value]-|Created callback.&]
[s0; &]
[s0;:`:`:pteback1`(Object`*`,void`(M`:`:`*`)`(P1`,P`)`,T`): [%00-00 template_<class_][%00-00*@4 O
bject][%00-00 , class_][%00-00*@4 M][%00-00 , class_][%00-00*@4 P1][%00-00 , 
class_][%00-00*@4 P][%00-00 , class_][%00-00*@4 T][%00-00 >]&]
[s0;:`:`:pteback1`(Object`*`,void`(M`:`:`*`)`(P1`,P`)`,T`): [%00-00 Callback1<][%00-00*@4 P
1][%00-00 >_][%00-00* pteback1][%00-00 (][%00-00*@4 Object][%00-00 _`*][%00-00*@3 object][%00-00 ,
 void_(][%00-00*@4 M][%00-00 `::`*][%00-00*@3 method][%00-00 )(][%00-00*@4 P1][%00-00 , 
][%00-00*@4 P][%00-00 ), ][%00-00*@4 T][%00-00 _][%00-00*@3 arg][%00-00 )]&]
[s2; Returns a single`-parameter callback for a method expecting 
two parameters. Additional parameter is supplied as [@3 arg] and 
stored in the Callback. The created Callback can be safely invoked 
even after the object instance is destructed.&]
[s4; [%00-00*C@3 object]-|Object. Must be an ancestor of [^topic`:`/`/Core`/src`/PtePtr`$en`-us`#`:`:Pte`:`:template `<class T`> class Pte^ P
te].&]
[s4; [%00-00*C@3 method]-|Method pointer.&]
[s4; [%00-00*C@3 arg]-|Second argument to the method.&]
[s0; &]
[s0;:`:`:callback1`(void`(`*`)`(P1`,P`)`,T`): [%00-00 template_<class_][%00-00*@4 T][%00-00 ,
 class_][%00-00*@4 P1][%00-00 , class_][%00-00*@4 P][%00-00 >]&]
[s0;:`:`:callback1`(void`(`*`)`(P1`,P`)`,T`): [%00-00 Callback1<][%00-00*@4 P1][%00-00 >_][%00-00* c
allback1][%00-00 (void_(`*][%00-00*@3 fn][%00-00 )(][%00-00*@4 P1][%00-00 , 
][%00-00*@4 P][%00-00 ), ][%00-00*@4 T][%00-00 _][%00-00*@3 arg][%00-00 )]&]
[s2; Returns a single`-parameter callback for a non`-member function 
expecting two parameters. Additional parameter is supplied as 
[@3 arg] and stored in the Callback.&]
[s4; [%00-00*C@3 fn]-|Function.&]
[s4; [%00-00*C@3 arg]-|Method pointer.&]
[s4; [*/ Return value]-|Created callback.&]
[s0; &]
[s0; [%00-00 template_<class_][%00-00*@4 T][%00-00 , class_][%00-00*@4 P][%00-00 >]&]
[s0; [%00-00 Callback_][%00-00* callback1][%00-00 (Callback1<][%00-00*@4 P][%00-00 >_][%00-00*@3 c
b][%00-00 , ][%00-00*@4 T][%00-00 _][%00-00*@3 arg][%00-00 )]&]
[s2; Combines a single`-parameter callback and an argument into a 
no`-parameter callback.&]
[s4; [%00-00*C@3 cb]-|Single`-parameter callback.&]
[s4; [%00-00*C@3 arg]-|Argument.&]
[s4; [*/ Return value]-|Created callback.&]
[s0; &]
[s0; [%00-00 template_<class_][%00-00*@4 T][%00-00 , class_][%00-00*@4 P1][%00-00 , 
class_][%00-00*@4 P][%00-00 >]&]
[s0; [%00-00 Callback1<][%00-00*@4 P1][%00-00 >_][%00-00* callback1][%00-00 (Callback2<][%00-00*@4 P
1][%00-00 , ][%00-00*@4 P][%00-00 >_][%00-00*@3 cb][%00-00 , ][%00-00*@4 T][%00-00 _][%00-00*@3 a
rg][%00-00 )]&]
[s2; Combines a double`-parameter callback and an argument into a 
single parameter callback.&]
[s4; [%00-00*C@3 cb]-|Double`-parameter callback.&]
[s4; [%00-00*C@3 arg]-|Argument.&]
[s4; [*/ Return value]-|Created callback.&]
[s0; &]
[s0;:`:`:callback2`(Object`*`,R`(O`:`:`*`)`(A`,B`)`,T1`,T2`): [%00-00 template_<class_][%00-00*@4 O
bject][%00-00 , class_][%00-00*@4 R][%00-00 , class_][%00-00*@4 O][%00-00 , 
class_][%00-00*@4 A][%00-00 , class_][%00-00*@4 B][%00-00 , class_][%00-00*@4 T1][%00-00 , 
class_][%00-00*@4 T2][%00-00 >]&]
[s0;:`:`:callback2`(Object`*`,R`(O`:`:`*`)`(A`,B`)`,T1`,T2`): [%00-00 Callback_][%00-00* c
allback2][%00-00 (][%00-00*@4 Object][%00-00 _`*][%00-00*@3 object][%00-00 , 
][%00-00*@4 R][%00-00 _(][%00-00*@4 O][%00-00 `::`*][%00-00*@3 method][%00-00 )(][%00-00*@4 A][%00-00 ,
 ][%00-00*@4 B][%00-00 ), ][%00-00*@4 T1][%00-00 _][%00-00*@3 arg1][%00-00 , 
][%00-00*@4 T2][%00-00 _][%00-00*@3 arg2][%00-00 )]&]
[s2; Returns a no`-parameter callback for a method taking two parameters. 
Parameters are supplied as [@3 arg1] and [@3 arg2. ]They are stored 
in the created callback object.&]
[s4; [%00-00*C@3 object]-|Object.&]
[s4; [%00-00*C@3 method]-|Method pointer.&]
[s4; [%00-00*C@3 arg1]-|First argument.&]
[s4; [%00-00*C@3 arg2]-|Second argument.&]
[s4; [*/ Return value]-|Created callback.&]
[s0; &]
[s0;:`:`:callback2`(const Object`*`,R`(O`:`:`*`)`(A`,B`)const`,T1`,T2`): [%00-00 templa
te_<class_][%00-00*@4 Object][%00-00 , class_][%00-00*@4 R][%00-00 , 
class_][%00-00*@4 O][%00-00 , class_][%00-00*@4 A][%00-00 , class_][%00-00*@4 B][%00-00 , 
class_][%00-00*@4 T1][%00-00 , class_][%00-00*@4 T2][%00-00 >]&]
[s0;:`:`:callback2`(const Object`*`,R`(O`:`:`*`)`(A`,B`)const`,T1`,T2`): [%00-00 Callba
ck_][%00-00* callback2][%00-00 (const_][%00-00*@4 Object][%00-00 _`*][%00-00*@3 object][%00-00 ,
 ][%00-00*@4 R][%00-00 _(][%00-00*@4 O][%00-00 `::`*][%00-00*@3 method][%00-00 )(][%00-00*@4 A][%00-00 ,
 ][%00-00*@4 B][%00-00 )_const, ][%00-00*@4 T1][%00-00 _][%00-00*@3 arg1][%00-00 , 
][%00-00*@4 T2][%00-00 _][%00-00*@3 arg2][%00-00 )]&]
[s2; Constant version of the previous callback.&]
[s4; [%00-00*C@3 object]-|Object.&]
[s4; [%00-00*C@3 method]-|Method.&]
[s4; [%00-00*C@3 arg1]-|First argument.&]
[s4; [%00-00*C@3 arg2]-|Second argument.&]
[s4; [*/ Return value]-|Created callback.&]
[s4;*C@3 &]
[s0;:`:`:pteback2`(Object`*`,R`(O`:`:`*`)`(A`,B`)`,T1`,T2`): [%00-00 template_<class_][%00-00*@4 O
bject][%00-00 , class_][%00-00*@4 R][%00-00 , class_][%00-00*@4 O][%00-00 , 
class_][%00-00*@4 A][%00-00 , class_][%00-00*@4 B][%00-00 , class_][%00-00*@4 T1][%00-00 , 
class_][%00-00*@4 T2][%00-00 >]&]
[s0;:`:`:pteback2`(Object`*`,R`(O`:`:`*`)`(A`,B`)`,T1`,T2`): [%00-00 Callback_][%00-00* p
teback2][%00-00 (][%00-00*@4 Object][%00-00 _`*][%00-00*@3 object][%00-00 , 
][%00-00*@4 R][%00-00 _(][%00-00*@4 O][%00-00 `::`*][%00-00*@3 method][%00-00 )(][%00-00*@4 A][%00-00 ,
 ][%00-00*@4 B][%00-00 ), ][%00-00*@4 T1][%00-00 _][%00-00*@3 arg1][%00-00 , 
][%00-00*@4 T2][%00-00 _][%00-00*@3 arg2][%00-00 )]&]
[s2; Returns a no`-parameter callback for a method taking two parameters. 
Parameter are supplied as [@3 arg1] and [@3 arg2. ]They are stored 
in the created callback. The created Callback can be safely invoked 
even after the object instance is destructed.&]
[s4; [%00-00*C@3 object]-|Object.&]
[s4; [%00-00*C@3 method]-|Method pointer.&]
[s4; [%00-00*C@3 arg1]-|First argument.&]
[s4; [%00-00*C@3 arg2]-|Second argument.&]
[s4; [*/ Return value]-|Created callback.&]
[s0; &]
[s0; &]
[s0;:`:`:callback2`(R`(`*`)`(A`,B`)`,T1`,T2`): [%00-00 template_<class_][%00-00*@4 R][%00-00 ,
 class_][%00-00*@4 A][%00-00 , class_][%00-00*@4 B][%00-00 , class_][%00-00*@4 T1][%00-00 , 
class_][%00-00*@4 T2][%00-00 >]&]
[s0;:`:`:callback2`(R`(`*`)`(A`,B`)`,T1`,T2`): [%00-00 Callback_][%00-00* callback2][%00-00 (
][%00-00*@4 R][%00-00 _(`*][%00-00*@3 fn][%00-00 )(][%00-00*@4 A][%00-00 , 
][%00-00*@4 B][%00-00 ), ][%00-00*@4 T1][%00-00 _][%00-00*@3 arg1][%00-00 , 
][%00-00*@4 T2][%00-00 _][%00-00*@3 arg2][%00-00 )]&]
[s2; Returns a no`-parameter callback for a non`-member function 
taking two parameters. Parameter are supplied as [@3 arg1] and 
[@3 arg2. ]They are stored in the created callback.&]
[s4; [%00-00*C@3 fn]-|Function pointer.&]
[s4; [%00-00*C@3 arg1]-|First argument.&]
[s4; [%00-00*C@3 arg2]-|Second argument.&]
[s4; [*/ Return value]-|Created callback.&]
[s0; &]
[s0; [%00-00 template_<class_][%00-00*@4 A][%00-00 , class_][%00-00*@4 B][%00-00 , 
class_][%00-00*@4 T1][%00-00 , class_][%00-00*@4 T2][%00-00 >]&]
[s0; [%00-00 Callback_][%00-00* callback2][%00-00 (Callback2<][%00-00*@4 A][%00-00 , 
][%00-00*@4 B][%00-00 >_][%00-00*@3 cb][%00-00 , ][%00-00*@4 T1][%00-00 _][%00-00*@3 arg1][%00-00 ,
 ][%00-00*@4 T2][%00-00 _][%00-00*@3 arg2][%00-00 )]&]
[s2; Combines a two`-parameter callback and two arguments into a 
no`-parameter callback.&]
[s4; [%00-00*C@3 cb]-|Two`-parameter callback.&]
[s4; [%00-00*C@3 arg1]-|First argument.&]
[s4; [%00-00*C@3 arg2]-|Second argument.&]
[s4; [*/ Return value]-|Created callback.&]
[s0;*C@3 &]
[s0; Callback macros&]
[s0; &]
[s0; Callback macros reduce verbosity of instance method callbacks. 
They expect CLASSNAME to be typedef`-ed as the type of current 
class and return method callbacks bound to the [*@(0.0.255) this] 
pointer.&]
[s0; &]
[s0;:`:`:THISBACK`(x`): [%00-00* THISBACK][%00-00 (][%00-00*@3 x][%00-00 )]&]
[s2; Expands to callback(this, `&CLASSNAME`::[@3 x]).&]
[s4; [%00-00*C@3 x]-|Name of method.&]
[s0; &]
[s0;:`:`:THISBACK1`(x`, arg`): [%00-00* THISBACK1][%00-00 (][%00-00*@3 x][%00-00 , 
][%00-00*@3 arg][%00-00 )]&]
[s2; Expands to callback1(this, `&CLASSNAME`::[@3 x], [@3 arg]).&]
[s4; [%00-00*C@3 x]-|Name of method.&]
[s4; [%00-00*C@3 arg]-|Additional parameter.&]
[s0; &]
[s0;:`:`:THISBACK2`(m`, a`, b`): [%00-00* THISBACK2][%00-00 (][%00-00*@3 m][%00-00 , 
][%00-00*@3 a][%00-00 , ][%00-00*@3 b][%00-00 )]&]
[s2; Expands to callback2(this, `&CLASSNAME`::[@3 x], [@3 a],[@3  b]).&]
[s4; [%00-00*C@3 m]-|Name of method.&]
[s4; [%00-00*C@3 a]-|First additional parameter.&]
[s4; [%00-00*C@3 b]-|Second additional parameter.&]
[s0; &]
[s0;:`:`:PTEBACK`(x`): [%00-00* PTEBACK][%00-00 (][%00-00*@3 x][%00-00 )]&]
[s2; Expands to pteback(this, `&CLASSNAME`::[@3 x]).&]
[s4; [%00-00*C@3 x]-|Name of method.&]
[s0; &]
[s0;:`:`:PTEBACK1`(x`, arg`): [%00-00* PTEBACK1][%00-00 (][%00-00*@3 x][%00-00 , 
][%00-00*@3 arg][%00-00 )]&]
[s2; Expands to pteback1(this, `&CLASSNAME`::[@3 x], [@3 arg]).&]
[s4; [%00-00*C@3 x]-|Name of method.&]
[s4; [%00-00*C@3 arg]-|Additional parameter.&]
[s0; &]
[s0;:`:`:PTEBACK2`(m`, a`, b`): [%00-00* PTEBACK2][%00-00 (][%00-00*@3 m][%00-00 , 
][%00-00*@3 a][%00-00 , ][%00-00*@3 b][%00-00 )]&]
[s2; Expands to pteback2(this, `&CLASSNAME`::[@3 x], [@3 a],[@3  b]).&]
[s4; [%00-00*C@3 m]-|Name of method.&]
[s4; [%00-00*C@3 a]-|First additional parameter.&]
[s4; [%00-00*C@3 b]-|Second additional parameter.&]
[s2; &]
[s0; &]
[s0; [%00-00 CallbackArgTarget]&]
[s0; &]
[s0; [%00-00 template_<class_][%00-00*@4 T][%00-00 >]&]
[s0;* [%00-00* class_][%00-00 CallbackArgTarget]&]
[s2; This helper class is useful in a situation when a set of callbacks 
define some output value. Example is pop`-up menu that provides 
selection of one character `- in that case, CallbackArgTarget 
can be used instead of dedicated method to obtain result of user 
selection.&]
[s0; [%00-00*C@4 T]-|Type of output value. Must be able to be assigned 
Null.&]
[s2; &]
[s0;:`:`:CallbackArgTarget`:`:CallbackArgTarget`(`): [%00-00* CallbackArgTarget][%00-00 (
)]&]
[s2; Constructor. Assigns Null to the output value.&]
[s2; &]
[s0;:`:`:CallbackArgTarget`:`:operator const T`&`(`): [%00-00* operator_const_T`&][%00-00 (
)]&]
[s4; [*/ Return value]-|The result. If no CallbackArgTarget was invoked, 
returns Null.&]
[s0; &]
[s0;:`:`:CallbackArgTarget`:`:IsNullInstance`(`)const: [%00-00 bool_][%00-00* IsNullInsta
nce][%00-00 ()_const]&]
[s4; [*/ Return value]-|True if there is not Null in output value.&]
[s0; &]
[s0;:`:`:CallbackArgTarget`:`:operator`[`]`(const T`&`): [%00-00 Callback_][%00-00* opera
tor`[`]][%00-00 (const_T`&_][%00-00*@3 value][%00-00 )]&]
[s2; Returns a Callback that, when invoked, assigns [@3 value] to the 
output value.&]
[s4; [%00-00*C@3 value]-|Value.&]
[s4; [*/ Return value]-|Callback.&]
[s0; ]