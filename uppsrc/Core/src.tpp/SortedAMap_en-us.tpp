topic "SortedAMap";
[2 $$0,0#00000000000000000000000000000000:Default]
[i448;a25;kKO9;2 $$1,0#37138531426314131252341829483380:class]
[l288;2 $$2,2#27521748481378242620020725143825:desc]
[0 $$3,0#96390100711032703541132217272105:end]
[H6;0 $$4,0#05600065144404261032431302351956:begin]
[i448;a25;kKO9;2 $$5,0#37138531426314131252341829483370:item]
[l288;a4;*@5;1 $$6,6#70004532496200323422659154056402:requirement]
[l288;i1121;b17;O9;~~~.1408;2 $$7,0#10431211400427159095818037425705:param]
[i448;b42;O9;2 $$8,8#61672508125594000341940100500538:tparam]
[b42;2 $$9,9#13035079074754324216151401829390:normal]
[{_}%EN-US 
[ {{10000@(113.42.0) [s0; [*@7;4 SortedAMap]]}}&]
[s0;3%- &]
[s1;:noref:%- [@(0.0.255)3 template][3 _<][@(0.0.255)3 class][3 _][*@4;3 K][3 , 
][@(0.0.255)3 class][3 _][*@4;3 T][3 , ][@(0.0.255)3 class][3 _][*@4;3 Less][3 , 
][@(0.0.255)3 class][3 _][*@4;3 Data][3 >]&]
[s1;:SortedAMap`:`:class:%- [@(0.0.255) class]_[* SortedAMap]_:_[@(0.0.255) private]_[*@3 Move
ableAndDeepCopyOption]<_[* SortedAMap]<[*@4 K], [*@4 T], [*@4 Less], 
[*@4 Data]>_>_&]
[s2; SortedAMap implements methods common to both SortedVectorMap 
and SortedArrayMap. Sorted map is composition of [^topic`:`/`/Core`/src`/SortedIndex`$en`-us^ S
ortedIndex] and [^topic`:`/`/Core`/src`/InVector`$en`-us^ InVector] 
or [^topic`:`/`/Core`/src`/InArray`$en`-us^ InArray]. It uses strict 
ordering of keys by predicate [%-*@4 Less] to binary search for 
element. Type of keys [%-*@4 K] is required to by [*/^topic`:`/`/Core`/srcdoc`/Moveable`$en`-us^ m
oveable][*/  ]type. Like any other NTL container, AMap is [*/^topic`:`/`/Core`/srcdoc`/Moveable`$en`-us^ m
oveable][*/  ]type with [*/^topic`:`/`/Core`/srcdoc`/Moveable`$en`-us^ pick 
and optional deep copy] transfer semantics, although these features 
are more important in derived concrete AMap flavors.&]
[s0;%- &]
[ {{10000F(128)G(128)@1 [s0; [* Public Method List]]}}&]
[s3;%- &]
[s5;:SortedAMap`:`:FindLowerBound`(const K`&`)const:%- [@(0.0.255) int]_[* FindLowerBound
]([@(0.0.255) const]_[*@4 K][@(0.0.255) `&]_[*@3 k])_[@(0.0.255) const]&]
[s2; Returns lower`-bound index of element with key [%-*@3 k].&]
[s3; &]
[s4; &]
[s5;:SortedAMap`:`:FindUpperBound`(const K`&`)const:%- [@(0.0.255) int]_[* FindUpperBound
]([@(0.0.255) const]_[*@4 K][@(0.0.255) `&]_[*@3 k])_[@(0.0.255) const]&]
[s2; Returns upper bound index of element with key [%-*@3 k] .&]
[s3; &]
[s4; &]
[s5;:SortedAMap`:`:Find`(const K`&`)const:%- [@(0.0.255) int]_[* Find]([@(0.0.255) const]_[*@4 K
][@(0.0.255) `&]_[*@3 k])_[@(0.0.255) const]&]
[s2; Returns the minimum index of element with key equal to [%-*@3 k] 
or negative number if not found.&]
[s3; &]
[s4; &]
[s5;:SortedAMap`:`:FindNext`(int`)const:%- [@(0.0.255) int]_[* FindNext]([@(0.0.255) int]_[*@3 i
])_[@(0.0.255) const]&]
[s2; If the key at [%-*@3 i] `+ 1 is equal to the key at [%-*@3 i] , 
returns [%-*@3 i] `+ 1, otherwise returns negative number.&]
[s3; &]
[s4; &]
[s5;:SortedAMap`:`:FindLast`(const K`&`)const:%- [@(0.0.255) int]_[* FindLast]([@(0.0.255) c
onst]_[*@4 K][@(0.0.255) `&]_[*@3 k])_[@(0.0.255) const]&]
[s2; Returns the meximum index of element with key equal to [%-*@3 k] 
or negative number if not found.&]
[s3; &]
[s4; &]
[s5;:SortedAMap`:`:FindPrev`(int`)const:%- [@(0.0.255) int]_[* FindPrev]([@(0.0.255) int]_[*@3 i
])_[@(0.0.255) const]&]
[s2; If the key at [%-*@3 i] `- 1 is equal to the element at [%-*@3 i] 
, returns [%-*@3 i] `- 1, otherwise returns negative number.&]
[s3; &]
[s4; &]
[s5;:SortedAMap`:`:Get`(const K`&`):%- [*@4 T][@(0.0.255) `&]_[* Get]([@(0.0.255) const]_[*@4 K
][@(0.0.255) `&]_[*@3 k])&]
[s5;:SortedAMap`:`:Get`(const K`&`)const:%- [@(0.0.255) const]_[*@4 T][@(0.0.255) `&]_[* Get](
[@(0.0.255) const]_[*@4 K][@(0.0.255) `&]_[*@3 k])_[@(0.0.255) const]&]
[s2; Returns the value of the first element with key [%-*@3 k]. If 
not found, behaviour is undefined.&]
[s3; &]
[s4; &]
[s5;:SortedAMap`:`:Get`(const K`&`,const T`&`)const:%- [@(0.0.255) const]_[*@4 T][@(0.0.255) `&
]_[* Get]([@(0.0.255) const]_[*@4 K][@(0.0.255) `&]_[*@3 k], [@(0.0.255) const]_[*@4 T][@(0.0.255) `&
]_[*@3 d])_[@(0.0.255) const]&]
[s2; Returns the value of the first element with key [%-*@3 k]. If 
not found, returns [%-*@3 d].&]
[s3; &]
[s4; &]
[s5;:SortedAMap`:`:FindPtr`(const K`&`):%- [*@4 T]_`*[* FindPtr]([@(0.0.255) const]_[*@4 K][@(0.0.255) `&
]_[*@3 k])&]
[s5;:SortedAMap`:`:FindPtr`(const K`&`)const:%- [@(0.0.255) const]_[*@4 T]_`*[* FindPtr]([@(0.0.255) c
onst]_[*@4 K][@(0.0.255) `&]_[*@3 k])_[@(0.0.255) const]&]
[s2; Returns to pointer to the value of the first element with key 
[%-*@3 k]. If not found, returns NULL.&]
[s3; &]
[s4; &]
[s5;:SortedAMap`:`:GetKey`(int`)const:%- [@(0.0.255) const]_[*@4 K][@(0.0.255) `&]_[* GetKey](
[@(0.0.255) int]_[*@3 i])_[@(0.0.255) const]&]
[s2; Returns the key of element at [%-*@3 i].&]
[s3;@(0.0.255)%- &]
[s4;@(0.0.255)%- &]
[s5;:SortedAMap`:`:operator`[`]`(int`)const:%- [@(0.0.255) const]_[*@4 T][@(0.0.255) `&]_[* o
perator`[`]]([@(0.0.255) int]_[*@3 i])_[@(0.0.255) const]&]
[s5;:SortedAMap`:`:operator`[`]`(int`):%- [*@4 T][@(0.0.255) `&]_[* operator`[`]]([@(0.0.255) i
nt]_[*@3 i])&]
[s2; Returns the value of element at [%-*@3 i].&]
[s3; &]
[s4; &]
[s5;:SortedAMap`:`:GetCount`(`)const:%- [@(0.0.255) int]_[* GetCount]()_[@(0.0.255) const]&]
[s2; Returns the number of elements.&]
[s3; &]
[s4; &]
[s5;:SortedAMap`:`:IsEmpty`(`)const:%- [@(0.0.255) bool]_[* IsEmpty]()_[@(0.0.255) const]&]
[s2; Same as GetCount() `=`= 0.&]
[s3; &]
[s4; &]
[s5;:SortedAMap`:`:Clear`(`):%- [@(0.0.255) void]_[* Clear]()&]
[s2; Removes all elements.&]
[s3; &]
[s4; &]
[s5;:SortedAMap`:`:Shrink`(`):%- [@(0.0.255) void]_[* Shrink]()&]
[s2; Minimizes the memory usage, dropping allocation reserves.&]
[s3; &]
[s4; &]
[s5;:SortedAMap`:`:Remove`(int`):%- [@(0.0.255) void]_[* Remove]([@(0.0.255) int]_[*@3 i])&]
[s2; Removes element at [%-*@3 i].&]
[s3; &]
[s4; &]
[s5;:SortedAMap`:`:Remove`(int`,int`):%- [@(0.0.255) void]_[* Remove]([@(0.0.255) int]_[*@3 i
], [@(0.0.255) int]_[*@3 count])&]
[s2; Removes [%-*@3 count] elements at [%-*@3 i].&]
[s3; &]
[s4; &]
[s5;:SortedAMap`:`:RemoveKey`(const K`&`):%- [@(0.0.255) int]_[* RemoveKey]([@(0.0.255) con
st]_[*@4 K][@(0.0.255) `&]_[*@3 k])&]
[s2; Removes all elements with key [%-*@3 k].&]
[s3; &]
[s4; &]
[s5;:SortedAMap`:`:Swap`(SortedAMap`&`):%- [@(0.0.255) void]_[* Swap]([_^SortedAMap^ Sorted
AMap][@(0.0.255) `&]_[*@3 x])&]
[s2; Swaps this SortedAMap with another one.&]
[s3; &]
[s4; &]
[s5;:SortedAMap`:`:IsPicked`(`)const:%- [@(0.0.255) bool]_[* IsPicked]()_[@(0.0.255) const]&]
[s2; Returns true if SortedAMap is picked.&]
[s3; &]
[s4; &]
[s5;:SortedAMap`:`:GetIndex`(`)const:%- [@(0.0.255) const]_[_^SortedIndex^ SortedIndex]<[*@4 K
]>`&_[* GetIndex]()_[@(0.0.255) const]&]
[s2; Returns a reference to the internal index of keys.&]
[s3; &]
[s4; &]
[s5;:SortedAMap`:`:GetKeys`(`)const:%- [@(0.0.255) const]_[_^InVector^ InVector]<[*@4 K]>`&
_[* GetKeys]()_[@(0.0.255) const]&]
[s2; Returns a reference to the internal InVector of keys.&]
[s3; &]
[s4; &]
[s5;:SortedAMap`:`:SortedAMap`(const SortedAMap`&`,int`):%- [* SortedAMap]([@(0.0.255) co
nst]_[* SortedAMap][@(0.0.255) `&]_[*@3 s], [@(0.0.255) int])&]
[s2; Deep copy constructor.&]
[s3; &]
[s4; &]
[s5;:SortedAMap`:`:KeyBegin`(`)const:%- [_^SortedAMap`:`:KeyConstIterator^ KeyConstIter
ator]_[* KeyBegin]()_[@(0.0.255) const]&]
[s5;:SortedAMap`:`:KeyEnd`(`)const:%- [_^SortedAMap`:`:KeyConstIterator^ KeyConstIterat
or]_[* KeyEnd]()_[@(0.0.255) const]&]
[s5;:SortedAMap`:`:KeyGetIter`(int`)const:%- [_^SortedAMap`:`:KeyConstIterator^ KeyCons
tIterator]_[* KeyGetIter]([@(0.0.255) int]_[*@3 pos])_[@(0.0.255) const]&]
[s2; Returns iterator to key at begin/end/[%-*@3 pos].&]
[s3; &]
[s4; &]
[s5;:SortedAMap`:`:Begin`(`):%- [_^SortedAMap`:`:Iterator^ Iterator]_[* Begin]()&]
[s5;:SortedAMap`:`:End`(`):%- [_^SortedAMap`:`:Iterator^ Iterator]_[* End]()&]
[s5;:SortedAMap`:`:GetIter`(int`):%- [_^SortedAMap`:`:Iterator^ Iterator]_[* GetIter]([@(0.0.255) i
nt]_[*@3 pos])&]
[s5;:SortedAMap`:`:Begin`(`)const:%- [_^SortedAMap`:`:ConstIterator^ ConstIterator]_[* Be
gin]()_[@(0.0.255) const]&]
[s5;:SortedAMap`:`:End`(`)const:%- [_^SortedAMap`:`:ConstIterator^ ConstIterator]_[* End](
)_[@(0.0.255) const]&]
[s5;:SortedAMap`:`:GetIter`(int`)const:%- [_^SortedAMap`:`:ConstIterator^ ConstIterator
]_[* GetIter]([@(0.0.255) int]_[*@3 pos])_[@(0.0.255) const]&]
[s2; Returns iterator to value at begin/end/[%-*@3 pos].&]
[s2; ]]