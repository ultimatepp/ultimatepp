topic "New features of U++ Core 2016";
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
[s2; New features of U`+`+ Core 2016&]
[s3; Allocator&]
[s5; U`+`+ allocator now always returns 16`-byte aligned blocks. 
This is to simplify SSE requirements, at the price of smallest 
possible allocation being 32`-bytes (request for smaller blocks 
are rounded up to 32`-bytes).&]
[s5; In situation where allocating a lot of blocks smaller than 32 
bytes (typical is controlling reference counter block in shared`_ptr 
like class), new [* TinyAlloc] / [* TinyFree] functions can be used, 
or templated `"new/delete`" like functions [* tiny`_new] / [* tiny`_delete].&]
[s5; &]
[s3; Multithreading&]
[s0; &]
[s0; One of biggest advances of C`+`+11 is support for multithreading 
memory model. That is why we could drop U`+`+ memory model support 
and use what language/standard library provides `- [* Atomic ]is 
now reimplemented using [* std`::atomic<int>], thread local variables 
are language standard. OTOH, beyond memory model, U`+`+ [* Thread 
]class and [* CoWork ]class are still basis of multithreading.&]
[s0; &]
[s0; [* ConditionVariable ]is now recommended over [* Semaphore]. We 
keep U`+`+ class for this one, because in Win32 ConditionVariable 
requires Windows 7 and we still want to keep WinXP support (U`+`+ 
ConditionVariable contains internal implementation when system 
API isThread`::Exitmissing).&]
[s0; &]
[s0; [* Thread`::Exit ]can be used to exit the thread (from within), 
similar to [* Exit] to exit the whole program. It throws exception, 
which is caught at the end of thread routine.&]
[s0; &]
[s0; [* CoWork] was optimized and also changes so that each `"master 
thread`" has private thread pool. This is important to avoid 
stealing of work between master threads, which can lead to problems 
(e.g. delays in the GUI).&]
[s0; &]
[s0; [* CoWork`::FinLock] now provides `'finalization lock`', useful 
to avoid additional mutex lock for e.g. storing results.&]
[s0; &]
[s0; [* CoWork] now also has pipeline support, where thread performs 
one specific task and eventually passes results to another thread 
to continue processing.&]
[s5; &]
[s3; Callback changes&]
[s0; &]
[s0; Callbacks are refactored with C`+`+11 lambdas and varargs templates. 
There are now 3 classes:&]
[s0; &]
[s0; [* Function] is similar to std`::function, generic callable with 
any number of parameters and any return value in addition it 
has ability to `'add`' functions using operator<<.&]
[s0; &]
[s0; [* Event] is equivalent of [* Callback] `- unlike [* Callback], it 
is not necessary to have [* Callback], [* Callback1], [* Callback2], 
number of parameters is resolved by C`+`+11 template varargs. 
For backward compatibility [* Callback`[N`]] are aliased as [* Event] 
variants.&]
[s0; &]
[s0; Similarly, [* EventGate] is a new equivalent of [* Gate].&]
[s0; &]
[s3; Algorithms and Containers&]
[s0; &]
[s0; The set of algorithms provided by U`+`+ is now streamlined by 
introduction of [/ Range] concept. Range is entity that provides 
begin/end/GetCount/operator`[`] methods. All U`+`+ algorithms 
expect Range as input. All U`+`+ containers satisfy Range concept. 
In addition there are&]
[s0; &]
[s0; [* SubRangeClass] `- represents subrange of container, or just 
begin/end pair.&]
[s0; &]
[s0; [* ConstRangeClass] `- trivial single`-value range&]
[s0; &]
[s0; [* ViewRangeClass] `- is basically a list of indicies inside another 
range. [* FilterRange] function can create [* ViewRangeClass] based 
on predicate.&]
[s0; &]
[s0; U`+`+ containers now support [* InsertRange]/[* AppendRange ]template 
methods.&]
[s0; &]
[s0; New, parallel algorithms are introduced: [* CoLoop], [* CoAccumulate], 
[* CoSum], [* CoFindBest], [* CoFindMin], [* CoMin], [* CoFindMax], [* CoMax], 
[* CoFindMatch], [* CoFindIndex], [* CoSort],[*  CoStableSort],[*  CoIndexSort], 
[* CoStableIndexSort], [* CoIndexSort2], [* CoStableIndexSort2], [* CoIndexSort3], 
[* CoStableIndexSort3], [* CoGetSortOrder], [* CoGetStableSortOrder], 
[* CoSortByKey], [* CoSortByValue], [* CoStableSortByKey], [* CoStableSortByValue], 
[* CoSortIndex], [* CoStableSortIndex].&]
[s5; &]
[s3; Tuple&]
[s5; Tuple now does not require the specific template type based 
on number of elements (instead of Tuple3<int, int, String> it 
can be used just Tuple<int, int, String>). Tuple now has methods 
for retrieving the number of elements and index based access 
of elements via Value and also C`+`+11 like access via template 
methods (index based or type based):&]
[s5; &]
[s5; &]
[s5; &]
[s3; Changes in r`-value and pick&]
[s0; Generic Tuple&]
[s0; String() <<&]
[s0; &]
[s0; String `&`&&]
[s0; &]
[s0; Remove IsPicked&]
[s0; &]
[s0; Remove AddPick (keep as synonym)&]
[s0; &]
[s0; void XmlNode`::SetAttrsPick(VectorMap<String, String>`&`& a)&]
[s0; &]
[s0; inline String`& operator<<(String`&`& s, const T`& x)&]
[s0; &]
[s5; &]
[s0;* &]
[s3; Smaller issues&]
[s5; Stream`::SerializeRaw count is now int64 (instead of 32`-bit 
int)&]
[s5; 2G items limit is better implemented in StringBuffer, WStringBuffer, 
StringStream.&]
[s5; StringStream now has SizeLimit option, if breached (when storing 
data to StringStream), exception is thrown.&]
[s5; NanoStrings class is removed.&]
[s0; Remove HashFn&]
[s0; &]
[s0; Remove precomputed hash&]
[s0; &]
[s0; Create like emplace everywhere&]
[s0; Optimize Index`::Hash&]
[s0; ]]