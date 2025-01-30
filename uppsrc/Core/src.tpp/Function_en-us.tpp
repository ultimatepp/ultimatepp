topic "Function";
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
[ {{10000@(113.42.0) [s0; [*@7;4 Function]]}}&]
[s3;%- &]
[s1;:noref:%- [@(0.0.255)3 template][3 _<][@(0.0.255)3 typename][3 _][*@4;3 Res][3 , 
][@(0.0.255)3 typename...][3 _][*@4;3 ArgTypes][3 >]&]
[s1;:Function`<Res`(ArgTypes`.`.`.`)`> `:`:class:%- [@(0.0.255) class]_[* Function<Res(Ar
gTypes...)>_]:_[@(0.0.255) private]_[*@3 Moveable]<Function<[*@4 Res]([*@4 ArgTypes][@(0.0.255) .
..])>>_&]
[s2; Upp`::Function is wrapper to represent callable operation. It 
is similar to std`::function with two differences:&]
[s2;i150;O0; Calling empty Function is allowed (and NOP). Returns 
zero.&]
[s2;i150;O0; Functions can be combined (chained) using operator<<.&]
[s3;%- &]
[ {{10000F(128)G(128)@1 [s0; [* Public Method List]]}}&]
[s3; &]
[s5;:Function`<Res`(ArgTypes`.`.`.`)`> `:`:Function`(`):%- [* Function]()&]
[s5;:Upp`:`:Function`<Res`(ArgTypes`.`.`.`)`> `:`:Function`(const Upp`:`:Nuller`&`):%- [* F
unction]([@(0.0.255) const]_[_^Upp`:`:Nuller^ Nuller][@(0.0.255) `&])&]
[s2; Creates empty Function. Invoking empty function is NOP and eventual 
return value is Res() (which results in 0 for fundamental types). 
Variant with Nuller is useful when defining function declaration 
with optional Function parameters like [*C@5 void Foo(Function<void 
()> `= Null);]&]
[s3;%- &]
[s4; &]
[s5;:Function`<Res`(ArgTypes`.`.`.`)`> `:`:Function`(F`):%- [@(0.0.255) template]_<[@(0.0.255) c
lass]_[*@4 F]>_[* Function]([*@4 F]_[*@3 fn])&]
[s2; This is the `'catch lambda`' constructor which allows converting 
generic lambda to Function value.&]
[s3; &]
[s4; &]
[s5;:Function`<Res`(ArgTypes`.`.`.`)`> `:`:Function`(const Function`&`):%- [* Function](
[@(0.0.255) const]_[* Function][@(0.0.255) `&]_[*@3 src])&]
[s5;:Function`<Res`(ArgTypes`.`.`.`)`> `:`:operator`=`(const Function`&`):%- Function
[@(0.0.255) `&]_[* operator`=]([@(0.0.255) const]_Function[@(0.0.255) `&]_[*@3 src])&]
[s5;:Function`<Res`(ArgTypes`.`.`.`)`> `:`:Function`(Function`&`&`):%- [* Function]([* Fu
nction][@(0.0.255) `&`&]_[*@3 src])&]
[s5;:Function`<Res`(ArgTypes`.`.`.`)`> `:`:operator`=`(Function`&`&`):%- Function[@(0.0.255) `&
]_[* operator`=](Function[@(0.0.255) `&`&]_[*@3 src])&]
[s2; Usual copy/move constructors and assignment operators.&]
[s3; &]
[s4; &]
[s5;:Function`<Res`(ArgTypes`.`.`.`)`> `:`:Proxy`(`)const:%- Function_[* Proxy]()_[@(0.0.255) c
onst]&]
[s2; Returns a new Function, which, when invoked, invokes this Function. 
This is useful when some Function is to changed later, but we 
need another Function to represent it.&]
[s3; &]
[s4; &]
[s5;:Function`<Res`(ArgTypes`.`.`.`)`> `:`:operator`<`<`(F`):%- [@(0.0.255) template]_<
[@(0.0.255) class]_[*@4 F]>_Function[@(0.0.255) `&]_[* operator<<]([*@4 F]_[*@3 fn])&]
[s5;:Function`<Res`(ArgTypes`.`.`.`)`> `:`:operator`<`<`(const Function`&`):%- Functi
on[@(0.0.255) `&]_[* operator<<]([@(0.0.255) const]_Function[@(0.0.255) `&]_[*@3 fn])&]
[s5;:Function`<Res`(ArgTypes`.`.`.`)`> `:`:operator`<`<`(Function`&`&`):%- Function[@(0.0.255) `&
]_[* operator<<](Function[@(0.0.255) `&`&]_[*@3 fn])&]
[s2; These operators combine two Functions or function with lambda. 
Original `'this`' function is invoked first and then [%-*@3 fn]. 
If either of Functions returns value, value of [%-*@3 fn] (argument 
to the right of <<) represents the value of whole Function.&]
[s3; &]
[s4; &]
[s5;:Function`<Res`(ArgTypes`.`.`.`)`> `:`:operator`(`)`(ArgTypes`.`.`.args`)const:%- [*@4 R
es]_[* operator()]([*@4 ArgTypes][@(0.0.255) ...]_args)_[@(0.0.255) const]&]
[s2; Performs the Functions.&]
[s3; &]
[s4; &]
[s5;:Function`<Res`(ArgTypes`.`.`.`)`> `:`:operator bool`(`)const:%- [* operator_bool](
)_[@(0.0.255) const]&]
[s2; Returns true if Function is not empty.&]
[s3; &]
[s4; &]
[s5;:Function`<Res`(ArgTypes`.`.`.`)`> `:`:Clear`(`):%- [@(0.0.255) void]_[* Clear]()&]
[s2; Sets Function empty.&]
[s3; &]
[s0;%- &]
[ {{10000@(113.42.0) [s0; [*@7;4 Event and Gate]]}}&]
[s3;%- &]
[s0;%- Two most common types of Function in U`+`+ are those that 
return `'void`' and those that return `'bool`'. U`+`+ defines 
two convenience template aliases:&]
[s0;%- &]
[s5;:Upp`:`:Event:%- [@(0.0.255) template <typename... ][*@4 ArgTypes][@(0.0.255) > 
using]_[* Event]_`=_Function<[@(0.0.255) void]_(ArgTypes[@(0.0.255) ...])>&]
[s2; Creates alias for Function returning [@(0.0.255) void]. For example, 
[*C@5 Event<int, String>] is equivalent of [*C@5 Function<void (int, 
String)>].&]
[s3;%- &]
[s4;%- &]
[s5;:Upp`:`:Gate:%- [@(0.0.255) template <typename... ][*@4 ArgTypes][@(0.0.255) > 
using]_[* Gate]_`=_Function<[@(0.0.255) bool]_(ArgTypes[@(0.0.255) ...])>&]
[s2; Creates alias for Function returning bool. For example, [*C@5 Gate<int, 
String>] is equivalent of [*C@5 Function<bool (int, String)>]. 
Note that empty Gate returns false (because empty Function returns 
bool(0), which is false).&]
[s3;%- &]
[s0;%- &]
[ {{10000@(113.42.0) [s0; [*@7;4 MemFn and THISFN]]}}&]
[s3;%- &]
[s3;%- &]
[s0;%- Despide the power of C`+`+11, there are still situations where 
it is better to put the callable code into member function. C`+`+11 
lamda syntax can be tedious in that case. MemFn template function 
and THISFN serve to reduce typing overhead.&]
[s0;%- &]
[s5;:Upp`:`:MemFn`(Upp`:`:Ptr`,Res`(`*`)`(ArgTypes`.`.`.`)`):%- [@(0.0.255) template]_<
[@(0.0.255) class]_[*@4 Ptr], [@(0.0.255) class]_[*@4 Class], [@(0.0.255) class]_[*@4 Res], 
[@(0.0.255) class...]_[*@4 ArgTypes]>_[_^Upp`:`:Event^ Event]<[*@4 ArgTypes][@(0.0.255) ...
]>_[* MemFn]([*@4 Ptr]_[*@3 object], [*@4 Res]_(Class`::[@(0.0.255) `*][*@3 member`_function])
([*@4 ArgTypes][@(0.0.255) ...]))&]
[s2; Equivalent of std`::mem`_fn returning U`+`+ Function. Creates 
Function that represents calling [%-*@3 member`_function] of instance 
[%-*@3 object].&]
[s3;%- &]
[s4;%- &]
[s5;:THISFN:%- [* THISFN](x)&]
[s2; This macro is defined a MemFn(this, `&CLASSNAME`::x). It is a 
useful shortcut for converting member method to Function. If 
a class has member function&]
[s2; &]
[s2; [C void Menu(Bar`&);]&]
[s2; &]
[s2; then following two lines are equivalent:&]
[s2; &]
[s2; [C menu.Set(`[`=`] (Bar`& bar) `{ Menu(bar); `});]&]
[s2; [C menu.Set(THISFN(Menu));]&]
[s2;C &]
[s2; CLASSNAME must be defined as nested typedef of current class.&]
[s3;%- &]
[s0;@(0.0.255)3%- ]]