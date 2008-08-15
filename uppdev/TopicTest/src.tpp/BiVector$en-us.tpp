TITLE("BiVector class template")
REF("T *vector")
REF("T& AddTail()")
REF("void AddHead(const T& x)")
REF("void AddTail(const T& x)")
REF("void AddHeadPick(pick_ T& x)")
REF("void AddTailPick(pick_ T& x)")
REF("T& Head()")
REF("T& Tail()")
REF("const T& Head() const")
REF("const T& Tail() const")
REF("void DropHead()")
REF("void DropTail()")
REF("T& operator[](int i)")
REF("const T& operator[](int i) const")
REF("void Shrink()")
REF("bool IsEmpty() const")
REF("void Clear()")
REF("void Reserve(int n)")
REF("int GetAlloc() const")
REF("friend BiVector& operator<<(BiVector& b, const T& x)")
REF("friend BiVector& operator>>(const T& x, BiVector& b)")
REF("void Serialize(Stream& s)")
REF("bool IsPicked()")
REF("~BiVector()")
REF("void operator=(pick_ BiVector& src)")
REF("BiVector()")
REF("typedef T ValueType")
REF("ConstIterator Begin() const")
REF("ConstIterator End() const")
REF("ConstIterator GetIter(int pos) const")
REF("Iterator Begin()")
REF("Iterator End()")
REF("Iterator GetIter(int pos)")
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
"5:package`-title][{_}%EN-US [s10;~~~64; template <class [*@4 T]>&][s10;~~~64;* [*"
" class ]BiVector&][s0; &][s1;l0;~~~.704; [*C@4 T]-|Type of elements stored in Vec"
"tor. T is required to be [*/ moveable ]and must have either [*/ deep copy constru"
"ctor], [*/ pick constructor] or [*/ default constructor].&][s5; Vector flavor of "
"bidirectional container. Allows adding elements at both sides of sequence at cons"
"tant amortized time.&][s5; Like any other NTL container, BiVector is [*/ moveable"
"][/  ]type with [*/ pick and optional deep copy] transfer semantics. Calling meth"
"ods of picked BiVector is logic error with exception of&][s0;C+75 void [* operato"
"r=](pick`_ Vector`& [*@3 v])&][s0;/+75 [/C void ][*/C operator<<=][/C (const Vect"
"or`& ][*/C@3 v][/C )][/  ](defined using DeepCopyOptionTemplate)&][s0;C+75 void ["
"* Clear]()&][s0;C+75 bool [* IsPicked]() const&][s5; Opti")
TOPIC_TEXT(
"onal deep copy is implemented through DeepCopyOptionTemplate macro.&][s3; Member"
"s&][s4;:T `*vector: T`& [* AddHead]()&][s6; Adds new default constructed element "
"at the head of BiVector. The new element will be at position 0.&][s11; Requires T"
" to have default constructor.&][s11; Invalidates iterators and references to BiVe"
"ctor.&][s1; [*/ Return value]-|Reference to newly added default constructed eleme"
"nt.&][s4;:T`& AddTail`(`): T`& [* AddTail]()&][s6; Adds new default constructed e"
"lement at the tail of BiVector. The new element will be at position GetCount() - "
"1.&][s11; Requires T to have default constructor.&][s11; Invalidates iterators an"
"d references to BiVector.&][s1; [*/ Return value]-|Reference to newly added defau"
"lt constructed element.&][s4;:void AddHead`(const T`& x`): void [* AddHead](const"
" T`& [*@3 x])&][s6; Adds new element with specified value at the head of BiVector"
". The new element will be at position 0.&][s11; Requires T to have deep copy cons"
"tructor.&][s11; Invalidates iterators and references to BiVector.&][s1; [*C@3 x]-"
"|The value that is copied to the newly created element.&][s4;:void AddTail`(const"
" T`& x`): void [* AddTail](const T`& [*@3 x])&][s6; Adds new element with specifi"
"ed value at the tail of BiVector. The new element will be at position GetCount() "
"- 1.&][s11; Requires T to have deep copy constructor.&][s11; Invalidates iterator"
"s and references to BiVector.&][s1; [*C@3 x]-|The value that is copied to the new"
"ly created element.&][s4;:void AddHeadPick`(pick`_ T`& x`): void [* AddHeadPick]("
"pick`_ T`& [*@3 x])&][s6; Adds new element at the head of BiVector and picks valu"
"e of parameter to it. The new element will be at position 0.&][s11; Requires T to"
" have pick constructor.&][s11; Invalidates iterators and references to BiVector.&"
"][s1; [*C@3 x]-|Source instance of T that is to be picked.&][s4;:void AddTailPick"
"`(pick`_ T`& x`): void [* AddTailPick](pick`_ T`& [*@3 x])&][s6; Adds new element"
" at the tail of BiVector and picks value of parameter to ")
TOPIC_TEXT(
"it. The new element will be at position GetCount() - 1.&][s11; Requires T to hav"
"e pick constructor.&][s11; Invalidates iterators and references to BiVector.&][s1"
"; [*C@3 x]-|Source instance of T that is to be picked.&][s4;:T`& Head`(`): T`& [*"
" Head]()&][s6; Returns reference to the head of BiVector. Same as operator`[`](0)"
".&][s1; [*/ Return value]-|Reference to the head of BiVector.&][s4;:T`& Tail`(`):"
" T`& [* Tail]()&][s6; Returns reference to the tail of BiVector. Same as operator"
"`[`](GetCount() - 1).&][s1; [*/ Return value]-|Reference to the tail of BiVector."
"&][s4;:const T`& Head`(`) const: const T`& [* Head]() const&][s6; Returns referen"
"ce to the head of BiVector. Same as operator`[`](0).&][s1; [*/ Return value]-|Con"
"stant reference to the head of BiVector.&][s4;:const T`& Tail`(`) const: const T`"
"& [* Tail]() const&][s6; Returns reference to the tail of BiVector. Same as opera"
"tor`[`](GetCount() - 1).&][s1; [*/ Return value]-|Constant reference to the tail "
"of BiVector.&][s4;:void DropHead`(`): void [* DropHead]()&][s6; Removes element a"
"t the head of BiVector.&][s11; Invalidates iterators and references to BiVector.&"
"][s4;:void DropTail`(`): void [* DropTail]()&][s6; Removes element at the tail of"
" BiVector.&][s11; Invalidates iterators and references to BiVector.&][s4;:T`& ope"
"rator`[`]`(int i`): T`& [* operator]`[`](int [*@3 i])&][s6; Returns a reference t"
"o the element at specified position.&][s1; [*C@3 i]-|Position of element.&][s1; ["
"*/ Return value]-|Reference to the element.&][s4;:const T`& operator`[`]`(int i`)"
" const: const T`& [* operator]`[`](int [*@3 i]) const&][s6; Returns a reference t"
"o the element at specified position.&][s1; [*C@3 i]-|Position of element.&][s1; ["
"*/ Return value]-|Constant reference to the element.&][s4;:void Shrink`(`): int ["
"* GetCount]() const&][s6; Returns the number of elements in BiVector.&][s1; [*/ R"
"eturn value]-|Actual number of elements.&][s4;:bool IsEmpty`(`) const: bool [* Is"
"Empty]() const&][s6; Tests whether BiVector is empty. Sam")
TOPIC_TEXT(
"e as GetCount() == 0.&][s1; [*/ Return value]-|true if Vector is empty, false ot"
"herwise.&][s4;:void Clear`(`): void [* Clear]()&][s6; Removes all elements from B"
"iVector.&][s4;~~~.992; void [* Shrink]()&][s6; Minimizes memory consumption of Bi"
"Vector by minimizing capacity.&][s4;:void Reserve`(int n`): void [* Reserve](int "
"[*@3 n])&][s6; Reserves capacity. If required capacity is greater than the curren"
"t capacity, capacity is increased to the required value.&][s1; [*C@3 n]-|Required"
" capacity.&][s4;:int GetAlloc`(`) const: int [* GetAlloc]() const&][s6; Returns c"
"urrent capacity of BiVector.&][s1; [*/ Return value]-|Capacity of BiVector.&][s4;"
":friend BiVector`& operator`<`<`(BiVector`& b`, const T`& x`): friend BiVector`& "
"[* operator]<<(BiVector`& [*@3 b], const T`& [*@3 x])&][s6; Operator replacement "
"of [* void AddTail(const T`&x)]. By returning reference to the BiVector allows ad"
"ding more elements in single expression, thus e.g. allowing to construct temporar"
"y BiVector as part of expression like Foo(BiVector<int>() << 1 << 2 << 4).&][s11;"
" Requires T to have deep copy constructor.&][s11; Invalidates iterators and refer"
"ences to BiVector.&][s1; [*C@3 x]-|The value that is copied to the newly created "
"element.&][s1; [*/ Return value]-|Reference to BiVector (that is *this).&][s4;:fr"
"iend BiVector`& operator`>`>`(const T`& x`, BiVector`& b`): friend BiVector`& [* "
"operator]>>(const T`& [*@3 x], BiVector`& [*@3 b])&][s6; Operator replacement of "
"[* void AddHead(const T`&x)]. By returning reference to the BiVector allows addin"
"g more elements in single expression, thus e.g. allowing to construct temporary B"
"iVector as part of expression like Foo(1 >> (2 >> BiVector<int>())).&][s11; Requi"
"res T to have deep copy constructor.&][s11; Invalidates iterators and references "
"to BiVector.&][s1; [*C@3 x]-|The value that is copied to the newly created elemen"
"t.&][s1; [*/ Return value]-|Reference to BiVector (that is *this).&][s4;:void Ser"
"ialize`(Stream`& s`): void [* Serialize](Stream`& [*@3 s]")
TOPIC_TEXT(
")&][s6; Serializes content of BiVector to/from Stream. Works only if NTL is used"
" as part of UPP.&][s11; Requires T to have serialization operator defined.&][s1; "
"[*C@3 s]-|Target/source stream.&][s4;:bool IsPicked`(`): [* BiVector]()&][s6; Def"
"ault constructor. Constructs empty BiVector.&][s4; ~[* BiVector]()&][s6; Destruct"
"or. Calls destructors of all elements in BiVector.&][s4;:`~BiVector`(`): [* BiVec"
"tor](pick`_ BiVector`& [*@3 src])&][s6; Pick constructor. Transfers source BiVect"
"or in low constant time, but destroys it by picking.&][s1; [*C@3 src]-|Source BiV"
"ector.&][s4;:void operator`=`(pick`_ BiVector`& src`): void [* operator]=(pick`_ "
"BiVector`& [*@3 src])&][s6; Pick operator. Transfers source BiVector in low const"
"ant time, but destroys it by picking.&][s1; [*C@3 v]-|Source BiVector.&][s4; bool"
" [* IsPicked]()&][s6; Returns [* true] if BiVector is in picked state.&][s1; [*/ "
"Return value]-|[* true] if BiVector is in picked state, [* false] otherwise.&][s4"
";:BiVector`(`): [* BiVector](const BiVector`& [*@3 src], int)&][s6; Optional deep"
" copy constructor.&][s11; Requires T to have deep copy constructor or optional de"
"ep copy constructor.&][s1; [*C@3 v]-|Source BiVector.&][s4; void [* operator<<=]("
"const BiVector`& [*@3 v])&][s6; Optional deep copy operator. Defined using DeepCo"
"pyOptionTemplate.&][s11; Requires T to have deep copy operator or optional deep c"
"opy operator.&][s1; [*C@3 v]-|Source Vector.&][s4;:typedef T ValueType:* [* typed"
"ef T ]ValueType&][s6; Typedef of T for use in templated algorithms.&][s4;:typedef"
" T ValueType: typedef [/ type] [* Iterator];&][s6; Iterator type.&][s4; typedef ["
"/ type] [* ConstIterator];&][s6; Constant iterator type.&][s4;:ConstIterator Begi"
"n`(`) const: ConstIterator [* Begin]() const&][s6; Returns constant iterator to t"
"he first element in BiVector.&][s1; [*/ Return value]-|Iterator.&][s4;:ConstItera"
"tor End`(`) const: ConstIterator [* End]() const&][s6; Returns constant iterator "
"to the position just beyond the last element in BiVector.")
TOPIC_TEXT(
"&][s1; [*/ Return value]-|Iterator.&][s4;:ConstIterator GetIter`(int pos`) const"
": ConstIterator [* GetIter](int [*@3 pos]) const&][s6; Returns constant iterator "
"to the element at specified position. Same as [* Begin() + i]. Benefit of this me"
"thods is that in debug mode [* pos] is range checked.&][s1; [*C@3 i]-|Required po"
"sition.&][s1; [*/ Return value]-|Iterator.&][s4;:Iterator Begin`(`): Iterator [* "
"Begin]()&][s6; Returns non-constant iterator to the first element in Vector.&][s1"
"; [*/ Return value]-|Iterator.&][s4;:Iterator End`(`): Iterator [* End]()&][s6; R"
"eturns non-constant iterator to the position just beyond the last element in Vect"
"or.&][s1; [*/ Return value]-|Iterator.&][s4;:Iterator GetIter`(int pos`): Iterato"
"r [* GetIter](int [*@3 pos])&][s6; Returns non-constant iterator to the element a"
"t specified position. Same as [* Begin() + pos]. Benefit of this methods is that "
"in debug mode [* pos] is range checked.&][s1; [*C@3 i]-|Required position.&][s1; "
"[*/ Return value]-|Iterator.&][s4; friend void [* Swap](BiVector`& [*@3 a], BiVec"
"tor`& [*@3 b])&][s6; Specialization of generic [* Swap] for BiVector. Swaps BiVec"
"tor in low constant time operation.&][s1; [*C@3 a]-|First BiVector to swap.&][s1;"
" [*C@3 b]-|Second BiVector to swap.&][s0; ")
