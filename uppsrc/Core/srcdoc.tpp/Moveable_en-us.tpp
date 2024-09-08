topic "Trivially relocatable types (Moveable)";
[l288;i704;a17;O9;~~~.992;2 $$1,0#10431211400427159095818037425705:param]
[a83;*R6 $$2,5#31310162474203024125188417583966:caption]
[b83;*2 $$3,5#07864147445237544204411237157677:title]
[b167;a42;C2 $$4,6#40027414424643823182269349404212:item]
[b42;a42;ph2 $$5,5#45413000475342174754091244180557:text]
[l288;a17;2 $$6,6#27521748481378242620020725143825:desc]
[l321;t246;C@5;1 $$7,7#20902679421464641399138805415013:code]
[b2503;2 $$8,0#65142375456100023862071332075487:separator]
[*@(0.0.255)2 $$9,0#83433469410354161042741608181528:base]
[t4167;C2 $$10,0#37138531426314131251341829483380:class]
[l288;a17;*1 $$11,11#70004532496200323422659154056402:requirement]
[i417;b42;a42;O9;~~~.416;2 $$12,12#10566046415157235020018451313112:tparam]
[b167;C2 $$13,13#92430459443460461911108080531343:item1]
[i288;a42;O9;C2 $$14,14#77422149456609303542238260500223:item2]
[*@2$(0.128.128)2 $$15,15#34511555403152284025741354420178:NewsDate]
[l321;*C$7;2 $$16,16#03451589433145915344929335295360:result]
[l321;b83;a83;*C$7;2 $$17,17#07531550463529505371228428965313:result`-line]
[l160;t4167;*C+117 $$18,5#88603949442205825958800053222425:package`-title]
[2 $$0,0#00000000000000000000000000000000:Default]
[{_}%EN-US 
[s2; Trivially relocatable types (Moveable)&]
[s5;%- [%% First important note: U`+`+ Moveable is not to be confused 
with C`+`+ standard library move semantics, but is the original 
name of U`+`+ equivalent of ][%%^https`:`/`/www`.open`-std`.org`/jtc1`/sc22`/wg21`/docs`/papers`/2024`/p1144r10`.html^ t
rivially`_relocatable][%%  type `- ]type’s relocation operation 
has novisible side`-effects other than a copy of the underlying 
bytes, as if by the library function std`::memcpy&]
[s5; U`+`+ flavor of this technique is based on marking trivially 
relocatable types, as defined by future C`+`+ standard according 
to [%-*^https`:`/`/www`.open`-std`.org`/jtc1`/sc22`/wg21`/docs`/papers`/2024`/p1144r10`.html^ P
1144] by inheriting from TriviallyRelocatable<T> or Moveable<T> 
base (older shorter synonym).&]
[s5; &]
[s7; struct Foo : [* TriviallyRelocatable]<Foo> `{&]
[s7; .....&]
[s7; `};&]
[s5; or&]
[s7; struct Foo : [* Moveable]<Foo> `{&]
[s7; .....&]
[s7; `};&]
[s5; U`+`+ require that elements of Upp`::Vector, Upp`::BiVector, Upp`::Index, 
Upp`::VectorMap and [/ keys] of Upp`::VectorMap and Upp`::ArrayMap 
are trivially relocatable or trivially copyable (that allows 
all POD types without any further warrants by programmer). Alternatively, 
type can be explicitly allowed to be part of these containers 
[/ without] being trivially relocatable using&]
[s7; template <>&]
[s7; inline constexpr bool Upp`::[* is`_upp`_guest]<std`::string> `= 
true;&]
[s5; U`+`+ then uses normal move contructor and destructor to relocate 
element.&]
[s5; Obviously, in the future, when / if P1144 is accepted, Upp trivially 
relocatable types will also include the proposed mechanisms in 
C`+`+ standard.&]
[s5; ]]