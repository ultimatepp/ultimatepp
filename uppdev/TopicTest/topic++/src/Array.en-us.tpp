TITLE("Array class template")
REF("::Array::Add()")
REF("::Array::Add(const T&)")
REF("void AddPick(pick_ T& x)")
REF("::Array::Add(T*)")
REF("::Array::operator[](int)const")
REF("::Array::operator[](int)")
REF("::Array::GetCount()const")
REF("::Array::IsEmpty()const")
REF("::Array::Trim(int)")
REF("::Array::SetCount(int)")
REF("::Array::SetCount(int,const T&)")
REF("::Array::SetCountR(int)")
REF("::Array::SetCountR(int,const T&)")
REF("::Array::Clear()")
REF("::Array::At(int)")
REF("::Array::At(int,const T&)")
REF("::Array::Shrink()")
REF("::Array::Reserve(int)")
REF("::Array::GetAlloc()const")
REF("::Array::Set(int,const T&,int)")
REF("::Array::Remove(int,int)")
REF("::Array::Remove(const int*,int)")
REF("::Array::Remove(const Vector<int>&)")
REF("::Array::InsertN(int,int)")
REF("::Array::Insert(int)")
REF("::Array::Insert(int,const T&,int)")
REF("::Array::Insert(int,const Array&)")
REF("::Array::Insert(int,const Array&,int,int)")
REF("::Array::Append(const Array&)")
REF("::Array::Append(const Array&,int,int)")
REF("::Array::InsertPick(int,pick_ Array&)")
REF("::Array::AppendPick(pick_ Array&)")
REF("::Array::Swap(int,int)")
REF("::Array::Move(int,int)")
REF("::Array::Detach(int)")
REF("::Array::Set(int,T*)")
REF("::Array::Insert(int,T*)")
REF("::Array::Drop(int)")
REF("::Array::Top()")
REF("::Array::Top()const")
REF("::Array::PopDetach()")
REF("::Array::Swap(Array&)")
REF("::Array::operator<<(const T&)")
REF("::Array::operator<<(T*)")
REF("Array& operator|(pick_ T& x)")
REF("::Array::IsPicked()const")
REF("::Array::Serialize(Stream&)")
REF("::Array::Array()")
REF("::Array::")
REF("::Array::Array(pick_ Array&)")
REF("::Array::operator=(pick_ Array&)")
REF("::Array::Array(const Array&,int)")
REF("::Array::operator<<=(const Array&)")
REF("::Array::typedef T ValueType")
REF("::Array::typedef T *Iterator")
REF("::Array::Begin()")
REF("::Array::End()")
REF("::Array::GetIter(int)")
REF("::Array::Begin()const")
REF("::Array::End()const")
REF("::Array::GetIter(int)const")
REF("friend void Swap(Array& a, Array& b)")
REF("friend void IterSwap(Iterator a, Iterator b)")
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
" class ]Array&][s0; &][s12; [*C@4 T]-|Type or base class of elements stored in Ar"
"ray. There is no common requirement for T.&][s5; The most universal form of basic"
" random access container. Its features are derived from fact that typically it is"
"  implemented using indirect container of pointers to T like Vector<T*>. Neverthe"
"less it supports common concepts as ownership of contained elements, reference (n"
"ot pointer) access to elements and so on.&][s5; It provides almost all operations"
" of Vector with same semantics and almost any Vector can be directly replaced by "
"Array. On the other hand, it provides some special operations impossible for Vect"
"or and most important, it never invalidates references (that means C`++ reference"
"s and pointers) to elements (it often invalidates iterato")
TOPIC_TEXT(
"rs, though).&][s5; Array can be also used to store polymorphic elements - stored"
" elements could be derived from T. To store such elements, you pass pointer to el"
"ement created on heap. Still, Array takes over ownership of such element (it e.g."
" deletes it when appropriate). You can also use this method to create Array of el"
"ements that do not have either pick or deep copy constructor.&][s5; There are als"
"o operations that allow detaching an element from Array, removing it but not dest"
"roying. Pointer to such element is returned from this operation and Array gives u"
"p ownership.&][s5; Disadvantage of Array over Vector is performance - most operat"
"ions are significantly slower than Vector's one (by factor up to 8, depends on sp"
"eed of malloc/free). Still, some operations might be even faster, e.g. Insert and"
" Remove operations usually move much less memory.&][s5; As for memory, for small "
"size of elements, Array memory consumption is significantly worse than Vector con"
"sumption. As size of elements grows, Array starts to be better than Vector.&][s5;"
"~~~.704; Iterators to Array satisfy all C`++ standard library requirements for ra"
"ndom access iterator plus they allow assignment (and copy constructor) and testin"
"g for 0 (that is NULL) pointer.&][s5; Like any other NTL container, Array is [*/^"
"topic`:`/`/Core`/srcdoc`/Moveable`:EN`-US^ moveable][*/  ]type with [*/^topic`:`/"
"`/Core`/srcdoc`/pick`_`:EN`-US^ pick and optional deep copy] transfer semantics. "
"Calling methods of picked Array is logic error with exception of&][s0;C+75 void ["
"* operator=](pick`_ Array`& [*@3 v])&][s0;/+75 [/C void ][*/C operator<<=][/C (co"
"nst Array`& ][*/C@3 v][/C )][/  ](defined using DeepCopyOptionTemplate)&][s0;C+75"
" void [* Clear]()&][s0;C+75 bool [* IsPicked]() const&][s5; Optional deep copy is"
" implemented through DeepCopyOptionTemplate macro.&][s3; Members&][s4;:`:`:Array`"
":`:Add`(`): T`& [* Add]()&][s6; Adds new default constructed element to Array.&]["
"s11; Requires T to have default constructor.&][s11; Inval")
TOPIC_TEXT(
"idates iterators to Array.&][s1; [*/ Return value]-|Reference to newly added def"
"ault constructed element.&][s4;:`:`:Array`:`:Add`(const T`&`): void [* Add](const"
" T`& [*@3 x])&][s6; Adds new element with specified value to Array.&][s11; Requir"
"es T to have deep copy constructor.&][s11; Invalidates iterators to Array.&][s1; "
"[*C@3 x]-|The value that is copied to newly created element.&][s4;:void AddPick`("
"pick`_ T`& x`): void [* AddPick](pick`_ T`& [*@3 x])&][s6; Adds new element to Ar"
"ray and picks value of parameter to it. &][s11; Requires T to have pick construct"
"or.&][s11; Invalidates iterators to Array.&][s1; [*C@3 x]-|Source instance of T t"
"hat is to be picked.&][s4;:`:`:Array`:`:Add`(T`*`): T`& [* Add](T *[*@3 newt])&]["
"s6; Adds new element to Array. Element is specified by pointer to object. Array t"
"akes over ownership of this this object. This variant allows use of Array as poly"
"morphic container, because type of added element can be also derived from T as we"
"ll. No constructor is applied. &][s1; [*C@3 newt]-|Object to be added.&][s1; [*/ "
"Return value]-|Reference to object  (that is [* *newt]).&][s4;:`:`:Array`:`:opera"
"tor`[`]`(int`)const:~~~.992; const T`& [* operator`[`]](int [*@3 i]) const&][s6; "
"Returns a reference to the element at specified position.&][s1; [*C@3 i]-|Positio"
"n of element.&][s1; [*/ Return value]-|Constant reference to the element.&][s4;:`"
":`:Array`:`:operator`[`]`(int`): T`& [* operator`[`]](int [*@3 i])&][s6; Returns "
"a reference to the element at specified position.&][s1; [*C@3 i]-|Position of ele"
"ment.&][s1; [*/ Return value]-|Reference to the element.&][s4;:`:`:Array`:`:GetCo"
"unt`(`)const: int [* GetCount]() const&][s6; Return the number of elements in Arr"
"ay.&][s1; [*/ Return value]-|Actual number of elements.&][s4;:`:`:Array`:`:IsEmpt"
"y`(`)const: bool [* IsEmpty]() const&][s6; Tests whether Array is empty. Same as "
"GetCount() == 0.&][s1; [*/ Return value]-|true if Array is empty, false otherwise"
".&][s4;:`:`:Array`:`:Trim`(int`): void [* Trim](int [*@3 ")
TOPIC_TEXT(
"n])&][s6; Reduces number of elements in Array to specified number. Required numb"
"er must be less than or equal to actual number of elements in Array.&][s1; [*C@3 "
"n]-|Required number of elements.&][s4;:`:`:Array`:`:SetCount`(int`): void [* SetC"
"ount](int [*@3 n])&][s6; Changes count of elements in Array to specified value. I"
"f required number of elements is greater than actual number, newly added elements"
" are default constructed. If Array has to increase capacity, the new capacity wil"
"l exactly match required number of elements (unlike [* SetCountR]).&][s11; Requir"
"es T to have default constructor.&][s11; Invalidates iterators to Array.&][s1; [*"
"C@3 n]-|Required number of elements.&][s4;:`:`:Array`:`:SetCount`(int`,const T`&`"
"): void [* SetCount](int [*@3 n], const T`& [*@3 init])&][s6; Changes count of el"
"ements in Array to specified value. If required number of elements is greater tha"
"n actual number, newly added elements are initialized to specified value using co"
"py constructor. If Array has to increase capacity, the new capacity will exactly "
"match required number of elements (unlike [* SetCountR]).&][s11; Requires T to ha"
"ve deep copy constructor.&][s11; Invalidates iterators to Array.&][s1; [*C@3 n]-|"
"Required number of elements.&][s1; [*C@3 init]-|Initialization value of newly add"
"ed elements.&][s4;:`:`:Array`:`:SetCountR`(int`): void [* SetCountR](int [*@3 n])"
"&][s6; Changes count of elements in Array to specified value. If required number "
"of elements is greater than actual number, newly added elements are default const"
"ructed. If Array has to increase capacity, the new capacity will be greater than "
"required number of elements (unlike [* SetCount]) to allow adding other elements "
"without further increasing capacity.&][s11; Requires T to have default constructo"
"r.&][s11; Invalidates iterators to Array.&][s1; [*C@3 n]-|Required number of elem"
"ents.&][s4;:`:`:Array`:`:SetCountR`(int`,const T`&`):~~~.992; void [* SetCountR]("
"int [*@3 n], const T`& [*@3 init])&][s6; Changes count of")
TOPIC_TEXT(
" elements in Array to specified value. If required number of elements is greater"
" than actual number, newly added elements are initialized to specified value usin"
"g copy constructor. If Array has to increase capacity, the new capacity will be g"
"reater than required number of elements (unlike [* SetCount]) to allow adding oth"
"er elements without further increasing capacity.&][s11; Requires T to have deep c"
"opy constructor.&][s11; Invalidates iterators to Array.&][s1; [*C@3 n]-|Required "
"number of elements.&][s1; [*C@3 init]-|Initialization value of newly added elemen"
"ts.&][s4;:`:`:Array`:`:Clear`(`):~~~.992; void [* Clear]()&][s6; Removes all elem"
"ents from Array. Capacity is also cleared to zero.&][s11; Invalidates iterators t"
"o Array.&][s4;:`:`:Array`:`:At`(int`):~~~.992; T`& [* At](int [*@3 i])&][s6; If s"
"pecified position is lower than number of elements in Array ([* i < GetCount()]),"
" returns reference to element at specified position. Otherwise increases number o"
"f elements in Array to [* i + 1]. Newly added elements are default constructed. I"
"f Array has to increase capacity, the new capacity will be greater than required "
"number of elements to allow adding other elements without further increasing capa"
"city.&][s11; Requires T to have default constructor.&][s11; Invalidates iterators"
" to Array.&][s1; [*C@3 i]-|Position of required element.&][s1; [*/ Return value]-"
"|Reference to required element.&][s4;:`:`:Array`:`:At`(int`,const T`&`): T`& [* A"
"t](int [*@3 i], const T`& [*@3 x])&][s6; If specified position is lower than numb"
"er of elements in Array ([* i < GetCount()]), returns reference to element at spe"
"cified position. Otherwise increases number of elements in Array to [* i + 1]. Ne"
"wly added elements are copy constructed from [* x]. If Array has to increase capa"
"city, the new capacity will be greater than required number of elements to allow "
"adding other elements without further increasing capacity.&][s11; Requires T to h"
"ave deep copy constructor.&][s11; Invalidates iterators t")
TOPIC_TEXT(
"o Array.&][s1; [*C@3 i]-|Position of required element.&][s1; [*C@3 x]-|Initializ"
"ation value of newly added elements.&][s1; [*/ Return value]-|Reference to requir"
"ed element.&][s4;:`:`:Array`:`:Shrink`(`): void [* Shrink]()&][s6; Minimizes memo"
"ry consumption of Array by decreasing capacity to number of elements.&][s4;:`:`:A"
"rray`:`:Reserve`(int`):~~~.992; void [* Reserve](int [*@3 xtra])&][s6; Reserves c"
"apacity. If required capacity is greater than current capacity, capacity is incre"
"ased to the required value.&][s1; [*C@3 xtra]-|Required capacity.&][s4;:`:`:Array"
"`:`:GetAlloc`(`)const:~~~.992; int [* GetAlloc]() const&][s6; Returns current cap"
"acity of Array.&][s1; [*/ Return value]-|Capacity of Array.&][s4;:`:`:Array`:`:Se"
"t`(int`,const T`&`,int`): void [* Set](int [*@3 i], const T`& [*@3 x], int [*@3 c"
"ount] = 1)&][s6; Sets requested number of elements starting at the position [* i]"
" to the specified value. If required number of elements exceeds existing elements"
" of Array then elements are added to Array.&][s11; Requires T to have deep copy c"
"onstructor.&][s11; Invalidates iterators to Array.&][s1; [*C@3 i]-|Starting posit"
"ion.&][s1; [*C@3 x]-|Value.&][s1; [*C@3 count]-|Number of elements.&][s4;:`:`:Arr"
"ay`:`:Remove`(int`,int`): void [* Remove](int [*@3 i], int [*@3 count] = 1)&][s6;"
" Removes requested number of elements.&][s11; Invalidates iterators to Array.&][s"
"1; [*C@3 i]-|Position.&][s1; [*C@3 count]-|Number of elements to remove.&][s4;:`:"
"`:Array`:`:Remove`(const int`*`,int`): void [* Remove](const int *[*@3 sorted`_li"
"st], int [*@3 n])&][s6; Removes number of elements from Array. Time of operation "
"almost does not depend on number of elements.&][s11; Invalidates iterators to Arr"
"ay.&][s1; [*C@3 sorted`_list]-|Pointer to array of positions to remove. It must b"
"e sorted from lowest to greatest value.&][s1; [*C@3 n]-|Number of elements to rem"
"ove.&][s4;:`:`:Array`:`:Remove`(const Vector`<int`>`&`): void [* Remove](const Ve"
"ctor<int>`& [*@3 sorted`_list])&][s6; Removes number of e")
TOPIC_TEXT(
"lements form Array. Same as [* Remove(sorted`_list, sorted`_list.GetCount())].&]"
"[s11; Invalidates iterators to Array.&][s1; [*C@3 sorted`_list]-|Sorted Vector of"
" positions to remove.&][s4;:`:`:Array`:`:InsertN`(int`,int`): void [* InsertN](in"
"t [*@3 i], int [*@3 count] = 1)&][s6; Inserts a specified number of default const"
"ructed elements at a specified position.&][s11; Requires T to have default constr"
"uctor.&][s11; Invalidates iterators to Array.&][s1; [*C@3 i]-|Position.&][s1; [*C"
"@3 count]-|Number of elements to insert.&][s4;:`:`:Array`:`:Insert`(int`):~~~.992"
"; T`& [* Insert](int [*@3 i])&][s6; Inserts one default constructed element at th"
"e specified position.&][s1;*1 Requires T to have default constructor.&][s11; Inva"
"lidates iterators to Array.&][s1; [*C@3 i]-|Position.&][s4;:`:`:Array`:`:Insert`("
"int`,const T`&`,int`): void [* Insert](int [*@3 i], const T`& [*@3 x], int [*@3 c"
"ount] = 1)&][s6; Inserts a specified number of elements, setting them to a specif"
"ied value.&][s11;~~~.992; Requires T to have deep copy constructor.&][s11; Invali"
"dates iterators to Array.&][s1; [*C@3 i]-|Position.&][s1; [*C@3 x]-|Value of inse"
"rted elements.&][s1; [*C@3 count]-|Number of elements to insert.&][s4;:`:`:Array`"
":`:Insert`(int`,const Array`&`):~~~.992; void [* Insert](int [*@3 i], const Array"
"`& [*@3 x])&][s6; Inserts all elements from another Array.&][s11;~~~.992; Require"
"s T to have deep copy constructor.&][s11; Invalidates iterators to Array.&][s1; ["
"*C@3 i]-|Position.&][s1; [*C@3 x]-|Source Array.&][s4;:`:`:Array`:`:Insert`(int`,"
"const Array`&`,int`,int`):~~~.992; void [* Insert](int [*@3 i], const Array`& [*@"
"3 x], int [*@3 offset], int [*@3 count])&][s6; Inserts a range of elements from a"
"nother Array.&][s11;~~~.992; Requires T to have deep copy constructor.&][s11; Inv"
"alidates iterators to Array.&][s1; [*C@3 i]-|Insertion position.&][s1; [*C@3 x]-|"
"Source Array.&][s1; [*C@3 offset]-|Position of first element in source Array to b"
"e inserted.&][s1; [*C@3 count]-|Number of elements to ins")
TOPIC_TEXT(
"ert.&][s4;:`:`:Array`:`:Append`(const Array`&`): void [* Append](const Array`& ["
"*@3 x])&][s6; Appends all elements of source Array.&][s11;~~~.992; Requires T to "
"have deep copy constructor.&][s11; Invalidates iterators to Array.&][s1; [*C@3 x]"
"-|Source Array.&][s4;:`:`:Array`:`:Append`(const Array`&`,int`,int`): void [* App"
"end](const Array`& [*@3 x], int [*@3 o], int [*@3 c])&][s6; Appends a range of el"
"ements from source Array.&][s11;~~~.992; Requires T to have deep copy constructor"
".&][s11; Invalidates iterators to Array.&][s1; [*C@3 x]-|Source Array.&][s1; [*C@"
"3 o]-|Position of first element in source Array to be inserted.&][s1; [*C@3 c]-|N"
"umber of elements to insert.&][s4;:`:`:Array`:`:InsertPick`(int`,pick`_ Array`&`)"
": void [* InsertPick](int [*@3 i], pick`_ Array`& [*@3 x])&][s6; Inserts source A"
"rray at specified position using pick transfer semantics. It is faster than deep "
"copy insert, does not use deep copy constructor for T, but destroys source Array "
"by picking.&][s11; Invalidates iterators to Array.&][s1; [*C@3 i]-|Insertion posi"
"tion.&][s1; [*C@3 x]-|Source Array.&][s4;:`:`:Array`:`:AppendPick`(pick`_ Array`&"
"`): void [* AppendPick](pick`_ Array`& [*@3 x])&][s6; Appends source Array using "
"pick transfer semantics. It is faster than deep copy insert, does not use deep co"
"py constructor for T, but destroys source Array by picking.&][s11; Invalidates it"
"erators to Array.&][s1; [*C@3 x]-|Source Array.&][s4;:`:`:Array`:`:Swap`(int`,int"
"`): void [* Swap](int [*@3 i1], int [*@3 i2])&][s6; Swaps elements without using "
"copy constructor.&][s11; Invalidates iterators to Array.&][s1; [*C@3 i1]-|Positio"
"n of first element.&][s1; [*C@3 i2]-|Position of second element.&][s4;:`:`:Array`"
":`:Move`(int`,int`): void [* Move](int [*@3 i1], int [*@3 i2])&][s6; Removes elem"
"ent at position [* i1] and inserts it at [* i2], without using copy constructor o"
"f T.&][s11; Invalidates iterators to Array.&][s1; [*C@3 i1]-|Position of element "
"to move.&][s1; [*C@3 i2]-|Target position.&][s4;:`:`:Arra")
TOPIC_TEXT(
"y`:`:Detach`(int`): T *[* Detach](int [*@3 i])&][s6; Removes element at position"
" [* i], giving up ownership. Client is responsible for deletion of element.&][s11"
"; Invalidates iterators to Array.&][s1; [*C@3 i]-|Position of element to remove.&"
"][s1; [*/ Return value]-|Pointer to element allocated on the heap.&][s4;:`:`:Arra"
"y`:`:Set`(int`,T`*`): T`& [* Set](int [*@3 i], T *[*@3 newt])&][s6; Replaces elem"
"ent at the specified position by element created on heap. Array takes over owners"
"hip of element.&][s1; [*C@3 i]-|Position.&][s1; [*C@3 newt]-|New element allocate"
"d on heap.&][s1; [*/ Return value]-|Reference to new element.&][s4;:`:`:Array`:`:"
"Insert`(int`,T`*`): void [* Insert](int [*@3 i], T *[*@3 newt])&][s6; Inserts ele"
"ment created on heap at the specified position. Array takes over ownership of ele"
"ment.&][s11; Invalidates iterators to Array.&][s1; [*C@3 i]-|Insertion position.&"
"][s1; [*C@3 newt]-|New element allocated on heap.&][s1; [*/ Return value]-|Pointe"
"r to element allocated on heap.&][s4;:`:`:Array`:`:Drop`(int`): void [* Drop](int"
" [*@3 n] = 1)&][s6; Drops specified number of last elements in the Array (same as"
" Trim(GetCount() - n)).&][s1;* [*C@3 n]-|Number of elements.&][s4;:`:`:Array`:`:T"
"op`(`): T`& [* Top]()&][s6; Returns reference to the last element in the Array.&]"
"[s1; [*/ Return value]-|Reference of last element in the Array.&][s4;:`:`:Array`:"
"`:Top`(`)const: const T`& [* Top]() const&][s6; Returns constant reference to the"
" last element in the Array.&][s1; [*/ Return value]-|Reference of last element in"
" the Array.&][s4;:`:`:Array`:`:PopDetach`(`): T *[* PopDetach]()&][s6; Drops last"
" element in the Array, giving up ownership (same as Detach(GetCount() - 1)). Clie"
"nt is responsible for deletion of element.&][s11; Invalidates iterators to Array."
"&][s1; [*/ Return value]-|Pointer to element allocated on the heap.&][s4;:`:`:Arr"
"ay`:`:Swap`(Array`&`): void [* Swap](Array`& [*@3 b])&][s6; Swaps content of Arra"
"y with another array in constant time operation.&][s1; [*")
TOPIC_TEXT(
"C@3 b]-|Target array.&][s4;:`:`:Array`:`:operator`<`<`(const T`&`): Array`& [* o"
"perator<<](const T`& [*@3 x])&][s6; Operator replacement of [* void Add(const T`&"
"x)]. By returning reference to to Array allows adding more elements in single exp"
"ression, thus e.g. allowing to construct temporary Array as part of expression li"
"ke Foo((Array<int>() << 1 << 2 << 4)).&][s11; Requires T to have deep copy constr"
"uctor.&][s11; Invalidates iterators to Array.&][s1; [*C@3 x]-|The value that is c"
"opied to newly created element.&][s1; [*/ Return value]-|Reference to Array (*thi"
"s).&][s4;:`:`:Array`:`:operator`<`<`(T`*`): Array`& [* operator<<](T *[*@3 newt])"
"&][s6; Operator replacement of [* void Add(T *x)]. By returning reference to to a"
"rray allows adding more elements in single expression, thus e.g. allowing to cons"
"truct temporary array as part of expression like Foo((Array<Bar>() << new Bar << "
"new DerivedFromBar)).&][s11; Invalidates iterators to Array.&][s1; [*C@3 newt]-|O"
"bject to be added.&][s1; [*/ Return value]-|Reference to Array (*this).&][s4;:Arr"
"ay`& operator`|`(pick`_ T`& x`): Array`& [* operator|](pick`_ T`& [*@3 x])&][s6; "
"Operator replacement of [* void AddPick(pick`_ T`&x)]. By returning reference to "
"to array allows adding more elements in single expression, thus e.g. allowing to "
"construct temporary array as part of expression.&][s11; Requires T to have pick c"
"onstructor.&][s11; Invalidates iterators to Array.&][s1; [*C@3 x]-|Source instanc"
"e of T that is to be picked.&][s1; [*/ Return value]-|Reference to Array (*this)."
"&][s4;:`:`:Array`:`:IsPicked`(`)const: bool [* IsPicked]() const&][s6; Returns [*"
" true] if Array is in picked state.&][s1; [*/ Return value]-|[* true] if Array is"
" in picked state, [* false] otherwise.&][s4;:`:`:Array`:`:Serialize`(Stream`&`): "
"void [* Serialize](Stream`& [*@3 s])&][s6; Serializes content of Array to/from St"
"ream. Works only if NTL is used as part of UPP.&][s11; Requires T to have seriali"
"zation operator defined.&][s1; [*C@3 s]-|Target/source st")
TOPIC_TEXT(
"ream.&][s4;:`:`:Array`:`:Array`(`): [* Array]()&][s6; Default constructor. Const"
"ructs empty Array.&][s4;:`:`:Array`:`:: ~[* Array]()&][s6; Destructor. Calls dest"
"ructors of all elements in Array.&][s4;:`:`:Array`:`:Array`(pick`_ Array`&`): [* "
"Array](pick`_ Array`& [*@3 v])&][s6; Pick constructor. Transfers source Array in "
"low constant time, but destroys it by picking. &][s1; [*C@3 v]-|Source Array.&][s"
"4;:`:`:Array`:`:operator`=`(pick`_ Array`&`):~~~.992; void [* operator=](pick`_ A"
"rray`& [*@3 v])&][s6; Pick operator. Transfers source Array in low constant time,"
" but destroys it by picking.&][s1; [*C@3 v]-|Source Array.&][s4;:`:`:Array`:`:Arr"
"ay`(const Array`&`,int`): [* Array](const Array`& [*@3 v], int)&][s6; Optional de"
"ep copy constructor.&][s11; Requires T to have deep copy constructor or optional "
"deep copy constructor if Array stores only objects of type T.&][s11; Requires pol"
"ymorphic deep copy if Array stores also objects of type derived from T.&][s1; [*C"
"@3 v]-|Source Array.&][s4;:`:`:Array`:`:operator`<`<`=`(const Array`&`): void [* "
"operator<<=](const Array`& [*@3 v])&][s6; Optional deep copy operator. Defined us"
"ing DeepCopyOptionTemplate.&][s11; Requires T to have deep copy constructor or op"
"tional deep copy if Array stores only objects of type T.&][s11; Requires polymorp"
"hic deep copy if Array stores also objects of type derived from T.&][s1; [*C@3 v]"
"-|Source Array.&][s4;:`:`:Array`:`:typedef T ValueType:* [* typedef T ]ValueType&"
"][s6; Typedef of T for use in templated algorithms.&][s4;:`:`:Array`:`:typedef T "
"`*Iterator: typedef [/ type] [* Iterator];&][s6; Iterator type.&][s4; typedef [/ "
"type] [* ConstIterator];&][s6; Constant iterator type.&][s4;:`:`:Array`:`:Begin`("
"`): Iterator [* Begin]()&][s6; Returns non-constant iterator to the first element"
" in Array. &][s1; [*/ Return value]-|Iterator.&][s4;:`:`:Array`:`:End`(`): Iterat"
"or [* End]()&][s6; Returns non-constant iterator to the position just beyond the "
"last element in Array.&][s1; [*/ Return value]-|Iterator.")
TOPIC_TEXT(
"&][s4;:`:`:Array`:`:GetIter`(int`): Iterator [* GetIter](int [*@3 pos])&][s6; Re"
"turns non-constant iterator to the element at specified position. Same as [* Begi"
"n() + pos]. Benefit of this methods is that in debug mode [* pos] is range checke"
"d.&][s1; [*C@3 pos]-|Required position.&][s1; [*/ Return value]-|Iterator.&][s4;:"
"`:`:Array`:`:Begin`(`)const: ConstIterator [* Begin]() const&][s6; Returns consta"
"nt iterator to the first element in Array. &][s1; [*/ Return value]-|Iterator.&]["
"s4;:`:`:Array`:`:End`(`)const: ConstIterator [* End]() const&][s6; Returns consta"
"nt iterator to the position just beyond the last element in Array.&][s1; [*/ Retu"
"rn value]-|Iterator.-|&][s4;:`:`:Array`:`:GetIter`(int`)const: ConstIterator [* G"
"etIter](int [*@3 pos]) const&][s6; Returns constant iterator to the element at sp"
"ecified position. Same as [* Begin() + pos]. Benefit of this methods is that in d"
"ebug mode [* pos] is range checked.&][s1; [*C@3 pos]-|Required position.&][s1; [*"
"/ Return value]-|Iterator.&][s4;:friend void Swap`(Array`& a`, Array`& b`): frien"
"d void [* Swap](Array`& [*@3 a], Array`& [*@3 b])&][s6; Specialization of generic"
" [* Swap] for Array. Swaps array in simple constant time operation.&][s1; [*C@3 a"
"]-|First Array to swap.&][s1; [*C@3 b]-|Second Array to swap.&][s4;:friend void I"
"terSwap`(Iterator a`, Iterator b`): friend void [* IterSwap](Iterator [*@3 a], It"
"erator [*@3 b])&][s6; Specialization of generic [* IterSwap] for Array. Swaps ele"
"ments in Array without any requirements for T.&][s1; [*C@3 a]-|Iterator to first "
"element.&][s1; [*C@3 b]-|Iterator to second element.&][s0; ")
