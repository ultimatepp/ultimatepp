topic "Resolving memory leaks";
[2 $$0,0#00000000000000000000000000000000:Default]
[l288;i1120;a17;O9;~~~.1408;2 $$1,0#10431211400427159095818037425705:param]
[a83;*R6 $$2,5#31310162474203024125188417583966:caption]
[H4;b83;*4 $$3,5#07864147445237544204411237157677:title]
[i288;O9;C2 $$4,6#40027414424643823182269349404212:item]
[b42;a42;2 $$5,5#45413000475342174754091244180557:text]
[l288;b17;a17;2 $$6,6#27521748481378242620020725143825:desc]
[l321;C@5;1 $$7,7#20902679421464641399138805415013:code]
[b2503;2 $$8,0#65142375456100023862071332075487:separator]
[*@(0.0.255)2 $$9,0#83433469410354161042741608181528:base]
[C2 $$10,0#37138531426314131251341829483380:class]
[l288;a17;*1 $$11,11#70004532496200323422659154056402:requirement]
[i417;b42;a42;O9;~~~.416;2 $$12,12#10566046415157235020018451313112:tparam]
[b167;C2 $$13,13#92430459443460461911108080531343:item1]
[i288;a42;O9;C2 $$14,14#77422149456609303542238260500223:item2]
[*@2$(0.128.128)2 $$15,15#34511555403152284025741354420178:NewsDate]
[l321;*C$7;2 $$16,16#03451589433145915344929335295360:result]
[l321;b83;a83;*C$7;2 $$17,17#07531550463529505371228428965313:result`-line]
[l160;*C+117 $$18,5#88603949442205825958800053222425:package`-title]
[2 $$19,0#53580023442335529039900623488521:gap]
[C2 $$20,20#70211524482531209251820423858195:class`-nested]
[b50;2 $$21,21#03324558446220344731010354752573:Par]
[{_}%EN-US 
[s2; Resolving memory leaks&]
[s0; Before 2008.1 release, the memory leaks debugging facilities 
have changed. Here we try to explain why logic has changed and 
what problem is it supposed to solve:&]
[s0; &]
[s0; Previous method involved watching for particular ADDRESS to 
be allocated (so that you could detemine the source of leak). 
Anyway, that has a problem than often the same memory block is 
going to be allocated more than one time `- obviously, in that 
case the watch would only catch the first allocation, which would 
not be the cause of the problem.&]
[s0; &]
[s0; That is why logic was improved by introducing `"memory allocation 
serial number`" `- each memory allocation has associated serial 
number (incremental). When U`+`+ detects a leak, it dumps all 
leak blocks (note that sometimes you can resolve the problem 
just by examining its content) to standard .log file (you can 
see it in theide by pressing Alt`+L). And in the .log, serial 
number is printed for each leaked memory block. It is printed 
after `"`-`-memory`-breakpoint`_`_`".&]
[s0; &]
[s0; Now the only thing you need to do is to add `"void MemoryBreakpoint(dword 
serial)`". When Alloc for that serial happens again, `_`_BREAK`_`_ 
will segfault (`"`*(void `*)0 `= 0`") the application and you 
will be able to catch the problem in the debugger.&]
[s0; &]
[s0; Of course, the only problem with this is that the order of allocations 
must be exactly the same in the `"test run`". Which is often 
not, but usually it is possible to arrange it so after spending 
a couple of nights desperately looking for the source of leak 
in the code using other methods...&]
[s0; ]