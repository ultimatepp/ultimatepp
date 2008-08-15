TITLE("Vector class template")
REF("void AddPick(pick_ T& x)")
REF("void AddN(int n)")
REF("const T& operator[](int i) const")
REF("T& operator[](int i)")
REF("int GetCount() const")
REF("bool IsEmpty() const")
REF("void Trim(int n)")
REF("void SetCount(int n, const T& init)")
REF("void SetCountR(int n)")
REF("T& At(int i)")
REF("T& At(int i, const T& x)")
REF("void Shrink()")
REF("void Remove(const int *sorted_list, int n)")
REF("void Remove(const Vector<int>& sorted_list)")
REF("void InsertN(int i, int count = 1)")
REF("T& Insert(int i)")
REF("void Insert(int i, const T& x, int count = 1)")
REF("void Insert(int i, const Vector& x)")
REF("void Insert(int i, const Vector& x, int offset, int count)")
REF("void InsertPick(int i, pick_ Vector& x)")
REF("void Append(const Vector& x)")
REF("void Append(const Vector& x, int o, int c)")
REF("void AppendPick(pick_ Vector& x)")
REF("void Drop(int n = 1)")
REF("T& Top()")
REF("const T& Top() const")
REF("operator T*()")
REF("operator const T*() const")
REF("Vector& operator<<(const T& x)")
REF("Vector& operator|(pick_ T& x)")
REF("void Serialize(Stream& s)")
REF("Vector()")
REF("~Vector()")
REF("Vector(pick_ Vector& v)")
REF("void operator=(pick_ Vector& v)")
REF("bool IsPicked() const")
REF("Vector(const Vector& v, int)")
REF("void operator<<=(const Vector& v)")
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
"5:package`-title][{_}%EN-US [s10;~~~64; template <class [*@4 T]>&][s10;~~~64;* [*"
" class ]Vector&][s0; &][s12; [*C@4 T]-|Type of elements stored in Vector. T is re"
"quired to be [*/ moveable ]and must have either [*/ deep copy constructor], [*/ p"
"ick constructor] or [*/ default constructor].&][s5; The most effective flavor of "
"basic random access container. Its features are derived from fact that it is impl"
"emented as simple C-like vector of elements.&][s5; Disadvantage of Vector is comm"
"on requirement for elements to be stored in it - they must be [*/ moveable]. Anot"
"her disadvantage is fact that many operations invalidate references (that means C"
"`++ references and pointers) to elements.&][s5; Like any other NTL container, Vec"
"tor is [*/ moveable ]type with [*/ pick and optional deep copy] transfer semantic"
"s. Calling methods of picked Vector is logic error with e")
TOPIC_TEXT(
"xception of&][s0;C+75 void [* operator=](pick`_ Vector`& [*@3 v])&][s0;/+75 [/C "
"void ][*/C operator<<=][/C (const Vector`& ][*/C@3 v][/C )][/  ](defined using De"
"epCopyOptionTemplate)&][s0;C+75 void [* Clear]()&][s0;C+75 bool [* IsPicked]() co"
"nst&][s5; Optional deep copy is implemented through DeepCopyOptionTemplate macro."
"&][s3; Members&][s4; T`& [* Add]()&][s6; Adds new default constructed element to "
"Vector.&][s11; Requires T to have default constructor.&][s11; Invalidates iterato"
"rs and references to Vector.&][s1; [*/ Return value]-|Reference to newly added de"
"fault constructed element.&][s4;~~~.992; void [* Add](const T`& [*@3 x])&][s6; Ad"
"ds new element with specified value to Vector.&][s11; Requires T to have deep cop"
"y constructor.&][s11; Invalidates iterators and references to Vector.&][s1; [*C@3"
" x]-|The value that is copied to newly created element.&][s4;:void AddPick`(pick`"
"_ T`& x`): void [* AddPick](pick`_ T`& [*@3 x])&][s6; Adds new element to Vector "
"and picks value of parameter to it. &][s11; Requires T to have pick constructor.&"
"][s11; Invalidates iterators and references to Vector.&][s1; [*C@3 x]-|Source ins"
"tance of T that is to be picked.&][s4;:void AddN`(int n`): void [* AddN](int [*@3"
" n])&][s6; Adds specified number of default constructed elements to Vector.&][s11"
"; Requires T to have default constructor.&][s11; Invalidates iterators and refere"
"nces to Vector.&][s1; [*C@3 n]-|Number of elements to add&][s4;:const T`& operato"
"r`[`]`(int i`) const: const T`& [* operator`[`]](int [*@3 i]) const&][s6; Returns"
" a reference to the element at specified position.&][s1; [*C@3 i]-|Position of el"
"ement.&][s1; [*/ Return value]-|Constant reference to the element.&][s4;:T`& oper"
"ator`[`]`(int i`): T`& [* operator`[`]](int [*@3 i])&][s6; Returns a reference to"
" the element at specified position.&][s1; [*C@3 i]-|Position of element.&][s1; [*"
"/ Return value]-|Reference to the element.&][s4;:int GetCount`(`) const: int [* G"
"etCount]() const&][s6; Return the number of elements in V")
TOPIC_TEXT(
"ector.&][s1; [*/ Return value]-|Actual number of elements.&][s4;:bool IsEmpty`(`"
") const: bool [* IsEmpty]() const&][s6; Tests whether Vector  is empty. Same as G"
"etCount() == 0.&][s1; [*/ Return value]-|true if Vector is empty, false otherwise"
".&][s4;:void Trim`(int n`): void [* Trim](int [*@3 n])&][s6; Reduces number of el"
"ements in Vector to specified number. Required number must be less than or equal "
"to actual number of elements in Vector.&][s11; Invalidates iterators to Vector.&]"
"[s1; [*C@3 n]-|Required number of elements.&][s4;:const T`& operator`[`]`(int i`)"
" const: void [* SetCount](int [*@3 n])&][s6; Changes count of elements in Vector "
"to specified value. If required number of elements is greater than actual number,"
" newly added elements are default constructed. If Vector has to increase capacity"
", the new capacity will exactly match required number of elements (unlike [* SetC"
"ountR]).&][s11; Requires T to have default constructor.&][s11; Invalidates iterat"
"ors and references to Vector.&][s1; [*C@3 n]-|Required number of elements.&][s4;:"
"void SetCount`(int n`, const T`& init`): void [* SetCount](int [*@3 n], const T`&"
" [*@3 init])&][s6; Changes count of elements in Vector to specified value. If req"
"uired number of elements is greater than actual number, newly added elements are "
"initialized to specified value using copy constructor. If Vector has to increase "
"capacity, the new capacity will exactly match required number of elements (unlike"
" [* SetCountR]).&][s11; Requires T to have deep copy constructor.&][s11; Invalida"
"tes iterators and references to Vector.&][s1; [*C@3 n]-|Required number of elemen"
"ts.&][s1; [*C@3 init]-|Initialization value of newly added elements.&][s4;:void S"
"etCountR`(int n`): void [* SetCountR](int [*@3 n])&][s6; Changes count of element"
"s in Vector to specified value. If required number of elements is greater than ac"
"tual number, newly added elements are default constructed. If Vector has to incre"
"ase capacity, the new capacity will be greater than requi")
TOPIC_TEXT(
"red number of elements (unlike [* SetCount]) to allow adding other elements with"
"out further increasing capacity.&][s11; Requires T to have default constructor.&]"
"[s11; Invalidates iterators and references to Vector.&][s1; [*C@3 n]-|Required nu"
"mber of elements.&][s4;~~~.992; void [* SetCountR](int [*@3 n], const T`& [*@3 in"
"it])&][s6; Changes count of elements in Vector to specified value. If required nu"
"mber of elements is greater than actual number, newly added elements are initiali"
"zed to specified value using copy constructor. If Vector has to increase capacity"
", the new capacity will be greater than required number of elements (unlike [* Se"
"tCount]) to allow adding other elements without further increasing capacity.&][s1"
"1; Requires T to have deep copy constructor.&][s11; Invalidates iterators and ref"
"erences to Vector.&][s1; [*C@3 n]-|Required number of elements.&][s1; [*C@3 init]"
"-|Initialization value of newly added elements.&][s4;~~~.992; void [* Clear]()&]["
"s6; Removes all elements from Vector. Capacity is also cleared to zero.&][s11; In"
"validates iterators and references to Vector.&][s4;:T`& At`(int i`):~~~.992; T`& "
"[* At](int [*@3 i])&][s6; If specified position is lower than number of elements "
"in Vector ([* i < GetCount()]), returns reference to element at specified positio"
"n. Otherwise increases number of elements in Vector to [* i + 1]. Newly added ele"
"ments are default constructed. If Vector has to increase capacity, the new capaci"
"ty will be greater than required number of elements to allow adding other element"
"s without further increasing capacity.&][s11; Requires T to have default construc"
"tor.&][s11; Invalidates iterators and references to Vector.&][s1; [*C@3 i]-|Posit"
"ion of required element.&][s1; [*/ Return value]-|Reference to required element.&"
"][s4;:T`& At`(int i`, const T`& x`): T`& [* At](int [*@3 i], const T`& [*@3 x])&]"
"[s6; If specified position is lower than number of elements in Vector ([* i < Get"
"Count()]), returns reference to element at specified posi")
TOPIC_TEXT(
"tion. Otherwise increases number of elements in Vector to [* i + 1]. Newly added"
" elements are copy constructed from [* x]. If Vector has to increase capacity, th"
"e new capacity will be greater than required number of elements to allow adding o"
"ther elements without further increasing capacity.&][s11; Requires T to have deep"
" copy constructor.&][s11; Invalidates iterators and references to Vector.&][s1; ["
"*C@3 i]-|Position of required element.&][s1; [*C@3 x]-|Initialization value of ne"
"wly added elements.&][s1; [*/ Return value]-|Reference to required element.&][s4;"
":void Shrink`(`): void [* Shrink]()&][s6; Minimizes memory consumption of Vector "
"by decreasing capacity to number of elements.&][s4;~~~.992; void [* Reserve](int "
"[*@3 xtra])&][s6; Reserves capacity. If required capacity is greater than current"
" capacity, capacity is increased to the required value.&][s1; [*C@3 xtra]-|Requir"
"ed capacity.&][s4;~~~.992; int [* GetAlloc]() const&][s6; Returns current capacit"
"y of Vector.&][s1; [*/ Return value]-|Capacity of Vector.&][s4;~~~.992; void [* S"
"et](int [*@3 i], const T`& [*@3 x], int [*@3 count] = 1)&][s6; Sets requested num"
"ber of elements starting at the position [* i] to the specified value. If require"
"d number of elements exceeds existing elements of Vector then elements are added "
"to Vector.&][s11; Requires T to have deep copy constructor.&][s11; Invalidates it"
"erators and references to Vector.&][s1; [*C@3 i]-|Starting position.&][s1; [*C@3 "
"x]-|Value.&][s1; [*C@3 count]-|Number of elements.&][s4;~~~.992; void [* Remove]("
"int [*@3 i], int [*@3 count] = 1)&][s6; Removes requested number of elements.&][s"
"11; Invalidates iterators and references to Vector.&][s1; [*C@3 i]-|Position.&][s"
"1; [*C@3 count]-|Number of elements to remove.&][s4;:void Remove`(const int `*sor"
"ted`_list`, int n`): void [* Remove](const int *[*@3 sorted`_list], int [*@3 n])&"
"][s6; Removes number of elements from Vector. Time of operation almost does not d"
"epend on number of elements.&][s11; Invalidates iterators")
TOPIC_TEXT(
" and references to Vector.&][s1; [*C@3 sorted`_list]-|Pointer to array of positi"
"ons to remove. It must be sorted from lowest to greatest value.&][s1; [*C@3 n]-|N"
"umber of elements to remove.&][s4;:void Remove`(const Vector`<int`>`& sorted`_lis"
"t`): void [* Remove](const Vector<int>`& [*@3 sorted`_list])&][s6; Removes number"
" of elements form Vector. Same as [* Remove(sorted`_list, sorted`_list.GetCount()"
")].&][s11; Invalidates iterators and references to Vector.&][s1; [*C@3 sorted`_li"
"st]-|Sorted Vector of positions to remove.&][s4;:void InsertN`(int i`, int count "
"`= 1`):~~~.992; void [* InsertN](int [*@3 i], int [*@3 count] = 1)&][s6; Inserts "
"a specified number of default constructed elements at a specified position.&][s11"
"; Requires T to have default constructor.&][s11; Invalidates iterators and refere"
"nces to Vector.&][s1; [*C@3 i]-|Position.&][s1; [*C@3 count]-|Number of elements "
"to insert.&][s4;:T`& Insert`(int i`):~~~.992; T`& [* Insert](int [*@3 i])&][s6; I"
"nserts one default constructed element at the specified position.&][s1;*1 Require"
"s T to have default constructor.&][s11; Invalidates iterators and references to V"
"ector.&][s1; [*C@3 i]-|Position.&][s4;:void Insert`(int i`, const T`& x`, int cou"
"nt `= 1`): void [* Insert](int [*@3 i], const T`& [*@3 x], int [*@3 count] = 1)&]"
"[s6; Inserts a specified number of elements, setting them to a specified value.&]"
"[s11;~~~.992; Requires T to have deep copy constructor.&][s11; Invalidates iterat"
"ors and references to Vector.&][s1; [*C@3 i]-|Position.&][s1; [*C@3 x]-|Value of "
"inserted elements.&][s1; [*C@3 count]-|Number of elements to insert.&][s4;:void I"
"nsert`(int i`, const Vector`& x`):~~~.992; void [* Insert](int [*@3 i], const Vec"
"tor`& [*@3 x])&][s6; Inserts all elements from another Vector.&][s11;~~~.992; Req"
"uires T to have deep copy constructor.&][s11; Invalidates iterators and reference"
"s to Vector.&][s1; [*C@3 i]-|Position.&][s1; [*C@3 x]-|Source Vector.&][s4;:void "
"Insert`(int i`, const Vector`& x`, int offset`, int count")
TOPIC_TEXT(
"`):~~~.992; void [* Insert](int [*@3 i], const Vector`& [*@3 x], int [*@3 offset"
"], int [*@3 count])&][s6; Inserts a range of elements from another Vector.&][s11;"
"~~~.992; Requires T to have deep copy constructor.&][s11; Invalidates iterators a"
"nd references to Vector.&][s1; [*C@3 i]-|Insertion position.&][s1; [*C@3 x]-|Sour"
"ce Vector.&][s1; [*C@3 offset]-|Position of first element in source Vector to be "
"inserted.&][s1; [*C@3 count]-|Number of elements to insert.&][s4;:void InsertPick"
"`(int i`, pick`_ Vector`& x`):~~~.992; void [* InsertPick](int [*@3 i], pick`_ Ve"
"ctor`& [*@3 x])&][s6; Inserts source Vector at specified position using pick tran"
"sfer semantics. It is faster than deep copy insert, does not use deep copy constr"
"uctor for T, but destroys source Vector.&][s11; Invalidates iterators and referen"
"ces to Vector.&][s1; [*C@3 i]-|Insertion position.&][s1; [*C@3 x]-|Source Vector."
"&][s4;:void Append`(const Vector`& x`): void [* Append](const Vector`& [*@3 x])&]"
"[s6; Appends all elements of source Vector.&][s11;~~~.992; Requires T to have dee"
"p copy constructor.&][s11; Invalidates iterators and references to Vector.&][s1; "
"[*C@3 x]-|Source Vector.&][s4;:void Append`(const Vector`& x`, int o`, int c`): v"
"oid [* Append](const Vector`& [*@3 x], int [*@3 o], int [*@3 c])&][s6; Appends a "
"range of elements from source Vector.&][s11;~~~.992; Requires T to have deep copy"
" constructor.&][s11; Invalidates iterators and references to Vector.&][s1; [*C@3 "
"x]-|Source Vector.&][s1; [*C@3 o]-|Position of first element in source Vector to "
"be inserted.&][s1; [*C@3 c]-|Number of elements to insert.&][s4;:void AppendPick`"
"(pick`_ Vector`& x`):~~~.992; void [* AppendPick](pick`_ Vector`& [*@3 x])&][s6; "
"Appends source Vector using pick transfer semantics. It is faster than deep copy "
"insert, does not use deep copy constructor for T, but destroys source Vector by p"
"icking.&][s11; Invalidates iterators and references to Vector.&][s1; [*C@3 x]-|So"
"urce Vector.&][s4;:void Drop`(int n `= 1`): void [* Drop]")
TOPIC_TEXT(
"(int [*@3 n] = 1)&][s6; Drops specified number of last elements in the Vector (s"
"ame as Trim(GetCount() - n)).&][s1; [*C@3 n]-|Number of elements.&][s4;:T`& Top`("
"`): T`& [* Top]()&][s6; Returns reference to the last element in the Vector.&][s1"
"; [*/ Return value]-|Reference of last element in the Vector.&][s4;:const T`& Top"
"`(`) const: const T`& [* Top]() const&][s6; Returns constant reference to the las"
"t element in the Vector.&][s1; [*/ Return value]-|Reference of last element in th"
"e Vector.&][s4;~~~.992; T [* Pop]()&][s6; Drops last element of Vector and return"
"s its value.&][s11; Requires T to have deep copy constructor.&][s1; [*/ Return va"
"lue]-|Value of dropped element.&][s4;:operator T`*`(`): [* operator T*]()&][s6; R"
"eturns non-constant pointer to elements.&][s1; [*/ Return value]-|Pointer to elem"
"ents.&][s4;:operator const T`*`(`) const: [* operator const T*]() const&][s6; Ret"
"urns constant pointer to elements.&][s1; [*/ Return value]-|Pointer to elements.&"
"][s4;:Vector`& operator`<`<`(const T`& x`): Vector`& [* operator<<](const T`& [*@"
"3 x])&][s6; Operator replacement of [* void Add(const T`&x)]. By returning refere"
"nce to to Vector allows adding more elements in single expression, thus e.g. allo"
"wing to construct temporary Vector as part of expression like Foo((Vector<int>() "
"<< 1 << 2 << 4)).&][s11; Requires T to have deep copy constructor.&][s11; Invalid"
"ates iterators and references to Vector.&][s1; [*C@3 x]-|The value that is copied"
" to newly created element.&][s1; [*/ Return value]-|Reference to Vector (*this).&"
"][s4;:Vector`& operator`|`(pick`_ T`& x`):~~~.992; Vector`& [* operator|](pick`_ "
"T`& [*@3 x])&][s6; Operator replacement of [* void AddPick(pick`_ T`&x)]. By retu"
"rning reference to to Vector allows adding more elements in single expression, th"
"us e.g. allowing to construct temporary Vector as part of expression.&][s11; Requ"
"ires T to have pick constructor.&][s11; Invalidates iterators and references to V"
"ector.&][s1; [*C@3 x]-|Source instance of T that is to be")
TOPIC_TEXT(
" picked.&][s1; [*/ Return value]-|Reference to Vector (*this).&][s4;:void Serial"
"ize`(Stream`& s`): void [* Serialize](Stream`& [*@3 s])&][s6; Serializes content "
"of Vector to/from Stream. Works only if NTL is used as part of UPP.&][s11; Requir"
"es T to have serialization operator defined.&][s1; [*C@3 s]-|Target/source stream"
".&][s4;:Vector`(`): [* Vector]()&][s6; Default constructor. Constructs empty Vect"
"or.&][s4;:`~Vector`(`): ~[* Vector]()&][s6; Destructor. Calls destructors of all "
"elements in Vector.&][s4;:Vector`(pick`_ Vector`& v`): [* Vector](pick`_ Vector`&"
" [*@3 v])&][s6; Pick constructor. Transfers source Vector in low constant time, b"
"ut destroys it by picking.&][s1; [*C@3 v]-|Source Vector.&][s4;:void operator`=`("
"pick`_ Vector`& v`): void [* operator]=(pick`_ Vector`& [*@3 v])&][s6; Pick opera"
"tor. Transfers source Vector in low constant time, but destroys it by picking.&]["
"s1; [*C@3 v]-|Source Vector.&][s4;:bool IsPicked`(`) const: bool [* IsPicked]() c"
"onst&][s6; Returns [* true] if Vector is in picked state.&][s1; [*/ Return value]"
"-|[* true] if Vector is in picked state, [* false] otherwise.&][s4;:Vector`(const"
" Vector`& v`, int`): [* Vector](const [* Vector]`& [*@3 v], int)&][s6; Optional d"
"eep copy constructor.&][s11; Requires T to have deep copy constructor or optional"
" deep copy constructor.&][s1; [*C@3 v]-|Source Vector.&][s4;:void operator`<`<`=`"
"(const Vector`& v`): void [* operator<<=](const Vector`& [*@3 v])&][s6; Optional "
"deep copy operator. Defined using DeepCopyOptionTemplate.&][s11; Requires T to ha"
"ve deep copy operator or optional deep copy operator.&][s1; [*C@3 v]-|Source Vect"
"or.&][s4;:typedef T ValueType:~~~.992;* [* typedef T ]ValueType&][s6; Typedef of "
"T for use in templated algorithms.&][s4;:typedef T `*Iterator`;: typedef T *[* It"
"erator];&][s6; Iterator type. Iterator is guaranteed to be of T* type.&][s4; type"
"def T *[* ConstIterator];&][s6; Constant iterator type. Iterator is guaranteed to"
" be of const T* type.&][s4;:ConstIterator Begin`(`) const")
TOPIC_TEXT(
": ConstIterator [* Begin]() const&][s6; Returns constant iterator to the first e"
"lement in Vector.&][s1; [*/ Return value]-|Iterator.&][s4;:ConstIterator End`(`) "
"const: ConstIterator [* End]() const&][s6; Returns constant iterator to the posit"
"ion just beyond the last element in Vector.&][s1; [*/ Return value]-|Iterator.&]["
"s4;:ConstIterator GetIter`(int i`) const: ConstIterator [* GetIter](int [*@3 i]) "
"const&][s6; Returns constant iterator to the element at specified position. Same "
"as [* Begin() + i]. Benefit of this methods is that in debug mode [* pos] is rang"
"e checked.&][s1; [*C@3 i]-|Required position.&][s1; [*/ Return value]-|Iterator.&"
"][s4;:Iterator Begin`(`): Iterator [* Begin]()&][s6; Returns non-constant iterato"
"r to the first element in Vector.&][s1; [*/ Return value]-|Iterator.&][s4;:Iterat"
"or End`(`): Iterator [* End]()&][s6; Returns non-constant iterator to the positio"
"n just beyond the last element in Vector.&][s1; [*/ Return value]-|Iterator.&][s4"
";:Iterator GetIter`(int i`): Iterator [* GetIter](int [*@3 i])&][s6; Returns non-"
"constant iterator to the element at specified position. Same as [* Begin() + pos]"
". Benefit of this methods is that in debug mode [* pos] is range checked.&][s1; ["
"*C@3 i]-|Required position.&][s1; [*/ Return value]-|Iterator.&][s4;:friend void "
"Swap`(Vector`& a`, Vector`& b`): friend void [* Swap](Vector`& [*@3 a], Vector`& "
"[*@3 b])&][s6; Specialization of generic [* Swap] for Vector. Swaps Vector in sim"
"ple constant time operation.&][s1; [*C@3 a]-|First Vector to swap.&][s1; [*C@3 b]"
"-|Second Vector to swap.")
