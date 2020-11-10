topic "New features of U++ Core 2016 (C++11)";
[l288;i1120;a17;O9;~~~.1408;2 $$1,0#10431211400427159095818037425705:param]
[a83;*R6 $$2,5#31310162474203024125188417583966:caption]
[H4;b83;*4 $$3,5#07864147445237544204411237157677:title]
[i288;O9;C2 $$4,6#40027414424643823182269349404212:item]
[b42;a42;ph2 $$5,5#45413000475342174754091244180557:text]
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
[2 $$0,0#00000000000000000000000000000000:Default]
[{_}%EN-US 
[s2; New features of U`+`+ Core 2016 (C`+`+11)&]
[s3; Table of contents&]
[s0; &]
[s0; [^topic`:`/`/Core`/srcdoc`/Core2016`_en`-us`#1^ 1. Allocator]&]
[s0; [^topic`:`/`/Core`/srcdoc`/Core2016`_en`-us`#2^ 2. Multithreading]&]
[s0; [^topic`:`/`/Core`/srcdoc`/Core2016`_en`-us`#3^ 3. Callback changes]&]
[s0; [^topic`:`/`/Core`/srcdoc`/Core2016`_en`-us`#4^ 4. Algorithms 
and Containers]&]
[s0; [^topic`:`/`/Core`/srcdoc`/Core2016`_en`-us`#5^ 5. Tuple]&]
[s0; [^topic`:`/`/Core`/srcdoc`/Core2016`_en`-us`#6^ 6. Changes in 
r`-value and pick]&]
[s0; [^topic`:`/`/Core`/srcdoc`/Core2016`_en`-us`#7^ 7. Smaller improvements]&]
[s0; &]
[s3;:1: 1. Allocator&]
[s5; U`+`+ allocator now always returns 16`-byte aligned blocks. 
This is to simplify SSE requirements, at the price of smallest 
possible allocation being 32`-bytes (request for smaller blocks 
are rounded up to 32`-bytes).&]
[s5; In situation where allocating a lot of blocks smaller than 32 
bytes (typical is controlling reference counter block in shared`_ptr 
like class), new [* TinyAlloc]/[* TinyFree] functions can be used, 
or templated `"new/delete`" like functions [* tiny`_new]/[* tiny`_delete].&]
[s3;:2: 2. Multithreading&]
[s5; One of biggest advances of C`+`+11 is support for multithreading 
memory model. That is why we could drop U`+`+ memory model support 
and use what language/standard library provides. [* Atomic ]is 
now reimplemented using [* std`::atomic<int>], thread local variables 
are language standard. OTOH, beyond memory model, U`+`+ [* Thread 
]class and [* CoWork ]class are still basis of multithreading.&]
[s5; [* ConditionVariable ]is now recommended over [* Semaphore]. We 
keep U`+`+ class for this one, because in Win32 [* ConditionVariable 
]requires Windows 7 and we still want to keep WinXP support (U`+`+ 
[* ConditionVariable ]contains internal implementation when system 
API is missing).&]
[s5; [* Thread`::Exit] can be used to exit the thread (from within), 
similar to [* Exit ]to exit the whole program. It throws exception, 
which is caught at the end of thread routine.&]
[s5; [* CoWork ]was optimized and also changed so that each `"master 
thread`" has private thread pool. This is important to avoid 
stealing of work between master threads, which can lead to problems 
(e.g. delays in the GUI).&]
[s5; [* CoWork`::FinLock] now provides `'finalization lock`', useful 
to avoid additional mutex lock for e.g. storing results.&]
[s5; [* CoWork ]now also has pipeline support, where thread performs 
one specific task and eventually passes results to another thread 
to continue processing.&]
[s3;:3: 3. Callback changes&]
[s5; Callbacks are refactored with C`+`+11 lambdas and varargs templates. 
There are now 3 classes:&]
[s5; [* Function ]is similar to [* std`::function], generic callable with 
any number of parameters and any return value in addition it 
has ability to `'add`' functions using operator<<. Also, unlike 
[* std`::function], calling empty Function is no operation, not 
exception.&]
[s5; [* Event ]is equivalent of Callback `- unlike Callback, it is 
not necessary to have Callback, Callback1, Callback2, number 
of parameters is resolved by C`+`+11 template varargs.&]
[s5; Callback is deprecated but supported for backward compatibility.&]
[s3;:4: 4. Algorithms and Containers&]
[s5; The set of algorithms provided by U`+`+ is now streamlined by 
introduction of [/ Range] concept. Range is entity that provides 
begin/end/GetCount/operator`[`] methods. All U`+`+ algorithms 
expect Range as input. All U`+`+ containers satisfy Range concept. 
In addition there are&]
[s5; [* SubRangeClass ]`- represents subrange of container, or just 
begin/end pair.&]
[s5; [* ConstRangeClass ]`- trivial single`-value range&]
[s5; [* ViewRangeClass ]`- is basically a list of indicies inside another 
range. [* FilterRange] function can create ViewRangeClass based 
on predicate.&]
[s5; U`+`+ containers now support [* InsertRange]/[* AppendRange ]template 
methods.&]
[s5; New, parallel algorithms are introduced: [* CoLoop], [* CoAccumulate], 
[* CoSum], [* CoFindBest], [* CoFindMin], [* CoMin], [* CoFindMax], [* CoMax], 
[* CoFindMatch], [* CoFindIndex], [* CoSort],[*  CoStableSort],[*  CoIndexSort], 
[* CoStableIndexSort], [* CoIndexSort2], [* CoStableIndexSort2], [* CoIndexSort3], 
[* CoStableIndexSort3], [* CoGetSortOrder], [* CoGetStableSortOrder], 
[* CoSortByKey], [* CoSortByValue], [* CoStableSortByKey], [* CoStableSortByValue], 
[* CoSortIndex], [* CoStableSortIndex].&]
[s3;:5: 5. Tuple&]
[s5; Tuple now does not require the specific template type based 
on number of elements (instead of Tuple3<int, int, String> it 
can now be used just Tuple<int, int, String>). Tuple now has 
methods for retrieving the number of elements and index based 
access of elements via Value and also C`+`+11 like access via 
template methods (index based or type based):&]
[s0; &]
[s7; [* Tuple]<int, String> x `= [* MakeTuple](12, (const char `*)`"hello`");&]
[s7; -|&]
[s7; DUMP(x.a);&]
[s7; DUMP(x.b);&]
[s7; DUMP(x);&]
[s7; &]
[s7; DUMP(x.[* GetCount]());&]
[s7; DUMP(x.[* Get](0));&]
[s7; DUMP(x.[* Get](1));&]
[s7; DUMP(x.[* GetArray]());&]
[s7; DUMP(x.[* Get<0>]());&]
[s7; DUMP(x.[* Get<String>]());&]
[s7; -|&]
[s7; x.Set(1, `"bar`");&]
[s7; DUMP(x);&]
[s7; ValueArray va `{ 22, `"world`" `};&]
[s7; x.[* SetArray](va);&]
[s7; DUMP(x);&]
[s0; &]
[s5; Unfortunately, as `'Tuple`' id now represents template class, 
`'Tuple`' function had to be removed. Use MakeTuple instead.&]
[s3;:6: 6. Changes in r`-value and pick&]
[s5; All features required for compatibility with C`+`+99 (pick`_ 
etc...) were removed in favor of using r`-value references. `"picked`" 
state was removed, picking now simply clears the source to `"empty`" 
state. AddPick methods in containers are deprecated, use Add(pick(x)) 
instead. `'pick`' is kept as synonym to std`::move and clone is 
still required (containers do not provide default copy constructor).&]
[s5; Throughout library, `"opportunity`" r`-value constructors/assignments 
are added. That has perhaps the most optimizing effect with chaining 
String operator`+: String a; ... String x `= a`+ `" foo `" `+ 
`"bar`" does not perform any unnecessary copies now.&]
[s3;:7: 7. Smaller improvements&]
[s5; Below is the list of various smaller improvements:&]
[s5;l128;i150;O0; Stream`::SerializeRaw count is now int64 (instead 
of 32`-bit int)&]
[s5;l128;i150;O0; 2GB items limit is better implemented in StringBuffer, 
WStringBuffer, StringStream.&]
[s5;l128;i150;O0; StringStream now has SizeLimit option, if breached 
(when storing data to StringStream), exception is thrown.&]
[s5;l128;i150;O0; NanoStrings class is removed.&]
[s5;l128;i150;O0; HashFn / precomputed hash options in Index/Maps 
are removed.&]
[s5;l128;i150;O0; Index/Maps are further optimized (about 5% gain 
in benchmarks).&]
[s0; ]]