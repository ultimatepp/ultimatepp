topic "Sorting template functions";
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
[{_} 
[ {{10000@(113.42.0) [s0;%% [*@7;4 Sorting template functions]]}}&]
[s3; &]
[s5;:Sort`(T`&`,const Less`&`): [@(0.0.255) template]_<[@(0.0.255) class]_[*@4 T], 
[@(0.0.255) class]_[*@4 Less][@(0.0.255) >]_[@(0.0.255) void]_[* Sort]([*@4 T][@(0.0.255) `&]_[*@3 c
], [@(0.0.255) const]_[*@4 Less][@(0.0.255) `&]_[*@3 less])&]
[s2;%% Sorts container [%-*@3 c] with ordering is defined by [%-*@3 less]. 
The order of elements with the same value can be changed (unstable 
sort).&]
[s6;%% IterSwap must be defined for IteratorOf<T>.&]
[s3; &]
[s4; &]
[s5;:Sort`(T`&`): [@(0.0.255) template]_<[@(0.0.255) class]_[*@4 T][@(0.0.255) >]_[@(0.0.255) v
oid]_[* Sort]([*@4 T][@(0.0.255) `&]_[*@3 c])&]
[s2;%% Sorts container [%-*@3 c] with ordering is determined by [*/ operator<]. 
The order of elements with the same value can be changed (unstable 
sort).&]
[s6;%% IterSwap must be defined for IteratorOf<T>.&]
[s3;%% &]
[s4; &]
[s5;:StableSort`(T`&`,const Less`&`): [@(0.0.255) template]_<[@(0.0.255) class]_[*@4 T], 
[@(0.0.255) class]_[*@4 Less]>_[@(0.0.255) void]_[* StableSort]([*@4 T][@(0.0.255) `&]_[*@3 c],
 [@(0.0.255) const]_[*@4 Less][@(0.0.255) `&]_[*@3 less])&]
[s2;%% Sorts container [%-*@3 c] with ordering is defined by [%-*@3 less]. 
The order of elements with the same value stays unchanged (stable 
sort).&]
[s6;%% IterSwap must be defined for IteratorOf<T>.&]
[s3; &]
[s4; &]
[s5;:StableSort`(T`&`): [@(0.0.255) template]_<[@(0.0.255) class]_[*@4 T]>_[@(0.0.255) void]_
[* StableSort]([*@4 T][@(0.0.255) `&]_[*@3 c])&]
[s2;%% Sorts container [%-*@3 c] with ordering is determined by [*/ operator<]. 
 The order of elements with the same value stays unchanged (stable 
sort).&]
[s6;%% IterSwap must be defined for IteratorOf<T>.&]
[s3;%% &]
[s4; &]
[s5;:IndexSort`(KC`&`,VC`&`,const Less`&`): [@(0.0.255) template]_<[@(0.0.255) class]_[*@4 K
C], [@(0.0.255) class]_[*@4 VC], [@(0.0.255) class]_[*@4 Less][@(0.0.255) >]_[@(0.0.255) void
]_[* IndexSort]([*@4 KC][@(0.0.255) `&]_[*@3 keys], [*@4 VC][@(0.0.255) `&]_[*@3 values], 
[@(0.0.255) const]_[*@4 Less][@(0.0.255) `&]_[*@3 less])&]
[s2;%% Sorts pair of containers. Both containers must have same number 
of items. Resulting order is defined by the [* keys] container. 
Ordering is defined by [* less].&]
[s3; &]
[s4; &]
[s5;:IndexSort`(KC`&`,VC`&`): [@(0.0.255) template]_<[@(0.0.255) class]_[*@4 KC], 
[@(0.0.255) class]_[*@4 VC][@(0.0.255) >]_[@(0.0.255) void]_[* IndexSort]([*@4 KC][@(0.0.255) `&
]_[*@3 keys], [*@4 VC][@(0.0.255) `&]_[*@3 values])&]
[s2;%% Sorts pair of containers. Both containers must have same number 
of items. Resulting order is determined by the [* keys] container. 
Ordering is determined by [*/ operator<].&]
[s3; &]
[s4; &]
[s5;:GetSortOrder`(const C`&`,const Less`&`): [@(0.0.255) template]_<[@(0.0.255) class]_[*@4 C
], [@(0.0.255) class]_[*@4 Less][@(0.0.255) >]_Vector[@(0.0.255) <int>]_[* GetSortOrder]([@(0.0.255) c
onst]_[*@4 C][@(0.0.255) `&]_[*@3 container], [@(0.0.255) const]_[*@4 Less][@(0.0.255) `&]_[*@3 l
ess])&]
[s2;%% Creates ascending order of values in [%-*@3 container]. Ordering 
is determined by [%-*@3 less]. The order of elements with the same 
value can be changed (unstable sort).&]
[s3; &]
[s4; &]
[s5;:GetSortOrder`(const C`&`): [@(0.0.255) template]_<[@(0.0.255) class]_[*@4 C][@(0.0.255) >
]_Vector[@(0.0.255) <int>]_[* GetSortOrder]([@(0.0.255) const]_[*@4 C][@(0.0.255) `&]_[*@3 co
ntainer])&]
[s2;%% Creates ascending order of values in [%-*@3 container]. Ordering 
is determined by [*/ operator<]. The order of elements with the 
same value can be changed (unstable sort).&]
[s7;%% &]
[s3;%% &]
[s4; &]
[s5;:GetStableSortOrder`(const C`&`,const Less`&`): [@(0.0.255) template]_<[@(0.0.255) cl
ass]_[*@4 C], [@(0.0.255) class]_[*@4 Less]>_[_^Vector^ Vector]<[@(0.0.255) int]>_[* GetStabl
eSortOrder]([@(0.0.255) const]_[*@4 C][@(0.0.255) `&]_[*@3 container], 
[@(0.0.255) const]_[*@4 Less][@(0.0.255) `&]_[*@3 less])&]
[s2;%% Creates ascending order of values in [%-*@3 container]. Ordering 
is determined by [%-*@3 less]. The order of elements with the same 
value stays unchanged (stable sort).&]
[s3;%% &]
[s4; &]
[s5;:GetStableSortOrder`(const C`&`): [@(0.0.255) template]_<[@(0.0.255) class]_[*@4 C]>_[_^Vector^ V
ector]<[@(0.0.255) int]>_[* GetStableSortOrder]([@(0.0.255) const]_[*@4 C][@(0.0.255) `&]_[*@3 c
ontainer])&]
[s2;%% Creates ascending order of values in [%-*@3 container]. Ordering 
is determined by [*/ operator<]. The order of elements with the 
same value stays unchanged (stable sort).&]
[s3;%% &]
[s4; &]
[s5;:SortByKey`(Map`&`,const Less`&`): [@(0.0.255) template]_<[@(0.0.255) class]_[*@4 Map],
 [@(0.0.255) class]_[*@4 Less]>_[@(0.0.255) void]_[* SortByKey]([*@4 Map][@(0.0.255) `&]_[*@3 m
ap], [@(0.0.255) const]_[*@4 Less][@(0.0.255) `&]_[*@3 less])&]
[s2;%% Sorts VectorMap or ArrayMap (or any other hypothetical container 
that supports required interfaces) [%-*@3 map] by keys, using [%-*@3 less] 
as sorting predicate.&]
[s3;%% &]
[s4; &]
[s5;:SortByKey`(Map`&`): [@(0.0.255) template]_<[@(0.0.255) class]_[*@4 Map]>_[@(0.0.255) voi
d]_[* SortByKey]([*@4 Map][@(0.0.255) `&]_[*@3 map])&]
[s2;%% Sorts VectorMap or ArrayMap (or any other hypothetical container 
that supports required interfaces) [%-*@3 map] by keys, using operator< 
as sorting predicate.&]
[s3;%% &]
[s4; &]
[s5;:SortByValue`(Map`&`,const Less`&`): [@(0.0.255) template]_<[@(0.0.255) class]_[*@4 Map
], [@(0.0.255) class]_[*@4 Less]>_[@(0.0.255) void]_[* SortByValue]([*@4 Map][@(0.0.255) `&]_
[*@3 map], [@(0.0.255) const]_[*@4 Less][@(0.0.255) `&]_[*@3 less])&]
[s2;%% Sorts VectorMap or ArrayMap (or any other hypothetical container 
that supports required interfaces) [%-*@3 map] by values, using 
[%-*@3 less] as sorting predicate.&]
[s3;%% &]
[s4; &]
[s5;:SortByValue`(Map`&`): [@(0.0.255) template]_<[@(0.0.255) class]_[*@4 Map]>_[@(0.0.255) v
oid]_[* SortByValue]([*@4 Map][@(0.0.255) `&]_[*@3 map])&]
[s2;%% Sorts VectorMap or ArrayMap (or any other hypothetical container 
that supports required interfaces) [%-*@3 map] by values, using 
operator< as sorting predicate.&]
[s3;%% &]
[s4; &]
[s5;:StableSortByKey`(Map`&`,const Less`&`): [@(0.0.255) template]_<[@(0.0.255) class]_[*@4 M
ap], [@(0.0.255) class]_[*@4 Less]>_[@(0.0.255) void]_[* StableSortByKey]([*@4 Map][@(0.0.255) `&
]_[*@3 map], [@(0.0.255) const]_[*@4 Less][@(0.0.255) `&]_[*@3 less])&]
[s2;%% Sorts VectorMap or ArrayMap (or any other hypothetical container 
that supports required interfaces) [%-*@3 map] by keys, using [%-*@3 less] 
as sorting predicate. Stable: retains the order of equal elements.&]
[s3;%% &]
[s4; &]
[s5;:StableSortByKey`(Map`&`): [@(0.0.255) template]_<[@(0.0.255) class]_[*@4 Map]>_[@(0.0.255) v
oid]_[* StableSortByKey]([*@4 Map][@(0.0.255) `&]_[*@3 map])&]
[s2;%% Sorts VectorMap or ArrayMap (or any other hypothetical container 
that supports required interfaces) [%-*@3 map] by keys, using operator< 
as sorting predicate. Stable: retains the order of equal elements.&]
[s3;%% &]
[s4; &]
[s5;:StableSortByValue`(Map`&`,const Less`&`): [@(0.0.255) template]_<[@(0.0.255) class]_
[*@4 Map], [@(0.0.255) class]_[*@4 Less]>_[@(0.0.255) void]_[* StableSortByValue]([*@4 Map][@(0.0.255) `&
]_[*@3 map], [@(0.0.255) const]_[*@4 Less][@(0.0.255) `&]_[*@3 less])&]
[s2;%% Sorts VectorMap or ArrayMap (or any other hypothetical container 
that supports required interfaces) [%-*@3 map] by values, using 
[%-*@3 less] as sorting predicate. Stable: retains the order of 
equal elements.&]
[s3;%% &]
[s4; &]
[s5;:StableSortByValue`(Map`&`): [@(0.0.255) template]_<[@(0.0.255) class]_[*@4 Map]>_[@(0.0.255) v
oid]_[* StableSortByValue]([*@4 Map][@(0.0.255) `&]_[*@3 map])&]
[s2;%% Sorts VectorMap or ArrayMap (or any other hypothetical container 
that supports required interfaces) [%-*@3 map] by values, using 
operator< as sorting predicate. Stable: retains the order of equal 
elements.&]
[s3;%% &]
[s4; &]
[s5;:SortIndex`(Index`&`,const Less`&`): [@(0.0.255) template]_<[@(0.0.255) class]_[*@4 Ind
ex], [@(0.0.255) class]_[*@4 Less]>_[@(0.0.255) void]_[* SortIndex]([*@4 Index][@(0.0.255) `&
]_[*@3 index], [@(0.0.255) const]_[*@4 Less][@(0.0.255) `&]_[*@3 less])&]
[s2;%% Sorts Index or ArrayIndex using [%-*@3 less] as sorting predicate.&]
[s3;%% &]
[s4; &]
[s5;:SortIndex`(Index`&`): [@(0.0.255) template]_<[@(0.0.255) class]_[*@4 Index]>_[@(0.0.255) v
oid]_[* SortIndex]([*@4 Index][@(0.0.255) `&]_[*@3 index])&]
[s2;%% Sorts Index or ArrayIndex.&]
[s3;%% &]
[s4; &]
[s5;:StableSortIndex`(Index`&`,const Less`&`): [@(0.0.255) template]_<[@(0.0.255) class]_
[*@4 Index], [@(0.0.255) class]_[*@4 Less]>_[@(0.0.255) void]_[* StableSortIndex]([*@4 Index][@(0.0.255) `&
]_[*@3 index], [@(0.0.255) const]_[*@4 Less][@(0.0.255) `&]_[*@3 less])&]
[s2;%% Sorts Index or ArrayIndex using [%-*@3 less] as sorting predicate. 
Stable: retains the order of equal elements.&]
[s3;%% &]
[s4; &]
[s5;:StableSortIndex`(Index`&`): [@(0.0.255) template]_<[@(0.0.255) class]_[*@4 Index]>_[@(0.0.255) v
oid]_[* StableSortIndex]([*@4 Index][@(0.0.255) `&]_[*@3 index])&]
[s2;%% Sorts Index or ArrayIndex.  Stable: retains the order of equal 
elements.&]
[s3;%% ]]