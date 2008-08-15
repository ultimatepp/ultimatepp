TITLE("AMap class template")
REF("::AMap::template <class K, class T, class V, class HashFn> class AMap")
REF("::AMap::Add(const K&,const T&)")
REF("void AddPick(const K& k, pick_ T& x)")
REF("::AMap::Add(const K&)")
REF("::AMap::FindAdd(const K&)")
REF("::AMap::Find(const K&)const")
REF("::AMap::Find(const K&,unsigned)const")
REF("::AMap::FindLast(const K&,unsigned)const")
REF("::AMap::FindLast(const K&)const")
REF("::AMap::FindPrev(int)const")
REF("::AMap::FindAdd(const K&,const T&)")
REF("int FindAddPick(const K& k, pick_ T& init)")
REF("::AMap::Unlink(int)")
REF("::AMap::Put(const K&,const T&)")
REF("void PutPick(const K& k, pick_ T& x)")
REF("::AMap::Put(const K&)")
REF("::AMap::FindPut(const K&)")
REF("::AMap::FindPut(const K&,const T&)")
REF("::AMap::FindPutPick(const K&,pick_ T&)")
REF("::AMap::Get(const K&)")
REF("::AMap::Get(const K&)const")
REF("::AMap::Get(const K&,const T&)const")
REF("::AMap::GetAdd(const K&)")
REF("::AMap::GetAdd(const K&,const T&)")
REF("::AMap::GetAddPick(const K&,pick_ T&)")
REF("::AMap::GetPut(const K&)")
REF("::AMap::GetPut(const K&,const T&)")
REF("::AMap::GetPutPick(const K&,pick_ T&)")
REF("::AMap::SetKey(int,const K&)")
REF("::AMap::FindPtr(const K&)")
REF("::AMap::FindPtr(const K&)const")
REF("::AMap::operator()(const K&)")
REF("::AMap::operator()(const K&)const")
REF("::AMap::operator()(const K&,const T&)const")
REF("::AMap::UnlinkKey(const K&,unsigned)")
REF("::AMap::UnlinkKey(const K&)")
REF("::AMap::IsUnlinked(int)const")
REF("::AMap::Insert(int,const K&)")
REF("::AMap::Insert(int,const K&,const T&)")
REF("::AMap::Remove(int)")
REF("::AMap::Remove(const int*,int)")
REF("::AMap::Remove(const Vector<int>&)")
REF("::AMap::RemoveKey(const K&)")
REF("::AMap::operator[](int)const")
REF("::AMap::operator[](int)")
REF("::AMap::GetCount()const")
REF("::AMap::IsEmpty()const")
REF("::AMap::Clear()")
REF("::AMap::Shrink()")
REF("::AMap::Reserve(int)")
REF("::AMap::GetAlloc()const")
REF("::AMap::Drop(int)")
REF("::AMap::Top()")
REF("::AMap::Top()const")
REF("::AMap::TopKey()const")
REF("::AMap::PopKey()")
REF("::AMap::GetKey(int)const")
REF("::AMap::Serialize(Stream&)")
REF("::AMap::GetIndex()const")
REF("::AMap::PickIndex()")
REF("::AMap::GetKeys()const")
REF("::AMap::PickKeys()")
REF("::AMap::GetValues()const")
REF("::AMap::PickValues()")
REF("::AMap::AMap()")
REF("::AMap::AMap(const AMap&,int)")
REF("::AMap::AMap(pick_ Index<K>&,pick_ V&)")
REF("::AMap::AMap(pick_ Vector<K>&,pick_ V&)")
REF("::AMap::typedef K KeyType")
REF("::AMap::typedef typename Index<K>::ConstIterator KeyConstIterator")
REF("::AMap::KeyBegin()const")
REF("::AMap::KeyEnd()const")
REF("::AMap::KeyGetIter(int)const")
REF("::AMap::typedef T ValueType")
REF("::AMap::typedef typename V::ConstIterator ConstIterator")
REF("::AMap::typedef typename V::Iterator Iterator")
REF("::AMap::Begin()")
REF("::AMap::End()")
REF("::AMap::GetIter(int)")
REF("::AMap::Begin()const")
REF("::AMap::End()const")
REF("::AMap::GetIter(int)const")
REF("friend int GetCount(const AMap& v)")
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
"5:package`-title][{_}%EN-US [s10;:`:`:AMap`:`:template `<class K`, class T`, clas"
"s V`, class HashFn`> class AMap:~~~64; template <class [*@4 K], class [*@4 T], cl"
"ass [*@4 V], class [*@4 HashFn]>&][s10;~~~64;* [* class ]AMap&][s0; &][s12; [*C@4"
" K]-|Type of keys. K must have deep copy constructor, be [*/^dpp`:`/`/SourceDoc`/"
"Containers`/Moveable^ moveable] and must have [*C operator==] defined.&][s12; [*C"
"@4 T]-|Type of values. T must satisfy requirements for container flavor identifie"
"d by parameter V.&][s12; [*C@4 V]-|Type of basic random access container.&][s12; "
"[*C@4 HashFn]-|Hashing class. Must have defined [*C unsigned operator()(const K`&"
" x)] method returning hash value for elements.&][s5;~~~.416; AMap is a class that"
" combines Index of keys with basic random access container of values, forming map"
" flavors. It is used as base class for concrete map flavo")
TOPIC_TEXT(
"rs, [* VectorMap], [* ArrayMap] and [* SegtorMap].&][s5;~~~.416; Like any other "
"NTL container, AMap is [*/^dpp`:`/`/SourceDoc`/Containers`/Moveable^ moveable][*/"
"  ]type with [*/^dpp`:`/`/SourceDoc`/Containers`/pick`_^ pick and optional deep c"
"opy] transfer semantics, although these features are more important in derived co"
"ncrete index flavors.&][s3; Members&][s4;:`:`:AMap`:`:Add`(const K`&`,const T`&`)"
": void [* Add](const K`& [*@3 k], const T`& [*@3 x])&][s6; Adds a key and value p"
"air to the AMap.&][s11; T must have deep copy constructor.&][s11; Invalidates ite"
"rators to AMap.&][s11; Invalidates references to keys.&][s11; Invalidates referen"
"ces to VectorMap values.&][s1; [*C@3 k]-|Key.&][s1; [*C@3 x]-|Value.&][s4;:void A"
"ddPick`(const K`& k`, pick`_ T`& x`): void [* AddPick](const K`& [*@3 k], pick`_ "
"T`& [*@3 x])&][s6; Adds a key and value pair to the AMap. Value is transfered by "
"pick constructor in low constant time, but source value is destroyed.&][s11; T mu"
"st have pick constructor.&][s11; Invalidates iterators to AMap.&][s11; Invalidate"
"s references to keys.&][s11; Invalidates references to VectorMap values.&][s1; [*"
"C@3 k]-|Key.&][s1; [*C@3 x]-|Value.&][s4;:`:`:AMap`:`:Add`(const K`&`): T`& [* Ad"
"d](const K`& [*@3 k])&][s6; Adds a key to the AMap and returns the reference to t"
"he corresponding default constructed value.&][s11; T must have default constructo"
"r.&][s11; Invalidates iterators to AMap.&][s11; Invalidates references to keys.&]"
"[s11; Invalidates references to VectorMap values.&][s1; [*C@3 k]-|Key.&][s1; [*/ "
"Return value]-|Reference to value.&][s4;:`:`:AMap`:`:FindAdd`(const K`&`): int [*"
" Find](const K`& [*@3 k], unsigned [*@3 h]) const&][s6; Retrieves position of fir"
"st element with specified key in AMap, using precomputed hash value. Precomputed "
"hash value must be same as hash value that would be result of HashFn. If multi-ke"
"y ordering is not broken and more than one element with the same value exists in "
"AMap, lowest position is retrieved. If the element does n")
TOPIC_TEXT(
"ot exist in AMap, negative number is returned. Unlinked elements are ignored.&]["
"s1; [*C@3 x]-|Key to find.&][s1; [*C@3 h]-|Precomputed hash value.&][s1; [*/ Retu"
"rn value]-|Position of element or negative value if element is not in AMap.&][s4;"
":`:`:AMap`:`:Find`(const K`&`)const:~~~.992; int [* Find](const K`& [*@3 k]) cons"
"t&][s6; Retrieves position of first element with specified key in AMap. If multi-"
"key ordering is not broken and more than one element with the same value exists i"
"n AMap, lowest position is retrieved. If the element does not exist in AMap, nega"
"tive number is returned. Unlinked elements are ignored.&][s1; [*C@3 x]-|Key to fi"
"nd.&][s1; [*/ Return value]-|Position of element or negative value if element is "
"not in AMap.&][s4;:`:`:AMap`:`:Find`(const K`&`,unsigned`)const: int [* FindNext]"
"(int [*@3 i]) const&][s6; Retrieves position of next element with the same key as"
" element at the specified position. If multi-key ordering is not broken and more "
"than one element with that value exists in AMap, lowest position greater than spe"
"cified one is retrieved (so that positions got by subsequent calls to FindNext ar"
"e in ascending order). When there are no more elements with required key, negativ"
"e number is returned. Unlinked elements are ignored.&][s1; [*C@3 i]-|Position of "
"element.&][s1; [*/ Return value]-|Position of next element with same value.&][s4;"
":`:`:AMap`:`:FindLast`(const K`&`,unsigned`)const: int [* FindLast](const K`& [*@"
"3 k], unsigned [*@3 h]) const&][s6; Retrieves position of last element with speci"
"fied key in AMap, using precomputed hash value. Precomputed hash value must be sa"
"me as hash value that would be result of HashFn. If multi-key ordering is not bro"
"ken and more than one element with the same value exists in AMap, greatest positi"
"on is retrieved. If element does not exist in AMap, negative number is returned. "
"Unlinked elements are ignored.&][s1; [*C@3 x]-|Key to find.&][s1; [*C@3 h]-|Preco"
"mputed hash value.&][s1; [*/ Return value]-|Position of e")
TOPIC_TEXT(
"lement or negative value if element is not in AMap.&][s4;:`:`:AMap`:`:FindLast`("
"const K`&`)const: int [* FindLast](const K`& [*@3 k]) const&][s6; Retrieves posit"
"ion of last element with specified key in AMap. If multi-key ordering is not brok"
"en and more than one element with the same value exists in AIndex, greatest posit"
"ion is retrieved. If element does not exist in AMap, negative number is returned."
" Unlinked elements are ignored.&][s1; [*C@3 x]-|Element to find.&][s1; [*/ Return"
" value]-|Position of element or negative value if element is not in AMap.&][s4;:`"
":`:AMap`:`:FindPrev`(int`)const: int [* FindPrev](int [*@3 i]) const&][s6; Retrie"
"ves position of previous element with the same key as element at specified positi"
"on. If multi-key ordering is not broken and more than one element with that value"
" exists in AMap, greatest position lower than specified one is retrieved (so that"
" positions got by subsequent calls to FindNext are in descending order). When the"
"re are no more elements with required key, negative number is returned. Unlinked "
"elements are ignored.&][s1; [*C@3 i]-|Position of element.&][s1; [*/ Return value"
"]-|Position of previous element with same value.&][s4;:`:`:AMap`:`:FindAdd`(const"
" K`&`):~~~.992; int [* FindAdd](const K`& [*@3 k])&][s6; Retrieves position of fi"
"rst element with specified key in AMap. If multi-key ordering is not broken and m"
"ore than one element with the same value exists in AMap, lowest position is retri"
"eved. If the element does not exist in AMap, adds new default constructed element"
" at the end of AMap and returns its position. Unlinked elements are ignored.&][s1"
"1; T must have default constructor.&][s11; Invalidates iterators to AMap.&][s11; "
"Invalidates references to keys.&][s11; Invalidates references to VectorMap values"
".&][s1; [*C@3 k]-|Key to find.&][s1; [*/ Return value]-|Position of element (eith"
"er found or added).&][s4;:`:`:AMap`:`:FindAdd`(const K`&`,const T`&`): int [* Fin"
"dAdd](const K`& [*@3 k], const T`& [*@3 init])&][s6; Retr")
TOPIC_TEXT(
"ieves position of first element with specified key in AMap. If multi-key orderin"
"g is not broken and more than one element with the same value exists in AMap, low"
"est position is retrieved. Unlinked elements are ignored. If the element does not"
" exist in AMap, adds new element, deep copy constructed from [*/ init], at the en"
"d of AMap and returns its position.&][s11; T must have deep copy constructor.&][s"
"11; Invalidates iterators to AMap.&][s11; Invalidates references to keys.&][s11; "
"Invalidates references to VectorMap values.&][s1; [*C@3 k]-|Key to find.&][s1; [*"
"C@3 init]-|Value to add if key is not in AMap yet.&][s1; [*/ Return value]-|Posit"
"ion of element (either found or added).&][s4;:int FindAddPick`(const K`& k`, pick"
"`_ T`& init`): int [* FindAddPick](const K`& [*@3 k], pick`_ T`& [*@3 init])&][s6"
"; Retrieves position of first element with specified key in AMap. If multi-key or"
"dering is not broken and more than one element with the same value exists in AMap"
", lowest position is retrieved. Unlinked elements are ignored. If the element doe"
"s not exist in AMap, adds new element, pick constructed from [*/ init], at the en"
"d of AMap and returns its position.&][s11; T must have pick constructor.&][s11; I"
"nvalidates iterators to AMap.&][s11; Invalidates references to keys.&][s11; Inval"
"idates references to VectorMap values.&][s1; [*C@3 k]-|Key to find.&][s1; [*C@3 i"
"nit]-|Value to add if key is not in AMap yet.&][s1; [*/ Return value]-|Position o"
"f element (either found or added).&][s4;:`:`:AMap`:`:Unlink`(int`): void [* Unlin"
"k](int [*@3 i])&][s6; Unlinks element at the specified position. Unlinked item st"
"ays in AMap but is ignored by any Find operation.&][s1; [*C@3 i]-|Position of ele"
"ment to unlink.&][s4;:`:`:AMap`:`:Put`(const K`&`,const T`&`): void [* Put](const"
" K`& [*@3 k], const T`& [*@3 x])&][s6; If there is any unlinked element in AMap, "
"it is replaced by specified key/value pair. If there is none unlinked element, ke"
"y/value pair is added at the end of AIndex using [* Add].")
TOPIC_TEXT(
" Value is transfered using deep copy constructor.&][s11; T must have deep copy c"
"onstructor.&][s11; Invalidates multi-key ordering.&][s11; Invalidates iterators t"
"o AMap.&][s11; Invalidates references to keys.&][s11; Invalidates references to V"
"ectorMap values.&][s1; [*C@3 k]-|Key.&][s1; [*C@3 x]-|Value.&][s4;:void PutPick`("
"const K`& k`, pick`_ T`& x`): void [* PutPick](const K`& [*@3 k], pick`_ T`& [*@3"
" x])&][s6; If there is any unlinked element in AMap, it is replaced by specified "
"key/value pair. If there is none unlinked element, key/value pair is added at the"
" end of AIndex using [* Add]. Value is transfered using pick constructor.&][s11; "
"T must have pick constructor.&][s11; Invalidates multi-key ordering.&][s11; Inval"
"idates iterators to AMap.&][s11; Invalidates references to keys.&][s11; Invalidat"
"es references to VectorMap values.&][s1; [*C@3 k]-|Key.&][s1; [*C@3 x]-|Value.&]["
"s4;:`:`:AMap`:`:Put`(const K`&`): T`& [* Put](const K`& [*@3 k])&][s6; If there i"
"s any unlinked element in AMap, it is replaced by specified key and reference to "
"the value is returned. If there is none unlinked element, key is added at the end"
" of AIndex using [* Add ]and reference to corresponding default constructed Value"
" is returned.&][s11; T must have default constructor.&][s11; Invalidates multi-ke"
"y ordering.&][s11; Invalidates iterators to AMap.&][s11; Invalidates references t"
"o keys.&][s11; Invalidates references to VectorMap values.&][s1; [*C@3 k]-|Key.&]"
"[s1; [*/ Return value]-|Reference to the corresponding value.&][s4;:`:`:AMap`:`:F"
"indPut`(const K`&`): int [* FindPut](const K`& [*@3 k])&][s6; Retrieves position "
"of first element with specified key in AMap. Unlinked elements are ignored. If th"
"e element does not exist in AMap, puts new default constructed element into AMap "
"using [* Put] and returns its position. &][s11; T must have default constructor.&"
"][s11; Invalidates iterators to AMap.&][s11; Invalidates references to keys.&][s1"
"1; Invalidates references to VectorMap values.&][s1; [*C@")
TOPIC_TEXT(
"3 k]-|Key to find.&][s1; [*/ Return value]-|Position of element (either found or"
" added).&][s4;:`:`:AMap`:`:FindPut`(const K`&`,const T`&`): int [* FindPut](const"
" K`& [*@3 k], const T`& [*@3 init])&][s6; Retrieves position of first element wit"
"h specified key in AMap. Unlinked elements are ignored. If the element does not e"
"xist in AMap, puts new element, deep copy constructed from [* init], using [* Put"
"] and returns its position. &][s11; T must have deep copy constructor.&][s11; Inv"
"alidates iterators to AMap.&][s11; Invalidates references to keys.&][s11; Invalid"
"ates references to VectorMap values.&][s1; [*C@3 k]-|Key to find.&][s1; [*C@3 ini"
"t]-|Value to add if key is not in AMap yet.&][s1; [*/ Return value]-|Position of "
"element (either found or added).&][s4;:`:`:AMap`:`:FindPutPick`(const K`&`,pick`_"
" T`&`): int [* FindPutPick](const K`& [*@3 k], pick`_ T`& [*@3 init])&][s6; Retri"
"eves position of first element with specified key in AMap. Unlinked elements are "
"ignored. If the element does not exist in AMap, puts new element, pick constructe"
"d from [* init], using [* Put] and returns its position. &][s11; T must have deep"
" copy constructor.&][s11; Invalidates iterators to AMap.&][s11; Invalidates refer"
"ences to keys.&][s11; Invalidates references to VectorMap values.&][s1; [*C@3 k]-"
"|Key to find.&][s1; [*C@3 init]-|Value to add if key is not in AMap yet.&][s1; [*"
"/ Return value]-|Position of element (either found or added).&][s4;:`:`:AMap`:`:G"
"et`(const K`&`): T`& [* Get](const K`& [*@3 k])&][s6; Retrieves a reference value"
" of the first element with the specified key. If multi-key ordering is not broken"
" and more than one element with the same value exists in AMap, lowest position el"
"ement is retrieved. Unlinked elements are ignored. Required key must be in AMap, "
"otherwise it is logic error (asserted in debug mode).&][s1; [*C@3 k]-|Key to find"
".&][s1; [*/ Return value]-|Reference to corresponding value.&][s4;:`:`:AMap`:`:Ge"
"t`(const K`&`)const: const T`& [* Get](const K`& [*@3 k])")
TOPIC_TEXT(
" const&][s6; Retrieves a constant reference value of the first element with the "
"specified key. If multi-key ordering is not broken and more than one element with"
" the same value exists in AMap, lowest position element is retrieved. Unlinked el"
"ements are ignored. Required key must be in AMap, otherwise it is logic error (as"
"serted in debug mode).&][s1; [*C@3 k]-|Key to find.&][s1; [*/ Return value]-|Refe"
"rence to corresponding value.&][s4;:`:`:AMap`:`:Get`(const K`&`,const T`&`)const:"
" const T`& [* Get](const K`& [*@3 k], const T`& [*@3 d]) const&][s6; Retrieves a "
"constant reference value of the first element with the specified key. If multi-ke"
"y ordering is not broken and more than one element with the same value exists in "
"AMap, lowest position element is retrieved. Unlinked elements are ignored. If req"
"uired key is not in the AMap, constant reference to the specified value is return"
"ed instead.&][s1; [*C@3 k]-|Key to find.&][s1; [*C@3 d]-|Value to be returned if "
"key is not found.&][s1; [*/ Return value]-|Reference to found or supplied value.&"
"][s4;:`:`:AMap`:`:GetAdd`(const K`&`): T`& [* GetAdd](const K`& [*@3 k])&][s6; Re"
"trieves a constant reference value of the first element with the specified key. I"
"f multi-key ordering is not broken and more than one element with the same value "
"exists in AMap, lowest position element is retrieved. Unlinked elements are ignor"
"ed. If required key is not in the AMap, adds new default constructed element at t"
"he end of AMap and returns reference to it.&][s11; T must have default constructo"
"r.&][s11; Invalidates iterators to AMap.&][s11; Invalidates references to keys.&]"
"[s11; Invalidates references to VectorMap values.&][s1; [*C@3 k]-|Key to find.&]["
"s1; [*/ Return value]-|Reference to corresponding value.&][s4;:`:`:AMap`:`:GetAdd"
"`(const K`&`,const T`&`): T`& [* GetAdd](const K`& [*@3 k], const T`& [*@3 x])&]["
"s6; Retrieves a constant reference value of the first element with the specified "
"key. If multi-key ordering is not broken and more than on")
TOPIC_TEXT(
"e element with the same value exists in AMap, lowest position element is retriev"
"ed. Unlinked elements are ignored. If required key is not in the AMap, adds new e"
"lement, deep copy constructed from [* x],[*  ]at the end of AMap and returns refe"
"rence to it.&][s11; T must have deep copy constructor.&][s11; Invalidates iterato"
"rs to AMap.&][s11; Invalidates references to keys.&][s11; Invalidates references "
"to VectorMap values.&][s1; [*C@3 k]-|Key to find.&][s1; [*C@3 x]-|Value to add if"
" key is not in AMap.&][s1; [*/ Return value]-|Reference to corresponding value.&]"
"[s4;:`:`:AMap`:`:GetAddPick`(const K`&`,pick`_ T`&`):~~~.992; T`& [* GetAddPick]("
"const K`& [*@3 k], pick`_ T`& [*@3 x])&][s6; Retrieves a constant reference value"
" of the first element with the specified key. If multi-key ordering is not broken"
" and more than one element with the same value exists in AMap, lowest position el"
"ement is retrieved. Unlinked elements are ignored. If required key is not in the "
"AMap, adds new element, pick constructed from [* x],[*  ]at the end of AMap and r"
"eturns reference to it.&][s11; T must have pick constructor.&][s11; Invalidates i"
"terators to AMap.&][s11; Invalidates references to keys.&][s11; Invalidates refer"
"ences to VectorMap values.&][s1; [*C@3 k]-|Key to find.&][s1; [*C@3 x]-|Value to "
"add if key is not in AMap.&][s1; [*/ Return value]-|Reference to corresponding va"
"lue.&][s4;:`:`:AMap`:`:GetPut`(const K`&`): T`& [* GetPut](const K`& [*@3 k])&][s"
"6; Retrieves a constant reference value of the first element with the specified k"
"ey. If multi-key ordering is not broken and more than one element with the same v"
"alue exists in AMap, lowest position element is retrieved. Unlinked elements are "
"ignored. If required key is not in the AMap, puts new default constructed element"
" into the AMap using [* Put] and returns reference to it.&][s11; T must have defa"
"ult constructor.&][s11; Invalidates iterators to AMap.&][s11; Invalidates referen"
"ces to keys.&][s11; Invalidates references to VectorMap v")
TOPIC_TEXT(
"alues.&][s1; [*C@3 k]-|Key to find.&][s1; [*/ Return value]-|Reference to corres"
"ponding value.&][s4;:`:`:AMap`:`:GetPut`(const K`&`,const T`&`): T`& [* GetPut](c"
"onst K`& [*@3 k], const T`& [*@3 x])&][s6; Retrieves a constant reference value o"
"f the first element with the specified key. If multi-key ordering is not broken a"
"nd more than one element with the same value exists in AMap, lowest position elem"
"ent is retrieved. Unlinked elements are ignored. If required key is not in the AM"
"ap, puts new element, deep copy constructed from [* x],[*  ]into the AMap using ["
"* Put] and returns reference to it.&][s11; T must have deep copy constructor.&][s"
"11; Invalidates iterators to AMap.&][s11; Invalidates references to keys.&][s11; "
"Invalidates references to VectorMap values.&][s1; [*C@3 k]-|Key to find.&][s1; [*"
"C@3 x]-|Value to add if key is not in AMap.&][s1; [*/ Return value]-|Reference to"
" corresponding value.&][s4;:`:`:AMap`:`:GetPutPick`(const K`&`,pick`_ T`&`): T`& "
"[* GetPutPick](const K`& [*@3 k], pick`_ T`& [*@3 x])&][s6; Retrieves a constant "
"reference value of the first element with the specified key. If multi-key orderin"
"g is not broken and more than one element with the same value exists in AMap, low"
"est position element is retrieved. Unlinked elements are ignored. If required key"
" is not in the AMap, puts new element, pick constructed from [* x], into the AMap"
" using [* Put] and returns reference to it.&][s11; T must have pick constructor.&"
"][s11; Invalidates iterators to AMap.&][s11; Invalidates references to keys.&][s1"
"1; Invalidates references to VectorMap values.&][s1; [*C@3 k]-|Key to find.&][s1;"
" [*C@3 x]-|Value to add if key is not in AMap.&][s1; [*/ Return value]-|Reference"
" to corresponding value.&][s4;:`:`:AMap`:`:SetKey`(int`,const K`&`): void [* SetK"
"ey](int [*@3 i], const K`& [*@3 k])&][s6; Replaces key of element at specified po"
"sition.&][s1; [*C@3 i]-|Position of element.&][s1; [*C@3 k]-|New key value.&][s4;"
":`:`:AMap`:`:FindPtr`(const K`&`): T *[* FindPtr](const K")
TOPIC_TEXT(
"`& [*@3 k])&][s6; Retrieves a pointer to value of first element with specified k"
"ey in AMap. If multi-key ordering is not broken and more than one element with th"
"e same value exists in AMap, lowest position is retrieved. If the element does no"
"t exist in AMap, NULL pointer is returned. Unlinked elements are ignored.&][s1; ["
"*C@3 k]-|Key to find.&][s1; [*/ Return value]-|Pointer to value or NULL pointer i"
"f element is not in AMap.&][s4;:`:`:AMap`:`:FindPtr`(const K`&`)const: const T *["
"* FindPtr](const K`& [*@3 k]) const&][s6; Retrieves a constant pointer to value o"
"f first element with specified key in AMap. If multi-key ordering is not broken a"
"nd more than one element with the same value exists in AMap, lowest position is r"
"etrieved. If the element does not exist in AMap, NULL pointer is returned. Unlink"
"ed elements are ignored.&][s1; [*C@3 k]-|Key to find.&][s1; [*/ Return value]-|Po"
"inter to value or NULL pointer if element is not in AMap.&][s4;:`:`:AMap`:`:opera"
"tor`(`)`(const K`&`): T`& [* operator]()(const K`& [*@3 k])&][s6; Operator versio"
"n of T`& [* Get](const K`& [*@3 k]).&][s1; [*C@3 k]-|Key to find.&][s1; [*/ Retur"
"n value]-|Reference to corresponding value.&][s4;:`:`:AMap`:`:operator`(`)`(const"
" K`&`)const: const T`& [* operator]()(const K`& [*@3 k]) const&][s6; Operator ver"
"sion of const T`& [* Get](const K`& [*@3 k], const T`& [*@3 d]) const.&][s1; [*C@"
"3 k]-|Key to find.&][s1; [*/ Return value]-|Reference to corresponding value.&][s"
"4;:`:`:AMap`:`:operator`(`)`(const K`&`,const T`&`)const: const T`& [* operator]("
")(const K`& [*@3 k], const T`& [*@3 d]) const&][s6; Operator version of const T`&"
" [* Get](const K`& [*@3 k], const T`& [*@3 d]) const.&][s1; [*C@3 k]-|Key to find"
".&][s1; [*C@3 d]-|Value to be returned if key is not found.&][s1; [*/ Return valu"
"e]-|Reference to corresponding value.&][s4;:`:`:AMap`:`:UnlinkKey`(const K`&`,uns"
"igned`): int [* UnlinkKey](const K`& [*@3 k], unsigned [*@3 h])&][s6; Unlinks all"
" elements with specified key using precomputed hash-value")
TOPIC_TEXT(
". Unlinked elements stay in AIndex but are ignored by any Find operations. Preco"
"mputed hash value must be same as hash value that would be result of HashFn. &][s"
"1; [*C@3 k]-|Key to unlink.&][s1; [*C@3 h]-|Precomputed hash value.&][s1; [*/ Ret"
"urn value]-|Number of elements unlinked.&][s4;:`:`:AMap`:`:UnlinkKey`(const K`&`)"
": int [* UnlinkKey](const K`& [*@3 k])&][s6; Unlinks all elements with specified "
"key. Unlinked elements stay in AIndex but are ignored by any Find operations.&][s"
"1; [*C@3 k]-|Key to unlink.&][s1; [*/ Return value]-|Number of elements unlinked."
"&][s4;:`:`:AMap`:`:IsUnlinked`(int`)const: bool [* IsUnlinked](int [*@3 i]) const"
"&][s6; Tests whether element at specified position is unlinked.&][s1; [*C@3 i]-|P"
"osition.&][s1; [*/ Return value]-|true if element is unlinked.&][s4;:`:`:AMap`:`:"
"Insert`(int`,const K`&`): T`& [* Insert](int [*@3 i], const K`& [*@3 k])&][s6; In"
"serts element with specified key and default constructed value at specified posit"
"ion. This is slow operation, especially when combined with any search operations."
"&][s11; Requires T to have default constructor.&][s11; Invalidates iterators to A"
"Map.&][s11; Invalidates references to keys.&][s11; Invalidates references to Vect"
"orMap values.&][s1; [*C@3 i]-|Insert position.&][s1; [*C@3 k]-|Key to insert.&][s"
"4;:`:`:AMap`:`:Insert`(int`,const K`&`,const T`&`): void [* Insert](int [*@3 i], "
"const K`& [*@3 k], const T`& [*@3 x])&][s6; Inserts element with specified key an"
"d value copy constructed from [*/ x] at specified position. This is slow operatio"
"n, especially when combined with any search operations.&][s11; Requires T to have"
" deep copy constructor.&][s11; Invalidates iterators to AMap.&][s11; Invalidates "
"references to keys.&][s11; Invalidates references to VectorMap values.&][s1; [*C@"
"3 i]-|Insert position.&][s1; [*C@3 k]-|Key to insert.&][s1; [*C@3 x]-|Value to in"
"sert.&][s4;:`:`:AMap`:`:Remove`(int`): void [* Remove](int [*@3 i])&][s6; Removes"
" element at specified position. This is slow operation, e")
TOPIC_TEXT(
"specially when combined with any search operations.&][s11; Invalidates iterators"
" to AMap.&][s11; Invalidates references to keys.&][s11; Invalidates references to"
" VectorMap values.&][s1; [*C@3 i]-|Position of element to remove.&][s4;:`:`:AMap`"
":`:Remove`(const int`*`,int`): void [* Remove](const int *[*@3 sl], int [*@3 n])&"
"][s6; Removes number of elements from AMap. Time of operation almost does not dep"
"end on number of elements. This is slow operation, especially when combined with "
"any search operations.&][s11; Invalidates iterators to AMap.&][s11; Invalidates r"
"eferences to keys.&][s11; Invalidates references to VectorMap values.&][s1; [*C@3"
" i]-|Position of element to remove.&][s1; [*C@3 sl]-|Pointer to array of position"
"s to remove. It must be sorted from lowest to greatest value.&][s1; [*C@3 n]-|Num"
"ber of elements to remove.Removes element at specified position. This is slow ope"
"ration, especially when combined with any search operations.&][s4;:`:`:AMap`:`:Re"
"move`(const Vector`<int`>`&`): void [* Remove](const Vector<int>`& [*@3 sl])&][s6"
"; Removes number of elements form AMap. Same as Remove(sorted`_list, sorted`_list"
".GetCount()).&][s11; Invalidates iterators to AMap.&][s11; Invalidates references"
" to keys.&][s11; Invalidates references to VectorMap values.&][s1; [*C@3 sl]-|Sor"
"ted Vector of positions to remove.&][s4;:`:`:AMap`:`:RemoveKey`(const K`&`): int "
"[* RemoveKey](const K`& [*@3 k])&][s6; Removes all elements with specified value."
" This is slow operation, especially when combined with any search operations.&][s"
"1; [*C@3 k]-|Key to remove.&][s4;:`:`:AMap`:`:operator`[`]`(int`)const: const T`&"
" [* operator]`[`](int [*@3 i]) const&][s6; Returns constant reference of element "
"at specified position.&][s1; [*C@3 i]-|Position.&][s1; [*/ Return value]-|Constan"
"t reference to element at specified position.&][s4;:`:`:AMap`:`:operator`[`]`(int"
"`): T`& [* operator]`[`](int [*@3 i])&][s6; Returns reference of element at speci"
"fied position.&][s1; [*C@3 i]-|Position.&][s1; [*/ Return")
TOPIC_TEXT(
" value]-|Constant reference to element at specified position.&][s4;:`:`:AMap`:`:"
"GetCount`(`)const: int [* GetCount]() const&][s6; Returns number of elements in A"
"Map.&][s1; [*/ Return value]-|Actual number of elements.&][s4;:`:`:AMap`:`:IsEmpt"
"y`(`)const: bool [* IsEmpty]() const&][s6; Tests whether AMap is empty. Same as G"
"etCount() == 0.&][s1; [*/ Return value]-|true if AMap is empty, false otherwise.&"
"][s4;:`:`:AMap`:`:Clear`(`): void [* Clear]()&][s6; Removes all elements from AMa"
"p.&][s4;:`:`:AMap`:`:Shrink`(`): void [* Shrink]()&][s6; Minimizes memory consump"
"tion of AMap by decreasing capacity to number of elements.&][s4;:`:`:AMap`:`:Rese"
"rve`(int`): void [* Reserve](int [*@3 xtra])&][s6; Reserves capacity. If required"
" capacity is greater than current capacity, capacity is increased to the required"
" value.&][s1; [*C@3 n]-|Required capacity.&][s4;:`:`:AMap`:`:GetAlloc`(`)const: i"
"nt [* GetAlloc]() const&][s6; Returns current capacity of Array.&][s1; [*/ Return"
" value]-|Capacity of Array.&][s4;:`:`:AMap`:`:Drop`(int`): void [* Drop](int [*@3"
" n] = 1)&][s6; Drops the specified number of elements at the end of the AMap.&][s"
"1; [*C@3 n]-|Number of elements.&][s4;:`:`:AMap`:`:Top`(`): T`& [* Top]()&][s6; R"
"eturns reference to the value of the last element of AMap.&][s1; [*/ Return value"
"]-|Reference to the value of the last element.&][s4;:`:`:AMap`:`:Top`(`)const: co"
"nst T`& [* Top]() const&][s6; Returns constant reference to the value of the last"
" element of AMap.&][s1; [*/ Return value]-|Reference to the value of the last ele"
"ment.&][s4;:`:`:AMap`:`:TopKey`(`)const: const K`& [* TopKey]() const&][s6; Retur"
"ns constant reference to the key of the last element of AMap.&][s1; [*/ Return va"
"lue]-|Reference to the key of the last element.&][s4;:`:`:AMap`:`:PopKey`(`): K ["
"* PopKey]()&][s6; Drops last element of AMap and returns the key of the dropped e"
"lement.&][s1; [*/ Return value]-|Key of the element dropped at the end of AMap.&]"
"[s4;:`:`:AMap`:`:GetKey`(int`)const: const K`& [* GetKey]")
TOPIC_TEXT(
"(int [*@3 i]) const&][s6; Returns constant reference to the key of element at sp"
"ecified position.&][s1; [*C@3 i]-|Position.&][s1; [*/ Return value]-|Constant ref"
"erence to the key.&][s4;:`:`:AMap`:`:Serialize`(Stream`&`): void [* Serialize](St"
"ream`& [*@3 s])&][s6; Serializes content of AMap to/from Stream. Works only if NT"
"L is used as part of UPP.&][s11; Requires T to have serialization operator define"
"d.&][s1; [*C@3 s]-|Target/source stream.&][s4;:`:`:AMap`:`:GetIndex`(`)const: con"
"st Index<K>`& [* GetIndex]() const&][s6; Returns a constant reference to the inte"
"rnal Index of keys.&][s1; [*/ Return value]-|Constant reference to the Index of k"
"eys.&][s4;:`:`:AMap`:`:PickIndex`(`): Index<K> [* PickIndex]()&][s6; Returns Inde"
"x of keys. Destroys AMap by picking.&][s1; [*/ Return value]-|Index of keys.&][s4"
";:`:`:AMap`:`:GetKeys`(`)const: const Vector<K>`& [* GetKeys]() const&][s6; Retur"
"ns a constant reference to the Vector of keys.&][s1; [*/ Return value]-|Constant "
"reference to the Vector of keys.&][s4;:`:`:AMap`:`:PickKeys`(`): Vector<K> [* Pic"
"kKeys]()&][s6; Returns Vector of keys. Destroys AMap by picking.&][s1; [*/ Return"
" value]-|Vector of keys.&][s4;:`:`:AMap`:`:GetValues`(`)const: const V`& [* GetVa"
"lues]() const&][s6; Returns a constant reference to the basic random access conta"
"iner of values. Destroys AIndex by picking.&][s1; [*/ Return value]-|Constant ref"
"erence to the basic random access container of values.&][s4;:`:`:AMap`:`:PickValu"
"es`(`): V [* PickValues]()&][s6; Returns basic random access container of values."
" Destroys AIndex by picking.&][s1; [*/ Return value]-|Basic random access contain"
"er of values.&][s4;:`:`:AMap`:`:AMap`(`): [* AMap]()&][s6; Constructor. Construct"
"s an empty AMap.&][s4;:`:`:AMap`:`:AMap`(const AMap`&`,int`): [* AMap](const [* A"
"Map]`& [*@3 s], int)&][s6; Optional deep copy constructor.&][s11; Requires T to h"
"ave deep copy constructor or optional deep copy constructor.&][s1; [*C s]-|Source"
" AMap.&][s4;:`:`:AMap`:`:AMap`(pick`_ Index`<K`>`&`,pick`")
TOPIC_TEXT(
"_ V`&`): [* AMap](pick`_ Index<K>`& [*@3 ndx], pick`_ V`& [*@3 val])&][s6; This "
"form of constructors creates AMap by picking Index of keys and basic random acces"
"s container of values. Both containers must have same number of elements.&][s1; ["
"*C@3 ndx]-|Keys.&][s1; [*C@3 val]-|Values.&][s4;:`:`:AMap`:`:AMap`(pick`_ Vector`"
"<K`>`&`,pick`_ V`&`): [* AMap](pick`_ Vector<K>`& [*@3 ndx], pick`_ V`& [*@3 val]"
")&][s6; This form of constructors creates AMap by picking Vector of keys and basi"
"c random access container of values. Both containers must have same number of ele"
"ments.&][s1; [*C@3 ndx]-|Keys.&][s1; [*C@3 val]-|Values.&][s4;:`:`:AMap`:`:typede"
"f K KeyType:* [* typedef ]K[*  ]KeyType&][s6; Typedef of K for use in templated a"
"lgorithms.&][s4;:`:`:AMap`:`:typedef typename Index`<K`>`:`:ConstIterator KeyCons"
"tIterator:* [* typedef ][*/ type][*  ]KeyConstIterator&][s6; Key iterator type.&]"
"[s4;:`:`:AMap`:`:KeyBegin`(`)const: KeyConstIterator [* KeyBegin]() const&][s6; R"
"eturns constant iterator to the first key in AMap.&][s1; [*/ Return value]-|Const"
"ant key iterator.&][s4;:`:`:AMap`:`:KeyEnd`(`)const: KeyConstIterator [* KeyEnd]("
") const&][s6; Returns constant iterator to the key just beyond the last key in AM"
"ap.&][s1; [*/ Return value]-|Constant key iterator.&][s4;:`:`:AMap`:`:KeyGetIter`"
"(int`)const: KeyConstIterator [* KeyGetIter](int [*@3 pos]) const&][s6; Returns c"
"onstant iterator to the key at specified position. Same as [* KeyBegin() + i]. Be"
"nefit of this methods is that in debug mode [* pos] is range checked.&][s1; [*C@3"
" i]-|Required position.&][s1; [*/ Return value]-|Constant key iterator.&][s4;:`:`"
":AMap`:`:typedef T ValueType:* [* typedef T ]ValueType&][s6; Typedef of T for use"
" in templated algorithms.&][s4;:`:`:AMap`:`:typedef typename V`:`:ConstIterator C"
"onstIterator:* [* typedef ][*/ type][*  ]ConstIterator&][s6; Constant value itera"
"tor type.&][s4;:`:`:AMap`:`:typedef typename V`:`:Iterator Iterator:* [* typedef "
"][*/ type][*  ]Iterator&][s6; Value iterator type.&][s4;:")
TOPIC_TEXT(
"`:`:AMap`:`:Begin`(`): Iterator [* Begin]()&][s6; Returns iterator to the first "
"value in AMap.&][s1; [*/ Return value]-|Value iterator.&][s4;:`:`:AMap`:`:End`(`)"
": Iterator [* End]()&][s6; Returns constant iterator to the value just beyond the"
" last key in AMap.&][s1; [*/ Return value]-|Value iterator.&][s4;:`:`:AMap`:`:Get"
"Iter`(int`): Iterator [* GetIter](int [*@3 pos])&][s6; Returns iterator to the va"
"lue at specified position. Same as [* Begin() + i]. Benefit of this methods is th"
"at in debug mode [* pos] is range checked.&][s1; [*C@3 i]-|Required position.&][s"
"1; [*/ Return value]-|Value iterator.&][s4;:`:`:AMap`:`:Begin`(`)const: ConstIter"
"ator [* Begin]() const&][s6; Returns constant iterator to the first value in AMap"
".&][s1; [*/ Return value]-|Constant value iterator.&][s4;:`:`:AMap`:`:End`(`)cons"
"t: ConstIterator [* End]() const&][s6; Returns constant iterator to the value jus"
"t beyond the last value in AMap.&][s1; [*/ Return value]-|Constant value iterator"
".&][s4;:`:`:AMap`:`:GetIter`(int`)const: ConstIterator [* GetIter](int [*@3 pos])"
" const&][s6; Returns constant iterator to the value at specified position. Same a"
"s [* Begin() + i]. Benefit of this methods is that in debug mode [* pos] is range"
" checked.&][s1; [*C@3 i]-|Required position.&][s1; [*/ Return value]-|Constant va"
"lue iterator.&][s4;:friend int GetCount`(const AMap`& v`): friend void [* Swap](A"
"Map`& [*@3 a], AMap`& [*@3 b])&][s6; Specialization of generic [* Swap] for AMap."
" Swaps array in simple constant time operation.&][s1; [*C@3 a]-|First AMap to swa"
"p.&][s1; [*C@3 b]-|Second AMap to swap.")
