topic "NTL and standard library";
[2 $$0,0#00000000000000000000000000000000:Default]
[l288;i704;a17;O9;~~~.992;2 $$1,0#10431211400427159095818037425705:param]
[a83;*R6 $$2,5#31310162474203024125188417583966:caption]
[b83;*2 $$3,5#07864147445237544204411237157677:title]
[b167;a42;C2 $$4,6#40027414424643823182269349404212:item]
[b42;a42;2 $$5,5#45413000475342174754091244180557:text]
[l288;a17;2 $$6,6#27521748481378242620020725143825:desc]
[l321;C@5;1 $$7,7#20902679421464641399138805415013:code]
[b2503;2 $$8,0#65142375456100023862071332075487:separator]
[*@(0.0.255)2 $$9,0#83433469410354161042741608181528:base]
[C7 $$10,0#37138531426314131251341829483380:class]
[l288;a17;*1 $$11,11#70004532496200323422659154056402:requirement]
[i417;b42;a42;O9;~~~.416;2 $$12,12#10566046415157235020018451313112:tparam]
[b167;C2 $$13,13#92430459443460461911108080531343:item1]
[i288;a42;O9;C2 $$14,14#77422149456609303542238260500223:item2]
[*@2$(0.128.128)2 $$15,15#34511555403152284025741354420178:NewsDate]
[l321;*C$7;2 $$16,16#03451589433145915344929335295360:result]
[l321;b83;a83;*C$7;2 $$17,17#07531550463529505371228428965313:result`-line]
[l160;*C+117 $$18,5#88603949442205825958800053222425:package`-title]
[{_}%EN-US 
[s2; NTL and standard library&]
[s5; While NTL is kind of `"alien technology`" with respect to C`+`+ 
standard library, in fact it is largely inter`-operable with 
it.&]
[s3; Iterators&]
[s5; Each NTL container (with exception of One) provides iterator 
access. All iterators satisfy STL requirements for random access 
iterators. This also means that most of STL (and STL based) algorithms 
can be used with NTL containers.&]
[s3; auto`_ptr&]
[s5; Funny part about standard library and NTL relationship is that 
any known (and imaginable) implementation of auto`_ptr is moveable 
`- so you can store it into any flavor of containers. All you 
need to do is to mark an instantiation with the Moveable template.&]
[s5; Even better is that you do not need Vector of auto`_ptr `- because 
Array flavor will do the same job more comfortably.&]
[s0; ]]