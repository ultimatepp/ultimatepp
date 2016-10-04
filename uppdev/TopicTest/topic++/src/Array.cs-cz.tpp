TITLE("templateclassTclassArray")
REF("void Add(const T& x)")
REF("void AddPick(pick_ T& x)")
REF("const T& operator[](int i) const")
REF("T& operator[](int i)")
REF("int GetCount() const")
REF("bool IsEmpty() const")
REF("void SetCount(int i)")
REF("void SetCount(int i, const T& init)")
REF("void SetCountR(int i)")
REF("void SetCountR(int i, const T& init)")
REF("void Clear()")
REF("T& DoIndex(int i)")
REF("T& DoIndex(int i, const T& x)")
REF("void Shrink()")
REF("void Reserve(int xtra)")
REF("int GetAlloc() const")
REF("void Set(int i, const T& x, int count = 1)")
REF("void Remove(int i, int count = 1)")
REF("void InsertN(int i, int count = 1)")
REF("T& Insert(int i)")
REF("void Insert(int i, const T& x, int count = 1)")
REF("void Insert(int i, const Array& x)")
REF("void Insert(int i, const Array& x, int offset, int count)")
REF("void Append(const Array& x)")
REF("void Append(const Array& x, int o, int c)")
REF("void InsertPick(int i, pick_ Array& x)")
REF("void AppendPick(pick_ Array& x)")
REF("int GetIndex(const T& item) const")
REF("void Swap(int i1, int i2)")
REF("void Move(int i1, int i2)")
REF("T& Add(T *newt)")
REF("T *Detach(int i)")
REF("T& Set(int i, T *newt)")
REF("void Insert(int i, T *newt)")
REF("void Drop(int n = 1)")
REF("T& Top()")
REF("const T& Top() const")
REF("T *PopDetach()")
REF("void Swap(Array& b)")
REF("Array& operator<<(const T& x)")
REF("Array& operator|(pick_ T& x)")
REF("bool IsPicked() const")
REF("void Serialize(Stream& s)")
REF("Array()")
REF("~Array()")
REF("Array(pick_ Array& v)")
REF("void operator=(pick_ Array& v)")
REF("Array(const Array& v, int)")
REF("typedef T ValueType")
REF("Iterator Begin()")
REF("Iterator End()")
REF("Iterator GetIter(int pos)")
REF("ConstIterator Begin() const")
REF("ConstIterator End() const")
REF("ConstIterator GetIter(int pos) const")
REF("friend int GetCount(ConstIterator a, ConstIterator b)")
REF("friend int GetCount(Iterator a, Iterator b)")
REF("friend void Swap(Array& a, Array& b)")
REF("static void IterSwap0(Iterator a, Iterator b)")
REF("friend void IterSwap(Iterator a, Iterator b)")
REF("PickTemplate(Array) MoveTemplate(Array)")
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
"5:package`-title][{_}%EN-US [s10;* [* template <class ][@4 T][* > class ]Array&]["
"s5; &][s1; [*C@4 T]-|&][s3;C [* T`&] Add()&][s6; Adds new default constructed ele"
"ment to Array. Requires T to have default constructor.&][s1; [*/ Return value]-|r"
"eference to newly added default constructed element&][s4;:void Add`(const T`& x`)"
": void [* Add](const T`& [*@3 x])&][s6; Adds new element with specified value to "
"Array. Requires T to have copy constructor.&][s1; [*C@4 x]-|&][s1; [*/ Return val"
"ue]-|&][s4;:void AddPick`(pick`_ T`& x`): void [* AddPick](pick`_ T`& [*@3 x])&]["
"s6; &][s1; [*C@4 x]-|&][s1; [*/ Return value]-|&][s4;:const T`& operator`[`]`(int"
" i`) const: const T`& [* operator`[`]](int [*@3 i]) const&][s6; &][s1; [*C@4 i]-|"
"&][s1; [*/ Return value]-|&][s4;:T`& operator`[`]`(int i`): T`& [* operator`[`]]("
"int [*@3 i])&][s6; &][s1; [*C@4 i]-|&][s1; [*/ Return val")
TOPIC_TEXT(
"ue]-|&][s4;:int GetCount`(`) const: int [* GetCount]() const&][s6; &][s1; -|&][s"
"1; [*/ Return value]-|&][s4;:bool IsEmpty`(`) const: bool [* IsEmpty]() const&][s"
"6; &][s1; -|&][s1; [*/ Return value]-|&][s4;:void SetCount`(int i`): void [* SetC"
"ount](int [*@3 i])&][s6; &][s1; [*C@4 i]-|&][s1; [*/ Return value]-|&][s4;:void S"
"etCount`(int i`, const T`& init`): void [* SetCount](int [*@3 i], const T`& [*@3 "
"init])&][s6; &][s1; [*C@4 i]-|&][s1; [*C@4 init]-|&][s1; [*/ Return value]-|&][s4"
";:void SetCountR`(int i`): void [* SetCountR](int [*@3 i])&][s6; &][s1; [*C@4 i]-"
"|&][s1; [*/ Return value]-|&][s4;:void SetCountR`(int i`, const T`& init`): void "
"[* SetCountR](int [*@3 i], const T`& [*@3 init])&][s6; &][s1; [*C@4 i]-|&][s1; [*"
"C@4 init]-|&][s1; [*/ Return value]-|&][s4;:void Clear`(`): void [* Clear]()&][s6"
"; &][s1; -|&][s1; [*/ Return value]-|&][s4;:T`& DoIndex`(int i`): T`& [* DoIndex]"
"(int [*@3 i])&][s6; &][s1; [*C@4 i]-|&][s1; [*/ Return value]-|&][s4;:T`& DoIndex"
"`(int i`, const T`& x`): T`& [* DoIndex](int [*@3 i], const T`& [*@3 x])&][s6; &]"
"[s1; [*C@4 i]-|&][s1; [*C@4 x]-|&][s1; [*/ Return value]-|&][s4;:void Shrink`(`):"
" void [* Shrink]()&][s6; &][s1; -|&][s1; [*/ Return value]-|&][s4;:void Reserve`("
"int xtra`): void [* Reserve](int [*@3 xtra])&][s6; &][s1; [*C@4 xtra]-|&][s1; [*/"
" Return value]-|&][s4;:int GetAlloc`(`) const: int [* GetAlloc]() const&][s6; &]["
"s1; -|&][s1; [*/ Return value]-|&][s4;:void Set`(int i`, const T`& x`, int count "
"`= 1`): void [* Set](int [*@3 i], const T`& [*@3 x], int [*@3 count] = 1)&][s6; &"
"][s1; [*C@4 i]-|&][s1; [*C@4 x]-|&][s1; [*C@4 count]-|&][s1; [*/ Return value]-|&"
"][s4;:void Remove`(int i`, int count `= 1`): void [* Remove](int [*@3 i], int [*@"
"3 count] = 1)&][s6; &][s1; [*C@4 i]-|&][s1; [*C@4 count]-|&][s1; [*/ Return value"
"]-|&][s4;:void InsertN`(int i`, int count `= 1`): void [* InsertN](int [*@3 i], i"
"nt [*@3 count] = 1)&][s6; &][s1; [*C@4 i]-|&][s1; [*C@4 count]-|&][s1; [*/ Return"
" value]-|&][s4;:T`& Insert`(int i`): T`& [* Insert](int [")
TOPIC_TEXT(
"*@3 i])&][s6; &][s1; [*C@4 i]-|&][s1; [*/ Return value]-|&][s4;:void Insert`(int"
" i`, const T`& x`, int count `= 1`): void [* Insert](int [*@3 i], const T`& [*@3 "
"x], int [*@3 count] = 1)&][s6; &][s1; [*C@4 i]-|&][s1; [*C@4 x]-|&][s1; [*C@4 cou"
"nt]-|&][s1; [*/ Return value]-|&][s4;:void Insert`(int i`, const Array`& x`): voi"
"d [* Insert](int [*@3 i], const Array`& [*@3 x])&][s6; &][s1; [*C@4 i]-|&][s1; [*"
"C@4 x]-|&][s1; [*/ Return value]-|&][s4;:void Insert`(int i`, const Array`& x`, i"
"nt offset`, int count`): void [* Insert](int [*@3 i], const Array`& [*@3 x], int "
"[*@3 offset], int [*@3 count])&][s6; &][s1; [*C@4 i]-|&][s1; [*C@4 x]-|&][s1; [*C"
"@4 offset]-|&][s1; [*C@4 count]-|&][s1; [*/ Return value]-|&][s4;:void Append`(co"
"nst Array`& x`): void [* Append](const Array`& [*@3 x])&][s6; &][s1; [*C@4 x]-|&]"
"[s1; [*/ Return value]-|&][s4;:void Append`(const Array`& x`, int o`, int c`): vo"
"id [* Append](const Array`& [*@3 x], int [*@3 o], int [*@3 c])&][s6; &][s1; [*C@4"
" x]-|&][s1; [*C@4 o]-|&][s1; [*C@4 c]-|&][s1; [*/ Return value]-|&][s4;:void Inse"
"rtPick`(int i`, pick`_ Array`& x`): void [* InsertPick](int [*@3 i], pick`_ Array"
"`& [*@3 x])&][s6; &][s1; [*C@4 i]-|&][s1; [*C@4 x]-|&][s1; [*/ Return value]-|&]["
"s4;:void AppendPick`(pick`_ Array`& x`): void [* AppendPick](pick`_ Array`& [*@3 "
"x])&][s6; &][s1; [*C@4 x]-|&][s1; [*/ Return value]-|&][s4;:int GetIndex`(const T"
"`& item`) const: int [* GetIndex](const T`& [*@3 item]) const&][s6; &][s1; [*C@4 "
"item]-|&][s1; [*/ Return value]-|&][s4;:void Swap`(int i1`, int i2`): void [* Swa"
"p](int [*@3 i1], int [*@3 i2])&][s6; &][s1; [*C@4 i1]-|&][s1; [*C@4 i2]-|&][s1; ["
"*/ Return value]-|&][s4;:void Move`(int i1`, int i2`): void [* Move](int [*@3 i1]"
", int [*@3 i2])&][s6; &][s1; [*C@4 i1]-|&][s1; [*C@4 i2]-|&][s1; [*/ Return value"
"]-|&][s4;:T`& Add`(T `*newt`): T`& [* Add](T *[*@3 newt])&][s6; &][s1; [*C@4 newt"
"]-|&][s1; [*/ Return value]-|&][s4;:T `*Detach`(int i`): T *[* Detach](int [*@3 i"
"])&][s6; &][s1; [*C@4 i]-|&][s1; [*/ Return value]-|&][s4")
TOPIC_TEXT(
";:T`& Set`(int i`, T `*newt`): T`& [* Set](int [*@3 i], T *[*@3 newt])&][s6; &]["
"s1; [*C@4 i]-|&][s1; [*C@4 newt]-|&][s1; [*/ Return value]-|&][s4;:void Insert`(i"
"nt i`, T `*newt`): void [* Insert](int [*@3 i], T *[*@3 newt])&][s6; &][s1; [*C@4"
" i]-|&][s1; [*C@4 newt]-|&][s1; [*/ Return value]-|&][s4;:void Drop`(int n `= 1`)"
": void [* Drop](int [*@3 n] = 1)&][s6; &][s1; [*C@4 n]-|&][s1; [*/ Return value]-"
"|&][s4;:T`& Top`(`): T`& [* Top]()&][s6; &][s1; -|&][s1; [*/ Return value]-|&][s4"
";:const T`& Top`(`) const: const T`& [* Top]() const&][s6; &][s1; -|&][s1; [*/ Re"
"turn value]-|&][s4;:T `*PopDetach`(`): T *[* PopDetach]()&][s6; &][s1; -|&][s1; ["
"*/ Return value]-|&][s4;:void Swap`(Array`& b`): void [* Swap](Array`& [*@3 b])&]"
"[s6; &][s1; [*C@4 b]-|&][s1; [*/ Return value]-|&][s4;:Array`& operator`<`<`(cons"
"t T`& x`): Array`& operator<<(const T`& [*@3 x])&][s6; &][s1; [*C@4 x]-|&][s1; [*"
"/ Return value]-|&][s4;:Array`& operator`|`(pick`_ T`& x`): Array`& operator|(pic"
"k`_ T`& [*@3 x])&][s6; &][s1; [*C@4 x]-|&][s1; [*/ Return value]-|&][s4;:bool IsP"
"icked`(`) const: bool [* IsPicked]() const&][s6; &][s1; -|&][s1; [*/ Return value"
"]-|&][s4;:void Serialize`(Stream`& s`): void [* Serialize](Stream`& [*@3 s])&][s6"
"; &][s1; [*C@4 s]-|&][s1; [*/ Return value]-|&][s4;:Array`(`): [* Array]()&][s6; "
"&][s1; -|&][s4;:`~Array`(`): ~[* Array]()&][s6; &][s4;:Array`(pick`_ Array`& v`):"
" [* Array](pick`_ [* Array]`& [*@3 v])&][s6; &][s1; [*C@4 v]-|&][s4;:void operato"
"r`=`(pick`_ Array`& v`): void operator=(pick`_ Array`& [*@3 v])&][s6; &][s1; [*C@"
"4 v]-|&][s1; [*/ Return value]-|&][s4;:Array`(const Array`& v`, int`): [* Array]("
"const [* Array]`& [*@3 v], int)&][s6; &][s1; [*C@4 v]-|&][s4;:typedef T ValueType"
":* [* typedef T ]ValueType&][s6; &][s4;:Iterator Begin`(`): Iterator [* Begin]()&"
"][s6; &][s1; -|&][s1; [*/ Return value]-|&][s4;:Iterator End`(`): Iterator [* End"
"]()&][s6; &][s1; -|&][s1; [*/ Return value]-|&][s4;:Iterator GetIter`(int pos`): "
"Iterator [* GetIter](int [*@3 pos])&][s6; &][s1; [*C@4 po")
TOPIC_TEXT(
"s]-|&][s1; [*/ Return value]-|&][s4;:ConstIterator Begin`(`) const: ConstIterato"
"r [* Begin]() const&][s6; &][s1; -|&][s1; [*/ Return value]-|&][s4;:ConstIterator"
" End`(`) const: ConstIterator [* End]() const&][s6; &][s1; -|&][s1; [*/ Return va"
"lue]-|&][s4;:ConstIterator GetIter`(int pos`) const: ConstIterator [* GetIter](in"
"t [*@3 pos]) const&][s6; &][s1; [*C@4 pos]-|&][s1; [*/ Return value]-|&][s4;:frie"
"nd int GetCount`(ConstIterator a`, ConstIterator b`): friend int [* GetCount](Con"
"stIterator [*@3 a], ConstIterator [*@3 b])&][s6; &][s1; [*C@3 a]-|&][s1; [*C@3 b]"
"-|&][s1; [*/ Return value]-|&][s4;:friend int GetCount`(Iterator a`, Iterator b`)"
": friend int [* GetCount](Iterator [*@3 a], Iterator [*@3 b])&][s6; &][s1; [*C@3 "
"a]-|&][s1; [*C@3 b]-|&][s1; [*/ Return value]-|&][s4;:friend void Swap`(Array`& a"
"`, Array`& b`): friend void [* Swap](Array`& [*@3 a], Array`& [*@3 b])&][s6; &][s"
"1; [*C@3 a]-|&][s1; [*C@3 b]-|&][s1; [*/ Return value]-|&][s4;:static void IterSw"
"ap0`(Iterator a`, Iterator b`): static void [* IterSwap0](Iterator [*@3 a], Itera"
"tor [*@3 b])&][s6; &][s1; [*C@4 a]-|&][s1; [*C@4 b]-|&][s1; [*/ Return value]-|&]"
"[s4;:friend void IterSwap`(Iterator a`, Iterator b`): friend void [* IterSwap](It"
"erator [*@3 a], Iterator [*@3 b])&][s6; &][s1; [*C@3 a]-|&][s1; [*C@3 b]-|&][s1; "
"[*/ Return value]-|&][s4;:PickTemplate`(Array`) MoveTemplate`(Array`): PickTempla"
"te(Array) [* MoveTemplate](Array)&][s6; &][s1; -|&][s1; [*/ Return value]-|&][s0;"
" ")
