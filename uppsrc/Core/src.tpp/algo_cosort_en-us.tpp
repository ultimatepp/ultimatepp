topic "Parallel Sorting Template Functions";
[i448;a25;kKO9;2 $$1,0#37138531426314131252341829483380:class]
[l288;2 $$2,2#27521748481378242620020725143825:desc]
[0 $$3,0#96390100711032703541132217272105:end]
[H6;0 $$4,0#05600065144404261032431302351956:begin]
[i448;a25;kKO9;2 $$5,0#37138531426314131252341829483370:item]
[l288;a4;*@5;1 $$6,6#70004532496200323422659154056402:requirement]
[l288;i1121;b17;O9;~~~.1408;2 $$7,0#10431211400427159095818037425705:param]
[i448;b42;O9;2 $$8,8#61672508125594000341940100500538:tparam]
[b42;2 $$9,9#13035079074754324216151401829390:normal]
[2 $$0,0#00000000000000000000000000000000:Default]
[{_} 
[ {{10000@(113.42.0) [s0;%% [*@7;4 Parallel Sorting Template Functions]]}}&]
[s3; &]
[s5;:Upp`:`:CoSort`(Range`&`&`,const Less`&`): [@(0.0.255) template]_<[@(0.0.255) class]_
[*@4 Range], [@(0.0.255) class]_[*@4 Less]>&]
[s5;:Upp`:`:CoSort`(Range`&`&`,const Less`&`): [@(0.0.255) void]_[* CoSort]([*@4 Range][@(0.0.255) `&
`&]_[*@3 c], [@(0.0.255) const]_[*@4 Less][@(0.0.255) `&]_[*@3 less])&]
[s2;%% Sorts range [%-@3 c] with ordering is defined by [%-*@3 less]. 
The order of elements with the same value can be changed (unstable 
sort).&]
[s3; &]
[s4; &]
[s5;:Upp`:`:CoSort`(Range`&`&`): [@(0.0.255) template]_<[@(0.0.255) class]_[*@4 Range]>&]
[s5;:Upp`:`:CoSort`(Range`&`&`): [@(0.0.255) void]_[* CoSort]([*@4 Range][@(0.0.255) `&`&]_[*@3 c
])&]
[s2;%% Sorts range [%-@3 c] with ordering is defined by [*/ operator<]. 
The order of elements with the same value can be changed (unstable 
sort).&]
[s3;%% &]
[s4; &]
[s5;:Upp`:`:CoStableSort`(Range`&`&`,const Less`&`): [@(0.0.255) template]_<[@(0.0.255) c
lass]_[*@4 Range], [@(0.0.255) class]_[*@4 Less]>&]
[s5;:Upp`:`:CoStableSort`(Range`&`&`,const Less`&`): [@(0.0.255) void]_[* CoStableSort]([*@4 R
ange][@(0.0.255) `&`&]_[*@3 r], [@(0.0.255) const]_[*@4 Less][@(0.0.255) `&]_[*@3 less])&]
[s2;%% Sorts container [%-@3 c] with ordering is defined by [%-*@3 less]. 
The order of elements with the same value stays unchanged (stable 
sort).&]
[s6;%% &]
[s4; &]
[s5;:Upp`:`:CoStableSort`(Range`&`&`): [@(0.0.255) template]_<[@(0.0.255) class]_[*@4 Range
]>&]
[s5;:Upp`:`:CoStableSort`(Range`&`&`): [@(0.0.255) void]_[* CoStableSort]([*@4 Range][@(0.0.255) `&
`&]_[*@3 r])&]
[s2;%% Sorts container [%-@3 c] with ordering is defined by [*/ operator<]. 
The order of elements with the same value stays unchanged (stable 
sort).&]
[s6;%% &]
[s4; &]
[s5;:Upp`:`:CoIndexSort`(MasterRange`&`&`,Range2`&`&`,const Less`&`): [@(0.0.255) templ
ate]_<[@(0.0.255) class]_[*@4 MasterRange], [@(0.0.255) class]_[*@4 Range2], 
[@(0.0.255) class]_[*@4 Less]>&]
[s5;:Upp`:`:CoIndexSort`(MasterRange`&`&`,Range2`&`&`,const Less`&`): [@(0.0.255) void]_
[* CoIndexSort]([*@4 MasterRange][@(0.0.255) `&`&]_[*@3 r], [*@4 Range2][@(0.0.255) `&`&]_[*@3 r
2], [@(0.0.255) const]_[*@4 Less][@(0.0.255) `&]_[*@3 less])&]
[s5;:Upp`:`:CoIndexSort2`(MasterRange`&`&`,Range2`&`&`,Range3`&`&`,const Less`&`): [@(0.0.255) t
emplate]_<[@(0.0.255) class]_[*@4 MasterRange], [@(0.0.255) class]_[*@4 Range2], 
[@(0.0.255) class]_[*@4 Range3], [@(0.0.255) class]_[*@4 Less]>&]
[s5;:Upp`:`:CoIndexSort2`(MasterRange`&`&`,Range2`&`&`,Range3`&`&`,const Less`&`): [@(0.0.255) v
oid]_[* CoIndexSort2]([*@4 MasterRange][@(0.0.255) `&`&]_[*@3 r], [*@4 Range2][@(0.0.255) `&`&
]_[*@3 r2], [*@4 Range3][@(0.0.255) `&`&]_[*@3 r3], [@(0.0.255) const]_[*@4 Less][@(0.0.255) `&
]_[*@3 less])&]
[s5;:Upp`:`:CoIndexSort3`(MasterRange`&`&`,Range2`&`&`,Range3`&`&`,Range4`&`&`,const Less`&`): [@(0.0.255) t
emplate]_<[@(0.0.255) class]_[*@4 MasterRange], [@(0.0.255) class]_[*@4 Range2], 
[@(0.0.255) class]_[*@4 Range3], [@(0.0.255) class]_[*@4 Range4], [@(0.0.255) class]_[*@4 Les
s]>&]
[s5;:Upp`:`:CoIndexSort3`(MasterRange`&`&`,Range2`&`&`,Range3`&`&`,Range4`&`&`,const Less`&`): [@(0.0.255) v
oid]_[* CoIndexSort3]([*@4 MasterRange][@(0.0.255) `&`&]_[*@3 r], [*@4 Range2][@(0.0.255) `&`&
]_[*@3 r2], [*@4 Range3][@(0.0.255) `&`&]_[*@3 r3], [*@4 Range4][@(0.0.255) `&`&]_[*@3 r4], 
[@(0.0.255) const]_[*@4 Less][@(0.0.255) `&]_[*@3 less])&]
[s2;%% Sorts 2, 3 or 4 containers. All containers must have same 
number of items. Resulting order is defined by the [%-*@3 r ]container. 
Ordering is defined by [* less].&]
[s3;%% &]
[s4; &]
[s5;:Upp`:`:CoIndexSort`(MasterRange`&`&`,Range2`&`&`): [@(0.0.255) template]_<[@(0.0.255) c
lass]_[*@4 MasterRange], [@(0.0.255) class]_[*@4 Range2]>&]
[s5;:Upp`:`:CoIndexSort`(MasterRange`&`&`,Range2`&`&`): [@(0.0.255) void]_[* CoIndexSort](
[*@4 MasterRange][@(0.0.255) `&`&]_[*@3 r], [*@4 Range2][@(0.0.255) `&`&]_[*@3 r2])&]
[s5;:Upp`:`:CoIndexSort2`(MasterRange`&`&`,Range2`&`&`,Range3`&`&`): [@(0.0.255) templa
te]_<[@(0.0.255) class]_[*@4 MasterRange], [@(0.0.255) class]_[*@4 Range2], 
[@(0.0.255) class]_[*@4 Range3]>&]
[s5;:Upp`:`:CoIndexSort2`(MasterRange`&`&`,Range2`&`&`,Range3`&`&`): [@(0.0.255) void]_
[* CoIndexSort2]([*@4 MasterRange][@(0.0.255) `&`&]_[*@3 r], [*@4 Range2][@(0.0.255) `&`&]_[*@3 r
2], [*@4 Range3][@(0.0.255) `&`&]_[*@3 r3])&]
[s5;:Upp`:`:CoIndexSort3`(MasterRange`&`&`,Range2`&`&`,Range3`&`&`,Range4`&`&`): [@(0.0.255) t
emplate]_<[@(0.0.255) class]_[*@4 MasterRange], [@(0.0.255) class]_[*@4 Range2], 
[@(0.0.255) class]_[*@4 Range3], [@(0.0.255) class]_[*@4 Range4]>&]
[s5;:Upp`:`:CoIndexSort3`(MasterRange`&`&`,Range2`&`&`,Range3`&`&`,Range4`&`&`): [@(0.0.255) v
oid]_[* CoIndexSort3]([*@4 MasterRange][@(0.0.255) `&`&]_[*@3 r], [*@4 Range2][@(0.0.255) `&`&
]_[*@3 r2], [*@4 Range3][@(0.0.255) `&`&]_[*@3 r3], [*@4 Range4][@(0.0.255) `&`&]_[*@3 r4])&]
[s2;%% Sorts 2, 3 or 4 containers. All containers must have same 
number of items. Resulting order is defined by the [%-*@3 r ]container. 
Ordering is defined by [*/ operator<].&]
[s3;%% &]
[s4; &]
[s5;:Upp`:`:CoStableIndexSort`(MasterRange`&`&`,Range2`&`&`,const Less`&`): [@(0.0.255) t
emplate]_<[@(0.0.255) class]_[*@4 MasterRange], [@(0.0.255) class]_[*@4 Range2], 
[@(0.0.255) class]_[*@4 Less]>&]
[s5;:Upp`:`:CoStableIndexSort`(MasterRange`&`&`,Range2`&`&`,const Less`&`): [@(0.0.255) v
oid]_[* CoStableIndexSort]([*@4 MasterRange][@(0.0.255) `&`&]_[*@3 r], 
[*@4 Range2][@(0.0.255) `&`&]_[*@3 r2], [@(0.0.255) const]_[*@4 Less][@(0.0.255) `&]_[*@3 less])
&]
[s5;:Upp`:`:CoStableIndexSort`(MasterRange`&`&`,Range2`&`&`,Range3`&`&`,const Less`&`): [@(0.0.255) t
emplate]_<[@(0.0.255) class]_[*@4 MasterRange], [@(0.0.255) class]_[*@4 Range2], 
[@(0.0.255) class]_[*@4 Range3],[@(0.0.255) class]_[*@4 Less]>&]
[s5;:Upp`:`:CoStableIndexSort2`(MasterRange`&`&`,Range2`&`&`,Range3`&`&`,const Less`&`): [@(0.0.255) v
oid]_[* CoStableIndexSort2]([*@4 MasterRange][@(0.0.255) `&`&]_[*@3 r], 
[*@4 Range2][@(0.0.255) `&`&]_[*@3 r2], [*@4 Range3][@(0.0.255) `&`&]_[*@3 r3], 
[@(0.0.255) const]_[*@4 Less][@(0.0.255) `&]_[*@3 less])&]
[s5;:Upp`:`:CoStableIndexSort3`(MasterRange`&`&`,Range2`&`&`,Range3`&`&`,Range4`&`&`,const Less`&`): [@(0.0.255) t
emplate]_<[@(0.0.255) class]_[*@4 MasterRange], [@(0.0.255) class]_[*@4 Range2], 
[@(0.0.255) class]_[*@4 Range3], [@(0.0.255) class]_[*@4 Range4], [@(0.0.255) class]_[*@4 Les
s]>&]
[s5;:Upp`:`:CoStableIndexSort3`(MasterRange`&`&`,Range2`&`&`,Range3`&`&`,Range4`&`&`,const Less`&`): [@(0.0.255) v
oid]_[* CoStableIndexSort3]([*@4 MasterRange][@(0.0.255) `&`&]_[*@3 r], 
[*@4 Range2][@(0.0.255) `&`&]_[*@3 r2], [*@4 Range3][@(0.0.255) `&`&]_[*@3 r3], 
[*@4 Range4][@(0.0.255) `&`&]_[*@3 r4], [@(0.0.255) const]_[*@4 Less][@(0.0.255) `&]_[*@3 less])
&]
[s2;%% Sorts 2, 3 or 4 containers. All containers must have same 
number of items. Resulting order is defined by the [%-*@3 r ]container. 
Ordering is defined by [* less]. The order of elements with the 
same value stays unchanged (stable sort).&]
[s3;%% &]
[s4; &]
[s5;:Upp`:`:CoStableIndexSort`(MasterRange`&`&`,Range2`&`&`): [@(0.0.255) template]_<[@(0.0.255) c
lass]_[*@4 MasterRange], [@(0.0.255) class]_[*@4 Range2]>&]
[s5;:Upp`:`:CoStableIndexSort`(MasterRange`&`&`,Range2`&`&`): [@(0.0.255) void]_[* CoStab
leIndexSort]([*@4 MasterRange][@(0.0.255) `&`&]_[*@3 r], [*@4 Range2][@(0.0.255) `&`&]_[*@3 r
2])&]
[s5;:Upp`:`:CoStableIndexSort2`(MasterRange`&`&`,Range2`&`&`,Range3`&`&`): [@(0.0.255) t
emplate]_<[@(0.0.255) class]_[*@4 MasterRange], [@(0.0.255) class]_[*@4 Range2], 
[@(0.0.255) class]_[*@4 Range3]>&]
[s5;:Upp`:`:CoStableIndexSort2`(MasterRange`&`&`,Range2`&`&`,Range3`&`&`): [@(0.0.255) v
oid]_[* CoStableIndexSort2]([*@4 MasterRange][@(0.0.255) `&`&]_[*@3 r], 
[*@4 Range2][@(0.0.255) `&`&]_[*@3 r2], [*@4 Range3][@(0.0.255) `&`&]_[*@3 r3])&]
[s5;:Upp`:`:CoStableIndexSort3`(MasterRange`&`&`,Range2`&`&`,Range3`&`&`,Range4`&`&`): [@(0.0.255) t
emplate]_<[@(0.0.255) class]_[*@4 MasterRange], [@(0.0.255) class]_[*@4 Range2], 
[@(0.0.255) class]_[*@4 Range3], [@(0.0.255) class]_[*@4 Range4]>&]
[s5;:Upp`:`:CoStableIndexSort3`(MasterRange`&`&`,Range2`&`&`,Range3`&`&`,Range4`&`&`): [@(0.0.255) v
oid]_[* CoStableIndexSort3]([*@4 MasterRange][@(0.0.255) `&`&]_[*@3 r], 
[*@4 Range2][@(0.0.255) `&`&]_[*@3 r2], [*@4 Range3][@(0.0.255) `&`&]_[*@3 r3], 
[*@4 Range4][@(0.0.255) `&`&]_[*@3 r4])&]
[s2;%% Sorts pair of containers. Both containers must have same number 
of items. Resulting order is determined by the [%-*@3 r ]container. 
Ordering is determined by [*/ operator<].&]
[s3;%% &]
[s4; &]
[s5;:Upp`:`:CoGetSortOrder`(const Range`&`,const Less`&`): [@(0.0.255) template]_<[@(0.0.255) c
lass]_[*@4 Range], [@(0.0.255) class]_[*@4 Less]>&]
[s5;:Upp`:`:CoGetSortOrder`(const Range`&`,const Less`&`): [_^Upp`:`:Vector^ Vector]<[@(0.0.255) i
nt]>_[* CoGetSortOrder]([@(0.0.255) const]_[*@4 Range][@(0.0.255) `&]_[*@3 r], 
[@(0.0.255) const]_[*@4 Less][@(0.0.255) `&]_[*@3 less])&]
[s2;%% Creates ascending order of values in [%-*@3 container]. Ordering 
is determined by [%-*@3 less]. The order of elements with the same 
value can be changed (unstable sort).&]
[s3;%% &]
[s4; &]
[s5;:Upp`:`:CoGetSortOrder`(const Range`&`): [@(0.0.255) template]_<[@(0.0.255) class]_[*@4 R
ange]>&]
[s5;:Upp`:`:CoGetSortOrder`(const Range`&`): [_^Upp`:`:Vector^ Vector]<[@(0.0.255) int]>_
[* CoGetSortOrder]([@(0.0.255) const]_[*@4 Range][@(0.0.255) `&]_[*@3 r])&]
[s2;%% Creates ascending order of values in [%-*@3 container]. Ordering 
is determined by [*/ operator<]. The order of elements with the 
same value can be changed (unstable sort).&]
[s3;%% &]
[s4; &]
[s5;:Upp`:`:CoGetStableSortOrder`(const Range`&`,const Less`&`): [@(0.0.255) template]_
<[@(0.0.255) class]_[*@4 Range], [@(0.0.255) class]_[*@4 Less]>&]
[s5;:Upp`:`:CoGetStableSortOrder`(const Range`&`,const Less`&`): [_^Upp`:`:Vector^ Vect
or]<[@(0.0.255) int]>_[* CoGetStableSortOrder]([@(0.0.255) const]_[*@4 Range][@(0.0.255) `&
]_[*@3 r], [@(0.0.255) const]_[*@4 Less][@(0.0.255) `&]_[*@3 less])&]
[s2;%% Creates ascending order of values in [%-*@3 container]. Ordering 
is determined by [%-*@3 less]. The order of elements with the same 
value stays unchanged (stable sort).&]
[s3;%% &]
[s4; &]
[s5;:Upp`:`:CoGetStableSortOrder`(const Range`&`): [@(0.0.255) template]_<[@(0.0.255) cla
ss]_[*@4 Range]>&]
[s5;:Upp`:`:CoGetStableSortOrder`(const Range`&`): [_^Upp`:`:Vector^ Vector]<[@(0.0.255) i
nt]>_[* CoGetStableSortOrder]([@(0.0.255) const]_[*@4 Range][@(0.0.255) `&]_[*@3 r])&]
[s2;%% Creates ascending order of values in [%-*@3 container]. Ordering 
is determined by [*/ operator<]. The order of elements with the 
same value stays unchanged (stable sort).&]
[s3;%% &]
[s4; &]
[s5;:Upp`:`:CoSortByKey`(Map`&`,const Less`&`): [@(0.0.255) template]_<[@(0.0.255) class]_
[*@4 Map], [@(0.0.255) class]_[*@4 Less]>&]
[s5;:Upp`:`:CoSortByKey`(Map`&`,const Less`&`): [@(0.0.255) void]_[* CoSortByKey]([*@4 Map][@(0.0.255) `&
]_[*@3 map], [@(0.0.255) const]_[*@4 Less][@(0.0.255) `&]_[*@3 less])&]
[s2;%% Sorts VectorMap or ArrayMap (or any other hypothetical container 
that supports required interfaces) [%-*@3 map] by keys, using [%-*@3 less] 
as sorting predicate.&]
[s3;%% &]
[s4; &]
[s5;:Upp`:`:CoSortByKey`(Map`&`): [@(0.0.255) template]_<[@(0.0.255) class]_[*@4 Map]>&]
[s5;:Upp`:`:CoSortByKey`(Map`&`): [@(0.0.255) void]_[* CoSortByKey]([*@4 Map][@(0.0.255) `&]_
[*@3 map])&]
[s2;%% Sorts VectorMap or ArrayMap (or any other hypothetical container 
that supports required interfaces) [%-*@3 map] by keys, using operator< 
as sorting predicate.&]
[s3;%% &]
[s4; &]
[s5;:Upp`:`:CoSortByValue`(Map`&`,const Less`&`): [@(0.0.255) template]_<[@(0.0.255) clas
s]_[*@4 Map], [@(0.0.255) class]_[*@4 Less]>&]
[s5;:Upp`:`:CoSortByValue`(Map`&`,const Less`&`): [@(0.0.255) void]_[* CoSortByValue]([*@4 M
ap][@(0.0.255) `&]_[*@3 map], [@(0.0.255) const]_[*@4 Less][@(0.0.255) `&]_[*@3 less])&]
[s2;%% Sorts VectorMap or ArrayMap (or any other hypothetical container 
that supports required interfaces) [%-*@3 map] by values, using 
[%-*@3 less] as sorting predicate.&]
[s3;%% &]
[s4; &]
[s5;:Upp`:`:CoSortByValue`(Map`&`): [@(0.0.255) template]_<[@(0.0.255) class]_[*@4 Map]>&]
[s5;:Upp`:`:CoSortByValue`(Map`&`): [@(0.0.255) void]_[* CoSortByValue]([*@4 Map][@(0.0.255) `&
]_[*@3 map])&]
[s2;%% Sorts VectorMap or ArrayMap (or any other hypothetical container 
that supports required interfaces) [%-*@3 map] by values, using 
operator< as sorting predicate.&]
[s3;%% &]
[s4; &]
[s5;:Upp`:`:CoStableSortByKey`(Map`&`,const Less`&`): [@(0.0.255) template]_<[@(0.0.255) c
lass]_[*@4 Map], [@(0.0.255) class]_[*@4 Less]>&]
[s5;:Upp`:`:CoStableSortByKey`(Map`&`,const Less`&`): [@(0.0.255) void]_[* CoStableSortBy
Key]([*@4 Map][@(0.0.255) `&]_[*@3 map], [@(0.0.255) const]_[*@4 Less][@(0.0.255) `&]_[*@3 less
])&]
[s2;%% Sorts VectorMap or ArrayMap (or any other hypothetical container 
that supports required interfaces) [%-*@3 map] by keys, using [%-*@3 less] 
as sorting predicate. Stable: retains the order of equal elements.&]
[s3;%% &]
[s4; &]
[s5;:Upp`:`:CoStableSortByKey`(Map`&`): [@(0.0.255) template]_<[@(0.0.255) class]_[*@4 Map]>
&]
[s5;:Upp`:`:CoStableSortByKey`(Map`&`): [@(0.0.255) void]_[* CoStableSortByKey]([*@4 Map][@(0.0.255) `&
]_[*@3 map])&]
[s2;%% Sorts VectorMap or ArrayMap (or any other hypothetical container 
that supports required interfaces) [%-*@3 map] by keys, using operator< 
as sorting predicate. Stable: retains the order of equal elements.&]
[s3;%% &]
[s4; &]
[s5;:Upp`:`:CoStableSortByValue`(Map`&`,const Less`&`): [@(0.0.255) template]_<[@(0.0.255) c
lass]_[*@4 Map], [@(0.0.255) class]_[*@4 Less]>&]
[s5;:Upp`:`:CoStableSortByValue`(Map`&`,const Less`&`): [@(0.0.255) void]_[* CoStableSort
ByValue]([*@4 Map][@(0.0.255) `&]_[*@3 map], [@(0.0.255) const]_[*@4 Less][@(0.0.255) `&]_[*@3 l
ess])&]
[s2;%% Sorts VectorMap or ArrayMap (or any other hypothetical container 
that supports required interfaces) [%-*@3 map] by values, using 
[%-*@3 less] as sorting predicate. Stable: retains the order of 
equal elements.&]
[s3;%% &]
[s4; &]
[s5;:Upp`:`:CoStableSortByValue`(Map`&`): [@(0.0.255) template]_<[@(0.0.255) class]_[*@4 Ma
p]>&]
[s5;:Upp`:`:CoStableSortByValue`(Map`&`): [@(0.0.255) void]_[* CoStableSortByValue]([*@4 Ma
p][@(0.0.255) `&]_[*@3 map])&]
[s2;%% Sorts VectorMap or ArrayMap (or any other hypothetical container 
that supports required interfaces) [%-*@3 map] by values, using 
operator< as sorting predicate. Stable: retains the order of equal 
elements.&]
[s3;%% &]
[s4; &]
[s5;:Index`&`,const Less`&`): [@(0.0.255) template]_<[@(0.0.255) class]_[*@4 Index], 
[@(0.0.255) class]_[*@4 Less]>&]
[s5;:Index`&`,const Less`&`): [@(0.0.255) void]_[* CoSortIndex]([*@4 Index][@(0.0.255) `&]_[*@3 i
ndex], [@(0.0.255) const]_[*@4 Less][@(0.0.255) `&]_[*@3 less])&]
[s2;%% Sorts Index or ArrayIndex using [%-*@3 less] as sorting predicate.&]
[s3;%% &]
[s4; &]
[s5;:Index`&`): [@(0.0.255) template]_<[@(0.0.255) class]_[*@4 Index]>&]
[s5;:Index`&`): [@(0.0.255) void]_[* CoSortIndex]([*@4 Index][@(0.0.255) `&]_[*@3 index])&]
[s2;%% Sorts Index or ArrayIndex.&]
[s3;%% &]
[s4; &]
[s5;:Index`&`,const Less`&`): [@(0.0.255) template]_<[@(0.0.255) class]_[*@4 Index], 
[@(0.0.255) class]_[*@4 Less]>&]
[s5;:Index`&`,const Less`&`): [@(0.0.255) void]_[* CoStableSortIndex]([*@4 Index][@(0.0.255) `&
]_[*@3 index], [@(0.0.255) const]_[*@4 Less][@(0.0.255) `&]_[*@3 less])&]
[s2;%% Sorts Index or ArrayIndex using [%-*@3 less] as sorting predicate. 
Stable: retains the order of equal elements.&]
[s3;%% &]
[s4; &]
[s5;:Index`&`): [@(0.0.255) template]_<[@(0.0.255) class]_[*@4 Index]>&]
[s5;:Index`&`): [@(0.0.255) void]_[* CoStableSortIndex]([*@4 Index][@(0.0.255) `&]_[*@3 index])
&]
[s2;%% Sorts Index or ArrayIndex.  Stable: retains the order of equal 
elements.&]
[s0;%% ]]