topic "Transfer semantics";
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
[s2; Transfer semantics&]
[s3; Explicit pick/clone&]
[s5; U`+`+ containers require the transfer mode (deep copy or move) 
to be explicitly specified when transfering the content of the 
container (exception is the temporary value, which can be pick 
assigned without explicit pick). This decision has the advantage 
of not accidentally using costly deep copy semantics, when in 
reality move is required in majority of case..&]
[s7; Vector<int> a, b;&]
[s7; a `= pick(b); // moves content of b to a, b is cleared&]
[s7; b `= clone(a); // a and b now contain the same data&]
[s5; &]
[s3; Composition&]
[s5; When class contains members with pick semantics, a lot of error`-prone 
work is saved when compiler is able to generate pick constructor/operator`= 
for the class. C`+`+11 is quite capable of doing so, but needs 
to explicitly activate it with default memebers:&]
[s7; Foo(Foo`&`&) `= default;&]
[s7; Foo`& operator`=(Foo`&`&) `= default;&]
[s5; Meanwhile, C`+`+03 does need these and does not recognize them. 
To make things easier, we define macro rval`_default, which simplifies 
this and irons out differences&]
[s7; rval`_default(Foo)&]
[s3; Optional deep copy `- clone, uniform access to deep copy&]
[s5; To support clone, class has to define special constructor of 
form&]
[s7; T(const T`&, int)&]
[s5; and to derive from DeepCopyOption<T> class, which provides support 
for static/dynamic construction of instances.&]
[s3; Changing default semantics&]
[s5; If for some reason you need version of [/ optional] deep copy 
type with [/ default] deep copy, you can easily create it with 
[* WithDeepCopy ]template&]
[s7; IntArray a `= MakeArray(100);&]
[s7; WithDeepCopy<IntArray> b(a); // b now has deep copy semantics&]
[s7; a`[3`] `= 10; //legal&]
[s7; b `= a; // deep copy&]
[s7; a `= pick(b); // pick&]
[s7; b`[4`] `= 1; // illegal&]
[s0; ]]