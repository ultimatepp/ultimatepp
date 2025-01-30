topic "Callbacks";
[i448;a25;kKO9;2 $$1,0#37138531426314131252341829483380:class]
[l288;2 $$2,2#27521748481378242620020725143825:desc]
[0 $$3,0#96390100711032703541132217272105:end]
[H6;0 $$4,0#05600065144404261032431302351956:begin]
[i448;a25;kKO9;2 $$5,0#37138531426314131252341829483370:item]
[l288;a4;*@5;1 $$6,6#70004532496200323422659154056402:requirement]
[l288;i1121;b17;O9;~~~.1408;2 $$7,0#10431211400427159095818037425705:param]
[i448;b42;O9;2 $$8,8#61672508125594000341940100500538:tparam]
[b42;2 $$9,9#13035079074754324216151401829390:normal]
[2 $$0,0#00000000000000000000000000000000:Default]
[{_}%EN-US 
[ {{10000@(113.42.0) [s0; [*@7;4 Callbacks]]}}&]
[s9; [*/^topic`:`/`/Core`/src`/Function`_en`-us^ Callbacks are now 
deprecated. Use Function/Event/Gate instead!]&]
[s9; Callbacks can be described as a very generalized form of function 
pointers. Each Callback represents some kind of action (usually 
calling a certain function or a certain object method) that can 
be invoked at any time.&]
[s9; If you are happy enough to be able to use C`+`+11, you can also 
assign C`+`+ lambda to Callback using operator << (see bellow).&]
[s9; There are several basic callback types, depending on number 
of parameters passed and return value. In order to keep description 
of callbacks short, all these types are described in a single 
`"parametrized`" description, with parameters [*/@(128.0.255) highlighted].&]
[s9; Generally, callbacks are now defined for up to 5 parameters 
of target (function, method, another callback).&]
[s0; &]
[s0; Callback types:&]
[s3; &]
[ {{1514:5185:1933:1368h1;t/17b/17@(204) [s0;%- [*/@(128.0.255)1 CallbackType]]
:: [s0;%- [*/@(128.0.255)1 parameters]]
:: [s0;%- [*/@(128.0.255)1 arguments]]
:: [s0;%- [*/@(128.0.255)1 return`_type]]
::@2 [s0;:Callback`:`:class:%- [*C+75 Callback]]
:: [s0;%- [*/C@(0.0.255)+75 none]]
:: [s0;%- [*/C@(0.0.255)+75 none]]
:: [s0;%- [*/C+75 void]]
:: [s0;:Callback1`:`:class:%- [*C+75 Callback1]]
:: [s0;%- [*/C+75 class P1]]
:: [s0;%- [*/C+75 P1]]
:: [s0;%- [*/C+75 void]]
:: [s0;:Callback2`:`:class: [*C+75 Callback2]]
:: [s0; [*/C+75 class P1, class P2]]
:: [s0; [*/C+75 P1, P2]]
:: [s0; [*/C+75 void]]
:: [s0;:Callback3`:`:class: [*C+75 Callback3]]
:: [s0; [*/C+75 class P1, class P2, class P3]]
:: [s0; [*/C+75 P1, P2, P3]]
:: [s0; [*/C+75 void]]
:: [s0;:Callback4`:`:class: [*C+75 Callback4]]
:: [s0; [*/C+75 class P1, class P2, class P3, class P4]]
:: [s0; [*/C+75 P1, P2, P3, P4]]
:: [s0; [*/C+75 void]]
:: [s0;:Callback5`:`:class: [*C+75 Callback5]]
:: [s0; [*/C+75 class P1, ... , class P5]]
:: [s0; [*/C+75 P1, ... , P5]]
:: [s0; [*/C+75 void]]}}&]
[s0; &]
[s0; `[ template_<[*/@(128.0.255) parameters]> `]&]
[s0; [%- class_][*/@(128.0.255) CallbackType]&]
[s2; &]
[s2; Callback type. Callbacks are moveable types with fast deep copy 
(using reference counting).&]
[s2; &]
[s0; &]
[s0;%- explicit_[%%*/@(128.0.255) CallbackType]([%%*/@(128.0.255) CallbackType]Action<[%%*/@(128.0.255) a
rguments>]_`*[*@3 newaction])&]
[s2; Constructs callback based from new action.&]
[s7; [%-*C@3 newaction]-|Action. Must be created using [* new] operator. 
Callback takes over its ownership.&]
[s3; &]
[s0; &]
[s0;:Callback1`:`:Callback1`(`):%- [%%*/@(128.0.255) CallbackType]()&]
[s2; Creates an empty callback.&]
[s3; &]
[s4; &]
[s0;:Callback1`:`:Callback1`(const Callback1`&`):%- [%%*/@(128.0.255) CallbackType](con
st_[%%*/@(128.0.255) CallbackType]`&_[*@3 c])&]
[s2; Copy constructor. &]
[s7; [%-*C@3 c]-|Source callback.&]
[s3; &]
[s4; &]
[s0;:`:`:Callback1`:`:`~Callback1`(`):%- `~[%%*/@(128.0.255) CallbackType]()&]
[s2; Destructor.&]
[s3; &]
[s4; &]
[s0;:`:`:Callback1`:`:Clear`(`):%- void_[* Clear]()&]
[s2; Empties the callback.&]
[s3; &]
[s4; &]
[s0;:`:`:Gate`:`:ClearTrue`(`):%- void_[* ClearTrue]()&]
[s6; Only in Gate callbacks.&]
[s2; Clears Gate. After this method is invoked, the Gate performs 
no actions but returns true.&]
[s3; &]
[s4; &]
[s0;:`:`:Gate`:`:ClearFalse`(`):%- void_[* ClearFalse]()&]
[s6; Only in Gate callbacks.&]
[s2; Clears Gate. After this method is invoked, the Gate performs 
no actions and returns false. This is the same as Clear.&]
[s3; &]
[s4; &]
[s0;:`:`:Callback1`:`:operator bool`(`)const:%- [* operator_bool]()_const&]
[s7; [*/ Return value]-|True if the callback is valid. Invoking a valid 
callback invokes an action.&]
[s3; &]
[s4; &]
[s0;:`:`:Callback1`:`:Execute`(P1`)const:%- [*/@(128.0.255) return`_type]_[* Execute]([%%*/@(128.0.255) a
rguments])_const&]
[s2; Executes the callback with given set of arguments.&]
[s3; &]
[s4; &]
[s0;:`:`:Callback1`:`:operator`(`)`(P1`)const:%- [%%*/@(128.0.255) return`_type]_[* opera
tor()]([%%*/@(128.0.255) arguments])_const&]
[s2; Same as the Execute method.&]
[s3; &]
[s0; &]
[s0;:Callback1Action`:`:struct:%- `[ template_<[*/@(128.0.255) parameters]> 
`]&]
[s0;%- struct_[*/@(128.0.255) CallbackType][* Action]&]
[s2; This is the abstract base class for callback action implementations. 
Callback holds reference counted pointer to instance of class 
derived from this class.&]
[s3; &]
[s0; &]
[s0;:Callback1Action`:`:Callback1Action`(`):%- [*/@(128.0.255) CallbackType][* Action]()&]
[s2; Constructor. Assigns 1 to the reference count.&]
[s3; &]
[s4; &]
[s0;:Callback1Action`:`:`~Callback1Action`(`):%- `~[*/@(128.0.255) CallbackType][* Action
]()&]
[s2; Virtual destructor.&]
[s3; &]
[s4; &]
[s0;:Callback1Action`:`:Execute`(P1`):%- virtual [%%*/@(128.0.255) return`_type]_[* Execu
te]([*/@(128.0.255) arguments])_`=_[@3 0]&]
[s2; Performs the action with specified set of arguments.&]
[s3; &]
[s4; &]
[s0;:Callback1Action`:`:IsValid`(`)const:%- virtual bool_[* IsValid]()_const&]
[s7; [*/ Return value]-|true if the Action is valid. Default implementation 
returns true.&]
[s3; &]
[s4; &]
[s0;:Callback1Action`:`:count:%- Atomic_[* count]&]
[s2; Member variable used as reference count.&]
[s3; &]
[s0;%- &]
[s0;%- &]
[s0;%- &]
[ {{10000@(113.42.0) [s0; [*@7;4 Common Callback/Gate functions]]}}&]
[s3; &]
[s0;:`:`:callback`(OBJECT`*`,void`(METHOD`:`:`*`)`(P1 p1`)`): template_<class 
[*@4 OBJECT], class [*@4 METHOD][*@(128.0.255) ,][*@4  ][*/@(128.0.255) parameters]>&]
[s0;%- [*/@(128.0.255) CallbackType]<[*/@(128.0.255) arguments]> [* callback]([*@4 OBJECT]_`*
[*@3 object], void_([*@4 METHOD]`::`*[*@3 method])([%%*/@(128.0.255) arguments]))&]
[s2; Creates a callback that invokes the specified method of the 
specified object.&]
[s7; [%-*C@3 object]-|Pointer to the object.&]
[s7; [%-*C@3 method]-|Pointer to the method.&]
[s7; [*/ Return value]-|Created callback.&]
[s3; &]
[s4; &]
[s0;:`:`:callback`(OBJECT`*`,void`(METHOD`:`:`*`)`(P1 p1`)`): template_<class 
[*@4 OBJECT], class [*@4 METHOD][*@(128.0.255) ,][*@4  ][*/@(128.0.255) parameters]>&]
[s0;%- [*/@(128.0.255) CallbackType]<[*/@(128.0.255) arguments]> [* callback](const 
[*@4 OBJECT]_`*[*@3 object], void_([*@4 METHOD]`::`*[*@3 method])([%%*/@(128.0.255) argument
s]) const)&]
[s2; Constant variant of callback. The created Callback object invokes 
a constant method of the object.&]
[s7; [%-*C@3 object]-|Pointer to the object.&]
[s7; [%-*C@3 method]-|Pointer to the constant method to invoke.&]
[s7; [*/ Return value]-|Created callback.&]
[s3; &]
[s4; &]
[s0;:`:`:pteback`(OBJECT`*`,void`(METHOD`:`:`*`)`(P1 p1`)`): `[ template_<[*/@(128.0.255) p
arameters]> `]&]
[s0;%- [*/@(128.0.255) CallbackType][%% <][%%*/@(128.0.255) arguments][%% > 
][* pteback]([*@4 OBJECT]_`*[*@3 object], void_([*@4 METHOD]`::`*[*@3 method])([%%*/@(128.0.255) a
rguments]))&]
[s2; Creates a Callback object which is similar to the one created 
by callback, but which can be safely invoked even after destruction 
of [@3 object] (results in no operation).&]
[s7; [%-*C@3 object]-|Pointer to the object. Must be an ancestor of 
the [^topic`:`/`/Core`/src`/PtePtr`$en`-us`#`:`:Pte`:`:template `<class T`> class Pte^ P
te] class.&]
[s7; [%-*C@3 method]-|Pointer to the method.&]
[s7; [*/ Return value]-|Created callback.&]
[s3; &]
[s4; &]
[s0;:`:`:callback`(void`(`*`)`(P1 p1`)`): `[ template_<[*/@(128.0.255) parameters]> 
`]&]
[s0;%- [*/@(128.0.255) CallbackType][%% <][%%*/@(128.0.255) arguments][%% > 
][* callback](void_(`*[*@3 fn])([%%*/@(128.0.255) arguments]))&]
[s2; Creates a Callback object that invokes a non`-member function.&]
[s7; [%-*C@3 fn]-|Pointer to the function.&]
[s7; [*/ Return value]-|Created callback.&]
[s3; &]
[s4; &]
[s0;:Proxy`(Callback1`<P1`>`&`): `[ template_<[*/@(128.0.255) parameters]> 
`]&]
[s0;%- [*/@(128.0.255) CallbackType][%% <][%%*/@(128.0.255) arguments][%% >] 
[* Proxy]([*/@(128.0.255) CallbackType][%% <][%%*/@(128.0.255) arguments][%% >]`&_[*@3 cb])&]
[s2; Creates a Callback object that invokes another Callback object. 
Useful for callback routing (e.g. when some parent Ctrl wants 
to reroute some of its child callbacks via its interface). Note 
that this function stores a reference to the target callback 
in the created callback `- storing callbacks that are used as 
Proxy targets in Vector flavor of containers is not a good idea.&]
[s7; [%-*C@3 cb]-|Target callback.&]
[s7; [*/ Return value]-|Created callback.&]
[s3; &]
[s4; &]
[s0;:callback`(Callback1`<P1`>`,Callback1`<P1`>`): `[ template_<[*/@(128.0.255) paramet
ers]> `]&]
[s0;%- [%%*/@(128.0.255) CallbackType][%% <][%%*/@(128.0.255) arguments][%% > 
][* callback]([%%*/@(128.0.255) CallbackType][%% <][%%*/@(128.0.255) arguments][%% > 
]_[*@3 cb1], [%%*/@(128.0.255) CallbackType][%% <][%%*/@(128.0.255) arguments][%% > 
]_[*@3 cb2])&]
[s2; Creates a Callback object that invokes two other callbacks. 
Note that this function stores a reference to the target callbacks 
in the created callback `- storing callbacks that are used as 
targets here in Vector flavor of containers is not a good idea. 
For Gate callbacks, the result of this operation is the boolean 
value returned by [*@3 cb2].&]
[s7; [%-*C@3 cb1]-|First target callback.&]
[s7; [%-*C@3 cb2]-|Second target callback.&]
[s7; [*/ Return value]-|Created callback.&]
[s3; &]
[s4; &]
[s0;:operator`<`<`(Callback1`<P1`>`&`,Callback1`<P1`>`): `[ template_<[*/@(128.0.255) p
arameters]> `]&]
[s0;%- [%%*/@(128.0.255) CallbackType][%% <][%%*/@(128.0.255) arguments][%% >]`&_[* operator<<](
[%%*/@(128.0.255) CallbackType][%% <][%%*/@(128.0.255) arguments][%% > 
]`&_[*@3 a], [%%*/@(128.0.255) CallbackType][%% <][%%*/@(128.0.255) arguments][%% >]_[*@3 b])
&]
[s2; Operator variant of the previous function. Allows chaining thus 
imitating callback insertion.&]
[s7; [%-*C@3 a]-|First callback. Its value is replaced by a callback 
that calls both the first and the second callback.&]
[s7; [%-*C@3 b]-|Second callback.&]
[s7; [*/ Return value]-|Reference to the first callback.&]
[s3; &]
[s4;%- &]
[s0;:operator`<`<`(Callback1`<P1`>`&`,Callback1`<P1`>`): `[ template_<[*/@(128.0.255) p
arameters]> `]&]
[s0;%- [%%*/@(128.0.255) CallbackType][%% <][%%*/@(128.0.255) arguments][%% >]`&_[* operator<<](
[%%*/@(128.0.255) CallbackType][%% <][%%*/@(128.0.255) arguments][%% > 
]`&_[*@3 a], [%%*/@(128.0.255) C`+`+11Lambda]_[*@3 b])&]
[s2; Variant of merging callbacks that can be used for C`+`+11 lambdas.&]
[s3; &]
[s0; &]
[ {{10000@(113.42.0) [s0; [*@7;4 Storing arguments in callback]]}}&]
[s0; &]
[s0; It is possible to store some arguments that are passed to target 
(function or method) as parameters when creating. If simultaneously 
passing other parameters in callback call, those passed on call 
represent beginning arguments and those defined in callback creation 
fill the rest of arguments.&]
[s0; &]
[s0; Again, all this is defined for up to 5 target parameters:&]
[s0; &]
[s0;:`:`:callback1`(Object`*`,void`(M`:`:`*`)`(P`)`,T`):%- template_<class_[*@4 Object],
 class_[*@4 M], class_[*@4 P1], ..., class_[*@4 T1], ...>&]
