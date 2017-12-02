topic "SortedIndex";
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
[ {{10000@(113.42.0) [s0; [*@7;4 SortedIndex]]}}&]
[s0;3%- &]
[s1;:noref:%- [@(0.0.255)3 template][3 _<][@(0.0.255)3 class][3 _][*@4;3 T][3 , 
][@(0.0.255)3 class][3 _][*@4;3 Less][3 _`=_StdLess<][*@4;3 T][3 >_>]&]
[s1;:SortedIndex`:`:class:%- [@(0.0.255) class]_[* SortedIndex]_:_[@(0.0.255) private]_[*@3 Mo
veableAndDeepCopyOption]<_[* SortedIndex]<[*@4 T], [*@4 Less]>_>_&]
[s2; SortedIndex is a random access (vector`-like) container which 
keeps its element sorted at all times using [%-*@4 Less] predicate. 
SortedIndex is relatively simple adapter over [^topic`:`/`/Core`/src`/InVector`$en`-us^ I
nVector] `- use it as reference for performace aspects.&]
[s2; [%-*@4 T] is required to by [*/^topic`:`/`/Core`/srcdoc`/Moveable`$en`-us^ moveable][*/  
]type. Like any other NTL container, AIndex is [*/^topic`:`/`/Core`/srcdoc`/Moveable`$en`-us^ m
oveable][*/  ]type with [*/^topic`:`/`/Core`/srcdoc`/Moveable`$en`-us^ pick 
and optional deep copy] transfer semantics.&]
[s0;3%- &]
[ {{10000F(128)G(128)@1 [s0; [* Public Method List]]}}&]
[s3;%- &]
[s5;:SortedIndex`:`:Add`(const T`&`):%- [@(0.0.255) int]_[* Add]([@(0.0.255) const]_[*@4 T][@(0.0.255) `&
]_[*@3 x])&]
[s2; Inserts a new item [%-*@3 x] into upper`-bound position and returns 
its index.&]
[s3; &]
[s4; &]
[s5;:SortedIndex`:`:FindAdd`(const T`&`):%- [@(0.0.255) int]_[* FindAdd]([@(0.0.255) const]_
[*@4 T][@(0.0.255) `&]_[*@3 key])&]
[s2; If there is already [%-*@3 key] in SortedIndex, returns its index. 
If there is not, inserts it and returns its index.&]
[s3; &]
[s4; &]
[s5;:SortedIndex`:`:operator`<`<`(const T`&`):%- [_^SortedIndex^ SortedIndex][@(0.0.255) `&
]_[* operator<<]([@(0.0.255) const]_[*@4 T][@(0.0.255) `&]_[*@3 x])&]
[s2; Same as Add([%-*@3 x]).&]
[s3; &]
[s4; &]
[s5;:SortedIndex`:`:FindLowerBound`(const T`&`)const:%- [@(0.0.255) int]_[* FindLowerBoun
d]([@(0.0.255) const]_[*@4 T][@(0.0.255) `&]_[*@3 x])_[@(0.0.255) const]&]
[s2; Finds lower`-bound index of [%-*@3 x].&]
[s3; &]
[s4; &]
[s5;:SortedIndex`:`:FindUpperBound`(const T`&`)const:%- [@(0.0.255) int]_[* FindUpperBoun
d]([@(0.0.255) const]_[*@4 T][@(0.0.255) `&]_[*@3 x])_[@(0.0.255) const]&]
[s2; Finds upper`-bound index of [%-*@3 x].&]
[s3; &]
[s4; &]
[s5;:SortedIndex`:`:Find`(const T`&`)const:%- [@(0.0.255) int]_[* Find]([@(0.0.255) const]_
[*@4 T][@(0.0.255) `&]_[*@3 x])_[@(0.0.255) const]&]
[s2; Returns the minimum index of element equal to [%-*@3 x] or negative 
number if not found.&]
[s3; &]
[s4; &]
[s5;:SortedIndex`:`:FindNext`(int`)const:%- [@(0.0.255) int]_[* FindNext]([@(0.0.255) int]_
[*@3 i])_[@(0.0.255) const]&]
[s2; If the element at [%-*@3 i] `+ 1 is equal to the element at [%-*@3 i] 
, returns [%-*@3 i] `+ 1, otherwise returns negative number.&]
[s3; &]
[s4; &]
[s5;:SortedIndex`:`:FindLast`(const T`&`)const:%- [@(0.0.255) int]_[* FindLast]([@(0.0.255) c
onst]_[*@4 T][@(0.0.255) `&]_[*@3 x])_[@(0.0.255) const]&]
[s2; Finds the maximum index equal to [%-*@3 x] or negative number 
if there is none.&]
[s3; &]
[s4; &]
[s5;:SortedIndex`:`:FindPrev`(int`)const:%- [@(0.0.255) int]_[* FindPrev]([@(0.0.255) int]_
[*@3 i])_[@(0.0.255) const]&]
[s2; If the element at [%-*@3 i] `- 1 is equal to the element at [%-*@3 i] 
, returns [%-*@3 i] `- 1, otherwise returns negative number.&]
[s3; &]
[s4; &]
[s5;:SortedIndex`:`:Remove`(int`):%- [@(0.0.255) void]_[* Remove]([@(0.0.255) int]_[*@3 i])&]
[s2; Removes the element at index [%-*@3 i].&]
[s3; &]
[s4; &]
[s5;:SortedIndex`:`:Remove`(int`,int`):%- [@(0.0.255) void]_[* Remove]([@(0.0.255) int]_[*@3 i
], [@(0.0.255) int]_[*@3 count])&]
[s2; Removes the [%-*@3 count]  elements starting at index [%-*@3 i].&]
[s3; &]
[s4; &]
[s5;:SortedIndex`:`:RemoveKey`(const T`&`):%- [@(0.0.255) int]_[* RemoveKey]([@(0.0.255) co
nst]_[*@4 T][@(0.0.255) `&]_[*@3 x])&]
[s2; Removes all elements equal to [%-*@3 x].&]
[s3; &]
[s4; &]
[s5;:SortedIndex`:`:operator`[`]`(int`)const:%- [@(0.0.255) const]_[*@4 T][@(0.0.255) `&]_[* o
perator`[`]]([@(0.0.255) int]_[*@3 i])_[@(0.0.255) const]&]
[s2; Returns element at index [%-*@3 i].&]
[s3; &]
[s4; &]
[s5;:SortedIndex`:`:GetCount`(`)const:%- [@(0.0.255) int]_[* GetCount]()_[@(0.0.255) const]&]
[s2; Returns the cound of elements.&]
[s3; &]
[s4; &]
[s5;:SortedIndex`:`:IsEmpty`(`)const:%- [@(0.0.255) bool]_[* IsEmpty]()_[@(0.0.255) const]&]
[s2; Same as GetCount() `=`= 0.&]
[s3; &]
[s4; &]
[s5;:SortedIndex`:`:Clear`(`):%- [@(0.0.255) void]_[* Clear]()&]
[s2; Removes all elements.&]
[s3; &]
[s4; &]
[s5;:SortedIndex`:`:Trim`(int`):%- [@(0.0.255) void]_[* Trim]([@(0.0.255) int]_[*@3 n])&]
[s2; Removes all elements starting at index [%-*@3 n].&]
[s3; &]
[s4; &]
[s5;:SortedIndex`:`:Drop`(int`):%- [@(0.0.255) void]_[* Drop]([@(0.0.255) int]_[*@3 n]_`=_[@3 1
])&]
[s2; Removes the last [%-*@3 n] elements.&]
[s3; &]
[s4; &]
[s5;:SortedIndex`:`:Top`(`)const:%- [@(0.0.255) const]_[*@4 T][@(0.0.255) `&]_[* Top]()_[@(0.0.255) c
onst]&]
[s2; Returns the last element.&]
[s3; &]
[s4; &]
[s5;:SortedIndex`:`:Shrink`(`):%- [@(0.0.255) void]_[* Shrink]()&]
[s2; Drops allocation reserve (reduces memory footprint).&]
[s3; &]
[s4; &]
[s5;:SortedIndex`:`:Begin`(`)const:%- [_^SortedIndex`:`:ConstIterator^ ConstIterator]_[* B
egin]()_[@(0.0.255) const]&]
[s2; Returns iterator at the begin of SortedIndex.&]
[s3; &]
[s4; &]
[s5;:SortedIndex`:`:End`(`)const:%- [_^SortedIndex`:`:ConstIterator^ ConstIterator]_[* En
d]()_[@(0.0.255) const]&]
[s2; Returns iterator at the end of SortedIndex.&]
[s3; &]
[s4; &]
[s5;:SortedIndex`:`:GetIter`(int`)const:%- [_^SortedIndex`:`:ConstIterator^ ConstIterat
or]_[* GetIter]([@(0.0.255) int]_[*@3 pos])_[@(0.0.255) const]&]
[s2; Returns iterator at the index [%-*@3 pos] .&]
[s3; &]
[s4;%- &]
[s5;:SortedIndex`:`:GetKeys`(`)const:%- [@(0.0.255) const]_[_^InVector^ InVector]<[*@4 T]>`&
_[* GetKeys]()_[@(0.0.255) const]&]
[s2; Retruns a reference to InVector of keys.&]
[s3; &]
[s4; &]
[s5;:SortedIndex`:`:SortedIndex`(const SortedIndex`&`,int`):%- [* SortedIndex]([@(0.0.255) c
onst]_[* SortedIndex][@(0.0.255) `&]_[*@3 s], [@(0.0.255) int])&]
[s2; Deep copy constructor.&]
[s3; &]
[s4; &]
[s5;:SortedIndex`:`:Swap`(SortedIndex`&`):%- [@(0.0.255) void]_[* Swap]([_^SortedIndex^ Sor
tedIndex][@(0.0.255) `&]_[*@3 a])&]
[s2; Swaps contents of two SortedIndex containers.&]
[s0; ]]