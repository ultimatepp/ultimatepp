TITLE("tArrayIndex class template")
REF("void Add(const T& x, unsigned _hash)")
REF("void Add(const T& x)")
REF("void Set(int i, const T& x, unsigned _hash)")
REF("void Set(int i, const T& x)")
REF("void Add(T *newt, unsigned _hash)")
REF("void Add(T *newt)")
REF("void Set(int i, T *newt, unsigned _hash)")
REF("void Set(int i, T *newt)")
REF("ArrayIndex()")
REF("ArrayIndex(pick_ ArrayIndex& s)")
REF("ArrayIndex(const ArrayIndex& s, int)")
REF("ArrayIndex(pick_ Array<T>& s)")
REF("ArrayIndex(const Array<T>& s, int)")
REF("ArrayIndex& operator=(pick_ Array<T>& x)")
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
"5:package`-title][{_}%EN-US [s10;~~~64; template <class [*@4 T], class [*@4 HashF"
"n] = StdHash<[*@4 T]> >&][s10;~~~64;* [* class ]ArrayIndex&][s0; &][s12;i480;~~~."
"480; [*C@4 T]-|Type or base class of elements stored in Array. There is no common"
" requirement for T.&][s12;i480;~~~.480; [*C@4 HashFn]-|Hashing class. Must have d"
"efined [*C unsigned operator()(const T`& x)] method returning hash value for elem"
"ents. Defaults to[*  ][*C StdHash<T>] which requires [*C unsigned GetHashValue(co"
"nst T`&)][C  ]function returning hash value of elements to be defined.&][s3; Base"
" class&][s9;^dpp`:`/`/`:`:`/AIndex`<class T`, class V`, class HashFn`>`/template "
"`<class T`, class V`, class HashFn`> class AIndex^ AIndex<T, Array<T>, HashFn>&]["
"s0; &][s5; Array flavor of index. Inherits most of its functionality from [^dpp`:"
"`/`/`:`:`/AIndex`<class T`, class V`, class HashFn`>`/tem")
TOPIC_TEXT(
"plate `<class T`, class V`, class HashFn`> class AIndex^ AIndex] and adds only m"
"embers specific for its flavor.&][s5; Like any other NTL container, ArrayIndex is"
" [*/^dpp`:`/`/SourceDoc`/Containers`/Moveable^ moveable][*/  ]type with [*/^dpp`:"
"`/`/SourceDoc`/Containers`/pick`_^ pick and optional deep copy] transfer semantic"
"s. Calling methods of picked Index is logic error with exception of&][s0;/+75 [/C"
" void ][*/C operator=][/C (pick`_ ArrayIndex`& ][*/C@3 v][/C ) ](defined by compo"
"sition)&][s0;/+75 [/C void ][*/C operator<<=][/C (const AIndex`& ][*/C@3 v][/C )]"
"[/  ](defined in AIndex)&][s0;C+75 void [* Clear]()&][s0;C+75 bool [* IsPicked]()"
" const&][s5; Optional deep copy operator is inherited from AIndex class. Pick ope"
"rator is implicitly defined by composition.&][s4;:void Add`(const T`& x`, unsigne"
"d `_hash`): void [* Add](const T`& [*@3 x], unsigned [*@3 `_hash])&][s6; Adds a n"
"ew element with precomputed hash value. Precomputed hash value must be same as ha"
"sh value that would be result of HashFn. Benefit of using precomputed hash value "
"is that sometimes you can compute hash-value as the part of other process, like f"
"etching string from input stream. This method has to be reimplemented in ArrayInd"
"ex (using simple forwarding) due to overloading of [* Add] in other forms.&][s11;"
" Requires T to have deep copy constructor.&][s11; Invalidates iterators to ArrayI"
"ndex.&][s1; [*C@3 x]-|Element to add.&][s1; [*C@3 `_hash]-|Precomputed hash value"
".&][s4;:void Add`(const T`& x`): void [* Add](const T`& [*@3 x])&][s6; Adds a new"
" element to Index. This method has to be reimplemented in ArrayIndex (using simpl"
"e forwarding) due to overloading of [* Add] in other forms.&][s11; Requires T to "
"have deep copy constructor.&][s11; Invalidates iterators to ArrayIndex.&][s1; [*C"
"@3 x]-|Element to add.&][s4;:void Set`(int i`, const T`& x`, unsigned `_hash`): v"
"oid [* Set](int [*@3 i], const T`& [*@3 x], unsigned [*@3 `_hash])&][s6; Replaces"
" element at specified position with new element with spec")
TOPIC_TEXT(
"ified value, using precomputed hash-value. Speed of operation depends on total n"
"umber of elements with the same as specified value in ArrayIndex. This method has"
" to be reimplemented in ArrayIndex (using simple redirection) due to overloading "
"of [* Set] in other forms.&][s11; Requires T to have deep copy constructor.&][s11"
"; Invalidates iterators to ArrayIndex.&][s1; [*C@3 i]-|Position of element.&][s1;"
" [*C@3 x]-|Value to set.&][s1; [*C@3 `_hash]-|Precomputed hash value.&][s4;:void "
"Set`(int i`, const T`& x`): void [* Set](int [*@3 i], const T`& [*@3 x])&][s6; Re"
"places element at specified position with new element with specified value. Speed"
" of operation depends on total number of elements with the same as specified valu"
"e in ArrayIndex. This method has to be reimplemented in ArrayIndex (using simple "
"redirection) due to overloading of [* Set] in other forms.&][s11; Requires T to h"
"ave deep copy constructor.&][s11; Invalidates iterators to ArrayIndex.&][s1; [*C@"
"3 i]-|Position of element.&][s1; [*C@3 x]-|Value to set.&][s4;:void Add`(T `*newt"
"`, unsigned `_hash`): void [* Add](T *[*@3 newt], unsigned [*@3 `_hash])&][s6; Ad"
"ds a new element created on heap to ArrayIndex using precomputed hash value. Elem"
"ent is specified by pointer to object. ArrayIndex takes over ownership of this th"
"is object. This variant allows use of ArrayIndex as polymorphic container, becaus"
"e type of added element can be also derived from T as well. No constructor is app"
"lied. &][s1; [*C@3 newt]-|Element created on heap.&][s4;:void Add`(T `*newt`): vo"
"id [* Add](T *[*@3 newt])&][s6; Adds a new element to ArrayIndex. Element is spec"
"ified by pointer to object. ArrayIndex takes over ownership of this this object. "
"This variant allows use of ArrayIndex as polymorphic container, because type of a"
"dded element can be also derived from T as well. No constructor is applied. &][s1"
"; [*C@3 newt]-|Object to be added.&][s1; [*/ Return value]-|&][s4;:void Set`(int "
"i`, T `*newt`, unsigned `_hash`): void [* Set](int [*@3 i")
TOPIC_TEXT(
"], T *[*@3 newt], unsigned [*@3 `_hash])&][s6; &][s1; [*C@3 i]-|Position.&][s1; "
"[*C@3 newt]-|New element created on heap.&][s1; [*C@3 `_hash]-|&][s1; [*/ Return "
"value]-|&][s4;:void Set`(int i`, T `*newt`): void [* Set](int [*@3 i], T *[*@3 ne"
"wt])&][s6; Replaces element at the specified position by element created on heap."
" ArrayIndex takes over ownership of element.&][s0; &][s1; [*C@3 i]-|Position.&][s"
"1; [*C@3 newt]-|New element created on heap.&][s1; [*/ Return value]-|&][s4;:Arra"
"yIndex`(`): [* ArrayIndex]()&][s6; Constructor. Constructs empty ArrayIndex.&][s4"
";:ArrayIndex`(pick`_ ArrayIndex`& s`): [* ArrayIndex](pick`_ [* ArrayIndex]`& [*@"
"3 s])&][s6; Pick constructor. Transfers source Index in low constant time, but de"
"stroys it by picking.&][s1; [*C@3 s]-|Source ArrayIndex.&][s4;:ArrayIndex`(const "
"ArrayIndex`& s`, int`): [* ArrayIndex](const [* ArrayIndex]`& [*@3 s], int)&][s6;"
" Optional deep copy constructor.&][s11; Requires T to have deep copy constructor "
"or optional deep copy constructor.&][s1; [*C@3 s]-|Source ArrayIndex.&][s4;:Array"
"Index`(pick`_ Array`<T`>`& s`): [* ArrayIndex](pick`_ [* Array]<T>`& [*@3 s])&][s"
"6; Pick-constructs ArrayIndex from Array. Transfers source container in low const"
"ant time, but destroys it by picking.&][s1; [*C@3 s]-|Source Array.&][s4;:ArrayIn"
"dex`(const Array`<T`>`& s`, int`): [* ArrayIndex](const [* Array]<T>`& [*@3 s], i"
"nt)&][s6; Deep-copy constructs ArrayIndex from Array.&][s11; Requires T to have d"
"eep copy constructor or optional deep copy constructor.&][s1; [*C@3 s]-|Source Ve"
"ctor.&][s4;:ArrayIndex`& operator`=`(pick`_ Array`<T`>`& x`): ArrayIndex`& [* ope"
"rator]=(pick`_ Array<T>`& [*@3 x])&][s6; Pick operator. Transfers source Array to"
" ArrayIndex, but destroys it by picking.&][s1; [*C@3 x]-|Source Array.")
