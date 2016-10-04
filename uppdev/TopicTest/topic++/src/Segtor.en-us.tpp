TITLE("Segtor class template")
REF("void Add(const T& x)")
REF("void AddPick(pick_ T& x)")
REF("T& operator[](int i)")
REF("const T& operator[](int i) const")
REF("int GetCount() const")
REF("bool IsEmpty() const")
REF("void SetCount(int n)")
REF("void SetCountR(int n)")
REF("void SetCountR(int n, const T& init)")
REF("T& DoIndex(int i)")
REF("T& DoIndex(int i, const T& x)")
REF("void Shrink()")
REF("void Reserve(int xtra)")
REF("int GetAlloc() const")
REF("void Set(int i, const T& x, int count = 1)")
REF("int GetIndex(const T& item) const")
REF("T& Top()")
REF("const T& Top() const")
REF("T Pop()")
REF("void Swap(Segtor& b)")
REF("Segtor& operator|(pick_ T& x)")
REF("bool IsPicked() const")
REF("void Serialize(Stream& s)")
REF("Segtor(pick_ Segtor& s)")
REF("Segtor(const Segtor& s, int)")
REF("~Segtor()")
REF("typedef ConstIIterator<Segtor> ConstIterator")
REF("typedef T ValueType")
REF("typedef T *Iterator;")
REF("ConstIterator Begin() const")
REF("ConstIterator End() const")
REF("ConstIterator GetIter(int i) const")
REF("Iterator Begin()")
REF("Iterator End()")
REF("Iterator GetIter(int i)")
REF("friend void Swap(Vector& a, Vector& b)")
TOPIC_TEXT(
"[ $$0,0#00000000000000000000000000000000:Default][l288;i704;a17;O9;~~~.992; $$1,"
"0#10431211400427159095818037425705:param][a83;*R6 $$2,5#3131016247420302412518841"
"7583966:caption][b83;* $$3,5#07864147445237544204411237157677:title][b167;a42;C $"
"$4,6#40027414424643823182269349404212:item][b42;a42; $$5,5#4541300047534217475409"
"1244180557:text][l288;a17; $$6,6#27521748481378242620020725143825:desc][l321;t246"
";C@5;1 $$7,7#20902679421464641399138805415013:code][b2503; $$8,0#6514237545610002"
"3862071332075487:separator][*@(0.0.255) $$9,0#83433469410354161042741608181528:ba"
"se][t4167;C+117 $$10,0#37138531426314131251341829483380:class][l288;a17;*1 $$11,1"
"1#70004532496200323422659154056402:requirement][i416;b42;a42;O9;~~~.416; $$12,12#"
"10566046415157235020018451313112:tparam][b167;C $$13,13#9243045944346046191110808"
"0531343:item1][a42;C $$14,14#77422149456609303542238260500223:item2][*@2$(0.128.1"
"28) $$15,15#34511555403152284025741354420178:NewsDate][l321;*C$7 $$16,16#03451589"
"433145915344929335295360:result][l321;b83;a83;*C$7 $$17,17#0753155046352950537122"
"8428965313:result`-line][l160;t4167;*C+117 $$18,5#8860394944220582595880005322242"
"5:package`-title][{_}%EN-US [s10;~~~64; template <class [*@4 T], int [*@4 NBLK] ="
" 16>&][s10;~~~64;* [* class ]Segtor&][s12; [*C@4 T]-|Type of elements stored in S"
"egtor. T  must have either [*/ deep copy constructor], [*/ pick constructor] or ["
"*/ default constructor].&][s12; [*C@4 NBLK]-|Size of segments.&][s5;~~~.416; Segt"
"or is special flavor of random access container that can be sometimes used to imp"
"rove memory consumption. It never invalidates iterators nor references to element"
"s, but it also lacks operations to insert and remove elements at any position. Se"
"gtor is designed to be used in situation where sizeof element is small, but it is"
" either non-moveable or references to elements must be never invalidated.&][s5;~~"
"~.416; Typical implementation stores elements in segments of fixed size. This fix"
"ed size is determined by template parameter NBLK.&][s5;~~")
TOPIC_TEXT(
"~.416; Iterators to Segtor satisfy all C`++ standard library requirements for ra"
"ndom access iterator plus they allow assignment (and copy constructor) and testin"
"g for 0 (that is NULL) pointer.&][s5; Like any other NTL container, Segtor is [*/"
" moveable ]type with [*/ pick and optional deep copy] transfer semantics. Calling"
" methods of picked Segtor is logic error with exception of&][s0;C+75 void [* oper"
"ator=](pick`_ Array`& [*@3 v])&][s0;/+75 [/C void ][*/C operator<<=][/C (const Ar"
"ray`& ][*/C@3 v][/C )][/  ](defined using DeepCopyOptionTemplate)&][s0;C+75 void "
"[* Clear]()&][s0;C+75 bool [* IsPicked]() const&][s0;b42;a42; Optional deep copy "
"is implemented through DeepCopyOptionTemplate macro.&][s3; Members&][s4; T`& [* A"
"dd]()&][s6; Adds new default constructed element to Segtor.&][s11; Requires T to "
"have default constructor.&][s1; [*/ Return value]-|Reference to newly added defau"
"lt constructed element.&][s4;:void Add`(const T`& x`): void [* Add](const T`& [*@"
"3 x])&][s6; Adds new element with specified value to Segtor.&][s11; Requires T to"
" have deep copy constructor.&][s1; [*C@3 x]-|The value that is copied to newly cr"
"eated element.&][s4;:void AddPick`(pick`_ T`& x`): void [* AddPick](pick`_ T`& [*"
"@3 x])&][s6; Adds new element to Segtor and picks value of parameter to it. &][s1"
"1; Requires T to have pick constructor.&][s1; [*C@3 x]-|Source instance of T that"
" is to be picked.&][s4;:T`& operator`[`]`(int i`): T`& [* operator]`[`](int [*@3 "
"i])&][s6; Returns a reference to the element at specified position.&][s1; [*C@3 i"
"]-|Position of element.&][s1; [*/ Return value]-|Reference to the element.&][s4;:"
"const T`& operator`[`]`(int i`) const: const T`& [* operator]`[`](int [*@3 i]) co"
"nst&][s6; Returns a reference to the element at specified position.&][s1; [*C@3 i"
"]-|Position of element.&][s1; [*/ Return value]-|Constant reference to the elemen"
"t.&][s4;:int GetCount`(`) const: int [* GetCount]() const&][s6; Return the number"
" of elements in Segtor.&][s1; [*/ Return value]-|Actual n")
TOPIC_TEXT(
"umber of elements.&][s4;:bool IsEmpty`(`) const: bool [* IsEmpty]() const&][s6; "
"Tests whether Segtor  is empty. Same as GetCount() == 0.&][s1; [*/ Return value]-"
"|true if Vector is empty, false otherwise.&][s4;:void SetCount`(int n`): void [* "
"SetCount](int [*@3 n])&][s6; Changes count of elements in Segtor to specified val"
"ue. If required number of elements is greater than actual number, newly added ele"
"ments are default constructed.&][s11; Requires T to have default constructor.&][s"
"1; [*C@3 n]-|Required number of elements.&][s4;:void SetCountR`(int n`): void [* "
"SetCount](int [*@3 n], const T`& [*@3 init])&][s6; Changes count of elements in S"
"egtor to specified value. If required number of elements is greater than actual n"
"umber, newly added elements are initialized to specified value using copy constru"
"ctor.&][s11; Requires T to have deep copy constructor.&][s1; [*C@3 n]-|Required n"
"umber of elements.&][s1; [*C@3 init]-|Initialization value of newly added element"
"s.&][s4;:void SetCountR`(int n`, const T`& init`): void [* Clear]()&][s6; Removes"
" all elements from Segtor. Capacity is also cleared to zero.&][s4;:T`& DoIndex`(i"
"nt i`): T`& [* At](int [*@3 i])&][s6; If specified position is lower than number "
"of elements in Segtor ([* i < GetCount()]), returns reference to element at speci"
"fied position. Otherwise increases number of elements in Segtor to [* i + 1]. New"
"ly added elements are default constructed. &][s11; Requires T to have default con"
"structor.&][s1; [*C@3 i]-|Position of required element.&][s1; [*/ Return value]-|"
"Reference to required element.&][s4;:T`& DoIndex`(int i`, const T`& x`): T`& [* A"
"t](int [*@3 i], const T`& [*@3 x])&][s6; If specified position is lower than numb"
"er of elements in Segtor ([* i < GetCount()]), returns reference to element at sp"
"ecified position. Otherwise increases number of elements in Segtor to [* i + 1]. "
"Newly added elements are copy constructed from [* x].&][s11; Requires T to have d"
"eep copy constructor.&][s1; [*C@3 i]-|Position of require")
TOPIC_TEXT(
"d element.&][s1; [*C@3 x]-|Initialization value of newly added elements.&][s1; ["
"*/ Return value]-|Reference to required element.&][s4;:void Shrink`(`): void [* S"
"hrink]()&][s6; Minimizes memory consumption of Segtor.&][s1; [*/ Return value]-|&"
"][s4;:void Reserve`(int xtra`): void [* Reserve](int [*@3 xtra])&][s6; Reserves c"
"apacity. If required capacity is greater than current capacity, capacity is incre"
"ased to the required value.&][s1; [*C@3 xtra]-|Required capacity.&][s4;:int GetAl"
"loc`(`) const: int [* GetAlloc]() const&][s6; Returns current capacity of Segtor."
"&][s1; [*/ Return value]-|Capacity of Segtor.&][s4;:void Set`(int i`, const T`& x"
"`, int count `= 1`): void [* Set](int [*@3 i], const T`& [*@3 x], int [*@3 count]"
" = 1)&][s6; Sets requested number of elements starting at the position [* i] to t"
"he specified value. If required number of elements exceeds existing elements of S"
"egtor then elements are added to Segtor.&][s11; Requires T to have deep copy cons"
"tructor.&][s1; [*C@3 i]-|Starting position.&][s1; [*C@3 x]-|Value.&][s1; [*C@3 co"
"unt]-|Number of elements.&][s4;:int GetIndex`(const T`& item`) const: void [* Dro"
"p](int [*@3 n] = 1)&][s6; Drops specified number of last elements in the Segtor.&"
"][s1; [*C@3 n]-|Number of elements.&][s4;:T`& Top`(`): [* T]`& [* Top]()&][s6; Re"
"turns reference to the last element in the Segtor.&][s1; [*/ Return value]-|Refer"
"ence of last element in the Segtor.&][s4;:const T`& Top`(`) const: const [* T]`& "
"[* Top]() const&][s6; Returns constant reference to the last element in the Segto"
"r.&][s1; [*/ Return value]-|Reference of last element in the Segtor.&][s4;:T Pop`"
"(`): T [* Pop]()&][s6; Drops last element of Segtor and returns its value.&][s11;"
" Requires T to have deep copy constructor.&][s1; [*/ Return value]-|Value of drop"
"ped element.&][s4;:void Swap`(Segtor`& b`): Segtor`& [* operator]<<(const T`& [*@"
"3 x])&][s6; Operator replacement of [* void Add(const T`&x)]. By returning refere"
"nce to to Segtor allows adding more elements in single ex")
TOPIC_TEXT(
"pression, thus e.g. allowing to construct temporary Segtor as part of expression"
" like Foo((Segtor<int>() << 1 << 2 << 4)).&][s11; Requires T to have deep copy co"
"nstructor.&][s1; [*C@3 x]-|The value that is copied to newly created element.&][s"
"1; [*/ Return value]-|Reference to Segtor (*this).&][s4;:Segtor`& operator`|`(pic"
"k`_ T`& x`): Segtor`& [* operator]|(pick`_ T`& [*@3 x])&][s6; Operator replacemen"
"t of [* void AddPick(pick`_ T`&x)]. By returning reference to to Segtor allows ad"
"ding more elements in single expression, thus e.g. allowing to construct temporar"
"y Segtor as part of expression.&][s11; Requires T to have pick constructor.&][s1;"
" [*C@3 x]-|Source instance of T that is to be picked.&][s1; [*/ Return value]-|Re"
"ference to Segtor (*this).&][s4;:bool IsPicked`(`) const: void [* Serialize](Stre"
"am`& [*@3 s])&][s6; Serializes content of Vector to/from Stream. Works only if NT"
"L is used as part of UPP.&][s11; Requires T to have serialization operator define"
"d.&][s1; [*C@3 s]-|Target/source stream.&][s4;~~~.992; bool [* IsPicked]() const&"
"][s6; Returns [* true] if Segtor is in picked state.&][s1; [*/ Return value]-|[* "
"true] if Segtor is in picked state, [* false] otherwise.&][s4;:void Serialize`(St"
"ream`& s`): [* Segtor]()&][s6; Default constructor. Constructs empty Segtor.&][s4"
";:Segtor`(pick`_ Segtor`& s`): [* Segtor](pick`_ [* Segtor]`& [*@3 s])&][s6; Pick"
" constructor. Transfers source Segtor in low constant time, but destroys it by pi"
"cking.&][s1; [*C@3 v]-|Source Segtor.&][s4;:Segtor`(const Segtor`& s`, int`): [* "
"Segtor](const [* Segtor]`& [*@3 s], int)&][s6; Optional deep copy constructor.&]["
"s11; Requires T to have deep copy constructor or optional deep copy constructor.&"
"][s1; [*C@3 v]-|Source array.&][s4;:`~Segtor`(`): ~[* Segtor]()&][s6; Destructor."
" Calls destructors of all elements in Vector.&][s4;:typedef ConstIIterator`<Segto"
"r`> ConstIterator: void [* operator<<=](const Segtor`& [*@3 v])&][s6; Optional de"
"ep copy operator. Defined using DeepCopyOptionTemplate.&]")
TOPIC_TEXT(
"[s11; Requires T to have deep copy constructor.&][s1; [*C@3 v]-|Source Segtor.&]"
"[s4;:typedef T ValueType:* [* typedef T ]ValueType&][s6; Typedef of T for use in "
"templated algorithms.&][s4;:typedef T `*Iterator`;: typedef [/ type] [* Iterator]"
";&][s6; Iterator type.&][s4; typedef [/ type] [* ConstIterator];&][s6; Constant i"
"terator type.&][s4;:ConstIterator Begin`(`) const: ConstIterator [* Begin]() cons"
"t&][s6; Returns constant iterator to the first element in Segtor.&][s1; [*/ Retur"
"n value]-|Iterator.&][s4;:ConstIterator End`(`) const: ConstIterator [* End]() co"
"nst&][s6; Returns constant iterator to the position just beyond the last element "
"in Segtor.&][s1; [*/ Return value]-|Iterator.&][s4;:ConstIterator GetIter`(int i`"
") const: ConstIterator [* GetIter](int [*@3 i]) const&][s6; Returns constant iter"
"ator to the element at specified position. Same as [* Begin() + pos]. Benefit of "
"this methods is that in debug mode [* pos] is range checked.&][s1; [*C@3 i]-|Requ"
"ired position.&][s1; [*/ Return value]-|Iterator.&][s4;:Iterator Begin`(`): Itera"
"tor [* Begin]()&][s6; Returns non-constant iterator to the first element in Segto"
"r.&][s1; [*/ Return value]-|Iterator.&][s4;:Iterator End`(`): Iterator [* End]()&"
"][s6; Returns non-constant iterator to the position just beyond the last element "
"in Segtor.&][s1; [*/ Return value]-|Iterator.&][s4;:Iterator GetIter`(int i`): It"
"erator [* GetIter](int [*@3 i])&][s6; Returns non-constant iterator to the elemen"
"t at specified position. Same as [* Begin() + pos]. Benefit of this methods is th"
"at in debug mode [* pos] is range checked.&][s1; [*C@3 i]-|Required position.&][s"
"1; [*/ Return value]-|Iterator.&][s4;:friend void Swap`(Vector`& a`, Vector`& b`)"
": friend void [* Swap](Segtor`& [*@3 a], Segtor`& [*@3 b])&][s6; Specialization o"
"f generic [* Swap] for Segtor. Swaps array in simple constant time operation.&][s"
"1; [*C@3 a]-|First Segtor to swap.&][s1; [*C@3 b]-|Second Segtor to swap.")
