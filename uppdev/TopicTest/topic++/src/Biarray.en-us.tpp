TITLE("BiArray class template")
REF("T& AddTail()")
REF("void AddHead(const T& x)")
REF("void AddTail(const T& x)")
REF("void AddHeadPick(pick_ T& x)")
REF("void AddTailPick(pick_ T& x)")
REF("void AddHead(T *newt)")
REF("void AddTail(T *newt)")
REF("T& Head()")
REF("T& Tail()")
REF("const T& Head() const")
REF("const T& Tail() const")
REF("void DropHead()")
REF("void DropTail()")
REF("T *DetachHead()")
REF("T *DetachTail()")
REF("T& operator[](int i)")
REF("const T& operator[](int i) const")
REF("void Shrink()")
REF("bool IsEmpty() const")
REF("void Clear()")
REF("void Reserve(int n)")
REF("int GetAlloc() const")
REF("friend BiArray& operator<<(BiArray& b, const T& x)")
REF("friend BiArray& operator>>(const T& x, BiArray& b)")
REF("void Serialize(Stream& s)")
REF("bool IsPicked() const")
REF("BiArray(const BiArray& v, int)")
REF("BiArray(pick_ BiArray& src)")
REF("void operator=(pick_ BiArray& src)")
REF("BiArray()")
REF("~BiArray()")
REF("typedef T ValueType")
REF("typedef T *Iterator;")
REF("typedef ConstIIterator<BiArray> ConstIterator")
REF("Iterator End()")
REF("Iterator GetIter(int pos)")
REF("ConstIterator Begin() const")
REF("ConstIterator End() const")
REF("ConstIterator GetIter(int pos) const")
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
" class ]BiArray&][s0; &][s12; [*C@4 T]-|Type or base class of elements stored in "
"BiArray. There is no common requirement for T.&][s5; The universal form of bidire"
"ctional random access container. Its features are derived from fact that typicall"
"y it is  implemented using indirect container of pointers to T like BiVector<T*>."
" Nevertheless it supports common concepts as ownership of contained elements, ref"
"erence (not pointer) access to elements and so on.&][s5; It provides almost all o"
"perations of BiVector with the same semantics and almost any BiVector can be dire"
"ctly replaced by BiArray. On the other hand, it provides some special operations "
"impossible for BiVector and most important, it never invalidates references (that"
" means C`++ references and pointers) to elements (it ofte")
TOPIC_TEXT(
"n invalidates iterators, though).&][s5; BiArray can be also used to store polymo"
"rphic elements - stored elements could be derived from T. To store such elements,"
" you pass pointer to element created on the heap. Still, BiArray takes over owner"
"ship of such element (it e.g. deletes it when appropriate). You can also use this"
" method to create BiArray of elements that do not have pick nor deep copy constru"
"ctor.&][s5; There are also operations that allow detaching an element from BiArra"
"y, removing it but not destroying. Pointer to such an element is returned from th"
"is operation and BiArray gives up ownership.&][s5; Disadvantage of BiArray over B"
"iVector is performance - most operations are significantly slower than BiVector's"
" one (by factor up to 8, depends on speed of malloc/free).&][s5; As for memory, f"
"or small size of elements, BiArray memory consumption is significantly worse than"
" BiVector consumption. As size of elements grows, BiArray starts to be better tha"
"n BiVector.&][s5;~~~.704; Iterators to BiArray satisfy all C`++ standard library "
"requirements for random access iterator plus they allow assignment (and copy cons"
"tructor) and testing for 0 (that is NULL) pointer.&][s5; Like any other NTL conta"
"iner, BiArray is [*/ moveable ]type with [*/ pick and optional deep copy] transfe"
"r semantics. Calling methods of picked BiArray is logic error with exception of&]"
"[s0;C+75 void [* operator=](pick`_ BiArray`& [*@3 v])&][s0;/+75 [/C void ][*/C op"
"erator<<=][/C (const BiArray`& ][*/C@3 v][/C )][/  ](defined using DeepCopyOption"
"Template)&][s0;C+75 void [* Clear]()&][s0;C+75 bool [* IsPicked]() const&][s5; Op"
"tional deep copy is implemented through DeepCopyOptionTemplate macro.&][s3; Membe"
"rs&][s4; T`& [* AddHead]()&][s6; Adds new default constructed element at the head"
" of BiArray. The new element will be at position 0.&][s11; Requires T to have def"
"ault constructor.&][s11; Invalidates iterators  to BiArray.&][s1; [*/ Return valu"
"e]-|Reference to newly added default constructed element.")
TOPIC_TEXT(
"&][s4;:T`& AddTail`(`): T`& [* AddTail]()&][s6; Adds new default constructed ele"
"ment at the tail of BiArray. The new element will be at position GetCount() - 1.&"
"][s11; Requires T to have default constructor.&][s11; Invalidates iterators  to B"
"iArray.&][s1; [*/ Return value]-|Reference to newly added default constructed ele"
"ment.&][s4;:void AddHead`(const T`& x`): void [* AddHead](const T`& [*@3 x])&][s6"
"; Adds new element with specified value at the head of BiArray. The new element w"
"ill be at position 0.&][s11; Requires T to have deep copy constructor.&][s11; Inv"
"alidates iterators to BiArray.&][s1; [*C@3 x]-|The value that is copied to the ne"
"wly created element.&][s4;:void AddTail`(const T`& x`): void [* AddTail](const T`"
"& [*@3 x])&][s6; Adds new element with specified value at the tail of BiArray. Th"
"e new element will be at position GetCount() - 1.&][s11; Requires T to have deep "
"copy constructor.&][s11; Invalidates iterators to BiArray.&][s1; [*C@3 x]-|The va"
"lue that is copied to the newly created element.&][s4;:void AddHeadPick`(pick`_ T"
"`& x`): void [* AddHeadPick](pick`_ T`& [*@3 x])&][s6; Adds new element at the he"
"ad of BiArray and picks value of parameter to it. The new element will be at posi"
"tion 0.&][s11; Requires T to have pick constructor.&][s11; Invalidates iterators "
"to BiArray.&][s1; [*C@3 x]-|Source instance of T that is to be picked.&][s4;:void"
" AddTailPick`(pick`_ T`& x`): void [* AddTailPick](pick`_ T`& [*@3 x])&][s6; Adds"
" new element at the tail of BiArray and picks value of parameter to it. The new e"
"lement will be at position GetCount() - 1.&][s11; Requires T to have pick constru"
"ctor.&][s11; Invalidates iterators to BiArray.&][s1; [*C@3 x]-|Source instance of"
" T that is to be picked.&][s4;:void AddHead`(T `*newt`): void [* AddHead](T *[*@3"
" newt])&][s6; Adds new element at the head of BiArray. Element is specified by po"
"inter to an object. BiArray takes over ownership of this this object. This varian"
"t allows use of BiArray as polymorphic container, because")
TOPIC_TEXT(
" type of added element can be also derived from T as well. No constructor is app"
"lied. The new element will be at position 0.&][s11; Invalidates iterators to BiAr"
"ray.&][s1; [*C@3 newt]-|The object to be added.&][s1; [*/ Return value]-|Referenc"
"e to the object - [* *newt].&][s4;:void AddTail`(T `*newt`): void [* AddTail](T *"
"[*@3 newt])&][s6; Adds new element at the tail of BiArray. Element is specified b"
"y pointer to an object. BiArray takes over ownership of this this object. This va"
"riant allows use of BiArray as polymorphic container, because type of added eleme"
"nt can be also derived from T as well. No constructor is applied. The new element"
" will be at position GetCount() - 1.&][s11; Invalidates iterators to BiArray.&][s"
"1; [*C@3 newt]-|The object to be added.&][s1; [*/ Return value]-|Reference to the"
" object - [* *newt].&][s4;:T`& Head`(`): T`& [* Head]()&][s6; Returns reference t"
"o the head of BiArray. Same as operator`[`](0).&][s1; [*/ Return value]-|Referenc"
"e to the head of BiArray.&][s4;:T`& Tail`(`): T`& [* Tail]()&][s6; Returns refere"
"nce to the tail of BiArray. Same as operator`[`](GetCount() - 1).&][s1; [*/ Retur"
"n value]-|Reference to the tail of BiArray.&][s4;:const T`& Head`(`) const: const"
" T`& [* Head]() const&][s6; Returns reference to the head of BiArray. Same as ope"
"rator`[`](0).&][s1; [*/ Return value]-|Constant reference to the head of BiArray."
"&][s4;:const T`& Tail`(`) const: const T`& [* Tail]() const&][s6; Returns referen"
"ce to the tail of BiArray. Same as operator`[`](GetCount() - 1).&][s1; [*/ Return"
" value]-|Constant reference to the tail of BiArray.&][s4;:void DropHead`(`): void"
" [* DropHead]()&][s6; Removes element at the head of BiArray.&][s11; Invalidates "
"iterators to BiArray.&][s4;:void DropTail`(`): void [* DropTail]()&][s6; Removes "
"element at the tail of BiArray.&][s11; Invalidates iterators to BiArray.&][s4;:T "
"`*DetachHead`(`): T *[* DetachHead]()&][s6; Removes element at the head of BiArra"
"y, giving up ownership. Client is responsible for deletio")
TOPIC_TEXT(
"n of element.&][s11; Invalidates iterators to BiArray.&][s1; [*/ Return value]-|"
"Pointer to the element allocated on the heap.&][s4;:T `*DetachTail`(`): T *[* Det"
"achTail]()&][s6; Removes element at the tail of BiArray, giving up ownership. Cli"
"ent is responsible for deletion of element.&][s11; Invalidates iterators to BiArr"
"ay.&][s1; [*/ Return value]-|Pointer to the element allocated on the heap.&][s4;:"
"T`& operator`[`]`(int i`): T`& [* operator`[`]](int [*@3 i])&][s6; Returns a refe"
"rence to the element at specified position.&][s1; [*C@3 i]-|Position of element.&"
"][s1; [*/ Return value]-|Reference to the element.&][s4;:const T`& operator`[`]`("
"int i`) const: const T`& [* operator`[`]](int [*@3 i]) const&][s6; Returns a refe"
"rence to the element at specified position.&][s1; [*C@3 i]-|Position of element.&"
"][s1; [*/ Return value]-|Constant reference to the element.&][s4;:void Shrink`(`)"
": int [* GetCount]() const&][s6; Returns the number of elements in BiArray.&][s1;"
" [*/ Return value]-|Actual number of elements.&][s4;:bool IsEmpty`(`) const: bool"
" [* IsEmpty]() const&][s6; Tests whether BiArray is empty. Same as GetCount() == "
"0.&][s1; [*/ Return value]-|true if Vector is empty, false otherwise.&][s4;:void "
"Clear`(`): void [* Clear]()&][s6; Removes all elements from BiArray.&][s4; void ["
"* Shrink]()&][s6; Minimizes memory consumption of BiArray by minimizing capacity."
"&][s4;:void Reserve`(int n`): void [* Reserve](int [*@3 n])&][s6; Reserves capaci"
"ty. If required capacity is greater than the current capacity, capacity is increa"
"sed to the required value.&][s1; [*C@3 n]-|Required capacity.&][s4;:int GetAlloc`"
"(`) const: int [* GetAlloc]() const&][s6; Returns current capacity of BiArray.&]["
"s1; [*/ Return value]-|Capacity of BiArray.&][s4;:friend BiArray`& operator`<`<`("
"BiArray`& b`, const T`& x`): friend BiArray`& [* operator]<<(BiArray`& [*@3 b], c"
"onst T`& [*@3 x])&][s6; Operator replacement of [* void AddTail(const T`&x)]. By "
"returning reference to the BiArray allows adding more ele")
TOPIC_TEXT(
"ments in single expression, thus e.g. allowing to construct temporary BiArray as"
" part of expression like Foo(BiArray<int>() << 1 << 2 << 4).&][s11; Requires T to"
" have deep copy constructor.&][s11; Invalidates iterators to BiArray.&][s1; [*C@3"
" x]-|The value that is copied to the newly created element.&][s1; [*/ Return valu"
"e]-|Reference to BiArray (that is *this).&][s4;:friend BiArray`& operator`>`>`(co"
"nst T`& x`, BiArray`& b`): friend BiArray`& [* operator]>>(const T`& [*@3 x], BiA"
"rray`& [*@3 b])&][s6; Operator replacement of [* void AddHead(const T`&x)]. By re"
"turning reference to the BiArray allows adding more elements in single expression"
", thus e.g. allowing to construct temporary BiArray as part of expression like Fo"
"o(1 >> (2 >> BiArray<int>())).&][s11; Requires T to have deep copy constructor.&]"
"[s11; Invalidates iterators to BiArray.&][s1; [*C@3 x]-|The value that is copied "
"to the newly created element.&][s1; [*/ Return value]-|Reference to BiArray (that"
" is *this).&][s4;:void Serialize`(Stream`& s`): void [* Serialize](Stream`& [*@3 "
"s])&][s6; Serializes content of BiArray to/from Stream. Works only if NTL is used"
" as part of UPP.&][s11; Requires T to have serialization operator defined.&][s1; "
"[*C@3 s]-|Target/source stream.&][s4;:bool IsPicked`(`) const: bool [* IsPicked]("
") const&][s6; Returns [* true] if BiArray is in picked state.&][s1; [*/ Return va"
"lue]-|[* true] if BiArray is in picked state, [* false] otherwise.&][s4;:BiArray`"
"(const BiArray`& v`, int`): [* BiArray](const [* BiArray]`& [*@3 v], int)&][s6; O"
"ptional deep copy constructor.&][s11; Requires T to have deep copy constructor or"
" optional deep copy constructor if Array stores only objects of type T.&][s11; Re"
"quires polymorphic deep copy if Array stores also objects of type derived from T."
"&][s1; [*C@3 v]-|Source Array.&][s4;:BiArray`(pick`_ BiArray`& src`): [* BiArray]"
"(pick`_ [* BiArray]`& [*@3 src])&][s6; Pick constructor. Transfers source BiArray"
" in low constant time, but destroys it by picking. &][s1;")
TOPIC_TEXT(
" [*C@3 v]-|Source BiArray.&][s4;:void operator`=`(pick`_ BiArray`& src`): void ["
"* operator]=(pick`_ BiArray`& [*@3 src])&][s6; Pick operator. Transfers source Ar"
"ray in low constant time, but destroys it by picking.&][s1; [*C@3 v]-|Source Arra"
"y.&][s4; void [* operator<<=](const Array`& [*@3 v])&][s6; Optional deep copy ope"
"rator. Defined using DeepCopyOptionTemplate.&][s11; Requires T to have deep copy "
"constructor or optional deep copy if Array stores only objects of type T.&][s11; "
"Requires polymorphic deep copy if Array stores also objects of type derived from "
"T.&][s1; [*C@3 v]-|Source Array.&][s4;:BiArray`(`): [* BiArray]()&][s6; Default c"
"onstructor. Constructs empty BiArray.&][s4;:`~BiArray`(`): ~[* BiArray]()&][s6; D"
"estructor. Calls destructors of all elements in BiArray.&][s4;:typedef T ValueTyp"
"e:* [* typedef T ]ValueType&][s6; Typedef of T for use in templated algorithms.&]"
"[s4;:typedef T `*Iterator`;: typedef [/ type] [* Iterator];&][s6; Iterator type.&"
"][s4; typedef [/ type] [* ConstIterator];&][s6; Constant iterator type.&][s4;:typ"
"edef ConstIIterator`<BiArray`> ConstIterator: Iterator [* Begin]()&][s6; Returns "
"non-constant iterator to the head of BiArray. &][s1; [*/ Return value]-|Iterator."
"&][s4;:Iterator End`(`): Iterator [* End]()&][s6; Returns non-constant iterator t"
"o the position just beyond the tail of BiArray.&][s1; [*/ Return value]-|Iterator"
".&][s4;:Iterator GetIter`(int pos`): Iterator [* GetIter](int [*@3 pos])&][s6; Re"
"turns non-constant iterator to the element at specified position. Same as [* Begi"
"n() + pos]. Benefit of this methods is that in debug mode [* pos] is range checke"
"d.&][s1; [*C@3 pos]-|Required position.&][s1; [*/ Return value]-|Iterator.&][s4;:"
"ConstIterator Begin`(`) const: ConstIterator [* Begin]() const&][s6; Returns cons"
"tant iterator to the head of BiArray. &][s1; [*/ Return value]-|Iterator.&][s4;:C"
"onstIterator End`(`) const: ConstIterator [* End]() const&][s6; Returns constant "
"iterator to the position just beyond the tail of Array.&]")
TOPIC_TEXT(
"[s1; [*/ Return value]-|Iterator.-|&][s4;:ConstIterator GetIter`(int pos`) const"
": ConstIterator [* GetIter](int [*@3 pos]) const&][s6; Returns constant iterator "
"to the element at specified position. Same as [* Begin() + pos]. Benefit of this "
"methods is that in debug mode [* pos] is range checked.&][s1; [*C@3 pos]-|Require"
"d position.&][s1; [*/ Return value]-|Iterator.&][s4; friend void [* Swap](BiArray"
"`& [*@3 a], BiArray`& [*@3 b])&][s6; Specialization of generic [* Swap] for BiArr"
"ay. Swaps BiArray in low constant time operation.&][s1; [*C@3 a]-|First BiArray t"
"o swap.&][s1; [*C@3 b]-|Second BiArray to swap.&][s0;2 ")
