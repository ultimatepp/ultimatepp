topic "Callback example";
[2 $$0,0#00000000000000000000000000000000:Default]
[l288;i1120;a17;O9;~~~.1408;2 $$1,0#10431211400427159095818037425705:param]
[a83;*R6 $$2,5#31310162474203024125188417583966:caption]
[b83;*4 $$3,5#07864147445237544204411237157677:title]
[i288;O9;C2 $$4,6#40027414424643823182269349404212:item]
[b42;a42;2 $$5,5#45413000475342174754091244180557:text]
[l288;b17;a17;2 $$6,6#27521748481378242620020725143825:desc]
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
[2 $$19,0#53580023442335529039900623488521:gap]
[t4167;C2 $$20,20#70211524482531209251820423858195:class`-nested]
[b50;2 $$21,21#03324558446220344731010354752573:Par]
[{_}%EN-US 
[s2; Callback example&]
[s5; [^topic`:`/`/Core`/src`/Callbacks`$en`-us^ Callbacks] can be described 
as a very generalized form of function pointers. Each Callback 
represents some kind of action (usually calling a certain function 
or a certain object method) that can be invoked at any time.&]
[s5; When you need some kind of Action`-Reaction behavior, callbacks 
are very handy. Typical example is when you have some button 
in your GUI application. When a user clicks on that button (action) 
you wont some part of your code to be executed (reaction). You 
tell button witch part of code, in some yours function, to execute 
via callback object.&]
[s5; This tutorial will clear some basic things in U`+`+ callback 
mehanics. Here is code of our example&]
[s7; #include <Core/Core.h>&]
[s7; &]
[s7; struct Foo `{&]
[s7; -|int x;&]
[s7; -|&]
[s7; -|void Action()                `{ Cout() << `"Action: `" << x 
<< `'`\n`'; `}&]
[s7; -|void ActionWithParam(int y)  `{ Cout() << `"ActionWithParam: 
`" << x `+ y << `'`\n`'; `}&]
[s7; &]
[s7; -|Callback WhenDo;&]
[s7; -|void Do()                    `{ WhenDo(); `}&]
[s7; &]
[s7; -|Foo(int x `= 0) : x(x) `{`}&]
[s7; `};&]
[s7; &]
[s7; void Fn()&]
[s7; `{&]
[s7; -|Cout() << `"Fn!`" << `'`\n`';&]
[s7; `}&]
[s7; &]
[s7; struct Bar : Pte<Bar> `{&]
[s7; -|Foo foo;&]
[s7; &]
[s7; -|void Action() `{ Cout() << `"foo`'s Do called`\n`"; `}&]
[s7; -|&]
[s7; -|typedef Bar CLASSNAME;&]
[s7; -|&]
[s7; -|Bar() `{ foo.WhenDo `= THISBACK(Action); `}&]
[s7; `};&]
[s7; &]
[s7; struct Safe : Pte<Safe> `{&]
[s7; -|void Action() `{ Cout() << `"safe action!`\n`"; `}&]
[s7; `};&]
[s7; &]
[s7; CONSOLE`_APP`_MAIN&]
[s7; `{&]
[s7; -|Foo a(10);&]
[s7; -|Callback cb1 `= callback(`&a, `&Foo`::Action);&]
[s7; -|Callback cb2 `= callback(Fn);&]
[s7; -|Callback1<int> cb3 `= callback(`&a, `&Foo`::ActionWithParam);&]
[s7; -|Callback cb4 `= callback1(`&a, `&Foo`::ActionWithParam, 30);&]
[s7; -|&]
[s7; -|cb1();&]
[s7; -|cb2();&]
[s7; -|cb3(10);&]
[s7; -|cb4();&]
[s7; &]
[s7; -|Cout() << `"`-`-`-`-`-`-`-`-`-`\n`";&]
[s7; -|cb4 << cb2;&]
[s7; -|cb4();&]
[s7; -|&]
[s7; -|Cout() << `"`-`-`-`-`-`-`-`-`-`\n`";&]
[s7; -|Bar b;&]
[s7; -|b.foo.Do();&]
[s7; &]
[s7; -|/`*Cout() << `"`-`-`-`-`-`-`-`-`-`\n`";&]
[s7; -|`{&]
[s7; -|-|Safe f;&]
[s7; -|-|cb4 `= pteback(`&f, `&Safe`::Action);&]
[s7; -|-|Cout() << `"callback valid: `" << (bool)cb4 << `'`\n`';&]
[s7; -|-|cb4();&]
[s7; -|`}&]
[s7; -|Cout() << `"callback valid: `" << (bool)cb4 << `'`\n`';&]
[s7; -|cb4();`*/&]
[s7; `}&]
[s5; First we declared Foo class, and the only interesting thing 
in this class is that it has one callback member object WhenDo, 
and method Do() in which WhenDo object [^topic`:`/`/Core`/src`/Callbacks`$en`-us`#`:`:Callback1`:`:operator`(`)`(P1`)const^ (
)] operator is called. This operator will call WhenDo.Execute() 
which executes callback with a given set of arguments.&]
[s5; Then we have global function Fn() that just prints `"Fn!`" to 
console.&]
[s5; We then derive two class Bar and Safe from [^topic`:`/`/Core`/src`/PtePtr`$en`-us^ P
te] class templates. Pte adds necessary functionality to object 
so that [^topic`:`/`/Core`/src`/PtePtr`$en`-us^ Ptr] smart pointer 
can point to that object. Ptr and Pte gives smart pointers system 
with two important characteristics: pointers are cleared when 
pointed object is destructed and pointers have persistence features.&]
[s5; Bar have one member object foo, and in it constructor we assign 
to foo`'s WhenDo new callback object which when executed calls 
bar`'s Action method.&]
[s5; That`'s it, now lets put our new classes to show:). In CONSOLE`_APP`_MAIN 
we first have&]
[s7; Foo a(10);&]
[s7; Callback cb1 `= callback(`&a, `&Foo`::Action);&]
[s7; Callback cb2 `= callback(Fn);&]
[s7; Callback1<int> cb3 `= callback(`&a, `&Foo`::ActionWithParam);&]
[s7; Callback cb4 `= callback1(`&a, `&Foo`::ActionWithParam, 30);&]
[s7; &]
[s7; cb1();&]
[s7; cb2();&]
[s7; cb3(10);&]
[s7; cb4();&]
[s5; We have 4 variants of Callback objects. When cb1 is executed 
it will call a.Action(). cb2 will call global function Fn(). 
cb3 is little more interesting, notice that it is of type Callback1, 
and this callback enable us to call function with one parameter, 
in this example when you execute cb3(10) callback will call a.ActionWithParam(10)
. cb4 will also call ActionWithParam but this time parameter 
is saved directly in callback object, so when you execute cb4() 
it will always call a.ActionWithParam(30).&]
[s5; Actually U`+`+ Callbacks support up to 4 parameters (it can 
be expande later, but this seems to be enough). Respective types 
are Callback, Callback1, Callback2, Callback3, Callback4. Note 
that 1`-4 are templates (types of parameters). `"callback`" is 
an overloaded function that is capable of creating all of these 
based on actuall parameter used.&]
[s5; Next is&]
[s7; cb4 << cb2;&]
[s7; cb4();&]
[s5; Operator << demonstrate one nice feature of U`+`+ callbacks, 
so call chaining of callbacks. Basically it means, create new 
callback that invokes actions of cb4 and cb2, and then store 
that callback in cb4. So when we execute cb4() it will call a.ActionWithParam(30)
 and then Fn()[@N .]&]
[s5; Next few lines are&]
[s7; Bar b;&]
[s7; b.foo.Do();&]
[s5; We create one Bar object and then calls foo`'s Do which executes 
foo`'s WhenDo callback, so on the end b.Action() will be executed 
`- feel little lost here:), don`'t be callbacks are just a way 
to tell some object what to do in different situations.&]
[s5; On end of our main function we have next block of code&]
[s7; `{&]
[s7; -|Safe f;&]
[s7; -|cb4 `= pteback(`&f, `&Safe`::Action);&]
[s7; -|Cout() << `"callback valid: `" << (bool)cb4 << `'`\n`';&]
[s7; -|cb4();&]
[s7; `}&]
[s7; Cout() << `"callback valid: `" << (bool)cb4 << `'`\n`';&]
[s7; cb4();&]
[s5; In inner scope we construct f and then create callback to f.Action(). 
pteback is similar to callback method, but pteback will create 
a callback that can be safly invoked even after destruction of 
f object, and in order to work f must be ancestor of Pte.&]
[s5; In Cout line we see nice way of telling if callback object is 
valid (can invoke some action) or invalid. Callback have overloaded 
operator bool() so when we cast some callback object to bool 
this method will return True if callback can invoke some action.&]
[s5; Then in the end of inner scope we execute cb4. Now when we get 
out of the inner scope, f object will be destroyed and cb4 will 
have pointer to destroyed object, so every execution of cb4 callback 
would be dangerous. But thanks to U`+`+ smart pointer system 
it is safe to execute cb4 callback `- nothing happens like it 
should.&]
[s5; Just to be sure, here is the output of our program&]
[s16; Action: 10&]
[s16; Fn!&]
[s16; ActionWithParam: 20&]
[s16; ActionWithParam: 40&]
[s16; `-`-`-`-`-`-`-`-`-&]
[s16; ActionWithParam: 40&]
[s16; Fn!&]
[s16; `-`-`-`-`-`-`-`-`-&]
[s16; foo`'s Do called&]
[s16; `-`-`-`-`-`-`-`-`-&]
[s16; callback valid: true&]
[s16; safe action!&]
[s16; callback valid: false&]
[s5; &]
[s5; So you see that U`+`+ callbacks are really easy to use. It syntax 
is nice and clean, and yet they are very powerful.&]
[s5; [@3 Maybe something more in conclusion?]]]