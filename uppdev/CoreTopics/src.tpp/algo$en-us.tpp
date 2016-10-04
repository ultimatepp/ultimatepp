topic "Algorithms";
[2 $$0,0#00000000000000000000000000000000:Default]
[i448;a25;kKO9;2 $$1,0#37138531426314131252341829483380:class]
[l288;2 $$2,0#27521748481378242620020725143825:desc]
[0 $$3,0#96390100711032703541132217272105:end]
[H6;0 $$4,0#05600065144404261032431302351956:begin]
[i448;a25;kKO9;R+75 $$5,0#37138531426314131252341829483370:item]
[l288;a17;*@5;1 $$6,6#70004532496200323422659154056402:requirement]
[l288;i1121;b17;O9;~~~.1408;2 $$7,0#10431211400427159095818037425705:param]
[{_}%EN-US 
[s0;a83; [*R6 Algorithms]&]
[s0; &]
[s3;%- &]
[s4;%- &]
[s5;:`:`:`:`:Swap`(T`&`,T`&`):%- [@(0.0.255) template]_<[@(0.0.255) class]_[*@4 T]>_[@(0.0.255) v
oid]_[* Swap]([*@4 T]`&_[*@3 a], [*@4 T]`&_[*@3 b])&]
[s2; Swaps values. Specific types might specialize [* Swap] with more 
effective variants.&]
[s6; T must have either deep copy operator or pick operator.&]
[s7; [*C@4 T]-|Type of values.&]
[s7; [*C@3 a]-|First value to swap.&]
[s7; [*C@3 b]-|Second value to swap.&]
[s3;%- &]
[s4;%- &]
[s5;:`:`:`:`:IterSwap`(I`,I`):%- [@(0.0.255) template]_<[@(0.0.255) class]_[*@4 I]>_[@(0.0.255) v
oid]_[* IterSwap]([*@4 I]_[*@3 a], [*@4 I]_[*@3 b])&]
[s2; Swaps values pointed to by iterators. Specific types might specialize 
[* IterSwap] with more effective variants.&]
[s6; Swap must be defined for type pointed to by I.&]
[s7; [*C@4 I]-|Iterator type.&]
[s7; [*C@3 a]-|Iterator pointing to first value.&]
[s7; [*C@3 b]-|Iterator pointing to second value.&]
[s3;%- &]
[s4;%- &]
[s5;:`:`:`:`:FindLowerBound`(const C`&`,int`,int`,const T`&`,const L`&`):%- [@(0.0.255) t
emplate]_<[@(0.0.255) class]_[*@4 C], [@(0.0.255) class]_[*@4 T], [@(0.0.255) class]_[*@4 L]>
_[@(0.0.255) int]_[* FindLowerBound]([@(0.0.255) const]_[*@4 C]`&_[*@3 v], 
[@(0.0.255) int]_[*@3 pos], [@(0.0.255) int]_[*@3 count], [@(0.0.255) const]_[*@4 T]`&_[*@3 val
], [@(0.0.255) const]_[*@4 L]`&_[*@3 less])&]
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
[s3;%- &]
[s4;%- &]
[s5;:`:`:`:`:FindLowerBound`(const C`&`,const T`&`,const L`&`):%- [@(0.0.255) template]_
<[@(0.0.255) class]_[*@4 C], [@(0.0.255) class]_[*@4 T], [@(0.0.255) class]_[*@4 L]>_[@(0.0.255) i
nt]_[* FindLowerBound]([@(0.0.255) const]_[*@4 C]`&_[*@3 v], [@(0.0.255) const]_[*@4 T]`&_[*@3 v
al], [@(0.0.255) const]_[*@4 L]`&_[*@3 less])&]
[s2; Finds first position in sorted by [* less] predicate where [* val] 
can be inserted without breaking the ordering.&]
[s7; [*C@4 C]-|Type of container.&]
[s7; [*C@4 T]-|Type of value.&]
[s7; [*C@3 v]-|Container.&]
[s7; [*C@3 val]-|Value to find.&]
[s7; [*C@3 less]-|Ordering predicate.&]
[s7; [*/ Return value]-|Position in container.&]
[s3;%- &]
[s4;%- &]
[s5;:`:`:`:`:FindLowerBound`(const C`&`,const T`&`):%- [@(0.0.255) template]_<[@(0.0.255) c
lass]_[*@4 C], [@(0.0.255) class]_[*@4 T]>_[@(0.0.255) int]_[* FindLowerBound]([@(0.0.255) co
nst]_[*@4 C]`&_[*@3 v], [@(0.0.255) const]_[*@4 T]`&_[*@3 val])&]
[s2; Finds first position in sorted by [*/ operator<] predicate where 
[* val] can be inserted without breaking the ordering.&]
[s7; [*C@4 C]-|Type of container.&]
[s7; [*C@4 T]-|Type of value.&]
[s7; [*C@3 v]-|Container.&]
[s7; [*C@3 val]-|Value to find.&]
[s7; [*/ Return value]-|Position in container.&]
[s3;%- &]
[s4;%- &]
[s5;:`:`:`:`:FindUpperBound`(const C`&`,int`,int`,const T`&`,const L`&`):%- [@(0.0.255) t
emplate]_<[@(0.0.255) class]_[*@4 C], [@(0.0.255) class]_[*@4 T], [@(0.0.255) class]_[*@4 L]>
_[@(0.0.255) int]_[* FindUpperBound]([@(0.0.255) const]_[*@4 C]`&_[*@3 v], 
[@(0.0.255) int]_[*@3 pos], [@(0.0.255) int]_[*@3 count], [@(0.0.255) const]_[*@4 T]`&_[*@3 val
], [@(0.0.255) const]_[*@4 L]`&_[*@3 less])&]
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
[s3;%- &]
[s4;%- &]
[s5;:`:`:`:`:FindUpperBound`(const C`&`,const T`&`,const L`&`):%- [@(0.0.255) template]_
<[@(0.0.255) class]_[*@4 C], [@(0.0.255) class]_[*@4 T], [@(0.0.255) class]_[*@4 L]>_[@(0.0.255) i
nt]_[* FindUpperBound]([@(0.0.255) const]_[*@4 C]`&_[*@3 v], [@(0.0.255) const]_[*@4 T]`&_[*@3 v
al], [@(0.0.255) const]_[*@4 L]`&_[*@3 less])&]
[s2; Finds last position in sorted by [* less] predicate where [* val] 
can be inserted without breaking the ordering.&]
[s7; [*C@4 C]-|Type of container.&]
[s7; [*C@4 T]-|Type of value.&]
[s7; [*C@3 v]-|Container.&]
[s7; [*C@3 val]-|Value to find.&]
[s7; [*C@3 less]-|Ordering predicate.&]
[s7; [*/ Return value]-|Position in container.&]
[s3;%- &]
[s4;%- &]
[s5;:`:`:`:`:FindUpperBound`(const C`&`,const T`&`):%- [@(0.0.255) template]_<[@(0.0.255) c
lass]_[*@4 C], [@(0.0.255) class]_[*@4 T]>_[@(0.0.255) int]_[* FindUpperBound]([@(0.0.255) co
nst]_[*@4 C]`&_[*@3 v], [@(0.0.255) const]_[*@4 T]`&_[*@3 val])&]
[s2; Finds last position in sorted by [*/ operator<] predicate where 
[* val] can be inserted without breaking the ordering.&]
[s7; [*C@4 C]-|Type of container.&]
[s7; [*C@4 T]-|Type of value.&]
[s7; [*C@3 v]-|Container.&]
[s7; [*C@3 val]-|Value to find.&]
[s7; [*/ Return value]-|Position in container.&]
[s3;%- &]
[s4;%- &]
[s5;:`:`:`:`:FindBinary`(const C`&`,const T`&`,int`,int`,const L`&`):%- [@(0.0.255) tem
plate]_<[@(0.0.255) class]_[*@4 C], [@(0.0.255) class]_[*@4 T], [@(0.0.255) class]_[*@4 L]>_[@(0.0.255) i
nt]_[* FindBinary]([@(0.0.255) const]_[*@4 C]`&_[*@3 v], [@(0.0.255) const]_[*@4 T]`&_[*@3 val],
 [@(0.0.255) int]_[*@3 pos], [@(0.0.255) int]_[*@3 count], [@(0.0.255) const]_[*@4 L]`&_[*@3 le
ss])&]
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
[s3;%- &]
[s4;%- &]
[s5;:`:`:`:`:FindBinary`(const C`&`,const T`&`,const L`&`):%- [@(0.0.255) template]_<[@(0.0.255) c
lass]_[*@4 C], [@(0.0.255) class]_[*@4 T], [@(0.0.255) class]_[*@4 L]>_[@(0.0.255) int]_[* Find
Binary]([@(0.0.255) const]_[*@4 C]`&_[*@3 v], [@(0.0.255) const]_[*@4 T]`&_[*@3 val], 
[@(0.0.255) const]_[*@4 L]`&_[*@3 less])&]
[s2; Finds position of element with specified value in the container 
sorted by [* less] predicate. If no such element exists, a negative 
value is returned.&]
[s7; [*C@4 C]-|Type of container.&]
[s7; [*C@4 T]-|Type of value.&]
[s7; [*C@3 v]-|Container.&]
[s7; [*C@3 val]-|Value to find.&]
[s7; [*C@3 less]-|Ordering predicate.&]
[s7; [*/ Return value]-|Position in container.&]
[s3;%- &]
[s4;%- &]
[s5;:`:`:`:`:FindBinary`(const C`&`,const T`&`):%- [@(0.0.255) template]_<[@(0.0.255) cla
ss]_[*@4 C], [@(0.0.255) class]_[*@4 T]>_[@(0.0.255) int]_[* FindBinary]([@(0.0.255) const]_[*@4 C
]`&_[*@3 v], [@(0.0.255) const]_[*@4 T]`&_[*@3 val])&]
[s2; Finds position of element with specified value in the container 
sorted by [*/ operator<] predicate. If no such element exists, 
a negative value is returned.&]
[s7; [*C@4 C]-|Type of container.&]
[s7; [*C@4 T]-|Type of value.&]
[s7; [*C@3 v]-|Container.&]
[s7; [*C@3 val]-|Value to find.&]
[s7; [*/ Return value]-|Position in container.&]
[s3;%- &]
[s4;%- &]
[s5;:`:`:`:`:AppendSorted`(C`&`,const C`&`,const L`&`):%- [@(0.0.255) template]_<[@(0.0.255) c
lass]_[*@4 C], [@(0.0.255) class]_[*@4 L]>_[*@4 C]`&_[* AppendSorted]([*@4 C]`&_[*@3 dest], 
[@(0.0.255) const]_[*@4 C]`&_[*@3 src], [@(0.0.255) const]_[*@4 L]`&_[*@3 less])&]
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
[s3;%- &]
[s4;%- &]
[s5;:`:`:`:`:AppendSorted`(C`&`,const C`&`):%- [@(0.0.255) template]_<[@(0.0.255) class]_
[*@4 C]>_[*@4 C]`&_[* AppendSorted]([*@4 C]`&_[*@3 dest], [@(0.0.255) const]_[*@4 C]`&_[*@3 src])
&]
[s2;~~~.992; Merges source NTL container to destination NTL container. 
Both containers must be sorted in ascending order. After the 
operation, destination container is sorted in ascending order 
and contains values from both containers. Duplicate values are 
preserved. Ordering is determined by [*/ operator<].&]
[s7; [*C@4 C]-|Type of container.&]
[s7; [*C@3 dest]-|Destination container.&]
[s7; [*C@3 src]-|Source container.&]
[s7; [*/ Return value]-|Destination container.[C template<class ][*C@4 C][C >]&]
[s3;%- &]
[s4;%- &]
[s5;:`:`:`:`:UnionSorted`(C`&`,const C`&`,const L`&`):%- [@(0.0.255) template]_<[@(0.0.255) c
lass]_[*@4 C], [@(0.0.255) class]_[*@4 L]>_[*@4 C]`&_[* UnionSorted]([*@4 C]`&_[*@3 dest], 
[@(0.0.255) const]_[*@4 C]`&_[*@3 src], [@(0.0.255) const]_[*@4 L]`&_[*@3 less])&]
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
[s3;%- &]
[s4;%- &]
[s5;:`:`:`:`:UnionSorted`(C`&`,const C`&`):%- [@(0.0.255) template]_<[@(0.0.255) class]_[*@4 C
]>_[*@4 C]`&_[* UnionSorted]([*@4 C]`&_[*@3 dest], [@(0.0.255) const]_[*@4 C]`&_[*@3 src])&]
[s2; Merges the source NTL container to the destination NTL container. 
Both containers must be sorted in ascending order and values 
must be unique. After the operation, destination container is 
sorted in ascending order and contains unique values from both 
containers. Ordering is determined by [*/ operator<].&]
[s7; [*C@4 C]-|Type of container.&]
[s7; [*C@3 dest]-|Destination container.&]
[s7; [*C@3 src]-|Source container.&]
[s7; [*/ Return value]-|Destination container.&]
[s3;%- &]
[s4;%- &]
[s5;:`:`:`:`:RemoveSorted`(C`&`,const C`&`,const L`&`):%- [@(0.0.255) template]_<[@(0.0.255) c
lass]_[*@4 C], [@(0.0.255) class]_[*@4 L]>_[*@4 C]`&_[* RemoveSorted]([*@4 C]`&_[*@3 from], 
[@(0.0.255) const]_[*@4 C]`&_[*@3 what], [@(0.0.255) const]_[*@4 L]`&_[*@3 less])&]
[s2; Removes elements of source container from destination container. 
Both containers must be sorted in ascending order and values 
must be unique. Ordering is determined by [* less].&]
[s7; [*C@4 C]-|Type of container.&]
[s7; [*C@3 from]-|Destination container.&]
[s7; [*C@3 what]-|Source container.&]
[s7; [*C@3 less]-|Ordering predicate.&]
[s7; [*/ Return value]-|Destination container.&]
[s3;%- &]
[s4;%- &]
[s5;:`:`:`:`:RemoveSorted`(C`&`,const C`&`):%- [@(0.0.255) template]_<[@(0.0.255) class]_
[*@4 C]>_[*@4 C]`&_[* RemoveSorted]([*@4 C]`&_[*@3 from], [@(0.0.255) const]_[*@4 C]`&_[*@3 what])
&]
[s2; Removes elements of source container from destination container. 
Both containers must be sorted in ascending order and values 
must be unique. Ordering is determined by [*/ operator<].&]
[s7; [*C@4 C]-|Type of container.&]
[s7; [*C@3 from]-|Destination container.&]
[s7; [*C@3 what]-|Source container.&]
[s7; [*/ Return value]-|Destination container.&]
[s3;%- &]
[s4;%- &]
[s5;:`:`:`:`:IntersectSorted`(D`&`,const S`&`,const L`&`):%- [@(0.0.255) template]_<[@(0.0.255) c
lass]_[*@4 D], [@(0.0.255) class]_[*@4 S], [@(0.0.255) class]_[*@4 L]>_[*@4 D]`&_[* IntersectSo
rted]([*@4 D]`&_[*@3 dest], [@(0.0.255) const]_[*@4 S]`&_[*@3 src], [@(0.0.255) const]_[*@4 L]`&
_[*@3 less])&]
[s2; Removes elements from destination container that are not contained 
in source container. Both containers must be sorted in ascending 
order and values must be unique. Ordering is determined by [* less].&]
[s7; [*C@4 D]-|Type of destination container.&]
[s7; [*C@4 S]-|Type of source container.&]
[s7; [*C@3 dest]-|Destination container.&]
[s7; [*C@3 src]-|Source container.&]
[s7; [*C@3 less]-|Destination container.&]
[s3;%- &]
[s4;%- &]
[s5;:`:`:`:`:IntersectSorted`(D`&`,const S`&`):%- [@(0.0.255) template]_<[@(0.0.255) clas
s]_[*@4 D], [@(0.0.255) class]_[*@4 S]>_[*@4 D]`&_[* IntersectSorted]([*@4 D]`&_[*@3 dest], 
[@(0.0.255) const]_[*@4 S]`&_[*@3 src])&]
[s2; Removes elements from destination container that are not contained 
in source container. Both containers must be sorted in ascending 
order and values must be unique. Ordering is determined by [*/ operator<].&]
[s7; [*C@4 D]-|Type of destination container.&]
[s7; [*C@4 S]-|Type of source container.&]
[s7; [*C@3 dest]-|Destination container.&]
[s7; [*C@3 src]-|Source container.&]
[s7; [*/ Return value]-|Destination container.&]
[s3;%- &]
[s4;%- &]
[s5;:`:`:`:`:Sort`(T`&`,const Less`&`):%- [@(0.0.255) template]_<[@(0.0.255) class]_[*@4 T],
 [@(0.0.255) class]_[*@4 Less]>_[@(0.0.255) void]_[* Sort]([*@4 T]`&_[*@3 c], 
[@(0.0.255) const]_[*@4 Less]`&_[*@3 less])&]
[s2; Sorts container. Ordering is determined by [* less].&]
[s6; IterSwap must be defined for T`::Iterator.&]
[s7; [*C@4 T]-|Type of container.&]
[s7; [*C@3 c]-|Container.&]
[s7; [*C@3 less]-|Ordering predicate.&]
[s3;%- &]
[s4;%- &]
[s5;:`:`:`:`:Sort`(T`&`):%- [@(0.0.255) template]_<[@(0.0.255) class]_[*@4 T]>_[@(0.0.255) vo
id]_[* Sort]([*@4 T]`&_[*@3 c])&]
[s2; Sorts container. Ordering is determined by [*/ operator<].&]
[s2; [*1 IterSwap must be defined for T`::Iterator.]&]
[s7; [*C@4 T]-|Type of container.&]
[s7; [*C@3 c]-|Container.&]
[s3;%- &]
[s4;%- &]
[s5;:`:`:`:`:IndexSort`(KC`&`,VC`&`,const Less`&`):%- [@(0.0.255) template]_<[@(0.0.255) c
lass]_[*@4 KC], [@(0.0.255) class]_[*@4 VC], [@(0.0.255) class]_[*@4 Less]>_[@(0.0.255) void]_
[* IndexSort]([*@4 KC]`&_[*@3 keys], [*@4 VC]`&_[*@3 values], [@(0.0.255) const]_[*@4 Less]`&_[*@3 l
ess])&]
[s2; Sorts pair of containers. Both containers must have same number 
of items. Resulting order is determined by the [* keys] container. 
Ordering is determined by [* less].&]
[s7; [*C@4 KC]-|Type of keys container.&]
[s7; [*C@4 VC]-|Type of values.&]
[s7; [*C@3 keys]-|Container of keys.&]
[s7; [*C@3 values]-|Container of values.&]
[s7; [*C@3 less]-|Ordering predicate.&]
[s3;%- &]
[s4;%- &]
[s5;:`:`:`:`:IndexSort`(KC`&`,VC`&`):%- [@(0.0.255) template]_<[@(0.0.255) class]_[*@4 KC],
 [@(0.0.255) class]_[*@4 VC]>_[@(0.0.255) void]_[* IndexSort]([*@4 KC]`&_[*@3 keys], 
[*@4 VC]`&_[*@3 values])&]
[s2; Sorts pair of containers. Both containers must have same number 
of items. Resulting order is determined by the [* keys] container. 
Ordering is determined by [*/ operator<].&]
[s7; [*C@4 KC]-|Type of keys container.&]
[s7; [*C@4 VC]-|Type of values container.&]
[s7; [*C@3 keys]-|Container of keys.&]
[s7; [*C@3 values]-|Container of values.&]
[s3;%- &]
[s4;%- &]
[s5;:`:`:`:`:GetSortOrder`(const C`&`,const Less`&`):%- [@(0.0.255) template]_<[@(0.0.255) c
lass]_[*@4 C], [@(0.0.255) class]_[*@4 Less]>_[_^`:`:Vector^ Vector]<[@(0.0.255) int]>_[* Get
SortOrder]([@(0.0.255) const]_[*@4 C]`&_[*@3 container], [@(0.0.255) const]_[*@4 Less]`&_[*@3 l
ess])&]
[s2; Creates ascending order of values in container. Ordering is 
determined by [* less].&]
[s7; [*C@4 C]-|Type of container.&]
[s7; [*C@3 container]-|Source container.&]
[s7; [*C@3 less]-|Ordering predicate.&]
[s7; [*/ Return value]-|Vector of positions of source container in sorted 
order.&]
[s3;%- &]
[s4;%- &]
[s5;:`:`:`:`:GetSortOrder`(const C`&`):%- [@(0.0.255) template]_<[@(0.0.255) class]_[*@4 C]>
_[_^`:`:Vector^ Vector]<[@(0.0.255) int]>_[* GetSortOrder]([@(0.0.255) const]_[*@4 C]`&_[*@3 c
ontainer])&]
[s2; Creates ascending order of values in container. Ordering is 
determined by [*/ operator<].&]
[s7; [*C@4 C]-|Type of container.&]
[s7; [*C@3 container]-|Source container.&]
[s7; [*/ Return value]-|Vector of positions of source container in sorted 
order.&]
[s3;%- &]
[s4;%- &]
[s5;:`:`:`:`:FieldRelation`(O`(T`:`:`*`)`,const R`&`):%- [@(0.0.255) template]_<[@(0.0.255) c
lass]_[*@4 O], [@(0.0.255) class]_[*@4 T], [@(0.0.255) class]_[*@4 R]>_[_^`:`:FieldRelationCls^ F
ieldRelationCls]<[*@4 O], [*@4 T], [*@4 R]>_[* FieldRelation]([*@4 O]_(T`::`*[*@3 member]), 
[@(0.0.255) const]_[*@4 R]`&_[*@3 relation])&]
[s2; Creates ordering predicate for [* T] based on the value of member 
variable of[*  T].&]
[s7; [*C@4 T]-|Type of element.&]
[s7; [*C@3 member]-|Member variable of T.&]
[s7; [*C@3 relation]-|Ordering relation for [* member].&]
[s7; [*/ Return value]-|Ordering predicate.&]
[s3;%- &]
[s4;%- &]
[s5;:`:`:`:`:MethodRelation`(O`(T`:`:`*`)`(`)`,const R`&`):%- [@(0.0.255) template]_<[@(0.0.255) c
lass]_[*@4 O], [@(0.0.255) class]_[*@4 T], [@(0.0.255) class]_[*@4 R]>_[_^`:`:MethodRelationCls^ M
ethodRelationCls]<[*@4 O]_(T`::`*)(), [*@4 T], [*@4 R]>_[* MethodRelation]([*@4 O]_(T`::`*[*@3 m
ethod])(), [@(0.0.255) const]_[*@4 R]`&_[*@3 relation])&]
[s2; Creates ordering predicate for [* T] based on the value returned 
by non`-const method of [* T].&]
[s7; [*C@4 T]-|Type of element.&]
[s7; [*C@3 method]-|Method of T.&]
[s7; [*C@3 relation]-|Ordering relation for value returned by method.&]
[s7; [*/ Return value]-|Ordering predicate.&]
[s3;%- &]
[s4;%- &]
[s5;:`:`:`:`:MethodRelation`(O`(T`:`:`*`)`(`)const`,const R`&`):%- [@(0.0.255) template
]_<[@(0.0.255) class]_[*@4 O], [@(0.0.255) class]_[*@4 T], [@(0.0.255) class]_[*@4 R]>_[_^`:`:MethodRelationCls^ M
ethodRelationCls]<[*@4 O]_(T`::`*)()_[@(0.0.255) const], [*@4 T], [*@4 R]>_[* MethodRelation
]([*@4 O]_(T`::`*[*@3 method])()_[@(0.0.255) const], [@(0.0.255) const]_[*@4 R]`&_[*@3 relatio
n])&]
[s2; Creates ordering predicate for [* T] based on the value returned 
by const method of [* T].&]
[s7; [*C@4 T]-|Type of element.&]
[s7; [*C@3 method]-|Method of T.&]
[s7; [*C@3 relation]-|Ordering relation for value returned by method.&]
[s7; [*/ Return value]-|Ordering predicate.&]
[s0; ]