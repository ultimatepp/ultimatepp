TITLE("AIndex class template")
REF("::AIndex::template <class T, class V, class HashFn> class AIndex")
REF("::AIndex::Add(const T&,unsigned)")
REF("::AIndex::Add(const T&)")
REF("::AIndex::Find(const T&,unsigned)const")
REF("::AIndex::Find(const T&)const")
REF("::AIndex::FindNext(int)const")
REF("::AIndex::FindLast(const T&,unsigned)const")
REF("::AIndex::FindLast(const T&)const")
REF("::AIndex::FindPrev(int)const")
REF("::AIndex::FindAdd(const T&,unsigned)")
REF("::AIndex::FindAdd(const T&)")
REF("::AIndex::operator<<(const T&)")
REF("::AIndex::Unlink(int)")
REF("::AIndex::Put(const T&,unsigned)")
REF("::AIndex::Put(const T&)")
REF("::AIndex::FindPut(const T&,unsigned)")
REF("::AIndex::FindPut(const T&)")
REF("::AIndex::Set(int,const T&,unsigned)")
REF("::AIndex::Set(int,const T&)")
REF("::AIndex::operator[](int)const")
REF("::AIndex::GetCount()const")
REF("::AIndex::IsEmpty()const")
REF("::AIndex::Clear()")
REF("::AIndex::ClearIndex()const")
REF("::AIndex::UnlinkKey(const T&,unsigned)")
REF("::AIndex::UnlinkKey(const T&)")
REF("::AIndex::IsUnlinked(int)const")
REF("::AIndex::Sweep()")
REF("::AIndex::Insert(int,const T&,unsigned)")
REF("::AIndex::Insert(int,const T&)")
REF("::AIndex::Remove(int)")
REF("::AIndex::Remove(const int*,int)")
REF("::AIndex::Remove(const Vector<int>&)")
REF("::AIndex::RemoveKey(const T&,unsigned)")
REF("::AIndex::RemoveKey(const T&)")
REF("::AIndex::Trim(int)")
REF("::AIndex::Drop(int)")
REF("::AIndex::Top()const")
REF("::AIndex::Reserve(int)")
REF("::AIndex::Shrink()")
REF("::AIndex::GetAlloc()const")
REF("::AIndex::Serialize(Stream&)")
REF("::AIndex::PickKeys()")
REF("::AIndex::GetKeys()const")
REF("::AIndex::operator=(pick_ V&)")
REF("::AIndex::operator<<=(const V&)")
REF("::AIndex::AIndex(pick_ V&)")
REF("::AIndex::AIndex(const V&,int)")
REF("::AIndex::typedef T ValueType")
REF("::AIndex::typedef typename V::ConstIterator ConstIterator")
REF("::AIndex::Begin()const")
REF("::AIndex::End()const")
REF("::AIndex::GetIter(int)const")
REF("::AIndex::AIndex()")
REF("::AIndex::AIndex(const AIndex&,int)")
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
"5:package`-title][{_}%EN-US [s10;:`:`:AIndex`:`:template `<class T`, class V`, cl"
"ass HashFn`> class AIndex:t292; template <class [*@4 T], class [*@4 V], class [*@"
"4 HashFn]>&][s10;t292;* [* class ]AIndex&][s0; &][s12; [*C@4 T]-|Type of elements"
" to store. T must satisfy requirements for container flavor identified by paramet"
"er V and must have [*C operator==] defined.&][s12; [*C@4 V]-|Basic random access "
"container.&][s12; [*C@4 HashFn]-|Hashing class. Must have defined [*C unsigned op"
"erator()(const T`& x)] method returning hash value for elements.&][s5; This templ"
"ate class adds associative capabilities to basic random access containers, formin"
"g flavors of Index. It is used as base class for concrete index flavors, [* Index"
"] and [* ArrayIndex].&][s5; It allows adding elements at the end of sequence in c"
"onstant amortized time like basic random container. Addit")
TOPIC_TEXT(
"ionally, it also allows fast retrieval of a position of the element with specifi"
"ed value. Hashing is used for this operation. AIndex stores hash-values of elemen"
"ts, so it has no sense to cache them externally.&][s5; Building of internal hash "
"maps of AIndex is always deferred till search operation. This effectively avoids "
"unneeded remapping if large number of elements is added.&][s5; Interesting proble"
"m of AIndex causes removing of elements. While it is possible to simply remove (a"
"nd even insert) element at specified position, such operation has to move a lot o"
"f elements and also scratches internal hash maps. That is why removing elements t"
"his way from index is slow, especially when combined with searching.&][s5; Soluti"
"on of this problem is [*/ unlinking] of elements. Unlinked elements are not remov"
"ed from index, but they are [*/ ignored][/  ]by search operations. Unlinking is s"
"imple low constant time operation. Further, it is possible to place element at fi"
"rst available unlinked position (rather that to the end of sequence) using [* Put"
"] method, reusing unlinked position at low constant time.&][s5; Only problem of u"
"nlinking is breaking of so called [* multi-key ordering]. This term means that if"
" there are more elements with the same value in the index and they are iterated t"
"hrough using FindNext method, their positions (as get as result of Find and subse"
"quent FindNext methods) are in ascending order. Problem is that it is impossible "
"to implement placing elements at unlinked positions in low time while preserving "
"this ordering. On the other hand, usage scenarios for indexes show that need for "
"unlinking elements and multi-key ordering is almost always disjunct. For the rest"
" of cases, it is always possible to restore ordering by [* Reindex] or [* Sweep] "
"methods.&][s5; Like any other NTL container, AIndex is [*/^dpp`:`/`/SourceDoc`/Co"
"ntainers`/Moveable^ moveable][*/  ]type with [*/^dpp`:`/`/SourceDoc`/Containers`/"
"pick`_^ pick and optional deep copy] transfer semantics, ")
TOPIC_TEXT(
"although these features are more important in derived concrete index flavors.&]["
"s3; Members&][s4;:`:`:AIndex`:`:Add`(const T`&`,unsigned`): void [* Add](const T`"
"& [*@3 x], unsigned [*@3 `_hash])&][s6; Adds a new element with precomputed hash "
"value. Precomputed hash value must be same as hash value that would be result of "
"HashFn. Benefit of this variant is that sometimes you can compute hash-value as t"
"he part of other process, like fetching string from input stream.&][s11; Requires"
" T to have deep copy constructor.&][s11; Invalidates iterators to AIndex.&][s11; "
"Invalidates references to Index.&][s1; [*C@3 x]-|Element to add.&][s1; [*C@3 `_ha"
"sh]-|Precomputed hash value.&][s4;:`:`:AIndex`:`:Add`(const T`&`): void [* Add](c"
"onst T`& [*@3 x])&][s6; Adds a new element to AIndex.&][s11; Requires T to have d"
"eep copy constructor.&][s11; Invalidates iterators to AIndex.&][s11; Invalidates "
"references to Index.&][s1; [*C@3 x]-|Element to add.&][s4;:`:`:AIndex`:`:Find`(co"
"nst T`&`,unsigned`)const: int [* Find](const T`& [*@3 x], unsigned [*@3 `_hash]) "
"const&][s6; Retrieves position of first element with specified value in AIndex, u"
"sing precomputed hash value. Precomputed hash value must be same as hash value th"
"at would be result of HashFn. If multi-key ordering is not broken and more than o"
"ne element with the same value exists in AIndex, lowest position is retrieved. If"
" element does not exist in AIndex, negative number is returned. Unlinked elements"
" are ignored.&][s1; [*C@3 x]-|Element to find.&][s1; [*C@3 `_hash]-|Precomputed h"
"ash value.&][s1; [*/ Return value]-|Position of element or negative value if elem"
"ent is not in AIndex.&][s4;:`:`:AIndex`:`:Find`(const T`&`)const: int [* Find](co"
"nst T`& [*@3 x]) const&][s6; Retrieves position of first element with specified v"
"alue in AIndex. If multi-key ordering is not broken and more than one element wit"
"h the same value exists in AIndex, lowest position is retrieved. If element does "
"not exist in AIndex, negative number is returned. Unlinke")
TOPIC_TEXT(
"d elements are ignored.&][s1; [*C@3 x]-|Element to find.&][s1; [*/ Return value]"
"-|Position of element or negative value if element is not in AIndex.&][s4;:`:`:AI"
"ndex`:`:FindNext`(int`)const:~~~.992; int [* FindNext](int [*@3 i]) const&][s6; R"
"etrieves position of next element with the same value as element at specified pos"
"ition. If multi-key ordering is not broken and more than one element with that va"
"lue exists in AIndex, lowest position greater than specified one is retrieved (so"
" that positions got by subsequent calls to FindNext are in ascending order). When"
" there are no more elements with required value, negative number is returned. Unl"
"inked elements are ignored.&][s1; [*C@3 i]-|Position of element.&][s1; [*/ Return"
" value]-|Position of next element with same value.&][s4;:`:`:AIndex`:`:FindLast`("
"const T`&`,unsigned`)const: int [* FindLast](const T`& [*@3 x], unsigned [*@3 `_h"
"ash]) const&][s6; Retrieves position of last element with specified value in AInd"
"ex, using precomputed hash value. Precomputed hash value must be same as hash val"
"ue that would be result of HashFn. If multi-key ordering is not broken and more t"
"han one element with the same value exists in AIndex, greatest position is retrie"
"ved. If element does not exist in AIndex, negative number is returned. Unlinked e"
"lements are ignored.&][s1; [*C@3 x]-|Element to find.&][s1; [*C@3 `_hash]-|Precom"
"puted hash value.&][s1; [*/ Return value]-|Position of element or negative value "
"if element is not in AIndex.&][s4;:`:`:AIndex`:`:FindLast`(const T`&`)const: int "
"[* FindLast](const T`& [*@3 x]) const&][s6; Retrieves position of last element wi"
"th specified value in AIndex. If multi-key ordering is not broken and more than o"
"ne element with the same value exists in AIndex, greatest position is retrieved. "
"If element does not exist in AIndex, negative number is returned. Unlinked elemen"
"ts are ignored.&][s1; [*C@3 x]-|Element to find.&][s1; [*C@3 `_hash]-|Precomputed"
" hash value.&][s1; [*/ Return value]-|Position of element")
TOPIC_TEXT(
" or negative value if element is not in AIndex.&][s4;:`:`:AIndex`:`:FindPrev`(in"
"t`)const: int [* FindPrev](int [*@3 i]) const&][s6; Retrieves position of previou"
"s element with the same value as element at specified position. If multi-key orde"
"ring is not broken and more than one element with that value exists in AIndex, gr"
"eatest position lower than specified one is retrieved (so that positions got by s"
"ubsequent calls to FindNext are in descending order). When there are no more elem"
"ents with required value, negative number is returned. Unlinked elements are igno"
"red.&][s1; [*C@3 i]-|Position of element.&][s1; [*/ Return value]-|Position of pr"
"evious element with same value.&][s4;:`:`:AIndex`:`:FindAdd`(const T`&`,unsigned`"
"): int [* FindAdd](const T`& [*@3 key], unsigned [*@3 `_hash])&][s6; Retrieves po"
"sition of first element with specified value in AIndex, using precomputed hash va"
"lue. Precomputed hash value must be same as hash value that would be result of Ha"
"shFn. If multi-key ordering is not broken and more than one element with the same"
" value exists in AIndex, lowest position is retrieved. If element does not exist "
"in AIndex, it is added to AIndex and position of this newly added element is retu"
"rned. Unlinked elements are ignored.&][s1; [*C@3 key]-|Element to find or add.&]["
"s1; [*C@3 `_hash]-|Precomputed hash value.&][s1; [*/ Return value]-|Position of f"
"ound or added element.&][s4;:`:`:AIndex`:`:FindAdd`(const T`&`): int [* FindAdd]("
"const T`& [*@3 key])&][s6; Retrieves position of first element with specified val"
"ue in AIndex. If multi-key ordering is not broken and more than one element with "
"the same value exists in AIndex, lowest position is retrieved. If element does no"
"t exist in AIndex, it is added to AIndex and position of this newly added element"
" is returned. Unlinked elements are ignored.&][s1; [*C@3 key]-|Element to find or"
" add.&][s1; [*/ Return value]-|Position of found or added element.&][s4;:`:`:AInd"
"ex`:`:operator`<`<`(const T`&`): AIndex`& [* operator]<<(")
TOPIC_TEXT(
"const T`& [*@3 x])&][s6; Operator replacement of [* void Add(const T`& x)]. By r"
"eturning reference to AIndex allows adding more elements in single expression, th"
"us e.g. allowing to construct temporary Index as part of expression like Foo((Ind"
"ex<int>() << 1 << 2)).&][s11; Requires T to have deep copy constructor.&][s11; In"
"validates iterators to AIndex.&][s11; Invalidates references to Index.&][s1; [*C@"
"3 newt]-|Element to be added.&][s1; [*/ Return value]-|Reference to AIndex.&][s4;"
":`:`:AIndex`:`:Unlink`(int`): void [* Unlink](int [*@3 i])&][s6; Unlinks element "
"at specified position. Unlinked item stays in AIndex but is ignored by any Find o"
"peration.&][s1; [*C@3 i]-|Position of item to unlink.&][s4;:`:`:AIndex`:`:Put`(co"
"nst T`&`,unsigned`):~~~.992; int [* Put](const T`& [*@3 x], unsigned [*@3 `_hash]"
")&][s6; If there is any unlinked element in AIndex, it is replaced by specified v"
"alue. If there is none unlinked element, element with specified value is added at"
" the end of AIndex using [* Add]. Precomputed hash, same as result of HashFn, is "
"used. Position of placed element is returned.&][s11; Invalidates multi-key orderi"
"ng.&][s11; Requires T to have deep copy constructor.&][s11; Invalidates iterators"
" to AIndex.&][s11; Invalidates references to Index.&][s1; [*C@3 x]-|Element to pu"
"t into AIndex.&][s1; [*C@3 `_hash]-|Precomputed hash value.&][s1; [*/ Return valu"
"e]-|Position where element is placed.&][s4;:`:`:AIndex`:`:Put`(const T`&`): int ["
"* Put](const T`& [*@3 x])&][s6; If there is any unlinked element in AIndex, it is"
" replaced by specified value. If there is none unlinked element, element with spe"
"cified value is added at the end of AIndex using [* Add]. Position of placed elem"
"ent is returned.&][s11; Invalidates multi-key ordering.&][s11; Requires T to have"
" deep copy constructor.&][s11; Invalidates iterators to AIndex.&][s1; [*C@3 x]-|E"
"lement to put into AIndex.&][s1; [*/ Return value]-|Position where element is pla"
"ced.&][s4;:`:`:AIndex`:`:FindPut`(const T`&`,unsigned`): ")
TOPIC_TEXT(
"int [* FindPut](const T`& [*@3 key], unsigned [*@3 `_hash])&][s6; Retrieves posi"
"tion of first element with specified value in AIndex, using precomputed hash valu"
"e. Precomputed hash value must be same as hash value that would be result of Hash"
"Fn. If element does not exist in the AIndex, it is placed to it using [* Put(cons"
"t T`& x, unsigned `_hash).] Position of found or placed element is returned.&][s1"
"1;~~~.992; Invalidates multi-key ordering.&][s11; Requires T to have deep copy co"
"nstructor.&][s11; Invalidates iterators to AIndex.&][s11; Invalidates references "
"to Index.&][s1; [*C@3 key]-|Element to find or put.&][s1; [*C@3 `_hash]-|Precompu"
"ted hash value.&][s1; [*/ Return value]-|Position of found or placed element.&][s"
"4;:`:`:AIndex`:`:FindPut`(const T`&`): int [* FindPut](const T`& [*@3 key])&][s6;"
" Retrieves position of first element with specified value in AIndex.  If element "
"does not exist in the AIndex, it is placed to it using [* Put(const T`& x).] Posi"
"tion of found or placed element is returned.&][s11; Invalidates multi-key orderin"
"g.&][s11; Requires T to have deep copy constructor.&][s11; Invalidates iterators "
"to AIndex.&][s11; Invalidates references to Index.&][s1; [*C@3 key]-|Element to f"
"ind or put.&][s1; [*C@3 `_hash]-|Precomputed hash value.&][s1; [*/ Return value]-"
"|Position of found or placed element.&][s4;:`:`:AIndex`:`:Set`(int`,const T`&`,un"
"signed`): void [* Set](int [*@3 i], const T`& [*@3 x], unsigned [*@3 `_hash])&][s"
"6; Replaces element at specified position with new element with specified value, "
"using precomputed hash-value. Speed of operation depends on total number of eleme"
"nts with the same as specified value in AIndex.&][s11; Requires T to have deep co"
"py constructor.&][s11; Invalidates iterators to AIndex.&][s11; Invalidates refere"
"nces to Index.&][s1; [*C@3 i]-|Position of element.&][s1; [*C@3 x]-|Value to set."
"&][s1; [*C@3 `_hash]-|Precomputed hash value.&][s4;:`:`:AIndex`:`:Set`(int`,const"
" T`&`): void [* Set](int [*@3 i], const T`& [*@3 x])&][s6")
TOPIC_TEXT(
"; Replaces element at specified position with new element with specified value. "
"Speed of operation depends on total number of elements with the same as specified"
" value in AIndex.&][s11; Requires T to have deep copy constructor.&][s11; Invalid"
"ates iterators to AIndex.&][s11; Invalidates references to Index.&][s1; [*C@3 i]-"
"|Position of element.&][s1; [*C@3 x]-|Value to set.&][s4;:`:`:AIndex`:`:operator`"
"[`]`(int`)const: const T`& [* operator]`[`](int [*@3 i]) const&][s6; Returns the "
"element at the specified position.&][s1; [*C@3 i]-|Position of the element.&][s1;"
" [*/ Return value]-|Constant reference to element.&][s4;:`:`:AIndex`:`:GetCount`("
"`)const: int [* GetCount]() const&][s6; Returns number of elements in AIndex.&][s"
"1; [*/ Return value]-|Actual number of elements.&][s4;:`:`:AIndex`:`:IsEmpty`(`)c"
"onst: bool [* IsEmpty]() const&][s6; Tests whether AIndex is empty. Same as GetCo"
"unt() == 0.&][s1; [*/ Return value]-|true if AIndex is empty, false otherwise.&]["
"s4;:`:`:AIndex`:`:Clear`(`): void [* Clear]()&][s6; Removes all elements from AIn"
"dex.&][s4;:`:`:AIndex`:`:ClearIndex`(`)const: void [* Reindex]() const&][s6; Rest"
"ores multi-key ordering.&][s4;:`:`:AIndex`:`:UnlinkKey`(const T`&`,unsigned`): in"
"t [* UnlinkKey](const T`& [*@3 k], unsigned [*@3 h])&][s6; Unlinks all elements w"
"ith specified value using precomputed hash-value. Unlinked elements stay in AInde"
"x but are ignored by any Find operations. Precomputed hash value must be same as "
"hash value that would be result of HashFn. &][s1; [*C@3 k]-|Value of elements to "
"unlink.&][s1; [*C@3 h]-|Precomputed hash value.&][s1; [*/ Return value]-|Number o"
"f elements unlinked.&][s4;:`:`:AIndex`:`:UnlinkKey`(const T`&`): int [* UnlinkKey"
"](const T`& [*@3 k])&][s6; Unlinks all elements with specified value. Unlinked el"
"ements stay in AIndex but are ignored by any Find operations.&][s1; [*C@3 k]-|Val"
"ue of elements to unlink.&][s1; [*/ Return value]-|Number of elements unlinked.&]"
"[s4;:`:`:AIndex`:`:IsUnlinked`(int`)const: bool [* IsUnli")
TOPIC_TEXT(
"nked](int [*@3 i]) const&][s6; Tests whether element at specified position is un"
"linked.&][s1; [*C@3 i]-|Position.&][s1; [*/ Return value]-|true if element is unl"
"inked.&][s4;:`:`:AIndex`:`:Sweep`(`): void [* Sweep]()&][s6; Removes all unlinked"
" elements from AIndex. Complexity of operation depends on number of elements in A"
"Index, not on number of unlinked elements. Also restores multi-key ordering.&][s4"
";:`:`:AIndex`:`:Insert`(int`,const T`&`,unsigned`): void [* Insert](int [*@3 i], "
"const T`& [*@3 k], unsigned [*@3 h])&][s6; Inserts element with specified value o"
"n specified position, using precomputed hash value. Precomputed hash value must b"
"e same as hash value that would be result of HashFn. This is slow operation, espe"
"cially when combined with any search operations.&][s11; Requires T to have deep c"
"opy constructor.&][s11; Invalidates iterators to AIndex.&][s11; Invalidates refer"
"ences to Index.&][s1; [*C@3 i]-|Insert position.&][s1; [*C@3 k]-|Element to inser"
"t.&][s1; [*C@3 h]-|Precomputed hash value.&][s4;:`:`:AIndex`:`:Insert`(int`,const"
" T`&`): void [* Insert](int [*@3 i], const T`& [*@3 k])&][s6; Inserts element wit"
"h specified value on specified position. This is slow operation, especially when "
"combined with any search operations.&][s11; Requires T to have deep copy construc"
"tor.&][s11; Invalidates iterators to AIndex.&][s11; Invalidates references to Ind"
"ex.&][s1; [*C@3 i]-|Insert position.&][s1; [*C@3 k]-|Element to insert.&][s4;:`:`"
":AIndex`:`:Remove`(int`): void [* Remove](int [*@3 i])&][s6; Removes element at s"
"pecified position. This is slow operation, especially when combined with any sear"
"ch operations.&][s11; Invalidates iterators to AIndex.&][s11; Invalidates referen"
"ces to Index.&][s1; [*C@3 i]-|Position of element to remove.&][s4;:`:`:AIndex`:`:"
"Remove`(const int`*`,int`): void [* Remove](const int *[*@3 sorted`_list], int [*"
"@3 count])&][s6; Removes number of elements from AIndex. Time of operation almost"
" does not depend on number of elements. This is slow oper")
TOPIC_TEXT(
"ation, especially when combined with any search operations.&][s11; Invalidates i"
"terators to AIndex.&][s11; Invalidates references to Index.&][s1; [*C@3 sorted`_l"
"ist]-|Pointer to array of positions to remove. It must be sorted from lowest to g"
"reatest value.&][s1; [*C@3 count]-|Number of elements to remove.&][s4;:`:`:AIndex"
"`:`:Remove`(const Vector`<int`>`&`): void [* Remove](const Vector<int>`& [*@3 sor"
"ted`_list])&][s6; Removes number of elements form AIndex. Same as Remove(sorted`_"
"list, sorted`_list.GetCount()).&][s11; Invalidates iterators to AIndex.&][s11; In"
"validates references to Index.&][s1; [*C@3 sorted`_list]-|Sorted Vector of positi"
"ons to remove.&][s4;:`:`:AIndex`:`:RemoveKey`(const T`&`,unsigned`): int [* Remov"
"eKey](const T`& [*@3 k], unsigned [*@3 h])&][s6; Removes all elements with specif"
"ied value using precomputed hash-value. Precomputed hash value must be same as ha"
"sh value that would be result of HashFn. This is slow operation, especially when "
"combined with any search operations.&][s1; [*C@3 k]-|Value of elements to remove."
"&][s1; [*C@3 h]-|Precomputed hash value.&][s4;:`:`:AIndex`:`:RemoveKey`(const T`&"
"`): int [* RemoveKey](const T`& [*@3 k])&][s6; Removes all elements with specifie"
"d value. This is slow operation, especially when combined with any search operati"
"ons.&][s1; [*C@3 k]-|Value of elements to remove.&][s4;:`:`:AIndex`:`:Trim`(int`)"
": void [* Trim](int [*@3 n])&][s6; Reduces number of elements in AIndex to specif"
"ied number. Required number must be less than or equal to actual number of elemen"
"ts in AIndex.&][s1; [*C@3 n]-|Required number of elements.&][s4;:`:`:AIndex`:`:Dr"
"op`(int`): void [* Drop](int [*@3 n] = 1)&][s6; Drops the specified number of ele"
"ments at the end of the AIndex (same as Trim(GetCount() - n)).&][s1; [*C@3 n]-|Nu"
"mber of elements.&][s4;:`:`:AIndex`:`:Top`(`)const: const [* T]`& [* Top]() const"
"&][s6; Returns reference to the last element in the AIndex.&][s1; [*/ Return valu"
"e]-|Reference of last element in the AIndex.&][s4;:`:`:AI")
TOPIC_TEXT(
"ndex`:`:Reserve`(int`): void [* Reserve](int [*@3 n])&][s6; Reserves capacity. I"
"f required capacity is greater than current capacity, capacity is increased to th"
"e required value.&][s1; [*C@3 n]-|Required capacity.&][s4;:`:`:AIndex`:`:Shrink`("
"`): void [* Shrink]()&][s6; Minimizes memory consumption of AIndex by decreasing "
"capacity to number of elements.&][s4;:`:`:AIndex`:`:GetAlloc`(`)const: int [* Get"
"Alloc]() const&][s6; Returns current capacity of AIndex.&][s1; [*/ Return value]-"
"|Capacity of AIndex.&][s4;:`:`:AIndex`:`:Serialize`(Stream`&`): void [* Serialize"
"](Stream`& [*@3 s])&][s6; Serializes content of AIndex to/from Stream. Works only"
" if NTL is used as part of UPP.&][s11; Requires T to have serialization operator "
"defined.&][s1; [*C@3 s]-|Target/source stream.&][s4;:`:`:AIndex`:`:PickKeys`(`): "
"V [* PickKeys]()&][s6; Returns basic random access container of elements. Destroy"
"s AIndex by picking.&][s1; [*/ Return value]-|Basic random access container of el"
"ements in AIndex.&][s4;:`:`:AIndex`:`:GetKeys`(`)const: const V`& [* GetKeys]() c"
"onst&][s6; Returns constant reference to basic random access container of element"
"s.&][s1; [*/ Return value]-|Constant references to basic random access container "
"of elements.&][s4;:`:`:AIndex`:`:operator`=`(pick`_ V`&`): AIndex`& [* operator]="
"(pick`_ V`& [*@3 s])&][s6; Assigns basic random access container to AIndex. Trans"
"fers source container in low constant time, but destroys it by picking.&][s1; [*C"
"@3 s]-|Source container.&][s4;:`:`:AIndex`:`:operator`<`<`=`(const V`&`): AIndex`"
"& [* operator]<<=(const V`& [*@3 s])&][s6; Assigns basic random access container "
"to AIndex while preserving its value.&][s11; Requires T to have deep copy constru"
"ctor or optional deep copy.&][s1; [*C@3 s]-|Source container.&][s4;:`:`:AIndex`:`"
":AIndex`(pick`_ V`&`): [* AIndex](pick`_ V`& [*@3 s])&][s6; Pick-constructs AInde"
"x from basic random access container. Transfers source container in low constant "
"time, but destroys it by picking.&][s1; [*C@3 s]-|Source ")
TOPIC_TEXT(
"basic random access container.&][s4;:`:`:AIndex`:`:AIndex`(const V`&`,int`): [* "
"AIndex](const V`& [*@3 s], int)&][s6; Deep-copy constructs AIndex from basic rand"
"om access container.&][s11; Requires T to have deep copy constructor or optional "
"deep copy constructor.&][s1; [*C@3 s]-|Source AIndex.&][s4;:`:`:AIndex`:`:typedef"
" T ValueType:~~~.992;* [* typedef T ]ValueType&][s6; Typedef of T for use in temp"
"lated algorithms.&][s4;:`:`:AIndex`:`:typedef T ValueType: typedef [/ type] [* It"
"erator];&][s6; Iterator type.&][s4;:`:`:AIndex`:`:typedef typename V`:`:ConstIter"
"ator ConstIterator: typedef [/ type] [* ConstIterator];&][s6; Constant iterator t"
"ype.&][s4;:`:`:AIndex`:`:Begin`(`)const: ConstIterator [* Begin]() const&][s6; Re"
"turns constant iterator to the first element in AIndex.&][s1; [*/ Return value]-|"
"Iterator.&][s4;:`:`:AIndex`:`:End`(`)const: ConstIterator [* End]() const&][s6; R"
"eturns constant iterator to the position just beyond the last element in AIndex.&"
"][s1; [*/ Return value]-|Iterator.&][s4;:`:`:AIndex`:`:GetIter`(int`)const: Const"
"Iterator [* GetIter](int [*@3 pos]) const&][s6; Returns constant iterator to the "
"element at specified position. Same as [* Begin() + i]. Benefit of this methods i"
"s that in debug mode [* pos] is range checked.&][s1; [*C@3 i]-|Required position."
"&][s1; [*/ Return value]-|Iterator.&][s4;:`:`:AIndex`:`:AIndex`(`): [* AIndex]()&"
"][s6; Constructor. Constructs an empty AIndex.&][s4;:`:`:AIndex`:`:AIndex`(const "
"AIndex`&`,int`): [* AIndex](const [* AIndex]`& [*@3 s], int)&][s6; Optional deep "
"copy constructor.&][s11; Requires T to have deep copy constructor or optional dee"
"p copy constructor.&][s1; [*C s]-|Source AIndex.&][s0; ")