[s0;:`:`:callback1`(Object`*`,void`(M`:`:`*`)`(P`)`,T`):%- [%%*/@(128.0.255) CallbackTy
pe][%% <][%%*/@(128.0.255) arguments][%% >]_[* callback1]([*@4 Object]_`*[*@3 object], 
void_([*@4 M]`::`*[*@3 method])([*@4 P1], ...), [*@4 T1]_[*@3 arg1], ...)&]
[s3;%- &]
[s0;:`:`:callback1`(const Object`*`,void`(M`:`:`*`)`(P`)const`,T`):%- template_<class
_[*@4 Object], class_[*@4 M], class_[*@4 P1], ..., class_[*@4 T1], ...>&]
[s0;:`:`:callback1`(const Object`*`,void`(M`:`:`*`)`(P`)const`,T`):%- [%%*/@(128.0.255) C
allbackType][%% <][%%*/@(128.0.255) arguments][%% >]_[* callback1]([*@4 Object]_`*[*@3 object
], void_([*@4 M]`::`*[*@3 method])([*@4 P1], ...) const, [*@4 T1]_[*@3 arg1], 
...)&]
[s3;%- &]
[s0;:`:`:pteback1`(Object`*`,void`(M`:`:`*`)`(P`)`,T`):%- template_<class_[*@4 Object],
 class_[*@4 M], class_[*@4 P1], ..., class_[*@4 T1], ...>&]
[s0;:`:`:pteback1`(Object`*`,void`(M`:`:`*`)`(P`)`,T`):%- [%%*/@(128.0.255) CallbackTyp
e][%% <][%%*/@(128.0.255) arguments][%% >]_[* pteback1]([*@4 Object]_`*[*@3 object], 
void_([*@4 M]`::`*[*@3 method])([*@4 P1], ...), [*@4 T1]_[*@3 arg1], ...)&]
[s3;%- &]
[s0;:`:`:pteback1`(Object`*`,void`(M`:`:`*`)`(P`)const`,T`):%- template_<class_[*@4 Obj
ect], class_[*@4 M], class_[*@4 P1], ..., class_[*@4 T1], ...>&]
[s0;:`:`:pteback1`(Object`*`,void`(M`:`:`*`)`(P`)const`,T`):%- [%%*/@(128.0.255) Callba
ckType][%% <][%%*/@(128.0.255) arguments][%% >]_[* pteback1]([*@4 Object]_`*[*@3 object], 
void_([*@4 M]`::`*[*@3 method])([*@4 P1], ...) const, [*@4 T1]_[*@3 arg1], 
...)&]
[s3;%- &]
[s0;:`:`:callback1`(void`(`*`)`(P`)`,T`):%- template_<class_[*@4 P1], 
..., class_[*@4 T1], ...>&]
[s0;:`:`:callback1`(void`(`*`)`(P`)`,T`):%- [%%*/@(128.0.255) CallbackType][%% <][%%*/@(128.0.255) a
rguments][%% >]_[* callback1](void_(`*[*@3 fn])([*@4 P1], ...), [*@4 T1]_[*@3 arg], 
...)&]
[s0;%- &]
[s2; [%-*@4 P1] ([%-*@4 P2, P3 ][%- ...]) are arguments passed on method 
invocation, [%-*@4 T1] ([%-*@4 T2, T3 ][%- ...]) are parameters stored 
in Callback object.&]
[s3; &]
[s0; &]
[s0;%- &]
[ {{10000@(113.42.0) [s0; [*@7;4 Callback macros]]}}&]
[s9; Callback macros reduce verbosity of instance method callbacks. 
They expect CLASSNAME to be typedef`-ed as the type of current 
class and return method callbacks bound to the [*@(0.0.255) this] 
pointer.&]
[s3; &]
[s0; &]
[s0;:`:`:THISBACK`(x`):%- [* THISBACK]([*@3 x])&]
[s2; Expands to callback(this, `&CLASSNAME`::[@3 m]).&]
[s7; [%-*C@3 m]-|Name of method.&]
[s3; &]
[s4; &]
[s0;:`:`:THISBACK1`(x`, arg`):%- [* THISBACK1]([*@3 x], [*@3 arg])&]
[s2; Expands to callback1(this, `&CLASSNAME`::[@3 m], [@3 arg]).&]
[s7; [%-*C@3 m]-|Name of method.&]
[s7; [%-*C@3 arg]-|Additional parameter.&]
[s3; &]
[s4; &]
[s0;:`:`:THISBACK2`(m`, a`, b`):%- [* THISBACK2]([*@3 m], [*@3 a], [*@3 b])&]
[s2; Expands to callback2(this, `&CLASSNAME`::[@3 m], [@3 a],[@3  b]).&]
[s7; [%-*C@3 m]-|Name of method.&]
[s7; [%-*C@3 a]-|First additional parameter.&]
[s7; [%-*C@3 b]-|Second additional parameter.&]
[s3; &]
[s4; &]
[s0;:`:`:PTEBACK`(x`):%- [* PTEBACK]([*@3 x])&]
[s2; Expands to pteback(this, `&CLASSNAME`::[@3 m]).&]
[s7; [%-*C@3 m]-|Name of method.&]
[s3; &]
[s4; &]
[s0;:`:`:PTEBACK1`(x`, arg`):%- [* PTEBACK1]([*@3 x], [*@3 arg])&]
[s2; Expands to pteback1(this, `&CLASSNAME`::[@3 m], [@3 arg]).&]
[s7; [%-*C@3 m]-|Name of method.&]
[s7; [%-*C@3 arg]-|Additional parameter.&]
[s3; &]
[s4; &]
[s0;:`:`:PTEBACK2`(m`, a`, b`):%- [* PTEBACK2]([*@3 m], [*@3 a], [*@3 b])&]
[s2; Expands to pteback2(this, `&CLASSNAME`::[@3 m], [@3 a],[@3  b]).&]
[s7; [%-*C@3 m]-|Name of method.&]
[s7; [%-*C@3 a]-|First additional parameter.&]
[s7; [%-*C@3 b]-|Second additional parameter.&]
[s3; &]
[s0; &]
[s0;%- &]
[s0;%- &]
[ {{10000@(113.42.0) [s0; [*@7;4 EventArgTarget]]}}&]
[s3;%- &]
[s1;:noref:%- [@(0.0.255)3 template][3 _<][@(0.0.255)3 class][3 _][*@4;3 T][3 >]&]
[s1;:EventArgTarget`:`:class:%- [@(0.0.255) class]_[* CallbackArgTarget]&]
[s9; This helper class is useful in a situation when a set of callbacks 
define some output value. Example is pop`-up menu that provides 
selection of one character `- in that case, CallbackArgTarget 
can be used instead of dedicated method to obtain result of user 
selection.&]
[s8; [%-*@4 T]-|Type of output value. Must be able to be assigned Null.&]
[s3; &]
[s0;%- &]
[ {{10000F(128)G(128)@1 [s0; [* Public Member List]]}}&]
[s3; &]
[s5;:EventArgTarget`:`:EventArgTarget`(`):%- [* CallbackArgTarget]()&]
[s2; Assigns Null to the output value.&]
[s3; &]
[s4; &]
[s5;:EventArgTarget`:`:operator const T`&`(`)const:%- [* operator_const_T`&]()_[@(0.0.255) c
onst]&]
[s7; [*/ Return value]-|The result. If no CallbackArgTarget was invoked, 
returns Null.&]
[s3; &]
[s4;%- &]
[s5;:EventArgTarget`:`:IsNullInstance`(`)const:%- [@(0.0.255) bool]_[* IsNullInstance]()_
[@(0.0.255) const]&]
[s7; [*/ Return value]-|True if there is not Null in output value.&]
[s3; &]
[s4;%- &]
[s5;:EventArgTarget`:`:operator`[`]`(const T`&`):%- [_^Callback^ Callback]_[* operator`[`]
]([@(0.0.255) const]_[*@4 T][@(0.0.255) `&]_[*@3 value])&]
[s2; Returns a Callback that, when invoked, assigns [@3 value] to the 
output value.&]
[s7; [%-*C@3 value]-|Value.&]
[s7; [*/ Return value]-|Callback.&]
[s3; &]
[s0; ]]