REF("T& Add()")
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
REF("void Insert(int i, const Vector& x)")
REF("void Insert(int i, const Vector& x, int offset, int count)")
REF("void InsertPick(int i, pick_ Vector& x)")
REF("void Append(const Vector& x)")
REF("void Append(const Vector& x, int o, int c)")
REF("void AppendPick(pick_ Vector& x)")
REF("int GetIndex(const T& item) const")
REF("void Drop(int n = 1)")
REF("T& Top()")
REF("const T& Top() const")
REF("T Pop()")
REF("operator T*()")
REF("operator const T*() const")
REF("Vector& operator<<(const T& x)")
REF("Vector& operator|(pick_ T& x)")
REF("void Serialize(Stream& s)")
REF("void operator=(pick_ Vector& v)")
REF("ConstIterator Begin() const")
REF("ConstIterator End() const")
REF("ConstIterator GetIter(int i) const")
REF("Iterator Begin()")
REF("Iterator End()")
REF("Iterator GetIter(int i)")
STYLESHEET("import$B0FD6119441B5FE2A0C4339DF4C2EDAA")
TITLE("templateclassTclassVector")
TOPIC_TEXT(
"[ $$0,0#00000000000000000000000000000000:Default]\r\n[l288;i704;a17;O9;~~~.992; "
"$$1,0#10431211400427159095818037425705:param]\r\n[a83;*R6 $$2,5#31310162474203024"
"125188417583966:caption]\r\n[b83;* $$3,5#07864147445237544204411237157677:title]\r"
"\n[b167;a42;C $$4,6#40027414424643823182269349404212:item]\r\n[b42;a42; $$5,5#454"
"13000475342174754091244180557:text]\r\n[l288;a17; $$6,6#2752174848137824262002072"
"5143825:desc]\r\n[l321;t246;C@5;1 $$7,7#20902679421464641399138805415013:code]\r\n"
"[b2503; $$8,0#65142375456100023862071332075487:separator]\r\n[*@(0.0.255) $$9,0#8"
"3433469410354161042741608181528:base]\r\n[t4167;C+117 $$10,0#37138531426314131251"
"341829483380:class]\r\n[l288;a17;*1 $$11,11#70004532496200323422659154056402:requ"
"irement]\r\n[i416;b42;a42;O9;~~~.416; $$12,12#10566046415157235020018451313112:tp"
"aram]\r\n[b167;C $$13,13#92430459443460461911108080531343:item1]\r\n[a42;C $$14,1"
"4#77422149456609303542238260500223:item2]\r\n[*@2$(0.128.128) $$15,15#34511555403"
"152284025741354420178:NewsDate]\r\n[l321;*C$7 $$16,16#034515894331459153449293352"
"95360:result]\r\n[l321;b83;a83;*C$7 $$17,17#07531550463529505371228428965313:resu"
"lt`-line]\r\n[l160;t4167;*C+117 $$18,5#88603949442205825958800053222425:package`-"
"title]\r\n[{_}%EN-US \r\n[s4;* [* template <class T> class ]Vector&]\r\n[s5; &]\r"
"\n[s3; Template parameters&]\r\n[s7; T&]\r\n[s6; Type of elements stored in Vecto"
"r. T has to be moveable. Some \r\nmethods may add other requirements for T.&]\r\n"
"[s3; Base classes&]\r\n[s9; RawVector<sizeof(T)> &]\r\n[s6; &]\r\n[s8; &]\r\n[s4;"
":T`& Add`(`): T`& [* Add]()&]\r\n[s5; &]\r\n[s3; Parameters&]\r\n[s1; &]\r\n[s6; "
"&]\r\n[s3; Return value&]\r\n[s6; &]\r\n[s3; Detailed description&]\r\n[s5; &]\r\n"
"[s8; &]\r\n[s4;:void Add`(const T`& x`): void [* Add](const T`& [*@3 x])&]\r\n[s5"
"; &]\r\n[s3; Parameters&]\r\n[s1; x&]\r\n[s6; &]\r\n[s3; Return value&]\r\n[s6; &"
"]\r\n[s3; Detailed description&]\r\n[s5; &]\r\n[s8; &]\r\n[s4;:void AddPick`(pick"
"`_ T`& x`): void [* AddPick](pick`_ T`& [*@3 x])&]\r\n[s5; &]\r\n[s3; Parameters&"
"]\r\n[s1; x&]\r\n[s6; &]\r\n[s3; Return value&]\r\n[s6; &]\r\n[s3; Detailed descr"
"iption")
TOPIC_TEXT(
"&]\r\n[s5; &]\r\n[s8; &]\r\n[s4;:const T`& operator`[`]`(int i`) const: const T`"
"& operator`[`](int \r\n[*@3 i]) const&]\r\n[s5; &]\r\n[s3; Parameters&]\r\n[s1; i"
"&]\r\n[s6; &]\r\n[s3; Return value&]\r\n[s6; &]\r\n[s3; Detailed description&]\r\n"
"[s5; &]\r\n[s8; &]\r\n[s4;:T`& operator`[`]`(int i`): T`& [* operator`[`]](int [*"
"@3 i])&]\r\n[s5; &]\r\n[s3; Parameters&]\r\n[s1; i&]\r\n[s6; &]\r\n[s3; Return va"
"lue&]\r\n[s6; &]\r\n[s3; Detailed description&]\r\n[s5; &]\r\n[s8; &]\r\n[s4;:int"
" GetCount`(`) const: int [* GetCount]() const&]\r\n[s5; &]\r\n[s3; Parameters&]\r"
"\n[s1; &]\r\n[s6; &]\r\n[s3; Return value&]\r\n[s6; &]\r\n[s3; Detailed descripti"
"on&]\r\n[s5; &]\r\n[s8; &]\r\n[s4;:bool IsEmpty`(`) const: bool [* IsEmpty]() con"
"st&]\r\n[s5; &]\r\n[s3; Parameters&]\r\n[s1; &]\r\n[s6; &]\r\n[s3; Return value&]"
"\r\n[s6; &]\r\n[s3; Detailed description&]\r\n[s5; &]\r\n[s8; &]\r\n[s4;:void Set"
"Count`(int i`): void [* SetCount](int [*@3 i])&]\r\n[s5; &]\r\n[s3; Parameters&]\r"
"\n[s1; i&]\r\n[s6; &]\r\n[s3; Return value&]\r\n[s6; &]\r\n[s3; Detailed descript"
"ion&]\r\n[s5; &]\r\n[s8; &]\r\n[s4;:void SetCount`(int i`, const T`& init`): void"
" [* SetCount](int \r\n[*@3 i], const T`& [*@3 init])&]\r\n[s5; &]\r\n[s3; Paramet"
"ers&]\r\n[s1; i&]\r\n[s6; &]\r\n[s1; init&]\r\n[s6; &]\r\n[s3; Return value&]\r\n"
"[s6; &]\r\n[s3; Detailed description&]\r\n[s5; &]\r\n[s8; &]\r\n[s4;:void SetCoun"
"tR`(int i`): void [* SetCountR](int [*@3 i])&]\r\n[s5; &]\r\n[s3; Parameters&]\r\n"
"[s1; i&]\r\n[s6; &]\r\n[s3; Return value&]\r\n[s6; &]\r\n[s3; Detailed descriptio"
"n&]\r\n[s5; &]\r\n[s8; &]\r\n[s4;:void SetCountR`(int i`, const T`& init`): void "
"[* SetCountR](int \r\n[*@3 i], const T`& [*@3 init])&]\r\n[s5; &]\r\n[s3; Paramet"
"ers&]\r\n[s1; i&]\r\n[s6; &]\r\n[s1; init&]\r\n[s6; &]\r\n[s3; Return value&]\r\n"
"[s6; &]\r\n[s3; Detailed description&]\r\n[s5; &]\r\n[s8; &]\r\n[s4;:void Clear`("
"`): void [* Clear]()&]\r\n[s5; &]\r\n[s3; Parameters&]\r\n[s1; &]\r\n[s6; &]\r\n["
"s3; Return value&]\r\n[s6; &]\r\n[s3; Detailed description&]\r\n[s5; &]\r\n[s8; &"
"]\r\n[s4;:T`& DoIndex`(int i`): T`& [* DoIndex](int [*@3 i])&]\r\n[s5; &]\r\n[s3;"
" Parameters&]\r\n[s1; i&]\r\n[s6; &]\r\n[s3; Return value&]\r\n[s6; &]\r\n[s3; De"
"tailed description&]\r\n[s5")
TOPIC_TEXT(
"; &]\r\n[s8; &]\r\n[s4;:T`& DoIndex`(int i`, const T`& x`): T`& [* DoIndex](int "
"[*@3 i], \r\nconst T`& [*@3 x])&]\r\n[s5; &]\r\n[s3; Parameters&]\r\n[s1; i&]\r\n"
"[s6; &]\r\n[s1; x&]\r\n[s6; &]\r\n[s3; Return value&]\r\n[s6; &]\r\n[s3; Detailed"
" description&]\r\n[s5; &]\r\n[s8; &]\r\n[s4;:void Shrink`(`): void [* Shrink]()&]"
"\r\n[s5; &]\r\n[s3; Parameters&]\r\n[s1; &]\r\n[s6; &]\r\n[s3; Return value&]\r\n"
"[s6; &]\r\n[s3; Detailed description&]\r\n[s5; &]\r\n[s8; &]\r\n[s4;:void Reserve"
"`(int xtra`): void [* Reserve](int [*@3 xtra])&]\r\n[s5; &]\r\n[s3; Parameters&]\r"
"\n[s1; xtra&]\r\n[s6; &]\r\n[s3; Return value&]\r\n[s6; &]\r\n[s3; Detailed descr"
"iption&]\r\n[s5; &]\r\n[s8; &]\r\n[s4;:int GetAlloc`(`) const: int [* GetAlloc]()"
" const&]\r\n[s5; &]\r\n[s3; Parameters&]\r\n[s1; &]\r\n[s6; &]\r\n[s3; Return val"
"ue&]\r\n[s6; &]\r\n[s3; Detailed description&]\r\n[s5; &]\r\n[s8; &]\r\n[s4;:void"
" Set`(int i`, const T`& x`, int count `= 1`): void [* Set](int \r\n[*@3 i], const"
" T`& [*@3 x], int [*@3 count] = 1)&]\r\n[s5; &]\r\n[s3; Parameters&]\r\n[s1; i&]\r"
"\n[s6; &]\r\n[s1; x&]\r\n[s6; &]\r\n[s1; count&]\r\n[s6; &]\r\n[s3; Return value&"
"]\r\n[s6; &]\r\n[s3; Detailed description&]\r\n[s5; &]\r\n[s8; &]\r\n[s4;:void Re"
"move`(int i`, int count `= 1`): void [* Remove](int [*@3 i], \r\nint [*@3 count] "
"= 1)&]\r\n[s5; &]\r\n[s3; Parameters&]\r\n[s1; i&]\r\n[s6; &]\r\n[s1; count&]\r\n"
"[s6; &]\r\n[s3; Return value&]\r\n[s6; &]\r\n[s3; Detailed description&]\r\n[s5; "
"&]\r\n[s8; &]\r\n[s4;:void InsertN`(int i`, int count `= 1`): void [* InsertN](in"
"t [*@3 i], \r\nint [*@3 count] = 1)&]\r\n[s5; &]\r\n[s3; Parameters&]\r\n[s1; i&]"
"\r\n[s6; &]\r\n[s1; count&]\r\n[s6; &]\r\n[s3; Return value&]\r\n[s6; &]\r\n[s3; "
"Detailed description&]\r\n[s5; &]\r\n[s8; &]\r\n[s4;:T`& Insert`(int i`): T`& [* "
"Insert](int [*@3 i])&]\r\n[s5; &]\r\n[s3; Parameters&]\r\n[s1; i&]\r\n[s6; &]\r\n"
"[s3; Return value&]\r\n[s6; &]\r\n[s3; Detailed description&]\r\n[s5; &]\r\n[s8; "
"&]\r\n[s4;:void Insert`(int i`, const T`& x`, int count `= 1`): void [* Insert](i"
"nt \r\n[*@3 i], const T`& [*@3 x], int [*@3 count] = 1)&]\r\n[s5; &]\r\n[s3; Para"
"meters&]\r\n[s1; i&]\r\n[s6; &]\r\n[s1; x&]\r\n[s6; &]\r\n[s1; count&]\r\n[s6; &]"
"\r\n[s3; Return value&]\r")
TOPIC_TEXT(
"\n[s6; &]\r\n[s3; Detailed description&]\r\n[s5; &]\r\n[s8; &]\r\n[s4;:void Inse"
"rt`(int i`, const Vector`& x`): void [* Insert](int [*@3 i], \r\nconst Vector`& ["
"*@3 x])&]\r\n[s5; &]\r\n[s3; Parameters&]\r\n[s1; i&]\r\n[s6; &]\r\n[s1; x&]\r\n["
"s6; &]\r\n[s3; Return value&]\r\n[s6; &]\r\n[s3; Detailed description&]\r\n[s5; &"
"]\r\n[s8; &]\r\n[s4;:void Insert`(int i`, const Vector`& x`, int offset`, int cou"
"nt`): void \r\n[* Insert](int [*@3 i], const Vector`& [*@3 x], int [*@3 offset], "
"int \r\n[*@3 count])&]\r\n[s5; &]\r\n[s3; Parameters&]\r\n[s1; i&]\r\n[s6; &]\r\n"
"[s1; x&]\r\n[s6; &]\r\n[s1; offset&]\r\n[s6; &]\r\n[s1; count&]\r\n[s6; &]\r\n[s3"
"; Return value&]\r\n[s6; &]\r\n[s3; Detailed description&]\r\n[s5; &]\r\n[s8; &]\r"
"\n[s4;:void InsertPick`(int i`, pick`_ Vector`& x`): void [* InsertPick](int \r\n"
"[*@3 i], pick`_ Vector`& [*@3 x])&]\r\n[s5; &]\r\n[s3; Parameters&]\r\n[s1; i&]\r"
"\n[s6; &]\r\n[s1; x&]\r\n[s6; &]\r\n[s3; Return value&]\r\n[s6; &]\r\n[s3; Detail"
"ed description&]\r\n[s5; &]\r\n[s8; &]\r\n[s4;:void Append`(const Vector`& x`): v"
"oid [* Append](const Vector`& \r\n[*@3 x])&]\r\n[s5; &]\r\n[s3; Parameters&]\r\n["
"s1; x&]\r\n[s6; &]\r\n[s3; Return value&]\r\n[s6; &]\r\n[s3; Detailed description"
"&]\r\n[s5; &]\r\n[s8; &]\r\n[s4;:void Append`(const Vector`& x`, int o`, int c`):"
" void [* Append](const \r\nVector`& [*@3 x], int [*@3 o], int [*@3 c])&]\r\n[s5; "
"&]\r\n[s3; Parameters&]\r\n[s1; x&]\r\n[s6; &]\r\n[s1; o&]\r\n[s6; &]\r\n[s1; c&]"
"\r\n[s6; &]\r\n[s3; Return value&]\r\n[s6; &]\r\n[s3; Detailed description&]\r\n["
"s5; &]\r\n[s8; &]\r\n[s4;:void AppendPick`(pick`_ Vector`& x`): void [* AppendPic"
"k](pick`_ \r\nVector`& [*@3 x])&]\r\n[s5; &]\r\n[s3; Parameters&]\r\n[s1; x&]\r\n"
"[s6; &]\r\n[s3; Return value&]\r\n[s6; &]\r\n[s3; Detailed description&]\r\n[s5; "
"&]\r\n[s8; &]\r\n[s4;:int GetIndex`(const T`& item`) const: int [* GetIndex](cons"
"t T`& \r\n[*@3 item]) const&]\r\n[s5; &]\r\n[s3; Parameters&]\r\n[s1; item&]\r\n["
"s6; &]\r\n[s3; Return value&]\r\n[s6; &]\r\n[s3; Detailed description&]\r\n[s5; &"
"]\r\n[s8; &]\r\n[s4;:void Drop`(int n `= 1`): void [* Drop](int [*@3 n] = 1)&]\r\n"
"[s5; &]\r\n[s3; Parameters&]\r\n[s1; n&]\r\n[s6; &]\r\n[s3; Return value&]\r\n[s6"
"; &]\r\n[s3; Detail")
TOPIC_TEXT(
"ed description&]\r\n[s5; &]\r\n[s8; &]\r\n[s4;:T`& Top`(`): T`& [* Top]()&]\r\n["
"s5; &]\r\n[s3; Parameters&]\r\n[s1; &]\r\n[s6; &]\r\n[s3; Return value&]\r\n[s6; "
"&]\r\n[s3; Detailed description&]\r\n[s5; &]\r\n[s8; &]\r\n[s4;:const T`& Top`(`)"
" const: const T`& [* Top]() const&]\r\n[s5; &]\r\n[s3; Parameters&]\r\n[s1; &]\r\n"
"[s6; &]\r\n[s3; Return value&]\r\n[s6; &]\r\n[s3; Detailed description&]\r\n[s5; "
"&]\r\n[s8; &]\r\n[s4;:T Pop`(`): T [* Pop]()&]\r\n[s5; &]\r\n[s3; Parameters&]\r\n"
"[s1; &]\r\n[s6; &]\r\n[s3; Return value&]\r\n[s6; &]\r\n[s3; Detailed description"
"&]\r\n[s5; &]\r\n[s8; &]\r\n[s4;:operator T`*`(`): operator T*()&]\r\n[s5; &]\r\n"
"[s3; Parameters&]\r\n[s1; &]\r\n[s6; &]\r\n[s3; Return value&]\r\n[s6; &]\r\n[s3;"
" Detailed description&]\r\n[s5; &]\r\n[s8; &]\r\n[s4;:operator const T`*`(`) cons"
"t: operator const T*() const&]\r\n[s5; &]\r\n[s3; Parameters&]\r\n[s1; &]\r\n[s6;"
" &]\r\n[s3; Return value&]\r\n[s6; &]\r\n[s3; Detailed description&]\r\n[s5; &]\r"
"\n[s8; &]\r\n[s4;:Vector`& operator`<`<`(const T`& x`): Vector`& operator<<(const"
" \r\nT`& [*@3 x])&]\r\n[s5; &]\r\n[s3; Parameters&]\r\n[s1; x&]\r\n[s6; &]\r\n[s3"
"; Return value&]\r\n[s6; &]\r\n[s3; Detailed description&]\r\n[s5; &]\r\n[s8; &]\r"
"\n[s4;:Vector`& operator`|`(pick`_ T`& x`): Vector`& operator|(pick`_ \r\nT`& [*@"
"3 x])&]\r\n[s5; &]\r\n[s3; Parameters&]\r\n[s1; x&]\r\n[s6; &]\r\n[s3; Return val"
"ue&]\r\n[s6; &]\r\n[s3; Detailed description&]\r\n[s5; &]\r\n[s8; &]\r\n[s4;:void"
" Serialize`(Stream`& s`): void [* Serialize](Stream`& [*@3 s])&]\r\n[s5; &]\r\n[s"
"3; Parameters&]\r\n[s1; s&]\r\n[s6; &]\r\n[s3; Return value&]\r\n[s6; &]\r\n[s3; "
"Detailed description&]\r\n[s5; &]\r\n[s4; [* Vector]()&]\r\n[s4; &]\r\n[s5; &]\r\n"
"[s4; ~[* Vector]()&]\r\n[s4; &]\r\n[s5; &]\r\n[s4; [* Vector](pick`_ [* Vector]`&"
" v)&]\r\n[s4; &]\r\n[s5; &]\r\n[s8; &]\r\n[s4;:void operator`=`(pick`_ Vector`& v"
"`): void operator=(pick`_ \r\nVector`& [*@3 v])&]\r\n[s5; &]\r\n[s3; Parameters&]"
"\r\n[s1; v&]\r\n[s6; &]\r\n[s3; Return value&]\r\n[s6; &]\r\n[s3; Detailed descri"
"ption&]\r\n[s5; &]\r\n[s4; [* Vector](const [* Vector]`& v, int)&]\r\n[s4; &]\r\n"
"[s5; &]\r\n[s4;* [* typedef T ]ValueType&]\r\n[s4; &]\r\n[s5; &]\r\n[s4;* [* type"
"def T *]Iterator&]\r\n[s4; &]\r\n[")
TOPIC_TEXT(
"s5; &]\r\n[s4;* [* typedef const T *]ConstIterator&]\r\n[s4; &]\r\n[s5; &]\r\n[s"
"8; &]\r\n[s4;:ConstIterator Begin`(`) const: ConstIterator [* Begin]() const&]\r\n"
"[s5; &]\r\n[s3; Parameters&]\r\n[s1; &]\r\n[s6; &]\r\n[s3; Return value&]\r\n[s6;"
" &]\r\n[s3; Detailed description&]\r\n[s5; &]\r\n[s8; &]\r\n[s4;:ConstIterator En"
"d`(`) const: ConstIterator [* End]() const&]\r\n[s5; &]\r\n[s3; Parameters&]\r\n["
"s1; &]\r\n[s6; &]\r\n[s3; Return value&]\r\n[s6; &]\r\n[s3; Detailed description&"
"]\r\n[s5; &]\r\n[s8; &]\r\n[s4;:ConstIterator GetIter`(int i`) const: ConstIterat"
"or [* GetIter](int \r\n[*@3 i]) const&]\r\n[s5; &]\r\n[s3; Parameters&]\r\n[s1; i"
"&]\r\n[s6; &]\r\n[s3; Return value&]\r\n[s6; &]\r\n[s3; Detailed description&]\r\n"
"[s5; &]\r\n[s8; &]\r\n[s4;:Iterator Begin`(`): Iterator [* Begin]()&]\r\n[s5; &]\r"
"\n[s3; Parameters&]\r\n[s1; &]\r\n[s6; &]\r\n[s3; Return value&]\r\n[s6; &]\r\n[s"
"3; Detailed description&]\r\n[s5; &]\r\n[s8; &]\r\n[s4;:Iterator End`(`): Iterato"
"r [* End]()&]\r\n[s5; &]\r\n[s3; Parameters&]\r\n[s1; &]\r\n[s6; &]\r\n[s3; Retur"
"n value&]\r\n[s6; &]\r\n[s3; Detailed description&]\r\n[s5; &]\r\n[s8; &]\r\n[s4;"
":Iterator GetIter`(int i`): Iterator [* GetIter](int [*@3 i])&]\r\n[s5; &]\r\n[s3"
"; Parameters&]\r\n[s1; i&]\r\n[s6; &]\r\n[s3; Return value&]\r\n[s6; &]\r\n[s3; D"
"etailed description&]\r\n[s5; &]\r\n[s4; friend int [* GetCount](ConstIterator pt"
"r, ConstIterator end)&]\r\n[s4; &]\r\n[s5; &]\r\n[s4; friend int [* GetCount](Ite"
"rator ptr, Iterator end)&]\r\n[s4; &]\r\n[s5; &]\r\n[s4; friend void [* Swap](Vec"
"tor`& a, Vector`& b)&]\r\n[s4; &]\r\n[s5; &]\r\n[s4; friend void [* Append](Vecto"
"r`& dst, const Vector`& src)&]\r\n[s4; &]\r\n[s5; &]\r\n[s4; PickTemplate([* Vect"
"or])&]\r\n[s4; &]\r\n[s5; &]\r\n[s4; MoveTemplate([* Vector])&]\r\n[s4; &]\r\n[s5"
"; &]\r\n[s0; ")
