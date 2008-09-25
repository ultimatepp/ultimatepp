topic "Algorithms";
[2 $$0,0#00000000000000000000000000000000:Default]
[i448;a25;kKO9;2 $$1,0#37138531426314131252341829483380:class]
[l288;2 $$2,0#27521748481378242620020725143825:desc]
[0 $$3,0#96390100711032703541132217272105:end]
[H6;0 $$4,0#05600065144404261032431302351956:begin]
[i448;a25;kKO9;2 $$5,0#37138531426314131252341829483370:item]
[l288;a17;*@5;1 $$6,6#70004532496200323422659154056402:requirement]
[l288;i1121;b17;O9;~~~.1408;2 $$7,0#10431211400427159095818037425705:param]
[{_}%EN-US 
[s0; Algorithms&]
[s0; &]
[s5;K%- template_<[@(0.0.255) class][@(64) _][@4 T][@(64) >__][@(0.0.255) void][@(64) _]Swap[@(64) (
][^T^@(64) T][@(64) `&_][@3 a][@(64) , ][^T^@(64) T][@(64) `&_][@3 b][@(64) )]&]
[s2; Swaps values. Specific types might specialize [* Swap] with more 
effective variants.&]
[s6; T must have either deep copy operator or pick operator.&]
[s7; [*C@4 T]-|Type of values.&]
[s7; [*C@3 a]-|First value to swap.&]
[s7; [*C@3 b]-|Second value to swap.&]
[s0; &]
[s5;K%- template_<[@(0.0.255) class][@(64) _][@4 I][@(64) >__][@(0.0.255) void][@(64) _]IterSwap[@(64) (
][^I^@(64) I][@(64) _][@3 a][@(64) , ][^I^@(64) I][@(64) _][@3 b][@(64) )]&]
[s2; Swaps values pointed to by iterators. Specific types might specialize 
[* IterSwap] with more effective variants.&]
[s6; Swap must be defined for type pointed to by I.&]
[s7; [*C@4 I]-|Iterator type.&]
[s7; [*C@3 a]-|Iterator pointing to first value.&]
[s7; [*C@3 b]-|Iterator pointing to second value.&]
[s0; &]
[s5;K%- template_<[@(0.0.255) class][@(64) _][@4 C][@(64) , ][@(0.0.255) class][@(64) _][@4 T][@(64) ,
 ][@(0.0.255) class][@(64) _][@4 L][@(64) >__][@(0.0.255) int][@(64) _]FindLowerBound[@(64) (][@(0.0.255) c
onst][@(64) _][^C^@(64) C][@(64) `&_][@3 v][@(64) , ][@(0.0.255) int][@(64) _][@3 pos][@(64) , 
][@(0.0.255) int][@(64) _][@3 count][@(64) , ][@(0.0.255) const][@(64) _][^T^@(64) T][@(64) `&_][@3 v
al][@(64) , ][@(0.0.255) const][@(64) _][^L^@(64) L][@(64) `&_][@3 less][@(64) )]&]
[s2; Finds first position in range of container sorted by [* less] 
predicate where [* val] can be inserted without breaking the ordering.&]
[s7; [*C@4 C]-|Type of container.&]
[s7; [*C@4 T]-|Type of value.&]
[s7; [*C@3 v]-|Container.&]
[s7; [*C@3 pos]-|Beginning of range.&]
[s7; [*C@3 count]-|Number of elements in range.&]
[s7; [*C@3 val]-|Value to find.&]
[s7; [*C@3 less]-|Ordering predicate.&]
[s7; [*/ Return value]-|Position in container.&]
[s0; &]
[s5;K%- template_<[@(0.0.255) class][@(64) _][@4 C][@(64) , ][@(0.0.255) class][@(64) _][@4 T][@(64) ,
 ][@(0.0.255) class][@(64) _][@4 L][@(64) >__][@(0.0.255) int][@(64) _]FindLowerBound[@(64) (][@(0.0.255) c
onst][@(64) _][^C^@(64) C][@(64) `&_][@3 v][@(64) , ][@(0.0.255) const][@(64) _][^T^@(64) T][@(64) `&
_][@3 val][@(64) , ][@(0.0.255) const][@(64) _][^L^@(64) L][@(64) `&_][@3 less][@(64) )]&]
[s2; Finds first position in sorted by [* less] predicate where [* val] 
can be inserted without breaking the ordering.&]
[s7; [*C@4 C]-|Type of container.&]
[s7; [*C@4 T]-|Type of value.&]
[s7; [*C@3 v]-|Container.&]
[s7; [*C@3 val]-|Value to find.&]
[s7; [*C@3 less]-|Ordering predicate.&]
[s7; [*/ Return value]-|Position in container.&]
[s0; &]
[s5;K%- template_<[@(0.0.255) class][@(64) _][@4 C][@(64) , ][@(0.0.255) class][@(64) _][@4 T][@(64) >
__][@(0.0.255) int][@(64) _]FindLowerBound[@(64) (][@(0.0.255) const][@(64) _][^C^@(64) C][@(64) `&
_][@3 v][@(64) , ][@(0.0.255) const][@(64) _][^T^@(64) T][@(64) `&_][@3 val][@(64) )]&]
[s2; Finds first position in sorted by [*/ operator<] predicate where 
[* val] can be inserted without breaking the ordering.&]
[s7; [*C@4 C]-|Type of container.&]
[s7; [*C@4 T]-|Type of value.&]
[s7; [*C@3 v]-|Container.&]
[s7; [*C@3 val]-|Value to find.&]
[s7; [*/ Return value]-|Position in container.&]
[s0; &]
[s5;K%- template_<[@(0.0.255) class][@(64) _][@4 C][@(64) , ][@(0.0.255) class][@(64) _][@4 T][@(64) ,
 ][@(0.0.255) class][@(64) _][@4 L][@(64) >__][@(0.0.255) int][@(64) _]FindUpperBound[@(64) (][@(0.0.255) c
onst][@(64) _][^C^@(64) C][@(64) `&_][@3 v][@(64) , ][@(0.0.255) int][@(64) _][@3 pos][@(64) , 
][@(0.0.255) int][@(64) _][@3 count][@(64) , ][@(0.0.255) const][@(64) _][^T^@(64) T][@(64) `&_][@3 v
al][@(64) , ][@(0.0.255) const][@(64) _][^L^@(64) L][@(64) `&_][@3 less][@(64) )]&]
[s2; Finds last position in range of container sorted by [* less] predicate 
where [* val] can be inserted without breaking the ordering.&]
[s7; [*C@4 C]-|Type of container.&]
[s7; [*C@4 T]-|Type of value.&]
[s7; [*C@3 v]-|Container.&]
[s7; [*C@3 pos]-|Beginning of range.&]
[s7; [*C@3 count]-|Number of elements in range.&]
[s7; [*C@3 val]-|Value to find.&]
[s7; [*C@3 less]-|Ordering predicate.&]
[s7; [*/ Return value]-|Position in container.&]
[s0; &]
[s5;K%- template_<[@(0.0.255) class][@(64) _][@4 C][@(64) , ][@(0.0.255) class][@(64) _][@4 T][@(64) ,
 ][@(0.0.255) class][@(64) _][@4 L][@(64) >__][@(0.0.255) int][@(64) _]FindUpperBound[@(64) (][@(0.0.255) c
onst][@(64) _][^C^@(64) C][@(64) `&_][@3 v][@(64) , ][@(0.0.255) const][@(64) _][^T^@(64) T][@(64) `&
_][@3 val][@(64) , ][@(0.0.255) const][@(64) _][^L^@(64) L][@(64) `&_][@3 less][@(64) )]&]
[s2; Finds last position in sorted by [* less] predicate where [* val] 
can be inserted without breaking the ordering.&]
[s7; [*C@4 C]-|Type of container.&]
[s7; [*C@4 T]-|Type of value.&]
[s7; [*C@3 v]-|Container.&]
[s7; [*C@3 val]-|Value to find.&]
[s7; [*C@3 less]-|Ordering predicate.&]
[s7; [*/ Return value]-|Position in container.&]
[s0;:template`<class C`, class T`> int FindUpperBound`(const C`& v`, const T`& val`): &]
[s5;K%- template_<[@(0.0.255) class][@(64) _][@4 C][@(64) , ][@(0.0.255) class][@(64) _][@4 T][@(64) >
__][@(0.0.255) int][@(64) _]FindUpperBound[@(64) (][@(0.0.255) const][@(64) _][^C^@(64) C][@(64) `&
_][@3 v][@(64) , ][@(0.0.255) const][@(64) _][^T^@(64) T][@(64) `&_][@3 val][@(64) )]&]
[s2; Finds last position in sorted by [*/ operator<] predicate where 
[* val] can be inserted without breaking the ordering.&]
[s7; [*C@4 C]-|Type of container.&]
[s7; [*C@4 T]-|Type of value.&]
[s7; [*C@3 v]-|Container.&]
[s7; [*C@3 val]-|Value to find.&]
[s7; [*/ Return value]-|Position in container.&]
[s0; &]
[s5;K%- template_<[@(0.0.255) class][@(64) _][@4 C][@(64) , ][@(0.0.255) class][@(64) _][@4 T][@(64) ,
 ][@(0.0.255) class][@(64) _][@4 L][@(64) >__][@(0.0.255) int][@(64) _]FindBinary[@(64) (][@(0.0.255) c
onst][@(64) _][^C^@(64) C][@(64) `&_][@3 v][@(64) , ][@(0.0.255) const][@(64) _][^T^@(64) T][@(64) `&
_][@3 val][@(64) , ][@(0.0.255) int][@(64) _][@3 pos][@(64) , ][@(0.0.255) int][@(64) _][@3 count][@(64) ,
 ][@(0.0.255) const][@(64) _][^L^@(64) L][@(64) `&_][@3 less][@(64) )]&]
[s2; Finds position of element with specified value in a range of 
container sorted by [* less] predicate. If no such element exists, 
a negative value is returned.&]
[s7; [*C@4 C]-|Type of container.&]
[s7; [*C@4 T]-|Type of value.&]
[s7; [*C@3 v]-|Container.&]
[s7; [*C@3 pos]-|Beginning of range.&]
[s7; [*C@3 count]-|Number of elements in range.&]
[s7; [*C@3 val]-|Value to find.&]
[s7; [*C@3 less]-|Ordering predicate.&]
[s7; [*/ Return value]-|Position in container.&]
[s0; &]
[s5;K%- template_<[@(0.0.255) class][@(64) _][@4 C][@(64) , ][@(0.0.255) class][@(64) _][@4 T][@(64) ,
 ][@(0.0.255) class][@(64) _][@4 L][@(64) >__][@(0.0.255) int][@(64) _]FindBinary[@(64) (][@(0.0.255) c
onst][@(64) _][^C^@(64) C][@(64) `&_][@3 v][@(64) , ][@(0.0.255) const][@(64) _][^T^@(64) T][@(64) `&
_][@3 val][@(64) , ][@(0.0.255) const][@(64) _][^L^@(64) L][@(64) `&_][@3 less][@(64) )]&]
[s2; Finds position of element with specified value in the container 
sorted by [* less] predicate. If no such element exists, a negative 
value is returned.&]
[s7; [*C@4 C]-|Type of container.&]
[s7; [*C@4 T]-|Type of value.&]
[s7; [*C@3 v]-|Container.&]
[s7; [*C@3 val]-|Value to find.&]
[s7; [*C@3 less]-|Ordering predicate.&]
[s7; [*/ Return value]-|Position in container.&]
[s0; &]
[s5;K%- template_<[@(0.0.255) class][@(64) _][@4 C][@(64) , ][@(0.0.255) class][@(64) _][@4 T][@(64) >
__][@(0.0.255) int][@(64) _]FindBinary[@(64) (][@(0.0.255) const][@(64) _][^C^@(64) C][@(64) `&
_][@3 v][@(64) , ][@(0.0.255) const][@(64) _][^T^@(64) T][@(64) `&_][@3 val][@(64) )]&]
[s2; Finds position of element with specified value in the container 
sorted by [*/ operator<] predicate. If no such element exists, 
a negative value is returned.&]
[s7; [*C@4 C]-|Type of container.&]
[s7; [*C@4 T]-|Type of value.&]
[s7; [*C@3 v]-|Container.&]
[s7; [*C@3 val]-|Value to find.&]
[s7; [*/ Return value]-|Position in container.&]
[s0;~~~.992; &]
[s5;K%- template_<[@(0.0.255) class][@(64) _][^C^@(64) C][@(64) , ][@(0.0.255) class][@(64) _][@4 L
][@(64) >__][^C^@(64) C][@(64) `&_]AppendSorted[@(64) (][^C^@(64) C][@(64) `&_][@3 dest][@(64) , 
][@(0.0.255) const][@(64) _][^C^@(64) C][@(64) `&_][@3 src][@(64) , ][@(0.0.255) const][@(64) _][^L^@(64) L
][@(64) `&_][@3 less][@(64) )]&]
[s2; Merges source NTL container to destination NTL container. Both 
containers must be sorted in ascending order. After the operation, 
destination container is sorted in ascending order and contains 
values from both containers. Duplicate values are preserved. 
Ordering predicate is determined by [* less].&]
[s7; [*C@4 C]-|Type of container.&]
[s7; [*C@3 dest]-|Destination container.&]
[s7; [*C@3 src]-|Source container.&]
[s7; [*C@3 less]-|Ordering predicate.&]
[s7; [*/ Return value]-|Destination container.[C template<class ][*C@4 C][C >]&]
[s0;~~~.992; &]
[s5;K%- template_<[@(0.0.255) class][@(64) _][^C^@(64) C][@(64) >__][^C^@(64) C][@(64) `&_]Append
Sorted[@(64) (][^C^@(64) C][@(64) `&_][@3 dest][@(64) , ][@(0.0.255) const][@(64) _][^C^@(64) C][@(64) `&
_][@3 src][@(64) )]&]
[s2;~~~.992; Merges source NTL container to destination NTL container. 
Both containers must be sorted in ascending order. After the 
operation, destination container is sorted in ascending order 
and contains values from both containers. Duplicate values are 
preserved. Ordering is determined by [*/ operator<].&]
[s7; [*C@4 C]-|Type of container.&]
[s7; [*C@3 dest]-|Destination container.&]
[s7; [*C@3 src]-|Source container.&]
[s7; [*/ Return value]-|Destination container.[C template<class ][*C@4 C][C >]&]
[s0; &]
[s5;K%- template_<[@(0.0.255) class][@(64) _][^C^@(64) C][@(64) , ][@(0.0.255) class][@(64) _][@4 L
][@(64) >__][^C^@(64) C][@(64) `&_]UnionSorted[@(64) (][^C^@(64) C][@(64) `&_][@3 dest][@(64) , 
][@(0.0.255) const][@(64) _][^C^@(64) C][@(64) `&_][@3 src][@(64) , ][@(0.0.255) const][@(64) _][^L^@(64) L
][@(64) `&_][@3 less][@(64) )]&]
[s2; Merges source NTL container to destination NTL container. Both 
containers must be sorted in ascending order and values must 
be unique. After the operation, destination container is sorted 
in ascending order and contains unique values from both containers. 
Ordering is determined by [* less].&]
[s7; [*C@4 C]-|Type of container.&]
[s7; [*C@3 dest]-|Destination container.&]
[s7; [*C@3 src]-|Source container.&]
[s7; [*C@3 less]-|Ordering predicate.&]
[s7; [*/ Return value]-|Destination container.&]
[s0; &]
[s5;K%- template_<[@(0.0.255) class][@(64) _][^C^@(64) C][@(64) >__][^C^@(64) C][@(64) `&_]UnionS
orted[@(64) (][^C^@(64) C][@(64) `&_][@3 dest][@(64) , ][@(0.0.255) const][@(64) _][^C^@(64) C][@(64) `&
_][@3 src][@(64) )]&]
[s2; Merges the source NTL container to the destination NTL container. 
Both containers must be sorted in ascending order and values 
must be unique. After the operation, destination container is 
sorted in ascending order and contains unique values from both 
containers. Ordering is determined by [*/ operator<].&]
[s7; [*C@4 C]-|Type of container.&]
[s7; [*C@3 dest]-|Destination container.&]
[s7; [*C@3 src]-|Source container.&]
[s7; [*/ Return value]-|Destination container.&]
[s0; &]
[s5;K%- template_<[@(0.0.255) class][@(64) _][^C^@(64) C][@(64) , ][@(0.0.255) class][@(64) _][@4 L
][@(64) >__][^C^@(64) C][@(64) `&_]RemoveSorted[@(64) (][^C^@(64) C][@(64) `&_][@3 from][@(64) , 
][@(0.0.255) const][@(64) _][^C^@(64) C][@(64) `&_][@3 what][@(64) , ][@(0.0.255) const][@(64) _][^L^@(64) L
][@(64) `&_][@3 less][@(64) )]&]
[s2; Removes elements of source container from destination container. 
Both containers must be sorted in ascending order and values 
must be unique. Ordering is determined by [* less].&]
[s7; [*C@4 C]-|Type of container.&]
[s7; [*C@3 from]-|Destination container.&]
[s7; [*C@3 what]-|Source container.&]
[s7; [*C@3 less]-|Ordering predicate.&]
[s7; [*/ Return value]-|Destination container.&]
[s0; &]
[s5;K%- template_<[@(0.0.255) class][@(64) _][^C^@(64) C][@(64) >__][^C^@(64) C][@(64) `&_]Remove
Sorted[@(64) (][^C^@(64) C][@(64) `&_][@3 from][@(64) , ][@(0.0.255) const][@(64) _][^C^@(64) C][@(64) `&
_][@3 what][@(64) )]&]
[s2; Removes elements of source container from destination container. 
Both containers must be sorted in ascending order and values 
must be unique. Ordering is determined by [*/ operator<].&]
[s7; [*C@4 C]-|Type of container.&]
[s7; [*C@3 from]-|Destination container.&]
[s7; [*C@3 what]-|Source container.&]
[s7; [*/ Return value]-|Destination container.&]
[s0; &]
[s5;K%- template_<[@(0.0.255) class][@(64) _][^D^@(64) D][@(64) , ][@(0.0.255) class][@(64) _][@4 S
][@(64) , ][@(0.0.255) class][@(64) _][@4 L][@(64) >__][^D^@(64) D][@(64) `&_]IntersectSorted[@(64) (
][^D^@(64) D][@(64) `&_][@3 dest][@(64) , ][@(0.0.255) const][@(64) _][^S^@(64) S][@(64) `&_][@3 sr
c][@(64) , ][@(0.0.255) const][@(64) _][^L^@(64) L][@(64) `&_][@3 less][@(64) )]&]
[s2; Removes elements from destination container that are not contained 
in source container. Both containers must be sorted in ascending 
order and values must be unique. Ordering is determined by [* less].&]
[s7; [*C@4 D]-|Type of destination container.&]
[s7; [*C@4 S]-|Type of source container.&]
[s7; [*C@3 dest]-|Destination container.&]
[s7; [*C@3 src]-|Source container.&]
[s7; [*C@3 less]-|Destination container.&]
[s0; &]
[s5;K%- template_<[@(0.0.255) class][@(64) _][^D^@(64) D][@(64) , ][@(0.0.255) class][@(64) _][@4 S
][@(64) >__][^D^@(64) D][@(64) `&_]IntersectSorted[@(64) (][^D^@(64) D][@(64) `&_][@3 dest][@(64) ,
 ][@(0.0.255) const][@(64) _][^S^@(64) S][@(64) `&_][@3 src][@(64) )]&]
[s2; Removes elements from destination container that are not contained 
in source container. Both containers must be sorted in ascending 
order and values must be unique. Ordering is determined by [*/ operator<].&]
[s7; [*C@4 D]-|Type of destination container.&]
[s7; [*C@4 S]-|Type of source container.&]
[s7; [*C@3 dest]-|Destination container.&]
[s7; [*C@3 src]-|Source container.&]
[s7; [*/ Return value]-|Destination container.&]
[s0; &]
[s5;K%- template_<[@(0.0.255) class][@(64) _][@4 T][@(64) , ][@(0.0.255) class][@(64) _][@4 Less][@(64) >
__][@(0.0.255) void][@(64) _]Sort[@(64) (][^T^@(64) T][@(64) `&_][@3 c][@(64) , 
][@(0.0.255) const][@(64) _][^Less^@(64) Less][@(64) `&_][@3 less][@(64) )]&]
[s2; Sorts container. Ordering is determined by [* less].&]
[s6; IterSwap must be defined for T`::Iterator.&]
[s7; [*C@4 T]-|Type of container.&]
[s7; [*C@3 c]-|Container.&]
[s7; [*C@3 less]-|Ordering predicate.&]
[s0; &]
[s5;K%- template_<[@(0.0.255) class][@(64) _][@4 T][@(64) >__][@(0.0.255) void][@(64) _]Sort[@(64) (
][^T^@(64) T][@(64) `&_][@3 c][@(64) )]&]
[s2; Sorts container. Ordering is determined by [*/ operator<].&]
[s2; [*1 IterSwap must be defined for T`::Iterator.]&]
[s7; [*C@4 T]-|Type of container.&]
[s7; [*C@3 c]-|Container.&]
[s0; &]
[s5;K%- template_<[@(0.0.255) class][@(64) _][@4 KC][@(64) , ][@(0.0.255) class][@(64) _][@4 VC][@(64) ,
 ][@(0.0.255) class][@(64) _][@4 Less][@(64) >__][@(0.0.255) void][@(64) _]IndexSort[@(64) (][^KC^@(64) K
C][@(64) `&_][@3 keys][@(64) , ][^VC^@(64) VC][@(64) `&_][@3 values][@(64) , 
][@(0.0.255) const][@(64) _][^Less^@(64) Less][@(64) `&_][@3 less][@(64) )]&]
[s2; Sorts pair of containers. Both containers must have same number 
of items. Resulting order is determined by the [* keys] container. 
Ordering is determined by [* less].&]
[s7; [*C@4 KC]-|Type of keys container.&]
[s7; [*C@4 VC]-|Type of values.&]
[s7; [*C@3 keys]-|Container of keys.&]
[s7; [*C@3 values]-|Container of values.&]
[s7; [*C@3 less]-|Ordering predicate.&]
[s0; &]
[s5;K%- template_<[@(0.0.255) class][@(64) _][@4 KC][@(64) , ][@(0.0.255) class][@(64) _][@4 VC][@(64) >
__][@(0.0.255) void][@(64) _]IndexSort[@(64) (][^KC^@(64) KC][@(64) `&_][@3 keys][@(64) , 
][^VC^@(64) VC][@(64) `&_][@3 values][@(64) )]&]
[s2; Sorts pair of containers. Both containers must have same number 
of items. Resulting order is determined by the [* keys] container. 
Ordering is determined by [*/ operator<].&]
[s7; [*C@4 KC]-|Type of keys container.&]
[s7; [*C@4 VC]-|Type of values container.&]
[s7; [*C@3 keys]-|Container of keys.&]
[s7; [*C@3 values]-|Container of values.&]
[s0; &]
[s5;K%- template_<[@(0.0.255) class][@(64) _][@4 C][@(64) , ][@(0.0.255) class][@(64) _][@4 Less][@(64) >
__][^`:`:Vector^@(64) Vector][@(64) <][^int^@(64) int][@(64) >_]GetSortOrder[@(64) (][@(0.0.255) c
onst][@(64) _][^C^@(64) C][@(64) `&_][@3 container][@(64) , ][@(0.0.255) const][@(64) _][^Less^@(64) L
ess][@(64) `&_][@3 less][@(64) )]&]
[s2; Creates ascending order of values in container. Ordering is 
determined by [* less].&]
[s7; [*C@4 C]-|Type of container.&]
[s7; [*C@3 container]-|Source container.&]
[s7; [*C@3 less]-|Ordering predicate.&]
[s7; [*/ Return value]-|Vector of positions of source container in sorted 
order.&]
[s0; &]
[s5;K%- template_<[@(0.0.255) class][@(64) _][@4 C][@(64) >__][^`:`:Vector^@(64) Vector][@(64) <][^int^@(64) i
nt][@(64) >_]GetSortOrder[@(64) (][@(0.0.255) const][@(64) _][^C^@(64) C][@(64) `&_][@3 contain
er][@(64) )]&]
[s2; Creates ascending order of values in container. Ordering is 
determined by [*/ operator<].&]
[s7; [*C@4 C]-|Type of container.&]
[s7; [*C@3 container]-|Source container.&]
[s7; [*/ Return value]-|Vector of positions of source container in sorted 
order.&]
[s0; &]
[s5;K%- template_<[@(0.0.255) class][@(64) _][^O^@(64) O][@(64) , ][@(0.0.255) class][@(64) _][^T^@(64) T
][@(64) , ][@(0.0.255) class][@(64) _][^R^@(64) R][@(64) >__][^`:`:FieldRelationCls^@(64) Fie
ldRelationCls][@(64) <][^O^@(64) O][@(64) , ][^T^@(64) T][@(64) , ][^R^@(64) R][@(64) >_]FieldR
elation[@(64) (][^O^@(64) O][@(64) _(T`::`*][@3 member][@(64) ), ][@(0.0.255) const][@(64) _][^R^@(64) R
][@(64) `&_][@3 relation][@(64) )]&]
[s2; Creates ordering predicate for [* T] based on the value of member 
variable of[*  T].&]
[s7; [*C@4 T]-|Type of element.&]
[s7; [*C@3 member]-|Member variable of T.&]
[s7; [*C@3 relation]-|Ordering relation for [* member].&]
[s7; [*/ Return value]-|Ordering predicate.&]
[s0; &]
[s5;K%- template_<[@(0.0.255) class][@(64) _][@4 O][@(64) , ][@(0.0.255) class][@(64) _][^T^@(64) T
][@(64) , ][@(0.0.255) class][@(64) _][^R^@(64) R][@(64) >__][^`:`:MethodRelationCls^@(64) Me
thodRelationCls][@(64) <][@4 O][@(64) _(T`::`*)(), ][^T^@(64) T][@(64) , 
][^R^@(64) R][@(64) >_]MethodRelation[@(64) (][^O^@(64) O][@(64) _(T`::`*][@3 method][@(64) )(),
 ][@(0.0.255) const][@(64) _][^R^@(64) R][@(64) `&_][@3 relation][@(64) )]&]
[s2; Creates ordering predicate for [* T] based on the value returned 
by non`-const method of [* T].&]
[s7; [*C@4 T]-|Type of element.&]
[s7; [*C@3 method]-|Method of T.&]
[s7; [*C@3 relation]-|Ordering relation for value returned by method.&]
[s7; [*/ Return value]-|Ordering predicate.&]
[s0; &]
[s5;K%- template_<[@(0.0.255) class][@(64) _][@4 O][@(64) , ][@(0.0.255) class][@(64) _][^T^@(64) T
][@(64) , ][@(0.0.255) class][@(64) _][^R^@(64) R][@(64) >__][^`:`:MethodRelationCls^@(64) Me
thodRelationCls][@(64) <][@4 O][@(64) _(T`::`*)()_][@(0.0.255) const][@(64) , 
][^T^@(64) T][@(64) , ][^R^@(64) R][@(64) >_]MethodRelation[@(64) (][^O^@(64) O][@(64) _(T`::`*][@3 m
ethod][@(64) )()_][@(0.0.255) const][@(64) , ][@(0.0.255) const][@(64) _][^R^@(64) R][@(64) `&_
][@3 relation][@(64) )]&]
[s2; Creates ordering predicate for [* T] based on the value returned 
by const method of [* T].&]
[s7; [*C@4 T]-|Type of element.&]
[s7; [*C@3 method]-|Method of T.&]
[s7; [*C@3 relation]-|Ordering relation for value returned by method.&]
[s7; [*/ Return value]-|Ordering predicate.&]
[s0; ]