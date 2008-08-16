[2 $$0,0#00000000000000000000000000000000:Default]
[l288;i704;a17;O9;~~~.992;2 $$1,0#10431211400427159095818037425705:param]
[a83;*R6 $$2,5#31310162474203024125188417583966:caption]
[b83;*2 $$3,5#07864147445237544204411237157677:title]
[b167;a42;C2 $$4,6#40027414424643823182269349404212:item]
[b42;a42;2 $$5,5#45413000475342174754091244180557:text]
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
[{_}%EN-US 
[s2; Storing custom data types in Values&]
[s0; Of course it is possible to store custom data types in Values, 
that`'s what they`'re for. In fact, even the `"standard`" value 
types like INT`_V or STRING`_V have very little special support 
in Value, in most aspects their encapsulation in Value is implemented 
in just the same way as for any other types.&]
[s0; &]
[s0; The main question is: how `"rich`" encapsulation you want? The 
simplest available option is called `"raw value type`". The only 
two things it supports is wrapping an object into a Value and 
getting it back. To turn an object into Value in this way, use 
e.g.:&]
[s0; &]
[s7; MyType t;&]
[s7; Value v;&]
[s7; &]
[s7; v `= RawToValue(t);&]
[s0; &]
[s0; For pick types, i.e. for types with `"destructive copy`" as 
default, you can use&]
[s0; &]
[s7; RawPickToValue(t)&]
[s0; &]
[s0; or, if type has alternative deep copy option&]
[s0; &]
[s7; RawDeepToValue(t)&]
[s0; &]
[s0; depending on whether the value of t should be picked (destroyed). 
When it should, using RawPickToValue should be faster.&]
[s0; &]
[s0; When you need the value back, use&]
[s0; &]
[s7; const MyType`& t `= ValueTo<MyType>(v);&]
[s0; &]
[s0; Note that the function ValueTo returns a constant reference. 
This is consistent with default Value behaviour, according to 
which data once put into Value should never change afterwards. 
This is necessary because, upon copying, multiple variables of 
type Value can hold the same data `"packet`" and changing its 
contents would affect all these copies, which is normally undesirable. 
If you break the const`-ness using a mutable member or by a const`_cast, 
you should not forget that.&]
[s0; &]
[s0; Of course, you can make copy of value referenced by this constant 
reference as well&]
[s0; &]
[s7; -|MyType t `= ValueTo<MyType>(v);&]
[s0; &]
[s0; but be careful about transfer semantics of MyType (doing this 
with type that has pick default transfer semantics is error).&]
[s0; &]
[s0; A higher level of encapsulation defines a few more operations 
on such custom Value types: it is called the `"rich value type`" 
and allows such Values to be compared (for equality, there is 
currently no general way to define ordering for custom Value 
types), text`-formatted, serialized and checked for Null`-ness. 
You can either use RichToValue (much like RawToValue), or (which 
is common for objects with high level Value encapsulation) define 
a conversion operator within the object class. ValueTo<> is the 
same as in the raw case, moreover it is usually handy to define 
a constructor from a Value or const Value`& for this purpose. 
You can get clue about his this is done by examining e.g. Color 
class code.&]
[s0; &]
[s0; The highest currently possible encapsulation level is the rich 
value type `+ type registration, which allows the Value serializer 
to recognize the custom types when loading data from a stream 
and create the necessary custom Value object on`-the`-fly. You 
activate this behaviour by defining a ValueTypeNo function and 
assigning the new type an unique code.&]
[s0; ]